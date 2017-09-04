InstanceKuafu3v3 = class("InstanceKuafu3v3", InstanceInstBase)

InstanceKuafu3v3.Name = "InstanceKuafu3v3"
InstanceKuafu3v3.exit_time = 30
InstanceKuafu3v3.Time_Out_Fail_Callback = "timeoutCallback"
-- 全图可见
InstanceKuafu3v3.broadcast_nogrid = 1

function InstanceKuafu3v3:ctor(  )
	
end

--初始化脚本函数
function InstanceKuafu3v3:OnInitScript()
	InstanceInstBase.OnInitScript(self) --调用基类
	
	-- 先给它一个初始的结束时间, 为了保证过程中不被释放掉
	-- self:SetMapEndTime(os.time() + 600)
	
	self:OnTaskStart()
	self:AddCountDown()
end

function InstanceKuafu3v3:AddCountDown()
	local timestamp = os.time() + tb_kuafu3v3_base[ 1 ].cd
	self:SetMapStartTime(timestamp)
	self:AddTimeOutCallback("countdown", timestamp)
end

-- 倒计时结束
function InstanceKuafu3v3:countdown()
	-- 符点刷新定时器
	mapLib.AddTimer(self.ptr, 'OnBuffRefresh', tb_kuafu3v3_base[ 1 ].interval * 1000)
end

-- 活动正式开始
function InstanceKuafu3v3:OnTaskStart()
	local timestamp = os.time() +  tb_kuafu3v3_base[ 1 ].last + tb_kuafu3v3_base[ 1 ].cd
	-- 加任务时间
	self:SetMapQuestEndTime(timestamp)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp)
end

-- 刷新buff
function InstanceKuafu3v3:OnBuffRefresh()
	-- 处理刷新次数
	local times = self:GetMapReserveValue1()
	if times < tb_kuafu3v3_base[ 1 ].times then
		self:SetMapReserveValue1(times+1)
	end
	
	-- 刷新把原来的先删掉
	local allGameObjects = mapLib.GetAllGameObject(self.ptr)
	for _, gameobject in pairs(allGameObjects) do
		mapLib.RemoveWorldObject(self.ptr, gameobject)
	end
	
	-- 刷新新的
	local indice = GetRandomIndexTable(#tb_buff_kuafu3v3, #tb_kuafu3v3_base[ 1 ].buffPos)
	for i = 1, #indice do
		local pos = tb_kuafu3v3_base[ 1 ].buffPos[ i ]
		local indx = indice[ i ]
		local entry = tb_buff_kuafu3v3[indx].gameobject_id
		mapLib.AddGameObject(self.ptr, entry, pos[ 1 ], pos[ 2 ], GO_GEAR_STATUS_END)
	end
	
	app:CallOptResult(self.ptr, OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_BUFF_OCCUR)
	
	return self:GetMapReserveValue1() < tb_kuafu3v3_base[ 1 ].times
end

-- 副本失败退出
function InstanceKuafu3v3:timeoutCallback()
	self:SyncResultToWeb()
	self:SetMapState(self.STATE_FINISH)
	return false
end

--当副本状态发生变化时间触发
function InstanceKuafu3v3:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		
		--10s后结束副本
		local timestamp = os.time() + InstanceKuafu3v3.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
	end
end

-- 判断是否能退出副本
function InstanceKuafu3v3:DoPlayerExitInstance(player)
	-- 直接回原服
	local playerInfo = UnitInfo:new {ptr = player}
	local login_fd = serverConnList:getLogindFD()
	call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
	return 0	--返回1的话为正常退出，返回0则不让退出
end

--玩家加入地图
function InstanceKuafu3v3:OnJoinPlayer(player)
	
	InstanceInstBase.OnJoinPlayer(self, player)
	
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		local login_fd = serverConnList:getLogindFD()
		call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
		return
	end
	
	-- 不能重复进入
	if self:findIndexByName(playerInfo:GetName()) > -1 then
		local login_fd = serverConnList:getLogindFD()
		call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
		return
	end
	
	-- 设置名称
	local emptyIndex = self:findIndexByName()
	if emptyIndex > -1 then
		local strStart = KUAFU_3V3_FIELDS_STR_INFO_START + emptyIndex * MAX_KUAFU_3V3_STR_COUNT
		local intStart = KUAFU_3V3_FIELDS_INT_INFO_START + emptyIndex * MAX_KUAFU_3V3_INT_COUNT
		
		self:SetStr(strStart + KUAFU_3V3_PLAYER_NAME, playerInfo:GetName())
		self:SetStr(strStart + KUAFU_3V3_PLAYER_GUID, playerInfo:GetPlayerGuid())
		
		self:SetByte(intStart + KUAFU_3V3_PLAYER_SHOW_INFO, 0, playerInfo:GetGender())
		self:SetByte(intStart + KUAFU_3V3_PLAYER_SHOW_INFO, 1, 100)
		self:SetUInt16(intStart + KUAFU_3V3_PLAYER_SHOW_INFO, 1, playerInfo:GetLevel())
		self:SetByte(intStart + KUAFU_3V3_PLAYER_SETTLEMENT, 2, playerInfo:GetVirtualCamp())
		playerInfo:SetToGroupMode(''..playerInfo:GetVirtualCamp())
	end
end

-- 获得名字对应的位置, ''表示用来查询空位位置
function InstanceKuafu3v3:findIndexByName(name)
	name = name or ''
	local start = KUAFU_3V3_FIELDS_STR_INFO_START
	for i = 0, MAX_KUAFU_3V3_COUNT-1 do
		if self:GetStr(start+KUAFU_3V3_PLAYER_NAME) == name then
			return i
		end
		start = start + MAX_KUAFU_3V3_STR_COUNT
	end
	
	return -1
end

-- 是否某方阵营全部死亡
function InstanceKuafu3v3:IsOneCmapAllDead()
	return self:GetWinCmap() > 0
end

-- 获得胜利方阵营
function InstanceKuafu3v3:GetWinCmap()
	local ret = {}
	
	local tmp = {}
	local start = KUAFU_3V3_FIELDS_INT_INFO_START
	for i = 0, MAX_KUAFU_3V3_COUNT-1 do
		if self:GetByte(start + KUAFU_3V3_PLAYER_SETTLEMENT, 1) == 0 then
			local camp = self:GetByte(start + KUAFU_3V3_PLAYER_SETTLEMENT, 2)
			if camp > 0 and not tmp[camp] then
				tmp[camp] = 1
				table.insert(ret, camp)
			end
		end
		start = start + MAX_KUAFU_3V3_INT_COUNT
	end
	
	-- 平局
	if #ret == 2 or #ret == 0 then
		return 0
	end
	-- 胜利
	return ret[ 1 ]
end

--当玩家死亡后触发()
function InstanceKuafu3v3:OnPlayerDeath(player)
	-- 如果状态已经改变, 即使死了也不再更新时间
	if self:GetMapState() ~= self.STATE_START then
		return
	end
	
	-- 设置死亡状态
	local playerInfo = UnitInfo:new{ptr = player}
	local indx = self:findIndexByName(playerInfo:GetName())
	local intStart = KUAFU_3V3_FIELDS_INT_INFO_START + indx * MAX_KUAFU_3V3_INT_COUNT
	self:SetByte(intStart + KUAFU_3V3_PLAYER_SETTLEMENT, 1, 1)
	
	-- 判断某方阵营全部死亡
	if self:IsOneCmapAllDead() then
		self:SyncResultToWeb()
		self:SetMapState(self.STATE_FINISH)
	end
end

-- 同步数据到场景服
function InstanceKuafu3v3:SyncResultToWeb()
	local sendInfo = {}
	
	-- 获胜方阵营 (0:表示平局)
	local win_camp = self:GetWinCmap()
	-- 全场最佳的下标
	local bestIndx = self:GetTheBest()
	-- 双方阵营的排名
	local topThree = {self:GetTopThree(1), self:GetTopThree(2)}
	
	-- 伤害对应的积分
	local damageScoreConfig = tb_kuafu3v3_base[ 1 ].damageScore
	
	local info = {}
	local intstart = KUAFU_3V3_FIELDS_INT_INFO_START
	local strstart = KUAFU_3V3_FIELDS_STR_INFO_START
	for i = 0, MAX_KUAFU_3V3_COUNT-1 do
		local camp = self:GetByte(intstart + KUAFU_3V3_PLAYER_SETTLEMENT, 2)
		
		-- 取得胜负积分
		local score = tb_kuafu3v3_base[ 1 ].resultScore[ 2 ]
		if camp == win_camp then
			score = tb_kuafu3v3_base[ 1 ].resultScore[ 1 ]
		end
		
		-- 击杀积分
		local killed = self:GetByte(intstart + KUAFU_3V3_PLAYER_SETTLEMENT, 0)
		local scorePerKill = tb_kuafu3v3_base[ 1 ].scorePerKill
		score = score + killed * scorePerKill
		
		-- 伤害积分
		local config = topThree[camp]
		if config and config[ i ] then
			score = score + damageScoreConfig[config[ i ]]
		end
		
		-- 连胜积分
		local player_guid = self:GetStr(strstart + KUAFU_3V3_PLAYER_GUID)
		local player = mapLib.GetPlayerByPlayerGuid(self.ptr, player_guid)
		if player then
			local playerInfo = UnitInfo:new {ptr = player}
			local prev = playerInfo:GetKuafu3v3TrendInfo()
			if prev > 0 and camp == win_camp then
				score = score + tb_kuafu3v3_base[ 1 ].winStreakScore
			end
		end
		
		-- 是否全场最佳
		if i == bestIndx then
			self:SetByte(intstart + KUAFU_3V3_PLAYER_HONOR, 1, 1)
			score = score + tb_kuafu3v3_base[ 1 ].bestScore
		end
		
		-- 设置荣誉
		local honor = tb_kuafu3v3_base[ 1 ].resultHonor[ 2 ]
		if camp == win_camp then
			honor = tb_kuafu3v3_base[ 1 ].resultHonor[ 1 ]
		end
		self:SetByte(intstart + KUAFU_3V3_PLAYER_HONOR, 0, honor)
		
		-- 设置积分
		self:SetByte(intstart + KUAFU_3V3_PLAYER_SETTLEMENT, 3, score)
		
		local result = 0
		-- 为平局
		if win_camp > 0 then
			if camp == win_camp then
				result = 1
			else
				result = -1
			end
		end
		
		-- 如果玩家存在
		if player_guid and string.len(player_guid) > 0 then
			-- result(0:平局, 1:胜利, -1:失败)
			table.insert(sendInfo, {player_guid, score, honor, result})
		end
		
		intstart = intstart + MAX_KUAFU_3V3_INT_COUNT
		strstart = strstart + MAX_KUAFU_3V3_STR_COUNT
	end
	
	local retInfo = {}
	-- 然后同步到web
	for _, info in pairs(sendInfo) do
		table.insert(retInfo, string.join(",", info))
	end
	
	
	local url = globalGameConfig:GetExtWebInterface().."world_3v3/match_result"
	local data = {}
	-- 这里获得3v3队伍匹配的信息
	data.ret = string.join(";", retInfo)
	data.open_time = 1
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		outFmtDebug("response = %s", tostring(response))
	end)
end

-- 通过阵营获得输出前3的玩家
function InstanceKuafu3v3:GetTopThree(win_camp)
	win_camp = win_camp or 0
	
	local info = {}
	local intstart = KUAFU_3V3_FIELDS_INT_INFO_START
	for i = 0, MAX_KUAFU_3V3_COUNT-1 do
		local damage = self:GetDouble(intstart + KUAFU_3V3_PLAYER_DAMAGE)
		local camp = self:GetByte(intstart + KUAFU_3V3_PLAYER_SETTLEMENT, 2)
		if win_camp == 0 or win_camp == camp then
			table.insert(info, {indx = i, damage=damage})
		end

		intstart = intstart + MAX_KUAFU_3V3_INT_COUNT
	end
	
	table.sort(info, function(a, b)
		return a.damage > b.damage
	end)
	
	-- 下标对应的排名
	local ret = {}
	-- 伤害为0的不算
	for i = 1, #info do
		if info[ i ].damage == 0 then
			break
		end
		ret[info[ i ].indx] = i
	end
	
	return ret
end

-- 获得全场最佳
function InstanceKuafu3v3:GetTheBest(win_camp)
	win_camp = win_camp or 0
	local info = {}
	local intstart = KUAFU_3V3_FIELDS_INT_INFO_START
	local strstart = KUAFU_3V3_FIELDS_STR_INFO_START
	for i = 0, MAX_KUAFU_3V3_COUNT-1 do
		local killed = self:GetByte(intstart + KUAFU_3V3_PLAYER_SETTLEMENT, 0)
		local damage = self:GetDouble(intstart + KUAFU_3V3_PLAYER_DAMAGE)
		local player_guid = self:GetStr(strstart + KUAFU_3V3_PLAYER_GUID)
		local camp = self:GetByte(intstart + KUAFU_3V3_PLAYER_SETTLEMENT, 2)
		local player = mapLib.GetPlayerByPlayerGuid(self.ptr, player_guid)
		local force = 1000000000000
		if player then
			local playerInfo = UnitInfo:new {ptr = player}
			force = playerInfo:GetForce()
		end
		if win_camp == 0 or win_camp == camp then
			table.insert(info, {indx = i, killed=killed, damage=damage, force=force})
		end

		intstart = intstart + MAX_KUAFU_3V3_INT_COUNT
		strstart = strstart + MAX_KUAFU_3V3_STR_COUNT
	end
	
	table.sort(info, function(a, b)
		if a.killed == b.killed then
			if a.damage == b.damage then
				return a.force < b.force
			end
			return a.damage > b.damage
		end
		return a.killed > b.killed
	end)
	
	return info[ 1 ].indx
end

-- 复活后
function InstanceKuafu3v3:DoAfterRespawn(unit_ptr)
	local unitInfo = UnitInfo:new{ptr = unit_ptr}
	-- 加无敌隐身buff
	unitLib.AddBuff(unit_ptr, BUFF_INVINCIBLE, unit_ptr, 1, MAX_BUFF_DURATION)
	unitLib.AddBuff(unit_ptr, BUFF_INVISIBLE, unit_ptr, 1, MAX_BUFF_DURATION)
end

-- 玩家受到实际伤害(负数表示加血)
function InstanceKuafu3v3:OnPlayerHurt(killer, player, damage)
	local killerInfo = UnitInfo:new{ptr = killer}
	local targetInfo = UnitInfo:new{ptr = player}
	
	-- 计算玩家血量
	local indx = self:findIndexByName(targetInfo:GetName())
	local rate = math.floor((targetInfo:GetHealth() - damage) * 100 / targetInfo:GetMaxHealth())
	local intStart = KUAFU_3V3_FIELDS_INT_INFO_START + indx * MAX_KUAFU_3V3_INT_COUNT
	rate = math.min(rate, 100)
	self:SetByte(intStart + KUAFU_3V3_PLAYER_SHOW_INFO, 1, rate)
	-- 计算玩家伤害
	if damage > 0 then
		indx = self:findIndexByName(killerInfo:GetName())
		intStart = KUAFU_3V3_FIELDS_INT_INFO_START + indx * MAX_KUAFU_3V3_INT_COUNT
		self:AddDouble(intStart + KUAFU_3V3_PLAYER_DAMAGE, damage)
	end
	
	return 0
end

function InstanceKuafu3v3:OnPlayerKilled(player, killer)
	local killerInfo = UnitInfo:new{ptr = killer}
	local playerInfo = UnitInfo:new{ptr = player}
	
	-- 击杀者加击杀数
	local indx1 = self:findIndexByName(killerInfo:GetName())
	local intStart = KUAFU_3V3_FIELDS_INT_INFO_START + indx1 * MAX_KUAFU_3V3_INT_COUNT
	self:AddByte(intStart + KUAFU_3V3_PLAYER_SETTLEMENT, 0, 1)
	
	-- 被杀者下标
	local indx2 = self:findIndexByName(playerInfo:GetName())
	
	-- 通知所有人击杀信息
	local strstart = KUAFU_3V3_FIELDS_STR_INFO_START
	for i = 0, MAX_KUAFU_3V3_COUNT-1 do
		local player_guid = self:GetStr(strstart + KUAFU_3V3_PLAYER_GUID)
		local player = mapLib.GetPlayerByPlayerGuid(self.ptr, player_guid)
		if player then
			local playerInfo = UnitInfo:new {ptr = player}
			playerInfo:call_kuafu_3v3_kill_detail(indx1, indx2)
		end
		
		strstart = strstart + MAX_KUAFU_3V3_STR_COUNT
	end
	
	-- self:SetUInt16(KUAFU_3V3_FIELDS_INT_LAST_KILL, 0, indx1)
	-- self:SetUInt16(KUAFU_3V3_FIELDS_INT_LAST_KILL, 1, indx2)
	
	return 0
end

--当玩家离开时触发
function InstanceKuafu3v3:OnLeavePlayer( player, is_offline)
	-- 活动副本结束了就不进行处理
	if self:GetMapState() == self.STATE_FINISH then
		return
	end
	-- 设置死亡状态
	local playerInfo = UnitInfo:new{ptr = player}
	local indx = self:findIndexByName(playerInfo:GetName())
	local intStart = KUAFU_3V3_FIELDS_INT_INFO_START + indx * MAX_KUAFU_3V3_INT_COUNT
	if self:GetByte(intStart + KUAFU_3V3_PLAYER_SETTLEMENT, 1) == 0 then
		self:SetByte(intStart + KUAFU_3V3_PLAYER_SETTLEMENT, 1, 1)
	end
	
	-- 判断某方阵营全部死亡
	if self:IsOneCmapAllDead() then
		self:SyncResultToWeb()
		self:SetMapState(self.STATE_FINISH)
	end
end

--使用游戏对象之前
--返回1的话就继续使用游戏对象，返回0的话就不使用
function InstanceKuafu3v3:OnBeforeUseGameObject(user, go, go_entryid, posX, posY)
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
function InstanceKuafu3v3:OnUseGameObject(user, go, go_entryid, posX, posY)
	-- 判断对应的是那种buff
	for _, obj in ipairs(tb_buff_kuafu3v3) do
		if obj.gameobject_id == go_entryid then
			local effects = obj.type_effect
			for _, effect in pairs(effects) do
				local buffId = effect[ 1 ]
				local lv = effect[ 2 ]
				local duration = tb_buff_template[buffId].duration
				--SpelladdBuff(user, buffId, user, lv, duration)
			end
			break
		end
	end
	
	-- 需要删除对象
	mapLib.RemoveWorldObject(self.ptr, go)
	
	return 1	
end


-- 地图需要清空人时要做的事
function InstanceKuafu3v3:IsNeedTeleportWhileMapClear(player)
	local playerInfo = UnitInfo:new {ptr = player}
	local login_fd = serverConnList:getLogindFD()
	call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
	return 0
end

return InstanceKuafu3v3