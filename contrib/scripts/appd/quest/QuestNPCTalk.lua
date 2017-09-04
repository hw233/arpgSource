local QuestNPCTalk = class("QuestNPCTalk", AbstractQuest)

IQuestNPCTalk = QuestNPCTalk:new{}

function QuestNPCTalk:ctor()
	
end

function QuestNPCTalk:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestNPCTalk:GetTargetValue(targetInfo)
	return 1
end

-- 更新进度, 如果目标完成返回true
-- 对话任务只能完成选定的任务, 不能全部都完成
function QuestNPCTalk:OnUpdate(playerInfo, start, offset, params)
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	
	local paramQuestId = params[ 2 ]
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	if questId ~= paramQuestId then
		return false
	end
	params[ 2 ] = 1
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params)
end

return QuestNPCTalk