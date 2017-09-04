#include "context_login_mgr.h"
#include "server/svr_context_manager.h"

ContextLoginMgr::ContextLoginMgr()
{

}

ContextLoginMgr::~ContextLoginMgr()
{

}

void ContextLoginMgr::Update(uint32 diff)
{
	for (auto it = m_action.begin(); it != m_action.end();)
	{
		auto action = it->second.front();
		ASSERT(action);
		if(!action->Update(diff))
		{
			//printf("ContextLoginMgr::Update %s %u %u \n", action->m_guid.c_str(), action->m_fd, action->GetType());
			safe_delete(action);
			it->second.pop();
			if(it->second.empty())
			{
				it = m_action.erase(it);
				continue;
			}
		}
		++it;
	}
}
