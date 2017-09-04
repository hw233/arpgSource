local QuestJoinFactionActivity = class("QuestJoinFactionActivity", AbstractQuest)

IQuestJoinFactionActivity = QuestJoinFactionActivity:new{}

function QuestJoinFactionActivity:ctor()
	
end

function QuestJoinFactionActivity:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestJoinFactionActivity:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestJoinFactionActivity:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestJoinFactionActivity