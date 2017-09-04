InstanceXianfuTest = class("InstanceXianfuTest", InstanceInstBase)

InstanceXianfuTest.Name = "InstanceXianfuTest"
InstanceXianfuTest.exit_time = 10
InstanceXianfuTest.Time_Out_Fail_Callback = "timeoutCallback"
InstanceXianfuTest.dummys = #tb_kuafu_xianfu_tst_base[ 1 ].dummyPos

InstanceXianfuTest.BOX_EXIST_TIME = 1800

function InstanceXianfuTest:ctor(  )
	
end

function InstanceXianfuTest:IsNeedTeleportToOrigin()
	return true
end

--初始化脚本函数
function InstanceXianfuTest:OnInitScript()
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

function InstanceXianfuTest:parseGeneralId()
	local generalId	= self:GetMapGeneralId()
	local hard = 1
	self:SetHard(hard)
end

function InstanceXianfuTest:SetHard(hard)
	self:SetUInt32(KUAFU_XIANFU_FIELDS_HARD, hard)
end


function InstanceXianfuTest:GetHard()
	return self:GetUInt32(KUAFU_XIANFU_FIELDS_HARD)
end


function InstanceXianfuTest:AddDummyCount()
	self:AddUInt32(XIANFU_TEST_DUMMY_COUNT, 1)
end

function InstanceXianfuTest:IsDummyEnough()
	return self:GetDummyCount() >= self.dummys
end

function InstanceXianfuTest:GetDummyCount()
	return self:GetUInt32(XIANFU_TEST_DUMMY_COUNT)
end


function InstanceXianfuTest:GetBoxid()
	local hard = self:GetHard()
	return tb_kuafu_xianfu_condition[hard].boxid
end

-- 初始化boss数据
function InstanceXianfuTest:OnInitBossInfo ()
	local timestamp = os.time()	
	local intstart = KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START
	for indx, bossInfo in ipairs(tb_kuafu_xianfu_tst_boss) do
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
function InstanceXianfuTest:GetBinlogIndxByEntry (entry)
	
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
function InstanceXianfuTest:SendMiniMapInfo()
	
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

function InstanceXianfuTest:AddCountDown()
	local timestamp = os.time() + tb_kuafu_xianfu_tst_base[ 1 ].cd
	self:SetMapStartTime(timestamp)
	mapLib.AddTimer(self.ptr, 'OnDummyRefresh', 500)
end

-- 刷新假人
function InstanceXianfuTest:OnDummyRefresh()
	-- 判断假人数量
	if self:IsDummyEnough() then
		return false
	end
	
	self:AddDummyCount()
	local id = self:GetDummyCount()
	
	local dummyInfo = tb_kuafu_xianfu_tst_dummy[ id ]
	local config = {}
	config.name   = dummyInfo.name
	config.gender = dummyInfo.gender
	config.level  = dummyInfo.level
	config.attrs  = dummyInfo.attrs
	config.weapon = dummyInfo.weapon
	config.avatar = dummyInfo.avatar
	config.divine = dummyInfo.divine
	config.spells = dummyInfo.spells
	config.passivespells = dummyInfo.passivespells
	config.force  = dummyInfo.force
	config.vip    = dummyInfo.vip
	config.reverse1 = 0
	config.reverse2 = 0
	config.reverse3 = 0
	config.reverse4 = 0
	config.reverse5 = 0
	
	local pos = tb_kuafu_xianfu_tst_base[ 1 ].dummyPos[id]
	local offsetX = 0
	local offsetY = 0
	
	local image = self:GetImageInfo(config)
	self:AddImageBody(image, pos[ 1 ] + offsetX, pos[ 2 ] + offsetY, "AI_XianfuDummy", nil, tb_kuafu_xianfu_tst_base[ 1 ].seconds, nil)
	
	-- 设置名称
	local emptyIndex = self:findIndexByName()
	if emptyIndex > -1 then
		local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + emptyIndex * MAX_KUAFU_XIANFU_INT_COUNT
		local strstart = KUAFU_XIANFU_FIELDS_STR_INFO_START + emptyIndex * MAX_KUAFU_XIANFU_STR_COUNT

		self:SetStr(strstart + KUAFU_XIANFU_PLAYER_NAME, config.name)
		self:SetStr(strstart + KUAFU_XIANFU_PLAYER_GUID, 'robot'..id)

		self:SetDouble(intstart + KUAFU_XIANFU_PLAYER_SETTLEMENT, config.force)
	end
	
	return true
end

 --按怪物等级初始化怪物信息
function InstanceXianfuTest:InitCreatureInfo( creature_ptr, bRecal, mul)
	mul = mul or 1
	local creature = UnitInfo:new{ptr = creature_ptr}	
	local entry = creature:GetEntry()
	local config = tb_creature_template[entry]
	if config then
		--给怪物添加基本信息
		creature:SetNpcFlags(config.npcflag)
		if creature:GetLevel() == 0 then
			creature:SetLevel(config.level)
		end

		--给怪物添加技能
		self:SetSpells(creature_ptr, config.spell)

		self:SetCreaturePro(creature, config.pro, bRecal, mul)
	else
		outFmtError("no entry[%d] for creature", entry)
	end
end

-- 活动正式开始
function InstanceXianfuTest:OnTaskStart()
	local timestamp = os.time() +  tb_kuafu_xianfu_tst_base[ 1 ].last + tb_kuafu_xianfu_tst_base[ 1 ].cd
	-- 加任务时间
	self:SetMapQuestEndTime(timestamp)
	-- 副本时间超时回调
	self:AddTimeOutCallback(self.Time_Out_Fail_Callback, timestamp)
end

-- 刷新boss
function InstanceXianfuTest:OnMonsterRefresh()
	if self:GetMapState() == self.STATE_FINISH then
		return false
	end
	
	local timestamp = os.time()
	for indx, bossInfo in pairs(tb_kuafu_xianfu_tst_boss) do
		local info = bossInfo.bossTime
		local createBossTime = info[ 1 ] * 60 + info[ 2 ]
		
		if self:GetMapCreateTime() + createBossTime <= timestamp then
			local bossintstart = KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START + MAX_KUAFU_XIANFU_BOSS_INT_COUNT * (indx - 1)
			local activeInfo = self:GetUInt32(bossintstart + KUAFU_XIANFU_BOSS_BORN_INFO)
			if activeInfo == 0 then
				-- 获得怪物列表
				local dummys = mapLib.GetAllCreature(self.ptr)
				
				self:SetUInt32(bossintstart + KUAFU_XIANFU_BOSS_BORN_INFO, 1)
				local pos = bossInfo.bossPos
				local entry = bossInfo.bossEntry
				local bossName = tb_creature_template[entry].name
				local creature = mapLib.AddCreature(self.ptr, {
						templateid = entry, x = pos[ 1 ], y = pos[ 2 ], 
						active_grid = true, alias_name = bossName, ainame = "AI_XianfuTestBoss", npcflag = {}
					}
				)
				-- 标识为boss怪
				local creatureInfo = UnitInfo:new{ptr = creature}
				creatureInfo:SetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE)
				
				local place = string.char(64+indx)
				
				self:SetUInt16(bossintstart + KUAFU_XIANFU_BOSS_SHOW_INFO, 1, 1)
				local boxid = self:GetBoxid()
				app:CallOptResult(self.ptr, OPRATE_TYPE_XIANFU, XIANFU_TYPE_BOSS_OCCUR, {bossName, place, tb_item_template[boxid].name})
				
				-- 设置仇恨
				for _, dummy_ptr in ipairs(dummys) do
					creatureLib.ModifyThreat(dummy_ptr, creature, 999999)
				end
			end
		end
	end
	
	-- 刷新
	self:SendMiniMapInfo()
	
	return true
end

-- 副本失败退出
function InstanceXianfuTest:timeoutCallback()
	-- 获得奖励
	self:SetMapState(self.STATE_FINISH)
	return false
end

--当副本状态发生变化时间触发
function InstanceXianfuTest:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH then
		self:RemoveTimeOutCallback(self.Time_Out_Fail_Callback)
		
		--10s后结束副本
		local timestamp = os.time() + InstanceXianfuTest.exit_time
		
		self:AddTimeOutCallback(self.Leave_Callback, timestamp)
		self:SetMapEndTime(timestamp)
	end
end

-- 判断是否能退出副本
function InstanceXianfuTest:DoPlayerExitInstance(player)
	return 1	--返回1的话为正常退出，返回0则不让退出
end

--玩家加入地图
function InstanceXianfuTest:OnJoinPlayer(player)	
	InstanceInstBase.OnJoinPlayer(self, player)
	
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
		mapLib.ExitInstance(self.ptr, player)
		return
	end
	
	-- 不能重复进入
	if self:findIndexByName(playerInfo:GetName()) > -1 then
		mapLib.ExitInstance(self.ptr, player)
		return
	end
	
	-- 设置名称
	local emptyIndex = self:findIndexByName()
	if emptyIndex > -1 then
		-- 设置幻化
		local illusionId = tb_kuafu_xianfu_tst_base[ 1 ].illusions[playerInfo:GetGender()]
		playerInfo:SetCharIllusion(illusionId)
		-- 设置属性
		local props = tb_kuafu_xianfu_tst_base[ 1 ].prop
		for _, attrInfo in ipairs(props) do
			local attrId = attrInfo[ 1 ]
			local value  = attrInfo[ 2 ]
			local binlogIndex = GetAttrUnitBinlogIndex(attrId)
			if binlogIndex then
				binLogLib.SetUInt32(player, binlogIndex, value)
				if attrId == EQUIP_ATTR_MAX_HEALTH then
					playerInfo:ModifyHealth(value)
				end
			end
		end
		
		local maxHealth = binLogLib.GetUInt32(player, UNIT_FIELD_MAX_HEALTH)
		unitLib.SetHealth(player, maxHealth)
	
		local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + emptyIndex * MAX_KUAFU_XIANFU_INT_COUNT
		local strstart = KUAFU_XIANFU_FIELDS_STR_INFO_START + emptyIndex * MAX_KUAFU_XIANFU_STR_COUNT

		self:SetStr(strstart + KUAFU_XIANFU_PLAYER_NAME, playerInfo:GetName())
		self:SetStr(strstart + KUAFU_XIANFU_PLAYER_GUID, playerInfo:GetPlayerGuid())

		self:SetDouble(intstart + KUAFU_XIANFU_PLAYER_SETTLEMENT,playerInfo:GetForce())
		
		playerInfo:ChangeToFamilyMode()
	end
end

-- 获得名字对应的位置, ''表示用来查询空位位置
function InstanceXianfuTest:findIndexByName(name)
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

-- 仙府夺宝不加宠物
function InstanceXianfuTest:OnAddPet(player)
	
end

--当玩家死亡后触发()
function InstanceXianfuTest:OnPlayerDeath(player)
	-- 如果状态已经改变, 即使死了也不再更新时间
	if self:GetMapState() ~= self.STATE_START then
		return
	end
	
end

-- 复活后
function InstanceXianfuTest:DoAfterRespawn(unit_ptr)
	local unitInfo = UnitInfo:new{ptr = unit_ptr}
	-- 加无敌buff
	unitLib.AddBuff(unit_ptr, BUFF_INVINCIBLE, unit_ptr, 1, 3)
	
	local unitInfo = UnitInfo:new {ptr = unit_ptr}
	if unitInfo:GetTypeID() == TYPEID_PLAYER then
		-- 如果不对 退出跨服
		if unitInfo:GetUseRespawnMapId() > 0 then
			if unitInfo:GetUseRespawnMapId() ~= self:GetMapId() then
				mapLib.ExitInstance(self.ptr, unit_ptr)
			end
		else
			self:FindAPlaceToRespawn(unit_ptr)
		end
		unitInfo:SetUseRespawnMapId(0)
	else
		local ox = randInt(-2, 2)
		local oy = randInt(-2, 2)
		self:FindAPlaceToRespawn(unit_ptr, true, ox, oy)
	end
end

function InstanceXianfuTest:FindAPlaceToRespawn(unit_ptr, isDummy, offsetX, offsetY)
	isDummy = isDummy or false
	-- 随机复活区域
	local a = GetRandomIndexTable(#tb_kuafu_xianfu_tst_base[ 1 ].respawnPos, 1)
	local pos = tb_kuafu_xianfu_tst_base[ 1 ].respawnPos[a[ 1 ]]
	offsetX = offsetX or 0
	offsetY = offsetY or 0
	
	local toX = pos[ 1 ] + offsetX
	local toY = pos[ 2 ] + offsetY
	
	if not isDummy then
		local toMapId = self:GetMapId()
		local lineNo = self:GetMapLineNo()
		local generalId	= self:GetMapGeneralId()
		playerLib.Teleport(unit_ptr, toMapId, toX, toY, lineNo, generalId)
	else
		local dummyGuid = binLogLib.GetStr(unit_ptr, BINLOG_STRING_FIELD_GUID)
		local dummyName= binLogLib.GetStr(unit_ptr, BINLOG_STRING_FIELD_NAME)
		unitLib.SetPos(unit_ptr, toX, toY, true)
	end
end

function InstanceXianfuTest:OnPlayerKilled(player, killer)
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
function InstanceXianfuTest:OnPlayerKilledByMonster(player, killer)
	local killerInfo = UnitInfo:new{ptr = killer}
	local playerInfo = UnitInfo:new{ptr = player}
	
	-- 被BOSS杀死
	if killerInfo:GetUnitFlags(UNIT_FIELD_FLAGS_IS_BOSS_CREATURE) then
		-- 增加死亡次数
		self:AddPlayerKilledCount(playerInfo)
		
		-- 掉落箱子 如果身上有的话
		self:OnDropTreasure(playerInfo, killerInfo)
	else
		-- 被假人杀死
		self:OnPlayerKilled(player, killer)
	end
	
	return 0
end

function InstanceXianfuTest:OnDropTreasure(playerInfo, killerInfo, is_offline)
	local belongGuid = ''
	if killerInfo and killerInfo:GetTypeID() == TYPEID_PLAYER and playerInfo:GetTypeID() == TYPEID_PLAYER then
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
		for k, v in pairs(tb_kuafu_xianfu_tst_killed_drop) do
			if v.ownRange[ 1 ] <= has and has <= v.ownRange[ 2 ] then
				local rand = randInt(1, 10000)
				if rand <= v.rate then
					count = math.min(v.drop, has)
					protectTime = v.protect
				end
				break
			end
		end
		
		self:OnSendDeathInfo(playerInfo,killerInfo:GetName(), string.format("%d", count))			
		if count > 0 then				
			app:CallOptResult(self.ptr, OPRATE_TYPE_XIANFU, XIANFU_TYPE_BOSS_KILL, {playerInfo:GetName(), killerInfo:GetName(), count})
		end
	end
	
	if count > 0 then
		local loot_entry = self:GetBoxid()
		if killerInfo and killerInfo:GetTypeID() == TYPEID_UNIT then
			self:OnCheckIfSendToAppdAfterLootSelect(killerInfo.ptr, loot_entry, count)
		else
			for i = 1, count do
				--模板,数量,绑定与否,存在时间,保护时间,强化等级
				--local drop_item_config = {entry, 1, 0, 1800, protectTime, 0}		
				AddLootGameObject(self.ptr, playerInfo.ptr, belongGuid, loot_entry, 1, 0, InstanceXianfuTest.BOX_EXIST_TIME, protectTime, 0)
			end
		end
		
		self:SubByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 0, count)
		-- 同步到u对象
		playerInfo:SubXianfuBoxCount(count)
	end
end

--当玩家离开时触发
function InstanceXianfuTest:OnLeavePlayer( player, is_offline)
	
	-- 活动副本结束了就不进行处理
	if self:GetMapState() == self.STATE_FINISH then
		return
	end
	
	-- 给玩家奖励
	local dropConfig 	= tb_kuafu_xianfu_tst_boss[1].bossDrop
	local count			= dropConfig[ 1 ]
	local loot_entry 	= self:GetBoxid()
	
	--发到应用服进行进入判断
	playerLib.SendToAppdDoSomething(player, SCENED_APPD_XIANFU_PRACTISE, loot_entry, ""..count)
		
	-- 如果没人了 那就结束
	local persons = mapLib.GetPlayersCounts(self.ptr)
	if persons == 0 then
		self:SetMapState(self.STATE_FINISH)
	end
end

--使用游戏对象之前
--返回1的话就继续使用游戏对象，返回0的话就不使用
function InstanceXianfuTest:OnBeforeUseGameObject(user, go, go_entryid, posX, posY)
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
function InstanceXianfuTest:OnUseGameObject(user, go, go_entryid, posX, posY)
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
function InstanceXianfuTest:IsNeedTeleportWhileMapClear(player)
	return 1
end


-- 判断战利品是否需要发送到场景服
function InstanceXianfuTest:OnCheckIfSendToAppdAfterLootSelect(player, entry, count)
	local playerInfo = UnitInfo:new {ptr = player}
	local binindx = self:findIndexByName(playerInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + binindx * MAX_KUAFU_XIANFU_INT_COUNT
	
	self:AddByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 0, count)
	-- 同步到u对象
	playerInfo:AddXianfuBoxCount(count)
	
	return 0
end

-- 判断是否够钱花元宝复活
function InstanceXianfuTest:OnCheckIfCanCostRespawn(player)
	
	local unitInfo = UnitInfo:new {ptr = player}
	if unitInfo:IsAlive() then
		return
	end
	
	self:OnCostRespawn(unitInfo)
end

--[[
KUAFU_XIANFU_PLAYER_SHOW_INFO = 6	--  4个byte(byte0:宝箱数量, byte1:击杀人数, byte2:击杀BOSS数量,byte3:连杀次数)
--]]

function InstanceXianfuTest:AddKillCount(intstart)
	self:AddByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 1, 1)
	self:AddByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 3, 1)
end

function InstanceXianfuTest:AddPlayerKilledCount(playerInfo)
	local indx = self:findIndexByName(playerInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + indx * MAX_KUAFU_XIANFU_INT_COUNT
	self:SetByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 3, 0)
	playerInfo:AddXianfuDeathCount()
end

---------------------------仙府夺宝假人-------------------------
AI_XianfuDummy = class("AI_XianfuDummy", AI_Base)
AI_XianfuDummy.ainame = "AI_XianfuDummy"

--死亡
function AI_XianfuDummy:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	
	local dummyInfo = UnitInfo:new{ptr = owner}
	local killerInfo = UnitInfo:new{ptr = killer_ptr}
	
	local instanceInfo = InstanceXianfuTest:new{ptr = map_ptr}
	
	----------------------------------------------------------------------------------
	-- 击杀者加击杀数
	local indx1 = instanceInfo:findIndexByName(killerInfo:GetName())
	local intStart = KUAFU_XIANFU_FIELDS_INT_INFO_START + indx1 * MAX_KUAFU_XIANFU_INT_COUNT
	instanceInfo:AddKillCount(intStart)
	
	-- 被杀者下标
	local indx2 = instanceInfo:findIndexByName(dummyInfo:GetName())
	
	-- 通知所有人击杀信息
	local strstart = KUAFU_XIANFU_FIELDS_STR_INFO_START
	for i = 0, MAX_KUAFU_XIANFU_COUNT-1 do
		local player_guid = instanceInfo:GetStr(strstart + KUAFU_XIANFU_PLAYER_GUID)
		local player = mapLib.GetPlayerByPlayerGuid(map_ptr, player_guid)
		if player then
			local playerInfo = UnitInfo:new {ptr = player}
			playerInfo:call_kuafu_3v3_kill_detail(indx1, indx2)
		end
		
		strstart = strstart + MAX_KUAFU_XIANFU_STR_COUNT
	end
	
	-- 增加死亡次数
	instanceInfo:AddPlayerKilledCount(dummyInfo)
	
	-- 掉落箱子 如果身上有的话
	instanceInfo:OnDropTreasure(dummyInfo, killerInfo)
	
	return 0
end



-------------------------仙府夺宝boss---------------------------
AI_XianfuTestBoss = class("AI_XianfuTestBoss", AI_Base)
AI_XianfuTestBoss.ainame = "AI_XianfuTestBoss"

--死亡
function AI_XianfuTestBoss:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)
	local bossInfo = UnitInfo:new{ptr = owner}
	local playerInfo = UnitInfo:new{ptr = killer_ptr}
	
	local instanceInfo = InstanceXianfuTest:new{ptr = map_ptr}
	
	-- 提示击杀奖励
	local entry = bossInfo:GetEntry()
	local indx = instanceInfo:GetBinlogIndxByEntry (entry)
	local boxes = tb_kuafu_xianfu_tst_boss[indx].bossDrop[ 1 ]
	app:CallOptResult(map_ptr, OPRATE_TYPE_XIANFU, XIANFU_TYPE_BOSS_KILL, {bossInfo:GetName(), playerInfo:GetName(), boxes})
	
	
	-- 加杀boss数量
	local binindx = instanceInfo:findIndexByName(playerInfo:GetName())
	local intstart = KUAFU_XIANFU_FIELDS_INT_INFO_START + binindx * MAX_KUAFU_XIANFU_INT_COUNT
	instanceInfo:AddByte(intstart + KUAFU_XIANFU_PLAYER_SHOW_INFO, 2, 1)
	
	-- 设置boss信息
	local bossintstart = KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START + MAX_KUAFU_XIANFU_BOSS_INT_COUNT * (indx - 1)
	instanceInfo:SetUInt16(bossintstart + KUAFU_XIANFU_BOSS_SHOW_INFO, 1, 2)
	

	local dropConfig = tb_kuafu_xianfu_tst_boss[indx].bossDrop
	local boxes			= dropConfig[ 1 ]
	local protectTime	= dropConfig[ 2 ]
	local loot_entry 	= instanceInfo:GetBoxid()
	
	local dummyPick = boxes
	local dummy_ptr = killer_ptr
	-- 如果是玩家杀的
	if playerInfo:GetTypeID() == TYPEID_PLAYER then
		dummy_ptr = nil
		dummyPick = tb_kuafu_xianfu_tst_boss[ 1 ].dummpy_pick
		for i = 1, boxes-dummyPick do
			--模板,数量,绑定与否,存在时间,保护时间,强化等级
			--local drop_item_config = {entry, 1, 0, 1800, protectTime, 0}		
			AddLootGameObject(map_ptr, owner, '', loot_entry, 1, fcm, InstanceXianfuTest.BOX_EXIST_TIME, protectTime, 0)
		end
	end
	
	if not dummy_ptr then
		-- 删掉BOSS
		local dummys = mapLib.GetAllCreature(map_ptr)
		for i, dummy in ipairs(dummys) do
			if binLogLib.GetBit(dummy, UNIT_FIELD_FLAGS, UNIT_FIELD_FLAGS_IS_BOSS_CREATURE) then
				table.remove(dummys, i)
				break
			end
		end
		
		local indice = GetRandomIndexTable(#dummys, 1)
		dummy_ptr = dummys[ 1 ]
	end
	
	if dummy_ptr then
		instanceInfo:OnCheckIfSendToAppdAfterLootSelect(dummy_ptr, loot_entry, dummyPick)
	end
	
	return 0
end

return InstanceXianfuTest