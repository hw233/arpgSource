--系统邮箱信息
local GlobalOfflineMail = class('GlobalOfflineMail', assert(BinLogObject))

function GlobalOfflineMail:ctor()
	
end

--礼包信息个数加1
function GlobalOfflineMail:Next()
	local cur_num = self:GetAttackStrutCount()
	if(cur_num >= MAX_OFFLINE_MAIL_INFO_COUNT-1)then		--离线礼包最多保留MAX_OFFLINE_MAIL_INFO_COUNT
		self:SetAttackStrutCount(0)	
	else
		self:AddUInt32(MAX_GIFTPACKS_INFO_INT_NOW_INDEX, 1)
	end
end	

--礼包信息int开始下标
function GlobalOfflineMail:Start()
	return GIFTPACKS_INT_FIELD_BEGIN + self:GetAttackStrutCount() * MAX_GIFTPACKS_INFO_INT
end

--礼包信息string开始下标
function GlobalOfflineMail:StringStart()
	return GIFTPACKS_STRING_FIELD_BEGIN + self:GetAttackStrutCount() * MAX_GIFTPACKS_INFO_STRING 
end

--获得礼包信息当前索引
function GlobalOfflineMail:GetAttackStrutCount()
	return self:GetUInt32(MAX_GIFTPACKS_INFO_INT_NOW_INDEX)
end

--设置礼包信息当前索引
function GlobalOfflineMail:SetAttackStrutCount( val)
	self:SetUInt32(MAX_GIFTPACKS_INFO_INT_NOW_INDEX, val)
end

--获取礼包ID
function GlobalOfflineMail:GetGiftPacksID()
	return self:GetUInt32(self:Start() + GIFTPACKS_INFO_INT_ID)
end

--设置礼包ID
function GlobalOfflineMail:SetGiftPacksID(val)
	self:SetUInt32(self:Start() + GIFTPACKS_INFO_INT_ID, val)
end

--获取礼包发放时间
function GlobalOfflineMail:GetGiftPacksStartTime()
	return self:GetUInt32(self:Start() + GIFTPACKS_INFO_INT_START_TIME)
end

--设置礼包发放时间
function GlobalOfflineMail:SetGiftPacksStartTime(val)
	self:SetUInt32(self:Start() + GIFTPACKS_INFO_INT_START_TIME, val)
end

--获取礼包结束时间
function GlobalOfflineMail:GetGiftPacksEndTime()
	return self:GetUInt32(self:Start() + GIFTPACKS_INFO_INT_END_TIME)
end

--设置礼包结束时间
function GlobalOfflineMail:SetGiftPacksEndTime(val)
	self:SetUInt32(self:Start() + GIFTPACKS_INFO_INT_END_TIME, val)
end

--获取礼包类型
function GlobalOfflineMail:GetGiftPacksType()
	return self:GetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE,0)
end

--设置礼包类型
function GlobalOfflineMail:SetGiftPacksType(val)
	self:SetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE, 0 ,val)
end

--获取礼包领取状态
function GlobalOfflineMail:GetGiftPacksReceive()
	return self:GetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE,1)
end

--设置礼包领取状态
function GlobalOfflineMail:SetGiftPacksReceive(val)
	self:SetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE, 1 ,val)
end

--获取礼包名称
function GlobalOfflineMail:GetGiftPacksName()
	return self:GetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_NAME)
end

--设置礼包名称
function GlobalOfflineMail:SetGiftPacksName(val)
	self:SetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_NAME,val)
end

--获取礼包说明
function GlobalOfflineMail:GetGiftPacksDesc()
	return self:GetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_DESC)
end

--设置礼包说明
function GlobalOfflineMail:SetGiftPacksDesc(val)
	self:SetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_DESC,val)
end

--获取礼包所属
function GlobalOfflineMail:GetGiftPacksFrom()
	return self:GetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_FROM)
end

--设置礼包所属
function GlobalOfflineMail:SetGiftPacksFrom(val)
	self:SetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_FROM,val)
end

--获取礼包物品
function GlobalOfflineMail:GetGiftPacksItem(pos)
	local start
	if(pos == nil)then
		start = self:StringStart()
	else
		start = GIFTPACKS_STRING_FIELD_BEGIN + pos * MAX_GIFTPACKS_INFO_STRING
	end
	
	return self:GetStr(start + GIFTPACKS_INFO_STRING_GIFT_ITEM)
end

--设置礼包物品
function GlobalOfflineMail:SetGiftPacksItem(val)
	self:SetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_ITEM,val)
end

--[[
SYSTEM_MAIL_INFO_INT_ID = 0	-- 礼包ID
SYSTEM_MAIL_INFO_INT_START_TIME = 1	-- 发放时间
SYSTEM_MAIL_INFO_INT_END_TIME = 2	-- 结束时间
SYSTEM_MAIL_INFO_INT_TYPE = 3	-- 礼包类型
MAX_SYSTEM_MAIL_INFO_INT = 4
SYSTEM_MAIL_INFO_STRING_NAME = 0	-- 系统邮件名称
SYSTEM_MAIL_INFO_STRING_DESC = 1	-- 系统邮件说明
SYSTEM_MAIL_INFO_STRING_FROM = 2	-- 系统邮件出处，当玩家赠送时填写玩家名字，默认为空，系统赠送
SYSTEM_MAIL_INFO_STRING_ITEM = 3	-- 礼包物品集合
MAX_SYSTEM_MAIL_INFO_STRING = 4
]]

return GlobalOfflineMail