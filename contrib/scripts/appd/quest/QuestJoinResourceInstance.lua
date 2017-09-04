local QuestJoinResourceInstance = class("QuestJoinResourceInstance", AbstractQuest)

IQuestJoinResourceInstance = QuestJoinResourceInstance:new{}

function QuestJoinResourceInstance:ctor()
	
end

function QuestJoinResourceInstance:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestJoinResourceInstance:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestJoinResourceInstance:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestJoinResourceInstance