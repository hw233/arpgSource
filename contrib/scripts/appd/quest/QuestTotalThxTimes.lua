local QuestTotalThxTimes = class("QuestTotalThxTimes", AbstractQuest)

IQuestTotalThxTimes = QuestTotalThxTimes:new{}

function QuestTotalThxTimes:ctor()
	
end

function QuestTotalThxTimes:OnInit(playerInfo, start, offset)
	self:OnUpdate(playerInfo, start, offset, {playerInfo:GetFactionGiftBeenThankCount()})
end

-- 获得目标值
function QuestTotalThxTimes:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestTotalThxTimes:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestTotalThxTimes