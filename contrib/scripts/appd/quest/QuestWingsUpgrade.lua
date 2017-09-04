local QuestWingsUpgrade = class("QuestWingsUpgrade", AbstractQuest)

IQuestWingsUpgrade = QuestWingsUpgrade:new{}

function QuestWingsUpgrade:ctor()
	
end

function QuestWingsUpgrade:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local level = playerInfo:GetWingsUpgradeLevel()
	-- 获得翅膀阶数
	self:OnUpdate(playerInfo, start, offset, {level})
end

-- 获得目标值
function QuestWingsUpgrade:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestWingsUpgrade:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeValue(playerInfo, start, offset, params)
end

return QuestWingsUpgrade