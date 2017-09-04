local QuestWingsStrengthTimes = class("QuestWingsStrengthTimes", AbstractQuest)

IQuestWingsStrengthTimes = QuestWingsStrengthTimes:new{}

function QuestWingsStrengthTimes:ctor()
	
end

function QuestWingsStrengthTimes:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestWingsStrengthTimes:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestWingsStrengthTimes:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestWingsStrengthTimes