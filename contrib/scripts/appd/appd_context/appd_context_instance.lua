-- 检测能否进入VIP副本
function PlayerInfo:checkVipMapTeleport(id, hard)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_INSTANCE, MODULE_INSTANCE_VIP)) then
		return
	end

	local instMgr = self:getInstanceMgr()
	instMgr:checkIfCanEnterVIP(id, hard)
end

-- 通关VIP关卡
function PlayerInfo:passVipInstance(id, hard)
	local instMgr = self:getInstanceMgr()
	instMgr:passVipInstance(id, hard)
end

-- 进行扫荡
function PlayerInfo:sweepVIP(id)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_INSTANCE, MODULE_INSTANCE_VIP)) then
		return
	end
	-- 每个信息4个byte[0:下次需要通关难度,1:当前难度,2:挑战次数,3:购买次数]
	local config = tb_map_vip[id]
	local indx = INSTANCE_INT_FIELD_VIP_START + id - 1
	local instMgr = self:getInstanceMgr()
	
	-- 判断是否需要购买
	local times = instMgr:GetByte(indx, 2)
	if times < config.times then
		outFmtError("not need to buy because of full times")
		return
	end
	
	-- 有通关难度才让挑战
	local hard = instMgr:GetByte(indx, 1)
	if hard == 0 then
		outFmtError("not pass any hard in id = %d", id)
		return
	end
	
	-- 购买次数已满
	local buyTimes = instMgr:GetByte(indx, 3)
	if buyTimes >= #config.cost then
		outFmtError("buy times is not enough")
		return
	end
	
	local cost = config.cost[buyTimes+1]
	
	if self:costMoneys(MONEY_CHANGE_BUY_VIP_INSTANCE, {{MONEY_TYPE_GOLD_INGOT, cost}}) then
		instMgr:AddByte(indx, 3, 1)
		instMgr:sweepVIPInstance(id, hard)
	end
	
end
---------------------------资源副本-----------------------------
-- 检测能否进入资源副本
function PlayerInfo:checkResMapTeleport(id)
	local instMgr = self:getInstanceMgr()
	instMgr:checkIfCanEnterResInstance(id)
end

---------------------------试炼塔-----------------------------
-- 检测能否进入试炼塔副本
function PlayerInfo:checkTrialMapTeleport()
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_INSTANCE, MODULE_INSTANCE_TRIAL)) then
		return
	end
	local instMgr = self:getInstanceMgr()
	instMgr:checkIfCanEnterTrial()
end

-- 通关关卡
function PlayerInfo:passTrialInstance(id)
	local instMgr = self:getInstanceMgr()
	instMgr:passInstance(id)
	
	-- 设置试炼塔通关层数
	if self:GetUInt32(PLAYER_FIELD_TRIAL_LAYERS) < id then
		self:SetUInt32(PLAYER_FIELD_TRIAL_LAYERS, id)
	end
	-- 更新排名
	rankInsertTask(self:GetGuid(), RANK_TYPE_TRIAL)
end


-- 一键扫荡
function PlayerInfo:sweepTrial()
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_INSTANCE, MODULE_INSTANCE_TRIAL)) then
		return
	end
	local instMgr = self:getInstanceMgr()
	instMgr:sweepTrialInstance()
end

-- 重置试炼塔
function PlayerInfo:resetTrial()
	local instMgr = self:getInstanceMgr()
	instMgr:resetTrialInstance()
end


-- 通关关卡
function PlayerInfo:passResInstance(id)
	local instMgr = self:getInstanceMgr()
	instMgr:passResInstance(id)
	
end

-----------通关世界冒险-----------
function PlayerInfo:passWorldRiskInstance(id)
	local questMgr = self:getQuestMgr()
	questMgr:OnUpdate(QUEST_TARGET_TYPE_PASS_WORLD_RISK, {id})
	
	globalCounter:onRiskRank(self, id)
end



function PlayerInfo:sweepResInstance(id)
	local vip = self:GetVIP()
	if vip <= 0 then
		outFmtDebug("not vip cannot sweepResInstance")
		return
	end
	local instMgr = self:getInstanceMgr()
	instMgr:sweepResInstance(id)
end


-----------------------------------------------------------------------------------
--- 副本模块每日重置
function PlayerInfo:instanceDailyReset()
	local instMgr = self:getInstanceMgr()
	instMgr:instanceDailyReset()
end














-----------------世界BOSS----------------
-- 报名列表
Enroll = {}

-- 进行报名
function onEnrole(playerInfo)
	-- 一定要是报名阶段
	if not globalValue:IsWorldBossEnroll() then
		outFmtDebug("========================= not in enroll time")
		return
	end
	
	Enroll[playerInfo:GetGuid()] = 1
	local id = globalValue:GetWorldBossTimes()
	playerInfo:SetLastJoinID(id)
	playerInfo:SetLastState(PLAYER_WORLD_BOSS_ENROLLED)
	playerInfo:SetLastLine(0)
	playerInfo:SetLastDeath(0)
	
	playerInfo:CallScenedDoSomething(APPD_SCENED_WORLD_BOSS_WAITING)
end

function ClearWorldBossEnrollData()
	Enroll = {}
end

-- 获得报名的所有玩家
function GetEnrolledPlayer()
	local tb = {}
	
	for playerGuid, _ in pairs(Enroll) do
		local playerInfo = app.objMgr:getObj(playerGuid)
		outFmtDebug(playerGuid, playerInfo)
		if playerInfo then
			table.insert(tb, playerInfo)
		end
	end
	
	return tb
end

-- 报名世界boss
function OnEnrollWorldBoss()
	-- 世界BOSS开启次数+1
	globalValue:AddWorldBossTimes()
	-- 设置报名状态
	globalValue:SetWorldBossState(WORLD_BOSS_PROCESS_ENROLL)
	-- 设置报名时间
	globalValue:SetEnrollTime()
	--清理世界BOSS报名数据
	ClearWorldBossEnrollData()
end

-- 安排世界BOSS房间
function ArrangeWorldBossRoom()
	
	local playerDict = GetEnrolledPlayer()
	local count = #playerDict
	
	if count == 0 then
		return {}, {}
	end
	
	local room = math.ceil(count / tb_worldboss_time[ 1 ].roomLimit)
	local playersPerRoom = math.floor(count / room)
	local rest = count % room
	
	-- 算出每个房间容纳的人数
	local roomInfo = {}
	for i = 1, room do
		local num = playersPerRoom
		if rest > 0 then
			rest = rest - 1
			num = num + 1
		end
		table.insert(roomInfo, num)
	end
	
	return playerDict, roomInfo
end

-- 进行传送
function DoWorldBossTeleport(playerDict, roomInfo)
	local line = 1
	for _, playerInfo in pairs(playerDict) do
		-- 进行传送(必要的话进行分时传送)
		playerInfo:CallScenedDoSomething(APPD_SCENED_WORLD_BOSS_ENTER, line)
		roomInfo[line] = roomInfo[line] - 1
		if roomInfo[line] == 0 then
			line = line + 1
		end
		-- 参加野外BOSS
		local questMgr = playerInfo:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_WORLD_BOSS)
		
		playerInfo:AddActiveItem(VITALITY_TYPE_WORLD_BOSS)
	end
end

function PlayerInfo:checkMassBossMapTeleport(id)
	-- 模块没开 不让进
	if not self:GetOpenMenuFlag(MODULE_BOSS, MODULE_BOSS_RISK_BOSS) then
		return
	end
	local instMgr = self:getInstanceMgr()
	instMgr:checkIfCanEnterMassBoss(id)
end