--应用服
function  DoGMScripts(player_ptr, gm_commands)
	local player = app.objMgr:fromPtr(player_ptr)
	local result = ""

	
	if(gm_commands == "@hello")then
		result = "ni ye hello"
		player:CallOptResult(1,1,result)
		return result
	end
	
	local tokens = lua_string_split(gm_commands," ")
	if(#tokens == 0) then
		--闹哪样啊??
		return result
	end
	local paras = {0}
	for i = 2, #tokens do
		paras[i] = tonumber(tokens[i])
		if(paras[i] == nil)then
			paras[i] = 0
		end
	end
	if(tokens[1] == '@mp')then
		if(#paras >= 2)then			
			player:SetDouble(PLAYER_FIELD_POWERS + POWER_MANA*2, paras[2])
			player:SetDouble(PLAYER_FIELD_AUTO_MP, paras[2])			
		end
	elseif tokens[1] == '@VIP' then	
		local lv = paras[2] or 1
		local tm = os.time() + 30 * 24 * 3600
		player:SetVIP(lv, tm)
		
	elseif(tokens[1] == '@hp')then
		if(#paras >= 2)then			
			player:SetDouble(PLAYER_FIELD_AUTO_HP, paras[2])
			player:SetDouble(PLAYER_FIELD_HEALTH, paras[2])
		end
	elseif(tokens[1] == "@每日在线") then	--设置每日在线时长		
		local minute = 10	--默认为10min
		if (#tokens >= 2) then
			minute = tonumber(tokens[2])
		end
		
		player:SetDailyOnlineTime(minute)
		
	elseif(tokens[1] == "@http") then
		player:OnWorld3v3Match()
	elseif(tokens[1] == "@重置")then		
		if(#tokens > 2)then
			return result
		end
		if(tonumber(tokens[2]) < 24 and tonumber(tokens[2]) >= 0)then
			player:SetResetDailyTime(tonumber(tokens[2]), os.time()-86400)
		end
		player:DoResetDaily()
	
	elseif (tokens[1] == "@重置帮派")then
		local factionGuid = player:GetFactionId()
		if string.len(factionGuid) > 0 then
			local faction = app.objMgr:getObj(factionGuid)
			faction:ResetFaction()
		end
	elseif(tokens[1] == "@排位连胜")then
		if tokens[2] then
			local cnt = tonumber(tokens[ 2 ])
			if cnt then
				local instMgr = player:getInstanceMgr()
				for i = 1, #tb_single_pvp_extra do
					local wins = tb_single_pvp_extra[ i ].wins
					if cnt >= wins then
						instMgr:SetQualifyExtraObtain(i-1)
					end
				end
			end
		end
	elseif(tokens[1] == "@选择主线")then
		if tokens[2] then
			local id = tonumber(tokens[ 2 ])
			if id then
				if tb_quest[id].type == QUEST_TYPE_MAIN then
					local questMgr = player:getQuestMgr()
					questMgr:OnSelectMainQuest(id)
				end
			end
		end
	elseif(tokens[1] == "@解锁外观")then
		if tokens[2] then
			local id = tonumber(tokens[ 2 ])
			if id then
				local pkt = {id = id}
				player:Handle_Active_Appearance(pkt)
			end
		end
	elseif(tokens[1] == "@装备外观")then
		if tokens[2] then
			local id = tonumber(tokens[ 2 ])
			if id then
				local pkt = {id = id}
				player:Handle_Equip_Appearance(pkt)
			end
		end
	
	elseif(tokens[1] == "@增加修炼经验")then
		if tokens[2] then
			local val = tonumber(tokens[ 2 ])
			local spellMgr = player:getSpellMgr()
			spellMgr:addMeridianExp(val)
		end
	elseif(tokens[ 1 ] == "@属性")then
		if tokens[ 2 ] then
			local equipmentId = tonumber(tokens[ 2 ])
			if equipmentId < 1 or equipmentId > #attrKeys then
				return
			end
			
			local playerAttrId = attrKeys[equipmentId]
			if tokens[ 3 ] then
				local value = tonumber(tokens[ 3 ])
				if value < 0 then
					value = math.max(value, -player:GetDouble(playerAttrId))
				end
				player:AddDouble(playerAttrId, value)
				local prevlist = {
					[equipmentId] = value,
				}
				player:SendAttrChanged(prevlist)
			end
		end		
	elseif(tokens[1] == "@仙府体验")then
		player:EnterXianfuTest()
	elseif(tokens[1] == "@原地副本")then
		local x, y = player:GetPosition()
		local mapid = self:GetMapId()
		player:EnterRemindInstance(4001, 36, 85, 7403, x+1, y+1, mapid)
	elseif(tokens[1] == "@下一个主线")then
		if tokens[2] then
			local id = tonumber(tokens[ 2 ])
			if id then
				if tb_quest[id].type == QUEST_TYPE_MAIN then
					local nextid = tb_quest[id].nextid
					if nextid > 0 then
						local questMgr = player:getQuestMgr()
						questMgr:OnSelectMainQuest(nextid)
					end
				end
			end
		end
	elseif (tokens[1] == "@完成当前主线")then
		local questMgr = player:getQuestMgr()
		questMgr:FinishCurrentMainQuest()
	elseif(tokens[1] == "@军团等级")then
		local faction = app.objMgr:getObj(player:GetFactionId())
		if (faction ~= nil) then
			local level = 1
			if(#tokens >= 2 and tonumber(tokens[2]) ~= nil)then
				level = tonumber(tokens[2])
			end
			if(level > 5)then
				level = 5
			end
			faction:SetFactionLevel(level)
		end

	elseif(tokens[1] == "@声望")then		
		if(#tokens == 2)then
			local shengwang = tonumber(tokens[2])
			player:SetDailyPres(shengwang)
		end
	elseif(tokens[1] == "@登陆器")then
		player:SetPresDengLuQi()
	elseif(tokens[1] == "@军团加入时间")then
		local faction = app.objMgr:getObj(player:GetFactionId())
		if (faction ~= nil) then
			local FactionPos = faction:FindPlayerIndex(player:GetGuid())
			if(FactionPos ~= -1)then
				faction:SetFactionMemberJoinTime(FactionPos,os.time()-86400)
			end
		end	
	elseif(tokens[1] == "@PK值")then		
		if(#tokens == 2)then
			local val = tonumber(tokens[2])
			player:SetPKValue(val)
		end
	elseif(tokens[1] == "@充值")then		
		if(#tokens == 2)then
			local val = tonumber(tokens[2])
			player:SetUInt32(PLAYER_APPD_INT_FIELD_RECHARGE_SUM, val)
			outDebug('val ='..player:GetUInt32(PLAYER_APPD_INT_FIELD_RECHARGE_SUM, val))
		end
	elseif(tokens[1] == "@在线等级")then		
		local val = 0
		if(#tokens >= 2)then
			val = tonumber(tokens[2])			
		end
		player:SetOnlineTime(val)
	
	elseif(tokens[1] == "@兑换商店")then
		local entry = 878
		if(#tokens >= 2 and tonumber(tokens[2]) > 0)then
			entry = tonumber(tokens[2])
		end		
		player:DoZBJFExchangeItems(entry)
	elseif(tokens[1] == "@连登")then
		if(#tokens == 2 and tonumber(tokens[2]) > 0)then
			day = tonumber(tokens[2])			
			player:SetUInt32(PLAYER_EXPAND_INT_CREATE_TIME,os.time() - day*86400)
		end

	elseif(tokens[1] == "@万能礼包")then
		local gift_id = 1
		local gift_type = 1
		if(#tokens >= 2 and tonumber(tokens[2]) > 1)then
			gift_id = tonumber(tokens[2])
		end
		if(#tokens >= 3 and tonumber(tokens[3]) > 1)then
			gift_type = tonumber(tokens[3])
		end		
		player:ReceiveGift(gift_id, gift_type)
	elseif(tokens[1] == "@限时活动")then
		local gift_id = 0
		local gift_type = 1
		if(#tokens >= 2 and tonumber(tokens[2]) > 0)then
			gift_id = tonumber(tokens[2])
		end
		if(#tokens >= 3 and tonumber(tokens[3]) > 1)then
			gift_type = tonumber(tokens[3])
		end
		local la = GetToDayLimitActivity()
		local limit_acti = player:getLimitActivityInfo(la)
		if(la == nil)then
			return result
		end
		if(limit == nil)then
			return result
		end
		limit_acti:ReceiveFunc(player, gift_id, gift_type)
	elseif(tokens[1] == "@创建时间")then		
		if(#tokens >= 2 and tonumber(tokens[2]) > 1)then
			player:SetUInt32(PLAYER_EXPAND_INT_CREATE_TIME, tonumber(tokens[2]))
		end
	elseif(tokens[1] == "@假充值")then
		local amount = 100
		if(#tokens >= 2 and tonumber(tokens[2]) > 0)then
			amount = tonumber(tokens[2])
		end
		local cur_time = os.time()		
		player:DoAddRechargeSum(tostring(cur_time), amount, cur_time)
	elseif(tokens[1] == "@锻造材料")then		
		if(#tokens == 2 or #tokens == 1)then
			local value = paras[2] or 100
			for i = Item_Loot_Wood,Item_Loot_Crystal
			do
				player:SetNumberMaterial(i,value)
			end
		elseif(#tokens == 3)then
			local type_
			if(tokens[2] == "木头")then
				type_ = Item_Loot_Wood	
			elseif(tokens[2] == "矿石")then
				type_ = Item_Loot_Mineral	
			elseif(tokens[2] == "煤炭")then
				type_ = Item_Loot_Coal	
			elseif(tokens[2] == "皮革")then
				type_ = Item_Loot_Leather	
			elseif(tokens[2] == "珍珠")then
				type_ = Item_Loot_Pearl	
			elseif(tokens[2] == "水晶")then		
				type_ = Item_Loot_Crystal
			end
			if(type_)then
				player:SetNumberMaterial(type_,paras[3])
			end
		end
	elseif(tokens[1] == "@锻造加成")then		
		local cur_time = os.time()
		player:SetLidao(cur_time)
		player:SetHuohou(cur_time)
		player:SetLianqishi(cur_time)
		player:SetJishi(cur_time)
	elseif(tokens[1] == "@锻造次数")then		
		local type = paras[2] or 1
		local value = paras[3] or 1
		if type == 1 then
			player:SetForgeCount(value)
			player:SetForgeCountForShuXing(value)
		else
			globalCounter:SetTotalForgeCount(value)	
		end
	elseif(tokens[1] == "@锻造等级")then		
		if #paras == 2 or #paras == 1 then
			--不传参数就把所有位置锻造等级都提高
			for i = 1, EQUIPMENT_TYPE_MAX_POS do
				player:SetForgeProficiencyLv(i-1,  (paras[2] or 1) )			
			end
		end
	elseif(tokens[1] == "@锻造熟练度")then		
		if #paras == 2 or #paras == 1 then
			--不传参数就把所有位置锻造等级都提高
			for i = 1, EQUIPMENT_TYPE_MAX_POS do
				player:SetForgeProficiency(i-1, player:GetForgeProficiency(i-1)+ (paras[2] or 1) )			
			end		
		end			
	elseif(tokens[1] == "@武将天赋")then		
		local index = 0
		local level = 0
		if(#tokens >= 2 and tonumber(tokens[2]) >= 1 and tonumber(tokens[2]) <= 199)then
			index = tonumber(tokens[2])
		end
		if(#tokens >= 3 and tonumber(tokens[3]) >= 1)then
			level = tonumber(tokens[3]) - 1
		end
		player:GMupGradeGeneralsTalent(index, level)	
	
	elseif(tokens[1] == "@酒馆") then		
		local typed = tonumber(tokens[2])
		local nowtime = os.time()
		if(typed == 1)then
			player:SetJiuguanChengCounts(0)
		elseif(typed == 2)then
			player:SetJiuguanZiCounts(0)
		elseif(typed == 3)then
			player:SetPlayerFreeTime(os.time())
		elseif(typed == 4)then
			player:UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_JIUGUAN_HUNDRED)
		elseif(typed == 5)then
			player:SetPlayerFreeTime(0)
		else
			--player:InitJiuguanData()
		end	
	elseif(tokens[1] == "@背包扩展")then		
		local itemMgr = self:getItemMgr( )
		if(#tokens == 2 and tonumber(tokens[2]) > 0)then
			local size = tonumber(tokens[2])
			if(tonumber(tokens[2]) > config.bag_max_size_main)then
				size = config.bag_max_size_main
			end	
			
			itemMgr:setBagSize(BAG_TYPE_MAIN_BAG,size)
			local bag_size = itemMgr:getBagSize(BAG_TYPE_MAIN_BAG)
			local next_time = math.floor(1.5 * (bag_size + 1 - 34) * (bag_size + 1 - 35)) + 30
			player:SetExtensionEndTime(os.time() + next_time)
		elseif(#tokens == 3)then
			local size = tonumber(tokens[2])
			local pos = tonumber(tokens[3])
			player:DoBagExtension(BAG_TYPE_MAIN_BAG,size,pos)
		end
	elseif(tokens[1] == "@在线玩家数")then		
		if(#tokens >= 2 and tonumber(tokens[2]) > 0 and tonumber(tokens[2]) < 65536)then
			globalValue:SetQuanFuOnLinePlayerNum(tonumber(tokens[2]))
		end
	elseif(tokens[1] == "@挖宝")then
		globalValue:SetWaBaoNextTime(os.time() - 10)
		DoSetWaBaoTime()
	elseif(tokens[1] == "@藏宝图")then		
		if(#tokens >= 2 and tonumber(tokens[2]) > 0 and tonumber(tokens[2]) < 10)then
			player:SetPlayerWaBaoNum(tonumber(tokens[2]))
		end
	elseif(tokens[1] == "@藏宝图传送")then		
		local mapid = player:GetPlayerWaBaoMapId()
		local cbt_posx = player:GetPlayerWaBaoPosX()
		local cbt_posy = player:GetPlayerWaBaoPosY()
		if mapid ~= 0 and cbt_posx ~= 0 and cbt_posy ~= 0 then
			call_appd_teleport(player:GetScenedFD(), player:GetGuid(), cbt_posx, cbt_posy, mapid)
		end		
	elseif(tokens[1] == "@军团排名")then
		local faction = app.objMgr:getObj(player:GetFactionId())
		if (faction ~= nil) then
			faction:DoFactionTop()
		end
	elseif(tokens[1] == "@军团可加入")then
		
		local leave_time = 0
		if(#tokens == 2)then
			leave_time = tonumber(tokens[2])
		end
		player:SetPlayerLeaveFactionTime(leave_time)
	elseif(tokens[1] == "@登录天数")then		
		local ntime = 0
		if(#tokens == 2)then
			ntime = tonumber(tokens[2])
		end
		player:SetNumberOfLoginDays(ntime)
	elseif(tokens[1] == '@兵魂排名')then
		local ntime = 0
		local ntype = 0
		if(#tokens == 3)then
			ntime = tonumber(tokens[3])
			ntype = tonumber(tokens[2])
			globalValue:SetBingHunJiHuoNum(ntype,ntime)
		end
	elseif(tokens[1] == '@兵魂激活')then		
		if(#tokens == 2)then
			local ntype = tonumber(tokens[2])
			if(ntype < MAX_SHENBING_TYPE)then
				player:SetBingHunFlags(ntype)
				playerLib.SendAttr(player_ptr)		-- 属性重算下
			end
		end
	elseif(tokens[1] == "@团长离线时间")then
		local faction = app.objMgr:getObj(player:GetFactionId())
		if(#tokens ~= 2)then
			return result
		end	
		local last_time =tonumber(tokens[2])
		if (faction ~= nil) then
			local manager_guid = faction:GetTuanzhangGuid()
			local pos = faction:FindPlayerIndex(manager_guid)
			if(pos ~= -1)then
				faction:SetFactionMemberLogoutTime(pos,last_time)
			end
		end
	elseif(tokens[1] == "@托管结束时间")then
		local ntime = os.time()
		if(#tokens == 2)then
			ntime = tonumber(tokens[2])
		end		
		player:SetPlayerHostingTime(ntime)
	elseif(tokens[1] == "@聊天")then
		playerLib.SendChat(player_ptr, CHAT_TYPE_WORLD, TEXT_WABAO_APPER_GG)
		--onSendNotice(NOTICE_TYPE_PROMPT, TEXT_DOG_BITE_WORLD_MSG, string.format("budao|%s",self:GetPlayerGuid()))
	
	elseif(tokens[1] == "@客服GM")then		
		player:SetByte(PLAYER_FIELD_BYTES_5, 0, 13)
	elseif(tokens[1] == "@美女主播")then		
		player:SetByte(PLAYER_FIELD_BYTES_5, 0, 103)
	elseif(tokens[1] == "@激活神兵")then
		local jihuo_type = SHENBING_TYPE_TANLANGXING
		if(#tokens >= 2 and tonumber(tokens[2]) >= SHENBING_TYPE_TANLANGXING and tonumber(tokens[2]) < MAX_SHENBING_TYPE_COUNT)then
			jihuo_type = tonumber(tokens[2])
		end		
		player:SetBingHunFlags(jihuo_type)
	elseif(tokens[1] == "@清空元宝")then		
		local num = player:GetMoney(MONEY_TYPE_GOLD_INGOT)
		player:SubMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_MLWY_GET_GIFT, num)
		num = player:GetMoney(MONEY_TYPE_SILVER)
		player:SubMoney(MONEY_TYPE_SILVER, MONEY_CHANGE_MLWY_GET_GIFT, num)
	
	elseif (tokens[1] == "@Resource")then
		local id = tonumber(tokens[ 2 ])
		local value = tonumber(tokens[ 3 ])
		if id < -1 or id >= MAX_MONEY_TYPE then
			return
		end
		
		local a = id
		local b = id
		if id == -1 then
			a = 0
			b = MAX_MONEY_TYPE - 1
		end
		
		for id = a, b do
			local num = player:GetMoney(id)
			if value < num then
				player:SubMoney(id, MONEY_CHANGE_GM_COMMAND, num-value)
			else
				player:AddMoney(id, MONEY_CHANGE_GM_COMMAND, value-num)
			end
		end
		
	elseif(tokens[1] == "@付费等级")then
		local level = 0
		if(#tokens >= 2)then
			level = tonumber(tokens[2])
		end		
		player:SetByte(PLAYER_FIELD_BYTES_2, 1, level)
		
	elseif(tokens[1] == "@系统邮件")then
		globalSystemMail:AddSystemMailInfo(GIFT_PACKS_TYPE_SYSTEM_COMPENSATION, os.time(),os.time() + 86400*30, "系统补偿", "由于服务器停机维护, 对您的照成的损失进行补偿", "50001,1,50003,10", "")

	elseif (tokens[1] == "@激活幻化")then
		local id = tonumber(tokens[2])
		if tb_mount_illusion[id] then
			player:onActiveIllusion(id)
		end
	
	elseif(tokens[1] == "@邮件")then
		--发放礼包
		local toguid = player:GetGuid()
		if #tokens >= 2 then
			toguid = tokens[2]
		end
		
		local item = ""

		if tokens[3] then
			item = tokens[3]
		end
		--AddGiftPacksData(player:GetGuid(),0,GIFT_PACKS_TYPE_FRIEND,os.time(),os.time() + 86400*30,GIFT_MASSAGE_BAGUAZHEN.GIFT_NAME,GIFT_MASSAGE_BAGUAZHEN.GIFT_NAME,"2372,1,0,2322,1,0,2334,1,0,2344,1,0,254,1,0",player:GetName())
		AddGiftPacksData(toguid,0,GIFT_PACKS_TYPE_ITEM_GIVE,os.time(),os.time() + 86400*30,"GIFT_NAME","小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比",item,player:GetName())
	
	elseif (tokens[1] == "@野外BOSS")then
		local H = tonumber(tokens[ 2 ])
		local M = tonumber(tokens[ 3 ])
		local a = tonumber(tokens[ 4 ])
		local b = tonumber(tokens[ 5 ])
		app:InitFieldBossCorn(H, M, a, b)
		
	elseif (tokens[1] == "@世界BOSS")then
		-- 先把原来的状态设置下
		globalValue:SetWorldBossState(WORLD_BOSS_PROCESS_TYPE_FINISH)
		-- 重新设置BOSS
		local H = tonumber(tokens[ 2 ])
		local M = tonumber(tokens[ 3 ])
		local a = tonumber(tokens[ 4 ])
		local b = tonumber(tokens[ 5 ])
		app:InitWorldBossCorn(H, M, tb_worldboss_time[ 1 ].enrolllast, 15, 3)
		
	elseif(tokens[1] == "@NA")then
		--发放礼包
		local toguid = player:GetGuid()
		if #tokens >= 2 then
			toguid = tokens[2]
		end
		--AddGiftPacksData(player:GetGuid(),0,GIFT_PACKS_TYPE_FRIEND,os.time(),os.time() + 86400*30,GIFT_MASSAGE_BAGUAZHEN.GIFT_NAME,GIFT_MASSAGE_BAGUAZHEN.GIFT_NAME,"2372,1,0,2322,1,0,2334,1,0,2344,1,0,254,1,0",player:GetName())
		AddGiftPacksData(toguid,0,GIFT_PACKS_TYPE_ITEM_GIVE,os.time(),os.time() + 86400*30,"GIFT_NAME","小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比，小高是都比","",player:GetName())
	
	
	elseif(tokens[1] == "@在线奖励次数")then
		local count = 0
		if(#tokens >= 2)then
			count = tonumber(tokens[2])
		end		
		player:SetOnlineAwardCount(count)
	elseif(tokens[1] == "@打印摊位记录")then
		local playerStallInfo = player:getStallTradeRecordInfo() 
		playerStallInfo:PrintStallTradeRecord()
	elseif(tokens[1] == "@境界")then
		local num = 0
		if(#paras >= 2 and paras[2] >= 0 and paras[2] <= 8)then
			num = paras[2]
		end		
		player:SetJingjieLevel(num)
		playerLib.SendAttr(player_ptr)
	
	elseif(tokens[1] == "@称号")then		
		if(#paras == 3)then
			player:DoTitleOpt(paras[2],paras[3])
		elseif(#paras == 4)then
			player:DoTitleOpt(paras[2],paras[3],paras[4])
		end
	elseif(tokens[1] == "@军团解散")then
		local faction = app.objMgr:getObj(player:GetFactionId())
		if (faction ~= nil) then
			faction:AllFactionPlayerQuit()
		end
	elseif(tokens[1] == "@军团小黑屋")then		
		if(#paras == 2 and paras[2] >= 0)then
			player:SetFactionXiaoHeiWu(paras[2])
		end
	elseif(tokens[1] == "@军团打印")then
		local faction = app.objMgr:getObj(player:GetFactionId())
		if (faction ~= nil) then
			if(#paras == 2)then
				local pos = paras[2]
				print("force===="..faction:GetFactionMemberForce(pos))
				print("level===="..faction:GetFactionMemberLevel(pos))
				print("is_online===="..faction:GetFactionMemberIsOnline(pos))
				print("identity===="..faction:GetFactionMemberIdentity(pos))
				print("gender===="..faction:GetFactionMemberGender(pos))
				print("VIP===="..faction:GetFactionMemberVipLevel(pos))
				print("online_time===="..faction:GetFactionMemberOnlineTime(pos))
				print("XHW===="..faction:GetFactionMemberXHWEndTime(pos))
				print("logout_time===="..faction:GetFactionMemberLogoutTime(pos))
				print("guid===="..faction:GetFactionMemberGuid(pos))
				print("name===="..faction:GetFactionMemberName(pos))
				print("old_xhw====="..faction:GetUInt32(CORPS_INT_FIELD_APPLY_PLAYER+pos*MAX_CORPS_INT_MEMBER + CORPS_INT_MEMBER_XIAOHEIWU_END_TIME))
				print("old_logout_time====="..faction:GetUInt32(CORPS_INT_FIELD_APPLY_PLAYER+pos*MAX_CORPS_INT_MEMBER + CORPS_INT_MEMBER_LOGOUT_TIME))
				print("index == "..CORPS_INT_FIELD_APPLY_PLAYER+pos*MAX_CORPS_INT_MEMBER + CORPS_INT_MEMBER_XIAOHEIWU_END_TIME)
			end
		end
	
	elseif(tokens[1] == "@设置任务状态")then		
		local quest_id = 1
		if(#paras >= 2 and paras[2] >= 0)then
			quest_id = paras[2]
		end
		if player:GetQuestCompleteListFlag(quest_id) then
			player:UnSetQuestCompleteListFlag(quest_id)
		else
			player:SetQuestCompleteListFlag(quest_id)
		end
	elseif(tokens[1] == "@清空报名")then		
		if player:GetBangId() ~= "" then
			local bang_ptr = playerLib.FindBangInfo(player_ptr)
			if bang_ptr then
				local bangInfo = BangInfo:new{ptr = bang_ptr}
				bangInfo:ClearBaoMingFlag()
			end
		end
	elseif(tokens[1] == "@转生元神")then 	   
	   local val = paras[2] or 1
	   player:AddYuanshenCount(val)	
	elseif(tokens[1] == "@转生五行")then 	   
	   local v1,v2,v3,v4,v5 = player:GetWuxingCount()
	   v1 = v1 + paras[2] or 0
	   v2 = v2 + paras[3] or 0
	   v3 = v3 + paras[4] or 0
	   v4 = v4 + paras[5] or 0
	   v5 = v5 + paras[6] or 0
	   player:SetWuxingCount(v1,v2,v3,v4,v5)
	elseif(tokens[1] == "@转生雷劫")then 	   
	   local val = paras[2] or 1
	   player:AddLeijieCount(val)	   
	elseif(tokens[1] == "@转生点脉")then 	   
	   local val = paras[2] or 1
	   player:AddDianmaiCount(val)
	elseif(tokens[1] == "@弈棋")then 		
		local lv = paras[2] or 1
		if(lv < 0 or lv >= #tb_yiqi)then
			lv = #tb_yiqi-1
		end
		player:SetYiQiLevel(lv)
		player:SetYiQiJinDu(0)
		playerLib.SendAttr(player_ptr)
	elseif(tokens[1] == "@木桩")then 		
		local lv = paras[2] or 1
		if(lv < 0 or lv >= #tb_muzhuang)then
			lv = #tb_muzhuang-1
		end
		player:SetMuZhuangLevel(lv)
		player:SetMuZhuangJinDu(0)   
		playerLib.SendAttr(player_ptr)
	elseif(tokens[1] == "@弹琴")then 		
		local lv = paras[2] or 1
		if(lv < 0 or lv >= #tb_tanqin)then
			lv = #tb_tanqin-1
		end
		player:SetTanQinLevel(lv)
		player:SetTanQinJinDu(0)   
		playerLib.SendAttr(player_ptr)	
	elseif(tokens[1] == "@作画")then 		
		local lv = paras[2] or 1
		if(lv < 0 or lv >= #tb_zuohua)then
			lv = #tb_zuohua-1
		end
		player:SetZuoHuaLevel(lv)
		player:SetZuoHuaJinDu(0)   
		playerLib.SendAttr(player_ptr)	
	elseif(tokens[1] == "@劈砖")then 		
		local lv = paras[2] or 1
		if(lv < 0 or lv >= #tb_pizhuan)then
			lv = #tb_pizhuan-1
		end
		player:SetPiZhuanLevel(lv)
		player:SetPiZhuanJinDu(0)   
		playerLib.SendAttr(player_ptr)
	elseif(tokens[1] == "@藏书")then 		
		local lv = paras[2] or 1
		if(lv < 0 or lv > #tb_cangshu)then
			lv = #tb_cangshu
		end
		player:SetCangShuIsLinMo(lv-1)
		playerLib.SendAttr(player_ptr)
	elseif(tokens[1] == "@重置藏书")then 		
		if(#tokens == 1)then
			local len = #tb_cangshu
			for i = 1, len do
				player:UnSetCangShuIsLinMo(i -1)
			end
		elseif(#tokens == 2)then
			local lv = paras[2] or 1
			if(lv < 0 or lv > #tb_cangshu)then
				lv = #tb_cangshu
			end
			player:UnSetCangShuIsLinMo(lv - 1)
		end
		playerLib.SendAttr(player_ptr)
	elseif(tokens[1] == "@擂鼓")then 		
		local lv = paras[2] or 1
		if(lv < 0 or lv >= #tb_leigu)then
			lv = #tb_leigu-1
		end
		player:SetLeiGuLevel(lv)
		playerLib.SendAttr(player_ptr)
	elseif(tokens[1] == "@擂鼓次数")then 
		local lg_add_count = paras[2] or 1
		local lg_count = paras[3] or -1		
		if(lg_count >= 0)then
			player:SetLeiGuCount(lg_count)
		else
			player:AddLeiGuCount(lg_add_count)
		end
	elseif(tokens[1] == "@擂鼓时间")then 
		local lg_time = paras[2] or 0		
		player:SetLeiGuTime(os.time() + lg_time)
	elseif(tokens[1] == "@重置擂鼓")then		
		player:SetLeiGuLevel(0)
		player:SetLeiGuCount(MAX_LEIGU_COUNT)
		player:SetLeiGuTime(os.time() + tb_game_set[56].value[1])
		player:SetLeiGuExp(0)
		player:SetLeiGuOverTime(0)
		player:SetLeiGuAddExp(0)
		player:SetLeiGuAddLevel(0)
		player:SetLeiGuComboCount(0)
		playerLib.SendAttr(player_ptr)
	elseif(tokens[1] == "@淬体")then 		
		local lv = paras[2] or 1
		if(lv < 0 or lv >= #tb_cuiti)then
			lv = #tb_cuiti-1
		end
		player:SetCuiTiLevel(lv)
		player:SetCuiTiExp(0)
		player:SetCuiTiHuoHou(0)
		player:SetCuiTiKuangWu(0)
		player:SetCuiTiStartExpTime(0)
		player:SetCuiTiHuoHouTime(0)
		player:SetCuiTiKuangWuTime(0)
		playerLib.SendAttr(player_ptr)	
	elseif(tokens[1] == "@日环重置")then			
		player:SetRihuanBugCount(0)	--购买次数
		player:SetRihuanCompleteLun(0)	--完成次数
		player:SetRihuanRefreshXingCount(0)	--刷星次数
		player:SetRihuanHuan(0)	--环数
		player:SetRihuanLun(0)	--当前轮数	
	elseif(tokens[1] == "@红颜")then		
		if(#tokens == 2 or #tokens == 1)then
			local value = paras[2] or 100
			for i = HONGYAN_PLAYER_ATTRS_XIAYI,HONGYAN_PLAYER_ATTRS_XIANGMAO
			do
				player:SetHongYanPlayerAttrs(i,value)
			end
		elseif(#tokens == 3)then
			local type_
			if(tokens[2] == "侠义值")then
				type_ = HONGYAN_PLAYER_ATTRS_XIAYI	
			elseif(tokens[2] == "风流")then
				type_ = HONGYAN_PLAYER_ATTRS_FENGLIU	
			elseif(tokens[2] == "名声")then
				type_ = HONGYAN_PLAYER_ATTRS_MINGSHENG	
			elseif(tokens[2] == "文韬")then
				type_ = HONGYAN_PLAYER_ATTRS_WENTAO	
			elseif(tokens[2] == "武略")then
				type_ = HONGYAN_PLAYER_ATTRS_WULUE	
			elseif(tokens[2] == "才艺")then		
				type_ = HONGYAN_PLAYER_ATTRS_CAIYI	
			elseif(tokens[2] == "亲和")then		
				type_ = HONGYAN_PLAYER_ATTRS_QINHE	
			elseif(tokens[2] == "礼仪")then		
				type_ = HONGYAN_PLAYER_ATTRS_LIYI	
			elseif(tokens[2] == "相貌")then		
				type_ = HONGYAN_PLAYER_ATTRS_XIANGMAO		
			elseif(tokens[2] == "活力")then		
				type_ = HONGYAN_PLAYER_ATTRS_HUOLI			
			end
			if(type_)then
				player:SetHongYanPlayerAttrs(type_,paras[3])
			end
		end
	elseif(tokens[1] == "@相册激活")then
		player:DoXiangCeJiHuo(paras[2] or 1)
	elseif(tokens[1] == "@友好度")then		
		if(#tokens == 3)then
			local meiren_id = paras[2] or 1
			local val = paras[3] or 1
			if(meiren_id <= 0 or meiren_id > #tb_peri_base)then
				meiren_id = 1
			end
			player:DoJieShiMeiRen(meiren_id)
			player:SetHongYanMeiRenQinMiDu(meiren_id - 1,val)
		elseif(#tokens == 2)then
			local val = paras[2] or 1
			for i = 1,#tb_peri_base
			do
				player:DoJieShiMeiRen(i)
				player:SetHongYanMeiRenQinMiDu(i - 1,val)
			end
		end
	elseif(tokens[1] == "@增加内务")then		
		player:DoAddNeiWuShiJian()
		player:SetHongYanAddNeiWuTime(os.time())
	elseif(tokens[1] == "@出游次数")then		
		player:SetChuYouCount(paras[2] or 1)	
	elseif(tokens[1] == "@红颜天气")then
		local val = paras[2] or 1
		if(val >= 1 and val <= 3)then
			player:SetByte(PLAYER_APPD_INT_FIELD_HONGYAN_BYTE_CHUYOU_1,3, val)
		end
	elseif(tokens[1] == "@slg")then	
		TestSlgFight(player)
	elseif(tokens[1] == "@领地招募刷新")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoFreshLingDiZhaoMuWorkerList(player)
		end
	elseif(tokens[1] == "@领地招募")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiZhaoMuWorker(player,0,paras[2] or 0)
		end
	elseif(tokens[1] == "@领地解雇")then
		local w_pos = paras[2] or 0		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			local w_guid =  lingdiInfo:GetLingDiWorkerGuid(w_pos)
			lingdiInfo:DoLingDiZhaoMuWorker(player,1,w_guid)
		end
	elseif(tokens[1] == "@领地粮食")then		
		if player ~= nil then
			player:AddNumberMaterial(Item_Loot_Food,paras[2] or 1)
			print(string.format("-----------lingdi food cur:%d,max:%d",player:GetNumberMaterial(Item_Loot_Food),player:GetMaxFood()))
		end
	elseif(tokens[1] == "@领地一键上阵")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiWorkerWorking(player,0,"",paras[2] or 1)
		end
	elseif(tokens[1] == "@领地一键下阵")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiWorkerWorking(player,1,"",paras[2] or 1)
		end
	elseif(tokens[1] == "@领地工人结算")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiWorkerWorking(player,2,"",1)
		end
	elseif(tokens[1] == "@打印领地工人动态")then
		-- local report_ptr = playerLib.GetWusheFightPtr(player_ptr)
		-- if(report_ptr ~= nil) then
			-- local report = LingDiZhaoMuEvent:new{ptr = report_ptr}
			-- report:PrintLingDiZhaoMuEvent()
		-- end
	elseif(tokens[1] == "@领地仓库升级")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiWareHouse(player,1)
		end
	elseif(tokens[1] == "@领地仓库等级")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:SetLingDiWareHouseLevel(paras[2] or 1)
		end
	elseif(tokens[1] == "@清空领地仓库")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:SetLingDiResouce(0,0)
			lingdiInfo:SetLingDiResouce(1,0)
			lingdiInfo:SetLingDiResouce(2,0)
			lingdiInfo:SetLingDiResouce(3,0)
		end
	elseif(tokens[1] == "@领地秘境")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiMiJingGo(player,paras[2] or 0,paras[3] or 0)
		end
	elseif(tokens[1] == "@领地秘境包裹")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiMiJingBagOneKeyGet(player,paras[2] or "")
		end
	elseif(tokens[1] == "@打印领地秘境包裹")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			local str = ""
			for i = 1,MAX_LINGDI_MIJING_BAG_SIZE do
				local pos = i - 1
				local entry = lingdiInfo:GetLingDiMiJingBagItemId(pos)
				local count = lingdiInfo:GetLingDiMiJingBagItemCount(pos)
				local isBind = lingdiInfo:GetLingDiMiJingBagItemBind(pos)
				str = string.format("%s,%d,%d,%d",str,entry,count,isBind)
			end
			print("------------lingdi mijing bag:"..str)
		end
	elseif(tokens[1] == "@领地悬赏次数")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:AddLingDiXuanShangCount(paras[2] or 1)
		end
	elseif(tokens[1] == "@领地悬赏进度")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:AddLingDiXuanShangProgress(paras[2] or 1)
		end
	elseif(tokens[1] == "@领地悬赏刷新")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiXuanShangQuestFresh(player)
		end
	elseif(tokens[1] == "@领地悬赏操作")then		
		local lingdiInfo = player:getLingDiInfo()
		if(lingdiInfo ~= nil)then
			lingdiInfo:DoLingDiXuanShang(player,paras[2] or 0,paras[3] or 0)
		end
	elseif(tokens[1] == "@风流镇酒馆竞拍")then		
		if(player ~= nil)then
			player:DoAuctionFengLiuZhenPubWujiang(paras[2] or 0,paras[3] or 0)
		end
	elseif(tokens[1] == "@打印风流镇酒馆信息")then
		local str = ""
		for i=1,MAX_FENGLIUZHEN_PUB_WUJIANG_COUNT do
			local pos = i-1
			local is_zm = 0
			if(globalValue:GetFengLiuZhenPubWuJiangIsZhaoMu(pos))then
				is_zm = 1
			end
			local w_id = globalValue:GetFengLiuZhenPubWuJiangId(pos)
			local s_time = globalValue:GetFengLiuZhenPubWuJiangStayTime(pos)
			local price = globalValue:GetFengLiuZhenPubWuJiangHighPrice(pos)
			local p_guid = globalValue:GetFengLiuZhenPubWuJiangHighPriceGuid(pos)
			local p_name = globalValue:GetFengLiuZhenPubWuJiangHighPriceName(pos)
			str = string.format("%s|pos_%d:%d,%d,%d,%s,%s,%d",str,pos,w_id,s_time,price,p_guid,p_name,is_zm)
		end
		print("-----------fengliuzhen pub info:"..str)
	elseif(tokens[1] == "@打印风流镇酒馆动态信息")then
		outDebug('-----------------FengLiuZhenPubEvent-----------------')
		for i = 0, MAX_FENGLIUZHEN_PUB_EVENT-1 do
			outDebug('FengLiuZhenPubEvent time = '..globalValue:GetUInt32(GLOBALVALUE_INT_FIELD_FENGLIUZHEN_PUB_EVENT_INFO_START + i * MAX_FENGLIUZHEN_PUB_EVENT_INT_FIELD + FENGLIUZHEN_PUB_EVENT_INT_FIELD_TIME))
			outDebug('FengLiuZhenPubEvent type = '..globalValue:GetByte(GLOBALVALUE_INT_FIELD_FENGLIUZHEN_PUB_EVENT_INFO_START + i * MAX_FENGLIUZHEN_PUB_EVENT_INT_FIELD + FENGLIUZHEN_PUB_EVENT_INT_FIELD_TYPE,0))
			outDebug('FengLiuZhenPubEvent w_id = '..globalValue:GetUInt32(GLOBALVALUE_INT_FIELD_FENGLIUZHEN_PUB_EVENT_INFO_START + i * MAX_FENGLIUZHEN_PUB_EVENT_INT_FIELD + FENGLIUZHEN_PUB_EVENT_INT_FIELD_WUJIANG_ID))
			outDebug('FengLiuZhenPubEvent price = '..globalValue:GetUInt32(GLOBALVALUE_INT_FIELD_FENGLIUZHEN_PUB_EVENT_INFO_START + i * MAX_FENGLIUZHEN_PUB_EVENT_INT_FIELD + FENGLIUZHEN_PUB_EVENT_INT_FIELD_COST))
			outDebug('FengLiuZhenPubEvent guid = '..globalValue:GetStr(GLOBALVALUE_STRING_FIELD_FENGLIUZHEN_PUB_EVENT_INFO_START + i * MAX_FENGLIUZHEN_PUB_EVENT_STR_FIELD + FENGLIUZHEN_PUB_EVENT_STR_FIELD_PLAYER_GUID))
			outDebug('FengLiuZhenPubEvent name = '..globalValue:GetStr(GLOBALVALUE_STRING_FIELD_FENGLIUZHEN_PUB_EVENT_INFO_START + i * MAX_FENGLIUZHEN_PUB_EVENT_STR_FIELD + FENGLIUZHEN_PUB_EVENT_STR_FIELD_PLAYER_NAME))
		end	
	elseif(tokens[1] == "@添加武将")then		
		local wujianginfo = player:getWuJiangInfo()
		local id = paras[2] or 1
		if(wujianginfo ~= nil)then
			wujianginfo:AddWujiang(player,id)
		end
	elseif(tokens[1] == "@激活外观")then		
		local val = paras[2] or 1
		player:SetAvatarOpen(val-1)
		playerLib.SendAttr(player_ptr)	
	elseif(tokens[1] == "@领地征兵操作")then		
		local lingdiInfo = player:getLingDiInfo()
		if lingdiInfo ~= nil then
			lingdiInfo:DoLingDiZhengBing(player,paras[2] or 0,paras[3] or 0)
		end
	elseif(tokens[1] == "@领地征兵等级")then		
		local lingdiInfo = player:getLingDiInfo()
		if lingdiInfo ~= nil then
			lingdiInfo:DoUpdateLingDiBingYingLevel(player,paras[2] or 0)
		end
	elseif(tokens[1] == "@领地征兵科技等级")then		
		local lingdiInfo = player:getLingDiInfo()
		if lingdiInfo ~= nil then
			lingdiInfo:SetLingDiScienceLevel(paras[2] -1 or 0,paras[3] or 0)
			lingdiInfo:DoSomethingByLingDiScienceLevel(player,paras[2] or 0,paras[3] or 0)
		end
	elseif(tokens[1] == "@领地手动征兵")then		
		local lingdiInfo = player:getLingDiInfo()
		if lingdiInfo ~= nil then
			lingdiInfo:AddLingDiBingYingManualDraft(paras[2] or 0)
			player:AddNumberMaterial(Item_Loot_BingLi,paras[2] or 0)
		end
	elseif(tokens[1] == "@重置领地征兵")then		
		local lingdiInfo = player:getLingDiInfo()
		if lingdiInfo ~= nil then
			lingdiInfo:DoResetLingDiZhengBingInfo(player)
		end
	elseif(tokens[1] == "@打印领地征兵信息")then		
		local lingdiInfo = player:getLingDiInfo()
		if lingdiInfo == nil then return end
		outDebug('-----------------LingDiZhengBingInfo-----------------')
		outDebug('LingDiZhengBingInfo bingli = '..player:GetUInt32(PLAYER_INT_FIELD_BINGLI))
		outDebug('LingDiZhengBingInfo bingli_max = '..player:GetUInt32(PLAYER_INT_FIELD_BINGLI_MAX))
		outDebug('LingDiZhengBingInfo bingying_lv = '..lingdiInfo:GetUInt32(LINGDI_INT_FIELD_BINGYING_LEVEL))
		outDebug('LingDiZhengBingInfo speed = '..lingdiInfo:GetUInt32(LINGDI_INT_FIELD_BINGYING_SPEED))
		outDebug('LingDiZhengBingInfo manual = '..lingdiInfo:GetUInt32(LINGDI_INT_FIELD_BINGYING_MANUAL_DRAFT))
		outDebug('LingDiZhengBingInfo manual_max = '..lingdiInfo:GetUInt32(LINGDI_INT_FIELD_BINGYING_MANUAL_DRAFT_MAX))
		outDebug('LingDiZhengBingInfo next_time = '..lingdiInfo:GetUInt32(LINGDI_INT_FIELD_BINGYING_AUTO_DRAFT_NEXT_TIME))
		for i= 1,MAX_LINGDI_BINGYING_SCIENCE do
			outDebug('LingDiZhengBingInfo science_id='..i..',science_lv = '..lingdiInfo:GetUInt32(LINGDI_INT_FIELD_BINGYING_SCIENCE_LEVEL_START + i-1))
		end
	elseif(tokens[1] == "@清理兵力")then		
		player:SetNumberMaterial(Item_Loot_BingLi,0)
	elseif(tokens[1] == "@辅助技能")then		
		if(#tokens == 2 or #tokens == 1)then
			local val = paras[2] or 1
			if(val > 400)then
				val = 400
			end
			for i = 1,#tb_assistspell
			do
				player:SetPlayerAssistspellLv(i,val)
				playerLib.SendAttr(player_ptr)
			end
		elseif(#tokens == 3)then
			local type
			local val = paras[3] or 1
			if(val > 400)then
				val = 400
			end
			if(tokens[2] == "长生诀")then
				type = 1	
			elseif(tokens[2] == "九阳功")then
				type = 2	
			elseif(tokens[2] == "九阴决")then
				type = 3	
			elseif(tokens[2] == "玄武诀")then
				type = 4	
			elseif(tokens[2] == "归元诀")then
				type = 5	
			elseif(tokens[2] == "追魂诀")then		
				type = 6	
			elseif(tokens[2] == "凌风诀")then		
				type = 7	
			elseif(tokens[2] == "狂圣诀")then		
				type = 8	
			elseif(tokens[2] == "摩诃诀")then		
				type = 9				
			end
			if(type)then
				player:SetPlayerAssistspellLv(type,val)
				playerLib.SendAttr(player_ptr)
			end
		end	
	elseif(tokens[1] == "@转盘")then
		local val = paras[2] or 1			
		local zphandler = require('appd.fengliuzhen.ZhuanpanHandler')
		zphandler:enter_room(player,val)
	elseif(tokens[1] == "@删除武将")then
		
		local wujianginfo = player:getWuJiangInfo()
		if(wujianginfo ~= nil)then
			if(#tokens == 1)then
				for index = 1,MAX_WUJIANG_COUNT
				do
					wujianginfo:ClearWujiangInfo(index)
				end
				wujianginfo:SetHasWujiangCount(0)
			elseif(#tokens == 2)then
				local id = paras[2] or 1
				for index = 1,MAX_WUJIANG_COUNT
				do
					if(wujianginfo:GetWujiangID(index) == id)then
						local wj_guid = wujianginfo:GetWuJiangGuid(index)
						wujianginfo:DeleteWujiang(wj_guid)
						break
					end
				end
			end
		end
	elseif(tokens[1] == "@武将征战操作")then		
		player:doWuJiangZhengZhan(paras[2] or 0, paras[3] or 0,paras[4] or 0)
	elseif(tokens[1] == "@武将征战关卡")then
		local pass_id = paras[2] or 0
		if(pass_id <= 0)then return end
		for i = 1,pass_id do
			player:SetZhengZhanPassIsActivity(i-1,1)
			player:SetZhengZhanPassStar(i-1,3)
		end
		--激活下一关卡
		local next_pass = pass_id + 1
		if(next_pass <= #tb_wujiang_instance)then
			player:SetZhengZhanPassIsActivity(pass_id,1)
		end
	elseif(tokens[1] == "@增加军令")then		
		player:AddNumberMaterial(Item_Loot_JunLing,paras[2] or 0)
	elseif(tokens[1] == "@清理军令")then		
		player:SetNumberMaterial(Item_Loot_JunLing,0)
	elseif(tokens[1] == "@打印武将征战信息")then		
		outDebug('-----------------WuJiangZhengZhanInfo-----------------')
		outDebug('WuJiangZhengZhanInfo junling = '..player:GetUInt32(PLAYER_INT_FIELD_JUNLING))
		outDebug('WuJiangZhengZhanInfo buycount = '..player:GetUInt16(PLAYER_INT_FIELD_JUNLING_BUY_COUNT_UINT16,0))
		outDebug('WuJiangZhengZhanInfo buycount_max = '..player:GetUInt16(PLAYER_INT_FIELD_JUNLING_BUY_COUNT_UINT16,1))
		outDebug('WuJiangZhengZhanInfo next_time = '..player:GetUInt32(PLAYER_INT_FIELD_JUNLING_HUIFU_NEXT_TIME))
		for i= 1,MAX_ZHENGZHAN_PASS_COUNT do
			local isac = player:GetByte(PLAYER_INT_FIELD_ZHENGZHAN_PASS_BYTE_START + i-1,0)
			local star = player:GetByte(PLAYER_INT_FIELD_ZHENGZHAN_PASS_BYTE_START + i-1,1)
			local cc = player:GetByte(PLAYER_INT_FIELD_ZHENGZHAN_PASS_BYTE_START + i-1,2)
			outDebug('WuJiangZhengZhanInfo pass_id='..i..',act ='..isac..',star ='..star..',num ='..cc)
		end
		for i= 1,6 do
			local isGet = 0
			if(player:GetBit(PLAYER_INT_FIELD_ZHENGZHAN_CHAPTER_FLAG , i-1) == true)then
				isGet = 1
			end
			outDebug('WuJiangZhengZhanInfo chapter_id='..i..',get ='..isGet)
		end
	elseif(tokens[1] == "@武将")then		
		local wujianginfo = player:getWuJiangInfo()
		if(wujianginfo ~= nil)then
			local id = paras[3] or 1
			local val = paras[4] or 1
			if(tokens[2] == "等级")then
				for index = 1,MAX_WUJIANG_COUNT
				do
					if(wujianginfo:GetWujiangID(index) == id)then
						wujianginfo:SetWujiangLevel(index,val)
						break
					end
				end
			elseif(tokens[2] == "升星")then
				for index = 1,MAX_WUJIANG_COUNT
				do
					if(wujianginfo:GetWujiangID(index) == id)then
						wujianginfo:SetWujiangStarsLv(index,val)
						break
					end
				end
			elseif(tokens[2] == "转生")then	
				for index = 1,MAX_WUJIANG_COUNT
				do
					if(wujianginfo:GetWujiangID(index) == id)then
						wujianginfo:SetWujiangZhuanShengLv(index,val)
						break
					end
				end
			end
		end	
	elseif(tokens[1] == "@军功")then
		local val = paras[2] or 1
		player:SetUInt32(PLAYER_INT_FIELD_JUNGONG,val)
	elseif(tokens[1] == "@签到")then
		if(#tokens < 2)then
			return result
		end
		local day = paras[2]
		player:WelfareCheckIn(day)
	elseif(tokens[1] == "@制造")then
		
		if(#tokens < 2)then
			return result
		end
		local id = paras[2]
		if(id == 0)then
			for key,val in pairs(tb_item_template) do
				if(tokens[2] == val.name)then
					id = val.id
					break
				end
			end
		end

		--100之后才是物品~
		if id <= 100 or not tb_item_template[id] then
			return result
		end

		--outFmtDebug("zhiao2")
		local count = paras[3] or 1
		local bind = paras[4] or false
		local strong = paras[5] or 0
		local fail_time = paras[6] or 0
		local itemMgr = player:getItemMgr()
		local bagType = tb_item_template[id].belong_bag
		if bagType >= 0 then
			player:AppdAddItems({{id, count}}, MONEY_CHANGE_GM_COMMAND, LOG_ITEM_OPER_TYPE_GM_COMMAND)
		end
	elseif(tokens[1] == "@清理")then
		local bag_id = paras[2] or BAG_TYPE_MAIN_BAG
		if(bag_id > MAX_BAG)then
			return result
		end
		
		local itemMgr = player:getItemMgr()
		itemMgr:clearBag(bag_id)
		if(bag_id == BAG_TYPE_EQUIP)then
			player:UpdatePlayerEquipment()
			playerLib.SendAttr(player_ptr)		-- 属性重算下
		end
	elseif(tokens[1] == "@装备")then
		local level = paras[2] or 1 
		local quality = paras[3] or 1
		local streng_lv = paras[4] or 0
		local gender = player:GetGender()
		local itemMgr = player:getItemMgr()
		itemMgr:clearBag(BAG_TYPE_EQUIP)
		for i = EQUIPMENT_TYPE_MAIN_WEAPON,EQUIPMENT_TYPE_LRING
		do
			for _,val in pairs(tb_item_template) do
				-- 类型，部位，等级，品质，性别
				if(ITEM_TYPE_EQUIP == val.type and i == val.pos and level == val.level and quality == val.quality and gender == val.sex)then
					itemMgr:addItem(val.id,1,true,true,true,streng_lv,0,BAG_TYPE_EQUIP,i)
					break
				end
			end
 		end
		player:UpdatePlayerEquipment()
 		playerLib.SendAttr(player_ptr)		-- 属性重算下
	elseif(tokens[1] == "@风流镇主考官操作")then
		player:doFengLiuZhenExam(paras[2] or 0,paras[3] or 0)
	elseif(tokens[1] == "@打印风流镇主考官")then
		outDebug('-----------------FengLiuZhenExamInfo-----------------')
		outDebug('FengLiuZhenExamInfo cur_inedx = '..player:GetByte(PLAYER_INT_FIELD_FENGLIUZHEN_EXAM_TITLE_DATA,0))
		outDebug('FengLiuZhenExamInfo score = '..player:GetUInt32(PLAYER_INT_FIELD_FENGLIUZHEN_EXAM_CUR_SCORE))
		outDebug('FengLiuZhenExamInfo over_time = '..player:GetUInt32(PLAYER_INT_FIELD_FENGLIUZHEN_EXAM_TITLE_OVER_TIME))
		for i= 1,MAX_FENGLIUZHEN_EXAM_TITLE_COUNT do
			local off = i - 1
			local idx = off % 2
			off = math.floor(off / 2)
			local title_id = player:GetUInt16(PLAYER_INT_FIELD_FENGLIUZHEN_EXAM_TITLE_ID_START + off,idx)
			local is_answer = 0
			if(player:GetBit(PLAYER_INT_FIELD_FENGLIUZHEN_EXAM_TITLE_FLAG,i-1))then
				is_answer = 1
			end
			outDebug('FengLiuZhenExamInfo idx = '..i..' title_id='..title_id.." is_answer = "..is_answer)
		end
		
	elseif(tokens[1] == "@主考官时间")then
		local room_id = player:GetKuafuWarId()
		if app.flzMgr then
			local zhukaoguan = app.flzMgr:getZhuKaoguanInfo(room_id)
			if(#tokens >= 2)then
				zhukaoguan:clearRankPlayerData()
				globalValue:setFengLiuZhenExamStartTime(paras[2] or 0)
			else
				zhukaoguan:randExamStartTime()
			end
		end
		
	elseif(tokens[1] == "@打印风流镇主考官排名")then
		outDebug('FengLiuZhenExamRank start_time = '..globalValue:GetUInt32(GLOBALVALUE_INT_FENGLIUZHEN_EXAM_START_TIME))
		local room_id = player:GetKuafuWarId()
		if app.flzMgr then
			local zhukaoguan = app.flzMgr:getZhuKaoguanInfo(room_id)
			zhukaoguan:printZhukaoguanInfo()
		end		
	elseif(tokens[1] == "@清理摊位")then
		for i = 1,6 do
			local stall_data = app.dbAccess:searchTradeItem(-i,0,0,0,"",paras[2] or 6,paras[3] or 0,1)
			local results = stall_data[2]
			print("------stall results len "..#results..' '..-i)
			for k,v in pairs(results) do
				app.dbAccess:deleteStallItem(v.s_item_guid, v.owner_guid)
			end
		end
	elseif(tokens[1] == "@添加武将装备")then
		local WjEquipInfo = player:getWjEquipInfo()
		local count = paras[3] or 1
		for i = 1,count do
			WjEquipInfo:addEquip(player,paras[2] or 1)
		end
	elseif(tokens[1] == "@删除武将装备")then
		local WjEquipInfo = player:getWjEquipInfo()
		local id = paras[2] or 1
		for i = 1,MAX_WUJIANG_EQUIP_COUNT
		do
			if(id == WjEquipInfo:getEquipID(i))then
				WjEquipInfo:delEquip(WjEquipInfo:getEquipGuid(i))
				break
			end
		end
	elseif(tokens[1] == "@转盘信息")then		
		local zhuanpan = app.objMgr:getObj(player:GetZhuanpanGuid())
		if(zhuanpan)then
			zhuanpan:printZhuanpanInfo()
		end
	elseif(tokens[1] == "@会试")then
		local kaozhuangyuan = app.objMgr:getObj(player:GetKaozhuangyuanGuid())
		if(kaozhuangyuan)then
			for i = 1,16
			do
				kaozhuangyuan:setFailType(i,0)
				kaozhuangyuan:setVictoryType(i,1)
			end
			for i = 17,32
			do
				kaozhuangyuan:setFailType(i,1)
			end
			kaozhuangyuan:setRoomState(4)
		end
	elseif(tokens[1] == "@殿试")then
		local kaozhuangyuan = app.objMgr:getObj(player:GetKaozhuangyuanGuid())
		if(kaozhuangyuan)then
			for i = 1,8
			do
				kaozhuangyuan:setFailType(i,0)
				kaozhuangyuan:setVictoryType(i,2)
			end
			for i = 9,32
			do
				kaozhuangyuan:setFailType(i,2)
			end
			kaozhuangyuan:setRoomState(8)
		end	
	elseif tokens[1] == "@公告" then
		if(#tokens < 2)then
			return result
		end
		local content = tokens[2]
		app:SendNotice(NOTICE_TYPE_SCROLL, content)		
	elseif tokens[1] == "@世界提示" then
		if(#tokens < 2)then
			return result
		end
		local content = tokens[2]
		app:SendNotice(NOTICE_TYPE_PROMPT, content)
	elseif tokens[1] == "@技能激活" then
		if(#tokens < 2)then
			return result
		end
		local spell_id = paras[2]
		if(spell_id < 1 or spell_id > 30)then
			return result
		end
		local config = tb_skill_base[spell_id]
		if(config == nil or config.is_open == 1)then
			return result
		end
		player:ActiveSpell(spell_id)
	elseif tokens[1] == "@技能升级" then
		if(#tokens < 3)then
			return result
		end
		local spell_id = paras[2] or 1
		local level = paras[3] or 1 
		player:SetSpellSysSpellLevel(spell_id,level)
		playerLib.SendAttr(player_ptr)
	elseif tokens[1] == "@找回重置" then
		player:SetWelfareBackAllNum()
	elseif tokens[1] == "@找回" then
		if(#tokens < 4)then
			return result
		end
		player:AddWelfareBackGmLog(paras[2],paras[3],paras[4])
	elseif tokens[1] == "@次数3v3" then
		if(#tokens < 2)then
			return result
		end
		player:Gm3v3EnterTimes(paras[2])
	elseif tokens[1] == "@发奖3v3" then
		Rank3v3kuafuWeek()
	elseif tokens[1] == "@段位" then
		if(#tokens < 2)then
			return result
		end
		player:SetKuafu3v3TotalScore(paras[2])
	elseif tokens[1] == "@段位奖励" then
		player:Rank3v3SegmentReward()
	elseif tokens[1] == "@斗剑台" then
		player:OnDoujiantaiFight(1)
	elseif tokens[1] == "@斗剑台连胜" then
		if(#tokens < 2)then
			return result
		end
		player:DoujianCombatWin(paras[2]==1)
	elseif tokens[1] == "@斗剑台首胜" then
		if(#tokens < 2)then
			return result
		end
		player:DoujianFirstReward(paras[2])
	elseif tokens[1] == "@斗剑台每日" then
		player:DoDoujiantaiDayReward()
	elseif tokens[1] == "@成就" then
		if(#tokens < 3)then
			return result
		end
		
		player:AddAchieve(paras[2],paras[3])
	
	elseif tokens[ 1 ] == '@冒险时间' then
		local hours = 1
		if tokens[2] then
			hours = tonumber(tokens[2])
		end
		player:SubUInt32(PLAYER_INT_FILED_LEAVE_RISK_TIME, hours * 3600)
	end
	
	return result
end
