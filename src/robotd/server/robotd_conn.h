#ifndef _ROBOTD_CONNECTION_H_
#define _ROBOTD_CONNECTION_H_

#include "robotd_app.h"

class RobotdGameConnection;
class RobotdContext;

#define PACKET_HEAD_FIRST_PART_SIZE sizeof(uint16_t)

#define parser_head_len(data)\
	*((uint16_t*)data)

#define to_uint8_t(content)\
	*((uint8_t*)content)

#define parser_websocket_packet_len_flag(data)\
	to_uint8_t(data + 1) & 0x7F

#define parser_websocket_packet_opcode(data)\
	to_uint8_t(data) & 0xF

#define parser_websocket_mask(data)\
	to_uint8_t(data + 1) >> 7

#define parser_websocket_byte(data, offset)\
	to_uint8_t(data + offset)

#define parser_websocket_mask_info_by_offset(data, mask_offset, mask_array_offset)\
	parser_websocket_byte(data, mask_offset + mask_array_offset)

//机器人登录状态
enum RobotLoginState
{
	ROBOT_LOGIN_STATE_NONE,						//无状态
	ROBOT_LOGIN_STATE_CONN_SERVER_COUNTDOWN,	//连接服务器倒计时中
	ROBOT_LOGIN_STATE_CONN_SERVER_FAIL,			//连接服务器出错
	ROBOT_LOGIN_STATE_CONN_SERVER_OK,			//连接服务器成功
	ROBOT_LOGIN_STATE_SHAKING_HAND,				//握手中
	ROBOT_LOGIN_STATE_SHAKE_HAND_END,			//握手结束
	ROBOT_LOGIN_STATE_GETSESSION,				//正在Get Session
	ROBOT_LOGIN_STATE_NOT_CHARS,				//Get Session已返回，当前未建角色
	ROBOT_LOGIN_STATE_CREATE_CHARS,				//创建角色中
	ROBOT_LOGIN_STATE_ERR_NAME,					//创建角色名字异常
	ROBOT_LOGIN_STATE_HAS_CHARS,				//Get Session 或者 创建橘色 已返回，当前已有角色
	ROBOT_LOGIN_STATE_LOGIN,					//角色登录中
	ROBOT_LOGIN_STATE_LOGIN_DATA_HAS,			//已收到玩家数据
	ROBOT_LOGIN_STATE_LOGIN_UNIT_HAS,			//已收到精灵数据
	ROBOT_LOGIN_STATE_LOGIN_SUCCEED,			//登录完毕
	ROBOT_LOGIN_STATE_FORCED_INTO,				//强制踢在线中
	ROBOT_LOGIN_STATE_WAIT_CLOSE,				//关闭中
	ROBOT_LOGIN_STATE_FAILURE,					//登录失败次数太多，不再登录
	ROBOT_LOGIN_STATE_GAIMIN,					//改名中
	MAX_ROBOT_LOGIN_STATE
};


class RobotdConnection : public core_socket::TcpConnection
{
public:
	RobotdConnection(RobotdGameConnection *owner):m_owner(owner),m_open(false),isShakeHand(true)
	{
		ASSERT(m_owner);
	}

	virtual ~RobotdConnection(){}
public:
	uint32 GetFD(){return fd_;}
	RobotdGameConnection *GetOwner(){return m_owner;}
	void UnsetOwner(){m_owner = nullptr;}
	void SetOpen(bool b){m_open = b;}
	bool IsOpen()const{return m_open;}
	
	void SendFirstPacket(const char* str, int length){
		send_buf_.write(str, length);
	} 

	virtual void SendPacket(PacketType& pkt);

	//传入数据,判断是否已成为一个完整包，如果已经完成，
	//那么调用包处理函数，并根据返回值修改连接的当前数据包
	//返回还剩下多少的字节数未读
	virtual int ComplatePacket(uint8_t *buf,int len);
	
	virtual void ReadPacket();

	// 完整消息头的长度
	int get_real_head_len(PacketType *cur_pkt_);

	//获得消息的长度
	int get_packet_payload(PacketType *cur_pkt_);

protected:
	virtual void OnClose();

public :
	bool isShakeHand;
protected:
	RobotdGameConnection *m_owner;
	bool m_open;
};

class RobotdGameConnection
{
	struct OpcodeHandler
	{
		char 	const* name;
		void 	(RobotdContext::*handler)(uint16,PacketType&);
		OpcodeHandler():name(0),handler(0)
		{};		
	};
public:
	static void SendFirstShakeProtocol(RobotdConnection *conn_);
	RobotdGameConnection(RobotdContext *context, const string &conn_host, int conn_port)
		:m_context(context), conn_(nullptr), m_conn_host(conn_host), m_conn_port(conn_port)
		,m_login_state(ROBOT_LOGIN_STATE_NONE), m_close_count(0), closed(false)
	{
		OpcodeHandlerInitor();

		m_timer_wait_unit.SetInterval(2000);
		m_timer_login.SetInterval(30000);
		m_timer_getSession.SetInterval(30000);
		m_timer_has_player_data.SetInterval(30000);
		m_timer_forced_info.SetInterval(2000);
		m_timer_clear_close_count.SetInterval(1000);
		m_timer_GaiMing.SetInterval(3000);
	}

	~RobotdGameConnection()
	{
		//Close();
	}
public:
	const string &GetConnHost(){return m_conn_host;}
	uint32 GetConnPort(){return m_conn_port;}
	int GetFD()
	{
		if(conn_)
			return conn_->GetFD();
		return 0;
	}
	uint16 GetLoginState(){return m_login_state;}
	void SetLoginState(uint16 val);

	bool ConnToNetgd();

	void Close();
	virtual void OnClose();
	virtual bool LoginUpdate(uint32 diff);
	void OnConnToWar();
	void OnFreeWar();
protected:
	virtual void OpcodeHandlerInitor();
	virtual void CreateConn();
private:
	uint32 m_close_count;					//网络断开连接次数
	bool closed;
protected:
	RobotdContext *m_context;
	//函数路由表
	OpcodeHandler opcode_handler_[NUM_MSG_TYPES + 1];

	IntervalTimer m_timer_login;			//登录定时器
	IntervalTimer m_timer_getSession;		//getsession定时器
	IntervalTimer m_timer_has_player_data;	//取玩家数据定时器
	IntervalTimer m_timer_forced_info;		//踢人定时器
	IntervalTimer m_timer_wait_unit;		//精灵等待定时器
	IntervalTimer m_timer_clear_close_count;//清空网络断开次数定时器
	IntervalTimer m_timer_GaiMing;			//改名定时器
	//连接和登录相关
	string m_conn_host;				//游戏服务器ip
	int m_conn_port;				//游戏服务器端口
public:
	RobotdConnection *conn_;

	

	RobotLoginState m_login_state;			//登录状态
};

class RobotdWarConnection : public RobotdGameConnection
{
public:
	RobotdWarConnection(RobotdContext *context, const string &conn_host, int conn_port, const string &session_str)
		:RobotdGameConnection(context, conn_host, conn_port), m_session_str(session_str)
	{

	}
public:
	void OnClose();
	bool LoginUpdate(uint32 diff);
protected:
	void OpcodeHandlerInitor();
	void CreateConn();
public:
	string m_session_str;			//跨服传送时指定的get session串
};

#endif
