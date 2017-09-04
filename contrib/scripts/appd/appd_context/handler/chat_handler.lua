-- 私聊
function PlayerInfo:Handle_Chat_Whisper( pkt )
	local guid, content = pkt.guid, pkt.content

	if not self:CheckChatLimit(CHAT_TYPE_WHISPER, content) then
		return
	end
	
	if not app.objMgr:IsPlayerGuid(guid) then
		return
	end
	
	local socialMgr = self:getSocialMgr()
	if not socialMgr:isFriend(guid) then
		outFmtError("Handle_Chat_Whisper target not friend %s",guid)
		return
	end
	
	self:SendWhisperChat(guid, content)
end

--[[
	CHAT_TYPE_SYSTEM		= 0,		/*系统*/
	
	CHAT_TYPE_WORLD			= 1,		/*世界*/
	CHAT_TYPE_FACTION		= 2,		/*帮派*/
	CHAT_TYPE_CURRENT		= 3, 		/*当前(场景) */
	CHAT_TYPE_HORM			= 4,		/*喇叭*/	
	CHAT_TYPE_GROUP			= 5,		/*队伍*/
	
	CHAT_TYPE_WHISPER		= 6,		/*私聊*/
--]]

-- 不同频道聊天
function PlayerInfo:Handle_Chat_By_Channel(pkt)
	local channel = pkt.channel
	local content = pkt.content
	
	-- 判断频道是否合法
	if channel < 1 or channel >= MAX_CHAT_TYPE or channel == CHAT_TYPE_WHISPER then
		return
	end
	
	-- 判断聊天内容是否合法
	if not self:CheckChatLimit(channel, content) then
		return
	end

	if channel == CHAT_TYPE_WORLD then
		self:SendWorldChat(content)
	elseif channel == CHAT_TYPE_FACTION then
		self:SendFactionChat(content)
	elseif channel == CHAT_TYPE_CURRENT then
		self:SendNearChat(content)
	elseif channel == CHAT_TYPE_HORM then
		self:SendHornChat(content)
	elseif channel == CHAT_TYPE_GROUP then
		self:SendGroupChat(content)
	end
end

--[[
	PLAYER_FIELD_DECLINE_CHANNEL_BYTE0	= PLAYER_FIELD_TRIAL_LAYERS + 1,			//拒绝频道A(0系统，1帮派，2组队，3世界)
	PLAYER_FIELD_DECLINE_CHANNEL_BYTE1	= PLAYER_FIELD_DECLINE_CHANNEL_BYTE0 + 1,	//拒绝频道B(0:附近)
]]
function PlayerInfo:Handle_Msg_Decline(pkt)
	local value0 = pkt.value0
	local value1 = pkt.value1
	
	self:SetUInt32(PLAYER_FIELD_DECLINE_CHANNEL_BYTE0, value0)
	self:SetUInt32(PLAYER_FIELD_DECLINE_CHANNEL_BYTE1, value1)
end


-- 屏蔽某人
function PlayerInfo:Handle_Block_Chat(pkt)
	local guid = pkt.guid
	
	if not app.objMgr:IsPlayerGuid(guid) then
		return
	end
	
	-- 屏蔽的人一定要在线
	local blocker = app.objMgr:getObj(guid)
	if not blocker or guid == self:GetGuid() then return end
	
	local count = self:GetBlockCounts()
	if count >= MAX_CHAT_BLOCK_COUNT then
		-- TODO: 提示屏蔽列表已满
		return
	end
	
	local emptyIndx = -1
	for i = PLAYER_STRING_FIELD_BLOCK_START, PLAYER_STRING_FIELD_BLOCK_END-1 do
		local info = self:GetStr(i)
		if string.find(info, guid) then
			return
		elseif info == "" and emptyIndx == -1 then
			emptyIndx = i
		end
	end
	
	local info = string.format("%d|%d|%s|%d|%s|%s", blocker:GetGender(), blocker:GetLevel(), blocker:GetName(), blocker:GetVIP(), blocker:GetFactionName(), guid)
	self:SetStr(emptyIndx, info)
	self:AddBlockCounts()
end

-- 取消屏蔽
function PlayerInfo:Handle_Cancel_Block_Chat(pkt)
	local indx = pkt.indx
	if indx < 0 or indx >= MAX_CHAT_BLOCK_COUNT then
		return
	end
	local info = self:GetStr(PLAYER_STRING_FIELD_BLOCK_START + indx)
	if info ~= "" then
		self:SetStr(PLAYER_STRING_FIELD_BLOCK_START + indx, "")
		self:SubBlockCounts()
	end
	
end