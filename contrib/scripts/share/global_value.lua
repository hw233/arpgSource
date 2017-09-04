--世界变量
local GlobalValue = class('GlobalValue', assert(BinLogObject))

local MAX_BOSS_STATE_END = 1
local MAX_BOSS_STATE_RUNNING = 0


function GlobalValue:ctor( )
end

--获取全服在线玩家数
function GlobalValue:GetQuanFuOnLinePlayerNum()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_ONLINE_PLAYER_NUM)
end

--获取当前活动版本号
function GlobalValue:GetLimitActivityVersion()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_LIMIT_ACTIVITY_VERSION)
end

--设置当前活动版本号
function GlobalValue:SetLimitActivityVersion(val)
	self:SetUInt32(GLOBALVALUE_INT_FIELD_LIMIT_ACTIVITY_VERSION,val)
end

--获取当前活动脚本
function GlobalValue:GetLimitActivityScript()
	return self:GetStr(GLOBALVALUE_STRING_FIELD_LIMIT_ACTIVITY_SCRIPT)
end
--设置当前活动脚本
function GlobalValue:SetLimitActivityScript(val)
	self:SetStr(GLOBALVALUE_STRING_FIELD_LIMIT_ACTIVITY_SCRIPT,val)
end

--获取当前活动结束时间
function GlobalValue:GetLimitActivityEndTime()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_LIMIT_ACTIVITY_END_TIME)
end
--设置当前活动结束时间
function GlobalValue:SetLimitActivityEndTime(val)
	self:SetUInt32(GLOBALVALUE_INT_FIELD_LIMIT_ACTIVITY_END_TIME,val)
end

--清空一些合服的时候需要清空的数据
function GlobalValue:ClearNeedClearInfo_Hefu()
	
end

-- 获得野外BOSS序号
function GlobalValue:GetFieldBossIndex(mapid, lineNo)
	local mapIndex = tb_field_boss[mapid].indx
	return mapIndex * MAX_DEFAULT_LINE_COUNT + lineNo - 1
end

-- 获得野外BOSS数据的int起始值
function GlobalValue:GetFieldBossIntStart(indx)
	return GLOBALVALUE_INT_FIELD_FIELD_BOSS_START + indx * MAX_FIELD_BOSS_INT_DATA_COUNT
end

-- 获得野外BOSS数据的str起始值
function GlobalValue:GetFieldBossStrStart(indx)
	return GLOBALVALUE_STRING_FIELD_FIELD_BOSS_START + indx * MAX_FIELD_BOSS_STR_DATA_COUNT
end


-- 重置野外boss数据
function GlobalValue:ResetFieldBoss(mapid, lineNo)
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	local strStart = self:GetFieldBossStrStart(indx)

	-- 设置倒计时
	self:SetUInt32(intStart + FIELD_BOSS_DATA_PROCESS_TYPE, FIELD_BOSS_PROCESS_START_COUNTDOWN)
	-- 即将刷新的时间
	self:SetUInt32(intStart + FIELD_BOSS_DATA_NEXT_BORN_TIME, os.time() + tb_field_boss_time[1].noticestarttime * 60)
	-- 清空优先开启时间戳
	--self:SetUInt32(intStart + FIELD_BOSS_DATA_PRIORITY_TIME, 0)
	-- 清空优先开始玩家guid
	--self:SetStr(strStart + FIELD_BOSS_DATA_MAX_DAMAGE_GUID, "")
	--self:SetStr(strStart + FIELD_BOSS_DATA_NAME, "")
end

-- 野外刷新BOSS
function GlobalValue:BornFieldBoss(mapid, lineNo)
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	
	-- 设置最近一次刷新时间
	-- 设置boss刷新
	self:SetUInt32(intStart + FIELD_BOSS_DATA_PROCESS_TYPE, FIELD_BOSS_PROCESS_BORN)
	-- 血量满
	-- self:SetDouble(intStart + FIELD_BOSS_DATA_CURR_HP, 100)
end

-- 野外BOSS受到伤害
function GlobalValue:FieldBossDamageDeal(mapid, lineNo, percent)
	--[[
	血量好像不需要存
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	
	-- 设置血量
	self:SetDouble(intStart + FIELD_BOSS_DATA_CURR_HP, percent)
	--]]
end

-- 野外BOSS死亡, 刷新箱子
function GlobalValue:FieldBossKilled(mapid, lineNo, guid, name)
	-- 一定要boss刷出来才会刷新箱子
	if not self:IsFieldBossBorn(mapid, lineNo) then
		return
	end
	
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	local strStart = self:GetFieldBossStrStart(indx)
	
	-- 血量清零
	--self:SetDouble(intStart + FIELD_BOSS_DATA_CURR_HP, 0)
	-- 设置宝箱出现
	self:SetUInt32(intStart + FIELD_BOSS_DATA_PROCESS_TYPE, FIELD_BOSS_PROCESS_TREASURE)
	-- 设置BOSS击杀次数
	self:AddUInt32(intStart + FIELD_BOSS_DATA_KILLED, 1)
	
	-- 设置优先者 和 优先时间 及其名字
	self:SetUInt32(intStart + FIELD_BOSS_DATA_PRIORITY_TIME, os.time() + 60)
	self:SetStr(strStart + FIELD_BOSS_DATA_MAX_DAMAGE_GUID, guid)
	self:SetStr(strStart + FIELD_BOSS_DATA_NAME, name)
end

function GlobalValue:GetProtectCooldown(mapid, lineNo)
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	
	local future = self:GetUInt32(intStart + FIELD_BOSS_DATA_PRIORITY_TIME)
	return future - os.time()
end


-- 箱子被领取, 活动结束
function GlobalValue:FieldBossTreasurePicked(mapid, lineNo)
	-- 一定是箱子领了才会活动结束
	if not self:IsFieldBossTreasureOccur(mapid, lineNo) then
		return
	end
	
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	
	-- 设置结束
	self:SetUInt32(intStart + FIELD_BOSS_DATA_PROCESS_TYPE, FIELD_BOSS_PROCESS_TYPE_FINISH)
end

-- 是否已经结束
function GlobalValue:IsFieldBossFinish(mapid, lineNo)
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	
	return self:GetUInt32(intStart + FIELD_BOSS_DATA_PROCESS_TYPE) == FIELD_BOSS_PROCESS_TYPE_FINISH
end

-- 是否开始倒计时
function GlobalValue:IsFieldBossCountDown(mapid, lineNo)
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	
	return self:GetUInt32(intStart + FIELD_BOSS_DATA_PROCESS_TYPE) == FIELD_BOSS_PROCESS_START_COUNTDOWN
end

-- 是否刷新BOSS
function GlobalValue:IsFieldBossBorn(mapid, lineNo)
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	
	return self:GetUInt32(intStart + FIELD_BOSS_DATA_PROCESS_TYPE) == FIELD_BOSS_PROCESS_BORN
end

-- 是否是宝箱出现
function GlobalValue:IsFieldBossTreasureOccur(mapid, lineNo)
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)
	
	return self:GetUInt32(intStart + FIELD_BOSS_DATA_PROCESS_TYPE) == FIELD_BOSS_PROCESS_TREASURE
end

-- 是否能够采集宝箱
function GlobalValue:CanPickTreasure(mapid, lineNo, guid)
	local indx = self:GetFieldBossIndex(mapid, lineNo)
	local intStart = self:GetFieldBossIntStart(indx)	
	local strStart = self:GetFieldBossStrStart(indx)
	
	-- 设置优先者 和 优先时间
	local priorityTime = self:GetUInt32(intStart + FIELD_BOSS_DATA_PRIORITY_TIME)
	local priorityGuid = self:GetStr(strStart + FIELD_BOSS_DATA_MAX_DAMAGE_GUID)
	return priorityTime < os.time() or guid == priorityGuid
end



-------------------------------世界BOSS---------------------------------

-- 获得世界BOSS的等级
function GlobalValue:GetWorldBossLevel()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_LEVEL)
end

-- 增加世界BOSS的等级
function GlobalValue:AddWorldBossLevel()
	self:AddUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_LEVEL, 1)
end

-- 获得世界BOSS的状态
function GlobalValue:GetWorldBossState()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_STATE)
end

-- 设置世界BOSS的状态
function GlobalValue:SetWorldBossState(state)
	if self:GetWorldBossState() == state then
		return
	end
	self:SetUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_STATE, state)
end

function GlobalValue:SetEnrollTime()
	if self:IsWorldBossEnroll() then
		self:SetUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_LAST_ENROLL_TIME, os.time())
	end
end

function GlobalValue:GetEnrollCountdown()
	local lastEnrollTime = self:GetUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_LAST_ENROLL_TIME)
	return tb_worldboss_time[ 1 ].enrolllast * 60 + lastEnrollTime - os.time()
end

-- 获得世界BOSS的次数
function GlobalValue:GetWorldBossTimes()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_TIMES)
end

-- 设置世界BOSS的次数
function GlobalValue:AddWorldBossTimes()
	self:AddUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_TIMES, 1)
end

-- 是否是世界BOSS结束
function GlobalValue:IsWorldBossEnd()
	return self:GetWorldBossState() == WORLD_BOSS_PROCESS_TYPE_FINISH
end

-- 是否是世界BOSS报名阶段
function GlobalValue:IsWorldBossEnroll()
	return self:GetWorldBossState() == WORLD_BOSS_PROCESS_ENROLL
end

-- 是否是世界BOSS刷新阶段
function GlobalValue:IsWorldBossBorn()
	return self:GetWorldBossState() == WORLD_BOSS_PROCESS_BORN
end

-- 世界BOSS某条线是否结束
function GlobalValue:IsWorldBossEndInLine(lineNo)
	if lineNo < 1 or lineNo >= 32 then
		return
	end
	return self:GetBit(GLOBALVALUE_INT_FIELD_WORLD_BOSS_LINE_STATE, lineNo)
end

function GlobalValue:SetWorldBossEndInLine(lineNo)
	if lineNo < 1 or lineNo >= 32 then
		return
	end
	self:SetBit(GLOBALVALUE_INT_FIELD_WORLD_BOSS_LINE_STATE, lineNo)
end

function GlobalValue:UnSetWorldBossEndInLine()
	self:SetUInt32(GLOBALVALUE_INT_FIELD_WORLD_BOSS_LINE_STATE, 0)
end

-- 设置这2天要刷的世界BOSS类型
function GlobalValue:RandomStepWorldBoss()
	outFmtInfo("RandomStepWorldBoss")
	local dict = GetRandomIndexTable(#tb_worldboss_base, 2)
	self:SetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 0, dict[ 1 ])
	self:SetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 1, dict[ 2 ])
end

-- 服务器启动的时候调用 确定有需要随机的BOSS
function GlobalValue:RandomStepWorldBossIfNeverDoes()
	local id = self:GetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 0)
	local id2 = self:GetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 1)
	if not (0 < id and id <= #tb_worldboss_base) or not (0 < id2 and id2 <= #tb_worldboss_base) then
		globalValue:RandomStepWorldBoss()
	end
	outFmtInfo("curr boss id = %d, %d", self:GetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 0), self:GetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 1))
end

-- 设置这次的BOSS ID
function GlobalValue:RandomTodayWorldBossID()
	local indx = randInt(0, 1)
	local id = self:GetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, indx)
	self:SetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 2, id)
	
	return self:GetTodayWorldBossID()
end

-- 获得今日的世界BOSSid
function GlobalValue:GetTodayWorldBossID()
	return self:GetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 2)
end

-- BOSS结束的时候调用 确定有需要随机的BOSS
function GlobalValue:RandomWorldBossIfNextStep()
	local times = self:GetWorldBossTimes()
	if times % 2 == 0 then
		globalValue:RandomStepWorldBoss()
	end
end


-- 获得今日的世界BOSS房间个数
function GlobalValue:GetTodayWorldBossRoom()
	return self:GetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 3)
end

-- 设置今日的世界BOSS房间个数
function GlobalValue:SetTodayWorldBossRoom(rooms)
	self:SetByte(GLOBALVALUE_INT_FIELD_WORLD_BOSS_ID, 3, rooms)
end



------------------------------抽奖活动---------------------------------
--[[
#define MAX_LOTTERY_COUNT 10
#define MAX_LOTTERY_RECORD_COUNT 10

	GLOBALVALUE_INT_FIELD_LOTTERY_RECORD_CURSOR_START	= GLOBALVALUE_INT_FIELD_MASS_BOSS_END,		// 抽奖记录游标
	GLOBALVALUE_INT_FIELD_LOTTERY_RECORD_CURSOR_END		= GLOBALVALUE_INT_FIELD_LOTTERY_RECORD_CURSOR_START + MAX_LOTTERY_COUNT,
	
	GLOBALVALUE_STRING_FIELD_LOTTERY_RECORD_START = GLOBALVALUE_STRING_FIELD_GIFT_RANK_WINER_GUARD_NAME + 1,		// 抽奖记录数据开始
	GLOBALVALUE_STRING_FIELD_LOTTERY_RECORD_END   = GLOBALVALUE_STRING_FIELD_LOTTERY_RECORD_START + MAX_LOTTERY_COUNT * MAX_LOTTERY_RECORD_COUNT,	//
--]]

function GlobalValue:AddLotteryRecord(offset, record)
	if offset < 0 or offset >= MAX_LOTTERY_COUNT then
		return
	end
	local cursor = self:GetUInt32(GLOBALVALUE_INT_FIELD_LOTTERY_RECORD_CURSOR_START + offset)
	--print("GlobalValue:AddXianfuRecord", cursor, record)
	self:SetStr(cursor + GLOBALVALUE_STRING_FIELD_LOTTERY_RECORD_START + offset * MAX_LOTTERY_RECORD_COUNT, record)
	cursor = cursor + 1
	if cursor >= MAX_LOTTERY_RECORD_COUNT then
		cursor = 0
	end
	self:SetUInt32(GLOBALVALUE_INT_FIELD_LOTTERY_RECORD_CURSOR_START + offset, cursor)
end

------------------------------------仙府夺宝记录-----------------------------------
-- GLOBALVALUE_STRING_FIELD_XIANFU_RECORD_START
-- MAX_XIANFU_RECORD_COUNT
-- GLOBALVALUE_INT_FIELD_XIANFU_RECORD_CURSOR

function GlobalValue:AddXianfuRecord(record)
	local cursor = self:GetUInt32(GLOBALVALUE_INT_FIELD_XIANFU_RECORD_CURSOR)
	--print("GlobalValue:AddXianfuRecord", cursor, record)
	self:SetStr(cursor + GLOBALVALUE_STRING_FIELD_XIANFU_RECORD_START, record)
	cursor = cursor + 1
	if cursor >= MAX_XIANFU_RECORD_COUNT then
		cursor = 0
	end
	self:SetUInt32(GLOBALVALUE_INT_FIELD_XIANFU_RECORD_CURSOR, cursor)
end


function GlobalValue:doMassBossStart(id)
	local intstart = GLOBALVALUE_INT_FIELD_MASS_BOSS_START + (id - 1) * MAX_MASS_BOSS_INT_FIELD_COUNT
	self:SetUInt32(intstart + MASS_BOSS_STATE, MAX_BOSS_STATE_RUNNING)
	self:SetUInt32(intstart + MASS_BOSS_TIME ,     0)
end

function GlobalValue:doMassBossEnd(id)
	local intstart = GLOBALVALUE_INT_FIELD_MASS_BOSS_START + (id - 1) * MAX_MASS_BOSS_INT_FIELD_COUNT
	self:SetUInt32(intstart + MASS_BOSS_STATE, MAX_BOSS_STATE_END)
	local cd = tb_mass_boss_info[ id ].born * 60
	self:SetUInt32(intstart + MASS_BOSS_TIME, os.time() + cd)
end

function GlobalValue:isMassBossAlive(id)
	local intstart = GLOBALVALUE_INT_FIELD_MASS_BOSS_START + (id - 1) * MAX_MASS_BOSS_INT_FIELD_COUNT
	return self:GetUInt32(intstart + MASS_BOSS_STATE) == MAX_BOSS_STATE_RUNNING
end

function GlobalValue:checkMassBossReborn(id)
	local intstart = GLOBALVALUE_INT_FIELD_MASS_BOSS_START + (id - 1) * MAX_MASS_BOSS_INT_FIELD_COUNT
	local time = self:GetUInt32(intstart + MASS_BOSS_TIME)
	return time > 0 and os.time() >= time
end

------------------------------------------------------------------------
function GlobalValue:IsActivityRunning(actId)
	for i = GLOBALVALUE_INT_FIELD_ACTIVITIES_RUNNING_START, GLOBALVALUE_INT_FIELD_ACTIVITIES_RUNNING_END-1 do
		if self:GetUInt32(i) == actId then
			return true
		end
	end
	return false
end

function GlobalValue:SetActivityRunning(actId)
	for i = GLOBALVALUE_INT_FIELD_ACTIVITIES_RUNNING_START, GLOBALVALUE_INT_FIELD_ACTIVITIES_RUNNING_END-1 do
		if self:GetUInt32(i) == 0 then
			self:SetUInt32(i, actId)
			activityManagerRunning(actId)
			return
		end
	end
	globalCounter:SetActivityFinished(actId)
end

function GlobalValue:UnSetActivityRunning(actId)
	for i = GLOBALVALUE_INT_FIELD_ACTIVITIES_RUNNING_START, GLOBALVALUE_INT_FIELD_ACTIVITIES_RUNNING_END-1 do
		if self:GetUInt32(i) == actId then
			self:SetUInt32(i, 0)
			return
		end
	end
end

function GlobalValue:GetActivityRunningList()
	local list = {}
	for i = GLOBALVALUE_INT_FIELD_ACTIVITIES_RUNNING_START, GLOBALVALUE_INT_FIELD_ACTIVITIES_RUNNING_END-1 do
		local actId = self:GetUInt32(i)
		if actId > 0 then
			table.insert(list, actId)
		end
	end
	return list
end

--魅力排行记录信息
--[[
--上周魅力排行第一家族旗子
function GlobalValue:SetGiftRankWinerFactionFlag(value)
	self:SetUInt32(GLOBALVALUE_INT_FIELD_GIFT_RANK_WINER_FACTION_FLAG ,value)
end

function GlobalValue:GetGiftRankWinerFactionFlag()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_GIFT_RANK_WINER_FACTION_FLAG)
end

--上周魅力排行第一女王名称
function GlobalValue:SetGiftRankWinerQueenName(value)
	self:SetStr(GLOBALVALUE_STRING_FIELD_GIFT_RANK_WINER_QUEEN_NAME ,value)
end

function GlobalValue:GetGiftRankWinerQueenName()
	return self:GetStr(GLOBALVALUE_STRING_FIELD_GIFT_RANK_WINER_QUEEN_NAME)
end

--上周魅力排行第一家族名称
function GlobalValue:SetGiftRankWinerFactionName(value)
	self:SetStr(GLOBALVALUE_STRING_FIELD_GIFT_RANK_WINER_FACTION_NAME,value)
end

function GlobalValue:GetGiftRankWinerFactionName()
	return self:GetStr(GLOBALVALUE_STRING_FIELD_GIFT_RANK_WINER_FACTION_NAME)
end

--上周魅力排行第一骑士名称
function GlobalValue:SetGiftRankWinerGuardName(value)
	self:SetStr(GLOBALVALUE_STRING_FIELD_GIFT_RANK_WINER_GUARD_NAME,value)
end

function GlobalValue:GetGiftRankWinerGuardName()
	return self:GetStr(GLOBALVALUE_STRING_FIELD_GIFT_RANK_WINER_GUARD_NAME)
end


--帮派魅力排行奖励下一结算时间
function GlobalValue:GetFactionGiftRankNextUpdateTime()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_GIFT_RANK_NEXT_UPDATE_TIME)
end

function GlobalValue:SetFactionGiftRankNextUpdateTime(value)
	self:SetUInt32(GLOBALVALUE_INT_FIELD_GIFT_RANK_NEXT_UPDATE_TIME,value)
end

--帮派魅力排行当前轮数
function GlobalValue:GetFactionGiftRankCurRound()
	return self:GetUInt32(GLOBALVALUE_INT_FIELD_GIFT_RANK_CUR_ROUND)
end

function GlobalValue:SetFactionGiftRankCurRound(value)
	self:SetUInt32(GLOBALVALUE_INT_FIELD_GIFT_RANK_CUR_ROUND,value)
end

function GlobalValue:AddFactionGiftRankCurRound(value)
	self:AddUInt32(GLOBALVALUE_INT_FIELD_GIFT_RANK_CUR_ROUND,value)
end




--帮派魅力排行奖励下一结算时间重置
function GlobalValue:UpdateFactionGiftRank()
	if os.time() >= self:GetFactionGiftRankNextUpdateTime() then
		local update_cycle = tb_faction_gift_rank_base[1].update_cycle
		self:SetFactionGiftRankNextUpdateTime(GetTodayStartTimestamp() + update_cycle * 86400)
		self:AddFactionGiftRankCurRound(1)
	end
end
--]]
return GlobalValue
