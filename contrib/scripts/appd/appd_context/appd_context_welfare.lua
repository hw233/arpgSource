--领取首充奖励
function PlayerInfo:WelfareShouchong()
	local questMgr = self:getQuestMgr()
	if questMgr:getWelfareShouchong() == 1 then
		--outFmtDebug("has get shouchong")
		return
	end
	
	
	if self:GetRechageSum() == 0 then
		--outFmtDebug("has never Rechage")
		return
	end
	
	local config = tb_welfare_shouchong[1]
	if not config then
		return
	end
	
	self:AppdAddItems(config.item,MONEY_CHANGE_SHOUCHONG,LOG_ITEM_OPER_TYPE_SHOUCHONG)
	questMgr:setWelfareShouchong()
end
--每日签到奖励
function PlayerInfo:WelfareCheckIn(day)
	
	local questMgr = self:getQuestMgr()
	
	if questMgr:getWelfareCheckIn(day) then 
		--self:CallOptResult(OPRATE_TYPE_ACTIVITY, ACTIVITY_OPERATE_HASGET)
		return
	end
	
	local config = tb_welfare_checkin[day]
	if not config then
		return
	end
	
	local vip = self:GetVIP()
	local times = 1
	if vip >= config.vip then
		times = config.times
	end
	
	local itemdic = {}
	
	if times == 1 then
		itemdic = config.item
	else 
		for _,v in ipairs(config.item) do
			local tab = {}
			table.insert(tab,v[1])
			table.insert(tab,v[2] * times)
			
			table.insert(itemdic,tab)
		end
	end
	
	self:AppdAddItems(itemdic,MONEY_CHANGE_CHECKIN,LOG_ITEM_OPER_TYPE_CHECKIN)
	
	questMgr:setWelfareCheckIn(day)
end
--领取累积签到奖励
function PlayerInfo:WelfareCheckInAll(id)
	local questMgr = self:getQuestMgr()
	
	local config = tb_welfare_checkin_all[id]
	if not config then
		return
	end
	
	if questMgr:getWelfareCheckInAll(id) then 
		--self:CallOptResult(OPRATE_TYPE_ACTIVITY, ACTIVITY_OPERATE_HASGET)
		return
	end
	
	
	local allnum = questMgr:getWelfareCheckInDayNum()
	
	if allnum >= config.num then
		self:AppdAddItems(config.item,MONEY_CHANGE_TOTAL_CHECKIN,LOG_ITEM_OPER_TYPE_TOTAL_CHECKIN)
		questMgr:setWelfareCheckInAll(id)
	end
	
end
--领取等级奖励
function PlayerInfo:WelfareLevel(id)
	--outFmtDebug("000000000000")
	local questMgr = self:getQuestMgr()
	
	local config = tb_welfare_level[id]
	if not config then
		return
	end
	
	if questMgr:getWelfareLev(id) then 
		--self:CallOptResult(OPRATE_TYPE_ACTIVITY, ACTIVITY_OPERATE_HASGET)
		return
	end
	
	
	
	if self:GetLevel() < config.lev then
		return
	end
	
	self:AppdAddItems(config.item,MONEY_CHANGE_WELFARE_LEVEL,LOG_ITEM_OPER_TYPE_WELFARE_LEVEL)
	
	questMgr:setWelfareLev(id)
end
--福利找回奖励
function PlayerInfo:GetWelfareBackReward(type,bestGetback,backNum)
	local questMgr = self:getQuestMgr()
	--local curnum = questMgr:getWelfareBackAllNum(type)
	
	local maxNum = self:GetWelfareBackMaxNum(type)
	
	local allNum = questMgr:getWelfareBackAllNum(type)
	
	--for j=-3,-1 do
	--	local time = GetTodayStartTimestamp(j)
	--	local curnum = questMgr:getWelfareBackNum(type,time)
	--	allNum = allNum + maxNum - curnum
	--	outFmtDebug("type:%d curnum:%d",type,curnum)
	--end
	
	if allNum < backNum then
		return
	end
	
	local cost = self:GetWelfareBackRewardCost(type,bestGetback)
	
	if not self:costMoneys(MONEY_CHANGE_WELF_ACTIVE_GETBACK, cost, backNum) then
		self:CallOptResult(OPRATE_TYPE_ACHIEVE, ACHIEVE_OPERATE_NO_MONEY)
		return
	end
	
	local rewardConfig = self:GetWelfareBackRewardData(type,bestGetback)
	
	if rewardConfig then
		self:AppdAddItems(rewardConfig,MONEY_CHANGE_WELF_ACTIVE_GETBACK,LOG_ITEM_OPER_TYPE_GETBACK,backNum)
	end
	
	local lastNum = backNum
	
	for j=-3,-1 do
		
		local time = GetTodayStartTimestamp(j)
		
		
		local curNum = questMgr:getWelfareBackNum(type,time)
		local lastItemNum = maxNum - curNum
		if lastNum < lastItemNum then
			questMgr:addWelfareBackNum(type,time,lastNum)
			lastNum = 0
			break
		else 
			questMgr:addWelfareBackNum(type,time,lastItemNum)
			lastNum = lastNum - lastItemNum
		end
		
	end
	
	self:SetWelfareBackAllNum()
	
end
--一键找回所有奖励
function PlayerInfo:GetWelfareAllReward(bestGetback)
	local rewardList,costList = self:GetWelfareAllRewardListApply(bestGetback)
	
	if #rewardList == 0 or #costList == 0 then
		return
	end
	
	if not self:costMoneys(MONEY_CHANGE_WELF_ACTIVE_GETBACK, costList) then
		self:CallOptResult(OPRATE_TYPE_ACHIEVE, ACHIEVE_OPERATE_NO_MONEY)
		return
	end
	
	self:AppdAddItems(rewardList,MONEY_CHANGE_WELF_ACTIVE_GETBACK,LOG_ITEM_OPER_TYPE_GETBACK)
	
	self:ClearWelfareBackAllNum()
	
end
--一键找回资源列表
function PlayerInfo:GetWelfareAllRewardList(bestGetback)
	local rewardList,costList = self:GetWelfareAllRewardListApply(bestGetback)
	
	local rewardStr = ""

	for _,v in ipairs(rewardList) do
		local str = v[1] ..",".. v[2]..","
		rewardStr = rewardStr .. str
	end
	local costStr
	for _,v in ipairs(costList) do
		costStr = v[1] ..",".. v[2]..","
	end
	
	if #rewardList == 0 then
		rewardStr = ","
	end
	
	if #costList == 0 then
		costStr = ","
	end
	
	self:call_welfare_rewardlist_getback (rewardStr,costStr)
end

function PlayerInfo:GetWelfareAllRewardListApply(bestGetback)
	local questMgr = self:getQuestMgr()
	local typeary = self:GetWelfareBackTypeAry()
	
	local rewardList = {}
	local costList = {}
	for _,type in ipairs(typeary) do
		local allNum = questMgr:getWelfareBackAllNum(type)
		if allNum > 0 then
			local rewardConfig = self:GetWelfareBackRewardData(type,bestGetback)
			self:AddWelfareReward2List(rewardList,rewardConfig,allNum)
			
			local costConfig = tb_welfare_back[type]
			if bestGetback then
				self:AddWelfareReward2List(costList,costConfig.allcost,math.ceil(allNum * costConfig.alldc * 0.1))
			else 
				self:AddWelfareReward2List(costList,costConfig.basecost,math.ceil(allNum * costConfig.basedc * 0.1))
			end
		end
		
		
	end
	return rewardList,costList
	
end

function PlayerInfo:AddWelfareReward2List(targetTab,tab,num)
	
	for _,v in ipairs(tab) do
		local key = v[1]
		local val = v[2] * num
		local idx = self:GetWelfareRewardTargetIdx(targetTab,key)
		
		if idx == -1 then
			local item = {}
			table.insert(item,key)
			table.insert(item,val)
			
			table.insert(targetTab,item)
		else
			targetTab[idx][2] = targetTab[idx][2] + val
		end
		
	end
	
end

function PlayerInfo:GetWelfareRewardTargetIdx(targetTab,key)
	for i,v in ipairs(targetTab) do
		if v[1] == key then
			return i
		end
	end
	return -1
end

--添加可以找回的记录
function PlayerInfo:AddWelfareBackLog(type,num)
	local daytime = GetTodayStartTimestamp(0)
	local questMgr = self:getQuestMgr()
	questMgr:addWelfareBackNum(type,daytime,num)
	
	self:SetWelfareBackAllNum()
end

function PlayerInfo:AddWelfareBackGmLog(type,num,day)
	local off = -day
	local daytime = GetTodayStartTimestamp(off)
	local questMgr = self:getQuestMgr()
	questMgr:addWelfareBackNum(type,daytime,num)
	
	self:SetWelfareBackAllNum()
end

--根据日志重算所有次数
function PlayerInfo:SetWelfareBackAllNum()
	outFmtDebug("----------------------------------")
	local questMgr = self:getQuestMgr()
	--找回类型循环
	for i=0,WELFA_BACK_TYPE_COUNT do
		local type = i+1
		local maxNum = self:GetWelfareBackMaxNum(type)
		if maxNum > 0 then
			local allNum = 0
			--找回天数循环
			for j=-3,-1 do
				local time = GetTodayStartTimestamp(j)
				local curnum = questMgr:getWelfareBackNum(type,time)
				if curnum < maxNum and curnum >= 0 then
					allNum = allNum + maxNum - curnum
				end
				
				outFmtDebug("type:%d curnum:%d",type,curnum)
			end
			
			local time = GetTodayStartTimestamp(0)
			local curnum = questMgr:getWelfareBackNum(type,time)
			outFmtDebug("type:%d today:%d",type,curnum)
			
			questMgr:setWelfareBackAllNum(type,allNum)
			outFmtDebug("--------")
		end
	end
end
--清理可以找回的次数
function PlayerInfo:ClearWelfareBackAllNum()

	local questMgr = self:getQuestMgr()
	--找回类型循环
	for i=0,WELFA_BACK_TYPE_COUNT do
		local type = i+1
		local maxNum = self:GetWelfareBackMaxNum(type)
		if maxNum > 0 then
			local allNum = 0
			--找回天数循环
			for j=-3,-1 do
				local time = GetTodayStartTimestamp(j)
				questMgr:setWelfareBackNum(type,time,maxNum)
			end

			questMgr:setWelfareBackAllNum(type,0)
		end
	end
end

function PlayerInfo:GetWelfareBackRewardCost(type,bestGetback)
	local config = tb_welfare_back[type]
	local cost
	if bestGetback then
		cost = GetCostMulTab(config.allcost,config.alldc/10)
	else
		cost = GetCostMulTab(config.basecost,config.basedc/10)
	end
	return cost
end

------需要根据逻辑特殊处理---------------------------------------
--所有类型列表
function PlayerInfo:GetWelfareBackTypeAry()
	return {1,2,3,4,5}
end

function PlayerInfo:GetWelfareBackMaxNum(type)
	-- 1-5 表示资源副本 	
	if type >= 1 and type <= 5 then
		return tb_instance_res[type].times
	end

	return 0
end

function PlayerInfo:GetWelfareBackRewardData(type,bestGetback)
	-- 1-5 表示资源副本 	
	if type >= 1 and type <= 5 then
		local lev = self:GetLevel()
		local id = type * 10000 + lev * 100
		local config = tb_welfare_back_data[id]
		
		if config then
			if bestGetback then
				return config.bestitem
			else
				return config.baseitem
			end
		end
	end
	return nil
end

------需要根据逻辑特殊处理-end--------------------------------------
--累计元宝充值奖励
function PlayerInfo:GetWelfareRechargeReward(id)
	if id < 1 or id > 32 then
		outFmtError("GetWelfareRechargeReward id %d error",id)
		return
	end
	local config = tb_welfare_recharge[id]
	
	if not config then
		outFmtError("GetWelfareRechargeReward config %d not exist",id)
		return
	end
	
	local questMgr = self:getQuestMgr()
	if questMgr:GetRechargeRewardFlag(id - 1) then
		outFmtError("GetWelfareRechargeReward reward %d already get",id)
		return
	end
	
	if self:GetRechageSum() < config.money then
		outFmtError("GetWelfareRechargeReward money %d not reach target",self:GetRechageSum())
		return
	end
	
	self:AppdAddItems(config.item,MONEY_CHANGE_RECHARGE_REWARD,LOG_ITEM_OPER_TYPE_RECHARGE_REWARD)
	questMgr:SetRechargeRewardFlag(id - 1)
	
end

--累计元宝消费奖励
function PlayerInfo:GetWelfareConsumeReward(id)
	if id < 1 or id > 32 then
		outFmtError("GetWelfareConsumeReward id %d error",id)
		return
	end
	
	local config = tb_welfare_consume[id]
	
	if not config then
		outFmtError("GetWelfareConsumeReward config %d not exist",id)
		return
	end
	
	local questMgr = self:getQuestMgr()
	if questMgr:GetConsumeRewardFlag(id - 1) then
		outFmtError("GetWelfareConsumeReward reward %d already get",id)
		return
	end
	
	if self:GetConsumeSum() < config.money then
		outFmtError("GetWelfareConsumeReward money %d not reach target",self:GetConsumeSum())
		return
	end
	
	self:AppdAddItems(config.item,MONEY_CHANGE_CONSUME_REWARD,LOG_ITEM_OPER_TYPE_CONSUME_REWARD)
	questMgr:SetConsumeRewardFlag(id - 1)
end


function PlayerInfo:GetWelfareSevendayReward(id)
	if id < 1 or id > 7 then
		outFmtError("GetWelfareSevendayReward id %d error",id)
		return
	end
	
	local config = tb_welfare_sevengift[id]
	
	if not config then
		outFmtError("GetWelfareSevendayReward config %d not exist",id)
		return
	end
	
	local questMgr = self:getQuestMgr()
	if questMgr:GetSevenDayFlag(id - 1) then
		outFmtError("GetWelfareSevendayReward reward %d already get",id)
		return
	end
	
	if questMgr:GetSevenDayProcess() < id then
		outFmtError("GetWelfareSevendayReward login day %d not reach target",self:GetSevenDayProcess())
		return
	end
	
	self:AppdAddItems(config.item,MONEY_CHANGE_SEVEN_DAY_GIFT,LOG_ITEM_OPER_TYPE_SEVEN_DAY_GIFT)
	questMgr:SetSevenDayFlag(id - 1)
end