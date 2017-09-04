local QuestJoinDoujiantai = class("QuestJoinDoujiantai", AbstractQuest)

IQuestJoinDoujiantai = QuestJoinDoujiantai:new{}

function QuestJoinDoujiantai:ctor()
	
end

function QuestJoinDoujiantai:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestJoinDoujiantai:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestJoinDoujiantai:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestJoinDoujiantai