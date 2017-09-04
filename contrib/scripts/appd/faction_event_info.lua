--帮派动态信息
FactionInfo = class('FactionInfo', BinLogObject)

--帮派动态信息个数加1
function FactionInfo:Next()
	local cur_num = self:GetAttackStrutCount()
	if(cur_num >= MAX_FACTION_EVENT_INFO_COUNT-1)then		--动态信息最多保留MAX_FACTION_EVENT_INFO_COUNT
		self:SetAttackStrutCount(0)	
	else
		self:AddUInt32(MAX_FACTION_EVENT_INFO_INT_NOW_INDEX, 1)
	end
end	

--帮派留言个数加1
function FactionInfo:NextNOtice()
	local cur_num = self:GetNoticeCount()
	if(cur_num >= MAX_FACTION_EVENT_NOTICE_COUNT-1)then		--动态信息最多保留MAX_FACTION_EVENT_INFO_COUNT
		self:SetNoticeCount(0)	
	else
		self:AddUInt32(MAX_FACTION_EVENT_INFO_INT_NOTICE_INDEX, 1)
	end
end	

--帮派动态信息int开始下标
function FactionInfo:Start()
	return FACTION_EVENT_INFO_INT_FIELD_START + self:GetAttackStrutCount() * MAX_FACTION_EVENT_INFO_INT
end

--帮派动态信息string开始下标
function FactionInfo:StringStart()
	return FACTION_EVENT_INFO_STR_FIELD_START + self:GetAttackStrutCount() * MAX_FACTION_EVENT_INFO_STRING 
end

--帮派留言int开始下标
function FactionInfo:StartNotice()
	return FACTION_EVENT_INFO_INT_NOTICE_START + self:GetNoticeCount() * MAX_FACTION_EVENT_INFO_INT
end

--帮派留言string开始下标
function FactionInfo:StringStartNotice()
	return FACTION_EVENT_INFO_STR_NOTICE_START + self:GetNoticeCount() * MAX_FACTION_EVENT_INFO_STRING 
end

--获得帮派动态信息当前索引
function FactionInfo:GetAttackStrutCount()
	return self:GetUInt32(MAX_FACTION_EVENT_INFO_INT_NOW_INDEX)
end

--设置帮派动态信息当前索引
function FactionInfo:SetAttackStrutCount (val)
	self:SetUInt32(MAX_FACTION_EVENT_INFO_INT_NOW_INDEX, val)
end

--获得帮派留言当前索引
function FactionInfo:GetNoticeCount()
	return self:GetUInt32(MAX_FACTION_EVENT_INFO_INT_NOTICE_INDEX)
end

--设置帮派留言当前索引
function FactionInfo:SetNoticeCount (val)
	self:SetUInt32(MAX_FACTION_EVENT_INFO_INT_NOTICE_INDEX, val)
end

--添加一个帮派动态信息
function FactionInfo:AddFactionInfo (corpid, playerid, playername, eventTm, eventType, reserve1, reserve2, reserve3, reserve_str)
	--str部分
	self:SetFactionGuid(eventType, corpid)
	self:SetPlayerGuid(eventType, playerid)
	self:SetPlayerName(eventType, playername)
	if(reserve_str == nil)then
		reserve_str = ''
	end
	self:SetStrReserve1(eventType, reserve_str)
	--int部分
	self:SetEventType(eventType)
	if(reserve1 == nil)then
		reserve1 = 0
	end
	self:SetIntReserve1(eventType, reserve1)
	if(reserve2 == nil)then
		reserve2 = 0
	end
	self:SetIntReserve2(eventType, reserve2)
	if(reserve3 == nil)then
		reserve3 = 0
	end
	self:SetIntReserve3(eventType, reserve3)	
	self:SetEventTm(eventType, eventTm)
	self:Next()		--下移索引
end

--设置时间戳
function FactionInfo:SetEventTm (eventtype, val)	
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_TIME, val)
end

--设置event类型
function FactionInfo:SetEventType (eventtype)
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_EVENT_TYPE, eventtype)
end

--设置保留1
function FactionInfo:SetIntReserve1 (eventtype, val)
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_RESERVE1, val)
end

--设置保留2
function FactionInfo:SetIntReserve2 (eventtype, val)
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_RESERVE2, val)
end

--设置保留3
function FactionInfo:SetIntReserve3 (eventtype, val)
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_RESERVE3, val)
end

--设置帮派guid
function FactionInfo:SetFactionGuid (eventtype, val)
	self:SetStr(self:StringStart() + FACTION_EVENT_INFO_STRING_FACTION_GUID, val)
end

--设置player guid
function FactionInfo:SetPlayerGuid (eventtype, val)
	self:SetStr(self:StringStart() + FACTION_EVENT_INFO_STRING_PLAYER_GUID, val)
end

--设置玩家名字
function FactionInfo:SetPlayerName (eventtype, val)
	self:SetStr(self:StringStart() + FACTION_EVENT_INFO_STRING_PLAYER_NAME, val)
end

--设置str 保留1
function FactionInfo:SetStrReserve1 (eventtype, val)
	self:SetStr(self:StringStart() + FACTION_EVENT_INFO_STRING_RESERVE1, val)
end

--输出帮派动态信息
function FactionInfo:PrintFactionEvents (eventtype)
	for i = 0, MAX_FACTION_EVENT_INFO_COUNT-1 do
		if(self:GetStr(FACTION_EVENT_INFO_STR_FIELD_START + i * MAX_FACTION_EVENT_INFO_STRING + FACTION_EVENT_INFO_STRING_FACTION_GUID) ~= '')then
			outDebug('event time = '..self:GetUInt32(FACTION_EVENT_INFO_INT_FIELD_START + i * MAX_FACTION_EVENT_INFO_INT + FACTION_EVENT_INFO_INT_TIME))
			outDebug('event type = '..self:GetUInt32(FACTION_EVENT_INFO_INT_FIELD_START + i * MAX_FACTION_EVENT_INFO_INT + FACTION_EVENT_INFO_INT_EVENT_TYPE))
			outDebug('event corps guid = '..self:GetStr(FACTION_EVENT_INFO_STR_FIELD_START + i * MAX_FACTION_EVENT_INFO_STRING + FACTION_EVENT_INFO_STRING_FACTION_GUID))
			outDebug('event corps guid = '..self:GetStr(FACTION_EVENT_INFO_STR_FIELD_START + i * MAX_FACTION_EVENT_INFO_STRING + FACTION_EVENT_INFO_STRING_PLAYER_NAME))
			outDebug('event corps guid = '..self:GetStr(FACTION_EVENT_INFO_STR_FIELD_START + i * MAX_FACTION_EVENT_INFO_STRING + FACTION_EVENT_INFO_STRING_PLAYER_GUID))
		end
	end	
end

return FactionInfo