local Packet = require 'util.packet'
--应用服内部协议解包函数封装
function unpack_user_item_result( pkt )
	local ret, player_guid, item_entry, result, count
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, item_entry = pkt:readU32()
	if not ret then return false end
	ret, result = pkt:readByte()
	if not ret then return false end
	ret, count = pkt:readU32()
	if not ret then return false end
	return true, player_guid, item_entry, result, count
end

function unpack_loot_select( pkt )
	local ret, player_guid, item_entry, bind, count, failtime
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, item_entry = pkt:readU32()
	if not ret then return false end
	ret, bind = pkt:readU32()
	if not ret then return false end
	ret, count = pkt:readU32()
	if not ret then return false end
	ret, failtime = pkt:readU32()
	if not ret then return false end
	return true, player_guid, item_entry, bind, count, failtime
end

function unpack_quest_add_item( pkt )
	local ret, player_guid, item_entry, count, bind, stronglv, failtime, logtype
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, item_entry = pkt:readU32()
	if not ret then return false end
	ret, count = pkt:readU32()
	if not ret then return false end
	ret, bind = pkt:readByte()
	if not ret then return false end	
	ret, stronglv = pkt:readU32()
	if not ret then return false end
	ret, failtime = pkt:readU32()
	if not ret then return false end
	ret, logtype = pkt:readByte()
	if not ret then return false end
	
	return true, player_guid, item_entry, count, bind, stronglv, failtime, logtype
end

function unpack_add_items( pkt )
	local ret, player_guid, len, logtype, bagFullCategory
	local itemDict = {}
	
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	
	ret, len = pkt:readU16()
	if not ret then return false end
	for i = 1, len do
		ret, entry = pkt:readU32()
		if not ret then return false end
		
		ret, count = pkt:readU32()
		if not ret then return false end
		table.insert(itemDict, {entry, count})
	end

	ret, logtype = pkt:readByte()
	if not ret then return false end
	ret, bagFullCategory = pkt:readByte()
	
	return true, player_guid, itemDict, logtype, bagFullCategory
end

function unpack_player_upgrade( pkt )
	local ret, player_guid, prevLevel, player_lv
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, prevLevel = pkt:readU32()
	if not ret then return false end
	ret, player_lv = pkt:readU32()
	if not ret then return false end

	return true, player_guid, prevLevel, player_lv
end

function unpack_add_gift_packs( pkt )
	local ret, player_guid, id, gift_type, start_time, end_time, gift_name, gift_desc, item_config, item_from
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, id = pkt:readI32()
	if not ret then return false end
	ret, gift_type = pkt:readI32()
	if not ret then return false end
	ret, start_time = pkt:readU32()
	if not ret then return false end
	ret, end_time = pkt:readU32()
	if not ret then return false end
	ret, gift_name = pkt:readUTF()
	if not ret then return false end
	ret, gift_desc = pkt:readUTF()
	if not ret then return false end
	ret, item_config = pkt:readUTF()
	if not ret then return false end
	ret, item_from = pkt:readUTF()
	if not ret then return false end

	return true, player_guid, id, gift_type, start_time, end_time, gift_name, gift_desc, item_config, item_from
end

function unpack_add_number_material( pkt )
	local ret, player_guid, ntype, value
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, ntype = pkt:readU32()
	if not ret then return false end
	ret, value = pkt:readU32()
	if not ret then return false end

	return true, player_guid, ntype, value
end

--场景服通知玩家被杀死，需要处理社交系统
--返回被杀者，杀手，及铜钱数量
function unpack_player_killed( pkt )
	local ret, guid, killer_id, silver
	ret, guid = pkt:readUTF()
	if not ret then return false end
	ret, killer_id = pkt:readUTF()
	if not ret then return false end
	ret, silver = pkt:readU32()
	if not ret then return false end
	return true, guid, killer_id, silver
end

function unpack_send_to_appd_do_something( pkt )
	local ret, player_guid, ntype, data, str
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, ntype = pkt:readU32()
	if not ret then return false end
	ret, data = pkt:readU32()
	if not ret then return false end
	ret, str = pkt:readUTF()
	if not ret then return false end
	
	return true, player_guid, ntype, data,	str
end

function unpack_scened_send_notice( pkt )
	local ret, id, content, data
	ret, id = pkt:readU32()
	if not ret then return false end
	ret, content = pkt:readUTF()
	if not ret then return false end
	ret, data = pkt:readUTF()
	if not ret then return false end
	return true, id, content, data
end

--场景服发送聊天信息
function unpack_scened_send_char_info( pkt )
	local ret, c_type, guid, content, to_guid, to_name
	ret, c_type = pkt:readByte()
	if not ret then return false end
	ret, guid = pkt:readUTF()
	if not ret then return false end
	ret, content = pkt:readUTF()
	if not ret then return false end
	ret, to_guid = pkt:readUTF()
	if not ret then return false end
	ret, to_name = pkt:readUTF()
	if not ret then return false end
	return true, c_type, guid, content, to_guid, to_name
end


function unpack_scened_send_faction_challenge_boss_win( pkt )
	local ret, faction_guid
	ret, faction_guid = pkt:readUTF()
	if not ret then return false end
	return true, faction_guid
end

function unpack_scened_send_faction_challenge_boss_fail( pkt )
	local ret, faction_guid, fail_type
	ret, faction_guid = pkt:readUTF()
	if not ret then return false end
	ret, fail_type = pkt:readU32()
	if not ret then return false end
	return true, faction_guid, fail_type
end

function unpack_scened_send_faction_add_points( pkt )
	local ret,faction_guid, player_guid, points
	ret, faction_guid = pkt:readUTF()
	if not ret then return false end
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, points = pkt:readU32()
	if not ret then return false end
	
	return true, faction_guid, player_guid, points
end

function unpack_scened_send_faction_challenge_boss_damaged( pkt )
	local ret, faction_guid, player_guid, damage
	ret, faction_guid = pkt:readUTF()
	if not ret then return false end
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, damage = pkt:readU32()
	if not ret then return false end
	
	return true, faction_guid, player_guid, damage
end

function unpack_scened_send_faction_update_boss_info( pkt )
	local ret, faction_guid, hp_rate, x, y
	ret, faction_guid = pkt:readUTF()
	if not ret then return false end
	ret, hp_rate = pkt:readU32()
	if not ret then return false end
	ret, x = pkt:readU32()
	if not ret then return false end
	ret, y = pkt:readU32()
	if not ret then return false end
	return true, faction_guid, hp_rate, x, y
end

function unpack_scened_send_faction_update_target_info( pkt )
	local ret, faction_guid, hp_rate, x, y
	ret, faction_guid = pkt:readUTF()
	if not ret then return false end
	ret, hp_rate = pkt:readU32()
	if not ret then return false end
	ret, x = pkt:readU32()
	if not ret then return false end
	ret, y = pkt:readU32()
	if not ret then return false end
	return true, faction_guid, hp_rate, x, y
end

function unpack_scened_send_faction_bossdefense_win( pkt )
	local ret, faction_guid,player_guid, pool_id
	ret, faction_guid = pkt:readUTF()
	if not ret then return false end
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, pool_id = pkt:readU32()
	if not ret then return false end
	return true, faction_guid, player_guid, pool_id
end

function unpack_scened_send_faction_bossdefense_leave( pkt )
	local ret, faction_guid,player_guid, index, pool_id, hp
	ret, faction_guid = pkt:readUTF()
	if not ret then return false end
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, index = pkt:readU32()
	if not ret then return false end
	ret, pool_id = pkt:readU32()
	if not ret then return false end
	ret, hp = pkt:readU32()
	if not ret then return false end
	return true, faction_guid,player_guid, index, pool_id, hp
end

function unpack_logind_send_rename_check_result(pkt)
	local ret, player_guid, available, realName
	
	ret, player_guid = pkt:readUTF()
	if not ret then return false end
	ret, available = pkt:readU32()
	if not ret then return false end
	ret, realName = pkt:readUTF()
	if not ret then return false end
	
	return true, player_guid, available, realName
end


--通知场景服给玩家加经验
--@scened_fd 场景服连接id
--@guid 需要增加经验的玩家
--@exp 增加的经验数量
--@add_type 增加类型
function call_add_exp( scened_fd, guid, exp, add_type )
	local pkt = Packet.new(INTERNAL_OPT_ADD_EXP)
	pkt:writeUTF(guid)
	pkt:writeU32(exp)
	pkt:writeByte(add_type or 0)
	app:sendToConnection(scened_fd, pkt)
	pkt:delete()
end

--场景服发起传送
function call_appd_teleport( scened_fd, guid, x, y, mapid, genal_guid, instanceid)
	local pkt = Packet.new(INTERNAL_OPT_APPD_TELEPORT)
	pkt:writeUTF(guid)
	pkt:writeU16(x)
	pkt:writeU16(y)
	pkt:writeU32(mapid)
	pkt:writeU32(instanceid or 0)
	pkt:writeUTF(genal_guid or '')
	app:sendToConnection(scened_fd, pkt)
	pkt:delete()
end

--通知场景服干点什么，统一接口
function call_scened_appd_to_send_do_something(scened_fd, guid, typ, data, str)
	local pkt = Packet.new(INTERNAL_OPT_APPD_TO_SEND_DO_SOMETHING)
	pkt:writeUTF(guid)
	pkt:writeU32(typ)
	pkt:writeU32(data or 0)
	pkt:writeUTF(str or '')
	app:sendToConnection(scened_fd, pkt)
	pkt:delete()
end

-- 通知登录服 跨服数据
function call_appd_login_to_send_kuafu_info(login_fd, guid, war_id, pos, battle_server, watcher_guid, kuafu_type)
	local pkt = Packet.new(INTERNAL_OPT_KUAFU_ENTER)
	pkt:writeUTF(guid)
	pkt:writeUTF(war_id)
	pkt:writeU32(pos)
	pkt:writeUTF(battle_server)
	pkt:writeUTF(watcher_guid or '')
	pkt:writeU32(kuafu_type or KUAFU_TYPE_FENGLIUZHEN)
	app:sendToConnection(login_fd, pkt)
	pkt:delete()
end

--通知登录服把玩家传送回游戏服
function call_goback_game_svr( guid )
	local pkt = Packet.new(INTERNAL_OPT_GOBACK_GAME_SVR)
	pkt:writeUTF(guid)
	app:sendToLogind(pkt)
	pkt:delete()
end

--GM命令
function call_gm_command( guid, command )
	local pkt = Packet.new(INTERNAL_OPT_GM_COMMMANDS)
	pkt:writeUTF(guid)
	pkt:writeUTF(command)
	app:sendToLogind(pkt)
	app:sendToPoliced(pkt)
	serverConnList:forEachScenedFD(function (fd)
		app:sendToConnection(fd, pkt)
	end)
	pkt:delete()
end

--操作命令
function call_opt_command( guid, connectid, cmd )
	local pkt = Packet.new(INTERNAL_OPT_COMMMAND)
	pkt:writeU32(guid)
	pkt:writeU32(connectid)
	pkt:writeUTF(cmd)
	app:sendToCentd(pkt)
	pkt:delete()
end

--通知中心服断开连接
function call_opt_cent_destory_conn( fd, is_force )
	local pkt = Packet.new(INTERNAL_OPT_CENTD_DESTORY_CONN)
	pkt:writeU32(fd)
	pkt:writeU32(is_force)
	app:sendToCentd(pkt)
	pkt:delete()
end

-- 通知登录服登录
function call_opt_login_teleport(guid, factionId, mapId, x, y, generalId)
	local pkt = Packet.new(INTERNAL_OPT_INVITE_RET)
	pkt:writeUTF(guid)
	pkt:writeUTF(factionId)
	pkt:writeU32(mapId)
	pkt:writeU32(x)
	pkt:writeU32(y)
	pkt:writeUTF(generalId)
	app:sendToLogind(pkt)
	pkt:delete()
end

--场景服发起传送
function call_opt_login_check_name(player_guid, name)
	local pkt = Packet.new(INTERNAL_OPT_RENAME_CHECK)
	pkt:writeUTF(player_guid)
	pkt:writeUTF(name)
	app:sendToLogind(pkt)
	pkt:delete()
end

function call_opt_update_char_name(player_guid, name)
	local pkt = Packet.new(INTERNAL_OPT_UPDATE_CHAR_NAME)
	pkt:writeUTF(player_guid)
	pkt:writeUTF(name)
	app:sendToLogind(pkt)
	pkt:delete()
end