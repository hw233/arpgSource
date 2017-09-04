local QuestWingsUpgradeTimes = class("QuestWingsUpgradeTimes", AbstractQuest)

IQuestWingsUpgradeTimes = QuestWingsUpgradeTimes:new{}

function QuestWingsUpgradeTimes:ctor()
	
end

function QuestWingsUpgradeTimes:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestWingsUpgradeTimes:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestWingsUpgradeTimes:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeTimes(playerInfo, start, offset, params)
end

return QuestWingsUpgradeTimes