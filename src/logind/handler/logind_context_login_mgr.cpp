#include "logind_app.h"
#include "logind_context_login_mgr.h"
#include "common_context.h"

class LoginActionLogin : public LoginActionBase
{
public:
	LoginActionLogin(uint32 fd, const string guid) : LoginActionBase(fd, guid)
	{

	}
	~LoginActionLogin(){}
public:
	bool Update(uint32 diff)
	{
		auto context = CommonContext::FindContext(m_fd);
		if(!context)
		{
			tea_pdebug("LoginActionLogin::Update player not found, %s %u %u", m_guid.c_str(), m_fd, GetType());
			return false;	//玩家下线了
		}

		//释放数据中，请等待
		if(LogindApp::g_app->m_removing_player.find(context->GetGuid()) != LogindApp::g_app->m_removing_player.end())
			return true;

		switch (context->GetStatus())
		{
		case STATUS_AUTHED:
			context->PlayerLoadData();
			return true;
		case STATUS_TRANSFER:
			return true;//已经在load数据了,等着吧
		case STATUS_TRANSFER2:
			context->PutData();
			return true;
		case STATUS_PUT:
			return true;
		case STATUS_PUT_OK:
			context->LoginOK();
			return false;
		default:
			ASSERT(false);
			return true;
		}
	}
	EContextLoginAction GetType()
	{
		return CONTEXT_LOGIN_ACTION_LOGIN;
	}
};

class LoginActionClose : public LoginActionBase
{
public:
	LoginActionClose(uint32 fd, const string guid) : LoginActionBase(fd, guid), m_close(false)
	{

	}
	~LoginActionClose(){}
public:
	bool Update(uint32 diff)
	{
		if(m_close)
		{
			//能进这里，肯定是pk服
			ASSERT(LogindApp::g_app->IsPKServer());
			return ObjMgr.Get(m_guid) != nullptr;
		}

		CommonContext *context = nullptr;
		if(m_fd)
		{
			context = CommonContext::FindContext(m_fd);
			if(!context)
			{
				tea_pdebug("LoginActionClose::Update player not found, %s %u %u", m_guid.c_str(), m_fd, GetType());
				return false;	//玩家下线了
			}
		}
		else
		{
			context = LogindApp::g_app->m_login_queue->FindOfflineContext(m_guid);
			LogindApp::g_app->m_login_queue->RemoveOfflineContext(context);
		}
		context->OnClosed();
		safe_delete(context);

		//如果是pk服，必须等到对象缓存被释放才能完成下线
		if(LogindApp::g_app->IsPKServer())
		{
			m_close = true;
			return true;
		}
		return false;
	}
	EContextLoginAction GetType()
	{
		return CONTEXT_LOGIN_ACTION_CLOSE;
	}
private:
	bool m_close;
};

void LogindContextLoginMgr::PushAction(const string guid, uint32 fd,  EContextLoginAction type)
{
	tea_pdebug("LogindContextLoginMgr::PushAction %s %u %u", guid.c_str(), fd, type);
	CommonContext *session = nullptr;
	if(fd)	
		session = CommonContext::FindContext(fd);
	else
		session = FindOfflineContext(guid);
	ASSERT(session);
	if(session->GetAccount().empty())
	{
		tea_pinfo("LogindContextLoginMgr session account is nil");
		return;
	}

	LoginActionBase *action = nullptr;
	switch (type)
	{
	case CONTEXT_LOGIN_ACTION_LOGIN:
		{
			action = new LoginActionLogin(session->GetFD(), session->GetAccount());
		}
		break;
	case CONTEXT_LOGIN_ACTION_CLOSE:
		{
			action = new LoginActionClose(session->GetFD(), session->GetGuid());
		}
		break;
	default:
		break;
	}
	ASSERT(action);
	m_action[session->GetAccount()].push(action);
}

void LogindContextLoginMgr::AddOfflineContext(CommonContext *session)
{
	ASSERT(session);
	m_offline_context.push_back(session);
}

CommonContext *LogindContextLoginMgr::FindOfflineContext(const string &guid)
{
	for (auto session:m_offline_context)
	{
		if(session->GetGuid() == guid)
			return session;
	}
	ASSERT(false);
	return nullptr;
}

void LogindContextLoginMgr::RemoveOfflineContext(CommonContext *session)
{
	for (auto it = m_offline_context.begin(); it != m_offline_context.end(); ++it)
	{
		if(*it == session)
		{
			m_offline_context.erase(it);
			return;
		}
	}
	ASSERT(false);
}

