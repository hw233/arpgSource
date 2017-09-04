-- 升级基础(包括)技能
function PlayerInfo:Handle_Raise_BaseSpell(pkt)
	local spellId  = pkt.spellId
	local raiseType = pkt.raiseType
	local config   = tb_skill_base[spellId]
	if not config then
		return
	end
	local spellMgr = self:getSpellMgr()
	
	if raiseType == RAISE_BASE_SKILL then
		-- 系统未激活
		if (not self:GetOpenMenuFlag(MODULE_SPELL, MODULE_SPELL_ALL)) then
			return
		end
	elseif raiseType == RAISE_MOUNT_SKILL then
		-- 系统未激活
		if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_SKILL)) then
			return
		end
	else
		return
	end
	
	-- 判断技能是否存在
	if not self:isSpellExist(spellId) then
		outFmtError("spellId %d not exist", spellId)
		return
	end
	
	-- 判断玩家是否拥有这个技能
	if not self:hasSpell(spellId) then
		outFmtError("player has no spellId %d", spellId)
		return
	end
	
	-- 判断是否满级了
	if self:isTopLevel(spellId) then
		outFmtError("spellId %d is in topLevel", spellId)
		return
	end
	
--[[	-- 判断是否是基础(怒气)技能
	if not self:isBaseSkill(spellId) and not self:isAngerSpell(spellId) then
		outFmtError("spellId %d is cannot do in this way", spellId)
		return
	end--]]
	
	-- 判断人物等级
	local index = spellMgr:getSpellUpgradeIndex(spellId)
	local upLevelConfig = tb_skill_uplevel[index]
	if not self:checkPlayerLevel(upLevelConfig.need_level) then
		outFmtError("spellId %d, player level not enough, need level = %d", spellId, upLevelConfig.need_level)
		return
	end
	
	-- 判断消耗道具
	if #upLevelConfig.uplevel_item > 0 then
		if not self:hasMulItem(upLevelConfig.uplevel_item) then
			outFmtError("item not enough")
			return
		end
	end
	
	-- 判断消耗资源
	if #upLevelConfig.uplevel_cost > 0 then
		if not self:checkMoneyEnoughs(upLevelConfig.uplevel_cost) then
			outFmtError("resouce not enough")
			return
		end
	end
	
	self:DoHandleRaiseSpell(raiseType, spellId)
end

-- 怒气技能进阶
function PlayerInfo:Handle_Upgrade_AngleSpell(pkt)
	--[[
	local spellId  = pkt.spellId
	local config   = tb_skill_base[spellId]
	local spellMgr = self:getSpellMgr()
	
	-- 判断技能是否存在
	if not self:isSpellExist(spellId) then
		outFmtError("spellId %d not exist", spellId)
		return
	end
	
	-- 判断玩家是否拥有这个技能
	if not self:hasSpell(spellId) then
		outFmtError("player has no spellId %d", spellId)
		return
	end
	
	-- 判断是否满级了
	if not self:isTopLevel(spellId) then
		outFmtError("spellId %d is not in topLevel, cannot upgrade", spellId)
		return
	end
	
	-- 判断是否是怒气技能
	if not self:isAngerSpell(spellId) then
		outFmtError("spellId %d is not anger spell", spellId)
		return
	end
	
	-- 判断金币是否充足
	local upgradeConfig = tb_assistangerspell_upgrade[spellId]
	local cost = upgradeConfig.cost
	
	if not self:costMoneys(MONEY_CHANGE_SPELL_UP, cost) then
		outFmtError("resouce %d not enough", MONEY_TYPE_SILVER)
		return
	end
	
	self:DoHandleUpgradeAngleSpell(spellId)
	--]]
end

-- 申请升星坐骑
function PlayerInfo:Handle_Raise_Mount(pkt)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_UPGRADE)) then
		return
	end	
	
	local spellMgr = self:getSpellMgr()
	-- 当前是否有坐骑
	if not spellMgr:hasMount() then
		outFmtError("player not active mount")
		return
	end
	
	-- 是否可以再升(已到达10星)
	if not spellMgr:canRaise() then
		outFmtError("player cannot raise star because of full stars")
		return
	end
	
	local level = spellMgr:getMountLevel()
	local config = tb_mount_base[level]
	
	local star  = spellMgr:getMountStar()
	local trainExp = spellMgr:getTrainExp()
	
	local seq = (level - 1) * 11 + star + 1
	local trainConfig = tb_mount_train[seq]
	
	-- 扣资源
	if not self:useAllItems(MONEY_CHANGE_RAISE_MOUNT, trainConfig.traincost) then
		outFmtError("resouce not enough")
		return
	end
	
	self:DoHandleRaiseMount()
end

-- 申请升阶坐骑
function PlayerInfo:Handle_Upgrade_Mount(pkt)
	local spellMgr = self:getSpellMgr()
	local useItem = pkt.useItem
	
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_UPGRADE)) then
		return
	end
	
	-- 当前是否有坐骑
	if not spellMgr:hasMount() then
		outFmtError("player not active mount")
		return
	end
	
	-- 是否可以升阶
	if not spellMgr:canUpgrade() then
		outFmtError("player cannot upgrade")
		return
	end
	
	local level = spellMgr:getMountLevel()
	local upgradeConfig = tb_mount_upgrade[level]
	local cost = upgradeConfig.upgradecost
	
	-- 如果扣除物品失败, 返回
--	if useItem == 0 then
	if not self:useAllItems(MONEY_CHANGE_RAISE_MOUNT, cost) then
		outFmtError("player has not enough item")
		return
	end
--[[	else
		if not self:useMulItemIfCostMoneyEnabled(cost) then
			outFmtError("ingots not enough")
			return
		end
	end--]]
	
	self:DoHandleUpgradeMount()
end

-- 一键进阶
function PlayerInfo:Handle_Upgrade_Mount_One_Step(pkt)
	--[[
	local useItem = pkt.useItem
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_UPGRADE)) then
		return
	end	
	
	local spellMgr = self:getSpellMgr()
	-- 当前是否有坐骑
	if not spellMgr:hasMount() then
		outFmtError("player not active mount")
		return
	end
	
	-- 是否可以升阶
	if not spellMgr:canUpgrade() then
		outFmtError("player cannot upgrade")
		return
	end
	
	self:DoHandleUpgradeMountOneStep(useItem)
	--]]
end

ILLUSION_EXPIRE_ACTIVE = 3		--有实现的激活

-- 申请解锁幻化坐骑
function PlayerInfo:Handle_Illusion_Active(pkt)
	local illuId = pkt.illuId
	local spellMgr = self:getSpellMgr()
	
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_ILLUSION)) then
		return
	end	
	
	-- 当前是否有坐骑
	if not spellMgr:hasMount() then
		outFmtError("player not active mount")
		return
	end
	
	-- 幻化存在
	if tb_mount_illusion[illuId] == nil then
		outFmtError("illusion id = %d is not exist", illuId)
		return
	end
	
	local config = tb_mount_illusion[illuId]
	if config.condition == ILLUSION_EXPIRE_ACTIVE then
		outFmtError("Handle_Illusion_Active illusion id = %d cannot in this way", illuId)
		return
	end
	
	-- 幻化是否存在
	if spellMgr:hasIllusion(illuId) then
		outFmtError("player already has illusion id = %d", illuId)
		return
	end
	
	local level = spellMgr:getMountLevel()
	if level < tb_mount_illusion[illuId].mountLevel then
		outFmtError("Handle_Illusion_Active illusion id = %d need level = %d", illuId, tb_mount_illusion[illuId].mountLevel)
		return
	end
	
	self:onActiveIllusion(illuId)
end

-- 申请幻化坐骑
function PlayerInfo:Handle_Illusion(pkt)
	local illuId = pkt.illuId
	local spellMgr = self:getSpellMgr()
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_ILLUSION)) then
		return
	end	
	
	-- 当前是否有坐骑
	if not spellMgr:hasMount() then
		outFmtError("player not active mount")
		return
	end
	
	-- 幻化是否存在
	if not spellMgr:hasIllusion(illuId) then
		outFmtError("player has no illusion id = %d", illuId)
		return
	end
	
	self:DoHandleIllusion(illuId)
end


function PlayerInfo:Handle_Raise_Mount_Level_Base(pkt)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_LEVEL)) then
		return
	end

	self:raiseMountLevelBase()
end


-- 激活神兵
function PlayerInfo:Handle_Divine_Active(pkt)
	local id = pkt.id
	--outFmtInfo("divine active id %d",id)
	if tb_divine_base[id] == nil then
		outFmtError("table has no divine id = %d", id)
		return
	end

	local spellMgr = self:getSpellMgr()

	if spellMgr:hasDivine(id) then
		outFmtError("player has already active divine id = %d", id)
		return
	end

	if self:DivineActive(id) then
		-- 加任务
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_OWN_DIVINE, {id})
	end
end

-- 升级神兵
function PlayerInfo:Handle_Divine_UpLev(pkt)
	local id = pkt.id
	if tb_divine_base[id] == nil then
		outFmtError("table has no divine id = %d", id)
		return
	end
	local spellMgr = self:getSpellMgr()

	if not spellMgr:hasDivine(id) then
		outFmtError("player has not already active divine id = %d", id)
		return
	end

	self:DivineUpLev(id)
end
--切换神兵
function PlayerInfo:Handle_Divine_Switch(pkt)
	local id = pkt.id
	if tb_divine_base[id] == nil then
		outFmtError("table has no divine id = %d", id)
		return
	end
	local spellMgr = self:getSpellMgr()

	if not spellMgr:hasDivine(id) then
		outFmtError("swich divine - player has not already active divine id = %d", id)
		return
	end

	local prev = self:GetUInt32(PLAYER_INT_FIELD_DIVINE_ID)
	if prev == id then
		self:switchDivine(0)
	else 
		self:switchDivine(id)
	end

	
end

--神兵强化
function PlayerInfo:Handle_Divine_Forge(pkt)
	local id = pkt.id
	local count = pkt.count
	if tb_divine_base[id] == nil then
		outFmtError("table has no divine id = %d", id)
		return
	end
	local spellMgr = self:getSpellMgr()
	if not spellMgr:hasDivine(id) then
		outFmtError("forge divine - player has not already active divine id = %d", id)
		return
	end
	
	if count == 1 then
		self:DivineForgeUpLev(id)
	elseif count == 5 then
		for i = 1,5 do
			self:DivineForgeUpLev(id)
		end
	end
end

--神兵升阶
function PlayerInfo:Handle_Divine_Advance(pkt)
	local id = pkt.id
	if tb_divine_base[id] == nil then
		outFmtError("table has no divine id = %d", id)
		return
	end
	local spellMgr = self:getSpellMgr()
	if not spellMgr:hasDivine(id) then
		outFmtError("advance divine - player has not already active divine id = %d", id)
		return
	end
	
	self:DivineAdvanceUpLev(id)
end

--神兵铸魂
function PlayerInfo:Handle_Divine_Spirit(pkt)
	local id = pkt.id
	local protect = pkt.protect
	local improve = pkt.improve
	if tb_divine_base[id] == nil then
		outFmtError("table has no divine id = %d", id)
		return
	end
	local spellMgr = self:getSpellMgr()
	if not spellMgr:hasDivine(id) then
		outFmtError("spirit divine - player has not already active divine id = %d", id)
		return
	end
	
	self:DivineSpiritUpLev(id,protect,improve)
end

--法宝激活
function PlayerInfo:Handle_Talisman_Active(pkt)
	local id = pkt.id
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_DIVINE, MODULE_DIVINE_ALL)) then
		return
	end
	
	if tb_talisman_base[id] == nil then
		outFmtError("table has no talisman id = %d", id)
		return
	end
	local spellMgr = self:getSpellMgr()
	if spellMgr:hasTalisman(id) then
		outFmtError("active talisman - player has already active talisman id = %d", id)
		return
	end
	
	self:TalismanActive(id)
end

--法宝注灵
function PlayerInfo:Handle_Talisman_Lvup(pkt)
	local id = pkt.id
	if tb_talisman_base[id] == nil then
		outFmtError("table has no talisman id = %d", id)
		return
	end
	local spellMgr = self:getSpellMgr()
	if not spellMgr:hasTalisman(id) then
		outFmtError("lvup talisman - player has not already active talisman id = %d", id)
		return
	end
	
	self:TalismanLvUp(id)
end

--神羽激活
function PlayerInfo:Handle_Wings_Active(pkt)
	self:WingsActive()
end

--神羽祝福
function PlayerInfo:Handle_Wings_Bless(pkt)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_WING, MODULE_WING_UPGRADE)) then
		return
	end
	self:WingsBless()
end

--神羽升阶
function PlayerInfo:Handle_Wings_RankUp(pkt)
	self:WingsRankUp()
end

--神羽强化
function PlayerInfo:Handle_Wings_Strength(pkt)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_WING, MODULE_WING_STRENGTH)) then
		return
	end
	self:WingsStrength()
end

function PlayerInfo:Handle_Meridian_Practise(pkt)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_TCM, MODULE_TCM_ALL)) then
		return
	end
	self:meridianPractise()
end

function PlayerInfo:Handle_Add_Meridian_Exp(pkt)
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_TCM, MODULE_TCM_ALL)) then
		return
	end
	local id = pkt.id

	if not tb_meridian_item[ id ] then
		return
	end

	self:onAddMeridianExpItem(id)
end

function PlayerInfo:Handle_Equipdevelop_Operate(pkt)
	local opt_type = pkt.opt_type			--操作类型
	local reserve_int1 = pkt.reserve_int1   --预留int值1*/
	local reserve_int2 = pkt.reserve_int2   --预留int值2*/
	local reserve_str1 = pkt.reserve_str1   --预留string值1*/
	local reserve_str2 = pkt.reserve_str2   --预留string值2*/
	if opt_type == EQUIPDEVELOP_TYPE_STRENGTH_LVUP then
		-- 系统未激活
		if (not self:GetOpenMenuFlag(MODULE_MIX, MODULE_MIX_STRENGTH)) then
			return
		end
		if reserve_int1 < 1 or reserve_int1 > 10 or reserve_int2 <= 0 then
			return
		end
		
		self:EquipDevelopStrength(reserve_int1,reserve_int2)
	elseif opt_type == EQUIPDEVELOP_TYPE_REFINE_STAR_LVUP then
		-- 系统未激活
		if (not self:GetOpenMenuFlag(MODULE_MIX, MODULE_MIX_POLISHED)) then
			return
		end
		if reserve_int1 < 1 or reserve_int1 > 10 then
			return
		end
		
		self:EquipDevelopRefineStarUp(reserve_int1)
	elseif opt_type == EQUIPDEVELOP_TYPE_REFINE_RANK_LVUP then
		if reserve_int1 < 1 or reserve_int1 > 10 then
			return
		end
		self:EquipDevelopRefineRankUp(reserve_int1)
	elseif opt_type == EQUIPDEVELOP_TYPE_GEM_ACTIVE then
		-- 系统未激活
		if (not self:GetOpenMenuFlag(MODULE_MIX, MODULE_MIX_GEM)) then
			return
		end
		if reserve_int1 < 1 or reserve_int1 > 10 then
			return
		end
		if not tb_equipdevelop_gem_part[reserve_int1] or not tb_equipdevelop_gem_part[reserve_int1].gem_array[reserve_int2] then
			return
		end
		self:EquipDevelopGemActive(reserve_int1,reserve_int2)
	elseif opt_type == EQUIPDEVELOP_TYPE_GEM_LVUP then
		if reserve_int1 < 1 or reserve_int1 > 10 then
			return
		end
		if not tb_equipdevelop_gem_part[reserve_int1] or not tb_equipdevelop_gem_part[reserve_int1].gem_array[reserve_int2] then
			return
		end
		self:EquipDevelopGemLvUp(reserve_int1,reserve_int2)
	elseif opt_type == EQUIPDEVELOP_TYPE_WASHATTRS_WASH then
		-- 系统未激活
		if (not self:GetOpenMenuFlag(MODULE_MIX, MODULE_MIX_WASH)) then
			return
		end
		self:EquipDevelopWashAttrsWash(reserve_str1)
	elseif opt_type == EQUIPDEVELOP_TYPE_WASHATTRS_SAVE then
		self:EquipDevelopWashAttrsSave(reserve_str1)
	elseif opt_type == EQUIPDEVELOP_TYPE_WASHATTRS_DEL then
		self:EquipDevelopWashAttrsDel()
	end
end

function PlayerInfo:Handle_Active_Mount(pkt)
	local spellMgr = self:getSpellMgr()
	-- 系统未激活 或者 坐骑已经激活
	if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_UPGRADE)) or spellMgr:getMountLevel() > 0 then
		return
	end
	
	self:activeMount()
end

function PlayerInfo:Handle_Active_Appearance(pkt)
	local id = pkt.id


	-- 外观不存在
	if not tb_appearance_info[ id ] then
		return
	end
	
	local modules = MODULE_FASHION_WEAPON
	if tb_appearance_info[ id ].type == 1 then
		modules = MODULE_FASHION_CLOTHES
	end
	
	-- 系统未激活
	if (not self:GetOpenMenuFlag(MODULE_FASHION, modules)) then
		return
	end
	
	-- 是否匹配职业
	if not table.find(tb_appearance_info[ id ].genders, self:GetGender()) then
		return
	end
	
	local spellMgr = self:getSpellMgr()
	
	-- 判断重复激活
	local dict = spellMgr:GetActiveAppearance()
	if dict[id] then
		return
	end
	
	local costs = tb_appearance_info[ id ].costs
	if not self:useAllItems(MONEY_CHANGE_APPEARANCE, costs) then
		return
	end

	spellMgr:activeAppearance(id)
	if tb_appearance_info[ id ].type == 0 then
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_WEAPON_ILLUSION_ACTIVE)
	elseif tb_appearance_info[ id ].type == 1 then
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_CLOTH_ILLUSION_ACTIVE)
	end
	
	self:RecalcAttrAndBattlePoint()
end

function PlayerInfo:Handle_Equip_Appearance(pkt)
	local id = pkt.id
	--[[
	if (not self:GetOpenMenuFlag(MODULE_MOUNT, MODULE_MOUNT_UPGRADE)) or spellMgr:getMountLevel() > 0 then
		return
	end
	--]]
	
	-- 外观不存在
	if not tb_appearance_info[ id ] then
		return
	end
	
	local spellMgr = self:getSpellMgr()
	
	-- 判断是否存在
	local dict = spellMgr:GetActiveAppearance()
	if not dict[id] then
		return
	end
	
	self:SetAppearance(id)
	self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_FASHION_ILLUSION_SET)
	self:RecalcAttrAndBattlePoint()
end

function PlayerInfo:Handle_Cancel_Equip_Appearance(pkt)
	local type = pkt.type
	if type ~= 0 and type ~= 1 then
		return
	end
	self:UnsetAppearance(type)
	self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_FASHION_ILLUSION_UNSET)
	self:RecalcAttrAndBattlePoint()
end