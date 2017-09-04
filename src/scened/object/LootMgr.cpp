#include "LootMgr.h"
#include "Grid.h"
#include "Map.h"
#include "scened_context.h"
#include "scened_scripts.h"

LootObject::LootObject(Grid *g):BinLogObject(core_obj::SYNC_UPDATEDATA),
	m_uint_guid(0),m_map(nullptr),m_grid(g),m_private(nullptr),m_width(0)
{

}

LootObject::~LootObject()
{
	safe_delete(m_private);
}

void LootObject::WriteCreateBlock(ByteArray &bs)
{
	BinLogObject::WriteCreateBlock(bs);
}

void LootObject::Init(Map *m, int index)
{
	ASSERT(!m_map);
	m_map = m;
	stringstream ss;
	ss << index << "." << m->GetParentGuid() << "." << m->GetMapId();
	uint32 i = g_GuidMgr.NewIndex(ObjectTypeUnit);//这里取的是精灵的自增id
	SetGuid(g_GuidMgr.MAKE_NEW_GUID(ObjectTypeGridLoot, i, ss.str().c_str()));
	m_uint_guid = atol(SUB_GUID_STR_TO_INT(GetGuid()).c_str());
	m_private = new BinLogObject(core_obj::SYNC_NONE);

	uint16 m_start_y, m_start_x, m_end_x, m_end_y;
	m_start_x = m_grid->start_x;
	if(m_start_x % g_Config.loot_area)		
		m_start_x = (m_start_x + 1) / g_Config.loot_area * g_Config.loot_area;

	m_start_y = m_grid->start_y;
	if(m_start_y % g_Config.loot_area)		
		m_start_y = (m_start_y + 1) / g_Config.loot_area * g_Config.loot_area;

	m_end_x = m_grid->end_x;
	if(m_end_x % g_Config.loot_area)		
		m_end_x = m_end_x / g_Config.loot_area * g_Config.loot_area;

	m_end_y = m_grid->end_y;
	if(m_end_y % g_Config.loot_area)		
		m_end_y = m_end_y / g_Config.loot_area * g_Config.loot_area;

	m_width = (m_end_x - m_start_x) / g_Config.loot_area + 1;
	SetStartX(m_start_x);
	SetStartY(m_start_y);
	SetEndX(m_end_x);
	SetEndY(m_end_y);
}

void LootObject::PickUpLoot(ScenedContext *session, uint32 index, double &money)
{	
	uint32 entry = GetEntry(index);
	if(!entry)
		return;

	Player *player = session->GetPlayer();
	if(!player)
		return; 

	//验证归属
	const string &guid = GetLootOwner(index);
	if (!guid.empty())
	{
		if(GuidManager::GetPrefix(guid) == ObjectTypePlayer)
		{
			Player *owner = ObjMgr.FindPlayer(guid);
			//战利品所有者不是玩家且玩家不在归属者队伍里 不能拾取
			if(owner != player)
				return;
		}
	}

	uint32 count = GetCount(index);
	uint32 money_type;
	bool ret = DoGetMoneyType(entry, money_type);
	// 是资源
	if(ret)
	{
		money += count;
	}
	else
	{
		//通知应用服玩家拾取物品
		// 是否需要通知appd
		if (DoCheckIfSendToAppdAfterLootSelect(player, entry, count)) {
			ScenedApp::g_app->call_loot_select(session->GetGuid(), entry, 0, count, GetFailTime(index));
		}
	}
	ClearLoot(index);
	m_has_loot.erase(index);
}

void LootObject::ClearLoot(uint32 index)
{
	int x, y;
	GetLootPos(index, x, y);
	//清空
	for (int i = 0; i < MAX_LOOT_PUBLIC_INT_FIELD; i++)
	{
		if(GetInt32(LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + i))
			SetInt32(LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + i, 0);
	}
	for (int i = 0; i < MAX_LOOT_PUBLIC_STR_FIELD; i++)
	{
		if(!GetStr(LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PUBLIC_STR_FIELD + i).empty())
			SetStr(LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PUBLIC_STR_FIELD + i, "");
	}
	for (int i = 0; i < MAX_LOOT_PRIVATE_INT_FIELD; i++)
	{
		m_private->SetInt32(index * MAX_LOOT_PRIVATE_INT_FIELD + i, 0);
	}
	for (int i = 0; i < MAX_LOOT_PRIVATE_STRING; i++)
	{
		SetStr(LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PRIVATE_STRING + i, "");
	}

	m_map->UnSetMapSite((float)x, (float)y);
}

int LootObject::GetLootIndex(int x1, int y1)
{
	//ASSERT(x1 <= GetEndX() && y1 <= GetEndY() && x1 >= GetStartX() && y1 >= GetStartY());
	if(x1 < GetStartX() || x1 > GetEndX())
		return -1;
	if(y1 < GetStartY() || y1 > GetEndY())
		return -1;
	int x = (x1 - GetStartX()) / g_Config.loot_area;
	int y = (y1 - GetStartY()) / g_Config.loot_area;
	return y * m_width + x;
}

int LootObject::GetMaxIndex()
{
	//TODO:Grid最大的可以掉落100个	
	return 100;
}

void LootObject::GetLootPos(uint32 index, int &x, int &y)
{	
	x = index % m_width * g_Config.loot_area + GetStartX();
	y = index / m_width * g_Config.loot_area + GetStartY();
	ASSERT(x <= GetEndX() && y <= GetEndY() && x >= GetStartX() && y >= GetStartY());
}

void LootObject::Update(uint32 diff)
{
	for (auto it = m_has_loot.begin(); it != m_has_loot.end();)
	{
		uint32 index = *it;
		ASSERT(GetEntry(index));
		SubExistTime(index, diff);
		if(GetExistTime(index) <= 0)
		{
			ClearLoot(index);
			it = m_has_loot.erase(it);
			continue;
		}
		if(!GetLootOwner(index).empty())
		{
			SubOwnerTime(index,diff);
			if(GetOwnerTime(index) <= 0)
				SetLootOwner(index, "");
		}

		++it;
	}
}
