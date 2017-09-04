local QuestDailyTask = class("QuestDailyTask", AbstractQuest)

IQuestDailyTask = QuestDailyTask:new{}

function QuestDailyTask:ctor()
	
end

function QuestDailyTask:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestDailyTask:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestDailyTask:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestDailyTask