local QuestJoinFieldBoss = class("QuestJoinFieldBoss", AbstractQuest)

IQuestJoinFieldBoss = QuestJoinFieldBoss:new{}

function QuestJoinFieldBoss:ctor()
	
end

function QuestJoinFieldBoss:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestJoinFieldBoss:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestJoinFieldBoss:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestJoinFieldBoss