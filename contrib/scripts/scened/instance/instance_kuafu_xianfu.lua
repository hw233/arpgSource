local security = require("base/Security")

InstanceKuafuXianfu = class("InstanceKuafuXianfu", InstanceInstBase)

InstanceKuafuXianfu.Name = "InstanceKuafuXianfu"
InstanceKuafuXianfu.exit_time = 20
InstanceKuafuXianfu.Time_Out_Fail_Callback = "timeoutCallback"
--InstanceKuafuXianfu.broadcast_nogrid = 1
InstanceKuafuXianfu.sub = "world_xianfu"

InstanceKuafuXianfu.BOX_EXIST_TIME = 1800

function InstanceKuafuXianfu:ctor(  )
	
end

--初始化脚本函数
function InstanceKuafuXianfu:OnInitScript()
	InstanceInstBase.OnInitScript(self) --调用基类
	
	self:OnTaskStart()
	self:AddCountDown()
	
	-- boss刷新定时器
	mapLib.DelTimer(self.ptr, 'OnMonsterRefresh')
	mapLib.AddTimer(self.ptr, 'OnMonsterRefresh', 1000)
	
	self:OnInitBossInfo()
	
	-- 解析generalid
	self:parseGeneralId()
end

function InstanceKuafuXianfu:parseGeneralId()
	local generalId	= self:GetMapGeneralId()
	local params = string.split(generalId, '|')
	local hard = tonumber(params[ 2 ])
	self:SetHard(hard)
end

function InstanceKuafuXianfu:SetHard(hard)
	self:SetUInt32(KUAFU_XIANFU_FIELDS_HARD, hard)
end


function InstanceKuafuXianfu:GetHard()
	return self:GetUInt32(KUAFU_XIANFU_FIELDS_HARD)
end

function InstanceKuafuXianfu:GetBoxid()
	local hard = self:GetHard()
	return tb_kuafu_xianfu_condition[hard].boxid
end

-- 初始化boss数据
function InstanceKuafuXianfu:OnInitBossInfo ()
	local timestamp = os.time()	
	local intstart = KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START
	for indx, bossInfo in ipairs(tb_kuafu_xianfu_boss) do
		local info = bossInfo.bossTime
		local createBossTime = info[ 1 ] * 60 + info[ 2 ]
		local bornTime = self:GetMapCreateTime() + createBossTime
		local entry = bossInfo.bossEntry
		
		self:SetUInt16(intstart + KUAFU_XIANFU_BOSS_SHOW_INFO, 0, entry)
		self:SetUInt32(intstart + KUAFU_XIANFU_BOSS_BORN_TIME, bornTime)
		
		intstart = intstart + MAX_KUAFU_XIANFU_BOSS_INT_COUNT
	end
end

-- 
function InstanceKuafuXianfu:GetBinlogIndxByEntry (entry)
	
	local intstart = KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START
	for i = 1, MAX_KUAFU_XIANFU_BOSS_COUNT do
		if entry == self:GetUInt16(intstart + KUAFU_XIANFU_BOSS_SHOW_INFO, 0) then
			return i
		end
		intstart = intstart + MAX_KUAFU_XIANFU_BOSS_INT_COUNT
	end
	
	return 0
end

-- 显示小地图信息
function InstanceKuafuXianfu:SendMiniMapInfo()
	
	local playerPos = {}
	local allPlayers = mapLib.GetAllPlayer(self.ptr)
	for _, player in pairs(allPlayers) do
		local x, y = unitLib.GetPos(player)
		local guid = binLogLib.GetStr(player, BINLOG_STRING_FIELD_GUID)
		table.insert(playerPos, {guid, x, y})
	end
	
	local creaturePos = {}
	local allCreatures = mapLib.GetAllCreature(self.ptr)
	for _, creature in pairs(allCreatures) do
		local x, y = unitLib.GetPos(creature)
		local entry = binLogLib.GetUInt16(creature, UNIT_FIELD_UINT16_0, 0)
		table.insert(creaturePos, {entry, x, y})
	end
	
	local gameObjectPos = {}
	local allGameobjects = mapLib.GetAllGameObject(self.ptr)
	for _, gameObject in pairs(allGameobjects) do
		local x, y = unitLib.GetPos(gameObject)
		local entry = binLogLib.GetUInt16(gameObject, UNIT_FIELD_UINT16_0, 0)
		table.insert(gameObjectPos, {entry, x, y})
	end
	
	for _, player in pairs(allPlayers) do
		local playerInfo = UnitInfo:new {ptr = player}
		playerInfo:call_kuafu_xianfu_minimap_info_in_custom(playerPos, creaturePos, gameObjectPos)
	end
end

function InstanceKuafuXianfu:AddCountDown()
	local timestamp = os.time() + tb_kuafu_xianfu_base[ 1 ].cd
	self:SetMapStartTime(timestamp)
	self:AddTimeOutCallback("countdown", timestamp)
end

-- 倒计时结束
function InstanceKuafuXianfu:countdown()
	-- 符点刷新定时器
	mapLib.AddTimer(self.ptr, 'OnBuffRefresh', tb_kuafu_xianfu_base[ 1 ].interval * 1000)
end

-- 活动正式开始
function InstanceKuafuXianfu:OnTaskStart()
	local timestamp = os.time() +  tb_kuafu_xianfu_base[ 1 ].last + tb_kuafu_xianfu_base[ 1 ].cd
	-- 加任务时间
	self:SetMapQuestEndTime(timestamp)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp)
end

-- 刷新boss
function InstanceKuafuXianfu:OnMonsterRefresh()
	if self:GetMapState() == self.STATE_FINISH then
		return false
	end
	
	local timestamp = os.time()
	for indx, bossInfo in pairs(tb_kuafu_xianfu_boss) do
		local info = bossInfo.bossTime
		local createBossTime = info[ 1 ] * 60 + info[ 2 ]
		
		if self:GetMapCreateTime() + createBossTime <= timestamp then
			local bossintstart = KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START + MAX_KUAFU_XIANFU_BOSS_INT_COUNT * (indx - 1)
			local activeInfo = self:GetUInt32(bossintstart + KUAFU_XIANFU_BOSS_BORN_INFO)
			if activeInfo == 0 then
				self:SetUInt32(bossintstart + KUAFU_XIANFU_BOSS_BORN_INFO, 1)
				outFmtDebug("created indx = %d",  indx)
				local pos = bossInfo.bossPos
				local entry = bossInfo.bossEntry
				local bossName = tb_creature_template[entry].name
				local creature = mapLib.AddCreature(self.ptr, {
						templateid = entry, x = pos[ 1 ], y = pos[ 2 ], 
						active_grid = true, alias_name = bossName, ainame = "AI_XianfuBoss", npcflag = {}
					}
				)
				-- 标识为boss怪
				local creatureInfo = UnitInfo:new{ptr = creature}
				creatureInfo:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
				
				local place = string.char(64+indx)
				
				self:SetUInt16(bossintstart + KUAFU_XIANFU_BOSS_SHOW_INFO, 1, 1)
				local boxid = self:GetBoxid()
				app:CallOptResult(self.ptr, OPRATE_TYPE_XIANFU, XIANFU_TYPE_BOSS_OCCUR, {bossName, place, tb_item_template[boxid].name})
			end
		end
	end
	
	-- 刷新
	self:SendMiniMapInfo()
	
	return true
end

-- 刷新buff
function InstanceKuafuXianfu:OnBuffRefresh()
	-- 处理刷新次数
	local times = self:GetMapReserveValue1()
	if times < tb_kuafu_xianfu_base[ 1 ].times then
		self:SetMapReserveValue1(times+1)
	end
	
	-- 刷新把原来的先删掉
	local allGameObjects = mapLib.GetAllGameObject(self.ptr)
	for _, gameobject in pairs(allGameObjects) do
		mapLib.RemoveWorldObject(self.ptr, gameobject)
	end
	
	-- 刷新新的
	local indice = GetRandomIndexTable(#tb_buff_xianfu, #tb_kuafu_xianfu_base[ 1 ].buffPos)
	for i = 1, #indice do
		local pos = tb_kuafu_xianfu_base[ 1 ].buffPos[ i ]
		local indx = indice[ i ]
		local entry = tb_buff_xianfu[indx].gameobject_id
		mapLib.AddGameObject(self.ptr, entry, pos[ 1 ], pos[ 2 ], GO_GEAR_STATUS_END)
	end
	
	return self:GetMapReserveValue1() < tb_kuafu_xianfu_base[ 1 ].times
end

-- 副本失败退出
function InstanceKuafuXianfu:timeoutCallback()
	self:SyncResultToWeb()
	self:SetMapState(self.STATE_FINISH)
	return false
end

--当副本状态发生变化时间触发
function InstanceKuafuXianfu:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		
		--10s后结束副本
		local timestamp = os.time() + InstanceKuafuXianfu.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
	end
end

-- 判断是否能退出副本
function InstanceKuafuXianfu:DoPlayerExitInstance(player)
	-- 直接回原服
	local playerInfo = UnitInfo:new {ptr = player}
	local login_fd = serverConnList:getLogindFD()
	call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
	return 0	--返回1的话为正常退出，返回0则不让退出
end

--玩家加入地图
function InstanceKuafuXianfu:OnJoinPlayer(player)
	
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
		local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + emptyIndex * MAX_KUAFU_XIANFU_INT_COUNT
		local strstart = KUAFU_XIANFU_FIELDS_STR_INFO_START + emptyIndex * MAX_KUAFU_XIANFU_STR_COUNT

		self:SetStr(strstart + KUAFU_XIANFU_PLAYER_NAME, playerInfo:GetName())
		self:SetStr(strstart + KUAFU_XIANFU_PLAYER_GUID, playerInfo:GetPlayerGuid())

		self:SetDouble(intstart + KUAFU_XIANFU_PLAYER_SETTLEMENT,playerInfo:GetForce())
		
		-- 同步钱先
		self:OnSyncMoney(playerInfo:GetPlayerGuid(), emptyIndex)
		playerInfo:SetToGroupMode(playerInfo:GetPlayerGuid())
	end
end

-- 获得名字对应的位置, ''表示用来查询空位位置
function InstanceKuafuXianfu:findIndexByName(name)
	name = name or ''
	local start = KUAFU_XIANFU_FIELDS_STR_INFO_START
	for i = 0, MAX_KUAFU_XIANFU_COUNT-1 do
		if self:GetStr(start+KUAFU_XIANFU_PLAYER_NAME) == name then
			return i
		end
		start = start + MAX_KUAFU_XIANFU_STR_COUNT
	end
	
	return -1
end

--当玩家死亡后触发()
function InstanceKuafuXianfu:OnPlayerDeath(player)
	-- 如果状态已经改变, 即使死了也不再更新时间
	if self:GetMapState() ~= self.STATE_START then
		return
	end
	
end


--[[

// 仙府夺宝 玩家信息
enum KUAFU_XIANFU_PLAYER_INFO
{
	KUAFU_XIANFU_PLAYER_SHOW_INFO		= 0,										// 4个byte(byte0:宝箱数量, byte1:击杀人数, byte2:击杀BOSS数量)
	KUAFU_XIANFU_PLAYER_MONEY			= KUAFU_XIANFU_PLAYER_SHOW_INFO + 1,		// 元宝数据
	KUAFU_XIANFU_PLAYER_MONEY_CHANGED	= KUAFU_XIANFU_PLAYER_MONEY + 2,			// 元宝改变值
	KUAFU_XIANFU_PLAYER_SETTLEMENT		= KUAFU_XIANFU_PLAYER_MONEY_CHANGED + 2,	// 玩家战力
	MAX_KUAFU_XIANFU_INT_COUNT,														// kuafu属性数量

	KUAFU_XIANFU_PLAYER_NAME		= 0,									//玩家名称
	KUAFU_XIANFU_PLAYER_GUID		= KUAFU_XIANFU_PLAYER_NAME + 1,			//玩家guid
	MAX_KUAFU_XIANFU_STR_COUNT,
};


--]]

-- 同步数据到场景服
function InstanceKuafuXianfu:SyncResultToWeb()
	local sendInfo = {}

	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START
	local strstart = KUAFU_XIANFU_FIELDS_STR_INFO_START
	local loot_entry = self:GetBoxid()
	
	for i = 0, MAX_KUAFU_XIANFU_COUNT-1 do
		local player_guid = self:GetStr(strstart + KUAFU_XIANFU_PLAYER_GUID)
		if string.len(player_guid) > 0 then
			local reward = {}
			local hard = self:GetHard()
			for _, info in pairs(tb_kuafu_xianfu_condition[hard].joinReward) do
				table.insert(reward, info[ 1 ]..","..info[ 2 ])
			end
			local count   = self:GetByte  (intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 0)
			if count > 0 then
				table.insert(reward, loot_entry..","..count)
				outFmtDebug(" player %s has %d", player_guid, count)
			end
			table.insert(sendInfo, {player_guid, string.join(",", reward)})
		end
		
		intstart = intstart + MAX_KUAFU_XIANFU_INT_COUNT
		strstart = strstart + MAX_KUAFU_XIANFU_STR_COUNT
	end
	
	local retInfo = {}
	-- 然后同步到web
	for _, info in pairs(sendInfo) do
		table.insert(retInfo, string.join("|", info))
	end
	
	local url = string.format("%s%s/match_result", globalGameConfig:GetExtWebInterface(), InstanceKuafuXianfu.sub)
	local data = {}
	data.ret = string.join(";", retInfo)
	data.open_time = 1
	print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@result = ", data.ret)
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		outFmtDebug("response = %s", tostring(response))
	end)
end

function InstanceKuafuXianfu:OnPlayerKilled(player, killer)
	local killerInfo = UnitInfo:new{ptr = killer}
	local playerInfo = UnitInfo:new{ptr = player}
	
	-- 击杀者加击杀数
	local indx1 = self:findIndexByName(killerInfo:GetName())
	local intStart = KUAFU_XIANFU_FIELDS_INT_INFO_START + indx1 * MAX_KUAFU_XIANFU_INT_COUNT
	self:AddKillCount(intStart)
	
	-- 被杀者下标
	local indx2 = self:findIndexByName(playerInfo:GetName())
	
	-- 通知所有人击杀信息
	local strstart = KUAFU_XIANFU_FIELDS_STR_INFO_START
	for i = 0, MAX_KUAFU_XIANFU_COUNT-1 do
		local player_guid = self:GetStr(strstart + KUAFU_XIANFU_PLAYER_GUID)
		local player = mapLib.GetPlayerByPlayerGuid(self.ptr, player_guid)
		if player then
			local playerInfo = UnitInfo:new {ptr = player}
			playerInfo:call_kuafu_3v3_kill_detail(indx1, indx2)
		end
		
		strstart = strstart + MAX_KUAFU_XIANFU_STR_COUNT
	end
	
	-- 增加死亡次数
	self:AddPlayerKilledCount(playerInfo)
	
	-- 掉落箱子 如果身上有的话
	self:OnDropTreasure(playerInfo, killerInfo)
	
	return 0
end

-- 当玩家被怪物杀死
function InstanceKuafuXianfu:OnPlayerKilledByMonster(player, killer)
	local killerInfo = UnitInfo:new{ptr = killer}
	local playerInfo = UnitInfo:new{ptr = player}
	
	-- 增加死亡次数
	self:AddPlayerKilledCount(playerInfo)
	
	-- 掉落箱子 如果身上有的话
	self:OnDropTreasure(playerInfo, killerInfo)
	
	return 0
end


function InstanceKuafuXianfu:OnDropTreasure(playerInfo, killerInfo, is_offline)
	local belongGuid = ''
	if killerInfo and killerInfo:GetTypeID() == TYPEID_PLAYER then
		belongGuid = playerInfo:GetPlayerGuid()
	end
	is_offline = is_offline or false
	local indx = self:findIndexByName(playerInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + indx * MAX_KUAFU_XIANFU_INT_COUNT
	
	local protectTime = 5
	local has = self:GetByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 0)
	local count = has
	if not is_offline then
		-- 判断如果有箱子 会掉多少
		for k, v in pairs(tb_kuafu_xianfu_killed_drop) do
			if v.ownRange[ 1 ] <= has and has <= v.ownRange[ 2 ] then
				local rand = randInt(1, 10000)
				if rand <= v.rate then
					count = math.min(v.drop, has)
					protectTime = v.protect
				end
				break
			end
		end
		
		self:OnSendDeathInfo(playerInfo, killerInfo:GetName(), string.format("%d", count))			
		if count > 0 then				
			app:CallOptResult(self.ptr, OPRATE_TYPE_XIANFU, XIANFU_TYPE_BOSS_KILL, {playerInfo:GetName(), killerInfo:GetName(), count})
		end
	end
	
	if count > 0 then
		local loot_entry = self:GetBoxid()
		for i = 1, count do
			--模板,数量,绑定与否,存在时间,保护时间,强化等级
			--local drop_item_config = {entry, 1, 0, 1800, protectTime, 0}		
			AddLootGameObject(self.ptr, playerInfo.ptr, belongGuid, loot_entry, 1, 0, InstanceKuafuXianfu.BOX_EXIST_TIME, protectTime, 0)
		end
		
		self:SubByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 0, count)
		-- 同步到u对象
		playerInfo:SubXianfuBoxCount(count)
	end
end

--当玩家离开时触发
function InstanceKuafuXianfu:OnLeavePlayer( player, is_offline)
	local unitInfo = UnitInfo:new{ptr = player}
	-- 回存元宝信息
	self:OnRestoreGold(unitInfo)
	
	-- 活动副本结束了就不进行处理
	if self:GetMapState() == self.STATE_FINISH then
		return
	end
	
	self:OnDropTreasure(unitInfo, nil, true)
	-- 如果没人了 那就结束
	local persons = mapLib.GetPlayersCounts(self.ptr)
	if persons == 0 then
		self:SyncResultToWeb()
		self:SetMapState(self.STATE_FINISH)
	end
end

-- 回存元宝信息
function InstanceKuafuXianfu:OnRestoreGold(unitInfo)
	local url = string.format("%s%s/kuafu_restore", globalGameConfig:GetExtWebInterface(), InstanceKuafuXianfu.sub)
	local binindx = self:findIndexByName(unitInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + binindx * MAX_KUAFU_XIANFU_INT_COUNT
	local data = {}
	data.player_guid = unitInfo:GetPlayerGuid()
	data.changed = self:GetDouble(intstart + KUAFU_XIANFU_PLAYER_MONEY_CHANGED)
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
	end)
end

--使用游戏对象之前
--返回1的话就继续使用游戏对象，返回0的话就不使用
function InstanceKuafuXianfu:OnBeforeUseGameObject(user, go, go_entryid, posX, posY)
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
function InstanceKuafuXianfu:OnUseGameObject(user, go, go_entryid, posX, posY)
	-- 判断对应的是那种buff
	for _, obj in ipairs(tb_buff_xianfu) do
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
function InstanceKuafuXianfu:IsNeedTeleportWhileMapClear(player)
	local playerInfo = UnitInfo:new {ptr = player}
	local login_fd = serverConnList:getLogindFD()
	call_scene_login_to_kuafu_back(login_fd, playerInfo:GetPlayerGuid())
	return 0
end


-- 判断战利品是否需要发送到场景服
function InstanceKuafuXianfu:OnCheckIfSendToAppdAfterLootSelect(player, entry, count)
	local playerInfo = UnitInfo:new {ptr = player}
	local binindx = self:findIndexByName(playerInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + binindx * MAX_KUAFU_XIANFU_INT_COUNT
	
	self:AddByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 0, count)
	
	outFmtDebug("======== guid = %s,  binindx = %d, picked one , now = %d", playerInfo:GetPlayerGuid(), binindx, self:GetByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 0))
	
	-- 同步到u对象
	playerInfo:AddXianfuBoxCount(count)
	
	return 0
end

-- 判断是否够钱花元宝复活
function InstanceKuafuXianfu:OnCheckIfCanCostRespawn(player)
	
	local unitInfo = UnitInfo:new {ptr = player}
	if unitInfo:IsAlive() then
		return
	end
	
	-- 死亡次数
	local times = unitInfo:GetXianfuDeathCount()
	local cost = tb_kuafu_xianfu_base[ 1 ].gold * times
	
	local binindx = self:findIndexByName(unitInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + binindx * MAX_KUAFU_XIANFU_INT_COUNT
	local total = self:GetDouble(intstart + KUAFU_XIANFU_PLAYER_MONEY)
	local used = self:GetDouble(intstart + KUAFU_XIANFU_PLAYER_MONEY_CHANGED)
	
	-- 钱不足
	if used + cost > total then
		return
	end
	self:SubDouble(intstart + KUAFU_XIANFU_PLAYER_MONEY_CHANGED, cost)
	
	-- 如果钱够了 就去
	self:OnCostRespawn(unitInfo)
end
	

-- 同步钱
function InstanceKuafuXianfu:OnSyncMoney(player_guid, binindx)
	local url = string.format("%s%s/sync_money", globalGameConfig:GetExtWebInterface(), InstanceKuafuXianfu.sub)
	local data = {}
	data.player_guid = player_guid
	data.isPk = 1
	
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		
		outFmtDebug(response)
		local dict = nil
		security.call(
			--try block
			function()
				dict = json.decode(response)
			end
		)
		
		if dict then
			if dict.ret == 0 then
				local gold = dict.gold
				local changed = dict.changed
				local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + binindx * MAX_KUAFU_XIANFU_INT_COUNT
				self:SetDouble(intstart + KUAFU_XIANFU_PLAYER_MONEY, gold)
				self:SetDouble(intstart + KUAFU_XIANFU_PLAYER_MONEY_CHANGED, changed)
			end
		end
	end)
end


function InstanceKuafuXianfu:AddKillCount(intstart)
	self:AddByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 1, 1)
	self:AddByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 3, 1)
end

function InstanceKuafuXianfu:AddPlayerKilledCount(playerInfo)
	local indx = self:findIndexByName(playerInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + indx * MAX_KUAFU_XIANFU_INT_COUNT
	self:SetByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 3, 0)
	playerInfo:AddXianfuDeathCount()
end



-------------------------仙府夺宝boss---------------------------
AI_XianfuBoss = class("AI_XianfuBoss", AI_Base)
AI_XianfuBoss.ainame = "AI_XianfuBoss"

--死亡
function AI_XianfuBoss:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	local bossInfo = UnitInfo:new{ptr = owner}
	local playerInfo = UnitInfo:new{ptr = killer_ptr}
	
	local instanceInfo = InstanceKuafuXianfu:new{ptr = map_ptr}
	
	-- 提示击杀奖励
	local entry = bossInfo:GetEntry()
	local indx = instanceInfo:GetBinlogIndxByEntry (entry)
	local boxes = tb_kuafu_xianfu_boss[indx].bossDrop[ 1 ]
	app:CallOptResult(map_ptr, OPRATE_TYPE_XIANFU, XIANFU_TYPE_BOSS_KILL, {bossInfo:GetName(), playerInfo:GetName(), boxes})
	
	
	-- 加杀boss数量
	local binindx = instanceInfo:findIndexByName(playerInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + binindx * MAX_KUAFU_XIANFU_INT_COUNT
	instanceInfo:AddByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 2, 1)
	
	-- 设置boss信息
	local bossintstart = KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START + MAX_KUAFU_XIANFU_BOSS_INT_COUNT * (indx - 1)
	instanceInfo:SetUInt16(bossintstart + KUAFU_XIANFU_BOSS_SHOW_INFO, 1, 2)
	
	return 0
end

--生成战利品
function AI_XianfuBoss:LootAllot(owner, player, killer, drop_rate_multiples, boss_type, fcm)
	local playerInfo = UnitInfo:new{ptr = player}
	local player_guid = playerInfo:GetPlayerGuid()

	print("################ AI_XianfuBoss:LootAllot")
	local map_ptr = unitLib.GetMap(owner)
	local instanceInfo = InstanceKuafuXianfu:new{ptr = map_ptr}
	
	-- 生成奖励
	local bossInfo = UnitInfo:new{ptr = owner}
	local entry = bossInfo:GetEntry()
	local indx = instanceInfo:GetBinlogIndxByEntry (entry)
	
	local dropConfig = tb_kuafu_xianfu_boss[indx].bossDrop
	local boxes			= dropConfig[ 1 ]
	local protectTime	= dropConfig[ 2 ]
	local loot_entry 	= instanceInfo:GetBoxid()
	
	for i = 1, boxes do
		--模板,数量,绑定与否,存在时间,保护时间,强化等级
		--local drop_item_config = {entry, 1, 0, 1800, protectTime, 0}		
		AddLootGameObject(map_ptr, owner, player_guid, loot_entry, 1, fcm, InstanceKuafuXianfu.BOX_EXIST_TIME, protectTime, 0)
	end
			
end


return InstanceKuafuXianfu