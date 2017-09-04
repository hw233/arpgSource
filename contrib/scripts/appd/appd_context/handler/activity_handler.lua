---购买活动每日礼包
function PlayerInfo:Handle_Activity_Opt_Buy_DailyGift(pkt)
	local act_id = pkt.act_id
	local index = pkt.index
	
	local scriptId = getScriptIdByActId(act_id)
	if scriptId ~= ACT_DAILY_GIFT then
		outFmtDebug("actId %d not fit target handle %d ",act_id,ACT_DAILY_GIFT)
		return
	end
	
	DoActivityDataUpdate (self, act_id, {1,index + 1})
end

--购买活动限定礼包
function PlayerInfo:Handle_Activity_Opt_Buy_LimitGift(pkt)
	local act_id = pkt.act_id
	local index = pkt.index
	
	local scriptId = getScriptIdByActId(act_id)
	if scriptId ~= ACT_LIMIT_GIFT then
		outFmtDebug("actId %d not fit target handle %d ",act_id,ACT_LIMIT_GIFT)
		return
	end
	
	DoActivityDataUpdate (self, act_id, {1,index + 1})
end


function PlayerInfo:Handle_Draw_Lottery(pkt)
	local actId = pkt.actId
	local type = pkt.type
	
	local scriptId = getScriptIdByActId(actId)
	if scriptId ~= ACT_LOTTERY then
		outFmtDebug("actId %d not fit target handle %d ",actId,ACT_LOTTERY)
		return
	end
	
	DoActivityDataUpdate (self, actId, {type})
end


--领取开服排行进度奖励
function PlayerInfo:Handle_Activity_Opt_Get_Rank_Process_Reward(pkt)
	local act_id = pkt.act_id
	local index = pkt.index
	
	local scriptId = getScriptIdByActId(act_id)
	if scriptId ~= ACT_RANK then
		outFmtDebug("actId %d not fit target handle %d ",act_id,ACT_RANK)
		return
	end
	
	DoActivityDataUpdate (self, act_id, {1,index})
end

--获取开服排行榜
function PlayerInfo:Handle_Activity_Opt_Show_Rank_List(pkt)
	local act_id = pkt.act_id
	
	local scriptId = getScriptIdByActId(act_id)
	if scriptId ~= ACT_RANK then
		outFmtDebug("actId %d not fit target handle %d ",act_id,ACT_RANK)
		return
	end
	
	DoActivityDataUpdate (self, act_id, {2,index})
end