InstanceVIP = class("InstanceVIP", InstanceInstBase)

InstanceVIP.Name = "InstanceVIP"
InstanceVIP.exit_time = 10
InstanceVIP.BOSS_NAME = "VIP_BOSS"

function InstanceVIP:ctor(  )
	
end

--初始化脚本函数
function InstanceVIP:OnInitScript(  )
	InstanceInstBase.OnInitScript(self) --调用基类
	
	if self:GetMapQuestEndTime() > 0 then
		return
	end
	
	self:parseGeneralId()
	
	local id	= self:getIndex()
	local hard	= self:getHard()
	local time	= tb_map_vip[ id ].time
	local questTable = tb_map_vip[ id ].quests[hard]
	-- 加副本任务
	InstanceInstBase.OnAddQuests(self, questTable)
	-- 加任务任务时间
	local timestamp = os.time() + time
	
	self:SetMapQuestEndTime(timestamp)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp)
end

function InstanceVIP:getIndex()
	return self:GetUInt16(VIP_INSTANCE_FIELD_ID, 0)
end

function InstanceVIP:getHard()
	return self:GetUInt16(VIP_INSTANCE_FIELD_ID, 1)
end

function InstanceVIP:parseGeneralId()
	local generalId	= self:GetMapGeneralId()
	local params = string.split(generalId, ':')
	local indx = tonumber(params[ 1 ])
	local hard = tonumber(params[ 2 ])
	
	self:SetUInt16(VIP_INSTANCE_FIELD_ID, 0, indx)
	self:SetUInt16(VIP_INSTANCE_FIELD_ID, 1, hard)
end

--当副本状态发生变化时间触发
function InstanceVIP:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		
		--10s后结束副本
		local timestamp = os.time() + InstanceVIP.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
	end
end

-- 判断是否能退出副本
function InstanceVIP:DoPlayerExitInstance(player)
	return 1	--返回1的话为正常退出，返回0则不让退出
end

--玩家加入地图
function InstanceVIP:OnJoinPlayer(player)
	
	InstanceInstBase.OnJoinPlayer(self, player)
	
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
		mapLib.ExitInstance(self.ptr, player)
		self:SetMapState(self.STATE_FAIL)
	end
	
	-- 刷新BOSS
	self:OnRefreshBoss(player)
	
end

--刷怪
function InstanceVIP:OnRefreshBoss(player)
	
	-- 由于是进副本就刷的, 判断如果进入时间比开始时间开始时间超过2秒以上则不刷了
	-- 主要为了解决离线重连的问题
	local time = os.time()
	local startTime = self:GetMapCreateTime()
	if time - startTime > 2 then
		return
	end
	
	-- 其实这里就没必要判断了
	local boss = mapLib.AliasCreature(self.ptr, InstanceVIP.BOSS_NAME)
	if boss then
		return
	end
	
	local hard		= self:getHard()
	local id		= self:getIndex()
	local entry		= tb_map_vip[id].creatures[hard]
	
	local config	= tb_creature_template[entry]
	local bornX		= tb_map_vip[id].bossx
	local bornY		= tb_map_vip[id].bossy

	local creature = mapLib.AddCreature(self.ptr, {
			templateid = entry, x = bornX, y = bornY, 
			active_grid = true, alias_name = InstanceVIP.BOSS_NAME, ainame = config.ainame, npcflag = {}
		}
	)
	creatureLib.ModifyThreat(creature, player, self.THREAT_V)
end

--当玩家死亡后触发()
function InstanceVIP:OnPlayerDeath(player)
	-- 如果状态已经改变, 即使死了也不再更新时间
	if self:GetMapState() ~= self.STATE_START then
		return
	end
	self:SetMapState(self.STATE_FAIL)
end

--当玩家离开时触发
function InstanceVIP:OnLeavePlayer( player, is_offline)
	if not is_offline then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		self:RemoveTimeOutCallback(self.Leave_Callback)
		self:SetMapEndTime(os.time())
	end
end

-- 当进度更新时调用
function InstanceVIP:AfterProcessUpdate(player)
	-- 判断副本是否
	if self:CheckQuestAfterTargetUpdate() then
		-- 获得随机奖励dropIdTable
		local id  = self:getIndex()
		local hard   = self:getHard()
		local playerInfo = UnitInfo:new{ptr = player}
		local dropId = GetRewardIfGenderSensitive(tb_map_vip[id].rewards[hard], playerInfo:GetGender())

		local dict = self:RandomReward(player, {dropId})
		
		-- 扫荡的结果发送
		local list = Change_To_Item_Reward_Info(dict, true)
			
		playerInfo:call_send_instance_result(self:GetMapState(), self.exit_time, list, INSTANCE_SUB_TYPE_VIP, '')
		
		
		-- 设置状态
		self:SetMapState(self.STATE_FINISH)
		
		--发到应用服进行进入判断
		playerLib.SendToAppdDoSomething(player, SCENED_APPD_PASS_VIP_INSTANCE, id, ""..hard)
	end
end

-------------------------------- BOSS
AI_vipboss = class("AI_vipboss", AI_Base)
AI_vipboss.ainame = "AI_vipboss"
--死亡
function AI_vipboss:JustDied( map_ptr,owner,killer_ptr )	
	
	-- 先判断是不是VIP副本	
	local mapid = mapLib.GetMapID(map_ptr)
	if tb_map[mapid].inst_sub_type ~= 1 then
		return
	end
	
	local instanceInfo = InstanceVIP:new{ptr = map_ptr}
	
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

return InstanceVIP