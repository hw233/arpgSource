-- 聊天限制
chat_limit = {}
-- 最后聊天时间
last_chat_time = {}
-- 聊天时间
limit_t = 0
-- 聊天等级次数限制表
LIMIT_LEVEL_COUNT_CONFIG = {
-- 等级(10倍)	次数
	[0] = 50,
	[1] = 50,
	[2] = 50,
	[3] = 50,
	[4] = 80,
	[5] = 100,
	[6] = 120,
	[7] = 150,
	[8] = 180,
	[9] = 200,
}

-- 发送系统聊天信息
function PlayerInfo:SendSystemChat(content)
	if self:isDeclineSystemMsg() then
		return
	end
	self:call_send_chat (CHAT_TYPE_SYSTEM ,0 ,"" ,0 ,0 ,0 ,0 ,content, "", self:GetFactionId ())
end

-- 发送世界聊天信息
function PlayerInfo:SendWorldChat(content)
	-- 禁言
	if self:IsGag() then
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_RESULT_IS_GAG)
		return
	end
	
	-- 处理GM命令
	if string.sub(content,1,1) == '@' then
		self:GmCommand(content)
		return
	end
	
	-- 写下日志
	WriteChatLog(self:GetGuid(), CHAT_TYPE_WORLD, "", "", content, self:GetLevel(), self:GetGmNum())
	--屏蔽词
	if self:GetGmNum() == 0 then
		content = fuckPingBi(content)
	end
	
	-- 转义解析
	local valid
	valid, content = ChatMsgParser(self, content)
	if not valid then return end
	
	local faction_guid = self:GetFactionId ()
	-- 广播
	app.objMgr:foreachAllPlayer(function(player)
		if not player:isDeclineWorldMsg() then
			player:call_send_chat (CHAT_TYPE_WORLD ,self:GetGuid() ,0 ,self:GetName() ,self:GetVIP() ,0 ,self:GetLevel() ,self:GetGender() ,content, "", faction_guid)
		end
	end)
end


-- 发送帮派聊天信息
function PlayerInfo:SendFactionChat(content)

	-- 写下日志
	WriteChatLog(self:GetGuid(), CHAT_TYPE_FACTION, "", "", content, self:GetLevel(), self:GetGmNum())
	--屏蔽词
	if self:GetGmNum() == 0 then
		content = fuckPingBi(content)
	end
	--//过滤
--	content = ChatMsgFilter(content)
	--//加标识
--	content = ChatMsgAddSing(content, self:GetFalseGM(), self:GetGirlGM())
	
	local faction_guid = self:GetFactionId()
	if faction_guid == "" then
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_RESULT_NO_FACTION)
		return
	end
	
	local faction = app.objMgr:getObj(faction_guid)
	if faction == nil then
		return
	end
	
	local allPlayers = faction:GetFactionAllMemberPtr()
	for _, player in pairs(allPlayers) do
		if not player:isDeclineFactionMsg() then
			player:call_send_chat (CHAT_TYPE_FACTION ,self:GetGuid() ,0 ,self:GetName() ,self:GetVIP() ,0 ,self:GetLevel() ,self:GetGender() ,content, "", faction_guid)
		end
	end
end

-- 当前聊天
function PlayerInfo:SendNearChat(content)
	
	WriteChatLog(self:GetGuid(), CHAT_TYPE_CURRENT, "", "", content, self:GetLevel(), self:GetGmNum())
	--屏蔽词
	content = fuckPingBi(content)
	--发送到场景服
	self:CallScenedDoSomething(APPD_SCENED_NEAR_BY_CHAT, 0, content)
end

-- 发送喇叭消息
function PlayerInfo:SendHornChat(content)
	if self:GetMoney(MONEY_TYPE_GOLD_INGOT) < config.laba_use_need_money then
		return
	end
	-- 扣元宝
	if not self:SubMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_USE_LABA, config.laba_use_need_money) then
		return
	end
	--屏蔽词
	content = fuckPingBi(content)
	--//过滤
--	content = ChatMsgFilter(content)
	--//加标识
--	content = ChatMsgAddSing(content, self:GetFalseGM(), self:GetGirlGM())

	local faction_guid = self:GetFactionId()
	-- 广播
	app.objMgr:foreachAllPlayer(function(player)
		player:call_send_chat (CHAT_TYPE_HORM ,self:GetGuid() ,0 ,self:GetName() ,self:GetVIP() ,0 ,self:GetLevel() ,self:GetGender() ,content, "", faction_guid)
	end)
	-- 写下日志
	WriteChatLog(self:GetGuid(), CHAT_TYPE_HORM, "", "", content, self:GetLevel(), self:GetGmNum())
end

-- 发送队伍聊天信息
function PlayerInfo:SendGroupChat(content)
	--[[
	local groupID = self:GetGroupId()
	if groupID == "" then
		self:CallOptResult(OPRATE_TYPE_GROUP,ACCACK_PACKET_TYPE_DATA)
		return
	end
	
	--屏蔽词
	content = fuckPingBi(content)
	
	local group = app.objMgr:getObj(groupID)
	if not group then 
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_RESULT_NO_GROUP)
		return 
	end
	
	local guid_list = group:GetGroupAllMemberTable()
	local faction_guid = self:GetFactionId()
	for _,guid in pairs(guid_list) do
		local player = app.objMgr:getObj(guid)
		if player then
			player:call_send_chat (CHAT_TYPE_GROUP ,self:GetGuid() ,0 ,self:GetName() ,self:GetVIP() ,0 ,self:GetLevel() ,self:GetGender() ,content, "", faction_guid)
		end
	end
	WriteChatLog(self:GetGuid(), CHAT_TYPE_GROUP, "", "", msg, self:GetLevel(), self:GetGmNum())
	--]]
end

-- 发送私聊信息
function PlayerInfo:SendWhisperChat(guid, content)
	local recipient = app.objMgr:getObj(guid)

	if not recipient then
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_RESULT_NOT_PLAYER)
		return
	end
	
	--屏蔽词
	content = fuckPingBi(content)
	
	if guid ~= self:GetGuid() then
		--私聊后做点什么
		self:DoAfterChatWhisper(recipient)
		recipient:call_send_chat (CHAT_TYPE_WHISPER ,self:GetGuid() ,0 ,self:GetName() ,self:GetVIP() ,0 ,self:GetLevel() ,self:GetGender() ,content, guid, self:GetFactionId())
		WriteChatLog(self:GetGuid(), CHAT_TYPE_WHISPER, recipient:GetGuid(), recipient:GetName(), content, self:GetLevel(), self:GetGmNum())
	end
	self:call_send_chat (CHAT_TYPE_WHISPER ,self:GetGuid() ,0 ,self:GetName() ,self:GetVIP() ,0 ,self:GetLevel() ,self:GetGender() ,content, guid, self:GetFactionId())
	
end

--gm命令入口
function PlayerInfo:GmCommand(str)
	local command = str
	local tokens = lua_string_split(str, ' ')
	local gm_str = tokens[1]
	local size_t = #tokens
	local gm_key = GetGmCommandKey(gm_str)
	if(gm_key == GM_COMMAND_JIULONGCHAO)then	--//"@八仙"
		if size_t == 1 then
			return
		end
		local bx_tokens = lua_string_split(command, '|')
		if(#bx_tokens ~= 9)then
			return
		end
		local at_tokens = lua_string_split(tokens[1], ' ')
		if(#at_tokens ~= 2)then
			return
		end
		WriteClientInfoLog(self:GetGuid(), at_tokens[2], bx_tokens[2], bx_tokens[3], bx_tokens[4], bx_tokens[5], bx_tokens[6], bx_tokens[7], bx_tokens[8], bx_tokens[9])
		return
	end
	
	outFmtInfo("player %s send gm command : %s", self:GetGuid(), str)
	local gm_level = self:GetGMLevel()
	local whisper_str = "Please do not enter illegal characters"
	if(gm_level < GM_LEVEL_3)then
		outFmtInfo("权限不足")
		--self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
		return
	end
	-- //发给其他服务器
	call_gm_command(self:GetGuid(), command)
	
	if(gm_key == GM_COMMAND_SUCAI)then		-- @素材
		if(gm_level < GM_LEVEL_2)then
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
			return
		end
		call_opt_command(0, 0, "reload_template")
		
	elseif gm_key == GM_COMMAND_CUSTOM then	--@CUSTOM
		-- 应用服需要加的
		-- 发一套装备和武器, 背包中在需要一个替换衣服
		local vv = {
			{
				10001,
				10002,
				10003,
				10004,
				10005,
				10006,
				10007,
				10008,
				10009,
				10010,
			
				10022
			},
			{
				10011,
				10012,
				10013,
				10014,
				10015,
				10016,
				10017,
				10018,
				10019,
				10020,
			
				10032
			},
		
		}
		
		
		local gender = self:GetGender()
		local entrys = vv[gender]
		local itemMgr = self:getItemMgr()	
		
		for _, id in pairs(entrys) do
			itemMgr:addItem(id,1,1,true,true,0,0)
		end
		
		-- 穿戴装备和武器
		for i = 1, #entrys-1 do
			itemMgr:exchangePos(BAG_TYPE_MAIN_BAG, i-1, BAG_TYPE_EQUIP, tb_item_template[entrys[i]].pos)
		end
		
		-- 激活神兵和装备
		itemMgr:addItem(101,10,1,true,true,0,0)
		local divineId = 1
		self:DivineActive(divineId)
		self:switchDivine(divineId)
		
	elseif(gm_key == GM_COMMAND_JIAOBEN)then		-- @脚本
		--[[
		if(gm_level < GM_LEVEL_3)then
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
			return
		end
		]]
		local temp = "reload_script"
		for i=2, size_t do
			temp = temp..','..tokens[i]
		end
		call_opt_command(0, 0, temp)
	elseif(gm_key == GM_COMMAND_TI)then			-- @踢
		if(gm_level < GM_LEVEL_1)then
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
			return
		end
		if tokens[2] then
			-- 广播
			app.objMgr:foreachAllPlayer(function(player)
				if(player:GetName() == tokens[2])then
					player:Close(PLAYER_CLOSE_OPERTE_APPD_ONE4)
				end
			end)
		end
	elseif(gm_key == GM_COMMAND_ZHAOCAI)then		-- @招财
		if(gm_level < GM_LEVEL_1)then
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
			return
		end
		if tokens[2] then
			local m_type = 3
			if tokens[3] then
				m_type = math.fmod(tonumber(tokens[3]), MAX_MONEY_TYPE)
			end
			local val = tonumber(tokens[2])
			if val > 0 then
				self:AddMoney(m_type, MONEY_CHANGE_GM_COMMAND, val)
			elseif val < 0 then
				self:SubMoney(m_type, MONEY_CHANGE_GM_COMMAND, math.abs(val))
			end
		else
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), "{@ lucky number type [currency] }")
		end
	elseif(gm_key == GM_COMMAND_PINGBICI)then		-- @屏蔽词
		if(gm_level < GM_LEVEL_3)then
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
			return
		end
		loadFuckPingBi()
	elseif(gm_key == GM_COMMAND_PAIHANGBANG)then		-- @排行榜
		for i=0, MAX_RANK_TYPE-1 do
			clearRankTask(i)
		end
	elseif(gm_key == GM_COMMAND_RETURN_DATA)then		-- @回档数据
		self:Close(PLAYER_CLOSE_OPERTE_APPD_ONE5, "")
	elseif(gm_key == GM_COMMAND_PRINT_OBJECT)then		-- @打印对象
		if(gm_level < GM_LEVEL_3)then
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
			return
		end
		local temp = string.format("print_object,%s,%s,%s", tokens[2] or ".*", tokens[3] or "0", tokens[4] or "0")
		call_opt_command(0, 0, temp)
	elseif(gm_key == GM_COMMAND_TEST_HEFU)then		-- @合服测试
		
	elseif(gm_key == GM_COMMAND_MEMORY_RECOVERY)then		-- @内存回收
		call_opt_command(0, 0, "memory_recovery")
	elseif(gm_key == GM_COMMAND_RESTORE_SYSTEM)then		-- @后台命令
		if(gm_level < GM_LEVEL_3)then
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
			return
		end
		local temp = "restore_system"
		for i=2, size_t do
			temp = temp..','..tokens[i]
		end
		call_opt_command(0, 0, temp)
	elseif(gm_key == 993)then
		local faction = app.objMgr:getObj(self:GetFactionId())
		if faction == nil then
			return
		end
		local building_id = 500 + faction:GetBuildingLvByType(FACTION_BUILDING_TYPE_SKILL)
		if faction:GetBuildingLvByType(FACTION_BUILDING_TYPE_SKILL) == 5 then
			return
		end
		faction:ReplaceBuildingId(building_id,building_id + 1)
		faction:ResetAllMemberFactionSkill()
	elseif(gm_key == 994)then
		local pkt = {}
		pkt.opt_type = tonumber(tokens[2])
		pkt.reserve_int1 = tonumber(tokens[3])
		pkt.reserve_int2 = tonumber(tokens[4])
		pkt.reserve_str1 = tokens[5]
		pkt.reserve_str2 = ""
		self:Handle_Faction_People(pkt)
	elseif(gm_key == 995)then
		local pkt = {}
		pkt.opt_type = tonumber(tokens[2])
		pkt.reserve_int1 = tonumber(tokens[3])
		pkt.reserve_int2 = tonumber(tokens[4])
		pkt.reserve_str1 = tokens[5]
		pkt.reserve_str2 = ""
		if pkt.opt_type == 1 or pkt.opt_type == 2 or pkt.opt_type == 3 then
			for i = 10,1,-1 do
				pkt.reserve_int1 = i
				self:Handle_Equipdevelop_Operate(pkt)
			end
		elseif pkt.opt_type == 4 or pkt.opt_type == 5 then
			for i = 10,1,-1 do
				pkt.reserve_int1 = i
				for j = 1,3 do
					pkt.reserve_int2 = j
					self:Handle_Equipdevelop_Operate(pkt)
				end
			end
			
		elseif pkt.opt_type == 6 or pkt.opt_type == 7 then
			local itemMgr = self:getItemMgr()
			local item = itemMgr:getBagItemByPos(BAG_TYPE_EQUIP,pkt.reserve_int1)
			if item then
				outFmtInfo("#################### %s",item:getGuid())
				pkt.reserve_str1 = item:getGuid()
			end
			self:Handle_Equipdevelop_Operate(pkt)
		else
			self:Handle_Equipdevelop_Operate(pkt)
		end
		
		
	elseif(gm_key == 996)then		-- @远征 最高层数
		local faction = app.objMgr:getObj(self:GetFactionId())
		if faction == nil then
			return
		end
		local floor = tonumber(tokens[2]) or 5
		faction:SetFactionTowerTodayTopFloor(floor)
		faction:SetFactionTowerTodayTopIcon(1)
		faction:SetFactionTowerTodayTopForce(randInt(1,500000))
		faction:SetFactionTowerTodayTopName("名字名字"..randInt(1,50))
		
		self:SetFactionBossDefenseTickets(10)
	
	elseif(gm_key == 997)then		-- @家族商店刷新
		local faction_guid = self:GetFactionId()
		if faction_guid == "" then
			return
		end
		
		if not app.objMgr:IsFactionGuid(faction_guid) then
			return
		end
		
		local faction = app.objMgr:getObj(faction_guid)
		if faction == nil then
			return
		end
		--faction:RefreshShop()
		--faction:DailyResetGift()
		faction:ResetFaction()
		--self:DailyReset()
	elseif(gm_key == 998)then		-- @整理接口
		outFmtInfo("test sort: ")
		SortItem(self)
	elseif(gm_key == 999)then		-- @测试接口
		--[[
		local hours = 1
		if tokens[2] then
			hours = tonumber(tokens[2])
		end
		self:SubUInt32(PLAYER_INT_FILED_LEAVE_RISK_TIME, hours * 3600)
		--]]
		
		--local pkt = {}
		--pkt.opt_type = FACTION_MANAGER_TYPE_BUILDING_UPGRADE
		--pkt.reserve_int1 = tonumber(tokens[2])
		--self:Handle_Faction_People(pkt)
		--[[
		local pkt2 = {}
		pkt2.opt_type = FACTION_MANAGER_TYPE_BUILDING_UPGRADE_SPEEDUP
		pkt2.reserve_int1 = 1
		--self:Handle_Faction_People(pkt2)
		
		local pkt3 = {}
		pkt3.opt_type = FACTION_MANAGER_TYPE_DONATE_GIFT_EXCHANGE
		pkt3.reserve_int1 = 1
		--self:Handle_Faction_People(pkt3)
		
		local pkt4 = {}
		pkt4.opt_type = FACTION_MANAGER_TYPE_JUANXIAN
		pkt4.reserve_int1 = 1
		pkt4.reserve_int2 = 1
		self:Handle_Faction_People(pkt4)
		--]]
		--[[
		local pkt= {}
		pkt.list = {{1201,4}}			--道具list
		pkt.msg = "aaa"
		pkt.msg_type  = 0
		--self:Handle_Send_Faction_Gift(pkt)
		
		local pkt3 = {}
		pkt3.opt_type = FACTION_MANAGER_TYPE_GIFT_THANK
		pkt3.reserve_int1 = 5
		--self:Handle_Faction_People(pkt3)
		
		local pkt5 = {}
		pkt5.count_id = 4
		--self:Handle_Get_Faction_Gift_Exreward(pkt5)
		
		local pkt6 = {}
		pkt6.opt_type = FACTION_MANAGER_TYPE_GIFT_REPLY
		pkt6.reserve_int1 = 1
		pkt6.reserve_int2 = 0
		pkt6.reserve_str1 = "asdasdad"
		--self:Handle_Faction_People(pkt6)
		
		local pkt7 = {}
		pkt7.opt_type = FACTION_MANAGER_TYPE_GIFT_THANK_AND_REPLY
		pkt7.reserve_int1 = 6
		pkt7.reserve_int2 = 0
		pkt7.reserve_str1 = "wwwwww"
		--self:Handle_Faction_People(pkt7)
		
		
		local pkt4 = {}
		pkt4.opt_type = FACTION_MANAGER_TYPE_GIFT_SHOW_PAGE
		pkt4.reserve_int1 = 1
		--self:Handle_Faction_People(pkt4)
		--]]
		
		--local pkt = {}
		--pkt.id = 1
		--self:Handle_Talisman_Active(pkt)
		
		--self:Handle_Talisman_Lvup(pkt)
		
		--self:Handle_Wings_Active(pkt)
		--self:Handle_Wings_Bless(pkt)
		--self:Handle_Wings_RankUp(pkt)
		--self:Handle_Wings_Strength(pkt)
		
		--self:MatchGroupInstance(1)
		--self:OnBuyGroupInstanceTicket(1)
		--self:SetFactionBossDefenseTickets(10)
		local pkt4 = {}
		pkt4.opt_type = FACTION_MANAGER_TYPE_BOSSDEFENSE_CHALLENGE
		pkt4.reserve_int1 = 1
		--self:Handle_Faction_People(pkt4)
		
		local pkt5 = {}
		--pkt5.opt_type = FACTION_MANAGER_TYPE_TOWER_CHALLENGE
		pkt5.opt_type = FACTION_MANAGER_TYPE_TOWER_SWEEP
		--pkt5.opt_type = FACTION_MANAGER_TYPE_TOWER_CHEST
		pkt5.reserve_int1 = 1
		--self:Handle_Faction_People(pkt5)
		
		--self:ItemUnlockTitle(tonumber(tokens[2]))
		
		--self:getItemMgr().itemMgr:ForEachBagItem(BAG_TYPE_EQUIP
--, function(ptr)
			--local item = require("appd.appd_item").new(ptr)
			--outFmtInfo("=======================  %s, %s",tostring(item:getId()),item:toString())	
		--end)
		
		--DoActivityDataUpdate (self, 1, {1,1})
		--DoActivityDataUpdate (self, 10, {1,1})
		local number = tonumber(tokens[2])
		if number then
			self:SetConsumeSum(number)
		end
	else
		--[[
		if(gm_level < GM_LEVEL_1)then
			self:call_chat_whisper(self:GetGuid(), self:GetFaction(), self:GetName(), whisper_str)
			return
		end
		]]
		--outFmtInfo("DoGMScripts")
		DoGMScripts(self.ptr, str)
	end
end

-- 检测聊天限制
function PlayerInfo:CheckChatLimit(c_type, content)
	--gm很屌！！！
	if self:GetGmNum() > 0 then
		return true
	end
	--还没名字的玩家不能发言
	if self:GetName() == "" then
		return false
	end
	--没内容的发言
	if content == "" then
		return false
	end
	--[[
	--是否被禁言
	if self:IsGag() then
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_RESULT_IS_GAG)
		return false
	end
	--]]
	--聊天长度不能超过120
	if string.len(content) > 120 then
		return false
	end
	
	-- 等级限制
	local level = self:GetLevel()
	if(c_type == CHAT_TYPE_WHISPER and level < config.player_chat_whisper_level)then
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_LEVEL_WHISPER_LEVEL_NO)
		return false
	elseif(c_type == CHAT_TYPE_WORLD and level < config.player_chat_world_level)then
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_LEVEL_WORLD_LEVEL_NO)
		return false
	end
	
	--频率限制
	local guid = self:GetGuid()
	local t = os.time()
	local seconds = 1
	local last_time = last_chat_time[guid] or 0
	if(last_time + seconds > t)then
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_RESULT_CHECK_LIMIT)
		return false
	end
	last_chat_time[guid] = t
	
	-- 等级次数限制
	if(t > limit_t)then
		limit_t = t - math.fmod(t,3600) + 3600
		chat_limit = {}
	end
	
	local chat_num = chat_limit[guid] or 0
	local result = true
	local max_num = LIMIT_LEVEL_COUNT_CONFIG[math.floor(level/10)]
	if(max_num)then
		result = chat_num < max_num
	end
	if result then
		chat_limit[guid] = chat_num + 1
	else
		self:CallOptResult(OPRATE_TYPE_CHAT, CHAT_RESULT_CHECK_LIMIT)
	end
	return result
end