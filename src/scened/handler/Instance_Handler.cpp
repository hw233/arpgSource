#include "scened_context.h"
#include <shared/storage_loader.h>
#include "Player.h"
#include "Map.h"
#include "scened_scripts.h"
#include <shared/log_handler.h>

//进入副本
void ScenedContext::Handle_Instance_Enter(packet& pkt)
{
	uint32 map_id ;
	if (unpack_instance_enter(&pkt,&map_id))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_INSTANCE_ENTER,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	if (m_player->IsInTheInstanceMap())//MapTemplate::GetMapTempalte(m_player->GetMapId())->IsInstance())
	{
		tea_perror("Handle_Instance_Enter不能在副本里面进入副本");
		return;
	}
	if (!m_player->isAlive()) //玩家死了就不让发协议
	{
		tea_perror("error : player is dead!!!!!!!!!");
		return;
	}

	DoInstancEnter(m_player->GetMap(), m_player, m_player->GetMapId(), map_id);

}

//离开副本
void ScenedContext::Handle_Instance_Exit(packet& pkt)
{
	uint32 reserve;
	if(unpack_instance_exit(&pkt, &reserve))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_INSTANCE_EXIT,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	// 现在都回到主城了 无须判断了, 只要不是主城间
	//战斗状态判断,若是跨服玩家的话就不需要限制
	//if (!m_player->IsInTheInstanceMap() && !IsKuafuPlayer())
	//{
	//	tea_pdebug("不在副本地图中怎么离开副本");
	//	return;
	//}
	//若是跨服玩家的话就不需要限制
	if(m_player->GetPVPstate() && !IsKuafuPlayer())
	{
		tea_pdebug("PVP状态不许离开副本");
		return;
	}
	if (DoPlayerExitInstance(m_player->GetMap(), m_player, reserve) != 0)
	{
		SpecifyTeleportOrigin(m_player->GetMap(), m_player);
		((InstanceMap*)(m_player->GetMap()))->ExitInstance(m_player);	
	}	
}

void ScenedContext::Handle_Instance_Next_State(packet &pkt)
{
	uint16 level = 0;
	uint32 param = 0;
	if (unpack_instance_next_state(&pkt,&level,&param))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_INSTANCE_NEXT_STATE,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	if (!m_player->IsInTheInstanceMap())
	{
		tea_pdebug("不在副本地图中怎么进入副本下一状态");
		return;
	}
	//战斗状态判断
	if(m_player->GetPVPstate())
	{
		tea_pdebug("PVP状态不许进入副本下一状态");
		return;
	}
	OnInstanceTryToNextState(m_player, m_player->GetMap(),level,param);
}

