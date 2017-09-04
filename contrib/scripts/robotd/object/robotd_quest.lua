local RobotdQuest = class('RobotdQuest', BinLogObject)

--构造函数
function RobotdQuest:ctor()
	
end

-- 获得主线任务的信息
--[[
{
	questId = int,
	state = int,
	steps = {
		{
			stepState = int
			targetNum = int
			process   = int
		}
	}
}
--]]
--[[
QUEST_TYPE_MAIN = 0	-- 主线任务
QUEST_TYPE_EXTENSIONS = 1	-- 支线任务
QUEST_TYPE_ACTIVITY = 2	-- 活动任务
QUEST_TYPE_QIYU = 3	-- 奇遇任务
QUEST_TYPE_DAILY = 4	-- 每日任务
]]
local questPriority = {
	--QUEST_TYPE_DAILY,
	QUEST_TYPE_MAIN,
	--QUEST_TYPE_EXTENSIONS,
	--QUEST_TYPE_ACTIVITY,
	--QUEST_TYPE_QIYU
}

function RobotdQuest:GetQuestInfo()
	for _, type in ipairs(questPriority) do
		local indx = self:GetTypedQuestIndx(type)
		if indx then
			local questInfo = self:GetQuestInfoByIndx(indx)
			if self:IsAvailableQuest(questInfo) then
				return questInfo
			end
		end
	end
	
	return
end

function RobotdQuest:IsAvailableQuest(questInfo)
	if questInfo.state == QUEST_STATUS_COMPLETE then
		return true
	end
	
	-- 如果是提升等级/战力/荣誉点的就不做
	local config = tb_quest[questInfo.questId]
	local targets = config.targets
	-- 找任务目标
	for i, target in ipairs(targets) do
		local targetType = target[ 1 ]
		local stepInfo	 = questInfo.steps[i]
		outFmtDebug('targetType = %d',targetType)
		if not Quest_Function[targetType] then
			return false
		end
	end
	
	return true
end

-- 获得对应类型的任务binlogindx
function RobotdQuest:GetTypedQuestIndx(type)

	local intstart = QUEST_FIELD_QUEST_START
	for i = 1, MAX_QUEST_COUNT do
		local questId = self:GetUInt16(intstart + QUEST_INFO_ID, 0)
		if questId > 0 then
			if tb_quest[questId].type == type then
				return intstart
			end
		end
		intstart = intstart + MAX_QUEST_INFO_COUNT
	end

	return
end

-- 获得任务的下标
function RobotdQuest:GetQuestIndxById(matchQuestId)

	local intstart = QUEST_FIELD_QUEST_START
	for i = 1, MAX_QUEST_COUNT do
		local questId = self:GetUInt16(intstart + QUEST_INFO_ID, 0)
		if questId > 0 and questId == matchQuestId then
			return i-1
		end
		intstart = intstart + MAX_QUEST_INFO_COUNT
	end

	return
end

function RobotdQuest:CheckQuestIsFinished(matchQuestId)
	local intstart = QUEST_FIELD_QUEST_START
	for i = 1, MAX_QUEST_COUNT do
		local questId = self:GetUInt16(intstart + QUEST_INFO_ID, 0)
		if questId > 0 and questId == matchQuestId then
			return self:GetUInt16(intstart + QUEST_INFO_ID, 1) == QUEST_STATUS_COMPLETE
		end
		intstart = intstart + MAX_QUEST_INFO_COUNT
	end

	return false
end

-- 获得binlogindx的任务信息
function RobotdQuest:GetQuestInfoByIndx(intstart)
	local questId = self:GetUInt16(intstart + QUEST_INFO_ID, 0)
	if questId == 0 then
		return
	end
	
	local info = {}
	info.questId = questId
	info.state = self:GetUInt16(intstart + QUEST_INFO_ID, 1)
	info.steps = {}
	for i = QUEST_INFO_STEP_START, QUEST_INFO_STEP_END-1, MAX_QUEST_TARGET_INFO_COUNT do
		local targetIntstart = intstart + i
		local stepInfo = {}
		stepInfo.stepState = self:GetUInt16(targetIntstart + QUEST_TARGET_INFO_SHORT0, 0)
		stepInfo.targetNum = self:GetUInt16(targetIntstart + QUEST_TARGET_INFO_SHORT0, 1)
		stepInfo.process   = self:GetUInt32(targetIntstart + QUEST_TARGET_INFO_PROCESS)
		table.insert(info.steps, stepInfo)
	end
	
	return info
end

return RobotdQuest