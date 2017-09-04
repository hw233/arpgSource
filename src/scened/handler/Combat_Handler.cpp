#include "scened_app.h"
#include <object/SharedDef.h>
#include "scened_context.h"
#include "Player.h"
#include "Map.h"
#include "Creature.h"
#include "scened_scripts.h"
#include <shared/log_handler.h>
#include <object/OprateResult.h>

void ScenedContext::Call_Combat_State_Update(uint8 stat)
{
	packet *pkt;
	pack_combat_state_update(&pkt,stat);
	SendPacket(*pkt);
	external_protocol_free_packet(pkt);
}

void ScenedContext::Call_Peace_Mode_CD(uint8 cd)
{
	packet *pkt;
	pack_peace_mode_cd(&pkt,cd);
	SendPacket(*pkt);
	external_protocol_free_packet(pkt);
}

//设置攻击模式
void ScenedContext::Handle_Set_Attack_Mode(packet& pkt)		
{
	uint8 mode;
	uint64 reserve = 0;
	if (unpack_set_attack_mode(&pkt,&mode,&reserve))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_SET_ATTACK_MODE,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	if (mode>=MAX_ATTACK_MODE)
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_SET_ATTACK_MODE,ACCACK_PACKET_TYPE_DATA,"attack type error");
		return;
	}

	if(DoSetAttackModeCallback(m_player->GetMap(), m_player, mode) != 0)
	{
		m_player->SetAttackMode(mode);
		m_player->InvalidFriendlyCache();
	}
}


void ScenedContext::Handle_Select_Target(packet& pkt)			//选择目标
{
	uint32 victimid;

	if (unpack_select_target(&pkt,&victimid))
	{
		WriteAttackPacker(GetAccount(),m_lguid,pkt.head->optcode,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	Map * curmap = m_player->GetMap();
	
	if(!curmap){
		//tea_perror("找不到玩家GUID:%d 当前地图",m_player->GetGuid());
		return;
	}

	//tea_pdebug("Handle_Select_Target :%u",victimid);
	//m_player->SetTarget(NULL);	
	//Unit *unit = curmap->FindUnit(victimid);	
	//if (!unit){
	//	//tea_pdebug("找不到玩家GUID:%d 选择的目标GUID %d",m_player->GetGuid(),victimid);
	//	return;
	//}	
	//m_player->SetTarget(unit);

}

void ScenedContext::Handle_Set_Orient(packet& pkt) {
	uint16 angle;
	if (unpack_set_orient(&pkt, &angle)) {
		WriteAttackPacker(GetAccount(),m_lguid,MSG_SPELL_START,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	if (angle >= 360) {
		return;
	}

	m_player->SetOrientation(float(angle * M_PI / 180.0));
}

//-------------------------------------------------------------------------
//开始技能
void ScenedContext::Handle_Spell_Start(packet& pkt)			
{
	m_player->SetLastActionPktTime();
	uint32 spell_slot; 
	uint16 x,y;
	uint32 caster;
	uint32 target;
	if (unpack_spell_start(&pkt,&spell_slot,&x,&y,&caster,&target))
	{
		WriteAttackPacker(GetAccount(),m_lguid,MSG_SPELL_START,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	
	Map * curmap = m_player->GetMap();
	if(!curmap){
		tea_perror("找不到玩家GUID:%d 当前地图",m_player->GetUIntGuid());
		return;
	}
	/* 判断是否能使用技能 */
	if (!m_player->canUseSkill()) {
		tea_perror("two skills in interval %d", SKILL_INTERVAL);
		return;
	}
	Unit* targetUnit = curmap->FindUnit(target);
	uint32 spell_id = 0;
	//进lua判断开始施法前的逻辑
	if(DoHandleSpellStart(m_player, spell_slot, x, y, targetUnit, spell_id))
	{
		//处理开始攻击前需处理的逻辑，比如移除打坐buff
		if(OnStartAttack(m_player->GetMap(), m_player))
		{
			m_player->SetTarget(targetUnit);
			//设置技能释放间隔
			m_player->SetLastSpellTime();
			//施法
			m_player->CastSpell(spell_id, x, y);
		}
	}
}

//技能结束
void ScenedContext::Handle_Spell_Stop(packet& pkt)
{
	m_player->SetLastActionPktTime();
	m_player->SpellStop(true);
}

void ScenedContext::Handle_Resurrection(packet& pkt)
{
	uint8 type;
	uint64 reserve = 0;
	if (unpack_resurrection(&pkt,&type,&reserve))
	{
		WriteAttackPacker(GetAccount(),m_lguid,CMSG_RESURRECTION,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	//你活着
	if (m_player->isAlive())
	{
		tea_pwarn("[%s] isAlive Handle_Resurrection but!",m_player->GetName().c_str());
		return;
	}

	// 发送场景服使用复活丹
	string str = "";
	ScenedApp::g_app->call_player_do_something(m_player->GetSession()->GetGuid(), SCENED_APPD_USE_RESPAWN_ITEM, 0, str);


	////原地复活
	////m_player->Respawn();
	////如果不是原地复活则传送到最近复活点
	//uint16 x = (uint16)m_player->GetPositionX(),y = (uint16)m_player->GetPositionY();	
	//uint32 map_id = m_player->GetMapId();
	//if(RESURRECTION_SPAWNPOINT == type && !m_player->GetMap()->GetMapTemp()->GetSpwanPoint(x,y,m_player->GetFaction()))
	//{
	//	map_id = ZHUCHENG_DITU_ID;	
	//	vector<uint16> vect;
	//	DoGetRespawnPos(vect);
	//	if(vect.size() == 2)
	//	{
	//		x = vect.at(0);
	//		y = vect.at(1);
	//		//为防止玩家复活叠在一起,随机一下位置
	//		auto *mt = MapTemplate::GetMapTempalte(map_id);
	//		if(mt)
	//		{					
	//			float nx,ny;
	//			for (uint8 i=1; i<10; i++)
	//			{
	//				MovementGenerator::RandomPos(nx,ny,float(x),float(y),float(irand(1,10000)/1000));
	//				if(mt->IsCanRun(uint16(nx),uint16(ny)))
	//				{
	//					x = uint16(nx);
	//					y = uint16(ny);
	//					break;
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		x = ZHUCHENG_FUHUO_X;
	//		y = ZHUCHENG_FUHUO_Y;				
	//	}		
	//}	
	//DoRespawn(m_player,m_player->GetMap(),type,map_id,x,y);
}
