local protocols = require('share.protocols')

-- 获得当前生命
function PlayerInfo:GetHealth()
	return self:GetDouble(PLAYER_FIELD_HEALTH)
end

-- 获得最大生命
function PlayerInfo:GetMaxHealth()
	return self:GetDouble(PLAYER_FIELD_MAX_HEALTH)
end

-- 获得攻击力
function PlayerInfo:GetDamage()
	return self:GetDouble(PLAYER_FIELD_DAMAGE)
end

-- 获得防御力
function PlayerInfo:GetArmor()
	return self:GetDouble(PLAYER_FIELD_ARMOR)
end

-- 获得命中
function PlayerInfo:GetHit()
	return self:GetDouble(PLAYER_FIELD_HIT)
end

-- 获得闪避
function PlayerInfo:GetMiss()
	return self:GetDouble(PLAYER_FIELD_MISS)
end

-- 获得暴击
function PlayerInfo:GetCrit()
	return self:GetDouble(PLAYER_FIELD_CRIT)
end

-- 获得坚韧
function PlayerInfo:GetTough()
	return self:GetDouble(PLAYER_FIELD_TOUGH)
end

-- 获得攻击速度
function PlayerInfo:GetAttackSpeed()
	return self:GetDouble(PLAYER_FIELD_ATTACK_SPEED)
end

-- 获得移动速度
function PlayerInfo:GetMoveSpeed()
	return self:GetDouble(PLAYER_FIELD_MOVE_SPEED)
end

-- 获得忽视防御
function PlayerInfo:GetIgnoreArmor()
	return self:GetDouble(PLAYER_FIELD_IGNORE_ARMOR)
end

-- 获得忽视闪避
function PlayerInfo:GetIgnoreMiss()
	return self:GetDouble(PLAYER_FIELD_IGNORE_MISS)
end

-- 获得生命值回复
function PlayerInfo:GetRecovery()
	return self:GetDouble(PLAYER_FIELD_RECOVERY)
end

-- 获得伤害加深(万分比)
function PlayerInfo:GetDamageAmplifyRate()
	return self:GetDouble(PLAYER_FIELD_DAMAGE_AMPLIFY_RATE)
end

-- 获得伤害减免(万分比)
function PlayerInfo:GetDamageResistRate()
	return self:GetDouble(PLAYER_FIELD_DAMAGE_RESIST_RATE)
end

-- 获得反弹伤害(万分比)
function PlayerInfo:GetDamageReturnRate()
	return self:GetDouble(PLAYER_FIELD_DAMAGE_RETURN_RATE)
end

-- 获得吸血光环(万分比)
function PlayerInfo:GetVampiricRate()
	return self:GetDouble(PLAYER_FIELD_VAMPIRIC_RATE)
end

-- 获得回复效率(万分比)
function PlayerInfo:GetRecoveryRate()
	return self:GetDouble(PLAYER_FIELD_RECOVERY_RATE)
end

-- 获得暴击率(万分比)
function PlayerInfo:GetCritRate()
	return self:GetDouble(PLAYER_FIELD_CRIT_RATE)
end

-- 获得抗暴率(万分比)
function PlayerInfo:GetCritResistRate()
	return self:GetDouble(PLAYER_FIELD_CRIT_RESIST_RATE)
end

-- 获得暴击伤害倍数(万分比)
function PlayerInfo:GetCritDamRate()
	return self:GetDouble(PLAYER_FIELD_CRIT_DAM_RATE)
end

-- 获得降暴伤害倍数(万分比)
function PlayerInfo:GetCritResistDamRate()
	return self:GetDouble(PLAYER_FIELD_CRIT_RESIST_DAM_RATE)
end

-- 获得命中率(万分比)
function PlayerInfo:GetHitRate()
	return self:GetDouble(PLAYER_FIELD_HIT_RATE)
end

-- 获得闪避率(万分比)
function PlayerInfo:GetMissRate()
	return self:GetDouble(PLAYER_FIELD_MISS_RATE)
end

-- 获得眩晕
function PlayerInfo:GetStunRate()
	return self:GetDouble(PLAYER_FIELD_STUN_RATE)
end

-- 获得定身
function PlayerInfo:GetRootsRate()
	return self:GetDouble(PLAYER_FIELD_ROOTS_RATE)
end

-- 获得沉默
function PlayerInfo:GetSilenceRate()
	return self:GetDouble(PLAYER_FIELD_SILENCE_RATE)
end

-- 获得混乱
function PlayerInfo:GetChaosRate()
	return self:GetDouble(PLAYER_FIELD_CHAOS_RATE)
end

-- 获得魅惑
function PlayerInfo:GetCharmRate()
	return self:GetDouble(PLAYER_FIELD_CHARM_RATE)
end

-- 获得控制增强
function PlayerInfo:GetControlEnhanceRate()
	return self:GetDouble(PLAYER_FIELD_CONTROL_ENHANCE_RATE)
end

-- 获得控制减免
function PlayerInfo:GetControlResistRate()
	return self:GetDouble(PLAYER_FIELD_CONTROL_RESIST_RATE)
end

-- 获得强化护甲
function PlayerInfo:GetStrengthArmor()
	return self:GetDouble(PLAYER_FIELD_STRENGTH_ARMOR)
end

-- 设置当前生命
function PlayerInfo:SetHealth(val)
	self:SetDouble(PLAYER_FIELD_HEALTH, val)
end

-- 设置最大生命
function PlayerInfo:SetMaxHealth(val)
	self:SetDouble(PLAYER_FIELD_MAX_HEALTH, val)
end

-- 设置攻击力
function PlayerInfo:SetDamage(val)
	self:SetDouble(PLAYER_FIELD_DAMAGE, val)
end

-- 设置防御力
function PlayerInfo:SetArmor(val)
	self:SetDouble(PLAYER_FIELD_ARMOR, val)
end

-- 设置命中
function PlayerInfo:SetHit(val)
	self:SetDouble(PLAYER_FIELD_HIT, val)
end

-- 设置闪避
function PlayerInfo:SetMiss(val)
	self:SetDouble(PLAYER_FIELD_MISS, val)
end

-- 设置暴击
function PlayerInfo:SetCrit(val)
	self:SetDouble(PLAYER_FIELD_CRIT, val)
end

-- 设置坚韧
function PlayerInfo:SetTough(val)
	self:SetDouble(PLAYER_FIELD_TOUGH, val)
end

-- 设置攻击速度
function PlayerInfo:SetAttackSpeed(val)
	self:SetDouble(PLAYER_FIELD_ATTACK_SPEED, val)
end

-- 设置移动速度
function PlayerInfo:SetMoveSpeed(val)
	self:SetDouble(PLAYER_FIELD_MOVE_SPEED, val)
end

-- 设置忽视防御
function PlayerInfo:SetIgnoreArmor(val)
	self:SetDouble(PLAYER_FIELD_IGNORE_ARMOR, val)
end

-- 设置忽视闪避
function PlayerInfo:SetIgnoreMiss(val)
	self:SetDouble(PLAYER_FIELD_IGNORE_MISS, val)
end

-- 设置生命值回复
function PlayerInfo:SetRecovery(val)
	self:SetDouble(PLAYER_FIELD_RECOVERY, val)
end

-- 设置伤害加深(万分比)
function PlayerInfo:SetDamageAmplifyRate(val)
	self:SetDouble(PLAYER_FIELD_DAMAGE_AMPLIFY_RATE, val)
end

-- 设置伤害减免(万分比)
function PlayerInfo:SetDamageResistRate(val)
	self:SetDouble(PLAYER_FIELD_DAMAGE_RESIST_RATE, val)
end

-- 设置反弹伤害(万分比)
function PlayerInfo:SetDamageReturnRate(val)
	self:SetDouble(PLAYER_FIELD_DAMAGE_RETURN_RATE, val)
end

-- 设置吸血光环(万分比)
function PlayerInfo:SetVampiricRate(val)
	self:SetDouble(PLAYER_FIELD_VAMPIRIC_RATE, val)
end

-- 设置回复效率(万分比)
function PlayerInfo:SetRecoveryRate(val)
	self:SetDouble(PLAYER_FIELD_RECOVERY_RATE, val)
end

-- 设置暴击率(万分比)
function PlayerInfo:SetCritRate(val)
	self:SetDouble(PLAYER_FIELD_CRIT_RATE, val)
end

-- 设置抗暴率(万分比)
function PlayerInfo:SetCritResistRate(val)
	self:SetDouble(PLAYER_FIELD_CRIT_RESIST_RATE, val)
end

-- 设置暴击伤害倍数(万分比)
function PlayerInfo:SetCritDamRate(val)
	self:SetDouble(PLAYER_FIELD_CRIT_DAM_RATE, val)
end

-- 设置降暴伤害倍数(万分比)
function PlayerInfo:SetCritResistDamRate(val)
	self:SetDouble(PLAYER_FIELD_CRIT_RESIST_DAM_RATE, val)
end

-- 设置命中率(万分比)
function PlayerInfo:SetHitRate(val)
	self:SetDouble(PLAYER_FIELD_HIT_RATE, val)
end

-- 设置闪避率(万分比)
function PlayerInfo:SetMissRate(val)
	self:SetDouble(PLAYER_FIELD_MISS_RATE, val)
end

-- 设置眩晕
function PlayerInfo:SetStunRate(val)
	self:SetDouble(PLAYER_FIELD_STUN_RATE, val)
end

-- 设置定身
function PlayerInfo:SetRootsRate(val)
	self:SetDouble(PLAYER_FIELD_ROOTS_RATE, val)
end

-- 设置沉默
function PlayerInfo:SetSilenceRate(val)
	self:SetDouble(PLAYER_FIELD_SILENCE_RATE, val)
end

-- 设置混乱
function PlayerInfo:SetChaosRate(val)
	self:SetDouble(PLAYER_FIELD_CHAOS_RATE, val)
end

-- 设置魅惑
function PlayerInfo:SetCharmRate(val)
	self:SetDouble(PLAYER_FIELD_CHARM_RATE, val)
end

-- 设置控制增强
function PlayerInfo:SetControlEnhanceRate(val)
	self:SetDouble(PLAYER_FIELD_CONTROL_ENHANCE_RATE, val)
end

-- 设置控制减免
function PlayerInfo:SetControlResistRate(val)
	self:SetDouble(PLAYER_FIELD_CONTROL_RESIST_RATE, val)
end

-- 设置强化护甲
function PlayerInfo:SetStrengthArmor(val)
	self:SetDouble(PLAYER_FIELD_STRENGTH_ARMOR, val)
end

local PlayerInfo_Get_Attr_Func = {
	[EQUIP_ATTR_MAX_HEALTH] = PlayerInfo.GetMaxHealth,	--设置最大生命
	[EQUIP_ATTR_DAMAGE] = PlayerInfo.GetDamage,	--设置攻击力
	[EQUIP_ATTR_ARMOR] = PlayerInfo.GetArmor,	--设置防御力
	[EQUIP_ATTR_HIT] = PlayerInfo.GetHit,	--设置命中
	[EQUIP_ATTR_MISS] = PlayerInfo.GetMiss,	--设置闪避
	[EQUIP_ATTR_CRIT] = PlayerInfo.GetCrit,	--设置暴击
	[EQUIP_ATTR_TOUGH] = PlayerInfo.GetTough,	--设置坚韧
	[EQUIP_ATTR_ATTACK_SPEED] = PlayerInfo.GetAttackSpeed,	--设置攻击速度
	[EQUIP_ATTR_MOVE_SPEED] = PlayerInfo.GetMoveSpeed,	--设置移动速度
	[EQUIP_ATTR_IGNORE_ARMOR] = PlayerInfo.GetIgnoreArmor,	--设置忽视防御
	[EQUIP_ATTR_IGNORE_MISS] = PlayerInfo.GetIgnoreMiss,	--设置忽视闪避
	[EQUIP_ATTR_RECOVERY] = PlayerInfo.GetRecovery,	--设置生命值回复
	[EQUIP_ATTR_DAMAGE_AMPLIFY_RATE] = PlayerInfo.GetDamageAmplifyRate,	--设置伤害加深(万分比)
	[EQUIP_ATTR_DAMAGE_RESIST_RATE] = PlayerInfo.GetDamageResistRate,	--设置伤害减免(万分比)
	[EQUIP_ATTR_DAMAGE_RETURN_RATE] = PlayerInfo.GetDamageReturnRate,	--设置反弹伤害(万分比)
	[EQUIP_ATTR_VAMPIRIC_RATE] = PlayerInfo.GetVampiricRate,	--设置吸血光环(万分比)
	[EQUIP_ATTR_RECOVERY_RATE] = PlayerInfo.GetRecoveryRate,	--设置回复效率(万分比)
	[EQUIP_ATTR_CRIT_RATE] = PlayerInfo.GetCritRate,	--设置暴击率(万分比)
	[EQUIP_ATTR_CRIT_RESIST_RATE] = PlayerInfo.GetCritResistRate,	--设置抗暴率(万分比)
	[EQUIP_ATTR_CRIT_DAM_RATE] = PlayerInfo.GetCritDamRate,	--设置暴击伤害倍数(万分比)
	[EQUIP_ATTR_CRIT_RESIST_DAM_RATE] = PlayerInfo.GetCritResistDamRate,	--设置降暴伤害倍数(万分比)
	[EQUIP_ATTR_HIT_RATE] = PlayerInfo.GetHitRate,	--设置命中率(万分比)
	[EQUIP_ATTR_MISS_RATE] = PlayerInfo.GetMissRate,	--设置闪避率(万分比)
	[EQUIP_ATTR_STUN_RATE] = PlayerInfo.GetStunRate,	--设置眩晕
	[EQUIP_ATTR_ROOTS_RATE] = PlayerInfo.GetRootsRate,	--设置定身
	[EQUIP_ATTR_SILENCE_RATE] = PlayerInfo.GetSilenceRate,	--设置沉默
	[EQUIP_ATTR_CHAOS_RATE] = PlayerInfo.GetChaosRate,	--设置混乱
	[EQUIP_ATTR_CHARM_RATE] = PlayerInfo.GetCharmRate,	--设置魅惑
	[EQUIP_ATTR_CONTROL_ENHANCE_RATE] = PlayerInfo.GetControlEnhanceRate,	--设置控制增强
	[EQUIP_ATTR_CONTROL_RESIST_RATE] = PlayerInfo.GetControlResistRate,	--设置控制减免
	[EQUIP_ATTR_STRENGTH_ARMOR] = PlayerInfo.GetStrengthArmor,	--设置强化护甲
}

local PlayerInfo_Set_Attr_Func = {
	[EQUIP_ATTR_MAX_HEALTH] = PlayerInfo.SetMaxHealth,	--设置最大生命
	[EQUIP_ATTR_DAMAGE] = PlayerInfo.SetDamage,	--设置攻击力
	[EQUIP_ATTR_ARMOR] = PlayerInfo.SetArmor,	--设置防御力
	[EQUIP_ATTR_HIT] = PlayerInfo.SetHit,	--设置命中
	[EQUIP_ATTR_MISS] = PlayerInfo.SetMiss,	--设置闪避
	[EQUIP_ATTR_CRIT] = PlayerInfo.SetCrit,	--设置暴击
	[EQUIP_ATTR_TOUGH] = PlayerInfo.SetTough,	--设置坚韧
	[EQUIP_ATTR_ATTACK_SPEED] = PlayerInfo.SetAttackSpeed,	--设置攻击速度
	[EQUIP_ATTR_MOVE_SPEED] = PlayerInfo.SetMoveSpeed,	--设置移动速度
	[EQUIP_ATTR_IGNORE_ARMOR] = PlayerInfo.SetIgnoreArmor,	--设置忽视防御
	[EQUIP_ATTR_IGNORE_MISS] = PlayerInfo.SetIgnoreMiss,	--设置忽视闪避
	[EQUIP_ATTR_RECOVERY] = PlayerInfo.SetRecovery,	--设置生命值回复
	[EQUIP_ATTR_DAMAGE_AMPLIFY_RATE] = PlayerInfo.SetDamageAmplifyRate,	--设置伤害加深(万分比)
	[EQUIP_ATTR_DAMAGE_RESIST_RATE] = PlayerInfo.SetDamageResistRate,	--设置伤害减免(万分比)
	[EQUIP_ATTR_DAMAGE_RETURN_RATE] = PlayerInfo.SetDamageReturnRate,	--设置反弹伤害(万分比)
	[EQUIP_ATTR_VAMPIRIC_RATE] = PlayerInfo.SetVampiricRate,	--设置吸血光环(万分比)
	[EQUIP_ATTR_RECOVERY_RATE] = PlayerInfo.SetRecoveryRate,	--设置回复效率(万分比)
	[EQUIP_ATTR_CRIT_RATE] = PlayerInfo.SetCritRate,	--设置暴击率(万分比)
	[EQUIP_ATTR_CRIT_RESIST_RATE] = PlayerInfo.SetCritResistRate,	--设置抗暴率(万分比)
	[EQUIP_ATTR_CRIT_DAM_RATE] = PlayerInfo.SetCritDamRate,	--设置暴击伤害倍数(万分比)
	[EQUIP_ATTR_CRIT_RESIST_DAM_RATE] = PlayerInfo.SetCritResistDamRate,	--设置降暴伤害倍数(万分比)
	[EQUIP_ATTR_HIT_RATE] = PlayerInfo.SetHitRate,	--设置命中率(万分比)
	[EQUIP_ATTR_MISS_RATE] = PlayerInfo.SetMissRate,	--设置闪避率(万分比)
	[EQUIP_ATTR_STUN_RATE] = PlayerInfo.SetStunRate,	--设置眩晕
	[EQUIP_ATTR_ROOTS_RATE] = PlayerInfo.SetRootsRate,	--设置定身
	[EQUIP_ATTR_SILENCE_RATE] = PlayerInfo.SetSilenceRate,	--设置沉默
	[EQUIP_ATTR_CHAOS_RATE] = PlayerInfo.SetChaosRate,	--设置混乱
	[EQUIP_ATTR_CHARM_RATE] = PlayerInfo.SetCharmRate,	--设置魅惑
	[EQUIP_ATTR_CONTROL_ENHANCE_RATE] = PlayerInfo.SetControlEnhanceRate,	--设置控制增强
	[EQUIP_ATTR_CONTROL_RESIST_RATE] = PlayerInfo.SetControlResistRate,	--设置控制减免
	[EQUIP_ATTR_STRENGTH_ARMOR] = PlayerInfo.SetStrengthArmor,	--设置强化护甲
}


function PlayerInfo:RecalcAttrAndBattlePoint()
	playerLib.SendAttr(self.ptr)
end

--属性重算入口
function PlayerInfo:DoCalculAttr  ( attr_binlog)
	-- 如果是跨服的就不重算了
	local isPkServer = globalGameConfig:IsPKServer()
	if isPkServer then
		return
	end
	
	local attrs = {}
	local battleForce = 0
	for attrId,_ in pairs(PlayerInfo_Set_Attr_Func) do
		attrs[attrId] = 0
	end
	-- 技能管理类
	local spellMgr = self:getSpellMgr()
	local skillForce = 0
	--基础技能战力
	for i = SPELL_INT_FIELD_BASE_SPELL_START, SPELL_INT_FIELD_BASE_SPELL_END - 1 do
		local spellID	= spellMgr:GetUInt16(i, 0)
		local lv 		= spellMgr:GetUInt16(i, 1)
		if spellID > 0 then
			local bp = self:GetSkillBattlePoint(spellID, lv)
			skillForce = skillForce + bp
		end
	end
	self:SetSkillForce(skillForce)
	
	battleForce = battleForce + skillForce
	outFmtDebug("base skill force %d", battleForce)
	
	-- 装备
	local suitBaseForce = 0
	local suitBaseAttribute = {}
	local itemMgr = self:getItemMgr()
	if itemMgr then 
		suitBaseForce = itemMgr:itemCalculAttr(attrs, suitBaseAttribute)
	end
	
	battleForce = battleForce + suitBaseForce
	
	printAttr("suit ", attrs)

	-- 坐骑
	local mountNonprosForce = spellMgr:calculMountAttr(attrs)
	battleForce = battleForce + mountNonprosForce
	
	outFmtDebug("mount force %d", battleForce)
	printAttr("mount ", attrs)
	
	-- 神兵
	spellMgr:calculDivineAttr(attrs)
	
	printAttr("divine ", attrs)
	
	--法宝
	spellMgr:calculTalismanAttr(attrs)
	
	printAttr("talisman ", attrs)
	
	--神羽
	spellMgr:calculWingsAttr(attrs)
	
	printAttr("wings ", attrs)
	
	-- 经脉
	spellMgr:calcMeridianAttr(attrs)
	printAttr("meridian ", attrs)
		
	--称号
	self:calculTitleAttr(attrs)
	
	printAttr("title ", attrs)
	
	-- 被动技能属性
	self:PassiveSpellAttr(attrs)
	
	printAttr("passive attr ", attrs)
	
	-- 获得玩家速度
	local speed = GetPlayerSpeed(self:GetLevel(), spellMgr:getMountLevel(), self:GetCurrIllusionId(), self:isRide(), self:GetGender())
	attrs[EQUIP_ATTR_MOVE_SPEED] = speed
	
	-- 外观
	local id1 = self:GetAppearance(0)
	local id2 = self:GetAppearance(1)
	local attrsRate = {}
	spellMgr:calcAppearanceAttr(attrs, attrsRate, id1, id2)
	for attrId, attrVal in pairs(attrsRate) do
		if not attrs[attrId] then
			attrs[attrId] = 0
		end
		attrs[attrId] = math.floor(attrs[attrId] * (100 + attrVal) / 100)
	end
	printAttr("appearance ", attrs)
	
	-- 算属性的战力
	battleForce = battleForce + math.floor(DoAnyOneCalcForce(attrs, self:GetGender()) / 100)
	
	-- 玩家基础属性
	local level = self:GetLevel()
	local config = tb_char_level[level]
	local jobIndx = getJobIndxByGender(self:GetGender())
	if config then
		local baseprop = config["prop"..jobIndx]
		for _, val in ipairs(baseprop) do
			local indx = val[ 1 ]
			-- 不是速度属性 或者 是速度属性 但是未骑乘
			if indx ~= EQUIP_ATTR_MOVE_SPEED then
				attrs[indx] = val[ 2 ]
			end
		end
		battleForce = battleForce + config["battlePoint"..jobIndx]
	end
	
	printAttr("base ", attrs)
	
	local prevlist = {}
	-- 设置到playerBase中
	for attrId, val in pairs(attrs) do
		val = math.floor(val)
		--local index = attrId - 1
		--binLogLib.SetUInt32(attr_binlog, index, val)
		local getFunc = PlayerInfo_Get_Attr_Func[attrId]
		if getFunc then
			local prev = getFunc(self)
			if val ~= prev then
				prevlist[attrId] = val - prev
			end
		end
		
		if suitBaseAttribute[attrId] then
			val = val + suitBaseAttribute[attrId]
		end
		
		local func = PlayerInfo_Set_Attr_Func[attrId]
		if func then
			local index = attrId - 1
			func(self, val)
		end
	end
	
	outFmtDebug("all force %d", battleForce)	
	
	battleForce = math.floor(battleForce)
	
	binLogLib.SetDouble(attr_binlog, 0, battleForce)
	
	self:SendAttrChanged(prevlist)
end

function PlayerInfo:PassiveSpellAttr(attrs)
	for i = PLAYER_INT_FIELD_PASSIVE_SPELL_START, PLAYER_INT_FIELD_PASSIVE_SPELL_END-1 do
		local spellID	= self:GetUInt16(i, 0)
		local level		= self:GetUInt16(i, 1)
		if spellID > 0 then
			local index		= tb_skill_base[spellID].uplevel_id[ 1 ] + level - 1
			local config	= tb_skill_uplevel[index]
			-- 加固定属性的
			if config and config.dispatch_condition[ 1 ] == PASSIVE_DISPATCH_TYPE_FOREVER then
				for _, passiveInfo in ipairs(config.passive_type) do
					if passiveInfo[ 1 ] == PASSIVE_EFFECT_TYPE_PLAYER_ATTR then
						local attrId = passiveInfo[ 2 ]
						local values = passiveInfo[ 3 ]
						attrs[attrId] = attrs[attrId] + values
					end
				end
			end
		end
	end
end

-- 发送修改的属性
function PlayerInfo:SendAttrChanged(prevlist)
	local attrChangedList = {}
	for attrId, val in pairs(prevlist) do
		table.insert(attrChangedList, {attrId, val})
	end
	-- 通知客户端属性改变
	local pkt = protocols.pack_attribute_changed ()
	pkt:writeU16(#attrChangedList)
	for _, info in ipairs(attrChangedList) do
		pkt:writeU16(info[ 1 ])
		pkt:writeDouble(info[ 2 ])
	end
	self:SendPacket(pkt)
	pkt:delete()
end 

function printAttr(str, dict)
	outFmtDebug("attr after %s", str)
	local list = {}
	for k, v in pairs(dict) do
		table.insert(list, k..":"..v)
	end
	outFmtDebug(string.join(" ", list))
	outFmtDebug("----------------")
end

function PlayerInfo:GetSkillBattlePoint(spellId, lv)
	local indx = tb_skill_base[spellId].uplevel_id[ 1 ] + lv - 1
	return tb_skill_uplevel[indx].fight_value
end

--重设装备下标
function PlayerInfo:UpdatePlayerEquipment  ()
	for i = 0, EQUIPMENT_TYPE_MAX_POS-1 do
		self:UpdateEquipDisplay(i)
	end
end

--更新玩家装备显示
function PlayerInfo:UpdateEquipDisplay  ( pos)
	
--[[
	local useFashion = self:GetBit(PLAYER_EXPAND_INT_USE_FASHION, pos)
	
	--时装优先
	if (useFashion) then
		local fashion_pos = pos + EQUIPMENT_TYPE_MAX_POS
		if (self:TrySetEquipment(pos, fashion_pos))then
			return
		end
	end
--]]
	-- --看下普通位置
	--if(pos == EQUIPMENT_TYPE_MAIN_WEAPON)then
	-- --已穿戴的兵魂设置
	-- for i = SHENBING_TYPE_TANLANGXING, MAX_SHENBING_TYPE - 1 do
	-- if(self:GetShenBingWearFlags(i))then
	-- if(tb_weapon[i+1].item_id ~= nil)then
	-- self:SetEquipment(EQUIPMENT_TYPE_MAIN_WEAPON, tb_weapon[i+1].item_id)
	-- end
	-- return
	-- end
	-- end
	-- end
	
	if pos ~= EQUIPMENT_TYPE_MAIN_WEAPON then
		return
	end
	
	if (self:TrySetEquipment(pos, pos))then
		return
	end
--	什么都没有
	self:SetEquipment(pos, 0)
end

--尝试下设置装备显示
function PlayerInfo:TrySetEquipment  (equip_pos,item_pos)
	local itemMgr = self:getItemMgr()
	local item = itemMgr:getBagItemByPos(BAG_TYPE_EQUIP, item_pos)
	if item then
		local entry = item:getEntry()
		if (entry) then
			self:SetEquipment(equip_pos, entry)
			return true
		end
	end
	
	return false
end

