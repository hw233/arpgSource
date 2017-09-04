local QuestRefineSuit = class("QuestRefineSuit", AbstractQuest)

IQuestRefineSuit = QuestRefineSuit:new{}

function QuestRefineSuit:ctor()
	
end

function QuestRefineSuit:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestRefineSuit:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestRefineSuit:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestRefineSuit