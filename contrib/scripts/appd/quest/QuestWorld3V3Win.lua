local QuestWorld3v3Win = class("QuestWorld3v3Win", AbstractQuest)

IQuestWorld3v3Win = QuestWorld3v3Win:new{}

function QuestWorld3v3Win:ctor()
	
end

function QuestWorld3v3Win:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestWorld3v3Win:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestWorld3v3Win:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestWorld3v3Win