#include "scened_context.h"
#include "Player.h"
#include "Map.h"
#include "Creature.h"
#include "GameObject.h"
#include "LootManager.h"
#include "scened_scripts.h"
#include <object/OprateResult.h>
#include <shared/log_handler.h>
#include "BuffManager.h"

//使用游戏对象
void ScenedContext::Handle_Use_Gameobject(packet& pkt)
{
	uint32 target;
	if (unpack_use_gameobject(&pkt, &target))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_USE_GAMEOBJECT,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	if (!m_player->isAlive())	//玩家死了就不要使用游戏对象
	{
		Call_Operation_Failed(OPRATE_TYPE_USE_GAMEOBJECT, USE_GAMEOBJECT_FAIL);
		tea_perror(" player is dead ,do not handle use gameobject ");
		return;
	}
	
	auto temp_target = (target == 0 ? m_player->GetTargetGuid() : target);
	if (!temp_target)
	{
		tea_perror("你需要一个目标");
		return;
	}

 	GameObject *tele = (GameObject *)(m_player->GetMap()->FindWorldObject(temp_target));
	if (!tele) {
		tea_perror("you need a target");
		return;
	}
	
	// 有可能存在像怪物一样死亡
	if(!tele->isAlive()) {
		return;
	}

	if (!tele || tele->IsGarbage())
	{
		Call_Operation_Failed(OPRATE_TYPE_USE_GAMEOBJECT, USE_GAMEOBJECT_FAIL);
		return;
	}
	if((tele->NeedUseMode() || tele->IsGEAR()) && !m_player->CanUseGameObject())
	{
		Call_Operation_Failed(OPRATE_TYPE_USE_GAMEOBJECT, USE_GAMEOBJECT_TOO_FAST);
		return;
	}
	m_player->SetLastActionPktTime();
	if (!m_player->IsReasonableDistancePos(tele))
	{	
		Call_Operation_Failed(OPRATE_TYPE_INTERACTION,INTERACTION_TOO_FAR);
		tea_perror("使用GAMEOBJ距离太远 当前坐标（%f,%f）",m_player->GetPositionX(),m_player->GetPositionY());
		return;
	}

	//如果不可以被使用
	if (!tele->CanUse())
	{
		tea_perror("player[%s],gameobject[%s],entry[%u] can't use!",
			m_player->GetGuid().c_str(),tele->GetGuid().c_str(),tele->GetEntry());
		return;
	}

	//机关类
	if (tele->IsGEAR())
	{
		tele->UseGear(m_player);
	}
	else if (tele->IsTeleteport()) //传送点
	{
		return;
		//没有限制传送的buff，地图已开启，且不在pvp状态
		//if (m_player->isCanTeleport() && !m_player->GetPVPstate())
		//{
		//	Teleport(tele->GetToPositionX(),tele->GetToPositionY(),tele->GetToMapId(),"");
		//}
	}
	else if(tele->IsQuestGO())		//任务
	{
		_Handle_QuestObject_Use(tele);	
	}	
	
	//如果需要进脚本,机关的处理过程不太一样
	if (tele->IsExecScript() && !tele->IsGEAR())
	{
		uint32 entry = tele->GetEntry();
		if(DoBeforUseGameObjectScript(m_player,tele,entry) == 0)
			return;
		DoGameObjectScript(m_player,tele,entry);
		return;
	}
	// 需要重生的走creature的Onkilled流程
	if (tele->IsReborn()) {
		tele->OnKilled();
	}
	//如果不是永久使用的马上消失,机关类自己有单独处理，不需要进来
	else if (!tele->IsForeverUse() && !tele->IsGEAR() && !tele->CanLoot()) {
		tea_pdebug("删除gameobject id = %u playerguid = %s  playername = %s", tele->GetEntry(), m_player->GetGuid().c_str(), m_player->GetName().c_str());
		tele->GetMap()->m_go_dynamic.push_back(std::make_pair(-1, tele->GetGuid()));
		tele->SetGarbage(true);
	}	
	
}


