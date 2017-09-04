local QuestSingleThxTimes = class("QuestSingleThxTimes", AbstractQuest)

IQuestSingleThxTimes = QuestSingleThxTimes:new{}

function QuestSingleThxTimes:ctor()
	
end

function QuestSingleThxTimes:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestSingleThxTimes:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestSingleThxTimes:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestSingleThxTimes