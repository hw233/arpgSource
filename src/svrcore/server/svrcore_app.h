#ifndef _SVRCORE_APP_H_
#define _SVRCORE_APP_H_

#include <comm/common_stl.h>
#include <comm/configloader.h>
#include <net/netstd.h>
#include <protocol/internal.h>
#include <fastdelegate/FastDelegate.h>
#include <functional>
#include <object/BinLogObject.h>
#include <protocol/WorldPacket.h>
#include <server/svr_context_manager.h>
#include <server/svr_object_manager.h>
#include <object/GlobalValueManager.h>

extern "C" {
	#include <ahocorasick/aho_corasick.h>
}

#define NETGD_HOST	"127.0.0.1"				/*网关服务IP*/
#define NETGD_PORT	10086					/*网关服务端口*/

#define CENTD_HOST	"127.0.0.1"				/*中心服IP*/
#define CENTD_PORT	10087					/*中心服端口*/

const int DAY_SECONDS = 60*60*24;			//每天的秒数
const int TIME_ZONE = 8;					//时区
const int CVT_DIFF_UTC = 60*60*TIME_ZONE;	//+8时区与UTC时间差异秒数

#define WANNENG_LOGIN_KEY "dc829703a6039ff5262ec2d0d520444a"		//万能登录密钥

//////////////////////////////////////////////////////////////////////////
//命令行参数
struct TEA_SVRCORE_SPEC SvrCoreParams
{

	typedef void (*usage_ptr)(const char *s);	

	bool Load(int argc,const char **argv,usage_ptr __usage);
public:
	SvrCoreParams():generic_id(0),run_test(0),centd_port(0),web_port(""),context_file_name(""){}

	string config_file;	
	int generic_id;
	int run_test;
	uint16 centd_port;
	string web_port;
	string context_file_name;
};

//////////////////////////////////////////////////////////////////////////
//配置信息
struct TEA_SVRCORE_SPEC SvrCoreConfig
{
	virtual ~SvrCoreConfig(){}
	
	uint16 netgd_port;		//网关服端口
	string netgd_host;		//网关服服务IP

	uint16 centd_port;		//中心服端口
	string centd_host;		//中心服IP	

	string log_folder;		//日志路径
	string log_name;		//日志模块名称

	string backup_hdd_path;					//备份路径
	string player_data_hdd_path;			//玩家数据保存路径
	string player_data_hdd_path_recycle;	//数据回收站

	int	   log_level;		//日志等级

	string storage_path;	//数据存储路径
	bool	load_pingbi;	//载入屏蔽词

	//载入配置文件
	bool Load(string file, uint32 centd_port_para);
	bool ReLoad();
protected:
	string m_config_file_path;
protected:
	virtual bool _Load(config_loader *loader);
};

//游戏配置
struct TEA_SVRCORE_SPEC GameConfig:public SvrCoreConfig
{
	virtual ~GameConfig(){}
	
	string script_folder;	//脚本文件夹
	string map_folder;		//地图文件夹
	
	bool load_creature_path;//载入生物干道
	string test_server_live_uid;//特殊UID
protected:
	virtual bool _Load(config_loader *loader);
};

//带数据库配置
struct TEA_SVRCORE_SPEC DbSvrCoreConfig:public GameConfig
{
	virtual ~DbSvrCoreConfig (){}

	string db_chars_log;		//角色数据库日志记录
	string db_character;		//角色库连接字符串	
	string logdb_log;			//角色数据库日志记录
	string logdb_character;		//角色库连接字符串
protected:
	virtual bool _Load(config_loader *loader);
};

//////////////////////////////////////////////////////////////////////////
//应用程序基类
struct TimerHolder;
typedef fastdelegate::FastDelegate1<TimerHolder&,void> Timer_Callback;
struct TimerHolder
{
	Timer_Callback cb;	//触发回调
	char sTimer[10];	//每天触发的时间点,24小时制,精确到分钟,如果为空则只触发一次
	uint32 _next_time;	//运行时需要,下一次触发时间,初始化为0,如果小于现在则会被移除，若手工修改或设置值，则需要注意时区问题
	void *param;		//回调保留参数	
	int deleted;
};

template<typename PARAM>
struct InternalCallback;

//服务器状态
enum EAppStatus
{
	APP_STATUS_INIT						= 0,		//初始化
	APP_STATUS_CONN_CENTD				= 1,		//连接中心服成功
	APP_STATUS_WAIT_LOGIND_READY_OK		= 2,		//等待登录服连接完毕
	APP_STATUS_LOGIND_READY_OK			= 3,		//登录服连接完毕
	APP_STATUS_GET_ONLINE_DATA			= 4,		//获取本服务器所关心的在线数据

	APP_STATUS_READY_OK					= 5,		//数据准备完毕

	APP_STATUS_CLOSE					= 10,		//准备关闭
};

//跨服战结果领取加密key
#define KFZ_RESULT_KEY	"suibianla..123456"

class SvrCoreAppImpl:public ContextManager, public GlobalValueManager
{
protected:
	//typedef WORLD_SLEEP_CONST 50 ;		//20帧
	typedef std::vector<TimerHolder> TimerHolderArr;	
	typedef std::function<int (tcp_connection *,server_packet *pkt,uint32 guid,uint32 connectionid,Tokens &tokens)> func_command_handler;
	typedef std::map<string,func_command_handler> Command_Handler_Map;
	typedef core_obj::EventDispatcher<bool> EventMgr;

	static void* WorldTick(void *arg);
public:
	
	typedef int (*netgd_handle_ptr)(tcp_connection *,server_packet *pkt);				
	SvrCoreAppImpl(SvrCoreParams& params,SvrCoreConfig& config);
	virtual ~SvrCoreAppImpl();

	virtual void Update(uint32 diff);
	void Wait(){WorldTick(this);};

	void SendToNetgd(server_packet *pkt);
	void SendToCentd(server_packet *pkt);

	bool SendToServerByFd(WorldPacket &pkt, uint32 fd);
	inline bool SendToScened(WorldPacket &pkt, uint32 fd){return SendToServerByFd(pkt, fd);}
	bool SendToAppd(WorldPacket &pkt);
	bool SendToPoliced(WorldPacket &pkt);
	bool SendToLogind(WorldPacket &pkt);

	inline log_holder *get_logger()
	{
		ASSERT(m_logger);
		return m_logger;
	}	

	inline tcp_connection *get_connection()
	{
		return m_netgd_connection;
	}

	void TryClose()
	{
		if(m_logger)
			out_str(m_logger,"try stop SvrCoreApp");
		SetStatus(APP_STATUS_CLOSE);
	}

	//注册定时器回调
	void RegisterTimer(Timer_Callback cb,const char* t,void *param=NULL);		//可重复触发
	void RegisterTimer(Timer_Callback cb,uint32 t,void *param=NULL)	;			//正常只触发一次
	void CancleTimer(Timer_Callback cb);	

	//////////////////////////////////////////////////////////////////////////
	//发包给某链接，如果包太大，则分包发送
	typedef struct splice_pkt_t
	{
		uint32 id;
		uint32 max_size;
		time_t t;
		packet *pkt;
	}splice_pkt;
	vector<splice_pkt*> m_splice_pkt_map;
	void SendSplicePkt(tcp_connection *conn,packet *pkt);

	//////////////////////////////////////////////////////////////////////////
	//载入屏蔽词
	bool LoadFuckPingbi(string path);
	bool FuckPingbi(char* str);

	//注册协议分配到本服务器
	void RegSessionOpts(uint32 fd);
	void RegSessionOpts(uint32 fd,uint16 optcode,uint32 session_fd = 0);
	void RegSessionOpts();

	uint32 Get_Connection_ID(){return m_connection_id;}
	EAppStatus GetStatus(){	return m_status;}

	//产生回调
	EventMgr::EventID AddCallback(std::function<void(bool)> func);
	//触发回调
	void DispatchCallback(EventMgr::EventID id, bool result);
	//加密世界战斗结果
	string Md5WorldWarResult(string guid, uint32 type1, uint32 type2, uint32 result, uint32 data1, uint32 data2, uint32 data3, uint32 data4, time_t t);
public:
	uint32 m_open_time;						//服务器启动时间
	uint32 m_tick_timers;					//心跳次数
	InternalCallback<packet&> *m_inter_cb;

	vector<uint16> m_opts;					//游戏服需要的协议号
	vector<uint16> m_pk_opts;					//pk服需要的协议号
protected:
	uint32 WORLD_SLEEP_CONST_;
	//应用程序命令行参数
	SvrCoreParams& m_params;
	//应用程序配置信息
	SvrCoreConfig& m_config;
	//服务器状态
	EAppStatus m_status;

	//日志
	log_holder *m_logger;	
	//网关服连接
	tcp_connection *m_netgd_connection;
	//中心服连接
	tcp_connection *m_centd_connection;

	//当前服务器的连接ID
	int m_connection_id;

	//准备发往网关服的数据
	//list_t m_to_send_list;
	//收到的客户端数据包队列
	list_t m_recv_pkt_list;
	netgd_handle_ptr m_netgd_request_map[MAX_INTERNAL_OPT * 2];

	Command_Handler_Map m_command_hanlders;

	inline reactor *get_reactor()
	{
		ASSERT(m_rea);
		return m_rea;
	}

	void SetStatus(EAppStatus status)
	{
		out_str(m_logger, "set server status, from %u to %u", m_status, status);
		m_status = status;
	}

	void UpdateTimer(uint32 diff);
	reactor *m_rea;		

	static void sig_handler(int sig);
	uint32 m_gid;
	uint32 m_sid;
	uint32 m_oldsid;

	AC_AUTOMATA *m_aca;					//屏蔽词组件
	char *m_fuck_pingbi;				//所有屏蔽词
	TimerHolderArr m_timerHolder;		//分钟定时器回触发器
	TimerHolderArr m_timerHolder_toadd;	//待加入的定时器
	TimeTracker m_callback_timer;		//每分钟触发一次
	int m_signal_no ;		

	uint32 m_pingpont;					//下次发ping包的时间

	EventMgr m_event_mgr;				//回调管理器
};

template<class T>
class SvrCoreApp:public SvrCoreAppImpl
{
public:
	typedef SvrCoreApp<T> base;
	SvrCoreApp(SvrCoreParams& params,SvrCoreConfig& config);

	virtual bool Open();

	//////////////////////////////////////////////////////////////////////////
	//信号	
	static void HookSignals();
	static void UnHookSinals();
	static void OnSignal(int s);	

	static T *g_app;

	static void SendToCentd(WorldPacket& pkt)
	{
		((SvrCoreAppImpl*)(g_app))->SendToCentd(pkt.GetPktPtr());
	}
protected:
	virtual SvrObjectManager *GetObjMgr() = 0;
protected:
	static int netgd_handle_accept(struct _io_watcher_t *ev,EV_FLAGS flag);
	static int netgd_handle_close(io_watcher* ev,EV_FLAGS flag);				//当网关服关闭的时候
	static int centd_handle_close(io_watcher* ev,EV_FLAGS flag);				//当中心服关闭的时候
	static int netgd_handle_request(tcp_connection *,server_packet *pkt);	

	//////////////////////////////////////////////////////////////////////////
	//网关服协议处理函数	
	static int on_netgd_lua(tcp_connection *,server_packet *);
	static int on_netgd_packet_gate(tcp_connection *,server_packet *pkt);
	static int on_netgd_packet_gate_list(tcp_connection *,server_packet *pkt);	
	static int on_netgd_reg_server(tcp_connection *, server_packet *pkt);
	static int on_netgd_ping_pong(tcp_connection *, server_packet *pkt);
	static int on_commands(tcp_connection *, server_packet *pkt);
	static int on_splice(tcp_connection *, server_packet *pkt);
	static int on_call_back_result(tcp_connection *, server_packet *pkt);

	static int on_print_object(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_memory_recovery(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
};

void SendCommandResult(tcp_connection *conn, uint32 connectid, const char *result,...);

#include <server/svrcore_app.h.inl>


#endif
