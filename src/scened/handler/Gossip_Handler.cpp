#include "scened_context.h"
#include "Player.h"
#include "Map.h"
#include "Creature.h"
#include "scened_scripts.h"
#include "QuestGetter.h"
#include <shared/log_handler.h>
#include <object/OprateResult.h>

void ScenedContext::Handle_Gossip_Hello(packet& pkt)
{
	uint32 npc_guid;
	if(unpack_gossip_hello(&pkt,&npc_guid))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_GOSSIP_HELLO,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	string a ="";
	Call_Gossip_Message(npc_guid,0,a);
} 

void ScenedContext::Handle_Gossip_Select_Option(packet& pkt)
{
	uint32 option_id;
	uint32 npc_guid;
	char* key;
	if (unpack_gossip_select_option(&pkt,&option_id,&npc_guid,&key))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_GOSSIP_SELECT_OPTION,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	string text = key;
	Call_Gossip_Message(npc_guid,option_id,text);
}

void ScenedContext::Call_Gossip_Message(uint32 npc_guid,uint32 option_id,string &key)
{

	//验证此生物是否是NPC
	//验证选项ID合法性
	vector<gossip_menu_option_info> gossip_menu;
	Creature* obj = NULL;
	if(m_player->GetMap())
		obj = m_player->GetMap()->FindCreature(npc_guid);
	if(!obj)
		return;
	if (!m_player->IsReasonableDistancePos(obj))
	{
		//INTERACTION
		Call_Operation_Failed(OPRATE_TYPE_INTERACTION,INTERACTION_TOO_FAR);
		tea_perror("和NPCHELLO距离太远,当前坐标（%f,%f）",m_player->GetPositionX(),m_player->GetPositionY());
		return;
	}
	
	string text;
	if (strlen(obj->GetAIName())==0)
		return;
	if(DoGossipMessage(obj->GetAIName(),obj,m_player,option_id,text,gossip_menu,key) == 1)
	{
		Call_gossip_message(m_delegate_sendpkt,npc_guid,obj->GetEntry(),option_id,text.c_str(),gossip_menu);
	}
}
