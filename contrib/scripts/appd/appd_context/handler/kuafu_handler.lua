-- 3v3 匹配
function PlayerInfo:Handle_Kuafu_3v3_Match(pkt)
	local config = tb_kuafu3v3_base[1]
	
	--local lev = self:GetLevel()
	--outFmtDebug("Kuafu_3v3_----------------%d",lev)
	if self:GetLevel() < config.limlev then
		self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_NO_LEV,config.limlev)
		return
	end
	
	if self:GetForce() < config.limforce then
		self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_NO_FORCE,config.limforce)
		return
	end
	
	local curtime = os.time()
	local intime = false
	for _,v in ipairs(config.activetime) do
		local t1 = GetTodayHMSTimestamp(v[1],v[2],0)
		local t2 = GetTodayHMSTimestamp(v[3],v[4],0)
		if curtime >= t1 and curtime <= t2 then
			intime = true
			break
		end
	end
	if not intime then
		self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_NO_OPEN)
		return
	end
	
	
	local instMgr = self:getInstanceMgr()
	local curNum = instMgr:get3v3EnterTimes()
	local buyNum = instMgr:get3v3BuyTimes()
	
	outFmtDebug("--------curNum:%d,buyNum:%d,maxnum:%d,buyNum:%d",curNum,buyNum,config.daytimes,config.daybuytimes)
	
	if curNum >= (buyNum + config.daytimes) then
		
		if buyNum < config.daybuytimes then
			self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_NO_TIME_BUY)
		else
			self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_NO_TIME)
		end
	
		return
	end
	
	-- 模块没开 不让进
	if not self:GetOpenMenuFlag(MODULE_ARENA, MODULE_ARENA_XIANMO) then
		return
	end

	if app:IsInKuafuTypeMatching(self:GetGuid()) then
		self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_IN_MATCH)
		return
	end
	
	local tf = self:OnWorld3v3Match()
	
	if tf then
		self:call_kuafu_match_start(KUAFU_TYPE_FENGLIUZHEN)
		--instMgr:add3v3EnterTimes()
	end
	
end

function PlayerInfo:Gm3v3EnterTimes(num)
	outFmtDebug("Gm3v3EnterTimes %d",num)
	local instMgr = self:getInstanceMgr()	
	instMgr:set3v3EnterTimes(num)
end
--3v3购买次数
function PlayerInfo:Handle_Kuafu_3v3_BuyTimes(pkt)
	outFmtDebug("Handle_Kuafu_3v3_BuyTimes")
	local num = pkt.num
	
	if num < 0 then 
		return
	end
	
	local config = tb_kuafu3v3_base[1]
	
	local instMgr = self:getInstanceMgr()
	local buyNum = instMgr:get3v3BuyTimes()
	
	if (num + buyNum) > config.daybuytimes then
		self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_MAX_BUY)
		return
	end
	
	if not self:costMoneys(MONEY_CHANGE_WELF_ACTIVE_GETBACK, config.daybuycost,num) then
		self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_NO_MONEY)
		return
	end
	
	instMgr:add3v3BuyTimes(num)
	
end
-- 3v3 每日活跃奖励
function PlayerInfo:Handle_Kuafu_3v3Day_Reward(pkt)
	outFmtDebug("Handle_Kuafu_3v3Day_Reward")
	local id = pkt.id
	local config = tb_kuafu3v3_day_reward[id]
	
	if not config then
		return
	end
	
	outFmtDebug("Handle_Kuafu_3v3Day_Reward2222222222")
	
	local instMgr = self:getInstanceMgr()
	if instMgr:get3v3DayReward(id) == 1 then
		self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_HAS_DAY_REWARD)
		return
	end
	
	local curNum = instMgr:get3v3EnterTimes()
	if curNum < config.num then
		self:CallOptResult(OPRATE_TYPE_ATHLETICS, ATHLETICS_OPERATE_NO_DAY_REWARD,config.num)
		return
	end
	
	instMgr:set3v3DayReward(id)
	
	self:AppdAddItems(config.reward,MONEY_CHANGE_3V3KUAFU,LOG_ITEM_OPER_TYPE_3V3_KUAFU)
	
end
--3v3排行榜
function PlayerInfo:Handle_Kuafu_3v3_RankList(pkt)
	local ranklist = app.kuafu_rank
	
	local str = ""
	for i=1,10  do
		if ranklist[i] then
			local itemStr = ""
			for _,v in ipairs(ranklist[i]) do
				itemStr = itemStr .. v .. ";"
			end
			str = str .. itemStr .. "\n"
		end
	end
	
	self:call_kuafu_3v3_ranlist(str)
	
