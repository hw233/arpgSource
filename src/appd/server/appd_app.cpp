#include "appd_app.h"
#include "appd_context.h"
#include "RankListManager.h"
#include "appd_script.h"
#include "appd_object_manager.h"
#include <shared/storage_strus.h>
#include <shared/storage_loader.h>
#include <shared/log_handler.h>
#include <comm/crypt.h>
#include "appd_mongodb.h"
#include <comm/crypt.h>
#include <server/Storage.h>

bool AppConfig::_Load(config_loader *loader)
{
	DbSvrCoreConfig::_Load(loader);

	recharge_folder = player_data_hdd_path + "/recharge";
	mail_folder = player_data_hdd_path + "/offlinemail";
	return true;
}

//////////////////////////////////////////////////////////////////////////
// for AppdApp
AppdApp::AppdApp(SvrCoreParams &params, SvrCoreConfig &config): base(params, config)
	,m_rank_list_mgr(nullptr),m_equip_attrs(nullptr),m_obj_mgr(nullptr)
	,m_db_access_mgr(nullptr)
	,m_login_mgr(nullptr)
{
	m_netgd_request_map[INTERNAL_OPT_UD_OBJECT] = &on_centd_guid_object_table;
	m_netgd_request_map[INTERNAL_OPT_UD_CONTROL] = &on_centd_guid_object_table;
	m_netgd_request_map[INTERNAL_OPT_UD_CONTROL_RESULT] = &on_centd_guid_object_table;

	m_netgd_request_map[INTERNAL_OPT_PLAYER_LOGIN] = &on_player_login;				//玩家登陆
	m_netgd_request_map[INTERNAL_OPT_PLAYER_LOGOUT] = &on_player_logout;			//玩家登出

	m_netgd_request_map[INTERNAL_OPT_OFFLINE_OPER] = &on_offline_opert;
	m_netgd_request_map[INTERNAL_OPT_RECALCULATION_ATTR] = &on_scened_recalculation_attr;//场景服通知应用服重算属性
	m_netgd_request_map[INTERNAL_OPT_GET_ONLINE_PLAYER] = &on_get_online_player;		//中心服通知连接完毕
	m_netgd_request_map[INTERNAL_OPT_OPEN_TIME] = &on_centd_open_time;
	m_netgd_request_map[INTERNAL_OPT_DEL_CHAR] = &on_del_char;
	m_netgd_request_map[INTERNAL_OPT_APPD_ADD_MONEY] = &on_scened_player_addmoney;	//场景服通知应用服增加money
	m_netgd_request_map[INTERNAL_OPT_APPD_SUB_MONEY] = &on_scened_player_submoney;	//场景服通知应用服扣money	
	m_netgd_request_map[INTERNAL_OPT_SYNC_GUID_MAX] = &on_logind_sync_maxguid;		//同步guid管理器最大累加值
	m_netgd_request_map[INTERNAL_OPT_LOGIND_MERGE_SERVER] = &on_logind_merge_server;//登陆服通知合服
	//////////////////////////////////////////////////////////////////////////
	//后台命令
	m_command_hanlders.insert(std::make_pair("reload_script", &AppdApp::on_command_reload_scripts));
	m_command_hanlders.insert(std::make_pair("reload_template",&AppdApp::on_command_reload_template));
	m_command_hanlders.insert(std::make_pair("add_item",&AppdApp::on_command_add_item));
	m_command_hanlders.insert(std::make_pair("top_start",&AppdApp::on_command_top_start));
	m_command_hanlders.insert(std::make_pair("print_object", &AppdApp::on_print_object));
	m_command_hanlders.insert(std::make_pair("memory_recovery", &AppdApp::on_command_memory_recovery));
}

AppdApp::~AppdApp()
{
	for(auto it = m_sys_notice.begin();it != m_sys_notice.end();++it)
	{
		free(it->second);
	}
	m_sys_notice.clear();
}

bool AppdApp::Open()
{
	fp_send_to_policed = std::bind(&AppdApp::SendToPoliced,this,std::placeholders::_1);
    tea_pinfo("\n\n开始载入素材...");

	//载入屏蔽词
	if(GetConfig().load_pingbi && !base::LoadFuckPingbi(GetConfig().storage_path))
		return false;	

	//排行榜
	m_rank_list_mgr = new RankListManager;
	//初始化玩家属性重算的BINLOG
	m_equip_attrs = new BinLogObject;
	m_equip_attrs->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
	//初始化对象管理器
	m_obj_mgr = new AppdObjectManager(this);
	//初始化登录管理器
	m_login_mgr = new AppdLoginMgr;
	if(!base::Open())
		return false;
	
    return true;
}

AppdApp::CultivationMap AppdApp::m_cultivationMap;
string AppdApp::XIULIAN_FILE_NAME = "xiulian.yy";

void AppdApp::loadXiulian() {
	core_obj::Storage m_storage(nullptr);
	m_storage.Goto(g_Config.player_data_hdd_path);
	m_storage.ReadFile(AppdApp::XIULIAN_FILE_NAME, [](const string &content) {
		if (content.length() == 0) {
			return false;
		}
		Tokens token;
		StrSplit(token, content, '|');
		string guid = token[ 0 ];
		std::vector<uint32> list;
		for (uint32 i = 1; i < token.size(); ++ i) {
			list.push_back(atol(token[ i ].c_str()));
		}
		AppdApp::m_cultivationMap.insert(std::make_pair(guid, list));
		return true;
	});
}

void AppdApp::saveXiulian() {
	core_obj::Storage m_storage(nullptr);
	stringstream ss;
	for (auto it : AppdApp::m_cultivationMap) {
		std::vector<uint32> list = it.second;
		ss << it.first;
		for (uint32 i = 0; i < list.size(); ++ i) {
			ss << "|" << list[ i ];
		}
		ss << '\n';
	}
	string content = ss.str();
	//开始保存
	m_storage.Goto(g_Config.player_data_hdd_path);
	m_storage.SaveFile(AppdApp::XIULIAN_FILE_NAME, content);
}

void AppdApp::Close()
{
	
	//AppdContext::RemoveAllContext();
	//重载前先做点什么
	DoCloseServerSaveDB();
	scripts_free();

	safe_delete(m_equip_attrs);
	safe_delete(m_rank_list_mgr);
	//应用服的player和session是同一个实例,所以对象管理器释放的时候已经把内存回收了，需要清空会话
	safe_delete(m_obj_mgr);
	context_map_.clear();
	safe_delete(m_login_mgr);
	for(auto it = m_sys_notice.begin();it != m_sys_notice.end();++it)
	{
		free(it->second);
	}
	m_sys_notice.clear();
}

void AppdApp::Update(uint32 diff)
{
	base::Update(diff);
	if(m_status == APP_STATUS_READY_OK)
	{
		m_login_mgr->Update(diff);

		auto& contexts = AppdApp::g_app->context_map_;
		for(auto it = contexts.begin();it != contexts.end();++it)
		{
			if(it->second == NULL)
				continue;
			SvrContext *context = it->second;
			//登录完成才能心跳
			if(STATUS_LOGGEDIN == context->GetStatus())
			{
				AppdContext *appContext = dynamic_cast<AppdContext*>(it->second);
				ASSERT(appContext);
				appContext->Update(diff);
			}
		}
		m_rank_list_mgr->Update(diff);
		//准备好了，脚本可以心跳了
		int result = DoUpdateLua(diff);
		if(result)
		{
			//出错了
			tea_perror("AppdApp::Update DoUpdateLua %d", result);
		}
		if(m_db_access_mgr)
			m_db_access_mgr->UpdateAsync();
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
		// 帮派信息
		ObjMgr.CallAddTagWatch(FACTION_BINLOG_OWNER_STRING);
		// 帮派数据(送礼物)
		ObjMgr.CallAddTagWatch(FACTION_DATA_OWNER_STRING);

		ObjMgr.CallWatchObjects(REGEX_GLOBAL,[&](){
			WorldPacket pkt(INTERNAL_OPT_GET_ONLINE_PLAYER);
			AppdApp::g_app->SendToCentd(pkt);
			AppdApp::g_app->AddWatchGlobalValueOk();
		});

		SetStatus(APP_STATUS_GET_ONLINE_DATA);
	}

	//数据管理器
	m_obj_mgr->Update();
}

SvrObjectManager *AppdApp::GetObjMgr()
{
    return dynamic_cast<SvrObjectManager *>(m_obj_mgr);
}

//////////////////////////////////////////////////////////////////////////
//网络&协议
int AppdApp::on_player_login(tcp_connection *,server_packet *pkt)
{
	uint32 fd = 0;
	string lguid;

	*pkt >> fd;
	*pkt >> lguid;

	tea_pdebug("AppdApp::on_player_login %s %u", lguid.c_str(), fd);

	g_app->m_login_mgr->PushAction(lguid, fd, CONTEXT_LOGIN_ACTION_LOGIN);

	return 0;
}

int AppdApp::on_player_logout(tcp_connection *,server_packet *pkt)
{
	uint32 fd = 0;
	string lguid;

	*pkt >> fd;
	*pkt >> lguid;

	tea_pdebug("AppdApp::on_player_logout %s %u", lguid.c_str(), fd);

	g_app->m_login_mgr->PushAction(lguid, fd, CONTEXT_LOGIN_ACTION_CLOSE);
	return 0;
}

