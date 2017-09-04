
local HttpTencent = {}

--获得腾讯接口并生成url
function HttpTencent:getTencentIntf( url )
	local intf = globalGameConfig:GetExtWebInterface()	--self:getWebConfig("tencent_web_intf")
	if intf == "" then return nil end
	return self:getUrl(intf,assert(url))
end

--//通过外部服务器接口获取腾讯黄钻vip信息
function HttpTencent:extWeb_getuser_is_vip(openid, openkey, pf, userip, blue_vip, yellow_vip )	
	local url = self:getTencentIntf("/s/user_vip_info")

	local data = {
		openid = openid,
		openkey = openkey,
		pf = pf,
		userip = userip,
		blue_vip = blue_vip,
		yellow_vip = yellow_vip
	}

	local retcode,response = self:post(url, string.toQueryString(data))
	local ret_o = self:tryDecode(response)	
	return ret_o
end

--通知接口服务器 确认收货
function HttpTencent:extWeb_confirmDelivery( account, ordids, providetype )
	local url = self:getTencentIntf("/s/confirmDelivery")
	local tokens = string.split(account,'_')
	local data = {}
	data.pid, data.sid = unpack(tokens)	 
	data.ordids = ordids
	data.providetype = providetype

	local retcode, response = self:post(url, string.toQueryString(data))

	if retcode ~= 200 then return false end

	local ret_o = self:tryDecode(response)
	if not ret_o then return false end

	return ret_o.ret == 0
end

--处理单条订单
function HttpTencent:dealProvide( v )	
	-- v.providetype 0,充值 1,道具赠送 3，任务集市奖励
	--如果玩家不在线或者没有奖励物品直接跳过
	local player = app.objMgr:getByAccount(v.account)
	if not player then
		return 
	end
	
	local guid = player:GetGuid()
	--充值直接给元宝
	local providetype = tonumber(v.providetype)

	--从web服务器确认掉才可以继续往下走
	if not self:extWeb_confirmDelivery(player:GetAccount(), v.id,providetype) then 
		return 
	end

	local tokens = string.split(v.payitem, '*')

	--充值
	if providetype == 0 then
		local gold = tonumber(v.gold)
		if v.billno ~= nil and v.billno ~= '' and gold > 0 then								
			app.dbAccess:setQQRechargeNum(player:GetAccount(), gold, 1, v.billno)	

			--记录日志				
			WriteTXRechargeLog(player:GetAccount(),guid,player:GetName(),player:GetLevel(), 
				v.billno, v.currency, "", 0, tonumber(v.money), gold, 0, 0, tonumber(v.gDepay), 0, os.time(), player:GetPingTaiInfo())
		end
		return
	end

	--蓝钻特权积分
	if providetype == 1 then
		if #tokens == 3 then
			local score = math.ceil(tonumber(tokens[3])/10) * 100
			local now = os.time()
			if score > 0 and now >= 1442246400 and now <= 1442764799 and providetype == 1 then
				player:AddLanZuanJF(score)
			end
		end
		return
	end

	--集市任务订单,发礼包或物品
	if  providetype == 2 then
		local gift_type = ""
		--订单类型 奖励类型(0:礼包,1:物品) 礼包|物品id 数量
		if #tokens == 3 then
			gift_type = string.format("%s,1,%s,%s",v.providetype, tokens[1], tokens[3])
		elseif #tokens == 1 then
			gift_type = string.format("%s,0,%s,1",v.providetype, tokens[1])
		end
		player:DoreceivePlatAward(gift_type)
		return
	end
end

--检查腾讯直购订单
function HttpTencent:checkExtWeb_QQDeliveryOrder( )
	
	--TODO:如果合服中则返回
	local conf, data = globalGameConfig, {}	
	local url = self:getTencentIntf("/s/getDeliveryOrder")
	local allServerNames = conf:GetMergeServerName()
	
	data.server_names = string.join(';', allServerNames)

	--local status_code, response = self:post(url, string.toQueryString(data))
	self:async_post(url, string.toQueryString(data), function ( status_code, response )
		if status_code ~= 200 then return end
		local o = self:tryDecode(response)
		if o == nil then return end
	
		table.foreach(o.rows, function ( _,v )
			self:dealProvide(v)
		end)
	end)
end

-- function HttpTencent:run_tests(  )
-- 	local response = [[{"ret":0,"msg":"","rows":[{"id":762,"status":1,"money":10,"billno":"-APPDJSX31748-20150826-1723134978","providetype":0,"server_name":"360_2660","gold":100,"currency":"RMB","gDepay":0,"payitem":"1*100*1","account":"2_2590_1"}]}]]
-- 	local o = self:tryDecode(response)
-- 	self:dealProvide(o)
-- end

--扩展该方法
function HttpTencent.extend(self)
	for k,v in pairs(HttpTencent) do self[k] = v end
end

return HttpTencent
