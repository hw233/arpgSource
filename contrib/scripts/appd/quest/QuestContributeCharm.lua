local QuestContributeCharm = class("QuestContributeCharm", AbstractQuest)

IQuestContributeCharm = QuestContributeCharm:new{}

function QuestContributeCharm:ctor()
	
end

function QuestContributeCharm:OnInit(playerInfo, start, offset)
	self:OnUpdate(playerInfo, start, offset, {playerInfo:GetFactionGiftPointCount()})
end

-- 获得目标值
function QuestContributeCharm:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestContributeCharm:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestContributeCharm