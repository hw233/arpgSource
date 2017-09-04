local QuestKillMonster = class("QuestKillMonster", AbstractQuest)

IQuestKillMonster = QuestKillMonster:new{}

function QuestKillMonster:ctor()
	
end

function QuestKillMonster:OnInit(playerInfo, start, offset)
	
end

-- 获得目标值
function QuestKillMonster:GetTargetValue(targetInfo)
	return targetInfo[ 3 ]
end

-- 更新进度, 如果目标完成返回true
function QuestKillMonster:OnUpdate(playerInfo, start, offset, params)
	local showname = ''
	local questMgr = playerInfo:getQuestMgr()
	local quest_ptr = questMgr.ptr
	local questId = binLogLib.GetUInt16(quest_ptr, start + QUEST_INFO_ID, 0)
	local mode = tb_quest[questId].targets[offset+1][ 2 ]
	if mode == 0 then
		showname = '小怪'
	elseif tb_creature_template[params[ 1 ]] then
		showname = tb_creature_template[params[ 1 ]].name
	end
	return self:OnUpdateModeObjectTimes(playerInfo, start, offset, params, showname)
end

return QuestKillMonster