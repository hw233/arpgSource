InstanceFaction = class("InstanceFaction", Instance_base)

InstanceFaction.Name = "InstanceFaction"
InstanceFaction.REFRESH_CREATURE_INTERNAL = 1000	--1s检测一次
InstanceFaction.RESPAWN_TIME = 10000				--10秒复活时间
InstanceFaction.DURATION_TIME = 1800				--副本持续时间
InstanceFaction.FACTION_BOSS_NAME = "faction_boss"	--帮派boss名字
InstanceFaction.FACTION_MONSTER_NAME = "faction_monster"	--帮派怪物名字
InstanceFaction.FACTION_TARGET_NAME = "faction_target"	--帮派保护目标名字


InstanceFaction.FAIL_TYPE_TARGET_LOSE = 1	--目标保护失败
InstanceFaction.FAIL_TYPE_TIME_OUT = 2		--挑战超时失败

--帮派 Guid        = self:GetMapGeneralId()



--获取家族guid
function InstanceFaction:GetFactionGuid()
	return self:GetStr(FACTION_INSTANCE_FIELD_GUID) 
end
--设置家族guid
function InstanceFaction:SetFactionGuid(val)
	self:SetStr(FACTION_INSTANCE_FIELD_GUID, val) 
end

function InstanceFaction:ctor(  )
	
end

--初始化脚本函数
function InstanceFaction:OnInitScript(  )
	Instance_base.OnInitScript(self)					--调用基类
	--self:SetMapEndTime(os.time() + self.DURATION_TIME)	--30分钟后回收
	
	--保存帮派guid
	self:parseGeneralId()
	
	--添加Boss刷新监听
	mapLib.DelTimer(self.ptr, 'OnTimer_CheckRefreshBoss')
	mapLib.AddTimer(self.ptr, 'OnTimer_CheckRefreshBoss', self.REFRESH_CREATURE_INTERNAL)
	
	self:OnRefreshMonster()
	
	-- 加结束时间
	local timestamp = os.time() + 180
	self:SetMapQuestEndTime(timestamp)
	self:SetMapEndTime(timestamp)
end

function InstanceFaction:parseGeneralId()
	local guid	= self:GetMapGeneralId()
	--outFmtInfo('============================= %s', tostring(guid))
	self:SetFactionGuid(guid)
end

-- 判断是否能退出副本
function InstanceFaction:DoPlayerExitInstance(player)
	return 0
end

--当副本状态发生变化时间触发
function InstanceFaction:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		--10s后结束副本
		--self:SetMapEndTime(os.time() + 5)
	end
end

--玩家加入地图
function InstanceFaction:OnJoinPlayer(player)
	Instance_base.OnJoinPlayer(self, player)
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
	end
end

--定时检查boss刷新
function InstanceFaction:OnTimer_CheckRefreshBoss()
	local timenow = os.time()
	local faction = app.objMgr:getObj(self:GetFactionGuid())
	
	if not faction then
		--outFmtDebug('OnTimer_CheckRefreshBoss faction is nil')
		return true
	end

	local faction_lv = faction:GetFactionLevel()
	--boss相关
	local boss_id = faction:GetChallengeBossId()
	local start_time = faction:GetChallengeBossStartTime()
	local boss_hp_rate = faction:GetChallengeBossHpRate()
	local boss_x,boss_y = faction:GetChallengeBossPos()
	local target_hp_rate = faction:GetProtectTargetHpRate()
	local target_x,target_y = faction:GetProtectTargetPos()
	
	if boss_id ~= 0 then
		local factionBossInfo = tb_faction_boss[boss_id]
		if factionBossInfo then
			if timenow >= start_time + factionBossInfo.wait_time and timenow <= start_time + factionBossInfo.wait_time + factionBossInfo.time then
				--刷新/还原boss hp为0则不刷     targethp==0 已经胜利
				local bossPtr = nil
				local targetPtr = nil
				if boss_hp_rate > 0 then
					local bossName = InstanceFaction.FACTION_BOSS_NAME
					local boss = mapLib.AliasCreature(self.ptr, bossName)
					if not boss then
						local bossConfig = tb_faction_boss[boss_id]
						local entry = bossConfig.entry
						local config = tb_creature_template[entry]
						local creature = mapLib.AddCreature(self.ptr, {
								templateid = entry, x = boss_x, y = boss_y, 
								active_grid = true, alias_name = bossName, ainame = "AI_faction_boss", npcflag = {}
							}
						)
						
						bossPtr = creature
						local bossUnit = UnitInfo:new{ptr = creature}
						if bossUnit and boss_hp_rate < 10000 then
							bossUnit:SetHealth(math.ceil(bossUnit:GetMaxHealth() * boss_hp_rate / 10000))
						end
						bossUnit:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
					else
						bossPtr = nil
						local bossUnit= UnitInfo:new{ptr = boss}
						if bossUnit then
							local currHealth = bossUnit:GetHealth()
							local maxHealth  = bossUnit:GetMaxHealth()
							local currX,currY = unitLib.GetPos(boss)
							bossUnit:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
							self:OnUpdateBossInfo(math.ceil(currHealth/maxHealth*10000),currX,currY)
						end
					end
				else
					--nothing
				end
				
				--保护目标相关生成/还原
				if target_hp_rate > 0 then
					local targetName = InstanceFaction.FACTION_TARGET_NAME
					local target = mapLib.AliasCreature(self.ptr, targetName)
					if not target then
						local targetConfig = tb_faction_boss[boss_id]
						local entry = targetConfig.target_entry
						local config = tb_creature_template[entry]
						local creature = mapLib.AddCreature(self.ptr, {
								templateid = entry, x = target_x, y = target_y, 
								active_grid = true, alias_name = targetName, ainame = "AI_faction_target", npcflag = {}
							}
						)
						targetPtr = creature
						local targetUnit= UnitInfo:new{ptr = creature}
						if targetUnit and target_hp_rate < 10000 then
							targetUnit:SetHealth(math.ceil(targetUnit:GetMaxHealth() * target_hp_rate / 10000))
						end
						
						--creatureLib.SetMonsterHost(target, player)
					else
						targetPtr = target
						local targetUnit= UnitInfo:new{ptr = target}
						if targetUnit then
							local currHealth = targetUnit:GetHealth()
							local maxHealth  = targetUnit:GetMaxHealth()
							local currX,currY = unitLib.GetPos(target)
							self:OnUpdateTargetInfo(math.ceil(currHealth/maxHealth*10000),currX,currY)
						end
						--creatureLib.SetMonsterHost(target, player)
					end
				else
					--nothing
				end
				
				if bossPtr and targetPtr then
					--creatureLib.ModifyThreat(bossPtr, targetPtr, self.THREAT_V)
					creatureLib.ModifyThreat(bossPtr, targetPtr, self.THREAT_V)
				end
				
			elseif timenow > start_time + factionBossInfo.wait_time + factionBossInfo.time then
				--挑战失败结算 bosshp > 0      targethp==0  已经胜利
				if boss_hp_rate > 0 then
					local bossName = InstanceFaction.FACTION_BOSS_NAME
					local boss = mapLib.AliasCreature(self.ptr, bossName)
					if boss then
						
						mapLib.RemoveWorldObject(self.ptr,boss)
					end
					local targetName = InstanceFaction.FACTION_TARGET_NAME
					local target = mapLib.AliasCreature(self.ptr, targetName)
					if target then
						
						mapLib.RemoveWorldObject(self.ptr,target)
					end
					
					--结算
					self:OnChallengeFail(InstanceFaction.FAIL_TYPE_TIME_OUT)
				else
					--nothing
				end
				
			end
		end
	else
		local bossName = InstanceFaction.FACTION_BOSS_NAME
		local boss = mapLib.AliasCreature(self.ptr, bossName)
		if boss then
			
			mapLib.RemoveWorldObject(self.ptr,boss)
		end
		local targetName = InstanceFaction.FACTION_TARGET_NAME
		local target = mapLib.AliasCreature(self.ptr, targetName)
		if target then
			
			mapLib.RemoveWorldObject(self.ptr,target)
		end
	end
	
	
	
	return true
end
	
--刷怪
function InstanceFaction:OnRefreshMonster()
	--野怪相关
	local timenow = os.time()
	local faction = app.objMgr:getObj(self:GetFactionGuid())
	if not faction then
		return
	end
	local faction_lv = faction:GetFactionLevel()
	local startTime = self:GetMapCreateTime()
	if timenow - startTime > 2 then
		return
	end
	for id,config in pairs(tb_faction_monster) do

		local bornX = config.born_pos[1]
		local bornY = config.born_pos[2]
		local entry = config.monster_table[faction_lv][2]
		--local point = config.monster_table[faction_lv][3]
		local entryInfo	= tb_creature_template[entry]
		local born_time = 5
		
		local creature = mapLib.AddCreature(self.ptr, {
			templateid = entry, x = bornX, y = bornY, 
			active_grid = true,respan_time = born_time, alias_name = InstanceFaction.FACTION_MONSTER_NAME, ainame = "AI_faction_monster", npcflag = {}
		}
	)
		
	end
	--[[
	local config = tb_bangpai_boss[self:GetFactionMapID()]
	if config == nil then 
		--直接结束副本
		self:SetMapState(self.STATE_FAIL)
		return false
	end
	
	local boss = mapLib.AliasCreature(self.ptr, self.FACTION_BOSS_NAME)
	if not boss then
		local creature = mapLib.AddCreature(self.ptr, {templateid = config.boss_id, x = config.boss_point[1], y = config.boss_point[2], active_grid = true, alias_name = self.FACTION_BOSS_NAME, ainame = "AI_faction_boss", npcflag = {}})
		if creature then
			local creatureInfo = UnitInfo:new{ptr = creature}
			-- 标识为boss怪
			creatureInfo:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
			return false
		end	
	end
	return true
	--]]
end

--挑战成功处理
function InstanceFaction:OnChallengeWin()
	local faction_guid = self:GetFactionGuid()
	call_appd_faction_challenge_boss_win(faction_guid)
end

--挑战失败处理
function InstanceFaction:OnChallengeFail(fail_type)
	local faction_guid = self:GetFactionGuid()
	call_appd_faction_challenge_boss_fail(faction_guid,fail_type)
end

--获得令牌积分
function InstanceFaction:OnAddTokenPoints(player_guid,points)
	local faction_guid = self:GetFactionGuid()
	call_appd_faction_add_token_points(faction_guid,player_guid,points)
end

--对boss造成伤害
function InstanceFaction:OnBossDamaged(player_guid,damage)
	local faction_guid = self:GetFactionGuid()
	call_appd_faction_challenge_boss_damaged(faction_guid,player_guid,damage)
end

--同步boss信息到帮派对象
function InstanceFaction:OnUpdateBossInfo(hp_rate,x,y)
	local faction_guid = self:GetFactionGuid()
	call_appd_faction_update_boss_info(faction_guid,hp_rate,x,y)
end

--同步保护目标信息到帮派对象
function InstanceFaction:OnUpdateTargetInfo(hp_rate,x,y)
	local faction_guid = self:GetFactionGuid()
	call_appd_faction_update_target_info(faction_guid,hp_rate,x,y)
end


--当玩家离开时触发
function InstanceFaction:OnLeavePlayer( player, is_offline)
	--self:SetMapEndTime(os.time())
	--RemoveNormalBuff(player,4)
	--RemoveNormalBuff(player,5)
end

