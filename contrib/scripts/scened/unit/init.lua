----------------------------------------------------------------------------------------------------------------------------------------------
--Unit封装
UnitInfo = class('UnitInfo', BinLogObject)

--让unitInfo暂时拥有发包功能,下一步将转移到playerContext
local Protocols = require('share.protocols')
Protocols:extend(UnitInfo)

--TODO:在unitInfo和playerContext分开未实现时，先这么处理
-- local ScenedContext = require 'scened.context.scened_context'
-- ScenedContext:extend(UnitInfo)

function UnitInfo:ctor( )
	if(GetUnitTypeID(self.ptr) == TYPEID_PLAYER)then
		self.ptr_player_data = playerLib.GetSession(self.ptr)
	end
end

function UnitInfo:unitCalcQueueIndx()
	local score = self:unitGetQualifyScore()
	local indx = #tb_single_pvp_grade	
	for i = 1, #tb_single_pvp_grade do
		local range = tb_single_pvp_grade[ i ].range
		if score >= range[ 1 ] and score <= range[ 2 ] then
			indx = i
		end
	end
	
	return indx
end

function GetUnitName(unit_ptr)
	return binLogLib.GetStr(unit_ptr, BINLOG_STRING_FIELD_NAME)
end

-- 获得排位赛积分
function UnitInfo:unitGetQualifyScore()
	return self:GetPlayerUInt32(PLAYER_INT_FIELD_QUALIFY_SCORE)
end



--获得通关状态
function UnitInfo:isGroupInstanceClearFlag(val)
	return self:GetPlayerBit(PLAYER_INT_FIELD_GROUP_INSTANCE_CLEAR_FLAG, val)
end

-- 是否在挂机
function UnitInfo:isInHook()
	return self:GetPlayerUInt32(PLAYER_FIELD_HOOK) > 0
end

--获得通关状态
function UnitInfo:isGroupInstancePassed(val)
	return self:GetPlayerBit(PLAYER_INT_FIELD_GROUP_INSTANCE_CLEAR_FLAG, val)
end

-- 是否自动使用复活丹(原地复活)
function UnitInfo:isUseRespawnItem()
	return self:GetPlayerByte(PLAYER_FIELD_HOOK_BYTE3, 0) > 0
end

-- 获得玩家的虚拟阵营
function UnitInfo:GetVirtualCamp()
	return self:GetPlayerUInt32(PLAYER_INT_FIELD_VIRTUAL_CAMP)
end

-- 是否拒绝接受附近消息
function UnitInfo:isDeclineNearMsg()
	return self:GetPlayerByte(PLAYER_FIELD_DECLINE_CHANNEL_BYTE1, 0) > 0
end

-- 获得跨服3v3积分
function UnitInfo:GetKuafu3v3Score()
	return self:GetPlayerUInt32(PLAYER_INT_FIELD_WORLD_3V3_SCORE)
end

-- 设置跨服3v3积分
function UnitInfo:SetKuafu3v3Score(val)
	self:SetPlayerUInt32(PLAYER_INT_FIELD_WORLD_3V3_SCORE, val)
end

-- 增加跨服3v3积分
function UnitInfo:AddKuafu3v3Score(val)
	-- 加临时积分
	local ret = val + self:GetKuafu3v3Score()
	ret = math.max(ret, 0)
	self:SetKuafu3v3Score(ret)
	
	-- 加总积分
	local totalret = val + self:GetKuafu3v3TotalScore()
	totalret = math.max(totalret, 0)
	self:SetKuafu3v3TotalScore(totalret)
end

-- 获得跨服3v3总积分
function UnitInfo:GetKuafu3v3TotalScore()
	return self:GetPlayerUInt32(PLAYER_INT_FIELD_WORLD_3V3_TOTAL_SCORE)
end

-- 设置跨服3v3总积分
function UnitInfo:SetKuafu3v3TotalScore(val)
	self:SetPlayerUInt32(PLAYER_INT_FIELD_WORLD_3V3_TOTAL_SCORE, val)
end

-- 获得跨服3v3胜负趋势
function UnitInfo:GetKuafu3v3TrendInfo()
	return self:GetPlayerInt32(PLAYER_INT_FIELD_WORLD_3V3_TREND_INFO)
end

-- 设置跨服3v3胜负趋势
function UnitInfo:SetKuafu3v3TrendInfo(value)
	self:SetPlayerInt32(PLAYER_INT_FIELD_WORLD_3V3_TREND_INFO, value)
end

-- 增加跨服3v3胜负趋势
function UnitInfo:Kuafu3v3Win()
	local value = self:GetKuafu3v3TrendInfo()
	if value < 0 then
		value = 0
	end
	value = value + 1
	self:SetKuafu3v3TrendInfo(value)
end

-- 减少跨服3v3胜负趋势
function UnitInfo:Kuafu3v3Lose()
	local value = self:GetKuafu3v3TrendInfo()
	if value > 0 then
		value = 0
	end
	value = value - 1
	self:SetKuafu3v3TrendInfo(value)
end

--获得int guid
function UnitInfo:GetIntGuid(  )
	return unitLib.GetIntGuid(self.ptr)
end

-- 判断大的进入条件是否满足
function UnitInfo:makeEnterTest(toMapId)
	local mapid = unitLib.GetMapID(self.ptr)
	local bbit  = tb_map[mapid].type
	local value = bit.lshift(1, bbit)
	
	local mask = tb_map[toMapId].enter_mask
	return bit.band(mask, value) > 0
end

--最重要的操作结果
function UnitInfo:CallOptResult( typed, reason, data )
	if GetUnitTypeID(self.ptr) ~= TYPEID_PLAYER then 
		outFmtError("UnitInfo:CallOptResult not player cant send packet")
		return
	end
	--碰上字符串数组自动拼接	
	if type(data) == 'table' then
		data = string.join('|', data)
	else
		data = tostring(data) or ''
	end
	self:call_operation_failed(typed, reason, data)
end

--发送技能开始给客户端
function UnitInfo:CallCastSpellStart( caster_guid, target_guid, spellid, data, isbrodcast )
	if GetUnitTypeID(self.ptr) ~= TYPEID_PLAYER 
		--and not self:GetUnitFlags(UNIT_FIELD_FLAGS_IS_ROBOT) 
		and not self:GetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
		--and not self:GetUnitFlags(UNIT_FIELD_FLAGS_IS_XIALV) 
		then 
		outFmtError("UnitInfo:CallCastSpellStart not player cant send packet")
		return
	end
	--碰上字符串数组自动拼接	
	if type(data) == 'table' then
		data = string.join('|', data)
	else
		data = tostring(data) or ''
	end

	local pkt = Protocols.pack_cast_spell_start(caster_guid, target_guid, spellid, data)
	if isbrodcast then
		app:Broadcast(self, pkt)
	else
		self:SendPacket(pkt)
	end
	pkt:delete()
end

function UnitInfo:CallSpellStop(isbrodcast)
	local pkt = Protocols.pack_spell_stop(self:GetGuid())
	if isbrodcast then
		app:Broadcast(self, pkt)
	else
		self:SendPacket(pkt)
	end
	pkt:delete()
end


function UnitInfo:CheckPlayer(name)
	if(self:GetTypeID() ~= TYPEID_PLAYER)then
		outDebug(string.format("func CheckPlayer  not player call player function !!! guid = %s name = %s   %s", self:GetGuid(), self:GetName(), name))
		bengdiaoba()
	end
end

function UnitInfo:GetPlayerInt32(index)
	self:CheckPlayer(string.format("GetPlayerInt32-- index = %d", index))
	checkNil(index)
	return binLogLib.GetInt32(self.ptr_player_data, index)
end

