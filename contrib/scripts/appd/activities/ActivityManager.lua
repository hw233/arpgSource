ACT_SCRIPT_MAP = {
	[ACT_DAILY_GIFT] = IActDailyGift,
	[ACT_LOTTERY] = IActLottery,
	[ACT_RANK] = IActRank,
	[ACT_LIMIT_GIFT] = IActLimitGift,
}

function DoActivityDataUpdate (playerInfo, actId, params)
	if not tb_activity_time[actId] then
		return
	end
	
	if not globalValue:IsActivityRunning(actId) then
		outFmtDebug("DoActivityDataUpdate not open")
		return
	end
	
	local scriptId = getScriptIdByActId(actId)
	ACT_SCRIPT_MAP[scriptId]:update(playerInfo, actId, params)
end

function DoActivitySystemDataUpdateByScriptId (ScriptId, params)
	local list = globalValue:GetActivityRunningList()
	for _, actId in ipairs(list) do
		local config = tb_activity_time[actId]
		if config.scriptIndx == ScriptId then
			ACT_SCRIPT_MAP[ScriptId]:systemUpdate(actId, params)
		end
	end
	
end



function DoActivityDataReset(playerInfo)
	local list = globalValue:GetActivityRunningList()
	for _, actId in ipairs(list) do
		local scriptId = getScriptIdByActId(actId)
		ACT_SCRIPT_MAP[scriptId]:reset(playerInfo, actId)
	end
end

function DoActivityDataInitOnLogin(playerInfo)
	local list = globalValue:GetActivityRunningList()
	for _, actId in ipairs(list) do
		playerInfo:ActiveActivity(actId)
	end
end

-- 活动开始初始化
function activityManagerRunning(actId)
	outFmtInfo("@@@@@@@@@@@ act %d is active", actId)
	local scriptId = getScriptIdByActId(actId)
	ACT_SCRIPT_MAP[scriptId]:init(actId)
	
	app.objMgr:foreachAllPlayer(function(player)
		player:ActiveActivity(actId)
	end)
end

-- 活动结束
function activityManagerFinished(actId)
	outFmtInfo("########## act %d is finish", actId)
	local scriptId = getScriptIdByActId(actId)
	ACT_SCRIPT_MAP[scriptId]:finish(actId)
end

function getScriptIdByActId(actId)
	if not tb_activity_time[actId] then
		return -1
	end
	return tb_activity_time[actId].scriptIndx
end