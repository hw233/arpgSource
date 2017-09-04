--[[
	当玩家进入场景是 playerbase对象同步技能等级和重要技能cd分别到player对象的临时信息m_spell_level和m_importance_spell_cd中
	玩家增加技能的时候, 需要修改playerbase对象技能信息和player对象的临时m_spell_level信息
	玩家使用技能时候, 通过字典表中查询need_save属性来确定是放到m_spell_cd(不需要存)还是m_importance_spell_cd(要存)中
	玩家离开场景时候, 把m_importance_spell_cd信息同步到playerbase对象中
]]

-- 技能类型is_initiative
SPELL_INITIATIVE = {
	[SPELL_INITIATIVE_DAMAGE]  = 1, -- 主动（伤害）
	[SPELL_INITIATIVE_PROTECT] = 2, -- 主动（防护）
	[SPELL_INITIATIVE_CONTROL] = 3, -- 主动（控制）
	[SPELL_INITIATIVE_CURE]    = 4, -- 主动（回复）
	[SPELL_INITIATIVE_BUFF]    = 5, -- 主动（增益）
}

SPELL_PASSIVE = {
	[SPELL_PASSIVE_DAMAGE] = 6, -- 被动（伤害）
	[SPELL_PASSIVE_PROTECT] = 7, -- 被动（防护）
	[SPELL_PASSIVE_CONTROL] = 8, -- 被动（控制）
	[SPELL_PASSIVE_CURE] = 9, -- 被动（回复）
	[SPELL_PASSIVE_BUFF] = 10, -- 被动（增益）
}

--添加技能
function UnitInfo:AddSpell(need_add_spell)
	--[[
	-- 当前3种技能的空位
	local emptySupport		= self:findSpellEmptyIndex(PLAYER_INT_FIELD_SUPPORT_SPELL_HAS_START, MAX_SPELL_BASE_COUNT, PLAYER_OWN_SUPPORT_SPELL_MAX_COUNT)
	local emptyInitative	= self:findSpellEmptyIndex(PLAYER_INT_FIELD_INITIATIVE_SPELL_START, MAX_SLOT_ATTR_COUNT, PLAYER_OWN_INITIATIVE_SPELL_MAX_COUNT)
	local emptyPassive		= self:findSpellEmptyIndex(PLAYER_INT_FIELD_PASSIVE_SPELL, MAX_SPELL_BASE_COUNT, PLAYER_OWN_PASSIVE_SPELL_MAX_COUNT)
	
	-- 当前3中技能空位对应的binlog偏移量
	local supportOffset		= emptySupport * MAX_SPELL_BASE_COUNT
	local initativeOffset	= emptyInitative * MAX_SLOT_ATTR_COUNT
	local passiveOffset		= emptyPassive * MAX_SPELL_BASE_COUNT
	
	--添加技能
	for _, spellInfo in ipairs(need_add_spell) do
		local spellId = spellInfo[ 1 ]
		local spellLv = spellInfo[ 2 ]
		
		local config = tb_skill_base[spellId];
		if config == nil then
			outFmtError("in function AddSpell, designer has record invalid data, spellId = %d is not exist", spellId)
		else
			local skillType = config.is_initiative
			local index
			
			if skillType == SPELL_SUPPORT then
				-- 辅助
				if emptySupport >= PLAYER_OWN_SUPPORT_SPELL_MAX_COUNT then
					outFmtDebug("in function AddSpell, designer has record invalid data, support spell is full")
				else
					index = supportOffset
					emptySupport = emptySupport + 1
					supportOffset = supportOffset + MAX_SPELL_BASE_COUNT
					outInfoDebug("add !support! spell, indexStart = %d, spellId = %d, spellLv = %d", index, spellId, spellLv)
				end
			elseif SPELL_INITIATIVE[skillType] ~= nil then 
				-- 主动
				if emptyInitative >= PLAYER_OWN_INITIATIVE_SPELL_MAX_COUNT then
					outFmtDebug("in function AddSpell, designer has record invalid data, initative spell is full")
				else
					index = initativeOffset
					emptyInitative = emptyInitative + 1
					initativeOffset = initativeOffset + MAX_SLOT_ATTR_COUNT
					outInfoDebug("add @initative@ spell, indexStart = %d, spellId = %d, spellLv = %d", index, spellId, spellLv)
				end
			else
				-- 被动
				if emptyPassive >= PLAYER_OWN_PASSIVE_SPELL_MAX_COUNT then
					outFmtDebug("in function AddSpell, designer has record invalid data, passive spell is full")
				else
					index = passiveOffset
					emptyPassive = emptyPassive + 1
					passiveOffset = passiveOffset + MAX_SPELL_BASE_COUNT
					outInfoDebug("add #passive# spell, indexStart = %d, spellId = %d, spellLv = %d", index, spellId, spellLv)
				end
			end
			
			if index ~= nil then
				self:AddSpellToPlayer(index, spellId, spellLv)
			end
		end
	end
	]]
end

--加到玩家身上
function UnitInfo:AddSpellToPlayer(index, spellId, spellLv, spellCd)
	self:SetPlayerUInt32(index + SPELL_ID, spellId)
	self:SetPlayerUInt32(index + SPELL_LV, spellLv)
	
	-- 设置技能等级
	playerLib.SetSpellLevel(self.ptr, spellId, spellLv)
	
	if spellCd then
		self:FinalSetSpellCD(spellId, spellCd)
	end
end

-- -1表示没找到
function UnitInfo:findSpellEmptyIndex(start, increase, maxCount)
	-- 找空位
	for i = 0, maxCount-1 do
		if self:GetPlayerUInt32(start + SPELL_ID) == 0 then
			return i
		end
		start = start + increase
	end
	
	return -1
end

function UnitInfo:PrintSpellSlotInfo()
	for i = PLAYER_SCENED_INT_FIELD_SLOT_SPELL_0, PLAYER_SCENED_INT_FIELD_SLOT_SPELL_END-1, MAX_SLOT_ATTR_COUNT do
		local spell_id = self:GetPlayerUInt32(i + SLOT_SPELL_ID)
		if(spell_id ~= 0)then
			outDebug('spell id = '..spell_id)
			outDebug('spell spell_lv = '..self:GetPlayerUInt32(i + SLOT_SPELL_LV))	
			outDebug('spell cd = '..self:GetPlayerUInt32(i + SLOT_SPELL_CD))					
		end
	end
end

--获得技能等级
function UnitInfo:GetSpellLevel(spellId)
	local level = playerLib.GetSpellLevel(self.ptr, spellId)
	if level == 0 then
		level = 1
	end
	return level
end



--判断释放技能的消耗是否够 返回true则够消耗
function UnitInfo:IsEnoughConsumption(spell_id)
	--FIXME
	if true then
		return true
	end

	spell_id = self:GetNextSpellID(spell_id)
	local config = tb_skill_base[spell_id].nuqi_change
	if config < 0 then
		if self:GetAnger() < math.abs(config) then
			return false
		end
	end
	return true
end

--处理释放技能消耗
function UnitInfo:SpellConsumption(spell_id)
	--FIXME
	if true then
		return true
	end

	spell_id = self:GetNextSpellID(spell_id)
	self:AddAnger(spell_id)
	return true
end

--设置技能cd
function UnitInfo:SetSpellCD(spell_id, nowtime)
	local config = tb_skill_base[spell_id]
	if config ~= nil then
		local levelIndex = self:GetSpellLvIndex(spell_id)
		local upConfig = tb_skill_uplevel[levelIndex]
		local category_cd = config.groupCD
		local single_cd = config.singleCD - upConfig.mcd
		
		if self:HasSpell(spell_id) then
			-- 设置cd即可
			self:FinalSetSpellCD(spell_id, nowtime + single_cd)
		end
		
		-- 设置释放技能结束时间
		self:SetCurSpellTime(nowtime + category_cd)
	end
end

-- 调用C++函数设置内存cd及其是否是重要CD
function UnitInfo:FinalSetSpellCD(spellId, cd)
	-- 需要判断是否是binlog存CD技能
	if self:isNeedSaveSpellCd(spellId) then
		playerLib.SetImportanceSpellCD(self.ptr, spellId, cd)
	else
		playerLib.SetSpellCD(self.ptr, spellId, cd)
	end
end

-- 是否需要存binlog
function UnitInfo:isNeedSaveSpellCd(spellId)
	return tb_skill_base[spellId].need_save == 1
end

--判断技能是否处于CD，返回为true则技能冷却中
-- 包括重要技能和一般技能的CD
function UnitInfo:IsSpellCD(spell_id, nowtime)
	--print("IsSpellCD, spellID:"..spell_id)
	local futureTimestamp = playerLib.GetSpellCD(self.ptr, spell_id)
	return nowtime < futureTimestamp;
end

function UnitInfo:GetSpellCD(spellId)
	local futureTimestamp = playerLib.GetSpellCD(self.ptr, spellId)
	local now = os.time()
	if now >= futureTimestamp then
		return 0
	end
	
	return futureTimestamp - now
end

-- 判断是否有装备某个技能
function UnitInfo:HasSpell(spellId)
	
	for i = PLAYER_INT_FIELD_SPELL_START, PLAYER_INT_FIELD_SPELL_END-1 do
		if self:GetPlayerUInt16(i, SHORT_SLOT_SPELL_ID) == spellId then
			return true
		end
	end
	
	return false
end

--触发被动技能
function UnitInfo:triggerPassiveSpell(spell_id,cd)
	local nowtime = os.time()
	if passive_spell_cd[spell_id] then
		local ps_table = passive_spell_cd[spell_id]
		for i = 1,#ps_table,2 do
			if ps_table[i] == self:GetGuid() then
				if nowtime >= ps_table[2] then
					self:SetUnitPassiveSpellCD(spell_id,cd)
					return true
				end
			end
		end
	else
		passive_spell_cd[spell_id] = {}
		self:SetUnitPassiveSpellCD(spell_id,cd)
		return true
	end
	return false
end

--设置被动技能cd
function UnitInfo:SetUnitPassiveSpellCD(spell_id,cd)
	local new_cd = os.time() + cd
	local ps_table = passive_spell_cd[spell_id]
	for i = 1,#ps_table,2 do
		if ps_table[i] == self:GetGuid() then
			ps_table[i+1] = new_cd
			return
		end
	end
	table.insert(ps_table,self:GetGuid())
	table.insert(ps_table,new_cd)
end