end

--3v3自己的名次
function PlayerInfo:Handle_Kuafu_3v3_My_Rank(pkt)
	local ranklist = app.kuafu_rank
	
	local name = self:GetName()
	local rank = 0
	for i,v in ipairs(ranklist) do
		if v[1] == name then
			rank = i
			break
		end
	end
	
	self:call_kuafu_3v3_myrank(rank)
	
end

function PlayerInfo:Handle_Kuafu_3v3_Match_Oper(pkt)
	local oper = pkt.oper
	if oper ~= 0 and oper ~= 1 then
		return
	end
	self:OnPrepareMatch(oper)
end

function PlayerInfo:Handle_Kuafu_3v3_Cancel_Match(pkt)
	local type = pkt.type
	if type == KUAFU_TYPE_FENGLIUZHEN then
		self:OnCancelWorld3v3MatchBeforeOffline()
	elseif type == KUAFU_TYPE_XIANFU then
		self:OnCancelWorldXianfuMatchBeforeOffline()
	elseif type == KUAFU_TYPE_GROUP_INSTANCE then
		self:OnCancelGroupInstanceMatchBeforeOffline()
	elseif type == MATCH_TYPE_LOCAL_SINGLE_PVP then
		self:OnCancelLocalSinglePVPMatchBeforeOffline()
	end
end



function PlayerInfo:Handle_Kuafu_Xianfu_Match(pkt)
	local indx = pkt.indx
	-- index是否正确
	if indx < 1 or indx > #tb_kuafu_xianfu_condition then
		return
	end
	
	-- 等级是否满足
	local config = tb_kuafu_xianfu_condition[indx]
	local levelRange = config.levelrange
	local level = self:GetLevel()
	if not (levelRange[ 1 ] <= level and level <= levelRange[ 2 ]) then
		return
	end
	
	-- 判断进入次数
	local instMgr = self:getInstanceMgr()
	if not instMgr:CheckXianfuDayTimes() then
		return
	end
	
	-- 判断仙府进入券是否足够
	if not self:hasMulItem(config.ticket) then
		return
	end
	
	-- 模块没开 不让进
	--[[
	if not self:GetOpenMenuFlag(MODULE_ARENA, MODULE_ARENA_XIANFU) then
		return
	end
	--]]
	
	local rt = self:OnWorldXianfuMatch(indx)
	if rt then
		-- 开始匹配
		self:call_kuafu_match_start(KUAFU_TYPE_XIANFU)
	end
end

function PlayerInfo:Handle_Buy_Xianfu_Item(pkt)
	local type = pkt.type
	local indx = pkt.indx
	local count = pkt.count
	
	-- type是否正确
	if type < 1 or type > #tb_kuafu_xianfu_condition then
		return
	end
	
	self:OnBuyTicket(type, indx, count)
end

function PlayerInfo:Handle_Doujiantai_Fight(pkt)
	local rank = pkt.rank
	-- 不再挑战名次内
	if rank < 0 or rank >= 3 then
		return
	end
	
	-- 模块没开 不让进
	if not self:GetOpenMenuFlag(MODULE_ARENA, MODULE_ARENA_DOUJIANTAI) then
		return
	end
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		return 
	end
	
	self:OnDoujiantaiFight(rank)
end

function PlayerInfo:Handle_Doujiantai_Enemys_Info(pkt)
	self:GetEnemyInfo()
end

function PlayerInfo:Handle_Doujiantai_Refresh_Enemys(pkt)
	----[[
	local last = self:GetDoujiantaiLastRefreshTime()
	if last + tb_doujiantai_base[ 1 ].refreshCountdown > os.time() then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_COUNTDOWN)
		return
	end
	--]]
	self:SetDoujiantaiLastRefreshTime(os.time())
	self:RefreshDoujiantaiEnemy()
	self:GetEnemyInfo()
end

function PlayerInfo:Handle_Doujian_BuyTimes(pkt)
	local num = pkt.num
	if num <= 0 then
		return
	end
	self:BuyDoujiantaiTime(num)
end

function PlayerInfo:Handle_Doujian_ClearCd(pkt)
	self:ClearDoujianCD()
end

function PlayerInfo:Handle_Doujian_FirstReward(pkt)
	local id = pkt.id
	self:GetDoujianFirstReward(id)
end

function PlayerInfo:Handle_Doujian_Get_Rank(pkt)
	local startIdx = pkt.startIdx
	local endIdx = pkt.endIdx
	if startIdx > endIdx then
		return
	end
	if startIdx < 1 then
		return 
	end
	if endIdx > MAX_DOUJIANTAI_RANK_COUNT then
		return 
	end
	self:GetDoujiantaiRankList(startIdx,endIdx)
end

function PlayerInfo:Handle_Doujiantai_Top3(pkt)
	self:GetTop3()
end


function PlayerInfo:Handle_Group_Instance_Match(pkt)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_INSTANCE, MODULE_INSTANCE_TEAM)) then
		outFmtInfo("==============")
		return
	end
	local indx = pkt.indx
	-- index是否正确
	if not tb_group_instance_base[indx] then
		outFmtInfo("============== 2")
		return
	end
	
	-- 等级是否满足
	local config = tb_group_instance_base[indx]
	local levelRange = config.limLev
	local level = self:GetLevel()
	if not (levelRange <= level) then
		return
	end
	
	local instMgr = self:getInstanceMgr()
	local count = instMgr:GetGroupInstanceChallengeCount()
	if self:GetGroupInstanceClearFlag(indx) and count == 0 then
		outFmtError("OnChallengeGroupInstance ticket not enough")
		return
	end
	
	-- 模块没开 不让进
	--[[
	if not self:GetOpenMenuFlag(MODULE_ARENA, MODULE_ARENA_XIANFU) then
		return
	end
	--]]
	
	local rt = self:OnGroupInstanceMatch(indx)
	if rt then
		-- 开始匹配
		self:call_kuafu_match_start(KUAFU_TYPE_GROUP_INSTANCE)
	end
end

function PlayerInfo:Handle_Match_Single_PVP(pkt)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_ARENA, MODULE_ARENA_RANK)) then
		return
	end
	
	if not self:isInPvpOpenTime() then
		return
	end
	
	local instMgr = self:getInstanceMgr()
	if instMgr:GetQualifyDailyTimes() < 1 then
		return
	end
	
	local rt = self:OnLocalSinglePVPMatch()
	if rt then
		self:call_kuafu_match_start(MATCH_TYPE_LOCAL_SINGLE_PVP)
	end
end

function PlayerInfo:isInPvpOpenTime()
	local dayStart = GetTodayStartTimestamp()
	local now = os.time()
	for _, info in pairs(tb_single_pvp_base[ 1 ].opentime) do
		local st = info[ 1 ] * 3600 + info[ 2 ] * 60
		local ed = info[ 3 ] * 3600 + info[ 4 ] * 60
		if now >= dayStart + st and now <= dayStart + ed then
			return true
		end
	end
	
	return false
end

function PlayerInfo:Handle_Buy_Match_Single_PVP_Times(pkt)
	local cnt = pkt.cnt
	
	if cnt <= 0 then
		return
	end
	
	local instMgr = self:getInstanceMgr()
	-- 购买次数不能超过上限
	local buyed = instMgr:GetQualifyBuyCount()
	if buyed + cnt > #tb_single_pvp_times then
		return
	end
	
	-- 次数满了不能购买
	local curr = instMgr:GetQualifyDailyTimes()
	if cnt + curr > tb_single_pvp_base[ 1 ].dailyTimes then
		return
	end
		
	local cost = {}
	for i = 1, cnt do
		local config = tb_single_pvp_times[buyed+i]
		for _, rinfo in pairs(config.cost) do
			AddTempInfoIfExist(cost, rinfo[ 1 ], rinfo[ 2 ])
		end
	end
	
	if not self:costMoneys(MONEY_CHANGE_SINGLE_PVP, cost, 1) then
		return
	end
	
	instMgr:AddQualifyBuyCount(cnt)
	instMgr:AddQualifyDailyTimes(cnt)
end

function PlayerInfo:Handle_Pick_Match_Single_PVP_Extra_Reward(pkt)
	local id = pkt.id
	
	if not tb_single_pvp_extra[id] then
		return
	end
	
	local instMgr = self:getInstanceMgr()
	if instMgr:IsQualifyExtraObtain(id-1) and not instMgr:IsQualifyExtraPicked(id-1) then
		instMgr:SetQualifyExtraPicked(id-1)
		local dict = {}
		DoRandomDrop(tb_single_pvp_extra[id].dropId, dict)
		local rewardDict = changeTableToList(dict)
		self:AppdAddItems(rewardDict, MONEY_CHANGE_SINGLE_PVP, LOG_ITEM_OPER_TYPE_SINGLE_PVP)
	end
end
