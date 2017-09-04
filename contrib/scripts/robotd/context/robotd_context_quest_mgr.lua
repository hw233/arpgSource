
-- 进行下一个任务
function PlayerInfo:DoNextQuest()
	
	if not self.quest then
		outFmtDebug("		quest binlogObject not init")
		return
	end
	
	-- 先日常任务
	local questInfo = self.quest:GetQuestInfo()
	if not questInfo then
		outFmtDebug("		no available quest")
		return
	end
	
	if self.questStart then
		outFmtDebug("		self.questStart true")
		return
	end
	
	local config = tb_quest[questInfo.questId]
	local targets = config.targets
	-- 找任务目标
	for indx, target in ipairs(targets) do
		local targetType = target[ 1 ]
		local stepInfo	 = questInfo.steps[indx]
		if stepInfo.stepState == 0 and Quest_Function[targetType] then
			return Quest_Function[targetType](self, questInfo.questId, indx)
		end
	end
	
	if questInfo.state == QUEST_STATUS_COMPLETE then
		outFmtDebug("		send for finish questId = %d", questInfo.questId)
		self:AutoSendFinishQuest(questInfo.questId)
		return
	end
	
	outFmtDebug("		no deal target for questId = %d", questInfo.questId)

	return 
end

-- 装备神兵
function PlayerInfo:RobotEquipDivine(questId, indx)
	if not self.spellMgr then
		outFmtDebug("spellMgr not attach")
		return
	end
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type = config.targets[indx][ 1 ]
	questOperate.questId = questId
	
	local divineId = config.targets[indx][ 2 ]
	local divineDict = self.spellMgr:getDivineIdList()
	
	local need = 0
	
	for _, divId in ipairs(divineDict) do
		if divineId == 0 or divId == divineId then
			need = divId
			break
		end
	end
	
	-- 存在神兵 装备
	if need > 0 then
		--divine_active ( id :number )
		questOperate.callback = function()
			outFmtDebug("call_divine_switch divineId = %d", need)
			self:call_divine_switch(divineId)
			-- 发送完成任务命令
			self:AutoSendFinishQuest(questId)
		end
	else
		-- 判断是否要购买
		if divineId == 0 then
			-- 没法购买
			outFmtDebug("not divineId to buy for questId = %d", questId)
			return
		else
			-- 激活神兵
			outFmtDebug("call_divine_active divineId = %d", need)
			self:call_divine_active (divineId)
			return
		end
	end
	
	return questOperate
end

-- 和NPC讲话
function PlayerInfo:TalkWithNPC(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type 		= config.targets[indx][ 1 ]
	questOperate.mapid		= stepParams[ 2 ]
	questOperate.x			= stepParams[ 3 ]
	questOperate.y			= stepParams[ 4 ]
	questOperate.objcet_id	= stepParams[ 5 ]
	questOperate.questId = questId

	local entry = questOperate.objcet_id
	questOperate.callback 	= function()
		outFmtDebug("call_talk_with_npc entry = %d, questId = %d", entry, questId)
		self:call_talk_with_npc (questOperate.objcet_id ,questId)
	end

	-- 寻路
	self.questStart = true

	return questOperate
end

-- 使用采集物
function PlayerInfo:RobotUseGameObject(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type 		= config.targets[indx][ 1 ]
	questOperate.mapid		= stepParams[ 2 ]
	questOperate.x			= stepParams[ 3 ]
	questOperate.y			= stepParams[ 4 ]
	questOperate.objcet_id	= stepParams[ 5 ]
	questOperate.questId = questId
	
	-- 寻路
	self.questStart = true
	
	return questOperate
end

-- 杀怪
function PlayerInfo:RobotKillMonster(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local targetPosType = stepParams[ 1 ]
	local questOperate = nil
	
	if targetPosType == 1 or targetPosType == 7 then
		questOperate = {}
		questOperate.type 		= config.targets[indx][ 1 ]
		questOperate.objcet_id	= config.targets[indx][ 2 ]
		questOperate.mapid		= stepParams[ 2 ]
		questOperate.x			= stepParams[ 3 ]
		questOperate.y			= stepParams[ 4 ]
		questOperate.questId = questId
		
		-- 判断任务是否完成
		questOperate.IsFinishMethod = function()
			local questIndx = self.quest:GetQuestIndxById(questId)
			local vist = not questIndx
			if not vist then
				local intstart = QUEST_FIELD_QUEST_START + questIndx * MAX_QUEST_INFO_COUNT
				local stepstart = intstart + QUEST_INFO_STEP_START + (indx-1) * MAX_QUEST_TARGET_INFO_COUNT
				vist =  self.quest:GetUInt16(stepstart + QUEST_TARGET_INFO_SHORT0, 0) == 1
			end
			return vist
		end
		
		-- 寻路
		self.questStart = true
	elseif targetPosType == 8 then
		local afterInfo = config.afterAccept
		if self:GetMapID() == afterInfo[ 2 ] then
		--	2,4002,126,30,7404,129,36,1002
			questOperate = {}
			questOperate.type 		= config.targets[indx][ 1 ]
			questOperate.objcet_id	= config.targets[indx][ 2 ]
			questOperate.mapid		= afterInfo[ 2 ]
			questOperate.x			= afterInfo[ 6 ]
			questOperate.y			= afterInfo[ 7 ]
			questOperate.questId = questId
			
			-- 判断任务是否完成
			questOperate.IsFinishMethod = function()
				local questIndx = self.quest:GetQuestIndxById(questId)
				local vist = not questIndx
				if not vist then
					local intstart = QUEST_FIELD_QUEST_START + questIndx * MAX_QUEST_INFO_COUNT
					local stepstart = intstart + QUEST_INFO_STEP_START + (indx-1) * MAX_QUEST_TARGET_INFO_COUNT
					vist =  self.quest:GetUInt16(stepstart + QUEST_TARGET_INFO_SHORT0, 0) == 1
				end
				return vist
			end
		else
			local questIndx = self.quest:GetQuestIndxById(questId)
			self:call_execute_quest_cmd_after_accepted(questIndx)
		end
	end
	
	return questOperate
end

-- 使用物品
function PlayerInfo:RobotUseItem(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
		
	local questOperate = {}
	questOperate.type = config.targets[indx][ 1 ]
	questOperate.questId = questId
	
	local entry = config.targets[indx][ 2 ]
	local item_guid = self.itemMgr:GetItemGuidByEntry(entry)

	questOperate.callback 	= function()
		if item_guid then
			outFmtDebug("call_bag_item_user entry = %d, item_guid = %s, questId = %d", entry, item_guid, questId)
			self:call_bag_item_user (item_guid, 1)
			-- 发送完成任务命令
			self:AutoSendFinishQuest(questId)
		else
			outFmtDebug("not find entry %d in bag", entry)
		end
	end
	
	return questOperate
end

-- 穿戴装备
function PlayerInfo:RobotSuit(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type = config.targets[indx][ 1 ]
	questOperate.questId = questId
	
	local suitPos = config.targets[indx][ 2 ]
	local suitEntry = config.targets[indx][ 3 ]
	local entry, src_pos = self.itemMgr:GetSuitEntryAndBagPosBySuitPos(suitPos, suitEntry)
	questOperate.callback = function()
		if src_pos then
			local dst_pos = tb_item_template[entry].pos
			outFmtDebug("call_bag_exchange_pos entry = %d, %d, %d, %d", BAG_TYPE_MAIN_BAG, src_pos, BAG_TYPE_EQUIP, dst_pos)
			self:call_bag_exchange_pos (BAG_TYPE_MAIN_BAG, src_pos, BAG_TYPE_EQUIP, dst_pos)
			-- 发送完成任务命令
			self:AutoSendFinishQuest(questId)
		else
			outFmtDebug("not find suitPos %d, suitEntry %d in bag", suitPos, suitEntry)
		end
	end
	
	return questOperate
end

--挑战资源副本
function PlayerInfo:RobotJoinResourceInstance(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type = config.targets[indx][ 1 ]
	questOperate.questId = questId
	
	local instanceId = config.targets[indx][ 2 ]
	local instanceCount = config.targets[indx][ 3 ]
	
	questOperate.callback = function()
		outFmtDebug("RobotJoinResourceInstance callback")

		self:call_res_instance_enter(4) -- 银币副本
		--self:call_instance_exit(0)
		self:AutoSendFinishQuest(questId)
	end
	
	return questOperate
end


--挑战斗剑台副本
function PlayerInfo:RobotJoinDoujiantai(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type = config.targets[indx][ 1 ]
	questOperate.questId = questId
	
	local count = config.targets[indx][ 2 ]
	
	questOperate.callback = function()
		outFmtDebug("RobotJoinDoujiantai callback")

		self:call_doujiantai_fight(0) -- 斗剑台
		--self:call_instance_exit(0)
		self:AutoSendFinishQuest(questId)
	end
	
	return questOperate
end

--升级装备部位
function PlayerInfo:RobotStrengthSuit(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type = config.targets[indx][ 1 ]
	questOperate.questId = questId
	
	local part = config.targets[indx][ 2 ]
	local count = config.targets[indx][ 3 ]

	
	questOperate.callback = function()
		outFmtDebug("RobotStrengthSuit callback")

		self:call_strength(1) -- 部位
		self:AutoSendFinishQuest(questId)
	end
	
	return questOperate
end


--升级技能
function PlayerInfo:RobotRaiseSkill(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type = config.targets[indx][ 1 ]
	questOperate.questId = questId
	
	local part = config.targets[indx][ 2 ]
	local count = config.targets[indx][ 3 ]

	
	questOperate.callback = function()
		outFmtDebug("RobotRaiseSkill callback")
		self:call_raise_base_spell(RAISE_BASE_SKILL,10005)
		self:call_raise_base_spell(RAISE_BASE_SKILL,10008)
		
		self:AutoSendFinishQuest(questId)
	end
	
	return questOperate
end

--升级坐骑
function PlayerInfo:RobotTrainMount(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local questOperate = {}
	questOperate.type = config.targets[indx][ 1 ]
	questOperate.questId = questId
	
	local count = config.targets[indx][ 2 ]

	
	questOperate.callback = function()
		outFmtDebug("RobotRaiseSkill callback")
		self:call_raise_mount()
		
		self:AutoSendFinishQuest(questId)
	end
	
	return questOperate
end


--升lv
function PlayerInfo:RobotLvUp(questId, indx)
	local config = tb_quest[questId]
	local stepParams = config.targetsPosition[indx]
	
	local lv = config.targets[indx][ 2 ]
	
	local cmd = "@Rank "..lv
	self:call_chat_by_channel(CHAT_TYPE_WORLD, cmd)
	
	return
end

function PlayerInfo:AutoSendFinishQuest(questId)
	local config = tb_quest[questId]
	-- 发送完成任务命令
	if config.popup ~= 0 then
		local questIndx = self.quest:GetQuestIndxById(questId)
		if questIndx then
			outFmtDebug("	call_pick_quest_reward questIndx = %d", questIndx)
			self:call_pick_quest_reward(questIndx)
		end
	end
end

Quest_Function = {
	[QUEST_TARGET_TYPE_PLAYER_LEVEL] 		= PlayerInfo.RobotLvUp,
--	[QUEST_TARGET_TYPE_PLAYER_FORCE] 		= IQuestPlayerForce,
--	[QUEST_TARGET_TYPE_FACTION] 			= IQuestJoinFaction,
--	[QUEST_TARGET_TYPE_FACTION_DONATION] 	= IQuestFactionDonate,
--	[QUEST_TARGET_TYPE_FACTION_ACTIVITY] 	= IQuestJoinFactionActivity,
--	[QUEST_TARGET_TYPE_FIELD_BOSS] 			= IQuestJoinFieldBoss,
--	[QUEST_TARGET_TYPE_WORLD_BOSS] 			= IQuestJoinWorldBoss,
	[QUEST_TARGET_TYPE_RESOURCE_INSTANCE] 	= PlayerInfo.RobotJoinResourceInstance,
--	[QUEST_TARGET_TYPE_TRIAL_INSTANCE] 		= IQuestJoinTrialInstance,
--	[QUEST_TARGET_TYPE_OWN_DIVINE] 			= IQuestOwnDivine,
	[QUEST_TARGET_TYPE_EQUIP_DIVINE] 		= PlayerInfo.RobotEquipDivine,
--	[QUEST_TARGET_TYPE_STRENGTH_DIVINE] 	= IQuestStrengthDivine,
	[QUEST_TARGET_TYPE_RAISE_SKILL] 		= PlayerInfo.RobotRaiseSkill,
	[QUEST_TARGET_TYPE_TRAIN_MOUNT] 		= PlayerInfo.RobotTrainMount,
--	[QUEST_TARGET_TYPE_RAISE_MOUNT_SKILL] 	= IQuestRaiseMountSkill,
--	[QUEST_TARGET_TYPE_FRIEND_DONATION] 	= IQuestFriendDonate,
	[QUEST_TARGET_TYPE_STRENGTH_SUIT]		= PlayerInfo.RobotStrengthSuit,
--	[QUEST_TARGET_TYPE_STRENGTH_GEM] 		= IQuestStrengthGem,
	[QUEST_TARGET_TYPE_TALK] 				= PlayerInfo.TalkWithNPC,
	[QUEST_TARGET_TYPE_PICK_GAME_OBJECT] 	= PlayerInfo.RobotUseGameObject,
--	[QUEST_TARGET_TYPE_CHECK_GAME_OBJECT] 	= IQuestInspect,
	[QUEST_TARGET_TYPE_KILL_MONSTER] 		= PlayerInfo.RobotKillMonster,
	[QUEST_TARGET_TYPE_USE_ITEM] 			= PlayerInfo.RobotUseItem,
	[QUEST_TARGET_TYPE_SUIT]				= PlayerInfo.RobotSuit,
--	QUEST_TARGET_TYPE_MOUNT_LEVEL = 25	-- 坐骑达到x
--	QUEST_TARGET_TYPE_FRIEND_NUM = 26	-- 拥有好友x
--	QUEST_TARGET_TYPE_KILL_MONSTER_NUM = 27	-- 击杀怪物x
--	QUEST_TARGET_TYPE_DIVINE_NUM = 28	-- 拥有神兵x
--	QUEST_TARGET_TYPE_DIVINE_LEV = 29	-- 神兵等级x
--	QUEST_TARGET_TYPE_DOUJIANTAI = 30	-- 斗剑台胜利x
--	QUEST_TARGET_TYPE_TITLE_NUMBER = 31	-- 称号数量达到x个
--	QUEST_TARGET_TYPE_FASHION_NUMBER = 32	-- 时装数量达到x个
--	QUEST_TARGET_TYPE_ACHIEVE = 40	-- 成就点数达到X点
--	QUEST_TARGET_TYPE_SUIT_TITLE = 41	-- 佩戴X称号
--	QUEST_TARGET_TYPE_SUIT_FASHION = 42	-- 穿上时装X
	[QUEST_TARGET_TYPE_JOIN_DOUJIANTAI]		= PlayerInfo.RobotJoinDoujiantai,
--	QUEST_TARGET_TYPE_JOIN_KUAFU_3V3 = 44	-- 参与X次3V3
--	QUEST_TARGET_TYPE_WIN_KUAFU_3V3 = 45	-- 3v3获胜x次
--	QUEST_TARGET_TYPE_JOIN_XIANFU = 46	-- 参与X次仙府夺宝
--	QUEST_TARGET_TYPE_WIN_XIANFU = 47	-- 仙府夺宝获胜X次
--	QUEST_TARGET_TYPE_KILL_MONSTER_COLLECT = 48	-- 杀怪收集物品
--	QUEST_TARGET_TYPE_TURN_ITEM_IN = 49	-- 上交物品
}