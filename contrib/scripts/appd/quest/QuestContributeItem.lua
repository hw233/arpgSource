local QuestContributeItem = class("QuestContributeItem", AbstractQuest)

IQuestContributeItem = QuestContributeItem:new{}

function QuestContributeItem:ctor()
	
end

function QuestContributeItem:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestContributeItem:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestContributeItem:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestContributeItem