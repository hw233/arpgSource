local QuestWashSuit = class("QuestWashSuit", AbstractQuest)

IQuestWashSuit = QuestWashSuit:new{}

function QuestWashSuit:ctor()
	
end

function QuestWashSuit:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestWashSuit:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestWashSuit:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestWashSuit