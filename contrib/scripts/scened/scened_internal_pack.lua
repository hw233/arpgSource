local Packet = require 'util.packet'

--通知应用服发公告
function call_appd_send_notice( id, content, data )
	local pkt = Packet.new(INTERNAL_OPT_NOTICE)
	pkt:writeU32(id)
	pkt:writeUTF(content)
	pkt:writeUTF(data)
	app:sendToAppd(pkt)
	pkt:delete()
end

--通知应用服发送聊天信息
function call_appd_send_chat(c_type, guid, content, to_guid, to_name)
	local pkt = Packet.new(INTERNAL_OPT_CHAT)
	pkt:writeByte(c_type)
	pkt:writeUTF(guid)
	pkt:writeUTF(content)
	pkt:writeUTF(to_guid)
	pkt:writeUTF(to_name)
	app:sendToAppd(pkt)
	pkt:delete()
end


-- 通知应用服加道具
function call_appd_add_items(guid, itemDict, logtype, bagFullCategory)
	local pkt = Packet.new(INTERNAL_OPT_ADD_ITEMS)
	pkt:writeUTF(guid)
	pkt:writeU16(#itemDict)
	for i = 1, #itemDict do
		local config = itemDict[ i ]
		pkt:writeU32(config[ 1 ])
		pkt:writeU32(config[ 2 ])
	end
	pkt:writeByte(logtype)
	pkt:writeByte(bagFullCategory)
	app:sendToAppd(pkt)
	pkt:delete()
end


function call_appd_faction_challenge_boss_win(faction_guid)
	local pkt = Packet.new(INTERNAL_OPT_FACTION_BOSS_WIN)
	pkt:writeUTF(faction_guid)
	app:sendToAppd(pkt)
	pkt:delete()
end

function call_appd_faction_challenge_boss_fail(faction_guid,fail_type)
	local pkt = Packet.new(INTERNAL_OPT_FACTION_BOSS_FAIL)
	pkt:writeUTF(faction_guid)
	pkt:writeU32(fail_type)
	app:sendToAppd(pkt)
	pkt:delete()
end

function call_appd_faction_add_token_points(faction_guid,player_guid,points)
	local pkt = Packet.new(INTERNAL_OPT_FACTION_ADD_TOKEN_POINTS)
	pkt:writeUTF(faction_guid)
	pkt:writeUTF(player_guid)
	pkt:writeU32(points)
	app:sendToAppd(pkt)
	pkt:delete()
end

function call_appd_faction_challenge_boss_damaged(faction_guid,player_guid,damage)
	local pkt = Packet.new(INTERNAL_OPT_FACTION_BOSS_DAMAGED)
	pkt:writeUTF(faction_guid)
	pkt:writeUTF(player_guid)
	pkt:writeU32(damage)
	app:sendToAppd(pkt)
	pkt:delete()
end

function call_appd_faction_update_boss_info(faction_guid,hp_rate,x,y)
	local pkt = Packet.new(INTERNAL_OPT_FACTION_UPDATE_BOSS_INFO)
	pkt:writeUTF(faction_guid)
	pkt:writeU32(hp_rate)
	pkt:writeU32(x)
	pkt:writeU32(y)
	app:sendToAppd(pkt)
	pkt:delete()
end

function call_appd_faction_update_target_info(faction_guid,hp_rate,x,y)
	local pkt = Packet.new(INTERNAL_OPT_FACTION_UPDATE_TARGET_INFO)
	pkt:writeUTF(faction_guid)
	pkt:writeU32(hp_rate)
	pkt:writeU32(x)
	pkt:writeU32(y)
	app:sendToAppd(pkt)
	pkt:delete()
end

--家族首领挑战击杀
function call_appd_faction_bossdefense_win(faction_guid,player_guid,pool_id)
	local pkt = Packet.new(INTERNAL_OPT_FACTION_BOSSDEFENSE_WIN)
	pkt:writeUTF(faction_guid)
	pkt:writeUTF(player_guid)
	pkt:writeU32(pool_id)
	app:sendToAppd(pkt)
	pkt:delete()
end

--家族首领挑战离开结算
function call_appd_faction_bossdefense_leave(faction_guid,player_guid,index,pool_id,hp)
	local pkt = Packet.new(INTERNAL_OPT_FACTION_BOSSDEFENSE_LEAVE)
	pkt:writeUTF(faction_guid)
	pkt:writeUTF(player_guid)
	pkt:writeU32(index)
	pkt:writeU32(pool_id)
	pkt:writeU32(hp)
	app:sendToAppd(pkt)
	pkt:delete()
end

-- 通知登录服 跨服数据
function call_scene_login_to_kuafu_back(login_fd, guid)
	local pkt = Packet.new(INTERNAL_OPT_KUAFU_BACK)
	pkt:writeUTF(guid)
	app:sendToConnection(login_fd, pkt)
	pkt:delete()
end