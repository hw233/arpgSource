require 'util.functions'
require 'util.json'

local Http = class("Http")

function Http:ctor(ext_web)
	self.cobj = http.client.client()
	--appd_http_port在config.lua中配置，一般是不需要配置的，除非有特殊的调试需求
	if appd_http_port ~= nil and appd_http_port ~= "" then
		self.sobj = http.server.server("0.0.0.0", appd_http_port, "")
	else
		self.sobj = http.server.server("0.0.0.0", "0", "")
	end
	--打印一下http监听的端口
	outString("$> HTTP Server listen:" .. self.sobj:get_port() .. "...")

	self:initIDIPHanlder()

	--注册状态, 0:初始化 1:准备注册 2:注册成功 -1:不需要注册
	self.register_state = 0

	--远程配置选项缓存
	self.web_config = {}

	--客户端的功能
	require('util.http_client').extend(self)
	--腾讯相关web接口
	require('appd.http_tencent').extend(self)
	--GM相关web接口
	require('appd.http_gm').extend(self)
	
	self:initGMIDIPHanlder()
end

function Http:initIDIPHanlder( )
	--默认处理函数
	self:add_hanlde("/", function ( req, rep )
		rep.content = "oh,no,appd!"
	end)

	--发送简单道具	
	self:add_hanlde("/idip/4099", function ( req, rep )
		--[[
			"body" :
		    {
		        "AreaId" : ,           /* 所在大区ID */
		        "OpenId" : "",         /* 用户OpenId */
		        "ItemId" : ,           /* 道具ID */
		        "ItemNum" : ,          /* 道具数量 */
		        "IsAging" : ,          /* 是否有时限 */
		        "BindStatus" : ,       /* 是否绑定 */
		        "MailTitle" : "",      /* 邮箱标题 */
		        "MailContent" : "",    /* 邮箱内容 */
		        "Source" : ,           /* 渠道号 */
		        "Serial" : ""          /* 流水号 */
		    }
		]]
		local obj = self:tryDecode(req.content)		
		if not obj or not obj.RoleId or not obj.ItemId or not obj.ItemNum then
			rep.status = 500
			outError("if not obj.RoleId or not obj.ItemId or not obj.ItemNum then")
			return
		end

		local guid = obj.RoleId		
		local repData = {}
		--如果物品数量小于1则返回错误
		if obj.ItemNum < 1 then
			repData["err"] = -104
			return
		end
		if obj.ItemNum > 1000 then
			repData["err"] = -103
			return
		end

		if tb_itme[obj.ItemId] then
			AddGiftPacksData(guid, 0, 1, 
				os.time(), 
				os.time()+ 7*86400,
				obj.MailTitle or '',
				obj.MailContent or '',
				string.format("%s,%s,0", obj.ItemId, obj.ItemNum),		-- 物品id, 数量, 限时时间
				""
			)
			repData["err"] = 0
		else
			repData["err"] = -102
		end		
		rep.content = json.encode(repData)
	end)
	

end

--通过本方法增加的hanlde，若出错会返回500
function Http:addHanlde(url, fun, err_cb)
	self:add_hanlde(url
		,function(req, rep)
			local ret = doxpcall(fun, req, rep)
			--执行期间出了异常
			if not ret then
				if not err_cb then
					rep.status = 500
				else
					err_cb(req, rep)
				end
			end
		end
	)
end

function Http:update( )
	self.sobj:poll()
	self.cobj:poll()
	--去注册一下吧
	if self.register_state == 0 then
		self:registerExtWeb()
	end	
end

function Http:registerExtWeb( force )
	--是否强制注册
	if force then 
		self.register_state = 0 
	end
	--除了初始化状态需要注册以外
	if self.register_state ~= 0 then
		return
	end

	--如果有配置外部web接口,那么就到这个服务器去注册一下
	--构造post请求
	local conf, data = globalGameConfig, {}
	data.server_names = string.join(';', conf:GetMergeServerName())
	data.port = self.sobj:get_port()
	data.type = 'appd'		--这是应用服接口

	local conf_svr_url = conf:GetConfSvrUrl()
	if conf_svr_url == '' then
		self.register_state = -1
		return
	end

	--//如果是腾讯,需要再特殊注册,就不管成功或者失败了
	if conf:GetPlatformID() == PLATFORM_QQ then
		local url = self:getTencentIntf("/s/reg_appweb")	
		local ret, response = self:post(url, string.toQueryString(data))
		if ret ~= 200 then
			outFmtError("conf:GetPlatformID() == PLATFORM_QQ error: %d url:%s response:%s", ret, url, response)
		else
			outFmtDebug("PLATFORM_QQ RegisterWeb,%d,%s",ret, response)
		end		
	end

	--这里注意可以有两个,正式地址 /cow_config/app_addr/reg
	local url = self:getUrl(conf_svr_url , "/cow_config/app_addr/reg")
	self:async_post(url,string.toQueryString(data), function ( status_code )
		--注册成功
		if status_code == 200 then
			self.register_state = 2
		else
			self.register_state = 0		--如果有错误则重新注册
		end
	end)

	--中间件的后台数据也需要重新推送一下，所以这里重新要一下数据
	--一般同一台web服务器的话不会两个接口一个成功一个失败，所以更用一个状态控制标志即可
	url = self:getUrl(conf_svr_url , "/gm_intf/reload")
	self:async_post(url,string.toQueryString(data), function ( status_code )
		--注册成功
		if status_code == 200 then
			outFmtDebug("appd reloadWeb success to ask web reload!")
		end
	end)

	self.register_state = 1
end

--注册web服务器的处理函数
--cb(request, reply)
function Http:add_hanlde( uri, cb )
	self.sobj:get_request_handler():add_hanlde(uri, cb)
end

--从远程服务器获得配置信息
function Http:getWebConfig( key, default )
	--先尝试从缓存获取
	if self.web_config[key]	 then
		return self.web_config[key]
	end
	local conf, data = globalGameConfig, {}
	local conf_svr_url = conf:GetConfSvrUrl()
	local url = self:getUrl(conf_svr_url , "/cow_config/game_config/get")	
	data.pid, data.sid = unpack(string.split(conf:GetServerID(),'_'))
	data.key = key

	local retcode, response = self:post(url, string.toQueryString(data))
	
	--如果成功返回后存入缓存
	if retcode == 200 then
		self.web_config[key] = response
		return response 
	else
		return default
	end
end


-- 发送信息到web服务器
function Http:SendWebData(url, str_data)
	assert(type(url)=="string" and url ~= "")
	assert(type(str_data)=="string" and str_data ~= "")
	
	local ret, response = self:post(url, str_data)
	if ret ~= 200 then
		outFmtError("SendWebData error: url:%s str_data:%s response:%s", url, str_data, response)
	end
	
	return response
end

-- 发送异步信息到web服务器
function Http:SendAsyncWebData(url, str_data, cb)
	assert(type(url)=="string" and url ~= "")
	assert(type(str_data)=="string" and str_data ~= "")
	assert(type(cb)=="function")
	
	local ret = self:async_post(url, str_data, cb)
	if not ret then
		outFmtError("SendAsyncWebData ret is nil error: url:%s str_data:%s cb:%s", url, str_data, cb)
	end

	return ret
end

return Http
