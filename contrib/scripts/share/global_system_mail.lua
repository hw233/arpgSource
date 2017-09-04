--系统邮箱信息
local GlobalSystemMail = class('GlobalSystemMail', assert(BinLogObject))

function GlobalSystemMail:ctor()
	
end

--系统邮件个数加1
function GlobalSystemMail:Next()
	local cur_num = self:GetMailIndex()
	if cur_num >= MAX_SYSTEM_MAIL_INFO_COUNT-1 then		--礼包信息最多保留MAX_SYSTEM_MAIL_INFO_COUNT
		self:SetMailIndex(0)	
	else
		self:AddUInt32(MAX_SYSTEM_MAIL_INFO_INT_NOW_INDEX, 1)
	end
	self:AddUInt32(SYSTEM_MAIL_COUNT, 1)
end	

--礼包信息int开始下标
function GlobalSystemMail:Start()
	return SYSTEM_MAIL_INT_FIELD_BEGIN + self:GetMailIndex() * MAX_SYSTEM_MAIL_INFO_INT
end

--礼包信息string开始下标
function GlobalSystemMail:StringStart()
	return SYSTEM_MAIL_STRING_FIELD_BEGIN + self:GetMailIndex() * MAX_SYSTEM_MAIL_INFO_STRING
end

-- 获得系统邮件数量
function GlobalSystemMail:GetMailCount()
	return self:GetUInt32(SYSTEM_MAIL_COUNT)
end

--获得礼包信息当前索引
function GlobalSystemMail:GetMailIndex()
	return self:GetUInt32(MAX_SYSTEM_MAIL_INFO_INT_NOW_INDEX)
end

--设置礼包信息当前索引
function GlobalSystemMail:SetMailIndex( val)
	self:SetUInt32(MAX_SYSTEM_MAIL_INFO_INT_NOW_INDEX, val)
end

--获取礼包ID
function GlobalSystemMail:GetSystemMailID()
	return self:GetUInt32(self:Start() + SYSTEM_MAIL_INFO_INT_ID)
end

--设置礼包ID
function GlobalSystemMail:SetSystemMailID(val)
	self:SetUInt32(self:Start() + SYSTEM_MAIL_INFO_INT_ID, val)
end

--获取礼包发放时间
function GlobalSystemMail:GetSystemMailStartTime()
	return self:GetUInt32(self:Start() + SYSTEM_MAIL_INFO_INT_START_TIME)
end

--设置礼包发放时间
function GlobalSystemMail:SetSystemMailStartTime(val)
	self:SetUInt32(self:Start() + SYSTEM_MAIL_INFO_INT_START_TIME, val)
end

--获取礼包结束时间
function GlobalSystemMail:GetSystemMailEndTime()
	return self:GetUInt32(self:Start() + SYSTEM_MAIL_INFO_INT_END_TIME)
end

--设置礼包结束时间
function GlobalSystemMail:SetSystemMailEndTime(val)
	self:SetUInt32(self:Start() + SYSTEM_MAIL_INFO_INT_END_TIME, val)
end

--获取礼包类型
function GlobalSystemMail:GetSystemMailType()
	return self:GetUInt32(self:Start() + SYSTEM_MAIL_INFO_INT_TYPE)
end

--设置礼包类型
function GlobalSystemMail:SetSystemMailType(val)
	self:SetUInt32(self:Start() + SYSTEM_MAIL_INFO_INT_TYPE, val)
end

--获取礼包名称
function GlobalSystemMail:GetSystemMailName()
	return self:GetStr(self:StringStart() + SYSTEM_MAIL_INFO_STRING_NAME)
end

--设置礼包名称
function GlobalSystemMail:SetSystemMailName(val)
	self:SetStr(self:StringStart() + SYSTEM_MAIL_INFO_STRING_NAME,val)
end

--获取礼包说明
function GlobalSystemMail:GetSystemMailDesc()
	return self:GetStr(self:StringStart() + SYSTEM_MAIL_INFO_STRING_DESC)
end

--设置礼包说明
function GlobalSystemMail:SetSystemMailDesc(val)
	self:SetStr(self:StringStart() + SYSTEM_MAIL_INFO_STRING_DESC,val)
end

--获取礼包来源
function GlobalSystemMail:GetSystemMailFrom()
	return self:GetStr(self:StringStart() + SYSTEM_MAIL_INFO_STRING_FROM)
end

--设置礼包来源
function GlobalSystemMail:SetSystemMailFrom(val)
	self:SetStr(self:StringStart() + SYSTEM_MAIL_INFO_STRING_FROM,val)
end

--获取礼包物品
function GlobalSystemMail:GetSystemMailItem(pos)
	local start
	
	if(pos == nil)then
		start = self:StringStart()
	else
		start = SYSTEM_MAIL_STRING_FIELD_BEGIN + pos * MAX_SYSTEM_MAIL_INFO_STRING
	end
	
	return self:GetStr(start + SYSTEM_MAIL_INFO_STRING_ITEM)
end

--设置礼包物品
function GlobalSystemMail:SetSystemMailItem(val)
	self:SetStr(self:StringStart() + SYSTEM_MAIL_INFO_STRING_ITEM,val)
end


--添加一个系统邮件
--[[
	gift_type   : 邮件类型
	start_time  : 发放时期
	end_time    : 结束日期
	gift_name   : 邮件名称
	gift_desc   : 邮件说明
	item_config : 邮件奖励
	item_from   : 邮件来源
]]
function GlobalSystemMail:AddSystemMailInfo( gift_type, start_time, end_time, gift_name, gift_desc, item_config, item_from)
	local id = self:GetMailCount() + 1
	
	self:SetSystemMailID(id)
	self:SetSystemMailStartTime(start_time)
	self:SetSystemMailEndTime(end_time)
	self:SetSystemMailType(gift_type)

	self:SetSystemMailName(gift_name)
	self:SetSystemMailDesc(gift_desc)
	self:SetSystemMailItem(item_config)
	self:SetSystemMailFrom(item_from)
	
	self:Next()
	
	-- 遍历所有在线玩家
	app.objMgr:foreachAllPlayer(function(playerInfo)
		-- 设置序列号
		playerInfo:SetSystemMailSeq(id)
		local indx = (id-1) % MAX_SYSTEM_MAIL_INFO_COUNT
		self:PickSystemMail(playerInfo, indx)
	end)
	
	return 1
end

-- 判断是否有系统邮件
function GlobalSystemMail:checkIfHasSystemMail(playerInfo)
	local vist = playerInfo:GetSystemMailSeq()
	local id = self:GetMailCount()
	local range = id - vist
	if range > MAX_GIFTPACKS_INFO_COUNT then
		vist = id - MAX_GIFTPACKS_INFO_COUNT
	end
	
	-- 获取最后的
	for g = vist, id-1 do
		local indx = g % MAX_SYSTEM_MAIL_INFO_COUNT
		self:PickSystemMail(playerInfo, indx)
	end
	
	-- 设置序列号
	playerInfo:SetSystemMailSeq(id)
end

-- 领取系统礼包
function GlobalSystemMail:PickSystemMail(playerInfo, indx)
	print("GlobalSystemMail:PickSystemMail guid =", playerInfo:GetGuid(), "indx =", indx)
	
	local intIndx = SYSTEM_MAIL_INT_FIELD_BEGIN + indx * MAX_SYSTEM_MAIL_INFO_INT
	local strIndx = SYSTEM_MAIL_STRING_FIELD_BEGIN + indx * MAX_SYSTEM_MAIL_INFO_STRING
	
	-- 玩家自己加邮件
	local giftPack = playerInfo:getGiftPacksInfo()
	
	local gift_type   = self:GetUInt32(intIndx + SYSTEM_MAIL_INFO_INT_TYPE)
	local start_time  = self:GetUInt32(intIndx + SYSTEM_MAIL_INFO_INT_START_TIME)
	local end_time    = self:GetUInt32(intIndx + SYSTEM_MAIL_INFO_INT_END_TIME)
	
	local gift_name   = self:GetStr(strIndx + SYSTEM_MAIL_INFO_STRING_NAME)
	local gift_desc   = self:GetStr(strIndx + SYSTEM_MAIL_INFO_STRING_DESC)
	local item_config = self:GetStr(strIndx + SYSTEM_MAIL_INFO_STRING_ITEM)
	
	print(gift_type, start_time, end_time, gift_name, gift_desc, item_config)
	
	giftPack:AddGiftPacksInfo(gift_type, start_time, end_time, gift_name, gift_desc, item_config, "")
end



return GlobalSystemMail