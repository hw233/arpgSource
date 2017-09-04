#ifndef _MOBILE_CONTEXT_H_
#define _MOBILE_CONTEXT_H_

#include <comm/common_stl.h>
#include <net/netstd.h>
#include <protocol/external.h>
#include "common_context.h"
#include "logind_app.h"
#include "logind_mongodb.h"

typedef map<string,uint32> MobileSessionMap;

class MobileContext : public CommonContext
{
public:
	//账号-fd
	static MobileSessionMap MobileSessionMaps;
	//回调第一个参数为guid,第二个是ip
	static void ForEachMobileContext(std::function<void(std::string, std::string)> func);	//遍历所有手机会话	
	static void ForEach(std::function<void(MobileContext*)> func);				//遍历所有手机会话	

	static MobileContext* FindContextByGuid(string guid);		//通过guid查找手游会话	
	static inline MobileContext* FindContext(uint32 fd)
	{
		return static_cast<MobileContext*>(LogindApp::g_app->FindContext(fd));
	}

public:
	MobileContext(uint32 fd,ContextManager *mgr);
	virtual ~MobileContext();

	//函数路由,初始化
	void OpcodeHandlerInitor();

	void Update(uint32)
	{

	};
	const string &GetGuid()const{return m_lguid;}
	const string &GetAccount()const{return m_account;}
	
	void Get_Session(std::map<string,string>& querys);
	static uint32 FindSessionID(const string &account);
	
	void OnClosed();
	//登录服崩溃以后重新初始化
	void CollapseReset(logind_player *player);
	void Call_Create_Connection();
	void LoginOK();
};

#endif // !_MOBILE_CONTEXT_H_


