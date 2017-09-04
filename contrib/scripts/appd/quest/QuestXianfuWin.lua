local QuestXianfuWin = class("QuestXianfuWin", AbstractQuest)

IQuestXianfuWin = QuestXianfuWin:new{}

function QuestXianfuWin:ctor()
	
end

function QuestXianfuWin:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestXianfuWin:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestXianfuWin:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestXianfuWin