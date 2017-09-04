local QuestDoujiantaiWin = class("QuestDoujiantaiWin", AbstractQuest)

IQuestDoujiantaiWin = QuestDoujiantaiWin:new{}

function QuestDoujiantaiWin:ctor()
	
end

function QuestDoujiantaiWin:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestDoujiantaiWin:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestDoujiantaiWin:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestDoujiantaiWin