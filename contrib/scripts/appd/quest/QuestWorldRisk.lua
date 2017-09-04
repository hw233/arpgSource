local QuestWorldRisk = class("QuestWorldRisk", AbstractQuest)

IQuestWorldRisk = QuestWorldRisk:new{}

function QuestWorldRisk:ctor()
	
end

function QuestWorldRisk:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local sectionId = tb_quest[questId].targets[offset+1][ 2 ]
	local passedSectionId = playerInfo:GetUInt32(PLAYER_INT_FIELD_TRIAL_FINISHED_SECTIONID)
	
	if passedSectionId >= sectionId then
		self:OnUpdate(playerInfo, start, offset, {sectionId})
	end
end

-- 获得目标值
function QuestWorldRisk:GetTargetValue(targetInfo)
	return 1
end

-- 更新进度, 如果目标完成返回true
function QuestWorldRisk:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestWorldRisk