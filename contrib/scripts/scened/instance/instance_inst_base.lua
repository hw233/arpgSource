InstanceInstBase = class("InstanceInstBase", Instance_base)

InstanceInstBase.Name = "InstanceInstBase"

InstanceInstBase.Time_Out_Fail_Callback = "instanceFail"

InstanceInstBase.Leave_Callback = "prepareToLeave"
InstanceInstBase.player_auto_respan = 9
InstanceInstBase.exit_time = 10

function InstanceInstBase:ctor(  )
	
end

--初始化脚本函数
function InstanceInstBase:OnInitScript(  )
	Instance_base.OnInitScript(self) --调用基类
end

-- 判断是否能退出副本
function InstanceInstBase:DoPlayerExitInstance(player)
	local playerInfo = UnitInfo:new {ptr = player}
	-- 死了让他活
	if not playerInfo:IsAlive() then
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
	end
	return 1	--返回1的话为正常退出，返回0则不让退出
end


-- 退出倒计时到了准备退出
function InstanceInstBase:prepareToLeave()
	mapLib.ExitInstance(self.ptr)
end

-- 设置总的波数
function InstanceInstBase:GetCurrFinishWave()
	return self:GetUInt16(MAP_INT_FIELD_INSTANCE_WAVE, 0)
end

-- 设置总的波数
function InstanceInstBase:SetWaves(val)
	self:SetUInt16(MAP_INT_FIELD_INSTANCE_WAVE, 1, val)
end

-- 增加当前波数
function InstanceInstBase:toNextWave()
	self:AddUInt16(MAP_INT_FIELD_INSTANCE_WAVE, 0, 1)
end

-- 判断波数是否完成
function InstanceInstBase:isWaveFinish()
	local curr = self:GetUInt16(MAP_INT_FIELD_INSTANCE_WAVE, 0)
	local alls = self:GetUInt16(MAP_INT_FIELD_INSTANCE_WAVE, 1)
	
	return curr >= alls and alls > 0
end

-- 副本失败退出
function InstanceInstBase:instanceFail()
	self:SetMapState(self.STATE_FAIL)
	local mapid = self:GetMapId()
	local type = tb_map[mapid].inst_sub_type
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	for _, unit_player in ipairs(allPlayers) do
		local playerInfo = UnitInfo:new {ptr = unit_player}
		playerInfo:call_send_instance_result(self:GetMapState(), self.exit_time, {}, type, '')
	end
end

--增加任务
function InstanceInstBase:OnAddQuests(questTable)
	self:ClearQuestMemory()
	for _, quest in pairs(questTable) do
		local instQuestType = quest[ 1 ]
		if Quest_Func_Table[instQuestType] then
			Quest_Func_Table[instQuestType](self, quest)
		end
	end
end

-- 获得任务空位置
function InstanceInstBase:GetEmptySlot()
	for i = MAP_INT_FIELD_QUESTS_START, MAP_INT_FIELD_QUESTS_END-1, 2 do
		if self:GetByte(i, 0) == 0 then
			return i
		end
	end
	
	return -1
end

function InstanceInstBase:ClearQuestMemory()
	for i = MAP_INT_FIELD_QUESTS_START, MAP_INT_FIELD_QUESTS_END-1 do
		self:SetUInt32(i, 0)
	end
	
	for i = MAP_INT_FIELD_QUESTS_PROCESS_START, MAP_INT_FIELD_QUESTS_PROCESS_END-1 do
		self:SetUInt32(i, 0)
	end
end

----------------------------------------增加任务 ------------------------------------------
--增加击杀怪物任务
function InstanceInstBase:OnAddKillMonsterQuest(quest)
	if(#quest ~= 3) then
		return -1
	end
	
	local indx = self:GetEmptySlot()
	
	if indx < 0 then
		return -1
	end
	
	self:SetByte(indx, 0, quest[ 1 ])
	self:SetByte(indx, 1, quest[ 2 ])
	self:SetUInt16(indx, 1, quest[ 3 ])
	
	return indx
end

--增加收集物品任务
function InstanceInstBase:OnAddPickItemQuest(quest)
	
end

--增加激活机关任务
function InstanceInstBase:OnAddActiveMachineQuest(quest)
	
end

--增加守护NPC任务
function InstanceInstBase:OnAddProtectNPCQuest(quest)
	-- 增加结构同击杀怪物任务一样
	local indx = self:OnAddKillMonsterQuest(quest)
	if indx > 0 then
		local offset = (indx - MAP_INT_FIELD_QUESTS_START) / 2
		self:SetByte(MAP_INT_FIELD_QUESTS_PROCESS_START, offset, 100)
	end
end

--增加护送NPC任务
function InstanceInstBase:OnAddEscortNPCQuest(quest)
	
end

--增加防守任务
function InstanceInstBase:OnAddDefenseQuest(quest)
	
end

--增加闯关任务
function InstanceInstBase:OnAddBreakThroughQuest(quest)
	
end



--[[
 = 1	-- 击杀怪物
 = 2	-- 收集物品
 = 3	-- 激活机关
 = 4	-- 守护NPC
 = 5	-- 护送NPC
 = 6	-- 防守
 = 7	-- 闯关
]]
Quest_Func_Table = {}
Quest_Func_Table[INSTANCE_QUEST_TYPE_KILL_MONSTER]	= InstanceInstBase.OnAddKillMonsterQuest
Quest_Func_Table[INSTANCE_QUEST_TYPE_PICK_ITEM]		= InstanceInstBase.OnAddPickItemQuest
Quest_Func_Table[INSTANCE_QUEST_TYPE_ACTIVE_MACHINE]= InstanceInstBase.OnAddActiveMachineQuest
Quest_Func_Table[INSTANCE_QUEST_TYPE_PROTECT_NPC]	= InstanceInstBase.OnAddProtectNPCQuest
Quest_Func_Table[INSTANCE_QUEST_TYPE_ESCORT_NPC]	= InstanceInstBase.OnAddEscortNPCQuest
Quest_Func_Table[INSTANCE_QUEST_TYPE_DEFENSE]		= InstanceInstBase.OnAddDefenseQuest
Quest_Func_Table[INSTANCE_QUEST_TYPE_BREAK_THROUGH]	= InstanceInstBase.OnAddBreakThroughQuest


----------------------------------------任务进度更新情况 ------------------------------------------

-- (1) 一个怪物被玩家杀了
function InstanceInstBase:OneMonsterKilled(entry)
	for i = MAP_INT_FIELD_QUESTS_START, MAP_INT_FIELD_QUESTS_END-1, 2 do
		if self:GetByte(i, 0) == INSTANCE_QUEST_TYPE_KILL_MONSTER and self:IsFitForKillMonsterQuest(self:GetUInt16(i, 1), entry) then
			local offset = (i - MAP_INT_FIELD_QUESTS_START) / 2
			local prev = self:GetByte(MAP_INT_FIELD_QUESTS_PROCESS_START, offset)
			self:SetByte(MAP_INT_FIELD_QUESTS_PROCESS_START, offset, prev + 1)
			return true
		end
	end
	
	return false
end

-- 是否满足击杀目标怪物
function InstanceInstBase:IsFitForKillMonsterQuest(dest, entry)
	return dest == entry or dest == 0 and tb_creature_template[entry].monster_type == 0
end


-- (4) 守护的被怪物杀了
function InstanceInstBase:ProtectorHit(entry, hpRate)
	for i = MAP_INT_FIELD_QUESTS_START, MAP_INT_FIELD_QUESTS_END-1, 2 do
		if self:GetByte(i, 0) == INSTANCE_QUEST_TYPE_PROTECT_NPC and self:GetUInt16(i, 1) == entry then
			local offset = (i - MAP_INT_FIELD_QUESTS_START) / 2
			hpRate = math.floor(hpRate)
			self:SetByte(MAP_INT_FIELD_QUESTS_PROCESS_START, offset, hpRate)
			return true
		end
	end
	
	return false
end

-- 还有几个TODO




----------------------------------------检查任务完成情况 ------------------------------------------

-- 副本时间到了以后或者进度更新后检测任务是否完成
function InstanceInstBase:CheckQuestAfterTargetUpdate(isTimeout)	
	isTimeout = isTimeout or false
	
	local ret = true
	
	for i = MAP_INT_FIELD_QUESTS_START, MAP_INT_FIELD_QUESTS_END-1, 2 do
		local targetType = self:GetByte(i, 0)
		if targetType > 0 then
			if Quest_Check_Func_Table[targetType] then
				local vist = Quest_Check_Func_Table[targetType](self, (i-MAP_INT_FIELD_QUESTS_START)/2, isTimeout)
				if not vist then ret = false end
			end
		end
	end
	
	return ret
end

--检查击杀怪物任务
function InstanceInstBase:OnCheckKillMonsterQuest(indx, isTimeout)
	local offset = MAP_INT_FIELD_QUESTS_START + indx * 2
	local process = self:GetByte(MAP_INT_FIELD_QUESTS_PROCESS_START, indx)
	local target = self:GetByte(offset, 1)
	
	return process >= target
end

--检查收集物品任务
function InstanceInstBase:OnCheckPickItemQuest(indx, isTimeout)
	return false
end

--检查激活机关任务
function InstanceInstBase:OnCheckActiveMachineQuest(indx, isTimeout)
	return false
end

--检查守护NPC任务
function InstanceInstBase:OnCheckProtectNPCQuest(indx, isTimeout)
	local offset = MAP_INT_FIELD_QUESTS_START + indx * 2
	--local process = self:GetByte(MAP_INT_FIELD_QUESTS_PROCESS_START, indx)
	
	local target = self:GetByte(offset, 1)
	local entry = self:GetUInt16(offset, 1)
	
	local ret = true
	local entryList = mapLib.QueryCreature(self.ptr, entry)
	for _, creature_ptr in pairs(entryList) do
		local creatureInfo = UnitInfo:new {ptr = creature_ptr}
		-- 目标未达到
		if creatureInfo:GetHealth() * 100 < creatureInfo:GetMaxHealth() * target then
			ret = false
			break
		end
	end
	-- 没有目标NPC
	if #entryList == 0 then
		ret = false
	end
	
	return ret
end

--检查护送NPC任务
function InstanceInstBase:OnCheckEscortNPCQuest(indx, isTimeout)
	return false
end

--检查防守任务
function InstanceInstBase:OnCheckDefenseQuest(indx, isTimeout)
	return false
end

--检查闯关任务
function InstanceInstBase:OnCheckBreakThroughQuest(indx, isTimeout)
	return false
end

--[[
 = 1	-- 击杀怪物
 = 2	-- 收集物品
 = 3	-- 激活机关
 = 4	-- 守护NPC
 = 5	-- 护送NPC
 = 6	-- 防守
 = 7	-- 闯关
]]
Quest_Check_Func_Table = {}
Quest_Check_Func_Table[INSTANCE_QUEST_TYPE_KILL_MONSTER]	= InstanceInstBase.OnCheckKillMonsterQuest
Quest_Check_Func_Table[INSTANCE_QUEST_TYPE_PICK_ITEM]		= InstanceInstBase.OnCheckPickItemQuest
Quest_Check_Func_Table[INSTANCE_QUEST_TYPE_ACTIVE_MACHINE]= InstanceInstBase.OnCheckActiveMachineQuest
Quest_Check_Func_Table[INSTANCE_QUEST_TYPE_PROTECT_NPC]	= InstanceInstBase.OnCheckProtectNPCQuest
Quest_Check_Func_Table[INSTANCE_QUEST_TYPE_ESCORT_NPC]	= InstanceInstBase.OnCheckEscortNPCQuest
Quest_Check_Func_Table[INSTANCE_QUEST_TYPE_DEFENSE]		= InstanceInstBase.OnCheckDefenseQuest
Quest_Check_Func_Table[INSTANCE_QUEST_TYPE_BREAK_THROUGH]	= InstanceInstBase.OnCheckBreakThroughQuest


--当玩家死亡后触发()
function InstanceInstBase:OnPlayerDeath(player)
	local playerInfo = UnitInfo:new{ptr = player}	
	local timestamp = os.time() + 10
	self:AddTimeOutCallback(self.Leave_Callback, timestamp)
end

-------------------------------------------------------------------------------------------

--玩家加入地图
function InstanceInstBase:OnJoinPlayer(player)
	Instance_base.OnJoinPlayer(self, player)
	
	local playerInfo = UnitInfo:new{ptr = player}
	playerInfo:ChangeToPeaceModeAfterTeleport()
end

--当玩家离开时触发
function InstanceInstBase:OnLeavePlayer( player, is_offline)
	Instance_base.OnJoinPlayer(self, player, is_offline)
	
	local playerInfo = UnitInfo:new{ptr = player}
	playerInfo:ChangeToPeaceModeAfterTeleport()
end

return InstanceInstBase