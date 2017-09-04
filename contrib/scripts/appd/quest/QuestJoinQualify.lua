local QuestJoinQualify = class("QuestJoinQualify", AbstractQuest)

IQuestJoinQualify = QuestJoinQualify:new{}

function QuestJoinQualify:ctor()
	
end

function QuestJoinQualify:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestJoinQualify:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestJoinQualify:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestJoinQualify