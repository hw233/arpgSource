--------------------------------------------------------------------------------------------------------------------------
--------------------------------------------以下代码为自动生成，请勿手工改动----------------------------------------------
--------------------------------------------------------------------------------------------------------------------------
if(OPRATE_TYPE_LOGIN == nil)then
	require "Config.OprateResult"
end
function OperationFailedToString(type, reason, data)
	if(type == OPRATE_TYPE_LOGIN)then
		if (reason ==OPRATE_RESULT_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_SUCCESS成功 - " .. data 
		end
		if (reason ==OPRATE_RESULT_NAME_REPEAT)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_NAME_REPEAT名称重复 - " .. data 
		end
		if (reason ==OPRATE_RESULT_NAME_ILLEGAL)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_NAME_ILLEGAL名称非法 - " .. data 
		end
		if (reason ==OPRATE_RESULT_CHAR_CAP)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_CHAR_CAP角色数量达到上限 - " .. data 
		end
		if (reason ==OPRATE_RESULT_SCENED_CLOSE)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_SCENED_CLOSE场景服未开启 - " .. data 
		end
		if (reason ==OPRATE_RESULT_SCENED_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_SCENED_ERROR场景服重置，其实就是崩了一个场景服换一个新的 - " .. data 
		end
		if (reason ==OPRATE_RESULT_LOGINED_IN)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_LOGINED_IN角色已登录 - " .. data 
		end
		if (reason ==OPRATE_RESULT_OTHER_LOGINED)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_OTHER_LOGINED角色其他地方登录 - " .. data 
		end
		if (reason ==OPRATE_RESULT_GAME_VERSION_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_GAME_VERSION_ERROR游戏版本不对 - " .. data 
		end
		if (reason ==OPRATE_RESULT_MAP_VERSION_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_MAP_VERSION_ERROR地图素材版本不对 - " .. data 
		end
		if (reason ==OPRATE_RESULT_DATA_VERSION_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_DATA_VERSION_ERRORdata素材版本不对 - " .. data 
		end
		if (reason ==OPRATE_RESULT_VERSION_FORMAT_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_VERSION_FORMAT_ERROR客户端发来的版本信息格式不对 - " .. data 
		end
		if (reason ==OPRATE_RESULT_APPD_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_APPD_ERROR应用服异常重启 - " .. data 
		end
		if (reason ==OPRATE_RESULT_LOCK_ACCOUNT)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_LOCK_ACCOUNT帐号被封 - " .. data 
		end
		if (reason ==OPRATE_RESULT_LOCK_IP)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_LOCK_IPIP被封 - " .. data 
		end
		if (reason ==OPRATE_RESULT_APPD_CLOSE)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_APPD_CLOSE应用服未开启 - " .. data 
		end
		if (reason ==OPRATE_RESULT_NAME_TOO_LONG)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_NAME_TOO_LONG名字太长 - " .. data 
		end
		if (reason ==OPRATE_RESULT_NAME_HAS_PINGBI)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_NAME_HAS_PINGBI有屏蔽词 - " .. data 
		end
		if (reason ==OPRATE_RESULT_LOGOUT_UNFINISHED)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_LOGOUT_UNFINISHED该角色上一次的登出未完成，请等待 - " .. data 
		end
		if (reason ==OPRATE_RESULT_PID_OR_SID_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_PID_OR_SID_ERROR服务器id或者平台id错误 - " .. data 
		end
		if (reason ==OPRATE_RESULT_DB_RESULT_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_DB_RESULT_ERROR数据库查询出问题了 - " .. data 
		end
		if (reason ==OPRATE_RESULT_MERGE_SERVER)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_MERGE_SERVER合服了 - " .. data 
		end
		if (reason ==OPRATE_RESULT_PLAYER_ZIBAO)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_PLAYER_ZIBAO自爆 - " .. data 
		end
		if (reason ==OPRATE_RESULT_GENDER_ILLEGAL)then
			return "ShowOperationFailed OPRATE_TYPE_LOGIN登录 - OPRATE_RESULT_GENDER_ILLEGAL角色种类异常 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_LOGIN " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_KICKING)then
		if (reason ==KICKING_TYPE_GM_LOCK_IP)then
			return "ShowOperationFailed OPRATE_TYPE_KICKING踢人 - KICKING_TYPE_GM_LOCK_IP封IP - " .. data 
		end
		if (reason ==KICKING_TYPE_GM_LOCK_ACCOUNT)then
			return "ShowOperationFailed OPRATE_TYPE_KICKING踢人 - KICKING_TYPE_GM_LOCK_ACCOUNT封号 - " .. data 
		end
		if (reason ==KICKING_TYPE_GM_KICKING)then
			return "ShowOperationFailed OPRATE_TYPE_KICKING踢人 - KICKING_TYPE_GM_KICKING从GM表中读取的踢人 - " .. data 
		end
		if (reason ==KICKING_TYPE_KUANGCHANG_RELIVE)then
			return "ShowOperationFailed OPRATE_TYPE_KICKING踢人 - KICKING_TYPE_KUANGCHANG_RELIVE荒芜矿场非法复活 - " .. data 
		end
		if (reason ==KICKING_TYPE_FUCK_PINGBI)then
			return "ShowOperationFailed OPRATE_TYPE_KICKING踢人 - KICKING_TYPE_FUCK_PINGBI聊天点带屏蔽词 - " .. data 
		end
		if (reason ==KICKING_TYPE_QUEST_GET_ITEM)then
			return "ShowOperationFailed OPRATE_TYPE_KICKING踢人 - KICKING_TYPE_QUEST_GET_ITEM刷物品 - " .. data 
		end
		if (reason ==KICKING_TYPE_NOT_MONEY)then
			return "ShowOperationFailed OPRATE_TYPE_KICKING踢人 - KICKING_TYPE_NOT_MONEY钱负数还继续消费 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_KICKING " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_JUMP)then
		if (reason ==JUMP_RESULT_NOMAL)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_NOMAL无，默认结果 - " .. data 
		end
		if (reason ==JUMP_RESULT_DEAD)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_DEAD死了你还跳，果断是诈尸 - " .. data 
		end
		if (reason ==JUMP_RESULT_ENERGY)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_ENERGY体力不足 - " .. data 
		end
		if (reason ==JUMP_RESULT_MAP_CANT_JUMP)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_MAP_CANT_JUMP地图无法跳跃 - " .. data 
		end
		if (reason ==JUMP_RESULT_CANT_JUMP)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_CANT_JUMP玩家被限制不能跳跃 - " .. data 
		end
		if (reason ==JUMP_RESULT_MAX_DISTANCE)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_MAX_DISTANCE超过最大距离 - " .. data 
		end
		if (reason ==JUMP_RESULT_WRONG_COORD)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_WRONG_COORD无效坐标，障碍点之流 - " .. data 
		end
		if (reason ==JUMP_RESULT_SPELL_CD)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_SPELL_CD跳跃技能cd中 - " .. data 
		end
		if (reason ==JUMP_RESULT_NOT_ACTIVE_SPELL)then
			return "ShowOperationFailed OPRATE_TYPE_JUMP跳 - JUMP_RESULT_NOT_ACTIVE_SPELL2段跳没有激活 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_JUMP " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_ATTK_LOSE)then
		if (reason ==ATTACK_LOST_CANTATTACK)then
			return "ShowOperationFailed OPRATE_TYPE_ATTK_LOSE攻击失败 - ATTACK_LOST_CANTATTACK被限制不能攻击 - " .. data 
		end
		if (reason ==ATTACK_LOST_TARGET_DEAD)then
			return "ShowOperationFailed OPRATE_TYPE_ATTK_LOSE攻击失败 - ATTACK_LOST_TARGET_DEAD目标已经死亡 - " .. data 
		end
		if (reason ==ATTACK_LOST_NEED_TARGET)then
			return "ShowOperationFailed OPRATE_TYPE_ATTK_LOSE攻击失败 - ATTACK_LOST_NEED_TARGET攻击需要一个目标 - " .. data 
		end
		if (reason ==ATTACK_LOST_OUT_OF_RANGE)then
			return "ShowOperationFailed OPRATE_TYPE_ATTK_LOSE攻击失败 - ATTACK_LOST_OUT_OF_RANGE超出攻击范围 - " .. data 
		end
		if (reason ==ATTACK_LOST_WRONG_TARGET)then
			return "ShowOperationFailed OPRATE_TYPE_ATTK_LOSE攻击失败 - ATTACK_LOST_WRONG_TARGET错误的目标 - " .. data 
		end
		if (reason ==ATTACK_LOST_MOVING)then
			return "ShowOperationFailed OPRATE_TYPE_ATTK_LOSE攻击失败 - ATTACK_LOST_MOVING正在移动状态下不能攻击 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_ATTK_LOSE " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_SPELL_LOSE)then
		if (reason ==LOST_RESON_SPELL_DOENT_EXIST)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_SPELL_DOENT_EXIST技能不存在 - " .. data 
		end
		if (reason ==LOST_RESON_NOT_HAVE_SPELL)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_NOT_HAVE_SPELL你没有这个技能 - " .. data 
		end
		if (reason ==LOST_RESON_NOT_ENOUGH_ANGER)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_NOT_ENOUGH_ANGER怒气值不足 - " .. data 
		end
		if (reason ==LOST_RESON_SPELL_COOLDOWN)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_SPELL_COOLDOWN冷却 - " .. data 
		end
		if (reason ==LOST_RESON_ALREADY_CAST)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_ALREADY_CAST已经在施法 - " .. data 
		end
		if (reason ==LOST_RESON_TARGET_DEAD)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_TARGET_DEAD目标已经死亡 - " .. data 
		end
		if (reason ==LOST_RESON_NEED_TARGET)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_NEED_TARGET此技能需要一个目标 - " .. data 
		end
		if (reason ==LOST_RESON_JUMP_DENY)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_JUMP_DENY跳跃状态不能施法 - " .. data 
		end
		if (reason ==LOST_RESON_SCENE_DENY)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_SCENE_DENY场景禁止施法 - " .. data 
		end
		if (reason ==LOST_RESON_CAN_NOT_CAST)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_CAN_NOT_CAST无法施法 - " .. data 
		end
		if (reason ==LOST_RESON_HAS_CONFLICT_SPELL)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_HAS_CONFLICT_SPELL有技能冲突 - " .. data 
		end
		if (reason ==LOST_RESON_NOTHAS_CHUZHAN)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_NOTHAS_CHUZHAN当前没有出战兵种 - " .. data 
		end
		if (reason ==LOST_RESON_WRONG_FACTION)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_WRONG_FACTION同一阵营不能攻击（争霸天下用） - " .. data 
		end
		if (reason ==LOST_RESON_ACTIVE_SPELL_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_ACTIVE_SPELL_SUCCESS技能激活成功 - " .. data 
		end
		if (reason ==LOST_RESON_IN_SAFE_ZONE)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_IN_SAFE_ZONE安全区域不能攻击（大沼泽用） - " .. data 
		end
		if (reason ==LOST_RESON_YXT_WRONG_FACTION)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_YXT_WRONG_FACTION同一阵营不能攻击（英雄帖用） - " .. data 
		end
		if (reason ==LOST_RESON_PVP_STATE)then
			return "ShowOperationFailed OPRATE_TYPE_SPELL_LOSE技能释放失败 - LOST_RESON_PVP_STATEPVP状态不能释放 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_SPELL_LOSE " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_INTERACTION)then
		if (reason ==INTERACTION_TOO_FAR)then
			return "ShowOperationFailed OPRATE_TYPE_INTERACTION交互,如使用GAMEOBJ 与NPC对话，交接任务等 - INTERACTION_TOO_FARNPC太远 - " .. data 
		end
		if (reason ==INTERACTION_BOOK_DAILYNUM_FULL)then
			return "ShowOperationFailed OPRATE_TYPE_INTERACTION交互,如使用GAMEOBJ 与NPC对话，交接任务等 - INTERACTION_BOOK_DAILYNUM_FULL奇遇日常任务次数已满 - " .. data 
		end
		if (reason ==INTERACTION_NO_ENOUGH_ENDURANCE)then
			return "ShowOperationFailed OPRATE_TYPE_INTERACTION交互,如使用GAMEOBJ 与NPC对话，交接任务等 - INTERACTION_NO_ENOUGH_ENDURANCE精力值不够 - " .. data 
		end
		if (reason ==INTERACTION_JHM_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_INTERACTION交互,如使用GAMEOBJ 与NPC对话，交接任务等 - INTERACTION_JHM_ERROR激活码错误 - " .. data 
		end
		if (reason ==INTERACTION_WORSHIP)then
			return "ShowOperationFailed OPRATE_TYPE_INTERACTION交互,如使用GAMEOBJ 与NPC对话，交接任务等 - INTERACTION_WORSHIP城主膜拜 - " .. data 
		end
		if (reason ==INTERACTION_JHM_HAVE)then
			return "ShowOperationFailed OPRATE_TYPE_INTERACTION交互,如使用GAMEOBJ 与NPC对话，交接任务等 - INTERACTION_JHM_HAVE激活码奖励已领取 - " .. data 
		end
		if (reason ==INTERACTION_JHM_IS_USED)then
			return "ShowOperationFailed OPRATE_TYPE_INTERACTION交互,如使用GAMEOBJ 与NPC对话，交接任务等 - INTERACTION_JHM_IS_USED激活码已使用 - " .. data 
		end
		if (reason ==INTERACTION_JHM_NOT_USE_RANGE)then
			return "ShowOperationFailed OPRATE_TYPE_INTERACTION交互,如使用GAMEOBJ 与NPC对话，交接任务等 - INTERACTION_JHM_NOT_USE_RANGE不在使用范围内 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_INTERACTION " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_USE_GAMEOBJECT)then
		if (reason ==USE_GAMEOBJECT_SUCCEED)then
			return "ShowOperationFailed OPRATE_TYPE_USE_GAMEOBJECT使用游戏对象 - USE_GAMEOBJECT_SUCCEED使用游戏对象成功 - " .. data 
		end
		if (reason ==USE_GAMEOBJECT_FAIL)then
			return "ShowOperationFailed OPRATE_TYPE_USE_GAMEOBJECT使用游戏对象 - USE_GAMEOBJECT_FAIL使用游戏对象失败 - " .. data 
		end
		if (reason ==USE_GAMEOBJECT_QUEST)then
			return "ShowOperationFailed OPRATE_TYPE_USE_GAMEOBJECT使用游戏对象 - USE_GAMEOBJECT_QUEST采集成功 - " .. data 
		end
		if (reason ==USE_GAMEOBJECT_TOO_FAST)then
			return "ShowOperationFailed OPRATE_TYPE_USE_GAMEOBJECT使用游戏对象 - USE_GAMEOBJECT_TOO_FAST采集速度太快 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_USE_GAMEOBJECT " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_BAG)then
		if (reason ==BAG_RESULT_NULL)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_NULL路过的 - " .. data 
		end
		if (reason ==BAG_RESULT_ITEM_NOT_EXIST)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_ITEM_NOT_EXIST物品不存在 - " .. data 
		end
		if (reason ==BAG_RESULT_CHANGE_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_CHANGE_ERROR交换位置错误 - " .. data 
		end
		if (reason ==BAG_RESULT_DESTROY_BIND)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_DESTROY_BIND删除绑定的 - " .. data 
		end
		if (reason ==BAG_RESULT_ITEM_NOT_SELF)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_ITEM_NOT_SELF不是自己的物品 - " .. data 
		end
		if (reason ==BAG_RESULT_REPAIR_FULL)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_REPAIR_FULL修理耐久已满的物品 - " .. data 
		end
		if (reason ==BAG_RESULT_BAG_NOT_EXIST)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_BAG_NOT_EXIST包裹不存在 - " .. data 
		end
		if (reason ==BAG_RESULT_POS_NOT_EXIST)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_POS_NOT_EXIST包裹位置不存在 - " .. data 
		end
		if (reason ==BAG_RESULT_DESTROY_TOOMUCH)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_DESTROY_TOOMUCH数量不够删 - " .. data 
		end
		if (reason ==BAG_RESULT_BAN_FOR_TRADE)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_BAN_FOR_TRADE交易中不允许操作 - " .. data 
		end
		if (reason ==BAG_RESULT_SPLIT_FAILURE)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_SPLIT_FAILURE物品拆分失败 - " .. data 
		end
		if (reason ==BAG_RESULT_LACK_USER)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_LACK_USER数量不够删 - " .. data 
		end
		if (reason ==BAG_RESULT_BAG_FULL)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_BAG_FULL包裹放不下 - " .. data 
		end
		if (reason ==BAG_RESULT_MAX_COUNT)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_MAX_COUNT超过最大拥有数 - " .. data 
		end
		if (reason ==BAG_RESULT_SELL_BIND)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_SELL_BIND定物品不允许出售 - " .. data 
		end
		if (reason ==BAG_RESULT_REPAIR_OK)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_REPAIR_OK铜钱宝箱开启成功 - " .. data 
		end
		if (reason ==BAG_RESULT_REPAIR_MONEY_LACK)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_REPAIR_MONEY_LACK钱不够无法修理 - " .. data 
		end
		if (reason ==BAG_RESULT_EXTENSION_BAG)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_EXTENSION_BAG包裹不支持扩展 - " .. data 
		end
		if (reason ==BAG_RESULT_EXTENSION_MAX_SIZE)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_EXTENSION_MAX_SIZE包裹大小已经最大 - " .. data 
		end
		if (reason ==BAG_RESULT_EXTENSION_MATERIAL_LACK)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_EXTENSION_MATERIAL_LACK包裹扩展材料不足 - " .. data 
		end
		if (reason ==BAG_RESULT_EXTENSION_TRADE)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_EXTENSION_TRADE交易中不允许包裹扩展 - " .. data 
		end
		if (reason ==BAG_RESULT_USE_ITEM_COUNT_MAX)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_USE_ITEM_COUNT_MAX使用物品数量达到上限 - " .. data 
		end
		if (reason ==BAG_RESULT_NOT_ITEM)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_NOT_ITEM没开出物品 - " .. data 
		end
		if (reason ==BAG_RESULT_ITEM_USEING)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_ITEM_USEING物品使用中 - " .. data 
		end
		if (reason ==BAG_RESULT_GET_ITEMS)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_GET_ITEMS获得物品（元宝卡） - " .. data 
		end
		if (reason ==BAG_RESULT_USE_ITEMS)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_USE_ITEMS使用物品（装备宝箱） - " .. data 
		end
		if (reason ==BAG_RESULT_GET_ITEMS_ORANGE)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_GET_ITEMS_ORANGE开启橙色装备 - " .. data 
		end
		if (reason ==BAG_RESULT_USE_ITEMS_GENERALS)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_USE_ITEMS_GENERALS使用物品（将魂使用） - " .. data 
		end
		if (reason ==BAG_RESULT_GET_ITEMS_CABX)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_GET_ITEMS_CABX获得 财神宝箱 - " .. data 
		end
		if (reason ==BAG_RESULT_GET_ITEMS_CABX_BUFF)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_GET_ITEMS_CABX_BUFF获得 财神宝箱的buff 提示 - " .. data 
		end
		if (reason ==BAG_RESULT_ALREADY_KAIGUANG)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_ALREADY_KAIGUANG设备已经开光 - " .. data 
		end
		if (reason ==BAG_RESULT_EXCHANGE_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_EXCHANGE_SUCCESS穿上装备成功 - " .. data 
		end
		if (reason ==BAG_RESULT_OPEN_ITEMS_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_OPEN_ITEMS_SUCCESS打开物品成功（用于概率获得） - " .. data 
		end
		if (reason ==BAG_RESULT_OPEN_ITEMS_FAIL)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_OPEN_ITEMS_FAIL打开物品失败（用于概率获得） - " .. data 
		end
		if (reason ==BAG_RESULT_GET_ITEMS_GENERALS_EQUIP)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_GET_ITEMS_GENERALS_EQUIP使用物品获得武将装备 - " .. data 
		end
		if (reason ==BAG_RESULT_GET_SHOWHAND_JIEZHI)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_GET_SHOWHAND_JIEZHI获得梭哈戒指 - " .. data 
		end
		if (reason ==BAG_RESULT_USE_WUJIANG_EXP_CARD)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_USE_WUJIANG_EXP_CARD使用武将经验卡 - " .. data 
		end
		if (reason ==BAG_RESULT_USE_ITEMS_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_USE_ITEMS_SUCCESS物品使用成功 - " .. data 
		end
		if (reason ==BAG_RESULT_BAG_FULL_SEND_MAIL)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_BAG_FULL_SEND_MAIL包裹已满 发送邮箱提示 - " .. data 
		end
		if (reason ==BAG_RESULT_BAG_FULL_AUTO_SELL)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_BAG_FULL_AUTO_SELL包裹已满自动出售 - " .. data 
		end
		if (reason ==BAG_RESULT_BAG_XIULIAN_USE)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_BAG_XIULIAN_USE使用修炼丹获得经验 - " .. data 
		end
		if (reason ==BAG_RESULT_BAG_SORT_CD)then
			return "ShowOperationFailed OPRATE_TYPE_BAG包裹 - BAG_RESULT_BAG_SORT_CD整理功能冷却中 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_BAG " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_TRADE)then
		if (reason ==TRADE_RESULT_TYPE_NOT_TARGET)then
			return "ShowOperationFailed OPRATE_TYPE_TRADE交易 - TRADE_RESULT_TYPE_NOT_TARGET没有交易对象 - " .. data 
		end
		if (reason ==TRADE_RESULT_TYPE_TRADEING)then
			return "ShowOperationFailed OPRATE_TYPE_TRADE交易 - TRADE_RESULT_TYPE_TRADEING已经交易中无法继续处理交易申请 - " .. data 
		end
		if (reason ==TRADE_RESULT_TYPE_NOT_TRADE)then
			return "ShowOperationFailed OPRATE_TYPE_TRADE交易 - TRADE_RESULT_TYPE_NOT_TRADE非交易进行中无效操作 - " .. data 
		end
		if (reason ==TRADE_RESULT_TYPE_BAG_SIZE)then
			return "ShowOperationFailed OPRATE_TYPE_TRADE交易 - TRADE_RESULT_TYPE_BAG_SIZE包裹空间不足交易失败 - " .. data 
		end
		if (reason ==TRADE_RESULT_TYPE_UNRECOGNISED)then
			return "ShowOperationFailed OPRATE_TYPE_TRADE交易 - TRADE_RESULT_TYPE_UNRECOGNISED未确认交易物品 - " .. data 
		end
		if (reason ==TRADE_RESULT_TYPE_TARGET_CANCEL)then
			return "ShowOperationFailed OPRATE_TYPE_TRADE交易 - TRADE_RESULT_TYPE_TARGET_CANCEL对方交已取消交易 - " .. data 
		end
		if (reason ==TRADE_RESULT_TYPE_LACK_MONEY)then
			return "ShowOperationFailed OPRATE_TYPE_TRADE交易 - TRADE_RESULT_TYPE_LACK_MONEY金钱不足 - " .. data 
		end
		if (reason ==TRADE_RESULT_TYPE_IS_BIND)then
			return "ShowOperationFailed OPRATE_TYPE_TRADE交易 - TRADE_RESULT_TYPE_IS_BIND绑定物品不允许交易 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_TRADE " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_CHAT)then
		if (reason ==CHAT_RESULT_NOT_PLAYER)then
			return "ShowOperationFailed OPRATE_TYPE_CHAT聊天 - CHAT_RESULT_NOT_PLAYER找不到玩家 - " .. data 
		end
		if (reason ==CHAT_RESULT_IS_GAG)then
			return "ShowOperationFailed OPRATE_TYPE_CHAT聊天 - CHAT_RESULT_IS_GAG被禁言 - " .. data 
		end
		if (reason ==CHAT_LEVEL_WHISPER_LEVEL_NO)then
			return "ShowOperationFailed OPRATE_TYPE_CHAT聊天 - CHAT_LEVEL_WHISPER_LEVEL_NO私聊等级不足 - " .. data 
		end
		if (reason ==CHAT_LEVEL_WORLD_LEVEL_NO)then
			return "ShowOperationFailed OPRATE_TYPE_CHAT聊天 - CHAT_LEVEL_WORLD_LEVEL_NO世界聊天等级不足 - " .. data 
		end
		if (reason ==CHAT_RESULT_CHECK_LIMIT)then
			return "ShowOperationFailed OPRATE_TYPE_CHAT聊天 - CHAT_RESULT_CHECK_LIMIT您说话太快了，赶紧练级等等再聊天吧… - " .. data 
		end
		if (reason ==CHAT_RESULT_NO_FACTION)then
			return "ShowOperationFailed OPRATE_TYPE_CHAT聊天 - CHAT_RESULT_NO_FACTION你暂无帮派 - " .. data 
		end
		if (reason ==CHAT_RESULT_NO_GROUP)then
			return "ShowOperationFailed OPRATE_TYPE_CHAT聊天 - CHAT_RESULT_NO_GROUP你暂无队伍 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_CHAT " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_RECEIVE_GIFT_PACKS)then
		if (reason ==RECEIVE_GIFT_PACKS_NOT_FIND)then
			return "ShowOperationFailed OPERTE_TYPE_RECEIVE_GIFT_PACKS领取补偿礼包 - RECEIVE_GIFT_PACKS_NOT_FIND找不到这条补偿礼包 - " .. data 
		end
		if (reason ==RECEIVE_GIFT_PACKS_CHAR_ERROR)then
			return "ShowOperationFailed OPERTE_TYPE_RECEIVE_GIFT_PACKS领取补偿礼包 - RECEIVE_GIFT_PACKS_CHAR_ERROR这礼包不是你的 - " .. data 
		end
		if (reason ==RECEIVE_GIFT_PACKS_CHAR_CREATE)then
			return "ShowOperationFailed OPERTE_TYPE_RECEIVE_GIFT_PACKS领取补偿礼包 - RECEIVE_GIFT_PACKS_CHAR_CREATE礼包指定的发放起始时间在你角色创建时间之前 - " .. data 
		end
		if (reason ==RECEIVE_GIFT_PACKS_RECEIVED)then
			return "ShowOperationFailed OPERTE_TYPE_RECEIVE_GIFT_PACKS领取补偿礼包 - RECEIVE_GIFT_PACKS_RECEIVED该礼包你已经领取过了 - " .. data 
		end
		if (reason ==RECEIVE_GIFT_PACKS_ONLINE_AWARD)then
			return "ShowOperationFailed OPERTE_TYPE_RECEIVE_GIFT_PACKS领取补偿礼包 - RECEIVE_GIFT_PACKS_ONLINE_AWARD在线礼包奖励 - " .. data 
		end
		if (reason ==RECEIVE_GIFT_PACKS_ONLINE_XIANSHI)then
			return "ShowOperationFailed OPERTE_TYPE_RECEIVE_GIFT_PACKS领取补偿礼包 - RECEIVE_GIFT_PACKS_ONLINE_XIANSHI在线礼包显示用 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_RECEIVE_GIFT_PACKS " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_QUEST)then
		if (reason ==INVALIDREASON_QUEST_FAILED_LOW_LEVEL)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_FAILED_LOW_LEVEL 等级太低 You are not high enough level for that quest. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_FAILED_WRONG_RACE)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_FAILED_WRONG_RACE 这个任务对于你的角色类别是无效的 That quest is not available to your race. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_ALREADY_DONE)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_ALREADY_DONE 任务已经完成 You have completed that quest. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_ONLY_ONE_TIMED)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_ONLY_ONE_TIMED 任务只能做一次 You can only be on one timed quest at a time. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_ALREADY_ON)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_ALREADY_ON 已经在任务中 You are already on that quest. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_FAILED_EXPANSION)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_FAILED_EXPANSION 物品太少 This quest requires an expansion enabled account. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_ALREADY_ON2)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_ALREADY_ON2 已经在任务中 ? You are already on that quest. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_FAILED_MISSING_ITEMS)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_FAILED_MISSING_ITEMS 没有完成任务需要的物品 You don't have the required items with you. Check storage. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_FAILED_NOT_ENOUGH_MONEY)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_FAILED_NOT_ENOUGH_MONEY 没有足够的金钱完成任务 You don't have enough money for that quest. - " .. data 
		end
		if (reason ==INVALIDREASON_DAILY_QUESTS_REMAINING)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_DAILY_QUESTS_REMAINING 你已经完成了每天的25个任务 You have already completed 25 daily quests today. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_FAILED_CAIS)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_FAILED_CAIS 有疲劳时间无法完成任务 You cannot complete quests once you have reached tired time. - " .. data 
		end
		if (reason ==INVALIDREASON_DAILY_QUEST_COMPLETED_TODAY)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_DAILY_QUEST_COMPLETED_TODAY 已经完成当天的任务 You have completed that daily quest today. - " .. data 
		end
		if (reason ==INVALIDREASON_QUEST_FINISH_ALL_RIHUAN)then
			return "ShowOperationFailed OPERTE_TYPE_QUEST任务 - INVALIDREASON_QUEST_FINISH_ALL_RIHUAN完成所有日环任务了 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_QUEST " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_NPCBUY)then
		if (reason ==QUEST_TYPE_PROCESS)then
			return "ShowOperationFailed OPERTE_TYPE_NPCBUY购买商品 - QUEST_TYPE_PROCESS任务进度, 名字:1/1 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_NPCBUY " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_FUBEN)then
		if (reason ==NPC_BUY_ITEM_NO_EXIST)then
			return "ShowOperationFailed OPERTE_TYPE_FUBEN副本 - NPC_BUY_ITEM_NO_EXIST物品不存在 - " .. data 
		end
		if (reason ==NPC_BUY_ITEM_UNDER)then
			return "ShowOperationFailed OPERTE_TYPE_FUBEN副本 - NPC_BUY_ITEM_UNDER下架的物品 - " .. data 
		end
		if (reason ==NPC_BUY_ITEM_NO_OPEN)then
			return "ShowOperationFailed OPERTE_TYPE_FUBEN副本 - NPC_BUY_ITEM_NO_OPEN商品还未到开放时间 - " .. data 
		end
		if (reason ==NPC_BUY_ITEM_OUT_TIME)then
			return "ShowOperationFailed OPERTE_TYPE_FUBEN副本 - NPC_BUY_ITEM_OUT_TIME商品已过期 - " .. data 
		end
		if (reason ==NPC_BUY_BAG_OUT_SIZE)then
			return "ShowOperationFailed OPERTE_TYPE_FUBEN副本 - NPC_BUY_BAG_OUT_SIZE包裹放不下了 - " .. data 
		end
		if (reason ==NPC_BUY_MONEY_NO_ENOUGH)then
			return "ShowOperationFailed OPERTE_TYPE_FUBEN副本 - NPC_BUY_MONEY_NO_ENOUGH钱不够了 - " .. data 
		end
		if (reason ==NPC_BUY_MONEY_TRANSFINITE)then
			return "ShowOperationFailed OPERTE_TYPE_FUBEN副本 - NPC_BUY_MONEY_TRANSFINITE购买金额过大 - " .. data 
		end
		if (reason ==NPC_BUY_SELL_OUT)then
			return "ShowOperationFailed OPERTE_TYPE_FUBEN副本 - NPC_BUY_SELL_OUT超出商品限购次数 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_FUBEN " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_KUAFU)then
		if (reason ==FUBEN_OPRATE_NO_GROUP_STATE)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_NO_GROUP_STATE玩家不在组队状态 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_NOT_TWO_PEOPLE)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_NOT_TWO_PEOPLE队伍不是2个人 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_NO_ENERGY)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_NO_ENERGY体力不足 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_NO_MONEY)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_NO_MONEY元宝不足 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_TEAM_NO_LEVEL)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_TEAM_NO_LEVEL队友等级不足 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_TEAM_NO_ENERGY)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_TEAM_NO_ENERGY队友体力不足 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_NO_GROUP)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_NO_GROUP队伍已经解散 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_SEND_TEAM_FRIEND)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_SEND_TEAM_FRIEND邀请队友进入组队副本 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_TEAM_REFUSE_ENETR)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_TEAM_REFUSE_ENETR队友拒绝进入组队副本 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_TEAM_ON_FUBEN_MAP)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_TEAM_ON_FUBEN_MAP队友在副本地图中 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_TEAM_HAVE_FLAGS)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_TEAM_HAVE_FLAGS队友已经发起邀请了 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_REFRESH_BOSS)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_REFRESH_BOSS副本刷出boss - " .. data 
		end
		if (reason ==FUBEN_OPRATE_KILLED_BOSS)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_KILLED_BOSS玩家成功击杀了boss - " .. data 
		end
		if (reason ==FUBEN_OPRATE_MORE_TIMES)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_MORE_TIMES全屏秒杀超过最大次数 - " .. data 
		end
		if (reason ==FUBEN_OPRATE_NO_CREATURES)then
			return "ShowOperationFailed OPERTE_TYPE_KUAFU跨服 - FUBEN_OPRATE_NO_CREATURES全屏秒杀当前没有怪物 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_KUAFU " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_RANK_LIST)then
		if (reason ==KUAFU_OPERATE_CONN_DISCONN)then
			return "ShowOperationFailed OPERTE_TYPE_RANK_LIST排行榜 - KUAFU_OPERATE_CONN_DISCONN游戏服与世界服的链接断开了 - " .. data 
		end
		if (reason ==KUAFU_OPERATE_HASNOT_REGISTER)then
			return "ShowOperationFailed OPERTE_TYPE_RANK_LIST排行榜 - KUAFU_OPERATE_HASNOT_REGISTER本服还没有向世界服注册成功 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_RANK_LIST " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_CLOSE)then
		if (reason ==RANK_LIST_OPERATE_SUCCEED)then
			return "ShowOperationFailed OPERTE_TYPE_CLOSE连接关闭 - RANK_LIST_OPERATE_SUCCEED成功 - " .. data 
		end
		if (reason ==RANK_LIST_OPERATE_TYPE_ERROR)then
			return "ShowOperationFailed OPERTE_TYPE_CLOSE连接关闭 - RANK_LIST_OPERATE_TYPE_ERROR查询类型错误 - " .. data 
		end
		if (reason ==RANK_LIST_OPERATE_OUT_OF_RANGE)then
			return "ShowOperationFailed OPERTE_TYPE_CLOSE连接关闭 - RANK_LIST_OPERATE_OUT_OF_RANGE查询范围超出界限 - " .. data 
		end
		if (reason ==RANK_LIST_OPERATE_MAX_LIKE)then
			return "ShowOperationFailed OPERTE_TYPE_CLOSE连接关闭 - RANK_LIST_OPERATE_MAX_LIKE点赞已超过最大次数 - " .. data 
		end
		if (reason ==RANK_LIST_OPERATE_HAS_LIKE)then
			return "ShowOperationFailed OPERTE_TYPE_CLOSE连接关闭 - RANK_LIST_OPERATE_HAS_LIKE已点过赞了 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_CLOSE " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_HOSTING)then
		if (reason ==ACTIVITY_OPERATE_NOTVIP)then
			return "ShowOperationFailed OPERTE_TYPE_HOSTING托管 - ACTIVITY_OPERATE_NOTVIP你不是vip无法领取vip奖励 - " .. data 
		end
		if (reason ==ACTIVITY_OPERATE_NOENOUGH)then
			return "ShowOperationFailed OPERTE_TYPE_HOSTING托管 - ACTIVITY_OPERATE_NOENOUGH活跃度不够无法领取 - " .. data 
		end
		if (reason ==ACTIVITY_OPERATE_HASGET)then
			return "ShowOperationFailed OPERTE_TYPE_HOSTING托管 - ACTIVITY_OPERATE_HASGET已经领取过该奖励 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_HOSTING " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_STRENGTH)then
		if (reason ==ACHIEVE_OPERATE_NO_FIND)then
			return "ShowOperationFailed OPERTE_TYPE_STRENGTH强化 - ACHIEVE_OPERATE_NO_FIND找不到成就 - " .. data 
		end
		if (reason ==ACHIEVE_OPERATE_NO_GET)then
			return "ShowOperationFailed OPERTE_TYPE_STRENGTH强化 - ACHIEVE_OPERATE_NO_GET尚未达成成就 - " .. data 
		end
		if (reason ==ACHIEVE_OPERATE_HASGET)then
			return "ShowOperationFailed OPERTE_TYPE_STRENGTH强化 - ACHIEVE_OPERATE_HASGET已经领取过成就奖励 - " .. data 
		end
		if (reason ==ACHIEVE_OPERATE_NO_ALL)then
			return "ShowOperationFailed OPERTE_TYPE_STRENGTH强化 - ACHIEVE_OPERATE_NO_ALL总成就不够无法领取 - " .. data 
		end
		if (reason ==ACHIEVE_OPERATE_TITLE_SUC)then
			return "ShowOperationFailed OPERTE_TYPE_STRENGTH强化 - ACHIEVE_OPERATE_TITLE_SUC设置称号成功 - " .. data 
		end
		if (reason ==ACHIEVE_OPERATE_TITLE_FAL)then
			return "ShowOperationFailed OPERTE_TYPE_STRENGTH强化 - ACHIEVE_OPERATE_TITLE_FAL称号不存在无法设置 - " .. data 
		end
		if (reason ==ACHIEVE_OPERATE_NO_MONEY)then
			return "ShowOperationFailed OPERTE_TYPE_STRENGTH强化 - ACHIEVE_OPERATE_NO_MONEY钱不够了不能找回 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_STRENGTH " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE)then
		if (reason ==ATHLETICS_OPERATE_NO_LEV)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_NO_LEV等级不够{1}，不能参加3v3 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_NO_FORCE)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_NO_FORCE战力不够{1}，不能参加3v3 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_NO_OPEN)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_NO_OPEN不在活动时间，不能参加 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_NO_TIME_BUY)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_NO_TIME_BUY3v3次数已用完，可以购买次数 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_NO_TIME)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_NO_TIME3v3次数已用完，请明天再来 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_IN_MATCH)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_IN_MATCH正在匹配中 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_MAX_BUY)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_MAX_BUY超出最大购买次数 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_NO_MONEY)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_NO_MONEY超出最大购买次数 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_HAS_DAY_REWARD)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_HAS_DAY_REWARD已经领取该奖励 - " .. data 
		end
		if (reason ==ATHLETICS_OPERATE_NO_DAY_REWARD)then
			return "ShowOperationFailed OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE切换战斗模式失败 - ATHLETICS_OPERATE_NO_DAY_REWARD尚未到达次数{1} - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_SOCIAL)then
		if (reason ==ATHLETICS_OPERATE_BUFF_OCCUR)then
			return "ShowOperationFailed OPERTE_TYPE_SOCIAL社交 - ATHLETICS_OPERATE_BUFF_OCCURbuff产生 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_SOCIAL " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_FACTION)then
		if (reason ==DOUJIAN_OPERATE_NO_LEV)then
			return "ShowOperationFailed OPERTE_TYPE_FACTION帮派 - DOUJIAN_OPERATE_NO_LEV等级不够，需要{0}才能参与斗剑台 - " .. data 
		end
		if (reason ==DOUJIAN_OPERATE_ZHUCHENG)then
			return "ShowOperationFailed OPERTE_TYPE_FACTION帮派 - DOUJIAN_OPERATE_ZHUCHENG只能在主城中参加活动 - " .. data 
		end
		if (reason ==DOUJIAN_OPERATE_NO_TIME)then
			return "ShowOperationFailed OPERTE_TYPE_FACTION帮派 - DOUJIAN_OPERATE_NO_TIME挑战次数已满 - " .. data 
		end
		if (reason ==DOUJIAN_OPERATE_IN_CD)then
			return "ShowOperationFailed OPERTE_TYPE_FACTION帮派 - DOUJIAN_OPERATE_IN_CD挑战CD中 - " .. data 
		end
		if (reason ==DOUJIAN_OPERATE_MAX_TIME)then
			return "ShowOperationFailed OPERTE_TYPE_FACTION帮派 - DOUJIAN_OPERATE_MAX_TIME超过最大购买次数 - " .. data 
		end
		if (reason ==DOUJIAN_OPERATE_NO_MONEY)then
			return "ShowOperationFailed OPERTE_TYPE_FACTION帮派 - DOUJIAN_OPERATE_NO_MONEY元宝不足购买斗剑次数 - " .. data 
		end
		if (reason ==DOUJIAN_OPERATE_NO_MONEY_CD)then
			return "ShowOperationFailed OPERTE_TYPE_FACTION帮派 - DOUJIAN_OPERATE_NO_MONEY_CD元宝不足不能清理CD - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_FACTION " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_FIELD_BOSS)then
		if (reason ==DOUJIAN_OPERATE_SELF_BATTLE)then
			return "ShowOperationFailed OPERTE_TYPE_FIELD_BOSS野外BOSS - DOUJIAN_OPERATE_SELF_BATTLE自己正在被别人挑战 - " .. data 
		end
		if (reason ==DOUJIAN_OPERATE_OTHER_BATTLE)then
			return "ShowOperationFailed OPERTE_TYPE_FIELD_BOSS野外BOSS - DOUJIAN_OPERATE_OTHER_BATTLE挑战的人正在被别人挑战 - " .. data 
		end
		if (reason ==DOUJIAN_OPERATE_COUNTDOWN)then
			return "ShowOperationFailed OPERTE_TYPE_FIELD_BOSS野外BOSS - DOUJIAN_OPERATE_COUNTDOWN刷新在冷却中 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_FIELD_BOSS " .. reason .. "    "  .. data
	end
	if(type == OPERTE_TYPE_WORLD_BOSS)then
		if (reason ==PLAYER_CLOSE_OPERTE_APPD_ONE1)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_APPD_ONE1插入单个物品时，包裹id 超过最大值 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_APPD_ONE2)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_APPD_ONE2创建玩家其他的对象的时候，发现之前已经创建过了 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_APPD_ONE3)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_APPD_ONE3托管到期 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_APPD_ONE4)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_APPD_ONE4GM命令 踢人 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_APPD_ONE5)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_APPD_ONE5GM命令 回档数据 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_APPD_ONE6)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_APPD_ONE6 踢玩家下线 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE4)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE4帐号信息不完整 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE5)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE5Char_Create put  fail! - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE6)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE6登录时候不是我的角色 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE7)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE7待定 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE8)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE8跨服回来的玩家 m_player 不存在 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE9)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE9初始化数据库数据失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE10)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE10看看有没有玩家数据需要修复或者升级的 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE11)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE11重设元宝数量 取数据时失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE12)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE12踢人 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE13)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE13如果连接不存在,则根据解开的session结果进行创建对应的session实例(on_create_conn_get_session create_sesstion duplicate) - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE14)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE14客户端连接失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE15)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE15把玩家由pk服回到游戏服,登录失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE16)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE16检测跨服玩家是否可以登陆 -玩家不存在 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE17)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE17检测跨服玩家是否可以登陆 -超时 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE18)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE18登陆队列 context->GetGuid().empty(） - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE19)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE19合服检测 手游下线 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE20)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE20通知场景服玩家加入地图 m_scened_conn不存在 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE21)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE21玩家加入地图实例时，玩家binlg不存在  - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE22)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE22玩家加入地图 创建实例失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE23)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE23如果已登录过 !m_account.empty() - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE24)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE24帐号名称超长 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE25)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE25验证是否可以登录本服验证失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE26)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE26账户表数据不存在 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE27)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE27玩家已在线关闭掉旧链接 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE28)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE28手游登录，但是还没有玩家角色 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE39)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE39数据准备完毕，玩家开始登录 玩家不存在 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE40)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE40获取sessionKey对象,已经登录过 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE41)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE41获取sessionKey对象，帐户名称，并且判断一下是否超长(超长则失败) - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE42)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE42获取sessionKey对象，Get_Session: other_data size is wrong - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE43)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE43获取sessionKey对象，验证session_key是否可以登录这个服务器（为不可登录时候断开） - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE44)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE44获取sessionKey对象，LogindContext::Get_Session:%s load account fail(获取帐号表数据异常) - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE45)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE45获取sessionKey对象，LogindContext::Get_Session:%s Get_Chars_List fail（获取角色列表数据异常） - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE46)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE46跨服读取数据失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE47)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE47运气不好，登录时碰上正在释放自己数据 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE48)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE48合服检测，从服玩家状态 不为 STATUS_LOGGEDIN - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE49)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE49封号 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE50)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE50踢人 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE51)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE51玩家不存在时关闭连接 LuaPlayerLogin %s, but not found - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE52)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE52角色在其他地方登陆 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE53)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE53登陆是无法获得数据 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE54)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE54登陆时 player login call puts fail %s, fd %u - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE55)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE55on_create_conn_get_session create_mobile_context duplicate - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE56)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE56登陆失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE57)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE57Gm命令 @自爆 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE58)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE58PK服登陆数据异常 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_ONE59)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_ONE59从数据库读取玩家数据失败 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_2046)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_2046一个角色对应两个账号	 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_2047)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_2047改名字的时候找不到account，刷新就可以过了 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_LOGDIN_2048)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_LOGDIN_2048session的account不等于player的account,简单来说,串号了. - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE29)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE29停止移动，地图模板为null - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE30)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE30对象移动，地图模板为null - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE31)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE31对象移动的时候  状态不为 STATUS_LOGGEDIN  时候 （assert error: Handle_Unit_Move  ASSERT(status_ == STATUS_LOGGEDIN)） - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE32)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE32传送是 状态不为 STATUS_LOGGEDIN （ScenedContext::Teleport status_ != STATUS_LOGGEDIN） - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE33)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE33如果玩家未正确的加入地图，这个时间传送也是失败的 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE34)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE34任务链条中断 assert error: Handle_QuestGiver_Status_Query  ASSERT(questGetter) - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE35)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE35玩家被杀死时，地图数据不存在 Player::OnKilled ASSERT(GetMap()) - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE36)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE36加入地图宠物不存在的时候  Player::OnJoinMap ASSERT(m_pets.empty()) - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE37)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE37任务管理者不存在的时候 Player::Create ASSERT(!m_questMgr) - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_SCREND_ONE38)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_SCREND_ONE38传送超时1分钟 - " .. data 
		end
		if (reason ==PLAYER_CLOSE_OPERTE_POLICED_4001)then
			return "ShowOperationFailed OPERTE_TYPE_WORLD_BOSS世界BOSS - PLAYER_CLOSE_OPERTE_POLICED_4001在日志服重复登录 - " .. data 
		end
		return "未知错误1  OPERTE_TYPE_WORLD_BOSS " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_MOUNT_QICHENG)then
		if (reason ==OPERTE_TYPE_FACTION_IS_HAVE)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_IS_HAVE你已经在帮派中 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_NAME_ERR)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_NAME_ERR帮派名称超过6个字符 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_NAME_HAVE_FUCK)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_NAME_HAVE_FUCK帮派名称中有非法字符 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_NAME_REPEAT)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_NAME_REPEAT帮派名称重复 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_CREATE_MAX)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_CREATE_MAX帮派数量达到上限 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_CREATE_COST)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_CREATE_COST元宝不足无法创建帮派 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_NOT_MANAGER)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_NOT_MANAGER不是帮主或副帮主无法编辑 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_NOTICE_ERR)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_NOTICE_ERR帮派公告超过144个字符 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_NOTICE_ERR_PB)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_NOTICE_ERR_PB帮派公告包含非法字符 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_LEV_LOW)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_LEV_LOW等级不够，不能加入帮派 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_MEMBER_MAX_COUNT)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_MEMBER_MAX_COUNT帮派人数已满无法加入 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_JOIN_SUCESS)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_JOIN_SUCESS恭喜你加入帮派 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_FACTION_REFUSED_JOIN)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_FACTION_REFUSED_JOIN你被拒绝加入帮派 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_KICKED)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_KICKED你被移出帮派 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_KICK_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_KICK_SUCCESS踢出帮派成员 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_MAX_ZHIWEI)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_MAX_ZHIWEI职位人数已满 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_APPOINT_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_APPOINT_SUCCESS任命成功 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_PROMOTED)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_PROMOTED你被任命为 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_APPOINT_NOSELF)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_APPOINT_NOSELF自己不能任命自己  - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_DONATIONMAX)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_DONATIONMAX捐献次数超过最大次数 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_DONATION_GOLD)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_DONATION_GOLD捐献所需金币不够 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_DONATION_YB)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_DONATION_YB捐献所需元宝不够 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_SHOP_NUMLOW)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_SHOP_NUMLOW商品剩余数量不足无法购买 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_DEVOTE)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_DEVOTE帮贡不足 - " .. data 
		end
		if (reason ==OPEATE_TYPE_FACTION_MONEY_ERR)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPEATE_TYPE_FACTION_MONEY_ERR帮派资金不足 - " .. data 
		end
		if (reason ==OPEATE_TYPE_FACTION_LEVEL_UP)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPEATE_TYPE_FACTION_LEVEL_UP帮派升级成功 - " .. data 
		end
		if (reason ==OPEATE_TYPE_FACTION_NOT_JOIN)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPEATE_TYPE_FACTION_NOT_JOIN没有可以加入的帮派 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_PLAYER_OFFLINE)then
			return "ShowOperationFailed OPRATE_TYPE_MOUNT_QICHENG坐骑骑乘 - OPERTE_TYPE_FACTION_PLAYER_OFFLINE对方不在线,无法加入成员 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_MOUNT_QICHENG " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_TELEPORT)then
		if (reason ==OPERTE_TYPE_FACTION_BOSS_START)then
			return "ShowOperationFailed OPRATE_TYPE_TELEPORT传送 - OPERTE_TYPE_FACTION_BOSS_START召唤boss - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_TELEPORT " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_UPGRADE)then
		if (reason ==OPERTE_TYPE_FACTION_GIFT_THANK)then
			return "ShowOperationFailed OPRATE_TYPE_UPGRADE游戏中的升级操作 - OPERTE_TYPE_FACTION_GIFT_THANK感谢完成 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_GIFT_THANK_ALL)then
			return "ShowOperationFailed OPRATE_TYPE_UPGRADE游戏中的升级操作 - OPERTE_TYPE_FACTION_GIFT_THANK_ALL全部感谢完成 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_GIFT_GET_EXREWARD)then
			return "ShowOperationFailed OPRATE_TYPE_UPGRADE游戏中的升级操作 - OPERTE_TYPE_FACTION_GIFT_GET_EXREWARD领取成功 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_GIFT_GET_EXREWARD_ALL)then
			return "ShowOperationFailed OPRATE_TYPE_UPGRADE游戏中的升级操作 - OPERTE_TYPE_FACTION_GIFT_GET_EXREWARD_ALL全部领取成功 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_GIFT_SEND)then
			return "ShowOperationFailed OPRATE_TYPE_UPGRADE游戏中的升级操作 - OPERTE_TYPE_FACTION_GIFT_SEND赠送成功 - " .. data 
		end
		if (reason ==OPERTE_TYPE_FACTION_GIFT_SEND_FAIL)then
			return "ShowOperationFailed OPRATE_TYPE_UPGRADE游戏中的升级操作 - OPERTE_TYPE_FACTION_GIFT_SEND_FAIL赠送成功 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_UPGRADE " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_ACTIVITY)then
		if (reason ==OPERTE_TYPE_FACTION_GIFT_NOTICE)then
			return "ShowOperationFailed OPRATE_TYPE_ACTIVITY活动 - OPERTE_TYPE_FACTION_GIFT_NOTICE礼物广播 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_ACTIVITY " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_ACHIEVE)then
		if (reason ==OPERTE_TYPE_FACTION_BOSSDEFENSE_TIME_ERROR)then
			return "ShowOperationFailed OPRATE_TYPE_ACHIEVE成就 - OPERTE_TYPE_FACTION_BOSSDEFENSE_TIME_ERROR非法时间 无法挑战boss - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_ACHIEVE " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_ATHLETICS)then
		if (reason ==OPERTE_TYPE_SOCIAL_HAS_SEND)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_HAS_SEND好友请求信息已发送 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_NOT_FIND)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_NOT_FIND角色不存在或者角色不在线 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_SELF_FULL)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_SELF_FULL自己好友列表已满 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_TARGET_FULL)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_TARGET_FULL对方好友列表已满 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_ALREADY_FRIEND)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_ALREADY_FRIEND对方已经是你的好友 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_ADD_MYSELF)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_ADD_MYSELF不能自己添加自己为好友 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_HAS_SEND_ADD)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_HAS_SEND_ADD已经发送申请 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_NO_MONEY)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_NO_MONEY元宝或者金币不足 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_NOT_ENEMY)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_NOT_ENEMY对方不是仇人 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_ENEMY_OFFLINE)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_ENEMY_OFFLINE对方处于离线中 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_ENEMY_NOT_IN_FIELD)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_ENEMY_NOT_IN_FIELD对方不在野外 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_REVENGE_TIMES_NOT_ENOUGH)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_REVENGE_TIMES_NOT_ENOUGH复仇传送次数不足 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_RENAME_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_RENAME_SUCCESS改名成功 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_OTHNER_FACTION)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_OTHNER_FACTION对方已在帮派 - " .. data 
		end
		if (reason ==OPERTE_TYPE_SOCIAL_SAME_FACTION)then
			return "ShowOperationFailed OPRATE_TYPE_ATHLETICS竞技 - OPERTE_TYPE_SOCIAL_SAME_FACTION对方已在同帮派 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_ATHLETICS " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_XIANFU)then
		if (reason ==HOSTING_OPERTE_IS_SELF)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_IS_SELF无法对自己操作 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_LEVEL_NOT)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_LEVEL_NOT等级不足 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_IS_HOSTING)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_IS_HOSTING托管中 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_IS_HOSTING_APPLY)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_IS_HOSTING_APPLY托管申请 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_FRIEND_NOT)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_FRIEND_NOT好友不存在 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_HAS_NOT_24H)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_HAS_NOT_24H还没到24小时 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_FRIEND_OUTLINE)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_FRIEND_OUTLINE好友不在线 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_FRIEND_ERR)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_FRIEND_ERR托管好友错误 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_NOT_HOSTING)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_NOT_HOSTING不在托管中 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_FRIEND_ONLINE)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_FRIEND_ONLINE好友在线 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_LOGIN_HOSTING)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_LOGIN_HOSTING托管登录中 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_FRIEND_APPLY)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_FRIEND_APPLY申请发送 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_LOGIN)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_LOGIN登录 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_SUCCESS)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_SUCCESS托管申请成功 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_CANCEL)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_CANCEL取消托管 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_REFUSED)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_REFUSED拒绝托管 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_NOT_DUE_TO)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_NOT_DUE_TO托管未到期 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_FRIEND_HOSTING_LOGIN)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_FRIEND_HOSTING_LOGIN对方托管登录中，无法操作 - " .. data 
		end
		if (reason ==HOSTING_OPERTE_FRIEND_NOT_HAS_YOU)then
			return "ShowOperationFailed OPRATE_TYPE_XIANFU仙府夺宝 - HOSTING_OPERTE_FRIEND_NOT_HAS_YOU对方好友列表没有你 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_XIANFU " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_REWARD)then
		if (reason ==STRENGTH_OPERTE_FAIL)then
			return "ShowOperationFailed OPRATE_TYPE_REWARD获得物品 - STRENGTH_OPERTE_FAIL强化失败 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_REWARD " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_DOUJIAN)then
		if (reason ==BATTLE_MODE_OPERTE_PEACE_MODE_DENY)then
			return "ShowOperationFailed OPRATE_TYPE_DOUJIAN斗剑台 - BATTLE_MODE_OPERTE_PEACE_MODE_DENY战斗中不能切换和平模式 - " .. data 
		end
		if (reason ==BATTLE_MODE_OPERTE_PEACE_MODE_IN_CD)then
			return "ShowOperationFailed OPRATE_TYPE_DOUJIAN斗剑台 - BATTLE_MODE_OPERTE_PEACE_MODE_IN_CD和平模式在CD中 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_DOUJIAN " .. reason .. "    "  .. data
	end
	if(type == OPRATE_TYPE_GEM)then
		if (reason ==FIELD_BOSS_OPERTE_WILL_START)then
			return "ShowOperationFailed OPRATE_TYPE_GEM宝石 - FIELD_BOSS_OPERTE_WILL_START野外BOSS即将在{1}分钟后开启, 请大家安排好时间 - " .. data 
		end
		if (reason ==FIELD_BOSS_OPERTE_WILL_OCCUR)then
			return "ShowOperationFailed OPRATE_TYPE_GEM宝石 - FIELD_BOSS_OPERTE_WILL_OCCUR野外boss即将出现，请各位大侠做好准备 - " .. data 
		end
		if (reason ==FIELD_BOSS_OPERTE_OCCUR)then
			return "ShowOperationFailed OPRATE_TYPE_GEM宝石 - FIELD_BOSS_OPERTE_OCCUR野外boss已经在各个地图刷新，请各位大侠火速前往击杀 查看详情 - " .. data 
		end
		if (reason ==FIELD_BOSS_OPERTE_BOSS_BORN)then
			return "ShowOperationFailed OPRATE_TYPE_GEM宝石 - FIELD_BOSS_OPERTE_BOSS_BORN{1}已出现在{2}，请各位大侠火速前往击杀 立即前往 - " .. data 
		end
		if (reason ==FIELD_BOSS_OPERTE_BOSS_KILL)then
			return "ShowOperationFailed OPRATE_TYPE_GEM宝石 - FIELD_BOSS_OPERTE_BOSS_KILL{1}已被成功击杀！恭喜{2}获得{3}1分钟的优先开启权利 - " .. data 
		end
		if (reason ==FIELD_BOSS_OPERTE_PROTECT)then
			return "ShowOperationFailed OPRATE_TYPE_GEM宝石 - FIELD_BOSS_OPERTE_PROTECT九龙宝箱保护时间结束，静待各位大侠开启 - " .. data 
		end
		if (reason ==FIELD_BOSS_OPERTE_PICKED)then
			return "ShowOperationFailed OPRATE_TYPE_GEM宝石 - FIELD_BOSS_OPERTE_PICKED恭喜{1}最终开启九龙宝箱，获得XXXX - " .. data 
		end
		if (reason ==FIELD_BOSS_OPERTE_TOO_FAR)then
			return "ShowOperationFailed OPRATE_TYPE_GEM宝石 - FIELD_BOSS_OPERTE_TOO_FAR太远了，你不可能开启这个宝箱 - " .. data 
		end
		return "未知错误1  OPRATE_TYPE_GEM " .. reason .. "    "  .. data
	end
	return "未知错误2   " .. type .. "  " .. reason .. "    "  .. data
end
