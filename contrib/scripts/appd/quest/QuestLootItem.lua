local QuestLootItem = class("QuestLootItem", AbstractQuest)

IQuestLootItem = QuestLootItem:new{}

function QuestLootItem:ctor()
	
end

function QuestLootItem:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestLootItem:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestLootItem:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestLootItem