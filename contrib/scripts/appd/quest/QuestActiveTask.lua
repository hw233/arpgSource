local QuestActiveTask = class("QuestActiveTask", AbstractQuest)

IQuestActiveTask = QuestActiveTask:new{}

function QuestActiveTask:ctor()
	
end

function QuestActiveTask:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestActiveTask:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestActiveTask:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestActiveTask