local QuestFriendDonate = class("QuestFriendDonate", AbstractQuest)

IQuestFriendDonate = QuestFriendDonate:new{}

function QuestFriendDonate:ctor()
	
end

function QuestFriendDonate:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestFriendDonate:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestFriendDonate:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestFriendDonate