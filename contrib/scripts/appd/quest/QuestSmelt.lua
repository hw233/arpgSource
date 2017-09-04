local QuestSmelt = class("QuestSmelt", AbstractQuest)

IQuestSmelt = QuestSmelt:new{}

function QuestSmelt:ctor()
	
end

function QuestSmelt:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestSmelt:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestSmelt:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestSmelt