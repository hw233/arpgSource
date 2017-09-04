
ActionBase = class('ActionBase')
__action_create_index = 0
--构造函数
function ActionBase:ctor(player, father_index)
	self.player = player
	self.wait_time_interval = 0
	self.optcode_regs = {}
	self.timers = {}

	self.create_time = os.time()
	__action_create_index = __action_create_index + 1
	self.create_index = __action_create_index
	self.father_index = father_index
end

--初始化变量
function ActionBase:Initialize(...)
	assert(false)
end

--获取类型
function ActionBase:GetType()
	return ROBOT_ACTION_TYPE_NONE
end

--获取类型名
function ActionBase:GetName()
	assert(false)
end

--获取栈的描述字符串
function ActionBase:ToString()
	local x,y = self.player:GetPos()
	local mapid = self.player:GetMapID()
	local nowpos = string.format("now (%d, %d, %d)", mapid, x, y)
	local strTable = self:GetName() .. " " .. nowpos .. " "
	for k,v in pairs(self) do
		local strK = ""
		if type(k) == "string" then
			strK = k
		else
			strK = "["..k.."]"
		end
		if type(v) == "string" then
			strTable = strTable..strK.." = "..[["]]..v..[["]].." "
		elseif type(v) == "number" then
			strTable = strTable..strK.." = "..v.." "
		end
	end
	return strTable
end

--释放
function ActionBase:Finalize()
	for k, v in pairs(self.optcode_regs) do
		self.player:RemoveActionOptHandle(k, self)
	end
end

--push一个新的action到栈顶
function ActionBase:PushAction(push_action_cls, ...)
	local push_action = require(push_action_cls).new(self.player, self.create_index)
	assert(self:GetType() == push_action:GetType())
	push_action:Initialize(...)
	self.player:PushAction(push_action, self:GetName())
end

--包注册
function ActionBase:RegOpcodeHandler(optcode, handle)
	self.player:RegActionOptHandle(optcode, self, handle)
	self.optcode_regs[optcode] = handle
end

--设置等待时间，一但设置这个时间以后，本动作就会被僵住一段时间
--这个阻塞对定时器心跳无效
--单位，毫秒
function ActionBase:SetWaitTimeInterval(interval)
	self.wait_time_interval = interval
end

--等待动作栈苏醒心跳
--本方法其他地方禁止调用
function ActionBase:Wait(diff)
	if(self.wait_time_interval - diff <= 0 )then
		self.wait_time_interval = 0
		return false
	end
	self.wait_time_interval = self.wait_time_interval - diff
	return true
end

--心跳
function ActionBase:Update(diff)
	assert(false)
end

--增加一个定时器，
--（定时器名称：字符串， 回调函数：函数， 时间间隔毫秒：整数，其他参数）
function ActionBase:AddTimer(name, callback, interval, ...)
	local info = self.timers[name]
	assert(info == nil)
	info = require('robotd.action.timer').new(name, callback, interval, ...)
	self.timers[name] = info	
end

--定时器心跳
function ActionBase:TimerUpdate(diff)
	for k, v in pairs(self.timers) do
		if(v:Update(diff) == false)then
			self.timers[k] = nil
			outFmtDebug('ActionBase:TimerUpdate pop timer [%s]', v.name)
		end
	end
end

return ActionBase
