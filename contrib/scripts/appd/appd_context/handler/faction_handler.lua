--角色登录 帮派处理
function PlayerInfo:factionLogin()
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local faction = app.objMgr:getObj(factionID)
		if faction then
			local player_guid = self:GetGuid()
			local index = faction:FindPlayerIndex(player_guid)
			if index == nil then
				self:clearFaction()
			else 
				faction:SetFactionMemberIsOnline(index,1)
				faction:SetFactionMemberLogoutTime(index,0)
			end
		else
			self:clearFaction()
		end
	end
end
--角色退出 帮派处理
function PlayerInfo:factionLogOut()
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local faction = app.objMgr:getObj(factionID)
		if faction then
			local player_guid = self:GetGuid()
			local index = faction:FindPlayerIndex(player_guid)
			if index ~= nil then
				faction:SetFactionMemberIsOnline(index,0)
				faction:SetFactionMemberLogoutTime(index,os.time())
			end
		end
	end
end
--帮派每日重置
function PlayerInfo:factionReset()
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local faction = app.objMgr:getObj(factionID)
		if faction then
			--faction:RefreshShop()
			faction:ResetMember(self)
		end
	end
end

function PlayerInfo:factionUpLevel()
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local lev = self:GetLevel()
		local faction = app.objMgr:getObj(factionID)
		if faction then
			local player_guid = self:GetGuid()
			local index = faction:FindPlayerIndex(player_guid)
			if index ~= nil then
				faction:SetFactionMemberLevel(index,lev)
			end
		end
	end
end

function PlayerInfo:factionUpForce()
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local force = self:GetForce()
		local faction = app.objMgr:getObj(factionID)
		if faction then
			local player_guid = self:GetGuid()
			local index = faction:FindPlayerIndex(player_guid)
			if index ~= nil then
				faction:SetFactionMemberForce(index,force)
			end
		end
	end
end
--清理角色帮派信息
function PlayerInfo:clearFaction()
	self:SetFactionId("")
	self:SetFactionName("")
	--Todo 退工会处理商店购买记录
	self:ClearBuyedFactionShopItem()
end
--创建帮派
function PlayerInfo:Handle_Faction_Create( pkt )
	local name = pkt.name
	local icon = pkt.icon

	outFmtDebug("Handle_Faction_Create")
	
	--玩家已经有帮派了
	if self:GetFactionId() ~= "" then
		self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_IS_HAVE)
		return
	end
	
	outFmtDebug("------------ %s",self:GetFactionId())
	
	local config = tb_faction_creat[1]

	
	-- 玩家名字不能是空的
	if self:GetName() == "" then
		--self:CallOptResult(OPRATE_TYPE_FACTION, OPRATE_TYPE_FACTION_PLAYER_NAME_ERR)
		outFmtInfo("user name null")
		return
	end
	
	--帮派名称不能为空 帮派名称不能超过6个中文字符

	if name == "" or string.utf8len(name) > 7 then
		self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NAME_ERR)
		return
	end
	
	--判断是否有屏蔽词
	local name = fuckPingBi(pkt.name)
	if name ~= pkt.name then
		self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NAME_HAVE_FUCK)
		return
	end
	
	
	--判断帮派名字是否重复
	local name_tab = lua_string_split(self:GetName(), ',')
	local faction_name = string.format("%s,%s,%s", name_tab[1], name_tab[2], name)
	local server_name = string.format("%s_%s", name_tab[1], name_tab[2])
	
	if not checkFactionNameAvailable (faction_name) then
		self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NAME_REPEAT)
		return
	end
	
	--判断帮派数量上限
	local faction_num = 0
	app.objMgr:foreachAllFaction(function(faction)
		faction_num = faction_num + 1
	end)
	if faction_num >= config.maxnum then
		self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_CREATE_MAX)
		return
	end
	

	--扣除相应资源 + 祝福值
	if not self:costMoneys(MONEY_CHANGE_CREATE_FACTION,config.cost) then
	 	self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_CREATE_COST)
		return
	end
	
	self:CreateFaction(server_name, faction_name, icon)
end

function PlayerInfo:NewPlayerCreateFaction(name, icon)
	
	local config = tb_faction_creat[1]

	-- 玩家名字不能是空的
	if self:GetName() == "" then
		outFmtInfo("user name null")
		return
	end
	
	
	--判断是否有屏蔽词
	local name_tab = string.split(self:GetName(), ',')
	
	local common = "的后宫"
	--帮派名称不能为空 帮派名称不能超过6个中文字符
	if name == "" or string.utf8len(name) > 7 or name ~= fuckPingBi(name) then
		name = string.format("%s%s", name_tab[ 3 ], common)
	end
			
	--判断帮派名字是否重复
	local faction_name = string.format("%s,%s,%s", name_tab[1], name_tab[2], name)
	local server_name = string.format("%s_%s", name_tab[1], name_tab[2])
	
	local dict = GetFactionNameDict()
	if not dict[faction_name] then
		for i = 0, 3000 do
			faction_name = string.format("%s,%s,%s%sd", name_tab[1], name_tab[2], name_tab[ 3 ], common, i)
			if checkFactionNameAvailable (faction_name) then
				break
			end
		end
	end
	
	--判断帮派数量上限
	local faction_num = 0
	app.objMgr:foreachAllFaction(function(faction)
		faction_num = faction_num + 1
	end)
	if faction_num >= config.maxnum then
		self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_CREATE_MAX)
		return
	end

	return self:CreateFaction(server_name, faction_name, icon)
end

function checkFactionNameAvailable (faction_name)
	local bRepeat = false
	app.objMgr:foreachAllFaction(function(faction)
		if faction:GetName() == faction_name then
			bRepeat = true
			return true
		end
	end)
	
	return not bRepeat
end

function GetFactionNameDict()
	local dict = {}
	app.objMgr:foreachAllFaction(function(faction)
		dict[faction:GetName()] = 1
		return true
	end)
	
	return dict
end

-- 实际创建帮派
function PlayerInfo:CreateFaction(server_name, faction_name, icon)
	-- 获取guid
	local new_guid = guidMgr:Make_New_Guid(guidMgr.ObjectTypeFaction, guidMgr:NewIndex(guidMgr.ObjectTypeFaction), server_name)
	local faction = app.objMgr:newAndCallPut(new_guid, FACTION_BINLOG_OWNER_STRING)
	if(not faction)then
		return
	end
	
	-- 获取帮派数据的guid
	local data_guid = guidMgr.replace(new_guid, guidMgr.ObjectTypeFactionData)
	local factionData = app.objMgr:newAndCallPut(data_guid, FACTION_DATA_OWNER_STRING)
	if(not factionData)then
		return
	end
	
	local faction_lv = 1

	faction:SetName(faction_name)
	faction:SetFactionLevel(faction_lv)
	faction:SetBangZhuName(self:GetName())
	faction:SetBangZhuGuid(self:GetGuid())
	
	faction:SetBangZhuInfo(self)
	faction:SetFactionCurFlagId(icon)

	if not faction:MemberAdd(self) then
		app.objMgr:callRemoveObject(new_guid)
		self:SetFactionId("")
		self:SetFactionName("")
		return
	end
	
	faction:OnMainHallLvUp(1) -- 创建1级主殿
	--faction:RefreshShop()
	
	
	--登录服也监听下
	app.objMgr:callAddWatch(serverConnList:getLogindFD(), new_guid)
	app.objMgr:callAddWatch(serverConnList:getLogindFD(), data_guid)
	
	--通知场景服监听
	serverConnList:forEachScenedFD(function (fd)
		app.objMgr:callAddWatch(fd, new_guid)
	end)
	
	rankInsertTask(faction:GetGuid(), RANK_TYPE_FACTION)
	
	return true
end

-- 升级
function PlayerInfo:Handle_Faction_Upgrade(pkt)
	local faction_guid = self:GetFactionId()
	if faction_guid == "" then
		outFmtDebug("Handle_Faction_Upgrade with no faction = ")
		return
	end
	if not app.objMgr:IsFactionGuid(faction_guid) then
		return
	end
	local faction = app.objMgr:getObj(faction_guid)
	if faction then
		faction:FactionLevelUp(self)
	end
end

--获取帮派列表
function PlayerInfo:Handle_Faction_Get_List( pkt )
	local page, num,grep = pkt.page, pkt.num,pkt.grep 
	
	if page < 1 or num < 1 then
		return
	end
	local lev = 0
	--outFmtDebug("self grep %d",grep)
	if grep == 1 then
		lev = self:GetLevel()
		--outFmtDebug("self level: %d",lev)
	end
	playerLib.FastGetFactionList(self:GetGuid(),page,num,lev)
	
end

--快速加入帮派
function PlayerInfo:Handle_Faction_FastJoin(pkt)
	outFmtDebug("Handle_Faction_FastJoin")
	local flag = true
	app.objMgr:foreachAllFaction(function(faction)
		
		if faction:GetFactionFlags(FACTION_FLAGS_AUTO) then
			local faclev = faction:GetFactionMinLev()
			if faclev > 0 then
				local lev = self:GetLevel()
				if lev >= faclev then
					flag = false
					return faction:MemberAdd(self)
				end
			else
				flag = false
				return faction:MemberAdd(self)
			end
		end
			
		
	end)
	
	if flag then
		self:CallOptResult(OPERTE_TYPE_FACTION, OPEATE_TYPE_FACTION_NOT_JOIN)
	end
	
end

--帮派申请
function PlayerInfo:Hanlde_Faction_Apply( pkt )
	local faction_guid = self:GetFactionId()
	if faction_guid ~= "" then
		outFmtDebug("you cannot join other, have faction = %s", faction_guid)
		return
	end
	
	local target_faction_guid = pkt.id
	
	if not app.objMgr:IsFactionGuid(target_faction_guid) then
		return
	end 
	
	local faction = app.objMgr:getObj(target_faction_guid)   --帮派guid
	
	if faction then		
		if self:GetLevel() < faction:GetFactionMinLev() then
			--玩家等级不够
			self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_LEV_LOW)
			return 
		end

		if faction:GetFactionFlags(FACTION_FLAGS_AUTO) then
			faction:MemberAdd(self)
		else
			faction:FactionApply(self)
		end
		
	end	
end

--退出帮派
function PlayerInfo:Handle_Faction_Quit( pkt )
	local faction_guid = self:GetFactionId()
	if faction_guid == "" then
		return
	end
	local faction = app.objMgr:getObj(faction_guid)
	if faction then
		faction:FactionQuit(self)
	end
end

--帮派管理
function PlayerInfo:Hanlde_Faction_Manager( pkt )
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
	
	local opt_type = pkt.opt_type			--操作类型
	local reserve_int1 = pkt.reserve_int1   --预留int值1*/
	local reserve_int2 = pkt.reserve_int2   --预留int值2*/
	local reserve_str1 = pkt.reserve_str1   --预留string值1*/
	local reserve_str2 = pkt.reserve_str2   --预留string值2*/
	
	--同意加入帮派
	if opt_type == FACTION_MANAGER_TYPE_AGREE_JOIN then
		faction:FactionAgreeJoin(self,reserve_str1)
	--拒绝加入帮派
	elseif opt_type == FACTION_MANAGER_TYPE_REFUSE_JOIN then
		faction:FactionRefuseJoin(self,reserve_str1)
	--踢出帮派	
	elseif opt_type == FACTION_MANAGER_TYPE_KICK then
		faction:MemberKicked(self, reserve_str1)
	--职位任免	
	elseif opt_type == FACTION_MANAGER_TYPE_APPOINT then
		faction:FactionAppoint(self, reserve_str1,reserve_int1)	
	--招募设置
	elseif opt_type == FACTION_MANAGER_TYPE_RECRUIT then
		faction:FactionRecruit(self, reserve_int1,reserve_int2,reserve_str1)
	--	帮派升级
	elseif opt_type == FACTION_MANAGER_TYPE_LEVEL_UP then
		faction:FactionLevelUp(self)	
	--	替换帮旗
	elseif opt_type == FACTION_MANAGER_TYPE_CHANGE_FLAGS then
		--faction:FactionChangeFlags(self,reserve_int1)		
	--帮会公告	
	elseif opt_type == FACTION_MANAGER_TYPE_NOTICE then	
		faction:FactionNotice(self,reserve_str1)	
	--同意所有加入帮派
	elseif opt_type == FACTION_MANAGER_TYPE_AGREE_JOIN_ALL then
		faction:FactionAgreeJoinAll(self)	
	--拒绝所有加入帮派
	elseif opt_type == FACTION_MANAGER_TYPE_REFUSE_JOIN_ALL then
		faction:FactionRefuseJoinAll(self)	
	elseif opt_type == FACTION_MANAGER_TYPE_QUIT then
		faction:FactionExit(self)	
	end
	
end

--帮众操作
function PlayerInfo:Handle_Faction_People( pkt )
	--outFmtDebug("juanxian")
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
	local data_guid = guidMgr.replace(faction_guid, guidMgr.ObjectTypeFactionData)
	local factionData = app.objMgr:getObj(data_guid)
	
	local pos = faction:FindPlayerIndex(self:GetGuid())
	if pos == nil then return end
	
	local opt_type = pkt.opt_type			--操作类型
	local reserve_int1 = pkt.reserve_int1   --预留int值1*/
	local reserve_int2 = pkt.reserve_int2   --预留int值2*/
	local reserve_str1 = pkt.reserve_str1   --预留string值1*/
	local reserve_str2 = pkt.reserve_str2   --预留string值2*/
	
	--捐献
	if opt_type == FACTION_MANAGER_TYPE_JUANXIAN then
		faction:FactionJuanXian(self,pos,reserve_int1,reserve_int2)
		--faction:RefreshShop()
	--领取福利	
	elseif opt_type == FACTION_MANAGER_TYPE_FULI then
		--faction:FactionFuLi(self,pos)
	--发红包	
	elseif opt_type == FACTION_MANAGER_TYPE_FA_HONGBAO then
		--faction:FactionFaHongBao(self,reserve_int1,reserve_int2)
	--领取红包	
	elseif opt_type == FACTION_MANAGER_TYPE_LQ_HONGBAO then	
		--faction:FactionLqHongBao(self,pos)
	--上香	
	elseif opt_type == FACTION_MANAGER_TYPE_SHANGXIANG then	
		--faction:FactionShangXiang(self,pos,reserve_int1)
	--
	elseif opt_type == FACTION_MANAGER_TYPE_SHOP then
		faction:ShopItem(self,reserve_int1,reserve_int2)
	--购买令牌
	elseif opt_type == FACTION_MANAGER_TYPE_BUY_TOKEN then
		faction:BuyToken(self,reserve_int1)
	--挑战家族boss
	elseif opt_type == FACTION_MANAGER_TYPE_CHALLENGE_BOSS then
		faction:ChallengeBoss(self,reserve_int1)
		
	--升级建筑
	elseif opt_type == FACTION_MANAGER_TYPE_BUILDING_UPGRADE then
		faction:UpgradeBuilding(self,reserve_int1)
	--加速升级建筑
	elseif opt_type == FACTION_MANAGER_TYPE_BUILDING_UPGRADE_SPEEDUP then
		faction:UpgradeBuildingSpeedUp(self,reserve_int1)
		
	--贡献礼包兑换
	elseif opt_type == FACTION_MANAGER_TYPE_DONATE_GIFT_EXCHANGE then
		faction:FactionDonateGiftExchange(self,reserve_int1)
		
	elseif opt_type == FACTION_MANAGER_TYPE_BOSSDEFENSE_CHALLENGE then
		faction:BossDenfenseChallenge(self,reserve_int1)
		
	elseif opt_type == FACTION_MANAGER_TYPE_BOSSDEFENSE_DAMAGE_LIST then
		faction:GetBossDenfenseDamageList(self,reserve_int1)
		
	elseif opt_type == FACTION_MANAGER_TYPE_TOWER_CHALLENGE then
		faction:TowerChallenge(self)
		
	elseif opt_type == FACTION_MANAGER_TYPE_TOWER_SWEEP then
		faction:TowerSweep(self)
		
	elseif opt_type == FACTION_MANAGER_TYPE_TOWER_CHEST then
		faction:OpenTowerChest(self,reserve_int1)
		
	elseif opt_type == FACTION_MANAGER_TYPE_SKILL_LVUP then
		faction:FactionSkillLvup(self,reserve_int1)
		
		
	end
	
	
end


function PlayerInfo:Handle_Storehouse_Hand_In(pkt)
	local pos_str = pkt.pos_str
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local faction = app.objMgr:getObj(factionID)
		if faction then
			faction:HandInReward(self, pos_str)
		end
	end
end

function PlayerInfo:Handle_Storehouse_Exchange(pkt)
	local pos = pkt.pos
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local faction = app.objMgr:getObj(factionID)
		if faction then
			faction:ExchangeSoreHouseItem(self, pos)
		end
	end
end

function PlayerInfo:Handle_Storehouse_Destroy(pkt)
	local pos = pkt.pos
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local faction = app.objMgr:getObj(factionID)
		if faction then
			faction:DestroySoreHouseItem(self, pos)
		end
	end
end

function PlayerInfo:Handle_Send_Faction_Invite(pkt)
	local guid = pkt.guid
	local factionID = self:GetFactionId()
	if factionID ~= "" then
		local faction = app.objMgr:getObj(factionID)
		if faction then
			if faction:GetMemberCount() >= faction:GetMemberMaxCount() then
				--帮派人数已满
				self:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_MEMBER_MAX_COUNT)
				return 
			end
			
			if not app.objMgr:IsPlayerGuid(guid) then
				self:CallOptResult(OPERTE_TYPE_SOCIAL,OPERTE_TYPE_SOCIAL_NOT_FIND)
				return
			end
			local player = app.objMgr:getObj(guid)
			
			if player then
				if player:GetFactionId() ~= "" then
					if player:GetFactionId() == self:GetFactionId() then
						self:CallOptResult(OPERTE_TYPE_SOCIAL,OPERTE_TYPE_SOCIAL_SAME_FACTION)
						return
					else
						self:CallOptResult(OPERTE_TYPE_SOCIAL,OPERTE_TYPE_SOCIAL_OTHNER_FACTION)
						return
					end
				end
				
				player:call_show_faction_invite(self:GetGuid(),self:GetName(),faction:GetGuid(),faction:GetName())
			end
			
			
		end
	end
	
end

--[[
--帮派部分
--赠送礼物 (item_table)
--send_faction_gift  --增加未感谢计数 1
function PlayerInfo:Handle_Send_Faction_Gift(pkt)
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
	local data_guid = guidMgr.replace(faction_guid, guidMgr.ObjectTypeFactionData)
	local factionData = app.objMgr:getObj(data_guid)
	if not factionData then
		return
	end
	
	local pos = faction:FindPlayerIndex(self:GetGuid())
	if pos == nil then return end
	
	local list = pkt.list			--道具list
	local item_table = {}
	for _,info in ipairs(list) do
		table.insert(item_table,{info.item_id,info.num})
	end
	local msg = pkt.msg
	local msg_type = pkt.msg_type
	
	faction:SendFactionGift(factionData,self,item_table,msg,msg_type)
	
	
end

--玩家部分
--领取额外奖励 (count_id)
--get_faction_gift_exreward
function PlayerInfo:Handle_Get_Faction_Gift_Exreward(pkt)
	local count_id = pkt.count_id
	self:GetFactionGiftExreward(count_id)
end

--领取所有额外奖励 ()
--get_all_faction_gift_exreward
function PlayerInfo:Handle_Get_All_Faction_Gift_Exreward(pkt)
	self:GetAllFactionGiftExreward()
end


--查询排行页数
function PlayerInfo:Handle_Get_Faction_Gift_Rank_Page(pkt)
	local page = pkt.page
	local result = {}
	if page > 0 then
		local from = page * 10 - 10 + 1
		local to = page * 10
		result = GetFactionRankInfoPage(from,to)
	end
	
	local info = GetFactionRankInfoByGuid(self:GetFactionId())
	
	--发送结果
	local list = {}
	for _,v in ipairs(result) do
	--奖励通知
		local stru = faction_gift_rank_info_t .new()
		stru.rank = v.rank
		stru.point = v.point
		stru.queen_name = v.queen_name
		stru.faction_name = v.faction_name
		stru.guard_name = v.guard_name
		stru.faction_flag = v.faction_flag
		stru.queen_vip = v.queen_vip
		stru.guard_vip = v.guard_vip
		table.insert(list, stru)
	end
	
	local faction_info = faction_gift_rank_info_t .new()
	if info == nil then
		if self:GetFactionId() == "" then
			faction_info.rank = 0
			faction_info.point = 0
			faction_info.queen_name = ""
			faction_info.faction_name = ""
			faction_info.guard_name = ""
			faction_info.faction_flag = 0
			faction_info.queen_vip = 0
			faction_info.guard_vip = 0
		else
			local faction_guid = self:GetFactionId()
			if not app.objMgr:IsFactionGuid(faction_guid) then
				return
			end
			local faction = app.objMgr:getObj(faction_guid)
			if faction == nil then
				return
			end
			faction_info.rank = 0
			faction_info.point = faction:GetFactionCharmPoint()
			faction_info.queen_name = faction:GetBangZhuName()
			faction_info.faction_name = faction:GetName()
			local guard_guid,guard_name,guard_vip = faction:GetWeekGuardInfo()
			faction_info.guard_name = guard_name
			faction_info.faction_flag = faction:GetFactionCurFlagId()
			faction_info.queen_vip = faction:GetFactionMemberVipLevel( faction:FindPlayerIndex(faction:GetBangZhuGuid()))
			faction_info.guard_vip = guard_vip
		end
	else
		faction_info.rank = info.rank
		faction_info.point = info.point
		faction_info.queen_name = info.queen_name
		faction_info.faction_name = info.faction_name
		faction_info.guard_name = info.guard_name
		faction_info.faction_flag = info.faction_flag
		faction_info.queen_vip = info.queen_vip
		faction_info.guard_vip = info.guard_vip
	end
	
	--outFmtDebug("==========before send")
	--outFmtDebug("==========before send len:%d",#list)
	if page == 0 then
		self:call_show_faction_gift_rank_info(faction_info)
	else
		self:call_show_faction_gift_rank_result_list(list,faction_info,page)
	end
	
	
	--outFmtDebug("==========after send")
end
--]]


