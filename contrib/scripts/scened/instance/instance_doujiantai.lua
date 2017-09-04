InstanceDoujiantai = class("InstanceDoujiantai", InstanceInstBase)

InstanceDoujiantai.Name = "InstanceDoujiantai"
InstanceDoujiantai.exit_time = 10
InstanceDoujiantai.broadcast_nogrid = 1

function InstanceDoujiantai:ctor(  )
	
end

--初始化脚本函数
function InstanceDoujiantai:OnInitScript()
	InstanceInstBase.OnInitScript(self) --调用基类
	
	-- 解析generalid
	self:parseGeneralId()
	self:OnCreateCreature()
	self:OnTaskStart()
	
	local timestamp = os.time() + tb_doujiantai_base[ 1 ].cd
	self:SetMapStartTime(timestamp)
end

function InstanceDoujiantai:parseGeneralId()
	local generalId	= self:GetMapGeneralId()
	
	local params = string.split(generalId, '|')
	local myRank 	= tonumber(params[ 3 ])
	local otherRank = tonumber(params[ 4 ])
	local history	= tonumber(params[ 5 ])
	local curr		= tonumber(params[ 6 ])
	
	self:SetMyRank(myRank)
	self:SetOtherRank(otherRank)
	self:SetHistory(history)
	self:SetCurrWins(curr)
end


function InstanceDoujiantai:SetMyRank(val)
	self:SetUInt16(DOUJIANTAI_FIELDS_INT_RANK, 0, val)
end
function InstanceDoujiantai:SetOtherRank(val)
	self:SetUInt16(DOUJIANTAI_FIELDS_INT_RANK, 1, val)
end

function InstanceDoujiantai:GetOtherRank()
	return self:GetUInt16(DOUJIANTAI_FIELDS_INT_RANK, 1)
end

function InstanceDoujiantai:SetHistory(val)
	self:SetUInt16(DOUJIANTAI_FIELDS_INT_COMBAT_WIN_INFO, 0, val)
end

function InstanceDoujiantai:SetCurrWins(val)
	self:SetUInt16(DOUJIANTAI_FIELDS_INT_COMBAT_WIN_INFO, 1, val)
end

function InstanceDoujiantai:GetHistory()
	return self:GetUInt16(DOUJIANTAI_FIELDS_INT_COMBAT_WIN_INFO, 0)
end

function InstanceDoujiantai:GetCurrWins()
	return self:GetUInt16(DOUJIANTAI_FIELDS_INT_COMBAT_WIN_INFO, 1)
end

function InstanceDoujiantai:OnCreateCreature()
	local rank = self:GetOtherRank()
	local config = globalCounter:GetRankInfo(rank)
	
	local enemy = nil
	if string.len(config.guid) > 0 then
		enemy = mapLib.GetPlayerByPlayerGuid(self.ptr, config.guid)
	end
	
	-- 玩家在线
	if enemy then
		local enemyInfo = UnitInfo:new {ptr = enemy}
		config = enemyInfo:GetDummyInfo()
	end
	
	local x = tb_doujiantai_base[ 1 ].bornPos[ 2 ][ 1 ]
	local y = tb_doujiantai_base[ 1 ].bornPos[ 2 ][ 2 ]
	local image = self:GetImageInfo(config)
	self:AddImageBody(image, x, y, "AI_Image", nil, nil, nil)
	
end

-- 活动正式开始
function InstanceDoujiantai:OnTaskStart()
	-- print("==================OnTaskStart====", tb_doujiantai_base[ 1 ].last + tb_doujiantai_base[ 1 ].cd)
	local timestamp = os.time() +  tb_doujiantai_base[ 1 ].last + tb_doujiantai_base[ 1 ].cd
	-- print("=========", timestamp)
	-- 加任务时间
	self:SetMapQuestEndTime(timestamp)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp)
end

-- 副本失败退出
function InstanceDoujiantai:instanceFail()
	self:SetMapState(self.STATE_FAIL)
	-- 获得player
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	local player = allPlayers[ 1 ]
	if player then
		self:OnFightResult(player, GlobalCounter.LOSE)
	end
	return false
end

--当副本状态发生变化时间触发
function InstanceDoujiantai:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		
		--10s后结束副本
		local timestamp = os.time() + InstanceDoujiantai.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
		
		local allPlayers = mapLib.GetAllPlayer(self.ptr)
		local player = allPlayers[ 1 ]
		if player then
			local playerInfo = UnitInfo:new {ptr = player}
			local dict = {}
			for _, info in pairs(tb_doujiantai_base[ 1 ].tryReward) do
				MergeSameTable(dict, info[ 1 ], info[ 2 ])
			end
			if tostate == self.STATE_FINISH then
				if self:GetCurrWins() + 1 > self:GetHistory() then
					local config = tb_doujiantai_combat_win[self:GetCurrWins()+1]
					if config then
						for _, info in pairs(config.reward) do
							MergeSameTable(dict, info[ 1 ], info[ 2 ])
						end
					end
				end
			end
			local list = Change_To_Item_Reward_Info(dict, true)
			
			playerInfo:call_send_instance_result(self:GetMapState(), self.exit_time, list, INSTANCE_SUB_TYPE_DOUJIANTAI, '')
		end
	end
end

--玩家加入地图
function InstanceDoujiantai:OnJoinPlayer(player)
	InstanceInstBase.OnJoinPlayer(self, player)
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() or self:GetMapState() ~= self.STATE_START then
		--死亡了还进来，直接弹出去
		mapLib.ExitInstance(self.ptr, player)
		self:SetMapState(self.STATE_FAIL)
		return
	end
	
	local maxHealth = binLogLib.GetUInt32(player, UNIT_FIELD_MAX_HEALTH)
	unitLib.SetHealth(player, maxHealth)
end

--当玩家离开时触发
function InstanceDoujiantai:OnLeavePlayer( player, is_offline)
	InstanceInstBase.OnLeavePlayer(self, player, is_offline)
	self:RemoveTimeOutCallback(self.Leave_Callback)
	if self:GetMapState() == self.STATE_START then
		self:OnFightResult(player, GlobalCounter.LOSE)
		self:SetMapState(self.STATE_FAIL)
	end
end

--当玩家死亡后触发()
function InstanceDoujiantai:OnPlayerDeath(player)
	-- AI win	
	self:OnFightResult(player, GlobalCounter.LOSE)
	self:SetMapState(self.STATE_FAIL)
end

-- 处理结果
function InstanceDoujiantai:OnFightResult(player, result)
	self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
	playerLib.SendToAppdDoSomething(player, SCENED_APPD_DOUJIANTAI, self:GetOtherRank(), ''..result)
end


-------------------------仙府夺宝boss---------------------------
AI_Image = class("AI_Image", AI_Base)
AI_Image.ainame = "AI_Image"

--死亡
function AI_Image:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)

	local instanceInfo = InstanceDoujiantai:new{ptr = map_ptr}
	
	instanceInfo:OnFightResult(killer_ptr, GlobalCounter.WIN)
	instanceInfo:SetMapState(instanceInfo.STATE_FINISH)
	
	return 0
end


return InstanceDoujiantai