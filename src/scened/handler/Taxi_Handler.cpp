#include "scened_context.h"
#include "Player.h"
#include "Map.h"
#include "Creature.h"
#include "scened_scripts.h"
#include <shared/log_handler.h>

void ScenedContext::Handle_Taxi_Hello(packet& pkt)
{
	uint32 npc_guid;
	if(unpack_taxi_hello(&pkt,&npc_guid))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_TAXI_HELLO,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	Call_Taxi_Station_List(npc_guid);
} 

//传送点选择
void ScenedContext::Handle_Taxi_Select_Station(packet& pkt)
{
	uint32 station_id;
	uint32 npc_guid;
	if (unpack_taxi_select_station(&pkt,&station_id,&npc_guid))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_TAXI_SELECT_STATION,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	/*TODO
	telport_db.add_select_condition(MEMDB_EQ(telport,id,station_id)); //搜索条件生物模板ID
	MemDB_Result<telport> telport_result = telport_db.select();
	if (telport_result.size())
	{
		m_player->GetSession()->Teleport(float(telport_result[0]->pos_x),float(telport_result[0]->pos_y),telport_result[0]->map_id,"");
	}
	*/
}

//发送传送点列表
void ScenedContext::Call_Taxi_Station_List(uint32 npc_guid)
{
	Creature* obj=(Creature*)m_player->GetMap()->FindCreature(npc_guid);
	if (obj)
	{
		vector<gossip_menu_option_info> gossip_menu;
		vector<taxi_menu_info> taxi_menu;

		/*TODO
		//搜索条件生物模板ID
		telport_db.add_select_condition(MEMDB_EQ(telport,template_id, obj->GetTemplate().id)); 
		MemDB_Result<telport> telport_result = telport_db.select();
		for(uint32 j = 0; j < telport_result.size(); j ++)
		{
			taxi_menu_info info ;
			info.id = telport_result[j]->id;
			info.map_id = telport_result[j]->map_id;
			info.pos_x = telport_result[j]->pos_x;
			info.pos_y = telport_result[j]->pos_y;
			strcpy(info.taxi_text , telport_result[j]->name);
			taxi_menu.push_back(info);
		}
		*/

		packet *_pkt = external_protocol_new_packet(SMSG_TAXI_STATIONS_LIST);
		pack_taxi_stations_list(&_pkt,npc_guid,(taxi_menu.size())?&taxi_menu[0]:NULL,taxi_menu.size());
		SendPacket(*_pkt);
		external_protocol_free_packet(_pkt);
	}
}

