#include "policed_app.h"
#include <net/http.h>

#include <object/SharedDef.h>

#include <shared/storage_strus.h>
#include <shared/storage_loader.h>

#include "log_file_tencent.h"
#include "game_police.h"
#include "platform_post.h"
#include "policed_context.h"
#include <svrcore/server/Storage.h>

const char *tencent_file_name[MAX_LOG_FILE_TENCENT_TYPE] = {
	"t_user",					//用户表
	"t_player",					//角色表
	"t_recharge",				//充值表
	"t_upgradelog",				//升级日志
	"t_newtask",				//新手日志
	"t_login",					//登陆日志
	"t_offline",				//离线日志
	"t_online",					//在线统计日志
	"t_moneylog",				//金币流量
};

string LogFileTencent::GetLastString(string name,const char &spit)
{
	size_t pos = name.rfind(spit);
	if(pos == string::npos)
		return name;
	return name.substr(pos+1,name.length());
}

void LogFileTencent::HandlerInit()
{
	for (uint32 i = 0; i < MAX_LOG_FILE_TENCENT_TYPE; i++)
	{
		m_tx_handle_map[i] = NULL;
	}
	m_tx_handle_map[LOG_FILE_TENCENT_USER]				= &LogFileTencent::Handle_Write_Tencent_User;
	m_tx_handle_map[LOG_FILE_TENCENT_PLAYER]			= &LogFileTencent::Handle_Write_Tencent_Player;
	m_tx_handle_map[LOG_FILE_TENCENT_RECHAGRE]			= &LogFileTencent::Handle_Write_Tencent_Recharge;
	m_tx_handle_map[LOG_FILE_TENCENT_UPGRADELOG]		= &LogFileTencent::Handle_Write_Tencent_UpgradeLog;
	m_tx_handle_map[LOG_FILE_TENCENT_NEWTASK]			= &LogFileTencent::Handle_Write_Tencent_Newtask;
	m_tx_handle_map[LOG_FILE_TENCENT_LOGIN]				= &LogFileTencent::Handle_Write_Tencent_Login;
	m_tx_handle_map[LOG_FILE_TENCENT_OFFLINE]			= &LogFileTencent::Handle_Write_Tencent_Offline;
	m_tx_handle_map[LOG_FILE_TENCENT_ONLINE]			= &LogFileTencent::Handle_Write_Tencent_Online;
	m_tx_handle_map[LOG_FILE_TENCENT_MONEYLOG]			= &LogFileTencent::Handle_Write_Tencent_MoneyLog;
}

LogFileTencent::LogFileTencent():m_today(999),m_host(999)
{
	HandlerInit();
}

LogFileTencent::~LogFileTencent()
{	
	safe_delete(m_file_map);
}

void LogFileTencent::Update(uint32 diff)
{
	struct tm *local;
	time_t t;
	t=time(NULL);
	local = localtime(&t);
	if(local->tm_yday != m_today || local->tm_hour != m_host)
		ResetMap(local);
}

//通过外部服务器接口post经分数据
void LogFileTencent::PostTXJinFen(string ext_url,string account,string user_guid,string pf,double modifyfee,uint32 onlinetime)
{
	stringstream ss;
	ss  << "account=" << account
		<< "&user_guid=" << user_guid
		<< "&pf=" << pf;
	if(modifyfee)
		ss << "&modifyfee=" << modifyfee;
	if(onlinetime)
		ss << "&onlinetime=" << onlinetime;

	string data = ss.str();
	PostTXJinFen(ext_url,data);
}

//通过外部服务器接口post经分数据
void LogFileTencent::PostTXJinFen(string ext_url,string data)
{
	//腾迅接口服务器
	string url=PolicedApp::g_app->GetExtWebInterface() + ":8001"+ext_url;

#if PLATFORM == PLATFORM_WINDOWS
	ASSERT(!url.empty());
	tea_pinfo("post data %s to %s", data.c_str(), url.c_str());
#endif
	if(url.empty())
	{
		tea_perror("post data %s error, url is null.", data.c_str());
		return;
	}	

	PolicedApp::g_app->m_post.async_post(url,data,[url, data](int status,const std::string& response){
		if (status != 200)	
			tea_perror("post data %s to %s fail,err code %d。response:%s", data.c_str(), url.c_str(), status,response.c_str());		
		else		
			tea_pdebug("post result : %s", response.c_str() );	
	});
}

void LogFileTencent::ResetMap(struct tm *local)
{
	char datetime[12];
	sprintf(datetime, "%04d%02d%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);
	string tencent_log = "tencentlog";
	string topath = g_Config.log_folder + "/" + tencent_log + "/";
	core_obj::Storage::CreatDir(topath);
	if(m_file_map.empty())
	{
		ASSERT(MAX_LOG_FILE_TENCENT_TYPE <= sizeof(tencent_file_name)/sizeof(const char*));
		for (uint32 i = 0; i < MAX_LOG_FILE_TENCENT_TYPE; i++)
		{
			//日志文件
			stringstream ss;
			ss << g_Config.log_folder << "/" << tencent_log << "/" << tencent_file_name[i] << "_" << datetime << ".log";
			LogFileStream *file = new LogFileStream(ss.str().c_str(),"|","\t\n\r\x0B");
			m_file_map[i] = file;
			string name = tencent_file_name[i];			
			StringToLower(name);
		}
	}
	else
	{
		for (FildMap::iterator it = m_file_map.begin(); it != m_file_map.end(); ++it)
		{
			ASSERT(it->second != NULL);
			stringstream ss;
			ss << g_Config.log_folder << "/" << tencent_log << "/" << tencent_file_name[it->first] << "_" << datetime << ".log";
			it->second->Open(ss.str().c_str());
		}
	}
	m_today = local->tm_yday;
	m_host = local->tm_hour;
}

void LogFileTencent::ReWriteLog(TimerHolder& th)
{
	log_packet *pkt = (log_packet*)th.param;
	uint32 log_type;
	char *account;
	char *player_id;
	uint32 sec, usec;
	pkt->pkt->rpos = 0;
	*pkt->pkt >> log_type >> sec >> usec >> account >> player_id;
	time_t t = time(NULL);
	t = t - 60;
	account_info *a_info = NULL;
	PolicedContext *player = NULL;
	if(!PolicedApp::g_app->LoadInfo(account, player_id, &a_info, &player))
	{		
		if(RETRY_MAX_COUNT > ++pkt->retry_count)
			th._next_time += 60;
		return;
	}
	else
	{
		WriteLog(pkt->pkt);
	}
	internal_protocol_free_packet(pkt->pkt);
}

void LogFileTencent::WriteBaseLog(uint32 type, LogFileStream& file, time_t sec, time_t usec, account_info *a_info, PolicedContext *player)
{
	char datetime[30];
	struct tm *local;
	local = localtime(&sec);
	sprintf(datetime, "%04d-%02d-%02d %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
	file << tencent_file_name[type] << datetime << (string)a_info->sid << GetLastString((string)a_info->id,'_') << player->guid();
}

void LogFileTencent::WriteLog(packet *pkt, account_info *a_info, PolicedContext *player)
{
	uint32 log_type;
	char *account;
	char *player_id;
	uint32 sec, usec;
	pkt->rpos = 0;
	*pkt >> log_type >> sec >> usec >> account >> player_id;

	if(log_type >= m_file_map.size())
	{
		tea_perror("LogFileOperate::WriteLog log_type %u player:%s , log_type >= m_file_map.size()", log_type, player_id);
		return;
	}

	if(a_info == NULL && !PolicedApp::g_app->LoadInfo(account, player_id, &a_info, &player))
	{
		tea_pwarn("LogFileOperate::WriteLog player_id = %s , account = %s not found, retry", player_id,account);
		sec = sec + 20;
		log_packet *_pkt = (log_packet *)malloc(sizeof(log_packet));
		_pkt->pkt = internal_protocol_new_packet(INTERNAL_OPT_TENCENT_LOG);
		packet_copy_from(_pkt->pkt, pkt);
		_pkt->retry_count = 1;
		RegTimer(fastdelegate::MakeDelegate(this, &LogFileTencent::ReWriteLog), (uint32)sec, _pkt);
		return;
	}

	LogFileStream *file = m_file_map.find(log_type)->second;
	if(log_type == LOG_FILE_TENCENT_ONLINE)	//在线日志另外写
	{
		string server_id = g_Config.server_id;
		size_t pos = server_id.rfind('_');
		if(pos != string::npos)	
			//得到帐号了
			server_id = server_id.substr(pos+1,server_id.length()-pos);
		char datetime[30];

		time_t tmp = sec;
		struct tm *local = localtime(&tmp);

		uint32 all_player,cashcount;
		*pkt >> all_player >> cashcount;
		sprintf(datetime, "%04d-%02d-%02d %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
		*file << tencent_file_name[log_type] << datetime << server_id << all_player << cashcount;

	}
	else
	{
		WriteBaseLog(log_type, *file, sec, usec, a_info, player);

		if(m_tx_handle_map[log_type])
		{
			(this->*m_tx_handle_map[log_type])(log_type, pkt, file, sec, usec, a_info, player);
		}
		else
		{
			tea_perror("TX来了一个不知道是啥日志的包，日志ID：%u", log_type);
		}
	}
		
	(*file) << LOG_SINGLE_END;
	file->Flush();
}

void LogFileTencent::Handle_Write_Tencent_User(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	string player_name,platdata,ip;
	uint32 player_level,create_time;
	*pkt >> player_name >> player_level >> platdata >> create_time >> ip;
	(*file) << GetLastString(player_name,',') << player_level << GetPlatInfo(platdata,"pf") << create_time;

	stringstream ss;
	ss  << "userip=" << ip
		<< "&account=" << a_info->id
		<< "&user_guid=" << player->GetGuid()
		<< "&pf=" << (GetPlatInfo(platdata,"opf").empty()? GetPlatInfo(platdata,"pf"):GetPlatInfo(platdata,"opf"))
		<< "&openkey=" << GetPlatInfo(platdata,"openkey")
		<< "&pfkey=" << GetPlatInfo(platdata,"pfkey")
		<< "&iopenid=" << GetPlatInfo(platdata,"iopenid")
		<< "&iopenuid=" << GetPlatInfo(platdata,"iopenuid");

	string data = ss.str();
	PostTXJinFen("/stat/report_register",data);
}

void LogFileTencent::Handle_Write_Tencent_Player(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	string player_name,platdata;
	uint32 player_level,create_time;
	*pkt >> player_name >> player_level >> platdata >> create_time;
	(*file) << GetLastString(player_name,',') << player_level << GetPlatInfo(platdata,"pf") << create_time;
}

void LogFileTencent::Handle_Write_Tencent_Recharge(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	string player_name,platdata,transactionId,currency,inpourType;
	uint32 player_level,create_time,addcoin,exchangeType,state;
	double money,gold,depay,gDepay;
	*pkt >> player_name >> player_level >> transactionId >> currency >> inpourType >> addcoin >> money >> gold >> exchangeType >> depay >> gDepay >> state >> create_time >> platdata;
	(*file) << GetLastString(player_name,',') << player_level << transactionId << currency << inpourType << addcoin << money << gold << exchangeType << depay << gDepay << state << create_time << GetPlatInfo(platdata,"pf");
	PostTXJinFen("/stat/report_recharge",a_info->id,player->GetGuid(),GetPlatInfo(platdata,"opf").empty()? GetPlatInfo(platdata,"pf"):GetPlatInfo(platdata,"opf"),money);
}

void LogFileTencent::Handle_Write_Tencent_UpgradeLog(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	string player_name,params;
	uint32 old_level,new_level,sucess,upgradeType,create_time;
	*pkt >> player_name >> old_level >> new_level >> sucess >> upgradeType >> params >> create_time;
	(*file) << GetLastString(player_name,',') << old_level << new_level << sucess << upgradeType << params << create_time;
}

void LogFileTencent::Handle_Write_Tencent_Newtask(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	string player_name;
	uint32 taskid,status,quality,create_time;
	*pkt >> player_name >> taskid >> status >> quality >> create_time;
	(*file) << GetLastString(player_name,',') << taskid << status << quality << create_time;
}

void LogFileTencent::Handle_Write_Tencent_Login(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	string player_name,siteUrl,adFrom,platdata,ip;
	uint32 player_level,create_time;
	*pkt >> player_name >> player_level >> siteUrl >> adFrom >> platdata >> ip >> create_time;
	(*file) << GetLastString(player_name,',') << player_level << siteUrl << adFrom << GetPlatInfo(platdata,"pf") << ip << create_time;

	PostTXJinFen("/stat/report_login",a_info->id,player->GetGuid(),GetPlatInfo(platdata,"opf").empty()? GetPlatInfo(platdata,"pf"):GetPlatInfo(platdata,"opf"));
}

void LogFileTencent::Handle_Write_Tencent_Offline(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	string player_name,siteUrl,adFrom,platdata,ip;
	uint32 player_level,create_time,online_time,history_time;
	*pkt >> player_name >> player_level >> siteUrl >> adFrom >>platdata >> ip >> online_time >> history_time >> create_time;
	(*file) << GetLastString(player_name,',') << player_level << siteUrl << adFrom << GetPlatInfo(platdata,"pf") << ip << online_time << history_time << create_time;
	PostTXJinFen("/stat/report_quit",a_info->id,player->GetGuid(),GetPlatInfo(platdata,"opf").empty()? GetPlatInfo(platdata,"pf"):GetPlatInfo(platdata,"opf"),0,online_time);
}

void LogFileTencent::Handle_Write_Tencent_Online(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	uint32 allcount,cashcount;
	*pkt >> allcount >> cashcount;
	(*file) << allcount << cashcount;
}

void LogFileTencent::Handle_Write_Tencent_MoneyLog(uint32 log_type, packet *pkt, LogFileStream *file, time_t, time_t , account_info *a_info, PolicedContext* player) 
{
	string player_name,platdata;
	uint32 create_time,rootid,type_id,gold_ticke,goods_id,num;
	double old_value,new_value,gold;
	*pkt >> player_name >> rootid >> type_id >> old_value >> new_value >> gold >> gold_ticke >> goods_id >> num >> create_time >> platdata;
	(*file) << GetLastString(player_name,',') << rootid << type_id << old_value << new_value << gold << gold_ticke << goods_id << num << create_time;
	if(new_value < old_value)
		PostTXJinFen("/stat/report_consume",a_info->id,player->GetGuid(),GetPlatInfo(platdata,"opf").empty()? GetPlatInfo(platdata,"pf"):GetPlatInfo(platdata,"opf"),num);
}
