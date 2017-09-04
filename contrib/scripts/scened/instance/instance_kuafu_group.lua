local security = require("base/Security")

InstanceKuafuGroup = class("InstanceKuafuGroup", InstanceInstBase)

InstanceKuafuGroup.Name = "InstanceKuafuGroup"
InstanceKuafuGroup.sub = "group_instance"

function InstanceKuafuGroup:ctor(  )
	
end

--初始化脚本函数
function InstanceKuafuGroup:OnInitScript()
	-- 不让重复初始化
	if self:isInstanceInit() then
		return
	end
	self:SetInstanceInited()
	
	InstanceInstBase.OnInitScript(self) --调用基类
	-- 解析generalid
	self:parseGeneralId()
	
	self:OnInitMapTime()
	self:AddCountDown()
	
end

-- 活动正式开始
function InstanceKuafuGroup:OnInitMapTime()
	local id = self:GetHard()
	local timestamp = os.time() + tb_group_instance_base[ id ].startcd + tb_group_instance_base[ id ].time
	-- 加任务时间
	self:SetMapQuestEndTime(timestamp)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp)
end

function InstanceKuafuGroup:AddCountDown()
	local id = self:GetHard()
	local timestamp = os.time() + tb_group_instance_base[ id ].startcd
	self:SetMapStartTime(timestamp)
	self:AddTimeOutCallback("TryToNextPart", timestamp)
end

function InstanceKuafuGroup:parseGeneralId()
	local generalId	= self:GetMapGeneralId()
	local params = string.split(generalId, '|')
	local hard = tonumber(params[ 2 ])
	self:SetHard(hard)
	self:SetWaves(#tb_group_instance_base[hard].monster)
end

function InstanceKuafuGroup:TryToNextPart()
	if self:isWaveFinish() then	
		-- 副本完成
		self:SetMapState(self.STATE_FINISH)
		return
	end
	
	self:OnMonsterRefresh()
end

-- 当进度更新时调用
function InstanceKuafuGroup:AfterProcessUpdate(player)
	-- 判断副本是否
	if self:CheckQuestAfterTargetUpdate() then
		self:toNextWave()
		self:TryToNextPart()
	end
end

function InstanceKuafuGroup:SetHard(hard)
	self:SetUInt32(KUAFU_GROUP_INSTANCE_FIELDS_HARD, hard)
end

function InstanceKuafuGroup:GetHard()
	return self:GetUInt32(KUAFU_GROUP_INSTANCE_FIELDS_HARD)
end

function InstanceKuafuGroup:GetPart()
	return self:GetCurrFinishWave()
end

-- 刷新boss
function InstanceKuafuGroup:OnMonsterRefresh()
	local id = self:GetHard()
	local part = self:GetPart()
	
	local entry = tb_group_instance_base[id].monster[part+1]
	local cnt = tb_group_instance_base[id].monsternum[part+1]
	local pos = tb_group_instance_base[id].monsterPos[part+1]
	
	local offs = 2
	local width = offs * 2 + 1
	local grids = width * width
	
	-- 左上角点的坐标
	local lx = pos[ 1 ] - offs
	local ly = pos[ 2 ] - offs
	
	local config = tb_creature_template[entry]
	local idTable = GetRandomIndexTable(grids, cnt)
	for _, indx in pairs(idTable) do
		local id = indx - 1
		local offx = id % width
		local offy = id / width
		local bornX = lx + offx
		local bornY = ly + offy
		
		local creature = mapLib.AddCreature(self.ptr, {
			templateid = entry, x = bornX, y = bornY,
			active_grid = true, ainame = config.ainame, npcflag = {}
		})
		if string.find(config.ainame, 'Boss') then
			local creatureInfo = UnitInfo:new{ptr = creature}
			creatureInfo:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
		end
	end
	
	-- 刷新任务
	local mapQuest = tb_group_instance_base[id].quests[part+1]
	-- 加副本任务
	InstanceInstBase.OnAddQuests(self, {mapQuest})
end

--当副本状态发生变化时间触发
function InstanceKuafuGroup:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		self:SendInstanceResult()
		self:SyncResultToWeb()
		--10s后结束副本
		local timestamp = os.time() + self.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
	end
end

-- 判断是否能退出副本
function InstanceKuafuGroup:SendInstanceResult()
	local id = self:GetHard()
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	for _, player_ptr in pairs(allPlayers) do		
		local playerInfo = UnitInfo:new {ptr = player_ptr}
		
		-- 选取通关奖励
		local config = tb_group_instance_base[id]
		local itemKeys = config.fpRewardId
		local itemVals = config.fpRewardCnt
		if playerInfo:isGroupInstancePassed(id) then
			itemKeys = config.passRewardId
			itemVals = config.passRewardCnt
		end

		local rewardDict = {}
		for i = 1, #itemKeys do
			local itemId = itemKeys[ i ]
			local count  = itemVals[ i ]
			rewardDict[itemId] = count
		end

		-- 扫荡的结果发送
		local list = Change_To_Item_Reward_Info(rewardDict, true)
		playerInfo:call_send_instance_result(self:GetMapState(), self.exit_time, list, INSTANCE_SUB_TYPE_KUAFU_GROUP, '')
		
	end
end

-- 判断是否能退出副本
function InstanceKuafuGroup:DoPlayerExitInstance(player)
	-- 直接回原服
	local playerInfo = UnitInfo:new {ptr = player}
	local login_fd = serverConnList:getLogindFD()
	call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
	return 0	--返回1的话为正常退出，返回0则不让退出
end

--玩家加入地图
function InstanceKuafuGroup:OnJoinPlayer(player)
	
	InstanceInstBase.OnJoinPlayer(self, player)
	
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
	end
	
	-- 不能重复进入
	if self:findIndexByName(playerInfo:GetName()) > -1 then
		local login_fd = serverConnList:getLogindFD()
		call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
		return
	end
	
	--[[
	local id = self:GetHard()
	-- 写奖励
	local config = tb_group_instance_base[id]
		
	local itemKeys = config.passRewardId
	local itemVals = config.passRewardCnt
	if not playerInfo:isGroupInstanceClearFlag(id) then
		itemKeys = config.fpRewardId
		itemVals = config.fpRewardCnt
	end
	
	local rewardDict = {}
	for i = 1, #itemKeys do
		local itemId = itemKeys[ i ]
		local count  = itemVals[ i ]
		table.insert(rewardDict, itemId..":"..count)
	end
	--]]
	
	-- 设置名称
	local emptyIndex = self:findIndexByName()
	if emptyIndex > -1 then
		local intstart = KUAFU_GROUP_INSTANCE_INT_FIELDS_PLAYER_INFO_START + emptyIndex * MAX_KUAFU_GROUP_INSTANCE_PLAYER_INT_COUNT
		local strstart = KUAFU_GROUP_INSTANCE_STR_FIELDS_PLAYER_INFO_START + emptyIndex * MAX_KUAFU_GROUP_INSTANCE_PLAYER_STR_COUNT
		
		--self:SetStr(strstart + KUAFU_GROUP_INSTANCE_PLAYER_REWARDS, string.join(",", rewardDict))
		self:SetStr(strstart + KUAFU_GROUP_INSTANCE_PLAYER_NAME, playerInfo:GetName())
		self:SetStr(strstart + KUAFU_GROUP_INSTANCE_PLAYER_GUID, playerInfo:GetPlayerGuid())
		
		self:SetByte(intstart + KUAFU_GROUP_INSTANCE_PLAYER_DAED_TIMES, 0, 0)
		
		playerInfo:SetToGroupMode('1')
	end
end

--当玩家离开时触发
function InstanceKuafuGroup:OnLeavePlayer( player, is_offline)
	local unitInfo = UnitInfo:new{ptr = player}

	-- 活动副本结束了就不进行处理
	if self:GetMapState() ~= self.STATE_START then
		return
	end

	-- 如果没人了 那就结束
	local persons = mapLib.GetPlayersCounts(self.ptr)
	if persons == 0 then
		self:SetMapState(self.STATE_FAIL)
	end
end

-- 获得名字对应的位置, ''表示用来查询空位位置
function InstanceKuafuGroup:findIndexByName(name)
	name = name or ''
	local start = KUAFU_GROUP_INSTANCE_STR_FIELDS_PLAYER_INFO_START
	for i = 0, MAX_GROUP_INSTANCE_PLAYER_COUNT - 1 do
		if self:GetStr(start + KUAFU_GROUP_INSTANCE_PLAYER_NAME) == name then
			return i
		end
		start = start + MAX_KUAFU_GROUP_INSTANCE_PLAYER_STR_COUNT
	end
	
	return -1
end

--当玩家死亡后触发()
function InstanceKuafuGroup:OnPlayerDeath(player)
	-- 如果状态已经改变, 即使死了也不再更新时间
	if self:GetMapState() ~= self.STATE_START then
		return
	end
	
	local playerInfo = UnitInfo:new {ptr = player}
	local emptyIndex = self:findIndexByName(playerInfo:GetName())
	if emptyIndex > -1 then
		local intstart = KUAFU_GROUP_INSTANCE_INT_FIELDS_PLAYER_INFO_START + emptyIndex * MAX_KUAFU_GROUP_INSTANCE_PLAYER_INT_COUNT
		self:AddByte(intstart + KUAFU_GROUP_INSTANCE_PLAYER_DAED_TIMES, 0, 1)
		local times = self:GetByte(intstart + KUAFU_GROUP_INSTANCE_PLAYER_DAED_TIMES, 0)
		local mapid = self:GetMapId()
		local waitTimeList = tb_map[mapid].rebornWaitTime
		if times > #waitTimeList then
			times = #waitTimeList
		end
		local sec = waitTimeList[times]
		self:SetUInt32(intstart + KUAFU_GROUP_INSTANCE_PLAYER_REBRON_CD, os.time() + sec)
	end
	
	self:OnSendDeathInfo(playerInfo, '', '')
end

-- 同步数据到场景服
function InstanceKuafuGroup:SyncResultToWeb()
	if self:GetMapState() ~= self.STATE_FINISH then
		return
	end
	
	local url = string.format("%s%s/match_result", globalGameConfig:GetExtWebInterface(), InstanceKuafuGroup.sub)
	local data = {}
	
	local sendInfo = {}
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	for _, player_ptr in pairs(allPlayers) do
		table.insert(sendInfo, GetPlayerGuid(player_ptr))
	end
	
	data.ret = string.join('|', sendInfo)..';'..self:GetHard()
	data.open_time = 1
	print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@result = ", data.ret)
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		outFmtDebug("response = %s", tostring(response))
	end)
end

function InstanceKuafuGroup:GetCostTimeCD(playerInfo)
	local emptyIndex = self:findIndexByName(playerInfo:GetName())
	if emptyIndex > -1 then
		local intstart = KUAFU_GROUP_INSTANCE_INT_FIELDS_PLAYER_INFO_START + emptyIndex * MAX_KUAFU_GROUP_INSTANCE_PLAYER_INT_COUNT
		return self:GetUInt32(intstart + KUAFU_GROUP_INSTANCE_PLAYER_REBRON_CD)
	end
	
	return 0
end

-- 获取死亡次数
function InstanceKuafuGroup:GetDeadTimes(playerInfo)
	local emptyIndex = self:findIndexByName(playerInfo:GetName())
	if emptyIndex > -1 then
		local intstart = KUAFU_GROUP_INSTANCE_INT_FIELDS_PLAYER_INFO_START + emptyIndex * MAX_KUAFU_GROUP_INSTANCE_PLAYER_INT_COUNT
		local times = self:GetByte(intstart + KUAFU_GROUP_INSTANCE_PLAYER_DAED_TIMES, 0)
		return times
	end
	
	return 99999999
end


-- 地图需要清空人时要做的事
function InstanceKuafuGroup:IsNeedTeleportWhileMapClear(player)
	local playerInfo = UnitInfo:new {ptr = player}
	local login_fd = serverConnList:getLogindFD()
	call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
	return 0
end


AI_GroupMonster = class("AI_GroupMonster", AI_Base)
AI_GroupMonster.ainame = "AI_GroupMonster"
--死亡
function AI_GroupMonster:JustDied( map_ptr,owner,killer_ptr )	
	-- 先判断是不是VIP副本	
	local mapid = mapLib.GetMapID(map_ptr)
	if tb_map[mapid].inst_sub_type ~= INSTANCE_SUB_TYPE_KUAFU_GROUP then
		return
	end
	
	local instanceInfo = InstanceKuafuGroup:new{ptr = map_ptr}
	
	-- 如果时间到了失败了 即使最后下杀死BOSS都没用
	if instanceInfo:GetMapState() ~= instanceInfo.STATE_START then
		return
	end
	
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	-- 更新杀怪进度
	local ownerInfo = UnitInfo:new {ptr = owner}
	local entry = ownerInfo:GetEntry()
	local updated = instanceInfo:OneMonsterKilled(entry)
	
	-- 更新进度
	if updated then
		instanceInfo:AfterProcessUpdate(killer_ptr)
	end
	
	return 0
end

AI_GroupBoss = class("AI_GroupBoss", AI_GroupMonster)
AI_GroupBoss.ainame = "AI_GroupBoss"

return InstanceKuafuGroup