#include <protocol/external.h>
#include <object/SharedDef.h>
#include "scened_context.h"
#include "Player.h"
#include "QuestGetter.h"
#include "Map.h"
#include "Grid.h"
#include "LootMgr.h"
#include <shared/log_handler.h>
#include <object/OprateResult.h>
#include "scened_scripts.h"

//////////////////////////////////////////////////////////////////////////
//for ScenedOpcodeHandler
void ScenedContext::OpcodeHandlerInitor()
{
	for (uint32 i = 0; i <= NUM_MSG_TYPES; i++)
	{
		opcode_handler_[i].handler = std::bind(&ScenedContext::Handle_LUA, this, std::placeholders::_1);
		opcode_handler_[i].status = STATUS_LOGGEDIN;
	}

	opcode_handler_[CMSG_TELEPORT].handler = std::bind(&ScenedContext::Handle_Teleport, this, std::placeholders::_1);
	opcode_handler_[CMSG_TELEPORT].status = STATUS_LOGGEDIN;

	opcode_handler_[MSG_MOVE_STOP].handler = std::bind(&ScenedContext::Handle_Move_Stop, this, std::placeholders::_1);
	opcode_handler_[MSG_MOVE_STOP].status = STATUS_LOGGEDIN;
	opcode_handler_[MSG_MOVE_STOP].cooldown = 0;

	opcode_handler_[MSG_UNIT_MOVE].handler = std::bind(&ScenedContext::Handle_Unit_Move, this, std::placeholders::_1);
	opcode_handler_[MSG_UNIT_MOVE].status = STATUS_LOGGEDIN;
	opcode_handler_[MSG_UNIT_MOVE].cooldown = 0;

	opcode_handler_[MSG_JUMP].handler = std::bind(&ScenedContext::Handle_Jump, this, std::placeholders::_1);
	opcode_handler_[MSG_JUMP].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_SET_ATTACK_MODE].handler = std::bind(&ScenedContext::Handle_Set_Attack_Mode, this, std::placeholders::_1);
	opcode_handler_[CMSG_SET_ATTACK_MODE].status = STATUS_LOGGEDIN;

	opcode_handler_[MSG_SELECT_TARGET].handler = std::bind(&ScenedContext::Handle_Select_Target, this, std::placeholders::_1);
	opcode_handler_[MSG_SELECT_TARGET].status = STATUS_LOGGEDIN;
	opcode_handler_[MSG_SELECT_TARGET].cooldown = 200;		//选择目标

	opcode_handler_[CMSG_SET_ORIENT].handler = std::bind(&ScenedContext::Handle_Set_Orient, this, std::placeholders::_1);
	opcode_handler_[CMSG_SET_ORIENT].status = STATUS_LOGGEDIN;

	opcode_handler_[MSG_SPELL_START].handler = std::bind(&ScenedContext::Handle_Spell_Start, this, std::placeholders::_1);
	opcode_handler_[MSG_SPELL_START].status = STATUS_LOGGEDIN;

	opcode_handler_[MSG_SPELL_STOP].handler = std::bind(&ScenedContext::Handle_Spell_Stop, this, std::placeholders::_1);
	opcode_handler_[MSG_SPELL_STOP].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_RESURRECTION].handler = std::bind(&ScenedContext::Handle_Resurrection, this, std::placeholders::_1);
	opcode_handler_[CMSG_RESURRECTION].status = STATUS_LOGGEDIN;

	/*opcode_handler_[MSG_CHAT_NEAR].handler = std::bind(&ScenedContext::Handle_Chat_Near, this, std::placeholders::_1);
	opcode_handler_[MSG_CHAT_NEAR].status = STATUS_LOGGEDIN;
	opcode_handler_[MSG_CHAT_NEAR].cooldown = 0;*/

	opcode_handler_[CMSG_USE_GAMEOBJECT].handler = std::bind(&ScenedContext::Handle_Use_Gameobject, this, std::placeholders::_1);
	opcode_handler_[CMSG_USE_GAMEOBJECT].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_USE_GAMEOBJECT].cooldown = 0;

	opcode_handler_[CMSG_GOSSIP_HELLO].handler = std::bind(&ScenedContext::Handle_Gossip_Hello, this, std::placeholders::_1);
	opcode_handler_[CMSG_GOSSIP_HELLO].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_GOSSIP_SELECT_OPTION].handler = std::bind(&ScenedContext::Handle_Gossip_Select_Option, this, std::placeholders::_1);
	opcode_handler_[CMSG_GOSSIP_SELECT_OPTION].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_TAXI_HELLO].handler = std::bind(&ScenedContext::Handle_Taxi_Hello, this, std::placeholders::_1);
	opcode_handler_[CMSG_TAXI_HELLO].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_TAXI_SELECT_STATION].handler = std::bind(&ScenedContext::Handle_Taxi_Select_Station, this, std::placeholders::_1);
	opcode_handler_[CMSG_TAXI_SELECT_STATION].status = STATUS_LOGGEDIN;

	opcode_handler_[MSG_SYNC_MSTIME].handler = std::bind(&ScenedContext::Handle_Syns_Mstime, this, std::placeholders::_1);
	opcode_handler_[MSG_SYNC_MSTIME].status = STATUS_NEVER;

	//任务相关
	opcode_handler_[CMSG_QUESTGIVER_STATUS_QUERY].handler = std::bind(&ScenedContext::Handle_QuestGiver_Status_Query, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTGIVER_STATUS_QUERY].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_QUESTHELP_GET_CANACCEPT_LIST].handler = std::bind(&ScenedContext::Handle_Questhelp_Get_CanAccept_List, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTHELP_GET_CANACCEPT_LIST].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_QUESTGIVER_ACCEPT_QUEST].handler = std::bind(&ScenedContext::Handle_QuestGiver_Accept_Quest, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTGIVER_ACCEPT_QUEST].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_QUESTLOG_REMOVE_QUEST].handler = std::bind(&ScenedContext::Handle_QuestGiver_Remove_Quest, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTLOG_REMOVE_QUEST].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_QUESTGIVER_COMPLETE_QUEST].handler = std::bind(&ScenedContext::Handle_QuestGiver_Complete_Quest, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTGIVER_COMPLETE_QUEST].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_QUESTUPDATE_USE_ITEM].handler = std::bind(&ScenedContext::Handle_QuestUpdate_Use_Item, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTUPDATE_USE_ITEM].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_QUESTHELP_COMPLETE].handler = std::bind(&ScenedContext::Handle_QuestHelp_Complete, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTHELP_COMPLETE].status = STATUS_NEVER;
	opcode_handler_[CMSG_QUESTHELP_UPDATE_STATUS].handler = std::bind(&ScenedContext::Handle_QuestHelp_Update_Status, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTHELP_UPDATE_STATUS].status = STATUS_NEVER;
	opcode_handler_[CMSG_QUESTHELP_TALK_OPTION].handler = std::bind(&ScenedContext::Handle_QuestHelp_Talk_Option, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTHELP_TALK_OPTION].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_QUESTHELP_QUERY_BOOK].handler = std::bind(&ScenedContext::Handle_QuestHelp_Query_Book, this, std::placeholders::_1);
	opcode_handler_[CMSG_QUESTHELP_QUERY_BOOK].status = STATUS_LOGGEDIN;
	opcode_handler_[MSG_QUERY_QUEST_STATUS].handler = std::bind(&ScenedContext::Handle_Query_Quest_Status, this, std::placeholders::_1);
	opcode_handler_[MSG_QUERY_QUEST_STATUS].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_INSTANCE_ENTER].handler = std::bind(&ScenedContext::Handle_Instance_Enter, this, std::placeholders::_1);
	opcode_handler_[CMSG_INSTANCE_ENTER].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_INSTANCE_EXIT].handler = std::bind(&ScenedContext::Handle_Instance_Exit, this, std::placeholders::_1);
	opcode_handler_[CMSG_INSTANCE_EXIT].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_INSTANCE_NEXT_STATE].handler = std::bind(&ScenedContext::Handle_Instance_Next_State, this, std::placeholders::_1);
	opcode_handler_[CMSG_INSTANCE_NEXT_STATE].status = STATUS_LOGGEDIN;
	
	opcode_handler_[CMSG_CLIENT_UPDATE_SCENED].handler = std::bind(&ScenedContext::Handle_Client_Update_Scened, this, std::placeholders::_1);
	opcode_handler_[CMSG_CLIENT_UPDATE_SCENED].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_LOOT_SELECT].handler = std::bind(&ScenedContext::Handle_Loot_Select, this, std::placeholders::_1);
	opcode_handler_[CMSG_LOOT_SELECT].status = STATUS_LOGGEDIN;

	opcode_handler_[MSG_CLIENTSUBSCRIPTION].handler = std::bind(&ScenedContext::Hanlde_Client_Subscription, this, std::placeholders::_1);
	opcode_handler_[MSG_CLIENTSUBSCRIPTION].status = STATUS_LOGGEDIN;
	opcode_handler_[MSG_CLIENTSUBSCRIPTION].cooldown = 800;

}

//////////////////////////////////////////////////////////////////////////
//fro scened_context

ScenedContext::ScenedContext(int fd,ContextManager *mgr):SvrContext(fd,mgr),m_player(NULL)
{
	OpcodeHandlerInitor();
}

//操作失败
void ScenedContext::Call_Operation_Failed(uint8 type,uint8 reason)
{
	packet *_pkt;
	pack_operation_failed(&_pkt,type,reason,"");
	update_packet_len(_pkt);
	SendPacket(*_pkt);
	external_protocol_free_packet(_pkt);
}

bool ScenedContext::IsCanDisMove(uint16 map_id)
{
	for(uint16 i = 0; i < g_Config.can_move_dis_map.size(); i++)
	{
		if ( g_Config.can_move_dis_map[i] == map_id)
			return true;
	}

	return false;
}

void ScenedContext::Handle_LUA(packet& pkt)
{
	//DoOpcodeHandler(this->m_player, pkt.head->optcode, &pkt);
	do_on_external_packet(this->m_player,&pkt);
}

//同步时间
void ScenedContext::Handle_Syns_Mstime(packet& pkt)
{
	uint32 time_now = ms_time();
	packet *_pkt;
	pack_sync_mstime(&_pkt,time_now,(uint32)time(NULL),ScenedApp::g_app->m_open_time);
	SendPacket(*_pkt);
	external_protocol_free_packet(_pkt);
	tea_pdebug("THE LOGIN TIME IS %d",time_now);
}

void ScenedContext::Handle_Client_Update_Scened(packet& pkt)
{
	if(!m_player)
		return;
	m_player->GetMap()->SendGridUnitFromPlayer(m_player);
}

void ScenedContext::Handle_Loot_Select(packet& pkt)
{	
	uint16 x, y;
	unpack_loot_select(&pkt, &x, &y);
	double count = 0;
	//如果x,y都是-1则是批量捡钱
	if(x == uint16(-1) && y == uint16(-1))
	{
		int loot_area = g_Config.loot_area;

		int x = int(m_player->GetPositionX());
		int y = int(m_player->GetPositionY());
		x = x - x%loot_area;
		y = y - y%loot_area;

		std::map<uint32, double> moneyHash;

		//10码范围内可以捡
		for (int i = x - 10; i < x + 10; i = i + loot_area)
		{
			for (int j = y - 10; j < y + 10; j = j + loot_area)
			{
				auto *grid = m_player->GetMap()->GetGridManager()->GridCoordPtr(i,j);
				if(!grid || !grid->loot)
					continue;

				int index = grid->loot->GetLootIndex(i, j);
				if(index < 0)
					continue;

				uint32 entry = grid->loot->GetEntry(index);
				uint32 money_type;
				bool ret = DoGetMoneyType(entry, money_type);
				if(!ret)
					continue;

				count = 0;
				if (moneyHash.find(money_type) != moneyHash.end()) {
					count = moneyHash[money_type];
				}
				grid->loot->PickUpLoot(this,index,count);
				moneyHash[money_type] = count;
			}
		}
		string p1 = "";
		for (auto it : moneyHash) {
			ScenedApp::g_app->call_player_addmoney(this->GetGuid(),it.first, MONEY_CHANGE_SELECT_LOOT, it.second, p1, 0, 0, 0, 0);
		}
	}
	else
	{

		Grid *grid = m_player->GetMap()->GetGridManager()->GridCoordPtr(x, y);
		if(!grid || !grid->loot)
			return;

		// 距离不能太远
		if (m_player->GetDistance(x, y) > 2) {
			return;
		}

		int index = grid->loot->GetLootIndex(x, y);
		// 位置一定要在范围
		if (index < 0) {
			return;
		}
		//////////////////////////////////////////////////////////////////////////		
		uint32 entry = grid->loot->GetEntry(index);
		grid->loot->PickUpLoot(this, index,count);
		if (entry > 0) {
			uint32 money_type;
			bool ret = DoGetMoneyType(entry, money_type);
			//如果是资源
			if(ret && count > 0)
			{
				string p1 = "";
				ScenedApp::g_app->call_player_addmoney(this->GetGuid(),money_type, MONEY_CHANGE_SELECT_LOOT, count, p1, 0, 0, 0, 0);	
			}
		}
	}
}
