InstanceWorldBoss = class("InstanceWorldBoss", InstanceInstBase)
local protocols = require('share.protocols')

InstanceWorldBoss.Name = "InstanceWorldBoss"
InstanceWorldBoss.player_auto_respan = 5

-- 所有线的排名
InstanceWorldBoss.rankList = {}

--[[
-- 所有先的roll点最高值和获得者, 及开始时间和结束时间
InstanceWorldBoss.rollList = {}

-- 所有roll过的名字列表
InstanceWorldBoss.rollNameList = {}
--]]

-- BOSS的血量
InstanceWorldBoss.boss_hp = {}

-- 死亡次数
InstanceWorldBoss.deathList = {}


-- 获胜标志 (0:未结束, 1:结束胜利, 2:结束失败)
InstanceWorldBoss.FLAG_WIN = 1
InstanceWorldBoss.FLAG_LOSE = 2

InstanceWorldBoss.WORLD_BOSS_NAME = "WORLD_BOSS"

-- 清理世界BOSS
function ClearWorldBossData(all)
	all  = all or 0
	InstanceWorldBoss.rankList = {}
	InstanceWorldBoss.boss_hp = {}
	InstanceWorldBoss.deathList = {}
--[[	
	if all == 1 then
		InstanceWorldBoss.rollList = {}
		InstanceWorldBoss.rollNameList = {}
	end
	--]]
end

-- 判断BOSS是否需要升级
function DoIfBOSSLevelUp()
	print("===========***======***========== WORLD BOSS end")
	
	local num = 0
	local rooms = globalValue:GetTodayWorldBossRoom()
	for i = 1, rooms do
		if globalValue:IsWorldBossEndInLine(i) then
			num = num + 1
		end
	end

	-- BOSS需要升级了
	if num * 2 > rooms then
		globalValue:AddWorldBossLevel()
	end
end
--[[
-- roll宝箱
function Roll_Treasure(playerInfo)
	-- 所有roll过的名字列表
	-- InstanceWorldBoss.rollNameList = {}
	local map_ptr = unitLib.GetMap(playerInfo.ptr)
	local instanceInfo = InstanceFieldBase:new{ptr = map_ptr}
	local lineNo = instanceInfo:GetMapLineNo()
	-- 判断是否在roll点期间
	local now = os.time()
	local rollConfig = InstanceWorldBoss.rollList[lineNo]
	if not rollConfig or not (rollConfig[ 3 ] <= now and now <= rollConfig[ 4 ]) then
		outFmtDebug("=================cannot roll")
		return
	end
	
	-- 是否参加这个活动
	local id = globalValue:GetWorldBossTimes()
	-- 没进行过报名的不能roll
	if id ~= playerInfo:GetLastJoinID() then
		outFmtDebug("current joinid = %d, but curr = %d", id, playerInfo:GetLastJoinID())
		return
	end
	
	-- 是否已经roll点
	if InstanceWorldBoss.rollNameList[lineNo][playerInfo:GetPlayerGuid()] then
		outFmtDebug("=================has already roll")
		return
	end
	
	-- 设置已经roll标志
	InstanceWorldBoss.rollNameList[lineNo][playerInfo:GetPlayerGuid()] = 1
	local gf = randInt(1, 100)
	local isHighest = 0
	if gf > rollConfig[ 1 ] then
		rollConfig[ 1 ] = gf
		rollConfig[ 2 ] = playerInfo:GetPlayerGuid()
		isHighest = 1
	end
	
	NotifyAllRollResult(map_ptr, gf, playerInfo:GetName(), isHighest, rollConfig[ 4 ], rollConfig[ 5 ])
end
--]]

function InstanceWorldBoss:ctor(  )
	
end


--初始化脚本函数
function InstanceWorldBoss:OnInitScript()
	-- 不让重复初始化
	if self:isInstanceInit() then
		return
	end
	self:SetInstanceInited()
	
	Instance_base.OnInitScript(self) --调用基类
	
	self:OnTaskStart()
	self:AddCountDown()
	
	-- 刷新BOSS 计时器
	mapLib.AddTimer(self.ptr, 'OnTimer_RefreshBoss', tb_worldboss_time[ 1 ].waitCountdown * 1000)
	
	-- 刷新排名 计时器
	mapLib.AddTimer(self.ptr, 'OnTimer_UpdateRank', 1000)
end