function UnitInfo:AddPlayerInt32(index, value)
	self:CheckPlayer(string.format("AddPlayerInt32-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.AddInt32(self.ptr_player_data, index, value)
end

function UnitInfo:SubPlayerInt32(index, value)
	self:CheckPlayer(string.format("SubPlayerInt32-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.SubInt32(self.ptr_player_data, index, value)
end

function UnitInfo:SetPlayerInt32(index, value)
	self:CheckPlayer(string.format("SetPlayerInt32-- index = %d ", index))
	checkNil(index)
	checkNil(value)
	binLogLib.SetInt32(self.ptr_player_data, index, value)
end

function UnitInfo:GetPlayerBit(index, offset)
	self:CheckPlayer(string.format("GetPlayerBit index = %d offset = %d ", index, offset))
	checkNil(index)
	checkNil(offset)
	return binLogLib.GetBit(self.ptr_player_data, index, offset)
end

function UnitInfo:GetPlayerByte(index, offset)
	self:CheckPlayer(string.format("GetPlayerByte-- index = %d offset = %d", index, offset))
	checkNil(index)
	checkNil(offset)
	return binLogLib.GetByte(self.ptr_player_data, index, offset)
end

function UnitInfo:GetPlayerUInt16(index, offset)
	self:CheckPlayer(string.format("GetPlayerUInt16-- index = %d offset = %d", index, offset))
	checkNil(index)
	checkNil(offset)
	return binLogLib.GetUInt16(self.ptr_player_data, index, offset)
end

function UnitInfo:GetPlayerUInt32(index)
	self:CheckPlayer(string.format("GetPlayerUInt32-- index = %d", index))
	checkNil(index)
	return binLogLib.GetUInt32(self.ptr_player_data, index)
end

function UnitInfo:SetPlayerBit(index, offset)
	self:CheckPlayer(string.format("SetPlayerBit index = %d offset = %d", index, offset))
	checkNil(index)
	checkNil(offset)
	binLogLib.SetBit(self.ptr_player_data, index, offset)
end

function UnitInfo:UnSetPlayerBit(index, offset)
	self:CheckPlayer(string.format("UnSetPlayerBit-- index =%d offset = %d ", index, offset))
	checkNil(index)
	checkNil(offset)
	binLogLib.UnSetBit(self.ptr_player_data, index, offset)
end

function UnitInfo:SetPlayerByte(index, offset, value)
	self:CheckPlayer(string.format("SetPlayerByte-- index = %d offset = %d", index, offset))
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.SetByte(self.ptr_player_data, index, offset, value)
end

function UnitInfo:SetPlayerUInt16(index, offset, value)
	self:CheckPlayer(string.format("SetPlayerUInt16-- index = %d offset = %d", index, offset))
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.SetUInt16(self.ptr_player_data, index, offset, value)
end

function UnitInfo:SubPlayerUInt16(index, offset, value)
	self:CheckPlayer(string.format("SubPlayerUInt16-- index = %d offset = %d", index, offset))
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.SubUInt16(self.ptr_player_data, index, offset, value)
end

function UnitInfo:AddPlayerUInt16(index, offset, value)
	self:CheckPlayer(string.format("SubPlayerUInt16-- index = %d offset = %d", index, offset))
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.AddUInt16(self.ptr_player_data, index, offset, value)
end

function UnitInfo:SetPlayerUInt32(index, value)
	self:CheckPlayer(string.format("SetPlayerUInt32-- index = %d ", index))
	checkNil(index)
	checkNil(value)
	binLogLib.SetUInt32(self.ptr_player_data, index, value)
end

function UnitInfo:AddPlayerUInt32(index, value)
	self:CheckPlayer(string.format("AddPlayerUInt32-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.AddUInt32(self.ptr_player_data, index, value)
end

function UnitInfo:SubPlayerUInt32(index, value)
	self:CheckPlayer(string.format("SubPlayerUInt32-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.SubUInt32(self.ptr_player_data, index, value)
end

function UnitInfo:GetPlayerDouble(index)
	self:CheckPlayer(string.format("GetPlayerDouble-- index = %d", index))
	checkNil(index)
	return binLogLib.GetDouble(self.ptr_player_data, index)
end

function UnitInfo:SetPlayerDouble(index, value)
	self:CheckPlayer(string.format("SetPlayerDouble-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.SetDouble(self.ptr_player_data, index, value)
end

function UnitInfo:AddPlayerDouble(index, value)
	self:CheckPlayer(string.format("AddPlayerDouble-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.AddDouble(self.ptr_player_data, index, value)
end

function UnitInfo:SubPlayerDouble(index, value)
	self:CheckPlayer(string.format("SubPlayerDouble-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.SubDouble(self.ptr_player_data, index, value)
end

function UnitInfo:AddPlayerByte(index, offset, value)
	self:CheckPlayer(string.format("AddPlayerByte-- index = % d offset = %d", index, offset))
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.AddByte(self.ptr_player_data, index, offset, value)
end

function UnitInfo:SetPlayerStr(index, value)
	self:CheckPlayer(string.format("SetPlayerStr-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.SetStr(self.ptr_player_data, index,value)
end

function UnitInfo:SetPlayerFloat(index, value)
	self:CheckPlayer(string.format("SetPlayerFloat-- index = %d", index))
	checkNil(index)
	checkNil(value)
	binLogLib.SetFloat(self.ptr_player_data, index, value)
end

function UnitInfo:GetPlayerFloat(index)
	self:CheckPlayer(string.format("GetPlayerFloat-- index = %d", index))
	checkNil(index)
	return binLogLib.GetFloat(self.ptr_player_data, index)
end

function UnitInfo:GetPlayerStr(index)
	self:CheckPlayer(string.format("GetPlayerStr-- index = %d", index))
	checkNil(index)
	return binLogLib.GetStr(self.ptr_player_data, index)
end

--获得主玩家guid
function UnitInfo:GetPlayerGuid()
	self:CheckPlayer("GetPlayerGuid")
	return binLogLib.GetStr(self.ptr_player_data, BINLOG_STRING_FIELD_GUID)
end

--把某玩家标志位变成1
function UnitInfo:SetFlags(index)
	self:SetPlayerBit(PLAYER_APPD_INT_FIELD_FLAG, index)
end

--把某玩家标志位变成0
function UnitInfo:UnSetFlags(index)
	self:UnSetPlayerBit(PLAYER_APPD_INT_FIELD_FLAG, index)
end

--获取某玩家标志位
function UnitInfo:GetFlags(index)
	return self:GetPlayerBit(PLAYER_APPD_INT_FIELD_FLAG, index)
end

--获得在线时长
function UnitInfo:GetOnlineTime()
	return self:GetPlayerUInt32(PLAYER_FIELD_ONLINE_TIME)
end

--获取类型
function UnitInfo:GetTypeID()
	return self:GetByte(UNIT_FIELD_BYTE_0, 0)
end

--获取模板
function UnitInfo:GetEntry()
	return self:GetUInt16(UNIT_FIELD_UINT16_0, 0)
end

--获取等级
function UnitInfo:GetLevel()
	return self:GetUInt16(UNIT_FIELD_UINT16_0, 1)
end

--设置等级
function UnitInfo:SetLevel(val)
	self:SetUInt16(UNIT_FIELD_UINT16_0, 1, val)
end

--获取头衔等级
function UnitInfo:GetTouXianLv()
	return self:GetByte(UNIT_FIELD_BYTE_0, 3)
end

--获得打坐时间
function UnitInfo:GetDaZuoTime()
	return playerLib.GetDaZuoStartTime(self.ptr)
end

-- 开始打坐
function UnitInfo:StartDaZuo()
	playerLib.SetDaZuoStartTime(self.ptr, os.time())
end

-- 取消打坐
function UnitInfo:CancelDaZuo()
	--TODO: 取消以后的一些逻辑
	playerLib.SetDaZuoStartTime(self.ptr, 0)
end

-- 是否在战斗中
function UnitInfo:isInBattle()
	
end

------------------------------------
--需要读条
function UnitInfo:NeedUseMode()
	return self:GetBit(GO_FIELD_FLAGS, GO_FLAG_USEMODE)
end
------------------------------------


function UnitInfo:SetProcessTime(val)
	self:SetUInt32(UNIT_FIELD_PROCESS_TIME, val)
end

function UnitInfo:SetProcessSeconds(val)
	self:SetUInt32(UNIT_FIELD_PROCESS_SECONDS, val)
end	

function UnitInfo:SetPickedName(name)
	self:SetStr(UNIT_STRING_FIELD_PICK_NAME, name)
end	

function UnitInfo:GetPassivespells()
	return {}
end

function UnitInfo:GetSpells()
	local spells = {}
	
	--local total_slot_1_cd = 0
	for i = PLAYER_INT_FIELD_SPELL_START, PLAYER_INT_FIELD_SPELL_END-1 do
		local id = self:GetPlayerUInt16(i, 0)
		if id > 0 then
			local config = tb_skill_base[id]
			if 1 <= config.skill_slot and config.skill_slot <= 5 then
				local level = self:GetPlayerByte(i, 2)
				local slot  = self:GetPlayerByte(i, 3)
				local groupCD = config.groupCD
				local rate = 0
				table.insert(spells, {id, rate, groupCD, level, slot})
			end
		end
	end
	
	rates = {4000, 3000, 3000}
	local rlen = #spells - 3	

	for i = 1, rlen do
		table.insert(rates, 10000)
	end
	
	for i = 1, #spells do
		spells[ i ][ 2 ] = rates[ i ]
	end
	
	return spells
end


-- 获得玩家属性列表
function UnitInfo:GetAttrs()
	local attrs = {}
	
	for attr_id, playerField in pairs(attrKeys) do
		local value = self:GetPlayerDouble(playerField)
		table.insert(attrs, {attr_id, value})
	end
	
	return attrs
end

function UnitInfo:GetDummyInfo()
	local config = {}
	config.name   = self:GetName()
	config.gender = self:GetGender()
	config.level  = self:GetLevel()
	config.attrs  = self:GetAttrs()
	config.weapon = self:GetWeapon()
	config.avatar = self:GetAvatar()
	config.divine = self:GetDivine()
	config.spells = self:GetSpells()
	config.passivespells = self:GetPassivespells()
	config.force  = self:GetForce()
	config.vip    = self:GetVIP()
	config.reverse1 = 0
	config.reverse2 = 0
	config.reverse3 = 0
	config.reverse4 = 0
	config.reverse5 = 0
	
	return config
end

local tBaseKey = {
	[EQUIP_ATTR_MAX_HEALTH] = UNIT_FIELD_MAX_HEALTH,	--最大生命
	[EQUIP_ATTR_DAMAGE] = UNIT_FIELD_DAMAGE,	--攻击力
	[EQUIP_ATTR_ARMOR] = UNIT_FIELD_ARMOR,	--防御力
	[EQUIP_ATTR_HIT] = UNIT_FIELD_HIT,	--命中
	[EQUIP_ATTR_MISS] = UNIT_FIELD_MISS,	--闪避
	[EQUIP_ATTR_CRIT] = UNIT_FIELD_CRIT,	--暴击
	[EQUIP_ATTR_TOUGH] = UNIT_FIELD_TOUGH,	--坚韧
	[EQUIP_ATTR_ATTACK_SPEED] = UNIT_FIELD_ATTACK_SPEED,	--攻击速度
	[EQUIP_ATTR_MOVE_SPEED] = UNIT_FIELD_MOVE_SPEED,	--移动速度
	[EQUIP_ATTR_IGNORE_ARMOR] = UNIT_FIELD_IGNORE_ARMOR,	--忽视防御
	[EQUIP_ATTR_IGNORE_MISS] = UNIT_FIELD_IGNORE_MISS,	--忽视闪避
	[EQUIP_ATTR_RECOVERY] = UNIT_FIELD_RECOVERY,	--生命值回复
	[EQUIP_ATTR_DAMAGE_AMPLIFY_RATE] = UNIT_FIELD_DAMAGE_AMPLIFY_RATE,	--伤害加深(万分比)
	[EQUIP_ATTR_DAMAGE_RESIST_RATE] = UNIT_FIELD_DAMAGE_RESIST_RATE,	--伤害减免(万分比)
	[EQUIP_ATTR_DAMAGE_RETURN_RATE] = UNIT_FIELD_DAMAGE_RETURN_RATE,	--反弹伤害(万分比)
	[EQUIP_ATTR_VAMPIRIC_RATE] = UNIT_FIELD_VAMPIRIC_RATE,	--吸血光环(万分比)
	[EQUIP_ATTR_RECOVERY_RATE] = UNIT_FIELD_RECOVERY_RATE,	--回复效率(万分比)
	[EQUIP_ATTR_CRIT_RATE] = UNIT_FIELD_CRIT_RATE,	--暴击率(万分比)
	[EQUIP_ATTR_CRIT_RESIST_RATE] = UNIT_FIELD_CRIT_RESIST_RATE,	--抗暴率(万分比)
	[EQUIP_ATTR_CRIT_DAM_RATE] = UNIT_FIELD_CRIT_DAM_RATE,	--暴击伤害倍数(万分比)
	[EQUIP_ATTR_CRIT_RESIST_DAM_RATE] = UNIT_FIELD_CRIT_RESIST_DAM_RATE,	--降暴伤害倍数(万分比)
	[EQUIP_ATTR_HIT_RATE] = UNIT_FIELD_HIT_RATE,	--命中率(万分比)
	[EQUIP_ATTR_MISS_RATE] = UNIT_FIELD_MISS_RATE,	--闪避率(万分比)
	[EQUIP_ATTR_STUN_RATE] = UNIT_FIELD_STUN_RATE,	--眩晕
	[EQUIP_ATTR_ROOTS_RATE] = UNIT_FIELD_ROOTS_RATE,	--定身
	[EQUIP_ATTR_SILENCE_RATE] = UNIT_FIELD_SILENCE_RATE,	--沉默
	[EQUIP_ATTR_CHAOS_RATE] = UNIT_FIELD_CHAOS_RATE,	--混乱
	[EQUIP_ATTR_CHARM_RATE] = UNIT_FIELD_CHARM_RATE,	--魅惑
	[EQUIP_ATTR_CONTROL_ENHANCE_RATE] = UNIT_FIELD_CONTROL_ENHANCE_RATE,	--控制增强
	[EQUIP_ATTR_CONTROL_RESIST_RATE] = UNIT_FIELD_CONTROL_RESIST_RATE,	--控制减免
	[EQUIP_ATTR_STRENGTH_ARMOR] = UNIT_FIELD_STRENGTH_ARMOR,	--强化护甲
}

function GetAttrSize()
	return #tBaseKey
end

function foreachAttr(callback)
	for attrId, binlogIndx in ipairs(tBaseKey) do
		if (callback) then
			callback(attrId, binlogIndx)
		end
	end
end

-- 获得属性id对应的精灵对象下标
function GetAttrUnitBinlogIndex(attrId)
	return tBaseKey[attrId]
end

function UnitInfo:addAllAttrRate(rate)
	foreachAttr(function (attrId, binlogIndx)
		local val = self:GetUInt32(binlogIndx)
		val = math.floor(val * (100 + rate) / 100)
		self:SetUInt32(binlogIndx, val)
	end)
end


--生物部分
function UnitInfo:SetBaseAttrs(info, bRecal, mul)
	for i = 1, #info do
		local attrtype = info[ i ][ 1 ]
		local attrval = info[ i ][ 2 ]
		if attrtype == EQUIP_ATTR_MAX_HEALTH then
			attrval = math.floor(attrval * mul)
		end
		if tBaseKey[attrtype] then
			local k = tBaseKey[attrtype]
			if(self:GetUInt32(k) ~= attrval)then
				self:SetUInt32(k, attrval)
			end
			if attrtype == EQUIP_ATTR_MAX_HEALTH and not bRecal then
				self:SetUInt32(UNIT_FIELD_HEALTH, attrval)
			end
		end	
	end
	--outFmtDebug('---> init creature attr %d %d %d', self:GetEntry(), self:GetUInt32(UNIT_FIELD_MAX_HEALTH), self:GetUInt32(UNIT_FIELD_HEALTH))
end

function UnitInfo:AddMaxHealth(val)
	self:SetMaxHealth(self:GetMaxHealth() + val)
end

--获取最大能量（包括：体力）
function UnitInfo:GetMaxPower(ntype) -- type: POWER_ENERGY 
	if(ntype >= MAX_POWERS)then
		return 0
	end
	if ntype == POWER_ENERGY then
		return self:GetUInt32(UNIT_FIELD_MAXPOWERS_ENERGY)
	end
	return 0
end

--设置最大能量（包括：体力）type: POWER_ENERGY 
function UnitInfo:SetMaxPower(ntype, val)
	if(ntype >= MAX_POWERS)then
		return 
	end
	if ntype == POWER_ENERGY then
		self:SetUInt32(UNIT_FIELD_MAXPOWERS_ENERGY, val)
	end
end

--增加当前血量
function UnitInfo:AddHealth(val)
	local current_hp = self:GetHealth()
	current_hp = current_hp + val
	self:SetHealth(current_hp)
end

--修改生物生命值
function UnitInfo:ModifyHealth(dval)
	unitLib.ModifyHealth(self.ptr, dval)
end

--获得能量（包括：体力）type: POWER_ENERGY 
function UnitInfo:GetPower(ntype)
	if(ntype >= MAX_POWERS)then
		return 0
	end
	if ntype == POWER_ENERGY then
		return self:GetUInt32(UNIT_FIELD_POWERS_ENERGY)
	end
	return 0
end

--设置能量（包括：体力）type:POWER_ENERGY
function UnitInfo:SetPower(ntype, val)
	if(ntype >= MAX_POWERS)then
		return
	end
	local maxPower = self:GetMaxPower(ntype)
	if(val > maxPower)then
		val = maxPower
	end
	if ntype == POWER_ENERGY then
		self:SetUInt32(UNIT_FIELD_POWERS_ENERGY, val)
	end
end

--修改能量（包括：体力）type: POWER_ENERGY
function UnitInfo:ModifyPower(ntype, dval)
	if(dval == 0 or ntype >= MAX_POWERS)then
		return
	end
	local curMp = self:GetPower(ntype)
	local val = curMp + dval
	local maxMp = self:GetMaxPower(ntype)
	if(val > maxMp)then
		val = maxMp
	end
	if(val < 0)then
		val = 0
	end
	self:SetPower(ntype, val)
end

--设置阵营
function UnitInfo:SetFaction(val)
	self:SetByte(UNIT_FIELD_BYTE_1, 1, val)
end

--获得阵营
function UnitInfo:GetFaction()
	return self:GetByte(UNIT_FIELD_BYTE_1, 1)
end

--获取地图ID
function UnitInfo:GetMapID()
	return self:GetUInt16(UNIT_FIELD_UINT16_1, 0)
end

--设置地图ID
function UnitInfo:SetMapID(val)
	self:SetUInt16(UNIT_FIELD_UINT16_1, 0, val)
end

--获得通用guid
function UnitInfo:GetGeneralGuid()
	return self:GetPlayerStr(PLAYER_STRING_FIELD_TELEPORT_GUID)
end

--获得跨服编号
function UnitInfo:GetKuafuNumber()
	return self:GetPlayerUInt16(PLAYER_INT_FIELD_KUAFU_NUMBER, 0)
end

--判断是否还活着
function UnitInfo:IsAlive()
	return self:GetByte(UNIT_FIELD_BYTE_0, 1) == DEATH_STATE_ALIVE
end

--设置生存状态
function UnitInfo:SetDeathState(val)
	self:SetByte(UNIT_FIELD_BYTE_0, 1,val)
end

--获取生命源泉
function UnitInfo:GetAutoHp()
	local hp = self:GetPlayerDouble(PLAYER_FIELD_AUTO_HP)
	return hp
end	

--设置生命源泉
function UnitInfo:SetAutoHp(val)
	self:SetPlayerDouble(PLAYER_FIELD_AUTO_HP, val)
end	

--获取内力源泉
function UnitInfo:GetAutoMp()
	local mp = self:GetPlayerDouble(PLAYER_FIELD_AUTO_MP)
	return mp
end	

--设置内力源泉
function UnitInfo:SetAutoMp(val)
	self:SetPlayerDouble(PLAYER_FIELD_AUTO_MP, val)
end

--获取角色id
function UnitInfo:GetGender()
	return self:GetByte(UNIT_FIELD_BYTE_1, 0)
end

--设置角色id
function UnitInfo:SetGender(val)
	self:SetByte(UNIT_FIELD_BYTE_1, 0, val)
end

--获取头像
function UnitInfo:GetHead()
	return self:GetByte(UNIT_FIELD_BYTE_3, 0)
end

--获取头像
function UnitInfo:SetHead(val)
	self:SetByte(UNIT_FIELD_BYTE_3, 0, val)
end

--获取种族
function UnitInfo:GetRace()
	return self:GetByte(UNIT_FIELD_BYTE_1, 2)
end

--获取种族
function UnitInfo:SetRace(val)
	self:SetByte(UNIT_FIELD_BYTE_1, 2, val)
end

--获取职业
function UnitInfo:GetProfession()
	return self:GetByte(UNIT_FIELD_BYTE_1, 3)
end

--获取职业
function UnitInfo:SetProfession(val)
	self:SetByte(UNIT_FIELD_BYTE_1, 3, val)
end

--设置生物变身（机器人的皮肤）
function UnitInfo:SetCreatureSkin(val)
	self:SetByte(UNIT_FIELD_BYTE_4, 2, val)
end

--获取生物变身（机器人的皮肤）
function UnitInfo:GetCreatureSkin()
	return self:GetByte(UNIT_FIELD_BYTE_4, 2)
end

--设置战斗力
function UnitInfo:SetForce(val)
	self:SetDouble(UNIT_FIELD_FORCE, val)
end

--获取战斗力
function UnitInfo:GetForce()
	return self:GetDouble(UNIT_FIELD_FORCE)
end

--设置npc标志
function UnitInfo:SetNpcFlags(val)
	self:SetUInt32(UNIT_FIELD_NPC_FLAG, val)
end

--获得npc标志
function UnitInfo:GetNpcFlags()
	return self:GetUInt32(UNIT_FIELD_NPC_FLAG)
end

--设置npc名字
function UnitInfo:SetName(val)
	self:SetStr(BINLOG_STRING_FIELD_NAME, val)
end

--获得pos槽中的buff id 
function UnitInfo:GetBuffByPos(pos)
	return self:GetUInt16(UNIT_FIELD_BUFF + math.floor(pos/2), math.fmod(pos,2))
end

--设置pos槽中的buff id 
function UnitInfo:SetBuffByPos(pos, val)
	self:SetUInt16(UNIT_FIELD_BUFF + math.floor(pos/2), math.fmod(pos,2), val)
end

--获得pos槽中的buff duration
function UnitInfo:GetBuffDuration(pos)
	return self:GetUInt16(UNIT_FIELD_BUFF_TM + math.floor(pos/2), math.fmod(pos,2))
end

--设置pos槽中的buff duration 
function UnitInfo:SetBuffDuration(pos, val)
	self:SetUInt16(UNIT_FIELD_BUFF_TM + math.floor(pos/2), math.fmod(pos,2), val)
end

--获取正在释放的技能ID
function UnitInfo:GetCurSpellId()
	return self:GetUInt32(UINT_FIELD_BOOS_CUR_SPELLID)
end

--设置正在释放的技能ID
function UnitInfo:SetCurSpellId(val)
	self:SetUInt32(UINT_FIELD_BOOS_CUR_SPELLID, val)
end

--获取释放的时间点
function UnitInfo:GetCurSpellTime()
	return self:GetUInt32(UINT_FIELD_BOOS_CUR_SPELL_TIME)
end

--设置释放的时间点
function UnitInfo:SetCurSpellTime(val)
	self:SetUInt32(UINT_FIELD_BOOS_CUR_SPELL_TIME, val)
end

--获取释放的次数
function UnitInfo:GetCurSpellCount()
	return self:GetUInt32(UINT_FIELD_BOOS_CUR_SPELL_LAST_COUNT)
end

--设置释放的次数
function UnitInfo:SetCurSpellCount(val)
	self:SetUInt32(UINT_FIELD_BOOS_CUR_SPELL_LAST_COUNT, val)
end

-- 减少释放次数
function UnitInfo:SubCurSpellCount()
	self:SubUInt32(UINT_FIELD_BOOS_CUR_SPELL_LAST_COUNT, 1)
end


--清理BOSS正在释放的技能
function UnitInfo:ClearCurSpell(is_ok)
	--被打断
	if not is_ok then
		if unitLib.HasBuff(self.ptr, BUFF_YINCHANG) then
			SpelladdBuff(self.ptr, BUFF_SHIHUA, self.ptr, 1, tb_buff_template[BUFF_SHIHUA].duration)
			SpelladdBuff(self.ptr, BUFF_YISHANG, self.ptr, 1, tb_buff_template[BUFF_YISHANG].duration)
			unitLib.RemoveBuff(self.ptr, BUFF_YINCHANG)
			ClearBuffFlags(self.ptr, BUFF_YINCHANG)
		end
	end
	self:SetCurSpellId(0)
	self:SetCurSpellTime(0)
	self:SetCurSpellCount(0)
end

--获得到坐标x,y的直线距离
function UnitInfo:GetDistance(x, y)
	local cas_x, cas_y = unitLib.GetPos(self.ptr)
	local dx = x - cas_x
	local dy = y - cas_y
	return math.sqrt(dy*dy + dx*dx)	
end

--判断是否能施法 返回值为true则可以施法
function UnitInfo:IsCanCast(spell_id)
	--local cant_casts = config.cant_cast		--取出配置
	--for k, buff in pairs(cant_casts) do
	--	if unitLib.HasBuff(self.ptr, buff) then
	--		return false				
	--	end
	--end
	if self:GetUnitFlags(UNIT_FIELD_FLAGS_CANT_CAST) then
		return false
	end
	return true
end

function UnitInfo:GetAngle(x, y)
	local cas_x, cas_y = unitLib.GetPos(self.ptr)
	local dx = x - cas_x
	local dy = y - cas_y
	
	local ang = math.atan2(dy, dx)
	if(ang < 0)then
		ang = ang + 2 * math.pi
	end
	return ang
end
	
--玩家部分
--获得付费等级
function UnitInfo:GetPayLevel()
	return self:GetByte(UNIT_FIELD_BYTE_2, 1)
end

--获取免费复活次数
function UnitInfo:GetFreeReliveCount()
	return self:GetByte(UNIT_FIELD_BYTE_2, 2)
end

--设置免费复活次数
function UnitInfo:SetFreeReliveCount(val)
	self:SetByte(UNIT_FIELD_BYTE_2, 2, val)
end

--获取玩家身上的money
function UnitInfo:GetMoney(type)
	if(type < MAX_MONEY_TYPE)then
		return self:GetPlayerDouble(PLAYER_EXPAND_INT_MONEY + type*2)
	end
	return 0
end

-- --扣金钱
-- function UnitInfo:SubMoney(money_type, oper_type, val, p1, p2, p3, p4, p5)
-- 	if(val <= 0)then
-- 		return false
-- 	end
-- 	if(p1 == nil)then
-- 		p1 = ''
-- 	end
-- 	if(p2 == nil)then
-- 		p2 = 0
-- 	end
-- 	if(p3 == nil)then
-- 		p3 = 0
-- 	end
-- 	if(p4 == nil)then
-- 		p4 = 0
-- 	end
-- 	if(p5 == nil)then
-- 		p5 = 0
-- 	end
-- 	return playerLib.SendSubMoney(self.ptr, money_type, oper_type, val, p1, p2, p3, p4, p5)
-- end

--金钱增加
function UnitInfo:AddMoney(money_type, oper_type, val, p1, p2, p3, p4, p5)
	if(val <= 0)then
		return false
	end
	if(p1 == nil)then
		p1 = ''
	end
	if(p2 == nil)then
		p2 = 0
	end
	if(p3 == nil)then
		p3 = 0
	end
	if(p4 == nil)then
		p4 = 0
	end
	if(p5 == nil)then
		p5 = 0
	end
	return playerLib.SendAddMoney(self.ptr, money_type, oper_type, val, p1, p2, p3, p4, p5)
end

--通过物品模板获取金钱数量
function UnitInfo:GetMoneyForEntry(entry)
	if(entry == Item_Loot_Silver)then
		return self:GetMoney(MONEY_TYPE_SILVER)
	elseif(entry == Item_Loot_Bind_Gold)then
		return self:GetMoney(MONEY_TYPE_BIND_GOLD)
	elseif(entry == Item_Loot_Gold)then
		return self:GetMoney(MONEY_TYPE_GOLD_INGOT)	
	end
end

-- --通过物品模板金钱减少
-- function UnitInfo:SubMoneyForEntry(entry,log_type, val)
-- 	local money_type
-- 	if(entry == Item_Loot_Silver)then
-- 		money_type = MONEY_TYPE_SILVER
-- 	elseif(entry == Item_Loot_Bind_Gold)then
-- 		money_type = MONEY_TYPE_BIND_GOLD
-- 	elseif(entry == Item_Loot_Gold)then
-- 		money_type = MONEY_TYPE_GOLD_INGOT
-- 	end
-- 	return self:SubMoney(money_type,log_type,val)
-- end

--获取元宝和绑元数量
function UnitInfo:GetGoldMoney()
	return self:GetMoney(MONEY_TYPE_GOLD_INGOT) + self:GetMoney(MONEY_TYPE_BIND_GOLD)
end

--获取帮派id
function UnitInfo:GetFactionId()
	return self:GetPlayerStr(PLAYER_STRING_FIELD_FACTION_GUID)
end
--获取队伍id
function UnitInfo:GetGroupId()
	return self:GetStr(UNIT_STRING_FIELD_GROUP)
end

--获取PK值
function UnitInfo:GetPKValue()
	return self:GetPlayerUInt32(PLAYER_APPD_INT_FIELD_PK_VALUE)
end

--获得精灵下标的flag标志
function UnitInfo:GetUnitFlags(index)
	return self:GetBit(UNIT_FIELD_FLAGS, index)
end
--设置精灵下标的flag标志
function UnitInfo:SetUnitFlags(index)
	self:SetBit(UNIT_FIELD_FLAGS, index)
end
--设置精灵下标的flag标志
function UnitInfo:UnSetUnitFlags(index)
	self:UnSetBit(UNIT_FIELD_FLAGS, index)
end

--获得玩家主下标的flag标志
function UnitInfo:GetPlayerFlags(index)
	return self:GetPlayerBit(PLAYER_FIELD_FLAGS, index)
end
--设置玩家主下标的flag标志
function UnitInfo:SetPlayerFlags(index)
	self:SetPlayerBit(PLAYER_FIELD_FLAGS, index)
end
--设置玩家主下标的flag标志
function UnitInfo:UnSetPlayerFlags(index)
	self:UnSetPlayerBit(PLAYER_FIELD_FLAGS, index)
end

--获取攻击模式
function UnitInfo:GetAttackMode()
	return self:GetByte(UNIT_FIELD_BYTE_3, 3)
end

--设置攻击模式
function UnitInfo:SetAttackMode(val)
	if self:GetAttackMode() == val then
		return
	end
	self:SetByte(UNIT_FIELD_BYTE_3, 3, val)
	unitLib.InvalidFriendlyCache(self.ptr)
end

--获取当前地图进度
function UnitInfo:GetMapStatus()
	return self:GetPlayerUInt32(PLAYER_EXPAND_INT_MAP_STATUS)
end

--设置当前地图进度
function UnitInfo:SetMapStatus(status)
	if(self:GetMapStatus() ~= status)then
		self:SetPlayerUInt32(PLAYER_EXPAND_INT_MAP_STATUS,status)
	end
end

--获得pvp状态
function UnitInfo:GetPVPState()
	return self:GetPlayerBit(PLAYER_FIELD_FLAGS, UNIT_FIELD_FLAGS_IS_PVP)
end

--获得pve状态
function UnitInfo:GetPVEState()
	--TODO: return self:GetPlayerBit(PLAYER_FIELD_FLAGS, UNIT_FIELD_FLAGS_IS_PVE)
end

--获得备战状态
function UnitInfo:GetCastState(  )
	--TODO: return self:GetPlayerBit(PLAYER_FIELD_FLAGS, UNIT_FIELD_FLAGS_IS_CAST)
end

--取消备战标识
function UnitInfo:UnSetCastState( )
	if self:GetCastState() then
		self:UnSetPlayerBit(PLAYER_FIELD_FLAGS, UNIT_FIELD_FLAGS_IS_CAST)
	end
end

--获取是否挂机
function UnitInfo:GetHungUp()
	return self:GetPlayerBit(PLAYER_SCENED_INT_FLAGS, PLAYER_SCENED_FLAS_HUNG_UP)
end

--set是否挂机
function UnitInfo:SetHungUp()
	self:SetPlayerBit(PLAYER_SCENED_INT_FLAGS, PLAYER_SCENED_FLAS_HUNG_UP)
end

--set是否挂机
function UnitInfo:UnSetHungUp()
	self:UnSetPlayerBit(PLAYER_SCENED_INT_FLAGS, PLAYER_SCENED_FLAS_HUNG_UP)
end

--设置开始挂机时间点
function UnitInfo:SetHungUpStartTime(val)
	self:SetPlayerUInt32(PLAYER_HT_INT_FIELD_HUNGUP_START_TIME, val)
end

--获取开始挂机时间点
function UnitInfo:GetHungUpStartTime()
	return self:GetPlayerUInt32(PLAYER_HT_INT_FIELD_HUNGUP_START_TIME)
end

--获取玩家退出副本时的x坐标
function UnitInfo:GetExitanceDBPosx()
	return self:GetPlayerFloat(PLAYER_EXPAND_INT_DB_POS_X)
end

--设置玩家退出副本时的x坐标
function UnitInfo:SetExitanceDBPosx(val)
	self:SetPlayerFloat(PLAYER_EXPAND_INT_DB_POS_X, val)
end

--获取玩家退出副本时的y坐标
function UnitInfo:GetExitanceDBPosy()
	return self:GetPlayerFloat(PLAYER_EXPAND_INT_DB_POS_Y)
end

--设置玩家退出副本时的y坐标
function UnitInfo:SetExitanceDBPosy(val)
	self:SetPlayerFloat(PLAYER_EXPAND_INT_DB_POS_Y, val)
end

--获取玩家退出副本时的地图id
function UnitInfo:GetExitanceDBMapid()
	return self:GetPlayerUInt32(PLAYER_EXPAND_INT_DB_MAP)
end

-- 获得怒气
function UnitInfo:GetSP()
	return self:GetUInt32(UNIT_FIELD_ANGER)
end

-- 设置怒气
function UnitInfo:SetSP(value)
	self:SetUInt32(UNIT_FIELD_ANGER, value)
end

-- 增加怒气
function UnitInfo:AddSP(value)
	-- 人形怪不加愤怒
	if self:GetTypeID() == TYPEID_UNIT then
		return
	end
	local cas = playerLib.GetSlotSpell(self.ptr, SLOT_ANGER)
	if cas > 0 then
		local angerLimit = tb_anger_limit[cas].limit
		local currAnger = self:GetSP()
		if currAnger >= angerLimit then
			return
		end
		local nextAnger = currAnger + value
		if nextAnger > angerLimit then
			nextAnger = angerLimit
		end
		self:SetSP(nextAnger)
	end
end

--设置玩家退出副本时的地图id
function UnitInfo:SetExitanceDBMapid(val)
	self:SetPlayerUInt32(PLAYER_EXPAND_INT_DB_MAP, val)
end

--获取防沉迷
function UnitInfo:GetFCMLoginTime()
	return self:GetPlayerUInt32(PLAYER_EXPAND_INT_FCM_LOGIN_TIME)
end

--获得任务完成列表bit
function UnitInfo:GetQuestCompleteListFlag(off)
	local off_index = math.floor(off/32)
	local off_bit = math.fmod(off, 32)
	return self:GetPlayerBit(PLAYER_SCENED_INT_FIELD_QUEST_COMPLETE_LIST + off_index, off_bit)
end

--获得玩家当前经验
function UnitInfo:GetPlayerCurExp()
	return self:GetPlayerDouble(PLAYER_EXPAND_INT_XP)
end

--获得玩家下一级经验
function UnitInfo:GetPlayerNextLevelExp()
	return self:GetPlayerDouble(PLAYER_EXPAND_INT_NEXT_LEVEL_XP)
end

-- 获取玩家基础属性
function UnitInfo:GetPlayerBaseAttr(offset)
	return self:GetPlayerDouble(PLAYER_SCENED_INT_FIELD_BASE_ATTR_START + offset*2)
end

-- 设置玩家基础属性
function UnitInfo:SetPlayerBaseAttr(offset, val)
	self:SetPlayerDouble(PLAYER_SCENED_INT_FIELD_BASE_ATTR_START + offset*2, val)
end

-- 获取玩家固定属性
function UnitInfo:GetPlayerFixedAttr(offset)
	return self:GetPlayerDouble(PLAYER_SCENED_INT_FIELD_FIXED_ATTR_START + offset*2)
end

-- 设置玩家固定属性
function UnitInfo:SetPlayerFixedAttr(offset, val)
	self:SetPlayerDouble(PLAYER_SCENED_INT_FIELD_FIXED_ATTR_START + offset*2, val)
end
	
-- 获取玩家名字（公告的时候用）
function UnitInfo:GetTextName()
	local sName = self:GetName()
	if sName ~= "" then
		local tName = lua_string_split(sName,",")
		sName = tName[#tName]
	end
	return sName
end

-- 是否已经接受任务
function UnitInfo:IsHasAcceptQuest(questid)
	if(self:GetQuestCompleteListFlag(questid) or playerLib.HasQuest(self.ptr,questid))then
		return true
	end
	return false
end

--获取挂机保护时间
function UnitInfo:GetGuaJiBaoHu()
	return self:GetPlayerUInt32(PLAYER_APPD_INT_FIELD_GUAJIBAOHU)
end

--设置挂机保护时间
function UnitInfo:SetGuaJiBaoHu(val)
	if self:GetGuaJiBaoHu() ~= val then
		self:SetPlayerUInt32(PLAYER_APPD_INT_FIELD_GUAJIBAOHU, val)
		if val < os.time() then
			self:UnSetPlayerFlags(UNIT_FIELD_FLAGS_BUFF_DAZUO)
		else
			self:SetPlayerFlags(UNIT_FIELD_FLAGS_BUFF_DAZUO)
		end
	end
end

-- 清除玩家buff对应的flag标识
function UnitInfo:ClearPlayerBuffFlags()
	local flag = false
	local cant_casts = config.cant_cast		--取出配置
	for k, buff in pairs(cant_casts) do
		if unitLib.HasBuff(self.ptr, buff) then
			flag = true		--如果玩家身上还有类似的buff，则暂不清flag
			break
		end
	end
	if(flag and self:GetPlayerFlags(UNIT_FIELD_FLAGS_CANT_CAST) == false)then
		self:SetPlayerFlags(UNIT_FIELD_FLAGS_CANT_CAST)
	elseif(self:GetPlayerFlags(UNIT_FIELD_FLAGS_CANT_CAST))then
		self:UnSetPlayerFlags(UNIT_FIELD_FLAGS_CANT_CAST)
	end
	flag = false
	local cant_moves = config.cant_move		--取出配置
	for k, buff in pairs(cant_moves) do
		if unitLib.HasBuff(self.ptr, buff) then
			flag = true		--如果玩家身上还有类似的buff，则暂不清flag
			break
		end
	end
	if(flag and self:GetPlayerFlags(UNIT_FIELD_FLAGS_CANT_MOVE) == false)then
		self:SetPlayerFlags(UNIT_FIELD_FLAGS_CANT_MOVE)
	elseif(self:GetPlayerFlags(UNIT_FIELD_FLAGS_CANT_MOVE))then
		self:UnSetPlayerFlags(UNIT_FIELD_FLAGS_CANT_MOVE)
	end
end

--添加总挂机时间
function UnitInfo:AddHungUpTime(val)
	self:AddPlayerUInt32(PLAYER_HT_INT_FIELD_ALL_HUNGUP_TIME, val)
end

--添加每日挂机时间
function UnitInfo:AddHungUpDailyTime(val)
	self:AddPlayerUInt32(PLAYER_HT_INT_FIELD_DAILY_HUNGUP_TIME, val)
end

-- 获取使用游戏对象时间
function UnitInfo:GetUseGameObjectTime()
	return self:GetPlayerUInt32(PLAYER_SCENED_INT_FIELD_USE_GAMEOBJECT_TIME)
end

-- 设置使用游戏对象时间
function UnitInfo:SetUseGameObjectTime(val)
	self:SetPlayerUInt32(PLAYER_SCENED_INT_FIELD_USE_GAMEOBJECT_TIME, val)
end

-- 获取使用游戏对象标识
function UnitInfo:GetUseGameObjectFlag()
	return self:GetUnitFlags(UNIT_FIELD_FLAGS_USE_GAMEOBJECT)
end

-- 设置使用游戏对象标识
function UnitInfo:SetUseGameObjectFlag(val)
	self:SetUnitFlags(UNIT_FIELD_FLAGS_USE_GAMEOBJECT)
end

function UnitInfo:UnSetUseGameObjectFlag(val)
	self:UnSetUnitFlags(UNIT_FIELD_FLAGS_USE_GAMEOBJECT)
end

--开始使用游戏对象
function UnitInfo:StartUseGameObject()
	--[[
	if self:GetUseGameObjectTime() == 0 then
		self:SetUseGameObjectTime(os.time())
	end
	]]
	if not self:GetUnitFlags(UNIT_FIELD_FLAGS_USE_GAMEOBJECT) then
		self:SetUnitFlags(UNIT_FIELD_FLAGS_USE_GAMEOBJECT)
	end
	
end

-- 移除使用游戏对象标识
function UnitInfo:RemoveUseGameObjectFlag()
	--[[
	if self:GetUseGameObjectTime() ~= 0 then
		self:SetUseGameObjectTime(0)
	end
	]]
	if self:GetUnitFlags(UNIT_FIELD_FLAGS_USE_GAMEOBJECT) then
		self:UnSetUnitFlags(UNIT_FIELD_FLAGS_USE_GAMEOBJECT)
	end
end

--获取玩家跳跃CD
function UnitInfo:GetPlayerJumpCd()
	return self:GetPlayerUInt32(PLAYER_SCENED_INT_FIELD_PLAYER_JUMP_CD)
end

--设置玩家跳跃CD
function UnitInfo:SetPlayerJumpCd(val)
	self:SetPlayerUInt32(PLAYER_SCENED_INT_FIELD_PLAYER_JUMP_CD,val)
end

-- 获取玩家技能等级对应skill_base表的索引
function UnitInfo:GetSpellLvIndex(spell_id)
	return tb_skill_base[spell_id].uplevel_id[1] + self:GetSpellLevel(spell_id) - 1
end

-- 获取精灵对象的类型（玩家、精灵）
function GetUnitTypeID(spirit)
	if not spirit then
		print( debug.traceback() )
	end
	return binLogLib.GetByte(spirit, UNIT_FIELD_BYTE_0, 0)
end

-- 获得帮派的guid
function GetFactionGuid(player_ptr)
	local player_data_ptr = playerLib.GetSession(player_ptr)
	return binLogLib.GetStr(player_data_ptr, PLAYER_STRING_FIELD_FACTION_GUID)
end

-- 获得组队模式的id
function GetGroupModeId(player_ptr)
	local player_data_ptr = playerLib.GetSession(player_ptr)
	return binLogLib.GetStr(player_data_ptr, PLAYER_STRING_FIELD_GROUP_PEACE_ID)
end

function GetPlayerGuid(player_ptr)
	local player_data_ptr = playerLib.GetSession(player_ptr)
	return binLogLib.GetStr(player_data_ptr, BINLOG_STRING_FIELD_GUID)
end


function GetAailableSectionid(player_ptr)
	local player_data_ptr = playerLib.GetSession(player_ptr)
	local passSectionId = binLogLib.GetUInt32(player_data_ptr, PLAYER_INT_FIELD_TRIAL_FINISHED_SECTIONID)
	return onGetAvailableSectionId(passSectionId)
end

-- 获得vip等级
function UnitInfo:GetVIP()
	local vipLevel = self:GetUInt32(PLAYER_FIELD_VIP_LEVEL)
	if self:GetUInt32(PLAYER_FIELD_VIP_TIME_OUT) < os.time() then
		vipLevel = 0
	end
	return vipLevel
end

-- 设置unit VIP等级
function UnitInfo:SetUnitVIP(vipLevel)
	self:SetUInt32(UINT_FIELD_VIP_LEVEL, vipLevel)
end

--获取VIP时间
function UnitInfo:GetVipEndTime(lv)
	if lv > MAX_GAME_VIP_TYPE then
		return
	end
	return self:GetPlayerUInt32(PLAYER_FIELD_VIP1_TIME_OUT + lv - 1)
end

--设置VIP时间
function UnitInfo:SetVipEndTime(lv,tm)
	if lv > MAX_GAME_VIP_TYPE then
		return
	end
	self:SetPlayerUInt32(PLAYER_FIELD_VIP1_TIME_OUT + lv - 1, tm)
end

--获得掉落物品的归属者guid
function UnitInfo:GetLootOwnerGuid(  )
	return self:GetStr(UNIT_STRING_FIELD_LOOT_OWNER_GUID)
end

--设置掉落物品的归属者guid
function UnitInfo:SetLootOwnerGuid( guid )
	if self:GetLootOwnerGuid() ~= guid then
		self:SetStr(UNIT_STRING_FIELD_LOOT_OWNER_GUID, guid)
	end
end

--获得掉落物品的归属者名字
function UnitInfo:GetLootOwnerName(  )
	return self:GetStr(UNIT_STRING_FIELD_LOOT_OWNER_NAME)
end

--设置掉落物品的归属者名字
function UnitInfo:SetLootOwnerName( name )
	if self:GetLootOwnerName() ~= name then
		self:SetStr(UNIT_STRING_FIELD_LOOT_OWNER_NAME, name)
	end
end

--获取被动技能ID
function UnitInfo:GetPassiveSpellId( index)
	return self:GetPlayerUInt16(PLAYER_SCENED_INT_FIELD_PASSIVE_SPELL + index * MAX_SLOT_PASSIVE_COUNT + SLOT_PASSIVE_SPELL_ID ,0)
end

--设置被动技能ID
function UnitInfo:SetPassiveSpellId( index, val)
	self:SetPlayerUInt16(PLAYER_SCENED_INT_FIELD_PASSIVE_SPELL + index * MAX_SLOT_PASSIVE_COUNT + SLOT_PASSIVE_SPELL_ID,0, val)
end

--获取被动技能等级
function UnitInfo:GetPassiveSpellLevel( index)
	return self:GetPlayerUInt16(PLAYER_SCENED_INT_FIELD_PASSIVE_SPELL + index * MAX_SLOT_PASSIVE_COUNT + SLOT_PASSIVE_SPELL_ID,1)
end

--设置被动技能等级
function UnitInfo:SetPassiveSpellLevel( index, val)
	self:SetPlayerUInt16(PLAYER_SCENED_INT_FIELD_PASSIVE_SPELL + index * MAX_SLOT_PASSIVE_COUNT + SLOT_PASSIVE_SPELL_ID,1, val)
end

--获取被动技能CD
function UnitInfo:GetPassiveSpellCD( index)
	return self:GetPlayerUInt32(PLAYER_SCENED_INT_FIELD_PASSIVE_SPELL + index * MAX_SLOT_PASSIVE_COUNT + SLOT_PASSIVE_SPELL_CD)
end
--设置被动技能CD
function UnitInfo:SetPassiveSpellCD( index, val)
	self:SetPlayerUInt32(PLAYER_SCENED_INT_FIELD_PASSIVE_SPELL + index * MAX_SLOT_PASSIVE_COUNT + SLOT_PASSIVE_SPELL_CD, val)
end

--完成任务后某系统开启做点什么
function UnitInfo:ActivedSystemToDoSomething(questid)
	for key,val in pairs(tb_system_open)do
		local active_type = val.type
		if active_type == 1 then--任务激活
			if questid == val.value then
				playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_ACTIVE_SYSTEM, key)
			end
		end
	end

	--新手引导，技能激活
	if questid == 3 then
		--发到应用服激活技能落雁斩
		playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_ACTIVE_SPELL, 1)
	--elseif questid == 8 then
	--	--发到应用服激活技能破天三试一段
	--	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_ACTIVE_SPELL, 10)
	end
end

-- 同一个队伍返回1, 否则返回0
function UnitInfo:sameTeamCode(playerInfo)
	return 0
end

-- 同一个家族返回1, 否则返回0
function UnitInfo:sameFamilyCode(playerInfo)
	return 0
end

-- 生成是否能成为攻击目标的掩码
function UnitInfo:generateBattleMask(killerInfo)
	local bbit = self:GetBattleMode() * 4 + self:sameTeamCode(killerInfo) * 2 + self:sameFamilyCode(killerInfo)
	return bit.lshift(1, bbit)
end

-- 获得和平模式CD
function UnitInfo:GetPeaceModeCD()
	return self:GetPlayerUInt32(PLAYER_FIELD_PEACE_MODE_CD)
end

-- 设置和平模式CD
function UnitInfo:SetPeaceModeCD(cd)
	self:SetPlayerUInt32(PLAYER_FIELD_PEACE_MODE_CD, cd)
end

-- 获得战斗模式
function UnitInfo:GetBattleMode()
	return self:GetPlayerUInt16(PLAYER_FIELD_NOTORIETY, 0)
end

function unitGetBattleMode(player_ptr)
	local player_data_ptr = playerLib.GetSession(player_ptr)
	return binLogLib.GetUInt16(player_data_ptr, PLAYER_FIELD_NOTORIETY, 0)
end

-- 设置战斗模式
function UnitInfo:SetBattleMode(value)
	self:SetPlayerUInt16(PLAYER_FIELD_NOTORIETY, 0, value)
end

-- 变成和平模式
function UnitInfo:ChangeToPeaceModeAfterTeleport()
	self:SetBattleMode(PEACE_MODE)
end

-- 变成组队模式
function UnitInfo:SetToGroupMode(groupId)
	self:SetBattleMode(GROUP_MODE)
	self:SetPlayerStr(PLAYER_STRING_FIELD_GROUP_PEACE_ID, groupId)
end

-- 变成家族模式
function UnitInfo:ChangeToFamilyMode()
	self:SetBattleMode(FAMILY_MODE)
end

-- 获得恶名值
function UnitInfo:GetNotoriety()
	return self:GetPlayerUInt16(PLAYER_FIELD_NOTORIETY, 1)
end

-- 是否是和平模式
function UnitInfo:isPeaceMode()
	return self:GetBattleMode() == PEACE_MODE
end

-- 设置恶名值
function UnitInfo:SetNotoriety(value)
	self:SetPlayerUInt16(PLAYER_FIELD_NOTORIETY, 1, value)
end



function UnitInfo:GetGroupModeId()
	return self:GetPlayerStr(PLAYER_STRING_FIELD_GROUP_PEACE_ID)
end

-- 模式转换
function UnitInfo:modeChange(mode)
	local prev = self:GetBattleMode()
	if mode == prev then
		return
	end
	
	self:SetBattleMode(mode)
end

-- 是否有坐骑
function UnitInfo:IsMountActived()
	local actived = self:GetMountLevel()
	return actived > 0
end

-- 坐骑阶数
function UnitInfo:GetMountLevel()
	return self:GetByte(UNIT_FIELD_MOUNT_LEVEL, 0)
end

-- 坐骑星级
function UnitInfo:GetMountStar()
	return self:GetByte(UNIT_FIELD_MOUNT_LEVEL, 1)
end

-- 骑乘状态
function UnitInfo:rideFlag()
	return self:GetByte(UNIT_FIELD_MOUNT_LEVEL, 2)
end

-- 幻化id
function UnitInfo:GetCurrIllusionId()
	return self:GetByte(UNIT_FIELD_MOUNT_LEVEL, 3)
end

-- 是否骑乘
function UnitInfo:isRide()
	return self:IsMountActived() and self:rideFlag() > 0
end

-- 骑乘
function UnitInfo:MountRide()
	--playerLib.SendMountJumpDown(self.ptr, 1)
	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_RIDE, 1)
end

-- 下骑
function UnitInfo:MountUnride()
	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_RIDE, 0)
end

-- 获得当前生命
function UnitInfo:GetHealth()
	return self:GetUInt32(UNIT_FIELD_HEALTH)
end

function UnitInfo:SetHealth(val)
	self:SetUInt32(UNIT_FIELD_HEALTH, val)
end

-- 获得最大生命
function UnitInfo:GetMaxHealth()
	return self:GetUInt32(UNIT_FIELD_MAX_HEALTH)
end

-- 获得攻击力
function UnitInfo:GetDamage()
	return self:GetUInt32(UNIT_FIELD_DAMAGE)
end

-- 获得防御力
function UnitInfo:GetArmor()
	return self:GetUInt32(UNIT_FIELD_ARMOR)
end

-- 获得命中
function UnitInfo:GetHit()
	return self:GetUInt32(UNIT_FIELD_HIT)
end

-- 获得闪避
function UnitInfo:GetMiss()
	return self:GetUInt32(UNIT_FIELD_MISS)
end

-- 获得暴击
function UnitInfo:GetCrit()
	return self:GetUInt32(UNIT_FIELD_CRIT)
end

-- 获得坚韧
function UnitInfo:GetTough()
	return self:GetUInt32(UNIT_FIELD_TOUGH)
end

-- 获得攻击速度
function UnitInfo:GetAttackSpeed()
	return self:GetUInt32(UNIT_FIELD_ATTACK_SPEED)
end

-- 获得移动速度
function UnitInfo:GetMoveSpeed()
	return self:GetUInt32(UNIT_FIELD_MOVE_SPEED)
end

-- 获得忽视防御
function UnitInfo:GetIgnoreArmor()
	return self:GetUInt32(UNIT_FIELD_IGNORE_ARMOR)
end

-- 获得忽视闪避
function UnitInfo:GetIgnoreMiss()
	return self:GetUInt32(UNIT_FIELD_IGNORE_MISS)
end

-- 获得生命值回复
function UnitInfo:GetRecovery()
	return self:GetUInt32(UNIT_FIELD_RECOVERY)
end

-- 获得伤害加深(万分比)
function UnitInfo:GetDamageAmplifyRate()
	return self:GetUInt32(UNIT_FIELD_DAMAGE_AMPLIFY_RATE)
end

-- 获得伤害减免(万分比)
function UnitInfo:GetDamageResistRate()
	return self:GetUInt32(UNIT_FIELD_DAMAGE_RESIST_RATE)
end

-- 获得反弹伤害(万分比)
function UnitInfo:GetDamageReturnRate()
	return self:GetUInt32(UNIT_FIELD_DAMAGE_RETURN_RATE)
end

-- 获得吸血光环(万分比)
function UnitInfo:GetVampiricRate()
	return self:GetUInt32(UNIT_FIELD_VAMPIRIC_RATE)
end

-- 获得回复效率(万分比)
function UnitInfo:GetRecoveryRate()
	return self:GetUInt32(UNIT_FIELD_RECOVERY_RATE)
end

-- 获得���击率(万分比)
function UnitInfo:GetCritRate()
	return self:GetUInt32(UNIT_FIELD_CRIT_RATE)
end

-- 获得抗暴率(万分比)
function UnitInfo:GetCritResistRate()
	return self:GetUInt32(UNIT_FIELD_CRIT_RESIST_RATE)
end

-- 获得暴击伤害倍数(万分比)
function UnitInfo:GetCritDamRate()
	return self:GetUInt32(UNIT_FIELD_CRIT_DAM_RATE)
end

-- 获得降暴伤害倍数(万分比)
function UnitInfo:GetCritResistDamRate()
	return self:GetUInt32(UNIT_FIELD_CRIT_RESIST_DAM_RATE)
end

-- 获得命中率(万分比)
function UnitInfo:GetHitRate()
	return self:GetUInt32(UNIT_FIELD_HIT_RATE)
end

-- 获得闪避率(万分比)
function UnitInfo:GetMissRate()
	return self:GetUInt32(UNIT_FIELD_MISS_RATE)
end

-- 获得眩晕
function UnitInfo:GetStunRate()
	return self:GetUInt32(UNIT_FIELD_STUN_RATE)
end

-- 获得定身
function UnitInfo:GetRootsRate()
	return self:GetUInt32(UNIT_FIELD_ROOTS_RATE)
end

-- 获得沉默
function UnitInfo:GetSilenceRate()
	return self:GetUInt32(UNIT_FIELD_SILENCE_RATE)
end

-- 获得混乱
function UnitInfo:GetChaosRate()
	return self:GetUInt32(UNIT_FIELD_CHAOS_RATE)
end

-- 获得魅惑
function UnitInfo:GetCharmRate()
	return self:GetUInt32(UNIT_FIELD_CHARM_RATE)
end

-- 获得控制增强
function UnitInfo:GetControlEnhanceRate()
	return self:GetUInt32(UNIT_FIELD_CONTROL_ENHANCE_RATE)
end

-- 获得控制减免
function UnitInfo:GetControlResistRate()
	return self:GetUInt32(UNIT_FIELD_CONTROL_RESIST_RATE)
end

-- 获得强化护甲
function UnitInfo:GetStrengthArmor()
	return self:GetUInt32(UNIT_FIELD_STRENGTH_ARMOR)
end

-- 设置最大生命
function UnitInfo:SetMaxHealth(val)
	self:SetUInt32(UNIT_FIELD_MAX_HEALTH, val)
end

-- 设置攻击力
function UnitInfo:SetDamage(val)
	self:SetUInt32(UNIT_FIELD_DAMAGE, val)
end

-- 设置防御力
function UnitInfo:SetArmor(val)
	self:SetUInt32(UNIT_FIELD_ARMOR, val)
end

-- 设置命中
function UnitInfo:SetHit(val)
	self:SetUInt32(UNIT_FIELD_HIT, val)
end

-- 设置闪避
function UnitInfo:SetMiss(val)
	self:SetUInt32(UNIT_FIELD_MISS, val)
end

-- 设置暴击
function UnitInfo:SetCrit(val)
	self:SetUInt32(UNIT_FIELD_CRIT, val)
end

-- 设置坚韧
function UnitInfo:SetTough(val)
	self:SetUInt32(UNIT_FIELD_TOUGH, val)
end

-- 设置攻击速度
function UnitInfo:SetAttackSpeed(val)
	self:SetUInt32(UNIT_FIELD_ATTACK_SPEED, val)
end

-- 设置移动速度
function UnitInfo:SetMoveSpeed(val)
	self:SetUInt32(UNIT_FIELD_MOVE_SPEED, val)
end

-- 设置忽视防御
function UnitInfo:SetIgnoreArmor(val)
	self:SetUInt32(UNIT_FIELD_IGNORE_ARMOR, val)
end

-- 设置忽视闪避
function UnitInfo:SetIgnoreMiss(val)
	self:SetUInt32(UNIT_FIELD_IGNORE_MISS, val)
end

-- 设置生命值回复
function UnitInfo:SetRecovery(val)
	self:SetUInt32(UNIT_FIELD_RECOVERY, val)
end

-- 设置伤害加深(万分比)
function UnitInfo:SetDamageAmplifyRate(val)
	self:SetUInt32(UNIT_FIELD_DAMAGE_AMPLIFY_RATE, val)
end

-- 设置伤害减免(万分比)
function UnitInfo:SetDamageResistRate(val)
	self:SetUInt32(UNIT_FIELD_DAMAGE_RESIST_RATE, val)
end

-- 设置反弹伤害(万分比)
function UnitInfo:SetDamageReturnRate(val)
	self:SetUInt32(UNIT_FIELD_DAMAGE_RETURN_RATE, val)
end

-- 设置吸血光环(万分比)
function UnitInfo:SetVampiricRate(val)
	self:SetUInt32(UNIT_FIELD_VAMPIRIC_RATE, val)
end

-- 设置回复效率(万分比)
function UnitInfo:SetRecoveryRate(val)
	self:SetUInt32(UNIT_FIELD_RECOVERY_RATE, val)
end

-- 设置暴击率(万分比)
function UnitInfo:SetCritRate(val)
	self:SetUInt32(UNIT_FIELD_CRIT_RATE, val)
end

-- 设置抗暴率(万分比)
function UnitInfo:SetCritResistRate(val)
	self:SetUInt32(UNIT_FIELD_CRIT_RESIST_RATE, val)
end

-- 设置暴击伤害倍数(万分比)
function UnitInfo:SetCritDamRate(val)
	self:SetUInt32(UNIT_FIELD_CRIT_DAM_RATE, val)
end

-- 设置降暴伤害倍数(万分比)
function UnitInfo:SetCritResistDamRate(val)
	self:SetUInt32(UNIT_FIELD_CRIT_RESIST_DAM_RATE, val)
end

-- 设置命中率(万分比)
function UnitInfo:SetHitRate(val)
	self:SetUInt32(UNIT_FIELD_HIT_RATE, val)
end

-- 设置闪避率(万分比)
function UnitInfo:SetMissRate(val)
	self:SetUInt32(UNIT_FIELD_MISS_RATE, val)
end

-- 设置眩晕
function UnitInfo:SetStunRate(val)
	self:SetUInt32(UNIT_FIELD_STUN_RATE, val)
end

-- 设置定身
function UnitInfo:SetRootsRate(val)
	self:SetUInt32(UNIT_FIELD_ROOTS_RATE, val)
end

-- 设置沉默
function UnitInfo:SetSilenceRate(val)
	self:SetUInt32(UNIT_FIELD_SILENCE_RATE, val)
end

-- 设置混乱
function UnitInfo:SetChaosRate(val)
	self:SetUInt32(UNIT_FIELD_CHAOS_RATE, val)
end

-- 设置魅惑
function UnitInfo:SetCharmRate(val)
	self:SetUInt32(UNIT_FIELD_CHARM_RATE, val)
end

-- 设置控制增强
function UnitInfo:SetControlEnhanceRate(val)
	self:SetUInt32(UNIT_FIELD_CONTROL_ENHANCE_RATE, val)
end

-- 设置控制减免
function UnitInfo:SetControlResistRate(val)
	self:SetUInt32(UNIT_FIELD_CONTROL_RESIST_RATE, val)
end

-- 设置强化护甲
function UnitInfo:SetStrengthArmor(val)
	self:SetUInt32(UNIT_FIELD_STRENGTH_ARMOR, val)
end

--属性重算（场景服）
function DoRecalculationAttrs(attrBinlog, player, runtime, bRecal)
	
end

-- 升级后增加的hp值 需要在场景服先算, 保证升级了血先满
function CalLevelUpRaisedHp(prev, curr)
	return calLevelHp(curr) - calLevelHp(prev)
end

function calLevelHp(level)
	local config = tb_char_level[level].prop
	if config then
		for _, attrInfo in pairs(config) do
			if attrInfo[ 1 ] == EQUIP_ATTR_MAX_HEALTH then
				return attrInfo[ 2 ]
			end 
		end
	end
	return 100
end

--通过原点和半径获取随机点坐标(b:为true时，为在地图内可在打码区，为false时，为玩家可到区域非打码区)
function GetRandPosByRadius(map_ptr, x, y, r, b)
	local beishu = 10000
	local pos_x, pos_y = x, y
	local dst_x, dst_y = x, y
	for i=1, 10 do		-- 10次没找到就把中心点给他
		local radius = randIntD(0, r*beishu)			-- 先随机半径
		local randX = randInt(-radius, radius)			-- 再随机x距离
		local randY = math.sqrt(math.pow(radius,2) - math.pow(randX,2))		-- 求出y距离
		pos_x = x + randX/beishu
		if randIntD(0, 100) <= 50 then
			pos_y = y + randY/beishu
		else
			pos_y = y - randY/beishu
		end
		if b and mapLib.IsInMap(map_ptr, pos_x, pos_y) ~= 0 or mapLib.IsCanRun(map_ptr, pos_x, pos_y) == 1 then
			dst_x, dst_y = pos_x, pos_y
			break
		end
	end
	return dst_x, dst_y
end
	
--计算经验加成(@player玩家对象，@creature怪物对象，@xp基本经验，@fcmtime防沉迷时间，@suitexp套装加成百分比，@percent可获得经验百分比)
function DOComputeExpBonusScript(player, creature, xp, fcmtime, percent)
	if(player == nil or GetUnitTypeID(player) ~= TYPEID_PLAYER)then
		return 0,0
	end
	if(xp == 0)then
		return 0,0
	end
	local cur_time = os.time()
	local result = xp
	local vip_exp = 0
	
	--[[
	-- 队伍加成
	if GetGroupAddExp(player) > 0 then
		result = result + xp*GetGroupAddExp(player)/100
	end
	]]

	--经验丹加成
	if unitLib.HasBuff(player, BUFF_ONEPOINTFIVE_JINGYAN) then		
		result =  result + xp*0.5		
	elseif unitLib.HasBuff(player, BUFF_TOW_JINGYAN) then
		result =  result + xp*1
	elseif unitLib.HasBuff(player, BUFF_THREE_JINGYAN) then
		result =  result + xp*2
	elseif unitLib.HasBuff(player, BUFF_FIVE_JINGYAN) then
		result =  result + xp*4		
	end
	
	--[[
	--防沉迷部分
	if(fcmtime ~= MAX_UINT32_NUM and fcmtime >=300) then
		result = 0
		vip_exp = 0
	elseif(fcmtime ~= MAX_UINT32_NUM and fcmtime >= 180) then
		result = result * 0.5
		vip_exp = vip_exp * 0.5
	end
	]]
	
	return result,vip_exp
end

-- 怪物初始化
function creatureInit(creature, entry)
	local obj = tb_creature_template[entry]
	creatureLib.SetActionRadius(creature, obj.actionradius)
	creatureLib.SetVisionRadius(creature, obj.visionradius)
	creatureLib.SetScriptAI(creature, obj.ainame)
	creatureLib.SetReactState(creature, obj.attack_type)
end

-- 采集任务物品
function DoHandlePickGameObject(player_ptr, gameObjectEntry)
	playerLib.SendToAppdDoSomething(player_ptr, SCENED_APPD_GAMEOBJECT, gameObjectEntry)
end

--应用服通知场景服消耗元宝或铜钱做些什么
function DoScenedComsumeMoney(player_ptr, money_type, use_type, use_param)
	local map_ptr = unitLib.GetMap(player_ptr)
	local mapid = unitLib.GetMapID(player_ptr)
	local playerInfo = UnitInfo:new{ptr = player_ptr}
	local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}

	if use_type == USE_GLOD_BUY_AUTO_RESPAWN then --原地复活
		-- 必须在世界地图
		if not IsWorldMap(mapid) then
			return
		end
		-- 必须是死了
		if playerInfo:IsAlive() then
			return
		end
		unitLib.Respawn(player_ptr, RESURRPCTION_HUANHUNDAN, 5)	
	elseif(use_type == USE_GOLD_BUY_PLAYER_FUBENLING)then		-- 购买副本令		
		local energy = playerInfo:GetPlayerCurEnergyCount()
		playerInfo:SetPlayerCurEnergyCount(energy + 10)
	end
end

-- 判断是否是没有buff NPC(不会有流血等buff)
function IsNoBuffNPC(entry)
	for k,v in pairs(no_buff_npc_entry_config) do
		if(entry == v)then
			return true
		end
	end
	return false
end

-- 获取点到点的距离
function GetTwoPointDistance(pos_x, pos_y, tar_x, tar_y)
	local dx = pos_x - tar_x
	local dy = pos_y - tar_y
	return math.sqrt(math.pow(dx,2)+math.pow(dy,2))
end

function UnitInfo:GetUseRespawnMapId()
	return self:GetUInt32(UNIT_FIELD_USE_RESPAWN_MAPID)
end

function UnitInfo:SetUseRespawnMapId(mapid)
	self:SetUInt32(UNIT_FIELD_USE_RESPAWN_MAPID, mapid)
end

-- 获得avatar
function UnitInfo:GetAvatar()
	return self:GetUInt32(UNIT_FIELD_EQUIPMENT_COAT)
end

-- 获得武器
function UnitInfo:GetWeapon()
	return self:GetUInt32(UNIT_FIELD_EQUIPMENT_MAIN_WEAPON)
end

-- 获得神兵
function UnitInfo:GetDivine()
	return self:GetUInt32(UNIT_FIELD_DIVINE_ID)
end


-- 设置avatar
function UnitInfo:SetAvatar(avatar)
	self:SetUInt32(UNIT_FIELD_EQUIPMENT_COAT, avatar)
end

-- 设置武器
function UnitInfo:SetWeapon(weapon)
	self:SetUInt32(UNIT_FIELD_EQUIPMENT_MAIN_WEAPON, weapon)
end

-- 设置神兵
function UnitInfo:SetDivine(divine)
	self:SetUInt32(UNIT_FIELD_DIVINE_ID, divine)
end


-- 获得最后一次参加世界BOSS的id
function UnitInfo:GetLastJoinID()
	return self:GetPlayerUInt32(PLAYER_INT_FIELD_WORLD_BOSS_JOIN_ID)
end

-- 设置最后一次参加世界BOSS的id
function UnitInfo:SetLastJoinID(id)
	if self:GetLastJoinID() == id then
		return
	end
	self:SetPlayerUInt32(PLAYER_INT_FIELD_WORLD_BOSS_JOIN_ID, id)
end

-- 获得最后一次参加世界BOSS的状态
function UnitInfo:GetLastState()
	return self:GetPlayerByte(PLAYER_INT_FIELD_WORLD_BOSS_JOIN_STATE, 0)
end

-- 设置最后一次参加世界BOSS的状态
function UnitInfo:SetLastState(state)
	if self:GetLastState() == state then
		return
	end
	self:SetPlayerByte(PLAYER_INT_FIELD_WORLD_BOSS_JOIN_STATE, 0, state)
end

-- 获得最后一次参加世界BOSS的分线
function UnitInfo:GetLastLine()
	return self:GetPlayerByte(PLAYER_INT_FIELD_WORLD_BOSS_JOIN_STATE, 1)
end

-- 设置最后一次参加世界BOSS的分线
function UnitInfo:SetLastLine(line)
	if self:GetLastLine() == line then
		return
	end
	self:SetPlayerByte(PLAYER_INT_FIELD_WORLD_BOSS_JOIN_STATE, 1, line)
end

-- 获得最后一次参加世界BOSS的自身死亡次数
function UnitInfo:GetLastDeath()
	return self:GetPlayerByte(PLAYER_INT_FIELD_WORLD_BOSS_JOIN_STATE, 2)
end

-- 增加最后一次参加世界BOSS的自身死亡次数
function UnitInfo:SetLastDeath(count)
	if self:GetLastDeath() == count then
		return
	end
	self:AddPlayerByte(PLAYER_INT_FIELD_WORLD_BOSS_JOIN_STATE, 2, count)
end



--- 设置仙府夺宝宝箱数量
function UnitInfo:SetXianfuBoxCount(val)
	self:SetByte(UINT_FIELD_XIANFU_INFO, 0, val)
end

--- 增加仙府夺宝宝箱数量
function UnitInfo:AddXianfuBoxCount(val)
	self:AddByte(UINT_FIELD_XIANFU_INFO, 0, val)
end

--- 减少仙府夺宝宝箱数量
function UnitInfo:SubXianfuBoxCount(val)
	self:SubByte(UINT_FIELD_XIANFU_INFO, 0, val)
end

--- 增加仙府夺宝死亡次数
function UnitInfo:AddXianfuDeathCount()
	self:AddByte(UINT_FIELD_XIANFU_INFO, 1, 1)
end

--- 获得仙府夺宝死亡次数
function UnitInfo:GetXianfuDeathCount()
	return self:GetByte(UINT_FIELD_XIANFU_INFO, 1)
end

-- /*仙府夺宝小地图信息*/	
function UnitInfo:call_kuafu_xianfu_minimap_info_in_custom(playerDict, creatureDict, gameobjectDict)
	local output = Protocols.pack_kuafu_xianfu_minimap_info()
	
	-- 玩家
	output:writeU16(#playerDict)
	for _, info in pairs(playerDict) do
		output:writeUTF(info[ 1 ])
		output:writeU16(info[ 2 ])
		output:writeU16(info[ 3 ])
	end
	
	-- 生物
	output:writeU16(#creatureDict)
	for _, info in pairs(creatureDict) do
		output:writeU16(info[ 1 ])
		output:writeU16(info[ 2 ])
		output:writeU16(info[ 3 ])
	end
	
	-- 采集物
	output:writeU16(#gameobjectDict)
	for _, info in pairs(gameobjectDict) do
		output:writeU16(info[ 1 ])
		output:writeU16(info[ 2 ])
		output:writeU16(info[ 3 ])
	end
	
	self:SendPacket(output)
	output:delete()
end

function UnitInfo:SetCharIllusion(val)
	self:SetUInt32(UINT_INT_FIELD_ILLUSION, val)
end

function UnitInfo:GetCharIllusion()
	return self:GetUInt32(UINT_INT_FIELD_ILLUSION)
end

function UnitInfo:SetCurrCastVampiric(val)
	self:SetDouble(UINT_FIELD_VAMPIRIC, val)
end


-------------------------------试炼塔------------------------------
function UnitInfo:getLastPassedSectionId()
	return self:GetPlayerUInt32(PLAYER_INT_FIELD_TRIAL_FINISHED_SECTIONID)
end

function UnitInfo:SetRiskMonsterCount(count)
	self:SetPlayerUInt16(PLAYER_INT_FIELD_TRIAL_PROCESS, 0, 0)
	self:SetPlayerUInt16(PLAYER_INT_FIELD_TRIAL_PROCESS, 1, count)
end

function UnitInfo:GetRiskMonsterCount()
	return self:GetPlayerUInt16(PLAYER_INT_FIELD_TRIAL_PROCESS, 1)
end

function UnitInfo:AddRiskMonsterKilledCount()
	if not self:isCanChallengeRiskBoss() then
		self:AddPlayerUInt16(PLAYER_INT_FIELD_TRIAL_PROCESS, 0, 1)
		
		--[[ 暂时不提示
		local showname = '小怪'
		local process	= self:GetPlayerUInt16(PLAYER_INT_FIELD_TRIAL_PROCESS, 0)
		local dest		= self:GetPlayerUInt16(PLAYER_INT_FIELD_TRIAL_PROCESS, 1)
		if process >= dest then
			showname = "[00FF00]"..showname
		end
		
		-- 提示客户端
		self:CallOptResult(OPERTE_TYPE_QUEST, QUEST_TYPE_PROCESS,{showname, process, dest})
		--]]
	end
end

function UnitInfo:isCanChallengeRiskBoss()
	local limit = self:GetPlayerUInt16(PLAYER_INT_FIELD_TRIAL_PROCESS, 1)
	return self:GetPlayerUInt16(PLAYER_INT_FIELD_TRIAL_PROCESS, 0) >= limit and limit > 0
end

function UnitInfo:passSection(sectionId)
	local passedSectionId = tb_risk_data[sectionId].relateId
	if GetAailableSectionid(self.ptr) == passedSectionId then
		self:SetPlayerUInt32(PLAYER_INT_FIELD_TRIAL_FINISHED_SECTIONID, passedSectionId)
		--发到应用服进行进入判断
		playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_PASS_WORLD_RISK, passedSectionId)
		self:SetRiskMonsterCount(0)
	end
end

-- 打断持续动作
function terminalContinuousAction(player_ptr)
	-- 野外boss开宝箱
	local map_ptr = unitLib.GetMap(player_ptr)
	local mapid = mapLib.GetMapID(map_ptr)
	
	local instanceInfo = Select_Instance_Script(mapid):new {ptr = map_ptr} 
	instanceInfo:OnPlayerAfterMove(player_ptr)
	
	-- 持续技能
	terminalSpell(player_ptr)
end


function ScenedGetRiskTeleportInfo(player_ptr)
	local player_data_ptr = playerLib.GetSession(player_ptr)
	local playerGuid = binLogLib.GetStr(player_data_ptr, BINLOG_STRING_FIELD_GUID)
	local passedSectionId = binLogLib.GetUInt32(player_data_ptr, PLAYER_INT_FIELD_TRIAL_FINISHED_SECTIONID)
	local mapid, x, y, generalId = onGetRiskTeleportInfo(playerGuid, passedSectionId)
	return mapid, x, y, generalId
end


function UnitInfo:teleportWorldRisk()
	local passedSectionId = self:getLastPassedSectionId()
	local mapid, x, y, generalId = onGetRiskTeleportInfo(self:GetPlayerGuid(), passedSectionId)
	
	playerLib.Teleport(self.ptr, mapid, x, y, 0, generalId)
end

function UnitInfo:GetOpenMenuFlag(id,subid)
	
	for i=0,self:GetPlayerUInt32(PLAYER_INT_FIELD_OPEN_MENU_INDEX) -1 do
		local temp_id = self:GetPlayerUInt32(PLAYER_INT_FIELD_OPEN_MENU_START + i * MAX_OPEN_MENU_ATTR_COUNT + OPEN_MENU_MAIN_ID)
		if temp_id == id then
			return self:GetPlayerBit(PLAYER_INT_FIELD_OPEN_MENU_START + i * MAX_OPEN_MENU_ATTR_COUNT + OPEN_MENU_SUB_FLAG,subid)
		end
	end
	return false
end


require 'scened.unit.unit_spell'
require 'scened.unit.scened_appd_dosomething'
