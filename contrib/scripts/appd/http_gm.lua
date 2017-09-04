local HttpGM = {}

--保存到文件时的切割字符串
local __RECHARGE_SEPARATOR__ = '\n--------------------------------\n'

function HttpGM:GetRechargeFileName(account)
	assert(__RECHARGE_FOLDER__)
	assert(__RECHARGE_FOLDER__ ~= "")
	local file_name = __RECHARGE_FOLDER__ .. "/" .. account
	return file_name
end

--保存不在线的数据到文件
function HttpGM:writeRechargeToHDD(account, text)
	local file_name = self:GetRechargeFileName(account)
	local file = io.open(file_name, "a")
	file:write(text)
	file:write(__RECHARGE_SEPARATOR__)
	file:close()
end

--错误枚举
local RECHARGE_RESLUT_SUCCED				= 0		--成功
local RECHARGE_RESLUT_REPATED				= 1 	--重复充值
local RECHARGE_RESLUT_INSUFFICIENT			= 2 	--余额不足，扣费失败
local RECHARGE_RESLUT_EXCEPTION				= 3 	--充值过程中，游戏服代码逻辑出现异常
local RECHARGE_RESLUT_OFFLINE				= 4 	--玩家不在线
local RECHARGE_RESLUT_OFFLINE_SUCCEED		= 5 	--充值时玩家不在线，重新登录以后充值成功
local RECHARGE_RESLUT_OFFLINE_REPATED		= 6 	--充值时玩家不在线，重新登录以后发现重复充值
local RECHARGE_RESLUT_OFFLINE_INSUFFICIENT	= 7 	--充值时玩家不在线，重新登录以后余额不足，扣费失败

--post充值处理结果给中间件
function HttpGM:PostRechargeResult(id, ret, msg)
	local post_url = self:getUrl(GlobalGameConfig:GetConfSvrUrl(), "/gm_intf/update_recharge_status")
	local data = {id = id, ret = ret, msg = msg}
	local post_data = string.toQueryString(data)
	outFmtInfo("HttpGM:PostRechargeResult %s %s", post_url, post_data)
	self:async_post(post_url,post_data, function ( status_code, response )
		-- if status_code == 200 and info and info.ret == 0  then
		-- end
	end)
end

--当玩家登录时从文件中将充值数据读出来，并为玩家充值
function HttpGM:RchhargeOnLogin(player)
	local file_name = self:GetRechargeFileName(player:GetAccount())
	local file = io.open(file_name, "r")
	if(not file)then
		return
	end
	local str = file:read("*a");
	file:close()
	outFmtInfo("HttpGM:RchhargeOnLogin read file %s %s", player:GetAccount(), str)
	os.remove(file_name)
	--开始处理数据
	local tab = lua_string_split(str, __RECHARGE_SEPARATOR__)	
	local post_url = self:getUrl(GlobalGameConfig:GetConfSvrUrl(), "/gm_intf/query_recharge_status")
	for i = 1, #tab do
		local data = tab[i]
		if(data ~= "")then
			outFmtInfo("HttpGM:RchhargeOnLogin tryDecode %s %s", player:GetAccount(), data)
			local obj = self:tryDecode(data)
			if(obj)then
				obj.type = tonumber(obj.type)
				obj.amount = tonumber(obj.amount)
				obj.adddate = tonumber(obj.adddate)
				if player:GetRechargeLastID() == obj.recharge_id then
					--重复订单到底要不要告诉中间件呢？
					--self:PostRechargeResult(obj.recharge_id, RECHARGE_RESLUT_OFFLINE_REPATED, "offline repeated recharge")
					desc = desc..", but has proceed"
					WriteRechargeLog( obj.account, obj.recharge_id, obj.type, obj.amount, obj.adddate, desc )
					--已经处理过
					outFmtError("HttpGM:RchhargeOnLogin has proceed %s   recharge_id is %s", obj.account, obj.recharge_id)					
				else
					--先去中间件查一下这个单子的状态
					local ret, response = self:post(post_url, string.toQueryString({id = obj.recharge_id}))
					if(ret == 200)then
						local info = self:tryDecode(response)
						--状态不对了
						if(info == nil or info.ret ~= 0 or info.status ~= RECHARGE_STATUS_TYPE_OUTLINE)then
							if(info)then
								outFmtError("HttpGM:RchhargeOnLogin query recharge status err, %u %d", info.ret, info.status)
							else
								outFmtError("HttpGM:RchhargeOnLogin query recharge status err, response %s", response)
							end
						else
							--充值
							if player:DoRecharge(obj) then
								self:PostRechargeResult(obj.recharge_id, RECHARGE_RESLUT_OFFLINE_SUCCEED, "offline recharge success")					
								self:writeRechargeLog(obj, "player login recharge and success")
							else
								assert(obj.type == GOLD_INGOT_RECHARGE_SUB)
								self:PostRechargeResult(obj.recharge_id, RECHARGE_RESLUT_OFFLINE_INSUFFICIENT, "offline sub money fail")
								self:writeRechargeLog(obj, "player login recharge but addmoney fail")
								outFmtError("HttpGM:RchhargeOnLogin submoney error account is %s recharge_id is %s", obj.account, obj.recharge_id)				 	
							end
						end
					else
						outFmtError("HttpGM:RchhargeOnLogin query recharge status fail, %s %s %u", obj.account, obj.recharge_id, ret);
					end
				end
			end
		end
	end
end

function HttpGM:initGMIDIPHanlder( )	
	local checkip = function(ip)
		local conf_svr_url = globalGameConfig:GetConfSvrUrl()
		local start = string.find(conf_svr_url, ip)
		if ip == "127.0.0.1" or start ~= nil then
			return true
		end
		return false
	end
	--充值路由
	self:addHanlde("/gm_intf/gm_recharge", function ( req, rep )
		outFmtInfo("/gm_intf/gm_recharge begin")
		if not checkip(req.remote_ip) then
			outFmtError("/gm_intf/gm_recharge invalid source request! remote: %s", req.remote_ip)
			rep.status = 500
			rep.content = json.encode({ ["ret"] = 1, ["msg"] = "invalid source!"})
			return
		end
		local repData = {}
		local obj = self:tryDecode(req.content)
		if not obj or not obj.account or not obj.recharge_id or not obj.type or not obj.amount or not obj.adddate then					
			rep.status = 500
			repData = { ["ret"] = 1, ["msg"] = "has empty data!"}
			rep.content = json.encode(repData)
			outFmtError("/gm_intf/gm_recharge if not obj or not obj.account or not obj.recharge_id or not obj.type or not obj.amount or not obj.adddate then")
			return
		end

		obj.type = tonumber(obj.type)
		obj.amount = tonumber(obj.amount)
		obj.adddate = tonumber(obj.adddate)
		outFmtInfo("/gm_intf/gm_recharge %s %s %u %u %u", obj.account, obj.recharge_id, obj.type, obj.amount, obj.adddate)
		local player = app.objMgr:getByAccount(obj.account)

		if player then
			if player:GetRechargeLastID() == obj.recharge_id then
				--已经处理过
				rep.status = 500
				repData = { ["ret"] = RECHARGE_RESLUT_REPATED, ["msg"] = "has proceed!"}
				rep.content = json.encode(repData)
				outFmtError("/gm_intf/gm_recharge has proceed %s   recharge_id is %s", obj.account, obj.recharge_id)
			elseif player:DoRecharge(obj) then
				self:writeRechargeLog(obj, "recharge online recharge success")
				rep.status = 200
				repData = { ["ret"] = RECHARGE_RESLUT_SUCCED, ["msg"] = "recharge success!"}
				rep.content = json.encode(repData)
			else
				assert(obj.type == GOLD_INGOT_RECHARGE_SUB)
				self:writeRechargeLog(obj, "recharge online but submoney fail")
				rep.status = 500
				repData = { ["ret"] = RECHARGE_RESLUT_INSUFFICIENT, ["msg"] = "submoney error!"}
				rep.content = json.encode(repData)
				outFmtError("/gm_intf/gm_recharge submoney error account is %s recharge_id is %s", obj.account, obj.recharge_id)
			end
		else
			outFmtError("/gm_intf/gm_recharge not find player %s", obj.account)
			self:writeRechargeToHDD(obj.account, req.content)
			rep.status = 200
			repData = { ["ret"] = RECHARGE_RESLUT_OFFLINE, ["msg"] = "player offline!"}
			rep.content = json.encode(repData)
		end

		
		outFmtInfo("/gm_intf/gm_recharge end")
	end, function (req, rep)
		rep.status = 500
		rep.content = json.encode({["ret"] = RECHARGE_RESLUT_EXCEPTION, ["msg"] = "program exception!"})
	end)
	--礼包路由
	self:addHanlde("/gm_intf/gm_gift_packs", function ( req, rep )
		if not checkip(req.remote_ip) then
			outFmtError("gm_gift_packs invalid source request!")
			rep.status = 500
			rep.content = json.encode({ ["ret"] = 1, ["msg"] = "invalid source!"})
			return
		end
		--local obj = string.parseQueryString(req.query_string)
		local obj = self:tryDecode(req.content)		
		local repData = {}
		--print('----> ', obj.id, obj.gift_type, obj.audience_type, obj.start_time, obj.end_time, obj.gift_name, obj.gift_desc, obj.item_config, obj.to_id)
		if not obj or not obj.id or not obj.gift_type or not obj.audience_type or not obj.start_time or not obj.end_time 
			or not obj.gift_name or not obj.gift_desc or not obj.item_config or not obj.to_id then					
			rep.status = 500
			repData = { ["ret"] = 1, ["msg"] = "has empty data!"}
			rep.content = json.encode(repData)
			outFmtError("if not obj or not obj.account or not obj.recharge_id or not obj.type or not obj.amount or not obj.adddate then")
			return
		end
		obj.id = tonumber(obj.id)
		obj.gift_type = tonumber(obj.gift_type)
		obj.audience_type = tonumber(obj.audience_type)
		obj.start_time = tonumber(obj.start_time)
		obj.end_time = tonumber(obj.end_time)
		local timenow = os.time()		-- 1449737628
		if obj.audience_type == GIFT_PACKS_AUDIENCE_TYPE_ONE and obj.end_time > timenow and obj.id > 0 then
			AddGiftPacksData(obj.to_id, obj.id, obj.gift_type, obj.start_time, obj.end_time, obj.gift_name, obj.gift_desc, obj.item_config,"")
			rep.status = 200
			repData = { ["ret"] = 0, ["msg"] = "gm_gift_packs success!", ["status"] = GIFT_PACKS_STATUS_OK}
			rep.content = json.encode(repData)
		elseif obj.audience_type == GIFT_PACKS_AUDIENCE_TYPE_ALL and obj.end_time > timenow and obj.id > 0 then
			--遍历所有玩家
			app.objMgr:foreachAllPlayer(function(player)
				if player:GetCharCreateTime() < obj.start_time and player:GetPlayerGiftPacksId() < obj.id then
					player:SetPlayerGiftPacksId(obj.id)
					AddGiftPacksData(player:GetGuid(), obj.id, obj.gift_type, obj.start_time, obj.end_time, obj.gift_name, obj.gift_desc, obj.item_config,"")
				end			
			end)
			rep.status = 200
			repData = { ["ret"] = 0, ["msg"] = "gm_gift_packs success!", ["status"] = GIFT_PACKS_STATUS_START} 
			rep.content = json.encode(repData)
		elseif obj.audience_type == GIFT_PACKS_AUDIENCE_TYPE_ALL_ONLINE and obj.end_time > timenow and obj.id > 0 then
			app.objMgr:foreachAllPlayer(function(player)
				AddGiftPacksData(player:GetGuid(), obj.id, obj.gift_type, obj.start_time, obj.end_time, obj.gift_name, obj.gift_desc, obj.item_config,"")				
			end)
			rep.status = 200
			repData = { ["ret"] = 0, ["msg"] = "gm_gift_packs success!", ["status"] = GIFT_PACKS_STATUS_OK}
			rep.content = json.encode(repData)
		end
	end)
	
	--公告
	self:addHanlde("/gm_intf/gm_notice", function(req, rep)
		local obj = self:tryDecode(req.content)
		local running_ids, end_ids = {}, {}
		for i = 1, #obj do
			local status = app.gmNotice:addGmNotice(tonumber(obj[i].id), tonumber(obj[i].u_start_time), tonumber(obj[i].u_end_time), 
				tonumber(obj[i].i_interval), obj[i].content, tonumber(obj[i].i_status) )			
			if status == SYSTEM_NOTICE_STATUS_GARBAGE then
				table.insert(end_ids, tonumber(obj[i].id))
			else
				table.insert(running_ids, tonumber(obj[i].id))
			end				
		end
		
		rep.status = 200
		local repData = {["ret"] = 0, ["msg"] = "send notice success!",}
		repData.running_ids = table.concat(running_ids, ',')
		repData.end_ids = table.concat(end_ids, ',')		
		rep.content = json.encode(repData)
	end)
	
	--内部扶持号
	self:addHanlde("/gm_intf/gm_inaccount_list", function(req, rep)
		local obj = self:tryDecode(req.content)
		local success_account = {}
		for _,info in ipairs(obj) do			
			local player = app.objMgr:getByAccount(info.account)			
			if player then
				if tonumber(info.i_account_type) == 1 then					
					player:SetFlags(PLAYER_FLAG_INACCOUNT)
				elseif tonumber(info.i_account_type) == 0 then					
					player:UnSetFlags(PLAYER_FLAG_INACCOUNT)
				end
				table.insert(success_account, info.account)
			end
		end
		
		rep.status = 200
		local repData = {["ret"] = 0, ["msg"] = "ok", success_account = success_account }		
		rep.content = json.encode(repData)
	end)
end

--扩展该方法
function HttpGM.extend( self )
	for k,v in pairs(HttpGM) do self[k] = v end
end

return HttpGM