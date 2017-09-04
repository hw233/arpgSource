InstanceQualify = class("InstanceQualify", InstanceInstBase)

InstanceQualify.Name = "InstanceQualify"
InstanceQualify.exit_time = 10
InstanceQualify.broadcast_nogrid = 1
InstanceQualify.AI_ALIAS_NAME = 'qualifyAI'


function InstanceQualify:ctor(  )
	
end

--初始化脚本函数
function InstanceQualify:OnInitScript()
	-- 不让重复初始化
	if self:isInstanceInit() then
		return
	end
	self:SetInstanceInited()

	InstanceInstBase.OnInitScript(self) --调用基类
	
	-- 解析generalid
	self:parseGeneralId()
	self:OnTaskStart()
end

function InstanceQualify:parseGeneralId()
	local generalId	= self:GetMapGeneralId()
	local params = string.split(generalId, '|')
	-- 没有匹配到人
	if #params == 3 then
		local id = tonumber(params[ 3 ])
		self:OnCreateCreature(id)
	end
end

function InstanceQualify:OnCreateCreature(id)
	local range = tb_single_pvp_grade[id].robots
	local robotId = randInt(range[ 1 ], range[ 2 ])
	local config = tb_single_pvp_robot[robotId]
	
	local x = tb_single_pvp_base[ 1 ].enterPos[ 2 ][ 1 ]
	local y = tb_single_pvp_base[ 1 ].enterPos[ 2 ][ 2 ]
	local image = self:GetImageInfo(config)
	self:AddImageBody(image, x, y, "AI_Qualify", nil, nil, nil, self.AI_ALIAS_NAME)	
end

-- 活动正式开始
function InstanceQualify:OnTaskStart()
	local timestamp = os.time() + tb_single_pvp_base[ 1 ].startcd
	self:SetMapStartTime(timestamp)
	-- 加任务时间
	self:SetMapQuestEndTime(timestamp + tb_single_pvp_base[ 1 ].time)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp + tb_single_pvp_base[ 1 ].time)
end

-- 副本失败退出
function InstanceQualify:instanceFail()
	self:SetMapState(self.STATE_FAIL)
	-- 按照血量百分比来进行判断胜利与否
	-- 相同血量百分比都输
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	if #allPlayers == 0 then
		return
	end
	
	local playerA = allPlayers[ 1 ]
	local playerB
	if #allPlayers == 2 then
		playerB = allPlayers[ 2 ]
	else
		playerB = mapLib.AliasCreature(self.ptr, self.AI_ALIAS_NAME)
	end
	
	sa, sb = self:sortUnitObject(playerA, playerB)
	-- 血量
	if not sa then
		self:OnFightResult(playerA, GlobalCounter.LOSE, GetUnitName(playerB))
		self:OnFightResult(playerB, GlobalCounter.LOSE, GetUnitName(playerA))
	else
		self:OnFightResult(sa     , GlobalCounter.WIN , GetUnitName(sb))
		self:OnFightResult(sb     , GlobalCounter.LOSE, GetUnitName(sa))
	end
	
	return false
end

-- 按照血量百分比优先级进行判断, 相同的
function InstanceQualify:sortUnitObject(unitA, unitB)
	local unitInfoA = UnitInfo:new {ptr = unitA}
	local unitInfoB = UnitInfo:new {ptr = unitB}
	
	local currA		= unitInfoA:GetHealth()
	local currAMax	= unitInfoA:GetMaxHealth()
	local currB		= unitInfoB:GetHealth()
	local currBMax	= unitInfoB:GetMaxHealth()
	if currA * currBMax == currB * currAMax then
		return
	end
	
	if currA * currBMax > currB * currAMax then
		return unitA, unitB
	end
	
	return unitB, unitA
end

--当副本状态发生变化时间触发
function InstanceQualify:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		
		--10s后结束副本
		local timestamp = os.time() + InstanceQualify.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
	end
end

--玩家加入地图
function InstanceQualify:OnJoinPlayer(player)
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
	playerInfo:SetToGroupMode(playerInfo:GetPlayerGuid())
end

--当玩家离开时触发
function InstanceQualify:OnLeavePlayer( player, is_offline)
	InstanceInstBase.OnLeavePlayer(self, player, is_offline)
	-- 血回满
	local maxHealth = binLogLib.GetUInt32(player, UNIT_FIELD_MAX_HEALTH)
	unitLib.SetHealth(player, maxHealth)
	
	-- 正常退出的不算
	if self:GetMapState() ~= self.STATE_START then
		return
	end
	
	self:SetMapState(self.STATE_FAIL)
	local enemy = self:GetEnemy(player)
	
	-- 退出玩家输了
	self:OnFightResult(player, GlobalCounter.LOSE, GetUnitName(enemy))

	-- 另外一个玩家获胜
	self:OnFightResult(enemy , GlobalCounter.WIN , GetUnitName(player))
end

function InstanceQualify:GetEnemy(player)
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	for _, exitPlayer in ipairs(allPlayers) do
		if player ~= exitPlayer then
			return exitPlayer
		end
	end
	
	return mapLib.AliasCreature(self.ptr, self.AI_ALIAS_NAME)
end

--当玩家被玩家杀掉时触发
function InstanceQualify:OnPlayerKilled(player, killer)
	self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
	self:SetMapState(self.STATE_FINISH)
	self:OnFightResult(killer, GlobalCounter.WIN , GetUnitName(player))
	self:OnFightResult(player, GlobalCounter.LOSE, GetUnitName(killer))
end

-- 当玩家被怪物杀死
function InstanceQualify:OnPlayerKilledByMonster(player, killer)
	self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
	self:SetMapState(self.STATE_FINISH)
	self:OnFightResult(player, GlobalCounter.LOSE, GetUnitName(killer))
end

function InstanceQualify:OnPlayerWin(player, fightName)
	self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
	self:SetMapState(self.STATE_FINISH)
	self:OnFightResult(player, GlobalCounter.WIN, fightName)
end

-- 处理结果
function InstanceQualify:OnFightResult(player, result, fightName)
	-- 不是玩家的不处理结果
	if GetUnitTypeID(player) ~= TYPEID_PLAYER then
		return
	end
	
	local dict = {}
	local playerInfo = UnitInfo:new {ptr = player}
	local indx = playerInfo:unitCalcQueueIndx()
	local gradeConfig = tb_single_pvp_grade[indx]
	local modfiyScore = 0
	local instanceRet
	if result == GlobalCounter.WIN then
		instanceRet = self.STATE_FINISH
		for i = 1, #gradeConfig.itemWinKeys do
			local entry = gradeConfig.itemWinKeys[ i ]
			local count = gradeConfig.itemWinValues[ i ]
			dict[entry] = count
		end
		modfiyScore = gradeConfig.win
	else
		instanceRet = self.STATE_FAIL
		for i = 1, #gradeConfig.itemLoseKeys do
			local entry = gradeConfig.itemLoseKeys[ i ]
			local count = gradeConfig.itemLoseValues[ i ]
			dict[entry] = count
		end
		modfiyScore = -gradeConfig.lose
	end
	
	-- 获得奖励
	PlayerAddRewards(player, dict, MONEY_CHANGE_SINGLE_PVP, LOG_ITEM_OPER_TYPE_SINGLE_PVP)
	-- 扫荡的结果发送
	local list = Change_To_Item_Reward_Info(dict, true)
	
	local score = playerInfo:unitGetQualifyScore()
	
	playerInfo:call_send_instance_result(instanceRet, self.exit_time, list, INSTANCE_SUB_TYPE_QUALIFY, string.format('%d|%d', modfiyScore, score+modfiyScore))
	-- 通知应用服结果
	playerLib.SendToAppdDoSomething(player, SCENED_APPD_SINGLE_PVP_RESULT, result, fightName)
end

AI_Qualify = class("AI_Qualify", AI_Base)
AI_Qualify.ainame = "AI_Qualify"

--死亡
function AI_Qualify:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)

	local instanceInfo = InstanceQualify:new{ptr = map_ptr}
	if GetUnitTypeID(killer_ptr) == TYPEID_UNIT then
		killer_ptr = creatureLib.GetMonsterHost(killer_ptr)
	end
	instanceInfo:OnPlayerWin(killer_ptr, GetUnitName(owner))
	
	return 0
end

return InstanceQualify