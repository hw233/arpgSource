local QuestRaiseMountSkill = class("QuestRaiseMountSkill", AbstractQuest)

IQuestRaiseMountSkill = QuestRaiseMountSkill:new{}

function QuestRaiseMountSkill:ctor()
	
end

function QuestRaiseMountSkill:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestRaiseMountSkill:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestRaiseMountSkill:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestRaiseMountSkill