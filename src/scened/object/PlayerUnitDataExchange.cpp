#include "Map.h"
#include "Player.h"
#include "QuestGetter.h"
#include <core_obj/BinLogStru.h>

void Player::UnitToPlayerUint32(uint32 index, uint32 player_index)
{
	if(mask_.GetBit(index) && m_session->GetUInt32(player_index) != GetUInt32(index))
		m_session->SetUInt32(player_index, GetUInt32(index));
}
void Player::UnitToPlayerDouble(uint32 index, uint32 player_index)
{
	if(mask_.GetBit(index) && m_session->GetDouble(player_index) != GetDouble(index))
		m_session->SetDouble(player_index, GetDouble(index));
}
void Player::UnitToPlayerString(uint32 index, uint32 player_index)
{
	if(mask_string_.GetBit(index) && m_session->GetStr(player_index) != GetStr(index))
		m_session->SetStr(player_index, GetStr(index));
}
void Player::UnitByteToPlayerUInt32(uint32 index, uint8 offset, uint32 player_index)
{
	if(mask_.GetBit(index) && m_session->GetUInt32(player_index) != GetByte(index, offset))
		m_session->SetUInt32(player_index, GetByte(index, offset));
}
void Player::UnitUInt16ToPlayerUInt32(uint32 index, uint8 offset, uint32 player_index)
{
	if(mask_.GetBit(index) && m_session->GetUInt32(player_index) != GetUInt16(index, offset))
		m_session->SetUInt32(player_index, GetUInt16(index, offset));
}
void Player::UnitUInt32ToPlayerDouble(uint32 index, uint32 player_index)
{
	if(mask_.GetBit(index) && m_session->GetDouble(player_index) != GetUInt32(index))
		m_session->SetDouble(player_index, GetUInt32(index));
}
void Player::UnitByteToPlayerByte(uint32 index, uint8 offset, uint32 player_index, uint8 player_offset)
{
	if(mask_.GetBit(index) && m_session->GetByte(player_index, player_offset) != GetByte(index, offset))
		m_session->SetByte(player_index, player_offset, GetByte(index, offset));
}
void Player::UnitByteToPlayerDouble(uint32 index,uint8 offset, uint32 player_index)
{
	if(mask_.GetBit(index) && m_session->GetDouble(player_index) != GetByte(index, offset))
		m_session->SetDouble(player_index, GetByte(index, offset));
}
void Player::UnitUInt16ToPlayerDouble(uint32 index,uint8 offset, uint32 player_index)
{
	if(mask_.GetBit(index) && m_session->GetDouble(player_index) != GetUInt16(index, offset))
		m_session->SetDouble(player_index, GetUInt16(index, offset));
}
inline void SET_VALUE(BinLogObject* obj, uint32 index, uint32 val)
{
	if (obj && obj->GetUInt32(index) != val)
	{
		obj->SetUInt32(index, val);
	}	
}

inline void SET_STRING(BinLogObject* obj, uint32 index, const string &val) {
	if (obj && obj->GetStr(index) != val)
	{
		obj->SetStr(index, val);
	}
}

inline void SET_DOUBLE(BinLogObject* obj, uint32 index, double val)
{
	if (obj && obj->GetDouble(index) != val)
	{
		obj->SetDouble(index, val);
	}	
}

inline void SET_BYTE(BinLogObject* obj, uint32 index, uint8 offset, uint8 val)
{
	if (obj && obj->GetByte(index, offset) != val)
	{
		obj->SetByte(index, offset, val);
	}	
}

inline void SET_UINT16(BinLogObject* obj, uint32 index, uint8 offset, uint16 val)
{
	if (obj && obj->GetUInt16(index, offset) != val)
	{
		obj->SetUInt16(index, offset, val);
	}	
}

inline void SET_BIT_VALUE(BinLogObject* obj, uint32 index, uint8 offset, bool b)
{
	if (obj && obj->GetBit(index, offset) != b)
	{
		if (b)
			obj->SetBit(index, offset);
		else
			obj->UnSetBit(index, offset);
	}
}

char Player::m_tmp[128];					//精灵ID转换用的

void Player::OnAfterPlayerDataUpdate(SyncEventRecorder *data,int flags,UpdateMask& mask,UpdateMask& str_mask)
{
	auto *self = dynamic_cast<ScenedContext*>(data);
	ASSERT(self == m_session);
	bool is_new = (flags & OBJ_OPT_NEW) != 0;

	//如果是创建包
	if(is_new)
	{
		//字符串部分
		SetName(self->GetName());
		//经验
		m_player_exp = GetSession()->GetExp();
		//int部分
		SET_UINT16(this, UNIT_FIELD_UINT16_1, 0, (uint16)self->GetUInt32(PLAYER_FIELD_MAP_ID));	
		this->SetPosition(self->GetPositionX(), self->GetPositionY());
		this->SetOrientation(self->GetOrientation());
		SET_BYTE(this, UNIT_FIELD_BYTE_0, 2, (uint8)self->GetDouble(PLAYER_FIELD_MOVESPEED));
		SET_UINT16(this, UNIT_FIELD_UINT16_0, 1, (uint16)self->GetUInt32(PLAYER_FIELD_LEVEL));
		SET_VALUE(this, UNIT_FIELD_BYTE_1, self->GetUInt32(PLAYER_FIELD_BYTES_0));
		SET_BYTE(this, UNIT_FIELD_BYTE_0, 1, (uint8)self->GetUInt32(PLAYER_FIELD_DEATH_STATE));
		SET_BYTE(this, UNIT_FIELD_BYTE_4, 2, (uint8)self->GetUInt32(PLAYER_FIELD_SKIN));
		SET_BYTE(this, UNIT_FIELD_BYTE_4, 1, (uint8)self->GetUInt32(PLAYER_FIELD_CUR_MOUNT));

		SET_BYTE(this, UNIT_FIELD_BYTE_3, 0, (uint8)self->GetByte(PLAYER_FIELD_BYTES_3, 0));
		SET_BYTE(this, UNIT_FIELD_BYTE_3, 1, (uint8)self->GetByte(PLAYER_FIELD_BYTES_3, 1));
		SET_BYTE(this, UNIT_FIELD_BYTE_3, 2, (uint8)self->GetByte(PLAYER_FIELD_BYTES_5, 0));
		SET_BYTE(this, UNIT_FIELD_BYTE_3, 3, (uint8)self->GetByte(PLAYER_FIELD_BYTES_3, 3));
		//同步flags
		SET_VALUE(this, UNIT_FIELD_FLAGS, self->GetUInt32(PLAYER_FIELD_FLAGS));
		
		SET_VALUE(this, UNIT_FIELD_ANGER, self->GetUInt32(PLAYER_FIELD_ANGER));
		// 同步虚拟阵营
		SET_VALUE(this, UINT_FIELD_VIRTUAL_CAMP, self->GetUInt32(PLAYER_INT_FIELD_VIRTUAL_CAMP));
		//VIP等级
		SET_VALUE(this, UINT_FIELD_VIP_LEVEL, self->GetUInt32(PLAYER_FIELD_VIP_LEVEL));
		//VIP超时时间
		SET_VALUE(this, UINT_FIELD_VIP_TIME_OUT, self->GetUInt32(PLAYER_FIELD_VIP_TIME_OUT));	
		
		for (uint32 i = 0; i < MAX_UNIT_BUFF; i++)
		{
			//buff槽
			SET_VALUE(this, UNIT_FIELD_BUFF + i, self->GetUInt32(PLAYER_FIELD_BUFF + i));
			//buff预留值
			SET_VALUE(this, UNIT_FIELD_BUFF_RESERVE + i, self->GetUInt32(PLAYER_FIELD_BUFF_RESERVE + i));
			//buff施法者guid
			SET_VALUE(this, UNIT_FIELD_BUFF_CASTER_GUID + i, self->GetUInt32(PLAYER_INT_FIELD_BUFF_GIVER_UINT_GUID + i));
		}
		//end
		//buff等级
		for (uint32 i = 0; i < MAX_UNIT_BUFF; i++)
		{
			SET_VALUE(this, UNIT_FIELD_BUFF_EFFECT_ID + i, self->GetUInt32(PLAYER_INT_FIELD_BUFF_EFFECT_ID_START + i));
		}
		//end

		/**
		 *	同步重要技能CD
		 */
		//重要技能cd
		for (uint32 i = PLAYER_INT_FIELD_IMPORTANT_SPELL_CD_START; i < PLAYER_INT_FIELD_IMPORTANT_SPELL_CD_END; i += MAX_IMPORTANT_SPELL_ATTR_COUNT) {

			uint32 spellId = self->GetUInt32(i + IMPORTANT_SPELL_ID);
			if (spellId == 0) {
				break;
			}
			uint32 spellCd = self->GetUInt32(i + IMPORTANT_SPELL_CD);
			spellCd = this->TimestampToServerRuntime(spellCd);
			m_importance_spell_cd[spellId] = spellCd;
		}
		//end
	}
	
	{	//需要更新的部分

		if (str_mask.GetBit(BINLOG_STRING_FIELD_NAME) || is_new)
			SetName(self->GetName());

		if (str_mask.GetBit(PLAYER_STRING_FIELD_FACTION_GUID) || is_new)
			SET_STRING(this, UNIT_STRING_FIELD_FACTION_GUID, self->GetStr(PLAYER_STRING_FIELD_FACTION_GUID));

		if (str_mask.GetBit(PLAYER_STRING_FIELD_GROUP_PEACE_ID) || is_new)
			SET_STRING(this, UNIT_STRING_FIELD_GROUP_PEACE_ID, self->GetStr(PLAYER_STRING_FIELD_GROUP_PEACE_ID));

		if(mask.GetBit(PLAYER_INT_FIELD_APPEARANCE) || is_new)
			SET_VALUE(this, UNIT_INT_FIELD_APPEARANCE, self->GetUInt32(PLAYER_INT_FIELD_APPEARANCE));
		
		if(mask.GetBit(PLAYER_FIELD_MAP_ID) || is_new)
			SET_UINT16(this, UNIT_FIELD_UINT16_1, 0, (uint16)self->GetUInt32(PLAYER_FIELD_MAP_ID));		
		if(mask.GetBit(PLAYER_FIELD_BYTES_0) || is_new)
			SET_VALUE(this, UNIT_FIELD_BYTE_1, self->GetUInt32(PLAYER_FIELD_BYTES_0));
		if(mask.GetBit(PLAYER_FIELD_CUR_MOUNT) || is_new)
			SET_BYTE(this, UNIT_FIELD_BYTE_4, 1, (uint8)self->GetUInt32(PLAYER_FIELD_CUR_MOUNT));
		if(mask.GetBit(PLAYER_INT_FIELD_KUAFU_NUMBER) || is_new)
			SET_VALUE(this, UNIT_FIELD_UINT16_2, self->GetUInt32(PLAYER_INT_FIELD_KUAFU_NUMBER));

		if (mask.GetBit(PLAYER_INT_FIELD_MOUNT_LEVEL) || is_new)
			SET_VALUE(this, UNIT_FIELD_MOUNT_LEVEL, self->GetUInt32(PLAYER_INT_FIELD_MOUNT_LEVEL));

		if (mask.GetBit(PLAYER_INT_FIELD_APPEARANCE) || is_new) {
			SET_VALUE(this, UNIT_FIELD_DIVINE_ID, self->GetUInt16(PLAYER_INT_FIELD_APPEARANCE, 0));
			SET_VALUE(this, UNIT_FIELD_EQUIPMENT_COAT, self->GetUInt16(PLAYER_INT_FIELD_APPEARANCE, 1));
		}

		//属性赋值
		if (mask.GetBit(PLAYER_FIELD_FORCE) || is_new)
			SET_DOUBLE(this, UNIT_FIELD_FORCE, self->GetDouble(PLAYER_FIELD_FORCE));

		bool recal = false;
		// 血量只会在第一次的时候赋值
		uint32 pIndex = PLAYER_FIELD_HEALTH;
		for (uint32 attrIndex = UNIT_FIELD_HEALTH; attrIndex < UNIT_FIELD_ATTRIBUTE_END; ++ attrIndex) {
			if (mask.GetBit(pIndex) || mask.GetBit(pIndex+1) || is_new) {
				SET_VALUE(this, attrIndex, (uint32)self->GetDouble(pIndex));
				recal = true;
				if (attrIndex == UNIT_FIELD_HEALTH) {
					tea_pdebug("p => u sync player health %d\n", this->GetUInt32(attrIndex));
				}
			}
			pIndex += 2;
		}
		/*
		SET_VALUE(this, UNIT_FIELD_HEALTH, (uint32)self->GetDouble(PLAYER_FIELD_HEALTH));
		SET_VALUE(this, UNIT_FIELD_MAXHEALTH, (uint32)self->GetDouble(PLAYER_FIELD_MAXHEALTH));
		SET_VALUE(this, UNIT_FIELD_DAMAGE, (uint32)self->GetDouble(PLAYER_FIELD_DAMAGE));
		SET_VALUE(this, UNIT_FIELD_ARMOR, (uint32)self->GetDouble(PLAYER_FIELD_ARMOR));
		SET_VALUE(this, UNIT_FIELD_HIT, (uint32)self->GetDouble(PLAYER_FIELD_HIT));
		SET_VALUE(this, UNIT_FIELD_DODGE, (uint32)self->GetDouble(PLAYER_FIELD_DODGE));
		SET_VALUE(this, UNIT_FIELD_CRIT, (uint32)self->GetDouble(PLAYER_FIELD_CRIT));
		SET_VALUE(this, UNIT_FIELD_TOUGH, (uint32)self->GetDouble(PLAYER_FIELD_TOUGH));
		SET_VALUE(this, UNIT_FIELD_ATTACK_SPEED, (uint32)self->GetDouble(PLAYER_FIELD_ATTACK_SPEED));
		SET_VALUE(this, UNIT_FIELD_MOVE_SPEED, (uint32)self->GetDouble(PLAYER_FIELD_MOVE_SPEED));
		SET_VALUE(this, UNIT_FIELD_AMPLIFY_DAMAGE, (uint32)self->GetDouble(PLAYER_FIELD_AMPLIFY_DAMAGE));
		SET_VALUE(this, UNIT_FIELD_IGNORE_DEFENSE, (uint32)self->GetDouble(PLAYER_FIELD_IGNORE_DEFENSE));
		SET_VALUE(this, UNIT_FIELD_DAMAGE_RESIST, (uint32)self->GetDouble(PLAYER_FIELD_DAMAGE_RESIST));
		SET_VALUE(this, UNIT_FIELD_DAMAGE_RETURNED, (uint32)self->GetDouble(PLAYER_FIELD_DAMAGE_RETURNED));
		SET_VALUE(this, UNIT_FIELD_HIT_RATE, (uint32)self->GetDouble(PLAYER_FIELD_HIT_RATE));
		SET_VALUE(this, UNIT_FIELD_DODGE_RATE, (uint32)self->GetDouble(PLAYER_FIELD_DODGE_RATE));
		SET_VALUE(this, UNIT_FIELD_CRIT_RATE , (uint32)self->GetDouble(PLAYER_FIELD_CRIT_RATE));
		SET_VALUE(this, UNIT_FIELD_CRITICAL_RESIST_RATE , (uint32)self->GetDouble(PLAYER_FIELD_CRITICAL_RESIST_RATE));
		SET_VALUE(this, UNIT_FIELD_DAMAGE_CRIT_MULTIPLE , (uint32)self->GetDouble(PLAYER_FIELD_DAMAGE_CRIT_MULTIPLE));
		SET_VALUE(this, UNIT_FIELD_RESIST_CRIT_MULTIPLE , (uint32)self->GetDouble(PLAYER_FIELD_RESIST_CRIT_MULTIPLE));
		*/
		//技能槽的技能
		for (uint32 i = PLAYER_INT_FIELD_SPELL_START; i < PLAYER_INT_FIELD_SPELL_END; ++ i) {
			if (mask.GetBit(i) || is_new) {
				uint16 spellId = self->GetUInt16(i, 0);
				if (spellId == 0) {
					continue;
				}
				uint8 spellLv = self->GetByte(i, 2);
				uint8 slot    = self->GetByte(i, 3);

				this->slotReplaceSpell(slot, spellId, spellLv);
			}
		}

		//如果有被动技能
		for (uint32 i = PLAYER_INT_FIELD_PASSIVE_SPELL_START, j = UINT_INT_FIELD_PASSIVE_SPELL_START; i < PLAYER_INT_FIELD_PASSIVE_SPELL_END; ++ i, ++ j) {
			if (mask.GetBit(i) || is_new) {
				SET_VALUE(this, j, (uint32)self->GetUInt32(i));
				
				/**
				uint16 spellId = self->GetUInt16(i, 0);
				if (spellId == 0) {
					// 可能需要删除
					auto it = this->m_index_spellId.find(i);
					if (it != this->m_index_spellId.end()) {
						uint16 spellId = it->second;
						this->m_index_spellId.erase(i);
						this->m_passive_spell_level.erase(spellId);
					}

					continue;
				}

				uint16 spelllv = self->GetUInt16(i, 1);

				this->m_index_spellId[ i ] = spellId;

				this->m_passive_spell_level[spellId] = (uint8)spelllv;
				*/
			}
		}

		if (mask.GetBit(PLAYER_FIELD_BYTES_2) || is_new)
			SET_VALUE(this, UNIT_FIELD_BYTE_2, self->GetUInt32(PLAYER_FIELD_BYTES_2));
		if(mask.GetBit(PLAYER_FIELD_FLAGS) || is_new)
			SET_VALUE(this, UNIT_FIELD_FLAGS, self->GetUInt32(PLAYER_FIELD_FLAGS));

		/**
		if(mask.GetBit(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_COAT) || is_new) {
			// 没有时装
			if (self->GetUInt32(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_FASHION) == 0) {
				SET_VALUE(this, UNIT_FIELD_EQUIPMENT_COAT, self->GetUInt32(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_COAT));
			}
		}

		if(mask.GetBit(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_FASHION) || is_new) {
			// 穿时装
			if (self->GetUInt32(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_FASHION) > 0) {
				SET_VALUE(this, UNIT_FIELD_EQUIPMENT_COAT, self->GetUInt32(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_FASHION));
			// 脱时装
			} else {
				SET_VALUE(this, UNIT_FIELD_EQUIPMENT_COAT, self->GetUInt32(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_COAT));
			}
		}
		*/
		
		if(mask.GetBit(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_MAIN_WEAPON) || is_new)
			SET_VALUE(this, UNIT_FIELD_EQUIPMENT_MAIN_WEAPON, self->GetUInt32(PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_MAIN_WEAPON));

		//神羽阶数
		if(mask.GetBit(PLAYER_INT_FIELD_WINGS_RANK) || is_new) {
			SET_VALUE(this, UINT_INT_FIELD_WINGS_RANK, self->GetUInt32(PLAYER_INT_FIELD_WINGS_RANK));
		}

		// 应用服经验
		if(mask.GetBit(PLAYER_EXPAND_INT_XP_ADDED) || is_new) {
			 double added = self->GetDouble(PLAYER_EXPAND_INT_XP_ADDED);
			 double picked = self->GetDouble(PLAYER_EXPAND_INT_XP_PICKED);
			 if (added > picked) {
				this->GainExp(added-picked,0);
				self->AddDouble(PLAYER_EXPAND_INT_XP_PICKED, added-picked);
			 }
		}

		// 进行属性重算
		if (recal) {
			this->SetCalculAttrStage(true);
		}
	}

	if (is_new)
		Clear();
}

uint32 Player::TimestampToServerRuntime(uint32 value) {

	uint32 timestamp = (uint32)time(NULL);
	uint32 serverRuntime = ms_time();

	if (value <= timestamp) {
		return serverRuntime;
	}

	return value - timestamp + serverRuntime; 
}

uint32 Player::ServerRuntimeToTimestamp(uint32 value) {
	
	uint32 timestamp = (uint32)time(NULL);
	uint32 serverRuntime = ms_time();

	if (value <= serverRuntime) {
		return timestamp;
	}

	return value - serverRuntime + timestamp;
}

//因为UNIT设置为主模式，所以是对象更新机制,那么就可以从mask_里面拿到更新信息
void Player::SyncUnitToPlayerData()
{
	if(m_need_sync_pos)
	{
		if(GetSession()->GetPositionX() != m_positionX || GetSession()->GetPositionY() != m_positionY)
		{
			GetSession()->SetPosition(m_positionX, m_positionY);
			GetSession()->SetOrientation(m_orientation);
		}
		m_need_sync_pos = false;
	}

	UnitUInt16ToPlayerUInt32(UNIT_FIELD_UINT16_0, 1, PLAYER_FIELD_LEVEL);
	UnitToPlayerUint32(UNIT_FIELD_BYTE_1, PLAYER_FIELD_BYTES_0);
	UnitByteToPlayerUInt32(UNIT_FIELD_BYTE_0, 1, PLAYER_FIELD_DEATH_STATE);
	UnitByteToPlayerUInt32(UNIT_FIELD_BYTE_4, 2, PLAYER_FIELD_SKIN);
	UnitToPlayerUint32(UNIT_FIELD_BYTE_2, PLAYER_FIELD_BYTES_2);

	UnitToPlayerUint32(UNIT_FIELD_MOUNT_LEVEL, PLAYER_INT_FIELD_MOUNT_LEVEL);
	//UnitToPlayerUint32(UNIT_FIELD_DIVINE_ID, PLAYER_INT_FIELD_DIVINE_ID);
	UnitToPlayerUint32(UNIT_FIELD_ANGER, PLAYER_FIELD_ANGER);

	UnitByteToPlayerByte(UNIT_FIELD_BYTE_3, 0, PLAYER_FIELD_BYTES_3, 0);
	UnitByteToPlayerByte(UNIT_FIELD_BYTE_3, 1, PLAYER_FIELD_BYTES_3, 1);
	UnitByteToPlayerByte(UNIT_FIELD_BYTE_3, 3, PLAYER_FIELD_BYTES_3, 3);
	UnitByteToPlayerByte(UNIT_FIELD_BYTE_3, 2, PLAYER_FIELD_BYTES_5, 0);
	UnitByteToPlayerDouble(UNIT_FIELD_BYTE_0, 2, PLAYER_FIELD_MOVESPEED);

	//属性
	UnitToPlayerDouble(UNIT_FIELD_FORCE, PLAYER_FIELD_FORCE);
	/*
	UnitUInt32ToPlayerDouble(UNIT_FIELD_HEALTH, PLAYER_FIELD_HEALTH);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_MAXHEALTH, PLAYER_FIELD_MAXHEALTH);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_DAMAGE, PLAYER_FIELD_DAMAGE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_ARMOR, PLAYER_FIELD_ARMOR);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_HIT, PLAYER_FIELD_HIT);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_DODGE, PLAYER_FIELD_DODGE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_CRIT, PLAYER_FIELD_CRIT);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_TOUGH, PLAYER_FIELD_TOUGH);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_ATTACK_SPEED, PLAYER_FIELD_ATTACK_SPEED);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_MOVE_SPEED, PLAYER_FIELD_MOVE_SPEED);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_AMPLIFY_DAMAGE, PLAYER_FIELD_AMPLIFY_DAMAGE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_IGNORE_DEFENSE, PLAYER_FIELD_IGNORE_DEFENSE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_DAMAGE_RESIST, PLAYER_FIELD_DAMAGE_RESIST);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_DAMAGE_RETURNED, PLAYER_FIELD_DAMAGE_RETURNED);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_HIT_RATE, PLAYER_FIELD_HIT_RATE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_DODGE_RATE, PLAYER_FIELD_DODGE_RATE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_CRIT_RATE , PLAYER_FIELD_CRIT_RATE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_CRITICAL_RESIST_RATE , PLAYER_FIELD_CRITICAL_RESIST_RATE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_DAMAGE_CRIT_MULTIPLE , PLAYER_FIELD_DAMAGE_CRIT_MULTIPLE);
	UnitUInt32ToPlayerDouble(UNIT_FIELD_RESIST_CRIT_MULTIPLE , PLAYER_FIELD_RESIST_CRIT_MULTIPLE);
	*/

	//同步flags
	UnitToPlayerUint32(UNIT_FIELD_FLAGS, PLAYER_FIELD_FLAGS);
	//buff相关
	for (uint32 i = UNIT_FIELD_BUFF, j = 0; i < UNIT_FIELD_BUFF + MAX_UNIT_BUFF; i++, j++)
	{
		UnitToPlayerUint32(i, PLAYER_FIELD_BUFF + j);
	}
	for (uint32 i = UNIT_FIELD_BUFF_EFFECT_ID, j = 0; i < UNIT_FIELD_BUFF_EFFECT_ID + MAX_UNIT_BUFF; i++, j++)
	{
		UnitToPlayerUint32(i, PLAYER_INT_FIELD_BUFF_EFFECT_ID_START + j);
	}
	for (uint32 i = UNIT_FIELD_BUFF_RESERVE, j = 0; i < UNIT_FIELD_BUFF_RESERVE + MAX_UNIT_BUFF; i++, j++)
	{
		UnitToPlayerUint32(i, PLAYER_FIELD_BUFF_RESERVE + j);
	}
	for (uint32 i = UNIT_FIELD_BUFF_CASTER_GUID, j = 0; i < UNIT_FIELD_BUFF_CASTER_GUID + MAX_UNIT_BUFF; i++, j++)
	{
		UnitToPlayerUint32(i, PLAYER_INT_FIELD_BUFF_GIVER_UINT_GUID + j);
	}
}

//unit同步到player
void Player::SyncCacheData()
{
	double curr = (double)this->GetUInt32(UNIT_FIELD_HEALTH);
	double real = m_session->GetDouble(PLAYER_FIELD_MAX_HEALTH);
	double health = curr <= real ? curr : real; 
	// 只有血量不大于血量上限的时候同步
	m_session->SetDouble(PLAYER_FIELD_HEALTH, health);

	tea_pdebug("u => p sync player health %d\n", this->GetUInt32(UNIT_FIELD_HEALTH));

	//同步重要技能cd
	//先把以前存cd的技能CD的index和最近一个空位置取出来
	std::map<uint32, uint32> hash;
	uint32 lastIndex = 0;
	for (uint32 i = PLAYER_INT_FIELD_IMPORTANT_SPELL_CD_START; i < PLAYER_INT_FIELD_IMPORTANT_SPELL_CD_END; i += MAX_IMPORTANT_SPELL_ATTR_COUNT) {

		uint32 spellId = m_session->GetUInt32(i + IMPORTANT_SPELL_ID);
		if (spellId == 0) {
			lastIndex = i;
			break;
		}
		hash[spellId] = i;
	}
	
	//遍历要存的技能cd
	for (auto it:m_importance_spell_cd)
	{
		uint32 spellId = it.first;
		uint32 spellCd = it.second;
		spellCd = this->ServerRuntimeToTimestamp(spellCd);
		auto iter = hash.find(spellId);
		//以前没设置过
		if (iter == hash.end()) {
			// 超过上限就不加了, 免得影响后面数据
			if (lastIndex >= PLAYER_INT_FIELD_IMPORTANT_SPELL_CD_END) {
				tea_perror("need save skill is larger than %d", PLAYER_IMPORTANT_CD_COUNT);
				continue;
			}
			m_session->SetUInt32(lastIndex + IMPORTANT_SPELL_ID, spellId);
			m_session->SetUInt32(lastIndex + IMPORTANT_SPELL_CD, spellCd);
			lastIndex += MAX_IMPORTANT_SPELL_ATTR_COUNT;
		}
		else {
			//只设置cd即可
			m_session->SetUInt32(iter->second + IMPORTANT_SPELL_CD, spellCd);
		}
	}
}