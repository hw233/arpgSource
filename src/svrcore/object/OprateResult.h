#ifndef _OPRATE_RESULT_H
#define _OPRATE_RESULT_H

//操作类型
enum ECharOprateType
{
	OPRATE_TYPE_LOGIN					= 1,		//登录
	OPRATE_TYPE_KICKING					= 2,		//踢人
	OPRATE_TYPE_JUMP					= 3,		//跳
	OPRATE_TYPE_ATTK_LOSE				= 4,		//攻击失败
	OPRATE_TYPE_SPELL_LOSE				= 5,		//技能释放失败
	OPRATE_TYPE_INTERACTION				= 6,		//交互,如使用GAMEOBJ 与NPC对话，交接任务等
	OPRATE_TYPE_USE_GAMEOBJECT			= 7,		//使用游戏对象
	OPRATE_TYPE_BAG						= 8,		//包裹
	OPRATE_TYPE_TRADE					= 9,		//交易
	OPRATE_TYPE_CHAT					= 10,		//聊天
	OPERTE_TYPE_RECEIVE_GIFT_PACKS		= 11,		//领取补偿礼包
	OPERTE_TYPE_QUEST					= 12,		//任务
	OPERTE_TYPE_NPCBUY					= 13,		//购买商品
	OPERTE_TYPE_FUBEN					= 14,		//副本
	OPERTE_TYPE_KUAFU					= 15,		//跨服
	OPERTE_TYPE_RANK_LIST				= 16,		//排行榜
	OPERTE_TYPE_CLOSE					= 17,		//连接关闭
	OPERTE_TYPE_HOSTING					= 18,		//托管
	OPERTE_TYPE_STRENGTH				= 19,		//强化
	OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE = 20,		//切换战斗模式失败
	OPERTE_TYPE_SOCIAL					= 21,		//社交
	OPERTE_TYPE_FACTION					= 22,		//帮派
	OPERTE_TYPE_FIELD_BOSS				= 23,		//野外BOSS
	OPERTE_TYPE_WORLD_BOSS				= 24,		//世界BOSS
	OPRATE_TYPE_MOUNT_QICHENG			= 25,		//坐骑骑乘
	OPRATE_TYPE_TELEPORT				= 26,		//传送
	OPRATE_TYPE_UPGRADE					= 27,		//游戏中的升级操作
	OPRATE_TYPE_ACTIVITY				= 28,		//活动
	OPRATE_TYPE_ACHIEVE					= 29,		//成就
	OPRATE_TYPE_ATHLETICS				= 30,		//竞技
	OPRATE_TYPE_XIANFU					= 31,		//仙府夺宝
	OPRATE_TYPE_REWARD					= 32,		//获得物品
	OPRATE_TYPE_DOUJIAN					= 33,		//斗剑台
	OPRATE_TYPE_GEM						= 34,		//宝石
};

//登录相关操作返回
enum CHAR_OPRATE_RESULT
{
	OPRATE_RESULT_SUCCESS				= 1,	//成功
	OPRATE_RESULT_NAME_REPEAT			= 2,	//名称重复
	OPRATE_RESULT_NAME_ILLEGAL			= 3,	//名称非法
	OPRATE_RESULT_CHAR_CAP 				= 4,	//角色数量达到上限
	OPRATE_RESULT_SCENED_CLOSE 			= 5,	//场景服未开启
	OPRATE_RESULT_SCENED_ERROR			= 6,	//场景服重置，其实就是崩了一个场景服换一个新的
	OPRATE_RESULT_LOGINED_IN			= 7,	//角色已登录
	OPRATE_RESULT_OTHER_LOGINED			= 8,	//角色其他地方登录
	OPRATE_RESULT_GAME_VERSION_ERROR	= 9,	//游戏版本不对
	OPRATE_RESULT_MAP_VERSION_ERROR		= 10,	//地图素材版本不对
	OPRATE_RESULT_DATA_VERSION_ERROR	= 11,	//data素材版本不对
	OPRATE_RESULT_VERSION_FORMAT_ERROR	= 12,	//客户端发来的版本信息格式不对
	OPRATE_RESULT_APPD_ERROR			= 13,	//应用服异常重启
	OPRATE_RESULT_LOCK_ACCOUNT			= 14,	//帐号被封
	OPRATE_RESULT_LOCK_IP				= 15,	//IP被封
	OPRATE_RESULT_APPD_CLOSE 			= 16,	//应用服未开启
	OPRATE_RESULT_NAME_TOO_LONG			= 17,	//名字太长
	OPRATE_RESULT_NAME_HAS_PINGBI		= 18,	//有屏蔽词
	OPRATE_RESULT_LOGOUT_UNFINISHED		= 19,	//该角色上一次的登出未完成，请等待
	OPRATE_RESULT_PID_OR_SID_ERROR		= 20,	//服务器id或者平台id错误
	OPRATE_RESULT_DB_RESULT_ERROR		= 21,	//数据库查询出问题了
	OPRATE_RESULT_MERGE_SERVER			= 22,	//合服了
	OPRATE_RESULT_PLAYER_ZIBAO			= 23,	//自爆
	OPRATE_RESULT_GENDER_ILLEGAL		= 24,	//角色种类异常
};

//系统踢人的原因
enum KICKING_TYPE
{
	KICKING_TYPE_GM_LOCK_IP				= 0,	//封IP
	KICKING_TYPE_GM_LOCK_ACCOUNT		= 1,	//封号
	KICKING_TYPE_GM_KICKING				= 2,	//从GM表中读取的踢人
	KICKING_TYPE_KUANGCHANG_RELIVE		= 3,	//荒芜矿场非法复活
	KICKING_TYPE_FUCK_PINGBI			= 4,	//聊天点带屏蔽词
	KICKING_TYPE_QUEST_GET_ITEM			= 5,	//刷物品
	KICKING_TYPE_NOT_MONEY				= 6,	//钱负数还继续消费
	KICKING_TYPE_MAX
};

//跳跃操作返回
enum JUMP_OPRATE_RESULT
{
	JUMP_RESULT_NOMAL					= 1,		//无，默认结果
	JUMP_RESULT_DEAD					= 2,		//死了你还跳，果断是诈尸
	JUMP_RESULT_ENERGY					= 3,		//体力不足
	JUMP_RESULT_MAP_CANT_JUMP			= 4,		//地图无法跳跃
	JUMP_RESULT_CANT_JUMP				= 5,		//玩家被限制不能跳跃
	JUMP_RESULT_MAX_DISTANCE			= 6,		//超过最大距离
	JUMP_RESULT_WRONG_COORD				= 7,		//无效坐标，障碍点之流
	JUMP_RESULT_SPELL_CD				= 8,		//跳跃技能cd中
	JUMP_RESULT_NOT_ACTIVE_SPELL		= 9,		//2段跳没有激活
};

//攻击操作返回
enum Attack_Lost_Reson
{
	ATTACK_LOST_CANTATTACK				= 1,		//被限制不能攻击
	ATTACK_LOST_TARGET_DEAD				= 2,		//目标已经死亡
	ATTACK_LOST_NEED_TARGET				= 3,		//攻击需要一个目标
	ATTACK_LOST_OUT_OF_RANGE			= 4,		//超出攻击范围
	ATTACK_LOST_WRONG_TARGET			= 5,		//错误的目标
	ATTACK_LOST_MOVING					= 6,		//正在移动状态下不能攻击
};

//技能释放操作返回
enum Spell_Lost_Reson
{
	LOST_RESON_SPELL_DOENT_EXIST		= 1,		//技能不存在
	LOST_RESON_NOT_HAVE_SPELL			= 2,		//你没有这个技能
	LOST_RESON_NOT_ENOUGH_ANGER			= 3,		//怒气值不足
	LOST_RESON_SPELL_COOLDOWN			= 4,		//冷却
	LOST_RESON_ALREADY_CAST				= 5,		//已经在施法
	LOST_RESON_TARGET_DEAD				= 6,		//目标已经死亡
	LOST_RESON_NEED_TARGET				= 7,		//此技能需要一个目标
	LOST_RESON_JUMP_DENY				= 8,		//跳跃状态不能施法
	LOST_RESON_SCENE_DENY				= 9,		//场景禁止施法
	LOST_RESON_CAN_NOT_CAST				= 10,		//无法施法
	LOST_RESON_HAS_CONFLICT_SPELL		= 11,		//有技能冲突
	LOST_RESON_NOTHAS_CHUZHAN			= 12,		//当前没有出战兵种
	LOST_RESON_WRONG_FACTION			= 13,		//同一阵营不能攻击（争霸天下用）
	LOST_RESON_ACTIVE_SPELL_SUCCESS		= 14,		//技能激活成功
	LOST_RESON_IN_SAFE_ZONE				= 15,		//安全区域不能攻击（大沼泽用）
	LOST_RESON_YXT_WRONG_FACTION		= 16,		//同一阵营不能攻击（英雄帖用）
	LOST_RESON_PVP_STATE				= 17,		//PVP状态不能释放
};

//交互操作返回
enum INTERACTION_RESULT
{
	INTERACTION_TOO_FAR					= 1,	//NPC太远
	INTERACTION_BOOK_DAILYNUM_FULL		= 2,	//奇遇日常任务次数已满
	INTERACTION_NO_ENOUGH_ENDURANCE		= 3,	//精力值不够
	INTERACTION_JHM_ERROR				= 4,	//激活码错误
	INTERACTION_WORSHIP					= 5,	//城主膜拜
	INTERACTION_JHM_HAVE				= 6,	//激活码奖励已领取
	INTERACTION_JHM_IS_USED				= 7,	//激活码已使用
	INTERACTION_JHM_NOT_USE_RANGE		= 8,	//不在使用范围内
};

//使用游戏对象操作返回
enum USE_GAMEOBJECT_OPEATE_RESULT
{
	USE_GAMEOBJECT_SUCCEED				= 1,		//使用游戏对象成功
	USE_GAMEOBJECT_FAIL					= 2,		//使用游戏对象失败
	USE_GAMEOBJECT_QUEST				= 3,		//采集成功
	USE_GAMEOBJECT_TOO_FAST				= 4,		//采集速度太快
};

//包裹操作返回
enum BAG_RESULT
{
	BAG_RESULT_NULL						= 1,	//路过的
	BAG_RESULT_ITEM_NOT_EXIST			= 2,	//物品不存在
	BAG_RESULT_CHANGE_ERROR				= 3,	//交换位置错误
	BAG_RESULT_DESTROY_BIND				= 4,	//删除绑定的
	BAG_RESULT_ITEM_NOT_SELF			= 5,	//不是自己的物品
	BAG_RESULT_REPAIR_FULL				= 6,	//修理耐久已满的物品
	BAG_RESULT_BAG_NOT_EXIST			= 7,	//包裹不存在
	BAG_RESULT_POS_NOT_EXIST			= 8,	//包裹位置不存在
	BAG_RESULT_DESTROY_TOOMUCH			= 9,	//数量不够删
	BAG_RESULT_BAN_FOR_TRADE			= 10,	//交易中不允许操作
	BAG_RESULT_SPLIT_FAILURE			= 11,	//物品拆分失败
	BAG_RESULT_LACK_USER				= 12,	//数量不够删
	BAG_RESULT_BAG_FULL					= 13,	//包裹放不下
	BAG_RESULT_MAX_COUNT				= 14,	//超过最大拥有数
	BAG_RESULT_SELL_BIND				= 15,	//定物品不允许出售
	BAG_RESULT_REPAIR_OK				= 16,	//铜钱宝箱开启成功
	BAG_RESULT_REPAIR_MONEY_LACK		= 17,	//钱不够无法修理
	BAG_RESULT_EXTENSION_BAG			= 18,	//包裹不支持扩展
	BAG_RESULT_EXTENSION_MAX_SIZE		= 19,	//包裹大小已经最大
	BAG_RESULT_EXTENSION_MATERIAL_LACK	= 20,	//包裹扩展材料不足
	BAG_RESULT_EXTENSION_TRADE			= 21,	//交易中不允许包裹扩展
	BAG_RESULT_USE_ITEM_COUNT_MAX		= 22,	//使用物品数量达到上限
	BAG_RESULT_NOT_ITEM					= 23,	//没开出物品
	BAG_RESULT_ITEM_USEING				= 24,	//物品使用中
	BAG_RESULT_GET_ITEMS				= 25,	//获得物品（元宝卡）
	BAG_RESULT_USE_ITEMS				= 26,	//使用物品（装备宝箱）
	BAG_RESULT_GET_ITEMS_ORANGE			= 27,	//开启橙色装备
	BAG_RESULT_USE_ITEMS_GENERALS		= 28,	//使用物品（将魂使用）
	BAG_RESULT_GET_ITEMS_CABX			= 29,	//获得 财神宝箱
	BAG_RESULT_GET_ITEMS_CABX_BUFF		= 30,	//获得 财神宝箱的buff 提示
	BAG_RESULT_ALREADY_KAIGUANG 		= 31,	//设备已经开光
	BAG_RESULT_EXCHANGE_SUCCESS			= 32,	//穿上装备成功
	BAG_RESULT_OPEN_ITEMS_SUCCESS		= 33,	//打开物品成功（用于概率获得）
	BAG_RESULT_OPEN_ITEMS_FAIL			= 34,	//打开物品失败（用于概率获得）
	BAG_RESULT_GET_ITEMS_GENERALS_EQUIP	= 35,	//使用物品获得武将装备
	BAG_RESULT_GET_SHOWHAND_JIEZHI		= 36,	//获得梭哈戒指
	BAG_RESULT_USE_WUJIANG_EXP_CARD		= 37,	//使用武将经验卡
	BAG_RESULT_USE_ITEMS_SUCCESS		= 38,	//物品使用成功
	BAG_RESULT_BAG_FULL_SEND_MAIL		= 39,	//包裹已满 发送邮箱提示
	BAG_RESULT_BAG_FULL_AUTO_SELL		= 40,	//包裹已满自动出售
	BAG_RESULT_BAG_XIULIAN_USE			= 41,	//使用修炼丹获得经验
	BAG_RESULT_BAG_SORT_CD				= 42,	//整理功能冷却中

};

//交易操作返回
enum TRADE_RESULT_TYPE
{
	TRADE_RESULT_TYPE_NOT_TARGET		= 1,			//没有交易对象
	TRADE_RESULT_TYPE_TRADEING			= 2,			//已经交易中无法继续处理交易申请
	TRADE_RESULT_TYPE_NOT_TRADE			= 3,			//非交易进行中无效操作
	TRADE_RESULT_TYPE_BAG_SIZE			= 4,			//包裹空间不足交易失败
	TRADE_RESULT_TYPE_UNRECOGNISED		= 5,			//未确认交易物品
	TRADE_RESULT_TYPE_TARGET_CANCEL		= 6,			//对方交已取消交易
	TRADE_RESULT_TYPE_LACK_MONEY		= 7,			//金钱不足
	TRADE_RESULT_TYPE_IS_BIND			= 8,			//绑定物品不允许交易
};

//聊天操作返回
enum CHAT_OPRATE_RESULT
{
	CHAT_RESULT_NOT_PLAYER				= 1,	//找不到玩家
	CHAT_RESULT_IS_GAG					= 2,	//被禁言
	CHAT_LEVEL_WHISPER_LEVEL_NO			= 3,	//私聊等级不足
	CHAT_LEVEL_WORLD_LEVEL_NO			= 4,	//世界聊天等级不足
	CHAT_RESULT_CHECK_LIMIT				= 5,	//您说话太快了，赶紧练级等等再聊天吧…
	CHAT_RESULT_NO_FACTION				= 6,	//你暂无帮派
	CHAT_RESULT_NO_GROUP				= 7,	//你暂无队伍
};

//领取补偿礼包操作返回
enum RECEIVE_GIFT_PACKS_OPEATE_RESULT
{
	RECEIVE_GIFT_PACKS_NOT_FIND			= 1,		//找不到这条补偿礼包
	RECEIVE_GIFT_PACKS_CHAR_ERROR		= 2,		//这礼包不是你的
	RECEIVE_GIFT_PACKS_CHAR_CREATE		= 3,		//礼包指定的发放起始时间在你角色创建时间之前
	RECEIVE_GIFT_PACKS_RECEIVED			= 4,		//该礼包你已经领取过了
	RECEIVE_GIFT_PACKS_ONLINE_AWARD		= 5,		//在线礼包奖励
	RECEIVE_GIFT_PACKS_ONLINE_XIANSHI	= 6,		//在线礼包显示用
};

//任务相关枚举
enum QuestFailedReasons
{
	INVALIDREASON_QUEST_FAILED_LOW_LEVEL        = 1,        // 等级太低 You are not high enough level for that quest.
	INVALIDREASON_QUEST_FAILED_WRONG_RACE       = 2,        // 这个任务对于你的角色类别是无效的 That quest is not available to your race.
	INVALIDREASON_QUEST_ALREADY_DONE            = 3,        // 任务已经完成 You have completed that quest.
	INVALIDREASON_QUEST_ONLY_ONE_TIMED          = 4,       // 任务只能做一次 You can only be on one timed quest at a time.
	INVALIDREASON_QUEST_ALREADY_ON              = 5,       // 已经在任务中 You are already on that quest.
	INVALIDREASON_QUEST_FAILED_EXPANSION        = 6,       // 物品太少 This quest requires an expansion enabled account.
	INVALIDREASON_QUEST_ALREADY_ON2             = 7,       // 已经在任务中 ? You are already on that quest.
	INVALIDREASON_QUEST_FAILED_MISSING_ITEMS    = 8,       // 没有完成任务需要的物品 You don't have the required items with you. Check storage.
	INVALIDREASON_QUEST_FAILED_NOT_ENOUGH_MONEY = 9,       // 没有足够的金钱完成任务 You don't have enough money for that quest.
	INVALIDREASON_DAILY_QUESTS_REMAINING        = 10,       // 你已经完成了每天的25个任务 You have already completed 25 daily quests today.
	INVALIDREASON_QUEST_FAILED_CAIS             = 11,       // 有疲劳时间无法完成任务 You cannot complete quests once you have reached tired time.
	INVALIDREASON_DAILY_QUEST_COMPLETED_TODAY   = 12,       // 已经完成当天的任务 You have completed that daily quest today.
	INVALIDREASON_QUEST_FINISH_ALL_RIHUAN		= 13,		//完成所有日环任务了

	QUEST_TYPE_PROCESS							= 14,		//任务进度, 名字:1/1
};

//购买商品
enum ENpcBuy_Result
{
	NPC_BUY_ITEM_NO_EXIST						= 0,		//物品不存在
	NPC_BUY_ITEM_UNDER							= 1,		//下架的物品
	NPC_BUY_ITEM_NO_OPEN						= 2,		//商品还未到开放时间
	NPC_BUY_ITEM_OUT_TIME						= 3,		//商品已过期
	NPC_BUY_BAG_OUT_SIZE						= 4,		//包裹放不下了
	NPC_BUY_MONEY_NO_ENOUGH						= 5,		//钱不够了
	NPC_BUY_MONEY_TRANSFINITE					= 6,		//购买金额过大
	NPC_BUY_SELL_OUT							= 7,		//超出商品限购次数
};

//副本
enum FUBEN_OPRATE_RESULT
{
	FUBEN_OPRATE_NO_GROUP_STATE				= 0,		//玩家不在组队状态
	FUBEN_OPRATE_NOT_TWO_PEOPLE				= 1,		//队伍不是2个人
	FUBEN_OPRATE_NO_ENERGY					= 2,		//体力不足
	FUBEN_OPRATE_NO_MONEY					= 3,		//元宝不足
	FUBEN_OPRATE_TEAM_NO_LEVEL				= 4,		//队友等级不足
	FUBEN_OPRATE_TEAM_NO_ENERGY				= 5,		//队友体力不足
	FUBEN_OPRATE_NO_GROUP					= 6,		//队伍已经解散
	FUBEN_OPRATE_SEND_TEAM_FRIEND			= 7,		//邀请队友进入组队副本
	FUBEN_OPRATE_TEAM_REFUSE_ENETR			= 8,		//队友拒绝进入组队副本
	FUBEN_OPRATE_TEAM_ON_FUBEN_MAP			= 9,		//队友在副本地图中
	FUBEN_OPRATE_TEAM_HAVE_FLAGS			= 10,		//队友已经发起邀请了
	FUBEN_OPRATE_REFRESH_BOSS				= 11,		//副本刷出boss
	FUBEN_OPRATE_KILLED_BOSS				= 12,		//玩家成功击杀了boss
	FUBEN_OPRATE_MORE_TIMES					= 13,		//全屏秒杀超过最大次数
	FUBEN_OPRATE_NO_CREATURES				= 14,		//全屏秒杀当前没有怪物
};

//跨服
enum KUAFU_OPERATE_RESULT
{
	KUAFU_OPERATE_CONN_DISCONN					= 0,		//游戏服与世界服的链接断开了
	KUAFU_OPERATE_HASNOT_REGISTER				= 1,		//本服还没有向世界服注册成功
};

//排行榜
enum ERankListOperateResult
{
	RANK_LIST_OPERATE_SUCCEED			= 0,	//成功
	RANK_LIST_OPERATE_TYPE_ERROR		= 1,	//查询类型错误
	RANK_LIST_OPERATE_OUT_OF_RANGE		= 2,	//查询范围超出界限
	RANK_LIST_OPERATE_MAX_LIKE			= 4,	//点赞已超过最大次数
	RANK_LIST_OPERATE_HAS_LIKE			= 5,	//已点过赞了

};

enum ActivityOperateResult
{
	ACTIVITY_OPERATE_NOTVIP			= 0, //你不是vip无法领取vip奖励
	ACTIVITY_OPERATE_NOENOUGH		= 1, //活跃度不够无法领取
	ACTIVITY_OPERATE_HASGET			= 2, //已经领取过该奖励
};

enum AchieveOperateResult
{
	ACHIEVE_OPERATE_NO_FIND			= 0, //找不到成就
	ACHIEVE_OPERATE_NO_GET			= 1, //尚未达成成就
	ACHIEVE_OPERATE_HASGET			= 2, //已经领取过成就奖励
	ACHIEVE_OPERATE_NO_ALL			= 3, //总成就不够无法领取
	ACHIEVE_OPERATE_TITLE_SUC		= 4,//设置称号成功
	ACHIEVE_OPERATE_TITLE_FAL		= 5,//称号不存在无法设置
	ACHIEVE_OPERATE_NO_MONEY		= 6,//钱不够了不能找回
};


enum AthleticsOperateResult
{
	ATHLETICS_OPERATE_NO_LEV			= 1, //等级不够{1}，不能参加3v3
	ATHLETICS_OPERATE_NO_FORCE			= 2, //战力不够{1}，不能参加3v3
	ATHLETICS_OPERATE_NO_OPEN			= 3, //不在活动时间，不能参加
	ATHLETICS_OPERATE_NO_TIME_BUY		= 4, //3v3次数已用完，可以购买次数
	ATHLETICS_OPERATE_NO_TIME			= 5, //3v3次数已用完，请明天再来
	ATHLETICS_OPERATE_IN_MATCH			= 6, //正在匹配中
	ATHLETICS_OPERATE_MAX_BUY			= 7, //超出最大购买次数
	ATHLETICS_OPERATE_NO_MONEY			= 8, //超出最大购买次数
	ATHLETICS_OPERATE_HAS_DAY_REWARD	= 9, //已经领取该奖励
	ATHLETICS_OPERATE_NO_DAY_REWARD		= 10, //尚未到达次数{1}

	ATHLETICS_OPERATE_BUFF_OCCUR		= 11,	//buff产生
};

enum DoujianOperateResult
{
	DOUJIAN_OPERATE_NO_LEV			= 0, //等级不够，需要{0}才能参与斗剑台
	DOUJIAN_OPERATE_ZHUCHENG		= 1, //只能在主城中参加活动
	DOUJIAN_OPERATE_NO_TIME			= 2, //挑战次数已满
	DOUJIAN_OPERATE_IN_CD			= 3, //挑战CD中
	DOUJIAN_OPERATE_MAX_TIME		= 4, //超过最大购买次数
	DOUJIAN_OPERATE_NO_MONEY		= 5, //元宝不足购买斗剑次数
	DOUJIAN_OPERATE_NO_MONEY_CD		= 6, //元宝不足不能清理CD

	DOUJIAN_OPERATE_SELF_BATTLE		= 7,	//自己正在被别人挑战
	DOUJIAN_OPERATE_OTHER_BATTLE	= 8,	//挑战的人正在被别人挑战
	DOUJIAN_OPERATE_COUNTDOWN		= 9,	//刷新在冷却中
};

//游戏关闭信息
enum PLAYER_CLOSE_OPERTE_RESULT
{
	PLAYER_CLOSE_OPERTE_APPD_ONE1						= 1000,	//插入单个物品时，包裹id 超过最大值
	PLAYER_CLOSE_OPERTE_APPD_ONE2						= 1001,	//创建玩家其他的对象的时候，发现之前已经创建过了
	PLAYER_CLOSE_OPERTE_APPD_ONE3						= 1002,	//托管到期
	PLAYER_CLOSE_OPERTE_APPD_ONE4						= 1003,	//GM命令 踢人
	PLAYER_CLOSE_OPERTE_APPD_ONE5						= 1004,	//GM命令 回档数据
	PLAYER_CLOSE_OPERTE_APPD_ONE6						= 1005,	// 踢玩家下线
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE4						= 2001,	//帐号信息不完整
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE5						= 2002,	//Char_Create put  fail!
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE6						= 2003,	//登录时候不是我的角色
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE7						= 6,	//待定
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE8						= 2004,	//跨服回来的玩家 m_player 不存在
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE9						= 2005,	//初始化数据库数据失败
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE10					= 2006,	//看看有没有玩家数据需要修复或者升级的
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE11					= 2007,	//重设元宝数量 取数据时失败
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE12					= 2008,	//踢人
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE13					= 2009,	//如果连接不存在,则根据解开的session结果进行创建对应的session实例(on_create_conn_get_session create_sesstion duplicate)
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE14					= 2010,	//客户端连接失败
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE15					= 2011,	//把玩家由pk服回到游戏服,登录失败
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE16					= 2012,	//检测跨服玩家是否可以登陆 -玩家不存在
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE17					= 2013,	//检测跨服玩家是否可以登陆 -超时
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE18					= 2014,	//登陆队列 context->GetGuid().empty(）
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE19					= 2015,	//合服检测 手游下线
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE20					= 2016,	//通知场景服玩家加入地图 m_scened_conn不存在
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE21					= 2017,	//玩家加入地图实例时，玩家binlg不存在 
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE22					= 2018,	//玩家加入地图 创建实例失败
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE23					= 2019,	//如果已登录过 !m_account.empty()
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE24					= 2020,	//帐号名称超长
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE25					= 2021,	//验证是否可以登录本服验证失败
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE26					= 2022,	//账户表数据不存在
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE27					= 2023,	//玩家已在线关闭掉旧链接
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE28					= 2024,	//手游登录，但是还没有玩家角色
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE39					= 2025,	//数据准备完毕，玩家开始登录 玩家不存在
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE40					= 2026,	//获取sessionKey对象,已经登录过
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE41					= 2027,	//获取sessionKey对象，帐户名称，并且判断一下是否超长(超长则失败)
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE42					= 2028,	//获取sessionKey对象，Get_Session: other_data size is wrong
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE43					= 2029,	//获取sessionKey对象，验证session_key是否可以登录这个服务器（为不可登录时候断开）
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE44					= 2030,	//获取sessionKey对象，LogindContext::Get_Session:%s load account fail(获取帐号表数据异常)
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE45					= 2031,	//获取sessionKey对象，LogindContext::Get_Session:%s Get_Chars_List fail（获取角色列表数据异常）
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE46					= 2032,	//跨服读取数据失败
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE47					= 2033,	//运气不好，登录时碰上正在释放自己数据
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE48					= 2034,	//合服检测，从服玩家状态 不为 STATUS_LOGGEDIN
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE49					= 2035,	//封号
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE50					= 2036,	//踢人
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE51					= 2037,	//玩家不存在时关闭连接 LuaPlayerLogin %s, but not found
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE52					= 2038,	//角色在其他地方登陆
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE53					= 2039,	//登陆是无法获得数据
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE54					= 2040,	//登陆时 player login call puts fail %s, fd %u
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE55					= 2041,	//on_create_conn_get_session create_mobile_context duplicate
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE56					= 2042,	//登陆失败
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE57					= 2045,	//Gm命令 @自爆
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE58					= 2043,	//PK服登陆数据异常
	PLAYER_CLOSE_OPERTE_LOGDIN_ONE59					= 2044,	//从数据库读取玩家数据失败
	PLAYER_CLOSE_OPERTE_LOGDIN_2046						= 2046,	//一个角色对应两个账号	
	PLAYER_CLOSE_OPERTE_LOGDIN_2047						= 2047,	//改名字的时候找不到account，刷新就可以过了
	PLAYER_CLOSE_OPERTE_LOGDIN_2048						= 2048,	//session的account不等于player的account,简单来说,串号了.
	PLAYER_CLOSE_OPERTE_SCREND_ONE29					= 3001,	//停止移动，地图模板为null
	PLAYER_CLOSE_OPERTE_SCREND_ONE30					= 3002,	//对象移动，地图模板为null
	PLAYER_CLOSE_OPERTE_SCREND_ONE31					= 3003,	//对象移动的时候  状态不为 STATUS_LOGGEDIN  时候 （assert error: Handle_Unit_Move  ASSERT(status_ == STATUS_LOGGEDIN)）
	PLAYER_CLOSE_OPERTE_SCREND_ONE32					= 3004,	//传送是 状态不为 STATUS_LOGGEDIN （ScenedContext::Teleport status_ != STATUS_LOGGEDIN）
	PLAYER_CLOSE_OPERTE_SCREND_ONE33					= 3005,	//如果玩家未正确的加入地图，这个时间传送也是失败的
	PLAYER_CLOSE_OPERTE_SCREND_ONE34					= 3006,	//任务链条中断 assert error: Handle_QuestGiver_Status_Query  ASSERT(questGetter)
	PLAYER_CLOSE_OPERTE_SCREND_ONE35					= 3007,	//玩家被杀死时，地图数据不存在 Player::OnKilled ASSERT(GetMap())
	PLAYER_CLOSE_OPERTE_SCREND_ONE36					= 3008,	//加入地图宠物不存在的时候  Player::OnJoinMap ASSERT(m_pets.empty())
	PLAYER_CLOSE_OPERTE_SCREND_ONE37					= 3009,	//任务管理者不存在的时候 Player::Create ASSERT(!m_questMgr)
	PLAYER_CLOSE_OPERTE_SCREND_ONE38					= 3010,	//传送超时1分钟
	PLAYER_CLOSE_OPERTE_POLICED_4001					= 4001,	//在日志服重复登录
};

enum FACTION_OPERTE_RESULT
{
	OPERTE_TYPE_FACTION_IS_HAVE						= 0,//你已经在帮派中
	OPERTE_TYPE_FACTION_NAME_ERR					= 1,//帮派名称超过6个字符
	OPERTE_TYPE_FACTION_NAME_HAVE_FUCK				= 2,//帮派名称中有非法字符
	OPERTE_TYPE_FACTION_NAME_REPEAT					= 3,//帮派名称重复
	OPERTE_TYPE_FACTION_CREATE_MAX					= 4,//帮派数量达到上限
	OPERTE_TYPE_FACTION_CREATE_COST					= 5,//元宝不足无法创建帮派
	OPERTE_TYPE_FACTION_NOT_MANAGER					= 6,//不是帮主或副帮主无法编辑
	OPERTE_TYPE_FACTION_NOTICE_ERR					= 7,//帮派公告超过144个字符
	OPERTE_TYPE_FACTION_NOTICE_ERR_PB				= 8,//帮派公告包含非法字符
	OPERTE_TYPE_FACTION_LEV_LOW						= 9,//等级不够，不能加入帮派
	OPERTE_TYPE_FACTION_MEMBER_MAX_COUNT			= 10,//帮派人数已满无法加入
	OPERTE_TYPE_FACTION_JOIN_SUCESS					= 11,//恭喜你加入帮派
	OPERTE_TYPE_FACTION_FACTION_REFUSED_JOIN		= 12,//你被拒绝加入帮派
	OPERTE_TYPE_FACTION_KICKED						= 13,//你被移出帮派
	OPERTE_TYPE_FACTION_KICK_SUCCESS				= 14,//踢出帮派成员
	OPERTE_TYPE_FACTION_MAX_ZHIWEI					= 15,//职位人数已满
	OPERTE_TYPE_FACTION_APPOINT_SUCCESS				= 16,//任命成功
	OPERTE_TYPE_FACTION_PROMOTED					= 17,//你被任命为
	OPERTE_TYPE_FACTION_APPOINT_NOSELF				= 18,//自己不能任命自己 
	OPERTE_TYPE_FACTION_DONATIONMAX					= 19,//捐献次数超过最大次数
	OPERTE_TYPE_FACTION_DONATION_GOLD				= 20,//捐献所需金币不够
	OPERTE_TYPE_FACTION_DONATION_YB					= 21,//捐献所需元宝不够
	OPERTE_TYPE_FACTION_SHOP_NUMLOW					= 22,//商品剩余数量不足无法购买
	OPERTE_TYPE_FACTION_DEVOTE						= 23,//帮贡不足
	OPEATE_TYPE_FACTION_MONEY_ERR					= 24,//帮派资金不足
	OPEATE_TYPE_FACTION_LEVEL_UP					= 25,//帮派升级成功
	OPEATE_TYPE_FACTION_NOT_JOIN					= 26,//没有可以加入的帮派
	OPERTE_TYPE_FACTION_PLAYER_OFFLINE				= 40,//对方不在线,无法加入成员

	OPERTE_TYPE_FACTION_BOSS_START					= 50,//召唤boss

	OPERTE_TYPE_FACTION_GIFT_THANK					= 59,//感谢完成
	OPERTE_TYPE_FACTION_GIFT_THANK_ALL					= 60,//全部感谢完成
	OPERTE_TYPE_FACTION_GIFT_GET_EXREWARD					= 61,//领取成功
	OPERTE_TYPE_FACTION_GIFT_GET_EXREWARD_ALL					= 62,//全部领取成功
	OPERTE_TYPE_FACTION_GIFT_SEND					= 63,//赠送成功
	OPERTE_TYPE_FACTION_GIFT_SEND_FAIL					= 64,//赠送成功

	OPERTE_TYPE_FACTION_GIFT_NOTICE					= 65, //礼物广播

	OPERTE_TYPE_FACTION_BOSSDEFENSE_TIME_ERROR		= 66, //非法时间 无法挑战boss

};

enum SOCIAL_OPERTE_RESULT
{
	OPERTE_TYPE_SOCIAL_HAS_SEND						= 1,//好友请求信息已发送
	OPERTE_TYPE_SOCIAL_NOT_FIND						= 2,//角色不存在或者角色不在线
	OPERTE_TYPE_SOCIAL_SELF_FULL					= 3,//自己好友列表已满
	OPERTE_TYPE_SOCIAL_TARGET_FULL					= 4,//对方好友列表已满
	OPERTE_TYPE_SOCIAL_ALREADY_FRIEND				= 5,//对方已经是你的好友
	OPERTE_TYPE_SOCIAL_ADD_MYSELF					= 6,//不能自己添加自己为好友
	OPERTE_TYPE_SOCIAL_HAS_SEND_ADD					= 7,//已经发送申请
	OPERTE_TYPE_SOCIAL_NO_MONEY						= 8,//元宝或者金币不足
	OPERTE_TYPE_SOCIAL_NOT_ENEMY					= 9,//对方不是仇人
	OPERTE_TYPE_SOCIAL_ENEMY_OFFLINE				= 10,//对方处于离线中
	OPERTE_TYPE_SOCIAL_ENEMY_NOT_IN_FIELD			= 11,//对方不在野外
	OPERTE_TYPE_SOCIAL_REVENGE_TIMES_NOT_ENOUGH		= 12,//复仇传送次数不足
	OPERTE_TYPE_SOCIAL_RENAME_SUCCESS				= 13,//改名成功
	OPERTE_TYPE_SOCIAL_OTHNER_FACTION				= 14,//对方已在帮派
	OPERTE_TYPE_SOCIAL_SAME_FACTION					= 15,//对方已在同帮派

};

//托管
enum HOSTING_OPERTE_RESULT
{
	HOSTING_OPERTE_IS_SELF							= 0,	//无法对自己操作
	HOSTING_OPERTE_LEVEL_NOT						= 1,	//等级不足
	HOSTING_OPERTE_IS_HOSTING						= 2,	//托管中
	HOSTING_OPERTE_IS_HOSTING_APPLY					= 3,	//托管申请
	HOSTING_OPERTE_FRIEND_NOT						= 4,	//好友不存在
	HOSTING_OPERTE_HAS_NOT_24H						= 5,	//还没到24小时
	HOSTING_OPERTE_FRIEND_OUTLINE					= 6,	//好友不在线
	HOSTING_OPERTE_FRIEND_ERR						= 7,	//托管好友错误
	HOSTING_OPERTE_NOT_HOSTING						= 8,	//不在托管中
	HOSTING_OPERTE_FRIEND_ONLINE					= 9,	//好友在线
	HOSTING_OPERTE_LOGIN_HOSTING					= 10,	//托管登录中
	HOSTING_OPERTE_FRIEND_APPLY						= 11,	//申请发送
	HOSTING_OPERTE_LOGIN							= 12,	//登录
	HOSTING_OPERTE_SUCCESS							= 13,	//托管申请成功
	HOSTING_OPERTE_CANCEL							= 14,	//取消托管
	HOSTING_OPERTE_REFUSED							= 15,	//拒绝托管
	HOSTING_OPERTE_NOT_DUE_TO						= 16,	//托管未到期
	HOSTING_OPERTE_FRIEND_HOSTING_LOGIN				= 17,	//对方托管登录中，无法操作
	HOSTING_OPERTE_FRIEND_NOT_HAS_YOU				= 18,	//对方好友列表没有你
};

//强化
enum STRENGTH_OPERTE_RESULT {
	STRENGTH_OPERTE_FAIL							= 0		//强化失败
};

//切换战斗模式
enum BATTLE_MODE_OPERTE_RESULT
{
	BATTLE_MODE_OPERTE_PEACE_MODE_DENY				= 0,	//战斗中不能切换和平模式
	BATTLE_MODE_OPERTE_PEACE_MODE_IN_CD				= 1,	//和平模式在CD中
};


// 野外BOSS
enum FIELD_BOSS_OPERTE_RESULT
{
	FIELD_BOSS_OPERTE_WILL_START = 1,	//野外BOSS即将在{1}分钟后开启, 请大家安排好时间
	FIELD_BOSS_OPERTE_WILL_OCCUR = 2,	//野外boss即将出现，请各位大侠做好准备
	FIELD_BOSS_OPERTE_OCCUR		 = 3,	//野外boss已经在各个地图刷新，请各位大侠火速前往击杀 查看详情
	FIELD_BOSS_OPERTE_BOSS_BORN  = 4,	//{1}已出现在{2}，请各位大侠火速前往击杀 立即前往
	FIELD_BOSS_OPERTE_BOSS_KILL  = 5,	//{1}已被成功击杀！恭喜{2}获得{3}1分钟的优先开启权利
	FIELD_BOSS_OPERTE_PROTECT	 = 6,	//九龙宝箱保护时间结束，静待各位大侠开启
	FIELD_BOSS_OPERTE_PICKED	 = 7,	//恭喜{1}最终开启九龙宝箱，获得XXXX
	FIELD_BOSS_OPERTE_TOO_FAR	 = 8,	//太远了，你不可能开启这个宝箱
};

// 世界BOSS
// OPERTE_TYPE_WORLD_BOSS
enum WORLD_BOSS_OPERTE_RESULT
{
	WORLD_BOSS_OPERTE_NOT_ENROLL = 0,	//本次活动您未报名，无法参加
	WORLD_BOSS_OPERTE_WILL_START = 1,	//世界boss即将开启，请各位大侠做好准备
	WORLD_BOSS_OPERTE_WILL_ROLL1 = 2,	//恭喜{1}赢得本次拼点奖励，BOSS在3秒后恢复正常
	WORLD_BOSS_OPERTE_WILL_ROLL2 = 3,	//恭喜{1}赢得本次拼点奖励，现在可自由离开战场
	WORLD_BOSS_OPERTE_ENROLL	 = 4,	//世界BOSS开始报名
};

// 坐骑骑乘
// OPRATE_TYPE_MOUNT_QICHENG
enum MOUNT_QICHENG_OPERTE_RESULT
{
	MOUNT_QICHENG_FIGHT			= 0,	//战斗状态不能骑乘
	MOUNT_QICHENG_JUMP			= 1,	//跳跃状态不能骑乘
};


// 传送
//OPRATE_TYPE_TELEPORT
enum TELEPORT_OPERTE_RESULT
{
	TELEPORT_OPRATE_PVP_STATE	= 0,	//pvp状态不能传送
};

// 升级操作
//OPRATE_TYPE_UPGRADE
enum UPGRADE_OPRATE_RESULT
{
	UPGRADE_OPRATE_SKILL_SUCCESS	= 0,	//技能升级成功
	UPGRADE_OPRATE_MOUNT_EXP		= 1,	//坐骑经验+{1}
	UPGRADE_OPRATE_MOUNT_STAR		= 2,	//坐骑星级+1
	UPGRADE_OPRATE_MOUNT			= 3,	//进阶成功
	UPGRADE_OPRATE_ANGLE_SKILL		= 4,	//怒气技能进阶成功
	UPGRADE_OPRATE_BLESS_SUCCESS		= 5 ,   //部位升级成功
	UPGRADE_OPRATE_BLESS_EXP			= 6,	//部位经验+{1}
	UPGRADE_OPRATE_GEM_SUCCESS		= 7 ,   //宝石激活成功
	UPGRADE_OPRATE_GEM_EXP			= 8,	//宝石经验+{1}
	UPGRADE_OPRATE_TALISMAN_ACTIVE	= 9 ,   //法宝激活成功
	UPGRADE_OPRATE_TALISMAN_LVUP	= 10,	//法宝注灵
	UPGRADE_OPRATE_WINGS_ACTIVE = 11,		//神羽激活成功
	UPGRADE_OPRATE_WINGS_BLESS = 12,		//神羽祝福成功
	UPGRADE_OPRATE_WINGS_RANKUP = 13,		//神羽升阶成功
	UPGRADE_OPRATE_WINGS_STRENGTH_SUCESS = 14,	//神羽强化成功
	UPGRADE_OPRATE_WINGS_STRENGTH_FAIL = 15,	//神羽强化失败
	UPGRADE_OPRATE_EQUIPDEVELOP_STRENGTH_SUCCESS = 16, //装备部位强化成功
	UPGRADE_OPRATE_EQUIPDEVELOP_REFINE_STAR_SUCCESS = 17, //装备部位升星成功
	UPGRADE_OPRATE_EQUIPDEVELOP_REFINE_STAR_FAIL = 18, //装备部位升星失败
	UPGRADE_OPRATE_EQUIPDEVELOP_REFINE_RANK_SUCCESS = 19, //装备部位升阶成功
	UPGRADE_OPRATE_EQUIPDEVELOP_REFINE_RANK_FAIL = 20, //装备部位升阶失败
	UPGRADE_OPRATE_EQUIPDEVELOP_GEM_ACTIVE_SUCCESS = 21, //装备宝石激活成功
	UPGRADE_OPRATE_EQUIPDEVELOP_GEM_LVUP_SUCCESS = 22, //装备宝石强化成功
	UPGRADE_OPRATE_EQUIPDEVELOP_WASHATTRS_WASH	= 23, //装备洗炼完成
	UPGRADE_OPRATE_EQUIPDEVELOP_WASHATTRS_SAVE	= 24, //装备洗炼保存
	UPGRADE_OPRATE_EQUIPDEVELOP_BONUS_STRENGTH	= 25, //装备强化奖励
	UPGRADE_OPRATE_EQUIPDEVELOP_BONUS_REFINE	= 26, //装备精炼奖励
	UPGRADE_OPRATE_EQUIPDEVELOP_BONUS_GEM	= 27, //装备镶嵌奖励

	UPGRADE_OPRATE_FACTIONSKILL_LVUP = 28, //家族技能升级成功

	UPGRADE_OPRATE_MOUNT_ILLUSION_ACTIVE = 29, //坐骑皮肤激活成功
	UPGRADE_OPRATE_CLOTH_ILLUSION_ACTIVE = 30, //衣服外观激活成功
	UPGRADE_OPRATE_WEAPON_ILLUSION_ACTIVE = 31, //武器外观激活成功
	UPGRADE_OPRATE_TITLE_ACTIVE = 32, //称号激活成功
	UPGRADE_OPRATE_MOUNT_ILLUSION_SET = 33, //坐骑幻化成功
	UPGRADE_OPRATE_MOUNT_ILLUSION_UNSET = 34, //坐骑取消幻化
	UPGRADE_OPRATE_FASHION_ILLUSION_SET = 35, //外观装备成功
	UPGRADE_OPRATE_FASHION_ILLUSION_UNSET = 36, //外观取消装备
	UPGRADE_OPRATE_MOUNT_LEVEL_UP = 37, //坐骑升级成功

};

//仙府夺宝
//OPRATE_TYPE_XIANFU
enum XIANFU_TYPE
{
	XIANFU_TYPE_BOSS_OCCUR		= 0,	//{1}已在{2}点出现, 击杀可掉落{3}
	XIANFU_TYPE_BOSS_KILL		= 1,	//{1}被{2}击杀, 掉落{3}个宝箱
};

// 奖励来源对应的不同的提示信息
enum REWARD_TYPE
{
	REWARD_TYPE_ACTIVITY				= 0,		//活动奖励获得
	REWARD_TYPE_RECHARGE				= 1,		//充值赠送获得
	REWARD_TYPE_GIFT_PACKS				= 2,		//补偿礼包获得
	REWARD_TYPE_QUEST					= 3,		//任务完成奖励获得
	REWARD_TYPE_NEWPLAYER				= 4,		//新手礼包获得
	REWARD_TYPE_OPEN_ITEM				= 5,		//使用道具获得
	REWARD_TYPE_LOOT					= 6,		//战利品
	REWARD_TYPE_GM_COMMAND				= 7,		//GM命令获得
	REWARD_TYPE_ITEM_BLEND				= 8,		//物品合成获得
	REWARD_TYPE_SEVEN_DAY_GIFT			= 9,		//连登礼包	
	REWARD_TYPE_JHM_GIFT				= 10,		//激活码礼包
	REWARD_TYPE_OPEN_BOX				= 11,		//开宝箱获得
	REWARD_TYPE_FUBEN_EWAIJINAGLI		= 12,		//副本额外奖励
	REWARD_TYPE_CAPTURE_YAOHUN			= 13,		//扑捉妖魂获得
	REWARD_TYPE_FSZL_EWAIJINAGLI		= 14,		//飞升之路奖励
	REWARD_TYPE_PRESTIGE				= 15,		//活跃度领取获得
	REWARD_TYPE_TIANDILINGKUANG			= 16,		//天地灵矿领取获得
	REWARD_TYPE_PANTAOYUAN				= 17,		//蟠桃园奖励
	REWARD_TYPE_WABAO					= 18,		//挖宝获得
	REWARD_TYPE_HONGYAN					= 19,		//红颜事件获得
	REWARD_TYPE_FORGE_DECOMPOSE			= 20,		//锻造分解获得
	REWARD_TYPE_WORLD_BOSS_ROLL			= 21,		//世界BOSSroll点
	REWARD_TYPE_VIP_INSTANCE_REWARD		= 22,		//vip副本奖励
	REWARD_TYPE_TRIAL_INSTANCE_REWARD	= 23,		//试炼塔奖励
	REWARD_TYPE_ACHIEVE					= 24,		//成就奖励
	REWARD_TYPE_SHOUCHONG				= 25,		//首充奖励
	REWARD_TYPE_CHECKIN					= 26,		//每日签到奖励
	REWARD_TYPE_GETBACK					= 27,		//找回次数奖励
	REWARD_TYPE_TOTAL_CHECKIN			= 28,		//每日累积签到奖励
	REWARD_TYPE_WELFARE_LEVEL			= 29,		//福利等级奖励
	REWARD_TYPE_3V3_KUAFU				= 30,		//3v3跨服奖励
	REWARD_TYPE_INSTANCE_SWEEP			= 31,		//副本奖励扫荡奖励
	REWARD_TYPE_XIANFU					= 32,		//仙府夺宝跨服奖励
};

enum GEM_TYPE
{
	GEM_TYPE_CRIT				= 0,		//宝石暴击
};

#endif //_OPRATE_RESULT_H
