local QuestAchieve = class("QuestAchieve", AbstractQuest)

IQuestAchieve = QuestAchieve:new{}

function QuestAchieve:ctor()
	
end

function QuestAchieve:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local achievePoint = playerInfo:GetAchieveAll()
	self:OnUpdate(playerInfo, start, offset, {achievePoint})
end

-- 获得目标值
function QuestAchieve:GetTargetValue(targetInfo)
	return targetInfo[ 2 ]
end

-- 更新进度, 如果目标完成返回true
function QuestAchieve:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeValue(playerInfo, start, offset, params)
end

return QuestAchieve