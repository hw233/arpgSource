/************************************************************************
 * file	: robotd_app.h
 * desc	: 机器人服
 *		就是专门做机器人这回事的服务器
 *
 *
 * author:	jiangzy
 * date:	20131116
 * version:
 ************************************************************************/
#ifndef _ROBOTD_APP_H_
#define _ROBOTD_APP_H_

#include <core_socket/Application.h>
//#include <svrcore.h>
#include <comm/timer.h>
#include <protocol/external.h>
#include "PathFinding.h"
#include <svrcore/server/Storage.h>

namespace core_obj {
	class SyncEventRecorder;
};
class RobotdContext;
class SvrObjectManager;
//////////////////////////////////////////////////////////////////////////
class RobotConfig:public DbSvrCoreConfig
{
public:	
	string login_key;						//与平台通讯 密钥
	string web_port;						//http端口
	string hdd_path;						//硬盘存储地址
	string context_file_name;				//保存登录账号的文件名
	bool login_queue;						//是否使用登录队列
protected:
	virtual bool _Load(config_loader *loader);
};

typedef struct OpSize_t
{
	uint32 opcode;
	uint32 size;
}OpSize;

namespace http{namespace server{
class server;
}}


class RobotdApp:public core_socket::Application
{
	static const int WORLD_SLEEP_CONST = 1000/10;			//10帧
	friend class SvrCoreApp<RobotdApp>;	
public:
	static RobotdApp *g_app;

	RobotdApp(SvrCoreParams &params, RobotConfig &config);
	virtual ~RobotdApp();
public:
	virtual bool Open();

	inline RobotConfig& GetConfig()
	{
		return m_config;
	}

	void Close();
	void Update(uint32 diff);
	
	RobotdContext *GetContext(const string &account);
	RobotdContext *GetContext(const string &pid,const string &sid,const string &uid);

	uint32 RobotCount(){return (uint32)m_all_context.size();}

	void ForeachAllContext(std::function<void(RobotdContext*)> func)
	{
		for (auto it:m_all_context)
		{
			func(it.second);
		}
	}

	void StatUpdateIndex(core_obj::SyncEventRecorder *blobj, UpdateMask& mask,UpdateMask& str_mask);
	void PrintUpdate(uint32 diff);
protected:
	void RegHttpHandler();
	string FromHdd(const string &str){return str == "null" ? "" : str;}
	string ToHdd(const string &str){return str.empty() ? "null" : str;}
	static void OnSignal(int s);
public:
	map<string, RobotdContext*> m_all_context;
	std::queue<string> m_login_queue;

	RobotConfig& m_config;

	//http控制接口
	http::server::server *web_svr_;
	core_obj::Storage			*m_storage;
	IntervalTimer m_opcode_size_timer;
	IntervalTimer m_login_timer;

	vector<OpSize> all_op_sizie_map;
	map<string, vector<uint32>> update_index_stat;
};

#if COMPILER == COMPILER_GNU
#  define tea_perror(str,...)	RobotdApp::g_app->logger()->Out(LOGLEVEL_ERROR,str,##__VA_ARGS__)
#  define tea_pdebug(str,...)	RobotdApp::g_app->logger()->Out(LOGLEVEL_DEBUG,str,##__VA_ARGS__)
#  define tea_pwarn(str,...)	RobotdApp::g_app->logger()->Out(LOGLEVEL_WARN,str,##__VA_ARGS__)
#  define tea_pinfo(str,...)	RobotdApp::g_app->logger()->Out(LOGLEVEL_INFO,str,##__VA_ARGS__)
#else
#  define tea_perror(str,...)	RobotdApp::g_app->logger()->Out(LOGLEVEL_ERROR,str,__VA_ARGS__)
#  define tea_pdebug(str,...)	RobotdApp::g_app->logger()->Out(LOGLEVEL_DEBUG,str,__VA_ARGS__)
#  define tea_pwarn(str,...)	RobotdApp::g_app->logger()->Out(LOGLEVEL_WARN,str,__VA_ARGS__)
#  define tea_pinfo(str,...)	RobotdApp::g_app->logger()->Out(LOGLEVEL_INFO,str,__VA_ARGS__)
#endif

#define g_Config RobotdApp::g_app->GetConfig()
#endif
