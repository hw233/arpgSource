
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------协议，本代码是自动生成，请勿手工改动----------------------------------------------------------------------------
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
local Packet = require 'util.packet'
local external_send = packet and packet.external_send

local Protocols = {}
---------------------------------------------
--以下为连接的状态
STATUS_NEVER = 1
STATUS_AUTHED = 2
STATUS_LOGGEDIN = 4
STATUS_TRANSFER = 8

---------------------------------------------
--协议枚举
MSG_NULL_ACTION		= 0	-- /*无效动作*/	
MSG_PING_PONG		= 1	-- /*测试连接状态*/	
CMSG_FORCED_INTO		= 2	-- /*踢掉在线的准备强制登陆*/	
CMSG_GET_SESSION		= 3	-- /*获得Session对象*/	
MSG_ROUTE_TRACE		= 4	-- /*网关服数据包路由测试*/	
CMSG_WRITE_CLIENT_LOG		= 5	-- /*记录客户端日志*/	
SMSG_OPERATION_FAILED		= 6	-- /*操作失败*/	
MSG_SYNC_MSTIME		= 7	-- /*同步时间*/	
SMSG_UD_OBJECT		= 8	-- /*对象更新*/	
CMSG_UD_CONTROL		= 9	-- /*对象更新控制协议*/	
SMSG_UD_CONTROL_RESULT		= 10	-- /*对象更新控制协议结果*/	
SMSG_GRID_UD_OBJECT		= 11	-- /*GRID的对象更新*/	
SMSG_GRID_UD_OBJECT_2		= 12	-- /*GRID的对象更新*/	
SMSG_LOGIN_QUEUE_INDEX		= 13	-- /*告诉客户端，目前自己排在登录队列的第几位*/	
SMSG_KICKING_TYPE		= 14	-- /*踢人原因*/	
CMSG_GET_CHARS_LIST		= 15	-- /*获取角色列表*/	
SMSG_CHARS_LIST		= 16	-- /*角色列表*/	
CMSG_CHECK_NAME		= 17	-- /*检查名字是否可以使用*/	
SMSG_CHECK_NAME_RESULT		= 18	-- /*检查名字结果*/	
CMSG_CHAR_CREATE		= 19	-- /*创建角色*/	
SMSG_CHAR_CREATE_RESULT		= 20	-- /*角色创建结果*/	
CMSG_DELETE_CHAR		= 21	-- /*删除角色*/	
SMSG_DELETE_CHAR_RESULT		= 22	-- /*角色删除结果*/	
CMSG_PLAYER_LOGIN		= 23	-- /*玩家登录*/	
CMSG_PLAYER_LOGOUT		= 24	-- /*玩家退出*/	
CMSG_REGULARISE_ACCOUNT		= 25	-- /*临时账号转正规*/	
CMSG_CHAR_REMOTESTORE		= 26	-- /*角色配置信息*/	
CMSG_CHAR_REMOTESTORE_STR		= 27	-- /*角色配置信息*/	
CMSG_TELEPORT		= 28	-- /*传送，如果是C->S，mapid变量请填成传送点ID*/	
MSG_MOVE_STOP		= 29	-- /*停止移动*/	
MSG_UNIT_MOVE		= 30	-- /*unit对象移动*/	
CMSG_USE_GAMEOBJECT		= 31	-- /*使用游戏对象*/	
CMSG_BAG_EXCHANGE_POS		= 32	-- /*包裹操作-交换位置*/	
CMSG_BAG_DESTROY		= 33	-- /*包裹操作-销毁物品*/	
CMSG_BAG_ITEM_SPLIT		= 34	-- /*分割物品*/	
CMSG_BAG_ITEM_USER		= 35	-- /*使用物品*/	
SMSG_BAG_ITEM_COOLDOWN		= 36	-- /*下发物品冷却*/	
SMSG_GRID_UNIT_MOVE		= 37	-- /*grid中的unit移动整体打包*/	
SMSG_GRID_UNIT_MOVE_2		= 38	-- /*grid中的unit移动整体打包2*/	
CMSG_EXCHANGE_ITEM		= 39	-- /*兑换物品*/	
CMSG_BAG_EXTENSION		= 40	-- /*背包扩展*/	
CMSG_NPC_GET_GOODS_LIST		= 41	-- /*请求NPC商品列表*/	
SMSG_NPC_GOODS_LIST		= 42	-- /*Npc商品列表*/	
CMSG_STORE_BUY		= 43	-- /*购买商品*/	
CMSG_NPC_SELL		= 44	-- /*出售物品*/	
CMSG_NPC_REPURCHASE		= 45	-- /*回购物品*/	
CMSG_AVATAR_FASHION_ENABLE		= 46	-- /*时装是否启用*/	
CMSG_QUESTHELP_TALK_OPTION		= 47	-- /*任务对话选项*/	
CMSG_TAXI_HELLO		= 48	-- /*与NPC对话获得传送点列表*/	
SMSG_TAXI_STATIONS_LIST		= 49	-- /*发送传送点列表*/	
CMSG_TAXI_SELECT_STATION		= 50	-- /*选择传送点*/	
CMSG_GOSSIP_SELECT_OPTION		= 51	-- /*与NPC交流选择选项*/	
CMSG_GOSSIP_HELLO		= 52	-- /*与NPC闲聊获取选项*/	
SMSG_GOSSIP_MESSAGE		= 53	-- /*发送闲聊信息和选项*/	
CMSG_QUESTGIVER_STATUS_QUERY		= 54	-- /*任务发布者状态查询*/	
SMSG_QUESTGIVER_STATUS		= 55	-- /*任务NPC状态*/	
MSG_QUERY_QUEST_STATUS		= 56	-- /*查询任务状态*/	
CMSG_QUESTHELP_GET_CANACCEPT_LIST		= 57	-- /*可接任务*/	
SMSG_QUESTHELP_CANACCEPT_LIST		= 58	-- /*下发可接任务列表*/	
SMSG_QUESTUPDATE_FAILD		= 59	-- /*任务失败*/	
SMSG_QUESTUPDATE_COMPLETE		= 60	-- /*任务条件完成*/	
CMSG_QUESTLOG_REMOVE_QUEST		= 61	-- /*放弃任务*/	
CMSG_QUESTGIVER_COMPLETE_QUEST		= 62	-- /*完成任务*/	
SMSG_QUESTHELP_NEXT		= 63	-- /*完成任务后通知任务链的下个任务*/	
CMSG_QUESTHELP_COMPLETE		= 64	-- /*任务系统强制完成任务*/	
SMSG_QUESTUPDATE_ACCEPT		= 65	-- /*接受任务成功*/	
CMSG_QUESTHELP_UPDATE_STATUS		= 66	-- /*更新任务进度_下标数量*/	
SMSG_QUESTGETTER_COMPLETE		= 67	-- /*任务已完成*/	
CMSG_QUESTGIVER_ACCEPT_QUEST		= 68	-- /*接任务*/	
CMSG_QUESTUPDATE_USE_ITEM		= 69	-- /*任务使用物品*/	
CMSG_QUESTHELP_QUERY_BOOK		= 70	-- /*查询天书任务*/	
SMSG_QUESTHELP_BOOK_QUEST		= 71	-- /*下发可接天书任务*/	
SMSG_USE_GAMEOBJECT_ACTION		= 72	-- /*玩家使用游戏对象以后的动作*/	
CMSG_SET_ATTACK_MODE		= 73	-- /*设置攻击模式*/	
MSG_SELECT_TARGET		= 74	-- /*选择目标*/	
SMSG_COMBAT_STATE_UPDATE		= 75	-- /*进入战斗*/	
SMSG_EXP_UPDATE		= 76	-- /*经验更新*/	
MSG_SPELL_START		= 77	-- /*客户端释放技能*/	
MSG_SPELL_STOP		= 78	-- /*施法停止*/	
MSG_JUMP		= 79	-- /*跳*/	
CMSG_RESURRECTION		= 80	-- /*复活*/	
MSG_TRADE_REQUEST		= 81	-- /*交易发出请求*/	
MSG_TRADE_REPLY		= 82	-- /*交易请求答复*/	
SMSG_TRADE_START		= 83	-- /*交易开始*/	
MSG_TRADE_DECIDE_ITEMS		= 84	-- /*交易确认物品*/	
SMSG_TRADE_FINISH		= 85	-- /*交易完成*/	
MSG_TRADE_CANCEL		= 86	-- /*交易取消*/	
MSG_TRADE_READY		= 87	-- /*交易准备好*/	
SMSG_CHAT_UNIT_TALK		= 88	-- /*生物讲话*/	
CMSG_CHAT_NEAR		= 89	-- /*就近聊天*/	
CMSG_CHAT_WHISPER		= 90	-- /*私聊*/	
MSG_CHAT_FACTION		= 91	-- /*阵营聊天*/	
MSG_CHAT_WORLD		= 92	-- /*世界*/	
MSG_CHAT_HORN		= 93	-- /*喇叭*/	
MSG_CHAT_NOTICE		= 94	-- /*公告*/	
CMSG_QUERY_PLAYER_INFO		= 95	-- /*查询玩家信息*/	
SMSG_QUERY_RESULT_UPDATE_OBJECT		= 96	-- /*查询信息对象更新*/	
CMSG_RECEIVE_GIFT_PACKS		= 97	-- /*领取礼包*/	
SMSG_MAP_UPDATE_OBJECT		= 98	-- /*地图对象更新*/	
SMSG_FIGHTING_INFO_UPDATE_OBJECT		= 99	-- /*战斗信息binlog*/	
SMSG_FIGHTING_INFO_UPDATE_OBJECT_2		= 100	-- /*战斗信息binlog*/	
CMSG_INSTANCE_ENTER		= 101	-- /*进入副本*/	
CMSG_INSTANCE_NEXT_STATE		= 102	-- /*向服务端发送副本进入下一阶段指令*/	
CMSG_INSTANCE_EXIT		= 103	-- /*副本退出*/	
CMSG_LIMIT_ACTIVITY_RECEIVE		= 104	-- /*限时活动领取*/	
SMSG_KILL_MAN		= 105	-- /*杀人啦~~！！！！*/	
SMSG_PLAYER_UPGRADE		= 106	-- /*玩家升级*/	
CMSG_WAREHOUSE_SAVE_MONEY		= 107	-- /*仓库存钱*/	
CMSG_WAREHOUSE_TAKE_MONEY		= 108	-- /*仓库取钱*/	
CMSG_USE_GOLD_OPT		= 109	-- /*使用元宝操作某事*/	
CMSG_USE_SILVER_OPT		= 110	-- /*使用铜钱操作某事*/	
SMSG_GM_RIGHTFLOAT		= 111	-- /*后台弹窗*/	
SMSG_DEL_GM_RIGHTFLOAT		= 112	-- /*删除某条后台弹窗*/	
MSG_SYNC_MSTIME_APP		= 113	-- /*应用服同步时间*/	
CMSG_OPEN_WINDOW		= 114	-- /*玩家打开某个窗口*/	
CMSG_PLAYER_GAG		= 115	-- /*禁言操作*/	
CMSG_PLAYER_KICKING		= 116	-- /*踢人操作*/	
SMSG_MERGE_SERVER_MSG		= 117	-- /*合服通知*/	
CMSG_RANK_LIST_QUERY		= 118	-- /*获取排行信息*/	
SMSG_RANK_LIST_QUERY_RESULT		= 119	-- /*客户端获取排行榜返回结果*/	
CMSG_CLIENT_UPDATE_SCENED		= 120	-- /*客户端热更场景模块后获取uint*/	
SMSG_NUM_LUA		= 121	-- /*数值包*/	
CMSG_LOOT_SELECT		= 122	-- /*战利品拾取*/	
CMSG_GOBACK_TO_GAME_SERVER		= 123	-- /*通知登录服把玩家传回游戏服*/	
CMSG_WORLD_WAR_CS_PLAYER_INFO		= 124	-- /*客户端把比赛人员数据传给比赛服*/	
SMSG_JOIN_OR_LEAVE_SERVER		= 125	-- /*玩家加入或者离开某服务器*/	
MSG_WORLD_WAR_SC_PLAYER_INFO		= 126	-- /*客户端请求跨服人员数据*/	
MSG_CLIENTSUBSCRIPTION		= 127	-- /*客户端订阅信息*/	
SMSG_LUA_SCRIPT		= 128	-- /*服务端下发lua脚本*/	
CMSG_CHAR_UPDATE_INFO		= 129	-- /*角色更改信息*/	
SMSG_NOTICE_WATCHER_MAP_INFO		= 130	-- /*通知客户端观察者的视角*/	
CMSG_MODIFY_WATCH		= 131	-- /*客户端订阅对象信息*/	
CMSG_KUAFU_CHUANSONG		= 132	-- /*跨服传送*/	
CMSG_SHOW_SUIT		= 133	-- /*显示当前装备*/	
CMSG_SHOW_POSITION		= 134	-- /*显示当前坐标*/	
CMSG_GOLD_RESPAWN		= 135	-- /*元宝复活*/	
SMSG_FIELD_DEATH_COOLDOWN		= 136	-- /*野外死亡倒计时*/	
CMSG_MALL_BUY		= 137	-- /*商城购买*/	
CMSG_STRENGTH		= 139	-- /*强化*/	
SMSG_STRENGTH_SUCCESS		= 140	-- /*强化成功*/	
CMSG_FORCEINTO		= 141	-- /*强制进入*/	
CMSG_CREATE_FACTION		= 142	-- /*创建帮派*/	
CMSG_FACTION_UPGRADE		= 143	-- /*升级帮派*/	
CMSG_FACTION_JOIN		= 144	-- /*申请加入帮派*/	
CMSG_RAISE_BASE_SPELL		= 145	-- /*申请升级技能*/	
CMSG_UPGRADE_ANGER_SPELL		= 146	-- /*申请升阶愤怒技能*/	
CMSG_RAISE_MOUNT		= 147	-- /*申请升级坐骑*/	
CMSG_UPGRADE_MOUNT		= 148	-- /*申请升阶坐骑*/	
CMSG_UPGRADE_MOUNT_ONE_STEP		= 149	-- /*申请一键升阶坐骑*/	
CMSG_ILLUSION_MOUNT_ACTIVE		= 150	-- /*申请解锁幻化坐骑*/	
CMSG_ILLUSION_MOUNT		= 151	-- /*申请幻化坐骑*/	
CMSG_RIDE_MOUNT		= 152	-- /*申请骑乘*/	
SMSG_GRID_UNIT_JUMP		= 153	-- /*grid中的unit跳跃*/	
CMSG_GEM		= 154	-- /*宝石*/	
CMSG_CHANGE_BATTLE_MODE		= 155	-- /*请求切换模式*/	
SMSG_PEACE_MODE_CD		= 156	-- /*和平模式CD*/	
CMSG_DIVINE_ACTIVE		= 157	-- /*激活神兵*/	
CMSG_DIVINE_UPLEV		= 158	-- /*激活神兵*/	
CMSG_DIVINE_SWITCH		= 159	-- /*切换神兵*/	
CMSG_JUMP_START		= 160	-- /*请求跳跃*/	
CMSG_ENTER_VIP_INSTANCE		= 161	-- /*请求进入vip副本*/	
CMSG_SWEEP_VIP_INSTANCE		= 162	-- /*请求扫荡vip副本*/	
CMSG_HANG_UP		= 163	-- /*进行挂机*/	
CMSG_HANG_UP_SETTING		= 164	-- /*进行挂机设置*/	
CMSG_ENTER_TRIAL_INSTANCE		= 165	-- /*请求进入试炼塔副本*/	
CMSG_SWEEP_TRIAL_INSTANCE		= 166	-- /*扫荡试炼塔副本*/	
CMSG_RESET_TRIAL_INSTANCE		= 167	-- /*重置试炼塔*/	
SMSG_SWEEP_INSTANCE_REWARD		= 168	-- /*扫荡副本奖励*/	
CMSG_REENTER_INSTANCE		= 169	-- /*重进副本*/	
SMSG_MERRY_GO_ROUND		= 170	-- /*走马灯信息*/	
CMSG_SOCIAL_ADD_FRIEND		= 171	-- /*添加好友*/	
CMSG_SOCIAL_SUREADD_FRIEND		= 172	-- /*同意添加好友*/	
CMSG_SOCIAL_GIFT_FRIEND		= 173	-- /*赠送礼物*/	
CMSG_SOCIAL_RECOMMEND_FRIEND		= 174	-- /*推荐好友列表*/	
SMSG_SOCIAL_GET_RECOMMEND_FRIEND		= 175	-- /*推荐好友列表*/	
CMSG_SOCIAL_REVENGE_ENEMY		= 176	-- /*复仇*/	
CMSG_SOCIAL_DEL_FRIEND		= 177	-- /*删除好友*/	
CMSG_TELEPORT_MAIN_CITY		= 178	-- /*回城*/	
CMSG_CHAT_BY_CHANNEL		= 179	-- /*不同频道聊天*/	
SMSG_SEND_CHAT		= 180	-- /*发送聊天*/	
CMSG_SOCIAL_CLEAR_APPLY		= 181	-- /*清空申请列表*/	
CMSG_MSG_DECLINE		= 182	-- /*设置拒绝接受消息*/	
SMSG_FACTION_GET_LIST_RESULT		= 183	-- /*帮派列表*/	
CMSG_FACTION_GETLIST		= 184	-- /*获取帮派列表*/	
CMSG_FACTION_MANAGER		= 185	-- /*帮派管理*/	
CMSG_FACTION_MEMBER_OPERATE		= 186	-- /*帮派成员操作*/	
CMSG_FACTION_FAST_JOIN		= 187	-- /*快速加入帮派*/	
CMSG_SOCIAL_ADD_FRIEND_BYNAME		= 188	-- /*通过名字添加好友*/	
CMSG_READ_MAIL		= 190	-- /*读邮件*/	
CMSG_PICK_MAIL		= 191	-- /*领取邮件*/	
CMSG_REMOVE_MAIL		= 192	-- /*删除邮件*/	
CMSG_PICK_MAIL_ONE_STEP		= 193	-- /*一键领取邮件*/	
CMSG_REMOVE_MAIL_ONE_STEP		= 194	-- /*一键删除邮件*/	
CMSG_BLOCK_CHAT		= 195	-- /*屏蔽某人*/	
CMSG_CANCEL_BLOCK_CHAT		= 196	-- /*取消屏蔽*/	
CMSG_USE_BROADCAST_GAMEOBJECT		= 200	-- /*使用需要广播的游戏对象*/	
CMSG_WORLD_BOSS_ENROLL		= 201	-- /*世界BOSS报名*/	
CMSG_WORLD_BOSS_FIGHT		= 202	-- /*世界BOSS挑战*/	
CMSG_CHANGE_LINE		= 203	-- /*换线*/	
CMSG_ROLL_WORLD_BOSS_TREASURE		= 204	-- /*roll世界BOSS箱子*/	
SMSG_ROLL_RESULT		= 205	-- /*roll点结果*/	
SMSG_BOSS_RANK		= 206	-- /*当前BOSS伤害排名*/	
CMSG_RANK_ADD_LIKE		= 207	-- /*排行榜点赞*/	
SMSG_RANK_ADD_LIKE_RESULT		= 208	-- /*排行榜点赞结果*/	
CMSG_RES_INSTANCE_ENTER		= 210	-- /*进入资源副本*/	
CMSG_RES_INSTANCE_SWEEP		= 211	-- /*扫荡资源副本*/	
CMSG_SHOW_MAP_LINE		= 212	-- /*查看本地图的分线号*/	
SMSG_SEND_MAP_LINE		= 213	-- /*返回本地图的分线号信息*/	
SMSG_ITEM_NOTICE		= 214	-- /*获得奖励提示*/	
CMSG_TELEPORT_MAP		= 216	-- /*传送到某个世界地图*/	
CMSG_TELEPORT_FIELD_BOSS		= 217	-- /*传送到野外boss旁边*/	
CMSG_GET_ACTIVITY_REWARD		= 218	-- /*活跃度奖励*/	
CMSG_GET_ACHIEVE_REWARD		= 220	-- /*成就奖励*/	
CMSG_GET_ACHIEVE_ALL_REWARD		= 221	-- /*总成就奖励*/	
CMSG_SET_TITLE		= 222	-- /*装备称号*/	
CMSG_INIT_TITLE		= 223	-- /*初始化称号*/	
CMSG_WELFARE_SHOUCHONG_REWARD		= 224	-- /*领取首充奖励*/	
CMSG_WELFARE_CHECKIN		= 225	-- /*每日签到奖励*/	
CMSG_WELFARE_CHECKIN_ALL		= 226	-- /*累积签到奖励*/	
CMSG_WELFARE_CHECKIN_GETBACK		= 227	-- /*补签奖励*/	
CMSG_WELFARE_LEVEL		= 228	-- /*等级奖励*/	
CMSG_WELFARE_ACTIVE_GETBACK		= 229	-- /*活动找回奖励*/	
CMSG_PICK_QUEST_REWARD		= 230	-- /*领取任务奖励*/	
CMSG_TALK_WITH_NPC		= 231	-- /*和npc对话*/	
CMSG_USE_VIRTUAL_ITEM		= 232	-- /*使用虚拟物品*/	
CMSG_PICK_QUEST_CHAPTER_REWARD		= 233	-- /*领取任务章节奖励*/	
CMSG_KUAFU_3V3_MATCH		= 234	-- /*3v3跨服匹配*/	
SMSG_KUAFU_MATCH_START		= 235	-- /*跨服开始匹配*/	
CMSG_KUAFU_3V3_BUYTIMES		= 236	-- /*3v3购买次数*/	
CMSG_KUAFU_3V3_DAYREWARD		= 237	-- /*3v3每日活跃奖励*/	
CMSG_KUAFU_3V3_GETRANLIST		= 238	-- /*请求3v3排行榜*/	
SMSG_KUAFU_3V3_RANLIST		= 239	-- /*3v3排行榜结果列表*/	
CMSG_WELFARE_GETALLLIST_GETBACK		= 240	-- /*福利所有奖励列表*/	
SMSG_WELFARE_REWARDLIST_GETBACK		= 241	-- /*奖励列表*/	
CMSG_WELFARE_GETALL_GETBACK		= 242	-- /*一键领取所有福利*/	
CMSG_KUAFU_3V3_GETMYRANK		= 248	-- /*请求3v3排行榜自己的名次*/	
SMSG_KUAFU_3V3_MYRANK		= 249	-- /*3v3排行榜自己的名次结果*/	
SMSG_KUAFU_3V3_KILL_DETAIL		= 250	-- /*击杀数据*/	
SMSG_KUAFU_3V3_WAIT_INFO		= 251	-- /*跨服匹配等待数据*/	
MSG_KUAFU_3V3_CANCEL_MATCH		= 252	-- /*取消匹配*/	
CMSG_KUAFU_3V3_MATCH_OPER		= 253	-- /*匹配到人&接受或者拒绝*/	
SMSG_KUAFU_3V3_DECLINE_MATCH		= 254	-- /*拒绝比赛*/	
CMSG_KUAFU_XIANFU_MATCH		= 255	-- /*仙府夺宝跨服匹配*/	
SMSG_KUAFU_MATCH_WAIT		= 256	-- /*单方跨服匹配等待*/	
SMSG_KUAFU_XIANFU_MINIMAP_INFO		= 257	-- /*仙府夺宝小地图信息*/	
CMSG_BUY_XIANFU_ITEM		= 258	-- /*购买仙府进入券*/	
CMSG_XIANFU_RANDOM_RESPAWN		= 259	-- /*随机复活*/	
CMSG_DOUJIANTAI_FIGHT		= 260	-- /*斗剑台挑战*/	
CMSG_DOUJIANTAI_BUYTIME		= 261	-- /*斗剑台购买次数*/	
CMSG_DOUJIANTAI_CLEARCD		= 262	-- /*斗剑台清理CD*/	
CMSG_DOUJIANTAI_FIRST_REWARD		= 263	-- /*斗剑台首胜奖励*/	
MSG_DOUJIANTAI_GET_ENEMYS_INFO		= 265	-- /*斗剑台挑战对手信息*/	
CMSG_DOUJIANTAI_GET_RANK		= 266	-- /*斗剑台排行榜*/	
CMSG_DOUJIANTAI_REFRESH_ENEMYS		= 270	-- /*斗剑台刷新对手*/	
MSG_DOUJIANTAI_TOP3		= 271	-- /*斗剑台三甲*/	
MSG_USE_JUMP_POINT		= 272	-- /*使用跳点*/	
CMSG_BAG_ITEM_SELL		= 273	-- /*出售物品*/	
CMSG_BAG_ITEM_SORT		= 274	-- /*整理物品*/	
CMSG_SUBMIT_QUEST_DAILY2		= 280	-- /*提交日常任务*/	
SMSG_ATTRIBUTE_CHANGED		= 281	-- /*属性改变*/	
SMSG_BAG_FIND_EQUIP_BETTER		= 282	-- /*背包有更强装备*/	
SMSG_MODULE_ACTIVE		= 283	-- /*模块解锁*/	
CMSG_PICK_DAILY2_QUEST_REWARD		= 284	-- /*领取日常任务奖励*/	
CMSG_FINISH_NOW_GUIDE		= 285	-- /*完成当前引导*/	
CMSG_GET_CULTIVATION_INFO		= 286	-- /*取得修炼场信息*/	
SMSG_UPDATE_CULTIVATION_INFO		= 287	-- /*返回修炼场信息*/	
CMSG_GET_CULTIVATION_RIVALS_INFO		= 288	-- /*取得当前所有修炼场对手信息*/	
SMSG_UPDATE_CULTIVATION_RIVALS_INFO_LIST		= 289	-- /*返回修炼场对手信息*/	
CMSG_GET_CULTIVATION_REWARD		= 290	-- /*领取修炼场奖励*/	
CMSG_REFRESH_CULTIVATION_RIVALS		= 291	-- /*刷新修炼场对手*/	
CMSG_PLUNDER_CULTIVATION_RIVAL		= 292	-- /*掠夺修炼场对手*/	
CMSG_REVENGE_CULTIVATION_RIVAL		= 293	-- /*反击复仇修炼场对手*/	
CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT		= 294	-- /*增加修炼场剩余挑战次数*/	
SMSG_SHOW_CULTIVATION_RESULT_LIST		= 295	-- /*返回修炼场战斗结果*/	
CMSG_GET_LOGIN_ACTIVITY_REWARD		= 296	-- /*领取登录大礼奖励*/	
SMSG_CAST_SPELL_START		= 300	-- /*通知客户端释放蓄力技能*/	
CMSG_FINISH_OPTIONAL_GUIDE_STEP		= 301	-- /*完成非强制引导的步骤*/	
CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED		= 302	-- /*执行接到任务以后的命令*/	
SMSG_SHOW_UNIT_ATTRIBUTE		= 310	-- /*通知客户端显示属性*/	
CMSG_BACK_TO_FAMITY		= 320	-- /*返回家族*/	
SMSG_FACTION_BOSS_SEND_RESULT		= 321	-- /*返回家族boss结果*/	
CMSG_CHALLANGE_BOSS		= 322	-- /*挑战boss*/	
CMSG_PICK_OFFLINE_REWARD		= 325	-- /*领取离线奖励*/	
SMSG_OFFLINE_REWARD_RESULT		= 326	-- /*离线奖励结果*/	
CMSG_SMELTING_EQUIP		= 327	-- /*熔炼装备*/	
CMSG_STOREHOUSE_HAND_IN		= 328	-- /*上交装备*/	
CMSG_STOREHOUSE_EXCHANGE		= 329	-- /*兑换装备*/	
CMSG_STOREHOUSE_DESTROY		= 330	-- /*销毁装备*/	
CMSG_SEND_FACTION_GIFT		= 331	-- /*赠送礼物*/	
CMSG_GET_FACTION_GIFT_EXREWARD		= 332	-- /*领取额外奖励*/	
CMSG_GET_ALL_FACTION_GIFT_EXREWARD		= 333	-- /*领取所有额外奖励*/	
SMSG_SHOW_FACTION_GIFT_PAGE		= 334	-- /*返回礼物列表*/	
SMSG_SHOW_FACTION_GIFT_INFO		= 335	-- /*返回礼物信息*/	
SMSG_SHOW_FACTION_GIFT_UNTHANK_PAGE		= 336	-- /*返回女王未感谢礼物*/	
SMSG_SHOW_FACTION_GIFT_HISTORY_PAGE		= 337	-- /*返回女王历史记录*/	
CMSG_GET_FACTION_GIFT_RANK_PAGE		= 338	-- /*请求家族魅力排行*/	
SMSG_SHOW_FACTION_GIFT_RANK_RESULT_LIST		= 339	-- /*返回家族魅力排行*/	
SMSG_SHOW_FACTION_GIFT_RANK_CHANGE		= 340	-- /*返回家族魅力排行变化*/	
SMSG_SHOW_FACTION_GIFT_RANK_INFO		= 341	-- /*返回本家族魅力排行*/	
CMSG_DIVINE_FORGE		= 342	-- /*神兵强化*/	
CMSG_DIVINE_ADVANCE		= 343	-- /*神兵升阶*/	
CMSG_DIVINE_SPIRIT		= 344	-- /*神兵铸魂*/	
CMSG_QUERY_MASS_BOSS_INFO		= 352	-- /*查询全民boss信息*/	
SMSG_MASS_BOSS_INFO_RET		= 353	-- /*全民boss信息结果*/	
CMSG_QUERY_MASS_BOSS_RANK		= 354	-- /*查询全民boss排行榜*/	
SMSG_MASS_BOSS_RANK_RESULT		= 355	-- /*全民boss排行结果*/	
CMSG_TRY_MASS_BOSS		= 356	-- /*挑战全民boss*/	
CMSG_BUY_MASS_BOSS_TIMES		= 357	-- /*购买挑战全民boss次数*/	
CMSG_GROUP_INSTANCE_MATCH		= 358	-- /*组队副本跨服匹配*/	
CMSG_BUY_GROUP_INSTANCE_TIMES		= 359	-- /*组队副本跨服次数购买*/	
CMSG_TALISMAN_ACTIVE		= 360	-- /*法宝激活*/	
CMSG_TALISMAN_LVUP		= 361	-- /*法宝注灵*/	
CMSG_WINGS_ACTIVE		= 362	-- /*神羽激活*/	
CMSG_WINGS_BLESS		= 363	-- /*神羽祝福*/	
CMSG_WINGS_RANKUP		= 364	-- /*神羽升阶*/	
CMSG_WINGS_STRENGTH		= 365	-- /*神羽强化*/	
CMSG_MERIDIAN_PRACTISE		= 366	-- /*经脉修炼*/	
CMSG_ADD_MERIDIAN_EXP		= 367	-- /*加经脉修炼经验值*/	
CMSG_RAISE_MOUNT_LEVEL_BASE		= 368	-- /*提升坐骑等级*/	
CMSG_ACTIVE_MOUNT		= 369	-- /*解锁坐骑*/	
SMSG_SHOW_FACTION_BOSSDEFENSE_DAMAGE_LIST		= 370	-- /*家族领主挑战输出排行*/	
SMSG_SHOW_FACTION_TOWER_SWEEP_LIST		= 371	-- /*家族无尽远征扫荡结果*/	
SMSG_SEND_INSTANCE_RESULT		= 375	-- /*副本结果*/	
CMSG_MATCH_SINGLE_PVP		= 376	-- /*匹配单人pvp*/	
CMSG_BUY_MATCH_SINGLE_PVP_TIMES		= 377	-- /*购买单人pvp次数*/	
CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD		= 378	-- /*领取单人pvp额外奖励*/	
CMSG_EQUIPDEVELOP_OPERATE		= 380	-- /*装备养成操作*/	
CMSG_ACTIVE_APPEARANCE		= 381	-- /*激活外观*/	
CMSG_EQUIP_APPEARANCE		= 382	-- /*装备外观*/	
CMSG_CANCEL_EQUIP_APPEARANCE		= 383	-- /*取消装备外观*/	
CMSG_RENAME		= 384	-- /*改名*/	
CMSG_UNLOCK_TITLE		= 385	-- /*道具解锁称号*/	
CMSG_SOCIAL_BUY_REVENGE_TIMES		= 386	-- /*购买复仇次数*/	
CMSG_ENTER_RISK_INSTANCE		= 387	-- /*请求进入世界冒险副本*/	
CMSG_SOCIAL_REMOVE_ENEMY		= 388	-- /*删除仇人*/	
CMSG_GET_PLAYER_OVERVIEW		= 389	-- /*查看玩家详情*/	
SMSG_SHOW_PLAYER_OVERVIEW		= 390	-- /*返回玩家详情*/	
CMSG_SEND_FACTION_INVITE		= 391	-- /*邀请加入帮派*/	
SMSG_SHOW_FACTION_INVITE		= 392	-- /*显示邀请*/	
CMSG_BUY_VIPGIFT		= 393	-- /*购买vip礼包*/	
CMSG_ACTIVITY_OPT_BUY_DAILYGIFT		= 394	-- /*购买活动每日礼包*/	
CMSG_DRAW_LOTTERY		= 395	-- /*抽奖*/	
CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD		= 396	-- /*获取活动开服排行进度奖励*/	
CMSG_ACTIVITY_OPT_GET_RANK_LIST		= 397	-- /*获取活动开服排行榜*/	
SMSG_ACTIVITY_OPT_SHOW_RANK_LIST		= 398	-- /*返回活动开服排行榜*/	
CMSG_ACTIVITY_OPT_BUY_LIMITGIFT		= 399	-- /*购买活动限定礼包*/	
CMSG_WELFARE_GET_RECHARGE_REWARD		= 400	-- /*领取累计充值奖励*/	
CMSG_WELFARE_GET_CONSUME_REWARD		= 401	-- /*领取累计消费奖励*/	
CMSG_WELFARE_GET_SEVENDAY_REWARD		= 402	-- /*领取七日大礼奖励*/	
SMSG_SEND_SERVER_OPEN_TIME		= 403	-- /*服务器开服时间*/	
CMSG_RISK_GET_RANK		= 404	-- /*请求世界冒险排行榜*/	
SMSG_RISK_GET_RANK_RESULT		= 405	-- /*世界冒险排行榜信息 */	
CMSG_SET_ORIENT		= 406	-- /*设置朝向*/	


---------------------------------------------------------------------
--/*坐标结构体*/

point_t = class('point_t')

function point_t:read( input )

	local ret
	ret,self.pos_x = input:readFloat() --/*坐标X*/

	if not ret then
		return ret
	end
	ret,self.pos_y = input:readFloat() --/*坐标Y*/

	if not ret then
		return ret
	end

	return input
end

function point_t:write( output )
	if(self.pos_x == nil)then
		self.pos_x = 0
	end
	output:writeFloat(self.pos_x)
	
	if(self.pos_y == nil)then
		self.pos_y = 0
	end
	output:writeFloat(self.pos_y)
	
	return output
end

---------------------------------------------------------------------
--/*传送地点结构体*/

taxi_menu_info_t = class('taxi_menu_info_t')

function taxi_menu_info_t:read( input )

	local ret
	ret,self.id = input:readI32() --/**/

	if not ret then
		return ret
	end
	ret,self.taxi_text = input:readUTFByLen(50)  --/*传送地点名称*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.map_id = input:readU32() --/*地图ID*/

	if not ret then
		return ret
	end
	ret,self.pos_x = input:readU16() --/*坐标X*/

	if not ret then
		return ret
	end
	ret,self.pos_y = input:readU16() --/*坐标Y*/

	if not ret then
		return ret
	end

	return input
end

function taxi_menu_info_t:write( output )
	if(self.id == nil)then
		self.id = 0
	end
	output:writeI32(self.id)
	
	if(self.taxi_text == nil)then
		self.taxi_text = ''
	end
	output:writeUTFByLen(self.taxi_text , 50 ) 
	
	if(self.map_id == nil)then
		self.map_id = 0
	end
	output:writeU32(self.map_id)
	
	if(self.pos_x == nil)then
		self.pos_x = 0
	end
	output:writeI16(self.pos_x)
	
	if(self.pos_y == nil)then
		self.pos_y = 0
	end
	output:writeI16(self.pos_y)
	
	return output
end

---------------------------------------------------------------------
--/*玩家角色创建选择信息*/

char_create_info_t = class('char_create_info_t')

function char_create_info_t:read( input )

	local ret
	ret,self.name = input:readUTFByLen(50)  --/*名称*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.faction = input:readByte() --/*阵营*/

	if not ret then
		return ret
	end
	ret,self.gender = input:readByte() --/*性别*/

	if not ret then
		return ret
	end
	ret,self.level = input:readU16() --/*等级*/

	if not ret then
		return ret
	end
	ret,self.guid = input:readUTFByLen(50)  --/**/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.head_id = input:readU32() --/*头像*/

	if not ret then
		return ret
	end
	ret,self.hair_id = input:readU32() --/*发型ID*/

	if not ret then
		return ret
	end
	ret,self.race = input:readByte() --/*种族，猛男美女萝莉那些*/

	if not ret then
		return ret
	end
	ret,self.inviteGuid = input:readUTFByLen(50)  --/*邀请的帮派id*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.faction_name = input:readUTFByLen(50)  --/*创建的帮派名称*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.icon = input:readByte() --/*创建的帮派标志*/

	if not ret then
		return ret
	end

	return input
end

function char_create_info_t:write( output )
	if(self.name == nil)then
		self.name = ''
	end
	output:writeUTFByLen(self.name , 50 ) 
	
	if(self.faction == nil)then
		self.faction = 0
	end
	output:writeByte(self.faction)
	
	if(self.gender == nil)then
		self.gender = 0
	end
	output:writeByte(self.gender)
	
	if(self.level == nil)then
		self.level = 0
	end
	output:writeI16(self.level)
	
	if(self.guid == nil)then
		self.guid = ''
	end
	output:writeUTFByLen(self.guid , 50 ) 
	
	if(self.head_id == nil)then
		self.head_id = 0
	end
	output:writeU32(self.head_id)
	
	if(self.hair_id == nil)then
		self.hair_id = 0
	end
	output:writeU32(self.hair_id)
	
	if(self.race == nil)then
		self.race = 0
	end
	output:writeByte(self.race)
	
	if(self.inviteGuid == nil)then
		self.inviteGuid = ''
	end
	output:writeUTFByLen(self.inviteGuid , 50 ) 
	
	if(self.faction_name == nil)then
		self.faction_name = ''
	end
	output:writeUTFByLen(self.faction_name , 50 ) 
	
	if(self.icon == nil)then
		self.icon = 0
	end
	output:writeByte(self.icon)
	
	return output
end

---------------------------------------------------------------------
--/*任务菜单*/

quest_option_t = class('quest_option_t')

function quest_option_t:read( input )

	local ret
	ret,self.quest_id = input:readU32() --/*任务id*/

	if not ret then
		return ret
	end
	ret,self.quest_icon = input:readU32() --/*图标*/

	if not ret then
		return ret
	end
	ret,self.quest_level = input:readU16() --/*任务等级*/

	if not ret then
		return ret
	end
	ret,self.quest_title = input:readUTFByLen(50)  --/*任务标题*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.flags = input:readU32() --/*标识*/

	if not ret then
		return ret
	end

	return input
end

function quest_option_t:write( output )
	if(self.quest_id == nil)then
		self.quest_id = 0
	end
	output:writeU32(self.quest_id)
	
	if(self.quest_icon == nil)then
		self.quest_icon = 0
	end
	output:writeU32(self.quest_icon)
	
	if(self.quest_level == nil)then
		self.quest_level = 0
	end
	output:writeI16(self.quest_level)
	
	if(self.quest_title == nil)then
		self.quest_title = ''
	end
	output:writeUTFByLen(self.quest_title , 50 ) 
	
	if(self.flags == nil)then
		self.flags = 0
	end
	output:writeU32(self.flags)
	
	return output
end

---------------------------------------------------------------------
--/*可接任务信息*/

quest_canaccept_info_t = class('quest_canaccept_info_t')

function quest_canaccept_info_t:read( input )

	local ret
	ret,self.quest_id = input:readU32() --/*任务ID*/

	if not ret then
		return ret
	end
	ret,self.quest_type = input:readByte() --/*任务类别*/

	if not ret then
		return ret
	end
	ret,self.title = input:readUTFByLen(50)  --/*标题*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.npc_id = input:readU32() --/*接受任务NPC模板id*/

	if not ret then
		return ret
	end
	ret,self.quest_level = input:readU32() --/*任务等级*/

	if not ret then
		return ret
	end

	return input
end

function quest_canaccept_info_t:write( output )
	if(self.quest_id == nil)then
		self.quest_id = 0
	end
	output:writeU32(self.quest_id)
	
	if(self.quest_type == nil)then
		self.quest_type = 0
	end
	output:writeByte(self.quest_type)
	
	if(self.title == nil)then
		self.title = ''
	end
	output:writeUTFByLen(self.title , 50 ) 
	
	if(self.npc_id == nil)then
		self.npc_id = 0
	end
	output:writeU32(self.npc_id)
	
	if(self.quest_level == nil)then
		self.quest_level = 0
	end
	output:writeU32(self.quest_level)
	
	return output
end

---------------------------------------------------------------------
--/*闲聊选项结构体*/

gossip_menu_option_info_t = class('gossip_menu_option_info_t')

function gossip_menu_option_info_t:read( input )

	local ret
	ret,self.id = input:readI32() --/*id*/

	if not ret then
		return ret
	end
	ret,self.option_icon = input:readI32() --/*选项icon图标*/

	if not ret then
		return ret
	end
	ret,self.option_title = input:readUTFByLen(200)  --/*选项文本*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end

	return input
end

function gossip_menu_option_info_t:write( output )
	if(self.id == nil)then
		self.id = 0
	end
	output:writeI32(self.id)
	
	if(self.option_icon == nil)then
		self.option_icon = 0
	end
	output:writeI32(self.option_icon)
	
	if(self.option_title == nil)then
		self.option_title = ''
	end
	output:writeUTFByLen(self.option_title , 200 ) 
	
	return output
end

---------------------------------------------------------------------
--/*物品冷却*/

item_cooldown_info_t = class('item_cooldown_info_t')

function item_cooldown_info_t:read( input )

	local ret
	ret,self.item = input:readU32() --/*物品摸版*/

	if not ret then
		return ret
	end
	ret,self.cooldown = input:readU32() --/*冷却时间*/

	if not ret then
		return ret
	end

	return input
end

function item_cooldown_info_t:write( output )
	if(self.item == nil)then
		self.item = 0
	end
	output:writeU32(self.item)
	
	if(self.cooldown == nil)then
		self.cooldown = 0
	end
	output:writeU32(self.cooldown)
	
	return output
end

---------------------------------------------------------------------
--/*任务状态*/

quest_status_t = class('quest_status_t')

function quest_status_t:read( input )

	local ret
	ret,self.quest_id = input:readU16() --/*任务ID*/

	if not ret then
		return ret
	end
	ret,self.status = input:readByte() --/*任务状态*/

	if not ret then
		return ret
	end

	return input
end

function quest_status_t:write( output )
	if(self.quest_id == nil)then
		self.quest_id = 0
	end
	output:writeI16(self.quest_id)
	
	if(self.status == nil)then
		self.status = 0
	end
	output:writeByte(self.status)
	
	return output
end

---------------------------------------------------------------------
--/*道具奖励信息*/

item_reward_info_t = class('item_reward_info_t')

function item_reward_info_t:read( input )

	local ret
	ret,self.item_id = input:readU16() --/*道具id*/

	if not ret then
		return ret
	end
	ret,self.num = input:readU32() --/*道具数量*/

	if not ret then
		return ret
	end

	return input
end

function item_reward_info_t:write( output )
	if(self.item_id == nil)then
		self.item_id = 0
	end
	output:writeI16(self.item_id)
	
	if(self.num == nil)then
		self.num = 0
	end
	output:writeU32(self.num)
	
	return output
end

---------------------------------------------------------------------
--/*好友信息*/

social_friend_info_t = class('social_friend_info_t')

function social_friend_info_t:read( input )

	local ret
	ret,self.guid = input:readUTFByLen(50)  --/*好友guid*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.name = input:readUTFByLen(50)  --/*名字*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.faction = input:readUTFByLen(50)  --/*帮派*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.level = input:readU16() --/*等级*/

	if not ret then
		return ret
	end
	ret,self.icon = input:readU16() --/*头像*/

	if not ret then
		return ret
	end
	ret,self.vip = input:readU16() --/*头像*/

	if not ret then
		return ret
	end

	return input
end

function social_friend_info_t:write( output )
	if(self.guid == nil)then
		self.guid = ''
	end
	output:writeUTFByLen(self.guid , 50 ) 
	
	if(self.name == nil)then
		self.name = ''
	end
	output:writeUTFByLen(self.name , 50 ) 
	
	if(self.faction == nil)then
		self.faction = ''
	end
	output:writeUTFByLen(self.faction , 50 ) 
	
	if(self.level == nil)then
		self.level = 0
	end
	output:writeI16(self.level)
	
	if(self.icon == nil)then
		self.icon = 0
	end
	output:writeI16(self.icon)
	
	if(self.vip == nil)then
		self.vip = 0
	end
	output:writeI16(self.vip)
	
	return output
end

---------------------------------------------------------------------
--/*帮派信息*/

faction_info_t = class('faction_info_t')

function faction_info_t:read( input )

	local ret
	ret,self.faction_guid = input:readUTFByLen(50)  --/*帮派guid*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.faction_name = input:readUTFByLen(50)  --/*名字*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.faction_bz = input:readUTFByLen(50)  --/*帮主名字*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.faction_gg = input:readUTFByLen(108)  --/*公告*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.level = input:readU16() --/*等级*/

	if not ret then
		return ret
	end
	ret,self.icon = input:readByte() --/*头像*/

	if not ret then
		return ret
	end
	ret,self.player_count = input:readU16() --/*帮派人数*/

	if not ret then
		return ret
	end
	ret,self.minlev = input:readU16() --/*等级限制*/

	if not ret then
		return ret
	end

	return input
end

function faction_info_t:write( output )
	if(self.faction_guid == nil)then
		self.faction_guid = ''
	end
	output:writeUTFByLen(self.faction_guid , 50 ) 
	
	if(self.faction_name == nil)then
		self.faction_name = ''
	end
	output:writeUTFByLen(self.faction_name , 50 ) 
	
	if(self.faction_bz == nil)then
		self.faction_bz = ''
	end
	output:writeUTFByLen(self.faction_bz , 50 ) 
	
	if(self.faction_gg == nil)then
		self.faction_gg = ''
	end
	output:writeUTFByLen(self.faction_gg , 108 ) 
	
	if(self.level == nil)then
		self.level = 0
	end
	output:writeI16(self.level)
	
	if(self.icon == nil)then
		self.icon = 0
	end
	output:writeByte(self.icon)
	
	if(self.player_count == nil)then
		self.player_count = 0
	end
	output:writeI16(self.player_count)
	
	if(self.minlev == nil)then
		self.minlev = 0
	end
	output:writeI16(self.minlev)
	
	return output
end

---------------------------------------------------------------------
--/*排名信息*/

rank_info_t = class('rank_info_t')

function rank_info_t:read( input )

	local ret
	ret,self.name = input:readUTFByLen(50)  --/*名字*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.value = input:readFloat() --/*伤害百分比*/

	if not ret then
		return ret
	end

	return input
end

function rank_info_t:write( output )
	if(self.name == nil)then
		self.name = ''
	end
	output:writeUTFByLen(self.name , 50 ) 
	
	if(self.value == nil)then
		self.value = 0
	end
	output:writeFloat(self.value)
	
	return output
end

---------------------------------------------------------------------
--/*坐标结构体*/

line_info_t = class('line_info_t')

function line_info_t:read( input )

	local ret
	ret,self.lineNo = input:readU16() --/*分线号*/

	if not ret then
		return ret
	end
	ret,self.rate = input:readByte() --/*玩家比率*/

	if not ret then
		return ret
	end

	return input
end

function line_info_t:write( output )
	if(self.lineNo == nil)then
		self.lineNo = 0
	end
	output:writeI16(self.lineNo)
	
	if(self.rate == nil)then
		self.rate = 0
	end
	output:writeByte(self.rate)
	
	return output
end

---------------------------------------------------------------------
--/*等待信息*/

wait_info_t = class('wait_info_t')

function wait_info_t:read( input )

	local ret
	ret,self.name = input:readUTFByLen(50)  --/*名字*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.state = input:readByte() --/*状态*/

	if not ret then
		return ret
	end

	return input
end

function wait_info_t:write( output )
	if(self.name == nil)then
		self.name = ''
	end
	output:writeUTFByLen(self.name , 50 ) 
	
	if(self.state == nil)then
		self.state = 0
	end
	output:writeByte(self.state)
	
	return output
end

---------------------------------------------------------------------
--/*修炼场对手信息*/

cultivation_rivals_info_t = class('cultivation_rivals_info_t')

function cultivation_rivals_info_t:read( input )

	local ret
	ret,self.index = input:readU32() --/*序号*/

	if not ret then
		return ret
	end
	ret,self.name = input:readUTFByLen(50)  --/*名字*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.level = input:readU32() --/*等级*/

	if not ret then
		return ret
	end
	ret,self.weapon = input:readU32() --/*武器*/

	if not ret then
		return ret
	end
	ret,self.avatar = input:readU32() --/*外观*/

	if not ret then
		return ret
	end
	ret,self.divine = input:readU32() --/*神兵*/

	if not ret then
		return ret
	end
	ret,self.force = input:readU32() --/*战力*/

	if not ret then
		return ret
	end
	ret,self.chest = input:readU32() --/*宝箱*/

	if not ret then
		return ret
	end
	ret,self.gender = input:readU32() --/*性别*/

	if not ret then
		return ret
	end

	return input
end

function cultivation_rivals_info_t:write( output )
	if(self.index == nil)then
		self.index = 0
	end
	output:writeU32(self.index)
	
	if(self.name == nil)then
		self.name = ''
	end
	output:writeUTFByLen(self.name , 50 ) 
	
	if(self.level == nil)then
		self.level = 0
	end
	output:writeU32(self.level)
	
	if(self.weapon == nil)then
		self.weapon = 0
	end
	output:writeU32(self.weapon)
	
	if(self.avatar == nil)then
		self.avatar = 0
	end
	output:writeU32(self.avatar)
	
	if(self.divine == nil)then
		self.divine = 0
	end
	output:writeU32(self.divine)
	
	if(self.force == nil)then
		self.force = 0
	end
	output:writeU32(self.force)
	
	if(self.chest == nil)then
		self.chest = 0
	end
	output:writeU32(self.chest)
	
	if(self.gender == nil)then
		self.gender = 0
	end
	output:writeU32(self.gender)
	
	return output
end

---------------------------------------------------------------------
--/*家族礼物信息*/

faction_gift_info_t = class('faction_gift_info_t')

function faction_gift_info_t:read( input )

	local ret
	ret,self.rank = input:readU32() --/*排行*/

	if not ret then
		return ret
	end
	ret,self.id = input:readU32() --/*id*/

	if not ret then
		return ret
	end
	ret,self.point = input:readU32() --/*魅力值*/

	if not ret then
		return ret
	end
	ret,self.thank = input:readU32() --/*感谢标识*/

	if not ret then
		return ret
	end
	ret,self.reply = input:readU32() --/*女王回复标识*/

	if not ret then
		return ret
	end
	ret,self.time = input:readU32() --/*时间*/

	if not ret then
		return ret
	end
	ret,self.count_id = input:readU32() --/*count_id*/

	if not ret then
		return ret
	end
	ret,self.guid = input:readUTFByLen(50)  --/*赠送者guid*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.msg = input:readUTFByLen(50)  --/*赠送者留言*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.item_list = input:readUTFByLen(150)  --/*赠送道具信息*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.reply_list = input:readUTFByLen(100)  --/*回复信息*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end

	return input
end

function faction_gift_info_t:write( output )
	if(self.rank == nil)then
		self.rank = 0
	end
	output:writeU32(self.rank)
	
	if(self.id == nil)then
		self.id = 0
	end
	output:writeU32(self.id)
	
	if(self.point == nil)then
		self.point = 0
	end
	output:writeU32(self.point)
	
	if(self.thank == nil)then
		self.thank = 0
	end
	output:writeU32(self.thank)
	
	if(self.reply == nil)then
		self.reply = 0
	end
	output:writeU32(self.reply)
	
	if(self.time == nil)then
		self.time = 0
	end
	output:writeU32(self.time)
	
	if(self.count_id == nil)then
		self.count_id = 0
	end
	output:writeU32(self.count_id)
	
	if(self.guid == nil)then
		self.guid = ''
	end
	output:writeUTFByLen(self.guid , 50 ) 
	
	if(self.msg == nil)then
		self.msg = ''
	end
	output:writeUTFByLen(self.msg , 50 ) 
	
	if(self.item_list == nil)then
		self.item_list = ''
	end
	output:writeUTFByLen(self.item_list , 150 ) 
	
	if(self.reply_list == nil)then
		self.reply_list = ''
	end
	output:writeUTFByLen(self.reply_list , 100 ) 
	
	return output
end

---------------------------------------------------------------------
--/*家族魅力排行信息*/

faction_gift_rank_info_t = class('faction_gift_rank_info_t')

function faction_gift_rank_info_t:read( input )

	local ret
	ret,self.rank = input:readU32() --/*排行*/

	if not ret then
		return ret
	end
	ret,self.point = input:readU32() --/*魅力值*/

	if not ret then
		return ret
	end
	ret,self.queen_name = input:readUTFByLen(50)  --/*女王名称*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.faction_name = input:readUTFByLen(50)  --/*家族名称*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.guard_name = input:readUTFByLen(50)  --/*骑士名称*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.faction_flag = input:readU32() --/*家族旗子*/

	if not ret then
		return ret
	end
	ret,self.queen_vip = input:readU32() --/*女王vip等级*/

	if not ret then
		return ret
	end
	ret,self.guard_vip = input:readU32() --/*骑士vip等级*/

	if not ret then
		return ret
	end

	return input
end

function faction_gift_rank_info_t:write( output )
	if(self.rank == nil)then
		self.rank = 0
	end
	output:writeU32(self.rank)
	
	if(self.point == nil)then
		self.point = 0
	end
	output:writeU32(self.point)
	
	if(self.queen_name == nil)then
		self.queen_name = ''
	end
	output:writeUTFByLen(self.queen_name , 50 ) 
	
	if(self.faction_name == nil)then
		self.faction_name = ''
	end
	output:writeUTFByLen(self.faction_name , 50 ) 
	
	if(self.guard_name == nil)then
		self.guard_name = ''
	end
	output:writeUTFByLen(self.guard_name , 50 ) 
	
	if(self.faction_flag == nil)then
		self.faction_flag = 0
	end
	output:writeU32(self.faction_flag)
	
	if(self.queen_vip == nil)then
		self.queen_vip = 0
	end
	output:writeU32(self.queen_vip)
	
	if(self.guard_vip == nil)then
		self.guard_vip = 0
	end
	output:writeU32(self.guard_vip)
	
	return output
end

---------------------------------------------------------------------
--/*全民boss信息*/

mass_boss_info_t = class('mass_boss_info_t')

function mass_boss_info_t:read( input )

	local ret
	ret,self.id = input:readByte() --/*全民boss编号*/

	if not ret then
		return ret
	end
	ret,self.state = input:readByte() --/*全民boss状态*/

	if not ret then
		return ret
	end
	ret,self.time = input:readU32() --/*全民boss刷新时间*/

	if not ret then
		return ret
	end
	ret,self.percent = input:readByte() --/*boss血量*/

	if not ret then
		return ret
	end
	ret,self.count = input:readU16() --/*挑战boss人数*/

	if not ret then
		return ret
	end

	return input
end

function mass_boss_info_t:write( output )
	if(self.id == nil)then
		self.id = 0
	end
	output:writeByte(self.id)
	
	if(self.state == nil)then
		self.state = 0
	end
	output:writeByte(self.state)
	
	if(self.time == nil)then
		self.time = 0
	end
	output:writeU32(self.time)
	
	if(self.percent == nil)then
		self.percent = 0
	end
	output:writeByte(self.percent)
	
	if(self.count == nil)then
		self.count = 0
	end
	output:writeI16(self.count)
	
	return output
end

---------------------------------------------------------------------
--/*全民boss排名*/

mass_boss_rank_info_t = class('mass_boss_rank_info_t')

function mass_boss_rank_info_t:read( input )

	local ret
	ret,self.name = input:readUTFByLen(50)  --/*名称*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.dam = input:readDouble() --/*伤害*/

	if not ret then
		return ret
	end

	return input
end

function mass_boss_rank_info_t:write( output )
	if(self.name == nil)then
		self.name = ''
	end
	output:writeUTFByLen(self.name , 50 ) 
	
	if(self.dam == nil)then
		self.dam = 0
	end
	output:writeDouble(self.dam)
	
	return output
end

---------------------------------------------------------------------
--/*装备信息*/

equip_info_t = class('equip_info_t')

function equip_info_t:read( input )

	local ret
	ret,self.equip = input:readUTFByLen(50)  --/*装备信息*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.strength_lv = input:readU32() --/*强化*/

	if not ret then
		return ret
	end
	ret,self.refine_rank = input:readU32() --/*精炼阶级*/

	if not ret then
		return ret
	end
	ret,self.refine_star = input:readU32() --/*精炼星级*/

	if not ret then
		return ret
	end
	ret,self.gem1_lv = input:readU32() --/*宝石1等级*/

	if not ret then
		return ret
	end
	ret,self.gem2_lv = input:readU32() --/*宝石1等级*/

	if not ret then
		return ret
	end
	ret,self.gem3_lv = input:readU32() --/*宝石1等级*/

	if not ret then
		return ret
	end

	return input
end

function equip_info_t:write( output )
	if(self.equip == nil)then
		self.equip = ''
	end
	output:writeUTFByLen(self.equip , 50 ) 
	
	if(self.strength_lv == nil)then
		self.strength_lv = 0
	end
	output:writeU32(self.strength_lv)
	
	if(self.refine_rank == nil)then
		self.refine_rank = 0
	end
	output:writeU32(self.refine_rank)
	
	if(self.refine_star == nil)then
		self.refine_star = 0
	end
	output:writeU32(self.refine_star)
	
	if(self.gem1_lv == nil)then
		self.gem1_lv = 0
	end
	output:writeU32(self.gem1_lv)
	
	if(self.gem2_lv == nil)then
		self.gem2_lv = 0
	end
	output:writeU32(self.gem2_lv)
	
	if(self.gem3_lv == nil)then
		self.gem3_lv = 0
	end
	output:writeU32(self.gem3_lv)
	
	return output
end

---------------------------------------------------------------------
--/*开服活动排名*/

act_rank_info_t = class('act_rank_info_t')

function act_rank_info_t:read( input )

	local ret
	ret,self.name = input:readUTFByLen(50)  --/*名称*/

	if not ret then
		return ret
	end

	if not ret then
		return ret
	end
	ret,self.value = input:readU32() --/*数值*/

	if not ret then
		return ret
	end

	return input
end

function act_rank_info_t:write( output )
	if(self.name == nil)then
		self.name = ''
	end
	output:writeUTFByLen(self.name , 50 ) 
	
	if(self.value == nil)then
		self.value = 0
	end
	output:writeU32(self.value)
	
	return output
end


---------------------------------------------
--协议打、解包

-- /*无效动作*/	
function Protocols.pack_null_action (  )
	local output = Packet.new(MSG_NULL_ACTION)
	return output
end

-- /*无效动作*/	
function Protocols.call_null_action ( playerInfo )
	local output = Protocols.	pack_null_action (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*无效动作*/	
function Protocols.unpack_null_action (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*测试连接状态*/	
function Protocols.pack_ping_pong (  )
	local output = Packet.new(MSG_PING_PONG)
	return output
end

-- /*测试连接状态*/	
function Protocols.call_ping_pong ( playerInfo )
	local output = Protocols.	pack_ping_pong (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*测试连接状态*/	
function Protocols.unpack_ping_pong (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*踢掉在线的准备强制登陆*/	
function Protocols.pack_forced_into (  )
	local output = Packet.new(CMSG_FORCED_INTO)
	return output
end

-- /*踢掉在线的准备强制登陆*/	
function Protocols.call_forced_into ( playerInfo )
	local output = Protocols.	pack_forced_into (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*踢掉在线的准备强制登陆*/	
function Protocols.unpack_forced_into (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*获得Session对象*/	
function Protocols.pack_get_session ( sessionkey ,account ,version)
	local output = Packet.new(CMSG_GET_SESSION)
	output:writeUTF(sessionkey)
	output:writeUTF(account)
	output:writeUTF(version)
	return output
end

-- /*获得Session对象*/	
function Protocols.call_get_session ( playerInfo, sessionkey ,account ,version)
	local output = Protocols.	pack_get_session ( sessionkey ,account ,version)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*获得Session对象*/	
function Protocols.unpack_get_session (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.sessionkey = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.account = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.version = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*网关服数据包路由测试*/	
function Protocols.pack_route_trace ( val)
	local output = Packet.new(MSG_ROUTE_TRACE)
	output:writeUTF(val)
	return output
end

-- /*网关服数据包路由测试*/	
function Protocols.call_route_trace ( playerInfo, val)
	local output = Protocols.	pack_route_trace ( val)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*网关服数据包路由测试*/	
function Protocols.unpack_route_trace (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.val = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*记录客户端日志*/	
function Protocols.pack_write_client_log ( type ,uid ,guid ,log)
	local output = Packet.new(CMSG_WRITE_CLIENT_LOG)
	output:writeU32(type)
	output:writeUTF(uid)
	output:writeUTF(guid)
	output:writeUTF(log)
	return output
end

-- /*记录客户端日志*/	
function Protocols.call_write_client_log ( playerInfo, type ,uid ,guid ,log)
	local output = Protocols.	pack_write_client_log ( type ,uid ,guid ,log)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*记录客户端日志*/	
function Protocols.unpack_write_client_log (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.uid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.log = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*操作失败*/	
function Protocols.pack_operation_failed ( type ,reason ,data)
	local output = Packet.new(SMSG_OPERATION_FAILED)
	output:writeI16(type)
	output:writeI16(reason)
	output:writeUTF(data)
	return output
end

-- /*操作失败*/	
function Protocols.call_operation_failed ( playerInfo, type ,reason ,data)
	local output = Protocols.	pack_operation_failed ( type ,reason ,data)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*操作失败*/	
function Protocols.unpack_operation_failed (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.reason = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.data = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*同步时间*/	
function Protocols.pack_sync_mstime ( mstime_now ,time_now ,open_time)
	local output = Packet.new(MSG_SYNC_MSTIME)
	output:writeU32(mstime_now)
	output:writeU32(time_now)
	output:writeU32(open_time)
	return output
end

-- /*同步时间*/	
function Protocols.call_sync_mstime ( playerInfo, mstime_now ,time_now ,open_time)
	local output = Protocols.	pack_sync_mstime ( mstime_now ,time_now ,open_time)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*同步时间*/	
function Protocols.unpack_sync_mstime (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.mstime_now = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.time_now = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.open_time = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*对象更新*/	
function Protocols.pack_ud_object (  )
	local output = Packet.new(SMSG_UD_OBJECT)
	return output
end

-- /*对象更新*/	
function Protocols.call_ud_object ( playerInfo )
	local output = Protocols.	pack_ud_object (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*对象更新*/	
function Protocols.unpack_ud_object (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*对象更新控制协议*/	
function Protocols.pack_ud_control (  )
	local output = Packet.new(CMSG_UD_CONTROL)
	return output
end

-- /*对象更新控制协议*/	
function Protocols.call_ud_control ( playerInfo )
	local output = Protocols.	pack_ud_control (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*对象更新控制协议*/	
function Protocols.unpack_ud_control (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*对象更新控制协议结果*/	
function Protocols.pack_ud_control_result (  )
	local output = Packet.new(SMSG_UD_CONTROL_RESULT)
	return output
end

-- /*对象更新控制协议结果*/	
function Protocols.call_ud_control_result ( playerInfo )
	local output = Protocols.	pack_ud_control_result (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*对象更新控制协议结果*/	
function Protocols.unpack_ud_control_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*GRID的对象更新*/	
function Protocols.pack_grid_ud_object (  )
	local output = Packet.new(SMSG_GRID_UD_OBJECT)
	return output
end

-- /*GRID的对象更新*/	
function Protocols.call_grid_ud_object ( playerInfo )
	local output = Protocols.	pack_grid_ud_object (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*GRID的对象更新*/	
function Protocols.unpack_grid_ud_object (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*GRID的对象更新*/	
function Protocols.pack_grid_ud_object_2 (  )
	local output = Packet.new(SMSG_GRID_UD_OBJECT_2)
	return output
end

-- /*GRID的对象更新*/	
function Protocols.call_grid_ud_object_2 ( playerInfo )
	local output = Protocols.	pack_grid_ud_object_2 (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*GRID的对象更新*/	
function Protocols.unpack_grid_ud_object_2 (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*告诉客户端，目前自己排在登录队列的第几位*/	
function Protocols.pack_login_queue_index ( index)
	local output = Packet.new(SMSG_LOGIN_QUEUE_INDEX)
	output:writeU32(index)
	return output
end

-- /*告诉客户端，目前自己排在登录队列的第几位*/	
function Protocols.call_login_queue_index ( playerInfo, index)
	local output = Protocols.	pack_login_queue_index ( index)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*告诉客户端，目前自己排在登录队列的第几位*/	
function Protocols.unpack_login_queue_index (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.index = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*踢人原因*/	
function Protocols.pack_kicking_type ( k_type)
	local output = Packet.new(SMSG_KICKING_TYPE)
	output:writeU32(k_type)
	return output
end

-- /*踢人原因*/	
function Protocols.call_kicking_type ( playerInfo, k_type)
	local output = Protocols.	pack_kicking_type ( k_type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*踢人原因*/	
function Protocols.unpack_kicking_type (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.k_type = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*获取角色列表*/	
function Protocols.pack_get_chars_list (  )
	local output = Packet.new(CMSG_GET_CHARS_LIST)
	return output
end

-- /*获取角色列表*/	
function Protocols.call_get_chars_list ( playerInfo )
	local output = Protocols.	pack_get_chars_list (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*获取角色列表*/	
function Protocols.unpack_get_chars_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*角色列表*/	
function Protocols.pack_chars_list ( list ,faction_name ,queen_name ,icon)
	local output = Packet.new(SMSG_CHARS_LIST)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	output:writeUTF(faction_name)
	output:writeUTF(queen_name)
	output:writeByte(icon)
	return output
end

-- /*角色列表*/	
function Protocols.call_chars_list ( playerInfo, list ,faction_name ,queen_name ,icon)
	local output = Protocols.	pack_chars_list ( list ,faction_name ,queen_name ,icon)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*角色列表*/	
function Protocols.unpack_chars_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = char_create_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end
	ret,param_table.faction_name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.queen_name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.icon = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*检查名字是否可以使用*/	
function Protocols.pack_check_name ( name)
	local output = Packet.new(CMSG_CHECK_NAME)
	output:writeUTF(name)
	return output
end

-- /*检查名字是否可以使用*/	
function Protocols.call_check_name ( playerInfo, name)
	local output = Protocols.	pack_check_name ( name)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*检查名字是否可以使用*/	
function Protocols.unpack_check_name (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*检查名字结果*/	
function Protocols.pack_check_name_result ( result)
	local output = Packet.new(SMSG_CHECK_NAME_RESULT)
	output:writeByte(result)
	return output
end

-- /*检查名字结果*/	
function Protocols.call_check_name_result ( playerInfo, result)
	local output = Protocols.	pack_check_name_result ( result)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*检查名字结果*/	
function Protocols.unpack_check_name_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.result = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*创建角色*/	
function Protocols.pack_char_create ( info)
	local output = Packet.new(CMSG_CHAR_CREATE)
	info :write(output)
	return output
end

-- /*创建角色*/	
function Protocols.call_char_create ( playerInfo, info)
	local output = Protocols.	pack_char_create ( info)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*创建角色*/	
function Protocols.unpack_char_create (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	param_table.info = char_create_info_t .new()
	if(param_table.info :read(input)==false)then
		return false
	end

	return true,param_table	

end


-- /*角色创建结果*/	
function Protocols.pack_char_create_result ( result)
	local output = Packet.new(SMSG_CHAR_CREATE_RESULT)
	output:writeByte(result)
	return output
end

-- /*角色创建结果*/	
function Protocols.call_char_create_result ( playerInfo, result)
	local output = Protocols.	pack_char_create_result ( result)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*角色创建结果*/	
function Protocols.unpack_char_create_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.result = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*删除角色*/	
function Protocols.pack_delete_char ( id)
	local output = Packet.new(CMSG_DELETE_CHAR)
	output:writeU32(id)
	return output
end

-- /*删除角色*/	
function Protocols.call_delete_char ( playerInfo, id)
	local output = Protocols.	pack_delete_char ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*删除角色*/	
function Protocols.unpack_delete_char (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*角色删除结果*/	
function Protocols.pack_delete_char_result ( result)
	local output = Packet.new(SMSG_DELETE_CHAR_RESULT)
	output:writeByte(result)
	return output
end

-- /*角色删除结果*/	
function Protocols.call_delete_char_result ( playerInfo, result)
	local output = Protocols.	pack_delete_char_result ( result)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*角色删除结果*/	
function Protocols.unpack_delete_char_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.result = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*玩家登录*/	
function Protocols.pack_player_login ( guid)
	local output = Packet.new(CMSG_PLAYER_LOGIN)
	output:writeUTF(guid)
	return output
end

-- /*玩家登录*/	
function Protocols.call_player_login ( playerInfo, guid)
	local output = Protocols.	pack_player_login ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*玩家登录*/	
function Protocols.unpack_player_login (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*玩家退出*/	
function Protocols.pack_player_logout (  )
	local output = Packet.new(CMSG_PLAYER_LOGOUT)
	return output
end

-- /*玩家退出*/	
function Protocols.call_player_logout ( playerInfo )
	local output = Protocols.	pack_player_logout (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*玩家退出*/	
function Protocols.unpack_player_logout (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*临时账号转正规*/	
function Protocols.pack_regularise_account ( uid)
	local output = Packet.new(CMSG_REGULARISE_ACCOUNT)
	output:writeUTF(uid)
	return output
end

-- /*临时账号转正规*/	
function Protocols.call_regularise_account ( playerInfo, uid)
	local output = Protocols.	pack_regularise_account ( uid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*临时账号转正规*/	
function Protocols.unpack_regularise_account (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.uid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*角色配置信息*/	
function Protocols.pack_char_remotestore ( key ,value)
	local output = Packet.new(CMSG_CHAR_REMOTESTORE)
	output:writeU32(key)
	output:writeU32(value)
	return output
end

-- /*角色配置信息*/	
function Protocols.call_char_remotestore ( playerInfo, key ,value)
	local output = Protocols.	pack_char_remotestore ( key ,value)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*角色配置信息*/	
function Protocols.unpack_char_remotestore (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.key = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.value = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*角色配置信息*/	
function Protocols.pack_char_remotestore_str ( key ,value)
	local output = Packet.new(CMSG_CHAR_REMOTESTORE_STR)
	output:writeU32(key)
	output:writeUTF(value)
	return output
end

-- /*角色配置信息*/	
function Protocols.call_char_remotestore_str ( playerInfo, key ,value)
	local output = Protocols.	pack_char_remotestore_str ( key ,value)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*角色配置信息*/	
function Protocols.unpack_char_remotestore_str (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.key = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.value = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*传送，如果是C->S，mapid变量请填成传送点ID*/	
function Protocols.pack_teleport ( intGuid)
	local output = Packet.new(CMSG_TELEPORT)
	output:writeU32(intGuid)
	return output
end

-- /*传送，如果是C->S，mapid变量请填成传送点ID*/	
function Protocols.call_teleport ( playerInfo, intGuid)
	local output = Protocols.	pack_teleport ( intGuid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*传送，如果是C->S，mapid变量请填成传送点ID*/	
function Protocols.unpack_teleport (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.intGuid = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*停止移动*/	
function Protocols.pack_move_stop ( guid ,pos_x ,pos_y)
	local output = Packet.new(MSG_MOVE_STOP)
	output:writeU32(guid)
	output:writeI16(pos_x)
	output:writeI16(pos_y)
	return output
end

-- /*停止移动*/	
function Protocols.call_move_stop ( playerInfo, guid ,pos_x ,pos_y)
	local output = Protocols.	pack_move_stop ( guid ,pos_x ,pos_y)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*停止移动*/	
function Protocols.unpack_move_stop (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.pos_x = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.pos_y = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*unit对象移动*/	
function Protocols.pack_unit_move ( guid ,pos_x ,pos_y ,path)
	local output = Packet.new(MSG_UNIT_MOVE)
	output:writeU32(guid)
	output:writeI16(pos_x)
	output:writeI16(pos_y)
	output:writeI16(#path)
	for i = 1,#path,1
	do
					output:writeByte(path[i])
			end
	return output
end

-- /*unit对象移动*/	
function Protocols.call_unit_move ( playerInfo, guid ,pos_x ,pos_y ,path)
	local output = Protocols.	pack_unit_move ( guid ,pos_x ,pos_y ,path)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*unit对象移动*/	
function Protocols.unpack_unit_move (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.pos_x = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.pos_y = input:readU16()
	if not ret then
		return false
	end
	param_table.path = {}
	ret,len = input:readU16()
	if not ret then
		return false
	end
	
	for i = 1,len,1
	do
					ret,param_table.path[i] = input:readByte()
				if not ret then
			return false
		end
	end

	return true,param_table	

end


-- /*使用游戏对象*/	
function Protocols.pack_use_gameobject ( target)
	local output = Packet.new(CMSG_USE_GAMEOBJECT)
	output:writeU32(target)
	return output
end

-- /*使用游戏对象*/	
function Protocols.call_use_gameobject ( playerInfo, target)
	local output = Protocols.	pack_use_gameobject ( target)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*使用游戏对象*/	
function Protocols.unpack_use_gameobject (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.target = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*包裹操作-交换位置*/	
function Protocols.pack_bag_exchange_pos ( src_bag ,src_pos ,dst_bag ,dst_pos)
	local output = Packet.new(CMSG_BAG_EXCHANGE_POS)
	output:writeU32(src_bag)
	output:writeU32(src_pos)
	output:writeU32(dst_bag)
	output:writeU32(dst_pos)
	return output
end

-- /*包裹操作-交换位置*/	
function Protocols.call_bag_exchange_pos ( playerInfo, src_bag ,src_pos ,dst_bag ,dst_pos)
	local output = Protocols.	pack_bag_exchange_pos ( src_bag ,src_pos ,dst_bag ,dst_pos)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*包裹操作-交换位置*/	
function Protocols.unpack_bag_exchange_pos (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.src_bag = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.src_pos = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.dst_bag = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.dst_pos = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*包裹操作-销毁物品*/	
function Protocols.pack_bag_destroy ( item_guid ,num ,bag_id)
	local output = Packet.new(CMSG_BAG_DESTROY)
	output:writeUTF(item_guid)
	output:writeU32(num)
	output:writeU32(bag_id)
	return output
end

-- /*包裹操作-销毁物品*/	
function Protocols.call_bag_destroy ( playerInfo, item_guid ,num ,bag_id)
	local output = Protocols.	pack_bag_destroy ( item_guid ,num ,bag_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*包裹操作-销毁物品*/	
function Protocols.unpack_bag_destroy (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.item_guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.num = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.bag_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*分割物品*/	
function Protocols.pack_bag_item_split ( bag_id ,src_pos ,count ,dst_pos ,dst_bag)
	local output = Packet.new(CMSG_BAG_ITEM_SPLIT)
	output:writeByte(bag_id)
	output:writeI16(src_pos)
	output:writeU32(count)
	output:writeI16(dst_pos)
	output:writeByte(dst_bag)
	return output
end

-- /*分割物品*/	
function Protocols.call_bag_item_split ( playerInfo, bag_id ,src_pos ,count ,dst_pos ,dst_bag)
	local output = Protocols.	pack_bag_item_split ( bag_id ,src_pos ,count ,dst_pos ,dst_bag)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*分割物品*/	
function Protocols.unpack_bag_item_split (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.bag_id = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.src_pos = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.dst_pos = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.dst_bag = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*使用物品*/	
function Protocols.pack_bag_item_user ( item_guid ,count)
	local output = Packet.new(CMSG_BAG_ITEM_USER)
	output:writeUTF(item_guid)
	output:writeU32(count)
	return output
end

-- /*使用物品*/	
function Protocols.call_bag_item_user ( playerInfo, item_guid ,count)
	local output = Protocols.	pack_bag_item_user ( item_guid ,count)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*使用物品*/	
function Protocols.unpack_bag_item_user (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.item_guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*下发物品冷却*/	
function Protocols.pack_bag_item_cooldown ( list)
	local output = Packet.new(SMSG_BAG_ITEM_COOLDOWN)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*下发物品冷却*/	
function Protocols.call_bag_item_cooldown ( playerInfo, list)
	local output = Protocols.	pack_bag_item_cooldown ( list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*下发物品冷却*/	
function Protocols.unpack_bag_item_cooldown (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = item_cooldown_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*grid中的unit移动整体打包*/	
function Protocols.pack_grid_unit_move (  )
	local output = Packet.new(SMSG_GRID_UNIT_MOVE)
	return output
end

-- /*grid中的unit移动整体打包*/	
function Protocols.call_grid_unit_move ( playerInfo )
	local output = Protocols.	pack_grid_unit_move (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*grid中的unit移动整体打包*/	
function Protocols.unpack_grid_unit_move (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*grid中的unit移动整体打包2*/	
function Protocols.pack_grid_unit_move_2 (  )
	local output = Packet.new(SMSG_GRID_UNIT_MOVE_2)
	return output
end

-- /*grid中的unit移动整体打包2*/	
function Protocols.call_grid_unit_move_2 ( playerInfo )
	local output = Protocols.	pack_grid_unit_move_2 (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*grid中的unit移动整体打包2*/	
function Protocols.unpack_grid_unit_move_2 (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*兑换物品*/	
function Protocols.pack_exchange_item ( entry ,count ,tar_entry)
	local output = Packet.new(CMSG_EXCHANGE_ITEM)
	output:writeU32(entry)
	output:writeU32(count)
	output:writeU32(tar_entry)
	return output
end

-- /*兑换物品*/	
function Protocols.call_exchange_item ( playerInfo, entry ,count ,tar_entry)
	local output = Protocols.	pack_exchange_item ( entry ,count ,tar_entry)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*兑换物品*/	
function Protocols.unpack_exchange_item (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.entry = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.tar_entry = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*背包扩展*/	
function Protocols.pack_bag_extension ( bag_id ,extension_type ,bag_pos)
	local output = Packet.new(CMSG_BAG_EXTENSION)
	output:writeByte(bag_id)
	output:writeByte(extension_type)
	output:writeU32(bag_pos)
	return output
end

-- /*背包扩展*/	
function Protocols.call_bag_extension ( playerInfo, bag_id ,extension_type ,bag_pos)
	local output = Protocols.	pack_bag_extension ( bag_id ,extension_type ,bag_pos)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*背包扩展*/	
function Protocols.unpack_bag_extension (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.bag_id = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.extension_type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.bag_pos = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*请求NPC商品列表*/	
function Protocols.pack_npc_get_goods_list ( npc_id)
	local output = Packet.new(CMSG_NPC_GET_GOODS_LIST)
	output:writeU32(npc_id)
	return output
end

-- /*请求NPC商品列表*/	
function Protocols.call_npc_get_goods_list ( playerInfo, npc_id)
	local output = Protocols.	pack_npc_get_goods_list ( npc_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求NPC商品列表*/	
function Protocols.unpack_npc_get_goods_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.npc_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*Npc商品列表*/	
function Protocols.pack_npc_goods_list ( goods ,npc_id)
	local output = Packet.new(SMSG_NPC_GOODS_LIST)
	output:writeI16(#goods)
	for i = 1,#goods,1
	do
					output:writeU32(goods[i])
			end
	output:writeU32(npc_id)
	return output
end

-- /*Npc商品列表*/	
function Protocols.call_npc_goods_list ( playerInfo, goods ,npc_id)
	local output = Protocols.	pack_npc_goods_list ( goods ,npc_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*Npc商品列表*/	
function Protocols.unpack_npc_goods_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	param_table.goods = {}
	ret,len = input:readU16()
	if not ret then
		return false
	end
	
	for i = 1,len,1
	do
					ret,param_table.goods[i] = input:readU32()
				if not ret then
			return false
		end
	end
	ret,param_table.npc_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*购买商品*/	
function Protocols.pack_store_buy ( id ,count)
	local output = Packet.new(CMSG_STORE_BUY)
	output:writeU32(id)
	output:writeU32(count)
	return output
end

-- /*购买商品*/	
function Protocols.call_store_buy ( playerInfo, id ,count)
	local output = Protocols.	pack_store_buy ( id ,count)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*购买商品*/	
function Protocols.unpack_store_buy (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*出售物品*/	
function Protocols.pack_npc_sell ( npc_id ,item_guid ,num)
	local output = Packet.new(CMSG_NPC_SELL)
	output:writeU32(npc_id)
	output:writeUTF(item_guid)
	output:writeU32(num)
	return output
end

-- /*出售物品*/	
function Protocols.call_npc_sell ( playerInfo, npc_id ,item_guid ,num)
	local output = Protocols.	pack_npc_sell ( npc_id ,item_guid ,num)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*出售物品*/	
function Protocols.unpack_npc_sell (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.npc_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.item_guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.num = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*回购物品*/	
function Protocols.pack_npc_repurchase ( item_id)
	local output = Packet.new(CMSG_NPC_REPURCHASE)
	output:writeUTF(item_id)
	return output
end

-- /*回购物品*/	
function Protocols.call_npc_repurchase ( playerInfo, item_id)
	local output = Protocols.	pack_npc_repurchase ( item_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*回购物品*/	
function Protocols.unpack_npc_repurchase (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.item_id = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*时装是否启用*/	
function Protocols.pack_avatar_fashion_enable ( pos)
	local output = Packet.new(CMSG_AVATAR_FASHION_ENABLE)
	output:writeByte(pos)
	return output
end

-- /*时装是否启用*/	
function Protocols.call_avatar_fashion_enable ( playerInfo, pos)
	local output = Protocols.	pack_avatar_fashion_enable ( pos)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*时装是否启用*/	
function Protocols.unpack_avatar_fashion_enable (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.pos = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*任务对话选项*/	
function Protocols.pack_questhelp_talk_option ( quest_id ,option_id ,value0 ,value1)
	local output = Packet.new(CMSG_QUESTHELP_TALK_OPTION)
	output:writeU32(quest_id)
	output:writeU32(option_id)
	output:writeI32(value0)
	output:writeI32(value1)
	return output
end

-- /*任务对话选项*/	
function Protocols.call_questhelp_talk_option ( playerInfo, quest_id ,option_id ,value0 ,value1)
	local output = Protocols.	pack_questhelp_talk_option ( quest_id ,option_id ,value0 ,value1)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*任务对话选项*/	
function Protocols.unpack_questhelp_talk_option (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.option_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.value0 = input:readI32()
	if not ret then
		return false
	end	
	ret,param_table.value1 = input:readI32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*与NPC对话获得传送点列表*/	
function Protocols.pack_taxi_hello ( guid)
	local output = Packet.new(CMSG_TAXI_HELLO)
	output:writeU32(guid)
	return output
end

-- /*与NPC对话获得传送点列表*/	
function Protocols.call_taxi_hello ( playerInfo, guid)
	local output = Protocols.	pack_taxi_hello ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*与NPC对话获得传送点列表*/	
function Protocols.unpack_taxi_hello (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*发送传送点列表*/	
function Protocols.pack_taxi_stations_list ( npcid ,stations)
	local output = Packet.new(SMSG_TAXI_STATIONS_LIST)
	output:writeU32(npcid)
	output:writeI16(#stations)
	for i = 1,#stations,1
	do
		stations[i]:write(output)
	end
	return output
end

-- /*发送传送点列表*/	
function Protocols.call_taxi_stations_list ( playerInfo, npcid ,stations)
	local output = Protocols.	pack_taxi_stations_list ( npcid ,stations)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*发送传送点列表*/	
function Protocols.unpack_taxi_stations_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.npcid = input:readU32()
	if not ret then
		return false
	end	
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.stations = {}
	for i = 1,len,1
	do
		local stru = taxi_menu_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.stations,stru)
	end

	return true,param_table	

end


-- /*选择传送点*/	
function Protocols.pack_taxi_select_station ( station_id ,guid)
	local output = Packet.new(CMSG_TAXI_SELECT_STATION)
	output:writeU32(station_id)
	output:writeU32(guid)
	return output
end

-- /*选择传送点*/	
function Protocols.call_taxi_select_station ( playerInfo, station_id ,guid)
	local output = Protocols.	pack_taxi_select_station ( station_id ,guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*选择传送点*/	
function Protocols.unpack_taxi_select_station (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.station_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*与NPC交流选择选项*/	
function Protocols.pack_gossip_select_option ( option ,guid ,unknow)
	local output = Packet.new(CMSG_GOSSIP_SELECT_OPTION)
	output:writeU32(option)
	output:writeU32(guid)
	output:writeUTF(unknow)
	return output
end

-- /*与NPC交流选择选项*/	
function Protocols.call_gossip_select_option ( playerInfo, option ,guid ,unknow)
	local output = Protocols.	pack_gossip_select_option ( option ,guid ,unknow)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*与NPC交流选择选项*/	
function Protocols.unpack_gossip_select_option (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.option = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.unknow = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*与NPC闲聊获取选项*/	
function Protocols.pack_gossip_hello ( guid)
	local output = Packet.new(CMSG_GOSSIP_HELLO)
	output:writeU32(guid)
	return output
end

-- /*与NPC闲聊获取选项*/	
function Protocols.call_gossip_hello ( playerInfo, guid)
	local output = Protocols.	pack_gossip_hello ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*与NPC闲聊获取选项*/	
function Protocols.unpack_gossip_hello (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*发送闲聊信息和选项*/	
function Protocols.pack_gossip_message ( npcid ,npc_entry ,option_id ,option_text ,gossip_items)
	local output = Packet.new(SMSG_GOSSIP_MESSAGE)
	output:writeU32(npcid)
	output:writeU32(npc_entry)
	output:writeU32(option_id)
	output:writeUTF(option_text)
	output:writeI16(#gossip_items)
	for i = 1,#gossip_items,1
	do
		gossip_items[i]:write(output)
	end
	return output
end

-- /*发送闲聊信息和选项*/	
function Protocols.call_gossip_message ( playerInfo, npcid ,npc_entry ,option_id ,option_text ,gossip_items)
	local output = Protocols.	pack_gossip_message ( npcid ,npc_entry ,option_id ,option_text ,gossip_items)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*发送闲聊信息和选项*/	
function Protocols.unpack_gossip_message (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.npcid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.npc_entry = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.option_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.option_text = input:readUTF()
	if not ret then
		return false
	end	
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.gossip_items = {}
	for i = 1,len,1
	do
		local stru = gossip_menu_option_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.gossip_items,stru)
	end

	return true,param_table	

end


-- /*任务发布者状态查询*/	
function Protocols.pack_questgiver_status_query ( guid)
	local output = Packet.new(CMSG_QUESTGIVER_STATUS_QUERY)
	output:writeU32(guid)
	return output
end

-- /*任务发布者状态查询*/	
function Protocols.call_questgiver_status_query ( playerInfo, guid)
	local output = Protocols.	pack_questgiver_status_query ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*任务发布者状态查询*/	
function Protocols.unpack_questgiver_status_query (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*任务NPC状态*/	
function Protocols.pack_questgiver_status ( guid ,status)
	local output = Packet.new(SMSG_QUESTGIVER_STATUS)
	output:writeU32(guid)
	output:writeByte(status)
	return output
end

-- /*任务NPC状态*/	
function Protocols.call_questgiver_status ( playerInfo, guid ,status)
	local output = Protocols.	pack_questgiver_status ( guid ,status)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*任务NPC状态*/	
function Protocols.unpack_questgiver_status (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.status = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*查询任务状态*/	
function Protocols.pack_query_quest_status ( quest_array)
	local output = Packet.new(MSG_QUERY_QUEST_STATUS)
	output:writeI16(#quest_array)
	for i = 1,#quest_array,1
	do
		quest_array[i]:write(output)
	end
	return output
end

-- /*查询任务状态*/	
function Protocols.call_query_quest_status ( playerInfo, quest_array)
	local output = Protocols.	pack_query_quest_status ( quest_array)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*查询任务状态*/	
function Protocols.unpack_query_quest_status (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.quest_array = {}
	for i = 1,len,1
	do
		local stru = quest_status_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.quest_array,stru)
	end

	return true,param_table	

end


-- /*可接任务*/	
function Protocols.pack_questhelp_get_canaccept_list (  )
	local output = Packet.new(CMSG_QUESTHELP_GET_CANACCEPT_LIST)
	return output
end

-- /*可接任务*/	
function Protocols.call_questhelp_get_canaccept_list ( playerInfo )
	local output = Protocols.	pack_questhelp_get_canaccept_list (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*可接任务*/	
function Protocols.unpack_questhelp_get_canaccept_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*下发可接任务列表*/	
function Protocols.pack_questhelp_canaccept_list ( quests)
	local output = Packet.new(SMSG_QUESTHELP_CANACCEPT_LIST)
	output:writeI16(#quests)
	for i = 1,#quests,1
	do
					output:writeU32(quests[i])
			end
	return output
end

-- /*下发可接任务列表*/	
function Protocols.call_questhelp_canaccept_list ( playerInfo, quests)
	local output = Protocols.	pack_questhelp_canaccept_list ( quests)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*下发可接任务列表*/	
function Protocols.unpack_questhelp_canaccept_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	param_table.quests = {}
	ret,len = input:readU16()
	if not ret then
		return false
	end
	
	for i = 1,len,1
	do
					ret,param_table.quests[i] = input:readU32()
				if not ret then
			return false
		end
	end

	return true,param_table	

end


-- /*任务失败*/	
function Protocols.pack_questupdate_faild ( reason)
	local output = Packet.new(SMSG_QUESTUPDATE_FAILD)
	output:writeByte(reason)
	return output
end

-- /*任务失败*/	
function Protocols.call_questupdate_faild ( playerInfo, reason)
	local output = Protocols.	pack_questupdate_faild ( reason)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*任务失败*/	
function Protocols.unpack_questupdate_faild (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.reason = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*任务条件完成*/	
function Protocols.pack_questupdate_complete ( quest_id)
	local output = Packet.new(SMSG_QUESTUPDATE_COMPLETE)
	output:writeU32(quest_id)
	return output
end

-- /*任务条件完成*/	
function Protocols.call_questupdate_complete ( playerInfo, quest_id)
	local output = Protocols.	pack_questupdate_complete ( quest_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*任务条件完成*/	
function Protocols.unpack_questupdate_complete (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*放弃任务*/	
function Protocols.pack_questlog_remove_quest ( slot ,reserve)
	local output = Packet.new(CMSG_QUESTLOG_REMOVE_QUEST)
	output:writeByte(slot)
	return output
end

-- /*放弃任务*/	
function Protocols.call_questlog_remove_quest ( playerInfo, slot ,reserve)
	local output = Protocols.	pack_questlog_remove_quest ( slot ,reserve)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*放弃任务*/	
function Protocols.unpack_questlog_remove_quest (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.slot = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*完成任务*/	
function Protocols.pack_questgiver_complete_quest ( guid ,quest_id ,reward)
	local output = Packet.new(CMSG_QUESTGIVER_COMPLETE_QUEST)
	output:writeU32(guid)
	output:writeU32(quest_id)
	output:writeByte(reward)
	return output
end

-- /*完成任务*/	
function Protocols.call_questgiver_complete_quest ( playerInfo, guid ,quest_id ,reward)
	local output = Protocols.	pack_questgiver_complete_quest ( guid ,quest_id ,reward)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*完成任务*/	
function Protocols.unpack_questgiver_complete_quest (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.reward = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*完成任务后通知任务链的下个任务*/	
function Protocols.pack_questhelp_next ( current_id ,next_id ,guid)
	local output = Packet.new(SMSG_QUESTHELP_NEXT)
	output:writeU32(current_id)
	output:writeU32(next_id)
	output:writeU32(guid)
	return output
end

-- /*完成任务后通知任务链的下个任务*/	
function Protocols.call_questhelp_next ( playerInfo, current_id ,next_id ,guid)
	local output = Protocols.	pack_questhelp_next ( current_id ,next_id ,guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*完成任务后通知任务链的下个任务*/	
function Protocols.unpack_questhelp_next (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.current_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.next_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*任务系统强制完成任务*/	
function Protocols.pack_questhelp_complete ( quest_id ,quest_statue ,reserve)
	local output = Packet.new(CMSG_QUESTHELP_COMPLETE)
	output:writeU32(quest_id)
	output:writeByte(quest_statue)
	output:writeByte(reserve)
	return output
end

-- /*任务系统强制完成任务*/	
function Protocols.call_questhelp_complete ( playerInfo, quest_id ,quest_statue ,reserve)
	local output = Protocols.	pack_questhelp_complete ( quest_id ,quest_statue ,reserve)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*任务系统强制完成任务*/	
function Protocols.unpack_questhelp_complete (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.quest_statue = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.reserve = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*接受任务成功*/	
function Protocols.pack_questupdate_accept ( quest_id)
	local output = Packet.new(SMSG_QUESTUPDATE_ACCEPT)
	output:writeU32(quest_id)
	return output
end

-- /*接受任务成功*/	
function Protocols.call_questupdate_accept ( playerInfo, quest_id)
	local output = Protocols.	pack_questupdate_accept ( quest_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*接受任务成功*/	
function Protocols.unpack_questupdate_accept (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*更新任务进度_下标数量*/	
function Protocols.pack_questhelp_update_status ( quest_id ,slot_id ,num)
	local output = Packet.new(CMSG_QUESTHELP_UPDATE_STATUS)
	output:writeU32(quest_id)
	output:writeU32(slot_id)
	output:writeU32(num)
	return output
end

-- /*更新任务进度_下标数量*/	
function Protocols.call_questhelp_update_status ( playerInfo, quest_id ,slot_id ,num)
	local output = Protocols.	pack_questhelp_update_status ( quest_id ,slot_id ,num)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*更新任务进度_下标数量*/	
function Protocols.unpack_questhelp_update_status (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.slot_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.num = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*任务已完成*/	
function Protocols.pack_questgetter_complete ( quest_id)
	local output = Packet.new(SMSG_QUESTGETTER_COMPLETE)
	output:writeU32(quest_id)
	return output
end

-- /*任务已完成*/	
function Protocols.call_questgetter_complete ( playerInfo, quest_id)
	local output = Protocols.	pack_questgetter_complete ( quest_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*任务已完成*/	
function Protocols.unpack_questgetter_complete (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*接任务*/	
function Protocols.pack_questgiver_accept_quest ( npcid ,quest_id)
	local output = Packet.new(CMSG_QUESTGIVER_ACCEPT_QUEST)
	output:writeU32(npcid)
	output:writeU32(quest_id)
	return output
end

-- /*接任务*/	
function Protocols.call_questgiver_accept_quest ( playerInfo, npcid ,quest_id)
	local output = Protocols.	pack_questgiver_accept_quest ( npcid ,quest_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*接任务*/	
function Protocols.unpack_questgiver_accept_quest (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.npcid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*任务使用物品*/	
function Protocols.pack_questupdate_use_item ( item_id)
	local output = Packet.new(CMSG_QUESTUPDATE_USE_ITEM)
	output:writeU32(item_id)
	return output
end

-- /*任务使用物品*/	
function Protocols.call_questupdate_use_item ( playerInfo, item_id)
	local output = Protocols.	pack_questupdate_use_item ( item_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*任务使用物品*/	
function Protocols.unpack_questupdate_use_item (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.item_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*查询天书任务*/	
function Protocols.pack_questhelp_query_book ( dynasty)
	local output = Packet.new(CMSG_QUESTHELP_QUERY_BOOK)
	output:writeU32(dynasty)
	return output
end

-- /*查询天书任务*/	
function Protocols.call_questhelp_query_book ( playerInfo, dynasty)
	local output = Protocols.	pack_questhelp_query_book ( dynasty)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*查询天书任务*/	
function Protocols.unpack_questhelp_query_book (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.dynasty = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*下发可接天书任务*/	
function Protocols.pack_questhelp_book_quest ( quest_id)
	local output = Packet.new(SMSG_QUESTHELP_BOOK_QUEST)
	output:writeU32(quest_id)
	return output
end

-- /*下发可接天书任务*/	
function Protocols.call_questhelp_book_quest ( playerInfo, quest_id)
	local output = Protocols.	pack_questhelp_book_quest ( quest_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*下发可接天书任务*/	
function Protocols.unpack_questhelp_book_quest (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.quest_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*玩家使用游戏对象以后的动作*/	
function Protocols.pack_use_gameobject_action ( player_id ,gameobject_id)
	local output = Packet.new(SMSG_USE_GAMEOBJECT_ACTION)
	output:writeU32(player_id)
	output:writeU32(gameobject_id)
	return output
end

-- /*玩家使用游戏对象以后的动作*/	
function Protocols.call_use_gameobject_action ( playerInfo, player_id ,gameobject_id)
	local output = Protocols.	pack_use_gameobject_action ( player_id ,gameobject_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*玩家使用游戏对象以后的动作*/	
function Protocols.unpack_use_gameobject_action (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.player_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.gameobject_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*设置攻击模式*/	
function Protocols.pack_set_attack_mode ( mode ,reserve)
	local output = Packet.new(CMSG_SET_ATTACK_MODE)
	output:writeByte(mode)
	return output
end

-- /*设置攻击模式*/	
function Protocols.call_set_attack_mode ( playerInfo, mode ,reserve)
	local output = Protocols.	pack_set_attack_mode ( mode ,reserve)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*设置攻击模式*/	
function Protocols.unpack_set_attack_mode (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.mode = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*选择目标*/	
function Protocols.pack_select_target ( id)
	local output = Packet.new(MSG_SELECT_TARGET)
	output:writeU32(id)
	return output
end

-- /*选择目标*/	
function Protocols.call_select_target ( playerInfo, id)
	local output = Protocols.	pack_select_target ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*选择目标*/	
function Protocols.unpack_select_target (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*进入战斗*/	
function Protocols.pack_combat_state_update ( cur_state)
	local output = Packet.new(SMSG_COMBAT_STATE_UPDATE)
	output:writeByte(cur_state)
	return output
end

-- /*进入战斗*/	
function Protocols.call_combat_state_update ( playerInfo, cur_state)
	local output = Protocols.	pack_combat_state_update ( cur_state)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*进入战斗*/	
function Protocols.unpack_combat_state_update (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.cur_state = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*经验更新*/	
function Protocols.pack_exp_update ( exp ,type ,vip_exp)
	local output = Packet.new(SMSG_EXP_UPDATE)
	output:writeI32(exp)
	output:writeByte(type)
	output:writeI32(vip_exp)
	return output
end

-- /*经验更新*/	
function Protocols.call_exp_update ( playerInfo, exp ,type ,vip_exp)
	local output = Protocols.	pack_exp_update ( exp ,type ,vip_exp)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*经验更新*/	
function Protocols.unpack_exp_update (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.exp = input:readI32()
	if not ret then
		return false
	end	
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.vip_exp = input:readI32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*客户端释放技能*/	
function Protocols.pack_spell_start ( spell_id ,target_pos_x ,target_pos_y ,caster ,target)
	local output = Packet.new(MSG_SPELL_START)
	output:writeU32(spell_id)
	output:writeI16(target_pos_x)
	output:writeI16(target_pos_y)
	output:writeU32(caster)
	output:writeU32(target)
	return output
end

-- /*客户端释放技能*/	
function Protocols.call_spell_start ( playerInfo, spell_id ,target_pos_x ,target_pos_y ,caster ,target)
	local output = Protocols.	pack_spell_start ( spell_id ,target_pos_x ,target_pos_y ,caster ,target)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*客户端释放技能*/	
function Protocols.unpack_spell_start (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.spell_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.target_pos_x = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.target_pos_y = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.caster = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.target = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*施法停止*/	
function Protocols.pack_spell_stop ( guid)
	local output = Packet.new(MSG_SPELL_STOP)
	output:writeUTF(guid)
	return output
end

-- /*施法停止*/	
function Protocols.call_spell_stop ( playerInfo, guid)
	local output = Protocols.	pack_spell_stop ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*施法停止*/	
function Protocols.unpack_spell_stop (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*跳*/	
function Protocols.pack_jump ( guid ,pos_x ,pos_y)
	local output = Packet.new(MSG_JUMP)
	output:writeU32(guid)
	output:writeFloat(pos_x)
	output:writeFloat(pos_y)
	return output
end

-- /*跳*/	
function Protocols.call_jump ( playerInfo, guid ,pos_x ,pos_y)
	local output = Protocols.	pack_jump ( guid ,pos_x ,pos_y)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*跳*/	
function Protocols.unpack_jump (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.pos_x = input:readFloat()
	if not ret then
		return false
	end
	ret,param_table.pos_y = input:readFloat()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*复活*/	
function Protocols.pack_resurrection ( type ,reserve)
	local output = Packet.new(CMSG_RESURRECTION)
	output:writeByte(type)
	return output
end

-- /*复活*/	
function Protocols.call_resurrection ( playerInfo, type ,reserve)
	local output = Protocols.	pack_resurrection ( type ,reserve)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*复活*/	
function Protocols.unpack_resurrection (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*交易发出请求*/	
function Protocols.pack_trade_request ( guid)
	local output = Packet.new(MSG_TRADE_REQUEST)
	output:writeUTF(guid)
	return output
end

-- /*交易发出请求*/	
function Protocols.call_trade_request ( playerInfo, guid)
	local output = Protocols.	pack_trade_request ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*交易发出请求*/	
function Protocols.unpack_trade_request (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*交易请求答复*/	
function Protocols.pack_trade_reply ( guid ,reply)
	local output = Packet.new(MSG_TRADE_REPLY)
	output:writeUTF(guid)
	output:writeByte(reply)
	return output
end

-- /*交易请求答复*/	
function Protocols.call_trade_reply ( playerInfo, guid ,reply)
	local output = Protocols.	pack_trade_reply ( guid ,reply)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*交易请求答复*/	
function Protocols.unpack_trade_reply (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.reply = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*交易开始*/	
function Protocols.pack_trade_start ( guid)
	local output = Packet.new(SMSG_TRADE_START)
	output:writeUTF(guid)
	return output
end

-- /*交易开始*/	
function Protocols.call_trade_start ( playerInfo, guid)
	local output = Protocols.	pack_trade_start ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*交易开始*/	
function Protocols.unpack_trade_start (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*交易确认物品*/	
function Protocols.pack_trade_decide_items ( items ,gold_ingot ,silver)
	local output = Packet.new(MSG_TRADE_DECIDE_ITEMS)
	output:writeI16(items)
	for i = 1,#items,1
	do
		output:writeUTF(items[i])
	end
	output:writeI32(gold_ingot)
	output:writeI32(silver)
	return output
end

-- /*交易确认物品*/	
function Protocols.call_trade_decide_items ( playerInfo, items ,gold_ingot ,silver)
	local output = Protocols.	pack_trade_decide_items ( items ,gold_ingot ,silver)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*交易确认物品*/	
function Protocols.unpack_trade_decide_items (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.items = {}
	for i = 1,len,1
	do
		ret, str = input:readUTF()
		if not ret then
			return false
		end		
		table.insert(param_table.items,str)
	end
	ret,param_table.gold_ingot = input:readI32()
	if not ret then
		return false
	end	
	ret,param_table.silver = input:readI32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*交易完成*/	
function Protocols.pack_trade_finish (  )
	local output = Packet.new(SMSG_TRADE_FINISH)
	return output
end

-- /*交易完成*/	
function Protocols.call_trade_finish ( playerInfo )
	local output = Protocols.	pack_trade_finish (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*交易完成*/	
function Protocols.unpack_trade_finish (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*交易取消*/	
function Protocols.pack_trade_cancel (  )
	local output = Packet.new(MSG_TRADE_CANCEL)
	return output
end

-- /*交易取消*/	
function Protocols.call_trade_cancel ( playerInfo )
	local output = Protocols.	pack_trade_cancel (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*交易取消*/	
function Protocols.unpack_trade_cancel (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*交易准备好*/	
function Protocols.pack_trade_ready (  )
	local output = Packet.new(MSG_TRADE_READY)
	return output
end

-- /*交易准备好*/	
function Protocols.call_trade_ready ( playerInfo )
	local output = Protocols.	pack_trade_ready (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*交易准备好*/	
function Protocols.unpack_trade_ready (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*生物讲话*/	
function Protocols.pack_chat_unit_talk ( guid ,content ,say)
	local output = Packet.new(SMSG_CHAT_UNIT_TALK)
	output:writeU32(guid)
	output:writeU32(content)
	output:writeUTF(say)
	return output
end

-- /*生物讲话*/	
function Protocols.call_chat_unit_talk ( playerInfo, guid ,content ,say)
	local output = Protocols.	pack_chat_unit_talk ( guid ,content ,say)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*生物讲话*/	
function Protocols.unpack_chat_unit_talk (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.content = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.say = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*就近聊天*/	
function Protocols.pack_chat_near ( content)
	local output = Packet.new(CMSG_CHAT_NEAR)
	output:writeUTF(content)
	return output
end

-- /*就近聊天*/	
function Protocols.call_chat_near ( playerInfo, content)
	local output = Protocols.	pack_chat_near ( content)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*就近聊天*/	
function Protocols.unpack_chat_near (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*私聊*/	
function Protocols.pack_chat_whisper ( guid ,content)
	local output = Packet.new(CMSG_CHAT_WHISPER)
	output:writeUTF(guid)
	output:writeUTF(content)
	return output
end

-- /*私聊*/	
function Protocols.call_chat_whisper ( playerInfo, guid ,content)
	local output = Protocols.	pack_chat_whisper ( guid ,content)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*私聊*/	
function Protocols.unpack_chat_whisper (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*阵营聊天*/	
function Protocols.pack_chat_faction ( guid ,name ,content ,faction)
	local output = Packet.new(MSG_CHAT_FACTION)
	output:writeUTF(guid)
	output:writeUTF(name)
	output:writeUTF(content)
	output:writeByte(faction)
	return output
end

-- /*阵营聊天*/	
function Protocols.call_chat_faction ( playerInfo, guid ,name ,content ,faction)
	local output = Protocols.	pack_chat_faction ( guid ,name ,content ,faction)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*阵营聊天*/	
function Protocols.unpack_chat_faction (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.faction = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*世界*/	
function Protocols.pack_chat_world ( guid ,faction ,name ,content)
	local output = Packet.new(MSG_CHAT_WORLD)
	output:writeUTF(guid)
	output:writeByte(faction)
	output:writeUTF(name)
	output:writeUTF(content)
	return output
end

-- /*世界*/	
function Protocols.call_chat_world ( playerInfo, guid ,faction ,name ,content)
	local output = Protocols.	pack_chat_world ( guid ,faction ,name ,content)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*世界*/	
function Protocols.unpack_chat_world (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.faction = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*喇叭*/	
function Protocols.pack_chat_horn ( guid ,faction ,name ,content)
	local output = Packet.new(MSG_CHAT_HORN)
	output:writeUTF(guid)
	output:writeByte(faction)
	output:writeUTF(name)
	output:writeUTF(content)
	return output
end

-- /*喇叭*/	
function Protocols.call_chat_horn ( playerInfo, guid ,faction ,name ,content)
	local output = Protocols.	pack_chat_horn ( guid ,faction ,name ,content)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*喇叭*/	
function Protocols.unpack_chat_horn (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.faction = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*公告*/	
function Protocols.pack_chat_notice ( id ,content ,data)
	local output = Packet.new(MSG_CHAT_NOTICE)
	output:writeU32(id)
	output:writeUTF(content)
	output:writeUTF(data)
	return output
end

-- /*公告*/	
function Protocols.call_chat_notice ( playerInfo, id ,content ,data)
	local output = Protocols.	pack_chat_notice ( id ,content ,data)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*公告*/	
function Protocols.unpack_chat_notice (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.data = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*查询玩家信息*/	
function Protocols.pack_query_player_info ( guid ,flag ,callback_id)
	local output = Packet.new(CMSG_QUERY_PLAYER_INFO)
	output:writeUTF(guid)
	output:writeU32(flag)
	output:writeU32(callback_id)
	return output
end

-- /*查询玩家信息*/	
function Protocols.call_query_player_info ( playerInfo, guid ,flag ,callback_id)
	local output = Protocols.	pack_query_player_info ( guid ,flag ,callback_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*查询玩家信息*/	
function Protocols.unpack_query_player_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.flag = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.callback_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*查询信息对象更新*/	
function Protocols.pack_query_result_update_object (  )
	local output = Packet.new(SMSG_QUERY_RESULT_UPDATE_OBJECT)
	return output
end

-- /*查询信息对象更新*/	
function Protocols.call_query_result_update_object ( playerInfo )
	local output = Protocols.	pack_query_result_update_object (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*查询信息对象更新*/	
function Protocols.unpack_query_result_update_object (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*领取礼包*/	
function Protocols.pack_receive_gift_packs (  )
	local output = Packet.new(CMSG_RECEIVE_GIFT_PACKS)
	return output
end

-- /*领取礼包*/	
function Protocols.call_receive_gift_packs ( playerInfo )
	local output = Protocols.	pack_receive_gift_packs (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取礼包*/	
function Protocols.unpack_receive_gift_packs (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*地图对象更新*/	
function Protocols.pack_map_update_object (  )
	local output = Packet.new(SMSG_MAP_UPDATE_OBJECT)
	return output
end

-- /*地图对象更新*/	
function Protocols.call_map_update_object ( playerInfo )
	local output = Protocols.	pack_map_update_object (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*地图对象更新*/	
function Protocols.unpack_map_update_object (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*战斗信息binlog*/	
function Protocols.pack_fighting_info_update_object (  )
	local output = Packet.new(SMSG_FIGHTING_INFO_UPDATE_OBJECT)
	return output
end

-- /*战斗信息binlog*/	
function Protocols.call_fighting_info_update_object ( playerInfo )
	local output = Protocols.	pack_fighting_info_update_object (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*战斗信息binlog*/	
function Protocols.unpack_fighting_info_update_object (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*战斗信息binlog*/	
function Protocols.pack_fighting_info_update_object_2 (  )
	local output = Packet.new(SMSG_FIGHTING_INFO_UPDATE_OBJECT_2)
	return output
end

-- /*战斗信息binlog*/	
function Protocols.call_fighting_info_update_object_2 ( playerInfo )
	local output = Protocols.	pack_fighting_info_update_object_2 (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*战斗信息binlog*/	
function Protocols.unpack_fighting_info_update_object_2 (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*进入副本*/	
function Protocols.pack_instance_enter ( instance_id)
	local output = Packet.new(CMSG_INSTANCE_ENTER)
	output:writeU32(instance_id)
	return output
end

-- /*进入副本*/	
function Protocols.call_instance_enter ( playerInfo, instance_id)
	local output = Protocols.	pack_instance_enter ( instance_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*进入副本*/	
function Protocols.unpack_instance_enter (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.instance_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*向服务端发送副本进入下一阶段指令*/	
function Protocols.pack_instance_next_state ( level ,param)
	local output = Packet.new(CMSG_INSTANCE_NEXT_STATE)
	output:writeI16(level)
	output:writeU32(param)
	return output
end

-- /*向服务端发送副本进入下一阶段指令*/	
function Protocols.call_instance_next_state ( playerInfo, level ,param)
	local output = Protocols.	pack_instance_next_state ( level ,param)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*向服务端发送副本进入下一阶段指令*/	
function Protocols.unpack_instance_next_state (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.level = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.param = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*副本退出*/	
function Protocols.pack_instance_exit ( reserve)
	local output = Packet.new(CMSG_INSTANCE_EXIT)
	output:writeU32(reserve)
	return output
end

-- /*副本退出*/	
function Protocols.call_instance_exit ( playerInfo, reserve)
	local output = Protocols.	pack_instance_exit ( reserve)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*副本退出*/	
function Protocols.unpack_instance_exit (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.reserve = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*限时活动领取*/	
function Protocols.pack_limit_activity_receive ( id ,type)
	local output = Packet.new(CMSG_LIMIT_ACTIVITY_RECEIVE)
	output:writeU32(id)
	output:writeU32(type)
	return output
end

-- /*限时活动领取*/	
function Protocols.call_limit_activity_receive ( playerInfo, id ,type)
	local output = Protocols.	pack_limit_activity_receive ( id ,type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*限时活动领取*/	
function Protocols.unpack_limit_activity_receive (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.type = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*杀人啦~~！！！！*/	
function Protocols.pack_kill_man ( killer ,killer_name ,victim ,victim_name)
	local output = Packet.new(SMSG_KILL_MAN)
	output:writeUTF(killer)
	output:writeUTF(killer_name)
	output:writeUTF(victim)
	output:writeUTF(victim_name)
	return output
end

-- /*杀人啦~~！！！！*/	
function Protocols.call_kill_man ( playerInfo, killer ,killer_name ,victim ,victim_name)
	local output = Protocols.	pack_kill_man ( killer ,killer_name ,victim ,victim_name)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*杀人啦~~！！！！*/	
function Protocols.unpack_kill_man (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.killer = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.killer_name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.victim = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.victim_name = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*玩家升级*/	
function Protocols.pack_player_upgrade ( guid)
	local output = Packet.new(SMSG_PLAYER_UPGRADE)
	output:writeU32(guid)
	return output
end

-- /*玩家升级*/	
function Protocols.call_player_upgrade ( playerInfo, guid)
	local output = Protocols.	pack_player_upgrade ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*玩家升级*/	
function Protocols.unpack_player_upgrade (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*仓库存钱*/	
function Protocols.pack_warehouse_save_money ( money ,money_gold ,money_bills)
	local output = Packet.new(CMSG_WAREHOUSE_SAVE_MONEY)
	output:writeI32(money)
	output:writeI32(money_gold)
	output:writeI32(money_bills)
	return output
end

-- /*仓库存钱*/	
function Protocols.call_warehouse_save_money ( playerInfo, money ,money_gold ,money_bills)
	local output = Protocols.	pack_warehouse_save_money ( money ,money_gold ,money_bills)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*仓库存钱*/	
function Protocols.unpack_warehouse_save_money (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.money = input:readI32()
	if not ret then
		return false
	end	
	ret,param_table.money_gold = input:readI32()
	if not ret then
		return false
	end	
	ret,param_table.money_bills = input:readI32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*仓库取钱*/	
function Protocols.pack_warehouse_take_money ( money ,money_gold ,money_bills)
	local output = Packet.new(CMSG_WAREHOUSE_TAKE_MONEY)
	output:writeI32(money)
	output:writeI32(money_gold)
	output:writeI32(money_bills)
	return output
end

-- /*仓库取钱*/	
function Protocols.call_warehouse_take_money ( playerInfo, money ,money_gold ,money_bills)
	local output = Protocols.	pack_warehouse_take_money ( money ,money_gold ,money_bills)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*仓库取钱*/	
function Protocols.unpack_warehouse_take_money (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.money = input:readI32()
	if not ret then
		return false
	end	
	ret,param_table.money_gold = input:readI32()
	if not ret then
		return false
	end	
	ret,param_table.money_bills = input:readI32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*使用元宝操作某事*/	
function Protocols.pack_use_gold_opt ( type ,param)
	local output = Packet.new(CMSG_USE_GOLD_OPT)
	output:writeByte(type)
	output:writeUTF(param)
	return output
end

-- /*使用元宝操作某事*/	
function Protocols.call_use_gold_opt ( playerInfo, type ,param)
	local output = Protocols.	pack_use_gold_opt ( type ,param)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*使用元宝操作某事*/	
function Protocols.unpack_use_gold_opt (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.param = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*使用铜钱操作某事*/	
function Protocols.pack_use_silver_opt ( type)
	local output = Packet.new(CMSG_USE_SILVER_OPT)
	output:writeByte(type)
	return output
end

-- /*使用铜钱操作某事*/	
function Protocols.call_use_silver_opt ( playerInfo, type)
	local output = Protocols.	pack_use_silver_opt ( type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*使用铜钱操作某事*/	
function Protocols.unpack_use_silver_opt (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*后台弹窗*/	
function Protocols.pack_gm_rightfloat ( id ,end_time ,zone1 ,zone2 ,zone3 ,subject ,content ,link ,mode)
	local output = Packet.new(SMSG_GM_RIGHTFLOAT)
	output:writeU32(id)
	output:writeU32(end_time)
	output:writeU32(zone1)
	output:writeU32(zone2)
	output:writeU32(zone3)
	output:writeUTF(subject)
	output:writeUTF(content)
	output:writeUTF(link)
	output:writeByte(mode)
	return output
end

-- /*后台弹窗*/	
function Protocols.call_gm_rightfloat ( playerInfo, id ,end_time ,zone1 ,zone2 ,zone3 ,subject ,content ,link ,mode)
	local output = Protocols.	pack_gm_rightfloat ( id ,end_time ,zone1 ,zone2 ,zone3 ,subject ,content ,link ,mode)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*后台弹窗*/	
function Protocols.unpack_gm_rightfloat (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.end_time = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.zone1 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.zone2 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.zone3 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.subject = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.link = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.mode = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*删除某条后台弹窗*/	
function Protocols.pack_del_gm_rightfloat ( id)
	local output = Packet.new(SMSG_DEL_GM_RIGHTFLOAT)
	output:writeU32(id)
	return output
end

-- /*删除某条后台弹窗*/	
function Protocols.call_del_gm_rightfloat ( playerInfo, id)
	local output = Protocols.	pack_del_gm_rightfloat ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*删除某条后台弹窗*/	
function Protocols.unpack_del_gm_rightfloat (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*应用服同步时间*/	
function Protocols.pack_sync_mstime_app ( mstime_now ,time_now ,open_time)
	local output = Packet.new(MSG_SYNC_MSTIME_APP)
	output:writeU32(mstime_now)
	output:writeU32(time_now)
	output:writeU32(open_time)
	return output
end

-- /*应用服同步时间*/	
function Protocols.call_sync_mstime_app ( playerInfo, mstime_now ,time_now ,open_time)
	local output = Protocols.	pack_sync_mstime_app ( mstime_now ,time_now ,open_time)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*应用服同步时间*/	
function Protocols.unpack_sync_mstime_app (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.mstime_now = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.time_now = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.open_time = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*玩家打开某个窗口*/	
function Protocols.pack_open_window ( window_type)
	local output = Packet.new(CMSG_OPEN_WINDOW)
	output:writeU32(window_type)
	return output
end

-- /*玩家打开某个窗口*/	
function Protocols.call_open_window ( playerInfo, window_type)
	local output = Protocols.	pack_open_window ( window_type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*玩家打开某个窗口*/	
function Protocols.unpack_open_window (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.window_type = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*禁言操作*/	
function Protocols.pack_player_gag ( player_id ,end_time ,content)
	local output = Packet.new(CMSG_PLAYER_GAG)
	output:writeUTF(player_id)
	output:writeU32(end_time)
	output:writeUTF(content)
	return output
end

-- /*禁言操作*/	
function Protocols.call_player_gag ( playerInfo, player_id ,end_time ,content)
	local output = Protocols.	pack_player_gag ( player_id ,end_time ,content)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*禁言操作*/	
function Protocols.unpack_player_gag (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.player_id = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.end_time = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*踢人操作*/	
function Protocols.pack_player_kicking ( player_id)
	local output = Packet.new(CMSG_PLAYER_KICKING)
	output:writeUTF(player_id)
	return output
end

-- /*踢人操作*/	
function Protocols.call_player_kicking ( playerInfo, player_id)
	local output = Protocols.	pack_player_kicking ( player_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*踢人操作*/	
function Protocols.unpack_player_kicking (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.player_id = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*合服通知*/	
function Protocols.pack_merge_server_msg ( merge_host ,merge_port ,merge_key ,merge_type ,reserve ,reserve2)
	local output = Packet.new(SMSG_MERGE_SERVER_MSG)
	output:writeUTF(merge_host)
	output:writeU32(merge_port)
	output:writeUTF(merge_key)
	output:writeU32(merge_type)
	output:writeU32(reserve)
	output:writeU32(reserve2)
	return output
end

-- /*合服通知*/	
function Protocols.call_merge_server_msg ( playerInfo, merge_host ,merge_port ,merge_key ,merge_type ,reserve ,reserve2)
	local output = Protocols.	pack_merge_server_msg ( merge_host ,merge_port ,merge_key ,merge_type ,reserve ,reserve2)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*合服通知*/	
function Protocols.unpack_merge_server_msg (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.merge_host = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.merge_port = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.merge_key = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.merge_type = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.reserve = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.reserve2 = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*获取排行信息*/	
function Protocols.pack_rank_list_query ( call_back_id ,rank_list_type ,start_index ,end_index)
	local output = Packet.new(CMSG_RANK_LIST_QUERY)
	output:writeU32(call_back_id)
	output:writeByte(rank_list_type)
	output:writeI16(start_index)
	output:writeI16(end_index)
	return output
end

-- /*获取排行信息*/	
function Protocols.call_rank_list_query ( playerInfo, call_back_id ,rank_list_type ,start_index ,end_index)
	local output = Protocols.	pack_rank_list_query ( call_back_id ,rank_list_type ,start_index ,end_index)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*获取排行信息*/	
function Protocols.unpack_rank_list_query (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.call_back_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.rank_list_type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.start_index = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.end_index = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*客户端获取排行榜返回结果*/	
function Protocols.pack_rank_list_query_result (  )
	local output = Packet.new(SMSG_RANK_LIST_QUERY_RESULT)
	return output
end

-- /*客户端获取排行榜返回结果*/	
function Protocols.call_rank_list_query_result ( playerInfo )
	local output = Protocols.	pack_rank_list_query_result (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*客户端获取排行榜返回结果*/	
function Protocols.unpack_rank_list_query_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*客户端热更场景模块后获取uint*/	
function Protocols.pack_client_update_scened (  )
	local output = Packet.new(CMSG_CLIENT_UPDATE_SCENED)
	return output
end

-- /*客户端热更场景模块后获取uint*/	
function Protocols.call_client_update_scened ( playerInfo )
	local output = Protocols.	pack_client_update_scened (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*客户端热更场景模块后获取uint*/	
function Protocols.unpack_client_update_scened (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*数值包*/	
function Protocols.pack_num_lua (  )
	local output = Packet.new(SMSG_NUM_LUA)
	return output
end

-- /*数值包*/	
function Protocols.call_num_lua ( playerInfo )
	local output = Protocols.	pack_num_lua (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*数值包*/	
function Protocols.unpack_num_lua (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*战利品拾取*/	
function Protocols.pack_loot_select ( x ,y)
	local output = Packet.new(CMSG_LOOT_SELECT)
	output:writeI16(x)
	output:writeI16(y)
	return output
end

-- /*战利品拾取*/	
function Protocols.call_loot_select ( playerInfo, x ,y)
	local output = Protocols.	pack_loot_select ( x ,y)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*战利品拾取*/	
function Protocols.unpack_loot_select (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.x = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.y = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*通知登录服把玩家传回游戏服*/	
function Protocols.pack_goback_to_game_server (  )
	local output = Packet.new(CMSG_GOBACK_TO_GAME_SERVER)
	return output
end

-- /*通知登录服把玩家传回游戏服*/	
function Protocols.call_goback_to_game_server ( playerInfo )
	local output = Protocols.	pack_goback_to_game_server (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*通知登录服把玩家传回游戏服*/	
function Protocols.unpack_goback_to_game_server (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*客户端把比赛人员数据传给比赛服*/	
function Protocols.pack_world_war_CS_player_info (  )
	local output = Packet.new(CMSG_WORLD_WAR_CS_PLAYER_INFO)
	return output
end

-- /*客户端把比赛人员数据传给比赛服*/	
function Protocols.call_world_war_CS_player_info ( playerInfo )
	local output = Protocols.	pack_world_war_CS_player_info (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*客户端把比赛人员数据传给比赛服*/	
function Protocols.unpack_world_war_CS_player_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*玩家加入或者离开某服务器*/	
function Protocols.pack_join_or_leave_server ( join_or_leave ,server_type ,server_pid ,server_fd ,time)
	local output = Packet.new(SMSG_JOIN_OR_LEAVE_SERVER)
	output:writeByte(join_or_leave)
	output:writeByte(server_type)
	output:writeU32(server_pid)
	output:writeU32(server_fd)
	output:writeU32(time)
	return output
end

-- /*玩家加入或者离开某服务器*/	
function Protocols.call_join_or_leave_server ( playerInfo, join_or_leave ,server_type ,server_pid ,server_fd ,time)
	local output = Protocols.	pack_join_or_leave_server ( join_or_leave ,server_type ,server_pid ,server_fd ,time)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*玩家加入或者离开某服务器*/	
function Protocols.unpack_join_or_leave_server (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.join_or_leave = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.server_type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.server_pid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.server_fd = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.time = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*客户端请求跨服人员数据*/	
function Protocols.pack_world_war_SC_player_info (  )
	local output = Packet.new(MSG_WORLD_WAR_SC_PLAYER_INFO)
	return output
end

-- /*客户端请求跨服人员数据*/	
function Protocols.call_world_war_SC_player_info ( playerInfo )
	local output = Protocols.	pack_world_war_SC_player_info (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*客户端请求跨服人员数据*/	
function Protocols.unpack_world_war_SC_player_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*客户端订阅信息*/	
function Protocols.pack_clientSubscription ( guid)
	local output = Packet.new(MSG_CLIENTSUBSCRIPTION)
	output:writeU32(guid)
	return output
end

-- /*客户端订阅信息*/	
function Protocols.call_clientSubscription ( playerInfo, guid)
	local output = Protocols.	pack_clientSubscription ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*客户端订阅信息*/	
function Protocols.unpack_clientSubscription (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*服务端下发lua脚本*/	
function Protocols.pack_lua_script (  )
	local output = Packet.new(SMSG_LUA_SCRIPT)
	return output
end

-- /*服务端下发lua脚本*/	
function Protocols.call_lua_script ( playerInfo )
	local output = Protocols.	pack_lua_script (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*服务端下发lua脚本*/	
function Protocols.unpack_lua_script (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*角色更改信息*/	
function Protocols.pack_char_update_info ( info)
	local output = Packet.new(CMSG_CHAR_UPDATE_INFO)
	info :write(output)
	return output
end

-- /*角色更改信息*/	
function Protocols.call_char_update_info ( playerInfo, info)
	local output = Protocols.	pack_char_update_info ( info)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*角色更改信息*/	
function Protocols.unpack_char_update_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	param_table.info = char_create_info_t .new()
	if(param_table.info :read(input)==false)then
		return false
	end

	return true,param_table	

end


-- /*通知客户端观察者的视角*/	
function Protocols.pack_notice_watcher_map_info ( wather_guid ,map_id ,instance_id)
	local output = Packet.new(SMSG_NOTICE_WATCHER_MAP_INFO)
	output:writeUTF(wather_guid)
	output:writeU32(map_id)
	output:writeU32(instance_id)
	return output
end

-- /*通知客户端观察者的视角*/	
function Protocols.call_notice_watcher_map_info ( playerInfo, wather_guid ,map_id ,instance_id)
	local output = Protocols.	pack_notice_watcher_map_info ( wather_guid ,map_id ,instance_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*通知客户端观察者的视角*/	
function Protocols.unpack_notice_watcher_map_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.wather_guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.map_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.instance_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*客户端订阅对象信息*/	
function Protocols.pack_modify_watch ( opt ,cid ,key)
	local output = Packet.new(CMSG_MODIFY_WATCH)
	output:writeByte(opt)
	output:writeU32(cid)
	output:writeUTF(key)
	return output
end

-- /*客户端订阅对象信息*/	
function Protocols.call_modify_watch ( playerInfo, opt ,cid ,key)
	local output = Protocols.	pack_modify_watch ( opt ,cid ,key)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*客户端订阅对象信息*/	
function Protocols.unpack_modify_watch (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.opt = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.cid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.key = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*跨服传送*/	
function Protocols.pack_kuafu_chuansong ( str_data ,watcher_guid ,reserve)
	local output = Packet.new(CMSG_KUAFU_CHUANSONG)
	output:writeUTF(str_data)
	output:writeUTF(watcher_guid)
	output:writeU32(reserve)
	return output
end

-- /*跨服传送*/	
function Protocols.call_kuafu_chuansong ( playerInfo, str_data ,watcher_guid ,reserve)
	local output = Protocols.	pack_kuafu_chuansong ( str_data ,watcher_guid ,reserve)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*跨服传送*/	
function Protocols.unpack_kuafu_chuansong (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.str_data = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.watcher_guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.reserve = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*显示当前装备*/	
function Protocols.pack_show_suit ( position)
	local output = Packet.new(CMSG_SHOW_SUIT)
	output:writeByte(position)
	return output
end

-- /*显示当前装备*/	
function Protocols.call_show_suit ( playerInfo, position)
	local output = Protocols.	pack_show_suit ( position)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*显示当前装备*/	
function Protocols.unpack_show_suit (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.position = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*显示当前坐标*/	
function Protocols.pack_show_position ( channel)
	local output = Packet.new(CMSG_SHOW_POSITION)
	output:writeByte(channel)
	return output
end

-- /*显示当前坐标*/	
function Protocols.call_show_position ( playerInfo, channel)
	local output = Protocols.	pack_show_position ( channel)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*显示当前坐标*/	
function Protocols.unpack_show_position (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.channel = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*元宝复活*/	
function Protocols.pack_gold_respawn ( useGold)
	local output = Packet.new(CMSG_GOLD_RESPAWN)
	output:writeByte(useGold)
	return output
end

-- /*元宝复活*/	
function Protocols.call_gold_respawn ( playerInfo, useGold)
	local output = Protocols.	pack_gold_respawn ( useGold)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*元宝复活*/	
function Protocols.unpack_gold_respawn (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.useGold = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*野外死亡倒计时*/	
function Protocols.pack_field_death_cooldown ( type ,value ,killername ,cooldown ,params)
	local output = Packet.new(SMSG_FIELD_DEATH_COOLDOWN)
	output:writeByte(type)
	output:writeU32(value)
	output:writeUTF(killername)
	output:writeI16(cooldown)
	output:writeUTF(params)
	return output
end

-- /*野外死亡倒计时*/	
function Protocols.call_field_death_cooldown ( playerInfo, type ,value ,killername ,cooldown ,params)
	local output = Protocols.	pack_field_death_cooldown ( type ,value ,killername ,cooldown ,params)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*野外死亡倒计时*/	
function Protocols.unpack_field_death_cooldown (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.value = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.killername = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.cooldown = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.params = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*商城购买*/	
function Protocols.pack_mall_buy ( id ,count ,time)
	local output = Packet.new(CMSG_MALL_BUY)
	output:writeU32(id)
	output:writeU32(count)
	output:writeU32(time)
	return output
end

-- /*商城购买*/	
function Protocols.call_mall_buy ( playerInfo, id ,count ,time)
	local output = Protocols.	pack_mall_buy ( id ,count ,time)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*商城购买*/	
function Protocols.unpack_mall_buy (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.time = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*强化*/	
function Protocols.pack_strength ( part)
	local output = Packet.new(CMSG_STRENGTH)
	output:writeByte(part)
	return output
end

-- /*强化*/	
function Protocols.call_strength ( playerInfo, part)
	local output = Protocols.	pack_strength ( part)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*强化*/	
function Protocols.unpack_strength (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.part = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*强化成功*/	
function Protocols.pack_strength_success ( level)
	local output = Packet.new(SMSG_STRENGTH_SUCCESS)
	output:writeI16(level)
	return output
end

-- /*强化成功*/	
function Protocols.call_strength_success ( playerInfo, level)
	local output = Protocols.	pack_strength_success ( level)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*强化成功*/	
function Protocols.unpack_strength_success (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.level = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*强制进入*/	
function Protocols.pack_forceInto (  )
	local output = Packet.new(CMSG_FORCEINTO)
	return output
end

-- /*强制进入*/	
function Protocols.call_forceInto ( playerInfo )
	local output = Protocols.	pack_forceInto (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*强制进入*/	
function Protocols.unpack_forceInto (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*创建帮派*/	
function Protocols.pack_create_faction ( name ,icon)
	local output = Packet.new(CMSG_CREATE_FACTION)
	output:writeUTF(name)
	output:writeByte(icon)
	return output
end

-- /*创建帮派*/	
function Protocols.call_create_faction ( playerInfo, name ,icon)
	local output = Protocols.	pack_create_faction ( name ,icon)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*创建帮派*/	
function Protocols.unpack_create_faction (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.icon = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*升级帮派*/	
function Protocols.pack_faction_upgrade (  )
	local output = Packet.new(CMSG_FACTION_UPGRADE)
	return output
end

-- /*升级帮派*/	
function Protocols.call_faction_upgrade ( playerInfo )
	local output = Protocols.	pack_faction_upgrade (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*升级帮派*/	
function Protocols.unpack_faction_upgrade (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*申请加入帮派*/	
function Protocols.pack_faction_join ( id)
	local output = Packet.new(CMSG_FACTION_JOIN)
	output:writeUTF(id)
	return output
end

-- /*申请加入帮派*/	
function Protocols.call_faction_join ( playerInfo, id)
	local output = Protocols.	pack_faction_join ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请加入帮派*/	
function Protocols.unpack_faction_join (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*申请升级技能*/	
function Protocols.pack_raise_base_spell ( raiseType ,spellId)
	local output = Packet.new(CMSG_RAISE_BASE_SPELL)
	output:writeByte(raiseType)
	output:writeI16(spellId)
	return output
end

-- /*申请升级技能*/	
function Protocols.call_raise_base_spell ( playerInfo, raiseType ,spellId)
	local output = Protocols.	pack_raise_base_spell ( raiseType ,spellId)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请升级技能*/	
function Protocols.unpack_raise_base_spell (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.raiseType = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.spellId = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*申请升阶愤怒技能*/	
function Protocols.pack_upgrade_anger_spell ( spellId)
	local output = Packet.new(CMSG_UPGRADE_ANGER_SPELL)
	output:writeI16(spellId)
	return output
end

-- /*申请升阶愤怒技能*/	
function Protocols.call_upgrade_anger_spell ( playerInfo, spellId)
	local output = Protocols.	pack_upgrade_anger_spell ( spellId)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请升阶愤怒技能*/	
function Protocols.unpack_upgrade_anger_spell (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.spellId = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*申请升级坐骑*/	
function Protocols.pack_raise_mount (  )
	local output = Packet.new(CMSG_RAISE_MOUNT)
	return output
end

-- /*申请升级坐骑*/	
function Protocols.call_raise_mount ( playerInfo )
	local output = Protocols.	pack_raise_mount (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请升级坐骑*/	
function Protocols.unpack_raise_mount (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*申请升阶坐骑*/	
function Protocols.pack_upgrade_mount ( useItem)
	local output = Packet.new(CMSG_UPGRADE_MOUNT)
	output:writeByte(useItem)
	return output
end

-- /*申请升阶坐骑*/	
function Protocols.call_upgrade_mount ( playerInfo, useItem)
	local output = Protocols.	pack_upgrade_mount ( useItem)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请升阶坐骑*/	
function Protocols.unpack_upgrade_mount (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.useItem = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*申请一键升阶坐骑*/	
function Protocols.pack_upgrade_mount_one_step ( useItem)
	local output = Packet.new(CMSG_UPGRADE_MOUNT_ONE_STEP)
	output:writeByte(useItem)
	return output
end

-- /*申请一键升阶坐骑*/	
function Protocols.call_upgrade_mount_one_step ( playerInfo, useItem)
	local output = Protocols.	pack_upgrade_mount_one_step ( useItem)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请一键升阶坐骑*/	
function Protocols.unpack_upgrade_mount_one_step (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.useItem = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*申请解锁幻化坐骑*/	
function Protocols.pack_illusion_mount_active ( illuId)
	local output = Packet.new(CMSG_ILLUSION_MOUNT_ACTIVE)
	output:writeI16(illuId)
	return output
end

-- /*申请解锁幻化坐骑*/	
function Protocols.call_illusion_mount_active ( playerInfo, illuId)
	local output = Protocols.	pack_illusion_mount_active ( illuId)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请解锁幻化坐骑*/	
function Protocols.unpack_illusion_mount_active (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.illuId = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*申请幻化坐骑*/	
function Protocols.pack_illusion_mount ( illuId)
	local output = Packet.new(CMSG_ILLUSION_MOUNT)
	output:writeI16(illuId)
	return output
end

-- /*申请幻化坐骑*/	
function Protocols.call_illusion_mount ( playerInfo, illuId)
	local output = Protocols.	pack_illusion_mount ( illuId)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请幻化坐骑*/	
function Protocols.unpack_illusion_mount (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.illuId = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*申请骑乘*/	
function Protocols.pack_ride_mount (  )
	local output = Packet.new(CMSG_RIDE_MOUNT)
	return output
end

-- /*申请骑乘*/	
function Protocols.call_ride_mount ( playerInfo )
	local output = Protocols.	pack_ride_mount (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*申请骑乘*/	
function Protocols.unpack_ride_mount (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*grid中的unit跳跃*/	
function Protocols.pack_grid_unit_jump (  )
	local output = Packet.new(SMSG_GRID_UNIT_JUMP)
	return output
end

-- /*grid中的unit跳跃*/	
function Protocols.call_grid_unit_jump ( playerInfo )
	local output = Protocols.	pack_grid_unit_jump (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*grid中的unit跳跃*/	
function Protocols.unpack_grid_unit_jump (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*宝石*/	
function Protocols.pack_gem ( part)
	local output = Packet.new(CMSG_GEM)
	output:writeByte(part)
	return output
end

-- /*宝石*/	
function Protocols.call_gem ( playerInfo, part)
	local output = Protocols.	pack_gem ( part)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*宝石*/	
function Protocols.unpack_gem (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.part = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*请求切换模式*/	
function Protocols.pack_change_battle_mode ( mode)
	local output = Packet.new(CMSG_CHANGE_BATTLE_MODE)
	output:writeByte(mode)
	return output
end

-- /*请求切换模式*/	
function Protocols.call_change_battle_mode ( playerInfo, mode)
	local output = Protocols.	pack_change_battle_mode ( mode)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求切换模式*/	
function Protocols.unpack_change_battle_mode (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.mode = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*和平模式CD*/	
function Protocols.pack_peace_mode_cd ( mode)
	local output = Packet.new(SMSG_PEACE_MODE_CD)
	output:writeByte(mode)
	return output
end

-- /*和平模式CD*/	
function Protocols.call_peace_mode_cd ( playerInfo, mode)
	local output = Protocols.	pack_peace_mode_cd ( mode)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*和平模式CD*/	
function Protocols.unpack_peace_mode_cd (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.mode = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*激活神兵*/	
function Protocols.pack_divine_active ( id)
	local output = Packet.new(CMSG_DIVINE_ACTIVE)
	output:writeByte(id)
	return output
end

-- /*激活神兵*/	
function Protocols.call_divine_active ( playerInfo, id)
	local output = Protocols.	pack_divine_active ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*激活神兵*/	
function Protocols.unpack_divine_active (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*激活神兵*/	
function Protocols.pack_divine_uplev ( id)
	local output = Packet.new(CMSG_DIVINE_UPLEV)
	output:writeByte(id)
	return output
end

-- /*激活神兵*/	
function Protocols.call_divine_uplev ( playerInfo, id)
	local output = Protocols.	pack_divine_uplev ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*激活神兵*/	
function Protocols.unpack_divine_uplev (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*切换神兵*/	
function Protocols.pack_divine_switch ( id)
	local output = Packet.new(CMSG_DIVINE_SWITCH)
	output:writeByte(id)
	return output
end

-- /*切换神兵*/	
function Protocols.call_divine_switch ( playerInfo, id)
	local output = Protocols.	pack_divine_switch ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*切换神兵*/	
function Protocols.unpack_divine_switch (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*请求跳跃*/	
function Protocols.pack_jump_start ( pos_x ,pos_y)
	local output = Packet.new(CMSG_JUMP_START)
	output:writeI16(pos_x)
	output:writeI16(pos_y)
	return output
end

-- /*请求跳跃*/	
function Protocols.call_jump_start ( playerInfo, pos_x ,pos_y)
	local output = Protocols.	pack_jump_start ( pos_x ,pos_y)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求跳跃*/	
function Protocols.unpack_jump_start (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.pos_x = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.pos_y = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*请求进入vip副本*/	
function Protocols.pack_enter_vip_instance ( id ,hard)
	local output = Packet.new(CMSG_ENTER_VIP_INSTANCE)
	output:writeI16(id)
	output:writeByte(hard)
	return output
end

-- /*请求进入vip副本*/	
function Protocols.call_enter_vip_instance ( playerInfo, id ,hard)
	local output = Protocols.	pack_enter_vip_instance ( id ,hard)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求进入vip副本*/	
function Protocols.unpack_enter_vip_instance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.hard = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*请求扫荡vip副本*/	
function Protocols.pack_sweep_vip_instance ( id)
	local output = Packet.new(CMSG_SWEEP_VIP_INSTANCE)
	output:writeI16(id)
	return output
end

-- /*请求扫荡vip副本*/	
function Protocols.call_sweep_vip_instance ( playerInfo, id)
	local output = Protocols.	pack_sweep_vip_instance ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求扫荡vip副本*/	
function Protocols.unpack_sweep_vip_instance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*进行挂机*/	
function Protocols.pack_hang_up (  )
	local output = Packet.new(CMSG_HANG_UP)
	return output
end

-- /*进行挂机*/	
function Protocols.call_hang_up ( playerInfo )
	local output = Protocols.	pack_hang_up (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*进行挂机*/	
function Protocols.unpack_hang_up (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*进行挂机设置*/	
function Protocols.pack_hang_up_setting ( value0 ,value1 ,value2 ,value3)
	local output = Packet.new(CMSG_HANG_UP_SETTING)
	output:writeU32(value0)
	output:writeU32(value1)
	output:writeU32(value2)
	output:writeU32(value3)
	return output
end

-- /*进行挂机设置*/	
function Protocols.call_hang_up_setting ( playerInfo, value0 ,value1 ,value2 ,value3)
	local output = Protocols.	pack_hang_up_setting ( value0 ,value1 ,value2 ,value3)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*进行挂机设置*/	
function Protocols.unpack_hang_up_setting (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.value0 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.value1 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.value2 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.value3 = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*请求进入试炼塔副本*/	
function Protocols.pack_enter_trial_instance (  )
	local output = Packet.new(CMSG_ENTER_TRIAL_INSTANCE)
	return output
end

-- /*请求进入试炼塔副本*/	
function Protocols.call_enter_trial_instance ( playerInfo )
	local output = Protocols.	pack_enter_trial_instance (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求进入试炼塔副本*/	
function Protocols.unpack_enter_trial_instance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*扫荡试炼塔副本*/	
function Protocols.pack_sweep_trial_instance (  )
	local output = Packet.new(CMSG_SWEEP_TRIAL_INSTANCE)
	return output
end

-- /*扫荡试炼塔副本*/	
function Protocols.call_sweep_trial_instance ( playerInfo )
	local output = Protocols.	pack_sweep_trial_instance (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*扫荡试炼塔副本*/	
function Protocols.unpack_sweep_trial_instance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*重置试炼塔*/	
function Protocols.pack_reset_trial_instance (  )
	local output = Packet.new(CMSG_RESET_TRIAL_INSTANCE)
	return output
end

-- /*重置试炼塔*/	
function Protocols.call_reset_trial_instance ( playerInfo )
	local output = Protocols.	pack_reset_trial_instance (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*重置试炼塔*/	
function Protocols.unpack_reset_trial_instance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*扫荡副本奖励*/	
function Protocols.pack_sweep_instance_reward ( inst_sub_type ,data1 ,data2 ,data3 ,list)
	local output = Packet.new(SMSG_SWEEP_INSTANCE_REWARD)
	output:writeByte(inst_sub_type)
	output:writeByte(data1)
	output:writeByte(data2)
	output:writeByte(data3)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*扫荡副本奖励*/	
function Protocols.call_sweep_instance_reward ( playerInfo, inst_sub_type ,data1 ,data2 ,data3 ,list)
	local output = Protocols.	pack_sweep_instance_reward ( inst_sub_type ,data1 ,data2 ,data3 ,list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*扫荡副本奖励*/	
function Protocols.unpack_sweep_instance_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.inst_sub_type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.data1 = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.data2 = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.data3 = input:readByte()
	if not ret then
		return false
	end
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = item_reward_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*重进副本*/	
function Protocols.pack_reenter_instance (  )
	local output = Packet.new(CMSG_REENTER_INSTANCE)
	return output
end

-- /*重进副本*/	
function Protocols.call_reenter_instance ( playerInfo )
	local output = Protocols.	pack_reenter_instance (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*重进副本*/	
function Protocols.unpack_reenter_instance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*走马灯信息*/	
function Protocols.pack_merry_go_round (  )
	local output = Packet.new(SMSG_MERRY_GO_ROUND)
	return output
end

-- /*走马灯信息*/	
function Protocols.call_merry_go_round ( playerInfo )
	local output = Protocols.	pack_merry_go_round (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*走马灯信息*/	
function Protocols.unpack_merry_go_round (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*添加好友*/	
function Protocols.pack_social_add_friend ( guid)
	local output = Packet.new(CMSG_SOCIAL_ADD_FRIEND)
	output:writeUTF(guid)
	return output
end

-- /*添加好友*/	
function Protocols.call_social_add_friend ( playerInfo, guid)
	local output = Protocols.	pack_social_add_friend ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*添加好友*/	
function Protocols.unpack_social_add_friend (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*同意添加好友*/	
function Protocols.pack_social_sureadd_friend ( guid)
	local output = Packet.new(CMSG_SOCIAL_SUREADD_FRIEND)
	output:writeUTF(guid)
	return output
end

-- /*同意添加好友*/	
function Protocols.call_social_sureadd_friend ( playerInfo, guid)
	local output = Protocols.	pack_social_sureadd_friend ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*同意添加好友*/	
function Protocols.unpack_social_sureadd_friend (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*赠送礼物*/	
function Protocols.pack_social_gift_friend ( guid ,gift)
	local output = Packet.new(CMSG_SOCIAL_GIFT_FRIEND)
	output:writeUTF(guid)
	output:writeI16(#gift)
	for i = 1,#gift,1
	do
		gift[i]:write(output)
	end
	return output
end

-- /*赠送礼物*/	
function Protocols.call_social_gift_friend ( playerInfo, guid ,gift)
	local output = Protocols.	pack_social_gift_friend ( guid ,gift)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*赠送礼物*/	
function Protocols.unpack_social_gift_friend (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.gift = {}
	for i = 1,len,1
	do
		local stru = item_reward_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.gift,stru)
	end

	return true,param_table	

end


-- /*推荐好友列表*/	
function Protocols.pack_social_recommend_friend (  )
	local output = Packet.new(CMSG_SOCIAL_RECOMMEND_FRIEND)
	return output
end

-- /*推荐好友列表*/	
function Protocols.call_social_recommend_friend ( playerInfo )
	local output = Protocols.	pack_social_recommend_friend (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*推荐好友列表*/	
function Protocols.unpack_social_recommend_friend (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*推荐好友列表*/	
function Protocols.pack_social_get_recommend_friend ( list)
	local output = Packet.new(SMSG_SOCIAL_GET_RECOMMEND_FRIEND)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*推荐好友列表*/	
function Protocols.call_social_get_recommend_friend ( playerInfo, list)
	local output = Protocols.	pack_social_get_recommend_friend ( list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*推荐好友列表*/	
function Protocols.unpack_social_get_recommend_friend (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = social_friend_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*复仇*/	
function Protocols.pack_social_revenge_enemy ( guid)
	local output = Packet.new(CMSG_SOCIAL_REVENGE_ENEMY)
	output:writeUTF(guid)
	return output
end

-- /*复仇*/	
function Protocols.call_social_revenge_enemy ( playerInfo, guid)
	local output = Protocols.	pack_social_revenge_enemy ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*复仇*/	
function Protocols.unpack_social_revenge_enemy (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*删除好友*/	
function Protocols.pack_social_del_friend ( guid)
	local output = Packet.new(CMSG_SOCIAL_DEL_FRIEND)
	output:writeUTF(guid)
	return output
end

-- /*删除好友*/	
function Protocols.call_social_del_friend ( playerInfo, guid)
	local output = Protocols.	pack_social_del_friend ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*删除好友*/	
function Protocols.unpack_social_del_friend (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*回城*/	
function Protocols.pack_teleport_main_city (  )
	local output = Packet.new(CMSG_TELEPORT_MAIN_CITY)
	return output
end

-- /*回城*/	
function Protocols.call_teleport_main_city ( playerInfo )
	local output = Protocols.	pack_teleport_main_city (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*回城*/	
function Protocols.unpack_teleport_main_city (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*不同频道聊天*/	
function Protocols.pack_chat_by_channel ( channel ,content)
	local output = Packet.new(CMSG_CHAT_BY_CHANNEL)
	output:writeByte(channel)
	output:writeUTF(content)
	return output
end

-- /*不同频道聊天*/	
function Protocols.call_chat_by_channel ( playerInfo, channel ,content)
	local output = Protocols.	pack_chat_by_channel ( channel ,content)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*不同频道聊天*/	
function Protocols.unpack_chat_by_channel (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.channel = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*发送聊天*/	
function Protocols.pack_send_chat ( channel ,guid ,title ,name ,vip ,zs ,lvl ,gender ,content ,to_guid ,faction_guid)
	local output = Packet.new(SMSG_SEND_CHAT)
	output:writeByte(channel)
	output:writeUTF(guid)
	output:writeByte(title)
	output:writeUTF(name)
	output:writeByte(vip)
	output:writeByte(zs)
	output:writeByte(lvl)
	output:writeByte(gender)
	output:writeUTF(content)
	output:writeUTF(to_guid)
	output:writeUTF(faction_guid)
	return output
end

-- /*发送聊天*/	
function Protocols.call_send_chat ( playerInfo, channel ,guid ,title ,name ,vip ,zs ,lvl ,gender ,content ,to_guid ,faction_guid)
	local output = Protocols.	pack_send_chat ( channel ,guid ,title ,name ,vip ,zs ,lvl ,gender ,content ,to_guid ,faction_guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*发送聊天*/	
function Protocols.unpack_send_chat (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.channel = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.title = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.vip = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.zs = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.lvl = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.gender = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.content = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.to_guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.faction_guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*清空申请列表*/	
function Protocols.pack_social_clear_apply (  )
	local output = Packet.new(CMSG_SOCIAL_CLEAR_APPLY)
	return output
end

-- /*清空申请列表*/	
function Protocols.call_social_clear_apply ( playerInfo )
	local output = Protocols.	pack_social_clear_apply (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*清空申请列表*/	
function Protocols.unpack_social_clear_apply (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*设置拒绝接受消息*/	
function Protocols.pack_msg_decline ( value0 ,value1)
	local output = Packet.new(CMSG_MSG_DECLINE)
	output:writeU32(value0)
	output:writeU32(value1)
	return output
end

-- /*设置拒绝接受消息*/	
function Protocols.call_msg_decline ( playerInfo, value0 ,value1)
	local output = Protocols.	pack_msg_decline ( value0 ,value1)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*设置拒绝接受消息*/	
function Protocols.unpack_msg_decline (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.value0 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.value1 = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*帮派列表*/	
function Protocols.pack_faction_get_list_result ( list ,curpag ,maxpag)
	local output = Packet.new(SMSG_FACTION_GET_LIST_RESULT)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	output:writeByte(curpag)
	output:writeByte(maxpag)
	return output
end

-- /*帮派列表*/	
function Protocols.call_faction_get_list_result ( playerInfo, list ,curpag ,maxpag)
	local output = Protocols.	pack_faction_get_list_result ( list ,curpag ,maxpag)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*帮派列表*/	
function Protocols.unpack_faction_get_list_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = faction_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end
	ret,param_table.curpag = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.maxpag = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*获取帮派列表*/	
function Protocols.pack_faction_getlist ( page ,num ,grep)
	local output = Packet.new(CMSG_FACTION_GETLIST)
	output:writeByte(page)
	output:writeByte(num)
	output:writeByte(grep)
	return output
end

-- /*获取帮派列表*/	
function Protocols.call_faction_getlist ( playerInfo, page ,num ,grep)
	local output = Protocols.	pack_faction_getlist ( page ,num ,grep)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*获取帮派列表*/	
function Protocols.unpack_faction_getlist (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.page = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.num = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.grep = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*帮派管理*/	
function Protocols.pack_faction_manager ( opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	local output = Packet.new(CMSG_FACTION_MANAGER)
	output:writeByte(opt_type)
	output:writeI16(reserve_int1)
	output:writeI16(reserve_int2)
	output:writeUTF(reserve_str1)
	output:writeUTF(reserve_str2)
	return output
end

-- /*帮派管理*/	
function Protocols.call_faction_manager ( playerInfo, opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	local output = Protocols.	pack_faction_manager ( opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*帮派管理*/	
function Protocols.unpack_faction_manager (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.opt_type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.reserve_int1 = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.reserve_int2 = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.reserve_str1 = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.reserve_str2 = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*帮派成员操作*/	
function Protocols.pack_faction_member_operate ( opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	local output = Packet.new(CMSG_FACTION_MEMBER_OPERATE)
	output:writeByte(opt_type)
	output:writeI16(reserve_int1)
	output:writeI16(reserve_int2)
	output:writeUTF(reserve_str1)
	output:writeUTF(reserve_str2)
	return output
end

-- /*帮派成员操作*/	
function Protocols.call_faction_member_operate ( playerInfo, opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	local output = Protocols.	pack_faction_member_operate ( opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*帮派成员操作*/	
function Protocols.unpack_faction_member_operate (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.opt_type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.reserve_int1 = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.reserve_int2 = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.reserve_str1 = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.reserve_str2 = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*快速加入帮派*/	
function Protocols.pack_faction_fast_join (  )
	local output = Packet.new(CMSG_FACTION_FAST_JOIN)
	return output
end

-- /*快速加入帮派*/	
function Protocols.call_faction_fast_join ( playerInfo )
	local output = Protocols.	pack_faction_fast_join (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*快速加入帮派*/	
function Protocols.unpack_faction_fast_join (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*通过名字添加好友*/	
function Protocols.pack_social_add_friend_byname ( name)
	local output = Packet.new(CMSG_SOCIAL_ADD_FRIEND_BYNAME)
	output:writeUTF(name)
	return output
end

-- /*通过名字添加好友*/	
function Protocols.call_social_add_friend_byname ( playerInfo, name)
	local output = Protocols.	pack_social_add_friend_byname ( name)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*通过名字添加好友*/	
function Protocols.unpack_social_add_friend_byname (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*读邮件*/	
function Protocols.pack_read_mail ( indx)
	local output = Packet.new(CMSG_READ_MAIL)
	output:writeI16(indx)
	return output
end

-- /*读邮件*/	
function Protocols.call_read_mail ( playerInfo, indx)
	local output = Protocols.	pack_read_mail ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*读邮件*/	
function Protocols.unpack_read_mail (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*领取邮件*/	
function Protocols.pack_pick_mail ( indx)
	local output = Packet.new(CMSG_PICK_MAIL)
	output:writeI16(indx)
	return output
end

-- /*领取邮件*/	
function Protocols.call_pick_mail ( playerInfo, indx)
	local output = Protocols.	pack_pick_mail ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取邮件*/	
function Protocols.unpack_pick_mail (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*删除邮件*/	
function Protocols.pack_remove_mail ( indx)
	local output = Packet.new(CMSG_REMOVE_MAIL)
	output:writeI16(indx)
	return output
end

-- /*删除邮件*/	
function Protocols.call_remove_mail ( playerInfo, indx)
	local output = Protocols.	pack_remove_mail ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*删除邮件*/	
function Protocols.unpack_remove_mail (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*一键领取邮件*/	
function Protocols.pack_pick_mail_one_step (  )
	local output = Packet.new(CMSG_PICK_MAIL_ONE_STEP)
	return output
end

-- /*一键领取邮件*/	
function Protocols.call_pick_mail_one_step ( playerInfo )
	local output = Protocols.	pack_pick_mail_one_step (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*一键领取邮件*/	
function Protocols.unpack_pick_mail_one_step (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*一键删除邮件*/	
function Protocols.pack_remove_mail_one_step (  )
	local output = Packet.new(CMSG_REMOVE_MAIL_ONE_STEP)
	return output
end

-- /*一键删除邮件*/	
function Protocols.call_remove_mail_one_step ( playerInfo )
	local output = Protocols.	pack_remove_mail_one_step (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*一键删除邮件*/	
function Protocols.unpack_remove_mail_one_step (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*屏蔽某人*/	
function Protocols.pack_block_chat ( guid)
	local output = Packet.new(CMSG_BLOCK_CHAT)
	output:writeUTF(guid)
	return output
end

-- /*屏蔽某人*/	
function Protocols.call_block_chat ( playerInfo, guid)
	local output = Protocols.	pack_block_chat ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*屏蔽某人*/	
function Protocols.unpack_block_chat (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*取消屏蔽*/	
function Protocols.pack_cancel_block_chat ( indx)
	local output = Packet.new(CMSG_CANCEL_BLOCK_CHAT)
	output:writeByte(indx)
	return output
end

-- /*取消屏蔽*/	
function Protocols.call_cancel_block_chat ( playerInfo, indx)
	local output = Protocols.	pack_cancel_block_chat ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*取消屏蔽*/	
function Protocols.unpack_cancel_block_chat (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*使用需要广播的游戏对象*/	
function Protocols.pack_use_broadcast_gameobject ( target)
	local output = Packet.new(CMSG_USE_BROADCAST_GAMEOBJECT)
	output:writeU32(target)
	return output
end

-- /*使用需要广播的游戏对象*/	
function Protocols.call_use_broadcast_gameobject ( playerInfo, target)
	local output = Protocols.	pack_use_broadcast_gameobject ( target)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*使用需要广播的游戏对象*/	
function Protocols.unpack_use_broadcast_gameobject (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.target = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*世界BOSS报名*/	
function Protocols.pack_world_boss_enroll (  )
	local output = Packet.new(CMSG_WORLD_BOSS_ENROLL)
	return output
end

-- /*世界BOSS报名*/	
function Protocols.call_world_boss_enroll ( playerInfo )
	local output = Protocols.	pack_world_boss_enroll (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*世界BOSS报名*/	
function Protocols.unpack_world_boss_enroll (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*世界BOSS挑战*/	
function Protocols.pack_world_boss_fight (  )
	local output = Packet.new(CMSG_WORLD_BOSS_FIGHT)
	return output
end

-- /*世界BOSS挑战*/	
function Protocols.call_world_boss_fight ( playerInfo )
	local output = Protocols.	pack_world_boss_fight (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*世界BOSS挑战*/	
function Protocols.unpack_world_boss_fight (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*换线*/	
function Protocols.pack_change_line ( lineNo)
	local output = Packet.new(CMSG_CHANGE_LINE)
	output:writeU32(lineNo)
	return output
end

-- /*换线*/	
function Protocols.call_change_line ( playerInfo, lineNo)
	local output = Protocols.	pack_change_line ( lineNo)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*换线*/	
function Protocols.unpack_change_line (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.lineNo = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*roll世界BOSS箱子*/	
function Protocols.pack_roll_world_boss_treasure (  )
	local output = Packet.new(CMSG_ROLL_WORLD_BOSS_TREASURE)
	return output
end

-- /*roll世界BOSS箱子*/	
function Protocols.call_roll_world_boss_treasure ( playerInfo )
	local output = Protocols.	pack_roll_world_boss_treasure (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*roll世界BOSS箱子*/	
function Protocols.unpack_roll_world_boss_treasure (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*roll点结果*/	
function Protocols.pack_roll_result ( point ,name ,isHighest ,timestamp ,rollid)
	local output = Packet.new(SMSG_ROLL_RESULT)
	output:writeByte(point)
	output:writeUTF(name)
	output:writeByte(isHighest)
	output:writeU32(timestamp)
	output:writeByte(rollid)
	return output
end

-- /*roll点结果*/	
function Protocols.call_roll_result ( playerInfo, point ,name ,isHighest ,timestamp ,rollid)
	local output = Protocols.	pack_roll_result ( point ,name ,isHighest ,timestamp ,rollid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*roll点结果*/	
function Protocols.unpack_roll_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.point = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.isHighest = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.timestamp = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.rollid = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*当前BOSS伤害排名*/	
function Protocols.pack_boss_rank ( rankType ,rankList ,mine)
	local output = Packet.new(SMSG_BOSS_RANK)
	output:writeByte(rankType)
	output:writeI16(#rankList)
	for i = 1,#rankList,1
	do
		rankList[i]:write(output)
	end
	output:writeByte(mine)
	return output
end

-- /*当前BOSS伤害排名*/	
function Protocols.call_boss_rank ( playerInfo, rankType ,rankList ,mine)
	local output = Protocols.	pack_boss_rank ( rankType ,rankList ,mine)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*当前BOSS伤害排名*/	
function Protocols.unpack_boss_rank (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.rankType = input:readByte()
	if not ret then
		return false
	end
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.rankList = {}
	for i = 1,len,1
	do
		local stru = rank_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.rankList,stru)
	end
	ret,param_table.mine = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*排行榜点赞*/	
function Protocols.pack_rank_add_like ( type ,guid)
	local output = Packet.new(CMSG_RANK_ADD_LIKE)
	output:writeByte(type)
	output:writeUTF(guid)
	return output
end

-- /*排行榜点赞*/	
function Protocols.call_rank_add_like ( playerInfo, type ,guid)
	local output = Protocols.	pack_rank_add_like ( type ,guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*排行榜点赞*/	
function Protocols.unpack_rank_add_like (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*排行榜点赞结果*/	
function Protocols.pack_rank_add_like_result ( type ,guid ,num)
	local output = Packet.new(SMSG_RANK_ADD_LIKE_RESULT)
	output:writeByte(type)
	output:writeUTF(guid)
	output:writeU32(num)
	return output
end

-- /*排行榜点赞结果*/	
function Protocols.call_rank_add_like_result ( playerInfo, type ,guid ,num)
	local output = Protocols.	pack_rank_add_like_result ( type ,guid ,num)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*排行榜点赞结果*/	
function Protocols.unpack_rank_add_like_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.num = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*进入资源副本*/	
function Protocols.pack_res_instance_enter ( id)
	local output = Packet.new(CMSG_RES_INSTANCE_ENTER)
	output:writeByte(id)
	return output
end

-- /*进入资源副本*/	
function Protocols.call_res_instance_enter ( playerInfo, id)
	local output = Protocols.	pack_res_instance_enter ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*进入资源副本*/	
function Protocols.unpack_res_instance_enter (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*扫荡资源副本*/	
function Protocols.pack_res_instance_sweep ( id)
	local output = Packet.new(CMSG_RES_INSTANCE_SWEEP)
	output:writeByte(id)
	return output
end

-- /*扫荡资源副本*/	
function Protocols.call_res_instance_sweep ( playerInfo, id)
	local output = Protocols.	pack_res_instance_sweep ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*扫荡资源副本*/	
function Protocols.unpack_res_instance_sweep (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*查看本地图的分线号*/	
function Protocols.pack_show_map_line (  )
	local output = Packet.new(CMSG_SHOW_MAP_LINE)
	return output
end

-- /*查看本地图的分线号*/	
function Protocols.call_show_map_line ( playerInfo )
	local output = Protocols.	pack_show_map_line (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*查看本地图的分线号*/	
function Protocols.unpack_show_map_line (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回本地图的分线号信息*/	
function Protocols.pack_send_map_line ( info)
	local output = Packet.new(SMSG_SEND_MAP_LINE)
	output:writeI16(#info)
	for i = 1,#info,1
	do
		info[i]:write(output)
	end
	return output
end

-- /*返回本地图的分线号信息*/	
function Protocols.call_send_map_line ( playerInfo, info)
	local output = Protocols.	pack_send_map_line ( info)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回本地图的分线号信息*/	
function Protocols.unpack_send_map_line (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.info = {}
	for i = 1,len,1
	do
		local stru = line_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.info,stru)
	end

	return true,param_table	

end


-- /*获得奖励提示*/	
function Protocols.pack_item_notice ( showType ,list)
	local output = Packet.new(SMSG_ITEM_NOTICE)
	output:writeByte(showType)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*获得奖励提示*/	
function Protocols.call_item_notice ( playerInfo, showType ,list)
	local output = Protocols.	pack_item_notice ( showType ,list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*获得奖励提示*/	
function Protocols.unpack_item_notice (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.showType = input:readByte()
	if not ret then
		return false
	end
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = item_reward_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*传送到某个世界地图*/	
function Protocols.pack_teleport_map ( mapid ,lineNo)
	local output = Packet.new(CMSG_TELEPORT_MAP)
	output:writeU32(mapid)
	output:writeU32(lineNo)
	return output
end

-- /*传送到某个世界地图*/	
function Protocols.call_teleport_map ( playerInfo, mapid ,lineNo)
	local output = Protocols.	pack_teleport_map ( mapid ,lineNo)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*传送到某个世界地图*/	
function Protocols.unpack_teleport_map (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.mapid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.lineNo = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*传送到野外boss旁边*/	
function Protocols.pack_teleport_field_boss ( mapid ,lineNo)
	local output = Packet.new(CMSG_TELEPORT_FIELD_BOSS)
	output:writeU32(mapid)
	output:writeU32(lineNo)
	return output
end

-- /*传送到野外boss旁边*/	
function Protocols.call_teleport_field_boss ( playerInfo, mapid ,lineNo)
	local output = Protocols.	pack_teleport_field_boss ( mapid ,lineNo)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*传送到野外boss旁边*/	
function Protocols.unpack_teleport_field_boss (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.mapid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.lineNo = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*活跃度奖励*/	
function Protocols.pack_get_activity_reward ( id ,vip)
	local output = Packet.new(CMSG_GET_ACTIVITY_REWARD)
	output:writeByte(id)
	output:writeByte(vip)
	return output
end

-- /*活跃度奖励*/	
function Protocols.call_get_activity_reward ( playerInfo, id ,vip)
	local output = Protocols.	pack_get_activity_reward ( id ,vip)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*活跃度奖励*/	
function Protocols.unpack_get_activity_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.vip = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*成就奖励*/	
function Protocols.pack_get_achieve_reward ( id)
	local output = Packet.new(CMSG_GET_ACHIEVE_REWARD)
	output:writeByte(id)
	return output
end

-- /*成就奖励*/	
function Protocols.call_get_achieve_reward ( playerInfo, id)
	local output = Protocols.	pack_get_achieve_reward ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*成就奖励*/	
function Protocols.unpack_get_achieve_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*总成就奖励*/	
function Protocols.pack_get_achieve_all_reward (  )
	local output = Packet.new(CMSG_GET_ACHIEVE_ALL_REWARD)
	return output
end

-- /*总成就奖励*/	
function Protocols.call_get_achieve_all_reward ( playerInfo )
	local output = Protocols.	pack_get_achieve_all_reward (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*总成就奖励*/	
function Protocols.unpack_get_achieve_all_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*装备称号*/	
function Protocols.pack_set_title ( id)
	local output = Packet.new(CMSG_SET_TITLE)
	output:writeByte(id)
	return output
end

-- /*装备称号*/	
function Protocols.call_set_title ( playerInfo, id)
	local output = Protocols.	pack_set_title ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*装备称号*/	
function Protocols.unpack_set_title (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*初始化称号*/	
function Protocols.pack_init_title ( id)
	local output = Packet.new(CMSG_INIT_TITLE)
	output:writeByte(id)
	return output
end

-- /*初始化称号*/	
function Protocols.call_init_title ( playerInfo, id)
	local output = Protocols.	pack_init_title ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*初始化称号*/	
function Protocols.unpack_init_title (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*领取首充奖励*/	
function Protocols.pack_welfare_shouchong_reward (  )
	local output = Packet.new(CMSG_WELFARE_SHOUCHONG_REWARD)
	return output
end

-- /*领取首充奖励*/	
function Protocols.call_welfare_shouchong_reward ( playerInfo )
	local output = Protocols.	pack_welfare_shouchong_reward (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取首充奖励*/	
function Protocols.unpack_welfare_shouchong_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*每日签到奖励*/	
function Protocols.pack_welfare_checkin (  )
	local output = Packet.new(CMSG_WELFARE_CHECKIN)
	return output
end

-- /*每日签到奖励*/	
function Protocols.call_welfare_checkin ( playerInfo )
	local output = Protocols.	pack_welfare_checkin (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*每日签到奖励*/	
function Protocols.unpack_welfare_checkin (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*累积签到奖励*/	
function Protocols.pack_welfare_checkin_all ( id)
	local output = Packet.new(CMSG_WELFARE_CHECKIN_ALL)
	output:writeByte(id)
	return output
end

-- /*累积签到奖励*/	
function Protocols.call_welfare_checkin_all ( playerInfo, id)
	local output = Protocols.	pack_welfare_checkin_all ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*累积签到奖励*/	
function Protocols.unpack_welfare_checkin_all (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*补签奖励*/	
function Protocols.pack_welfare_checkin_getback ( id)
	local output = Packet.new(CMSG_WELFARE_CHECKIN_GETBACK)
	output:writeByte(id)
	return output
end

-- /*补签奖励*/	
function Protocols.call_welfare_checkin_getback ( playerInfo, id)
	local output = Protocols.	pack_welfare_checkin_getback ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*补签奖励*/	
function Protocols.unpack_welfare_checkin_getback (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*等级奖励*/	
function Protocols.pack_welfare_level ( id)
	local output = Packet.new(CMSG_WELFARE_LEVEL)
	output:writeByte(id)
	return output
end

-- /*等级奖励*/	
function Protocols.call_welfare_level ( playerInfo, id)
	local output = Protocols.	pack_welfare_level ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*等级奖励*/	
function Protocols.unpack_welfare_level (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*活动找回奖励*/	
function Protocols.pack_welfare_active_getback ( id ,best ,num)
	local output = Packet.new(CMSG_WELFARE_ACTIVE_GETBACK)
	output:writeByte(id)
	output:writeByte(best)
	output:writeI16(num)
	return output
end

-- /*活动找回奖励*/	
function Protocols.call_welfare_active_getback ( playerInfo, id ,best ,num)
	local output = Protocols.	pack_welfare_active_getback ( id ,best ,num)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*活动找回奖励*/	
function Protocols.unpack_welfare_active_getback (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.best = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.num = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*领取任务奖励*/	
function Protocols.pack_pick_quest_reward ( indx)
	local output = Packet.new(CMSG_PICK_QUEST_REWARD)
	output:writeByte(indx)
	return output
end

-- /*领取任务奖励*/	
function Protocols.call_pick_quest_reward ( playerInfo, indx)
	local output = Protocols.	pack_pick_quest_reward ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取任务奖励*/	
function Protocols.unpack_pick_quest_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*和npc对话*/	
function Protocols.pack_talk_with_npc ( entry ,questId)
	local output = Packet.new(CMSG_TALK_WITH_NPC)
	output:writeI16(entry)
	output:writeI16(questId)
	return output
end

-- /*和npc对话*/	
function Protocols.call_talk_with_npc ( playerInfo, entry ,questId)
	local output = Protocols.	pack_talk_with_npc ( entry ,questId)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*和npc对话*/	
function Protocols.unpack_talk_with_npc (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.entry = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.questId = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*使用虚拟物品*/	
function Protocols.pack_use_virtual_item ( entry)
	local output = Packet.new(CMSG_USE_VIRTUAL_ITEM)
	output:writeI16(entry)
	return output
end

-- /*使用虚拟物品*/	
function Protocols.call_use_virtual_item ( playerInfo, entry)
	local output = Protocols.	pack_use_virtual_item ( entry)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*使用虚拟物品*/	
function Protocols.unpack_use_virtual_item (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.entry = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*领取任务章节奖励*/	
function Protocols.pack_pick_quest_chapter_reward ( indx)
	local output = Packet.new(CMSG_PICK_QUEST_CHAPTER_REWARD)
	output:writeByte(indx)
	return output
end

-- /*领取任务章节奖励*/	
function Protocols.call_pick_quest_chapter_reward ( playerInfo, indx)
	local output = Protocols.	pack_pick_quest_chapter_reward ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取任务章节奖励*/	
function Protocols.unpack_pick_quest_chapter_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*3v3跨服匹配*/	
function Protocols.pack_kuafu_3v3_match (  )
	local output = Packet.new(CMSG_KUAFU_3V3_MATCH)
	return output
end

-- /*3v3跨服匹配*/	
function Protocols.call_kuafu_3v3_match ( playerInfo )
	local output = Protocols.	pack_kuafu_3v3_match (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*3v3跨服匹配*/	
function Protocols.unpack_kuafu_3v3_match (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*跨服开始匹配*/	
function Protocols.pack_kuafu_match_start ( indx)
	local output = Packet.new(SMSG_KUAFU_MATCH_START)
	output:writeByte(indx)
	return output
end

-- /*跨服开始匹配*/	
function Protocols.call_kuafu_match_start ( playerInfo, indx)
	local output = Protocols.	pack_kuafu_match_start ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*跨服开始匹配*/	
function Protocols.unpack_kuafu_match_start (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*3v3购买次数*/	
function Protocols.pack_kuafu_3v3_buytimes ( num)
	local output = Packet.new(CMSG_KUAFU_3V3_BUYTIMES)
	output:writeByte(num)
	return output
end

-- /*3v3购买次数*/	
function Protocols.call_kuafu_3v3_buytimes ( playerInfo, num)
	local output = Protocols.	pack_kuafu_3v3_buytimes ( num)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*3v3购买次数*/	
function Protocols.unpack_kuafu_3v3_buytimes (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.num = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*3v3每日活跃奖励*/	
function Protocols.pack_kuafu_3v3_dayreward ( id)
	local output = Packet.new(CMSG_KUAFU_3V3_DAYREWARD)
	output:writeByte(id)
	return output
end

-- /*3v3每日活跃奖励*/	
function Protocols.call_kuafu_3v3_dayreward ( playerInfo, id)
	local output = Protocols.	pack_kuafu_3v3_dayreward ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*3v3每日活跃奖励*/	
function Protocols.unpack_kuafu_3v3_dayreward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*请求3v3排行榜*/	
function Protocols.pack_kuafu_3v3_getranlist (  )
	local output = Packet.new(CMSG_KUAFU_3V3_GETRANLIST)
	return output
end

-- /*请求3v3排行榜*/	
function Protocols.call_kuafu_3v3_getranlist ( playerInfo )
	local output = Protocols.	pack_kuafu_3v3_getranlist (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求3v3排行榜*/	
function Protocols.unpack_kuafu_3v3_getranlist (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*3v3排行榜结果列表*/	
function Protocols.pack_kuafu_3v3_ranlist ( list)
	local output = Packet.new(SMSG_KUAFU_3V3_RANLIST)
	output:writeUTF(list)
	return output
end

-- /*3v3排行榜结果列表*/	
function Protocols.call_kuafu_3v3_ranlist ( playerInfo, list)
	local output = Protocols.	pack_kuafu_3v3_ranlist ( list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*3v3排行榜结果列表*/	
function Protocols.unpack_kuafu_3v3_ranlist (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.list = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*福利所有奖励列表*/	
function Protocols.pack_welfare_getalllist_getback ( best)
	local output = Packet.new(CMSG_WELFARE_GETALLLIST_GETBACK)
	output:writeByte(best)
	return output
end

-- /*福利所有奖励列表*/	
function Protocols.call_welfare_getalllist_getback ( playerInfo, best)
	local output = Protocols.	pack_welfare_getalllist_getback ( best)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*福利所有奖励列表*/	
function Protocols.unpack_welfare_getalllist_getback (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.best = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*奖励列表*/	
function Protocols.pack_welfare_rewardlist_getback ( list ,cost)
	local output = Packet.new(SMSG_WELFARE_REWARDLIST_GETBACK)
	output:writeUTF(list)
	output:writeUTF(cost)
	return output
end

-- /*奖励列表*/	
function Protocols.call_welfare_rewardlist_getback ( playerInfo, list ,cost)
	local output = Protocols.	pack_welfare_rewardlist_getback ( list ,cost)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*奖励列表*/	
function Protocols.unpack_welfare_rewardlist_getback (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.list = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.cost = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*一键领取所有福利*/	
function Protocols.pack_welfare_getall_getback ( best)
	local output = Packet.new(CMSG_WELFARE_GETALL_GETBACK)
	output:writeByte(best)
	return output
end

-- /*一键领取所有福利*/	
function Protocols.call_welfare_getall_getback ( playerInfo, best)
	local output = Protocols.	pack_welfare_getall_getback ( best)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*一键领取所有福利*/	
function Protocols.unpack_welfare_getall_getback (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.best = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*请求3v3排行榜自己的名次*/	
function Protocols.pack_kuafu_3v3_getmyrank (  )
	local output = Packet.new(CMSG_KUAFU_3V3_GETMYRANK)
	return output
end

-- /*请求3v3排行榜自己的名次*/	
function Protocols.call_kuafu_3v3_getmyrank ( playerInfo )
	local output = Protocols.	pack_kuafu_3v3_getmyrank (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求3v3排行榜自己的名次*/	
function Protocols.unpack_kuafu_3v3_getmyrank (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*3v3排行榜自己的名次结果*/	
function Protocols.pack_kuafu_3v3_myrank ( rank)
	local output = Packet.new(SMSG_KUAFU_3V3_MYRANK)
	output:writeByte(rank)
	return output
end

-- /*3v3排行榜自己的名次结果*/	
function Protocols.call_kuafu_3v3_myrank ( playerInfo, rank)
	local output = Protocols.	pack_kuafu_3v3_myrank ( rank)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*3v3排行榜自己的名次结果*/	
function Protocols.unpack_kuafu_3v3_myrank (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.rank = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*击杀数据*/	
function Protocols.pack_kuafu_3v3_kill_detail ( indx1 ,indx2)
	local output = Packet.new(SMSG_KUAFU_3V3_KILL_DETAIL)
	output:writeU32(indx1)
	output:writeU32(indx2)
	return output
end

-- /*击杀数据*/	
function Protocols.call_kuafu_3v3_kill_detail ( playerInfo, indx1 ,indx2)
	local output = Protocols.	pack_kuafu_3v3_kill_detail ( indx1 ,indx2)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*击杀数据*/	
function Protocols.unpack_kuafu_3v3_kill_detail (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx1 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.indx2 = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*跨服匹配等待数据*/	
function Protocols.pack_kuafu_3v3_wait_info ( list)
	local output = Packet.new(SMSG_KUAFU_3V3_WAIT_INFO)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*跨服匹配等待数据*/	
function Protocols.call_kuafu_3v3_wait_info ( playerInfo, list)
	local output = Protocols.	pack_kuafu_3v3_wait_info ( list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*跨服匹配等待数据*/	
function Protocols.unpack_kuafu_3v3_wait_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = wait_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*取消匹配*/	
function Protocols.pack_kuafu_3v3_cancel_match ( type)
	local output = Packet.new(MSG_KUAFU_3V3_CANCEL_MATCH)
	output:writeU32(type)
	return output
end

-- /*取消匹配*/	
function Protocols.call_kuafu_3v3_cancel_match ( playerInfo, type)
	local output = Protocols.	pack_kuafu_3v3_cancel_match ( type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*取消匹配*/	
function Protocols.unpack_kuafu_3v3_cancel_match (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*匹配到人&接受或者拒绝*/	
function Protocols.pack_kuafu_3v3_match_oper ( oper)
	local output = Packet.new(CMSG_KUAFU_3V3_MATCH_OPER)
	output:writeU32(oper)
	return output
end

-- /*匹配到人&接受或者拒绝*/	
function Protocols.call_kuafu_3v3_match_oper ( playerInfo, oper)
	local output = Protocols.	pack_kuafu_3v3_match_oper ( oper)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*匹配到人&接受或者拒绝*/	
function Protocols.unpack_kuafu_3v3_match_oper (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.oper = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*拒绝比赛*/	
function Protocols.pack_kuafu_3v3_decline_match ( type)
	local output = Packet.new(SMSG_KUAFU_3V3_DECLINE_MATCH)
	output:writeU32(type)
	return output
end

-- /*拒绝比赛*/	
function Protocols.call_kuafu_3v3_decline_match ( playerInfo, type)
	local output = Protocols.	pack_kuafu_3v3_decline_match ( type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*拒绝比赛*/	
function Protocols.unpack_kuafu_3v3_decline_match (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*仙府夺宝跨服匹配*/	
function Protocols.pack_kuafu_xianfu_match ( indx)
	local output = Packet.new(CMSG_KUAFU_XIANFU_MATCH)
	output:writeByte(indx)
	return output
end

-- /*仙府夺宝跨服匹配*/	
function Protocols.call_kuafu_xianfu_match ( playerInfo, indx)
	local output = Protocols.	pack_kuafu_xianfu_match ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*仙府夺宝跨服匹配*/	
function Protocols.unpack_kuafu_xianfu_match (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*单方跨服匹配等待*/	
function Protocols.pack_kuafu_match_wait ( type ,target ,count ,data ,params)
	local output = Packet.new(SMSG_KUAFU_MATCH_WAIT)
	output:writeByte(type)
	output:writeByte(target)
	output:writeByte(count)
	output:writeU32(data)
	output:writeUTF(params)
	return output
end

-- /*单方跨服匹配等待*/	
function Protocols.call_kuafu_match_wait ( playerInfo, type ,target ,count ,data ,params)
	local output = Protocols.	pack_kuafu_match_wait ( type ,target ,count ,data ,params)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*单方跨服匹配等待*/	
function Protocols.unpack_kuafu_match_wait (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.target = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.count = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.data = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.params = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*仙府夺宝小地图信息*/	
function Protocols.pack_kuafu_xianfu_minimap_info (  )
	local output = Packet.new(SMSG_KUAFU_XIANFU_MINIMAP_INFO)
	return output
end

-- /*仙府夺宝小地图信息*/	
function Protocols.call_kuafu_xianfu_minimap_info ( playerInfo )
	local output = Protocols.	pack_kuafu_xianfu_minimap_info (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*仙府夺宝小地图信息*/	
function Protocols.unpack_kuafu_xianfu_minimap_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*购买仙府进入券*/	
function Protocols.pack_buy_xianfu_item ( type ,indx ,count)
	local output = Packet.new(CMSG_BUY_XIANFU_ITEM)
	output:writeByte(type)
	output:writeByte(indx)
	output:writeI16(count)
	return output
end

-- /*购买仙府进入券*/	
function Protocols.call_buy_xianfu_item ( playerInfo, type ,indx ,count)
	local output = Protocols.	pack_buy_xianfu_item ( type ,indx ,count)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*购买仙府进入券*/	
function Protocols.unpack_buy_xianfu_item (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.count = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*随机复活*/	
function Protocols.pack_xianfu_random_respawn (  )
	local output = Packet.new(CMSG_XIANFU_RANDOM_RESPAWN)
	return output
end

-- /*随机复活*/	
function Protocols.call_xianfu_random_respawn ( playerInfo )
	local output = Protocols.	pack_xianfu_random_respawn (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*随机复活*/	
function Protocols.unpack_xianfu_random_respawn (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*斗剑台挑战*/	
function Protocols.pack_doujiantai_fight ( rank)
	local output = Packet.new(CMSG_DOUJIANTAI_FIGHT)
	output:writeI16(rank)
	return output
end

-- /*斗剑台挑战*/	
function Protocols.call_doujiantai_fight ( playerInfo, rank)
	local output = Protocols.	pack_doujiantai_fight ( rank)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*斗剑台挑战*/	
function Protocols.unpack_doujiantai_fight (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.rank = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*斗剑台购买次数*/	
function Protocols.pack_doujiantai_buytime ( num)
	local output = Packet.new(CMSG_DOUJIANTAI_BUYTIME)
	output:writeByte(num)
	return output
end

-- /*斗剑台购买次数*/	
function Protocols.call_doujiantai_buytime ( playerInfo, num)
	local output = Protocols.	pack_doujiantai_buytime ( num)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*斗剑台购买次数*/	
function Protocols.unpack_doujiantai_buytime (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.num = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*斗剑台清理CD*/	
function Protocols.pack_doujiantai_clearcd (  )
	local output = Packet.new(CMSG_DOUJIANTAI_CLEARCD)
	return output
end

-- /*斗剑台清理CD*/	
function Protocols.call_doujiantai_clearcd ( playerInfo )
	local output = Protocols.	pack_doujiantai_clearcd (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*斗剑台清理CD*/	
function Protocols.unpack_doujiantai_clearcd (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*斗剑台首胜奖励*/	
function Protocols.pack_doujiantai_first_reward ( id)
	local output = Packet.new(CMSG_DOUJIANTAI_FIRST_REWARD)
	output:writeByte(id)
	return output
end

-- /*斗剑台首胜奖励*/	
function Protocols.call_doujiantai_first_reward ( playerInfo, id)
	local output = Protocols.	pack_doujiantai_first_reward ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*斗剑台首胜奖励*/	
function Protocols.unpack_doujiantai_first_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*斗剑台挑战对手信息*/	
function Protocols.pack_doujiantai_get_enemys_info (  )
	local output = Packet.new(MSG_DOUJIANTAI_GET_ENEMYS_INFO)
	return output
end

-- /*斗剑台挑战对手信息*/	
function Protocols.call_doujiantai_get_enemys_info ( playerInfo )
	local output = Protocols.	pack_doujiantai_get_enemys_info (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*斗剑台挑战对手信息*/	
function Protocols.unpack_doujiantai_get_enemys_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*斗剑台排行榜*/	
function Protocols.pack_doujiantai_get_rank ( startIdx ,endIdx)
	local output = Packet.new(CMSG_DOUJIANTAI_GET_RANK)
	output:writeI16(startIdx)
	output:writeI16(endIdx)
	return output
end

-- /*斗剑台排行榜*/	
function Protocols.call_doujiantai_get_rank ( playerInfo, startIdx ,endIdx)
	local output = Protocols.	pack_doujiantai_get_rank ( startIdx ,endIdx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*斗剑台排行榜*/	
function Protocols.unpack_doujiantai_get_rank (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.startIdx = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.endIdx = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*斗剑台刷新对手*/	
function Protocols.pack_doujiantai_refresh_enemys (  )
	local output = Packet.new(CMSG_DOUJIANTAI_REFRESH_ENEMYS)
	return output
end

-- /*斗剑台刷新对手*/	
function Protocols.call_doujiantai_refresh_enemys ( playerInfo )
	local output = Protocols.	pack_doujiantai_refresh_enemys (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*斗剑台刷新对手*/	
function Protocols.unpack_doujiantai_refresh_enemys (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*斗剑台三甲*/	
function Protocols.pack_doujiantai_top3 (  )
	local output = Packet.new(MSG_DOUJIANTAI_TOP3)
	return output
end

-- /*斗剑台三甲*/	
function Protocols.call_doujiantai_top3 ( playerInfo )
	local output = Protocols.	pack_doujiantai_top3 (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*斗剑台三甲*/	
function Protocols.unpack_doujiantai_top3 (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*使用跳点*/	
function Protocols.pack_use_jump_point ( id)
	local output = Packet.new(MSG_USE_JUMP_POINT)
	output:writeU32(id)
	return output
end

-- /*使用跳点*/	
function Protocols.call_use_jump_point ( playerInfo, id)
	local output = Protocols.	pack_use_jump_point ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*使用跳点*/	
function Protocols.unpack_use_jump_point (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*出售物品*/	
function Protocols.pack_bag_item_sell ( item_guid ,count)
	local output = Packet.new(CMSG_BAG_ITEM_SELL)
	output:writeUTF(item_guid)
	output:writeU32(count)
	return output
end

-- /*出售物品*/	
function Protocols.call_bag_item_sell ( playerInfo, item_guid ,count)
	local output = Protocols.	pack_bag_item_sell ( item_guid ,count)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*出售物品*/	
function Protocols.unpack_bag_item_sell (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.item_guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*整理物品*/	
function Protocols.pack_bag_item_sort (  )
	local output = Packet.new(CMSG_BAG_ITEM_SORT)
	return output
end

-- /*整理物品*/	
function Protocols.call_bag_item_sort ( playerInfo )
	local output = Protocols.	pack_bag_item_sort (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*整理物品*/	
function Protocols.unpack_bag_item_sort (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*提交日常任务*/	
function Protocols.pack_submit_quest_daily2 (  )
	local output = Packet.new(CMSG_SUBMIT_QUEST_DAILY2)
	return output
end

-- /*提交日常任务*/	
function Protocols.call_submit_quest_daily2 ( playerInfo )
	local output = Protocols.	pack_submit_quest_daily2 (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*提交日常任务*/	
function Protocols.unpack_submit_quest_daily2 (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*属性改变*/	
function Protocols.pack_attribute_changed (  )
	local output = Packet.new(SMSG_ATTRIBUTE_CHANGED)
	return output
end

-- /*属性改变*/	
function Protocols.call_attribute_changed ( playerInfo )
	local output = Protocols.	pack_attribute_changed (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*属性改变*/	
function Protocols.unpack_attribute_changed (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*背包有更强装备*/	
function Protocols.pack_bag_find_equip_better ( item_id ,pos)
	local output = Packet.new(SMSG_BAG_FIND_EQUIP_BETTER)
	output:writeU32(item_id)
	output:writeU32(pos)
	return output
end

-- /*背包有更强装备*/	
function Protocols.call_bag_find_equip_better ( playerInfo, item_id ,pos)
	local output = Protocols.	pack_bag_find_equip_better ( item_id ,pos)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*背包有更强装备*/	
function Protocols.unpack_bag_find_equip_better (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.item_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.pos = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*模块解锁*/	
function Protocols.pack_module_active ( moduleId)
	local output = Packet.new(SMSG_MODULE_ACTIVE)
	output:writeU32(moduleId)
	return output
end

-- /*模块解锁*/	
function Protocols.call_module_active ( playerInfo, moduleId)
	local output = Protocols.	pack_module_active ( moduleId)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*模块解锁*/	
function Protocols.unpack_module_active (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.moduleId = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*领取日常任务奖励*/	
function Protocols.pack_pick_daily2_quest_reward ( indx)
	local output = Packet.new(CMSG_PICK_DAILY2_QUEST_REWARD)
	output:writeByte(indx)
	return output
end

-- /*领取日常任务奖励*/	
function Protocols.call_pick_daily2_quest_reward ( playerInfo, indx)
	local output = Protocols.	pack_pick_daily2_quest_reward ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取日常任务奖励*/	
function Protocols.unpack_pick_daily2_quest_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*完成当前引导*/	
function Protocols.pack_finish_now_guide (  )
	local output = Packet.new(CMSG_FINISH_NOW_GUIDE)
	return output
end

-- /*完成当前引导*/	
function Protocols.call_finish_now_guide ( playerInfo )
	local output = Protocols.	pack_finish_now_guide (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*完成当前引导*/	
function Protocols.unpack_finish_now_guide (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*取得修炼场信息*/	
function Protocols.pack_get_cultivation_info (  )
	local output = Packet.new(CMSG_GET_CULTIVATION_INFO)
	return output
end

-- /*取得修炼场信息*/	
function Protocols.call_get_cultivation_info ( playerInfo )
	local output = Protocols.	pack_get_cultivation_info (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*取得修炼场信息*/	
function Protocols.unpack_get_cultivation_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回修炼场信息*/	
function Protocols.pack_update_cultivation_info ( start_time ,lost)
	local output = Packet.new(SMSG_UPDATE_CULTIVATION_INFO)
	output:writeU32(start_time)
	output:writeU32(lost)
	return output
end

-- /*返回修炼场信息*/	
function Protocols.call_update_cultivation_info ( playerInfo, start_time ,lost)
	local output = Protocols.	pack_update_cultivation_info ( start_time ,lost)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回修炼场信息*/	
function Protocols.unpack_update_cultivation_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.start_time = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.lost = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*取得当前所有修炼场对手信息*/	
function Protocols.pack_get_cultivation_rivals_info (  )
	local output = Packet.new(CMSG_GET_CULTIVATION_RIVALS_INFO)
	return output
end

-- /*取得当前所有修炼场对手信息*/	
function Protocols.call_get_cultivation_rivals_info ( playerInfo )
	local output = Protocols.	pack_get_cultivation_rivals_info (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*取得当前所有修炼场对手信息*/	
function Protocols.unpack_get_cultivation_rivals_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回修炼场对手信息*/	
function Protocols.pack_update_cultivation_rivals_info_list ( list)
	local output = Packet.new(SMSG_UPDATE_CULTIVATION_RIVALS_INFO_LIST)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*返回修炼场对手信息*/	
function Protocols.call_update_cultivation_rivals_info_list ( playerInfo, list)
	local output = Protocols.	pack_update_cultivation_rivals_info_list ( list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回修炼场对手信息*/	
function Protocols.unpack_update_cultivation_rivals_info_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = cultivation_rivals_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*领取修炼场奖励*/	
function Protocols.pack_get_cultivation_reward (  )
	local output = Packet.new(CMSG_GET_CULTIVATION_REWARD)
	return output
end

-- /*领取修炼场奖励*/	
function Protocols.call_get_cultivation_reward ( playerInfo )
	local output = Protocols.	pack_get_cultivation_reward (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取修炼场奖励*/	
function Protocols.unpack_get_cultivation_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*刷新修炼场对手*/	
function Protocols.pack_refresh_cultivation_rivals (  )
	local output = Packet.new(CMSG_REFRESH_CULTIVATION_RIVALS)
	return output
end

-- /*刷新修炼场对手*/	
function Protocols.call_refresh_cultivation_rivals ( playerInfo )
	local output = Protocols.	pack_refresh_cultivation_rivals (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*刷新修炼场对手*/	
function Protocols.unpack_refresh_cultivation_rivals (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*掠夺修炼场对手*/	
function Protocols.pack_plunder_cultivation_rival ( index)
	local output = Packet.new(CMSG_PLUNDER_CULTIVATION_RIVAL)
	output:writeU32(index)
	return output
end

-- /*掠夺修炼场对手*/	
function Protocols.call_plunder_cultivation_rival ( playerInfo, index)
	local output = Protocols.	pack_plunder_cultivation_rival ( index)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*掠夺修炼场对手*/	
function Protocols.unpack_plunder_cultivation_rival (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.index = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*反击复仇修炼场对手*/	
function Protocols.pack_revenge_cultivation_rival ( index)
	local output = Packet.new(CMSG_REVENGE_CULTIVATION_RIVAL)
	output:writeU32(index)
	return output
end

-- /*反击复仇修炼场对手*/	
function Protocols.call_revenge_cultivation_rival ( playerInfo, index)
	local output = Protocols.	pack_revenge_cultivation_rival ( index)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*反击复仇修炼场对手*/	
function Protocols.unpack_revenge_cultivation_rival (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.index = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*增加修炼场剩余挑战次数*/	
function Protocols.pack_buy_cultivation_left_plunder_count ( count)
	local output = Packet.new(CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT)
	output:writeU32(count)
	return output
end

-- /*增加修炼场剩余挑战次数*/	
function Protocols.call_buy_cultivation_left_plunder_count ( playerInfo, count)
	local output = Protocols.	pack_buy_cultivation_left_plunder_count ( count)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*增加修炼场剩余挑战次数*/	
function Protocols.unpack_buy_cultivation_left_plunder_count (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*返回修炼场战斗结果*/	
function Protocols.pack_show_cultivation_result_list ( result ,name ,list)
	local output = Packet.new(SMSG_SHOW_CULTIVATION_RESULT_LIST)
	output:writeI32(result)
	output:writeUTF(name)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*返回修炼场战斗结果*/	
function Protocols.call_show_cultivation_result_list ( playerInfo, result ,name ,list)
	local output = Protocols.	pack_show_cultivation_result_list ( result ,name ,list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回修炼场战斗结果*/	
function Protocols.unpack_show_cultivation_result_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.result = input:readI32()
	if not ret then
		return false
	end	
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = item_reward_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*领取登录大礼奖励*/	
function Protocols.pack_get_login_activity_reward ( id)
	local output = Packet.new(CMSG_GET_LOGIN_ACTIVITY_REWARD)
	output:writeU32(id)
	return output
end

-- /*领取登录大礼奖励*/	
function Protocols.call_get_login_activity_reward ( playerInfo, id)
	local output = Protocols.	pack_get_login_activity_reward ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取登录大礼奖励*/	
function Protocols.unpack_get_login_activity_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*通知客户端释放蓄力技能*/	
function Protocols.pack_cast_spell_start ( caster_guid ,target_guid ,spellid ,data)
	local output = Packet.new(SMSG_CAST_SPELL_START)
	output:writeU32(caster_guid)
	output:writeU32(target_guid)
	output:writeI16(spellid)
	output:writeUTF(data)
	return output
end

-- /*通知客户端释放蓄力技能*/	
function Protocols.call_cast_spell_start ( playerInfo, caster_guid ,target_guid ,spellid ,data)
	local output = Protocols.	pack_cast_spell_start ( caster_guid ,target_guid ,spellid ,data)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*通知客户端释放蓄力技能*/	
function Protocols.unpack_cast_spell_start (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.caster_guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.target_guid = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.spellid = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.data = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*完成非强制引导的步骤*/	
function Protocols.pack_finish_optional_guide_step ( guide_id ,step)
	local output = Packet.new(CMSG_FINISH_OPTIONAL_GUIDE_STEP)
	output:writeU32(guide_id)
	output:writeByte(step)
	return output
end

-- /*完成非强制引导的步骤*/	
function Protocols.call_finish_optional_guide_step ( playerInfo, guide_id ,step)
	local output = Protocols.	pack_finish_optional_guide_step ( guide_id ,step)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*完成非强制引导的步骤*/	
function Protocols.unpack_finish_optional_guide_step (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guide_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.step = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*执行接到任务以后的命令*/	
function Protocols.pack_execute_quest_cmd_after_accepted ( indx)
	local output = Packet.new(CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED)
	output:writeI16(indx)
	return output
end

-- /*执行接到任务以后的命令*/	
function Protocols.call_execute_quest_cmd_after_accepted ( playerInfo, indx)
	local output = Protocols.	pack_execute_quest_cmd_after_accepted ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*执行接到任务以后的命令*/	
function Protocols.unpack_execute_quest_cmd_after_accepted (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*通知客户端显示属性*/	
function Protocols.pack_show_unit_attribute (  )
	local output = Packet.new(SMSG_SHOW_UNIT_ATTRIBUTE)
	return output
end

-- /*通知客户端显示属性*/	
function Protocols.call_show_unit_attribute ( playerInfo )
	local output = Protocols.	pack_show_unit_attribute (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*通知客户端显示属性*/	
function Protocols.unpack_show_unit_attribute (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回家族*/	
function Protocols.pack_back_to_famity (  )
	local output = Packet.new(CMSG_BACK_TO_FAMITY)
	return output
end

-- /*返回家族*/	
function Protocols.call_back_to_famity ( playerInfo )
	local output = Protocols.	pack_back_to_famity (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回家族*/	
function Protocols.unpack_back_to_famity (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回家族boss结果*/	
function Protocols.pack_faction_boss_send_result ( result ,boss_id ,money)
	local output = Packet.new(SMSG_FACTION_BOSS_SEND_RESULT)
	output:writeU32(result)
	output:writeU32(boss_id)
	output:writeU32(money)
	return output
end

-- /*返回家族boss结果*/	
function Protocols.call_faction_boss_send_result ( playerInfo, result ,boss_id ,money)
	local output = Protocols.	pack_faction_boss_send_result ( result ,boss_id ,money)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回家族boss结果*/	
function Protocols.unpack_faction_boss_send_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.result = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.boss_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.money = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*挑战boss*/	
function Protocols.pack_challange_boss (  )
	local output = Packet.new(CMSG_CHALLANGE_BOSS)
	return output
end

-- /*挑战boss*/	
function Protocols.call_challange_boss ( playerInfo )
	local output = Protocols.	pack_challange_boss (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*挑战boss*/	
function Protocols.unpack_challange_boss (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*领取离线奖励*/	
function Protocols.pack_pick_offline_reward (  )
	local output = Packet.new(CMSG_PICK_OFFLINE_REWARD)
	return output
end

-- /*领取离线奖励*/	
function Protocols.call_pick_offline_reward ( playerInfo )
	local output = Protocols.	pack_pick_offline_reward (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取离线奖励*/	
function Protocols.unpack_pick_offline_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*离线奖励结果*/	
function Protocols.pack_offline_reward_result ( reserve ,reserve2 ,reserve3 ,reserve4 ,list)
	local output = Packet.new(SMSG_OFFLINE_REWARD_RESULT)
	output:writeU32(reserve)
	output:writeU32(reserve2)
	output:writeU32(reserve3)
	output:writeU32(reserve4)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*离线奖励结果*/	
function Protocols.call_offline_reward_result ( playerInfo, reserve ,reserve2 ,reserve3 ,reserve4 ,list)
	local output = Protocols.	pack_offline_reward_result ( reserve ,reserve2 ,reserve3 ,reserve4 ,list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*离线奖励结果*/	
function Protocols.unpack_offline_reward_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.reserve = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.reserve2 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.reserve3 = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.reserve4 = input:readU32()
	if not ret then
		return false
	end	
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = item_reward_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*熔炼装备*/	
function Protocols.pack_smelting_equip ( pos_str)
	local output = Packet.new(CMSG_SMELTING_EQUIP)
	output:writeUTF(pos_str)
	return output
end

-- /*熔炼装备*/	
function Protocols.call_smelting_equip ( playerInfo, pos_str)
	local output = Protocols.	pack_smelting_equip ( pos_str)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*熔炼装备*/	
function Protocols.unpack_smelting_equip (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.pos_str = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*上交装备*/	
function Protocols.pack_storehouse_hand_in ( pos_str)
	local output = Packet.new(CMSG_STOREHOUSE_HAND_IN)
	output:writeUTF(pos_str)
	return output
end

-- /*上交装备*/	
function Protocols.call_storehouse_hand_in ( playerInfo, pos_str)
	local output = Protocols.	pack_storehouse_hand_in ( pos_str)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*上交装备*/	
function Protocols.unpack_storehouse_hand_in (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.pos_str = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*兑换装备*/	
function Protocols.pack_storehouse_exchange ( pos)
	local output = Packet.new(CMSG_STOREHOUSE_EXCHANGE)
	output:writeU32(pos)
	return output
end

-- /*兑换装备*/	
function Protocols.call_storehouse_exchange ( playerInfo, pos)
	local output = Protocols.	pack_storehouse_exchange ( pos)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*兑换装备*/	
function Protocols.unpack_storehouse_exchange (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.pos = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*销毁装备*/	
function Protocols.pack_storehouse_destroy ( pos)
	local output = Packet.new(CMSG_STOREHOUSE_DESTROY)
	output:writeU32(pos)
	return output
end

-- /*销毁装备*/	
function Protocols.call_storehouse_destroy ( playerInfo, pos)
	local output = Protocols.	pack_storehouse_destroy ( pos)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*销毁装备*/	
function Protocols.unpack_storehouse_destroy (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.pos = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*赠送礼物*/	
function Protocols.pack_send_faction_gift ( list ,msg ,msg_type)
	local output = Packet.new(CMSG_SEND_FACTION_GIFT)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	output:writeUTF(msg)
	output:writeU32(msg_type)
	return output
end

-- /*赠送礼物*/	
function Protocols.call_send_faction_gift ( playerInfo, list ,msg ,msg_type)
	local output = Protocols.	pack_send_faction_gift ( list ,msg ,msg_type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*赠送礼物*/	
function Protocols.unpack_send_faction_gift (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = item_reward_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end
	ret,param_table.msg = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.msg_type = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*领取额外奖励*/	
function Protocols.pack_get_faction_gift_exreward ( count_id)
	local output = Packet.new(CMSG_GET_FACTION_GIFT_EXREWARD)
	output:writeU32(count_id)
	return output
end

-- /*领取额外奖励*/	
function Protocols.call_get_faction_gift_exreward ( playerInfo, count_id)
	local output = Protocols.	pack_get_faction_gift_exreward ( count_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取额外奖励*/	
function Protocols.unpack_get_faction_gift_exreward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.count_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*领取所有额外奖励*/	
function Protocols.pack_get_all_faction_gift_exreward (  )
	local output = Packet.new(CMSG_GET_ALL_FACTION_GIFT_EXREWARD)
	return output
end

-- /*领取所有额外奖励*/	
function Protocols.call_get_all_faction_gift_exreward ( playerInfo )
	local output = Protocols.	pack_get_all_faction_gift_exreward (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取所有额外奖励*/	
function Protocols.unpack_get_all_faction_gift_exreward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回礼物列表*/	
function Protocols.pack_show_faction_gift_page (  )
	local output = Packet.new(SMSG_SHOW_FACTION_GIFT_PAGE)
	return output
end

-- /*返回礼物列表*/	
function Protocols.call_show_faction_gift_page ( playerInfo )
	local output = Protocols.	pack_show_faction_gift_page (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回礼物列表*/	
function Protocols.unpack_show_faction_gift_page (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回礼物信息*/	
function Protocols.pack_show_faction_gift_info (  )
	local output = Packet.new(SMSG_SHOW_FACTION_GIFT_INFO)
	return output
end

-- /*返回礼物信息*/	
function Protocols.call_show_faction_gift_info ( playerInfo )
	local output = Protocols.	pack_show_faction_gift_info (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回礼物信息*/	
function Protocols.unpack_show_faction_gift_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回女王未感谢礼物*/	
function Protocols.pack_show_faction_gift_unthank_page (  )
	local output = Packet.new(SMSG_SHOW_FACTION_GIFT_UNTHANK_PAGE)
	return output
end

-- /*返回女王未感谢礼物*/	
function Protocols.call_show_faction_gift_unthank_page ( playerInfo )
	local output = Protocols.	pack_show_faction_gift_unthank_page (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回女王未感谢礼物*/	
function Protocols.unpack_show_faction_gift_unthank_page (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*返回女王历史记录*/	
function Protocols.pack_show_faction_gift_history_page (  )
	local output = Packet.new(SMSG_SHOW_FACTION_GIFT_HISTORY_PAGE)
	return output
end

-- /*返回女王历史记录*/	
function Protocols.call_show_faction_gift_history_page ( playerInfo )
	local output = Protocols.	pack_show_faction_gift_history_page (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回女王历史记录*/	
function Protocols.unpack_show_faction_gift_history_page (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*请求家族魅力排行*/	
function Protocols.pack_get_faction_gift_rank_page ( page)
	local output = Packet.new(CMSG_GET_FACTION_GIFT_RANK_PAGE)
	output:writeU32(page)
	return output
end

-- /*请求家族魅力排行*/	
function Protocols.call_get_faction_gift_rank_page ( playerInfo, page)
	local output = Protocols.	pack_get_faction_gift_rank_page ( page)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求家族魅力排行*/	
function Protocols.unpack_get_faction_gift_rank_page (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.page = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*返回家族魅力排行*/	
function Protocols.pack_show_faction_gift_rank_result_list ( list ,info ,page)
	local output = Packet.new(SMSG_SHOW_FACTION_GIFT_RANK_RESULT_LIST)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	info :write(output)
	output:writeU32(page)
	return output
end

-- /*返回家族魅力排行*/	
function Protocols.call_show_faction_gift_rank_result_list ( playerInfo, list ,info ,page)
	local output = Protocols.	pack_show_faction_gift_rank_result_list ( list ,info ,page)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回家族魅力排行*/	
function Protocols.unpack_show_faction_gift_rank_result_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = faction_gift_rank_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end
	param_table.info = faction_gift_rank_info_t .new()
	if(param_table.info :read(input)==false)then
		return false
	end
	ret,param_table.page = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*返回家族魅力排行变化*/	
function Protocols.pack_show_faction_gift_rank_change ( old_rank ,new_rank ,info)
	local output = Packet.new(SMSG_SHOW_FACTION_GIFT_RANK_CHANGE)
	output:writeU32(old_rank)
	output:writeU32(new_rank)
	info :write(output)
	return output
end

-- /*返回家族魅力排行变化*/	
function Protocols.call_show_faction_gift_rank_change ( playerInfo, old_rank ,new_rank ,info)
	local output = Protocols.	pack_show_faction_gift_rank_change ( old_rank ,new_rank ,info)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回家族魅力排行变化*/	
function Protocols.unpack_show_faction_gift_rank_change (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.old_rank = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.new_rank = input:readU32()
	if not ret then
		return false
	end	
	param_table.info = faction_gift_rank_info_t .new()
	if(param_table.info :read(input)==false)then
		return false
	end

	return true,param_table	

end


-- /*返回本家族魅力排行*/	
function Protocols.pack_show_faction_gift_rank_info ( info)
	local output = Packet.new(SMSG_SHOW_FACTION_GIFT_RANK_INFO)
	info :write(output)
	return output
end

-- /*返回本家族魅力排行*/	
function Protocols.call_show_faction_gift_rank_info ( playerInfo, info)
	local output = Protocols.	pack_show_faction_gift_rank_info ( info)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回本家族魅力排行*/	
function Protocols.unpack_show_faction_gift_rank_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	param_table.info = faction_gift_rank_info_t .new()
	if(param_table.info :read(input)==false)then
		return false
	end

	return true,param_table	

end


-- /*神兵强化*/	
function Protocols.pack_divine_forge ( id ,count)
	local output = Packet.new(CMSG_DIVINE_FORGE)
	output:writeU32(id)
	output:writeU32(count)
	return output
end

-- /*神兵强化*/	
function Protocols.call_divine_forge ( playerInfo, id ,count)
	local output = Protocols.	pack_divine_forge ( id ,count)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*神兵强化*/	
function Protocols.unpack_divine_forge (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*神兵升阶*/	
function Protocols.pack_divine_advance ( id)
	local output = Packet.new(CMSG_DIVINE_ADVANCE)
	output:writeU32(id)
	return output
end

-- /*神兵升阶*/	
function Protocols.call_divine_advance ( playerInfo, id)
	local output = Protocols.	pack_divine_advance ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*神兵升阶*/	
function Protocols.unpack_divine_advance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*神兵铸魂*/	
function Protocols.pack_divine_spirit ( id ,protect ,improve)
	local output = Packet.new(CMSG_DIVINE_SPIRIT)
	output:writeU32(id)
	output:writeU32(protect)
	output:writeU32(improve)
	return output
end

-- /*神兵铸魂*/	
function Protocols.call_divine_spirit ( playerInfo, id ,protect ,improve)
	local output = Protocols.	pack_divine_spirit ( id ,protect ,improve)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*神兵铸魂*/	
function Protocols.unpack_divine_spirit (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.protect = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.improve = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*查询全民boss信息*/	
function Protocols.pack_query_mass_boss_info ( id)
	local output = Packet.new(CMSG_QUERY_MASS_BOSS_INFO)
	output:writeByte(id)
	return output
end

-- /*查询全民boss信息*/	
function Protocols.call_query_mass_boss_info ( playerInfo, id)
	local output = Protocols.	pack_query_mass_boss_info ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*查询全民boss信息*/	
function Protocols.unpack_query_mass_boss_info (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*全民boss信息结果*/	
function Protocols.pack_mass_boss_info_ret ( count ,percent)
	local output = Packet.new(SMSG_MASS_BOSS_INFO_RET)
	output:writeU32(count)
	output:writeByte(percent)
	return output
end

-- /*全民boss信息结果*/	
function Protocols.call_mass_boss_info_ret ( playerInfo, count ,percent)
	local output = Protocols.	pack_mass_boss_info_ret ( count ,percent)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*全民boss信息结果*/	
function Protocols.unpack_mass_boss_info_ret (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.count = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.percent = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*查询全民boss排行榜*/	
function Protocols.pack_query_mass_boss_rank ( id)
	local output = Packet.new(CMSG_QUERY_MASS_BOSS_RANK)
	output:writeByte(id)
	return output
end

-- /*查询全民boss排行榜*/	
function Protocols.call_query_mass_boss_rank ( playerInfo, id)
	local output = Protocols.	pack_query_mass_boss_rank ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*查询全民boss排行榜*/	
function Protocols.unpack_query_mass_boss_rank (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*全民boss排行结果*/	
function Protocols.pack_mass_boss_rank_result ( info)
	local output = Packet.new(SMSG_MASS_BOSS_RANK_RESULT)
	output:writeI16(#info)
	for i = 1,#info,1
	do
		info[i]:write(output)
	end
	return output
end

-- /*全民boss排行结果*/	
function Protocols.call_mass_boss_rank_result ( playerInfo, info)
	local output = Protocols.	pack_mass_boss_rank_result ( info)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*全民boss排行结果*/	
function Protocols.unpack_mass_boss_rank_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.info = {}
	for i = 1,len,1
	do
		local stru = mass_boss_rank_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.info,stru)
	end

	return true,param_table	

end


-- /*挑战全民boss*/	
function Protocols.pack_try_mass_boss ( id)
	local output = Packet.new(CMSG_TRY_MASS_BOSS)
	output:writeByte(id)
	return output
end

-- /*挑战全民boss*/	
function Protocols.call_try_mass_boss ( playerInfo, id)
	local output = Protocols.	pack_try_mass_boss ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*挑战全民boss*/	
function Protocols.unpack_try_mass_boss (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*购买挑战全民boss次数*/	
function Protocols.pack_buy_mass_boss_times ( cnt)
	local output = Packet.new(CMSG_BUY_MASS_BOSS_TIMES)
	output:writeByte(cnt)
	return output
end

-- /*购买挑战全民boss次数*/	
function Protocols.call_buy_mass_boss_times ( playerInfo, cnt)
	local output = Protocols.	pack_buy_mass_boss_times ( cnt)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*购买挑战全民boss次数*/	
function Protocols.unpack_buy_mass_boss_times (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.cnt = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*组队副本跨服匹配*/	
function Protocols.pack_group_instance_match ( indx)
	local output = Packet.new(CMSG_GROUP_INSTANCE_MATCH)
	output:writeByte(indx)
	return output
end

-- /*组队副本跨服匹配*/	
function Protocols.call_group_instance_match ( playerInfo, indx)
	local output = Protocols.	pack_group_instance_match ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*组队副本跨服匹配*/	
function Protocols.unpack_group_instance_match (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*组队副本跨服次数购买*/	
function Protocols.pack_buy_group_instance_times ( count)
	local output = Packet.new(CMSG_BUY_GROUP_INSTANCE_TIMES)
	output:writeByte(count)
	return output
end

-- /*组队副本跨服次数购买*/	
function Protocols.call_buy_group_instance_times ( playerInfo, count)
	local output = Protocols.	pack_buy_group_instance_times ( count)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*组队副本跨服次数购买*/	
function Protocols.unpack_buy_group_instance_times (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.count = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*法宝激活*/	
function Protocols.pack_talisman_active ( id)
	local output = Packet.new(CMSG_TALISMAN_ACTIVE)
	output:writeU32(id)
	return output
end

-- /*法宝激活*/	
function Protocols.call_talisman_active ( playerInfo, id)
	local output = Protocols.	pack_talisman_active ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*法宝激活*/	
function Protocols.unpack_talisman_active (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*法宝注灵*/	
function Protocols.pack_talisman_lvup ( id)
	local output = Packet.new(CMSG_TALISMAN_LVUP)
	output:writeU32(id)
	return output
end

-- /*法宝注灵*/	
function Protocols.call_talisman_lvup ( playerInfo, id)
	local output = Protocols.	pack_talisman_lvup ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*法宝注灵*/	
function Protocols.unpack_talisman_lvup (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*神羽激活*/	
function Protocols.pack_wings_active (  )
	local output = Packet.new(CMSG_WINGS_ACTIVE)
	return output
end

-- /*神羽激活*/	
function Protocols.call_wings_active ( playerInfo )
	local output = Protocols.	pack_wings_active (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*神羽激活*/	
function Protocols.unpack_wings_active (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*神羽祝福*/	
function Protocols.pack_wings_bless (  )
	local output = Packet.new(CMSG_WINGS_BLESS)
	return output
end

-- /*神羽祝福*/	
function Protocols.call_wings_bless ( playerInfo )
	local output = Protocols.	pack_wings_bless (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*神羽祝福*/	
function Protocols.unpack_wings_bless (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*神羽升阶*/	
function Protocols.pack_wings_rankup (  )
	local output = Packet.new(CMSG_WINGS_RANKUP)
	return output
end

-- /*神羽升阶*/	
function Protocols.call_wings_rankup ( playerInfo )
	local output = Protocols.	pack_wings_rankup (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*神羽升阶*/	
function Protocols.unpack_wings_rankup (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*神羽强化*/	
function Protocols.pack_wings_strength (  )
	local output = Packet.new(CMSG_WINGS_STRENGTH)
	return output
end

-- /*神羽强化*/	
function Protocols.call_wings_strength ( playerInfo )
	local output = Protocols.	pack_wings_strength (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*神羽强化*/	
function Protocols.unpack_wings_strength (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*经脉修炼*/	
function Protocols.pack_meridian_practise (  )
	local output = Packet.new(CMSG_MERIDIAN_PRACTISE)
	return output
end

-- /*经脉修炼*/	
function Protocols.call_meridian_practise ( playerInfo )
	local output = Protocols.	pack_meridian_practise (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*经脉修炼*/	
function Protocols.unpack_meridian_practise (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*加经脉修炼经验值*/	
function Protocols.pack_add_meridian_exp ( id)
	local output = Packet.new(CMSG_ADD_MERIDIAN_EXP)
	output:writeByte(id)
	return output
end

-- /*加经脉修炼经验值*/	
function Protocols.call_add_meridian_exp ( playerInfo, id)
	local output = Protocols.	pack_add_meridian_exp ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*加经脉修炼经验值*/	
function Protocols.unpack_add_meridian_exp (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*提升坐骑等级*/	
function Protocols.pack_raise_mount_level_base (  )
	local output = Packet.new(CMSG_RAISE_MOUNT_LEVEL_BASE)
	return output
end

-- /*提升坐骑等级*/	
function Protocols.call_raise_mount_level_base ( playerInfo )
	local output = Protocols.	pack_raise_mount_level_base (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*提升坐骑等级*/	
function Protocols.unpack_raise_mount_level_base (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*解锁坐骑*/	
function Protocols.pack_active_mount (  )
	local output = Packet.new(CMSG_ACTIVE_MOUNT)
	return output
end

-- /*解锁坐骑*/	
function Protocols.call_active_mount ( playerInfo )
	local output = Protocols.	pack_active_mount (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*解锁坐骑*/	
function Protocols.unpack_active_mount (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*家族领主挑战输出排行*/	
function Protocols.pack_show_faction_bossdefense_damage_list ( list)
	local output = Packet.new(SMSG_SHOW_FACTION_BOSSDEFENSE_DAMAGE_LIST)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*家族领主挑战输出排行*/	
function Protocols.call_show_faction_bossdefense_damage_list ( playerInfo, list)
	local output = Protocols.	pack_show_faction_bossdefense_damage_list ( list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*家族领主挑战输出排行*/	
function Protocols.unpack_show_faction_bossdefense_damage_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = mass_boss_rank_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*家族无尽远征扫荡结果*/	
function Protocols.pack_show_faction_tower_sweep_list ( list)
	local output = Packet.new(SMSG_SHOW_FACTION_TOWER_SWEEP_LIST)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*家族无尽远征扫荡结果*/	
function Protocols.call_show_faction_tower_sweep_list ( playerInfo, list)
	local output = Protocols.	pack_show_faction_tower_sweep_list ( list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*家族无尽远征扫荡结果*/	
function Protocols.unpack_show_faction_tower_sweep_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = item_reward_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*副本结果*/	
function Protocols.pack_send_instance_result ( state ,cd ,list ,type ,data)
	local output = Packet.new(SMSG_SEND_INSTANCE_RESULT)
	output:writeByte(state)
	output:writeByte(cd)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	output:writeByte(type)
	output:writeUTF(data)
	return output
end

-- /*副本结果*/	
function Protocols.call_send_instance_result ( playerInfo, state ,cd ,list ,type ,data)
	local output = Protocols.	pack_send_instance_result ( state ,cd ,list ,type ,data)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*副本结果*/	
function Protocols.unpack_send_instance_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.state = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.cd = input:readByte()
	if not ret then
		return false
	end
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = item_reward_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.data = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*匹配单人pvp*/	
function Protocols.pack_match_single_pvp (  )
	local output = Packet.new(CMSG_MATCH_SINGLE_PVP)
	return output
end

-- /*匹配单人pvp*/	
function Protocols.call_match_single_pvp ( playerInfo )
	local output = Protocols.	pack_match_single_pvp (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*匹配单人pvp*/	
function Protocols.unpack_match_single_pvp (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*购买单人pvp次数*/	
function Protocols.pack_buy_match_single_pvp_times ( cnt)
	local output = Packet.new(CMSG_BUY_MATCH_SINGLE_PVP_TIMES)
	output:writeByte(cnt)
	return output
end

-- /*购买单人pvp次数*/	
function Protocols.call_buy_match_single_pvp_times ( playerInfo, cnt)
	local output = Protocols.	pack_buy_match_single_pvp_times ( cnt)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*购买单人pvp次数*/	
function Protocols.unpack_buy_match_single_pvp_times (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.cnt = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*领取单人pvp额外奖励*/	
function Protocols.pack_pick_match_single_pvp_extra_reward ( id)
	local output = Packet.new(CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD)
	output:writeByte(id)
	return output
end

-- /*领取单人pvp额外奖励*/	
function Protocols.call_pick_match_single_pvp_extra_reward ( playerInfo, id)
	local output = Protocols.	pack_pick_match_single_pvp_extra_reward ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取单人pvp额外奖励*/	
function Protocols.unpack_pick_match_single_pvp_extra_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*装备养成操作*/	
function Protocols.pack_equipdevelop_operate ( opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	local output = Packet.new(CMSG_EQUIPDEVELOP_OPERATE)
	output:writeByte(opt_type)
	output:writeI16(reserve_int1)
	output:writeI16(reserve_int2)
	output:writeUTF(reserve_str1)
	output:writeUTF(reserve_str2)
	return output
end

-- /*装备养成操作*/	
function Protocols.call_equipdevelop_operate ( playerInfo, opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	local output = Protocols.	pack_equipdevelop_operate ( opt_type ,reserve_int1 ,reserve_int2 ,reserve_str1 ,reserve_str2)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*装备养成操作*/	
function Protocols.unpack_equipdevelop_operate (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.opt_type = input:readByte()
	if not ret then
		return false
	end
	ret,param_table.reserve_int1 = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.reserve_int2 = input:readU16()
	if not ret then
		return false
	end
	ret,param_table.reserve_str1 = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.reserve_str2 = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*激活外观*/	
function Protocols.pack_active_appearance ( id)
	local output = Packet.new(CMSG_ACTIVE_APPEARANCE)
	output:writeI16(id)
	return output
end

-- /*激活外观*/	
function Protocols.call_active_appearance ( playerInfo, id)
	local output = Protocols.	pack_active_appearance ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*激活外观*/	
function Protocols.unpack_active_appearance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*装备外观*/	
function Protocols.pack_equip_appearance ( id)
	local output = Packet.new(CMSG_EQUIP_APPEARANCE)
	output:writeI16(id)
	return output
end

-- /*装备外观*/	
function Protocols.call_equip_appearance ( playerInfo, id)
	local output = Protocols.	pack_equip_appearance ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*装备外观*/	
function Protocols.unpack_equip_appearance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*取消装备外观*/	
function Protocols.pack_cancel_equip_appearance ( type)
	local output = Packet.new(CMSG_CANCEL_EQUIP_APPEARANCE)
	output:writeByte(type)
	return output
end

-- /*取消装备外观*/	
function Protocols.call_cancel_equip_appearance ( playerInfo, type)
	local output = Protocols.	pack_cancel_equip_appearance ( type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*取消装备外观*/	
function Protocols.unpack_cancel_equip_appearance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*改名*/	
function Protocols.pack_rename ( name)
	local output = Packet.new(CMSG_RENAME)
	output:writeUTF(name)
	return output
end

-- /*改名*/	
function Protocols.call_rename ( playerInfo, name)
	local output = Protocols.	pack_rename ( name)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*改名*/	
function Protocols.unpack_rename (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*道具解锁称号*/	
function Protocols.pack_unlock_title ( indx)
	local output = Packet.new(CMSG_UNLOCK_TITLE)
	output:writeByte(indx)
	return output
end

-- /*道具解锁称号*/	
function Protocols.call_unlock_title ( playerInfo, indx)
	local output = Protocols.	pack_unlock_title ( indx)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*道具解锁称号*/	
function Protocols.unpack_unlock_title (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.indx = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*购买复仇次数*/	
function Protocols.pack_social_buy_revenge_times ( count)
	local output = Packet.new(CMSG_SOCIAL_BUY_REVENGE_TIMES)
	output:writeByte(count)
	return output
end

-- /*购买复仇次数*/	
function Protocols.call_social_buy_revenge_times ( playerInfo, count)
	local output = Protocols.	pack_social_buy_revenge_times ( count)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*购买复仇次数*/	
function Protocols.unpack_social_buy_revenge_times (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.count = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*请求进入世界冒险副本*/	
function Protocols.pack_enter_risk_instance (  )
	local output = Packet.new(CMSG_ENTER_RISK_INSTANCE)
	return output
end

-- /*请求进入世界冒险副本*/	
function Protocols.call_enter_risk_instance ( playerInfo )
	local output = Protocols.	pack_enter_risk_instance (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求进入世界冒险副本*/	
function Protocols.unpack_enter_risk_instance (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*删除仇人*/	
function Protocols.pack_social_remove_enemy ( guid)
	local output = Packet.new(CMSG_SOCIAL_REMOVE_ENEMY)
	output:writeUTF(guid)
	return output
end

-- /*删除仇人*/	
function Protocols.call_social_remove_enemy ( playerInfo, guid)
	local output = Protocols.	pack_social_remove_enemy ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*删除仇人*/	
function Protocols.unpack_social_remove_enemy (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*查看玩家详情*/	
function Protocols.pack_get_player_overview ( guid)
	local output = Packet.new(CMSG_GET_PLAYER_OVERVIEW)
	output:writeUTF(guid)
	return output
end

-- /*查看玩家详情*/	
function Protocols.call_get_player_overview ( playerInfo, guid)
	local output = Protocols.	pack_get_player_overview ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*查看玩家详情*/	
function Protocols.unpack_get_player_overview (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*返回玩家详情*/	
function Protocols.pack_show_player_overview ( guid ,name ,force ,vip ,title ,gender ,coat ,weapon)
	local output = Packet.new(SMSG_SHOW_PLAYER_OVERVIEW)
	output:writeUTF(guid)
	output:writeUTF(name)
	output:writeU32(force)
	output:writeU32(vip)
	output:writeU32(title)
	output:writeU32(gender)
	output:writeU32(coat)
	output:writeU32(weapon)
	return output
end

-- /*返回玩家详情*/	
function Protocols.call_show_player_overview ( playerInfo, guid ,name ,force ,vip ,title ,gender ,coat ,weapon)
	local output = Protocols.	pack_show_player_overview ( guid ,name ,force ,vip ,title ,gender ,coat ,weapon)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回玩家详情*/	
function Protocols.unpack_show_player_overview (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.force = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.vip = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.title = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.gender = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.coat = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.weapon = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*邀请加入帮派*/	
function Protocols.pack_send_faction_invite ( guid)
	local output = Packet.new(CMSG_SEND_FACTION_INVITE)
	output:writeUTF(guid)
	return output
end

-- /*邀请加入帮派*/	
function Protocols.call_send_faction_invite ( playerInfo, guid)
	local output = Protocols.	pack_send_faction_invite ( guid)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*邀请加入帮派*/	
function Protocols.unpack_send_faction_invite (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*显示邀请*/	
function Protocols.pack_show_faction_invite ( guid ,name ,faction_guid ,faction_name)
	local output = Packet.new(SMSG_SHOW_FACTION_INVITE)
	output:writeUTF(guid)
	output:writeUTF(name)
	output:writeUTF(faction_guid)
	output:writeUTF(faction_name)
	return output
end

-- /*显示邀请*/	
function Protocols.call_show_faction_invite ( playerInfo, guid ,name ,faction_guid ,faction_name)
	local output = Protocols.	pack_show_faction_invite ( guid ,name ,faction_guid ,faction_name)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*显示邀请*/	
function Protocols.unpack_show_faction_invite (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.name = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.faction_guid = input:readUTF()
	if not ret then
		return false
	end	
	ret,param_table.faction_name = input:readUTF()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*购买vip礼包*/	
function Protocols.pack_buy_vipgift ( id)
	local output = Packet.new(CMSG_BUY_VIPGIFT)
	output:writeU32(id)
	return output
end

-- /*购买vip礼包*/	
function Protocols.call_buy_vipgift ( playerInfo, id)
	local output = Protocols.	pack_buy_vipgift ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*购买vip礼包*/	
function Protocols.unpack_buy_vipgift (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*购买活动每日礼包*/	
function Protocols.pack_activity_opt_buy_dailygift ( act_id ,index)
	local output = Packet.new(CMSG_ACTIVITY_OPT_BUY_DAILYGIFT)
	output:writeU32(act_id)
	output:writeU32(index)
	return output
end

-- /*购买活动每日礼包*/	
function Protocols.call_activity_opt_buy_dailygift ( playerInfo, act_id ,index)
	local output = Protocols.	pack_activity_opt_buy_dailygift ( act_id ,index)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*购买活动每日礼包*/	
function Protocols.unpack_activity_opt_buy_dailygift (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.act_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.index = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*抽奖*/	
function Protocols.pack_draw_lottery ( actId ,type)
	local output = Packet.new(CMSG_DRAW_LOTTERY)
	output:writeU32(actId)
	output:writeByte(type)
	return output
end

-- /*抽奖*/	
function Protocols.call_draw_lottery ( playerInfo, actId ,type)
	local output = Protocols.	pack_draw_lottery ( actId ,type)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*抽奖*/	
function Protocols.unpack_draw_lottery (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.actId = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.type = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*获取活动开服排行进度奖励*/	
function Protocols.pack_activity_opt_get_rank_process_reward ( act_id ,index)
	local output = Packet.new(CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD)
	output:writeU32(act_id)
	output:writeU32(index)
	return output
end

-- /*获取活动开服排行进度奖励*/	
function Protocols.call_activity_opt_get_rank_process_reward ( playerInfo, act_id ,index)
	local output = Protocols.	pack_activity_opt_get_rank_process_reward ( act_id ,index)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*获取活动开服排行进度奖励*/	
function Protocols.unpack_activity_opt_get_rank_process_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.act_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.index = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*获取活动开服排行榜*/	
function Protocols.pack_activity_opt_get_rank_list ( act_id)
	local output = Packet.new(CMSG_ACTIVITY_OPT_GET_RANK_LIST)
	output:writeU32(act_id)
	return output
end

-- /*获取活动开服排行榜*/	
function Protocols.call_activity_opt_get_rank_list ( playerInfo, act_id)
	local output = Protocols.	pack_activity_opt_get_rank_list ( act_id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*获取活动开服排行榜*/	
function Protocols.unpack_activity_opt_get_rank_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.act_id = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*返回活动开服排行榜*/	
function Protocols.pack_activity_opt_show_rank_list ( act_id ,list)
	local output = Packet.new(SMSG_ACTIVITY_OPT_SHOW_RANK_LIST)
	output:writeU32(act_id)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*返回活动开服排行榜*/	
function Protocols.call_activity_opt_show_rank_list ( playerInfo, act_id ,list)
	local output = Protocols.	pack_activity_opt_show_rank_list ( act_id ,list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*返回活动开服排行榜*/	
function Protocols.unpack_activity_opt_show_rank_list (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.act_id = input:readU32()
	if not ret then
		return false
	end	
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = act_rank_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*购买活动限定礼包*/	
function Protocols.pack_activity_opt_buy_limitgift ( act_id ,index)
	local output = Packet.new(CMSG_ACTIVITY_OPT_BUY_LIMITGIFT)
	output:writeU32(act_id)
	output:writeU32(index)
	return output
end

-- /*购买活动限定礼包*/	
function Protocols.call_activity_opt_buy_limitgift ( playerInfo, act_id ,index)
	local output = Protocols.	pack_activity_opt_buy_limitgift ( act_id ,index)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*购买活动限定礼包*/	
function Protocols.unpack_activity_opt_buy_limitgift (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.act_id = input:readU32()
	if not ret then
		return false
	end	
	ret,param_table.index = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*领取累计充值奖励*/	
function Protocols.pack_welfare_get_recharge_reward ( id)
	local output = Packet.new(CMSG_WELFARE_GET_RECHARGE_REWARD)
	output:writeByte(id)
	return output
end

-- /*领取累计充值奖励*/	
function Protocols.call_welfare_get_recharge_reward ( playerInfo, id)
	local output = Protocols.	pack_welfare_get_recharge_reward ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取累计充值奖励*/	
function Protocols.unpack_welfare_get_recharge_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*领取累计消费奖励*/	
function Protocols.pack_welfare_get_consume_reward ( id)
	local output = Packet.new(CMSG_WELFARE_GET_CONSUME_REWARD)
	output:writeByte(id)
	return output
end

-- /*领取累计消费奖励*/	
function Protocols.call_welfare_get_consume_reward ( playerInfo, id)
	local output = Protocols.	pack_welfare_get_consume_reward ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取累计消费奖励*/	
function Protocols.unpack_welfare_get_consume_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*领取七日大礼奖励*/	
function Protocols.pack_welfare_get_sevenday_reward ( id)
	local output = Packet.new(CMSG_WELFARE_GET_SEVENDAY_REWARD)
	output:writeByte(id)
	return output
end

-- /*领取七日大礼奖励*/	
function Protocols.call_welfare_get_sevenday_reward ( playerInfo, id)
	local output = Protocols.	pack_welfare_get_sevenday_reward ( id)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*领取七日大礼奖励*/	
function Protocols.unpack_welfare_get_sevenday_reward (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.id = input:readByte()
	if not ret then
		return false
	end

	return true,param_table	

end


-- /*服务器开服时间*/	
function Protocols.pack_send_server_open_time ( open_time)
	local output = Packet.new(SMSG_SEND_SERVER_OPEN_TIME)
	output:writeU32(open_time)
	return output
end

-- /*服务器开服时间*/	
function Protocols.call_send_server_open_time ( playerInfo, open_time)
	local output = Protocols.	pack_send_server_open_time ( open_time)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*服务器开服时间*/	
function Protocols.unpack_send_server_open_time (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.open_time = input:readU32()
	if not ret then
		return false
	end	

	return true,param_table	

end


-- /*请求世界冒险排行榜*/	
function Protocols.pack_risk_get_rank (  )
	local output = Packet.new(CMSG_RISK_GET_RANK)
	return output
end

-- /*请求世界冒险排行榜*/	
function Protocols.call_risk_get_rank ( playerInfo )
	local output = Protocols.	pack_risk_get_rank (  )
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*请求世界冒险排行榜*/	
function Protocols.unpack_risk_get_rank (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret

	return true,{}
	

end


-- /*世界冒险排行榜信息 */	
function Protocols.pack_risk_get_rank_result ( list)
	local output = Packet.new(SMSG_RISK_GET_RANK_RESULT)
	output:writeI16(#list)
	for i = 1,#list,1
	do
		list[i]:write(output)
	end
	return output
end

-- /*世界冒险排行榜信息 */	
function Protocols.call_risk_get_rank_result ( playerInfo, list)
	local output = Protocols.	pack_risk_get_rank_result ( list)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*世界冒险排行榜信息 */	
function Protocols.unpack_risk_get_rank_result (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,len = input:readU16()
	if not ret then
		return false
	end
	param_table.list = {}
	for i = 1,len,1
	do
		local stru = act_rank_info_t .new()
		if(stru:read(input)==false)then
			return false
		end
		table.insert(param_table.list,stru)
	end

	return true,param_table	

end


-- /*设置朝向*/	
function Protocols.pack_set_orient ( angle)
	local output = Packet.new(CMSG_SET_ORIENT)
	output:writeI16(angle)
	return output
end

-- /*设置朝向*/	
function Protocols.call_set_orient ( playerInfo, angle)
	local output = Protocols.	pack_set_orient ( angle)
	playerInfo:SendPacket(output)
	output:delete()
end

-- /*设置朝向*/	
function Protocols.unpack_set_orient (pkt)
	local input = Packet.new(nil, pkt)
	local param_table = {}
	local ret
	ret,param_table.angle = input:readU16()
	if not ret then
		return false
	end

	return true,param_table	

end



function Protocols:SendPacket(pkt)
	external_send(self.ptr_player_data or self.ptr, pkt.ptr)
end

function Protocols:extend(playerInfo)
	playerInfo.SendPacket = self.SendPacket
	playerInfo.call_null_action = self.call_null_action
	playerInfo.call_ping_pong = self.call_ping_pong
	playerInfo.call_forced_into = self.call_forced_into
	playerInfo.call_get_session = self.call_get_session
	playerInfo.call_route_trace = self.call_route_trace
	playerInfo.call_write_client_log = self.call_write_client_log
	playerInfo.call_operation_failed = self.call_operation_failed
	playerInfo.call_sync_mstime = self.call_sync_mstime
	playerInfo.call_ud_object = self.call_ud_object
	playerInfo.call_ud_control = self.call_ud_control
	playerInfo.call_ud_control_result = self.call_ud_control_result
	playerInfo.call_grid_ud_object = self.call_grid_ud_object
	playerInfo.call_grid_ud_object_2 = self.call_grid_ud_object_2
	playerInfo.call_login_queue_index = self.call_login_queue_index
	playerInfo.call_kicking_type = self.call_kicking_type
	playerInfo.call_get_chars_list = self.call_get_chars_list
	playerInfo.call_chars_list = self.call_chars_list
	playerInfo.call_check_name = self.call_check_name
	playerInfo.call_check_name_result = self.call_check_name_result
	playerInfo.call_char_create = self.call_char_create
	playerInfo.call_char_create_result = self.call_char_create_result
	playerInfo.call_delete_char = self.call_delete_char
	playerInfo.call_delete_char_result = self.call_delete_char_result
	playerInfo.call_player_login = self.call_player_login
	playerInfo.call_player_logout = self.call_player_logout
	playerInfo.call_regularise_account = self.call_regularise_account
	playerInfo.call_char_remotestore = self.call_char_remotestore
	playerInfo.call_char_remotestore_str = self.call_char_remotestore_str
	playerInfo.call_teleport = self.call_teleport
	playerInfo.call_move_stop = self.call_move_stop
	playerInfo.call_unit_move = self.call_unit_move
	playerInfo.call_use_gameobject = self.call_use_gameobject
	playerInfo.call_bag_exchange_pos = self.call_bag_exchange_pos
	playerInfo.call_bag_destroy = self.call_bag_destroy
	playerInfo.call_bag_item_split = self.call_bag_item_split
	playerInfo.call_bag_item_user = self.call_bag_item_user
	playerInfo.call_bag_item_cooldown = self.call_bag_item_cooldown
	playerInfo.call_grid_unit_move = self.call_grid_unit_move
	playerInfo.call_grid_unit_move_2 = self.call_grid_unit_move_2
	playerInfo.call_exchange_item = self.call_exchange_item
	playerInfo.call_bag_extension = self.call_bag_extension
	playerInfo.call_npc_get_goods_list = self.call_npc_get_goods_list
	playerInfo.call_npc_goods_list = self.call_npc_goods_list
	playerInfo.call_store_buy = self.call_store_buy
	playerInfo.call_npc_sell = self.call_npc_sell
	playerInfo.call_npc_repurchase = self.call_npc_repurchase
	playerInfo.call_avatar_fashion_enable = self.call_avatar_fashion_enable
	playerInfo.call_questhelp_talk_option = self.call_questhelp_talk_option
	playerInfo.call_taxi_hello = self.call_taxi_hello
	playerInfo.call_taxi_stations_list = self.call_taxi_stations_list
	playerInfo.call_taxi_select_station = self.call_taxi_select_station
	playerInfo.call_gossip_select_option = self.call_gossip_select_option
	playerInfo.call_gossip_hello = self.call_gossip_hello
	playerInfo.call_gossip_message = self.call_gossip_message
	playerInfo.call_questgiver_status_query = self.call_questgiver_status_query
	playerInfo.call_questgiver_status = self.call_questgiver_status
	playerInfo.call_query_quest_status = self.call_query_quest_status
	playerInfo.call_questhelp_get_canaccept_list = self.call_questhelp_get_canaccept_list
	playerInfo.call_questhelp_canaccept_list = self.call_questhelp_canaccept_list
	playerInfo.call_questupdate_faild = self.call_questupdate_faild
	playerInfo.call_questupdate_complete = self.call_questupdate_complete
	playerInfo.call_questlog_remove_quest = self.call_questlog_remove_quest
	playerInfo.call_questgiver_complete_quest = self.call_questgiver_complete_quest
	playerInfo.call_questhelp_next = self.call_questhelp_next
	playerInfo.call_questhelp_complete = self.call_questhelp_complete
	playerInfo.call_questupdate_accept = self.call_questupdate_accept
	playerInfo.call_questhelp_update_status = self.call_questhelp_update_status
	playerInfo.call_questgetter_complete = self.call_questgetter_complete
	playerInfo.call_questgiver_accept_quest = self.call_questgiver_accept_quest
	playerInfo.call_questupdate_use_item = self.call_questupdate_use_item
	playerInfo.call_questhelp_query_book = self.call_questhelp_query_book
	playerInfo.call_questhelp_book_quest = self.call_questhelp_book_quest
	playerInfo.call_use_gameobject_action = self.call_use_gameobject_action
	playerInfo.call_set_attack_mode = self.call_set_attack_mode
	playerInfo.call_select_target = self.call_select_target
	playerInfo.call_combat_state_update = self.call_combat_state_update
	playerInfo.call_exp_update = self.call_exp_update
	playerInfo.call_spell_start = self.call_spell_start
	playerInfo.call_spell_stop = self.call_spell_stop
	playerInfo.call_jump = self.call_jump
	playerInfo.call_resurrection = self.call_resurrection
	playerInfo.call_trade_request = self.call_trade_request
	playerInfo.call_trade_reply = self.call_trade_reply
	playerInfo.call_trade_start = self.call_trade_start
	playerInfo.call_trade_decide_items = self.call_trade_decide_items
	playerInfo.call_trade_finish = self.call_trade_finish
	playerInfo.call_trade_cancel = self.call_trade_cancel
	playerInfo.call_trade_ready = self.call_trade_ready
	playerInfo.call_chat_unit_talk = self.call_chat_unit_talk
	playerInfo.call_chat_near = self.call_chat_near
	playerInfo.call_chat_whisper = self.call_chat_whisper
	playerInfo.call_chat_faction = self.call_chat_faction
	playerInfo.call_chat_world = self.call_chat_world
	playerInfo.call_chat_horn = self.call_chat_horn
	playerInfo.call_chat_notice = self.call_chat_notice
	playerInfo.call_query_player_info = self.call_query_player_info
	playerInfo.call_query_result_update_object = self.call_query_result_update_object
	playerInfo.call_receive_gift_packs = self.call_receive_gift_packs
	playerInfo.call_map_update_object = self.call_map_update_object
	playerInfo.call_fighting_info_update_object = self.call_fighting_info_update_object
	playerInfo.call_fighting_info_update_object_2 = self.call_fighting_info_update_object_2
	playerInfo.call_instance_enter = self.call_instance_enter
	playerInfo.call_instance_next_state = self.call_instance_next_state
	playerInfo.call_instance_exit = self.call_instance_exit
	playerInfo.call_limit_activity_receive = self.call_limit_activity_receive
	playerInfo.call_kill_man = self.call_kill_man
	playerInfo.call_player_upgrade = self.call_player_upgrade
	playerInfo.call_warehouse_save_money = self.call_warehouse_save_money
	playerInfo.call_warehouse_take_money = self.call_warehouse_take_money
	playerInfo.call_use_gold_opt = self.call_use_gold_opt
	playerInfo.call_use_silver_opt = self.call_use_silver_opt
	playerInfo.call_gm_rightfloat = self.call_gm_rightfloat
	playerInfo.call_del_gm_rightfloat = self.call_del_gm_rightfloat
	playerInfo.call_sync_mstime_app = self.call_sync_mstime_app
	playerInfo.call_open_window = self.call_open_window
	playerInfo.call_player_gag = self.call_player_gag
	playerInfo.call_player_kicking = self.call_player_kicking
	playerInfo.call_merge_server_msg = self.call_merge_server_msg
	playerInfo.call_rank_list_query = self.call_rank_list_query
	playerInfo.call_rank_list_query_result = self.call_rank_list_query_result
	playerInfo.call_client_update_scened = self.call_client_update_scened
	playerInfo.call_num_lua = self.call_num_lua
	playerInfo.call_loot_select = self.call_loot_select
	playerInfo.call_goback_to_game_server = self.call_goback_to_game_server
	playerInfo.call_world_war_CS_player_info = self.call_world_war_CS_player_info
	playerInfo.call_join_or_leave_server = self.call_join_or_leave_server
	playerInfo.call_world_war_SC_player_info = self.call_world_war_SC_player_info
	playerInfo.call_clientSubscription = self.call_clientSubscription
	playerInfo.call_lua_script = self.call_lua_script
	playerInfo.call_char_update_info = self.call_char_update_info
	playerInfo.call_notice_watcher_map_info = self.call_notice_watcher_map_info
	playerInfo.call_modify_watch = self.call_modify_watch
	playerInfo.call_kuafu_chuansong = self.call_kuafu_chuansong
	playerInfo.call_show_suit = self.call_show_suit
	playerInfo.call_show_position = self.call_show_position
	playerInfo.call_gold_respawn = self.call_gold_respawn
	playerInfo.call_field_death_cooldown = self.call_field_death_cooldown
	playerInfo.call_mall_buy = self.call_mall_buy
	playerInfo.call_strength = self.call_strength
	playerInfo.call_strength_success = self.call_strength_success
	playerInfo.call_forceInto = self.call_forceInto
	playerInfo.call_create_faction = self.call_create_faction
	playerInfo.call_faction_upgrade = self.call_faction_upgrade
	playerInfo.call_faction_join = self.call_faction_join
	playerInfo.call_raise_base_spell = self.call_raise_base_spell
	playerInfo.call_upgrade_anger_spell = self.call_upgrade_anger_spell
	playerInfo.call_raise_mount = self.call_raise_mount
	playerInfo.call_upgrade_mount = self.call_upgrade_mount
	playerInfo.call_upgrade_mount_one_step = self.call_upgrade_mount_one_step
	playerInfo.call_illusion_mount_active = self.call_illusion_mount_active
	playerInfo.call_illusion_mount = self.call_illusion_mount
	playerInfo.call_ride_mount = self.call_ride_mount
	playerInfo.call_grid_unit_jump = self.call_grid_unit_jump
	playerInfo.call_gem = self.call_gem
	playerInfo.call_change_battle_mode = self.call_change_battle_mode
	playerInfo.call_peace_mode_cd = self.call_peace_mode_cd
	playerInfo.call_divine_active = self.call_divine_active
	playerInfo.call_divine_uplev = self.call_divine_uplev
	playerInfo.call_divine_switch = self.call_divine_switch
	playerInfo.call_jump_start = self.call_jump_start
	playerInfo.call_enter_vip_instance = self.call_enter_vip_instance
	playerInfo.call_sweep_vip_instance = self.call_sweep_vip_instance
	playerInfo.call_hang_up = self.call_hang_up
	playerInfo.call_hang_up_setting = self.call_hang_up_setting
	playerInfo.call_enter_trial_instance = self.call_enter_trial_instance
	playerInfo.call_sweep_trial_instance = self.call_sweep_trial_instance
	playerInfo.call_reset_trial_instance = self.call_reset_trial_instance
	playerInfo.call_sweep_instance_reward = self.call_sweep_instance_reward
	playerInfo.call_reenter_instance = self.call_reenter_instance
	playerInfo.call_merry_go_round = self.call_merry_go_round
	playerInfo.call_social_add_friend = self.call_social_add_friend
	playerInfo.call_social_sureadd_friend = self.call_social_sureadd_friend
	playerInfo.call_social_gift_friend = self.call_social_gift_friend
	playerInfo.call_social_recommend_friend = self.call_social_recommend_friend
	playerInfo.call_social_get_recommend_friend = self.call_social_get_recommend_friend
	playerInfo.call_social_revenge_enemy = self.call_social_revenge_enemy
	playerInfo.call_social_del_friend = self.call_social_del_friend
	playerInfo.call_teleport_main_city = self.call_teleport_main_city
	playerInfo.call_chat_by_channel = self.call_chat_by_channel
	playerInfo.call_send_chat = self.call_send_chat
	playerInfo.call_social_clear_apply = self.call_social_clear_apply
	playerInfo.call_msg_decline = self.call_msg_decline
	playerInfo.call_faction_get_list_result = self.call_faction_get_list_result
	playerInfo.call_faction_getlist = self.call_faction_getlist
	playerInfo.call_faction_manager = self.call_faction_manager
	playerInfo.call_faction_member_operate = self.call_faction_member_operate
	playerInfo.call_faction_fast_join = self.call_faction_fast_join
	playerInfo.call_social_add_friend_byname = self.call_social_add_friend_byname
	playerInfo.call_read_mail = self.call_read_mail
	playerInfo.call_pick_mail = self.call_pick_mail
	playerInfo.call_remove_mail = self.call_remove_mail
	playerInfo.call_pick_mail_one_step = self.call_pick_mail_one_step
	playerInfo.call_remove_mail_one_step = self.call_remove_mail_one_step
	playerInfo.call_block_chat = self.call_block_chat
	playerInfo.call_cancel_block_chat = self.call_cancel_block_chat
	playerInfo.call_use_broadcast_gameobject = self.call_use_broadcast_gameobject
	playerInfo.call_world_boss_enroll = self.call_world_boss_enroll
	playerInfo.call_world_boss_fight = self.call_world_boss_fight
	playerInfo.call_change_line = self.call_change_line
	playerInfo.call_roll_world_boss_treasure = self.call_roll_world_boss_treasure
	playerInfo.call_roll_result = self.call_roll_result
	playerInfo.call_boss_rank = self.call_boss_rank
	playerInfo.call_rank_add_like = self.call_rank_add_like
	playerInfo.call_rank_add_like_result = self.call_rank_add_like_result
	playerInfo.call_res_instance_enter = self.call_res_instance_enter
	playerInfo.call_res_instance_sweep = self.call_res_instance_sweep
	playerInfo.call_show_map_line = self.call_show_map_line
	playerInfo.call_send_map_line = self.call_send_map_line
	playerInfo.call_item_notice = self.call_item_notice
	playerInfo.call_teleport_map = self.call_teleport_map
	playerInfo.call_teleport_field_boss = self.call_teleport_field_boss
	playerInfo.call_get_activity_reward = self.call_get_activity_reward
	playerInfo.call_get_achieve_reward = self.call_get_achieve_reward
	playerInfo.call_get_achieve_all_reward = self.call_get_achieve_all_reward
	playerInfo.call_set_title = self.call_set_title
	playerInfo.call_init_title = self.call_init_title
	playerInfo.call_welfare_shouchong_reward = self.call_welfare_shouchong_reward
	playerInfo.call_welfare_checkin = self.call_welfare_checkin
	playerInfo.call_welfare_checkin_all = self.call_welfare_checkin_all
	playerInfo.call_welfare_checkin_getback = self.call_welfare_checkin_getback
	playerInfo.call_welfare_level = self.call_welfare_level
	playerInfo.call_welfare_active_getback = self.call_welfare_active_getback
	playerInfo.call_pick_quest_reward = self.call_pick_quest_reward
	playerInfo.call_talk_with_npc = self.call_talk_with_npc
	playerInfo.call_use_virtual_item = self.call_use_virtual_item
	playerInfo.call_pick_quest_chapter_reward = self.call_pick_quest_chapter_reward
	playerInfo.call_kuafu_3v3_match = self.call_kuafu_3v3_match
	playerInfo.call_kuafu_match_start = self.call_kuafu_match_start
	playerInfo.call_kuafu_3v3_buytimes = self.call_kuafu_3v3_buytimes
	playerInfo.call_kuafu_3v3_dayreward = self.call_kuafu_3v3_dayreward
	playerInfo.call_kuafu_3v3_getranlist = self.call_kuafu_3v3_getranlist
	playerInfo.call_kuafu_3v3_ranlist = self.call_kuafu_3v3_ranlist
	playerInfo.call_welfare_getalllist_getback = self.call_welfare_getalllist_getback
	playerInfo.call_welfare_rewardlist_getback = self.call_welfare_rewardlist_getback
	playerInfo.call_welfare_getall_getback = self.call_welfare_getall_getback
	playerInfo.call_kuafu_3v3_getmyrank = self.call_kuafu_3v3_getmyrank
	playerInfo.call_kuafu_3v3_myrank = self.call_kuafu_3v3_myrank
	playerInfo.call_kuafu_3v3_kill_detail = self.call_kuafu_3v3_kill_detail
	playerInfo.call_kuafu_3v3_wait_info = self.call_kuafu_3v3_wait_info
	playerInfo.call_kuafu_3v3_cancel_match = self.call_kuafu_3v3_cancel_match
	playerInfo.call_kuafu_3v3_match_oper = self.call_kuafu_3v3_match_oper
	playerInfo.call_kuafu_3v3_decline_match = self.call_kuafu_3v3_decline_match
	playerInfo.call_kuafu_xianfu_match = self.call_kuafu_xianfu_match
	playerInfo.call_kuafu_match_wait = self.call_kuafu_match_wait
	playerInfo.call_kuafu_xianfu_minimap_info = self.call_kuafu_xianfu_minimap_info
	playerInfo.call_buy_xianfu_item = self.call_buy_xianfu_item
	playerInfo.call_xianfu_random_respawn = self.call_xianfu_random_respawn
	playerInfo.call_doujiantai_fight = self.call_doujiantai_fight
	playerInfo.call_doujiantai_buytime = self.call_doujiantai_buytime
	playerInfo.call_doujiantai_clearcd = self.call_doujiantai_clearcd
	playerInfo.call_doujiantai_first_reward = self.call_doujiantai_first_reward
	playerInfo.call_doujiantai_get_enemys_info = self.call_doujiantai_get_enemys_info
	playerInfo.call_doujiantai_get_rank = self.call_doujiantai_get_rank
	playerInfo.call_doujiantai_refresh_enemys = self.call_doujiantai_refresh_enemys
	playerInfo.call_doujiantai_top3 = self.call_doujiantai_top3
	playerInfo.call_use_jump_point = self.call_use_jump_point
	playerInfo.call_bag_item_sell = self.call_bag_item_sell
	playerInfo.call_bag_item_sort = self.call_bag_item_sort
	playerInfo.call_submit_quest_daily2 = self.call_submit_quest_daily2
	playerInfo.call_attribute_changed = self.call_attribute_changed
	playerInfo.call_bag_find_equip_better = self.call_bag_find_equip_better
	playerInfo.call_module_active = self.call_module_active
	playerInfo.call_pick_daily2_quest_reward = self.call_pick_daily2_quest_reward
	playerInfo.call_finish_now_guide = self.call_finish_now_guide
	playerInfo.call_get_cultivation_info = self.call_get_cultivation_info
	playerInfo.call_update_cultivation_info = self.call_update_cultivation_info
	playerInfo.call_get_cultivation_rivals_info = self.call_get_cultivation_rivals_info
	playerInfo.call_update_cultivation_rivals_info_list = self.call_update_cultivation_rivals_info_list
	playerInfo.call_get_cultivation_reward = self.call_get_cultivation_reward
	playerInfo.call_refresh_cultivation_rivals = self.call_refresh_cultivation_rivals
	playerInfo.call_plunder_cultivation_rival = self.call_plunder_cultivation_rival
	playerInfo.call_revenge_cultivation_rival = self.call_revenge_cultivation_rival
	playerInfo.call_buy_cultivation_left_plunder_count = self.call_buy_cultivation_left_plunder_count
	playerInfo.call_show_cultivation_result_list = self.call_show_cultivation_result_list
	playerInfo.call_get_login_activity_reward = self.call_get_login_activity_reward
	playerInfo.call_cast_spell_start = self.call_cast_spell_start
	playerInfo.call_finish_optional_guide_step = self.call_finish_optional_guide_step
	playerInfo.call_execute_quest_cmd_after_accepted = self.call_execute_quest_cmd_after_accepted
	playerInfo.call_show_unit_attribute = self.call_show_unit_attribute
	playerInfo.call_back_to_famity = self.call_back_to_famity
	playerInfo.call_faction_boss_send_result = self.call_faction_boss_send_result
	playerInfo.call_challange_boss = self.call_challange_boss
	playerInfo.call_pick_offline_reward = self.call_pick_offline_reward
	playerInfo.call_offline_reward_result = self.call_offline_reward_result
	playerInfo.call_smelting_equip = self.call_smelting_equip
	playerInfo.call_storehouse_hand_in = self.call_storehouse_hand_in
	playerInfo.call_storehouse_exchange = self.call_storehouse_exchange
	playerInfo.call_storehouse_destroy = self.call_storehouse_destroy
	playerInfo.call_send_faction_gift = self.call_send_faction_gift
	playerInfo.call_get_faction_gift_exreward = self.call_get_faction_gift_exreward
	playerInfo.call_get_all_faction_gift_exreward = self.call_get_all_faction_gift_exreward
	playerInfo.call_show_faction_gift_page = self.call_show_faction_gift_page
	playerInfo.call_show_faction_gift_info = self.call_show_faction_gift_info
	playerInfo.call_show_faction_gift_unthank_page = self.call_show_faction_gift_unthank_page
	playerInfo.call_show_faction_gift_history_page = self.call_show_faction_gift_history_page
	playerInfo.call_get_faction_gift_rank_page = self.call_get_faction_gift_rank_page
	playerInfo.call_show_faction_gift_rank_result_list = self.call_show_faction_gift_rank_result_list
	playerInfo.call_show_faction_gift_rank_change = self.call_show_faction_gift_rank_change
	playerInfo.call_show_faction_gift_rank_info = self.call_show_faction_gift_rank_info
	playerInfo.call_divine_forge = self.call_divine_forge
	playerInfo.call_divine_advance = self.call_divine_advance
	playerInfo.call_divine_spirit = self.call_divine_spirit
	playerInfo.call_query_mass_boss_info = self.call_query_mass_boss_info
	playerInfo.call_mass_boss_info_ret = self.call_mass_boss_info_ret
	playerInfo.call_query_mass_boss_rank = self.call_query_mass_boss_rank
	playerInfo.call_mass_boss_rank_result = self.call_mass_boss_rank_result
	playerInfo.call_try_mass_boss = self.call_try_mass_boss
	playerInfo.call_buy_mass_boss_times = self.call_buy_mass_boss_times
	playerInfo.call_group_instance_match = self.call_group_instance_match
	playerInfo.call_buy_group_instance_times = self.call_buy_group_instance_times
	playerInfo.call_talisman_active = self.call_talisman_active
	playerInfo.call_talisman_lvup = self.call_talisman_lvup
	playerInfo.call_wings_active = self.call_wings_active
	playerInfo.call_wings_bless = self.call_wings_bless
	playerInfo.call_wings_rankup = self.call_wings_rankup
	playerInfo.call_wings_strength = self.call_wings_strength
	playerInfo.call_meridian_practise = self.call_meridian_practise
	playerInfo.call_add_meridian_exp = self.call_add_meridian_exp
	playerInfo.call_raise_mount_level_base = self.call_raise_mount_level_base
	playerInfo.call_active_mount = self.call_active_mount
	playerInfo.call_show_faction_bossdefense_damage_list = self.call_show_faction_bossdefense_damage_list
	playerInfo.call_show_faction_tower_sweep_list = self.call_show_faction_tower_sweep_list
	playerInfo.call_send_instance_result = self.call_send_instance_result
	playerInfo.call_match_single_pvp = self.call_match_single_pvp
	playerInfo.call_buy_match_single_pvp_times = self.call_buy_match_single_pvp_times
	playerInfo.call_pick_match_single_pvp_extra_reward = self.call_pick_match_single_pvp_extra_reward
	playerInfo.call_equipdevelop_operate = self.call_equipdevelop_operate
	playerInfo.call_active_appearance = self.call_active_appearance
	playerInfo.call_equip_appearance = self.call_equip_appearance
	playerInfo.call_cancel_equip_appearance = self.call_cancel_equip_appearance
	playerInfo.call_rename = self.call_rename
	playerInfo.call_unlock_title = self.call_unlock_title
	playerInfo.call_social_buy_revenge_times = self.call_social_buy_revenge_times
	playerInfo.call_enter_risk_instance = self.call_enter_risk_instance
	playerInfo.call_social_remove_enemy = self.call_social_remove_enemy
	playerInfo.call_get_player_overview = self.call_get_player_overview
	playerInfo.call_show_player_overview = self.call_show_player_overview
	playerInfo.call_send_faction_invite = self.call_send_faction_invite
	playerInfo.call_show_faction_invite = self.call_show_faction_invite
	playerInfo.call_buy_vipgift = self.call_buy_vipgift
	playerInfo.call_activity_opt_buy_dailygift = self.call_activity_opt_buy_dailygift
	playerInfo.call_draw_lottery = self.call_draw_lottery
	playerInfo.call_activity_opt_get_rank_process_reward = self.call_activity_opt_get_rank_process_reward
	playerInfo.call_activity_opt_get_rank_list = self.call_activity_opt_get_rank_list
	playerInfo.call_activity_opt_show_rank_list = self.call_activity_opt_show_rank_list
	playerInfo.call_activity_opt_buy_limitgift = self.call_activity_opt_buy_limitgift
	playerInfo.call_welfare_get_recharge_reward = self.call_welfare_get_recharge_reward
	playerInfo.call_welfare_get_consume_reward = self.call_welfare_get_consume_reward
	playerInfo.call_welfare_get_sevenday_reward = self.call_welfare_get_sevenday_reward
	playerInfo.call_send_server_open_time = self.call_send_server_open_time
	playerInfo.call_risk_get_rank = self.call_risk_get_rank
	playerInfo.call_risk_get_rank_result = self.call_risk_get_rank_result
	playerInfo.call_set_orient = self.call_set_orient
end

local unpack_handler = {

[MSG_NULL_ACTION] =  Protocols.unpack_null_action,
[MSG_PING_PONG] =  Protocols.unpack_ping_pong,
[CMSG_FORCED_INTO] =  Protocols.unpack_forced_into,
[CMSG_GET_SESSION] =  Protocols.unpack_get_session,
[MSG_ROUTE_TRACE] =  Protocols.unpack_route_trace,
[CMSG_WRITE_CLIENT_LOG] =  Protocols.unpack_write_client_log,
[SMSG_OPERATION_FAILED] =  Protocols.unpack_operation_failed,
[MSG_SYNC_MSTIME] =  Protocols.unpack_sync_mstime,
[SMSG_UD_OBJECT] =  Protocols.unpack_ud_object,
[CMSG_UD_CONTROL] =  Protocols.unpack_ud_control,
[SMSG_UD_CONTROL_RESULT] =  Protocols.unpack_ud_control_result,
[SMSG_GRID_UD_OBJECT] =  Protocols.unpack_grid_ud_object,
[SMSG_GRID_UD_OBJECT_2] =  Protocols.unpack_grid_ud_object_2,
[SMSG_LOGIN_QUEUE_INDEX] =  Protocols.unpack_login_queue_index,
[SMSG_KICKING_TYPE] =  Protocols.unpack_kicking_type,
[CMSG_GET_CHARS_LIST] =  Protocols.unpack_get_chars_list,
[SMSG_CHARS_LIST] =  Protocols.unpack_chars_list,
[CMSG_CHECK_NAME] =  Protocols.unpack_check_name,
[SMSG_CHECK_NAME_RESULT] =  Protocols.unpack_check_name_result,
[CMSG_CHAR_CREATE] =  Protocols.unpack_char_create,
[SMSG_CHAR_CREATE_RESULT] =  Protocols.unpack_char_create_result,
[CMSG_DELETE_CHAR] =  Protocols.unpack_delete_char,
[SMSG_DELETE_CHAR_RESULT] =  Protocols.unpack_delete_char_result,
[CMSG_PLAYER_LOGIN] =  Protocols.unpack_player_login,
[CMSG_PLAYER_LOGOUT] =  Protocols.unpack_player_logout,
[CMSG_REGULARISE_ACCOUNT] =  Protocols.unpack_regularise_account,
[CMSG_CHAR_REMOTESTORE] =  Protocols.unpack_char_remotestore,
[CMSG_CHAR_REMOTESTORE_STR] =  Protocols.unpack_char_remotestore_str,
[CMSG_TELEPORT] =  Protocols.unpack_teleport,
[MSG_MOVE_STOP] =  Protocols.unpack_move_stop,
[MSG_UNIT_MOVE] =  Protocols.unpack_unit_move,
[CMSG_USE_GAMEOBJECT] =  Protocols.unpack_use_gameobject,
[CMSG_BAG_EXCHANGE_POS] =  Protocols.unpack_bag_exchange_pos,
[CMSG_BAG_DESTROY] =  Protocols.unpack_bag_destroy,
[CMSG_BAG_ITEM_SPLIT] =  Protocols.unpack_bag_item_split,
[CMSG_BAG_ITEM_USER] =  Protocols.unpack_bag_item_user,
[SMSG_BAG_ITEM_COOLDOWN] =  Protocols.unpack_bag_item_cooldown,
[SMSG_GRID_UNIT_MOVE] =  Protocols.unpack_grid_unit_move,
[SMSG_GRID_UNIT_MOVE_2] =  Protocols.unpack_grid_unit_move_2,
[CMSG_EXCHANGE_ITEM] =  Protocols.unpack_exchange_item,
[CMSG_BAG_EXTENSION] =  Protocols.unpack_bag_extension,
[CMSG_NPC_GET_GOODS_LIST] =  Protocols.unpack_npc_get_goods_list,
[SMSG_NPC_GOODS_LIST] =  Protocols.unpack_npc_goods_list,
[CMSG_STORE_BUY] =  Protocols.unpack_store_buy,
[CMSG_NPC_SELL] =  Protocols.unpack_npc_sell,
[CMSG_NPC_REPURCHASE] =  Protocols.unpack_npc_repurchase,
[CMSG_AVATAR_FASHION_ENABLE] =  Protocols.unpack_avatar_fashion_enable,
[CMSG_QUESTHELP_TALK_OPTION] =  Protocols.unpack_questhelp_talk_option,
[CMSG_TAXI_HELLO] =  Protocols.unpack_taxi_hello,
[SMSG_TAXI_STATIONS_LIST] =  Protocols.unpack_taxi_stations_list,
[CMSG_TAXI_SELECT_STATION] =  Protocols.unpack_taxi_select_station,
[CMSG_GOSSIP_SELECT_OPTION] =  Protocols.unpack_gossip_select_option,
[CMSG_GOSSIP_HELLO] =  Protocols.unpack_gossip_hello,
[SMSG_GOSSIP_MESSAGE] =  Protocols.unpack_gossip_message,
[CMSG_QUESTGIVER_STATUS_QUERY] =  Protocols.unpack_questgiver_status_query,
[SMSG_QUESTGIVER_STATUS] =  Protocols.unpack_questgiver_status,
[MSG_QUERY_QUEST_STATUS] =  Protocols.unpack_query_quest_status,
[CMSG_QUESTHELP_GET_CANACCEPT_LIST] =  Protocols.unpack_questhelp_get_canaccept_list,
[SMSG_QUESTHELP_CANACCEPT_LIST] =  Protocols.unpack_questhelp_canaccept_list,
[SMSG_QUESTUPDATE_FAILD] =  Protocols.unpack_questupdate_faild,
[SMSG_QUESTUPDATE_COMPLETE] =  Protocols.unpack_questupdate_complete,
[CMSG_QUESTLOG_REMOVE_QUEST] =  Protocols.unpack_questlog_remove_quest,
[CMSG_QUESTGIVER_COMPLETE_QUEST] =  Protocols.unpack_questgiver_complete_quest,
[SMSG_QUESTHELP_NEXT] =  Protocols.unpack_questhelp_next,
[CMSG_QUESTHELP_COMPLETE] =  Protocols.unpack_questhelp_complete,
[SMSG_QUESTUPDATE_ACCEPT] =  Protocols.unpack_questupdate_accept,
[CMSG_QUESTHELP_UPDATE_STATUS] =  Protocols.unpack_questhelp_update_status,
[SMSG_QUESTGETTER_COMPLETE] =  Protocols.unpack_questgetter_complete,
[CMSG_QUESTGIVER_ACCEPT_QUEST] =  Protocols.unpack_questgiver_accept_quest,
[CMSG_QUESTUPDATE_USE_ITEM] =  Protocols.unpack_questupdate_use_item,
[CMSG_QUESTHELP_QUERY_BOOK] =  Protocols.unpack_questhelp_query_book,
[SMSG_QUESTHELP_BOOK_QUEST] =  Protocols.unpack_questhelp_book_quest,
[SMSG_USE_GAMEOBJECT_ACTION] =  Protocols.unpack_use_gameobject_action,
[CMSG_SET_ATTACK_MODE] =  Protocols.unpack_set_attack_mode,
[MSG_SELECT_TARGET] =  Protocols.unpack_select_target,
[SMSG_COMBAT_STATE_UPDATE] =  Protocols.unpack_combat_state_update,
[SMSG_EXP_UPDATE] =  Protocols.unpack_exp_update,
[MSG_SPELL_START] =  Protocols.unpack_spell_start,
[MSG_SPELL_STOP] =  Protocols.unpack_spell_stop,
[MSG_JUMP] =  Protocols.unpack_jump,
[CMSG_RESURRECTION] =  Protocols.unpack_resurrection,
[MSG_TRADE_REQUEST] =  Protocols.unpack_trade_request,
[MSG_TRADE_REPLY] =  Protocols.unpack_trade_reply,
[SMSG_TRADE_START] =  Protocols.unpack_trade_start,
[MSG_TRADE_DECIDE_ITEMS] =  Protocols.unpack_trade_decide_items,
[SMSG_TRADE_FINISH] =  Protocols.unpack_trade_finish,
[MSG_TRADE_CANCEL] =  Protocols.unpack_trade_cancel,
[MSG_TRADE_READY] =  Protocols.unpack_trade_ready,
[SMSG_CHAT_UNIT_TALK] =  Protocols.unpack_chat_unit_talk,
[CMSG_CHAT_NEAR] =  Protocols.unpack_chat_near,
[CMSG_CHAT_WHISPER] =  Protocols.unpack_chat_whisper,
[MSG_CHAT_FACTION] =  Protocols.unpack_chat_faction,
[MSG_CHAT_WORLD] =  Protocols.unpack_chat_world,
[MSG_CHAT_HORN] =  Protocols.unpack_chat_horn,
[MSG_CHAT_NOTICE] =  Protocols.unpack_chat_notice,
[CMSG_QUERY_PLAYER_INFO] =  Protocols.unpack_query_player_info,
[SMSG_QUERY_RESULT_UPDATE_OBJECT] =  Protocols.unpack_query_result_update_object,
[CMSG_RECEIVE_GIFT_PACKS] =  Protocols.unpack_receive_gift_packs,
[SMSG_MAP_UPDATE_OBJECT] =  Protocols.unpack_map_update_object,
[SMSG_FIGHTING_INFO_UPDATE_OBJECT] =  Protocols.unpack_fighting_info_update_object,
[SMSG_FIGHTING_INFO_UPDATE_OBJECT_2] =  Protocols.unpack_fighting_info_update_object_2,
[CMSG_INSTANCE_ENTER] =  Protocols.unpack_instance_enter,
[CMSG_INSTANCE_NEXT_STATE] =  Protocols.unpack_instance_next_state,
[CMSG_INSTANCE_EXIT] =  Protocols.unpack_instance_exit,
[CMSG_LIMIT_ACTIVITY_RECEIVE] =  Protocols.unpack_limit_activity_receive,
[SMSG_KILL_MAN] =  Protocols.unpack_kill_man,
[SMSG_PLAYER_UPGRADE] =  Protocols.unpack_player_upgrade,
[CMSG_WAREHOUSE_SAVE_MONEY] =  Protocols.unpack_warehouse_save_money,
[CMSG_WAREHOUSE_TAKE_MONEY] =  Protocols.unpack_warehouse_take_money,
[CMSG_USE_GOLD_OPT] =  Protocols.unpack_use_gold_opt,
[CMSG_USE_SILVER_OPT] =  Protocols.unpack_use_silver_opt,
[SMSG_GM_RIGHTFLOAT] =  Protocols.unpack_gm_rightfloat,
[SMSG_DEL_GM_RIGHTFLOAT] =  Protocols.unpack_del_gm_rightfloat,
[MSG_SYNC_MSTIME_APP] =  Protocols.unpack_sync_mstime_app,
[CMSG_OPEN_WINDOW] =  Protocols.unpack_open_window,
[CMSG_PLAYER_GAG] =  Protocols.unpack_player_gag,
[CMSG_PLAYER_KICKING] =  Protocols.unpack_player_kicking,
[SMSG_MERGE_SERVER_MSG] =  Protocols.unpack_merge_server_msg,
[CMSG_RANK_LIST_QUERY] =  Protocols.unpack_rank_list_query,
[SMSG_RANK_LIST_QUERY_RESULT] =  Protocols.unpack_rank_list_query_result,
[CMSG_CLIENT_UPDATE_SCENED] =  Protocols.unpack_client_update_scened,
[SMSG_NUM_LUA] =  Protocols.unpack_num_lua,
[CMSG_LOOT_SELECT] =  Protocols.unpack_loot_select,
[CMSG_GOBACK_TO_GAME_SERVER] =  Protocols.unpack_goback_to_game_server,
[CMSG_WORLD_WAR_CS_PLAYER_INFO] =  Protocols.unpack_world_war_CS_player_info,
[SMSG_JOIN_OR_LEAVE_SERVER] =  Protocols.unpack_join_or_leave_server,
[MSG_WORLD_WAR_SC_PLAYER_INFO] =  Protocols.unpack_world_war_SC_player_info,
[MSG_CLIENTSUBSCRIPTION] =  Protocols.unpack_clientSubscription,
[SMSG_LUA_SCRIPT] =  Protocols.unpack_lua_script,
[CMSG_CHAR_UPDATE_INFO] =  Protocols.unpack_char_update_info,
[SMSG_NOTICE_WATCHER_MAP_INFO] =  Protocols.unpack_notice_watcher_map_info,
[CMSG_MODIFY_WATCH] =  Protocols.unpack_modify_watch,
[CMSG_KUAFU_CHUANSONG] =  Protocols.unpack_kuafu_chuansong,
[CMSG_SHOW_SUIT] =  Protocols.unpack_show_suit,
[CMSG_SHOW_POSITION] =  Protocols.unpack_show_position,
[CMSG_GOLD_RESPAWN] =  Protocols.unpack_gold_respawn,
[SMSG_FIELD_DEATH_COOLDOWN] =  Protocols.unpack_field_death_cooldown,
[CMSG_MALL_BUY] =  Protocols.unpack_mall_buy,
[CMSG_STRENGTH] =  Protocols.unpack_strength,
[SMSG_STRENGTH_SUCCESS] =  Protocols.unpack_strength_success,
[CMSG_FORCEINTO] =  Protocols.unpack_forceInto,
[CMSG_CREATE_FACTION] =  Protocols.unpack_create_faction,
[CMSG_FACTION_UPGRADE] =  Protocols.unpack_faction_upgrade,
[CMSG_FACTION_JOIN] =  Protocols.unpack_faction_join,
[CMSG_RAISE_BASE_SPELL] =  Protocols.unpack_raise_base_spell,
[CMSG_UPGRADE_ANGER_SPELL] =  Protocols.unpack_upgrade_anger_spell,
[CMSG_RAISE_MOUNT] =  Protocols.unpack_raise_mount,
[CMSG_UPGRADE_MOUNT] =  Protocols.unpack_upgrade_mount,
[CMSG_UPGRADE_MOUNT_ONE_STEP] =  Protocols.unpack_upgrade_mount_one_step,
[CMSG_ILLUSION_MOUNT_ACTIVE] =  Protocols.unpack_illusion_mount_active,
[CMSG_ILLUSION_MOUNT] =  Protocols.unpack_illusion_mount,
[CMSG_RIDE_MOUNT] =  Protocols.unpack_ride_mount,
[SMSG_GRID_UNIT_JUMP] =  Protocols.unpack_grid_unit_jump,
[CMSG_GEM] =  Protocols.unpack_gem,
[CMSG_CHANGE_BATTLE_MODE] =  Protocols.unpack_change_battle_mode,
[SMSG_PEACE_MODE_CD] =  Protocols.unpack_peace_mode_cd,
[CMSG_DIVINE_ACTIVE] =  Protocols.unpack_divine_active,
[CMSG_DIVINE_UPLEV] =  Protocols.unpack_divine_uplev,
[CMSG_DIVINE_SWITCH] =  Protocols.unpack_divine_switch,
[CMSG_JUMP_START] =  Protocols.unpack_jump_start,
[CMSG_ENTER_VIP_INSTANCE] =  Protocols.unpack_enter_vip_instance,
[CMSG_SWEEP_VIP_INSTANCE] =  Protocols.unpack_sweep_vip_instance,
[CMSG_HANG_UP] =  Protocols.unpack_hang_up,
[CMSG_HANG_UP_SETTING] =  Protocols.unpack_hang_up_setting,
[CMSG_ENTER_TRIAL_INSTANCE] =  Protocols.unpack_enter_trial_instance,
[CMSG_SWEEP_TRIAL_INSTANCE] =  Protocols.unpack_sweep_trial_instance,
[CMSG_RESET_TRIAL_INSTANCE] =  Protocols.unpack_reset_trial_instance,
[SMSG_SWEEP_INSTANCE_REWARD] =  Protocols.unpack_sweep_instance_reward,
[CMSG_REENTER_INSTANCE] =  Protocols.unpack_reenter_instance,
[SMSG_MERRY_GO_ROUND] =  Protocols.unpack_merry_go_round,
[CMSG_SOCIAL_ADD_FRIEND] =  Protocols.unpack_social_add_friend,
[CMSG_SOCIAL_SUREADD_FRIEND] =  Protocols.unpack_social_sureadd_friend,
[CMSG_SOCIAL_GIFT_FRIEND] =  Protocols.unpack_social_gift_friend,
[CMSG_SOCIAL_RECOMMEND_FRIEND] =  Protocols.unpack_social_recommend_friend,
[SMSG_SOCIAL_GET_RECOMMEND_FRIEND] =  Protocols.unpack_social_get_recommend_friend,
[CMSG_SOCIAL_REVENGE_ENEMY] =  Protocols.unpack_social_revenge_enemy,
[CMSG_SOCIAL_DEL_FRIEND] =  Protocols.unpack_social_del_friend,
[CMSG_TELEPORT_MAIN_CITY] =  Protocols.unpack_teleport_main_city,
[CMSG_CHAT_BY_CHANNEL] =  Protocols.unpack_chat_by_channel,
[SMSG_SEND_CHAT] =  Protocols.unpack_send_chat,
[CMSG_SOCIAL_CLEAR_APPLY] =  Protocols.unpack_social_clear_apply,
[CMSG_MSG_DECLINE] =  Protocols.unpack_msg_decline,
[SMSG_FACTION_GET_LIST_RESULT] =  Protocols.unpack_faction_get_list_result,
[CMSG_FACTION_GETLIST] =  Protocols.unpack_faction_getlist,
[CMSG_FACTION_MANAGER] =  Protocols.unpack_faction_manager,
[CMSG_FACTION_MEMBER_OPERATE] =  Protocols.unpack_faction_member_operate,
[CMSG_FACTION_FAST_JOIN] =  Protocols.unpack_faction_fast_join,
[CMSG_SOCIAL_ADD_FRIEND_BYNAME] =  Protocols.unpack_social_add_friend_byname,
[CMSG_READ_MAIL] =  Protocols.unpack_read_mail,
[CMSG_PICK_MAIL] =  Protocols.unpack_pick_mail,
[CMSG_REMOVE_MAIL] =  Protocols.unpack_remove_mail,
[CMSG_PICK_MAIL_ONE_STEP] =  Protocols.unpack_pick_mail_one_step,
[CMSG_REMOVE_MAIL_ONE_STEP] =  Protocols.unpack_remove_mail_one_step,
[CMSG_BLOCK_CHAT] =  Protocols.unpack_block_chat,
[CMSG_CANCEL_BLOCK_CHAT] =  Protocols.unpack_cancel_block_chat,
[CMSG_USE_BROADCAST_GAMEOBJECT] =  Protocols.unpack_use_broadcast_gameobject,
[CMSG_WORLD_BOSS_ENROLL] =  Protocols.unpack_world_boss_enroll,
[CMSG_WORLD_BOSS_FIGHT] =  Protocols.unpack_world_boss_fight,
[CMSG_CHANGE_LINE] =  Protocols.unpack_change_line,
[CMSG_ROLL_WORLD_BOSS_TREASURE] =  Protocols.unpack_roll_world_boss_treasure,
[SMSG_ROLL_RESULT] =  Protocols.unpack_roll_result,
[SMSG_BOSS_RANK] =  Protocols.unpack_boss_rank,
[CMSG_RANK_ADD_LIKE] =  Protocols.unpack_rank_add_like,
[SMSG_RANK_ADD_LIKE_RESULT] =  Protocols.unpack_rank_add_like_result,
[CMSG_RES_INSTANCE_ENTER] =  Protocols.unpack_res_instance_enter,
[CMSG_RES_INSTANCE_SWEEP] =  Protocols.unpack_res_instance_sweep,
[CMSG_SHOW_MAP_LINE] =  Protocols.unpack_show_map_line,
[SMSG_SEND_MAP_LINE] =  Protocols.unpack_send_map_line,
[SMSG_ITEM_NOTICE] =  Protocols.unpack_item_notice,
[CMSG_TELEPORT_MAP] =  Protocols.unpack_teleport_map,
[CMSG_TELEPORT_FIELD_BOSS] =  Protocols.unpack_teleport_field_boss,
[CMSG_GET_ACTIVITY_REWARD] =  Protocols.unpack_get_activity_reward,
[CMSG_GET_ACHIEVE_REWARD] =  Protocols.unpack_get_achieve_reward,
[CMSG_GET_ACHIEVE_ALL_REWARD] =  Protocols.unpack_get_achieve_all_reward,
[CMSG_SET_TITLE] =  Protocols.unpack_set_title,
[CMSG_INIT_TITLE] =  Protocols.unpack_init_title,
[CMSG_WELFARE_SHOUCHONG_REWARD] =  Protocols.unpack_welfare_shouchong_reward,
[CMSG_WELFARE_CHECKIN] =  Protocols.unpack_welfare_checkin,
[CMSG_WELFARE_CHECKIN_ALL] =  Protocols.unpack_welfare_checkin_all,
[CMSG_WELFARE_CHECKIN_GETBACK] =  Protocols.unpack_welfare_checkin_getback,
[CMSG_WELFARE_LEVEL] =  Protocols.unpack_welfare_level,
[CMSG_WELFARE_ACTIVE_GETBACK] =  Protocols.unpack_welfare_active_getback,
[CMSG_PICK_QUEST_REWARD] =  Protocols.unpack_pick_quest_reward,
[CMSG_TALK_WITH_NPC] =  Protocols.unpack_talk_with_npc,
[CMSG_USE_VIRTUAL_ITEM] =  Protocols.unpack_use_virtual_item,
[CMSG_PICK_QUEST_CHAPTER_REWARD] =  Protocols.unpack_pick_quest_chapter_reward,
[CMSG_KUAFU_3V3_MATCH] =  Protocols.unpack_kuafu_3v3_match,
[SMSG_KUAFU_MATCH_START] =  Protocols.unpack_kuafu_match_start,
[CMSG_KUAFU_3V3_BUYTIMES] =  Protocols.unpack_kuafu_3v3_buytimes,
[CMSG_KUAFU_3V3_DAYREWARD] =  Protocols.unpack_kuafu_3v3_dayreward,
[CMSG_KUAFU_3V3_GETRANLIST] =  Protocols.unpack_kuafu_3v3_getranlist,
[SMSG_KUAFU_3V3_RANLIST] =  Protocols.unpack_kuafu_3v3_ranlist,
[CMSG_WELFARE_GETALLLIST_GETBACK] =  Protocols.unpack_welfare_getalllist_getback,
[SMSG_WELFARE_REWARDLIST_GETBACK] =  Protocols.unpack_welfare_rewardlist_getback,
[CMSG_WELFARE_GETALL_GETBACK] =  Protocols.unpack_welfare_getall_getback,
[CMSG_KUAFU_3V3_GETMYRANK] =  Protocols.unpack_kuafu_3v3_getmyrank,
[SMSG_KUAFU_3V3_MYRANK] =  Protocols.unpack_kuafu_3v3_myrank,
[SMSG_KUAFU_3V3_KILL_DETAIL] =  Protocols.unpack_kuafu_3v3_kill_detail,
[SMSG_KUAFU_3V3_WAIT_INFO] =  Protocols.unpack_kuafu_3v3_wait_info,
[MSG_KUAFU_3V3_CANCEL_MATCH] =  Protocols.unpack_kuafu_3v3_cancel_match,
[CMSG_KUAFU_3V3_MATCH_OPER] =  Protocols.unpack_kuafu_3v3_match_oper,
[SMSG_KUAFU_3V3_DECLINE_MATCH] =  Protocols.unpack_kuafu_3v3_decline_match,
[CMSG_KUAFU_XIANFU_MATCH] =  Protocols.unpack_kuafu_xianfu_match,
[SMSG_KUAFU_MATCH_WAIT] =  Protocols.unpack_kuafu_match_wait,
[SMSG_KUAFU_XIANFU_MINIMAP_INFO] =  Protocols.unpack_kuafu_xianfu_minimap_info,
[CMSG_BUY_XIANFU_ITEM] =  Protocols.unpack_buy_xianfu_item,
[CMSG_XIANFU_RANDOM_RESPAWN] =  Protocols.unpack_xianfu_random_respawn,
[CMSG_DOUJIANTAI_FIGHT] =  Protocols.unpack_doujiantai_fight,
[CMSG_DOUJIANTAI_BUYTIME] =  Protocols.unpack_doujiantai_buytime,
[CMSG_DOUJIANTAI_CLEARCD] =  Protocols.unpack_doujiantai_clearcd,
[CMSG_DOUJIANTAI_FIRST_REWARD] =  Protocols.unpack_doujiantai_first_reward,
[MSG_DOUJIANTAI_GET_ENEMYS_INFO] =  Protocols.unpack_doujiantai_get_enemys_info,
[CMSG_DOUJIANTAI_GET_RANK] =  Protocols.unpack_doujiantai_get_rank,
[CMSG_DOUJIANTAI_REFRESH_ENEMYS] =  Protocols.unpack_doujiantai_refresh_enemys,
[MSG_DOUJIANTAI_TOP3] =  Protocols.unpack_doujiantai_top3,
[MSG_USE_JUMP_POINT] =  Protocols.unpack_use_jump_point,
[CMSG_BAG_ITEM_SELL] =  Protocols.unpack_bag_item_sell,
[CMSG_BAG_ITEM_SORT] =  Protocols.unpack_bag_item_sort,
[CMSG_SUBMIT_QUEST_DAILY2] =  Protocols.unpack_submit_quest_daily2,
[SMSG_ATTRIBUTE_CHANGED] =  Protocols.unpack_attribute_changed,
[SMSG_BAG_FIND_EQUIP_BETTER] =  Protocols.unpack_bag_find_equip_better,
[SMSG_MODULE_ACTIVE] =  Protocols.unpack_module_active,
[CMSG_PICK_DAILY2_QUEST_REWARD] =  Protocols.unpack_pick_daily2_quest_reward,
[CMSG_FINISH_NOW_GUIDE] =  Protocols.unpack_finish_now_guide,
[CMSG_GET_CULTIVATION_INFO] =  Protocols.unpack_get_cultivation_info,
[SMSG_UPDATE_CULTIVATION_INFO] =  Protocols.unpack_update_cultivation_info,
[CMSG_GET_CULTIVATION_RIVALS_INFO] =  Protocols.unpack_get_cultivation_rivals_info,
[SMSG_UPDATE_CULTIVATION_RIVALS_INFO_LIST] =  Protocols.unpack_update_cultivation_rivals_info_list,
[CMSG_GET_CULTIVATION_REWARD] =  Protocols.unpack_get_cultivation_reward,
[CMSG_REFRESH_CULTIVATION_RIVALS] =  Protocols.unpack_refresh_cultivation_rivals,
[CMSG_PLUNDER_CULTIVATION_RIVAL] =  Protocols.unpack_plunder_cultivation_rival,
[CMSG_REVENGE_CULTIVATION_RIVAL] =  Protocols.unpack_revenge_cultivation_rival,
[CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT] =  Protocols.unpack_buy_cultivation_left_plunder_count,
[SMSG_SHOW_CULTIVATION_RESULT_LIST] =  Protocols.unpack_show_cultivation_result_list,
[CMSG_GET_LOGIN_ACTIVITY_REWARD] =  Protocols.unpack_get_login_activity_reward,
[SMSG_CAST_SPELL_START] =  Protocols.unpack_cast_spell_start,
[CMSG_FINISH_OPTIONAL_GUIDE_STEP] =  Protocols.unpack_finish_optional_guide_step,
[CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED] =  Protocols.unpack_execute_quest_cmd_after_accepted,
[SMSG_SHOW_UNIT_ATTRIBUTE] =  Protocols.unpack_show_unit_attribute,
[CMSG_BACK_TO_FAMITY] =  Protocols.unpack_back_to_famity,
[SMSG_FACTION_BOSS_SEND_RESULT] =  Protocols.unpack_faction_boss_send_result,
[CMSG_CHALLANGE_BOSS] =  Protocols.unpack_challange_boss,
[CMSG_PICK_OFFLINE_REWARD] =  Protocols.unpack_pick_offline_reward,
[SMSG_OFFLINE_REWARD_RESULT] =  Protocols.unpack_offline_reward_result,
[CMSG_SMELTING_EQUIP] =  Protocols.unpack_smelting_equip,
[CMSG_STOREHOUSE_HAND_IN] =  Protocols.unpack_storehouse_hand_in,
[CMSG_STOREHOUSE_EXCHANGE] =  Protocols.unpack_storehouse_exchange,
[CMSG_STOREHOUSE_DESTROY] =  Protocols.unpack_storehouse_destroy,
[CMSG_SEND_FACTION_GIFT] =  Protocols.unpack_send_faction_gift,
[CMSG_GET_FACTION_GIFT_EXREWARD] =  Protocols.unpack_get_faction_gift_exreward,
[CMSG_GET_ALL_FACTION_GIFT_EXREWARD] =  Protocols.unpack_get_all_faction_gift_exreward,
[SMSG_SHOW_FACTION_GIFT_PAGE] =  Protocols.unpack_show_faction_gift_page,
[SMSG_SHOW_FACTION_GIFT_INFO] =  Protocols.unpack_show_faction_gift_info,
[SMSG_SHOW_FACTION_GIFT_UNTHANK_PAGE] =  Protocols.unpack_show_faction_gift_unthank_page,
[SMSG_SHOW_FACTION_GIFT_HISTORY_PAGE] =  Protocols.unpack_show_faction_gift_history_page,
[CMSG_GET_FACTION_GIFT_RANK_PAGE] =  Protocols.unpack_get_faction_gift_rank_page,
[SMSG_SHOW_FACTION_GIFT_RANK_RESULT_LIST] =  Protocols.unpack_show_faction_gift_rank_result_list,
[SMSG_SHOW_FACTION_GIFT_RANK_CHANGE] =  Protocols.unpack_show_faction_gift_rank_change,
[SMSG_SHOW_FACTION_GIFT_RANK_INFO] =  Protocols.unpack_show_faction_gift_rank_info,
[CMSG_DIVINE_FORGE] =  Protocols.unpack_divine_forge,
[CMSG_DIVINE_ADVANCE] =  Protocols.unpack_divine_advance,
[CMSG_DIVINE_SPIRIT] =  Protocols.unpack_divine_spirit,
[CMSG_QUERY_MASS_BOSS_INFO] =  Protocols.unpack_query_mass_boss_info,
[SMSG_MASS_BOSS_INFO_RET] =  Protocols.unpack_mass_boss_info_ret,
[CMSG_QUERY_MASS_BOSS_RANK] =  Protocols.unpack_query_mass_boss_rank,
[SMSG_MASS_BOSS_RANK_RESULT] =  Protocols.unpack_mass_boss_rank_result,
[CMSG_TRY_MASS_BOSS] =  Protocols.unpack_try_mass_boss,
[CMSG_BUY_MASS_BOSS_TIMES] =  Protocols.unpack_buy_mass_boss_times,
[CMSG_GROUP_INSTANCE_MATCH] =  Protocols.unpack_group_instance_match,
[CMSG_BUY_GROUP_INSTANCE_TIMES] =  Protocols.unpack_buy_group_instance_times,
[CMSG_TALISMAN_ACTIVE] =  Protocols.unpack_talisman_active,
[CMSG_TALISMAN_LVUP] =  Protocols.unpack_talisman_lvup,
[CMSG_WINGS_ACTIVE] =  Protocols.unpack_wings_active,
[CMSG_WINGS_BLESS] =  Protocols.unpack_wings_bless,
[CMSG_WINGS_RANKUP] =  Protocols.unpack_wings_rankup,
[CMSG_WINGS_STRENGTH] =  Protocols.unpack_wings_strength,
[CMSG_MERIDIAN_PRACTISE] =  Protocols.unpack_meridian_practise,
[CMSG_ADD_MERIDIAN_EXP] =  Protocols.unpack_add_meridian_exp,
[CMSG_RAISE_MOUNT_LEVEL_BASE] =  Protocols.unpack_raise_mount_level_base,
[CMSG_ACTIVE_MOUNT] =  Protocols.unpack_active_mount,
[SMSG_SHOW_FACTION_BOSSDEFENSE_DAMAGE_LIST] =  Protocols.unpack_show_faction_bossdefense_damage_list,
[SMSG_SHOW_FACTION_TOWER_SWEEP_LIST] =  Protocols.unpack_show_faction_tower_sweep_list,
[SMSG_SEND_INSTANCE_RESULT] =  Protocols.unpack_send_instance_result,
[CMSG_MATCH_SINGLE_PVP] =  Protocols.unpack_match_single_pvp,
[CMSG_BUY_MATCH_SINGLE_PVP_TIMES] =  Protocols.unpack_buy_match_single_pvp_times,
[CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD] =  Protocols.unpack_pick_match_single_pvp_extra_reward,
[CMSG_EQUIPDEVELOP_OPERATE] =  Protocols.unpack_equipdevelop_operate,
[CMSG_ACTIVE_APPEARANCE] =  Protocols.unpack_active_appearance,
[CMSG_EQUIP_APPEARANCE] =  Protocols.unpack_equip_appearance,
[CMSG_CANCEL_EQUIP_APPEARANCE] =  Protocols.unpack_cancel_equip_appearance,
[CMSG_RENAME] =  Protocols.unpack_rename,
[CMSG_UNLOCK_TITLE] =  Protocols.unpack_unlock_title,
[CMSG_SOCIAL_BUY_REVENGE_TIMES] =  Protocols.unpack_social_buy_revenge_times,
[CMSG_ENTER_RISK_INSTANCE] =  Protocols.unpack_enter_risk_instance,
[CMSG_SOCIAL_REMOVE_ENEMY] =  Protocols.unpack_social_remove_enemy,
[CMSG_GET_PLAYER_OVERVIEW] =  Protocols.unpack_get_player_overview,
[SMSG_SHOW_PLAYER_OVERVIEW] =  Protocols.unpack_show_player_overview,
[CMSG_SEND_FACTION_INVITE] =  Protocols.unpack_send_faction_invite,
[SMSG_SHOW_FACTION_INVITE] =  Protocols.unpack_show_faction_invite,
[CMSG_BUY_VIPGIFT] =  Protocols.unpack_buy_vipgift,
[CMSG_ACTIVITY_OPT_BUY_DAILYGIFT] =  Protocols.unpack_activity_opt_buy_dailygift,
[CMSG_DRAW_LOTTERY] =  Protocols.unpack_draw_lottery,
[CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD] =  Protocols.unpack_activity_opt_get_rank_process_reward,
[CMSG_ACTIVITY_OPT_GET_RANK_LIST] =  Protocols.unpack_activity_opt_get_rank_list,
[SMSG_ACTIVITY_OPT_SHOW_RANK_LIST] =  Protocols.unpack_activity_opt_show_rank_list,
[CMSG_ACTIVITY_OPT_BUY_LIMITGIFT] =  Protocols.unpack_activity_opt_buy_limitgift,
[CMSG_WELFARE_GET_RECHARGE_REWARD] =  Protocols.unpack_welfare_get_recharge_reward,
[CMSG_WELFARE_GET_CONSUME_REWARD] =  Protocols.unpack_welfare_get_consume_reward,
[CMSG_WELFARE_GET_SEVENDAY_REWARD] =  Protocols.unpack_welfare_get_sevenday_reward,
[SMSG_SEND_SERVER_OPEN_TIME] =  Protocols.unpack_send_server_open_time,
[CMSG_RISK_GET_RANK] =  Protocols.unpack_risk_get_rank,
[SMSG_RISK_GET_RANK_RESULT] =  Protocols.unpack_risk_get_rank_result,
[CMSG_SET_ORIENT] =  Protocols.unpack_set_orient,

}

function Protocols.unpack_packet(opcode,pkt)
	return unpack_handler[opcode](pkt)
end

return Protocols