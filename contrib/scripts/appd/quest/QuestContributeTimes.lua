local QuestContributeTimes = class("QuestContributeTimes", AbstractQuest)

IQuestContributeTimes = QuestContributeTimes:new{}

function QuestContributeTimes:ctor()
	
end

function QuestContributeTimes:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestContributeTimes:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestContributeTimes:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestContributeTimes