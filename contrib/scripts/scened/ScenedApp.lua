require('util.functions')
local protocols = require('share.protocols')
local ScenedApp = class('ScenedApp',require('util.app_base'))

function ScenedApp:ctor( )
	super(self)	
	
	self.objMgr = require('scened.scened_obj_manager').new()
	self.http = require('scened.ScenedHttp').new()
end

function ScenedApp:update( diff )
	if self.http then
		--奔跑吧,http服务
		self.http:update()
	end
	--update函数被子类重写了,只好手工再调用
	self.super.update(self, diff)
end

--发公告
function ScenedApp:SendNotice( id, content, data )
	local data = data or ""
	local content = content or ""
	call_appd_send_notice(id, content, data)
end

--发聊天信息
function ScenedApp:SendChat( c_type, guid, content, to_guid , to_name)
	local to_guid = to_guid or ""
	local to_name = to_name or ""
	call_appd_send_chat(c_type, guid, content, to_guid, to_name)
end

--场景发送全地图通知包
function ScenedApp:CallOptResult(map_ptr, typ, reason, data)
	if type(data) == 'table' then
		data = string.join('|', data)
	else
		data = tostring(data) or ''
	end
	--print("data = ", data)
	local pkt = protocols.pack_operation_failed(typ, reason, data)
	self:BroadcastMap(map_ptr, pkt)
	pkt:delete()
end

--map广播包
function ScenedApp:BroadcastMap(map_ptr, pkt)
	mapLib.BroadcastMap(map_ptr, pkt.ptr)
end


--grid广播包
function ScenedApp:Broadcast(unit, pkt)
	mapLib.Broadcast(unit.ptr, pkt.ptr)
end

return ScenedApp
