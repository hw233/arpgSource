#ifndef _ROBOTD_CONTEXT_H_
#define _ROBOTD_CONTEXT_H_

#include "robotd_app.h"
#include "robotd_conn.h"
#include "ProtocolUtil.h"
#include <stack>
#include <object/SharedDef.h>
#include <object/PlayerBase.h>


class BinLogObject;
class RobotdObjectManager;
class RobotdGameConnection;
class RobotdUnit;

//应用服会话 上下文信息
class RobotdContext : public PlayerBase, public ProtocolUtil
{
public:
	RobotdContext(const string &pid, const string &sid, const string &uid, const string &remote_ip, const string &login_key, const string &conn_host, uint32 conn_port, const string &params);
	virtual ~RobotdContext();

	RobotdGameConnection *GetConn();
	
	void Close()
	{
		if(conn_)
			conn_->Close();
	}
	uint32 GetFD()
	{
		if(!conn_)
			return 0;
		return conn_->GetFD();
	}
public:
	void OnGameServerLogin(const string& login_host, uint32 login_port);
	void Update(uint32 diff);
	void Reset();
	void OnClose();
	void OnWarClose();
public:
	RobotdUnit *GetMyUnit(){return unit_;}
	void SetMyUnit(RobotdUnit *u);
	void OnAfterUpdate(SyncEventRecorder *data,int flags,UpdateMask& mask,UpdateMask& str_mask);	
	string MakeSessionKey(bool indulge, bool mobile = false);
public:
	void Handle_Lua(uint16 optcode, PacketType &pkt);
	void Handle_Ud_Object(uint16 optcode, PacketType &pkt);
	void Handle_Chars_List(uint16 optcode, PacketType &pkt);
	void Handle_Grid_Unit_Move(uint16 optcode, PacketType &pkt);						//Unit移动
	void Handle_Unit_Move_Stop(uint16 optcode, PacketType &pkt);						//Unit move stop
	void Handle_Operation_failed(uint16 optcode, PacketType &pkt);
	void Handle_Grid_Ud_Object(uint16 optcode, PacketType &pkt);						//grid更新精灵
	void Handle_Sync_Mstime(uint16 optcode, PacketType &pkt);							//场景服务同步服务器时间
	void Handle_Merge_Server_Msg(uint16 optcode, PacketType &pkt);						//合服通知
	void Handle_Map_Info_Update(uint16 optcode, PacketType &pkt);						//地图对象更新
	void Handle_join_or_leave_server(uint16 optcode, PacketType &bytes);				//加入获取离开否逻辑服务器
	void Handle_Fighting_Info_Update_Object(uint16 optcode, PacketType &bytes);			//战斗技能包
	void Handle_Fighting_Info_Update_Object_2(uint16 optcode, PacketType &bytes);		//战斗技能包
	void Handle_World_War_SC_Player_info(uint16 optcode, PacketType &bytes);			//跨服玩家数据
	void Handle_Strength_Success(uint16 optcode, PacketType &bytes);
	void Handle_World3v3_WaitInfo(uint16 optcode, PacketType &bytes);					//3v3准备列表

public:
	void OnRobotRespawn();

public:
	const string &GetPlatformId() const {return m_pid;}
	const string &GetServerId() const {return m_sid;}
	const string &GetUserId() const {return m_uid;}
	const string &GetRemoteIP() const {return m_remote_ip;}
	string GetAccount(){return m_pid + '_' + m_sid + '_' + m_uid;}
	bool IsLoginOk();
	const string &GetConnHost(){return conn_->GetConnHost();}
	uint32 GetConnPort(){return conn_->GetConnPort();}
	bool GetSwitch(uint8 type)
	{
		ASSERT(type < 32);
		return GET_BIT(m_action_switch, type);
	}
	int send_world_war_SC_player_info___ ();
public:
	RobotdObjectManager	*m_obj_mgr;		//对象管理器
/*
	uint32 m_updatemask_time[65535];
	uint32 m_updatemask_time_str[65535];*/

private:
	//游戏服tcp连接
	RobotdGameConnection *conn_;

	//战斗服tcp连接
	RobotdWarConnection *war_conn_;
	//跨服登录的玩家信息
	ByteArray m_war_bytes;

	RobotdUnit *unit_;

	const string m_pid;								//平台id
	const string m_sid;								//服务器id
	const string m_uid;								//机器人登录帐号id
	const string m_remote_ip;						//session key中带的remote_ip信息
	const string m_login_key;						//游戏登录时，使用的秘钥

	string m_params;								//机器人参数
	uint32 m_action_switch;							//机器人功能开关

public:
	static int LuaClose(lua_State *scriptL);
	static int LuaIsLoginOk(lua_State *scriptL);
};

#endif
