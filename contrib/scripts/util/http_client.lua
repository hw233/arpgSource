local HttpClient = {}

function HttpClient.extend( self )
	assert(self.cobj)
	for k,v in pairs(HttpClient) do self[k] = v end
end

function HttpClient:tryDecode( body )
	local o = nil
	local status, err = pcall(function (  )
		o = json.decode(body)
	end)
	if not status then
		outError(err)
		return nil
	end
	return o
end

--允许配置文件内的域名包含或者不包含'/'，但是page一定要使用绝对路径,即一定包含'/'
function HttpClient:getUrl( host_port, page )	
	--切割最后一个字符
	local len1 = string.len(host_port)
	local port_end = string.sub(host_port,len1,len1)
	--切割第一个字符
	local page_begin =  string.sub(page,1,1)
	--都有杠	
	if(port_end=='/' and page_begin=='/')then
		host_port = string.sub(host_port,1,len1-1)		
	--都没杠
	elseif(port_end~='/' and page_begin~='/')then
		host_port=host_port..'/'
	end	
	
	local url = host_port..page
	return url
end

--cb(status_code, response)
function HttpClient:async_post( url, data, cb )
	return self.cobj:async_post(url, data, cb)
end

--两个返回值 status_code, response
function HttpClient:post( url, data )
	return self.cobj:post(url, data)
end

return HttpClient
