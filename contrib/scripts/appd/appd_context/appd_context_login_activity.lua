
--累计登录天数
function PlayerInfo:SetLoginActivityTotalDays(value)
	self:SetUInt32(PLAYER_INT_FIELD_LOGIN_ACTIVITY_TOTAL_DAYS,value)
end

function PlayerInfo:GetLoginActivityTotalDays()
	return self:GetUInt32(PLAYER_INT_FIELD_LOGIN_ACTIVITY_TOTAL_DAYS)
end

function PlayerInfo:AddLoginActivityTotalDays(value)
	self:AddUInt32(PLAYER_INT_FIELD_LOGIN_ACTIVITY_TOTAL_DAYS,value)
end

--奖励领取记录
function PlayerInfo:SetLoginActivityRewardStatus(index)
	if index >= 0 and index <= 31 then
		self:SetBit(PLAYER_INT_FIELD_LOGIN_ACTIVITY_REWARD_STATUS_START,index)
	elseif index >= 32 and index <= 63 then
		self:SetBit(PLAYER_INT_FIELD_LOGIN_ACTIVITY_REWARD_STATUS_START + 1,index - 32)
	else
		outFmtDebug('SetLoginActivityRewardStatus: index = %d out of range',index)
	end
end


function PlayerInfo:GetLoginActivityRewardStatus(index)
	if index >= 0 and index <= 31 then
		return self:GetBit(PLAYER_INT_FIELD_LOGIN_ACTIVITY_REWARD_STATUS_START,index)
	elseif index >= 32 and index <= 63 then
		return self:GetBit(PLAYER_INT_FIELD_LOGIN_ACTIVITY_REWARD_STATUS_START + 1,index - 32)
	else
		outFmtDebug('GetLoginActivityRewardStatus: index = %d out of range',index)
		return false
	end
end

function PlayerInfo:UnSetLoginActivityRewardStatus(index)
	if index >= 0 and index <= 31 then
		self:UnSetBit(PLAYER_INT_FIELD_LOGIN_ACTIVITY_REWARD_STATUS_START,index)
	elseif index >= 32 and index <= 63 then
		self:UnSetBit(PLAYER_INT_FIELD_LOGIN_ACTIVITY_REWARD_STATUS_START + 1,index - 32)
	else
		outFmtDebug('UnSetLoginActivityRewardStatus: index = %d out of range',index)
	end
end

function PlayerInfo:InitLoginActivity()
	local day = self:GetLoginActivityTotalDays()
	if day == 0 then
		self:SetLoginActivityTotalDays(1)
	end
end


function PlayerInfo:DailyResetLoginActivity()
	local day = self:GetLoginActivityTotalDays()
	if day == 0 then
		self:SetLoginActivityTotalDays(1)
	else
		self:AddLoginActivityTotalDays(1)
	end
end

function PlayerInfo:GetLoginActivityReward(id)
	if id < 1 or id > 64 then
		outFmtDebug('GetLoginActivityReward: id = %d out of range',id)
		return
	end
	
	local info = tb_login_activity_reward[id]
	if info == nil then
		outFmtDebug('GetLoginActivityReward: tb_login_activity_reward not exist id = %d !!',id)
		return
	end
	
	local flag = self:GetLoginActivityRewardStatus(id - 1)
	if flag then
		outFmtDebug('GetLoginActivityReward: reward: %d already received!!',id)
		return
	end
	
	local days = self:GetLoginActivityTotalDays()
	if days < info.day then
		outFmtDebug('GetLoginActivityReward: total days : %d < require days: %d !!!',days,info.day)
		return
	end
	
	local rewards = info.reward
	
	self:SetLoginActivityRewardStatus(id - 1)
	self:AppdAddItems(rewards,MONEY_CHANGE_LOGIN_ACTIVITY_REWARD,LOG_ITEM_OPER_TYPE_LOGIN_ACTIVITY_REWARD)
			
	
end

