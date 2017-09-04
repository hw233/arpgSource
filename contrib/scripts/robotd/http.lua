require 'util.functions'
require 'util.json'

local Http = class("Http")
local json_decode = json.decode
local json_encode = json.encode
function Http:ctor(ext_web)
	self.cobj = http.client.client()

	--客户端的功能
	require('util.http_client').extend(self)
end

function Http:update( )
	self.cobj:poll()
end

-- 发送信息到web服务器
function Http:SendWebData(url, str_data)
	if url == "" or str_data == "" then
		outFmtError("SendWebData error: url:%s str_data:%s", url, str_data)
		return ""
	end
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
