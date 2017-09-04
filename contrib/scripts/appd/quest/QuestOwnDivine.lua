local QuestOwnDivine = class("QuestOwnDivine", AbstractQuest)

IQuestOwnDivine = QuestOwnDivine:new{}

function QuestOwnDivine:ctor()
	
end

function QuestOwnDivine:OnInit(playerInfo, start, offset)
	local quest_ptr = playerInfo:getQuestMgr().ptr
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local divineId = tb_quest[questId].targets[offset+1][ 2 ]
	
	local spellMgr = playerInfo:getSpellMgr()
	if spellMgr:hasDivineQuest(divineId) then
		self:OnUpdate(playerInfo, start, offset, {divineId})
	end
end

-- 获得目标值
function QuestOwnDivine:GetTargetValue(targetInfo)
	return 1
end

-- 更新进度, 如果目标完成返回true
function QuestOwnDivine:OnUpdate(playerInfo, start, offset, params)
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestOwnDivine