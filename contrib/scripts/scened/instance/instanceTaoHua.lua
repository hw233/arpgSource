InstanceTaoHua = class('InstanceTaoHua', Instance_base)

--副本持续时间
InstanceTaoHua.DURATION_TIME = 1800
--脚本名称(会被C++调用, 极其重要)
InstanceTaoHua.Name = "InstanceTaoHua"	
--BOSS模版
InstanceTaoHua.BossEntry = 3
--BOSS分身模版
InstanceTaoHua.BossCloneEntry = 4
--BOSS坐标点
InstanceTaoHua.BossPosition = {25,21}

--刷新坐标偏移值
InstanceTaoHua.RefreshOffset = 3;

-- 刷新坐标点
InstanceTaoHua.RefreshPositionTable = {
	{15,6},
	{34,5},
	{44,21},
	{34,38},
	{15,38},
	{6,22},
}

-- 每一波刷怪的怪物模版id
InstanceTaoHua.MonsterEntry = {
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,
}


--会刷出来的普通BUFF, 值为tb_normal_buff的id, 0表示给怪物的
InstanceTaoHua.NormalBuffIdList = {
	6, 7
}


function InstanceTaoHua:ctor()

end

--获得当前刷新次数
function InstanceTaoHua:GetRefreshTimes()
	return self:GetUInt32(TAOHUA_INT_FIELD_REFRESH_TIMES)
end

--设置当前刷新次数
function InstanceTaoHua:SetRefreshTimes(val)
	-- 刷新次数有误
	if not self:IsRefreshTimesInRange(val) then
		outFmtError("refresh times more than max = %d", TAOHUA_REFRESH_TIMES)
		return
	end

	return self:SetUInt32(TAOHUA_INT_FIELD_REFRESH_TIMES, val)
end

-- 刷新次数是否满足条件
function InstanceTaoHua:IsRefreshTimesInRange(val)
	return val >= 0 and val <= TAOHUA_REFRESH_TIMES
end

--获得当前BOSS真身GUID
function InstanceTaoHua:GetBossGuid()
	return self:GetStr(TAOHUA_STR_FIELD_REAL_BOSS_GUID)
end

--设置当前BOSS真身GUID
function InstanceTaoHua:SetBossGuid(guid)
	return self:SetStr(TAOHUA_STR_FIELD_REAL_BOSS_GUID, guid)
end

--获得当前BOSS血量
function InstanceTaoHua:GetBossHealth()
	return self:GetUInt32(TAOHUA_INT_FIELD_BOSS_CURR_HP)
end

--设置当前BOSS血量
function InstanceTaoHua:SetBossHealth(val)
	return self:SetUInt32(TAOHUA_INT_FIELD_BOSS_CURR_HP, val)
end


--初始化脚本函数
function InstanceTaoHua:OnInitScript()
	Instance_base.OnInitScript(self)					--调用基类
	self:SetMapEndTime(os.time() + self.DURATION_TIME)	--30分钟后回收
end


--当副本状态发生变化时间触发
function InstanceTaoHua:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		--10s后结束副本
		self:SetMapEndTime(os.time() + 5)
	end
end

--刷怪
function InstanceTaoHua:OnRefreshMonster()
	-- 判断刷怪次数是否正常
	local prev = self:GetRefreshTimes()
	if not self:IsRefreshTimesInRange(prev+1) then
		self:SetMapState(self.STATE_FAIL)
	end

	-- 判断副本状态是否正常
	local mapState = self:GetMapState()
	if mapState == self.STATE_FINISH or mapState == self.STATE_FAIL then
		return
	end

	-- 刷新怪物
	local pointIndex = randInt(1, #self.RefreshPositionTable)
	local bornPoint = self.RefreshPositionTable[pointIndex]

	local entry = self.MonsterEntry[prev+1]
	local config = tb_creature_template[entry]
	local cnt = randInt(5, 10)

	for i = 1, cnt do
		local bornX = bornPoint[ 1 ] + randInt(0, self.RefreshOffset)
		local bornY = bornPoint[ 2 ] + randInt(0, self.RefreshOffset)

		local creature = mapLib.AddCreature(self.ptr, 
							{templateid = entry, x = bornX, y = bornY, active_grid = true, alias_name = config.name, ainame = config.ainame, npcflag = {}})
		
	end
	
	--刷新次数
	self:SetRefreshTimes(prev+1)
end

-- 刷BOSS
function InstanceTaoHua:OnRefreshBoss()
	print("refresh boss")
	-- 判断副本状态是否正常
	local mapState = self:GetMapState()
	if mapState == self.STATE_FINISH or mapState == self.STATE_FAIL then
		return
	end

	local entry = InstanceTaoHua.BossEntry
	local bornX = InstanceTaoHua.BossPosition[ 1 ]
	local bornY = InstanceTaoHua.BossPosition[ 2 ]
	local config = tb_creature_template[entry]

	local creature = mapLib.AddCreature(self.ptr, 
							{templateid = entry, x = bornX, y = bornY, active_grid = true, alias_name = config.name, ainame = config.ainame, npcflag = {}})
	local bossHealth = self:GetBossHealth()
	if bossHealth ~= -1 then
		unitLib.SetHealth(creature, bossHealth)
	end
end

-- 刷BOSS分身
function InstanceTaoHua:OnRefreshBossClone(maxHealth)
	print("refresh clone")
	-- 判断副本状态是否正常
	local mapState = self:GetMapState()
	if mapState == self.STATE_FINISH or mapState == self.STATE_FAIL then
		return
	end

	local entry = self.BossCloneEntry
	local realIndex = randInt(1, #self.RefreshPositionTable)
	
	for i = 1, #self.RefreshPositionTable do
		local val = self.RefreshPositionTable[ i ]
		local bornX = val[ 1 ]
		local bornY = val[ 2 ]
		local config = tb_creature_template[entry]

		local creature = mapLib.AddCreature(self.ptr, 
							{templateid = entry, x = bornX, y = bornY, active_grid = true, alias_name = config.name, ainame = config.ainame, npcflag = {}})

		if i == realIndex then
			local creatureInfo = UnitInfo:new{ptr = creature}
			local currHealth = maxHealth * 0.5
			creatureInfo:SetMAXHealth(currHealth)
			unitLib.SetHealth(creature, currHealth)
			
			local guid = creatureInfo:GetGuid()
			self:SetBossGuid(guid)
		end
	end
	
end



--玩家加入地图
function InstanceTaoHua:OnJoinPlayer(player)
	print("on player join")
	Instance_base.OnJoinPlayer(self, player)
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
		mapLib.ExitInstance(self.ptr, player)
		self:SetMapState(self.STATE_FAIL)
	end

	self:SetRefreshTimes(0)
	self:OnRefreshMonster()
	--[[
	local lv = playerInfo:GetFactionFbCount() + 1
	self:SetFactionMapID(lv)
	--伤害加成buff
	if playerInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_FACTION_BOSS_ADD) then
		AddNormalBuff(player,4,player,1)
		AddNormalBuff(player,5,player,1)
	end
	]]
end

--是否友好
function InstanceTaoHua:DoIsFriendly(killer_ptr, target_ptr)
	local killerInfo = UnitInfo:new{ptr = killer_ptr}
	local targetInfo = UnitInfo:new{ptr = target_ptr}

	-- 先判断
	local ret = false
	if killerInfo:GetTypeID() == TYPEID_PLAYER then
		ret = targetInfo:GetTypeID() ~= TYPEID_UNIT or targetInfo:GetNpcFlags() ~= 0
	elseif killerInfo:GetTypeID() == TYPEID_UNIT then
		ret = targetInfo:GetTypeID() ~= TYPEID_PLAYER
	end
	
	if ret then
		return 1
	end
	return 0
end

--当玩家离开时触发
function InstanceTaoHua:OnLeavePlayer( player, is_offline)
	self:SetMapEndTime(os.time())
end

--删除其他怪物
function InstanceTaoHua:RemoveCreatures()
	-- 删怪
	local creatures = mapLib.GetAllCreature(self.ptr)
	for _, creature in pairs(creatures) do
		creatureLib.RemoveMonster(creature)
	end
end


---------------------- 普通小怪
AI_taohua_monster = class("AI_taohua_monster", AI_Base)
AI_taohua_monster.ainame = "AI_taohua_monster"
--死亡
function AI_taohua_monster:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	local number = #mapLib.GetAllCreature(map_ptr)
	local instanceInfo = InstanceTaoHua:new{ptr = map_ptr}
	local refreshs = instanceInfo:GetRefreshTimes()
	
	if number == 0 then
		if refreshs < TAOHUA_REFRESH_TIMES then
			mapLib.AddTimeStampTimer(map_ptr, "OnRefreshMonster", os.time() + 5)
		else
			instanceInfo:SetBossHealth(-1)
			mapLib.AddTimeStampTimer(map_ptr, "OnRefreshBoss", os.time() + 10)
		end
	end

	return 0
end

----------------------- BOSS 分身
AI_taohua_boss_clone = class("AI_taohua_boss_clone", AI_Base)
AI_taohua_boss_clone.ainame = "AI_taohua_boss_clone"
--死亡
function AI_taohua_boss_clone:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	local cloneInfo 	= UnitInfo:new{ptr = owner}
	local map_ptr 		= unitLib.GetMap(owner)
	local instanceInfo 	= InstanceTaoHua:new{ptr = map_ptr}
	local realBossGuid 	= instanceInfo:GetBossGuid()

	-- 碰到是真身
	if realBossGuid == cloneInfo:GetGuid() then
		-- 删怪
		mapLib.AddTimeStampTimer(map_ptr, "RemoveCreatures", os.time() + 1)
		-- 从新刷BOSS
		mapLib.AddTimeStampTimer(map_ptr, "OnRefreshBoss", os.time() + 10)
	end
	
	return 0
end



-------------------------------- BOSS
AI_taohua_boss = class("AI_taohua_boss", AI_Base)
AI_taohua_boss.ainame = "AI_taohua_boss"
--死亡
function AI_taohua_boss:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	local instanceInfo = InstanceTaoHua:new{ptr = map_ptr}
	instanceInfo:SetMapState(instanceInfo.STATE_FINISH)
	
	return 0
end

-- 触发事件的BOSS血量百分比
AI_taohua_boss.DISPATCH_EVENT_HEALTH_PERCENTS = {50, 20}

-- 收到伤害后
function AI_taohua_boss:DamageDeal( owner, unit, damage)
	local bossInfo = UnitInfo:new{ptr = owner}

	local currHealth = bossInfo:GetHealth()
	local maxHealth = bossInfo:GetMAXHealth()
	local prevHealth = currHealth + damage

	-- 判断是否满足BOSS触发事件条件
	for i = 1, #self.DISPATCH_EVENT_HEALTH_PERCENTS do
		local precent = self.DISPATCH_EVENT_HEALTH_PERCENTS[ i ]
		if currHealth * 100 <= maxHealth * precent and prevHealth * 100 > maxHealth * precent then
			local map_ptr = unitLib.GetMap(owner)
			local instanceInfo = InstanceTaoHua:new{ptr = map_ptr}
			-- 保存BOSS血量
			instanceInfo:SetBossHealth(currHealth)
			-- 删除BOSS
			creatureLib.RemoveMonster(owner)
			-- 刷6个小BOSS
			instanceInfo:OnRefreshBossClone(maxHealth)
			return
		end
	end
end

return InstanceTaoHua