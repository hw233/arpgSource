local QuestUseItem = class("QuestUseItem", AbstractQuest)

IQuestUseItem = QuestUseItem:new{}

function QuestUseItem:ctor()
	
end

function QuestUseItem:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestUseItem:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestUseItem:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestUseItem