local Packet = require 'util.packet'


-------------------------------------------------技能-------------------------------------------------------------

BASE_SKILL = 1				--基础技能

-- 升级基础(包括)技能
function PlayerInfo:DoHandleRaiseSpell(raiseType, spellId)
	local config   = tb_skill_base[spellId]
	local spellMgr = self:getSpellMgr()
	local index = spellMgr:getSpellUpgradeIndex(spellId)
	local upLevelConfig = tb_skill_uplevel[index]
	
	-- 扣除道具
	if #upLevelConfig.uplevel_item > 0 then
		if not self:useMulItem(upLevelConfig.uplevel_item) then
			outFmtError("use item fail")
			return
		end
	end
	
	-- 扣除资源
	if #upLevelConfig.uplevel_cost > 0 then
		if not self:costMoneys(MONEY_CHANGE_UP_ASSISTSPELL, upLevelConfig.uplevel_cost) then
			outFmtError("sub resouce fail")
			return
		end
	end
	
	local prev = spellMgr:getSpellLevel(spellId)
	spellMgr:onRaiseSpell(raiseType, spellId)
	local spellLv = spellMgr:getSpellLevel(spellId)
	--local spellTable = {}
	
	-- 在技能槽的修改技能槽数据(怒气技能也是主动技能)
	if self:isInitiativeSpell(config.is_initiative) or self:isAngerSpell(spellId) then
		--同步主动技能到p对象
		if self:isSloted(spellId) then
			local slot = config.skill_slot
			self:replace(slot, spellId, spellLv)
			--table.insert(spellTable, {spellId, spellLv})
			
			-- 如果是连招
			local arry = tb_skill_base[spellId].follow
			for _, id in pairs(arry) do
				self:replace(slot, id, spellLv)
			--	table.insert(spellTable, {id, spellLv})
			end
		else
			self:SetSpellInfo(spellId, spellLv)
		end
	elseif self:isPassiveSpell(config.is_initiative) then
		self:updatePassive(spellId, spellLv)
		--table.insert(spellTable, {spellId, spellLv})
	elseif self:isSupportSpell(config.is_initiative) then
		self:SetSpellInfo(spellId, spellLv)
	end
	
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
	-- 是否发送场景服
	--self:sendSpellInfoIfEnabled(config.is_initiative, spellTable)
	self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_SKILL_SUCCESS)
	
	local questMgr = self:getQuestMgr()
	questMgr:OnUpdate(QUEST_TARGET_TYPE_RAISE_SKILL, {spellId})
	
	outFmtDebug("raise spell %d success, from %d to %d", spellId, prev, spellLv)
end

-- 设置技能信息
function PlayerInfo:SetSpellInfo(spellId, spellLv)
	playerLib.SetSpellLevel(self.ptr, spellId, spellLv)
end

-- 怒气技能进阶
function PlayerInfo:DoHandleUpgradeAngleSpell(spellId)
	local config   = tb_skill_base[spellId]
	local spellMgr = self:getSpellMgr()
	local upgradeConfig = tb_assistangerspell_upgrade[spellId]

	local nextId = upgradeConfig.nextid
	--同步主动技能到p对象
	local slot = config.skill_slot
	
	--激活技能
	spellMgr:activeBaseSpell(nextId)
	
	self:replace(slot, nextId, 1)
	
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
	--发送到场景服替换主动技能信息
	--self:Send2ScenedReplaceEquipedSpell(slot, nextId, 1)
	self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_ANGLE_SKILL)
	
	outFmtDebug("upgrade spell success, from %d to %d", spellId, nextId)
end

-- 看看能否激活
function PlayerInfo:activeBaseSpell(spellId, activeType)
	
	local config   = tb_skill_base[spellId]
	local spellMgr = self:getSpellMgr()
	
	-- 判断技能是否存在
	if not self:isSpellExist(spellId) then
		outFmtError("spellId %d not exist", spellId)
		return
	end
	
	-- 判断玩家是否拥有这个技能
	if self:hasSpell(spellId) then
		outFmtError("player has exist spellId %d", spellId)
		return
	end
	
	--[[-- 等级是否满足条件
	local learnConfig = tb_learn_spell[spellId]
	if learnConfig == nil then
		outFmtError("spellId %d not in tb_learn_spell", spellId)
		return
	end 
	
	if activeType ~= SPELL_ACTIVE_BY_LEVEL and not self:checkPlayerLevel(learnConfig.playerLevel) then
		outFmtError("activeBaseSpell spellId %d, player level not enough, need level %d", spellId, learnConfig.playerLevel)
		return
	end
	
	-- 判断消耗道具
	if #learnConfig.item > 0 then
		if not self:hasMulItem(learnConfig.item) then
			outFmtError("item not enough")
			return
		end
	end
	
	if #learnConfig.resource > 0 then
		if not self:checkMoneyEnoughs(learnConfig.resource) then
			outFmtError("resouce not enough")
			return
		end
	end
	
	
	-- 扣除道具
	if #learnConfig.item > 0 then
		if not self:useMulItem(learnConfig.item) then
			outFmtError("use item fail")
			return
		end
	end
	
	-- 扣除资源
	if #learnConfig.resource > 0 then
		if not self:costMoneys(MONEY_CHANGE_ACTIVE_SPELL, learnConfig.resource) then
			outFmtError("sub resouce fail")
			return
		end
	end--]]

	-- 激活技能
	spellMgr:activeBaseSpell(spellId)
	
	self:onActiveSpell(spellId, true)
	
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
end

-- 判断人物等级是否满足条件
function PlayerInfo:checkPlayerLevel(needLevel)
	local level = self:GetLevel()
	return level >= needLevel
end

-- 是否是主动技能
function PlayerInfo:isInitiativeSpell(initiative)
	return initiative >= SPELL_INITIATIVE_DAMAGE and initiative <= SPELL_INITIATIVE_BUFF
end

-- 是否是被动技能
function PlayerInfo:isPassiveSpell(initiative)
	return initiative >= SPELL_PASSIVE_DAMAGE and initiative <= SPELL_PASSIVE_BUFF
end

-- 是否是辅助技能
function PlayerInfo:isSupportSpell(initiative)
	return initiative == SPELL_SUPPORT
end

-- 激活技能
function PlayerInfo:onActiveSpell(spellId, autoEquip)
	local config   = tb_skill_base[spellId]
	
	-- 判断技能是否存在
	if not self:isSpellExist(spellId) then
		outFmtError("spellId %d not exist", spellId)
		return
	end
	
	-- 判断玩家是否拥有这个技能
	if self:hasSpell(spellId) then
		outFmtError("player has exist spellId %d", spellId)
		return
	end
	
	-- 如果是主动技能
	if self:isInitiativeSpell(config.is_initiative) then
		local config = tb_skill_base[spellId]
		local slot = config.skill_slot
		if not self:hasSkillBySlot(slot) and autoEquip then
			-- 同步主动技能到p对象
			self:replace(slot, spellId, 1)
		else
			self:SetSpellInfo(spellId, 1)
		end
		outFmtDebug("on active spell %d", spellId)
		return
	end
	
	self:onActiveSpellWithoutInitiative(spellId)
end

-- 可适用坐骑激活技能
function PlayerInfo:onActiveSpellWithoutInitiative(spellId)
	local config   = tb_skill_base[spellId]
	
	-- 判断技能是否存在
	if not self:isSpellExist(spellId) then
		outFmtError("spellId %d not exist", spellId)
		return
	end
	
	-- 判断玩家是否拥有这个技能
	if self:hasSpell(spellId) then
		outFmtError("player has exist spellId %d", spellId)
		return
	end
	
	-- 被动技能发送场景服
	if self:isPassiveSpell(config.is_initiative) then
		--同步到p对象中
		self:updatePassive(spellId, 1)
		outFmtDebug("on active spell %d", spellId)
		return
	end
	
	-- 辅助技能
	if self:isSupportSpell(config.is_initiative) then
		playerLib.AddSupportSpell(self.ptr, spellId)
		outFmtDebug("on active spell %d", spellId)
		return
	end
end

-- 技能是否存在
function PlayerInfo:isSpellExist(spellId)
	return tb_skill_base[spellId] ~= nil
end

-- 是否是怒气技能
function PlayerInfo:isAngerSpell(spellId)
	return tb_skill_base[spellId].skill_slot == SLOT_ANGER
end

function PlayerInfo:isDivineSpell(spellId)
	return tb_skill_base[spellId].skill_type == SKILL_TYPE_DIVINE
end

-- 是否是基础技能
function PlayerInfo:isBaseSkill(spellId)
	return tb_skill_base[spellId].skill_type == BASE_SKILL
end

-- 是否达到顶级
function PlayerInfo:isTopLevel(spellId)
	local spellMgr = self:getSpellMgr()
	local spellLv  = self:GetSpellLevel(spellId)
	local topLevel = spellMgr:getSpellTopLevel(spellId)
	
	return spellLv == topLevel
end

-- 是否拥有这个技能
function PlayerInfo:hasSpell(spellId)
	local spellLv = self:GetSpellLevel(spellId)
	return spellLv > 0
end

-- 技能等级
function PlayerInfo:GetSpellLevel(spellId)
	local spellLv = playerLib.GetSpellLevel(self.ptr, spellId)
	return spellLv
end

--[[
-- TODO: (以后会用 暂时不用) 替换技能槽技能
function PlayerInfo:HandleReplaceSlotSpell(pkt)
	
	local spellId  = pkt.spellId
	local config   = tb_skill_base[spellId]
	local spellMgr = self:getSpellMgr()
	
	-- 判断技能是否存在
	if not self:isSpellExist(spellId) then
		return
	end
	
	-- 判断玩家是否拥有这个技能
	if not self:hasSpell(spellId) then
		return
	end	
	
	-- 当前技能是否为主动技能
	if not self:isInitiativeSpell(config.is_initiative) then
		return
	end
	
	-- 是否已经在技能槽中
	if self:isSloted(spellId) then
		return
	end
	
	-- 	槽位是否合法
	local slot = config.skill_slot
	if slot > SPELL_SLOT_COUNT or slot <= SLOT_COMBO then
		return
	end
	
	-- 同步主动技能到p对象
	self:replace(slot, spellId, spellLv)
	
	--发送到场景服替换主动技能信息
	self:Send2ScenedReplaceEquipedSpell(slot, spellId, spellLv)
end
]]

-- 是否已经在技能槽
function PlayerInfo:isSloted(spellId)
	for i = PLAYER_INT_FIELD_SPELL_START, PLAYER_INT_FIELD_SPELL_END-1 do
		if self:GetUInt16(i, SHORT_SLOT_SPELL_ID) == spellId then
			return true
		end
	end
	
	return false
end

-- 替换(包括升级情况)
function PlayerInfo:replace(slot, spellId, spellLv)
	playerLib.UpdateSlotSpell(self.ptr, slot, spellId, spellLv)
end

-- 被动技能同步到P数据
function PlayerInfo:updatePassive(spellId, spellLv)
	playerLib.UpdatePassive(self.ptr, spellId, spellLv)
end

-- 判断技能槽是否有技能
function PlayerInfo:hasSkillBySlot(slot)
	for i = PLAYER_INT_FIELD_SPELL_START, PLAYER_INT_FIELD_SPELL_END-1 do
		local sl = self:GetByte(i, BYTE_SLOT)
		if sl == slot then
			return true
		end
	end
	
	return false
end

--[[
-- 判断是否需要发送场景服
function PlayerInfo:sendSpellInfoIfEnabled(is_initiative, spellTable)
	-- 主动技能
	if self:isInitiativeSpell(is_initiative) then
		if self:isSloted(spellId) then
			self:Send2ScenedUpdateSpellInfo(spellTable)
		end
	elseif self:isPassiveSpell(is_initiative) then
		--TODO: 如果不是神兵技能, 或者当前技能是已经装备的神兵技能
		self:Send2ScenedUpdatePassiveInfo(spellTable)
	end
end


-- 发送到场景服替换主动技能信息
function PlayerInfo:Send2ScenedReplaceEquipedSpell(slot, spellId, spellLv)
	local pkt = Packet.new(INTERNAL_OPT_REPLACE_EQUIPED_SPELL)
	pkt:writeUTF(self:GetGuid())
	pkt:writeByte(slot)
	pkt:writeU16(spellId)
	pkt:writeByte(spellLv)
	app:sendToConnection(self:GetScenedFD(), pkt)
	pkt:delete()
end

-- 发送到场景服更新技能等级
function PlayerInfo:Send2ScenedUpdateSpell(spellType, spellTable)
	local pkt = Packet.new(INTERNAL_OPT_UPDATE_SPELL_INFO)
	
	pkt:writeUTF(self:GetGuid())
	pkt:writeByte(spellType)
	pkt:writeByte(#spellTable)
	for _, spell in pairs(spellTable) do
		local spellId = spell[ 1 ]
		local spellLv = spell[ 2 ]
		pkt:writeU16(spellId)
		pkt:writeByte(spellLv)
	end
	
	app:sendToConnection(self:GetScenedFD(), pkt)
		
	pkt:delete()
end

--INTERNAL_OPT_CHANGE_DIVINE_INFO	= 82

-- 发送到场景服更新技能槽技能信息
-- 愤怒技能修改也发这边
function PlayerInfo:Send2ScenedUpdateSpellInfo(spellTable)
	self:Send2ScenedUpdateSpell(TYPE_SPELL_SLOT, spellTable)
end

-- 发送到场景服更新被动技能信息
function PlayerInfo:Send2ScenedUpdatePassiveInfo(spellTable)
	self:Send2ScenedUpdateSpell(TYPE_SPELL_PASSIVE, spellTable)
end
]]




----------------------------------------------------------坐骑--------------------------------------------------------
UPGRADE_MODE_MANUAL = 1	-- 手动进阶

-- 申请升星坐骑
function PlayerInfo:DoHandleRaiseMount()
	local spellMgr = self:getSpellMgr()	
	local level = spellMgr:getMountLevel()
	local config = tb_mount_base[level]
	
	local star  = spellMgr:getMountStar()
	local trainExp = spellMgr:getTrainExp()	
	
	-- 获得暴击值
	local vip = false
	local multi = self:randomMulti(config, vip)
	
	-- 计算升星上线
	local seq = (level - 1) * 11 + star + 1
	local trainConfig = tb_mount_train[seq]
	local addExp = multi * trainConfig.addTrainExp
	local limit = trainConfig.exp
	
	self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_MOUNT_EXP, {addExp, multi})
	--if multi > 1 then
	--	self:call_operate_result_tip(MODULE_MOUNT,MODULE_MOUNT_UPGRADE,0,0,multi)
	--end
	-- 如果升星了
	if addExp + trainExp >= limit then
		local value = addExp + trainExp - limit
		spellMgr:setTrainExp(value)
		
		local nextStar = star + 1
		local upgradeConfg = tb_mount_upgrade[level]
		-- 不能进阶 或者 手动进阶
		if nextStar < 10 or upgradeConfg.upgradeMode == UPGRADE_MODE_MANUAL then
			spellMgr:setMountStar(nextStar)
			self:SetMountStar(nextStar)
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_MOUNT_STAR)
		else -- 自动进阶
			self:upgraded()
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_MOUNT)
		end
		
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		
		--更新开服排行
		DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_MOUNT,self})
	else
		-- 未升星, 只加经验
		spellMgr:addTrainExp(addExp)
	end
	
	local questMgr = self:getQuestMgr()
	questMgr:OnUpdate(QUEST_TARGET_TYPE_TRAIN_MOUNT)
	
	outFmtDebug("raise from (%d, %d, %d) to (%d, %d, %d)", level, star, trainExp, spellMgr:getMountLevel(), spellMgr:getMountStar(), spellMgr:getTrainExp())
end

-- 随机暴击值
function PlayerInfo:randomMulti(config, vip)
	-- 确实是VIP的随机方式, 还是普通的
	local plist = config.trainIds
	local randList = tb_mount_train_type[plist[ 1 ]].range
	if vip then
		randList = tb_mount_train_type[plist[ 2 ]].range
	end

	-- 通过随机值来获得随机到的倍数
	return GetRandomExp(randList)
end

-- 申请升阶坐骑
function PlayerInfo:DoHandleUpgradeMount()
	local spellMgr = self:getSpellMgr()
	local level = spellMgr:getMountLevel()
	--[[	
	local prev = spellMgr:getBlessExp()
	local ret, added = self:upgradeOnce(prev)
	
	if not ret and added == 0 then
		outFmtDebug("upgrade fail")
		return
	end
	local now = prev + added
	
	if ret then--]]
		self:upgraded()
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_MOUNT)
--[[		
		now = 0
	end
	
	if prev ~= now then
		spellMgr:setBlessExp(now)
	end--]]
	
	outFmtDebug("upgrade from (%d, 10) to (%d, %d)", level, spellMgr:getMountLevel(), spellMgr:getMountStar())
end

-- 一键进阶
function PlayerInfo:DoHandleUpgradeMountOneStep(useItem)
	local spellMgr = self:getSpellMgr()

	local level = spellMgr:getMountLevel()
	local upgradeConfig = tb_mount_upgrade[level]
	local cost = upgradeConfig.upgradecost
	
	local prev = spellMgr:getBlessExp()
	local now = prev
	local times = 0
	local vist = false
	
	while true do
		-- 如果扣除物品失败, break
		-- 判断是否同时拥有多个物品
		if useItem == 0 then
			if not self:hasMulItem(cost, times+1) then
				break
			end
		else
			local ret, _, _ = checkItemEnoughIfCostMoneyEnabled(costItemTable, multiple)
			if not ret then
				break
			end
		end
		-- 加扣除次数
		times = times + 1
		
		-- 如果升级了 也break
		local ret, added = self:upgradeOnce(now)
		if ret then
			now = 0
			vist = true
			break
		else
			now = now + added
		end
	end
	
	-- 扣道具
	
	if not self:useMulItemIfCostMoneyEnabled(cost) then
		outFmtError("one step upgrade alarm!!!!!!!!!!!!!!!!")
		return
	end
	
	if not self:useMulItem(cost, times) then
		outFmtError("one step upgrade alarm!!!!!!!!!!!!!!!!")
		return
	end
	
	-- 进阶了
	if vist then
		self:upgraded()
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_MOUNT)
	end
	
	-- 设置经验
	if prev ~= now then
		spellMgr:setBlessExp(now)
	end
	outFmtDebug("upgrade from (%d, 10, %d) to (%d, %d, %d)", level, prev, spellMgr:getMountLevel(), spellMgr:getMountStar(), spellMgr:getBlessExp())
end

-- 进行进阶一次
function PlayerInfo:upgradeOnce(blessExp)
	local spellMgr = self:getSpellMgr()
	local level = spellMgr:getMountLevel()
	local upgradeConfig = tb_mount_upgrade[level]
	local range = upgradeConfig.addBlessExpRange
	local limit = upgradeConfig.upgradeExp
	
	local added = GetRandomExp(range)
	
	-- 获得加的祝福值
	local now = blessExp + added
	
	-- 如果祝福值满足条件
	if now >= limit then
		return true, 0
	end
	
	return false, added
end

-- 激活坐骑
function PlayerInfo:activeMount()
	self:upgraded()
	local spellMgr = self:getSpellMgr()
	spellMgr:addMountLevelBase()
end

-- 进阶
function PlayerInfo:upgraded()
	local spellMgr = self:getSpellMgr()
	local level = spellMgr:getMountLevel()
		
	spellMgr:setMountLevel(level + 1)

	self:SetMountLevel(level+1)
	self:SetMountStar(0)
	
	spellMgr:setMountStar(0)
	self:DoAfterUpgrade(level+1)
	
	-- 成就
	self:AddAchieve(QUEST_TARGET_TYPE_MOUNT_LEVEL, 1)
	
end

-- 进阶后做某些事
function PlayerInfo:DoAfterUpgrade(level)
	
	-- 解锁技能
	local a = tb_mount_base[level].skills
	for _, spellId in pairs(a) do
		self:activeMountSpell(spellId)
	end
	
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
	
	--更新开服排行
	DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_MOUNT,self})
end

-- 解锁坐骑技能
function PlayerInfo:activeMountSpell(spellId)
	local config   = tb_skill_base[spellId]
	local spellMgr = self:getSpellMgr()
	
	-- 判断技能是否存在
	if not self:isSpellExist(spellId) then
		outFmtError("spellId %d not exist", spellId)
		return
	end
	
	-- 判断玩家是否拥有这个技能
	if self:hasSpell(spellId) then
		outFmtError("player has spellId %d", spellId)
		return
	end
	
	-- 激活技能
	spellMgr:activeMountSpell(spellId)
	
	self:onActiveSpellWithoutInitiative(spellId)
	
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
end


ILLUSION_ITEM_ACTIVE = 1		--消耗对应坐骑碎片
ILLUSION_RESOURCE_ACTIVE = 2	--消耗一定数量元宝

-- 解锁幻化操作
function PlayerInfo:onActiveIllusion(illuId)
	local config = tb_mount_illusion[illuId]
	local spellMgr = self:getSpellMgr()
	
	if config.condition == ILLUSION_ITEM_ACTIVE then
		if not self:useMulItem(config.costItem) then
			outFmtError("onActiveIllusion item not enough")
			return
		end
	elseif config.condition == ILLUSION_RESOURCE_ACTIVE then
		if not self:costMoneys(MONEY_CHANGE_ILLUSION, config.costResource) then
			outFmtError("onActiveIllusion resouce not enough")
			return
		end
	end
	
	-- 激活幻化
	spellMgr:onActiveIllusion(illuId)
	
	-- 激活幻化的技能
	for _, spellId in pairs(config.spells) do
		self:onActiveSpellWithoutInitiative(spellId)
	end
	self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_MOUNT_ILLUSION_ACTIVE)
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
end

-- 申请幻化坐骑
function PlayerInfo:DoHandleIllusion(illuId)
	local spellMgr = self:getSpellMgr()

	local curr = 0
	if self:GetCurrIllusionId() ~= illuId then
		curr = illuId
	end
	
	if curr == 0 then
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_MOUNT_ILLUSION_UNSET)
	else
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_MOUNT_ILLUSION_SET)
	end
	self:SetCurrIllusionId(curr)
	-- 发送到场景服
	--[[
	self:Send2ScenedIllusion(illuId)
	]]
end

function PlayerInfo:raiseMountLevelBase()
	local spellMgr = self:getSpellMgr()
	local levelBase = spellMgr:getMountLevelBase()
	if tb_mount_raise_level[levelBase+1] then
		if not self:useAllItems(MONEY_CHANGE_RAISE_MOUNT, tb_mount_raise_level[levelBase+1].cost) then
			return
		end
		spellMgr:addMountLevelBase()
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		self:CallOptResult(OPRATE_TYPE_UPGRADE,UPGRADE_OPRATE_MOUNT_LEVEL_UP)
	end
end

-- 移除过期幻化信息
function PlayerInfo:PlayerRemoveExpiredIllusion(expiredTable)
	-- 没有过期的return
	if #expiredTable == 0 then
		return
	end
	
	for _, info in pairs(expiredTable) do
		
		local illuId = info[ 1 ]
		-- 如果是幻化的需要除去幻化
		if self:GetCurrIllusionId() == illuId then
			self:SetCurrIllusionId(0)
		end
		
		-- 移除技能
		for i = 2, #info do
			local spellId = info[ i ]
			local config = tb_skill_base[spellId]
			if self:isPassiveSpell(config.is_initiative) then
				self:updatePassive(spellId, 0)
			elseif self:isSupportSpell(config.is_initiative) then
				self:SetSpellInfo(spellId, 0)
			end
		end
	end
	
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
end

-------------------------------------神兵------------------------------------------
-- 神兵的技能解锁

function PlayerInfo:onDivineActivedSpell(divineId, spellId,isPassive)
	
	-- 判断技能是否存在
	if not self:isSpellExist(spellId) then
		outFmtError("spellId %d not exist", spellId)
		return
	end
	
	-- 判断玩家是否拥有这个技能
	if self:hasSpell(spellId) then
		outFmtError("player has spellId %d", spellId)
		return
	end

	local spellMgr = self:getSpellMgr()
	spellMgr:addDivineSkill(divineId, spellId,isPassive)

	self:onActiveSpell(spellId)
	
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
end
--是否装配神兵
function PlayerInfo:hasEquDivine(divineId)
	local prev = self:GetUInt32(PLAYER_INT_FIELD_DIVINE_ID)
	if divineId == 0 and prev ~= 0 then
		return true
	elseif divineId ~= 0 and divineId == prev then
		return true
	end
	return false
end

-- 替换神兵
function PlayerInfo:switchDivine(divineId)
	local prev = self:GetUInt32(PLAYER_INT_FIELD_DIVINE_ID)
	if prev == divineId then

		return
	end

	self:SetUInt32(PLAYER_INT_FIELD_DIVINE_ID, divineId)
	
	local spellMgr = self:getSpellMgr()
	local spell = 0
	local lv = 0
	
	if divineId > 0 then
		-- TODO, 这个方法需要自己去写
		spell, lv = spellMgr:GetDivineInitiativeSpellInfo(divineId)
	end
	self:replace(SLOT_DIVINE, spell, lv)
	
	-- TODO, 这个方法需要自己去写, 返回格式 {{spellId, lv},...}
	local prevPassiveInfoTable = spellMgr:GetDivinePassiveSpellInfoTable(prev)
	for i = 1, #prevPassiveInfoTable do
		local spellId = prevPassiveInfoTable[ i ][ 1 ]
		self:updatePassive(spellId, 0)
	end
	
	local passiveInfoTable = spellMgr:GetDivinePassiveSpellInfoTable(divineId)
	for i = 1, #passiveInfoTable do
		local spellId = passiveInfoTable[ i ][ 1 ]
		local spellLv = passiveInfoTable[ i ][ 2 ]
		self:updatePassive(spellId, spellLv)
	end
	
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
	
	-- 加任务
	local questMgr = self:getQuestMgr()
	questMgr:OnUpdate(QUEST_TARGET_TYPE_EQUIP_DIVINE, {divineId})
end


-- 申请激活神兵
function PlayerInfo:DivineActive(divineId)

	local config = tb_divine_base[divineId]

	local activeType = config.avtivetype
	local activeData = config.avtivedata

	local spellMgr = self:getSpellMgr()

	--道具激活
	if activeType == 1 then
		if self:useMulItem(activeData) then
			return self:ApplyDivineActive(config.id,config.time)
		else 
			outFmtError("active divine has not enough item");
			return false;
		end

	--资源激活
	elseif activeType == 2 then
		local tf,tab = self:checkMoneyEnoughIfUseGoldIngot(activeData)

		if tf then 
			if self:costMoneys(MONEY_CHANGE_SHENBING_BUY,tab) then
				return self:ApplyDivineActive(config.id,config.time)
			else
				outFmtError("active divine has not enough money");
			end
		else 
			outFmtError("active divine has not enough money");
		end
	--累计登录激活
	elseif activeType == 3 then

	--累计pvp激活
	elseif activeType == 4 then

	--vip激活
	elseif activeType == 5 then

	end
	
	
	
	
end
--应用激活
function PlayerInfo:ApplyDivineActive(id,t)
	local time = 0
	if t > 0 then
		time = os.time() + t  * 3600 -- FIXME 需要自动移除
	end
	local spellMgr = self:getSpellMgr()
	if spellMgr:addDivine(id,time) then
		
		local num = spellMgr:getDivineNum()
		self:SetDivineNum(num)
		--激活主动技能
		local config = tb_divine_base[id]
		self:onDivineActivedSpell(id,config.skill,false)
		
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		
		self:AddAchieve(QUEST_TARGET_TYPE_DIVINE_NUM,1)
		
		return true
	end
	return false
end
--升级神兵
function PlayerInfo:DivineUpLev(divineId)
	local spellMgr = self:getSpellMgr()
	local idx,curlev,curbless = spellMgr:getDivinIdxLevBless(divineId)

	if curlev >= #tb_divine_bless then
		outFmtError("divine is max lev already")
		return 
	end

	local config = tb_divine_bless[curlev + 1]

	--是否有足够的道具
	if not self:hasMulItem(config.cost) then
		outFmtError("divine up lev resouce not enough")
		return
	end

	--扣除道具加祝福值
	if self:useMulItem(config.cost) then
		local bless = GetRandomExp(config.blessrate)
	 	local now = curbless + bless
	 	--祝福值满，升级
	 	if now >= config.bless then
	 		local nowLev = curlev + 1
	 		spellMgr:setDivinLevBless(idx,nowLev,now-config.bless)
			
			--重新设置神兵数量
			local num = spellMgr:getDivineNum()
			self:SetDivineNum(num)

	 		--激活对应的被动技能
	 		local slist = tb_divine_base[divineId].passiveskill
	 		--outFmtDebug("skill lengt",#slist)
	 		for _, skill in pairs(slist) do
	 			--outFmtDebug("passive skill %d,%d",skill[1],skill[2])
	 			if skill[2] == nowLev then
	 				--outFmtDebug("mingzhong passive skill %d",skill[1])
	 				self:onDivineActivedSpell(divineId,skill[1],true)
	 			end
	 		end
			-- 重算战斗力(当前和属性绑定在一起)
			self:RecalcAttrAndBattlePoint()
			
			self:AddAchieve(QUEST_TARGET_TYPE_DIVINE_LEV,1)
	 	else
	 		spellMgr:setDivinLevBless(idx,curlev,now)
	 	end
		
		-- 加任务
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_STRENGTH_DIVINE, {divineId})
	end

	
end

--铸造
function PlayerInfo:DivineForgeUpLev(divineId)
	local spellMgr = self:getSpellMgr()
	local index = spellMgr:GetDivineIndex(divineId)
	local curlev = spellMgr:GetDivineForgeLv(index)
	
	local config = tb_divine_forge[curlev + 1]
	if config == nil then
		outFmtError("divine forge is max lev already")
		return
	end
	
	--是否有足够的道具
	if not self:hasMulItem(config.cost) then
		outFmtError("divine forge up lev resouce not enough")
		return
	end
	
	if self:useMulItem(config.cost) then
		local nowLev = curlev + 1
	 	spellMgr:SetDivineForgeLv(index,nowLev)
		
		--提示前端
		
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		
	end
end

--升阶
function PlayerInfo:DivineAdvanceUpLev(divineId)
	local spellMgr = self:getSpellMgr()
	local index = spellMgr:GetDivineIndex(divineId)
	local curlev = spellMgr:GetDivineAdvanceLv(index)
	
	local config = tb_divine_advance[curlev + 1 + divineId * 100]
	if config == nil then
		outFmtError("divine advance is max lev already")
		return
	end
	
	--是否有足够的道具
	if not self:hasMulItem(config.cost) then
		outFmtError("divine advance up lev resouce not enough")
		return
	end
	
	if self:useMulItem(config.cost) then
		local nowLev = curlev + 1
	 	spellMgr:SetDivineAdvanceLv(index,nowLev)
		
		--提示前端
		
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		
	end
end

--铸魂
function PlayerInfo:DivineSpiritUpLev(divineId,protect,improve)
	local spellMgr = self:getSpellMgr()
	local index = spellMgr:GetDivineIndex(divineId)
	local curlev = spellMgr:GetDivineSpiritLv(index)
	
	local config = tb_divine_spirit[curlev + 1]
	if config == nil then
		outFmtError("divine spirit is max lev already")
		return
	end
	
	
	
	local cost = config.cost
	if protect == 1 then
		for _,v in pairs(config.protect_item) do
			table.insert(cost,v)
		end
	end
	
	if improve == 1 then
		for _,v in pairs(config.improve_item) do
			table.insert(cost,v)
		end
	end
	
	--是否有足够的道具
	if not self:hasMulItem(cost) then
		outFmtError("divine spirit up lev resouce not enough")
		return
	end
	
	if self:useMulItem(cost) then
		local chance = config.chance
		if improve == 1 then
			chance = chance + config.improve_chance
			if chance > 10000 then
				chance = 10000
			end
		end
		local random = randInt(1,10000)
		if random <= chance then
			--成功
			local nowLev = curlev + 1
			spellMgr:SetDivineSpiritLv(index,nowLev)
			--提示前端
		else
			--失败
			if config.fail_punish > 0 and protect == 0 then
				local nowLev = curlev - config.fail_punish
				spellMgr:SetDivineSpiritLv(index,nowLev)
				--提示前端失败并掉级
			else
				--提示前端失败
			end
		end
		
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		
	end
end

function PlayerInfo:getTalismanLevelIfNotReturnMaxLevelOfAllTalisman(id)
	local spellMgr = self:getSpellMgr()
	local dict = spellMgr:getTalismanList()
	
	local level = 0
	for k, v in pairs(dict) do
		if id == k then
			return v
		end
		if level < v then
			level = v
		end
	end
	
	return level
end


--法宝激活
function PlayerInfo:TalismanActive(id)
	local config = tb_talisman_base[id]
	if config == nil then
		outFmtDebug("talisman id: %d not exist",id)
		return
	end
	local spellMgr = self:getSpellMgr()
	
	if spellMgr:GetTalismanIndex(id) ~= 0 then
		outFmtDebug("talisman id: %d already active",id)
		return
	end
	
	if config.avtivetype == 1 then --材料激活
		if self:useMulItem(config.avtivedata) then
			spellMgr:AddTalisman(id)
			for _,v in pairs(config.passiveskill) do
				self:updatePassive(v[1], v[2])
			end
			spellMgr:calculTalismanForce(id)
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_TALISMAN_ACTIVE)
			self:RecalcAttrAndBattlePoint()
			
			--更新开服排行
			DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_TALISMAN,self})
		else 
			outFmtError("active talisman has not enough item");
			return
		end
	end
end
--法宝注灵
function PlayerInfo:TalismanLvUp(id)
	if tb_talisman_base[id] == nil then
		outFmtDebug("talisman id: %d not exist",id)
		return
	end
	local spellMgr = self:getSpellMgr()
	local index = spellMgr:GetTalismanIndex(id)
	if index == 0 then
		outFmtDebug("talisman id: %d not active",id)
		return
	end
	
	local curlev = spellMgr:GetTalismanLv(index)
	local config = tb_talisman_spirit[id * 1000 + curlev + 1]
	if config == nil then
		outFmtDebug("talisman id: %d curlev max",id)
		return
	end
	
	--消耗跟随法宝品质
	local money = config.money_cost[1]
	if not money then
		outFmtError("talisman up lev money_cost not exist")
		return
	end
	
	local tf,tab = self:checkMoneyEnoughIfUseGoldIngot({money})
	
	--是否有足够的道具
	local item = config.item_cost[1]
	if not item then
		outFmtError("talisman up lev item_cost not exist")
		return
	end

	if not tf or not self:hasMulItem({item}) then
		outFmtError("talisman up lev resouce not enough")
		return
	end
	
	if self:useMulItem({item}) and self:costMoneys(MONEY_CHANGE_SHENBING_BUY,tab) then
		local nowLev = curlev + 1
	 	spellMgr:SetTalismanLv(index,nowLev)
		spellMgr:calculTalismanForce(id)
		
		self:onAddMeridianExpSource(MERIDIAN_EXP_SOURCE_TALISMAN_ZHULING)
		
		--提示前端
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_TALISMAN_LVUP)
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		
		--更新开服排行
		DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_TALISMAN,self})
		
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_TALISMAN_LEVEL, {id,nowLev})
	end
	
end

--神羽激活
function PlayerInfo:WingsActive()
	local spellMgr = self:getSpellMgr()
	if spellMgr:GetWingsId() == 0 then
		spellMgr:SetWingsId(100) --初始阶数1 星级0
		
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_WINGS_ACTIVE)
		--重算战斗力
		self:RecalcAttrAndBattlePoint()
		
		--更新开服排行
		DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_WING,self})
		
		rankInsertTask(self:GetGuid(),RANK_TYPE_WINGS)
	end
end
--神羽祝福
function PlayerInfo:WingsBless()
	local spellMgr = self:getSpellMgr()
	local wings_id = spellMgr:GetWingsId()
	if wings_id == 0 then
		outFmtError("wings not active")
		return
	end
	local config = tb_wings_bless[wings_id]
	if not config then
		outFmtError("wings id:%d not exist",wings_id)
		return
	end
	if config.operate_type == 1 then
		local tf,tab = self:checkMoneyEnoughIfUseGoldIngot(config.money_cost)
		--是否有足够的道具
		if not tf or not self:hasMulItem(config.item_cost) then
			outFmtError("wings bless resouce not enough")
			return
		end
		
		if self:useMulItem(config.item_cost) and self:costMoneys(MONEY_CHANGE_SHENBING_BUY,tab) then
			local cur_exp = spellMgr:GetWingsBlessExp()
			outFmtDebug("wings cur exp:%d",cur_exp)
			if cur_exp + config.bless_exp >= config.need_exp then
				spellMgr:SetWingsBlessExp(0)
				spellMgr:SetWingsId(wings_id + 1)
				
				self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_WINGS_BLESS)
				-- 重算战斗力(当前和属性绑定在一起)
				self:RecalcAttrAndBattlePoint()
				
				--更新开服排行
				DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_WING,self})
				
				rankInsertTask(self:GetGuid(),RANK_TYPE_WINGS)
				self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_WINGS_UPDRADE_TIMES, {})
			else
				spellMgr:SetWingsBlessExp(cur_exp + config.bless_exp)
			end
		end
	else
		outFmtError("wings id:%d can not bless",wings_id)
		return
	end
end
--神羽升阶
function PlayerInfo:WingsRankUp()
	local spellMgr = self:getSpellMgr()
	local wings_id = spellMgr:GetWingsId()
	if wings_id == 0 then
		outFmtError("wings not active")
		return
	end
	local config = tb_wings_bless[wings_id]
	if not config then
		outFmtError("wings id:%d not exist",wings_id)
		return
	end
	if config.operate_type == 2 then
		local tf,tab = self:checkMoneyEnoughIfUseGoldIngot(config.money_cost)
		--是否有足够的道具
		if not tf or not self:hasMulItem(config.item_cost) then
			outFmtError("wings rank up resouce not enough")
			return
		end
		
		if self:useMulItem(config.item_cost) and self:costMoneys(MONEY_CHANGE_SHENBING_BUY,tab) then
			spellMgr:SetWingsId((config.rank + 1)*100)
			
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_WINGS_RANKUP)
			-- 重算战斗力(当前和属性绑定在一起)
			self:RecalcAttrAndBattlePoint()
			--更新开服排行
			DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_WING,self})
			
			rankInsertTask(self:GetGuid(),RANK_TYPE_WINGS)
			
			self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_WINGS_UPGRADE_LEVEL, {self:GetWingsUpgradeLevel()})
			
		end
	else
		outFmtError("wings id:%d can not rank up",wings_id)
		return
	end
end

--神羽强化
function PlayerInfo:WingsStrength()
	local spellMgr = self:getSpellMgr()
	local wings_id = spellMgr:GetWingsId()
	if wings_id == 0 then
		outFmtError("wings not active")
		return
	end
	if not tb_wings_bless[wings_id] then
		outFmtError("wings id:%d not exist",wings_id)
		return
	end
	
	local wings_level = spellMgr:GetWingsLevel()
	local config = tb_wings_strength[wings_level+1]
	
	if not config then
		outFmtError("wings level:%d can not strength more",wings_level)
		return
	end
	
	local tf,tab = self:checkMoneyEnoughIfUseGoldIngot(config.money_cost)
	--是否有足够的道具
	if not tf or not self:hasMulItem(config.item_cost) then
		outFmtError("wings strength resouce not enough")
		return
	end
	
	if self:useMulItem(config.item_cost) and self:costMoneys(MONEY_CHANGE_SHENBING_BUY,tab) then
		local random = randInt(1,10000)
		if random <= config.possibility then
			spellMgr:SetWingsLevel(wings_level + 1)
			outFmtInfo("wings strength success")
			
			self:onAddMeridianExpSource(MERIDIAN_EXP_SOURCE_WINGS_STRENGTH)
			
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_WINGS_STRENGTH_SUCESS)
			
			-- 重算战斗力(当前和属性绑定在一起)
			self:RecalcAttrAndBattlePoint()
			
			self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_WINGS_STRENGTH_LEVEL, {spellMgr:GetWingsLevel()})
			
		else
			outFmtInfo("wings strength fail")
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_WINGS_STRENGTH_FAIL)
		end
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_WINGS_STRENGTH_TIMES, {})
	end
	
end

function PlayerInfo:onAddMeridianExpSource(sourceId)
	local spellMgr = self:getSpellMgr()
	spellMgr:addMeridianExpSource(sourceId)
end

function PlayerInfo:onAddMeridianExpItem(id)
	local spellMgr = self:getSpellMgr()
	local config = tb_meridian_item[ id ]
	
	if not self:useMulItem({{config.itemId, 1}}) then
		return
	end
	
	spellMgr:addMeridianExp(config.exp)
	
	self:CallOptResult(OPRATE_TYPE_BAG,BAG_RESULT_BAG_XIULIAN_USE,{config.exp})
end


-- 设置经脉战力
function PlayerInfo:SetMeridianForce(val)
	self:SetUInt32(PLAYER_INT_FIELD_MERIDIAN_FORCE, val)
end


function PlayerInfo:meridianPractise()
	local spellMgr = self:getSpellMgr()
	local level = spellMgr:getMeridianLevel()
	local config = tb_meridian_info[level+1]
	
	-- 表示升级
	if not config then
		-- 已经满级了
		return
	end
	
	if #config.costMoney == 0 and config.costExp == 0 then
		return
	end
	
	if config.costExp > 0 then
		-- 活力不够
		if not spellMgr:costMeridianExp(config.costExp) then
			return
		end
	end
	
	if #config.costMoney > 0 then
		if not self:costMoneys(MONEY_CHANGE_MERIDIAN, config.costMoney) then
			return
		end
	end
	
	spellMgr:addMeridianLevel()
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
	
	--更新开服排行
	DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_MERIDIAN,self})
	
	self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_MERIDIAN_LEVEL, {spellMgr:getMeridianLevel()})
end


function PlayerInfo:OnResetMeridianDayTimes()
	local spellMgr = self:getSpellMgr()
	for i = SPELL_INT_FIELD_MERIDIAN_CNT_START, SPELL_INT_FIELD_MERIDIAN_CNT_END - 1 do
		spellMgr:SetUInt32(i, 0)
	end
end


------------------------------------装备养成开始----------------------------------------
--强化 (pos,count) 
function PlayerInfo:EquipDevelopStrength(pos,count)
	count = count or 1
	local spellMgr = self:getSpellMgr()
	local index = pos - 1
	local currLv = spellMgr:GetEquipDevelopStrengthLv(index)
	
	local playerLv = self:GetLevel()
	
	if currLv >= playerLv then
		outFmtDebug("EquipDevelopStrength reach player level %d can not lvup",currLv)
		return
	end
	
	local item_cost = {}
	local money_cost = {}
	local up_level = 0
	
	local temp_item = {}
	local temp_money = {}
	
	local tab = {}
		
	for i = 1,count do
		if currLv + i > playerLv then
			outFmtDebug("EquipDevelopStrength reach player level %d can not lvup",currLv + i)
			break
		end
		local config = tb_equipdevelop_strength[pos * 1000 + currLv + i]
		if not config then
			outFmtDebug("EquipDevelopStrength reach top level %d can not lvup",currLv + i - 1)
			break
		end
		
		local item_list = {}
		local money_list = {}
		
		for _,v in pairs(config.item_cost) do
			if temp_item[v[1]] then
				temp_item[v[1]] = temp_item[v[1]] + v[2]
			else
				temp_item[v[1]] = v[2]
			end
		end
		
		for _,v in pairs(config.money_cost) do
			if temp_money[v[1]] then
				temp_money[v[1]] = temp_money[v[1]] + v[2]
			else
				temp_money[v[1]] = v[2]
			end
		end
		
		for id,num in pairs(temp_item) do
			table.insert(item_list,{id,num})
		end
		for id,num in pairs(temp_money) do
			table.insert(money_list,{id,num})
		end
		
		
		local tf1,tab1 = self:checkMoneyEnoughIfUseGoldIngot(money_list)
	--是否有足够的道具
		local tf2 = self:hasMulItem(item_list)
		
		if tf1 and tf2 then
			up_level = i
			item_cost = clone(item_list)
			money_cost = clone(money_list)
			tab = clone(tab1)
		else
			outFmtError("EquipDevelopStrength strength resource not enough ,%s,%s",tf1,tf2)
			break
		end
		
	end
	
	if up_level == 0 then
		outFmtError("EquipDevelopStrength strength can not to lvup")
		return
	end
	
	if self:useMulItem(item_cost) and self:costMoneys(MONEY_CHANGE_EQUIPDEVELOP,tab) then
		spellMgr:SetEquipDevelopStrengthLv(index,currLv + up_level)
		outFmtInfo("EquipDevelopStrength strength success")
		
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_STRENGTH_SUCCESS)
		
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		
		--检测装备养成 奖励
		self:UpdateEquipDevelopStrengthBonus(currLv + up_level)
		--self:EquipDevelopGemActive(pos)
		
		--更新开服排行
		DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_STRENGTH,self})
		
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_STRENGTH_SUIT, {pos,up_level})
	end


end


--精炼升星 (pos)
function PlayerInfo:EquipDevelopRefineStarUp(pos)
	local spellMgr = self:getSpellMgr()
	local index = pos - 1
	local currRank = spellMgr:GetEquipDevelopRefineRank(index)
	local currStar = spellMgr:GetEquipDevelopRefineStar(index)
	
	
	local curr_config = tb_equipdevelop_refine[pos * 10000 + currRank * 100 + currStar]
	if not curr_config then
		outFmtDebug("EquipDevelopRefineStarUp curr_config not exist %d %d %d",pos,currRank,currStar)
		return
	end
	
	if curr_config.lvup_type ~= 1 then
		outFmtDebug("EquipDevelopRefineStarUp curr_config can not star up %d %d %d",pos,currRank,currStar)
		return
	end
	
	local next_config = tb_equipdevelop_refine[pos * 10000 + currRank * 100 + currStar + 1]
	if not next_config then
		outFmtDebug("EquipDevelopRefineStarUp next_config not exist %d %d %d",pos,currRank,currStar + 1)
		return
	end
	
	local tf1,tab = self:checkMoneyEnoughIfUseGoldIngot(next_config.money_cost)
	--是否有足够的道具
	local tf2 = self:hasMulItem(next_config.item_cost)
		
	if not tf1 or not tf2 then
		outFmtError("EquipDevelopRefineStarUp resouce not enough")
		return
	end
	
	if self:useMulItem(next_config.item_cost) and self:costMoneys(MONEY_CHANGE_EQUIPDEVELOP,tab)then
		local random = randInt(1,100)
		if random <= next_config.chance then
			spellMgr:SetEquipDevelopRefineStar(index,currStar + 1)
			outFmtInfo("EquipDevelopRefineStarUp refine success")
			
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_REFINE_STAR_SUCCESS)
			
			-- 重算战斗力(当前和属性绑定在一起)
			self:RecalcAttrAndBattlePoint()
			
			--检测装备养成 奖励
			
			--更新开服排行
			DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_DEFINE,self})
			
			
			
		else
			outFmtInfo("EquipDevelopRefineStarUp refine fail")
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_REFINE_STAR_FAIL)
		end
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_REFINE_SUIT, {pos})
	end
end

--精炼升阶 (pos)
function PlayerInfo:EquipDevelopRefineRankUp(pos)
	local spellMgr = self:getSpellMgr()
	local index = pos - 1
	local currRank = spellMgr:GetEquipDevelopRefineRank(index)
	local currStar = spellMgr:GetEquipDevelopRefineStar(index)
	
	
	local curr_config = tb_equipdevelop_refine[pos * 10000 + currRank * 100 + currStar]
	if not curr_config then
		outFmtDebug("EquipDevelopRefineRankUp curr_config not exist %d %d %d",pos,currRank,currStar)
		return
	end
	
	if curr_config.lvup_type ~= 2 then
		outFmtDebug("EquipDevelopRefineRankUp curr_config can not rank up %d %d %d",pos,currRank,currStar)
		return
	end
	
	local next_config = tb_equipdevelop_refine[pos * 10000 + (currRank + 1)* 100 + 0]
	if not next_config then
		outFmtDebug("EquipDevelopRefineRankUp next_config not exist %d %d %d",pos,currRank + 1,currStar)
		return
	end
	
	local tf1,tab = self:checkMoneyEnoughIfUseGoldIngot(next_config.money_cost)
	--是否有足够的道具
	local tf2 = self:hasMulItem(next_config.item_cost)
	
	if not tf1 or not tf2 then
		outFmtError("EquipDevelopRefineRankUp resouce not enough")
		return
	end
	
	if self:useMulItem(next_config.item_cost) and self:costMoneys(MONEY_CHANGE_EQUIPDEVELOP,tab)then
		local random = randInt(1,100)
		if random <= next_config.chance then
			spellMgr:SetEquipDevelopRefineStar(index,0)
			spellMgr:SetEquipDevelopRefineRank(index,currRank + 1)
			outFmtInfo("EquipDevelopRefineRankUp refine rank success")
			
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_REFINE_STAR_SUCCESS)
			
			-- 重算战斗力(当前和属性绑定在一起)
			self:RecalcAttrAndBattlePoint()
			
			--检测装备养成 奖励
			self:UpdateEquipDevelopRefineBonus(currRank + 1)
			--self:EquipDevelopGemActive(pos)
			
			--更新开服排行
			DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_DEFINE,self})
			
			
		else
			outFmtInfo("EquipDevelopRefineRankUp refine  rank fail")
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_REFINE_STAR_FAIL)
		end
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_REFINE_SUIT, {pos})
	end
end

--激活宝石 (pos,gem_pos)
function PlayerInfo:EquipDevelopGemActive(pos,gem_pos)
	local spellMgr = self:getSpellMgr()
	local index = pos - 1
	local part_config = tb_equipdevelop_gem_part[pos]

	local currLv = spellMgr:GetEquipDevelopGemLv(index,gem_pos - 1)
	if currLv ~= 0 then
		outFmtDebug("EquipDevelopGemActive gem lv ~= 0 can not active %d %d",pos,gem_pos)
		return
	end
	
	local gem_type = part_config.gem_array[gem_pos]
	local unlock_strength_lv = part_config.unlock_strength_lv[gem_pos]
	local unlock_refine_lv = part_config.unlock_refine_lv[gem_pos]
	
	if not gem_type or not unlock_strength_lv or not unlock_refine_lv then
		outFmtDebug("EquipDevelopGemActive gem part config error %d %d",pos,gem_pos)
		return
	end
	
	local strength_lv = spellMgr:GetEquipDevelopStrengthLv(index)
	local refine_rank = spellMgr:GetEquipDevelopRefineRank(index)
	--local refine_star = spellMgr:GetEquipDevelopRefineStar(index)
	
	if strength_lv >= unlock_strength_lv and refine_rank >= unlock_refine_lv then
		local next_config = tb_equipdevelop_gem[gem_type*1000+1]
		local tf1,tab = self:checkMoneyEnoughIfUseGoldIngot(next_config.money_cost)
		--是否有足够的道具
		local tf2 = self:hasMulItem(next_config.item_cost)
		
		if not tf1 or not tf2 then
			outFmtError("EquipDevelopGemActive resouce not enough")
			return
		end
		
		if self:useMulItem(next_config.item_cost) and self:costMoneys(MONEY_CHANGE_EQUIPDEVELOP,tab)then
			spellMgr:SetEquipDevelopGemLv(index,gem_pos-1,1)
			outFmtInfo("EquipDevelopGemActive gem active success")
			
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_GEM_ACTIVE_SUCCESS)
			
			-- 重算战斗力(当前和属性绑定在一起)
			self:RecalcAttrAndBattlePoint()
			
			--检测装备养成 奖励
			self:UpdateEquipDevelopGemBonus(1)
			--更新开服排行
			DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_GEM,self})
			
			local level = self:GetGemTotalLevel()
			self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_GEM_TOTAL_LEVEL, {level})
		end
	else
		outFmtError("EquipDevelopGemActive strength_lv refine_rank not enough")
	end
	
end

--升级宝石 (pos,gem_index)
function PlayerInfo:EquipDevelopGemLvUp(pos,gem_pos)
	local spellMgr = self:getSpellMgr()
	local index = pos - 1
	local currLv = spellMgr:GetEquipDevelopGemLv(index,gem_pos-1)
	if currLv == 0 then
		outFmtDebug("EquipDevelopGemLvUp gem lv == 0 can not lvup %d %d",pos,gem_pos)
		return
	end
	
	local gem_type = tb_equipdevelop_gem_part[pos].gem_array[gem_pos]
	local next_config = tb_equipdevelop_gem[gem_type*1000+currLv+1]
	if not next_config then
		outFmtDebug("EquipDevelopGemLvUp gem lv is max can not lvup %d %d",pos,gem_pos)
		return
	end
	
	local tf1,tab = self:checkMoneyEnoughIfUseGoldIngot(next_config.money_cost)
	--是否有足够的道具
	local tf2 = self:hasMulItem(next_config.item_cost)
	
	if not tf1 or not tf2 then
		outFmtError("EquipDevelopGemLvUp resouce not enough")
		return
	end
	
	if self:useMulItem(next_config.item_cost) and self:costMoneys(MONEY_CHANGE_EQUIPDEVELOP,tab)then
		
		spellMgr:SetEquipDevelopGemLv(index,gem_pos-1,currLv + 1)
		outFmtInfo("EquipDevelopGemLvUp gem lvup success")
		
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_GEM_LVUP_SUCCESS)
		
		-- 重算战斗力(当前和属性绑定在一起)
		self:RecalcAttrAndBattlePoint()
		
		--检测装备养成 奖励
		self:UpdateEquipDevelopGemBonus(currLv + 1)
		
		--更新开服排行
		DoActivitySystemDataUpdateByScriptId(ACT_RANK,{ACT_RANK_TYPE_GEM,self})
		
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_STRENGTH_GEM, {pos})
		
		local level = self:GetGemTotalLevel()
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_GEM_TOTAL_LEVEL, {level})
	end
end

--更新奖励等级 强化 ()
function PlayerInfo:UpdateEquipDevelopStrengthBonus(new_lv)
	local spellMgr = self:getSpellMgr()
	local min_lv = -1
	local curr_bonus_lv = spellMgr:GetEquipDevelopBonusStrengthLv()
	local curr_bonus_config = tb_equipdevelop_bonus[1*100+curr_bonus_lv]
	
	if curr_bonus_lv ~= 0 and curr_bonus_config and new_lv <= curr_bonus_config.need_lv[2] then
		outFmtInfo("UpdateEquipDevelopStrengthBonus bonus level not change")
		return
	end
	
	for index = 0,EQUIPMENT_COUNT - 1 do
		local level = spellMgr:GetEquipDevelopStrengthLv(index)
		if min_lv == -1 then
			min_lv = level
		else
			
			if level < min_lv then
				min_lv = level
			end
		end
		
	end
	
	if min_lv <= 0 then
		outFmtInfo("UpdateEquipDevelopStrengthBonus no bonus")
		return
	end
	
	for _,config in pairs(tb_equipdevelop_bonus) do
		if config.type == 1 then
			if min_lv >= config.need_lv[1] and min_lv <= config.need_lv[2] then
				if curr_bonus_lv < config.level then
					spellMgr:SetEquipDevelopBonusStrengthLv(config.level)
					outFmtInfo("UpdateEquipDevelopStrengthBonus bonus level change %d",config.level)
					self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_BONUS_STRENGTH)
					-- 重算战斗力(当前和属性绑定在一起)
					self:RecalcAttrAndBattlePoint()
					return
				else
					outFmtInfo("UpdateEquipDevelopStrengthBonus bonus level not change")
					return
				end
			end
		end
	end
end

--更新奖励等级 精炼 ()
function PlayerInfo:UpdateEquipDevelopRefineBonus(new_lv)
	local spellMgr = self:getSpellMgr()
	local min_lv = -1
	local curr_bonus_lv = spellMgr:GetEquipDevelopBonusRefineLv()
	local curr_bonus_config = tb_equipdevelop_bonus[2*100+curr_bonus_lv]
	
	if curr_bonus_lv ~= 0 and curr_bonus_config and new_lv <= curr_bonus_config.need_lv[2] then
		outFmtInfo("UpdateEquipDevelopRefineBonus bonus level not change")
		return
	end
	
	for index = 0,EQUIPMENT_COUNT - 1 do
		local level = spellMgr:GetEquipDevelopRefineRank(index)
		if min_lv == -1 then
			min_lv = level
		else
			
			if level < min_lv then
				min_lv = level
			end
		end
		
	end
	
	if min_lv <= 0 then
		outFmtInfo("UpdateEquipDevelopRefineBonus no bonus")
		return
	end
	
	for _,config in pairs(tb_equipdevelop_bonus) do
		if config.type == 2 then
			if min_lv >= config.need_lv[1] and min_lv <= config.need_lv[2] then
				if curr_bonus_lv < config.level then
					spellMgr:SetEquipDevelopBonusRefineLv(config.level)
					outFmtInfo("UpdateEquipDevelopRefineBonus bonus level change %d",config.level)
					self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_BONUS_REFINE)
					-- 重算战斗力(当前和属性绑定在一起)
					self:RecalcAttrAndBattlePoint()
					return
				else
					outFmtInfo("UpdateEquipDevelopRefineBonus bonus level not change")
					return
				end
			end
		end
	end
end

--更新奖励等级 镶嵌 () 等级总和
function PlayerInfo:UpdateEquipDevelopGemBonus(new_lv)
	local spellMgr = self:getSpellMgr()
	local count = 0
	local curr_bonus_lv = spellMgr:GetEquipDevelopBonusGemLv()
	local curr_bonus_config = tb_equipdevelop_bonus[3*100+curr_bonus_lv]
	
	--[[
	if curr_bonus_lv ~= 0 and curr_bonus_config and new_lv <= curr_bonus_config.need_lv[2] then
		outFmtInfo("UpdateEquipDevelopGemBonus bonus level not change")
		return
	end
	--]]
	
	count = self:GetGemTotalLevel()
	
	if count <= 0 then
		outFmtInfo("UpdateEquipDevelopGemBonus no bonus")
		return
	end
	
	for _,config in pairs(tb_equipdevelop_bonus) do
		if config.type == 3 then
			if count >= config.need_lv[1] and count <= config.need_lv[2] then
				if curr_bonus_lv < config.level then
					spellMgr:SetEquipDevelopBonusGemLv(config.level)
					outFmtInfo("UpdateEquipDevelopGemBonus bonus level change %d",config.level)
					self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_BONUS_GEM)
					-- 重算战斗力(当前和属性绑定在一起)
					self:RecalcAttrAndBattlePoint()
					return
				else
					outFmtInfo("UpdateEquipDevelopGemBonus bonus level not change")
					return
				end
			end
		end
	end
end

--选择装备进行洗炼 (equip_guid)
function PlayerInfo:EquipDevelopWashAttrsWash(equip_guid)
	local spellMgr = self:getSpellMgr()
	local itemMgr = self:getItemMgr()
	local item = itemMgr:getItemByGuid(equip_guid, BAG_TYPE_EQUIP)
	if not item then
		outFmtDebug("EquipDevelopWashAttrsWash item %s not exist",equip_guid)
		return
	end
	
	
	
	local entry = item:getEntry()
	local item_tempate = tb_item_template[entry]
	if not item_tempate then return end
	
	local washattrs_config = tb_equipdevelop_washattrs[item_tempate.pos * 100 + item_tempate.quality]
	
	if not washattrs_config then return end
	
	local tf1,tab = self:checkMoneyEnoughIfUseGoldIngot(washattrs_config.money_cost)
	--是否有足够的道具
	local tf2 = self:hasMulItem(washattrs_config.item_cost)
	
	if not tf1 or not tf2 then
		outFmtError("EquipDevelopGemLvUp resouce not enough")
		return
	end
	
	if self:useMulItem(washattrs_config.item_cost) and self:costMoneys(MONEY_CHANGE_EQUIPDEVELOP,tab)then
		local attr_config = item_tempate.forge_pro
		local attr_length = item_tempate.forge_pro_max[1]
		
		if attr_length < 1 then return end
		local ary = GetRandomIndexTable(#attr_config,attr_length)

		local info = equip_guid..'|'..#ary..'|'..item_tempate.pos
		for i = 1,#ary do
			local idx = ary[i]
			local attrInfo = attr_config[idx]
			local attrId = attrInfo[ 1 ]
			local a = attrInfo[ 2 ]
			local b = attrInfo[ 3 ]
			local val = randInt(a, b)
			local qua = GetAttrQuality(val, a, b)
			
			--outFmtDebug("idx%d",idx)
			--item:setAddAttr(self:getAddAttKey(,lev), )
			--item:addBaseAttr(attrId, val, qua);
			
			info = info..'|'..attrId..'|'..val..'|'..qua
		end
		outFmtInfo("+++++++++++++++++++++++ %s",info)
		spellMgr:SetEquipDevelopWashAttrsInfo(info)
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_WASHATTRS_WASH)
		
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_WASH_SUIT, {item_tempate.pos})
	end
	
end

--保存洗炼结果覆盖装备 (equip_guid)
function PlayerInfo:EquipDevelopWashAttrsSave(equip_guid)
	local spellMgr = self:getSpellMgr()
	local itemMgr = self:getItemMgr()
	local item = itemMgr:getItemByGuid(equip_guid, BAG_TYPE_EQUIP)
	if not item then
		outFmtDebug("EquipDevelopWashAttrsSave item %s not exist",equip_guid)
		return
	end
	
	local info = spellMgr:GetEquipDevelopWashAttrsInfo()
	local tokens = string.split(info, "|")
	if #tokens == 1 then
		outFmtDebug("EquipDevelopWashAttrsSave item not washed")
		return
	end
	
	if tokens[1] ~= equip_guid then
		outFmtDebug("EquipDevelopWashAttrsSave item not same equip cannot save %s  %s",tokens[1],equip_guid)
		return
	end
	
	itemMgr:clearBaseAtrr(item)
	local count = tonumber(tokens[2])
	
	for i = 0,count - 1 do
		local attrId = tonumber(tokens[4 + i * 3])
		local val = tonumber(tokens[5 + i * 3])
		local qua = tonumber(tokens[6 + i * 3])
		
		if attrId and val and qua then
			item:addBaseAttr(attrId, val, qua)
			outFmtInfo("EquipDevelopWashAttrsSave item attrs add %d %d %d ",attrId,val,qua)
		end
	end
	itemMgr:resetItemForce(item)
	
	spellMgr:SetEquipDevelopWashAttrsInfo("")
	outFmtInfo("EquipDevelopWashAttrsSave item attrs changed ")
	self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_EQUIPDEVELOP_WASHATTRS_SAVE)
	-- 重算战斗力(当前和属性绑定在一起)
	self:RecalcAttrAndBattlePoint()
end

function PlayerInfo:EquipDevelopWashAttrsDel()
	local spellMgr = self:getSpellMgr()
	spellMgr:SetEquipDevelopWashAttrsInfo("")
	outFmtInfo("EquipDevelopWashAttrsSave item attrs give up ")
end

------------------------------------装备养成结束----------------------------------------

function PlayerInfo:SetAppearance(id)
	self:SetUInt16(PLAYER_INT_FIELD_APPEARANCE, tb_appearance_info[ id ].type, id)
	self:UpdateFactionBangZhuInfo()
end

function PlayerInfo:UnsetAppearance(type)
	self:SetUInt16(PLAYER_INT_FIELD_APPEARANCE, type, 0)
	self:UpdateFactionBangZhuInfo()
end

function PlayerInfo:GetAppearance(type)
	return self:GetUInt16(PLAYER_INT_FIELD_APPEARANCE, type)
end

function PlayerInfo:SetAppearanceWeaponForce(force)
	self:SetUInt32(PLAYER_INT_FIELD_WEAPON_FORCE, force)
end

function PlayerInfo:SetAppearanceClothForce(force)
	self:SetUInt32(PLAYER_INT_FIELD_CLOTH_FORCE, force)
end

function PlayerInfo:SetAppearancePokedexForce(force)
	self:SetUInt32(PLAYER_INT_FIELD_POKEDEX_FORCE, force)
end

--[[
-- 发送到场景服替换主动技能信息
function PlayerInfo:Send2ScenedIllusion(illuId)
	local pkt = Packet.new(INTERNAL_OPT_ILLUSION)
	pkt:writeUTF(self:GetGuid())
	pkt:writeU16(illuId)
	app:sendToConnection(self:GetScenedFD(), pkt)
	pkt:delete()
end
]]