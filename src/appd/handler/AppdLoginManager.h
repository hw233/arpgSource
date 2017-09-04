#ifndef _APPD_LOGIN_MANAGER_H_
#define _APPD_LOGIN_MANAGER_H_

#include <shared/context_login_mgr.h>

class AppdLoginMgr : public ContextLoginMgr
{
public:
	void PushAction(const string guid, uint32 fd, EContextLoginAction type);
	void InsertCloseAction(const string guid);
};

#endif

