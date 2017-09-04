#include "policed_app.h"
#include "policed_context.h"
#include "policed_object_manager.h"
#include "policed_script.h"
#include <object/SharedDef.h>
#include <shared/storage_loader.h>
#include "log_file_operate.h"
#include "game_police.h"
#include "policed_mongodb.h"
#include "log_file_tencent.h"
#include <comm/b64.h>
#include <comm/crypt.h>
#include <net/http.h>
#include "platform_post.h"
#include "http_handler.h"

//////////////////////////////////////////////////////////////////////////
// for AppConfig
bool PolicedConfig::_Load(config_loader *loader)
{
	lock_ip_max			= config_loader_get_int(loader, "lock_ip_max", 10);			//封禁IP的最大次数
	game_id				= config_loader_get_int(loader, "game_id", 59);				//游戏ID
	char *_value;

	_value = config_loader_get_char(loader, "do_post", "N");		//是否POST数据
	do_post = (_value[0] == 'y' || _value[0] == 'Y');
	free(_value);

	_value = config_loader_get_char(loader, "online_port_url", "");		//在线人数POST地址
	online_port_url = _value;
	free(_value);

	_value = config_loader_get_char(loader, "online_port_key", "");		//在线人数post的KEY
	online_port_key = _value;
	free(_value);

	return DbSvrCoreConfig::_Load(loader);
}

//////////////////////////////////////////////////////////////////////////
// for PolicedApp
PolicedApp::PolicedApp(SvrCoreParams &params, SvrCoreConfig &config): base(params, config),m_log_fild_operate(0)
	,m_game_police(NULL),m_mongodb(NULL),m_obj_mgr(NULL),m_log_mongodb(NULL),m_log_fild_tencent(NULL), m_http(NULL)
	,m_login_mgr(nullptr)
{
	m_netgd_request_map[INTERNAL_OPT_WRITE_LOG]				= &on_write_log;		//写日志
	m_netgd_request_map[INTERNAL_OPT_TENCENT_LOG]			= &on_write_tencent_log;		//写腾讯日志
	m_netgd_request_map[INTERNAL_OPT_UD_OBJECT]				= &on_centd_guid_object_table;
	m_netgd_request_map[INTERNAL_OPT_UD_CONTROL]			= &on_centd_guid_object_table;
	m_netgd_request_map[INTERNAL_OPT_UD_CONTROL_RESULT]		= &on_centd_guid_object_table;

	m_netgd_request_map[INTERNAL_OPT_GET_ONLINE_PLAYER]		= &on_get_online_player;		//中心服通知连接完毕
	m_netgd_request_map[INTERNAL_OPT_GM_COMMMANDS]			= &on_gm_commands;
	m_netgd_request_map[INTERNAL_OPT_PLAYER_LOGIN]			= &on_player_login;				//玩家登陆
	m_netgd_request_map[INTERNAL_OPT_PLAYER_LOGOUT]			= &on_player_logout;			//玩家登出
	m_netgd_request_map[INTERNAL_OPT_HT_FORGE_UP]			= &on_ht_forge_up_info;			//锻造成功统计
	m_netgd_request_map[INTERNAL_OPT_SAVE_BACKUP]			= &on_save_backup;				//保存备份日志
	m_netgd_request_map[INTERNAL_OPT_LOGS_FIRST_RECHARGE]	= &on_save_first_recharge;				//保存备份日志
	m_netgd_request_map[INTERNAL_OPT_LOGIND_MERGE_SERVER]	= &on_logind_merge_server;//登陆服通知合服
	m_netgd_request_map[INTERNAL_OPT_UPDATE_GAG_STATUS]		= &on_update_gag_status;//更新禁言状态
	m_netgd_request_map[INTERNAL_OPT_UPDATE_LOCK_STATUS]	= &on_update_lock_status;//更新封号状态

	m_netgd_request_map[INTERNAL_OPT_ADD_GOLD_LOG]			= &on_add_gold_log;
	m_command_hanlders.insert(std::make_pair("reload_script",	&PolicedApp::on_command_reload_scripts));	
	m_command_hanlders.insert(std::make_pair("reload_template",&PolicedApp::on_command_reload_template));
	m_command_hanlders.insert(std::make_pair("print_object", &PolicedApp::on_print_object));
	m_command_hanlders.insert(std::make_pair("memory_recovery", &PolicedApp::on_command_memory_recovery));
	m_game_police = new GamePolice();
}

PolicedApp::~PolicedApp()
{	
	safe_delete(m_log_fild_operate);	
	safe_delete(m_log_fild_tencent);	
	safe_delete(m_game_police);
	for (vector<packet*>::iterator it = g_app->m_pkt_cache.begin(); it != g_app->m_pkt_cache.end(); ++it)
	{
		safe_delete(*it);
	}
	for(auto it = m_account.begin();it != m_account.end();++it)
	{
		free(it->second);
	}
	m_account.clear();
	for (auto it:m_platform_post)
	{
		safe_delete(it);
	}
	m_platform_post.clear();
}

bool PolicedApp::Open()
{
	fp_send_to_policed = PolicedApp::write_log_fd;    

	//载入屏蔽词
	/*if(GetConfig().load_pingbi && !base::LoadFuckPingbi(GetConfig().storage_path))
		return false;*/

	////如果配置文件里面的pid和sid为空则服务器不允许启动
	//if(GetConfig().platform_id.empty() || GetConfig().server_id.empty())
	//{
	//	tea_perror("config platform_id[%s] or server_id[%s] is empty!",GetConfig().platform_id.c_str(),GetConfig().server_id.c_str());
	//	return false;
	//}
	
	tea_pinfo("\n\n实例化对象管理器");
	m_obj_mgr = new PolicedObjectManager(this);
	m_login_mgr = new PolicedLoginMgr;
	//内存回收的定时器，服务器启动5分钟以后运行，每个小时来一次
	RegTimer(fastdelegate::MakeDelegate(this,&PolicedApp::MemoryRecovery),(uint32)time(NULL) + 300,NULL);

	if(!base::Open())
		return false;
    return true;
}

void PolicedApp::Close()
{	
	scripts_free();
	//关闭数据库
	if(m_mongodb)
		m_mongodb->Close();
	if(m_log_mongodb)
		m_log_mongodb->Close();
	safe_delete(m_mongodb);
	safe_delete(m_obj_mgr);
	safe_delete(m_login_mgr);
	//Context和Object是混合对象,所以释放的时候context_map下面一起清理掉
	context_map_.clear();
}

void PolicedApp::Update(uint32 diff)
{
	base::Update(diff);
	if(m_status == APP_STATUS_READY_OK)
	{
		m_login_mgr->Update(diff);
		//http服务
		if(m_http ) 
			m_http->Update();
		else
			m_http = new HttpHandler;
	}
	else if(m_status == APP_STATUS_CONN_CENTD)
	{
		ObjMgr.CallAddWatch(GLOBAL_SERVER_CONNN_LIST_GUID);
		SetStatus(APP_STATUS_WAIT_LOGIND_READY_OK);
	}
	else if(m_status == APP_STATUS_WAIT_LOGIND_READY_OK)
	{
		if(m_server_conn_list && m_server_conn_list->GetServerReadyCount(SERVER_TYPE_LOGIND))
		{
			SetStatus(APP_STATUS_LOGIND_READY_OK);
		}
	}
	else if(m_status == APP_STATUS_LOGIND_READY_OK)
	{
		ObjMgr.CallWatchObjects(REGEX_GLOBAL,[this](){
			WorldPacket pkt = WorldPacket(INTERNAL_OPT_GET_ONLINE_PLAYER);
			SendToCentd(pkt);
			AddWatchGlobalValueOk();
		});
		SetStatus(APP_STATUS_GET_ONLINE_DATA);
	}
	if(m_log_fild_operate)
		m_log_fild_operate->Update(diff);
	if(m_log_fild_tencent)
		m_log_fild_tencent->Update(diff);
	ObjMgr.Update();
	m_post.poll();
}

void PolicedApp::AddWatchGlobalValueOk()
{
	tea_pinfo("\n\n开始载入素材...");

	//载入脚本
	if(scripts_init(GetConfig().script_folder.c_str()) != 0)
	{
		ASSERT(0);
		return /*false*/;
	}

	//素材,由于依赖lua，所有也只能搬这里了
	LoadTemplate();

	GetConfig().server_id = GetServerID();
	tea_pinfo("sid:%s", g_Config.server_id.c_str());	

	tea_pinfo("\n\n开始数据库连接...");
	GetConfig().db_character = GetDbConnStr();
	PolicedApp::g_app->m_mongodb = new PolicedMongoDB(PolicedApp::g_app->g_app->get_logger());
	bool db_conn = PolicedApp::g_app->m_mongodb->Open(GetConfig().db_character,GetConfig().log_level,GetConfig().db_chars_log);
	ASSERT(db_conn);
	//操作日志
	GetConfig().logdb_character = GetLogDbConnStr();
	PolicedApp::g_app->m_log_mongodb=new PolicedMongoLog(PolicedApp::g_app->g_app->get_logger());
	db_conn = PolicedApp::g_app->m_log_mongodb->Open(GetConfig().logdb_character,GetConfig().log_level,GetConfig().logdb_log);
	ASSERT(db_conn);
	
	if(!IsPKServer())
	{
		//更新玩家在线
		g_LOG.UpdateAllHTBasicOnline(false);
		//保存后台信息
		uint32 ntime = (uint32)time(NULL) + 300;
		RegTimer(fastdelegate::MakeDelegate(PolicedApp::g_app->m_game_police,&GamePolice::SaveHTInfo),ntime,NULL);
		//保存排行榜
		RegTimer(fastdelegate::MakeDelegate(PolicedApp::g_app->m_game_police,&GamePolice::SaveTopInfo),ntime,NULL);		
		//获取在线人数
		RegTimer(fastdelegate::MakeDelegate(PolicedApp::g_app,&PolicedApp::GetOnlinePlayerNum),(uint32)time(NULL),NULL);
		//在线人数post
		RegTimer(fastdelegate::MakeDelegate(PolicedApp::g_app,&PolicedApp::PortOnline),(uint32)time(NULL) + irand(1,300),NULL);
	}

	ASSERT(!PolicedApp::g_app->m_log_fild_operate);
	PolicedApp::g_app->m_log_fild_operate = new LogFileOperate();
	for (vector<packet*>::iterator it = PolicedApp::g_app->m_pkt_cache.begin(); it != PolicedApp::g_app->m_pkt_cache.end(); ++it)
	{
		PolicedApp::g_app->m_log_fild_operate->WriteLog(*it);
		internal_protocol_free_packet(*it);
	}
	PolicedApp::g_app->m_pkt_cache.clear();

	account_info *ainfo = (account_info *)malloc(sizeof(account_info));
	strcpy(ainfo->id, "");
	strcpy(ainfo->sid, PolicedApp::g_app->GetServerID().c_str());
	strcpy(ainfo->uid, "");
	PolicedApp::g_app->m_account[""] = ainfo;
	ObjMgr.m_null_player->a_info = ainfo;

	ASSERT(!PolicedApp::g_app->m_log_fild_tencent);
	if(PolicedApp::g_app->GetPlatformID() == PLATFORM_QQ)
		PolicedApp::g_app->m_log_fild_tencent = new LogFileTencent();
	InitPlatfromPost(); 
}

bool PolicedApp::write_log_fd(WorldPacket &pkt)
{
	on_write_log(NULL, pkt.GetPktPtr());
	return true;
}

void PolicedApp::MemoryRecovery(TimerHolder& th)
{
	tea_pinfo("PolicedApp::MemoryRecovery");
	stringstream ss;
	ss << "memory_recovery";
	WorldPacket pack(INTERNAL_OPT_COMMMAND);
	pack << int(0) << int(0) << ss.str();
	PolicedApp::g_app->SendToCentd(pack);
	th._next_time += 60 * 60;// * 24;//一天一次
}

//定时统计在线人数
void PolicedApp::GetOnlinePlayerNum(TimerHolder& th)
{
	uint32 all_player = 0;
	uint32 cash_player = 0;
	ObjMgr.ForEachPlayer([&all_player,&cash_player](PolicedContext *player){
		//没有session的player，是已经下线的玩家
		if(player->GetStatus() == STATUS_LOGGEDIN)
		{
			all_player++;
			if(player->GetRechargeSum() > 0)
				cash_player++;
		}
	});
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_ONLINE, "", "");
	pkt << all_player << cash_player;
	on_write_tencent_log(NULL,pkt.GetPktPtr());
	th._next_time += 300;	//5分钟一次
}

//定时推送在线
void PolicedApp::PortOnline(TimerHolder& th)
{
	uint32 number = 0;
	std::set<string> ip_number;
	ObjMgr.ForEachPlayer([&number,&ip_number](PolicedContext *player){
		//没有session的player，是已经下线的玩家
		if(player->GetStatus() == STATUS_LOGGEDIN)
		{
			number++;
			ip_number.insert(player->GetLastLoginIp());
		}
	});
	//聊天监控
	stringstream key_str;
	string server_name = g_Config.server_id;
	string sid,pid;
	Tokens tokens;
	StrSplit(tokens,server_name,'_');
	if(tokens.size() != 2)
		return;
	sid = tokens[1];
	pid = tokens[0];
	uint32 now_time = (uint32)time(NULL);
	key_str<<sid<<pid<<number<<ip_number.size()<<now_time<<g_Config.online_port_key;

	static char tmp[10000];
	const char * md5_str = MD5CryptToStr32((int8*)key_str.str().c_str(),key_str.str().size(),tmp);

	stringstream ss;
	ss	<< "sid=" << sid
		<< "&pid=" << pid
		<< "&number=" << number
		<< "&ip_number=" << ip_number.size()
		<< "&time=" << now_time
		<< "&sign=" << md5_str;


	string url = g_Config.online_port_url;
	PortData(url,ss.str());

	th._next_time += 300;	//5分钟一次
}

void PolicedApp::PortData(string url,const string& data)
{
#if PLATFORM == PLATFORM_WINDOWS
	if(url.empty())
		return;
	tea_pinfo("post data %s to %s", data.c_str(), url.c_str());
#endif
	if(url.empty())
	{
		tea_perror("post data %s error, url is null.", data.c_str());
		return;
	}	
	char *response = NULL;
	int result = http_post(url.c_str(), data.c_str(), &response);
	if(result != 0)
	{
		tea_perror("post 数据 %s 到 %s 出错,错误代码 %d。", data.c_str(), url.c_str(), result);
	}	
	if (response)
	{
		tea_perror(response);
		free(response);	
	}
#if 0
	PolicedApp::g_app->m_post.async_post(url,data,[url, data](int status,const std::string& response){
		if (status != 200)	
			tea_perror("post data %s to %s fail,err code %d。response:%s", data.c_str(), url.c_str(), status,response.c_str());		
		else		
			tea_pdebug("post result : %s,%s,%s",data.c_str(), url.c_str(), response.c_str() );	
	});
#endif	
}

//////////////////////////////////////////////////////////////////////////
//网络&协议
int PolicedApp::on_player_login(tcp_connection *,server_packet *pkt)
{
	uint32 fd = 0;
	string lguid;

	*pkt >> fd;
	*pkt >> lguid;
	tea_pdebug("PolicedApp::on_player_login %s %u", lguid.c_str(), fd);
	
	g_app->m_login_mgr->PushAction(lguid, fd, CONTEXT_LOGIN_ACTION_LOGIN);
	return 0;
}

int PolicedApp::on_player_logout(tcp_connection *,server_packet *pkt)
{
	uint32 fd = 0;
	string lguid;

	*pkt >> fd;
	*pkt >> lguid;
	tea_pdebug("PolicedApp::on_player_logout %s %u", lguid.c_str(), fd);
	g_app->m_login_mgr->PushAction(lguid, fd, CONTEXT_LOGIN_ACTION_CLOSE);

	return 0;
}

//对象更新
int PolicedApp::on_centd_guid_object_table(tcp_connection *,server_packet *pkt)
{ 
	static ByteArray bytes ;
	bytes.clear();
	bytes.writeBytes(pkt->content,pkt->wpos);
	bytes.position(0);
	ObjMgr.HandlePacket(0,pkt->head->optcode,bytes);
	return 0;
}

int PolicedApp::on_get_online_player(tcp_connection *,server_packet *pkt)
{
	uint16 size;
	*pkt >> size;
	for (uint16 i = 0; i < size; i++)
	{
		int fd = 0;
		string lguid;
		string remote_ip;
		uint16 port = 0;
		uint8 type;
		*pkt >> fd;
		*pkt >> lguid;
		*pkt >> remote_ip;
		*pkt >> port;
		*pkt >> type;
		if(!lguid.empty() && type == CONTEXT_TYPE_YEYOU)
		{
			tea_pdebug("PolicedApp::on_get_online_player %s %u", lguid.c_str(), fd);
			g_app->m_login_mgr->PushAction(lguid, fd, CONTEXT_LOGIN_ACTION_LOGIN);
		}
	}

	ServerList.SetServerReadyOKFlag(g_app->m_connection_id);
	g_app->SetStatus(APP_STATUS_READY_OK);
	return 0;
}

int PolicedApp::on_write_log(tcp_connection *, server_packet *pkt)
{
	if(g_app->m_log_fild_operate)
	{
		g_app->m_log_fild_operate->WriteLog(pkt);
	}
	else
	{
		packet *_pkt = internal_protocol_new_packet(INTERNAL_OPT_WRITE_LOG);
		packet_copy_from(_pkt, pkt);
		g_app->m_pkt_cache.push_back(_pkt);
		if(g_app->m_pkt_cache.size() >= 10000)
		{
			tea_perror("日志服缓存的日志包超过一万条，应该是有异常了，强制退出。");
			g_app->TryClose();
		}
	}
	return 0;
}

//其他服务器发来写腾讯日志请求	
int PolicedApp::on_write_tencent_log(tcp_connection *, server_packet *pkt)				
{
	if(g_app->m_log_fild_tencent)
	{
		g_app->m_log_fild_tencent->WriteLog(pkt);
	}
	return 0;
}
int PolicedApp::on_command_reload_scripts(tcp_connection *conn, server_packet *,uint32 guid, uint32 connectid, Tokens& tokens)
{
	//重新载入脚本
	scripts_reload();
	LoadTemplate();
	return 0;
}

int PolicedApp::on_command_reload_template(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens)
{
	LoadTemplate();
	return 0;
}

account_info *PolicedApp::LoadAccountInfo(const char * account, const char *uid)
{
	if(strlen(uid) == 0)
	{
		map<string, account_info*>::iterator it_account = m_account.find(account);
		if(it_account == m_account.end())
		{
			account_info *info = g_DAL.LoadAccountByUserName(account);
			if(info)
				m_account[account] = info;
			return info;
		}
		else
		{
			return it_account->second;
		}
	}
	else
	{
		for (map<string, account_info*>::iterator it_account = m_account.begin(); it_account != m_account.end(); ++it_account)
		{
			if(strcmp(uid, it_account->second->uid) == 0)
			{
				return it_account->second;
			}
		}
		account_info *info = g_DAL.LoadAccountByUid(uid);
		if(info)
			m_account[info->id] = info;
		return info;		
	}
}