-- 活动正式开始
function InstanceWorldBoss:OnTaskStart()
	local waitTime = tb_worldboss_time[ 1 ].waitCountdown
		
	self:SetUInt32(WORLDBOSS_FIELDS_BORN_TIME, waitTime)
	
	-- 任务开始时间
	self:SetMapStartTime(os.time() + waitTime)
		
	local timestamp = os.time() + waitTime + tb_worldboss_time[ 1 ].time_last * 60
	-- 加任务结束时间
	self:SetMapQuestEndTime(timestamp)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp)
end

--当副本状态发生变化时间触发
function InstanceWorldBoss:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		
		--10s后结束副本
		local timestamp = os.time() + InstanceDoujiantai.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
	end
end


function InstanceWorldBoss:AddCountDown()
	local timestamp = os.time() + tb_worldboss_time[ 1 ].waitCountdown
	self:SetMapStartTime(timestamp)
	self:AddTimeOutCallback("countdown", timestamp)
end

-- 倒计时结束
function InstanceWorldBoss:countdown()
	-- 符点刷新定时器
	mapLib.AddTimer(self.ptr, 'OnBuffRefresh', tb_worldboss_time[ 1 ].interval * 1000)
end

-- 刷新buff
function InstanceWorldBoss:OnBuffRefresh()
	-- 处理刷新次数
	local times = self:GetMapReserveValue1()
	if times < tb_worldboss_time[ 1 ].times then
		self:SetMapReserveValue1(times+1)
	end
	
	-- 刷新把原来的先删掉
	local allGameObjects = mapLib.GetAllGameObject(self.ptr)
	for _, gameobject in pairs(allGameObjects) do
		mapLib.RemoveWorldObject(self.ptr, gameobject)
	end
	
	-- 刷新新的
	local indice = GetRandomIndexTable(#tb_worldboss_time[ 1 ].buffRandomPos, tb_worldboss_time[ 1 ].randomCount)
	for i = 1, #indice do
		local indx = indice[ i ]
		local pos = tb_worldboss_time[ 1 ].buffRandomPos[indx]
		local buffIndx = randInt(1, #tb_worldboss_buff)
		local entry =  tb_worldboss_buff[buffIndx].gameobject_id
		mapLib.AddGameObject(self.ptr, entry, pos[ 1 ], pos[ 2 ], GO_GEAR_STATUS_END)
	end
	
	app:CallOptResult(self.ptr, OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_BUFF_OCCUR)
	
	return self:GetMapReserveValue1() < tb_worldboss_time[ 1 ].times
end


--使用游戏对象之前
--返回1的话就继续使用游戏对象，返回0的话就不使用
function InstanceWorldBoss:OnBeforeUseGameObject(user, go, go_entryid, posX, posY)
	-- 如果已经死了 就不能捡了
	if unitLib.HasBuff(user, BUFF_INVINCIBLE) then
		return 0
	end
	
	if Script_Gameobject_Pick_Check(user, go_entryid, posX, posY) then
		return 1
	end
	return 0
end

--使用游戏对象
--返回1的话成功使用游戏对象，返回0的话使用不成功
function InstanceWorldBoss:OnUseGameObject(user, go, go_entryid, posX, posY)
	-- 判断对应的是那种buff
	for _, obj in ipairs(tb_worldboss_buff) do
		if obj.gameobject_id == go_entryid then
			local effectId = obj.buffEffect
			local buffId = tb_buff_effect[effectId].buff_id
			local duration = tb_buff_effect[effectId].duration
			SpelladdBuff(user, buffId, user, effectId, duration)
			break
		end
	end
	
	-- 需要删除对象
	mapLib.RemoveWorldObject(self.ptr, go)
	
	return 1	
end

-- 世界BOSS结束了
function InstanceWorldBoss:IsEnd()
	local lineNo = self:GetMapLineNo()
	return globalValue:IsWorldBossEnd() or globalValue:IsWorldBossEndInLine(lineNo)
end

-- 刷新BOSS
function InstanceWorldBoss:OnTimer_RefreshBoss()
	if globalValue:IsWorldBossBorn() then
		-- 刷BOSS		
		local boss = mapLib.AliasCreature(self.ptr, InstanceWorldBoss.WORLD_BOSS_NAME)
		if not boss then
			local indx   = globalValue:GetTodayWorldBossID()
			local wbconfig = tb_worldboss_base[indx]
			local entry  = wbconfig.entry
			local born   = wbconfig.born
		
			local config = tb_creature_template[entry]
			local creature = mapLib.AddCreature(self.ptr, {
					templateid = entry, x = born[ 1 ], y = born[ 2 ], active_grid = true, 
					alias_name = InstanceWorldBoss.WORLD_BOSS_NAME, ainame = config.ainame, npcflag = {}
				}
			)
			
			if creature then
				local creatureInfo = UnitInfo:new{ptr = creature}
				-- 标识为boss怪
				creatureInfo:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
			end
		end
	end

	return false
end

-- 初始化怪物信息
function InstanceWorldBoss:InitCreatureInfo(creature_ptr, bRecal, mul)
	local level = globalValue:GetWorldBossLevel()
	mul = 1 + level * 0.2
	Instance_base.InitCreatureInfo(self, creature_ptr, bRecal, mul)
end

-- 进行排名更新
function InstanceWorldBoss:OnTimer_UpdateRank()
	-- 更新排名
	local lineNo = self:GetMapLineNo()
	local rankInfo = InstanceWorldBoss.rankList[lineNo]
	local maxHP = InstanceWorldBoss.boss_hp[lineNo]
	
	local rankList = {}
	if not rankInfo then
		rankInfo = {}
	end
	
	local len = math.min(#rankInfo, 10)
	for i = 1, len do
		local stru = rank_info_t .new()
		stru.name = rankInfo[ i ][ 1 ]
		stru.value = rankInfo[ i ][ 2 ] * 100 / maxHP
		table.insert(rankList, stru)
	end
	
	NotifyAllRankUpdate(self.ptr, rankInfo, rankList)
	
	-- 世界BOSS结束
	if self:IsEnd() then
		InstanceWorldBoss.rankList[lineNo] = {}
		-- 设置提前结束时间
		-- self:SetMapEndTime(os.time() + 20)
		return false
	end
	
	return true
end


function NotifyAllRankUpdate(map_ptr, rankInfo, rankList)
	local allPlayers = mapLib.GetAllPlayer(map_ptr)
	local tmp = {}
	for i = 1, #rankInfo do
		tmp[rankInfo[ i ][ 1 ]] = i
	end
		
	for _, player in pairs(allPlayers) do
		local unitInfo = UnitInfo:new {ptr = player}
		local mine = tmp[unitInfo:GetName()]
		mine = mine or 0
		unitInfo:call_boss_rank(1, rankList, mine)
	end
end


function InstanceWorldBoss:OnLeavePlayer(player, isOffline)
	InstanceInstBase.OnLeavePlayer(self, player, isOffline)
end
--玩家加入地图
function InstanceWorldBoss:OnJoinPlayer(player)
	
	InstanceInstBase.OnJoinPlayer(self, player)
	
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
		mapLib.ExitInstance(self.ptr, player)
		return
	end
	
	-- 结束时间到就不让进了
	if os.time() >= self:GetMapQuestEndTime() or self:IsEnd() then
		mapLib.ExitInstance(self.ptr, player)
		return
	end
end

function InstanceWorldBoss:GetDeadTimes(playerInfo)
	local lineNo = self:GetMapLineNo()
	if not InstanceWorldBoss.deathList[lineNo] then
		InstanceWorldBoss.deathList[lineNo] = {}
	end
	local cnt = InstanceWorldBoss.deathList[lineNo][playerInfo:GetPlayerGuid()]
	cnt = cnt or 0
	
	return cnt
end

-- 当玩家死亡后
function InstanceWorldBoss:OnPlayerDeath(player)
	
	local lineNo = self:GetMapLineNo()
	if not InstanceWorldBoss.deathList[lineNo] then
		InstanceWorldBoss.deathList[lineNo] = {}
	end
	
	local playerInfo = UnitInfo:new{ptr = player}
	local prev = InstanceWorldBoss.deathList[lineNo][playerInfo:GetPlayerGuid()]
	prev = prev or 0
	InstanceWorldBoss.deathList[lineNo][playerInfo:GetPlayerGuid()] = prev + 1
end


--AI_WorldBoss
-------------------------世界boss---------------------------
AI_WorldBoss = class("AI_WorldBoss", AI_Base)
AI_WorldBoss.ainame = "AI_WorldBoss"

--死亡
function AI_WorldBoss:JustDied( map_ptr,owner,killer_ptr )
	outFmtInfo("============================ world BOSS was dead")
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	local instanceInfo = InstanceWorldBoss:new{ptr = map_ptr}
	local mapid  = instanceInfo:GetMapId()
	local lineNo = instanceInfo:GetMapLineNo()
	
	-- 设置状态
	globalValue:SetWorldBossEndInLine(lineNo)
	instanceInfo:SetMapEndTime(os.time() + 12)
	instanceInfo:SetMapState(instanceInfo.STATE_FINISH)
	
	-- 根据排名 发邮件
	local rankInfo = InstanceWorldBoss.rankList[lineNo]
	local indx = 1
	for i = 1, #rankInfo do
		local range = tb_worldboss_rank_reward[indx].range
		-- 不满足条件的往后移
		if range[ 2 ] > 0 and range[ 2 ] < i then
			indx = indx + 1
		end
		
		local playerGuid = rankInfo[ i ][ 3 ]
		--发到应用服发宝箱
		local player = mapLib.GetPlayerByPlayerGuid(map_ptr, playerGuid)
		local playerInfo = UnitInfo:new {ptr = player}
		
		
		local rankRewardConfig = tb_worldboss_rank_reward[indx]
		local items = rankRewardConfig.serverRewards
		local factor = rankRewardConfig.factor / 100
		
		local level = playerInfo:GetLevel()
		local lrc = tb_worldboss_rank_level_reward[ 1 ]
		for k = 1, #tb_worldboss_rank_level_reward do
			local levelRewardConfig = tb_worldboss_rank_level_reward[ k ]
			if levelRewardConfig.levelRange[ 1 ] <= level and level <= levelRewardConfig.levelRange[ 2 ] then
				lrc = levelRewardConfig
				break
			end
		end
		
		items = string.format("%s,%d,%d", items, lrc.serverRewards[ 1 ], math.floor(factor * lrc.serverRewards[ 2 ]))
		
		local mailInfo = {items, rankRewardConfig.name, rankRewardConfig.desc, GIFT_PACKS_TYPE_WORLD_BOSS}
		local str = string.join("|", mailInfo)
		
		playerLib.SendToAppdDoSomething(player, SCENED_APPD_ADD_MAIL, 0, str)
		
		local params = string.split(items, ',')
		local dict = {}
		for i = 1, #params, 2 do
			local entry = tonumber(params[ i ])
			local count = tonumber(params[i+1])
			if entry and count then
				dict[entry] = count
			end
		end
		local list = Change_To_Item_Reward_Info(dict, true)
		playerInfo:call_send_instance_result(instanceInfo:GetMapState(), instanceInfo.exit_time, list, INSTANCE_SUB_TYPE_WORLD_BOSS, '')
	end
	
	return 0
end

-- 伤害发生前
function AI_WorldBoss:DamageTaken(owner, unit, damage)
	local bossInfo = UnitInfo:new{ptr = owner}
	local map_ptr = unitLib.GetMap(owner)
	local instanceInfo = InstanceWorldBoss:new{ptr = map_ptr}
	local mapid  = instanceInfo:GetMapId()
	local lineNo = instanceInfo:GetMapLineNo()
	
	if damage <= 0 then
		return
	end
	
	local prev = bossInfo:GetHealth()
	local maxHealth  = bossInfo:GetMaxHealth()
	if damage > prev then
		damage = prev
	end
	
	currHealth = prev - damage
	
	if prev == 0 then
		return
	end
	
	local playerGuid = ''
	local name = ''
	local unitInfo = UnitInfo:new {ptr = unit}
	-- 如果是怪物就找归属者
	if unitInfo:GetTypeID() == TYPEID_UNIT then
		local host = creatureLib.GetMonsterHost(unit)
		if host then
			unitInfo = UnitInfo:new {ptr = host}
			playerGuid = unitInfo:GetPlayerGuid()
			name = unitInfo:GetName()
		end
	else
		playerGuid = unitInfo:GetPlayerGuid()
		name = unitInfo:GetName()
	end
	
	-- 设置BOSS最大血量
	InstanceWorldBoss.boss_hp[lineNo] = maxHealth
	
	-- 进行排名
	AddWorldBossDamage(lineNo, playerGuid, name, damage)
--[[	
	-- 遍历是否需要进行roll点	
	local prev = currHealth + damage
	local rollId = -1
	for i = 1, #tb_worldboss_roll do
		local hprate = tb_worldboss_roll[ i ].hprate
		if prev  * 100 > maxHealth * hprate and currHealth * 100 <= maxHealth * hprate then
			rollId = i
			break
		end
	end

	-- 需要roll点
	if rollId > 0 then
		-- 未死亡 加无敌buff
		if currHealth > 0 then
			unitLib.AddBuff(owner, BUFF_INVINCIBLE, owner, 0, config.world_boss_invincible_time)
			-- 通知进入无敌模式
		end
		
		-- 准备roll点
		instanceInfo:PrepareToRoll(rollId)
	end
	--]]
end

-- 受到伤害后
function AI_WorldBoss:DamageDeal( owner, unit, damage)
	local map_ptr = unitLib.GetMap(owner)
	local instanceInfo = InstanceWorldBoss:new{ptr = map_ptr}
	local lineNo = instanceInfo:GetMapLineNo()
	
	if damage < 0 then
		InstanceWorldBoss.rankList[lineNo] = {}
		return
	end
end

--[[
function NotifyAllRollResult(map_ptr, point, name, isHighest, cd, rollid)
	local allPlayers = mapLib.GetAllPlayer(map_ptr)
	for _, player in pairs(allPlayers) do
		local unitInfo = UnitInfo:new {ptr = player}
		unitInfo:call_roll_result(point, name, isHighest, cd, rollid)
	end
end

-- 准备roll点
function InstanceWorldBoss:PrepareToRoll(rollId)
	mapLib.AddTimer(self.ptr, 'OnTimer_RollStart', 1000, rollId)
end

-- roll提示开始
function InstanceWorldBoss:OnTimer_RollStart(rollId)
	
	local lineNo = self:GetMapLineNo()
	
	InstanceWorldBoss.rollList[lineNo] = {0, "", os.time(), os.time() + config.world_boss_roll_last_time, rollId}
	InstanceWorldBoss.rollNameList[lineNo] = {}
	
	mapLib.AddTimer(self.ptr, 'OnTimer_Roll', config.world_boss_roll_last_time * 1000, rollId)
	-- 通知所有在里面的人roll点
	NotifyAllRollResult(self.ptr, 0, "", 0, os.time() + config.world_boss_roll_last_time, rollId)
	
	return false
end

-- roll点时间结束
function InstanceWorldBoss:OnTimer_Roll(rollId)
	local lineNo = self:GetMapLineNo()
	local playerGuid = InstanceWorldBoss.rollList[lineNo][ 2 ]
	-- 没有人roll点
	if playerGuid == "" then
		return false
	end
	local player = mapLib.GetPlayerByPlayerGuid(self.ptr, playerGuid)
	local itemId = tb_worldboss_roll[rollId].itemid
	
	PlayerAddRewards(player, {[itemId] = 1}, MONEY_CHANGE_WORLD_BOSS_ROLL, LOG_ITEM_OPER_TYPE_WORLD_BOSS_ROLL)
	
	local reason = WORLD_BOSS_OPERTE_WILL_ROLL1
	if rollId == #tb_worldboss_roll then
		reason = WORLD_BOSS_OPERTE_WILL_ROLL2
	end
	local playerInfo = UnitInfo:new {ptr = player}
	local playerName = ToShowName(playerInfo:GetName())
	app:CallOptResult(self.ptr, OPERTE_TYPE_WORLD_BOSS, reason, {playerName})
	
	return false
end
--]]
function AddWorldBossDamage(lineNo, playerGuid, name, damage)
	if not InstanceWorldBoss.rankList[lineNo] then
		InstanceWorldBoss.rankList[lineNo] = {}
	end
	local rankInfo = InstanceWorldBoss.rankList[lineNo]
	local indx = #rankInfo + 1
	
	for i = 1, #rankInfo do
		local dataInfo = rankInfo[ i ]
		if dataInfo[ 1 ] == name then
			dataInfo[ 2 ] = dataInfo[ 2 ] + damage
			indx = i
			break
		end
	end
	
	if indx == #rankInfo + 1 then
		table.insert(rankInfo, {name, damage, playerGuid})
	end
	
	RankSort(rankInfo, indx)
end

-- 插入排序
function RankSort(rankInfo, indx)
	for i = indx, 2, -1 do
		local curr = rankInfo[ i ]
		local prev = rankInfo[i-1]
		if curr[ 2 ] > prev[ 2 ] then
			rankInfo[ i ] = prev
			rankInfo[i-1] = curr
		end
	end
end

return InstanceWorldBoss