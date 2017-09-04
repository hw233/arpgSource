--先载入一些常量
require("const")
outString('main script file is loading')
outString(__SCRIPTS_ROOT__..'tea_robotd.lua')

outString('util.functions is loading')
require('util.functions')

require("util/utils")

outString('operation_info.lua is loading')
require("template.operation_info")

outString('item_template.lua is loading')
require("template.tables.init")

config = {
	war_server_optcode ={
		CMSG_GET_SESSION,
		CMSG_WORLD_WAR_CS_PLAYER_INFO,
		CMSG_GOBACK_TO_GAME_SERVER,
		CMSG_MODIFY_WATCH,					--客户端订阅
		
		MSG_UNIT_MOVE,
		MSG_MOVE_STOP,
		MSG_SPELL_START,
		MSG_SPELL_STOP,
		MSG_SELECT_TARGET,
		CMSG_USE_GAMEOBJECT,
		--MSG_INSTANCE_READY,
		CMSG_INSTANCE_NEXT_STATE,
		MSG_SYNC_MSTIME, --同步时间
		--MSG_TEST_NETD_SCENED,
		CMSG_JUMP_START	,-- /*跳跃开始*/
		--MSG_JUMP_END	,-- /*跳跃结束*/
		CMSG_CLIENT_UPDATE_SCENED,	--/*客户端热更场景模块后获取uint*/
		CMSG_LOOT_SELECT,
		MSG_CLIENTSUBSCRIPTION,		--/*查询头像的buffer信息*/
        --CMSG_USE_GAMEOBJECT_START,		-- 使用有效对象开始
        --CMSG_FLZ_ENTER_FENGLIUZHEN_PUB,	--进入风流镇酒馆
		CMSG_INSTANCE_EXIT,			--退出副本
		--CMSG_MOUNT_RIDING,			--坐骑骑乘
		--CMSG_FLZ_SHWJ_KEJI_LEVELUP,	--生化危机升级科技
		--CMSG_FLZ_SHWJ_HUANZIDAN,	--生化危机换子弹
		--CMSG_FLZ_HDQS_OPT,			--荒岛求生玩家操作
		CMSG_GOLD_RESPAWN,			--元宝复活
		CMSG_XIANFU_RANDOM_RESPAWN,	--随机复活
	},
}

app = require("robotd.robotd_app").new()

guidMgr = require 'share.guid_manager'

function ___OUT_DEBUG_INFO__()
	outString("out debug info")
	app:OutDebugInfo()
end

