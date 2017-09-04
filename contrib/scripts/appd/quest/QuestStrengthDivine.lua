local QuestStrengthDivine = class("QuestStrengthDivine", AbstractQuest)

IQuestStrengthDivine = QuestStrengthDivine:new{}

function QuestStrengthDivine:ctor()
	
end

function QuestStrengthDivine:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestStrengthDivine:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestStrengthDivine:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestStrengthDivine