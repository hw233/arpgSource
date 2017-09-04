#include "logind_app.h"
#include "logind_context.h"

#include <shared/add_memdb_index.h>

#include <shared/log_handler.h>
#include <object/OprateResult.h>

#include "map_manager.h"
#include "logind_object_manager.h"
#include "logind_script.h"
#include "logind_cache.h"
#include "logind_context_login_mgr.h"
#include <comm/b64.h>
#include <comm/crypt.h>
#include <core_obj/SyncEventRecorder.h>
#include <shared/util.h>
#include "http_handler.h"
#include "logind_gm_table.h"

string account_table::ToString()
{
	static char str[1000];
	sprintf(str, "%s %s %s %s %s %d %d %s", sid, uid, pid, name, ip, is_FCM, gmlevel, platdata);
	return str;
}

void account_table::FromString(const string &str)
{
	Tokens tokens;
	StrSplit(tokens, str, '\n');
	ASSERT(tokens.size() >= 8);
	strncpy(sid, tokens[0].c_str(), 15);
	strncpy(uid, tokens[1].c_str(), 39);
	strncpy(pid, tokens[2].c_str(), 39);
	strncpy(name, tokens[3].c_str(), 59);
	strncpy(ip, tokens[4].c_str(), 29);
	is_FCM = atoi(tokens[5].c_str());
	gmlevel = atoi(tokens[6].c_str());
	strncpy(platdata, tokens[7].c_str(), 199);
}

void account_table::NewFromString(const string &str)
{
	Tokens tokens;
	StrSplit(tokens, str, ' ');
	ASSERT(tokens.size() >= 8);
	strncpy(sid, tokens[0].c_str(), 15);
	strncpy(uid, tokens[1].c_str(), 39);
	strncpy(pid, tokens[2].c_str(), 39);
	strncpy(name, tokens[3].c_str(), 59);
	strncpy(ip, tokens[4].c_str(), 29);
	is_FCM = atoi(tokens[5].c_str());
	gmlevel = atoi(tokens[6].c_str());
	strncpy(platdata, tokens[7].c_str(), 199);
}

bool LogindConfig::_Load(config_loader *loader)
{
	if(!DbSvrCoreConfig::_Load(loader))
		return false;

	//平台加密密钥
	char* _value = config_loader_get_char(loader,"login_key","");
	login_key = _value;
	free(_value);

	//平台ID
	_value = config_loader_get_char(loader, "platform_id","");
	platform_id = _value;
	free(_value);

	//服务器ID
	_value = config_loader_get_char(loader, "server_id", "");
	server_id = _value;
	free(_value);

	server_name = platform_id + "_" + server_id;

	login_queue_interval		= config_loader_get_int(loader, "login_queue_interval", 3000);		//轮询等待玩家是否可以进入游戏的时间间隔
	versions_requirements_internal = config_loader_get_int(loader, "versions_requirements_internal", 20);//默认20秒一次

	//是否校验时间
	_value = config_loader_get_char(loader, "check_session_key_time", "Y");
	check_session_key_time = (_value[0] == 'y' || _value[0] == 'Y');
	free(_value);

	_value = config_loader_get_char(loader, "gm_open", "N");
	gm_open = (_value[0] == 'y' || _value[0] == 'Y');
	free(_value);

	server_type = config_loader_get_int(loader, "server_type", 0);		//服务器类型

	scened_collapse_time_out = config_loader_get_int(loader, "scened_collapse_time_out", 8);

	//
	_value = config_loader_get_char(loader, "test_server_live_uid","testserverlive");
	test_server_live_uid = _value;
	free(_value);
	
	_value = config_loader_get_char(loader,"ext_web_interface", "");
	ext_web_interface = _value;
	free(_value);

	_value = config_loader_get_char(loader,"conf_svr", "");
	conf_svr = _value;
	free(_value);

	_value = config_loader_get_char(loader,"world_server_url", "");
	world_server_url = _value;
	free(_value);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// for LogindApp
LogindApp::LogindApp(SvrCoreParams &params, LogindConfig &config)
	:base(params,config),m_db_access_mgr(NULL),m_cache(NULL),m_obj_mgr(NULL)
	,m_login_queue(NULL),m_http(NULL),m_gm_table(NULL)
{	
	//////////////////////////////////////////////////////////////////////////
	//以下为所有的内部包
	m_netgd_request_map[INTERNAL_OPT_CREATE_CONN] = &on_netgd_create_conn;
	m_netgd_request_map[INTERNAL_OPT_DESTORY_CONN] = &on_netgd_destory_conn;
	m_netgd_request_map[INTERNAL_OPT_GET_SESSION] = &on_create_conn_get_session;

	m_netgd_request_map[INTERNAL_OPT_UD_OBJECT] = &on_centd_guid_object_table;
	m_netgd_request_map[INTERNAL_OPT_UD_CONTROL] = &on_centd_guid_object_table;
	m_netgd_request_map[INTERNAL_OPT_UD_CONTROL_RESULT] = &on_centd_guid_object_table;

	m_netgd_request_map[INTERNAL_OPT_DEL_MAP_INSTANCE] = &on_scened_del_map;								//中心服发来删除地图
	m_netgd_request_map[INTERNAL_OPT_GET_ONLINE_PLAYER] = &on_get_online_player;				//获取在线玩家数据完成

	m_netgd_request_map[INTERNAL_OPT_GM_COMMMANDS] = &on_gm_commands;
	m_netgd_request_map[INTERNAL_OPT_OFFLINE_OPER] = &on_offline_oper;						//请求离线数据
	m_netgd_request_map[INTERNAL_OPT_OFFLINE_OPER_RESULT] = &on_offline_oper_result;		//请求离线数据
	m_netgd_request_map[INTERNAL_OPT_SAVE_ALL_DATA] = &on_save_all_data;

	m_netgd_request_map[INTERNAL_OPT_MAP_ROUTER_PKT] = &on_map_router_pkt;	//地图给地图发包
	m_netgd_request_map[INTERNAL_OPT_LOGIND_HOSTING] = &on_logind_hosting;	//托管登录

	m_netgd_request_map[INTERNAL_OPT_KUAFU_ENTER] = &on_kuafu_enter;		//跨服登录
	m_netgd_request_map[INTERNAL_OPT_RENAME_CHECK] = &on_rename_check;		//修改名称检测
	m_netgd_request_map[INTERNAL_OPT_UPDATE_CHAR_NAME] = &on_update_charname;	//更新角色名
	m_netgd_request_map[INTERNAL_OPT_KUAFU_BACK] = &on_kuafu_back_to_game;	//跨服回游戏服

	m_netgd_request_map[INTERNAL_OPT_INVITE_RET] = &on_faction_invite_result;	//帮派邀请结果
	
	
	//////////////////////////////////////////////////////////////////////////
	//以下是GM命令
	m_command_hanlders.insert(std::make_pair("reload_script",	&LogindApp::on_command_reload_scripts));	
	m_command_hanlders.insert(std::make_pair("reload_template",&LogindApp::on_command_reload_template));
	m_command_hanlders.insert(std::make_pair("print_object", &LogindApp::on_print_object));
	m_command_hanlders.insert(std::make_pair("memory_recovery", &LogindApp::on_command_memory_recovery));
	m_command_hanlders.insert(std::make_pair("iscanhuidang",	&LogindApp::on_command_iscanhuidang));
	m_command_hanlders.insert(std::make_pair("dohuidang",	&LogindApp::on_command_dohuidang));
	m_command_hanlders.insert(std::make_pair("save_to_db",	&LogindApp::on_command_savetodb));
}

LogindApp::~LogindApp()
{	
	safe_delete(m_db_access_mgr);
	safe_delete(m_obj_mgr);
	safe_delete(m_cache);
	safe_delete(m_http);
	safe_delete(m_gm_table);
	g_app = nullptr;	
}

bool LogindApp::Open()

{
	fp_send_to_policed = std::bind(&LogindApp::SendToPoliced,this,std::placeholders::_1);
	//如果配置文件里面的pid和sid为空则服务器不允许启动
	if(GetConfig().platform_id.empty() || GetConfig().server_id.empty())
	{
		tea_perror("config platform_id[%s] or server_id[%s] is empty!",GetConfig().platform_id.c_str(),GetConfig().server_id.c_str());
		return false;
	}

	//载入屏蔽词
	if(GetConfig().load_pingbi && !base::LoadFuckPingbi(GetConfig().storage_path))
		return false;

	tea_pinfo("\n\n load map template...");
	if(!MapTemplate::Load(GetConfig().map_folder,true))
		return false;

	//对象管理器
	m_obj_mgr = new LogindObjectManager(this);

	//数据操作类
	m_db_access_mgr = new LogindMongoDB(m_obj_mgr,g_app->get_logger());
	//多起一个异步线程
	if(!m_db_access_mgr->Open(GetConfig().db_character,GetConfig().log_level,GetConfig().db_chars_log,1/*异步连接*/))
		return false;

	//日志操作类
	/*m_db_log_mgr=new logindLogsDB(m_obj_mgr,g_app->get_logger());
	if(!m_db_log_mgr->Open(GetConfig().logdb_character,GetConfig().log_level,GetConfig().logdb_log))
		return false;*/

	if(GetConfig().server_type != WORLD_SERVER_TYPE_PK)
	{
		tea_pinfo("\n\n reset all player outline");
		g_DAL.UpdateCharsOnline("",PLAYER_ONLINE_STATE_OUTLINE);		
	}
	//缓存
	m_cache = new LogindCache;
	m_cache->LoadAllAccountInfo();
	m_cache->LoadAllAccountCharInfo();
	m_cache->LoadAllCharName();
	//登录队列
	m_login_queue =  new LogindContextLoginMgr;
	//GM管理类
	m_gm_table = new LogindGMTable;
	tea_pinfo("\n\n init server start time");
	g_app->m_open_time = (uint32)time(NULL);
	return base::Open();
}

void LogindApp::Close()
{
	//保存一下所有玩家
	SaveAllData();
	//关闭之前清空一下地图实例并且保存一遍玩家数据
	if(LogindApp::g_app->m_map_Instance_list)
		MapMgr->Close();

	//释放脚本相关
	scripts_free();
	
	RemoveAllContext();	
	
	safe_delete(m_db_access_mgr);
	safe_delete(m_cache);
	safe_delete(m_obj_mgr);
	safe_delete(m_login_queue);
	MapTemplate::UnLoad();
}

void LogindApp::Update(uint32 diff)
{
	base::Update(diff);

	if(m_status == APP_STATUS_READY_OK)
	{
		//登录队列
		m_login_queue->Update(diff);

		//地图管理器心跳
		if(MapMgr)
			MapMgr->Update(diff);

		//所有的player心跳
		for (auto it = context_map_.begin();it != context_map_.end();++it)
		{
			it->second->Update(diff);
		}
		 
		//是否是异步查询回调
		if(m_db_access_mgr)
			m_db_access_mgr->UpdateAsync();
		
		//http服务
		if(m_http ) 
			m_http->Update();
		else
			m_http = new HttpHandler;
	}
	else if(m_status == APP_STATUS_CONN_CENTD)
	{
		//先取得所有数据
		ObjMgr.CallWatchObjects(".*",[this]() {
			WorldPacket pkt(INTERNAL_OPT_GET_ONLINE_PLAYER);//再取玩家
			SendToCentd(pkt);
		});

		SetStatus(APP_STATUS_GET_ONLINE_DATA);
	}

	//对象管理器
	m_obj_mgr->Update();

}

SvrObjectManager *LogindApp::GetObjMgr()
{
    return dynamic_cast<SvrObjectManager*>(m_obj_mgr);
}

void LogindApp::LoginQueuePlayer(TimerHolder& th)
{
	if(!LogindContext::SessionQues.empty())
	{
		uint32 login_count = LogindContext::LoginPlayerCount();
		int pass_count = g_Config.max_player_count - login_count;
		tea_pdebug("LogindContext::LoginQueuePlayer max %u , now %u, pass %u, queue %u"
			,g_Config.max_player_count, login_count, pass_count, LogindContext::SessionQues.size());
		while(pass_count > 0 && LogindContext::SessionQues.size() != 0)
		{
			uint32 fd = LogindContext::SessionQues[0];
			LogindContext::SessionQues.erase(LogindContext::SessionQues.begin());	//先删除队头

			LogindContext *context = LogindContext::FindContext(fd);					
			if(context == NULL)
				continue;
			if(context->GetStatus() == STATUS_TRANSFER || context->GetStatus() == STATUS_LOGGEDIN)
			{
				tea_perror("LogindContext::LoginQueuePlayer ,but status err, %s", context->GetGuid().c_str());
				continue;
			}
			if(context->GetGuid().empty())
			{
				tea_perror("LogindContext::LoginQueuePlayer, guid empty, account %s", context->GetAccount().c_str());
				context->Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE18,"");
				continue;
			}
			m_login_queue->PushAction(context->GetGuid(), context->GetFD(), CONTEXT_LOGIN_ACTION_LOGIN);
			pass_count--;
		}
		for (uint32 index = 0; index < LogindContext::SessionQues.size(); index ++)
		{
			uint32 fd =  LogindContext::SessionQues[index];
			LogindContext *context = LogindContext::FindContext(fd);
			if(context == NULL)
			{
				LogindContext::SessionQues.erase(LogindContext::SessionQues.begin() + index);
				index--;
				continue;
			}
			Call_login_queue_index(context->m_delegate_sendpkt, index);
		}
	}
	th._next_time += 1;
}

//检测版本需求
void LogindApp::check_versions_requirements(TimerHolder& th)
{
	if(m_gameconfig && GetAddTime() == 1)
		return;
	g_DAL.GetVersionsRequirements([&](uint32 type,const string &version,const string &comment, uint32 create_time){
		if(type >= MAX_CLIENT_MODULE) 
			return;
		if(!m_clientconfig)
			return;
		uint32 old_create_time = m_clientconfig->GetUInt32(CLIENT_CONFIG_INT_FIELD_MODULE_INFO_BEGIN + MAX_CLIENT_VERSION_INT * type +CLIENT_VERSION_INT_CREATE_TIME);
		if(old_create_time > create_time && create_time != 0)
			return;

		string old_version = m_clientconfig->GetStr(CLIENT_CONFIG_STRING_FIELD_MODULE_INFO_BEGIN + MAX_CLIENT_VERSION_STRING * type + CLIENT_VERSION_STRING_VERSIONS);
		if (old_version != version)
		{
			tea_pinfo("LogindApp::check_versions_requirements update version, type %d from %s to %s"
				, type
				, old_version.c_str()
				, version.c_str());
			m_clientconfig->SetStr(CLIENT_CONFIG_STRING_FIELD_MODULE_INFO_BEGIN + MAX_CLIENT_VERSION_STRING * type + CLIENT_VERSION_STRING_VERSIONS, version);
		}

		string old_comment = m_clientconfig->GetStr(CLIENT_CONFIG_STRING_FIELD_MODULE_INFO_BEGIN + MAX_CLIENT_VERSION_STRING * type + CLIENT_VERSION_STRING_COMMENT);
		if(old_comment != comment)
		{
			tea_pinfo("LogindApp::check_versions_requirements update comment, type %d from %s to %s"
				, type
				, old_comment.c_str()
				, comment.c_str());
			m_clientconfig->SetStr(CLIENT_CONFIG_STRING_FIELD_MODULE_INFO_BEGIN + MAX_CLIENT_VERSION_STRING * type + CLIENT_VERSION_STRING_COMMENT, comment);
		}

		if(old_create_time != create_time)
		{
			tea_pinfo("LogindApp::check_versions_requirements update comment, type %d from %d to %d"
				, type
				, old_create_time
				, create_time);
			m_clientconfig->SetUInt32(CLIENT_CONFIG_INT_FIELD_MODULE_INFO_BEGIN + MAX_CLIENT_VERSION_INT * type + CLIENT_VERSION_INT_CREATE_TIME, create_time);
		}
	});
	//如果没有配置，等于0，相当于只执行一次
	th._next_time += g_Config.versions_requirements_internal;
}

//重新载入游戏配置表
void LogindApp::ReloadGameConfigTable(TimerHolder& th)
{
	m_db_access_mgr->DbOperGameConfig([](string key, string value){
		DoReloadGameConfigTable(key.c_str(), value.c_str());
	});
	uint32 kaifushijian = g_DAL.LoadKaiFuShiJian();
	if(kaifushijian != GetKaiFuShiJian())
		SetKaiFuShiJian(kaifushijian);
	th._next_time += g_Config.reload_game_config_interval;
}

//保存世界变量
void LogindApp::SaveGlobalValue(TimerHolder& th)
{
	vector<GuidObject*> vec;
	//世界变量
	ObjMgr.GetDataSetAllObject(GLOBAL_VALUE_OWNER_STRING, vec);
	g_Cache.SavePlayerData(GLOBAL_VALUE_OWNER_STRING, vec, 0);

	//帮派
	vec.clear();
	ObjMgr.GetDataSetAllObject(FACTION_BINLOG_OWNER_STRING, vec);
	g_Cache.SavePlayerData(FACTION_BINLOG_OWNER_STRING, vec, 0);

	//帮派数据
	vec.clear();
	ObjMgr.GetDataSetAllObject(FACTION_DATA_OWNER_STRING, vec);
	g_Cache.SavePlayerData(FACTION_DATA_OWNER_STRING, vec, 0);

	th._next_time += g_Config.auto_save_time;
}

void LogindApp::BackupPlayerInfo(TimerHolder& th)
{
	g_Cache.Backup();
	th._next_time += g_Config.auto_backup_hdd_time;
}

//保存有变化的玩家到数据库
void LogindApp::SaveChangePlayerToDB(TimerHolder& th)
{
	g_Cache.SaveChangePlayerToDB();
	//一天一次
	th._next_time += 24 * 60 * 60;
}

//保存玩家到数据库
void LogindApp::SavePlayerToDB(TimerHolder& th)
{
	//服务器还没有准备好
	if(m_status != APP_STATUS_READY_OK){
		th._next_time += 1;
		return ;
	}

	SavePlayerGuidObj *obj = dynamic_cast<SavePlayerGuidObj*>(LogindApp::g_app->m_save_to_db_guid_list);
	ASSERT(obj);
	uint32 i = 0;
	//如果 一个都没有，那就合服掉吧
	if(obj->GetCount() < 1 && LogindApp::g_app->GetAddTime() != 0)
		data_save_player_to_merge();
	while(obj->GetIndex() < obj->GetCount())
	{
		string guid = obj->GetSavePlayerGuid(obj->GetIndex());
		ASSERT(!guid.empty());
		if(obj->GetPost())
		{
			string server_name = obj->GetToServerName();
			ASSERT(!server_name.empty());
			tea_pinfo("LogindApp::SavePlayerToDB post data %s %s begin, index %u, count %u"
				, server_name.c_str(), guid.c_str(), obj->GetIndex(), obj->GetCount());
			//if(!m_http->PostMergeData(guid, server_name))
			//{
			//	tea_pinfo("LogindApp::SavePlayerToDB post data %s fail", guid.c_str());
			//	//一秒以后重试
			//	break;
			//}
			if(m_http->PostMergeData(guid, server_name))
			{
				tea_pinfo("LogindApp::SavePlayerToDB post data %s end", guid.c_str());
			}
			else
			{
				tea_pinfo("LogindApp::SavePlayerToDB post data %s fail, retry..", guid.c_str());
				break;
			}
		}
		else
		{
			tea_pinfo("LogindApp::SavePlayerToDB %s begin, index %u, count %u"
				, guid.c_str(), obj->GetIndex(), obj->GetCount());
			g_DAL.SaveDataset(guid, false);
			tea_pinfo("LogindApp::SavePlayerToDB %s end", guid.c_str());
		}
		//存完一个了，是不是该干点啥？
		obj->AddIndex();
		if(obj->GetIndex() == obj->GetCount())
		{
			//存完了
			for (uint32 i = 0; i < obj->GetCount(); i++)
			{
				obj->SetSavePlayerGuid(i, "");
			}
			obj->SetIndex(0);
			obj->SetCount(0);
			obj->SetToday(0);
			//是不是该干点啥？
			//合服的时候，需要回调
			if(LogindApp::g_app->GetAddTime() != 0)
			{
				data_save_player_to_merge();
			}
			break;
		}
		if(i++ >= g_Config.Save_Player_To_DB_Count)
			break;
	}
	th._next_time = uint32(time(nullptr)) + 1;
}

//释放缓存的登出玩家
void LogindApp::FreeLogoutPlayer(TimerHolder &th)
{
	g_Cache.FreeLogoutPlayer();
	th._next_time += 1;
}

//合服检测
void LogindApp::check_merge_server_begin(TimerHolder& th)
{
	//从服初始化检测
	g_DAL.LoadMergeServerStatus(MERGE_OPT_TYPE_INIT,MERGE_SERVER_TYPE_ORIGINAL,[&](merge_info* info){

		if(info)
		{
			tea_pinfo(" LogindApp::check_merge_server_begin");
			if(strlen(info->server_name) == 0 || strlen(info->merge_server_name) == 0)
			{
				tea_pinfo(" LogindApp::check_merge_server_begin server_name or merge_server_name is nil");
				uint32 t = (uint32)time(NULL) + 60;
				RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::check_merge_server_begin),t , NULL);
				return;
			}
			//先让所有的手游下线
			MobileContext::ForEach([](MobileContext *session){
				if(session)	{
					session->Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE19, "");
				}
			});

			SetAddTime(1);
			g_DAL.UpdateSeverInfoMergeStatus();
			//g_DAL.SaveServerMaxGuid();

			//通知其他服务器合服了
			WorldPacket pkt (INTERNAL_OPT_LOGIND_MERGE_SERVER);
			pkt << info->server_name;
			LogindApp::g_app->SendToAppd(pkt);
			ServerList.ForEachScened([&](uint32 fd){
				LogindApp::g_app->SendToScened(pkt, fd);
				return false;
			});
			LogindApp::g_app->SendToPoliced(pkt);

			g_DAL.UpdateMergeServerStatus(MERGE_OPT_TYPE_SAVE_ING,MERGE_SERVER_TYPE_ORIGINAL,info->merge_server_name);

			//同步保存所有有变化的玩家
			g_Cache.SaveChangePlayerToDB(true);
			SavePlayerGuidObj *obj = dynamic_cast<SavePlayerGuidObj*>(LogindApp::g_app->m_save_to_db_guid_list);
			ASSERT(obj);
			obj->SetToServerName(info->merge_server_name);
			g_Config.Save_Player_To_DB_Count = 1000;
		}
		uint32 t = (uint32)time(NULL) + 60;
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::check_merge_server_begin),t , NULL);
			
	});
}

//数据保存后通知
void LogindApp::data_save_player_to_merge()
{
	tea_pinfo("LogindApp::data_save_player_to_merge");
	merge_info info;
	memset(&info,0,sizeof(merge_info));

	//从服初始化检测
	g_DAL.LoadMergeServerStatus(MERGE_OPT_TYPE_SAVE_ING,MERGE_SERVER_TYPE_ORIGINAL,[&](merge_info* info){
		if(!info || strlen(info->merge_server_name) == 0)
			return;
		//post合服成功去主服
		if(!m_http->PostMergeOK(info->merge_server_name))
		{
			tea_pinfo("LogindApp::data_save_player_to_merge post mergeok fail %s, retry", info->merge_server_name);
			return;
		}
		//设置下合服服务器列表
		g_DAL.UpdateMergeServerList(info->merge_server_name);
		//创建文件标志已经合完服了
		stringstream ss;
		ss << time(NULL);
		g_Cache.SaveFileToMergeServer("is_merge_server",ss.str());

		//开始转移玩家
		ObjMgr.ForEachPlayer([&](logind_player *player){
			auto *session = player->GetSession();
			if(!session)
				return;
			if(session->GetStatus() == STATUS_LOGGEDIN)
			{
				bool is_fcm = player->GetFCMLoginTime() != (-1);
				Call_merge_server_msg(player->GetSession()->m_delegate_sendpkt,info->merge_server_host,info->merge_server_port,
					MakeSessionKey(player->GetAccount(),is_fcm).c_str(),0,0,0);
			}
			else session->Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE48,"",true);
		});
		//状态设置为完毕
		g_DAL.UpdateMergeServerStatus(MERGE_OPT_TYPE_END,MERGE_SERVER_TYPE_ORIGINAL,info->merge_server_name);
	});	
}

//立刻保存所有数据
void LogindApp::SaveAllData()
{
	//open都还没执行完，算了吧
	if(!LogindApp::g_app->m_cache)
		return;
	//保存一下世界变量
	TimerHolder th;
	SaveGlobalValue(th);
	//TODO: 保存军团数据

	//保存一下所有玩家
	for(auto it = context_map_.begin();it != context_map_.end();++it)
	{
		LogindContext *session = dynamic_cast<LogindContext*>(it->second);
		if(!session || session->GetStatus() != STATUS_LOGGEDIN)
			continue;
		g_Cache.SavePlayerData(session->GetGuid());
	}
	g_Cache.FreeLogoutPlayer(true);
}

//生成session_key
string LogindApp::MakeSessionKey(string account, bool indulge, string otherdata/*=""*/, string login_key/*=""*/, bool is_hosting/*=false*/)
{
	string session_key = "";
	string sIndulge = "n";
	if(indulge)
		sIndulge = "y";
	string hosting = "n";
	if(is_hosting)
		hosting = "y";
	uint32 now = (uint32)time(NULL);
	//b64加密
	char buf[10000]={0};
	string pid,sid,uid;
	string str = account;
	//倒数第一个,号的后面存储着玩家真正的名称
	size_t pos = str.find('_');
	if(pos == string::npos)
		return "";
	//第一个逗号分割着pid
	pid = str.substr(0,pos);	
	str = str.substr(pos+1,str.length()-pos);
	//第二个逗号存着sid
	pos = str.find('_');//跳过,
	if(pos == string::npos)
		return "";
	sid = str.substr(0,pos);
	uid = str.substr(pos+1,str.length()-pos);

	sprintf(buf,"pid=%s&sid=%s&uid=%s&time=%u&indulge=%s&hosting=%s",pid.c_str(),sid.c_str(),uid.c_str(),now,sIndulge.c_str(),hosting.c_str());
	string src = buf;
	if (!otherdata.empty())
		src += otherdata;		//其他数据不空，则拼接上去
	char dst[1024] = {0};
	if(base64_encode(src.c_str(),src.size(),dst,(int)sizeof(dst)) == 0)
	{
		string auth = dst;
		if (login_key.empty())
			auth += g_Config.login_key;
		else
			auth += login_key;
		static char tmp[10000];
		const char * md5_str = MD5CryptToStr32((int8*)auth.c_str(),auth.size(),tmp);
		//校验码md5_str
		sprintf(buf,"?auth=%s&sign=%s",dst,md5_str);
		session_key = buf;
	}
	return session_key;
}

//同步guid管理器的最大guid累加数到其他服
void LogindApp::call_sync_guids_max(uint32 player_max)
{
	//下发给其他服务器之前先同步下本服的数据
	m_guid_mgr.SyncMaxGuid(player_max);
	WorldPacket _pkt (INTERNAL_OPT_SYNC_GUID_MAX);
	_pkt << player_max;
	LogindApp::g_app->SendToAppd(_pkt);	
	ServerList.ForEachScened([&](uint32 fd){
		LogindApp::g_app->SendToScened(_pkt, fd);
		return false;
	});	
}

