LogindPlayer = class('LogindPlayer', BinLogObject)

function LogindPlayer:GetAccount()
	return self:GetStr(PLAYER_STRING_FIELD_ACCOUNT)
end

function LogindPlayer:DefereSuccess()
	local slavehoder = self:GetSlavehoder()
	if(slavehoder > 0)then
		self:SetUInt32(PLAYER_FIELD_SLAVEHODER, -slavehoder)
	end
end

-- 获得角色id
function LogindPlayer:GetGender()
	return self:GetByte(PLAYER_FIELD_BYTES_0, 0)
end

--获取某玩家标志位
function LogindPlayer:GetFlags(index)
	return self:GetBit(PLAYER_APPD_INT_FIELD_FLAG, index)
end

function LogindPlayer:UnSetFlags(index)
	return self:UnSetBit(PLAYER_APPD_INT_FIELD_FLAG, index)
end


--获取某玩家礼包标志位
function LogindPlayer:GetGiftFlags(index)
	return self:GetBit(PLAYER_APPD_INT_FIELD_GIFT_FLAG, index)
end

--获取MapId
function LogindPlayer:GetMapId()
	return self:GetUInt32(PLAYER_FIELD_MAP_ID)
end

--设置传送的地图id和坐标
function LogindPlayer:SetTeleportInfo(mapid, posx, posy, general_id)
	self:SetUInt32(PLAYER_EXPAND_INT_TO_MAP, mapid)
	self:SetFloat(PLAYER_EXPAND_INT_TO_POS_X, posx)
	self:SetFloat(PLAYER_EXPAND_INT_TO_POS_Y, posy)
	self:SetUInt32(PLAYER_FIELD_INSTANCE_ID, 0)
	--outDebug('guid = '..self:GetGuid()..' name = '..self:GetName()..' mapid = '..mapid..' posx = '..posx..' posy = '..posy)
	if(general_id)then
		self:SetStr(PLAYER_STRING_FIELD_TELEPORT_GUID, general_id)
	end
end

function LogindPlayer:GetRiskTeleportInfo()
	local passedSectionId = self:GetUInt32(PLAYER_INT_FIELD_TRIAL_FINISHED_SECTIONID)
	local mapid, x, y, generalId = onGetRiskTeleportInfo(self:GetGuid(), passedSectionId)
	return mapid, x, y, generalId
end

KUAFU_FENGLIUZHEN_MAPID = 3002
KUAFU_XIANFU_MAPID = 3003

--pk服，根据跨服类型选择要传送到的地图id
function LogindPlayer:SelectKuafuMapid(warid, kuafu_type, number, reverse, reverse_str)
	outFmtDebug("####################### kuafu_type = %d", kuafu_type)
	
	if(kuafu_type == KUAFU_TYPE_FENGLIUZHEN)then
		local general_id = string.format("flz_%s", reverse_str)		--warid即房间id
		local config = tb_kuafu3v3_base[ 1 ].bornPos
		local pos = config[reverse]
		
		--生命回满
		self:SetHealth(self:GetMaxHealth())
		-- 设置玩家的虚拟阵营
		self:SetUInt32(PLAYER_INT_FIELD_VIRTUAL_CAMP, reverse)
		local offsetX = randInt(-3, 3)
		local offsetY = randInt(-3, 3)
		self:SetTeleportInfo(KUAFU_FENGLIUZHEN_MAPID, pos[ 1 ] + offsetX, pos[ 2 ] + offsetY, general_id)
		return true
	elseif (kuafu_type == KUAFU_TYPE_XIANFU) then
		local general_id = string.format("xianfu_%s|%d", reverse_str, reverse)		--warid即房间id
		local config = tb_kuafu_xianfu_base[ 1 ]
		local pos = config.bornPos[1]
		
		--生命回满
		self:SetHealth(self:GetMaxHealth())
		local offset = config.offset
		local offsetX = randInt(-offset, offset)
		local offsetY = randInt(-offset, offset)
		self:SetTeleportInfo(KUAFU_XIANFU_MAPID, pos[ 1 ] + offsetX, pos[ 2 ] + offsetY, general_id)
		return true
	elseif (kuafu_type == KUAFU_TYPE_GROUP_INSTANCE) then
		local general_id = string.format("group_instance_%s|%d", reverse_str, reverse)		--warid即房间id
		if reverse < 1 or reverse > #tb_group_instance_base then
			reverse = #tb_group_instance_base
		end
		local config = tb_group_instance_base[reverse]
		local pos = config.enterPos
		
		--生命回满
		self:SetHealth(self:GetMaxHealth())
		local offset = 2
		local offsetX = randInt(-offset, offset)
		local offsetY = randInt(-offset, offset)
		local mapid = config.mapid
		self:SetTeleportInfo(mapid, pos[ 1 ] + offsetX, pos[ 2 ] + offsetY, general_id)
		return true
	end
	
	return false
end

--离线做些什么
function LogindPlayer:Logout()
	
end

--付费等级计算
function LogindPlayer:DoPayLevel()
	local pay_table = {0,5000,20000,50000,100000,200000,500000,800000,1000000,1500000,2000000}
	local rechage = self:GetUInt32(PLAYER_APPD_INT_FIELD_RECHARGE_SUM)
	local pay_level = #pay_table - 1
	for i = 1,#pay_table do
		if(rechage < pay_table[i])then
			pay_level = i - 2
			break
		end
	end
	return pay_level
end


--合服数据处理
function DoMergeSomething()

end


-- 获得最大生命
function LogindPlayer:GetMaxHealth()
	return self:GetDouble(PLAYER_FIELD_MAX_HEALTH)
end

-- 设置当前生命
function LogindPlayer:SetHealth(val)
	self:SetDouble(PLAYER_FIELD_HEALTH, val)
end

-- 设置最大生命
function LogindPlayer:SetMaxHealth(val)
	self:SetDouble(PLAYER_FIELD_MAX_HEALTH, val)
end

-- 设置攻击力
function LogindPlayer:SetDamage(val)
	self:SetDouble(PLAYER_FIELD_DAMAGE, val)
end

-- 设置防御力
function LogindPlayer:SetArmor(val)
	self:SetDouble(PLAYER_FIELD_ARMOR, val)
end

-- 设置命中
function LogindPlayer:SetHit(val)
	self:SetDouble(PLAYER_FIELD_HIT, val)
end

-- 设置闪避
function LogindPlayer:SetMiss(val)
	self:SetDouble(PLAYER_FIELD_MISS, val)
end

-- 设置暴击
function LogindPlayer:SetCrit(val)
	self:SetDouble(PLAYER_FIELD_CRIT, val)
end

-- 设置坚韧
function LogindPlayer:SetTough(val)
	self:SetDouble(PLAYER_FIELD_TOUGH, val)
end

-- 设置攻击速度
function LogindPlayer:SetAttackSpeed(val)
	self:SetDouble(PLAYER_FIELD_ATTACK_SPEED, val)
end

-- 设置移动速度
function LogindPlayer:SetMoveSpeed(val)
	self:SetDouble(PLAYER_FIELD_MOVE_SPEED, val)
end

-- 设置忽视防御
function LogindPlayer:SetIgnoreArmor(val)
	self:SetDouble(PLAYER_FIELD_IGNORE_ARMOR, val)
end

-- 设置忽视闪避
function LogindPlayer:SetIgnoreMiss(val)
	self:SetDouble(PLAYER_FIELD_IGNORE_MISS, val)
end

-- 设置生命值回复
function LogindPlayer:SetRecovery(val)
	self:SetDouble(PLAYER_FIELD_RECOVERY, val)
end

-- 设置伤害加深(万分比)
function LogindPlayer:SetDamageAmplifyRate(val)
	self:SetDouble(PLAYER_FIELD_DAMAGE_AMPLIFY_RATE, val)
end

-- 设置伤害减免(万分比)
function LogindPlayer:SetDamageResistRate(val)
	self:SetDouble(PLAYER_FIELD_DAMAGE_RESIST_RATE, val)
end

-- 设置反弹伤害(万分比)
function LogindPlayer:SetDamageReturnRate(val)
	self:SetDouble(PLAYER_FIELD_DAMAGE_RETURN_RATE, val)
end

-- 设置吸血光环(万分比)
function LogindPlayer:SetVampiricRate(val)
	self:SetDouble(PLAYER_FIELD_VAMPIRIC_RATE, val)
end

-- 设置回复效率(万分比)
function LogindPlayer:SetRecoveryRate(val)
	self:SetDouble(PLAYER_FIELD_RECOVERY_RATE, val)
end

-- 设置暴击率(万分比)
function LogindPlayer:SetCritRate(val)
	self:SetDouble(PLAYER_FIELD_CRIT_RATE, val)
end

-- 设置抗暴率(万分比)
function LogindPlayer:SetCritResistRate(val)
	self:SetDouble(PLAYER_FIELD_CRIT_RESIST_RATE, val)
end

-- 设置暴击伤害倍数(万分比)
function LogindPlayer:SetCritDamRate(val)
	self:SetDouble(PLAYER_FIELD_CRIT_DAM_RATE, val)
end

-- 设置降暴伤害倍数(万分比)
function LogindPlayer:SetCritResistDamRate(val)
	self:SetDouble(PLAYER_FIELD_CRIT_RESIST_DAM_RATE, val)
end

-- 设置命中率(万分比)
function LogindPlayer:SetHitRate(val)
	self:SetDouble(PLAYER_FIELD_HIT_RATE, val)
end

-- 设置闪避率(万分比)
function LogindPlayer:SetMissRate(val)
	self:SetDouble(PLAYER_FIELD_MISS_RATE, val)
end

-- 设置眩晕
function LogindPlayer:SetStunRate(val)
	self:SetDouble(PLAYER_FIELD_STUN_RATE, val)
end

-- 设置定身
function LogindPlayer:SetRootsRate(val)
	self:SetDouble(PLAYER_FIELD_ROOTS_RATE, val)
end

-- 设置沉默
function LogindPlayer:SetSilenceRate(val)
	self:SetDouble(PLAYER_FIELD_SILENCE_RATE, val)
end

-- 设置混乱
function LogindPlayer:SetChaosRate(val)
	self:SetDouble(PLAYER_FIELD_CHAOS_RATE, val)
end

-- 设置魅惑
function LogindPlayer:SetCharmRate(val)
	self:SetDouble(PLAYER_FIELD_CHARM_RATE, val)
end

-- 设置控制增强
function LogindPlayer:SetControlEnhanceRate(val)
	self:SetDouble(PLAYER_FIELD_CONTROL_ENHANCE_RATE, val)
end

-- 设置控制减免
function LogindPlayer:SetControlResistRate(val)
	self:SetDouble(PLAYER_FIELD_CONTROL_RESIST_RATE, val)
end

-- 设置强化护甲
function LogindPlayer:SetStrengthArmor(val)
	self:SetDouble(PLAYER_FIELD_STRENGTH_ARMOR, val)
end

local InitAttrFunc = {
	[EQUIP_ATTR_MAX_HEALTH] = LogindPlayer.SetMaxHealth,	--设置最大生命
	[EQUIP_ATTR_DAMAGE] = LogindPlayer.SetDamage,	--设置攻击力
	[EQUIP_ATTR_ARMOR] = LogindPlayer.SetArmor,	--设置防御力
	[EQUIP_ATTR_HIT] = LogindPlayer.SetHit,	--设置命中
	[EQUIP_ATTR_MISS] = LogindPlayer.SetMiss,	--设置闪避
	[EQUIP_ATTR_CRIT] = LogindPlayer.SetCrit,	--设置暴击
	[EQUIP_ATTR_TOUGH] = LogindPlayer.SetTough,	--设置坚韧
	[EQUIP_ATTR_ATTACK_SPEED] = LogindPlayer.SetAttackSpeed,	--设置攻击速度
	[EQUIP_ATTR_MOVE_SPEED] = LogindPlayer.SetMoveSpeed,	--设置移动速度
	[EQUIP_ATTR_IGNORE_ARMOR] = LogindPlayer.SetIgnoreArmor,	--设置忽视防御
	[EQUIP_ATTR_IGNORE_MISS] = LogindPlayer.SetIgnoreMiss,	--设置忽视闪避
	[EQUIP_ATTR_RECOVERY] = LogindPlayer.SetRecovery,	--设置生命值回复
	[EQUIP_ATTR_DAMAGE_AMPLIFY_RATE] = LogindPlayer.SetDamageAmplifyRate,	--设置伤害加深(万分比)
	[EQUIP_ATTR_DAMAGE_RESIST_RATE] = LogindPlayer.SetDamageResistRate,	--设置伤害减免(万分比)
	[EQUIP_ATTR_DAMAGE_RETURN_RATE] = LogindPlayer.SetDamageReturnRate,	--设置反弹伤害(万分比)
	[EQUIP_ATTR_VAMPIRIC_RATE] = LogindPlayer.SetVampiricRate,	--设置吸血光环(万分比)
	[EQUIP_ATTR_RECOVERY_RATE] = LogindPlayer.SetRecoveryRate,	--设置回复效率(万分比)
	[EQUIP_ATTR_CRIT_RATE] = LogindPlayer.SetCritRate,	--设置暴击率(万分比)
	[EQUIP_ATTR_CRIT_RESIST_RATE] = LogindPlayer.SetCritResistRate,	--设置抗暴率(万分比)
	[EQUIP_ATTR_CRIT_DAM_RATE] = LogindPlayer.SetCritDamRate,	--设置暴击伤害倍数(万分比)
	[EQUIP_ATTR_CRIT_RESIST_DAM_RATE] = LogindPlayer.SetCritResistDamRate,	--设置降暴伤害倍数(万分比)
	[EQUIP_ATTR_HIT_RATE] = LogindPlayer.SetHitRate,	--设置命中率(万分比)
	[EQUIP_ATTR_MISS_RATE] = LogindPlayer.SetMissRate,	--设置闪避率(万分比)
	[EQUIP_ATTR_STUN_RATE] = LogindPlayer.SetStunRate,	--设置眩晕
	[EQUIP_ATTR_ROOTS_RATE] = LogindPlayer.SetRootsRate,	--设置定身
	[EQUIP_ATTR_SILENCE_RATE] = LogindPlayer.SetSilenceRate,	--设置沉默
	[EQUIP_ATTR_CHAOS_RATE] = LogindPlayer.SetChaosRate,	--设置混乱
	[EQUIP_ATTR_CHARM_RATE] = LogindPlayer.SetCharmRate,	--设置魅惑
	[EQUIP_ATTR_CONTROL_ENHANCE_RATE] = LogindPlayer.SetControlEnhanceRate,	--设置控制增强
	[EQUIP_ATTR_CONTROL_RESIST_RATE] = LogindPlayer.SetControlResistRate,	--设置控制减免
	[EQUIP_ATTR_STRENGTH_ARMOR] = LogindPlayer.SetStrengthArmor,	--设置强化护甲
}



--设置玩家初始信息
function LogindPlayer:SetNewPlayerInfo()
	-- 初始化属性
	local config = tb_char_level[1]
	if config then
		self:SetDouble(PLAYER_EXPAND_INT_NEXT_LEVEL_XP, config.next_exp)
		local jobIndx = getJobIndxByGender(self:GetGender())
		local baseprop = config["prop"..jobIndx]
		for _, val in ipairs(baseprop)do
			local func = InitAttrFunc[val[ 1 ]];
			if func ~= nil then
				func(self, val[2])
				-- 需要设置当前血量
				if val[ 1 ] == EQUIP_ATTR_MAX_HEALTH then
					self:SetHealth(val[ 2 ])
				end
			end
		end
	end
	
	-- 初始化玩家技能
	local gender = self:GetGender()
	local config = tb_char_skill[gender]

	if config then
		-- 初始主动技能
		local initiativeStart = PLAYER_INT_FIELD_SPELL_START
		
		for _, spellId in ipairs(config.init_spell) do
			
			local spellBaseConfig = tb_skill_base[spellId]
			if spellBaseConfig ~= nil then
				-- 主动技能列表
				self:SetUInt16(initiativeStart, SHORT_SLOT_SPELL_ID, spellId)
				self:SetByte(initiativeStart, BYTE_SLOT_SPELL_LV,		1)
				local slot = spellBaseConfig.skill_slot
				self:SetByte(initiativeStart, BYTE_SLOT,	slot)
				initiativeStart = initiativeStart + 1
			end
		end
	end
end