local QuestQualifyWin = class("QuestQualifyWin", AbstractQuest)

IQuestQualifyWin = QuestQualifyWin:new{}

function QuestQualifyWin:ctor()
	
end

function QuestQualifyWin:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestQualifyWin:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestQualifyWin:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestQualifyWin