
#include "GuidManager.h"

GuidManager::GuidManager() : m_data(NULL),m_my_data(NULL)
{

}

GuidManager::~GuidManager()
{
	safe_delete(m_my_data);
}

const char* GuidManager::MAKE_NEW_GUID(EObjectTypePrefix prefix,uint32 index,const char *suffix)
{
	if(strlen(suffix))
		sprintf(m_tmp,"%c%u.%s",prefix,index,suffix);
	else
		sprintf(m_tmp,"%c%u",prefix,index);
	return m_tmp;
}

string GuidManager::ReplaceSuffix(const string& guid,EObjectTypePrefix t)
{
	string ret_val = guid;
	if(GetPrefix(ret_val.c_str()) == ObjectTypeNone)
		return "";
	ret_val[0] = char(t);
	return ret_val;
}

/************************************************************************/
/* 替换前缀生成正则表达式                                                                     */
/************************************************************************/
string GuidManager::ScenePrefixRegex(const string& guid)
{
	if(GetPrefix(guid.c_str()) == ObjectTypeNone)
		return "";

	char pref[10];
	sprintf(pref, "[%c%c]", ObjectTypeSpell, ObjectTypePlayer);
	string prefix = pref;
	return prefix + guid.substr(1);
}

uint32 GuidManager::NewIndex(EObjectTypePrefix t)
{
	ASSERT(m_data);
	ASSERT(m_my_data);
	m_data->AddUInt32(int32(t), 1);
	m_my_data->AddUInt32(int32(t), 1);
	uint32 result = m_my_data->GetUInt32(int32(t));
	return result;
}

void GuidManager::LoadMyData()
{
	ASSERT(!m_my_data);
	m_my_data = new BinLogObject(core_obj::SYNC_NONE);
	m_my_data->SetBinlogMaxSize(core_obj::SyncEventRecorder::MAX_BINLOG_SIZE_UNLIME);
	for (uint32 i = 0; i < m_data->length_uint32(); i++)
	{
		m_my_data->SetUInt32(i, m_data->GetUInt32(i));
	}
}

//同步guid累加最大值
void GuidManager::SyncMaxGuid(uint32 player_max)
{
	ASSERT(m_my_data != NULL && m_data != NULL);
	if(m_my_data->GetUInt32(int32(ObjectTypePlayer)) > player_max)
		player_max = m_my_data->GetUInt32(int32(ObjectTypePlayer));
	player_max += 1500;

	m_my_data->SetUInt32(int32(ObjectTypePlayer), player_max);
	m_data->SetUInt32(int32(ObjectTypePlayer), player_max);

	m_my_data->Clear();
}

void GuidManager::SetUnitGuid(uint32 unit_id)
{
	m_my_data->SetUInt32(int32(ObjectTypeUnit), unit_id);
}
