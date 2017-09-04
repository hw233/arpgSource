local QuestStrengthSuit = class("QuestStrengthSuit", AbstractQuest)

IQuestStrengthSuit = QuestStrengthSuit:new{}

function QuestStrengthSuit:ctor()
	
end

function QuestStrengthSuit:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestStrengthSuit:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestStrengthSuit:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestStrengthSuit