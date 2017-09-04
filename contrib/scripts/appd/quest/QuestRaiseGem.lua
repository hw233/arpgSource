local QuestRaiseGem = class("QuestRaiseGem", AbstractQuest)

IQuestRaiseGem = QuestRaiseGem:new{}

function QuestRaiseGem:ctor()
	
end

function QuestRaiseGem:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestRaiseGem:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestRaiseGem:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestRaiseGem