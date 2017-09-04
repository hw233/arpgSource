#ifndef _LOGD_CONTEXT_H_
#define _LOGD_CONTEXT_H_

#include "policed_app.h"
#include <object/PlayerBase.h>

typedef struct account_info_t account_info;

class PolicedContext:public SvrContext,public PlayerBase
{
public:
	PolicedContext(ContextManager *mgr);
	virtual ~PolicedContext(){};

	//函数路由,初始化
	void OpcodeHandlerInitor();	

	void Login(uint32 fd);
	void Logout();
	void SetServerId();

	const string &GetServerId(){return m_sid;}
	const string &GetPlatformId(){return m_pid;}
public:
	account_info *a_info;
	string ip;			//玩家IP
	uint32 uip;				//玩家IP，数字类型
public:
	void Handle_write_client_log(packet& pkt);
	void Handle_Player_Gag(packet& pkt);				//禁言
private:
	string m_sid;		//服务器id
	string m_pid;		//平台id
};

#endif
