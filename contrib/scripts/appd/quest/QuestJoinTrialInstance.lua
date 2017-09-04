local QuestJoinTrialInstance = class("QuestJoinTrialInstance", AbstractQuest)

IQuestJoinTrialInstance = QuestJoinTrialInstance:new{}

function QuestJoinTrialInstance:ctor()
	
end

function QuestJoinTrialInstance:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestJoinTrialInstance:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestJoinTrialInstance:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestJoinTrialInstance