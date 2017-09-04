function PlayerInfo:CallScenedDoSomething(typ, data, str)
	call_scened_appd_to_send_do_something(self:GetScenedFD(), self:GetGuid(), typ, data, str)
end

-- 场景服发过来要应用服做点事
function PlayerInfo:DoGetScenedDoSomething  ( ntype, data, str)
	if SCENED_APPD_ENTER_VIP_INSTANCE == ntype then
		self:checkVipMapTeleport(data, str)
	elseif SCENED_APPD_ENTER_TRIAL_INSTANCE == ntype then
		self:checkTrialMapTeleport()
	elseif SCENED_APPD_ENTER_RES_INSTANCE == ntype then
		--进入资源副本
		self:checkResMapTeleport(data)
	elseif SCENED_APPD_PASS_RES_INSTANCE == ntype then
		--通关资源副本
		self:passResInstance(data)
	elseif SCENED_APPD_PASS_WORLD_RISK == ntype then
		self:passWorldRiskInstance(data)
	elseif SCENED_APPD_PASS_TRIAL_INSTANCE == ntype then
		self:passTrialInstance(data)
	elseif SCENED_APPD_PASS_VIP_INSTANCE == ntype then
		self:passVipInstance(data, tonumber(str))
	elseif SCENED_APPD_USE_ITEM == ntype then
		-- 判断如果道具不是药品就不使用了
		if tb_item_template[data].type == ITEM_TYPE_MEDICINE then
			UseHpItem(self, data, 1)
		end
	elseif SCENED_APPD_USE_RESPAWN_ITEM == ntype then
		useRespawn(self)
	elseif SCENED_APPD_ADD_ENEMY == ntype then
		self:AddEnemy(str, data)
	elseif SCENED_APPD_ADD_MAIL == ntype then
		local mailInfo = string.split(str, '|')
		local rewards = mailInfo[ 1 ]
		local name = mailInfo[ 2 ]
		local desc = mailInfo[ 3 ]
		local giftType = tonumber(mailInfo[ 4 ])
		AddGiftPacksData(self:GetGuid(),0,giftType,os.time(),os.time() + 86400*30, name, desc, rewards, SYSTEM_NAME)
	elseif SCENED_APPD_KILL_MONSTER == ntype then
		-- 成就
		self:AddAchieve(QUEST_TARGET_TYPE_KILL_MONSTER_NUM, 1)
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_KILL_MONSTER, {data})
	elseif SCENED_APPD_KILL_MONSTER_COLLECT == ntype then
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_KILL_MONSTER_COLLECT, {data})
	elseif SCENED_APPD_JOIN_FIELD_BOSS == ntype then
		-- 参加野外BOSS
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_FIELD_BOSS, {data})
	elseif SCENED_APPD_GAMEOBJECT == ntype then
		-- 采集物品
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_PICK_GAME_OBJECT, {data})
	elseif SCENED_APPD_TALK == ntype then
		-- 对话
		-- 加任务
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_TALK, {data, tonumber(str)})
	elseif SCENED_APPD_RIDE == ntype then
		self:SetRideState(data)
		
	elseif SCENED_APPD_GOLD_RESPAWN == ntype then
		if self:costMoneys(MONEY_CHANGE_ATUO_GOLD_RESPAWN, {{MONEY_TYPE_BIND_GOLD, data}}) then
			-- 通知场景服复活
			self:CallScenedDoSomething(APPD_SCENED_RESPAWN)
		end
	elseif SCENED_APPD_DOUJIANTAI == ntype then
		-- 斗剑台结果
		globalCounter:BattleFinish(self, data, tonumber(str))
		
	elseif SCENED_APPD_XIULIANCHANG == ntype then
		-- 修练场掠夺结果
		self:PlunderFinish(data, str)
	elseif SCENED_APPD_XIANFU_PRACTISE == ntype then
		self:AppdAddItems({{data, tonumber(str)}}, nil, LOG_ITEM_OPER_TYPE_XIANFU_PRACTISE)
	elseif SCENED_APPD_ADD_OFFLINE_RISK_REWARD == ntype then
		--[[local items = string.split(str, "|")
		local sell = 0
		local itemMgr = self:getItemMgr()
		local empty_count = itemMgr:getEmptyCount(BAG_TYPE_EQUIP_BAG)	--取出剩余位置个数
		local rewardDict = {}
		local dict = {}
		for _, item in ipairs(items) do
			local itemInfo = string.split(item, ',')
			local entry = tonumber(itemInfo[ 1 ])
			local count = tonumber(itemInfo[ 2 ])
			dict[entry] = count
			if tb_item_template[entry].pos > 0 then
				-- 已经满了
				if empty_count <= 0 then
					dict[entry] = nil
				end
				-- 当前会超过
				if empty_count > 0 and empty_count < count then
					dict[entry] = empty_count
				end

				empty_count = empty_count - count
				for k = 1, count do
					table.insert(rewardDict, {entry, 1})
				end
			else
				table.insert(rewardDict, {entry, count})
			end
		end
		
		if empty_count < 0 then
			sell = -empty_count
		end
		
		self:AppdAddItems(rewardDict, MONEY_CHANGE_OFFLINE, LOG_ITEM_OPER_TYPE_OFFLINE, nil, nil, nil, 3)
		
		local list = Change_To_Item_Reward_Info(dict)
		self:call_offline_reward_result (sell, data, 0, 0, list)--]]
	elseif SCENED_APPD_ENTER_MASS_BOSS_INSTANCE == ntype then
		self:checkMassBossMapTeleport(data)
	elseif SCENED_APPD_FACTION_TOWER_WIN == ntype then
		local faction = app.objMgr:getObj(self:GetFactionId())
		if faction then
			faction:OnTowerTodayFloorUpdate(self,data)
		end
	elseif SCENED_APPD_SINGLE_PVP_RESULT == ntype then
		self:OnProcessSingleMatchResult(data, str)
	end
end
