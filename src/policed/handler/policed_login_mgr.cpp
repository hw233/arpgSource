#include "policed_app.h"
#include "policed_login_mgr.h"
#include "policed_context.h"
#include <object/OprateResult.h>

class PolicedLoginActionLogin : public LoginActionBase
{
public:
	PolicedLoginActionLogin(uint32 fd, const string guid) : LoginActionBase(fd, guid), m_add_watch(false)
	{

	}
	~PolicedLoginActionLogin(){}
public:
	bool Update(uint32 diff)
	{
		//还没发送监听，那么监听吧。。。
		if(!m_add_watch)
		{
			ASSERT(!ObjMgr.Get(m_guid));
			ObjMgr.CallAddWatch(m_guid);
			m_add_watch = true;
			return true;
		}

		//等待对象的到来
		auto player = ObjMgr.FindPlayer(m_guid);
		if(!player)
			return true;

		player->Login(m_fd);
		return false;
	}
	EContextLoginAction GetType()
	{
		return CONTEXT_LOGIN_ACTION_LOGIN;
	}

private:
	bool m_add_watch;	//是否已经发送请求监听
};

class PolicedLoginActionClose : public LoginActionBase
{
public:
	PolicedLoginActionClose(uint32 fd, const string guid) : LoginActionBase(fd, guid), m_del_watch(false)
	{

	}
	~PolicedLoginActionClose(){}
public:
	bool Update(uint32 diff)
	{
		if(!m_del_watch)
		{
			PolicedContext *player = ObjMgr.FindPlayer(m_guid);
			if(!player)
				return false;
			
			player->Logout();
			m_del_watch = true;
			return true;
		}

		return ObjMgr.Get(m_guid) != nullptr;
	}
	EContextLoginAction GetType()
	{
		return CONTEXT_LOGIN_ACTION_CLOSE;
	}
private:
	bool m_del_watch;
};

void PolicedLoginMgr::PushAction(const string guid, uint32 fd, EContextLoginAction type)
{
	tea_pdebug("PolicedLoginMgr::PushAction %s %u %u", guid.c_str(), fd, type);
	LoginActionBase *action = nullptr;
	switch (type)
	{
	case CONTEXT_LOGIN_ACTION_LOGIN:
		{
			action = new PolicedLoginActionLogin(fd, guid);
		}
		break;
	case CONTEXT_LOGIN_ACTION_CLOSE:
		{
			action = new PolicedLoginActionClose(fd, guid);
		}
		break;
	default:
		break;
	}
	ASSERT(action);
	ASSERT(!guid.empty());
	m_action[guid].push(action);
}
