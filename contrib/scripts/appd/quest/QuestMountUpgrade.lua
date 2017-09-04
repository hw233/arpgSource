local QuestMountUpgrade = class("QuestMountUpgrade", AbstractQuest)

IQuestMountUpgrade = QuestMountUpgrade:new{}

function QuestMountUpgrade:ctor()
	
end

function QuestMountUpgrade:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local spellMgr = playerInfo:getSpellMgr()
	local level = spellMgr:getMountLevel()
	-- 获得坐骑阶数
	self:OnUpdate(playerInfo, start, offset, {level})
end

-- 获得目标值
function QuestMountUpgrade:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestMountUpgrade:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeValue(playerInfo, start, offset, params)
end

return QuestMountUpgrade