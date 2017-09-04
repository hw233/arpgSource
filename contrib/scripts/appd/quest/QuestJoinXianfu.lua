local QuestJoinXianfu = class("QuestJoinXianfu", AbstractQuest)

IQuestJoinXianfu = QuestJoinXianfu:new{}

function QuestJoinXianfu:ctor()
	
end

function QuestJoinXianfu:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestJoinXianfu:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestJoinXianfu:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestJoinXianfu