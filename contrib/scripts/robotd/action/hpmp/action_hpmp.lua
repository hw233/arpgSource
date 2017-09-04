
local ActionBase = require('robotd.action.action')
local ActionHpMp = class('ActionHpMp', ActionBase)

--获取类型
function ActionHpMp:GetType()
	return ROBOT_ACTION_TYPE_HPMP
end

--初始化变量
function ActionHpMp:Initialize(...)
	-- for i=1,MSG_LIMIT_GIFTBAG_BUY do
	-- 	self:RegOpcodeHandler(i, self.Handler)
	-- end
end

--测试包路由
function ActionHpMp:Handler(args)
	outFmtDebug("ActionHpMp:Handler %u", args.__optcode)
end

--获取类型名
function ActionHpMp:GetName()
	return 'ActionHpMp'
end

--获取栈的描述字符串
function ActionHpMp:ToString()
	return self:GetName()
end

--心跳
function ActionHpMp:Update(diff)
	--outDebug('ActionHpMp:Update')
	-- if(self.player:GetGMLevel() and self.player:GetSilver() < 1000000)then
	-- 	self.player:SendGmCommand("@招财 10000 2")
	-- 	self:SetWaitTimeInterval(1000)
	-- end
	return true
end

return ActionHpMp
