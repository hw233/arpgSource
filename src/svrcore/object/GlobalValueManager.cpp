#include "GlobalValueManager.h"
#include "BinLogObject.h"


BinLogObject *GlobalValueManager::CreateGlobalValues(core_obj::GuidObjectTable *objMgr, const string &guid)
{
	BinLogObject *result = NULL;
	if(guid == GLOBAL_OBJ_GUID)
	{
		result = CreateObject(guid, &m_globalvalue);
	}
	else if(guid == GLOBAL_SERVER_CONNN_LIST_GUID)
	{
		ASSERT(!m_server_conn_list);
		m_server_conn_list = new ServerConnList;
		m_server_conn_list->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		result = m_server_conn_list;
	}
	else if(guid == GLOBAL_MAP_INSTANCE_LIST_GUID)
	{
		ASSERT(!m_map_Instance_list);
		m_map_Instance_list = new BinLogObject;
		result = m_map_Instance_list;
	}
	else if(guid == GLOBAL_GUID_MANAGER_GUID)
	{
		ASSERT(!m_guid_mgr.m_data);
		m_guid_mgr.m_data = new BinLogObject;
		m_guid_mgr.m_data->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		result = m_guid_mgr.m_data;
	}
	else if(guid == GLOBAL_RANK_INFO)
	{
		ASSERT(!m_rank_info);
		m_rank_info = new BinLogObject;
		m_rank_info->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		result = m_rank_info;
	}
	else if(guid == GLOBAL_SYSTEM_MAIL)
	{
		ASSERT(!m_systemmail);
		m_systemmail = new BinLogObject;
		m_systemmail->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		result = m_systemmail;
	}
	else if(guid == GLOBAL_ASYNC_VALUE)
	{
		ASSERT(!m_asyncValue);
		m_asyncValue = new BinLogObject;
		m_asyncValue->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		result = m_asyncValue;
	}
	else if(guid == GLOBAL_OFFLINE_MAIL)
	{
		ASSERT(!m_offlinemail);
		m_offlinemail = new BinLogObject;
		m_offlinemail->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		result = m_offlinemail;
	}
	else if(guid == GLOBAL_RANK_INFO_RASK_POOL)
	{
		ASSERT(!m_rank_info_rask_pool);
		m_rank_info_rask_pool = new BinLogObject;
		m_rank_info_rask_pool->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		result = m_rank_info_rask_pool;
	}
	else if(guid == GLOBAL_RIGHT_FLOAT_GUID)
	{
		ASSERT(!m_rightfloat);
		m_rightfloat = new BinLogObject;
		result = m_rightfloat;
	}
	else if(guid == GLOBAL_GAME_CONFIG)
	{
		ASSERT(!m_gameconfig);
		m_gameconfig = new BinLogObject;
		result = m_gameconfig;
	}
	else if(guid == GLOBAL_CLIENT_GAME_CONFIG)
	{
		ASSERT(!m_clientconfig);
		m_clientconfig = new BinLogObject;
		result = m_clientconfig;
	}
	else if(guid == GLOBAL_NOTICE_MESSAGE)
	{
		ASSERT(!m_notice_message);
		m_notice_message = new BinLogObject;
		result = m_notice_message;
	}
	else if(guid == GLOBAL_SAVE_TO_DB)
	{
		ASSERT(!m_save_to_db_guid_list);
		m_save_to_db_guid_list = new BinLogObject;
		result = m_save_to_db_guid_list;
	}
	else if(guid == GLOBAL_COUNTER)
	{
		ASSERT(!m_counter);
		m_counter = new BinLogObject;
		m_counter->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		result = m_counter;
	}	
	
	if(result)
	{
		objMgr->RegisterCreateEvent(guid, [&](core_obj::GuidObject *obj){
			AfterCreateGlobalValue(dynamic_cast<BinLogObject*>(obj));
		});
	}
	return result;
}

void GlobalValueManager::AfterCreateGlobalValue(BinLogObject *binlog)
{
	if(binlog->GetGuid() == GLOBAL_GUID_MANAGER_GUID)
	{
		m_guid_mgr.LoadMyData();
	}
}

BinLogObject *GlobalValueManager::CreateObject(const string &guid, BinLogObject **ptr, std::function<BinLogObject*(const string &)> fun)
{
	ASSERT(!*ptr);
	if(fun)
	{
		*ptr = fun(guid);
	}
	else
	{
		*ptr = new BinLogObject;
	}
	ASSERT(*ptr);
	return *ptr;
}

void GlobalValueManager::GetMergeServerName(vector<string>& server_names)
{
	for (int i = 0; i < 1000; i++)
	{
		auto s = m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_SERVER + i);
		if (s.empty())
			break;
		server_names.push_back(s);
	}
}

bool GlobalValueManager::IsMyServer(const string &server_name)
{
	for (int i = 0; i < 1000; i++)
	{
		auto s = m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_SERVER + i);
		if (s.empty())
			break;
		if(s == server_name)
			return true;
	}
	return false;
}

void GlobalValueManager::AddServerID(const string &server_name)
{
	for (int i = 0; i < 1000; i++)
	{
		auto s = m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_SERVER + i);
		if(s == server_name)
			return;
		if (s.empty())
		{
			SetServerID(i, server_name);
			return;
		}
	}
}
