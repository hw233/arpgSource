local ActDailyGift = class("ActDailyGift", ActivityBase)

IActDailyGift = ActDailyGift:new{}


function ActDailyGift:ctor()
	
end

-- 系统初始化
function ActDailyGift:init(actId)
	
end

-- 系统结束
function ActDailyGift:finish(actId)
	
end


-- 更新(目标, 领取状态)
function ActDailyGift:update(playerInfo, actId, params)
	if params[1] == 1 then
		self:OnBuyDailyGift(playerInfo,actId,params[2])
	end
end

-- 玩家每日重置
function ActDailyGift:reset(playerInfo, actId)
	playerInfo:SetActivityDataUInt32(actId,0,0)
	playerInfo:SetActivityDataUInt32(actId,1,0)
	playerInfo:SetActivityDataUInt32(actId,2,0)
	
end

function ActDailyGift:OnBuyDailyGift(playerInfo,actId,index)
	local act_config = tb_activity_time[actId]
	local base_config = tb_activity_daily_gift_base[act_config.params[1]]
	
	if not base_config.gift_list[index] or index < 1 or index > 3 then
		--index error
		outFmtError("OnBuyDailyGift index error %d",index)
		return
	end
	
	local config = tb_activity_daily_gift[base_config.gift_list[index]]
	
	if playerInfo:GetVIP() < config.vip then
		--vip不足
		outFmtError("OnBuyDailyGift vip %d < %d ",playerInfo:GetVIP(),config.vip)
		return
	end
	
	if playerInfo:GetActivityDataUInt32(actId,index - 1) >= config.buy_count then
		--购买数量到达上限
		outFmtError("OnBuyDailyGift buy count %d >= %d ",playerInfo:GetActivityDataUInt32(actId,index - 1),config.buy_count)
		return
	end
	
	if not playerInfo:hasAllItems(config.cost) then
		--余额不足
		outFmtError("OnBuyDailyGift resource not enough")
		return
	end
	
	if playerInfo:useAllItems(MONEY_CHANGE_ACT_DAILYGIFT,config.cost) then
		playerInfo:AppdAddItems(config.item, MONEY_CHANGE_ACT_DAILYGIFT,LOG_ITEM_OPER_TYPE_ACT_DAILYGIFT)
		playerInfo:SetActivityDataUInt32(actId,index - 1,playerInfo:GetActivityDataUInt32(actId,index - 1) + 1)
	end
end

-- 系统更新
function ActDailyGift:systemUpdate(actId, params)
	
end

return ActDailyGift