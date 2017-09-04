#include "scened_app.h"
#include <object/SharedDef.h>
#include <shared/probability_rand.h>
#include <shared/storage_loader.h>
#include <math.h>
#include <object/PlayerBase.h>

#include "Creature.h"
#include "GameObject.h"
#include "LootManager.h"
#include "Map.h"
#include "Player.h"
#include "scened_context.h"
#include "QuestGetter.h"
#include "scened_scripts.h"



//物品掉落配置
vector<uint32> LootManager::Loot_possibility_range;			//几率范围

//-------------------------------For ownerships -------------------------------------------------
//生成任务品
void LootManager::GenerationQuestLoot(Player *player, Unit *go, OwnershipVec* m_ownerquests)
{
//    ASSERT(player);
	if (player == NULL)
	{
		tea_perror("assert error: GenerationQuestLoot ASSERT(player)");
		return;
	}
	

    int entry = go->GetEntry();

    if(go->GetTypeId() == TYPEID_GAMEOBJECT)
        entry *= -1;

    //生成任务物品
	quest_loot_item_db.add_select_condition(MEMDB_EQ(quest_loot_item, creature_id, entry));
	MemDB_Result<quest_loot_item> result1 = quest_loot_item_db.select();

    for(uint32 i = 0; i < result1.size(); i++)
    {
        if(!result1[i]->quest_id)
            continue;

        if(urand(0, Loot_possibility_range[1]) < result1[i]->possibility * player->GetCoefficient().drop_rate_multiples / 100)
        {
            if(m_ownerquests == NULL)
            {
                //判断归属者是否有这任务
                if(player->GetQuestMgr()->GetQuestStatus(result1[i]->quest_id) == QUEST_STATUS_INCOMPLETE)
                    player->GetQuestMgr()->AddQuestItem(result1[i]->entry, result1[i]->count);
            }
            else
            {
				//任务品摸者有份
                for(OwnershipVec::iterator it = m_ownerquests->begin();
                        it != m_ownerquests->end();)
                {
                    //归属者合法性判断，、在线情况、生存情况、距离--消除仇恨处理
                    Player* owner = go->GetMap()->FindPlayer(*it);

                    if(owner && owner->GetMap() == go->GetMap()												//是否本地图
                            && owner->GetQuestMgr()->GetQuestStatus(result1[i]->quest_id) == QUEST_STATUS_INCOMPLETE) //归属者是否有这任务
                    {
                        owner->GetQuestMgr()->AddQuestItem(result1[i]->entry, result1[i]->count);
                    }

                    it++;
                }
            }
        }
    }
}


//计算战利品坐标,判断IX与IY是否可用
bool LootManager::ConsLootPoint(Map *map, uint32 &x, uint32 &y, int ix, int iy)
{
    if(map->IsEnpty(ix, iy))
    {
		//乘以边长，还原为正常的战利品坐标
        x = ix * g_Config.loot_area;
        y = iy * g_Config.loot_area;
        return true;
    }

    return false;
}

//计算战利品坐标
void LootManager::ConsLootPoint(Map *map, uint32 &xm, uint32 &ym)
{
	if(ConsLootPoint(map, xm, ym, xm, ym))
	{
		return;
	}
	/**
	for(int w = 1; ; ++ w)
	{
		// 上
		for (int dx = -w; dx <= w; ++ dx) {
			if(ConsLootPoint(map, xm, ym, xm + dx, ym - w)) {
				return;
			}
		}

		// 右
		for (int dy = -w; dy <= w; ++ dy) {
			if(ConsLootPoint(map, xm, ym, xm + w, ym + dy)) {
				return;
			}
		}

		// 下
		for (int dx = w; dx >= -w; -- dx) {
			if(ConsLootPoint(map, xm, ym, xm + dx, ym + w)) {
				return;
			}
		}

		// 左
		for (int dy = w; dy >= -w; -- dy) {
			if(ConsLootPoint(map, xm, ym, xm - w, ym + dy)) {
				return;
			}
		}
	}
	*/
	for(int w = 1; ; w++)
	{
		for(int h = w; h >= -w; h--)
		{
			if(ConsLootPoint(map, xm, ym, xm - w, ym - h))
			{
				return;
			}
			if(ConsLootPoint(map, xm, ym, xm + w, ym - h))
			{
				return;
			}
			if(ConsLootPoint(map, xm, ym, xm + h, ym - w))
			{
				return;
			}
			if(ConsLootPoint(map, xm, ym, xm + h, ym + w))
			{
				return;
			}
		}
	}
}

//生成战利品
void LootManager::GenerationLoot(Player* player, Creature *m_obj, Unit* unit)
{
	if(!m_obj->GetMap()->HaveEnpty())
	{
		return;
	}
    //ASSERT(player);
	if (player == NULL)
	{
		tea_perror("assert error: GenerationLoot ASSERT(player)");
		return;
	}

    //运行战利品生成脚本
    auto *creatrue_template = creature_template_db[m_obj->GetEntry()];
	if(creatrue_template)
		DoLootAllot(player, (Creature*)m_obj, unit, creatrue_template->monster_type);	
}

//战利品分配
void LootManager::LootAllot(Unit* unit, Creature *m_obj, bool isOneOne)
{
    //经验分配 战利品生成下发
	Player *killer = NULL;	//最后一个击杀玩家
	if(unit->GetTypeId() == TYPEID_PLAYER)
	{
		killer = dynamic_cast<Player*>(unit);
	}

	if(!killer || !killer->GetSession())	//最后一击不是玩家
		return;
	double vip_exp = 0;
    //任务接口
    if(m_obj->GetTypeId() == TYPEID_UNIT)
    {
		//任务品摸者有份
		for(OwnershipVec::iterator it = m_obj->m_ownerships.begin();
			it != m_obj->m_ownerships.end();)
		{
			//归属者合法性判断，、在线情况、生存情况、距离--消除仇恨处理
			Player* t_owner = m_obj->GetMap()->FindPlayer(*it);

			if(t_owner && t_owner->GetMap() == m_obj->GetMap())												//是否本地图
				//&& t_owner->GetQuestMgr()->GetQuestStatus(result1[i]->quest_id) == QUEST_STATUS_INCOMPLETE) //归属者是否有这任务
			{
				creature_template const *temp = creature_template_db[m_obj->GetEntry()];
				t_owner->GetQuestMgr()->KilledMonster(temp);
				t_owner->GetQuestMgr()->KilledLevelMonster(temp->level);
				if(t_owner != killer)
				{
					//所有参与击杀怪物的玩家可获得50%的怪物经验			
					double exp =DOComputeExpBonusScript(t_owner, m_obj, m_obj->GetFinallyExp(t_owner), 50,vip_exp); //如果不是生物会被return
					t_owner->GainExp(exp,0,vip_exp);
				}
			}

			it++;
		}
	}
	//增加杀怪数
	killer->GetSession()->AddKillMonster();

	//普通生物
	GenerationQuestLoot(killer, m_obj, &m_obj->m_ownerships);
	
    //生成战利品
	if(!isOneOne)
	{
		GenerationLoot(killer, m_obj, unit);
		//生成战利品后清空所有者列表
		m_obj->m_ownerships.clear();

		if(m_obj->GetTypeId() == TYPEID_UNIT)
		{
			//经验加成,最后一下杀死怪物的玩家可获得该怪物100%的经验
			double exp =DOComputeExpBonusScript(killer, m_obj, m_obj->GetFinallyExp(killer),100,vip_exp); //如果不是生物会被return
			killer->GainExp(exp,0, vip_exp);
		}
	}
}

//玩家触碰机关
void GameObject::UseGear(Player *player)
{
	if(this->GetGearStatus() > GO_GEAR_STATUS_END)
	{
		tea_perror("机关状态异常，状态为%u，启动应急修复。", this->GetGearStatus());
		this->SetGearStatus(GO_GEAR_STATUS_START);
		return;
	}
	if(this->GetGearStatus() == GO_GEAR_STATUS_END)
	{
		tea_perror("机关已经挂掉了，为什么客户端还上传？？");
		return;
	}
	if(player->GetMap() != this->GetMap())
	{
		tea_perror("玩家和机关不在同一个地图.");
		return ;
	}

	//脚本是用来处理机关状态的
	if(this->IsExecScript())
	{
		DoGameObjectScript(player, this, this->GetEntry());
	}
	else
	{
		//若是可使用的机关，则机关状态+1，否则直接到底
		if(this->CanUse())
			this->SetGearStatus(this->GetGearStatus() + 1);
		else
			this->SetGearStatus(GO_GEAR_STATUS_END);
	}

	//广播一下动作包
	this->BroadcastAction(player->GetUIntGuid());

	//若机关已破，则爆战利品，消失。。。
	if(this->GetGearStatus() == GO_GEAR_STATUS_END)
	{
		//如果能出战利品
		if(this->CanLoot())
		{
			
		}
	
		//若机关状态到底了，并且机关不是长期存在的类型，则干掉
		if(!this->IsForeverUse())
		{
			this->GetMap()->m_go_dynamic.push_back(std::make_pair(g_Config.gear_exist_time, this->GetGuid()));
			this->SetGarbage(true);
		}
	}
}

//使用之后，广播动作包
bool GameObject::BroadcastAction(uint32 player_id)
{
	const gameobject_template* tpl = gameobject_template_db[this->GetEntry()];
	if(tpl->actionState != 0)
	{
		packet *pck;
		pack_use_gameobject_action(&pck, player_id, GetUIntGuid());
		this->GetMap()->Broadcast(*pck, this);
		return true;
	}
	return false;
}

