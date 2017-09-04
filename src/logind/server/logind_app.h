#ifndef _LOGIND_H_
#define _LOGIND_H_

#include <shared/storage_strus.h>
#include <protocol/external.h>
#include <server/svrcore_app.h>
#include <protocol/WorldPacket.h>
#include "logind_object_manager.h"

class DatabaseWorkerPool;
class MapManager;
class LogindContext;
class LogindObjectManager;
class LogindMongoDB;
class LogindCache;
class LogindContextLoginMgr;
class HttpHandler;
class LogindGMTable;

using core_obj::SyncEventRecorder;

//帐号结构体
typedef struct account_table_t
{
	char sid[16];
	char uid[40];
	char pid[40];
	char name[60];
	char ip[30];
	int is_FCM;
	int gmlevel;
	char platdata[200];
public:
	string ToString();
	void FromString(const string &str);
	void NewFromString(const string &str);
}account_table;

//////////////////////////////////////////////////////////////////////////
class LogindConfig:public DbSvrCoreConfig
{
public:
	string platform_id;		//平台id
	string login_key;						//与平台通讯 密钥
	string server_id;						//服务器ID
	string server_name;						//平台id_服务器id
	uint32 max_player_count;				//最大允许在线人数
	uint32 login_queue_interval;			//轮询等待玩家是否可以进入游戏的时间间隔
	uint32 versions_requirements_internal;	//检测版本需求
	uint32 reload_game_config_interval;		//重新读取游戏配置间隔
	bool check_session_key_time;			//校验session key时间
	bool gm_open;							//gm开关
	uint32 server_type;						//服务器类型
	uint32 max_secend_count;				//场景服最大数量
	uint32 scened_collapse_time_out;		//地图管理器对场景服崩溃的容忍时间
	string test_server_live_uid;			//测试服务器健康程度的特殊UID
	uint32 auto_save_time;					//自动保存数据时间间隔
	uint32 auto_backup_hdd_time;			//备份数据时间间隔
	uint32 Save_Player_To_DB_Count;			//每次轮询保存到数据库的个数
	vector<string> g_cant_make_name;		//取名屏蔽字
	string ext_web_interface;				//外部web接口配置
	string conf_svr;						//配置服web地址配置
	string world_server_url;				//世界服url配置


protected:
	virtual bool _Load(config_loader *loader);
};

typedef struct merge_info_t{
	char server_name[50];			//原始服务器名
	char merge_server_host[50];	//合服域名
	uint32 merge_server_port;		//合服端口
	char merge_server_name[50];		//合服服务器名
}merge_info;

//登录服应用
class LogindApp:public SvrCoreApp<LogindApp>
{
	static const int WORLD_SLEEP_CONST = 1000/60;			//60帧
	friend class SvrCoreApp<LogindApp>;
public:
	LogindApp(SvrCoreParams& params,LogindConfig& config);
	virtual ~LogindApp();
public:
	inline LogindConfig& GetConfig()
	{
		return  dynamic_cast<LogindConfig&>(m_config);
	}
	virtual bool Open();	
	void Close();
	
	virtual void Update(uint32 diff);

	LogindMongoDB *m_db_access_mgr;	//数据管理器
	LogindCache *m_cache;			//缓存
	LogindObjectManager	*m_obj_mgr;	//对象管理器
	LogindContextLoginMgr *m_login_queue;	//登录队列
	HttpHandler *m_http;
	LogindGMTable *m_gm_table;		//GM后台管理器

	map<string, int>			m_removing_player;		//移除数据中的玩家，这些玩家期间将无法登录
	//弹窗处理
	void loadRightFloatCallback(int id, int status, int zone1, int zone2, int zone3, int mode, uint32 ftime, uint32 ttime,uint32 adddate,
		string subject, string content, string link);

	//生成session_key
	string MakeSessionKey(string account, bool indulge, string otherdata="", string login_key="", bool is_hosting = false);
protected:
	SvrObjectManager *GetObjMgr();
	void LoginQueuePlayer(TimerHolder& th);				//登录队列
	void check_versions_requirements(TimerHolder& th);	//检测版本需求
	void check_merge_server_begin(TimerHolder& th);		//从服合服检测
	void ReloadGameConfigTable(TimerHolder& th);		//重新载入游戏配置表
	void SaveGlobalValue(TimerHolder& th);				//保存世界变量定时器
	void BackupPlayerInfo(TimerHolder& th);				//备份玩家数据定时器
	void SaveChangePlayerToDB(TimerHolder& th);			//保存有变化的玩家到数据库
	void SavePlayerToDB(TimerHolder& th);				//保存玩家到数据库
	void FreeLogoutPlayer(TimerHolder &th);				//释放缓存的登出玩家
	void SendWebConn(TimerHolder& th);					//战斗服定时通知web服服务器状态

