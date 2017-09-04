
local security = require("base/Security")

local sub = "world_xianfu"

function PlayerInfo:OnCheckWorldXianfuMatch()
	-- 匹配的不是3v3
	if not app:IsKuafuTypeMatching(self:GetGuid(), KUAFU_TYPE_XIANFU) then
		return
	end
	
	local url = string.format("%s%s/check_match", globalGameConfig:GetExtWebInterface(), sub)
	local data = {}
	local indx = app:GetKuafuTypeMatchingArg(self:GetGuid())
	-- 这里获得3v3队伍匹配的信息
	data.player_guid = self:GetGuid()
	data.indx = indx
	outFmtDebug("$$$$$$$$$$$$$$$$$$$$$$ %d", data.indx)
	data.open_time = 1
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		if response then
			outFmtDebug("OnCheckWorldXianfuMatch response = %s", response)
		end
		local dict = nil
		security.call(
			--try block
			function()
				dict = json.decode(response)
			end
		)
		
		
		if dict then
			outFmtDebug("OnCheckWorldXianfuMatch result %d %s", dict.ret, dict.msg)
			-- 匹配到了
			if dict.ret == 0 then
				local login_fd = serverConnList:getLogindFD()
				local guid = self:GetGuid()
				-- dict.enter_info = json.decode(dict.enter_info)
				local enter_info = dict.enter_info
				if type(enter_info) == "string" then
					enter_info = json.decode(enter_info)
				end
				
				-- 已经匹配到了
				app:SetMatchingKuafuType(self:GetGuid(), nil)
				
				-- 这时不能扣进入券就不让进
				local config = tb_kuafu_xianfu_condition[indx]
				if not self:useMulItem(config.ticket) then
					return
				end
				
				-- 增加进入次数
				local instMgr = self:getInstanceMgr()
				instMgr:AddXianfuDayTimes()
				
				-- 往web存钱(当然有可能数据比较慢到达)
				self:OnSaveMoney()
				
				local pos = enter_info.pos
				local war_id = enter_info.war_id
				local battle_server = enter_info.battle_server
				outFmtInfo("$$$$ on xianfu matched guid = %s war_id = %s", self:GetGuid(), war_id)
				call_appd_login_to_send_kuafu_info(login_fd, guid, war_id, indx, battle_server, '', KUAFU_TYPE_XIANFU)
				-- 设置正在进行跨服标志
				self:KuafuMarked(KUAFU_TYPE_XIANFU)
				
			-- timeout取消匹配
			elseif dict.ret == 1 then
				local target = dict.target
				local count = dict.count
				outFmtDebug("taget = %d, count = %d", target, count)
				self:call_kuafu_match_wait(KUAFU_TYPE_XIANFU, target ,count, 0, '')
			elseif dict.ret == 2 then
				outFmtDebug("== player on cancel match, %s", self:GetGuid())
				self:OnCancelMatch(KUAFU_TYPE_XIANFU)
			end
		end
	end)
end


-- 取消匹配仙府夺宝
function PlayerInfo:OnCancelWorldXianfuMatchBeforeOffline()
	
	if not app:IsKuafuTypeMatching(self:GetGuid(), KUAFU_TYPE_XIANFU) then
		return
	end
	
	local url = string.format("%s%s/cancel_match", globalGameConfig:GetExtWebInterface(), sub)
	local data = {}
	data.player_guid = self:GetGuid()
	data.indx = app:GetKuafuTypeMatchingArg(self:GetGuid())
	outFmtDebug("OnCancelWorldXianfuMatchBeforeOffline %d", data.indx)
	data.open_time = 1
	self:OnCancelMatch(KUAFU_TYPE_XIANFU)
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		if response then
			outFmtDebug("OnCancelWorldXianfuMatchBeforeOffline response = %s", response)
		end
	end)
end


-- 匹配仙府夺宝
function PlayerInfo:OnWorldXianfuMatch(indx)
	-- 已经在跨服了
	if self:IsKuafuing() then
		return false
	end
	
	if app:IsInKuafuTypeMatching(self:GetGuid()) then
		return false
	end
	
	outFmtInfo("###OnWorldXianfuMatch guid = %s", self:GetGuid())
	local url = string.format("%s%s/match", globalGameConfig:GetExtWebInterface(), sub)
	local data = {}
	data.player_guid = self:GetGuid()
	data.open_time = 1
	data.indx = indx
	
	app:SetMatchingKuafuType(self:GetGuid(), KUAFU_TYPE_XIANFU + indx * 65536)
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		if response then
			outFmtDebug("OnWorldXianfuMatch response = %s", response)
		end
		local dict = nil
		security.call(
			--try block
			function()
				dict = json.decode(response)
			end
		)
		
		if dict then
			outFmtDebug("OnWorldXianfuMatch %d %s", dict.ret, dict.msg)
		end
	end)
	
	return true
end


-- 检查跨服仙府夺宝奖励
function PlayerInfo:CheckWorldXianfuReward()
	local url = string.format("%s%s/check_match_result", globalGameConfig:GetExtWebInterface(), sub)
	local data = {}
	data.player_guid = self:GetGuid()
	data.open_time = 1
	
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		if response then
			outFmtDebug("CheckWorldXianfuReward response = %s", response)
		end
		local dict = nil
		security.call(
			--try block
			function()
				dict = json.decode(response)
			end
		)
		if dict then
			outFmtDebug("%d %s %s", dict.ret, dict.msg, self:GetGuid())
			
			if dict.ret == 0 then
				
				-- 任务
				local questMgr = self:getQuestMgr()
				questMgr:OnUpdate(QUEST_TARGET_TYPE_JOIN_XIANFU)
				
				local data = dict.details
				local itemInfoTable = string.split(data, ",")
				
				if #itemInfoTable > 0 then
					outFmtDebug("%s get reward %s", self:GetGuid(), data)
					local rewardDict = {}
					for i = 1, #itemInfoTable, 2 do
						local itemId = tonumber(itemInfoTable[ i ])
						local count  = tonumber(itemInfoTable[i+1])
						table.insert(rewardDict, {itemId, count})
					end
					
					self:AppdAddItems(rewardDict, MONEY_CHANGE_KUAFU_WORLD_XIANFU, LOG_ITEM_OPER_TYPE_XIANFU)
				end
			end
		end
		
	end)
end

-- 传送钱
function PlayerInfo:OnSaveMoney()
	local url = string.format("%s%s/save_money", globalGameConfig:GetExtWebInterface(), sub)
	local data = {}
	data.player_guid = self:GetGuid()
	data.gold = self:GetMoney(MONEY_TYPE_GOLD_INGOT)
	
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		
	end)
end

-- 同步钱
function PlayerInfo:OnSyncMoney()
	local url = string.format("%s%s/sync_money", globalGameConfig:GetExtWebInterface(), sub)
	local data = {}
	data.player_guid = self:GetGuid()
	data.isPk = 0
	
	app.http:async_post(url, string.toQueryString(data), function (status_code, response)
		if response then
			outFmtDebug("OnSyncMoney response = %s", response)
		end
		local dict = nil
		security.call(
			--try block
			function()
				dict = json.decode(response)
			end
		)
		
		if dict then
			if dict.ret == 0 then
				local changed = dict.changed
				if changed ~= 0 then
					outFmtDebug("got changed ====================== %d", changed)
				end
				local origin = MONEY_CHANGE_KUAFU_WORLD_XIANFU
				if changed > 0 then
					self:AddMoney(MONEY_TYPE_GOLD_INGOT, origin, changed)
				elseif changed < 0 then
					local arrears = 0
					local cost = -changed
					--self:SubMoney(MONEY_TYPE_GOLD_INGOT, origin, cost)
					if not self:checkMoneyEnough(MONEY_TYPE_GOLD_INGOT, cost) then
						local realcost = self:GetMoney(MONEY_TYPE_GOLD_INGOT)
						arrears = cost - realcost
						cost = realcost
					end
					self:costMoneys(origin, {{MONEY_TYPE_GOLD_INGOT, cost}})
					if arrears > 0 then
						-- TODO:记录下欠费记录
						self:AddArrears(arrears)
					end
				end
			end
		end
	end)
end

-- 购买进入券
function PlayerInfo:OnBuyTicket(type, indx, count)
	local config = tb_kuafu_xianfu_condition[type].price
	
	if not config[indx] then
		return
	end
	
	if count < 1 then
		return
	end
	
	local ticketid = tb_kuafu_xianfu_condition[type].ticketid
	local cost = {{config[indx][1], config[indx][2]}}
	
	local itemMgr = self:getItemMgr()
	local emptys  = itemMgr:getEmptyCount(BAG_TYPE_MAIN_BAG)
	if emptys < 1 then
		self:CallOptResult(OPRATE_TYPE_BAG, BAG_RESULT_BAG_FULL)
		return
	end
	
	if self:costMoneys(MONEY_CHANGE_BUY_XIANFU_TICKET, cost, count) then
		self:AppdAddItems({{ticketid, count}}, nil, LOG_ITEM_OPER_TYPE_XIANFU_BUY)
	end
end

-- 重置仙府
function PlayerInfo:OnResetXianfu()
	local instMgr = self:getInstanceMgr()
	instMgr:ResetXianfuDayTimes()
end