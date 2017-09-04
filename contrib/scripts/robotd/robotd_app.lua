RobotdApp = class('RobotdApp')

function RobotdApp:ctor()
	--实例化http客户端
	self.http = require('robotd.http').new()

	--key:account, val:robotd_context
	robot_map = {}

	--用于崩溃打印用
	self.__update_player = nil

	--当新对象来的时候
	objects.registerAttachObject(function(account, guid, ptr )
		local robot = robot_map[account]
		if(robot == nil)then
			return
		end
		local ret = doxpcall(robot.objMgr.onNewObj, robot.objMgr, guid, ptr)
		assert(ret)
	end)

	-----------------------------------------
	--远程创建新对象后触发
	--当新对象已经应用到对象更新并且被附加到对象表之后触发
	objects.registerAfterAttachObject( function(account, ptr)
		local robot = robot_map[account]
		if(robot == nil)then
			return
		end
		local o = robot.objMgr:fromPtr(ptr)
		if(o)then
			local ret = doxpcall(robot.objMgr.afterAttachObject, robot.objMgr, o)
			assert(ret)
		end
	end)

	--当对象释放的时候
	objects.registerReleaseObject(function (account, guid, ptr )
		local robot = robot_map[account]
		if(robot == nil)then
			return
		end
		local ret = doxpcall(robot.objMgr.onReleaseObject, robot.objMgr, guid, ptr)
		assert(ret)
	end)

end



--机器人连接到游戏网关服
function RobotdApp:RobotLoginOk(account, fd, robot_ptr)
	assert(robot_map[account] == nil)
	local robot = require("robotd.context.robotd_context").new(account, fd, robot_ptr)
	robot_map[account] = robot
end

function RobotdApp:RobotRespawn(account)
	local robot = robot_map[account]
	robot:Reset()
end

--机器人登出游戏服
function RobotdApp:RobotLogout(account, fd)
	local robot = robot_map[account]
	if(robot == nil)then
		return
	end
	robot:Logout()
	robot_map[account] = nil
end

--机器人连接跨服成功
function RobotdApp:RobotWarConn(account, fd)
	local robot = robot_map[account]
	assert(robot)
	--assert(robot.fd == fd)
	robot:RobotWarConn()
end

--机器人断开跨服连接
function RobotdApp:RobotWarClose(account, fd)
	local robot = robot_map[account]
	assert(robot)
	assert(robot.fd == fd)
	robot:RobotWarClose()
end

--包路由
function RobotdApp:PacketHandle(account, fd, pkt)
	local robot = robot_map[account]
	assert(robot)
	assert(robot.fd == fd)
	local ret = doxpcall(robot.Handle, robot, pkt)
	assert(ret)
end

--机器人准备开始跨服
function RobotdApp:RobotKuafuReady(account, fd)
	local robot = robot_map[account]
	assert(robot)
	assert(robot.fd == fd)
	local ret = doxpcall(robot.KuafuReady, robot, pkt)
	assert(ret)
end

function RobotdApp:Update(diff)
	if self.http then
		--奔跑吧,http服务
		self.http:update()
	end

	for	k,v in pairs(robot_map) do
		self.__update_player = v
		setUpdateRobot(v.ptr)
		v:Update(diff)
	end
	self.__update_player = nil
	setUpdateRobot(nil)
end

--机器人心跳
function RobotdApp:RobotUpdate(diff)	
	local ret = doxpcall(RobotdApp.Update, self, diff)
	assert(ret)
end

--打印调试信息
function RobotdApp:OutDebugInfo()
	--todo 打印环境变量

	--打印崩溃的玩家数据
	if(self.__update_player == nil)then
		outString("update_player is nil")
		return
	end
	outString(self.__update_player:GetDebugInfo())
end

--获取栈
function RobotdApp:GetRobotStack(account)
	local robot = robot_map[account]
	if(robot == nil)then
		return "can`t find this account:"..account
	end
	return robot:OutDebugInfo()
end

return RobotdApp