function InstanceFaction:DoIsFriendly(killer_ptr, target_ptr)
	local killer_typeId = GetUnitTypeID(killer_ptr)
	local target_typeId = GetUnitTypeID(target_ptr)
	-- 攻击者是玩家
	if killer_typeId == TYPEID_PLAYER then
		if target_typeId == TYPEID_PLAYER or binLogLib.GetStr(target_ptr,UNIT_STRING_FIELD_ALIAS_NAME) == InstanceFaction.FACTION_TARGET_NAME then
			return 1
		end
		return 0
	end

	-- 攻击者是生物
	if target_typeId == TYPEID_UNIT then
		if target_typeId == TYPEID_UNIT and binLogLib.GetStr(target_ptr,UNIT_STRING_FIELD_ALIAS_NAME) ~= InstanceFaction.FACTION_TARGET_NAME then
			return 1
		end
	end

	return 0
end
-----------------------------------------------------------------------------
--------------------------------帮派多人boss ai------------------------------
AI_faction_boss = class("AI_faction_boss", AI_Base)
AI_faction_boss.ainame = "AI_faction_boss"

--死亡
function AI_faction_boss:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	--local playerInfo = UnitInfo:new{ptr = killer_ptr}	
	--local lv = playerInfo:GetFactionFbCount()
	--playerInfo:SetFactionFbCount(lv + 1)
	--local mapInfo = InstanceFaction:new{ptr = map_ptr}
	--mapInfo:SetMapState(mapInfo.STATE_FINISH)
	
	--挑战成功结算
	local instanceInfo = InstanceFaction:new{ptr = map_ptr}
	instanceInfo:OnChallengeWin()
	return 0
end

-- 受到伤害后
function AI_faction_boss:DamageDeal( owner, unit, damage)

	local bossInfo = UnitInfo:new{ptr = owner}

	local currHealth = bossInfo:GetHealth()
	local maxHealth  = bossInfo:GetMaxHealth()
	
	local map_ptr = unitLib.GetMap(owner)
	local instanceInfo = InstanceFaction:new{ptr = map_ptr}
	local factionGuid = instanceInfo:GetFactionGuid()
	
	if damage > 0 then
		-- 如果是怪物就找归属者
		if GetUnitTypeID(unit) == TYPEID_UNIT then
			unit = creatureLib.GetMonsterHost(unit)
		end
		if unit then
			local unitInfo = UnitInfo:new {ptr = unit}
			local guid = unitInfo:GetPlayerGuid()
			

			instanceInfo:OnBossDamaged(guid,damage)

			--mapLib.AddFiledBossDamage(map_ptr, guid, damage)
			-- 参加野外BOSS
			--playerLib.SendToAppdDoSomething(unit, SCENED_APPD_JOIN_FIELD_BOSS, mapid)
		end
	else
		--mapLib.ClearFieldBossDamage(map_ptr)
	end
end


AI_faction_monster = class("AI_faction_monster", AI_Base)
AI_faction_monster.ainame = "AI_faction_monster"

--死亡
function AI_faction_monster:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	local creatureInfo = UnitInfo:new {ptr = owner}
	local entry = creatureInfo:GetEntry()
	local points = tb_creature_template[entry].faction_points

	
	--积分结算
	local playerInfo = UnitInfo:new{ptr = killer_ptr}
	local player_guid = playerInfo:GetPlayerGuid()
	
	local instanceInfo = InstanceFaction:new{ptr = map_ptr}
	instanceInfo:OnAddTokenPoints(player_guid,points)
	
	return 0
end

AI_faction_target = class("AI_faction_target", AI_Base)
AI_faction_target.ainame = "AI_faction_target"

--死亡
function AI_faction_target:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	--挑战失败结算
	local instanceInfo = InstanceFaction:new{ptr = map_ptr}
	instanceInfo:OnChallengeFail(InstanceFaction.FAIL_TYPE_TARGET_LOSE)
	
	return 0
end




return InstanceFaction