int AppdApp::on_get_online_player(tcp_connection *,server_packet *pkt)
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
			tea_pdebug("AppdApp::on_get_online_player %s %u", lguid.c_str(), fd);
			g_app->m_login_mgr->PushAction(lguid, fd, CONTEXT_LOGIN_ACTION_LOGIN);
		}
	}
	if(!g_app->IsPKServer())
	{
		//更新GM后台
		//更新在线玩家信息
		uint32 t = (uint32)time(NULL) + g_Config.update_online_playerinfo_interval;
		RegTimer(fastdelegate::MakeDelegate(g_app, &AppdApp::UpdateOnLInePlayerInfo), t, NULL);
	}
	ServerList.SetServerReadyOKFlag(g_app->m_connection_id);
	g_app->SetStatus(APP_STATUS_READY_OK);
	return 0;
}

int AppdApp::on_centd_open_time(tcp_connection *, server_packet *pkt)
{
	uint32 open_time;
	if(unpack_internal_open_time(pkt, &open_time,&__process__start_time))		//服务器运行到现在的毫秒数
	{
		tea_perror("on_centd_open_time解包失败!");
		return -1;
	}
	AppdApp::g_app->m_open_time = open_time;
	return 0;
}

int AppdApp::on_scened_recalculation_attr(tcp_connection *, server_packet *pkt)
{
	string guid;
	*pkt>>guid;
	AppdContext *player = AppdContext::FindPlayer(guid);
	if (!player)
		return 0;
	//下发装备信息下去
	player->SetCalculFlag();
	return 0;
}

void AppdApp::AddWatchGlobalValueOk()
{
	//如果不是pk服，该干嘛干嘛
	if (!IsPKServer())
	{
		GetConfig().db_character = GetDbConnStr();
		AppdApp::g_app->m_db_access_mgr = new AppdMongoDB(AppdApp::g_app->g_app->get_logger());
		bool db_conn = AppdApp::g_app->m_db_access_mgr->Open(GetConfig().db_character,GetConfig().log_level,GetConfig().db_chars_log,1);
		ASSERT(db_conn);

		//如果充值目录未创建，则建一个
		if(!core_obj::Storage::IsHaveFile(g_Config.recharge_folder))
			core_obj::Storage::CreatDir(g_Config.recharge_folder);

		//如果离线邮件目录未创建，则建一个
		if(!core_obj::Storage::IsHaveFile(g_Config.mail_folder))
			core_obj::Storage::CreatDir(g_Config.mail_folder);
	}

	//载入脚本
	if(scripts_init(GetConfig().script_folder.c_str()) != 0)
	{
		ASSERT(0);
		return /*false*/;
	}

	//素材,由于依赖lua，所有也只能搬这里了
	LoadTemplate();
	
	if(!IsPKServer())
	{
		uint32 t = (uint32)time(NULL);

		//设置下一小时重置时间点
		uint32 next_time = t - t % 3600 + 3600;

		//在这个位置数据都准备好了，可以调用初始化数据库连接了
		DoOpenMongodb(AppdApp::g_app->GetDbConnStr(),AppdApp::g_app->GetServerID());
		DoStartServerLoadDB();
		////操作日志
		//GetConfig().logdb_character = GetLogDbConnStr();
		//AppdApp::g_app->m_db_log_mgr=new AppdLogsDB(AppdApp::g_app->g_app->get_logger());
		//bool db_conn_log = AppdApp::g_app->m_db_log_mgr->Open(GetConfig().logdb_character,GetConfig().log_level,GetConfig().logdb_log);
		//ASSERT(db_conn_log);
		

		//设置下一小时重置时间点
		next_time = t - t % 86400 + CVT_DIFF_UTC;
		//设置重置时间
		RegTimer(fastdelegate::MakeDelegate(AppdApp::g_app, &AppdApp::ResetDailyCounter), next_time, NULL);

		//初始化排行榜
		m_rank_list_mgr->InitRankInfo();
		m_rank_list_mgr->InitTaskPool();
	}
}

//获取排行榜玩家guid
string AppdApp::GetRankPlayerGuidByType(int type, uint32 rank)
{
	RankListCurrentObj *obj = AppdApp::g_app->m_rank_list_mgr->FindRankDataByType(type,rank);
	string player_guid = "";
	if(obj)
		player_guid = obj->GetCurrentPlayerGuid();
	return player_guid;
}

//登录后映射下帐号ID和角色ID
void AppdApp::login_account_guid(AppdContext *player)
{
	m_account_guid.insert(std::make_pair(player->GetAccount(),player->GetGuid()));
}

//根据account获取guid
string AppdApp::getGuidFromAccount(string account)
{
	if (m_account_guid.find(account) != m_account_guid.end())
	{
		string guid = m_account_guid[account];
		auto player = ObjMgr.FindPlayer(guid);
		if (player && player->GetStatus() == STATUS_LOGGEDIN)
		{
			return guid;
		}
	}
	static string null_res = "";
	return  null_res;
}
