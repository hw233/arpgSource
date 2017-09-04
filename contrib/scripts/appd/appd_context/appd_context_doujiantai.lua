local protocols = require('share.protocols')

local DOUJIANTAI_MAP_ID = 3001
-- 斗剑台战斗
function PlayerInfo:OnDoujiantaiFight(indx)
	local config = tb_doujiantai_base[1]
	
	if self:GetLevel() < config.level then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_NO_LEV,config.level)
		--等级不够，需要{0}才能参与斗剑台
		return
	end
	
	--[[
	if self:GetMapId() ~= ZHUCHENG_DITU_ID then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_ZHUCHENG)
		--只能在主城中参加活动
		return 
	end
	--]]
	local instMgr = self:getInstanceMgr()
	
	local curtime = instMgr:getDoujianEnterTimes()
	local basetime = config.dailytimes
	local buytime = instMgr:getDoujianBuyTimes()
	
	outFmtDebug("curtime %d,basetime %d,buytime %d",curtime,basetime,buytime)
	
	if curtime >= (basetime+buytime) then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_NO_TIME)
		--挑战次数已满
		return 
	end
	
	local cdtime = instMgr:getDoujianCD()
	if cdtime > os.time() then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_IN_CD)
		--挑战CD中
		return
	end
	
	local rank2 = instMgr:GetEnemyRank(indx)
	if rank2 == 0 then
		return
	end
	
	local rank1 = self:GetDoujiantaiRank()
	-- 自己在被别人挑战中
	if globalCounter:IsInFight(rank1) then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_SELF_BATTLE)
		return
	end
	-- 对手在挑战中
	if globalCounter:IsInFight(rank2) then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_OTHER_BATTLE)
		return
	end

	globalCounter:lock(rank1, rank2)
	local history	= instMgr:getDoujianCombatWin()
	local curr		= instMgr:getDoujianCurrCombatWin()
	local generalId = string.format("%d|%s|%d|%d|%d|%d", os.time(), self:GetGuid(), rank1, rank2, history, curr)
	local x = tb_doujiantai_base[ 1 ].bornPos[ 1 ][ 1 ]
	local y = tb_doujiantai_base[ 1 ].bornPos[ 1 ][ 2 ]
	-- 发起传送
	call_appd_teleport(self:GetScenedFD(), self:GetGuid(), x, y, DOUJIANTAI_MAP_ID, generalId)
	
	-- 任务
	local questMgr = self:getQuestMgr()
	questMgr:OnUpdate(QUEST_TARGET_TYPE_JOIN_DOUJIANTAI)
	
	instMgr:addDoujianEnterTimes()
	curtime = curtime + 1

	local bcd = config.battleCountdown
	local cdSeconds
	if curtime >= #bcd then
		cdSeconds = bcd[#bcd]
	else
		cdSeconds = bcd[curtime]
	end
	
	cdSeconds = cdSeconds + os.time()
	
	instMgr:setDoujianCD(cdSeconds)
	
	self:AddActiveItem(VITALITY_TYPE_DOUJIANTAI)
end

-- 获取对手信息
function PlayerInfo:GetEnemyInfo()
	local instMgr = self:getInstanceMgr()
	local dict = instMgr:GetEnemyRankList()

	local ct = {}
	for i = 1, #dict do
		local rank = dict[ i ]
		local config = globalCounter:GetRankInfo(rank)
		table.insert(ct, config)
	end

	local output = protocols.pack_doujiantai_get_enemys_info()
	output:writeU16(#ct)
	for i = 1, #ct do
		local config = ct[ i ]
		output:writeByte(i)
		output:writeU16(dict[ i ])
		output:writeUTF(config.name)
		output:writeByte(config.gender)
		output:writeU16(config.level)
		output:writeU32(config.weapon)
		output:writeU32(config.avatar)
		output:writeU32(config.divine)
		output:writeDouble(config.force)
	end
	self:SendPacket(output)
	output:delete()
end

function PlayerInfo:GetTop3()
	local dict = {1, 2, 3}
	local ct = {}
	for i = 1, #dict do
		local rank = dict[ i ]
		local config = globalCounter:GetRankInfo(rank)
		table.insert(ct, config)
	end

	local output = protocols.pack_doujiantai_top3()
	output:writeU16(#ct)
	for i = 1, #ct do
		local config = ct[ i ]
		output:writeByte(i)
		output:writeU16(dict[ i ])
		output:writeUTF(config.name)
		output:writeByte(config.gender)
		output:writeU16(config.level)
		output:writeU32(config.weapon)
		output:writeU32(config.avatar)
		output:writeU32(config.divine)
		output:writeDouble(config.force)
	end
	self:SendPacket(output)
	output:delete()
end


-- 刷新斗剑台对手
function PlayerInfo:RefreshDoujiantaiEnemy()
	local instMgr = self:getInstanceMgr()
	instMgr:RefreshEnemy()
end

function PlayerInfo:BuyDoujiantaiTime(num)
	
	local config = tb_doujiantai_base[1].buyInfo
	
	local instMgr = self:getInstanceMgr()
	
	local hasbuy = instMgr:getDoujianBuyTimes()
	
	if (hasbuy + num ) > #config then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_MAX_TIME)
		--超过最大购买次数
		return
	end
	
	local cost = {}
	for i = 1, num do
		local idx = hasbuy + i
		AddTempInfoIfExist(cost, config[idx][ 1 ], config[idx][ 2 ])
	end

	if not self:useAllItems(MONEY_CHANGE_DOUJIAN_BUY_TIMES, cost, 1) then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_NO_MONEY)
		--元宝不足购买斗剑次数
		return
	end
	
	instMgr:addDoujianBuyTimes(num)
	
end

function PlayerInfo:ClearDoujianCD()
	local instMgr = self:getInstanceMgr()	
	local cdtime = instMgr:getDoujianCD()
	if cdtime <= os.time() then
		return
	end
	
	local config = tb_doujiantai_base[ 1 ]
	local minute = math.ceil((cdtime - os.time())/60)
	local cost = config.changeRate
	if not self:useAllItems(MONEY_CHANGE_DOUJIAN_CLEARCD, cost, minute) then
		self:CallOptResult(OPRATE_TYPE_DOUJIAN, DOUJIAN_OPERATE_NO_MONEY_CD)
		--元宝不足不能清理CD
		return
	end
	
	instMgr:setDoujianCD(0)
	
end

--首胜奖励
function PlayerInfo:DoujianFirstReward(rank)
	local instMgr = self:getInstanceMgr()	
	instMgr:setDoujianFirstRank(rank)
	instMgr:setDoujianMaxRank(rank)
end

--领取首胜奖励
function PlayerInfo:GetDoujianFirstReward(id)
	if id <=0 or id >= 31 then
		return
	end
	local instMgr = self:getInstanceMgr()
	
	if not instMgr:getDoujianFirstRank(id) then
		--outFmtDebug("no first")
		return
	end
	
	if not instMgr:getDoujianFirstReward(id) then
		local config = tb_doujiantai_first[id]
		if config then
			self:AppdAddItems(config.reward,MONEY_CHANGE_DOUJIAN_FIRST_REWARD,LOG_ITEM_OPER_TYPE_DOUJIAN_FIRST_REWARD)
			instMgr:setDoujianFirstReward(id)
		end
	end
	--outFmtDebug("has get")
end
--斗剑台连胜
function PlayerInfo:DoujianCombatWin(tf)
	local instMgr = self:getInstanceMgr()
	instMgr:setDoujianCombatWin(tf)
	
	if tf then
		local num = instMgr:getDoujianCombatWin()
		outFmtDebug("liansheng %d",num)
		local config = tb_doujiantai_combat_win[num]
		if config then
			
			if not instMgr:getDoujianCombatWinReward(config.pos) then
				self:AppdAddItems(config.reward,MONEY_CHANGE_DOUJIAN_COMBATWIN_REWARD,LOG_ITEM_OPER_TYPE_DOUJIAN_COMBAT_REWARD)
				instMgr:setDoujianCombatWinReward(config.pos)
			end
			
		end
	end
end
--每日奖励
function PlayerInfo:DoDoujiantaiDayReward()
	local rank = self:GetDoujiantaiRank()
	if rank == 0 then
		return
	end
	for i,v in ipairs(tb_doujiantai_day) do
		if rank <= v.rank then
			local rewardStr = ""
			for s,d in ipairs(v.reward) do
				if s > 1 then
					rewardStr = rewardStr .. ","
				end
				rewardStr = rewardStr..d[1]..","..d[2]
			end
			
			AddGiftPacksData(self:GetGuid(),0,GIFT_PACKS_TYPE_DOUJIAN_DAY,os.time(),os.time() + 86400*30, v.mailtitle, v.maildesc, rewardStr, SYSTEM_NAME)
			break
		end
	end
	
	--每日重置
	local instMgr = self:getInstanceMgr()
	instMgr:dayDoujianReset()
end

--斗剑台排名列表
function PlayerInfo:GetDoujiantaiRankList(startIdx,endIdx)
	
	local allnum = endIdx - startIdx + 1
	
	local output = protocols.pack_rank_list_query_result()
	
	output:writeU32(RANK_TYPE_DOUJIANTAI)
	output:writeI32(self:GetDoujiantaiRank())
	output:writeU32(100)
	output:writeU16(allnum)
	
	for i=startIdx,endIdx do
		local config = globalCounter:GetRankInfo(i)
		
		output:writeI16(RANK_LIST_CURRENT_OBJ_INT_FIELD_SHOW_END+1)
		for j=0,RANK_LIST_CURRENT_OBJ_INT_FIELD_SHOW_END do
			
			
			if j == RANK_LIST_CURRENT_OBJ_INT_FIELD_RANKING then
				output:writeU32(i)
			elseif j == RANK_LIST_CURRENT_OBJ_INT_FIELD_LEVEL then
				output:writeU32(config.level)
			elseif j == RANK_LIST_CURRENT_OBJ_INT_FIELD_FORCE then
				output:writeDouble(config.force)
			elseif j == (RANK_LIST_CURRENT_OBJ_INT_FIELD_FORCE+1) then
			elseif j == RANK_LIST_CURRENT_OBJ_INT_FIELD_BYTE then
				output:writeByte(config.gender)
				output:writeByte(0)
				output:writeByte(config.vip)
				output:writeByte(0)
			elseif j == RANK_LIST_CURRENT_OBJ_INT_FIELD_LEVEL then
				output:writeU32(config.level)
			elseif j == RANK_LIST_CURRENT_OBJ_INT_FIELD_SHOW then
				output:writeU32(config.avatar)
			elseif j == (RANK_LIST_CURRENT_OBJ_INT_FIELD_SHOW+1) then
				output:writeU32(config.weapon)
				
			else
				output:writeU32(0)
			end
			
		end
		
		output:writeI16(RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_NAME+1)
		
		--outFmtDebug("rank string ----  %d",RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_NAME+1)
		
		for j=0,RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_NAME do
			if j == RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_GUID then
				output:writeUTF(config.guid)
			elseif j == RANK_LIST_CURRENT_OBJ_STR_FIELD_PLAYER_NAME then
				output:writeUTF(config.name)
			else
				output:writeUTF("")
			end
		end
	end
	self:SendPacket(output)
	output:delete()
end

