local QuestKillMonsterCollect = class("QuestKillMonsterCollect", AbstractQuest)

IQuestKillMonsterCollect = QuestKillMonsterCollect:new{}

function QuestKillMonsterCollect:ctor()
	
end

function QuestKillMonsterCollect:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestKillMonsterCollect:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestKillMonsterCollect:OnUpdate(playerInfo, start, offset, params)	
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	-- 是否是对应的野怪
	local finishMode = params[ 1 ]
	local mode = tb_quest[questId].targets[offset+1][ 4 ]
	if mode ~= 0 and finishMode ~= mode then
		return false
	end
	-- 是否掉落物品
	local rate = tb_quest[questId].targets[offset+1][ 5 ]
	local ran = randInt(1, 10000)
	-- 没随机到
	if rate < ran then
		return false
	end
	-- 当怪物掉落了
	local entry = tb_quest[questId].targets[offset+1][ 2 ]
	local showname = nil
	if tb_item_template[entry] then
		showname = tb_item_template[entry].name
	end
	
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, {entry}, showname)
end

return QuestKillMonsterCollect