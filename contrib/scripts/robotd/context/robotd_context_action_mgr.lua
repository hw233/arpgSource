
ROBOT_ACTION_TYPE_NONE					= 0
ROBOT_ACTION_TYPE_APPD					= 1	--应用服部分
ROBOT_ACTION_TYPE_SCENED				= 2	--场景服部分
ROBOT_ACTION_TYPE_HPMP					= 3	--补给部分

__MAX_ACTION_LIST_SIZE__				= 20 --动作栈不能超过这个数量

--行为栈管理器初始化
function PlayerInfo:ActionInit()
	--应用服业务相关
	self.action_appd_impl = {}
	--场景服业务相关
	self.action_scened_impl = {}
	--补给相关
	self.action_hpmp_impl = {}
	--准备加入的action队列
	self.action_push_queue = {}
	--包注册到action的容器
	self.action_opt_handle = {}

	self.action_appd_impl[1] = require('robotd.action.appd.action_appd').new(self)
	self.action_appd_impl[1]:Initialize()
	self.action_scened_impl[1] = require('robotd.action.scened.action_scened').new(self)
	self.action_scened_impl[1]:Initialize()
	self.action_hpmp_impl[1] = require('robotd.action.hpmp.action_hpmp').new(self)
	self.action_hpmp_impl[1]:Initialize()
end

--心跳
function PlayerInfo:ActionUpdate(diff)
	--跳应用服业务
	self:ActionListUpdate(self.action_appd_impl, diff)
	--跳场景服业务
	self:ActionListUpdate(self.action_scened_impl, diff)
	--跳补给业务
	self:ActionListUpdate(self.action_hpmp_impl, diff)
	--新增action进栈
	self:PushActionQueueUpdate()
end

--通用的心跳流程
function PlayerInfo:ActionListUpdate(action_list, diff)
	if(next(action_list) == nil)then
		return
	end
	assert(#action_list <= __MAX_ACTION_LIST_SIZE__)
	local action = action_list[#action_list]
	--定时器心跳
	action:TimerUpdate(diff)
	--阻塞等待
	if(action:Wait(diff))then
		return
	end
	--正常心跳
	local result, p = action:Update(diff)
	if(result)then
		return
	end
	--一定得有返回值
	if(p == nil)then
		outFmtInfo("PlayerInfo:ActionListUpdate p == nil %s", action:ToString())
		assert(false)
	end
	table.remove(action_list)
	action:Finalize()
	--outFmtDebug("pop action %s %u %s", self:GetGuid(), action:GetType(), action:ToString())
end

--新增action进栈
function PlayerInfo:PushActionQueueUpdate()	
	while(next(self.action_push_queue) ~= nil)do
		local action = self.action_push_queue[1]
		table.remove(self.action_push_queue, 1)
		if(action:GetType() == ROBOT_ACTION_TYPE_APPD)then
			table.insert(self.action_appd_impl, action)
		elseif(action:GetType() == ROBOT_ACTION_TYPE_SCENED)then
			table.insert(self.action_scened_impl, action)
		elseif(action:GetType() == ROBOT_ACTION_TYPE_HPMP)then
			table.insert(self.action_hpmp_impl, action)
		else
			assert(false)
		end
		--outFmtDebug("push action %s", action:ToString())
	end
end

--push一个新的action，进缓冲队列
function PlayerInfo:PushAction(action, father_name)
	table.insert(self.action_push_queue, action)
	--outFmtDebug("push action %s %u %s , %s", self:GetGuid(), action:GetType(), action:GetName(), father_name)
end

--包注册
function PlayerInfo:RegActionOptHandle(optcode, action, handle)
	local handle_tab = self.action_opt_handle[optcode]
	if(handle_tab == nil)then
		handle_tab = {}
		self.action_opt_handle[optcode] = handle_tab
	end
	handle_tab[action] = handle
	--outFmtDebug("PlayerInfo:RegActionOptHandle %u, %s", optcode, action:ToString())
end

--取消包注册
function PlayerInfo:RemoveActionOptHandle(optcode, action)
	local handle_tab = self.action_opt_handle[optcode]
	assert(handle_tab)
	handle_tab[action] = nil
	if(next(handle_tab) == nil)then
		self.action_opt_handle[optcode] = {}
	end
end

--包路由
function PlayerInfo:ActionOptHandle(args)
	local handle_tab = self.action_opt_handle[args.__optcode]
	if(handle_tab == nil)then
		return
	end
	for k, v in pairs(handle_tab) do
		if(v ~= nil)then
			v(k, args)
		end
	end
end

--打印动作调试信息
function PlayerInfo:GetActionDebugInfo()
	local result = ""
	for i=#self.action_appd_impl, 1, -1 do
		result = result .. string.format("appd action %u : %s\n", i, self.action_appd_impl[i]:ToString())
	end
	for i=#self.action_scened_impl, 1, -1 do
		result = result .. string.format("scened action %u : %s\n", i, self.action_scened_impl[i]:ToString())
	end

	return result
end

--清空所有场景栈，压入跨服主栈
function PlayerInfo:ActionWarConn()
	while(next(self.action_scened_impl))do
		local action = self.action_scened_impl[#self.action_scened_impl]
		table.remove(self.action_scened_impl)
		action:Finalize()
	end
	self.action_scened_impl[1] = require('robotd.action.scened.action_scened_kuafu').new(self)
	self.action_scened_impl[1]:Initialize()
end

--清空所有场景栈，压入本服场景主栈
function PlayerInfo:ActionWarClose()
	while(next(self.action_scened_impl))do
		local action = self.action_scened_impl[#self.action_scened_impl]
		table.remove(self.action_scened_impl)
		action:Finalize()
	end
	self.action_scened_impl[1] = require('robotd.action.scened.action_scened').new(self)
	self.action_scened_impl[1]:Initialize()
end