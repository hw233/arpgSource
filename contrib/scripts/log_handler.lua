
local function newLogPacket(opt, log_type, account, player_id  )
	local Packet = require 'util.packet'
	local pkt = Packet.new(assert(opt))
	local tv_sec, tv_usec = gettimeofday()
	pkt:writeU32(assert(log_type))
	pkt:writeU32(tv_sec)
	pkt:writeU32(tv_usec)
	pkt:writeUTF(account or '')
	pkt:writeUTF(player_id or '')
	return pkt
end

--根据传入协议号创建一个函数
local function createWriteLogFunction( opt )	
	local function func( log_type, player_id, ... )
		local pkt = newLogPacket(opt, log_type, '', player_id)
		for _,v in ipairs({...}) do		
			if type(v) == 'number' then
				pkt:writeU32(v)
			else
				--assert(type(v) == 'string')
				pkt:writeUTF(v)
			end
		end
		app:sendToPoliced(pkt)
		pkt:delete()
	end
	return func
end

local writeLog = createWriteLogFunction(INTERNAL_OPT_WRITE_LOG)
local writeTencentLog = createWriteLogFunction(INTERNAL_OPT_TENCENT_LOG)

--道具所有行为
function WriteItemLog(player,income_type,entry,count,bind)
	local pkt = newLogPacket(INTERNAL_OPT_WRITE_LOG, LOG_FILE_TYPE_ITEM_LOG, player:GetAccount(), player:GetGuid())
	pkt:writeU32(entry)
	pkt:writeU32(count)
	pkt:writeU32(player:CountAllItem(entry,bind))
	pkt:writeU32(income_type)	
	pkt:writeU32(player:GetLevel())
	if bind == true then bind = 1 else bind = 0 end
	pkt:writeU32(bind)
	pkt:writeDouble(0)
	pkt:writeDouble(0)	
	pkt:writeU32(player:GetMapId())	
	app:sendToPoliced(pkt)
	pkt:delete()
end

--//TX用户充值记录日志
function WriteTXRechargeLog( account, player_id, player_name, player_level, transactionId, currency, inpourType, addcoin, money, gold, exchangeType, depay, gDepay, state, create_time, platdata )
	local pkt = newLogPacket(INTERNAL_OPT_TENCENT_LOG, LOG_FILE_TENCENT_RECHAGRE, account, player_id )
	pkt:writeUTF(player_name)
	pkt:writeU32(player_level)
	pkt:writeUTF(transactionId)
	pkt:writeUTF(currency)
	pkt:writeUTF(inpourType)
	pkt:writeU32(addcoin)
	pkt:writeDouble(money)
	pkt:writeDouble(gold)
	pkt:writeU32(exchangeType)
	pkt:writeDouble(depay)
	pkt:writeDouble(gDepay)
	pkt:writeU32(state)
	pkt:writeU32(create_time)
	pkt:writeUTF(platdata)
	app:sendToPoliced(pkt)
	pkt:delete()
end

--保存首充日志
function WriteFirstRechargeInfoLog(player,amount,addate,server_name)
	local values = {}
	local p_guid = player:GetGuid()
	values["server_name"] = server_name
	values["guid"] = p_guid
	values["name"] = player:GetName()
	values["i_level"] = tostring(player:GetLevel())
	values["i_value"] = tostring(amount)
	values["i_quest"] = tostring(player:GetMainQuestID())
	values["u_adddate"] = tostring(addate)
	
	local Packet = require 'util.packet'
	local pkt = Packet.new(assert(INTERNAL_OPT_LOGS_FIRST_RECHARGE))
	pkt:writeMap(values)
	app:sendToPoliced(pkt)
	pkt:delete()
end

-- 写聊天日志
function WriteChatLog(player_id, chat_type, target_id,target_name, msg, player_level, gm_level)
	writeLog(LOG_FILE_TYPE_CHAT, player_id, chat_type, target_id, target_name, msg, player_level, gm_level)
end

-- 记录客户端信息
function WriteClientInfoLog(player_id, client, browser, isdebug, language, version, flash_version, x, y, oss)
	writeLog(LOG_FILE_TYPE_CLIENT_INFO, player_id, client, browser, isdebug, language, version, flash_version, x, y, oss)
end