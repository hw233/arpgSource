--业务逻辑管理器

local AppLogicalMgr = class("AppLogicalMgr", BinLogObject)

function AppLogicalMgr:ctor()
	
end

-- ============================================这里是装备部位操作部分========================================================== --
-- 设置强化等级
function  AppLogicalMgr:setStrengthLevel (part, level)
	local index = self:getStrengthLevelIndex(part)
	outFmtInfo("player %s set strengthLevel, index = %d, level = %d", self:getPlayerGuid(), index, level)
	self:SetUInt32(index, level)
end

-- 增加强化等级
function  AppLogicalMgr:addStrengthLevel (part, level)
	local prevLevel = self:getStrengthLevel(part)
	self:setStrengthLevel(part, prevLevel + level)
end

-- 获得强化等级
function AppLogicalMgr:getStrengthLevel (part)
	local index = self:getStrengthLevelIndex(part)
	return self:GetUInt32(index)
end


-- 设置宝石等级
function AppLogicalMgr:setGemLevel(part, gemIndex, level)
	local index = self:getGemLevelIndex(part, gemIndex)
	outFmtInfo("player %s set setGemLevel, index = %d, level = %d", self:getPlayerGuid(), index, level)
	self:SetUInt32(index, level)
end

-- 获得宝石等级
function AppLogicalMgr:getGemLevel(part, gemIndex)
	local index = self:getGemLevelIndex(part, gemIndex)
	return self:GetUInt32(index)
end

-- 设置宝石经验
function AppLogicalMgr:setGemExp(part, gemIndex, exp)
	local index = self:getGemExpIndex(part, gemIndex)
	outFmtInfo("player %s set setGemExp, index = %d, exp = %d", self:getPlayerGuid(), index, exp)
	self:SetUInt32(index, exp)
end

-- 获得宝石经验
function AppLogicalMgr:getGemExp(part, gemIndex)
	local index = self:getGemExpIndex(part, gemIndex)
	return self:GetUInt32(index)
end

-- 获得强化等级的实际binlogIndex
function AppLogicalMgr:getStrengthLevelIndex(part)
	return APPD_LOGICAL_INFO_INT_FIELD_STRENGTH_START + EQUIP_PART_OPT_TYPE_STRENGTH_LEVEL_OFFSET + part * EQUIP_PART_OPT_TYPE_ONE_PART_INFO_LENGTH
end

-- 获得宝石等级的实际binlogIndex
function AppLogicalMgr:getGemLevelIndex(part, gemIndex)
	return self:getGemInfoIndexByOffset(part, gemIndex, EQUIP_PART_OPT_TYPE_GEM_LEVEL_OFFSET)
end

-- 获得宝石经验的实际binlogIndex
function AppLogicalMgr:getGemExpIndex(part, gemIndex)
	return self:getGemInfoIndexByOffset(part, gemIndex, EQUIP_PART_OPT_TYPE_GEM_EXP_OFFSET)
end

-- 获得宝石信息的实际binlogIndex
function AppLogicalMgr:getGemInfoIndexByOffset(part, gemIndex, gemInfoDetailOffset)
	-- 先计算部位中的偏移量
	local onePartOffset = EQUIP_PART_OPT_TYPE_GEM_INFO_OFFSET + gemIndex * EQUIP_PART_OPT_TYPE_ONE_GEM_INFO_LENGTH + gemInfoDetailOffset
	-- 在计算实际的偏移量
	local index = onePartOffset + APPD_LOGICAL_INFO_INT_FIELD_STRENGTH_START + part * EQUIP_PART_OPT_TYPE_ONE_PART_INFO_LENGTH

	return index
end

-- ============================================这里是装备部位操作部分 结束========================================================== --

-- 获得玩家guid
function AppLogicalMgr:getPlayerGuid()
	--物品管理器guid转玩家guid
	if not self.owner_guid then
		self.owner_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypePlayer)
	end
	return self.owner_guid
end

return AppLogicalMgr