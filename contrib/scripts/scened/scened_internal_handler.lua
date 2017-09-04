--场景服内部协议路由
require("scened.scened_internal_pack")
local Packet = require 'util.packet'


local scenedInsternalHanlders = {}


--网络包处理方法
packet.register_on_internal_packet(function ( cid, pkt )
	local optcode = packet.optcode(pkt)
	local f = scenedInsternalHanlders[optcode]
	if f then
		doxpcall(f, Packet.new(nil, pkt))
--		pkt:delete()
	else
		--print(cid)
	end
end)
