InstanceMassBoss = class("InstanceMassBoss", InstanceInstBase)
local protocols = require('share.protocols')

InstanceMassBoss.Name = "InstanceMassBoss"
InstanceMassBoss.player_auto_respan = 5
InstanceMassBoss.BOSS_ALIAS_NAME = 'MASS_BOSS'
-- 所有全民boss的排名
InstanceMassBoss.globalRankList = {}
InstanceMassBoss.bossHpRate = {}
InstanceMassBoss.bossMaxHp = {}
InstanceMassBoss.enterCount = {}

for i = 1, #tb_mass_boss_info do
	InstanceMassBoss.enterCount[ i ] = 0
	InstanceMassBoss.globalRankList[ i ] = {}
end

function InstanceMassBoss:ctor(  )
	
end

--初始化脚本函数
function InstanceMassBoss:OnInitScript(  )
	-- 不让重复初始化
	if self:isInstanceInit() then
		return
	end
	self:SetInstanceInited()
	
	InstanceInstBase.OnInitScript(self) --调用基类
	-- 解析generalid
	self:parseGeneralId()
	-- 刷boss
	self:onRefreshBoss()
	-- 由于没有时间限制 所以加不加结束时间 无所谓
	self:SetMapEndTime(os.time() + 180)
end

function InstanceMassBoss:parseGeneralId()
	local id = tonumber(self:GetMapGeneralId())
	self:SetMassBossId(id)
end

function InstanceMassBoss:SetMassBossId(id)
	self:SetUInt32(MAP_MASS_BOSS_INT_FIELD_ID, id)
end

function InstanceMassBoss:GetMassBossId()
	return self:GetUInt32(MAP_MASS_BOSS_INT_FIELD_ID)
end

function InstanceMassBoss:onRefreshBoss()
	local id = self:GetMassBossId()
	local boss = mapLib.AliasCreature(self.ptr, self.BOSS_ALIAS_NAME)
	if not boss then
		local bossconfig = tb_mass_boss_info[ id ]
		local entry  = bossconfig.bossEntry
		local born   = bossconfig.bossPos
		local config = tb_creature_template[entry]
		local creature = mapLib.AddCreature(self.ptr, 
			{
				templateid = entry, x = born[ 1 ], y = born[ 2 ], active_grid = true, 
				alias_name = self.BOSS_ALIAS_NAME, ainame = "AI_MassBoss", npcflag = {}
			}
		)
		
		if creature then
			local creatureInfo = UnitInfo:new{ptr = creature}
			-- 标识为boss怪
			creatureInfo:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
			self.bossHpRate[ id ] = 100
			self.bossMaxHp[ id ] = creatureInfo:GetMaxHealth()
			-- 有boss再刷
			mapLib.AddTimer(self.ptr, 'OnTimer_UpdateRank', 1000)
		end
	else
		local creatureInfo = UnitInfo:new{ptr = boss}
		self.bossMaxHp[ id ] = creatureInfo:GetMaxHealth()
		self.bossHpRate[ id ] = math.floor(creatureInfo:GetHealth() * 100 / creatureInfo:GetMaxHealth())
	end
end

-- 世界BOSS结束了
function InstanceMassBoss:IsEnd()
	return self:GetMapQuestEndTime() > 0 and self:GetMapQuestEndTime() <= os.time()
end

--当副本状态发生变化时间触发
function InstanceMassBoss:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH then
		--10s后结束副本
		local timestamp = os.time() + self.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
	end
end

--玩家加入地图
function InstanceMassBoss:OnJoinPlayer(player)
	
	InstanceInstBase.OnJoinPlayer(self, player)
	
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
		mapLib.ExitInstance(self.ptr, player)
		return
	end
	
	-- 结束时间到就不让进了
	if self:IsEnd() then
		mapLib.ExitInstance(self.ptr, player)
		return
	end
	
	local id = self:GetMassBossId()
	InstanceMassBoss.enterCount[ id ] = InstanceMassBoss.enterCount[ id ] + 1
	playerInfo:ChangeToFamilyMode()
end

--当玩家离开时触发
function InstanceMassBoss:OnLeavePlayer( player, is_offline)
	InstanceInstBase.OnLeavePlayer(self, player, is_offline)
	local id = self:GetMassBossId()
	InstanceMassBoss.enterCount[ id ] = InstanceMassBoss.enterCount[ id ] - 1
end

-- 当玩家死亡后
function InstanceMassBoss:OnPlayerDeath(player)
	-- 死亡立马复活并弹出去
	unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
	mapLib.ExitInstance(self.ptr, player)
end

function InstanceMassBoss:onBossDead()
	self:SetMapState(self.STATE_FINISH)
	local id = self:GetMassBossId()
	globalValue:doMassBossEnd(id)
	self.bossHpRate[ id ] = nil
	self:sendReward()
end

function onMassBossReborn(id)
	InstanceMassBoss.globalRankList[ id ] = {}
	InstanceMassBoss.enterCount[ id ] = 0
end

-- 进行排名更新
function InstanceMassBoss:OnTimer_UpdateRank()
	-- 更新排名
	local id = self:GetMassBossId()
	local rankInfo = self.globalRankList[id]
	local maxHP = self.bossMaxHp[id]
	
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
	
	self:NotifyAllRankUpdate(rankInfo, rankList)
	
	-- 全民BOSS结束
	if self:IsEnd() then
		-- outFmtInfo("############################# mass boss end")
		self.globalRankList[ id ] = {}
		self:SetMapQuestEndTime(os.time())
		return false
	end
	
	return true
end

function InstanceMassBoss:NotifyAllRankUpdate(rankInfo, rankList)
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	local tmp = {}
	for i = 1, #rankInfo do
		tmp[rankInfo[ i ][ 1 ]] = i
	end
		
	for _, player in pairs(allPlayers) do
		local unitInfo = UnitInfo:new {ptr = player}
		local mine = tmp[unitInfo:GetName()]
		mine = mine or 0
		unitInfo:call_boss_rank(2, rankList, mine)
	end
end


-- 发放奖励
function InstanceMassBoss:sendReward()
	-- 所有玩加的排名
	local tmpRank = {}
	local id = self:GetMassBossId()
	local ranklist = self.globalRankList[ id ]
	for rank, dataInfo in ipairs(ranklist) do
		tmpRank[dataInfo[ 3 ]] = rank
	end
	
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	for _, unit_player in ipairs(allPlayers) do
		local playerInfo = UnitInfo:new {ptr = unit_player}
		local playerGuid = playerInfo:GetPlayerGuid()
		local rank = tmpRank[playerGuid]
		if rank then
			local dict = {}
			-- 获得奖励
			local dropId = self:findDropId(rank)
			DoRandomDropTable({dropId}, dict)
			PlayerAddRewards(unit_player, dict, MONEY_CHANGE_MASS_BOSS, LOG_ITEM_OPER_TYPE_MASS_BOSS)
			-- 扫荡的结果发送
			local list = Change_To_Item_Reward_Info(dict, true)
			
			playerInfo:call_send_instance_result(self:GetMapState(), self.exit_time, list, INSTANCE_SUB_TYPE_MASS_BOSS, '')
		end
	end
end

function InstanceMassBoss:findDropId(rank)
	local indx = #tb_mass_boss_loot
	for i = 1, #tb_mass_boss_loot do
		local range = tb_mass_boss_loot[ i ].rankrange
		if range[ 1 ] <= rank and rank <= range[ 2 ] then
			indx = i
			break
		end
	end
	
	local id = self:GetMassBossId()
	return tb_mass_boss_loot[indx].dropid[ id ]
end

function InstanceMassBoss:AddMassBossDamage(name, damage, playerGuid)
	local id = self:GetMassBossId()
	local ranklist = self.globalRankList[ id ]
	local indx = #ranklist + 1
	
	for i = 1, #ranklist do
		local dataInfo = ranklist[ i ]
		if dataInfo[ 1 ] == name then
			dataInfo[ 2 ] = dataInfo[ 2 ] + damage
			indx = i
			break
		end
	end
	
	if indx == #ranklist + 1 then
		table.insert(ranklist, {name, damage, playerGuid})
	end
	
	self:RankSort(ranklist, indx)
end

function InstanceMassBoss:RankSort(ranklist, indx)
	for i = indx, 2, -1 do
		local curr = ranklist[ i ]
		local prev = ranklist[i-1]
		if curr[ 2 ] > prev[ 2 ] then
			ranklist[ i ] = prev
			ranklist[i-1] = curr
		end
	end
end


--AI_MassBoss
-------------------------世界boss---------------------------
AI_MassBoss = class("AI_MassBoss", AI_Base)
AI_MassBoss.ainame = "AI_MassBoss"

--死亡
function AI_MassBoss:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	local instanceInfo = InstanceMassBoss:new{ptr = map_ptr}
	instanceInfo:onBossDead()
	
	return 0
end

-- 伤害发生前
function AI_MassBoss:DamageTaken(owner, unit, damage)
	local bossInfo = UnitInfo:new{ptr = owner}
	local map_ptr = unitLib.GetMap(owner)
	local instanceInfo = InstanceMassBoss:new{ptr = map_ptr}
	local mapid  = instanceInfo:GetMapId()
	local lineNo = instanceInfo:GetMapLineNo()
	
	if damage <= 0 then return end
	
	local prev = bossInfo:GetHealth()
	local maxHealth  = bossInfo:GetMaxHealth()
	if damage > prev then damage = prev end
	
	currHealth = prev - damage
	if prev == 0 then return end

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
	
	local id = instanceInfo:GetMassBossId()
	instanceInfo.bossHpRate[ id ] = math.floor(currHealth * 100 / instanceInfo.bossMaxHp[ id ])
			
	-- 进行排名
	instanceInfo:AddMassBossDamage(name, damage, playerGuid)
end

return InstanceMassBoss