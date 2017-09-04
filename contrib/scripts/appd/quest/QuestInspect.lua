local QuestInspect = class("QuestInspect", AbstractQuest)

IQuestInspect = QuestInspect:new{}

function QuestInspect:ctor()
	
end

function QuestInspect:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestInspect:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestInspect:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestInspect