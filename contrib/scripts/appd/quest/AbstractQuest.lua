local AbstractQuest = class("AbstractQuest")

function AbstractQuest:ctor()
	
end

function AbstractQuest:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
-- 子类需要重写 默认return 1
function AbstractQuest:GetTargetValue(targetInfo)
	return 1
end

-- 更新进度, 如果目标完成返回true
-- 子类需要重写 默认return false
function AbstractQuest:OnUpdate(playerInfo, start, offset, params)
	return false
end

-- (对象, 次数)类的更新
function AbstractQuest:OnUpdateModeObjectTimes(playerInfo, start, offset, params, showname)
	local finishMode = params[ 1 ]
	local cnt = params[ 2 ] or 1
	
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local qtIndx = GetOneQuestTargetStartIndx(start, offset)
	local dest = binLogLib.GetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 1)
	local process = binLogLib.GetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS)
	
	local mode = tb_quest[questId].targets[offset+1][ 2 ]
	if mode ~= 0 and finishMode ~= mode then
		return false
	end
	
	process = math.min(process + cnt, dest)
	if showname then
		if process >= dest then
			showname = "[00FF00]"..showname
		end
		playerInfo:CallOptResult(OPERTE_TYPE_QUEST, QUEST_TYPE_PROCESS,{showname, process, dest})
	end
	binLogLib.SetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS, process)
	if process >= dest then
		binLogLib.SetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 0, 1)
		return true
	end
	
	return false
end

-- (次数)类的更新
function AbstractQuest:OnUpdateModeTimes(playerInfo, start, offset, params)
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	local cnt = params[ 1 ] or 1
	
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local qtIndx = GetOneQuestTargetStartIndx(start, offset)
	local dest = binLogLib.GetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 1)
	local process = binLogLib.GetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS)
	
	process = math.min(process + cnt, dest)
	binLogLib.SetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS, process)
	if process >= dest then
		binLogLib.SetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 0, 1)
		return true
	end
	
	return false
end

-- (数值)类的更新
function AbstractQuest:OnUpdateModeValue(playerInfo, start, offset, params)
	local value = params[ 1 ]
	
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local qtIndx = GetOneQuestTargetStartIndx(start, offset)
	local dest = binLogLib.GetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 1)
	
	local process = math.min(value, dest)
	binLogLib.SetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS, process)
	if process >= dest then
		binLogLib.SetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 0, 1)
		return true
	end
	return false
end

-- (对象, 数值)类的更新
function AbstractQuest:OnUpdateModeObjectValue(playerInfo, start, offset, params)
	local finishMode = params[ 1 ]
	local value = params[ 2 ]
	
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local qtIndx = GetOneQuestTargetStartIndx(start, offset)
	local dest = binLogLib.GetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 1)
	
	local mode = tb_quest[questId].targets[offset+1][ 2 ]
	if mode ~= 0 and finishMode ~= mode then
		return false
	end
	
	process = math.min(value, dest)
	binLogLib.SetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS, process)
	if process >= dest then
		binLogLib.SetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 0, 1)
		return true
	end
	
	return false
end

-- (对象, 对象)类的更新
function AbstractQuest:OnUpdateDoubleModeObject(playerInfo, start, offset, params)
	local finishMode  = params[ 1 ]
	local finishMode2 = params[ 2 ]
	
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local qtIndx = GetOneQuestTargetStartIndx(start, offset)
	local dest = binLogLib.GetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 1)
	local process = binLogLib.GetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS)
	
	local mode = tb_quest[questId].targets[offset+1][ 2 ]
	if mode ~= 0 and finishMode ~= mode then
		return false
	end
	
	local mode2 = tb_quest[questId].targets[offset+1][ 3 ]
	if mode2 ~= 0 and finishMode2 ~= mode2 then
		return false
	end
	
	binLogLib.SetUInt32(quest_ptr, qtIndx + QUEST_TARGET_INFO_PROCESS, 1)
	binLogLib.SetUInt16(quest_ptr, qtIndx + QUEST_TARGET_INFO_SHORT0, 0, 1)
	return true
end

-- 获得一个目标binlog的初始位置
function GetOneQuestTargetStartIndx(start, offset)
	return start + QUEST_INFO_STEP_START + offset * MAX_QUEST_TARGET_INFO_COUNT
end

return AbstractQuest