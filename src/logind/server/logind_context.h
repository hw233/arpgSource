#ifndef _CENTER_SESSION_H_
#define _CENTER_SESSION_H_

#include <comm/common_stl.h>
#include <net/netstd.h>
#include <protocol/external.h>
#include "logind_player.h"
#include "mobile_context.h"


class LogindMongoDB;

typedef map<string,uint32> SessionMap;
typedef std::deque<uint32> SessionQue;
//////////////////////////////////////////////////////////////////////////
//
class LogindContext : public CommonContext
{
public:
	//函数路由,初始化
	void OpcodeHandlerInitor();

	//账号-Session列表
	static SessionMap SessionMaps;
	//正在排队的玩家列表
	static SessionQue SessionQues;

	static inline LogindContext* FindContext(uint32 fd)
	{
		return static_cast<LogindContext*>(LogindApp::g_app->FindContext(fd));
	}

	static inline logind_player* FindPlayer(const string &lguid)
	{
		return dynamic_cast<logind_player*>(ObjMgr.Get(lguid));
	}
	static uint32 FindSessionID(const string &account);	
	static LogindContext* FindContextByGuid(string guid);		//通过guid查找页游会话
	static uint32 LoginPlayerCount();
public:
	LogindContext(int fd,ContextManager *mgr);
	virtual ~LogindContext();
	//重置下跨服信息
	void ResetKuafuInfo();
	void Update(uint32 diff);
	//尝试关闭该会话信息,需要同步信息等等
	void OnClosed();
	//登录服崩溃以后重新初始化
	void CollapseReset(logind_player *player);

	//异步初始化玩家信息
	void PlayerAfterLoadData(logind_player *player,vector<GuidObject *>vec,uint32 fd);
	//按充值总数设置RMB等级
	void SetPayLevel();
	void SetReceiveMatchInfoFlag(bool flag){m_bHasReceiveMatchInfo = flag;}
	bool GetReceiveMatchInfoFlag(){return m_bHasReceiveMatchInfo;}
	//保存到数据库
	void SaveToDB(bool fsync = false);
public:
	void Handle_Foeced_Into(packet& pkt);				//插入

	void Handle_Get_Chars_List(packet& pkt);			// 获取角色列表

	void Handle_Char_Create(packet& pkt);				// 角色创建
	void Handle_Char_Update_Info(packet& pkt);			// 角色信息变更
	void Handle_Delete_Char(packet& pkt);				// 角色删除
	void Handle_Player_Login(packet& pkt);				// 登陆
	void Handle_Player_Logout(packet& pkt);				// 小退
	void Handle_Change_Line(packet& pkt);				// 换线
	void Handle_Show_Line(packet& pkt);					// 查询分线信息
	void Handle_Regularise_Account(packet& pkt);		//非正式用户转成正式用户

	void Handle_Player_Kicking(packet& pkt);			//踢人

	void Handle_GoBackTo_GameServer(packet& pkt);		//客户端请求回传到游戏服
	void Hanlde_pk_login(packet &pkt);					//战斗服登录
	void Handle_World_War_CS_Player_Info(packet& pkt);	//请求打包比赛数据
	void Call_World_War_CS_Player_Info(const string &server_ip, uint32 port, const string &sessionkey);				//发送玩家打包数据给客户端
	void Hanlde_Modify_Watch(packet& pkt);				//修改监听
	void Hanlde_Kuafu_Enter(packet& pkt);				//进入跨服协议
public:
	// 获取sessionKey对象
	bool Get_Session(std::map<string,string>& querys);
	void LoginOK();					//登录完成，进行最后操作
	void onSetInvitedTeleportInfo(const string& factionId, uint32 mapid, uint32 x, uint32 y, const string& generalId);
	void onRealLoginOk(const string& factionId);	// 验证是否加入帮派后的实际登录
	void Call_Create_Connection();
	bool Check_Name(string& name, string& realName, string& prefix);				// 检查名称
	void Char_Name_Update(string &name);							// 更新名字
private:
	bool m_is_save_login_log;		//是否已经保存过登录记录
	uint32 m_is_canInto;			//是否可以顶人了
	bool m_is_hosting;				//是否是托管登录
	bool m_bHasReceiveMatchInfo;	//是否已收到跨服匹配信息
	bool m_bHasplatdata;			//登录sessionkey中是否携带平台信息
	bool m_bHasfcm;					//登录sessionkey中是否携带防沉迷信息
	bool m_isFcm;					//防沉迷
private:
	string m_watcher_guid;			//观察者guid
	string m_general_id;			//地图实例对应唯一id
	
private:
	void SavePlayerLoginLog();	//保存玩家登录记录
	void SavePlayerLogoutLog();	//保存玩家登出记录
	bool SendToScenedAddMapWatcher();//发内部协议到场景服,给这个连接下发对应的地图数据等
public:
	bool GoBackToGameSvr();		//把玩家由pk服回到游戏服
public:
	//传送参数
	uint32  to_mapid,to_instanceid,to_line_no;
	string to_teleport_id;
};

#endif
