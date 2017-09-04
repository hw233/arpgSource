#include "robotd_object_manager.h"
#include "robotd_context.h"
#include "robotd_unit.h"
#include <object/GuidManager.h>

extern void DoOnAttachObject(const string &account, GuidObject *obj);
extern void DoOnAfterAttachObject(const string &account, GuidObject *obj);
extern void DoOnReleaseObject(const string &account, GuidObject *obj);

//////////////////////////////////////////////////////////////////////////
//for RobotdObjectManager
RobotdObjectManager::RobotdObjectManager(SvrCoreAppImpl *app, RobotdContext *owner)
	:SvrObjectManager(app), m_owner(owner)
{
	//整形
	hashGUID_ = [this](const string& guid)->uint32_t{
		if(GuidManager::GetPrefix(guid) == ObjectTypeUnit || GuidManager::GetPrefix(guid) == ObjectTypeGridLoot)
		{
			stringstream ss;
			ss << SUB_GUID_STR_TO_INT(guid);
			uint32_t code = 0;
			ss >> code;
			//string gg = GetByUGUID(code);
			//tea_pinfo("hashGUID_ %s %s %u %s", this->m_owner->GetAccount().c_str(), guid.c_str(), code, gg.c_str());
			//ASSERT(gg.empty() || gg == guid);
			return code;
		} else {
			return 0;
		}
	};
}

RobotdObjectManager::~RobotdObjectManager()
{
	for (auto it = objs_.begin(); it != objs_.end(); ++it)	
	{
		if(it->second && it->second != m_owner) 
		{
			//tea_pdebug("RobotdObjectManager::~RobotdObjectManager delete %s %s %p", m_owner->GetAccount().c_str(), it->second->guid().c_str(), it->second);
			DoOnReleaseObject(m_owner->GetAccount(), it->second);
			delete it->second;
		}
	}
	objs_.clear();
}

//对象工厂
GuidObject *RobotdObjectManager::ObjectFactor(const string &guid)
{
	BinLogObject *ptr = NULL;
	switch (GuidManager::GetPrefix(guid))
	{
	case ObjectTypePlayer:
		ptr = m_owner;
		break;
	case ObjectTypeUnit:
		ptr = new RobotdUnit(m_owner);
		break;
	default:
		ptr = new BinLogObject;
		break;
	}
	return ptr;
}

GuidObject* RobotdObjectManager::CreateObject(string guid)
{
	//tea_pdebug("RobotdObjectManager::CreateObject %s %s", m_owner->GetAccount().c_str(), guid.c_str());
	GuidObject *ptr = ObjectFactor(guid);

	
	switch (GuidManager::GetPrefix(guid))
	{
	//case ObjectTypePlayer:
	//	ptr->after_update(std::bind(&RobotdContext::OnAfterUpdate,m_owner,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
	//	break;
	case ObjectTypeUnit:
		{
			RobotdUnit *unit = dynamic_cast<RobotdUnit*>(ptr);
			ASSERT(unit);
			unit->after_update(std::bind(&RobotdUnit::OnAfterUpdate,unit,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
			m_all_unit.insert(unit);
		}
		break;
	default:
		ptr->after_update(std::bind(&RobotdContext::OnAfterUpdate,m_owner,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
		break;
	}


	ptr->guid(guid);
	AttachObject(ptr);
	
	return ptr;
}

void RobotdObjectManager::AfterAttachObject(GuidObject *obj)
{
	DoOnAfterAttachObject(m_owner->GetAccount(), obj);
	string guid = obj->guid();
	if(GuidManager::GetPrefix(guid) == ObjectTypeUnit && guid.find('p') >= 0)
	{
		auto unit = dynamic_cast<RobotdUnit*>(obj);
		m_all_player_unit.insert(unit);

		auto guid_size = guid.size();
		auto owner_guid_size = m_owner->guid().size();
		if(guid_size > owner_guid_size
			&& guid.compare(guid_size - owner_guid_size, guid_size, m_owner->guid()) == 0)
		{
			if(m_owner->GetConn()->GetLoginState() < ROBOT_LOGIN_STATE_LOGIN_UNIT_HAS)
				m_owner->GetConn()->SetLoginState(ROBOT_LOGIN_STATE_LOGIN_UNIT_HAS);

			m_owner->SetMyUnit(unit);
		}
	}
}

void RobotdObjectManager::AttachObject(GuidObject* obj)
{
	InitNoticeInfo(obj);
	DoOnAttachObject(m_owner->GetAccount(), obj);
	return GuidObjectTable::AttachObject(obj);
}

void RobotdObjectManager::ReleaseObject(string guid)
{
	//tea_pdebug("RobotdObjectManager::ReleaseObject %s %s", m_owner->GetAccount().c_str(), guid.c_str());
	GuidObject *obj = Get(guid);
	if(obj)
	{
		auto unit = dynamic_cast<RobotdUnit*>(obj);

		if(unit)
		{
			if(unit == m_owner->GetMyUnit())
				m_owner->SetMyUnit(nullptr);
			m_all_player_unit.erase(unit);
			m_all_unit.erase(unit);
		}
		else
		{
			if(GuidManager::GetPrefix(guid) == ObjectTypeMap)
			{
				vector<string> guids;
				for (auto it:m_all_unit)
				{
					if(it->GetInstanceId() == obj->GetUInt32(MAP_INT_FIELD_INSTANCE_ID))
						guids.push_back(it->guid());
				}
				for (auto it:guids)
				{
					this->ReleaseObject(it);
					tea_pdebug("RobotdObjectManager::ClearUnit %s %s", m_owner->guid().c_str(), it.c_str());
				}
			}
		}
		DoOnReleaseObject(m_owner->GetAccount(), obj);
	}
	else
	{
		tea_pdebug("RobotdObjectManager::ReleaseObject but not found %s  %s", m_owner->GetAccount().c_str(), guid.c_str());
	}

	return GuidObjectTable::ReleaseObject(guid);
}

//发包函数直接到中心服
void RobotdObjectManager::Send(uint32_t connid,ByteArray& bytes)
{
	//在这个服务器没用
}

void RobotdObjectManager::ClearUnit()
{
	tea_pdebug("RobotdObjectManager::ClearUnit %s", m_owner->guid().c_str());
	vector<string> guids;
	for (auto it = objs_.begin();it != objs_.end();++it)
	{
		if(GuidManager::GetPrefix(it->second->guid()) == ObjectTypeMap)
			guids.push_back(it->second->guid());
	}
	for (auto it:guids)
	{
		this->ReleaseObject(it);
		tea_pdebug("RobotdObjectManager::ClearUnit %s %s", m_owner->guid().c_str(), it.c_str());
	}

	guids.clear();
	for (auto it = objs_.begin();it != objs_.end();++it)
	{
		if(GuidManager::GetPrefix(it->second->guid()) == ObjectTypeUnit)
			guids.push_back(it->second->guid());
	}
	for (auto it:guids)
	{
		this->ReleaseObject(it);
		tea_pdebug("RobotdObjectManager::ClearUnit %s %s", m_owner->guid().c_str(), it.c_str());
	}
}



