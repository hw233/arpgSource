--先载入一些常量
require("const")
outString('载入脚本主文件')
outString(__SCRIPTS_ROOT__..'tea_logind.lua')

outString('load share.global_manager')
require('share.global_manager')

outString('玩家信息')
require("logind/logind_player")

outString('开始载入文字信息脚本')
require("logind/notice_logind_CN")

outString('开始载入名字校验脚本')
require("logind/illegal_char")
require("template/logind_init")

require("util/utils")

-------------------------------------------------------------------------------
--配置文件信息
config = {
	auto_save_time = 360,						--自动保存数据时间间隔，秒
	auto_backup_hdd_time	= 1800,				--每30分钟存一次回档，秒
	reload_game_config_interval = 600,			--重新读取游戏配置间隔
	Save_Player_To_DB_Count = 1,				--每次轮询保存到数据库的个数\
	world_boss_waitting_room_limit = 50					,--世界BOSS房间人数上限
	cant_make_name = NAME_ILLEGAL_CHAR,			--取名屏蔽字
	--游戏服命令表
	external_router_map = {
		CMSG_FORCED_INTO,
		CMSG_GET_CHARS_LIST,
		CMSG_CHECK_NAME,
		CMSG_CHAR_CREATE,
		CMSG_CHAR_UPDATE_INFO,
		CMSG_DELETE_CHAR,
		CMSG_PLAYER_LOGIN,
		CMSG_PLAYER_LOGOUT,
		CMSG_REGULARISE_ACCOUNT,		
		CMSG_PLAYER_KICKING,	--踢人
		CMSG_CHANGE_LINE,
		CMSG_SHOW_MAP_LINE,
		--CMSG_KUAFU_FIGHT_SIGN,
		CMSG_GOBACK_TO_GAME_SERVER,
		--CMSG_KUAFU_FIGHT_CANCEL_SIGN,
		--CMSG_SEND_TO_WORLD,					--发给世界服
		--CMSG_ADD_WATCH_WORLD_OBJECT,		--请求监听世界服某对象
		--CMSG_DEL_WATCH_WORLD_OBJECT,		--取消世界服某对象的监听
		CMSG_MODIFY_WATCH,					--客户端订阅
		CMSG_KUAFU_CHUANSONG,				--跨服传送
	},
	--pk服命令表
	pk_external_router_map = {
		CMSG_WORLD_WAR_CS_PLAYER_INFO,
		CMSG_GOBACK_TO_GAME_SERVER,
		CMSG_MODIFY_WATCH,					--客户端订阅
	},
}

--校验协议注册是否有效
CheckRouterMap(config.external_router_map)
CheckRouterMap(config.pk_external_router_map)

--短信发送接口
function send_sms(tele_no,content)
	content = "《天书世界》意见提交验证码："..content.."，感谢您的支持，祝您游戏愉快。"
	cmd = 'sh '..__SCRIPTS_ROOT__..'sendsms.sh '..tele_no..' '..content..' &'
	os.execute(cmd)
end

--服务端异常发送短信接口
function send_serverabnormal(tele_no,server_type)
	--local lfs = require "lfs"
	--local sever = 0
	--if(server_type == 2)then
	--	sever = "场景服"
	--elseif(server_type == 3)then
	--	sever = "应用服"
	--end
	--content = "路径为"..lfs.currentdir().."下的"..sever.."发生异常"
	--cmd = 'sh '..__SCRIPTS_ROOT__..'sendsms.sh '..tele_no..' '..content..' &'
	--os.execute(cmd)
end

--应用数据库game_config表的配置
function DoReloadGameConfigTable(key, value)
	if(key == "GM_QQ")then
		local qq = globalGameConfig:GetGmQQ()
		if(qq ==nil or value ~= qq)then
			globalGameConfig:SetGmQQ(value)
		end
	elseif(key == "PT_NAME")then
		local pt_name = globalGameConfig:GetFlag(GLOBALVALUE_FIELD_FLAGS_SHOW_PLATFORM_NAME)
		if(tonumber(value) == 1 and pt_name == false)then
			globalGameConfig:SetFlag(GLOBALVALUE_FIELD_FLAGS_SHOW_PLATFORM_NAME)
		elseif(tonumber(value) == 0 and pt_name == true)then
			globalGameConfig:UnSetFlag(GLOBALVALUE_FIELD_FLAGS_SHOW_PLATFORM_NAME)
		end
	elseif(key == "SERVER_ID")then
		local server_id = globalGameConfig:GetFlag(GLOBALVALUE_FIELD_FLAGS_SHOW_SERVER_ID)
		if(tonumber(value) == 1 and server_id == false)then
			globalGameConfig:SetFlag(GLOBALVALUE_FIELD_FLAGS_SHOW_SERVER_ID)
		elseif(tonumber(value) == 0 and server_id == true)then
			globalGameConfig:UnSetFlag(GLOBALVALUE_FIELD_FLAGS_SHOW_SERVER_ID)
		end
	end
end

--修复玩家数据
function RepairPlayerData(account, data_table)	
	if(#data_table == 0)then
		outString(String.Format(" lua RepairPlayerData error, %s not data", account));
		return false
	end
	
	return true
end

--玩家下线的时候判断是否记录玩家所在地图的信息，最多保留10分钟
-- fuben_type = baseInfo.instance_type
function DoIsRecordIntanceInfo(player, map_id, isfuben, fuben_type)	
	if(isfuben == 0 or fuben_type ~= MAP_INST_TYP_SINGLETON)then
		return false
	end

	return true
end

--是否需要提供传送generalid,通过地图id判断
function DoIsNeedGeneralid(mapid)
	return tb_map[mapid] and tb_map[mapid].is_need_general == 1
end

function DoIsOfflineReenter(mapid)
	return tb_map[mapid] and tb_map[mapid].isOfflineReenter == 1
end

--地图管理器，查找或创建地图
function FindOrCreateMap(mapid, inst_type, general_id, lineno)
	if(inst_type == MAP_INST_TYP_SINGLETON)then	--单人副本
		return createInstance(mapid, general_id, lineno)
	end

	--找一下真正该去的分线号
	local lineno_true = GetWantToLineno(mapid, lineno, general_id)
	
	--遍历实例查找该地图
	local result = findInstance(mapid, general_id, lineno_true)
	outFmtDebug("---------FindOrCreateMap %u %u '%s' %u result = %d", mapid, inst_type, general_id, lineno_true, result)
	if(result < 0)then
		result = createInstance(mapid, general_id, lineno_true)		--如果没有就创建一个新的实例
	end
	return result
end

-- 获得地图人数配置
function OnGetMapLineCount(mapid)
	return map_line_config[mapid] or 5000
end

--地图分线人数配置
map_line_config = {
	[1] = 200,
	[1001] = 200,
	[1002] = 200,
	[2016] = config.world_boss_waitting_room_limit,
	[2017] = config.world_boss_waitting_room_limit,
}
--获取真正该传去的分线号
function GetWantToLineno(mapid, lineno, general_id)
	if(lineno > 0)then
		return lineno
	end
	local count = map_line_config[mapid]
	if(count == nil)then
		--为什么不返回零呢，因为有时候分线代表的是场次id
		return lineno
	end
	local i = 1
	--还是要给一个最大值，万一呢，哈哈，死循环！
	while(i < 2000)do
		if(mapPlayerCount(mapid, general_id, i) < count)then
			break
		end
		i = i + 1
	end
	--已经有那么那么多线了，哈哈，随便进一个吧
	if(i >= 2000)then
		i = randInt(1, 2000)
	end
	return i
end

--获取下次保存数据库的时间
function GetNextSaveTime()
	local t = os.time()
	local dt = os.date("*t", t)
	dt.hour = 2					--2点
	dt.min = 0
	dt.sec = 0
	if(os.time(dt) < t)then
		dt.day = dt.day + 1
	else
	end
	local begin_t = os.time(dt)
	dt.hour = dt.hour + 4		--6点
	local end_t = os.time(dt)
	local result = randInt(begin_t, end_t)
	dt = os.date("*t", result)
	outString(string.format("rand save time %d-%d %d:%d:%d", dt.month, dt.day, dt.hour, dt.min, dt.sec))
	return result
end