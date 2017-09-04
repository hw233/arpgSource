--返回某个重置时间点
function PlayerInfo:GetResetDailyTime  (index)
	return self:GetUInt32(PLAYER_APPD_INT_FIELD_RESET_DALIY+index)
end

--设置某个重置时间点
function PlayerInfo:SetResetDailyTime  (index,val)
	self:SetUInt32(PLAYER_APPD_INT_FIELD_RESET_DALIY+index,val)
end

-- 获得最近一周更新的时间点
function PlayerInfo:GetResetWeekTime()
	return self:GetUInt32(PLAYER_APPD_INT_FIELD_RESET_WEEK)
end

--设置最近一周更新的时间点
function PlayerInfo:SetResetWeekTime(val)
	self:SetUInt32(PLAYER_APPD_INT_FIELD_RESET_WEEK, val)
end

--每小时重置一次，看要做什么
function PlayerInfo:DoResetDaily  ()
	local itemMgr = self:getItemMgr()

	--0点重置
	if(self:ResetDaily_Time(0))then
	
		self:onRealReset()
		--重置下标时间设置下
		self:SetResetDailyTime(0,os.time())
	end
	
	--每日5点重置
	if(self:ResetDaily_Time(5))then
		--is_reset = true
		--重置下标时间设置下
		self:SetResetDailyTime(5,os.time())
	end
	
	--每日12点重置
	if(self:ResetDaily_Time(12))then
		self:ResetFactionBossDefenseTickets()
		--重置下标时间设置下
		self:SetResetDailyTime(12,os.time())
	end

	-- 每周重置
	if self:IsWeeklyReset(self:GetResetWeekTime()) then
		self:resetWeeklyInfo()
		self:SetResetWeekTime(os.time())
	end
	
	--每日累加登陆次数
	local limit_acti = self:getLimitActivityInfo()
	if limit_acti then
		limit_acti:OnLogin(self)
	end
end
	
--返回需要重置时间戳
function PlayerInfo:ResetDaily_Time  ( h)
	local time_table = os.date("*t")
	local now_time = time_table.hour
	time_table.hour = h
	time_table.min = 0
	time_table.sec = 0
	local reset_time = os.time(time_table)
	if(now_time < h)then
		reset_time = reset_time - 86400
	end
	if(self:GetResetDailyTime(h) < reset_time)then
		return true
	else
		return false
	end	
end

--算一下几次没重置
function PlayerInfo:ResetCount  (h)
	local now_time = os.time()
	local reset_time = self:GetResetDailyTime(h)
	local reset_table = os.date("*t",reset_time)
	local reset_hour = reset_table.hour
	local count = math.floor((now_time - reset_time)/86400)
	if(count == 0)then
		count = 1
	elseif(reset_hour < h)then
		count = count + 1
	end
	return count
end

--是否已经跨过周日0点了
function PlayerInfo:IsWeeklyReset  ( last_zero_reset_time)
	if last_zero_reset_time == 0 then
		return true
	end
	local last_reset_time = last_zero_reset_time --上一次重置的0点时间戳
	local nowtime = os.time()
	--算出上次的周日24点的时间戳
	local temp = os.date("*t", last_reset_time)
	local temp2 = os.time{year=temp.year,month=temp.month,day=temp.day,hour=24}
	local last_weekend_time = temp2 + (7-temp.wday+1)*86400
	if(temp.wday == 1)then --周日
		last_weekend_time = temp2
	end
	-- outDebug('-------now='..os.date("%Y/%m/%d %H:%M:%S", nowtime))
	-- outDebug('-------last='..os.date("%Y/%m/%d %H:%M:%S", last_reset_time))
	-- outDebug('-------last_weekend_time='..os.date("%Y/%m/%d %H:%M:%S", last_weekend_time))
	if(nowtime >= last_weekend_time)then
		return true
	end
	return false
end

-- reset
function PlayerInfo:onRealReset()
	self:instanceDailyReset()
	self:RankLikeRest()
	self:factionReset()
	self:resetDailyShop()
	self:SetWelfareBackAllNum()
	self:Rank3v3SegmentReward()
	self:OnResetXianfu()
	self:DoDoujiantaiDayReward()
	
	self:RefreshDoujiantaiEnemy()
	self:DailyQuestReset()
	
	self:ClearBuyedFactionShopItem()
	
	self:DailyResetCultivation()
	self:DailyResetLoginActivity()
	
	self:DailyResetFactionBuildingSpeedUpDailyCount()
	self:DailyResetFactionDonateGiftExchangeDailyCount()
	
	
	self:ResetMassBossTimes()
	self:OnResetGroupInstanceDayTimes()
	
	self:OnResetMeridianDayTimes()
	
	self:DailyResetTowerFlag()
	
	self:OnResetLocalSinglePVPDayTimes()
	
	self:DailyResetRevengeTimes()
	
	DoActivityDataReset(self)
	
	self:LoginUpdateSevenDayProcess()
	
	self:DailyResetActive()
end

function PlayerInfo:resetWeeklyInfo()
	self:OnResetLocalSinglePVPWeekInfo()
end
