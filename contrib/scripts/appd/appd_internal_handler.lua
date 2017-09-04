require("appd.appd_internal_pack")
local Packet = require 'util.packet'

--场景服发送使用物品结果
local function on_scened_use_item_result( pkt )
	local ret, player_guid, item_entry, result, count = unpack_user_item_result(pkt)
	if not ret then
		return
	end
	
	ScenedUseItemResult(player_guid, item_entry, count, result)
end

--场景服玩家拾取战利品
local function on_scened_loot_select( pkt )
	local ret, player_guid, item_entry, bind, count, failtime = unpack_loot_select(pkt)
	if not ret then
		return
	end
	--模板里没有的道具
	if not tb_item_template[item_entry] then 
		outFmtDebug("loot_select: player %s not find %d in template!", player_guid, item_entry)
		return 
	end	
	
	local player = app.objMgr:getObj(player_guid)
	if not player then
		outFmtDebug("loot_select:player %s not online item_entry %d count %d", player_guid, item_entry, count)
		return
	end
	
	player:AppdAddItems({{item_entry, count}}, nil, LOG_ITEM_OPER_TYPE_LOOT)
end

--场景服做任务给物品
local function on_scened_quest_add_item( pkt )
	local ret, player_guid, item_entry, count, bind, stronglv, failtime, logtype = unpack_quest_add_item(pkt)
	if not ret then
		return
	end
	
	local player = app.objMgr:getObj(player_guid)
	if not player then
		outFmtDebug("quest_add_item:player %s not online item_entry %d count %d", player_guid, item_entry, count)
		return
	end
	-- 从场景服过来的必须用这个
	player:PlayerAddItems({{item_entry, count}}, nil, logtype)
	--player:AddItemByEntry(item_entry, count, nil, logtype, bind, true, true, stronglv, failtime)
end

local function on_scened_add_items(pkt)
	local ret, player_guid, itemDict, logtype, bagFullCategory = unpack_add_items(pkt)
	if not ret then
		return
	end
	
	local player = app.objMgr:getObj(player_guid)
	if not player then
		outFmtDebug("on_scened_quest_add_item:player %s not online", player_guid)
		return
	end
	-- 场景服发送过来的走这里
	player:AppdAddItems(itemDict, nil, item_oper_type, nil, nil, nil, bagFullCategory)
	-- player:AppdAddItems(itemDict, nil, logtype, bagFullCategory)
end

--场景服通知应用服玩家升级了
local function on_scened_player_upgrade( pkt )
	local ret, player_guid, prevLevel, player_lv = unpack_player_upgrade(pkt)
	if not ret then
		return
	end
	
	local player = app.objMgr:getObj(player_guid)
	if not player then
		outFmtDebug("on_scened_player_upgrade:player %s not online player_lv %d", player_guid, player_lv)
		return
	end
	
	-- 为了重算属性用的
	player:SetLevel(player_lv)
	-- 通知重算属性
	playerLib.SendAttr(player.ptr)
	
	-- 处理升级以后的逻辑
	local gender = player:GetGender()
	local config = tb_char_skill[gender]
	for _, info in pairs(config.unlocks) do
		if prevLevel < info[ 1 ] and info[ 1 ] <= player_lv then
			player:activeBaseSpell(info[ 2 ], SPELL_ACTIVE_BY_LEVEL)
		end
	end
	
	--[[
	-- FIXME:解锁坐骑 先在这里处理
	if prevLevel < 8 and 8 <= player_lv then
		player:activeMount()
	end
	--]]
	-- 到C++中改变等级列表
	playerLevelChanged(player_guid, prevLevel, player_lv)
	
	-- 等级解锁任务
	for level = prevLevel + 1, player_lv do
		player:AddLevelActiveQuest(level)
	end
	
	player:UnlockModuleByLevel(prevLevel, player_lv)
	--[[
	-- 如技能解锁
	local socialSysInfo = player:getSocialSystem()
	if socialSysInfo then
		socialSysInfo:ProceedUpgrade(player, FRIEND_OPT_PLAYER_UPGRADE, player_lv)
	end
	]]
end

local function on_scened_add_gift_packs( pkt )
	local ret, player_guid, id, gift_type, start_time, end_time, gift_name, gift_desc, item_config, item_from = unpack_add_gift_packs(pkt)
	if not ret then
		return
	end
	
	AddGiftPacksData(player_guid,id,gift_type,start_time,end_time,gift_name,gift_desc,item_config,item_from)
end

local function on_scened_add_number_material( pkt )
	local ret, player_guid, ntype, value = unpack_add_number_material(pkt)
	if not ret then
		return
	end
	
	local player = app.objMgr:getObj(player_guid)
	if not player then
		outFmtDebug("on_scened_add_number_material:player %s not online type %d value %d", player_guid, ntype, value)
		return
	end
	player:AddNumberMaterial( ntype, value)
end

local function on_player_killed( pkt )
	local guid, kill_id, silver = unpack_player_killed(pkt)
	if not ret then return end
	if guid == kill_id then
		return
	end
	local player = app.objMgr:getObj(guid)
	if not player then 
		outFmtDebug("AppdApp::on_player_killed: player = null, %s", guid)
		return 
	end
	local killer = app.objMgr:getObj(kill_id)
	if not killer then 
		outFmtDebug("AppdApp::on_player_killed: killer = null, %s", kill_id)
		return 
	end
	player:DoKillPlayer(killer)
end

--发送给应用服应该做些什么事
local function on_scened_send_to_appd_do_something( pkt )
	local ret, guid, type, data, str = unpack_send_to_appd_do_something(pkt)
	if not ret then return end
	local player = app.objMgr:getObj(guid)
	if not player then 
		outFmtDebug("AppdApp::on_scened_send_to_appd_do_something: player = null, %s", guid)
		return 
	end
	player:DoGetScenedDoSomething(type,data,str)
end

--场景服发给应用服发公告
local function on_scened_send_ontice( pkt )
	local ret, id, content, data = unpack_scened_send_notice(pkt)
	if not ret then return end	
	app:SendNotice(id, content, data)
end

--场景服发送聊天信息
local function on_scened_send_char_info( pkt )
	local ret, c_type, guid, content, to_guid, to_name = unpack_scened_send_char_info(pkt)
	if not ret then return end
	local player = app.objMgr:getObj(guid)
	if not player then 
		outFmtDebug("AppdApp::on_scened_send_char_info: player = null, %s", guid)
		return 
	end
	player:SendChat(c_type, content, to_guid, to_name)
end

local function on_scened_send_faction_challenge_boss_win( pkt )
	local ret,faction_guid = unpack_scened_send_faction_challenge_boss_win(pkt)
	if not ret then return end
	local faction = app.objMgr:getObj(faction_guid)
	if not faction then 
		outFmtDebug("AppdApp::on_scened_send_faction_challenge_boss_win: faction = null, %s", faction_guid)
		return 
	end
	faction:ChallengeBossWin()
end

local function on_scened_send_faction_challenge_boss_fail( pkt )
	local ret, faction_guid, fail_type= unpack_scened_send_faction_challenge_boss_fail(pkt)
	if not ret then return end
	local faction = app.objMgr:getObj(faction_guid)
	if not faction then 
		outFmtDebug("AppdApp::on_scened_send_faction_challenge_boss_fail: faction = null, %s", faction_guid)
		return 
	end
	faction:ChallengedBossFail(fail_type)
end

local function on_scened_send_faction_add_points( pkt )
	local ret,faction_guid, player_guid, points = unpack_scened_send_faction_add_points(pkt)
	if not ret then return end
	local faction = app.objMgr:getObj(faction_guid)
	if not faction then 
		outFmtDebug("AppdApp::on_scened_send_faction_add_points: faction = null, %s", faction_guid)
		return 
	end
	faction:AddTokenPoints(player_guid, points)
end

local function on_scened_send_faction_challenge_boss_damaged( pkt )
	local ret, faction_guid, player_guid, damage = unpack_scened_send_faction_challenge_boss_damaged(pkt)
	if not ret then return end
	local faction = app.objMgr:getObj(faction_guid)
	if not faction then 
		outFmtDebug("AppdApp::on_scened_send_faction_challenge_boss_damaged: faction = null, %s", faction_guid)
		return 
	end
	faction:BossDamaged(player_guid, damage)
end

local function on_scened_send_faction_update_boss_info( pkt )
	local ret, faction_guid, hp_rate, x, y = unpack_scened_send_faction_update_boss_info(pkt)
	if not ret then return end
	local faction = app.objMgr:getObj(faction_guid)
	if not faction then 
		outFmtDebug("AppdApp::on_scened_send_faction_update_boss_info: faction = null, %s", faction_guid)
		return 
	end
	faction:UpdateBossInfo(hp_rate, x, y)
end

local function on_scened_send_faction_update_target_info( pkt )
	local ret, faction_guid, hp_rate, x, y = unpack_scened_send_faction_update_target_info(pkt)
	if not ret then return end
	local faction = app.objMgr:getObj(faction_guid)
	if not faction then 
		outFmtDebug("AppdApp::on_scened_send_faction_update_target_info: faction = null, %s", faction_guid)
		return 
	end
	faction:UpdateTargetInfo(hp_rate, x, y)
end

local function on_scened_send_faction_bossdefense_win( pkt )
	local ret, faction_guid,player_guid, pool_id = unpack_scened_send_faction_bossdefense_win(pkt)
	if not ret then return end
	local faction = app.objMgr:getObj(faction_guid)
	if not faction then 
		outFmtDebug("AppdApp::on_scened_send_faction_update_target_info: faction = null, %s", faction_guid)
		return 
	end
	faction:OnBossDenfenseChallengeKill(player_guid, pool_id)
end

local function on_scened_send_faction_bossdefense_leave( pkt )
	local ret, faction_guid,player_guid, index, pool_id, hp = unpack_scened_send_faction_bossdefense_leave(pkt)
	if not ret then return end
	local faction = app.objMgr:getObj(faction_guid)
	if not faction then 
		outFmtDebug("AppdApp::on_scened_send_faction_update_target_info: faction = null, %s", faction_guid)
		return 
	end
	faction:OnBossDenfenseChallengeFinish(player_guid,index,pool_id,hp)
end

local function on_logind_send_rename_check_result(pkt)
	local ret, player_guid, available, realName = unpack_logind_send_rename_check_result(pkt)
	if not ret then return end
	
	local playerInfo = app.objMgr:getObj(player_guid)
	if not playerInfo then
		return
	end
	
	if available == 1 then
		-- 判断扣钱逻辑
		local costs = playerInfo:GetRenameCost()
		if playerInfo:useAllItems(MONEY_CHANGE_RENAME, costs) then
			playerInfo:AddRenameTimes()
			--playerInfo:SetName(realName)
			call_opt_update_char_name(player_guid, realName)
			playerInfo:UpdateFactionBangZhuInfo()
			playerInfo:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_RENAME_SUCCESS)
		end
	end
end

local appdInsternalHanlders = {}
appdInsternalHanlders[INTERNAL_OPT_USER_ITEM_RESULT] = on_scened_use_item_result
appdInsternalHanlders[INTERNAL_OPT_LOOT_SELECT] = on_scened_loot_select
appdInsternalHanlders[INTERNAL_OPT_QUEST_ADD_REW_ITEM] = on_scened_quest_add_item
appdInsternalHanlders[INTERNAL_OPT_ADD_ITEMS] = on_scened_add_items

appdInsternalHanlders[INTERNAL_OPT_UPGRADE_LEVEL] = on_scened_player_upgrade
appdInsternalHanlders[INTERNAL_OPT_ADD_GIFT_PACKS] = on_scened_add_gift_packs
appdInsternalHanlders[INTERNAL_OPT_APPD_ADD_NUMBER_MATERIAL] = on_scened_add_number_material
appdInsternalHanlders[INTERNAL_OPT_USER_KILLED] = on_player_killed
appdInsternalHanlders[INTERNAL_OPT_SEND_TO_APPD_DO_SOMETHING] = on_scened_send_to_appd_do_something
appdInsternalHanlders[INTERNAL_OPT_NOTICE] = on_scened_send_ontice
appdInsternalHanlders[INTERNAL_OPT_CHAT] = on_scened_send_char_info

appdInsternalHanlders[INTERNAL_OPT_FACTION_BOSS_WIN] = on_scened_send_faction_challenge_boss_win
appdInsternalHanlders[INTERNAL_OPT_FACTION_BOSS_FAIL] = on_scened_send_faction_challenge_boss_fail
appdInsternalHanlders[INTERNAL_OPT_FACTION_ADD_TOKEN_POINTS] = on_scened_send_faction_add_points
appdInsternalHanlders[INTERNAL_OPT_FACTION_BOSS_DAMAGED] = on_scened_send_faction_challenge_boss_damaged
appdInsternalHanlders[INTERNAL_OPT_FACTION_UPDATE_BOSS_INFO] = on_scened_send_faction_update_boss_info
appdInsternalHanlders[INTERNAL_OPT_FACTION_UPDATE_TARGET_INFO] = on_scened_send_faction_update_target_info

appdInsternalHanlders[INTERNAL_OPT_FACTION_BOSSDEFENSE_WIN] = on_scened_send_faction_bossdefense_win
appdInsternalHanlders[INTERNAL_OPT_FACTION_BOSSDEFENSE_LEAVE] = on_scened_send_faction_bossdefense_leave
appdInsternalHanlders[INTERNAL_OPT_RENAME_CHECK_RESULT] = on_logind_send_rename_check_result


--网络包处理方法
packet.register_on_internal_packet(function ( cid, pkt )
	local optcode = packet.optcode(pkt)
	local f = appdInsternalHanlders[optcode]
	if f then
		doxpcall(f, Packet.new(nil, pkt))
--		pkt:delete()
	else
		--print(cid)
	end
end)
