#include <protocol/internal.h>
#include <protocol/WorldPacket.h>
#include <server/svrcore_app.h>
#include "shared/FactoryHolder.h"
#include "svr_object_manager.h"

//for svr_objects_script.cpp

extern void DoOnAttachObject(GuidObject *obj);
extern void DoOnAfterAttachObject(GuidObject *obj);
extern void DoOnReleaseObject(GuidObject *obj);

SvrObjectManager *SvrObjectManager::self = nullptr;

SvrObjectManager::SvrObjectManager(SvrCoreAppImpl *app)
	:GuidObjectTable(core_obj::SYNC_SLAVE),
	RemoteOperatorAdapter(this,std::bind(&SvrObjectManager::Send,this,std::placeholders::_1,std::placeholders::_2),
	INTERNAL_OPT_UD_CONTROL,
	INTERNAL_OPT_UD_OBJECT,
	INTERNAL_OPT_UD_CONTROL_RESULT)
{
	self = this;
	m_app = app;
}

SvrObjectManager::~SvrObjectManager()
{
	safe_delete(m_all_obj_owner_idx);
}

void SvrObjectManager::Update()
{
	RemoteOperatorAdapter::Update();
	m_compress_tool.Update();
}

void SvrObjectManager::AttachObject(GuidObject* obj)
{
	InitNoticeInfo(obj);
	DoOnAttachObject(obj);
	return GuidObjectTable::AttachObject(obj);
}

void SvrObjectManager::AfterAttachObject(GuidObject *obj)
{
	AfterCreateObjOwner(dynamic_cast<BinLogObject*>(obj));
    DoOnAfterAttachObject(obj);
}

void SvrObjectManager::ReleaseObject(string guid)
{
	auto *obj = Get(guid);
	if(obj) DoOnReleaseObject(obj);
	GuidObjectTable::ReleaseObject(guid);
}

//添加一个owner-guid
bool SvrObjectManager::AddToOwnerMap(const string &owner,GuidObject *obj)
{
	ASSERT(!owner.empty());
	OwnerDataSet *data = nullptr;
	auto it = m_all_obj_owner_idx.find(owner);
	if(it == m_all_obj_owner_idx.end())
	{
		data = NewOwnerDataSet(owner);
	}
	else
	{
		data = it->second;
		set<string> &all_data = data->GetAllData();
		if(all_data.find(obj->guid()) != all_data.end())
		{
			out_warn(m_app->get_logger(), "SvrObjectManager::AddToOwnerMap  %s, repeat to add ", obj->guid().c_str());
			return true;
		}
	}
	out_debug(m_app->get_logger(), "SvrObjectManager::AddToOwnerMap %s", obj->guid().c_str());
	data->GetAllData().insert(obj->guid());
	return true;
}

//删除指定guid的owner-guid
bool SvrObjectManager::DelOwnerMap(const string owner, GuidObject *obj)
{
	if(owner.empty())
		return false;

	auto it = m_all_obj_owner_idx.find(owner);
	if(it == m_all_obj_owner_idx.end())
		return false;

	OwnerDataSet *ds = it->second;
	set<string> &vec = ds->GetAllData();
	for (auto iter = vec.begin(); iter != vec.end(); ++iter)
	{
		if(*iter == obj->guid())
		{
			out_debug(m_app->get_logger(), "SvrObjectManager::DelOwnerMap %s", obj->guid().c_str());
			vec.erase(iter);
			if(vec.empty())
			{
				m_all_obj_owner_idx.erase(it);
				safe_delete(ds);
				out_debug(m_app->get_logger(), "SvrObjectManager::DelOwnerMap del dataset %s, vec is empty", owner.c_str());
			}
			return true;
		}
	}
	return false;
}

void SvrObjectManager::InsertObjOwner(const string &guid)
{
	BinLogObject *obj = dynamic_cast<BinLogObject*>(Get(guid));
	if(obj)
		AfterAttachObject(obj);	
}

//创建完binlog对象后的回调
void SvrObjectManager::AfterCreateObjOwner(BinLogObject *binlog)
{
	ASSERT(binlog);
	if(binlog)
	{
		string owner = binlog->GetOwner();
		if(owner == "")
		{
			switch (GuidManager::GetPrefix(binlog->GetGuid()))
			{
			case ObjectTypeGlobalValue:
				owner = GLOBAL_VALUE_OWNER_STRING;					
				break;
			default:
				out_debug(m_app->get_logger(),"AfterCreateObjOwner guid is %s",binlog->GetGuid().c_str());
				//ASSERT(false);
				break;
			}
			binlog->SetOwner(owner);
		}
		if (!owner.empty())
		{
			AddToOwnerMap(owner, binlog);
		}		
	}
}

OwnerDataSet *SvrObjectManager::NewOwnerDataSet(const string &guid)
{
	ASSERT(m_all_obj_owner_idx.find(guid) == m_all_obj_owner_idx.end());
	out_debug(m_app->get_logger(), "add new dataset %s", guid.c_str());
	OwnerDataSet *data = nullptr;
	data = new OwnerDataSet(guid);
	m_all_obj_owner_idx[guid] = data;
	return data;
}

//获得指定玩家的所有数据集
OwnerDataSet *SvrObjectManager::FindDataSetByOwnerGuid(const string &owner)
{
	auto it = m_all_obj_owner_idx.find(owner);
	if (it == m_all_obj_owner_idx.end())
		return nullptr;
	return it->second;
}

//根据正则表达式打印搜索的对象
void SvrObjectManager::PrintObjectByRegex(const string &regex_str, bool print_data)
{
	out_str(m_app->get_logger(), "--------------------------------------------------------------------------------------");
	out_str(m_app->get_logger(), "--------------------------------------------------------------------------------------");
	out_str(m_app->get_logger(), "--------------------------------------------------------------------------------------");
	out_str(m_app->get_logger(), "SvrObjectManager::PrintObjectByRegex %s begin.", regex_str.c_str());
	core_obj::Regex e(regex_str);
	for (auto it:objs_)
	{
		BinLogObject *obj = dynamic_cast<BinLogObject*>(it.second);
		ASSERT(obj);
		if(e.Search(obj->guid()))
		{
			out_str(m_app->get_logger(), "guid:%s		owener:%s", obj->guid().c_str(), obj->GetOwner().c_str());
			if(!print_data)
				continue;
			string data,data_str;
			obj->ToString(data, data_str);
			out_str(m_app->get_logger(), "%s", data.c_str());
			out_str(m_app->get_logger(), "%s", data_str.c_str());
		}
	}
	out_str(m_app->get_logger(), "SvrObjectManager::PrintObjectByRegex %s end.", regex_str.c_str());
	out_str(m_app->get_logger(), "--------------------------------------------------------------------------------------");
	out_str(m_app->get_logger(), "--------------------------------------------------------------------------------------");
	out_str(m_app->get_logger(), "--------------------------------------------------------------------------------------");
}

//获取某数据集下面所有的对象
void SvrObjectManager::GetDataSetAllObject(const string &guid, vector<GuidObject*> &result)
{
	auto it = m_all_obj_owner_idx.find(guid);
	if(it == m_all_obj_owner_idx.end())
	{
		return;
	}
	OwnerDataSet *ds = it->second;
	set<string> &vec = ds->GetAllData();
	for (auto it = vec.begin(); it != vec.end();)
	{
		GuidObject *obj = Get(*it);
		if(obj)
		{
			result.push_back(obj);
			++it;
		}
		else
		{
			it = vec.erase(it);
		}
	}
}

//byteArray内存池回收
void SvrObjectManager::ByteArrayPoolClear()
{
	for (auto it:bytes_pool_.bytes_pool_)
		delete it;
	bytes_pool_.bytes_pool_.clear();
}

//遍历所有对象
void SvrObjectManager::ForeachIndexer(const string& key,std::function<bool(GuidObject*)> f)
{
	//如果key为空,则简单遍历所有对象
	if(key.empty())
	{
		for (auto it = objs_.begin();it != objs_.end();++it)
		{
			if(f(it->second))
				return;		
		}
		return;
	}

	auto idx = indexer_.get(key);
	if(idx)
	{
		for (auto it = idx->begin();it != idx->end();++it)
		{
			if(f(it->second))
				return;
		}
	}
	else if(objs_.count(key) == 1)
	{
		f(Get(key));
	}
	else
	{		
		core_obj::Regex e(key);	
		for (ValueHashMap::iterator it = objs_.begin();it != objs_.end();++it)
		{
			if(e.Search(it->first) && f(Get(it->first)))
				return;
		}
	}
}


//添加对象到远程
void SvrObjectManager::CallPutObjects(const string& tag,vector<GuidObject *> &objs)
{
	CallPutsObject(tag, objs);	
	for (auto iter = objs.begin(); iter != objs.end(); ++iter)
		AfterAttachObject(*iter);
}
