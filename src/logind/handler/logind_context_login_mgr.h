#ifndef _LOGIND_CONTEXT_LOGIN_QUEUE_H_
#define _LOGIND_CONTEXT_LOGIN_QUEUE_H_

#include <shared/context_login_mgr.h>

class LogindContext;

class LogindContextLoginMgr : public ContextLoginMgr
{
public:
	void PushAction(const string guid, uint32 fd, EContextLoginAction type);

	void AddOfflineContext(CommonContext *session);
	CommonContext *FindOfflineContext(const string &guid);
	void RemoveOfflineContext(CommonContext *session);
protected:
	vector<CommonContext*> m_offline_context;
};

#endif

