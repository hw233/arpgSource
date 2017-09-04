#include "logind_object_manager.h"
#include "logind_mongodb.h"
#include "logind_cache.h"
#include "logind_player.h"
#include "map_manager.h"

LogindObjectManager::LogindObjectManager(SvrCoreAppImpl *app):SvrObjectManager(app)
{
	//添加索引器，为了遍历比较方便
	indexer_.CreateIndex(REGEX_PLAYER);
	indexer_.CreateIndex(REGEX_MAPPLAYERINFO);
	indexer_.CreateIndex(REGEX_WORLD);
}

LogindObjectManager::~LogindObjectManager()
{

}

//对象工厂
GuidObject *LogindObjectManager::ObjectFactor(const string &guid)
{
	BinLogObject *ptr = NULL;
	switch (GuidManager::GetPrefix(guid))
	{
	case ObjectTypePlayer:
		ptr = new logind_player;
		break;
	case ObjectTypeMapPlayerInfo:
		ptr = new BinLogObject(core_obj::SYNC_SLAVE | core_obj::SYNC_LOCAL);
		ptr->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		break;
	default:
		if(guid == GLOBAL_MAP_INSTANCE_LIST_GUID)
		{
			ASSERT(!LogindApp::g_app->m_map_Instance_list);
			LogindApp::g_app->m_map_Instance_list = new MapManager;
			ptr = LogindApp::g_app->m_map_Instance_list;
			break;
		}
		if(guid == GLOBAL_SAVE_TO_DB)
		{
			ASSERT(!LogindApp::g_app->m_save_to_db_guid_list);
			LogindApp::g_app->m_save_to_db_guid_list = new SavePlayerGuidObj;
			ptr = LogindApp::g_app->m_save_to_db_guid_list;
			break;
		}

		ptr = LogindApp::g_app->CreateGlobalValues(this, guid);
		if(!ptr)
		{
			ptr = new BinLogObject;
		}
		break;
	}
	return ptr;
}

GuidObject* LogindObjectManager::CreateObject(string guid)
{
	tea_pdebug("LogindObjectManager::CreateObject guid = '%s' ", guid.c_str());
	BinLogObject *ptr = dynamic_cast<BinLogObject*>(ObjectFactor(guid));
	ASSERT(ptr);	
	ptr->guid(guid);	
	AttachObject(ptr);
	return ptr;
}



void LogindObjectManager::ReleaseObject(string guid)
{
	tea_pdebug("LogindObjectManager::ReleaseObject guid = '%s' ", guid.c_str());
	BinLogObject *binlog = dynamic_cast<BinLogObject *>(Get(guid));
	if (binlog)
		DelOwnerMap(binlog->GetOwner(), binlog);
	return SvrObjectManager::ReleaseObject(guid);
}

//发包函数直接到中心服
void LogindObjectManager::Send(uint32_t connid,ByteArray& bytes)
{
	bytes.position(0);	
	WorldPacket pkt(0);
	WriteTo(bytes,pkt.GetPkt());
	LogindApp::g_app->SendToCentd(pkt);
}

BinLogObject *LogindObjectManager::Clone(BinLogObject *binlog)
{
	ASSERT(binlog);
	BinLogObject *result = NULL;
	if(GuidManager::GetPrefix(binlog->GetGuid()) == ObjectTypePlayer)
		result = new logind_player;
	else
		result = new BinLogObject;

	result->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
	for (uint32 i = 0; i < binlog->length_uint32(); i++)
	{
		result->SetUInt32(i, binlog->GetUInt32(i));
	}
	for (uint32 i = 0; i < binlog->length_str(); i++)
	{
		result->SetStr(i, binlog->GetStr(i));
	}
	result->SetGuid(binlog->GetGuid());

	result->Clear();
	result->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_2);
	return result;
}

logind_player *LogindObjectManager::FindPlayer(const string& guid)
{
	return dynamic_cast<logind_player*>(Get(guid));
}

//遍历所有的玩家
void LogindObjectManager::ForEachPlayer(std::function<void(logind_player*)> func)
{
	auto *idx = indexer().get(REGEX_PLAYER);	
	for (auto it = idx->begin();it != idx->end();++it)	
		func(dynamic_cast<logind_player*>(it->second));
}

void LogindObjectManager::CollapseReset()
{

}

//从中心服删除该玩家所有数据
void LogindObjectManager::RemovePlayerData(const string &guid)
{
	tea_pdebug("LogindObjectManager::RemovePlayerData %s", guid.c_str());
	auto ds = FindDataSetByOwnerGuid(guid);
	if(!ds)
		return;
	//先存在其他地方
	vector<string> vec;
	for (auto it:ds->GetAllData())
	{
		vec.push_back(it);
	}
	//移除
	string g = guid;
	for (auto it:vec)
	{
		CallRemoveObject(it, [g](GuidObject *){
			auto it = LogindApp::g_app->m_removing_player.find(g);
			ASSERT(it != LogindApp::g_app->m_removing_player.end());
			int i = it->second - 1;
			if(i <= 0)
			{
				LogindApp::g_app->m_removing_player.erase(g);
			}
			else
			{
				LogindApp::g_app->m_removing_player[g] = i;
			}
		});
	}
	ASSERT(LogindApp::g_app->m_removing_player.find(guid) == LogindApp::g_app->m_removing_player.end());
	LogindApp::g_app->m_removing_player[guid] = vec.size();
}

//读取玩家对象
bool LogindObjectManager::LoadPlayer(const string &guid, logind_player **player, vector<GuidObject*> &vec)
{
	GetDataSetAllObject(guid, vec);
	if(vec.empty())
		return false;
	for (auto it:vec)
	{
		if(it->guid() == guid)
		{
			*player = dynamic_cast<logind_player*>(it);
			ASSERT(*player);
			return true;
		}
	}
	//到了这里，呵呵，数据有问题了
	//ASSERT(false);
	return false;
}