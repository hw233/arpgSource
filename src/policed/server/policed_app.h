#ifndef _POLICED_APP_H_
#define _POLICED_APP_H_

#include <svrcore.h>
#include <comm/timer.h>
#include <protocol/external.h>
#include <object/SharedDef.h>
#include "policed_object_manager.h"
#include "policed_mongo_log.h"
#include <http/client/http_post.h>
#include "policed_login_mgr.h"

class LogFileOperate;
class LogFileTencent;
class GamePolice;
class PolicedMongoDB;
class PolicedMongoLog;
class PlatformPost;
class HttpHandler;

#if COMPILER == COMPILER_GNU
#  define tea_perror(str,...)	out_error(PolicedApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pdebug(str,...)	out_debug(PolicedApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pwarn(str,...)	    out_warn(PolicedApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pinfo(str,...)	    out_str(PolicedApp::g_app->get_logger(),str,##__VA_ARGS__)
#else
#  define tea_perror(str,...)	out_error(PolicedApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pdebug(str,...)	out_debug(PolicedApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pwarn(str,...)	    out_warn(PolicedApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pinfo(str,...)	    out_str(PolicedApp::g_app->get_logger(),str,__VA_ARGS__)
#endif

typedef struct account_info_t
{
	char id[50];					//帐号ID
	char sid[50];				//服务器ID
	char uid[50];				//平台的账号ID
}account_info;

//////////////////////////////////////////////////////////////////////////
class PolicedConfig:public DbSvrCoreConfig
{
public:
	string game_code;		//游戏名称简写
	uint32 lock_ip_max;		//封禁IP的最大次数
	uint32 game_id;			//游戏ID
	bool do_post;			//是否POST数据
	string server_id;		//服务器ID
	bool post_chat;			//是否post聊天记录
	string online_port_url;	//在线人数推送地址
	string online_port_key;	//在线人数推送key
protected:
	virtual bool _Load(config_loader *loader);
};

class PolicedApp:public SvrCoreApp<PolicedApp>
{
	static const int WORLD_SLEEP_CONST = 1000/12;			//12帧
	friend class SvrCoreApp<PolicedApp>;
public:
	PolicedApp(SvrCoreParams& params,SvrCoreConfig& config);
	virtual ~PolicedApp();

	virtual bool Open();
	
	inline PolicedConfig& GetConfig()
	{
		return dynamic_cast<PolicedConfig&>(m_config);
	}

	void Close();

	void Update(uint32 diff);
	void AddWatchGlobalValueOk();

	LogFileOperate *m_log_fild_operate;
	LogFileTencent *m_log_fild_tencent;
	GamePolice *m_game_police;
	PolicedMongoDB* m_mongodb;
	PolicedMongoLog* m_log_mongodb;
	PolicedObjectManager *m_obj_mgr;

	vector<packet*> m_pkt_cache;
	map<string, account_info*> m_account;	//帐号集合
	vector<PlatformPost*> m_platform_post;	//post作业类集合
	http::client::client m_post;			//异步推送
	HttpHandler *m_http;
	PolicedLoginMgr *m_login_mgr;
public:
	bool LoadInfo(const char *account, const char * player_id, account_info **a_info, PolicedContext **player, const char *uid = "");
	account_info *LoadAccountInfo(const char * account, const char *uid);
	void InitPlatfromPost();
	void PortData(string url,const string& data);	
	void MemoryRecovery(TimerHolder& th);	//内存回收
	void GetOnlinePlayerNum(TimerHolder& th);	//定时统计在线人数
	void PortOnline(TimerHolder& th);			//定时推送在线
protected:
	SvrObjectManager *GetObjMgr()
	{
		return dynamic_cast<SvrObjectManager *>(m_obj_mgr);
	}
private:		
	static bool write_log_fd(WorldPacket &pkt);
	//////////////////////////////////////////////////////////////////////////
	//		
	const static int SERVER_TYPE = SERVER_TYPE_POLICED;
	
	static int on_get_online_player(tcp_connection *,server_packet *pkt);
	static int on_write_log(tcp_connection *, server_packet *pkt);				//其他服务器发来写日志请求	
	static int on_write_tencent_log(tcp_connection *, server_packet *pkt);				//其他服务器发来写腾讯日志请求	
	static int on_player_login(tcp_connection *,server_packet *pkt);
	static int on_player_logout(tcp_connection *,server_packet *pkt);
	static int on_centd_guid_object_table(tcp_connection *,server_packet *pkt);
	static int on_gm_commands(tcp_connection *,server_packet *pkt);
	static int on_logind_merge_server(tcp_connection* coon, server_packet* pkt);	//登陆服通知合服了
	static int on_update_gag_status(tcp_connection* coon, server_packet* pkt);		//更新禁言状态
	static int on_update_lock_status(tcp_connection* coon, server_packet* pkt);		//更新禁言状态
	//后台数据统计相关
	static int on_ht_forge_up_info(tcp_connection *,server_packet *pkt);	//锻造成功统计
	static int on_save_backup(tcp_connection *,server_packet *pkt);	//锻造成功统计
	static int on_save_first_recharge(tcp_connection *,server_packet *pkt);	//首充日志
	static int on_add_gold_log(tcp_connection *, server_packet *pkt);
	////////////////////////////////////////////////////////////////////////////////////
	//后台命令
	static int on_command_reload_scripts(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_reload_template(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
};

#define g_Config PolicedApp::g_app->GetConfig()
#define g_DAL (*PolicedApp::g_app->m_mongodb)
#define g_LOG (*PolicedApp::g_app->m_log_mongodb)
#define RegTimer(cb,t,param) PolicedApp::g_app->RegisterTimer((cb),(t),(param));
#define CancleTimer(cb) PolicedApp::g_app->CancleTimer((cb));
#define ObjMgr (*PolicedApp::g_app->m_obj_mgr)
#define g_GuidMgr (PolicedApp::g_app->m_guid_mgr)
#define ServerList (*PolicedApp::g_app->m_server_conn_list)
#endif
