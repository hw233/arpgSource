InstanceFieldBase = class("InstanceFieldBase", Instance_base)

InstanceFieldBase.Name = "InstanceFieldBase"
InstanceFieldBase.player_auto_respan = 10
InstanceFieldBase.REFRESH_CREATURE_INTERNAL = 5000	--5s检测一次
InstanceFieldBase.Treasure_Entry = 7

function InstanceFieldBase:ctor(  )
	
end

--初始化脚本函数
function InstanceFieldBase:OnInitScript(  )
	Instance_base.OnInitScript(self) --调用基类
	
	--添加boss刷新器
	if self:IsFieldBossMap() then
		mapLib.DelTimer(self.ptr, 'OnTimer_CheckRefresh')
		mapLib.AddTimer(self.ptr, 'OnTimer_CheckRefresh', self.REFRESH_CREATURE_INTERNAL)
	end
end

-- 是否是野外boss刷新地图
function InstanceFieldBase:IsFieldBossMap()
	local mapid = self:GetMapId()
	local lineNo = self:GetMapLineNo()
	
	return tb_field_boss[mapid] and lineNo > 0 and lineNo <= MAX_DEFAULT_LINE_COUNT
end


--重置BOSS和宝箱
function InstanceFieldBase:OnClear()
	-- 移除boss如果有
	local bossName = self:GetBossAliasName()
	self:RemoveIfExist(bossName)
	
	-- 移除箱子如果有
	mapLib.RemoveGameObjectByEntry(self.ptr, self.Treasure_Entry)
end

-- 移除
function InstanceFieldBase:RemoveIfExist(aliasName)
	local creature = mapLib.AliasCreature(self.ptr, aliasName)
	if creature then
		creatureLib.RemoveMonster(creature)
	end
end

-- 得到宝箱名字
function InstanceFieldBase:GetTreaAliasName()
	local mapid = self:GetMapId()
	local lineNo = self:GetMapLineNo()
	
	return string.format("FBT%d_%d", mapid, lineNo)
end

-- 得到BOSS名字
function InstanceFieldBase:GetBossAliasName()
	local mapid = self:GetMapId()
	local lineNo = self:GetMapLineNo()
	
	return string.format("BOSS%d_%d", mapid, lineNo)
end

--定时检查刷新
function InstanceFieldBase:OnTimer_CheckRefresh()
	local timenow = os.time()
	
	local mapid = self:GetMapId()
	local lineNo = self:GetMapLineNo()
	
	-- 是否已经结束
	if globalValue:IsFieldBossFinish(mapid, lineNo) then
		return true
	end
	
	-- 是否开始倒计时
	if globalValue:IsFieldBossCountDown(mapid, lineNo) then
		self:OnClear()
		return true
	end

	-- 是否刷新BOSS
	if globalValue:IsFieldBossBorn(mapid, lineNo) then
		-- 判断时间到了没有
		local bossName = self:GetBossAliasName()
		local boss = mapLib.AliasCreature(self.ptr, bossName)
		if not boss then
			local bossConfig = tb_field_boss[mapid]
			local entry = bossConfig.entry
			local config = tb_creature_template[entry]
			local creature = mapLib.AddCreature(self.ptr, {
					templateid = entry, x = bossConfig.bossPosi[ 1 ], y = bossConfig.bossPosi[ 2 ], 
					active_grid = true, alias_name = bossName, ainame = "AI_FieldBoss", npcflag = {}
				}
			)
			
			if creature then
				local creatureInfo = UnitInfo:new{ptr = creature}
				-- 标识为boss怪
				creatureInfo:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
			end
			app:CallOptResult(self.ptr, OPERTE_TYPE_FIELD_BOSS, FIELD_BOSS_OPERTE_BOSS_BORN, {config.name, tb_map[mapid].name})
		end
		
		return true
	end

	-- 是否是宝箱出现
	if globalValue:IsFieldBossTreasureOccur(mapid, lineNo) then
		local posx = tb_field_boss[mapid].bossPosi[ 1 ]
		local posy = tb_field_boss[mapid].bossPosi[ 2 ]
		self:AddTreasure(posx, posy)
		
		return true
	end
	
	return true
end


-- 加宝箱
function InstanceFieldBase:OnTimer_AddTreasure(x, y)
	local gameObject = mapLib.GetGameObjectByEntry(self.ptr, self.Treasure_Entry)
	if not gameObject then
		mapLib.AddGameObject(self.ptr, self.Treasure_Entry, x, y, GO_GEAR_STATUS_END)
	end
	-- false表示定时刷新器结束
	return false
end



-- 加宝箱
function InstanceFieldBase:AddTreasure(x, y, last)
	last = last or 0
	local gameObject = mapLib.GetGameObjectByEntry(self.ptr, self.Treasure_Entry)
	if not gameObject then
		-- 加宝箱
		if last <= 0 then
			self:OnTimer_AddTreasure(x, y)
		else
			-- 给自己加个延迟
			mapLib.AddTimer(self.ptr, 'OnTimer_AddTreasure', last, x, y)
		end
		
			
		local mapid = self:GetMapId()
		local lineNo = self:GetMapLineNo()
		local countdown = globalValue:GetProtectCooldown(mapid, lineNo)
		
		if countdown > 0 then
			-- 宝箱保护倒计时
			mapLib.AddTimer(self.ptr, 'OnTimer_Priority', countdown * 1000)
		end
	end
end

function InstanceFieldBase:OnTimer_Priority()
	app:CallOptResult(self.ptr, OPERTE_TYPE_FIELD_BOSS, FIELD_BOSS_OPERTE_PROTECT, {tb_gameobject_template[self.Treasure_Entry].name})
	return false
end

--当玩家被玩家杀掉时触发
function InstanceFieldBase:OnPlayerKilled(player, killer)
	local playerInfo = UnitInfo:new {ptr = player}
	local killername = binLogLib.GetStr(killer, BINLOG_STRING_FIELD_NAME)
	self:OnSendDeathInfo(playerInfo, killername, '')
	
	return 0
end
	
-- 当玩家被怪物杀死
function InstanceFieldBase:OnPlayerKilledByMonster(player, killer)
	local playerInfo = UnitInfo:new {ptr = player}
	local killername = binLogLib.GetStr(killer, BINLOG_STRING_FIELD_NAME)
	self:OnSendDeathInfo(playerInfo, killername, '')
	
	return 0
end

--当玩家死亡后触发()
function InstanceFieldBase:OnPlayerDeath(player)
	
end

-- 使用需要读进度条广播的游戏对象
function InstanceFieldBase:OnUseBroadCastGameObject(playerInfo, gameObjectInfo)
	
	local entry = gameObjectInfo:GetEntry()
	if entry ~= self.Treasure_Entry then
		return
	end
	
	local mapid  = self:GetMapId()
	local lineNo = self:GetMapLineNo()
	
	-- 箱子已经被领了
	if not globalValue:IsFieldBossTreasureOccur(mapid, lineNo) then
		return
	end
	
	-- 在优先保护中不能采集
	if not globalValue:CanPickTreasure(mapid, lineNo, playerInfo:GetPlayerGuid()) then
		return
	end
	
	-- 已经有人在采集
	local openguid = mapLib.GetOnOpenGuid(self.ptr)
	if openguid ~= "" then
		-- 箱子正在被别人采集
		return
	end
	
	local seconds = tb_gameobject_template[entry].time
	-- 设置采集进度条
	self:OnProcess(playerInfo, seconds, 'OnTimer_PickingTreasure')
	mapLib.OnOpenTreasure(self.ptr, playerInfo:GetPlayerGuid())
end


-- 采集中
function InstanceFieldBase:OnProcess(playerInfo, seconds, callback)
	local gameObject = mapLib.GetGameObjectByEntry(self.ptr, self.Treasure_Entry)
	local gameObjectInfo = UnitInfo:new {ptr = gameObject}
	gameObjectInfo:SetProcessTime(os.time() + seconds)
	gameObjectInfo:SetProcessSeconds(seconds)
	gameObjectInfo:SetPickedName(playerInfo:GetName())
	mapLib.AddTimer(self.ptr, callback, seconds * 1000, playerInfo.ptr)
end

-- 箱子开启完成
function InstanceFieldBase:OnTimer_PickingTreasure(player)
	local mapid  = self:GetMapId()
	local lineNo = self:GetMapLineNo()
	globalValue:FieldBossTreasurePicked(mapid, lineNo)
	
	local openguid = mapLib.GetOnOpenGuid(self.ptr)	
	mapLib.OnOpenTreasure(self.ptr, "")
	-- 移除计时器
	mapLib.DelTimer(self.ptr, 'OnTimer_Priority')
	
	-- 删除箱子
	mapLib.RemoveGameObjectByEntry(self.ptr, self.Treasure_Entry)
	
	-- 播放消息
	-- 拾取宝箱
	local playerInfo = UnitInfo:new {ptr = player}
	local dropId = GetRewardIfGenderSensitive(tb_field_boss[mapid].dropTable, playerInfo:GetGender())
	local dict = {}
	DoRandomDropTable({dropId}, dict)
	PlayerAddRewards(player, dict)
	local rewards = {}
	for entry, _ in pairs(dict) do
		local itemConfig = tb_item_template[entry]
		for _, record in pairs(itemConfig.records) do
			if record == ITEM_RECORD_MAP then
				local itemName = GetColordItemName(entry)
				table.insert(rewards, itemName)
			end
		end
	end
	
	local reward = string.join(",", rewards)
	local playerInfo = UnitInfo:new {ptr = player}
	local playerName = ToShowName(playerInfo:GetName())
	app:CallOptResult(self.ptr, OPERTE_TYPE_FIELD_BOSS, FIELD_BOSS_OPERTE_PICKED, {playerName, tb_gameobject_template[self.Treasure_Entry].name, reward})
	
	return false
end

-- 箱子开启打断
function InstanceFieldBase:OnDisruptPicking()
	mapLib.DelTimer(self.ptr, 'OnTimer_PickingTreasure')
	mapLib.OnOpenTreasure(self.ptr, "")
	
	local gameObject = mapLib.GetGameObjectByEntry(self.ptr, self.Treasure_Entry)
	local gameObjectInfo = UnitInfo:new {ptr = gameObject}
	-- 置成过去时间
	gameObjectInfo:SetProcessTime(os.time() - 2)
	gameObjectInfo:SetPickedName("")
end

-- 被打断
function InstanceFieldBase:OnDisrupt(killer)
	self:OnDisruptPicking()
end

-- 当玩家移动后
function InstanceFieldBase:OnPlayerAfterMove(player)
	self:OnPlayerCancelPicking(player)
end

--跳跃前需要处理的逻辑
function InstanceFieldBase:OnStartJump(playerInfo)
	self:OnPlayerCancelPicking(playerInfo.ptr)
end

--释放技能需要处理的逻辑
function InstanceFieldBase:OnSpell(player)
	self:OnPlayerCancelPicking(player)
end

-- 自动取消采集
function InstanceFieldBase:OnPlayerCancelPicking(player)
	local openguid = mapLib.GetOnOpenGuid(self.ptr)
	local myguid = playerLib.GetPlayerGuid(player)
	if myguid and openguid ~= myguid then
		return
	end
	self:OnDisruptPicking()
end

--当玩家离开时触发
function InstanceFieldBase:OnLeavePlayer( player, is_offline)
	
	-- 离开打断
	local openguid = mapLib.GetOnOpenGuid(self.ptr)
	local playerInfo = UnitInfo:new{ptr = player}
	if playerInfo:GetPlayerGuid() == openguid then
		self:OnDisruptPicking(playerInfo)
	end
end

-------------------------野外boss---------------------------
AI_FieldBoss = class("AI_FieldBoss", AI_Base)
AI_FieldBoss.ainame = "AI_FieldBoss"

--死亡
function AI_FieldBoss:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	local bossInfo = UnitInfo:new{ptr = owner}
	local unitInfo = UnitInfo:new{ptr = killer_ptr}
	
	local instanceInfo = InstanceFieldBase:new{ptr = map_ptr}
	local mapid  = instanceInfo:GetMapId()
	local lineNo = instanceInfo:GetMapLineNo()
	local entry = bossInfo:GetEntry()
	
	globalValue:FieldBossDamageDeal(mapid, lineNo, 0)
	-- 获得伤害最高的guid
	local guid = mapLib.GetMaxinumFieldBossDamage(map_ptr)
	-- 清空BOSS伤害
	mapLib.ClearFieldBossDamage(map_ptr)
	
	-- 如果是怪物就找归属者
	if unitInfo:GetTypeID() == TYPEID_UNIT then
		local host = creatureLib.GetMonsterHost(killer_ptr)
		if host then
			unitInfo = UnitInfo:new {ptr = host}
		end
	end
	
	-- BOSS死亡
	globalValue:FieldBossKilled(mapid, lineNo, guid, unitInfo:GetName())
	local playerName = ToShowName(unitInfo:GetName())
	app:CallOptResult(instanceInfo.ptr, OPERTE_TYPE_FIELD_BOSS, FIELD_BOSS_OPERTE_BOSS_KILL, {tb_creature_template[entry].name, playerName, tb_gameobject_template[InstanceFieldBase.Treasure_Entry].name})
	
	-- 加宝箱
	local posx, posy = unitLib.GetPos(owner)
	instanceInfo:AddTreasure(posx, posy, 500)
	
	return 0
end

-- TODO:需要修复, DamageTaken 才精确
-- 受到伤害后
function AI_FieldBoss:DamageDeal( owner, unit, damage)
	local bossInfo = UnitInfo:new{ptr = owner}

	local currHealth = bossInfo:GetHealth()
	local maxHealth  = bossInfo:GetMaxHealth()
	
	local map_ptr = unitLib.GetMap(owner)
	local instanceInfo = InstanceFieldBase:new{ptr = map_ptr}
	local mapid  = instanceInfo:GetMapId()
	local lineNo = instanceInfo:GetMapLineNo()
	
	globalValue:FieldBossDamageDeal(mapid, lineNo, currHealth * 100 / maxHealth)
	
	if damage > 0 then
		-- 如果是怪物就找归属者
		if GetUnitTypeID(unit) == TYPEID_UNIT then
			unit = creatureLib.GetMonsterHost(unit)
		end
		if unit then
			local unitInfo = UnitInfo:new {ptr = unit}
			local guid = unitInfo:GetPlayerGuid()
			mapLib.AddFiledBossDamage(map_ptr, guid, damage)
			-- 参加野外BOSS
			playerLib.SendToAppdDoSomething(unit, SCENED_APPD_JOIN_FIELD_BOSS, mapid)
		end
	else
		mapLib.ClearFieldBossDamage(map_ptr)
	end
end

return InstanceFieldBase