bool PolicedApp::LoadInfo(const char * account, const char * player_id, account_info **a_info, PolicedContext **player, const char *uid)
{
	*a_info = LoadAccountInfo(account, uid);
	if(*a_info == NULL)
	{
		tea_perror("读取不到帐号:%s的信息", account);
		return false;
	}

	*player = ObjMgr.FindPlayer(player_id);

	if(*player == nullptr)
	{
		return false;
	}

	//if(strcmp(account, "") == 0 && strcmp(player_id, "") != 0)
	//	*a_info = (*player)->a_info;
	if(strlen(player_id))
		(*player)->a_info = *a_info;
	return true;
}


void PolicedApp::InitPlatfromPost()
{
	for (auto it:m_platform_post)
	{
		safe_delete(it);
	}
	m_platform_post.clear();
	vector<string> all_post;
	GetPlatfromPostType(all_post);
	for (auto it:all_post)
	{
		m_platform_post.push_back(PlatformPost::Factory(it));
	}
	GetPlatFormGameCode();
}

int PolicedApp::on_gm_commands(tcp_connection *,server_packet *pkt)
{
	string player_id, command;
	*pkt >> player_id >> command;
	tea_pinfo("player %s send gm command : %s", player_id.c_str(), command.c_str());

	g_LOG.WriteGmCommand(player_id, command);

	Tokens tokens;
	StrSplit(tokens, command, ' ');
	if(tokens.empty() || tokens[0].empty())
		return 0;

	int gm_comand_key = GetGmCommandKey(tokens[0]);
	return 0;
}

int PolicedApp::on_ht_forge_up_info(tcp_connection *,server_packet *pkt)
{
	string player_id;
	uint32 forge_type, forge_level;
	*pkt >> player_id >> forge_type >> forge_level;
	g_LOG.SaveHTForgeInfo(player_id,forge_type,forge_level);
	return 0;
}
//保存备份日志
int PolicedApp::on_save_backup(tcp_connection *,server_packet *pkt)	
{
	string guid,save_time;
	uint32 level;
	*pkt >> guid >> save_time >> level;
	g_LOG.SaveBackupInfo(guid,save_time,level);
	return 0;
}

//首充日志
int PolicedApp::on_save_first_recharge(tcp_connection *,server_packet *pkt)	
{
	map<string,string> values;
	*pkt >> values;
	g_LOG.SaveFirstRechargeInfo(values);
	return 0;
}

//登陆服通知合服了
int PolicedApp::on_logind_merge_server(tcp_connection* coon, server_packet* pkt)
{

	string server_name;
	*pkt >> server_name;
	//如果发过来是本服的，那说明自己是要合服,把直接这边的状态设置一下，让别人可以load到
	if(g_app->GetServerID() == server_name)
	{
		//g_DAL.UpdateGmLockStatus(LOCKING_STATUS_START);
		//g_DAL.UpdateGmLevelStatus(GM_LEVEL_TABLE_START);		
	}
	return 0;
}

//元宝日志保存
int PolicedApp::on_add_gold_log(tcp_connection *, server_packet *pkt)
{
	string account, player_id, trace_id, player_name;
	uint16 money_type, oper_type, item_bind, item_del;
	double v, old_value,  new_value, unit_price;
	uint32 item_id, count, quest, level;
	*pkt >> account >> player_id >> player_name >> money_type >> oper_type >> v >> old_value >> new_value >> trace_id >> item_id >> count
		>> unit_price >> item_bind >> item_del >> level >> quest;
	g_LOG.SaveGoldLog(account, player_id,player_name, money_type, oper_type, v, old_value, new_value, trace_id, item_id, count,
		unit_price, item_bind, item_del, level, quest);
	return 0;
}

//更新禁言状态
int PolicedApp::on_update_gag_status(tcp_connection* coon, server_packet* pkt)
{
	uint32 end_time;
	string guid;
	*pkt >> guid >> end_time;
	g_LOG.UpdatePlayerGagStatus(guid, end_time);	

	return 0;
}

//更新禁言状态
int PolicedApp::on_update_lock_status(tcp_connection* coon, server_packet* pkt)
{
	uint32 end_time;
	string account;
	*pkt >> account >> end_time;
	g_LOG.UpdatePlayerLockStatus(account, end_time);	

	return 0;
}