//为中心服加索引
void LogindApp::CallCentdCreateIndex()
{
	ObjMgr.CallCreateIndex(REGEX_GLOBAL);
	ObjMgr.CallCreateIndex(REGEX_PLAYER);
	ObjMgr.CallCreateIndex(REGEX_MAPPLAYERINFO);
	ObjMgr.CallCreateIndex(REGEX_LIMIT);
	ObjMgr.CallCreateIndex(REGEX_FACRION);
}

//检查一些变量是否存在，不存在则创建
void LogindApp::CheckObject()
{
	if(!ObjMgr.Get(GLOBAL_SAVE_TO_DB))
	{
		m_save_to_db_guid_list = new SavePlayerGuidObj;
		m_save_to_db_guid_list->SetGuid(GLOBAL_SAVE_TO_DB);
		m_save_to_db_guid_list->SetOwner(GLOBAL_VALUE_OWNER_STRING);
		ObjMgr.CallPutObject(m_save_to_db_guid_list);
	}
}

void LogindApp::OnDataReady()
{
	//重新修改一下连接字符串
	auto *g = dynamic_cast<GlobalValueManager*>(LogindApp::g_app);
	if (g)
	{
		g->SetDbConnStr(g_Config.db_character);
		g->SetLogDbConnStr(g_Config.logdb_character);
		g->SetLoginKey(g_Config.login_key);
		if (g->GetExtWebInterface() != g_Config.ext_web_interface)
			g->SetExtWebInterface(g_Config.ext_web_interface);
		if (g->GetConfSvrUrl() != g_Config.conf_svr)
			g->SetConfSvrUrl(g_Config.conf_svr);
		if (g->GetWorldServerUrl() != g_Config.world_server_url)
			g->SetWorldServerUrl(g_Config.world_server_url);
	}	
	
	uint32 t = uint32(time(NULL)) + 1;//别立刻执行
	RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::FreeLogoutPlayer), t, NULL);
	if(!IsPKServer())
	{
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::LoginQueuePlayer), t, NULL)
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::check_versions_requirements), t, NULL);
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::ReloadGameConfigTable), t, NULL);
		//RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::check_merge_server), t, NULL);
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::SaveGlobalValue), t + g_Config.auto_save_time, NULL);
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::check_merge_server_begin),t , NULL);
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::BackupPlayerInfo), t + g_Config.auto_backup_hdd_time, NULL);
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::SavePlayerToDB), t, NULL);
		//随机每天凌晨2 - 6点	
		//uint32 save_t;
		//if(DoGetNextSaveTime(save_t))
		//	ASSERT(false);
		//RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::SaveChangePlayerToDB),save_t, NULL);

		//每十分钟发送一次在线人数到日志服
		t = t - t % 600 + 600;
		RegTimer(fastdelegate::MakeDelegate(g_app->m_gm_table, &LogindGMTable::SendOnilne), t, NULL);
	}
	else
	{
		RegTimer(fastdelegate::MakeDelegate(g_app, &LogindApp::SendWebConn),  uint32(time(NULL)) + 10, NULL);
	}
	//ok,服务器准备完毕
	ServerList.SetServerReadyOKFlag(g_app->m_connection_id);
	SetStatus(APP_STATUS_READY_OK);
}

