InstanceResGem = class("InstanceResGem", InstanceResBase)

InstanceResGem.Name = "InstanceResGem"
InstanceResGem.exit_time = 10

InstanceResGem.GEM_NAME = "GEM11:11"

-- 仇恨度
InstanceResGem.THREAT_V = 100

function InstanceResGem:ctor(  )
	
end


-- 副本失败退出
function InstanceResGem:instanceFail()
	local state = self.STATE_FAIL
	if self:CheckQuestAfterTargetUpdate() then
		state = self.STATE_FINISH
	end
	self:SetMapState(state)
end

function InstanceResGem:InitRes(config)
	outFmtDebug("gem----------------------- %d", #config.protectors)
	-- 加晶石
	for _, pInfo in pairs(config.protectors) do
		local entry = pInfo[ 1 ]
		local bornX = pInfo[ 2 ]
		local bornY = pInfo[ 3 ]
		
		local creature = mapLib.AddCreature(self.ptr, 
				{templateid = entry, x = bornX, y = bornY, active_grid = true, alias_name = InstanceResGem.GEM_NAME, ainame = "AI_gem", npcflag = {}})
	end
	
	mapLib.AddTimer(self.ptr, 'OnTimerRefreshGemHp', 1000)
end

function InstanceResGem:OnAfterJoinPlayer(player)
	InstanceInstBase.OnAfterJoinPlayer(self, player)
	local GEM_NPC = mapLib.AliasCreature(self.ptr, InstanceResGem.GEM_NAME)
	if not GEM_NPC then
		return
	end
	creatureLib.SetMonsterHost(GEM_NPC, player)
end

--刷怪
function InstanceResGem:OnRefreshMonster(player)
	
	-- 由于是进副本就刷的, 判断如果进入时间比开始时间开始时间超过2秒以上则不刷了
	-- 主要为了解决离线重连的问题
	local time = os.time()
	local startTime = self:GetMapCreateTime()
	if time - startTime > 2 then
		return
	end
	
	self:RefreshMonsterBatch(player)

end


-- 更新晶石血量
function InstanceResGem:OnTimerRefreshGemHp()
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		return false
	end
	
	local GEM_NPC = mapLib.AliasCreature(self.ptr, InstanceResGem.GEM_NAME)
	if not GEM_NPC then
		return true
	end
	local gemInfo = UnitInfo:new {ptr = GEM_NPC}
	local hp = gemInfo:GetHealth()
	local maxHealth  = gemInfo:GetMaxHealth()
	local rate = math.floor(hp * 100 / maxHealth)
	self:ProtectorHit(gemInfo:GetEntry(), rate)
	
	return true
end

function InstanceResGem:ApplyRefreshMonsterBatch(player,batchIdx)
	outFmtDebug("gem ************")

	local id = self:GetIndex()
	local config = tb_instance_res[ id ]
	local prev = player:GetLevel()
	local cnt = config.monsternum
	local entry = config.monster[1]
	local monsterposlist = config.monsterInfo
	
	local ds = 0
	local bornX
	local bornY
	local indx
	
	for i = 1, cnt do
		bornX = monsterposlist[ 1 ][ 1 ]
		bornY = monsterposlist[ 1 ][ 2 ]
		indx = REFRESH_MONSTER_FIELD_INFO_START + ds * 2
		self:AddMonsterInfo(indx, entry, prev, bornX, bornY)
		ds = ds + 1
		
		bornX = monsterposlist[ 2 ][ 1 ]
		bornY = monsterposlist[ 2 ][ 2 ]
		
		indx = REFRESH_MONSTER_FIELD_INFO_START + ds * 2
		self:AddMonsterInfo(indx, entry, prev, bornX, bornY)
		ds = ds + 1
	end
	
	cnt = cnt + cnt
	self:SetUInt16(REFRESH_MONSTER_FIELD_ID, 0, 0)
	self:SetUInt16(REFRESH_MONSTER_FIELD_ID, 1, cnt)
	
	mapLib.DelTimer(self.ptr, 'OnTimer_MonsterBornOneByOne')
	mapLib.AddTimer(self.ptr, 'OnTimer_MonsterBornOneByOne', self.MonsterRefreshInterval)
	
	return true,cnt
end

function InstanceResGem:AddMonsterInfo(indx, entry, level, bornX, bornY)
	self:SetUInt16(indx, 0, entry)
	self:SetUInt16(indx, 1, level)
	self:SetUInt16(indx+1, 0, bornX)
	self:SetUInt16(indx+1, 1, bornY)
end

function InstanceResGem:OnTimer_MonsterBornOneByOne()
	local dids = self:GetUInt16(REFRESH_MONSTER_FIELD_ID, 0)
	local need = self:GetUInt16(REFRESH_MONSTER_FIELD_ID, 1)
	if dids >= need then
		return false
	end
	
	local indx = dids * 2 + REFRESH_MONSTER_FIELD_INFO_START
	self:CreateMonster(indx)

	indx = indx + 2
	self:CreateMonster(indx)
	
	self:AddUInt16(REFRESH_MONSTER_FIELD_ID, 0, 2)
	
	return true
end

function InstanceResGem:CreateMonster(indx)
	local entry = self:GetUInt16(indx  , 0)
	local level = self:GetUInt16(indx  , 1)
	local bornX = self:GetUInt16(indx+1, 0)
	local bornY = self:GetUInt16(indx+1, 1)
	
	local GEM_NPC = mapLib.AliasCreature(self.ptr, InstanceResGem.GEM_NAME)
	local tar_x, tar_y = unitLib.GetPos(GEM_NPC)
	local angle = postion.angle(bornX, bornY, tar_x, tar_y)
	local creature = mapLib.AddCreature(self.ptr, 
			{templateid = entry, x = bornX, y = bornY, level=level, active_grid = true, 
			ainame = "AI_res", npcflag = {}, attackType = REACT_AGGRESSIVE, toward = angle})
	
	-- 设置仇恨度
	creatureLib.ModifyThreat(creature, GEM_NPC, self.THREAT_V)
end

--刷新boss
function InstanceResGem:RefreshBoss(playerInfo)
	local id = self:GetIndex()
	local prev = playerInfo:GetLevel()
	
	mapLib.AddTimer(self.ptr, 'OnTimerRefreshBoss', 5000, id, prev)
end

--刷新boss
function InstanceResGem:OnTimerRefreshBoss(id, level)
	local id = self:GetIndex()
	local config = tb_instance_res[ id ]
	local entry = config.boss
	local bornPos = config.bosspos
	
	local creature = mapLib.AddCreature(self.ptr, {templateid = entry, x = bornPos[1], y = bornPos[2], level=level, 
		active_grid = true, alias_name = config.name, ainame = "AI_resBoss", npcflag = {}})
	
	-- 设置仇恨度
	local GEM_NPC = mapLib.AliasCreature(self.ptr, InstanceResGem.GEM_NAME)
	creatureLib.ModifyThreat(creature, GEM_NPC, self.THREAT_V)
	
	return false
end

----------------------------- 怪物----------------------------
AI_gem = class("AI_gem", AI_Base)
AI_gem.ainame = "AI_gem"
--死亡
function AI_gem:JustDied( map_ptr,owner,killer_ptr )
	-- 先判断是不是试炼塔副本
	local mapid = mapLib.GetMapID(map_ptr)
	if tb_map[mapid].inst_sub_type ~= INSTANCE_SUB_TYPE_RES then
		return
	end
	
	local instanceInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
	
	-- 如果时间到了失败了 即使最后下杀死BOSS都没用
	if instanceInfo:GetMapState() ~= instanceInfo.STATE_START then
		return
	end
	
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	-- 失败
	instanceInfo:SetMapState(instanceInfo.STATE_FAIL)
	
	return 0
end


return InstanceResGem