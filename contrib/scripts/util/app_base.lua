require 'util.functions'

local AppBase = class('AppBase')


function AppBase:ctor()
	--初始化定时器
	self.cron = require('util.crontab').new()	
end

--初始化数据库
function AppBase:openMongoDB( ip,port,user,pwd,db_name,db_prefix,server_name )
	--self.dbAccess = require('util.mongo').client {host = ip, port = port}
	
	--db_name = db_name or ''
	--db_prefix = db_prefix or ''
	
	--if user and user ~= "" and pwd and pwd ~= "" and not self.dbAccess:authenticate(db_name == '' and 'admin' or db_name , user, pwd) then
	--	outFmtError('open_mongodb failed,ip:%s, port:%d, user:%s, pwd:%s, db_name:%s!',ip or 'nil', port or 0, user or 'nil', pwd or 'nil', db_name or 'nil')
	--	return -1
	--end
	--outFmtInfo('open_mongodb ip:%s port:%d ok!', ip, port)
	
	--self.dbAccess = {}
	----如果有子类接口
	-- if self.__clsDbAccess then
		-- self.__clsDbAccess.extend(self.dbAccess, {})
	-- end
	if(not self.dbAccess )then
		self.dbAccess = {}
	end
	self.dbAccess.server_name = server_name
	self.dbAccess.db_prefix = db_prefix
	self.dbAccess.trade_data = {}
end

function AppBase:update(diff)
	self.cron:update(diff)
end

--发送给日志服
function AppBase:sendToPoliced( pkt )
	local allConn = serverConnList
	return packet.internal_send(allConn:getPolicedFD(), pkt.ptr)
end

--发送给登录服
function AppBase:sendToLogind( pkt )
	local allConn = serverConnList
	return packet.internal_send(allConn:getLogindFD(), pkt.ptr)
end

--发送给应用服
function AppBase:sendToAppd( pkt )
	local allConn = serverConnList
	return packet.internal_send(allConn:getAppdFD(), pkt.ptr)
end

--发送给指定连接
function AppBase:sendToConnection( fd, pkt)
	return packet.internal_send( fd, pkt.ptr)
end

--发送给中心服
function AppBase:sendToCentd( pkt )
	return packet.internal_send(0, pkt.ptr)
end


return AppBase
