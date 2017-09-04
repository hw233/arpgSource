local QuestTurnItemIn = class("QuestTurnItemIn", AbstractQuest)

IQuestTurnItemIn = QuestTurnItemIn:new{}

function QuestTurnItemIn:ctor()
	
end

function QuestTurnItemIn:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestTurnItemIn:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestTurnItemIn:OnUpdate(playerInfo, start, offset, params)
	
end

return QuestTurnItemIn