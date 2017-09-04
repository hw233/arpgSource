--先载入一些常量
require("const")
outString('load lua file')
outString(__SCRIPTS_ROOT__..'cow_policed.lua')

outString('load share.global_manager')
require('share.global_manager')
require("template/policed_init")

-------------------------------------------------------------------------------
--配置文件信息
config = {
	--游戏名称简写
	game_code = 'ts',
	--游戏服命令表
	external_router_map = {
		CMSG_WRITE_CLIENT_LOG,
		CMSG_PLAYER_GAG,	--禁言
	},
	--pk服命令表
	pk_external_router_map = {
		CMSG_WRITE_CLIENT_LOG,
	},
}

--校验协议注册是否有效
CheckRouterMap(config.external_router_map)
CheckRouterMap(config.pk_external_router_map)

--平台需要使用的post类集合
PlatfromPostTable = {
	[PLATFORM_GAME2] = {PLATFORM_GAME2},
}

--平台需要使用的游戏名称集合,无配置的平台默认使用game_code
PlatfromGameCodeTable = {
	--[PLATFORM_360] = 'tsol',
}

--post url配置
PostUrlTable = {
	[PLATFORM_GAME2] = {
		[POST_TYPE_NONE] = "",	
		[POST_TYPE_UPGRADE]	= "http://api.mas.g2.cn/updatePlayerInfo.php",
		[POST_TYPE_CHAT] = "http://61.160.220.89:9999/postalarm.php",
	},
}
