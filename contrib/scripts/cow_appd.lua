--先载入一些常量
require("const")
outString('load cow_appd.lua')
outString(__SCRIPTS_ROOT__..'cow_appd.lua')

outString('load share.global_manager')
require('share.global_manager')

outString('load template')
require("template/appd_init")
require("template/conditions")

require("util/utils")
require("util.json")

outString('load share.tick_name script')
require("share/tick_name")

-------------------------------------------------------------------------------
--配置文件信息
config = {
	----背包配置信息
	bag_init_size_main			= 35,		--主包裹初始化大小
	bag_init_size_equip			= 40,		--装备包裹初始化大小
	bag_init_size_storage		= 140,		--仓库初始化大小
	bag_init_size_repurchase	= 20,		--回购包裹初始化大小
	bag_init_size_system		= 42,		--系统包裹初始化大小
	bag_init_size_stall			= 10,		--摊位包裹初始化大小
	
	bag_max_size_main			= 280,		--主包裹最大容量
	bag_max_size_storage		= 280,		--仓库最大容量
	bag_extension_count			= 7	,		--包裹一次扩展的格子数
	bag_extension_material		= 22,		--包裹扩展的消耗材料模版
	max_mail_item_count			= 4	,		--邮件附件最大个数
	
	world_boss_enroll_time = {14, 05}		,--世界BOSS报名时间
	world_boss_enroll_time_last = 1			,--世界BOSS报名持续分钟
	world_boss_time_last = 15				,--世界BOSS持续分钟
	
	world_boss_room_limit = 50				,--世界BOSS房间人数上限

	field_boss_start_time = {{13,0},{21,0}} ,--野外boss开始时间
	field_boss_notice_time = 2				,--野外boss开启时间提醒(m)
	field_boss_born_time = 1				,--野外boss刷新时间提醒(m)

	player_max_level = 100,					--玩家最大等级
	
	player_chat_world_level		= 1,		--玩家世界频道最低等级发言
	player_chat_whisper_level	= 1,		--私聊最低等级限时

	update_firend_info_interval = 20,		--更新玩家好友资料间隔
	update_detection_player_info = 10,		--轮询玩家数据间隔

	faction_create_level 			= 30,		--玩家创建帮派的最低等级要求
	faction_join_level 				= 30,		--玩家加入帮派的最低等级要求
	faction_create_need_item		= 411,		--创建帮派需要的物品ID
	faction_create_max				= 3,		--帮派创建最多个数
	update_faction_player_info		= 20,		--帮派信息更新心跳
	
	laba_use_vip_level = 3,					-- 喇叭使用需要VIP等级
	laba_use_need_money = 10,				-- 喇叭使用需要元宝数
	rank_list_work_interval = 1,			-- 排行榜工作间隔，毫秒
	
	group_join_level				= 20,		--玩家加入队伍的最低等级要求
	
	update_online_playerinfo_interval = 60,	--更新在线玩家信息间隔
	--游戏服命令表
	external_router_map = {
		CMSG_BAG_EXCHANGE_POS,
--		CMSG_BAG_DESTROY,
		--CMSG_BAG_ITEM_SPLIT,
		CMSG_BAG_ITEM_USER,
		CMSG_BAG_ITEM_SELL,
		CMSG_BAG_ITEM_SORT,
		CMSG_FINISH_NOW_GUIDE,
		CMSG_GET_CULTIVATION_INFO,
		CMSG_GET_CULTIVATION_RIVALS_INFO,
		CMSG_GET_CULTIVATION_REWARD,
		CMSG_REFRESH_CULTIVATION_RIVALS,
		CMSG_PLUNDER_CULTIVATION_RIVAL,
		CMSG_REVENGE_CULTIVATION_RIVAL,
		CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT,
		CMSG_GET_LOGIN_ACTIVITY_REWARD,
		CMSG_SMELTING_EQUIP,
		CMSG_SOCIAL_BUY_REVENGE_TIMES,
		CMSG_SOCIAL_REMOVE_ENEMY,
		CMSG_GET_PLAYER_OVERVIEW,
		CMSG_SEND_FACTION_INVITE,
		CMSG_BUY_VIPGIFT,
		CMSG_ACTIVITY_OPT_BUY_DAILYGIFT,
		CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD,
		CMSG_ACTIVITY_OPT_GET_RANK_LIST,
		CMSG_ACTIVITY_OPT_BUY_LIMITGIFT,
		CMSG_WELFARE_GET_RECHARGE_REWARD,
		CMSG_WELFARE_GET_CONSUME_REWARD,
		CMSG_WELFARE_GET_SEVENDAY_REWARD,
		--CMSG_SEND_FACTION_GIFT,
		--CMSG_GET_FACTION_GIFT_EXREWARD,
		--CMSG_GET_ALL_FACTION_GIFT_EXREWARD,
		--CMSG_GET_FACTION_GIFT_RANK_PAGE,
		--CMSG_DIVINE_FORGE,
		--CMSG_DIVINE_ADVANCE,
		--CMSG_DIVINE_SPIRIT,
		CMSG_TALISMAN_ACTIVE,
		CMSG_TALISMAN_LVUP,
		CMSG_WINGS_ACTIVE,
		CMSG_WINGS_BLESS,
		CMSG_WINGS_RANKUP,
		CMSG_WINGS_STRENGTH,
		CMSG_EQUIPDEVELOP_OPERATE,
		CMSG_UNLOCK_TITLE,
		--CMSG_EXCHANGE_ITEM,
		--CMSG_BAG_EXTENSION,
		--CMSG_NPC_REPURCHASE,
		--CMSG_NPC_GET_GOODS_LIST,
		--CMSG_STORE_BUY,
		--CMSG_NPC_SELL,
--		MSG_CHAT_HORN,
		--CMSG_AVATAR_FASHION_ENABLE,
		--CMSG_RECEIVE_GIFT_PACKS,		
		--CMSG_LIMIT_ACTIVITY_RECEIVE,
		CMSG_RANK_LIST_QUERY,
		CMSG_QUERY_PLAYER_INFO,--查询玩家信息
		CMSG_CHAT_WHISPER,--私聊
		--CMSG_CHAR_REMOTESTORE,
		--CMSG_CHAR_REMOTESTORE_STR,
		--CMSG_USE_GOLD_OPT,--使用元宝做些什么*/
		--CMSG_USE_SILVER_OPT,--使用铜钱做些什么*/
		--CMSG_WAREHOUSE_SAVE_MONEY,	--仓库存钱
		--CMSG_WAREHOUSE_TAKE_MONEY,	--仓库取钱
		MSG_SYNC_MSTIME_APP,
		--CMSG_OPEN_WINDOW,
		CMSG_MALL_BUY,
		--CMSG_STRENGTH,
		CMSG_CREATE_FACTION,
--		CMSG_FACTION_UPGRADE,
		CMSG_FACTION_JOIN,
		CMSG_RAISE_BASE_SPELL,
		CMSG_UPGRADE_ANGER_SPELL,
		CMSG_RAISE_MOUNT,
		CMSG_UPGRADE_MOUNT,
		CMSG_UPGRADE_MOUNT_ONE_STEP,
		CMSG_ILLUSION_MOUNT_ACTIVE,
		CMSG_ILLUSION_MOUNT,
		--CMSG_GEM,
		--[[CMSG_DIVINE_ACTIVE,
		CMSG_DIVINE_UPLEV,
		CMSG_DIVINE_SWITCH,--]]
		CMSG_SWEEP_VIP_INSTANCE,
		CMSG_HANG_UP,				-- /*进行挂机*/	
		CMSG_HANG_UP_SETTING,		-- /*进行挂机设置*/
		CMSG_SWEEP_TRIAL_INSTANCE,
		CMSG_RESET_TRIAL_INSTANCE,
		CMSG_SOCIAL_ADD_FRIEND,
		CMSG_SOCIAL_SUREADD_FRIEND,
		CMSG_SOCIAL_GIFT_FRIEND,
		CMSG_SOCIAL_RECOMMEND_FRIEND,
		CMSG_SOCIAL_REVENGE_ENEMY,
		CMSG_SOCIAL_DEL_FRIEND,
		CMSG_SOCIAL_CLEAR_APPLY,
		CMSG_SOCIAL_ADD_FRIEND_BYNAME,
		CMSG_CHAT_BY_CHANNEL,
		CMSG_MSG_DECLINE,
		CMSG_BLOCK_CHAT, 	--屏蔽某人
		CMSG_FACTION_GETLIST,
		CMSG_FACTION_MANAGER,
		CMSG_FACTION_MEMBER_OPERATE,
		CMSG_FACTION_FAST_JOIN,
		-----------------------------
		CMSG_READ_MAIL,
		CMSG_PICK_MAIL,
		--CMSG_REMOVE_MAIL,
		CMSG_PICK_MAIL_ONE_STEP,
		CMSG_REMOVE_MAIL_ONE_STEP,
		CMSG_CANCEL_BLOCK_CHAT,
		-------------------------------
		CMSG_WORLD_BOSS_ENROLL,	-- /*世界BOSS报名*/	
		-----------------------------
		CMSG_RANK_ADD_LIKE,
		-------------------------------
		CMSG_RES_INSTANCE_SWEEP,
		--------------------------
		CMSG_GET_ACTIVITY_REWARD,
		--------------------------
		CMSG_GET_ACHIEVE_REWARD,
		CMSG_GET_ACHIEVE_ALL_REWARD,
		CMSG_SET_TITLE,
		CMSG_INIT_TITLE,
		CMSG_WELFARE_SHOUCHONG_REWARD,
		CMSG_WELFARE_CHECKIN,
		CMSG_WELFARE_CHECKIN_ALL,
		CMSG_WELFARE_CHECKIN_GETBACK,
		CMSG_WELFARE_LEVEL,
		CMSG_WELFARE_ACTIVE_GETBACK,
		CMSG_WELFARE_GETALLLIST_GETBACK,
		CMSG_WELFARE_GETALL_GETBACK,
		CMSG_PICK_QUEST_REWARD,
		CMSG_USE_VIRTUAL_ITEM,
		CMSG_PICK_QUEST_CHAPTER_REWARD,
		--[[CMSG_KUAFU_3V3_MATCH,
		CMSG_KUAFU_3V3_MATCH_OPER,
		CMSG_KUAFU_3V3_BUYTIMES,
		CMSG_KUAFU_3V3_DAYREWARD,
		CMSG_KUAFU_3V3_GETRANLIST,
		CMSG_KUAFU_3V3_GETMYRANK,
		--]]
		MSG_KUAFU_3V3_CANCEL_MATCH,
		CMSG_KUAFU_XIANFU_MATCH,
		CMSG_BUY_XIANFU_ITEM,
		CMSG_DOUJIANTAI_FIGHT,
		CMSG_DOUJIANTAI_BUYTIME,
		CMSG_DOUJIANTAI_CLEARCD,
		CMSG_DOUJIANTAI_FIRST_REWARD,
		
		MSG_DOUJIANTAI_GET_ENEMYS_INFO,
		CMSG_DOUJIANTAI_GET_RANK,
		CMSG_DOUJIANTAI_REFRESH_ENEMYS,
		
		MSG_DOUJIANTAI_TOP3,
		
		CMSG_SUBMIT_QUEST_DAILY2,
		CMSG_PICK_DAILY2_QUEST_REWARD,
		
		CMSG_FINISH_OPTIONAL_GUIDE_STEP,
		CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED,
		
		CMSG_STOREHOUSE_HAND_IN,	--/*上交装备*/
		CMSG_STOREHOUSE_EXCHANGE,	--/*兑换装备*/
		CMSG_STOREHOUSE_DESTROY,	--/*销毁装备*/
		
		CMSG_BUY_MASS_BOSS_TIMES,
		CMSG_GROUP_INSTANCE_MATCH,
		CMSG_BUY_GROUP_INSTANCE_TIMES,
		
		CMSG_MERIDIAN_PRACTISE,		-- /*经脉修炼*/	
		CMSG_ADD_MERIDIAN_EXP,		-- /*加经脉修炼经验值*/
		CMSG_RAISE_MOUNT_LEVEL_BASE,	-- 升级坐骑等级
		CMSG_ACTIVE_MOUNT,				-- 激活坐骑
		CMSG_MATCH_SINGLE_PVP,
		CMSG_BUY_MATCH_SINGLE_PVP_TIMES, -- /*购买单人pvp次数*/	
		CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD, -- /*领取单人pvp额外奖励*/
		
		CMSG_ACTIVE_APPEARANCE,		-- /*激活外观*/	
		CMSG_EQUIP_APPEARANCE,		-- /*装备外观*/	
		CMSG_CANCEL_EQUIP_APPEARANCE,
		CMSG_DRAW_LOTTERY,			-- 抽奖
		CMSG_RENAME,				--改名
		CMSG_RISK_GET_RANK,
	},
	--pk服命令表
	pk_external_router_map = {
--		MSG_CHAT_WORLD,			
	},
}

--校验协议注册是否有效
CheckRouterMap(config.external_router_map)
CheckRouterMap(config.pk_external_router_map)

require('appd.__init')
app = require('appd.appd_app'):new()

--返回开服后几天的时间戳
function DoHowLongKaiFuTime(val)
	local time_table = globalGameConfig:GetKaiFuShiJian()
	local time_diff = time_table%86400
	time_table = time_table - time_diff - TIME_ZONE_HOUR*3600
	local last_time = time_table + 86400*val - 1
	return last_time
end

--获取开服后第几天
function DoKaiFuDay()
	local days = math.floor((os.time() - globalGameConfig:GetKaiFuShiJian()) / (24*3600)) + 1
	return days
end

--合服后，应用服需要做的事情
function AppdMergeSomething()
	--TODO:由于合了一服就会执行一次,所以通过定时器延迟触发节约性能
	--推送功能必须重新注册一下
	app.http.register_state = 0
	
	-- 解散军团了
	app.objMgr:foreachAllFaction(function(faction)
		faction:AllFactionPlayerQuit()
		local faction_events_guid = faction:getFactionEventsGuid()
		app.objMgr:callRemoveObject(faction:GetGuid())
		app.objMgr:callRemoveObject(faction_events_guid)
	end)
end

--服务器重启后需要loadDB的数据
function StartServerLoadDB()
	
	-- app.dbAccess = {}
	-- --如果有子类接口
	-- if app.__clsDbAccess then
		-- app.__clsDbAccess.extend(app.dbAccess, {})
	-- end
	--//初始化限时活动
	InitLimitActivityConfig()
	GetTodayLimitActivityVersion()
	
	if(not app.dbAccess )then
		app.dbAccess = {}
	end
	app.dbAccess.server_name = globalGameConfig:GetOriginServerName()
	app.dbAccess.trade_data = {}
	app.dbAccess.trade_buy_data = {}
	app.dbAccess:loadTempStallItemData()
	app.dbAccess:loadTempTradeBuyItemData()
end

--服务器重启后需要loadDB的数据
function CloseServerSaveDB()
	--武将排行榜
	--WJForceRank:SaveDB()
	--武将装备排行榜
	--WJItemRank:SaveDB()
end

-- 存储机器人私聊信息
SYS_WHISPER_INFO = {}
function DoAddRobotSysWhisper(player, sys_type)	
	if player:GetLevel() < 40 then		-- 玩家等级小于40级，则不会收到系统私聊信息
		return
	end
	local cur_tm = os.time() + 3
	if SYS_WHISPER_INFO[cur_tm] == nil then
		SYS_WHISPER_INFO[cur_tm] = {}
	end
	table.insert(SYS_WHISPER_INFO[cur_tm], {player:GetGuid(), sys_type})
end

-- 设置挖宝时间
function DoSetWaBaoTime()
	-- 给全服在线玩家一张藏宝图
	--遍历所有玩家
	app.objMgr:foreachAllPlayer(function(player)
		local num = player:GetPlayerWaBaoNum()
		if(num < 5)then
			player:SetPlayerWaBaoNum(num + 1)
		end
	end)
	--发送公告
	onSendNotice(18)	
end
--[[
local appd_faction_rank = {}
--{queen_guid queen_name faction_guid faction_name guard_guid guard_name point icon}
--机器人 guid = ""

--update(faction) 赠送时更新point  调整名次  名次上升的帮派进行帮派广播 更新

local max_faction_rank = 2000

function UpdateFactionRank(point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name,queen_vip,guard_vip)
	if #appd_faction_rank == max_faction_rank and appd_faction_rank[max_faction_rank].point > point then
		--未上榜 不广播
		-- 0, 0
		return 0,0
	end
	
	if #appd_faction_rank == 0 then
		local prev_index = InsertFactionRank(1,point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name,queen_vip,guard_vip)
		return 0,1

	end
	
	if #appd_faction_rank < max_faction_rank and appd_faction_rank[#appd_faction_rank].point > point then
		local index = #appd_faction_rank + 1
		local prev_index = InsertFactionRank(index,point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name,queen_vip,guard_vip)
		return 0, index
	end
	
	if #appd_faction_rank < max_faction_rank and appd_faction_rank[#appd_faction_rank].point == point and (time == 0 or time >= appd_faction_rank[#appd_faction_rank].time) then
		local index = #appd_faction_rank + 1
		local prev_index = InsertFactionRank(index,point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name,queen_vip,guard_vip)
		return 0, index
	end
	
	for index = 1,#appd_faction_rank do
		if point > appd_faction_rank[index].point then
			--插入数据
			local prev_index = InsertFactionRank(index,point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name,queen_vip,guard_vip)
			
			if prev_index == 0 then
				--进入排名 广播
				return 0, index
			else
				--排名上升 广播
				return prev_index,index
			end
			break
		elseif point == appd_faction_rank[index].point and time ~= 0 and time < appd_faction_rank[index].time then
			--插入数据
			local prev_index = InsertFactionRank(index,point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name,queen_vip,guard_vip)
			
			if prev_index == 0 then
				--进入排名 广播
				return 0, index
			else
				--排名上升 广播
				return prev_index,index
			end
			break
		end
	end
	
	
	
	return 0,0
end

function InsertFactionRank(index,point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name,queen_vip,guard_vip)
	local info = {}
	info.point = point
	info.queen_guid = queen_guid
	info.queen_name = queen_name
	info.faction_guid = faction_guid
	info.faction_name = faction_name
	info.faction_icon = faction_icon
	info.time = time
	info.guard_guid = guard_guid
	info.guard_name = guard_name
	info.queen_vip = queen_vip
	info.guard_vip = guard_vip
	
	local temp = {}
	while index <= max_faction_rank do
		if appd_faction_rank[index] == nil then
			appd_faction_rank[index] = info
			return 0
		end
		
		if appd_faction_rank[index].faction_guid ~= "" and appd_faction_rank[index].faction_guid == faction_guid then
			appd_faction_rank[index] = info
			return index
		end
		
		temp = appd_faction_rank[index]
		appd_faction_rank[index] = info
		info = temp
		index = index + 1
	end
	return 0
end

--remove(info) 家族解散时 移除记录
function RemoveFactionRank(faction_guid)
	for index = 1,#appd_faction_rank do
		if appd_faction_rank[index].faction_guid ~= "" and appd_faction_rank[index].faction_guid == faction_guid then
			while index <= max_faction_rank do
				if appd_faction_rank[index+1] == nil then
					appd_faction_rank[index] = nil
					break
				end
				appd_faction_rank[index] = appd_faction_rank[index+1]
				index = index + 1
			end
			break
		end
	end
end

--getranklist 取得排名范围内的数据
function GetFactionRankInfoPage(from, to)
	local result = {}
	for index = from,to do
		if appd_faction_rank[index] ~= nil then
			local info = appd_faction_rank[index]
			info.rank = index
			table.insert(result,info)
		end
	end
	
	return result
end

function GetFactionRankInfoByGuid(guid)
	if guid == "" then
		return nil
	end
	for index = 1,#appd_faction_rank do
		if appd_faction_rank[index].faction_guid == guid then
			local info = appd_faction_rank[index]
			info.rank = index
			return info
		end
	end
	return nil
end

--家族魅力排行 每轮奖励 记录第一名信息
function SendFactionGiftRankReward()
	local cur_round = globalValue:GetFactionGiftRankCurRound()
	if os.time() < globalValue:GetFactionGiftRankNextUpdateTime() or cur_round > tb_faction_gift_rank_base[1].max_round then
		return
	end
	
	
	--奖励发放
	for index = cur_round * 100 + 1,cur_round * 100 + 100 do
		local v = tb_faction_gift_rank_reward[index]
		if v == nil then
			break
		end
		local mailItem = {}
		for i = indx, #v.reward do
			table.insert(mailItem, v.reward[ i ][ 1 ])
			table.insert(mailItem, v.reward[ i ][ 2 ])
		end
		local itemConfig = string.join(",", mailItem)
		for i = v.rank[1],v.rank[2] do
			if appd_faction_rank[i] ~= nil and appd_faction_rank[i].queen_guid ~= "" then
				AddGiftPacksData(appd_faction_rank[i].queen_guid,0,GIFT_PACKS_TYPE_FACTION_GIFT_RANK,os.time(),os.time() + 86400*30, v.mail_name, v.mail_desc, itemConfig, SYSTEM_NAME)
			end
		end
	end
	
	if appd_faction_rank[1] ~= nil then
		globalValue:SetGiftRankWinerFactionFlag(appd_faction_rank[1].icon)
		globalValue:SetGiftRankWinerQueenName(appd_faction_rank[1].queen_name)
		globalValue:SetGiftRankWinerFactionName(appd_faction_rank[1].faction_name)
		globalValue:SetGiftRankWinerGuardName(appd_faction_rank[1].guard_name)
	end
	
	globalValue:UpdateFactionGiftRank()
end

function printAllFactionRank()
	outFmtInfo("======================printAllFactionRank")
	for index = 1,#appd_faction_rank do
		outFmtInfo("name:%s guid:%s point:%d",appd_faction_rank[index].queen_name,appd_faction_rank[index].queen_guid,appd_faction_rank[index].point)
	end
end

function loadFactionInfo()
	app.objMgr:foreachAllFaction(function(faction)
		local point = faction:GetFactionCharmPoint()
		local queen_guid = faction:GetBangZhuGuid()
		local queen_name = faction:GetBangZhuName()
		local faction_guid = faction:GetGuid()
		local faction_name = faction:GetName()
		local faction_icon = faction:GetFactionCurFlagId()
		local time = faction:GetFactionGiftLastSendTime()
		local guard_guid = ""
		local guard_name = ""
		local guard_vip = 0
		guard_guid,guard_name,guard_vip = faction:GetWeekGuardInfo()
		local queen_vip = faction:GetFactionMemberVipLevel( faction:FindPlayerIndex(faction:GetBangZhuGuid()))
		if point > 0 then
			UpdateFactionRank(point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name)
		end
	end)
	
	for index = 1,#tb_faction_gift_robot_rank do
		local point = tb_faction_gift_robot_rank[index].point
		local queen_guid = ""
		local queen_name = tb_faction_gift_robot_rank[index].queen_name
		local faction_guid = ""
		local faction_name = tb_faction_gift_robot_rank[index].queen_name
		local faction_icon = tb_faction_gift_robot_rank[index].icon
		local time = 0
		local guard_guid = ""
		local guard_name = tb_faction_gift_robot_rank[index].guard_name
		local queen_vip = 0
		local guard_vip = 0
		
		UpdateFactionRank(point,queen_guid,queen_name,faction_guid,faction_name,faction_icon,time,guard_guid,guard_name,queen_vip,guard_vip)
	end
	
	--printAllFactionRank()
end

loadFactionInfo()
--]]

globalCounter:InitDoujiantaiRank()
globalCounter:activityInit()
--globalValue:UpdateFactionGiftRank()