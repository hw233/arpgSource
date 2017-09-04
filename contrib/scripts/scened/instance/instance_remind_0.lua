InstanceRemind0 = class("InstanceRemind0", InstanceInstBase)

InstanceRemind0.Name = "InstanceRemind0"
InstanceRemind0.BOSS_NAME = "InstanceRemindBOSS"

function InstanceRemind0:ctor(  )
	
end

-- 服务器退出
-- 客户端请求退出
-- 客户端下线

--初始化脚本函数
function InstanceRemind0:OnInitScript(  )
	-- 不让重复初始化
	if self:isInstanceInit() then
		return
	end
	self:SetInstanceInited()

	InstanceInstBase.OnInitScript(self) --调用基类
	
	self:parseGeneralId()
	
	local timestamp = os.time() + 60
	self:SetMapQuestEndTime(timestamp)
	self:SetMapEndTime(timestamp)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp)
end

-- 副本失败退出
function InstanceRemind0:timeoutCallback()
	self:SetMapState(self.STATE_FINISH)
	return false
end


function InstanceRemind0:parseGeneralId()
	local generalId	= self:GetMapGeneralId()
	local params = string.split(generalId, '|')
	
	local prevMapId = tonumber(params[ 3 ])
	local entry		= tonumber(params[ 6 ])
	local bornX		= tonumber(params[ 7 ])
	local bornY		= tonumber(params[ 8 ])
	local lineNo	= tonumber(params[ 9 ])
	self:SetUInt32(MAP_INT_FIELD_RESERVE1, prevMapId)
	self:SetUInt32(MAP_INT_FIELD_RESERVE2, lineNo)
	
	local config	= tb_creature_template[entry]
	local boss = mapLib.AliasCreature(self.ptr, self.BOSS_NAME)
	if not boss then
		local creature = mapLib.AddCreature(self.ptr, {
				templateid = entry, x = bornX, y = bornY, 
				active_grid = true, alias_name = self.BOSS_NAME, ainame = 'AI_RemindBoss0', npcflag = {}
			}
		)
	end
end

--当副本状态发生变化时间触发
function InstanceRemind0:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		
		--10s后结束副本
		local timestamp = os.time()
		self:SetMapEndTime(timestamp)
		
		-- 直接传出去
		local allPlayers = mapLib.GetAllPlayer(self.ptr)
		local player_ptr = allPlayers[ 1 ]
		local prevMapId = self:GetUInt32(MAP_INT_FIELD_RESERVE1)
		local lineNo = self:GetUInt32(MAP_INT_FIELD_RESERVE2)
		local toX, toY = unitLib.GetPos(player_ptr)
		playerLib.Teleport(player_ptr, prevMapId, toX, toY, lineNo)
	end
end

-- 副本失败退出
function InstanceRemind0:instanceFail()
	self:SetMapState(self.STATE_FAIL)
end

--玩家加入地图
function InstanceRemind0:OnJoinPlayer(player)
	
	InstanceInstBase.OnJoinPlayer(self, player)
	
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
		mapLib.ExitInstance(self.ptr, player)
		self:SetMapState(self.STATE_FAIL)
	end
end

--当玩家死亡后触发()
function InstanceRemind0:OnPlayerDeath(player)
	-- 如果状态已经改变, 即使死了也不再更新时间
	if self:GetMapState() ~= self.STATE_START then
		return
	end
	
	-- 立马复活
	unitLib.Respawn(player, RESURRPCTION_HUANHUNDAN, 100)	--原地复活
end

-- 复活后
function InstanceRemind0:DoAfterRespawn(unit_ptr)
	local unitInfo = UnitInfo:new{ptr = unit_ptr}
	-- 加无敌隐身buff
	unitLib.AddBuff(unit_ptr, BUFF_INVINCIBLE, unit_ptr, 1, 1)
	-- 设置状态
	self:SetMapState(self.STATE_FAIL)
end

--当玩家离开时触发
function InstanceRemind0:OnLeavePlayer( player, is_offline)
	self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
	self:RemoveTimeOutCallback(self.Leave_Callback)
	-- 为了处理游戏刷新(不知道什么时候退出的情况)
	if self:GetMapState() == self.STATE_START then
		self:SetMapState(self.STATE_FAIL)
	end

	-- 离线情况下
	if is_offline then
		local playerInfo = UnitInfo:new{ptr = player}
		local prevMapId = self:GetUInt32(MAP_INT_FIELD_RESERVE1)
		local toX, toY = unitLib.GetPos(player)
		-- 设置玩家坐标
		playerInfo:SetMapID(prevMapId)
		unitLib.SetPos(player, toX, toY)
	end
end

function InstanceRemind0:IsNeedTeleportToOrigin()
	return true
end

function InstanceRemind0:OnSpecifyTeleportOrigin(player)
	local prevMapId = self:GetUInt32(MAP_INT_FIELD_RESERVE1)
	local toX, toY = unitLib.GetPos(player)
	playerLib.SetPlayerEnterOrigin(player, prevMapId, toX, toY)
end

-------------------------------- BOSS
AI_RemindBoss0 = class("AI_RemindBoss0", AI_Base)
AI_RemindBoss0.ainame = "AI_RemindBoss0"
--死亡
function AI_RemindBoss0:JustDied( map_ptr,owner,killer_ptr )	
	
	local instanceInfo = InstanceRemind0:new{ptr = map_ptr}
	
	-- 如果时间到了失败了 即使最后下杀死BOSS都没用
	if instanceInfo:GetMapState() ~= instanceInfo.STATE_START then
		return
	end
	
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	instanceInfo:SetMapState(instanceInfo.STATE_FINISH)
	
	return 0
end

return InstanceRemind0