local QuestTalismanLevel = class("QuestTalismanLevel", AbstractQuest)

IQuestTalismanLevel = QuestTalismanLevel:new{}

function QuestTalismanLevel:ctor()
	
end

function QuestTalismanLevel:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	
	local id = tb_quest[questId].targets[offset+1][ 2 ]
	local level = playerInfo:getTalismanLevelIfNotReturnMaxLevelOfAllTalisman(id)
	
	self:OnUpdate(quest_ptr, start, offset, {id, level})
end

-- 获得目标值
function QuestTalismanLevel:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestTalismanLevel:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectValue(quest_ptr, start, offset, params)
end

return QuestTalismanLevel