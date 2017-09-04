
--公告信息
local GlobalNoticeMessage = class('GlobalNoticeMessage', assert(BinLogObject))

function GlobalNoticeMessage:ctor( )
end

--获得添加公告位置索引
function GlobalNoticeMessage:GetIndex(notice_type)
	return self:GetUInt32(NOTICE_MESSAGE_INT_FIELD_INDEX + notice_type)
end

function GlobalNoticeMessage:SetIndex(notice_type,val)
	self:SetUInt32(NOTICE_MESSAGE_INT_FIELD_INDEX + notice_type,val)
end

function GlobalNoticeMessage:NextIndex(notice_type)
	local count = self:GetIndex(notice_type)
	if(count == MAX_NOTICE_MESSAGE_NUMBER-1)then		--公告信息最多保留MAX_CORPSEVENT_INFO_COUNT
		self:SetIndex(notice_type,0)	
	else
		self:AddUInt32(NOTICE_MESSAGE_INT_FIELD_INDEX + notice_type, 1)
	end
end

--添加一条记录
function GlobalNoticeMessage:SetNoticeMessage(notice_type,message)
	self:SetStr(NOTICE_MESSAGE_STRING_FIELD_START + MAX_NOTICE_MESSAGE_NUMBER * notice_type + self:GetIndex(notice_type),message)
	self:NextIndex(notice_type)
end

--查询公告信息
function GlobalNoticeMessage:DoNticeMessageSelect(player, ntype)
	local data_str = ""
	local max_num = MAX_NOTICE_MESSAGE_NUMBER	-- 公告最大下发个数
	if ntype == NOTICE_MESSAGE_INFO_TYPE_KAIFUJIJIN then
		max_num = 50	-- 开服基金公告最大个数
	end
	
	local count = self:GetIndex(ntype)
	if count >= max_num then
		for i = count-max_num,count-1
		do
			local message = self:GetStr(NOTICE_MESSAGE_STRING_FIELD_START + MAX_NOTICE_MESSAGE_NUMBER * ntype + i) 
			if(message ~= "" and message ~= nil)then
				if(data_str == "")then
					data_str = string.format("%s",message)
				else
					data_str = string.format("%s|%s",data_str,message)
				end
			end
		end
	else
		if self:GetStr(NOTICE_MESSAGE_STRING_FIELD_START + MAX_NOTICE_MESSAGE_NUMBER * ntype + MAX_NOTICE_MESSAGE_NUMBER-1) == "" then
			for i = 0,count-1
			do
				local message = self:GetStr(NOTICE_MESSAGE_STRING_FIELD_START + MAX_NOTICE_MESSAGE_NUMBER * ntype + i) 
				if(message ~= "" and message ~= nil)then
					if(data_str == "")then
						data_str = string.format("%s",message)
					else
						data_str = string.format("%s|%s",data_str,message)
					end
				end
			end
		else
			for i = MAX_NOTICE_MESSAGE_NUMBER-max_num+count, MAX_NOTICE_MESSAGE_NUMBER-1
			do
				local message = self:GetStr(NOTICE_MESSAGE_STRING_FIELD_START + MAX_NOTICE_MESSAGE_NUMBER * ntype + i) 
				if(message ~= "" and message ~= nil)then
					if(data_str == "")then
						data_str = string.format("%s",message)
					else
						data_str = string.format("%s|%s",data_str,message)
					end
				end
			end
			for i = 0,count-1
			do
				local message = self:GetStr(NOTICE_MESSAGE_STRING_FIELD_START + MAX_NOTICE_MESSAGE_NUMBER * ntype + i) 
				if(message ~= "" and message ~= nil)then
					if(data_str == "")then
						data_str = string.format("%s",message)
					else
						data_str = string.format("%s|%s",data_str,message)
					end
				end
			end
		end
	end
	player:call_notice_message_select_result( ntype, data_str )
end	
	
	
return GlobalNoticeMessage
