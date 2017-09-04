#ifndef _LOOT_MGR_H_
#define _LOOT_MGR_H_

#include <comm/common_stl.h>
#include <shared/MemoryDB.h>
#include <shared/storage_strus.h>
#include <protocol/external.h>

class Unit;

class LootManager
{
public:
	static void LootAllot(Unit* player, Creature *m_obj,bool isOneOne);								//战利品分配 返回是否有战利品	

	static void GenerationQuestLoot(Player* player, Unit *go, OwnershipVec* m_ownerships);			//生成任务品	
	static void ConsLootPoint(Map *map, uint32 &x, uint32 &y);		//计算战利品坐标
	static bool ConsLootPoint(Map *map, uint32 &x, uint32 &y, int ix, int iy);//计算战利品坐标
	static void GenerationLoot(Player* player, Creature *m_obj, Unit* unit);			//生成战利品
	
	//物品掉落配置
	static vector<uint32> Loot_possibility_range;		//几率范围
};

#endif

