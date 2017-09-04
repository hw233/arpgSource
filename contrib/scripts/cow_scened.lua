--先载入一些常量
require("const")

outString('load share.global_manager')
require('share.global_manager')

outString('load template')
require("template/scened_init")
require("template/conditions")

require("util/utils")

outString('load share.tick_name script')
require("share/tick_name")
require 'scened.scened_internal_pack'

-- 第一此生成世界BOSS信息
globalValue:RandomStepWorldBossIfNeverDoes()
-------------------------------------------------------------------
--初始化一些东西，例如BUFF对游戏操作的限制 用数字的说明是一些特殊的东西
config = {
	--以下来自原来的配置文件
	max_player_level = 200					,--最大等级
	
	new_player_protected_level = 0			,--新手保护最大等级
		
	jump_cd = 4								,--跳cd
	jump_max_distance = 15					,--最大跳跃距离
	
	nomal_attack_distance	= 4				,--默认攻击距离
	
	can_change_mode	= {PEACE_MODE, FAMILY_MODE} ,--允许手动切换的模式
	
	peace_mode_cd = 60						,--和平模式CD(秒)
	
	field_boss_pick_max_distance = 5		,-- 野外boss开宝箱最大距离
	
	dead_protect_CD = 3600					,--死亡保护时间
	
	evil_max_value = 10						,--恶人值最大值					
	
	nomal_attack_time =  1300				,--默认攻击时间
	nomal_move_speed = 180					,--默认移动速度
	
	world_boss_wait		  = 15				,--世界BOSS等待刷新时间(s)
	world_boss_last		  = 15				,--世界BOSS持续时间(m)
	world_boss_invincible_time	= 11		,--世界BOSS无敌持续时间(s)
	world_boss_roll_last_time	= 7			,--世界BOSSroll点持续时间(s)
	
	field_boss_born_time  = 1				,--野外boss刷新时间提醒(m)

	update_ownership_time = 1500			,--更新怪物所有者的间隔
	left_fighting_time = 6000				,--脱离战斗时间
	left_cast_time = 3000					,--脱离施法战斗（即备战）时间
	heathy_auto_recovery_cycle = 3000		,--自动回血蓝时间
	heathy_auto_recovery_percent = 1		,--自动回血蓝比例
	energy_auto_recovery_cycle = 1000		,--自动回体时间
	energy_auto_recovery_values = 5			,--自动回体力值
	endurance_auto_recovery_cycle = 60000	,--回精力时间
	endurance_auto_recovery_values = 1 		,--回精力值
	error_distance = 7						,--服务端允许客户端距离误差

	jump_need_energy = 20					,--跳跃消耗体力
	notice_distance = 6						,--地图触发点通知距离
	zodiac = 12								,--生肖数量
	full_awaken = 1200						,--最大生肖唤醒值
	mount_spell1 = 61						,--坐骑一级技能
	mount_spell2 = 62						,--坐骑二级技能
	mount_spell3 = 63						,--坐骑三级技能
	mount_spell_evolve = 20					,--坐骑技能每20级变一次
	loot_area = 2							,--战利品占地面积的边长--
	loot_exist_timer = 20					,--战利品存在时间
	loot_change_owner = 10					,--战利品改变所有者时间
	loot_money_entry = 3					,--战利品钱的模板
	loot_caishenbaoxiang_entry = 368		,--战利品财神宝箱的模板
	respawn_max_level = 15					,--小于该等级可以原地复活
	sub_weapon_durability = 6				,--每攻击一次武器扣的耐久
	sub_other_durability = 1				,--每次扣除的装备(除了武器和无限耐久装备)耐久的基本单位
	new_bird_protect_lv = 50				,--新手保护等级
	monster_random_radius_percent = 0.16	,--怪物随机移动半径与怪物视野关径的比率,目前是 13*0.166
	red_name_time = 60						,--主动砍人后红名时间
	safe_distance = 6						,--复活点安全半径
	use_gameobject_diff = 0					,--使用需要读条的GO允许的时间差
	max_npc_distance = 20					,--允许与NPC的最长距离

	can_move_dis_map				= {57},--允许坐标不同步的地图
	cant_attack						= {BUFF_YUNXUAN,BUFF_CHENMO,BUFF_ZHIKONG,BUFF_SHIHUA,BUFF_YINCHANG},	--限制攻击的BUFF
	cant_cast						= {BUFF_YUNXUAN,BUFF_CHENMO,BUFF_ZHIKONG,BUFF_SHIHUA,BUFF_YINCHANG},	--限制施法的BUFF
	cant_move						= {BUFF_YUNXUAN,BUFF_DINGSHEN,BUFF_ZHIKONG,BUFF_SHIHUA,BUFF_YINCHANG,BUFF_HDQS_NET},--限制主动移动的BUFF
	cant_recovery					= {},	--被限制恢复体力，内力

	can_remove_buff					= {},	--属性重算的时候能移除的buff
	cant_jump						= {BUFF_YUNXUAN,BUFF_DINGSHEN,BUFF_ZHIKONG,BUFF_HDQS_NET},--限制跳跃的BUFF
	cant_teleport					= {},							--限制传送的BUFF
	cant_use_item					= {},							--限制使用物品的BUFF
	
	--注：羊羊无敌不可见仅用于怪物。客户端无视
	cant_see						= {},							--隐身的BUFF
	invincible						= {},							--无敌的BUFF
	cant_fight_with_player	 		= {},							--不能与玩家战斗的BUFF
	cant_attack_be_player			= {},							--不会被玩家攻击
	ignore_the_obstacles			= {},				--无视状态
	--战利品配置
	loot_possibility_range			= {0,9999},				--物品掉落配置,机率范围
	--游戏服命令表
	external_router_map = {
		MSG_UNIT_MOVE,
		CMSG_TELEPORT,
		MSG_MOVE_STOP,
		MSG_SPELL_START,
		--MSG_SPELL_STOP,
		--MSG_SELECT_TARGET,
		--CMSG_SET_ATTACK_MODE,
		CMSG_USE_GAMEOBJECT,
		--CMSG_GOSSIP_HELLO,
		--CMSG_GOSSIP_SELECT_OPTION,
		--CMSG_TAXI_HELLO,
		--CMSG_TAXI_SELECT_STATION,
--[[		CMSG_QUESTGIVER_STATUS_QUERY,
		CMSG_QUESTHELP_GET_CANACCEPT_LIST,
		CMSG_QUESTLOG_REMOVE_QUEST,
		CMSG_QUESTGIVER_ACCEPT_QUEST,
		CMSG_QUESTGIVER_COMPLETE_QUEST,
		CMSG_QUESTUPDATE_USE_ITEM,
		CMSG_QUESTHELP_COMPLETE,
		CMSG_QUESTHELP_UPDATE_STATUS,
		CMSG_QUESTHELP_TALK_OPTION,--]]
		--CMSG_RESURRECTION,
		CMSG_INSTANCE_EXIT,
--		CMSG_QUESTHELP_QUERY_BOOK,
--		CMSG_INSTANCE_NEXT_STATE,
--		MSG_QUERY_QUEST_STATUS,
--		CMSG_INSTANCE_ENTER,
		CMSG_RIDE_MOUNT,
		CMSG_CHANGE_BATTLE_MODE,
		CMSG_JUMP_START	,			-- /*跳跃开始*/
		CMSG_ENTER_VIP_INSTANCE,	-- 请求进入vip副本
		CMSG_ENTER_TRIAL_INSTANCE,
		CMSG_TELEPORT_MAIN_CITY,
		CMSG_USE_BROADCAST_GAMEOBJECT,
		CMSG_WORLD_BOSS_FIGHT,	-- /*世界BOSS挑战*/
		CMSG_ROLL_WORLD_BOSS_TREASURE,	-- roll点
		CMSG_RES_INSTANCE_ENTER,
		CMSG_TELEPORT_MAP,
		CMSG_TELEPORT_FIELD_BOSS,
		CMSG_TALK_WITH_NPC,
		CMSG_LOOT_SELECT,
		CMSG_GOLD_RESPAWN,			--元宝复活
		CMSG_XIANFU_RANDOM_RESPAWN,	--随机复活
		MSG_CLIENTSUBSCRIPTION,		--/*查询头像的buffer信息*/
		MSG_USE_JUMP_POINT,		-- 使用跳点
		CMSG_BACK_TO_FAMITY,		-- 返回家族
		CMSG_CHALLANGE_BOSS,		-- 挑战boss
		CMSG_PICK_OFFLINE_REWARD,
		
		CMSG_TRY_MASS_BOSS,			-- 挑战全民boss
		CMSG_QUERY_MASS_BOSS_INFO,	-- 查询挑战人员个数
		CMSG_QUERY_MASS_BOSS_RANK,	-- 查询排行榜
		CMSG_ENTER_RISK_INSTANCE,

		--CMSG_CHANGE_LINE,
		--[[
		CMSG_START_HUNG_UP,--开始挂机*/
		CMSG_STOP_HUNG_UP,--停止挂机*/
		MSG_SYNC_MSTIME,--同步时间 29
		MSG_TEST_NETD_SCENED,
		
		MSG_JUMP_END	,-- /*跳跃结束*/
		CMSG_CLIENT_UPDATE_SCENED,	--/*客户端热更场景模块后获取uint*/
		
		CMSG_ZHUAN_SHENG,	--转生
		CMSG_FUBEN_HUNG_UP,-- 副本挂机操作
		CMSG_ENTER_FEISHENGZHILU,	--进入飞升之路		
		CMSG_ENTER_QIYU,			--进入奇遇副本
		CMSG_QUERY_FUBEN_INFO,		-- /*查询副本信息*/
		SMSG_QUERY_FUBEN_INFO_RESULT,	-- /*查询副本信息结果*/
        CMSG_ENTER_PANTAOYUAN,  	-- /*进入蟠桃园*/
        CMSG_USE_GAMEOBJECT_START,		-- 使用有效对象开始
		CMSG_RECEIVE_OTHER_QUEST,	--接受其他任务(江湖密令、悬赏等)
		CMSG_COMPLETE_OTHER_QUEST,	--完成其他任务(江湖密令、悬赏等)
		CMSG_INSTANCE_ALL_KILL_OPT,	--全屏秒杀操作
		CMSG_MOUNT_RIDING,			--坐骑骑乘
		MSG_MAP_JUMP,				--地形跳跃
		CMSG_ENTER_WULIN,			--进入武林秘境
		CMSG_DABOSS_TELEPORT,		--打boss传送
		CMSG_BOSS_DROP_EVENT_QUERY,				--boss掉落动态查询
		MSG_QUERY_KILL_BOSS_PLAYERNAME,			--上轮被击杀BOSS查询
		CMSG_ENTER_WUSHEN_LEITAI,				--进入武神擂台
		CMSG_ASK_FOR_BUFF,						--请求BUFF处理
		CMSG_ENTER_DAILY_INSTANCE,				--进入日常副本
		CMSG_ENTER_SWFJ_INSTANCE,				--进入塞外伏击
		CMSG_GET_SWFJ_INSTANCE_REWARD,			--塞外伏击领取经验
		CMSG_SWFJ_INSTANCE_JIANZAO,				--塞外伏击建造
		CMSG_ENTER_FACTION_FUBEN,				--进入帮派副本
		]]
		CMSG_FORCEINTO,							--强制进入
		--CMSG_SET_ORIENT,
	},
	--pk服命令表
	pk_external_router_map = {
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
		--CMSG_SET_ORIENT,
	},
}

--校验协议注册是否有效
print("check external_router_map")
CheckRouterMap(config.external_router_map)
print("check pk_external_router_map")
CheckRouterMap(config.pk_external_router_map)

app = require('scened.ScenedApp'):new()

--------------------------
outString('load Unit script')
require 'scened.unit.init'

-------------------------------------------------------------------
--载入其他脚本
function load_lua_scripts()
	local tbl = {
		{'战斗信息脚本'		,'scened/fighting_info'},
		{'公式脚本'			,'scened/formula'},
		{'buffer脚本'		,'scened/buffer'},
		{'技能脚本脚本'		,'scened/spell'},
		{'技能范围脚本'		,'scened/hit'},
		{'使用物品脚本'		,'scened/item'},
		{'伤害计算脚本'		,'scened/meleeattack'},
		{'闲聊及任务脚本'	,'scened/gossip'},
		{'怪物智能脚本'		,'scened/script_ai'},
		{'NPC智能脚本'		,'scened/npc_ai'},
		{"战利品脚本"		,'scened/loot_manager'},
		
		{'地图基础脚本'		,'scened/instance/instance_base'},
		
		{'地图基础主城脚本'		,'scened/instance/instance_main_base'},
		{'地图基础副本脚本'		,'scened/instance/instance_inst_base'},
		{'地图基础野外脚本'		,'scened/instance/instance_field_base'},
		
		
		{'地图VIP副本脚本'		,'scened/instance/instance_vip'},
		{'试炼塔副本脚本'		,'scened/instance/instance_trial'},
		{'地图世界冒险副本脚本'	,'scened/instance/instance_worldrisk'},
		
		{'地图世界BOSS等待脚本'	,'scened/instance/instance_worldboss_prepare'},
		{'地图世界BOSS脚本'		,'scened/instance/instance_worldboss'},
		
		{'资源副本脚本'			,'scened/instance/instance_res_base'},
		{'资源真气副本脚本'		,'scened/instance/instance_res_zhenqi'},
		{'资源金币副本脚本'		,'scened/instance/instance_res_gold'},
		{'资源宝石精华副本脚本'	,'scened/instance/instance_res_gem'},
		{'仙府夺宝测试副本脚本'	,'scened/instance/instance_xianfu_test'},
		
		{'斗剑台'				,'scened/instance/instance_doujiantai'},
		{'跨服3v3'				,'scened/instance/instance_kuafu_3v3'},
		{'跨服仙府夺宝'			,'scened/instance/instance_kuafu_xianfu'},
		{'修炼场'				,'scened/instance/instance_xiulianchang'},
		{'原地副本0'			,'scened/instance/instance_remind_0'},
		{'家族场景'				,'scened/instance/instance_faction'},
		{'家族首领挑战'			,'scened/instance/instance_faction_bossdefense'},
		{'家族无尽远征'			,'scened/instance/instance_faction_tower'},
		{'排名赛'				,'scened/instance/instance_qualify'},
		--{'九重天脚本1'		,'scened/instance/instanceTower1'},
		--{'九重天脚本2'		,'scened/instance/instanceTower2'},
		--{'桃花迷阵脚本'		,'scened/instance/instanceTaoHua'},
		
		{'全民boss'				,'scened/instance/instance_mass_boss'},
		{'跨服组队副本'			,'scened/instance/instance_kuafu_group'},
		
		{'LUA之GM命令'		,'scened/gm_command'},
	}
	local i = 0
	repeat
		i = i + 1
		require(tbl[i][2])
	until #tbl == i

end
load_lua_scripts()

-- 映射mapid和脚本关系
INSTANCE_SCRIPT_TABLE = {}

function onRangeMapping (mapIdInfo, clazz)
	local paramType = type(mapIdInfo)
	if paramType == "table" then
		setMapScriptMappin(mapIdInfo, clazz)
	elseif paramType == "number" then
		setMapScriptMappin({mapIdInfo}, clazz)
	end
end

function setMapScriptMappin(mapIdList, clazz)
	for _, mapid in ipairs(mapIdList) do
		INSTANCE_SCRIPT_TABLE[mapid] = clazz
	end
end

function initScriptTable()
	--  mainCity:array 主城
	onRangeMapping(tb_script_base[ 1 ].mainCity, InstanceMainBase)
	
	--  field:array 野外
	onRangeMapping(tb_script_base[ 1 ].field, InstanceFieldBase)
	
	--  trial:int 试炼塔
	onRangeMapping(tb_script_base[ 1 ].trial, InstanceTrial)
	
	--  risk:array 世界冒险
	onRangeMapping(tb_script_base[ 1 ].risk, InstanceWorldRisk)
	
	--  worldbossPrepare:int 世界BOSS等待
	onRangeMapping(tb_script_base[ 1 ].worldbossPrepare, InstanceWorldBossP)
	
	--  worldboss:int 世界BOSS
	onRangeMapping(tb_script_base[ 1 ].worldboss, InstanceWorldBoss)
	
	--  resExp:int 资源经验
	onRangeMapping(tb_script_base[ 1 ].resExp, InstanceResBase)
	
	--  resQI:int 资源真气
	onRangeMapping(tb_script_base[ 1 ].resQI, InstanceResZhenQi)
	
	--  resSoul:int 资源兽灵
	onRangeMapping(tb_script_base[ 1 ].resSoul, InstanceResGold)

	--  resGold:int 资源银币
	onRangeMapping(tb_script_base[ 1 ].resGold, InstanceResGold)

	--  resGem:int 资源宝石
	onRangeMapping(tb_script_base[ 1 ].resGem, InstanceResGem)

	--  massBoss:array 全民boss
	onRangeMapping(tb_script_base[ 1 ].massBoss, InstanceMassBoss)
	
	--  doujiantai:int 斗剑台
	onRangeMapping(tb_script_base[ 1 ].doujiantai, InstanceDoujiantai)
	
	--  kuafuGroup:array 跨服组队
	onRangeMapping(tb_script_base[ 1 ].kuafuGroup, InstanceKuafuGroup)
	
	--  factionBossDefense:array 帮派boss防御
	onRangeMapping(tb_script_base[ 1 ].factionBossDefense, InstanceFactionBossDefense)
		
	--  factionTower:array 帮派爬塔
	onRangeMapping(tb_script_base[ 1 ].factionTower, InstanceFactionTower)
	
	--  faction:int 帮派地图
	onRangeMapping(tb_script_base[ 1 ].faction, InstanceFaction)
		
	--  qualify:int 排位赛
	onRangeMapping(tb_script_base[ 1 ].qualify, InstanceQualify)
	
	--  plot:array 剧情副本
	onRangeMapping(tb_script_base[ 1 ].plot, InstanceRemind0)
	
	
end

initScriptTable()

-- 复活点坐标
function GetRespawnPos()
	return {55,19}
end

-- 副本是否有时间限制
function isMapTimeLimit(mapid)
	return tb_map[mapid] and tb_map[mapid].isTimeLimit == 1
end

-- 获取地图模板名称
function GetMapTemplateName(mapid)
	if tb_map_info[mapid] == nil then
		return ""
	end
	return tb_map_info[mapid].name
end

-- 获取地图模板副本类型（2：单人，1：多人）
function GetMapFubenType(mapid)
	if tb_map_info[mapid] == nil then
		return 0
	end
	return tb_map_info[mapid].instance_type
end

-- 获取是否是副本地图
function IsFubenMap(mapid)
	if tb_map_info[mapid] == nil then
		return false
	end
	return tb_map_info[mapid].is_instance
end

require 'scened.context.init'