	void data_save_player_to_merge();				//数据保存后通知

	void SaveAllData();							//立刻保存所有数据
private:
	const static int SERVER_TYPE = SERVER_TYPE_LOGIND;
private:
	void CollapseReset(server_packet *pkt);
	void OnDataReady();

	void CallCentdCreateIndex();							//为中心服加索引
	void GoToPkServer(const string &player_guid);			//跨服到战斗服
	void CheckObject();										//检查一些变量是否存在，不存在则创建
private:
	//////////////////////////////////////////////////////////////////////////
	//协议
	static int on_netgd_create_conn(tcp_connection *,server_packet *pkt);
	static int on_netgd_destory_conn(tcp_connection *,server_packet *pkt);
	static int on_centd_open_time(tcp_connection *,server_packet *pkt);			//中心服同步服务器时间到场景服

	static int on_create_conn_get_session(tcp_connection *,server_packet *pkt);	//客户端连接来了
	static int on_scened_del_map(tcp_connection *, server_packet *pkt);					//场景服要求释放地图实例
	static int on_get_online_player(tcp_connection *, server_packet *pkt);						//连接中心服成功
	static int on_gm_commands(tcp_connection *, server_packet *pkt);		//GM命令
	static int on_centd_guid_object_table(tcp_connection *,server_packet *pkt);	
	static int on_offline_oper(tcp_connection *, server_packet *pkt);
	static int on_offline_oper_result(tcp_connection *, server_packet *pkt);
	static int on_save_all_data(tcp_connection *, server_packet *pkt);
	static int on_map_router_pkt(tcp_connection* coon, server_packet* pkt);	//地图给地图发包
	static int on_logind_hosting(tcp_connection* coon, server_packet* pkt);	//托管登录

	static int on_kuafu_enter(tcp_connection* coon, server_packet* pkt);	//跨服进入信息
	static int on_rename_check(tcp_connection* coon, server_packet* pkt);	//检测名称

	static int on_update_charname(tcp_connection* coon, server_packet* pkt);

	static int on_kuafu_back_to_game(tcp_connection* coon, server_packet* pkt);	//主动让跨服回游戏服

	static int on_faction_invite_result(tcp_connection* coon, server_packet* pkt);	//帮派邀请结果
	//////////////////////////////////////////////////////////////////////////
	//GM命令
	static int on_command_reload_scripts(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_reload_template(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_dohuidang(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);		//执行回档
	static int on_command_iscanhuidang(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);	//回档检测
	static int on_command_savetodb(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);	//保存到数据库
public:
	void call_sync_guids_max(uint32 player_max);		//同步guid管理器的最大guid累加数到其他服	
};

#define g_Config LogindApp::g_app->GetConfig()
#define globalValue (LogindApp::g_app->m_globalvalue)
#define Pingbi(str) LogindApp::g_app->FuckPingbi(str)
#define RegTimer(cb,t,param) LogindApp::g_app->RegisterTimer((cb),(t),(param));
#define CancleTimer(cb) LogindApp::g_app->CancleTimer((cb));
#define g_GuidMgr (LogindApp::g_app->m_guid_mgr)
#define g_DAL (*LogindApp::g_app->m_db_access_mgr)
#define g_Cache (*LogindApp::g_app->m_cache)
#define ObjMgr (*LogindApp::g_app->m_obj_mgr)
#define ServerList (*LogindApp::g_app->m_server_conn_list)
#define	MapMgr dynamic_cast<MapManager*>(LogindApp::g_app->m_map_Instance_list)
#define GmTable (*LogindApp::g_app->m_gm_table)

#if COMPILER == COMPILER_GNU
#  define tea_perror(str,...)	out_error(LogindApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pdebug(str,...)	out_debug(LogindApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pwarn(str,...)	out_warn(LogindApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pinfo(str,...)	out_str(LogindApp::g_app->get_logger(),str,##__VA_ARGS__)
#else
#  define tea_perror(str,...)	out_error(LogindApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pdebug(str,...)	out_debug(LogindApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pwarn(str,...)	out_warn(LogindApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pinfo(str,...)	out_str(LogindApp::g_app->get_logger(),str,__VA_ARGS__)
#endif

#endif


