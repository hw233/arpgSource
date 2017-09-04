local QuestJoinFaction = class("QuestJoinFaction", AbstractQuest)

IQuestJoinFaction = QuestJoinFaction:new{}

function QuestJoinFaction:ctor()
	
end

function QuestJoinFaction:OnInit(playerInfo, start, offset)
	local fguid = playerInfo:GetFactionId()
	if fguid ~= "" then
		local quest_ptr = playerInfo:getQuestMgr().ptr
		self:OnUpdate(playerInfo, start, offset, {})
	end
end

-- 获得目标值
function QuestJoinFaction:GetTargetValue(targetInfo)
	return 1
end

-- 更新进度, 如果目标完成返回true
function QuestJoinFaction:OnUpdate(playerInfo, start, offset, params)
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local qtIndx = GetOneQuestTargetStartIndx(start, offset)
	
	binLogLib.SetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS, 1)
	binLogLib.SetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 0, 1)
	
	return true
end

return QuestJoinFaction