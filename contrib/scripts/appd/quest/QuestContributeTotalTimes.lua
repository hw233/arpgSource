local QuestContributeTotalTimes = class("QuestContributeTotalTimes", AbstractQuest)

IQuestContributeTotalTimes = QuestContributeTotalTimes:new{}

function QuestContributeTotalTimes:ctor()
	
end

function QuestContributeTotalTimes:OnInit(playerInfo, start, offset)
	self:OnUpdate(playerInfo, start, offset, {playerInfo:GetFactionGiftSendCount()})
end

-- 获得目标值
function QuestContributeTotalTimes:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestContributeTotalTimes:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestContributeTotalTimes