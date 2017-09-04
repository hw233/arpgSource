local QuestJoinWorldBoss = class("QuestJoinWorldBoss", AbstractQuest)

IQuestJoinWorldBoss = QuestJoinWorldBoss:new{}

function QuestJoinWorldBoss:ctor()
	
end

function QuestJoinWorldBoss:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestJoinWorldBoss:GetTargetValue(targetInfo)
	return 1
end

-- 更新进度, 如果目标完成返回true
function QuestJoinWorldBoss:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestJoinWorldBoss