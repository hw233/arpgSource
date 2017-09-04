#ifndef _COMMON_CONTEXT_H_
#define _COMMON_CONTEXT_H_

#include <comm/common_stl.h>
#include <net/netstd.h>
#include <protocol/external.h>
#include <server/svr_context_manager.h>

class logind_player;
namespace core_obj{ class GuidObject; }
using core_obj::GuidObject;

//页游手游会话的抽象类
class CommonContext : public SvrContext
{
public:
	CommonContext(uint32 fd,ContextManager *mgr,int type = 0/*CONTEXT_TYPE_YEYOU*/);
	virtual ~CommonContext();

public:
	static CommonContext* FindContext(uint32 fd);
	uint8 GetContextType(){return m_context_type;}
	virtual void OnClosed() = 0;										//会话关闭
	virtual void CollapseReset(logind_player *player) = 0;				//登录服崩溃以后重新初始化
	virtual void Call_Create_Connection() = 0;							//通知其他服建立连接
	
	bool Get_Server_List(string login_server);							//验证登录服务器是否正确
	bool IsKuafuPlayer()
	{
		//是否跨服玩家
		if (!m_backsvrname.empty())
			return true;
		else
			return false;
	}

	const string &GetFromServerName()const {return m_from_server_name;}
	bool SetPlayer(logind_player *player);
	logind_player *GetPlayer(){return m_player;}
	const string &GetGuid()const{return m_lguid;}
	const string &GetAccount()const{return m_account;}
	//初始化玩家信息
	virtual void PlayerLoadData();
	void PutData();
	virtual void LoginOK() = 0;
private:
	uint8 m_context_type;			//会话类型
protected:
	vector<GuidObject*> m_temp_vec;	//登录过程中的临时数组
	logind_player *m_player;		//关联的玩家对象
	string m_account;				//账号
	string m_from_server_name;		//来自服务器
	string m_backsvrname;			//要返回的游戏服server_name	
	string m_kuafu_svrname;			//分配的战斗服server_name	
	uint32 m_kuafu_reserve2;		//跨服保留参数（具体用法根据需求来定）
public:
	uint32 m_warid;					//场次id
	uint8 m_kuafutype;				//跨服类型
	string m_remote_ip;
	uint16 m_remote_port;	
	uint32 m_number;				//跨服玩家编号
	uint32 m_kuafu_reserve;			//跨服保留参数（具体用法根据需求来定）
	string m_kuafu_reserve_str;		//跨服字符串预留
};

#endif // !_COMMON_CONTEXT_H_


