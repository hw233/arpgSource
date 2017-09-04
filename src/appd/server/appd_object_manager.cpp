#include "appd_object_manager.h"
#include "RankListManager.h"
#include "appd_script.h"


AppdObjectManager::AppdObjectManager(SvrCoreAppImpl *app)
	:SvrObjectManager(app)
{
	//添加索引器，为了遍历比较方便
	indexer_.CreateIndex(REGEX_PLAYER);
}

AppdObjectManager::~AppdObjectManager()
{

}

//对象工厂
GuidObject *AppdObjectManager::ObjectFactor(const string &guid)
{
	BinLogObject *ptr = NULL;
	switch (GuidManager::GetPrefix(guid))
	{
	case ObjectTypePlayer:
		ptr = new AppdContext(0,AppdApp::g_app);
		break;
	case ObjectTypeGiftPacks:
		ptr = new BinLogObject;
		ptr->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_3);
		break;
	case ObjectTypeFactionData:
		ptr = new BinLogObject;
		ptr->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		break;
	default:
		if(guid == GLOBAL_RANK_INFO_RASK_POOL)
		{
			ptr = new BinLogObject(core_obj::SYNC_LOCAL);
			ptr->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
		}
		if(!ptr)
			ptr = AppdApp::g_app->CreateGlobalValues(this, guid);
		if(!ptr)
			ptr = new BinLogObject;
		break;
	}
	return ptr;
}

GuidObject* AppdObjectManager::CreateObject(string guid)
{
	tea_pdebug("AppdObjectManager::CreateObject guid = '%s' ", guid.c_str());
	BinLogObject *ptr = dynamic_cast<BinLogObject*>(ObjectFactor(guid));
	ASSERT(ptr);
	switch (GuidManager::GetPrefix(guid))
	{
	case ObjectTypePlayer:	
		ptr->after_update(std::bind(&RankListManager::OnAfterUpdate,AppdApp::g_app->m_rank_list_mgr,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,std::placeholders::_4));
		break;
	default:
		if(guid == GLOBAL_RANK_INFO_RASK_POOL)
		{
			AppdApp::g_app->m_rank_info_rask_pool = ptr;
			AppdApp::g_app->m_rank_list_mgr->InitTaskPool();
		}
		break;
	}
	
	ptr->guid(guid);
	ptr->Clear();
	AttachObject(ptr);
	return ptr;
}

void AppdObjectManager::AfterAttachObject(GuidObject *obj)
{
	SvrObjectManager::AfterAttachObject(obj);	
}

void AppdObjectManager::ReleaseObject(string guid)
{
	tea_pdebug("AppdObjectManager::ReleaseObject guid = %s",guid.c_str());
	BinLogObject *binlog = dynamic_cast<BinLogObject *>(Get(guid));
	if (binlog)
	{
		DelOwnerMap(binlog->GetOwner(), binlog);
	}
	return SvrObjectManager::ReleaseObject(guid);
}

//发包函数直接到中心服
void AppdObjectManager::Send(uint32_t connid,ByteArray& bytes)
{
	bytes.position(0);	
	WorldPacket pkt(0);
	WriteTo(bytes,pkt.GetPkt());
	AppdApp::SendToCentd(pkt);
}

BinLogObject *AppdObjectManager::Clone(BinLogObject *binlog)
{
	ASSERT(binlog);
	BinLogObject *result = new BinLogObject;	
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

AppdContext *AppdObjectManager::FindPlayer(const string& guid)
{
	return dynamic_cast<AppdContext*>(Get(guid));
}

void AppdObjectManager::ForEachPlayer(std::function<void(AppdContext*)> func)
{
	auto *idx = indexer().get(REGEX_PLAYER);	
	for (auto it = idx->begin();it != idx->end();++it)	
		func(dynamic_cast<AppdContext*>(it->second));
}