void LogindApp::CollapseReset(server_packet *pkt)
{	
	//载入脚本
	scripts_init(g_Config.script_folder.c_str());
	LoadTemplate();
	
	tea_pinfo("logind is collapse, reset!");
	MapMgr->Restart();
	
	ObjMgr.CollapseReset();
	
	//恢复一下玩家
	uint16 size;
	*pkt >> size;
	for (uint16 i = 0; i < size; i++)
	{
		int fd = 0;
		string lguid;
		string remote_ip;
		uint16 port = 0;
		uint8 loging_type;
		*pkt >> fd;
		*pkt >> lguid;
		*pkt >> remote_ip;
		*pkt >> port;
		*pkt >> loging_type;
		if(lguid.empty())
		{
			tea_pwarn("LogindApp::on_get_online_player fd:%u guid is empty", fd);
			continue;
		}

		logind_player *player = ObjMgr.FindPlayer(lguid);
		if(!player)
		{
			tea_pinfo("LogindApp::on_get_online_player player data not found, CollapseReset end");
			continue;
		}
		if(loging_type == CONTEXT_TYPE_YEYOU)
		{
			tea_pinfo("LogindApp::on_get_online_player CONTEXT_TYPE_YEYOU player %s fd:%u CollapseReset begin", lguid.c_str(), fd);
			LogindContext *context = new LogindContext(fd,this);//LogindContext::CreateContext(fd);
			InsertContext(context);
			context->m_remote_ip = remote_ip;
			context->m_remote_port = port;
			context->CollapseReset(player);
			tea_pinfo("LogindApp::on_get_online_player CONTEXT_TYPE_YEYOU player %s fd:%u CollapseReset end", lguid.c_str(), fd);
		}
		else if (loging_type == CONTEXT_TYPE_PHONE)
		{
			tea_pinfo("LogindApp::on_get_online_player CONTEXT_TYPE_PHONE player %s fd:%u CollapseReset begin", lguid.c_str(), fd);
			MobileContext *context = new MobileContext(fd,this);//LogindContext::CreateContext(fd);
			InsertContext(context);
			context->m_remote_ip = remote_ip;
			context->m_remote_port = port;
			context->CollapseReset(player);
			tea_pinfo("LogindApp::on_get_online_player CONTEXT_TYPE_PHONE player %s fd:%u CollapseReset end", lguid.c_str(), fd);
		}
		else
		{
			ASSERT(false);
		}
	}

	//把登录服掉线期间下线的玩家下线。
	vector<string> guid_vec;
	ObjMgr.ForEachPlayer([&guid_vec](logind_player *player){
		//没有session的player，是已经下线的玩家
		if(!player->GetSession() && !player->GetMobileSession())
		{
			tea_pinfo("LogindApp::on_get_online_player player %s is outline", player->GetGuid().c_str());
			guid_vec.push_back(player->guid());
		}
	});

	for (auto guid:guid_vec)
	{
		//下线了
		logind_player *player = ObjMgr.FindPlayer(guid);
		LogindContext *context = new LogindContext(0,this);		
		context->SetPlayer(player);
		player->SetSession(context);
		context->SetStatus(STATUS_LOGGEDIN);
		m_login_queue->AddOfflineContext(context);
		m_login_queue->PushAction(context->GetGuid(), 0, CONTEXT_LOGIN_ACTION_CLOSE);
	}
	OnDataReady();
}

int LogindApp::on_command_reload_scripts(tcp_connection *conn, server_packet *,uint32 guid, uint32 connectid, Tokens& tokens)
{
	//重新载入脚本
	scripts_reload();
	LoadTemplate();
	return 0;
}

int LogindApp::on_command_reload_template(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens)
{
	//载入屏蔽词
	if(g_app->GetConfig().load_pingbi && !g_app->LoadFuckPingbi(g_app->GetConfig().storage_path))
		return false;

	LoadTemplate();
	tea_pinfo("\n\n开始载入地图模板信息...");
	if(!MapTemplate::Load(g_app->GetConfig().map_folder,true))
		return false;

	return 0;
}

//回档检测
int LogindApp::on_command_iscanhuidang(tcp_connection *conn, server_packet *,uint32 , uint32 connectid, Tokens& tokens)
{
	if (!g_app || !g_app->m_gameconfig)
	{
		tea_pinfo("server %s has not ready! ",tokens[1].c_str());
		SendCommandResult(conn, connectid, "server is error", getpid());
		return 0;
	}
	if(g_app->GetServerID() != tokens[1])
	{
		tea_pinfo("server is error %s",tokens[1].c_str());
		SendCommandResult(conn, connectid, "server is error", getpid());
		return 0;
	}

	tea_pinfo("DatadApp::on_command_iscanhuidang succeed");
	COMMAND_SEND_SUCCEED;
	return 0;
}

//执行回档
int LogindApp::on_command_dohuidang(tcp_connection *conn, server_packet *,uint32 , uint32 connectid, Tokens& tokens)
{
	//执行回档
	if (g_Cache.HuidangPlayerInfos(tokens[1], tokens[2]))
	{
		tea_pinfo("DatadApp::on_command_dohuidang succeed");
		COMMAND_SEND_SUCCEED;
	}
	else
	{
		tea_pinfo("DatadApp::on_command_dohuidang failed!!");
		SendCommandResult(conn, connectid, "huidang failed", getpid());
	}
	return 0;
}

//保存到数据库
int LogindApp::on_command_savetodb(tcp_connection *conn, server_packet *,uint32 , uint32 connectid, Tokens& tokens)
{
	g_Cache.SaveChangePlayerToDB();
	COMMAND_SEND_SUCCEED;
	return 0;
}

//战斗服定时通知web服服务器状态
void LogindApp::SendWebConn(TimerHolder& th)
{
	LogindApp::g_app->m_http->RegBattleSvrConn();
	th._next_time += 600;
}
