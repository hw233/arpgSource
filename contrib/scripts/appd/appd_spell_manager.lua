local AppSpellMgr = class("AppSpellMgr", BinLogObject)

function AppSpellMgr:ctor()
	
end

----------------------------------------基础技能部分------------------------------------

-- 升级技能分发
function AppSpellMgr:onRaiseSpell(raiseType, spellId)
	if raiseType == RAISE_BASE_SKILL then
		self:raiseBaseSpell(spellId)
	elseif raiseType == RAISE_MOUNT_SKILL then
		self:raiseMountSpell(spellId)
	elseif raiseType == RAISE_ILLUSION_SKILL then
		self:raiseIlluSpell(spellId)
	elseif raiseType == RAISE_DIVINE_SKILL then
		self:DivineSkillUpLev(spellId)
	end
end

-- 激活基础技能
function AppSpellMgr:activeBaseSpell(spellId)
	-- 激活
	local count = self:GetUInt32(SPELL_BASE_COUNT)
	local index = SPELL_INT_FIELD_BASE_SPELL_START + count
	self:SetUInt16(index, SHORT_SPELL_ID, spellId)
	self:SetUInt16(index, SHORT_SPELL_LV,       1)
	self:SetUInt32(SPELL_BASE_COUNT, count + 1)
end

-- 升级基础技能
function AppSpellMgr:raiseBaseSpell(spellId)
	local prev = self:getSpellLevel(spellId)
	
	-- 加技能等级
	self:SetBaseSpellLevel(spellId,  prev+1)
	
	-- 如果是3连击
	local arry = tb_skill_base[spellId].follow
	for _, id in pairs(arry) do
		self:SetBaseSpellLevel(id,  prev+1)
	end
end

--判断玩家是否拥有这个技能
function AppSpellMgr:getSpellLevel(spellId)
	local owner = self:getOwner()
	local level = playerLib.GetSpellLevel(owner.ptr, spellId)
	
	return level
end

--技能最大等级
function AppSpellMgr:getSpellTopLevel(spellId)
	local config = tb_skill_base[spellId]
	return config.uplevel_id[ 2 ] - config.uplevel_id[ 1 ] + 1
end

--当前升级id
function AppSpellMgr:getSpellUpgradeIndex(spellId)
	local config = tb_skill_base[spellId]
	local level = self:getSpellLevel(spellId)
	return config.uplevel_id[ 1 ] + level - 1
end

--设置技能等级
function AppSpellMgr:SetBaseSpellLevel(spellId, spellLv)
	for i = SPELL_INT_FIELD_BASE_SPELL_START, SPELL_INT_FIELD_BASE_SPELL_END-1 do
		if self:GetUInt16(i, SHORT_SPELL_ID) == spellId then
			self:SetUInt16(i, SHORT_SPELL_LV, spellLv)
			break
		end
	end
	
	-- 同步映射关系
	self:SetSpellLevel(spellId, spellLv)
end

-- 设置映射技能等级
function AppSpellMgr:SetSpellLevel(spellId, spellLv)
	local owner = self:getOwner()
	playerLib.SetSpellLevel(owner.ptr, spellId, spellLv)
end

----------------------------------------基础技能部分 结束------------------------------------


----------------------------------------------坐骑-----------------------------------------------
-- 玩家是否有坐骑
function AppSpellMgr:hasMount()
	return self:getMountLevel() > 0
end

-- 当前阶数
function AppSpellMgr:getMountLevel()
	return self:GetUInt16(SPELL_INT_FIELD_MOUNT_LEVEL, 0)
end

-- 设置当前阶数
function AppSpellMgr:setMountLevel(level)
	self:SetUInt16(SPELL_INT_FIELD_MOUNT_LEVEL, 0, level)
end

-- 玩家能否升星
function AppSpellMgr:canRaise()
	return self:getMountStar() < 10
end

function AppSpellMgr:getMountLevelBase()
	return self:GetUInt32(SPELL_INT_FIELD_MOUNT_LEVEL_BASE)
end

function AppSpellMgr:addMountLevelBase()
	self:AddUInt32(SPELL_INT_FIELD_MOUNT_LEVEL_BASE, 1)
	local playerInfo = self:getOwner()
	playerInfo:onUpdatePlayerQuest(QUEST_TARGET_TYPE_MOUNT_STRENGTH_LEVEL, {self:GetUInt32(SPELL_INT_FIELD_MOUNT_LEVEL_BASE)})
end

-- 玩家能否升阶
function AppSpellMgr:canUpgrade()
	return self:getMountLevel() < #tb_mount_upgrade and self:getMountStar() == 10
end

-- 当前星级
function AppSpellMgr:getMountStar()
	return self:GetUInt16(SPELL_INT_FIELD_MOUNT_LEVEL, 1)
end

-- 设置当前星级
function AppSpellMgr:setMountStar(star)
	self:SetUInt16(SPELL_INT_FIELD_MOUNT_LEVEL, 1, star)
end

-- 当前升星经验
function AppSpellMgr:getTrainExp()
	return self:GetUInt32(SPELL_INT_FIELD_MOUNT_TRAIN_EXP)
end

-- 设置当前升星经验
function AppSpellMgr:setTrainExp(value)
	self:SetUInt32(SPELL_INT_FIELD_MOUNT_TRAIN_EXP, value)
end

-- 增加当前升星经验
function AppSpellMgr:addTrainExp(value)
	local prev = self:getTrainExp()
	self:SetUInt32(SPELL_INT_FIELD_MOUNT_TRAIN_EXP, value + prev)
end

-- 获得祝福值
function AppSpellMgr:getBlessExp()
	return self:GetUInt32(SPELL_INT_FIELD_MOUNT_BLESS_EXP)
end

-- 设置祝福值
function AppSpellMgr:setBlessExp(value)
	self:SetUInt32(SPELL_INT_FIELD_MOUNT_BLESS_EXP, value)
end

-- 激活坐骑技能
function AppSpellMgr:activeMountSpell(spellId)
	for i = SPELL_INT_FIELD_MOUNT_SPELL_START, SPELL_INT_FIELD_MOUNT_SPELL_END-1 do
		if self:GetUInt16(i, SHORT_SPELL_ID) == 0 then
			self:SetUInt16(i, SHORT_SPELL_ID, spellId)
			self:SetUInt16(i, SHORT_SPELL_LV, 1)
			break
		end
	end
end

--坐骑属性加成
function AppSpellMgr:calculMountAttr(attrs)
	local player = self:getOwner()
	
	local allForce = 0
	local level = self:getMountLevel()
	local star  = self:getMountStar()
	local seq = (level - 1) * 11 + star + 1
	local trainConfig = tb_mount_train[seq]
	
	local mountAttr = {}
	-- 升阶
	if trainConfig then
		local upgradeAttrs = MergeAttrKeysAndValues(trainConfig.prosKeys, trainConfig.prosValues)
		mergeAttrs(mountAttr, upgradeAttrs)
	end
	
	-- 升级
	local mountLevel = self:getMountLevelBase()
	local levelConfig = tb_mount_raise_level[mountLevel]
	if levelConfig then
		local levelAttrs = MergeAttrKeysAndValues(levelConfig.prosKeys, levelConfig.prosValues)
		mergeAttrs(mountAttr, levelAttrs)
	end
		
	-- 坐骑进阶技能战力
	local nonForce = 0
	for i = SPELL_INT_FIELD_MOUNT_SPELL_START, SPELL_INT_FIELD_MOUNT_SPELL_END - 1 do
		local spellID	= self:GetUInt16(i, 0)
		local lv 		= self:GetUInt16(i, 1)
		if spellID > 0 then
			local bp = player:GetSkillBattlePoint(spellID, lv)
			allForce = allForce + bp
			nonForce = nonForce + bp
		end
	end
	
	-- 幻化
	for i = SPELL_INT_FIELD_MOUNT_ILLUSION_START, SPELL_INT_FIELD_MOUNT_ILLUSION_END, MAX_ILLUSION_ATTR_COUNT do
		local illusionId = self:GetUInt32(i+ILLUSION_ATTR_ID)
		if illusionId > 0 then
			local illuConfig = tb_mount_illusion[illusionId]
			if illuConfig then
				local illuAttrs = MergeAttrKeysAndValues(illuConfig.prosKeys, illuConfig.prosValues)
				mergeAttrs(mountAttr, illuAttrs)
			end
			
			-- 坐骑幻化技能战力
			for j = ILLUSION_ATTR_SPELL_START + i, ILLUSION_ATTR_SPELL_END + i - 1 do
				local spellID	= self:GetUInt16(j, 0)
				local lv		= self:GetUInt16(j, 1)
				if spellID > 0 then
					local bp = player:GetSkillBattlePoint(spellID, lv)
					nonForce = nonForce + bp
					allForce = allForce + bp
				end
			end
		end
	end
	
	-- 计算总的属性
	local baseForce = DoAnyOneCalcForce(mountAttr, player:GetGender())
	allForce = allForce + baseForce
	-- 属性
	for indx, val in pairs(mountAttr)do
		-- 速度属性就不在这里计算了
		if indx ~= EQUIP_ATTR_MOVE_SPEED then
			if attrs[indx] == nil then
				attrs[indx] = 0
			end
			attrs[indx] = attrs[indx] + val
		end
	end
	
	player:SetMountForce(allForce)
	
	return nonForce
end

-- 升级坐骑技能
function AppSpellMgr:raiseMountSpell(spellId)
	local prev = self:getSpellLevel(spellId)
	
	for i = SPELL_INT_FIELD_MOUNT_SPELL_START, SPELL_INT_FIELD_MOUNT_SPELL_END-1 do
		if self:GetUInt16(i, SHORT_SPELL_ID) == 0 then
			return
		elseif self:GetUInt16(i, SHORT_SPELL_ID) == spellId then
			self:SetUInt16(i, SHORT_SPELL_LV, prev + 1)
			self:SetSpellLevel(spellId, prev+1)
			-- 任务
			local player = self:getOwner()
			local questMgr = player:getQuestMgr()
			questMgr:OnUpdate(QUEST_TARGET_TYPE_RAISE_MOUNT_SKILL, {spellId})
			return
		end
	end
end

-- 是否存在当前幻化坐骑
function AppSpellMgr:hasIllusion(illuId)
	
	for i = SPELL_INT_FIELD_MOUNT_ILLUSION_START, SPELL_INT_FIELD_MOUNT_ILLUSION_END-1, MAX_ILLUSION_ATTR_COUNT do
		if self:GetUInt32(ILLUSION_ATTR_ID + i) == illuId then
			return true
		end
	end
	
	return false
end


-- 激活幻化
function AppSpellMgr:onActiveIllusion(illuId)
	-- 加幻化id
	local indx = self:findEmptyIllusionSlot()
	self:SetUInt32(ILLUSION_ATTR_ID + indx, illuId)
	
	outFmtInfo("active illusion %d", illuId)

	-- 加过期时间
	local config = tb_mount_illusion[illuId]
	local expire = 0
	if config.last > 0 then
		expire = os.time() + config.last * 60
	end
	self:SetUInt32(ILLUSION_ATTR_EXPIRE + indx, expire)
	
	-- 加技能
	local config = tb_mount_illusion[illuId]
	local st = indx + ILLUSION_ATTR_SPELL_START
	for _, spellId in pairs(config.spells) do
		self:SetUInt16(st, SHORT_SPELL_ID, spellId)
		self:SetUInt16(st, SHORT_SPELL_LV, 1)
		st = st + 1
	end
end

function AppSpellMgr:findEmptyIllusionSlot()
	for i = SPELL_INT_FIELD_MOUNT_ILLUSION_START, SPELL_INT_FIELD_MOUNT_ILLUSION_END-1, MAX_ILLUSION_ATTR_COUNT do
		if self:GetUInt32(ILLUSION_ATTR_ID + i) == 0 then
			return i
		end
	end
	
	return -1
end


-- 升级坐骑技能
function AppSpellMgr:raiseIlluSpell(spellId)

	local prev = self:getSpellLevel(spellId)

	for i = SPELL_INT_FIELD_MOUNT_ILLUSION_START, SPELL_INT_FIELD_MOUNT_ILLUSION_END-1, MAX_ILLUSION_ATTR_COUNT do	
		for j = ILLUSION_ATTR_SPELL_START + i, ILLUSION_ATTR_SPELL_END + i do
			if self:GetUInt16(j, SHORT_SPELL_ID) == spellId then
				self:SetUInt16(j, SHORT_SPELL_LV, prev + 1)
				self:SetSpellLevel(spellId, prev+1)
				return
			end
		end
	end
end

-- 获得幻化的binlogindex
function AppSpellMgr:getIllusionBinLogIndex(illuId)
	local indx = -1
	for i = SPELL_INT_FIELD_MOUNT_ILLUSION_START, SPELL_INT_FIELD_MOUNT_ILLUSION_END-1, MAX_ILLUSION_ATTR_COUNT do
		if self:GetUInt32(ILLUSION_ATTR_ID + i) == illuId then
			indx = i
			break
		end
	end
	
	return indx
end

-- 检测是否过期
-- 返回过期的幻化信息{{illusionId, skill1, skill2,...},...}
function AppSpellMgr:checkIfIllusionExpired()
	local expiredTable = {}
	local curr = os.time()
	
	for i = SPELL_INT_FIELD_MOUNT_ILLUSION_START, SPELL_INT_FIELD_MOUNT_ILLUSION_END, MAX_ILLUSION_ATTR_COUNT do
		local illuId = self:GetUInt32(ILLUSION_ATTR_ID + i)
		-- 有幻化id的表示有数据
		if illuId > 0 then
			local expireTime = self:GetUInt32(ILLUSION_ATTR_EXPIRE + i)
			-- 过期时间为0表示永不过期
			if expireTime > 0 and curr > expireTime then
				local info = {illuId}
				for j = ILLUSION_ATTR_SPELL_START, ILLUSION_ATTR_SPELL_END-1 do
					local spellId = self:GetUInt16(i + j, 0)
					if spellId > 0 then
						table.insert(info, spellId)
					end
				end
				table.insert(expiredTable, info)
				
				-- 清空数据
				self:SetUInt32(ILLUSION_ATTR_ID + i, 0)
				for j = ILLUSION_ATTR_SPELL_START, ILLUSION_ATTR_SPELL_END-1 do
					self:SetUInt32(i + j, 0)
				end
				self:SetUInt32(ILLUSION_ATTR_EXPIRE + i, 0)
			end
		end
	end
	
	return expiredTable
end

----------------------------------------------神兵-----------------------------------------------
--获取神兵列表
function AppSpellMgr:getDivineList()
	local tab = {}
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		--outFmtInfo("divine %d",i)
		local id = self:GetByte(i,0)
		if id ~= 0  then
			local lev = self:GetByte(i,1)
			tab[id] = lev
		end
	end
	return tab
end
--神兵属性加成
function AppSpellMgr:calculDivineAttr(attrs)
	local player = self:getOwner()
	local tab = self:getDivineList()
	--Ttab(attrs)
	local allForce = 0
	for k,v in pairs(tab) do
		local id = (k-1) * #tb_divine_bless + v + 1
		--local config = tb_divine_streng[id].props
		
		local props_table = {}
		for _,prop in pairs(tb_divine_base[k].props) do
			props_table[prop[1]] = prop[2]
		end
		local index = self:GetDivineIndex(k)
		local forge_lv = self:GetDivineForgeLv(index)
		local advance_lv = self:GetDivineAdvanceLv(index)
		local spirit_lv = self:GetDivineSpiritLv(index)
		
		local spirit_attr_scale = (tb_divine_spirit[spirit_lv].attr_scale + 100)/100
		
		local forge_attr_id = tb_divine_forge[forge_lv].attr_id
		local forge_attr_value = tb_divine_forge[forge_lv].attr_value
		
		for key,attr_id in pairs(forge_attr_id) do
			if props_table[attr_id] then
				props_table[attr_id] = props_table[attr_id] + forge_attr_value[key] * spirit_attr_scale
			end
		end
		
		if advance_lv > 0 then
			for advance_index = k*100 + 1,k*100 + advance_lv do
				local advance_config = tb_divine_advance[advance_index]
				local attr_id = advance_config.attr_id
				local attr_value = advance_config.attr_value
				if props_table[attr_id] then
					props_table[attr_id] = props_table[attr_id] + attr_value
				else
					props_table[attr_id] = attr_value
				end
			end
		end
		
		local config = {}
		for attr_id,attr_value in pairs(props_table) do
			table.insert(config,{attr_id,attr_value})
			
		end
		
		local baseForce = DoAnyOneCalcForceByAry(config, player:GetGender())
		allForce = allForce + baseForce
		for i=1,#config do
			attrs[config[i][1]] = attrs[config[i][1]] + config[i][2]
		end
	end
	
	player:SetDivineForce(allForce)
	--Ttab(attrs)
end
-- 是否存在神兵
function AppSpellMgr:hasDivine(divineId)
	
	--local count = self:GetUInt32(SPELL_DIVINE_COUNT)
	--local size = SPELL_DIVINE_START + count * MAX_DIVINE_COUNT
	
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		--outFmtInfo("divine %d",i)
		if self:GetByte(i,0) == divineId then
			return true
		end
	end
	
	return false
end
-- 任务判断是否存在神兵
function AppSpellMgr:hasDivineQuest(divineId)
	
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		local div = self:GetByte(i,0)
		if divineId ~= 0 and div == divineId then
			return true
		elseif divineId == 0 and div ~= 0 then
			return true
		end
	end
	
	return false
end
-- 当前神兵数量
function AppSpellMgr:getDivineNum()
	local num = 0
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		if self:GetByte(i,0) ~= 0 then
			num = num + 1
		end
	end
	return num
end

--添加神兵
function AppSpellMgr:addDivine(divineId,time)
	if self:hasDivine(divineId) then
		return
	end

	--outFmtInfo("add divine %d,%d",divineId,time)

	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		if self:GetByte(i,0) == 0 then
			self:SetByte(i,0,divineId)
			self:SetUInt32(i + DIVINE_TIME,time)
			return true;
		end
	end

	return false;
end

function AppSpellMgr:addDivineSkill(divineId,skill,isPassive)
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		if self:GetByte(i,0) == divineId then
			outFmtInfo("add skill %d,%d,%d",i,divineId,skill)
			if isPassive then -- 添加被动技能
				for j = 0,2 do 
					local pid = self:GetUInt16(i+DIVINE_PASSIVE_SKILL+j,0)
					if pid == skill then -- 已经存在被动技能
						outFmtError("divine passive skill has exist")
						return
					end

					if pid == 0 then
						self:SetUInt16(i+DIVINE_PASSIVE_SKILL+j,0,skill)
						self:SetUInt16(i+DIVINE_PASSIVE_SKILL+j,1,1)
						return
					end
				end
				outFmtError("divine passive skill canot add")
			else --添加主动技能
				self:SetUInt16(i+DIVINE_SKILL,0,skill)
				self:SetUInt16(i+DIVINE_SKILL,1,1)
			end
		end
	end

end
--获得神兵的index、等级和祝福值
function AppSpellMgr:getDivinIdxLevBless(divineId)
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		if self:GetByte(i,0) == divineId then
			return i,self:GetByte(i,1),self:GetByte(i,2)
		end
	end
	return 0
end
--设置神兵的等级和祝福值
function AppSpellMgr:setDivinLevBless(index,lev,bless)
	self:SetByte(index,1,lev)
	self:SetUInt16(index,1,bless)
end
--设置神兵的初始主动饥饿ngn
function AppSpellMgr:GetDivineInitiativeSpellInfo(divineId)
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		if self:GetByte(i,0) == divineId then
			local sid = self:GetUInt16(i+DIVINE_SKILL,0)
			local lev = self:GetUInt16(i+DIVINE_SKILL,1)
			return sid,lev
		end
	end
	return 0,0
end 
--获取神兵被动技能列表
function AppSpellMgr:GetDivinePassiveSpellInfoTable(divineId)
	
	local tab = {}
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		if self:GetByte(i,0) == divineId then
			for j=0,2 do
				local sid = self:GetUInt16(i+DIVINE_PASSIVE_SKILL+j,0)
				local lev = self:GetUInt16(i+DIVINE_PASSIVE_SKILL+j,1)
				table.insert(tab, {sid, lev})
			end
		end
	end
	return tab
end

--设置神兵技能等级
function AppSpellMgr:DivineSkillUpLev(skillid)
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		if self:GetByte(i,0) ~= 0 then
			local sid = self:GetUInt16(i+DIVINE_SKILL,0)
			local lev = self:GetUInt16(i+DIVINE_SKILL,1)
			--主动技能
			if sid == skillid then
				self:SetUInt16(i+DIVINE_SKILL,1,lev+1)
				-- 同步映射关系
				self:SetSpellLevel(skillid, lev+1)
				return true
			end
			--被动技能
			for j=0,2 do
				sid = self:GetUInt16(i+DIVINE_PASSIVE_SKILL+j,0)
				lev = self:GetUInt16(i+DIVINE_PASSIVE_SKILL+j,1)
				if sid == skillid then
					self:SetUInt16(i+DIVINE_PASSIVE_SKILL+j,1,lev+1)
					
					-- 同步映射关系
					self:SetSpellLevel(skillid, lev+1)
					return true
				end
			end
		end
	end
	return false
end


--神兵培养
--取得神兵下标
function AppSpellMgr:GetDivineIndex(divineId)
	for i = SPELL_DIVINE_START, SPELL_DIVINE_END - 1, MAX_DIVINE_COUNT do
		if self:GetByte(i,0) == divineId then
			return i
		end
	end
	return 0
end

--铸造
function AppSpellMgr:GetDivineForgeLv(index)
	return self:GetByte(index + DIVINE_IMPROVE,0)
end

function AppSpellMgr:SetDivineForgeLv(index,value)
	self:SetByte(index + DIVINE_IMPROVE,0,value)
end

--升阶
function AppSpellMgr:GetDivineAdvanceLv(index)
	return self:GetByte(index + DIVINE_IMPROVE,1)
end

function AppSpellMgr:SetDivineAdvanceLv(index,value)
	self:SetByte(index + DIVINE_IMPROVE,1,value)
end

--铸魂
function AppSpellMgr:GetDivineSpiritLv(index)
	return self:GetByte(index + DIVINE_IMPROVE,2)
end

function AppSpellMgr:SetDivineSpiritLv(index,value)
	self:SetByte(index + DIVINE_IMPROVE,2,value)
end


----------------------------------------------神兵结束-------------------------------------------

----------------------------------------------法宝-------------------------------------------

function AppSpellMgr:GetTalismanIndex(id)
	for i = SPELL_TALISMAN_START, SPELL_TALISMAN_END - 1, MAX_TALISMAN_COUNT do
		if self:GetByte(i,0) == id then
			return i
		end
	end
	return 0
end

function AppSpellMgr:AddTalisman(id)
	for i = SPELL_TALISMAN_START, SPELL_TALISMAN_END - 1, MAX_TALISMAN_COUNT do
		if self:GetByte(i,0) == id then
			return
		end
		if self:GetByte(i,0) == 0 then
			self:SetByte(i,0,id)
			self:SetByte(i,1,1)
			return
		end
	end
end

function AppSpellMgr:SetTalismanLv(index, value)
	self:SetByte(index,1,value)
end

function AppSpellMgr:GetTalismanLv(index)
	return self:GetByte(index,1)
end

function AppSpellMgr:SetTalismanForce(index,value)
	self:SetUInt32(index + TALISMAN_FORCE,value)
end

function AppSpellMgr:GetTalismanForce(index)
	return self:GetUInt32(index + TALISMAN_FORCE)
end

--获取法宝列表
function AppSpellMgr:getTalismanList()
	local tab = {}
	for i = SPELL_TALISMAN_START, SPELL_TALISMAN_END - 1, MAX_TALISMAN_COUNT do
		local id = self:GetByte(i,0)
		if id ~= 0  then
			local lev = self:GetByte(i,1)
			tab[id] = lev
		end
	end
	return tab
end

function AppSpellMgr:hasTalisman(id)
	for i = SPELL_TALISMAN_START, SPELL_TALISMAN_END - 1, MAX_TALISMAN_COUNT do
		if self:GetByte(i,0) == id then
			return true
		end
	end
	return false
end


--重算法宝战力
function AppSpellMgr:calculTalismanForce(id)
	local index = self:GetTalismanIndex(id)
	local curlev = self:GetTalismanLv(index)
	
	local props_table = {}
	--for _,prop in pairs(tb_talisman_base[id].props) do
	--	props_table[prop[1]] = prop[2]
	--end
	
	local spirit_attr_id = tb_talisman_spirit[id * 1000 + curlev].attr_id
	local spirit_attr_value = tb_talisman_spirit[id * 1000 + curlev].attr_value
	
	for key,attr_id in pairs(spirit_attr_id) do
		props_table[attr_id] = spirit_attr_value[key]
	end
	
	local config = {}
	for attr_id,attr_value in pairs(props_table) do
		table.insert(config,{attr_id,attr_value})
		
	end
	local player = self:getOwner()
	local baseForce = DoAnyOneCalcForceByAry(config, player:GetGender())
	self:SetTalismanForce(index,baseForce)
end


--法宝属性总加成
function AppSpellMgr:calculTalismanAttr(attrs)
	local player = self:getOwner()
	local tab = self:getTalismanList()
	--Ttab(attrs)
	local allForce = 0
	for k,v in pairs(tab) do
		local props_table = {}
		if not tb_talisman_base[k] then
			outFmtError("talisman id:%d not exist",k)
			return
		end
		
		--for _,prop in pairs(tb_talisman_base[k].props) do
		--	props_table[prop[1]] = prop[2]
		--end
		local index = self:GetTalismanIndex(k)
		local curlev = self:GetTalismanLv(index)
		
		local spirit_attr_id = tb_talisman_spirit[k * 1000 + curlev].attr_id
		local spirit_attr_value = tb_talisman_spirit[k * 1000 + curlev].attr_value
		
		for key,attr_id in pairs(spirit_attr_id) do
			props_table[attr_id] = spirit_attr_value[key]
		end
		
		local config = {}
		for attr_id,attr_value in pairs(props_table) do
			table.insert(config,{attr_id,attr_value})
			
		end
		
		local baseForce = DoAnyOneCalcForceByAry(config, player:GetGender())
		allForce = allForce + baseForce
		for i=1,#config do
			attrs[config[i][1]] = attrs[config[i][1]] + config[i][2]
		end
	end
	
	player:SetAllTalismanForce(allForce)
	--Ttab(attrs)
end
----------------------------------------------法宝结束-------------------------------------------

----------------------------------------------神羽开始-------------------------------------------
--神羽id
function AppSpellMgr:GetWingsId()
	return self:GetUInt32(SPELL_WINGS_ID)
end

function AppSpellMgr:SetWingsId(value)
	self:SetUInt32(SPELL_WINGS_ID,value)
	local owner = self:getOwner()
	owner:SetUInt32(PLAYER_INT_FIELD_WINGS_RANK,math.floor(value/100))
	owner:SetUInt32(PLAYER_INT_FIELD_WINGS_STAR,value%100)
end
--神羽强化等级
function AppSpellMgr:GetWingsLevel()
	return self:GetUInt32(SPELL_WINGS_LEVEL)
end

function AppSpellMgr:SetWingsLevel(value)
	self:SetUInt32(SPELL_WINGS_LEVEL,value)
end

--神羽祝福经验
function AppSpellMgr:GetWingsBlessExp()
	return self:GetUInt32(SPELL_WINGS_BLESS_EXP)
end

function AppSpellMgr:SetWingsBlessExp(value)
	self:SetUInt32(SPELL_WINGS_BLESS_EXP,value)
end

function AppSpellMgr:calculWingsAttr(attrs)
	local player = self:getOwner()
	local props_table = {}
	
	local id = self:GetWingsId()
	
	if id == 0 or not tb_wings_bless[id] then
		outFmtDebug("wings id:%d not exist",id)
		return
	end
	
	local level = self:GetWingsLevel()
	
	local bless_attr_id = tb_wings_bless[id].attr_id
	local bless_attr_value = tb_wings_bless[id].attr_value
	local attr_scale = 0
	if tb_wings_strength[level] then
		attr_scale = tb_wings_strength[level].improve
	end
	
	
	for key,attr_id in pairs(bless_attr_id) do
		props_table[attr_id] = bless_attr_value[key] * (1 + attr_scale / 10000)
	end
	
	local config = {}
	for attr_id,attr_value in pairs(props_table) do
		table.insert(config,{attr_id,attr_value})
		
	end
	
	local baseForce = DoAnyOneCalcForceByAry(config, player:GetGender())
	
	for i=1,#config do
		attrs[config[i][1]] = attrs[config[i][1]] + config[i][2]
	end
	
	player:SetWingsForce(baseForce)
	--Ttab(attrs)
end

----------------------------------------------神羽结束-------------------------------------------


----------------------------------------------经脉开始-------------------------------------------
function AppSpellMgr:getMeridianLevel()
	return self:GetUInt16(SPELL_INT_FIELD_MERIDIAN_LEVEL, 0)
end

function AppSpellMgr:addMeridianLevel()
	self:AddUInt16(SPELL_INT_FIELD_MERIDIAN_LEVEL, 0, 1)
end

function AppSpellMgr:isMeridianNeedBreak()
	return self:GetUInt16(SPELL_INT_FIELD_MERIDIAN_LEVEL, 1) == 1
end

function AppSpellMgr:setMeridianBreakState(state)
	self:SetUInt16(SPELL_INT_FIELD_MERIDIAN_LEVEL, 1, state)
end


function AppSpellMgr:getMeridianExp()
	return self:GetUInt32(SPELL_INT_FIELD_MERIDIAN_EXP)
end

function AppSpellMgr:addMeridianExp(val)
	self:AddUInt32(SPELL_INT_FIELD_MERIDIAN_EXP, val)
end

function AppSpellMgr:costMeridianExp(val)
	local curr = self:getMeridianExp()
	if curr < val or val < 0 then
		return false
	end
	
	self:subMeridianExp(val)
	return true
end

function AppSpellMgr:subMeridianExp(val)
	if val < 0 then val = -val end
	val = math.min(self:getMeridianExp(), val)
	self:SubUInt32(SPELL_INT_FIELD_MERIDIAN_EXP, val)
end

function AppSpellMgr:addMeridianExpSource(sourceId)
	local config = tb_meridian_source[sourceId]
	if not config then
		return
	end
	
	local limit = config.limit
	local prev = self:GetByte(SPELL_INT_FIELD_MERIDIAN_CNT_START, sourceId-1)
	-- 已经满了就不加了
	if prev >= limit then
		return
	end
	
	self:AddByte(SPELL_INT_FIELD_MERIDIAN_CNT_START, sourceId-1, 1)
	self:addMeridianExp(config.exp)
end

function AppSpellMgr:calcMeridianAttr(attrs)
	local player = self:getOwner()
	
	local meridianAttrs = {}
	local lv = self:getMeridianLevel()
	local config = tb_meridian_info[lv]
	
	local jobIndx = getJobIndxByGender(player:GetGender())
	local attrKeys = config["attrKeys"..jobIndx]
	local attrValues = config["attrValues"..jobIndx]
	for i = 1, #attrKeys do
		local attr_id		= attrKeys[ i ]
		local attr_value	= attrValues[ i ]
		table.insert(meridianAttrs, {attr_id, attr_value})
		if not attrs[attr_id] then
			attrs[attr_id] = 0
		end
		attrs[attr_id] = attrs[attr_id] + attr_value
	end
	
	local baseForce = DoAnyOneCalcForceByAry(meridianAttrs, player:GetGender())
	
	player:SetMeridianForce(baseForce)
end

----------------------------------------------经脉结束-------------------------------------------

----------------------------------------------强化宝石-----------------------------------------------
-- 获取部位强化等级
function AppSpellMgr:getStrengLev(part)
	return self:GetUInt16(SPELL_STRENGTHEN_START + (part-1) * MAX_STRENGTHEN_COUNT,0)
end
-- 设置部位强化等级
function AppSpellMgr:setStrengLev(part,lev)
	self:SetUInt16(SPELL_STRENGTHEN_START + (part-1) * MAX_STRENGTHEN_COUNT,0,lev)
end
-- 获取部位强化祝福值
function AppSpellMgr:getStrengBlessExp(part)
	return self:GetUInt16(SPELL_STRENGTHEN_START + (part-1) * MAX_STRENGTHEN_COUNT,1)
end
-- 设置部位强化祝福值
function AppSpellMgr:setStrengBlessExp(part,val)
	return self:SetUInt16(SPELL_STRENGTHEN_START + (part-1) * MAX_STRENGTHEN_COUNT,1,val)
end
-- 设置全身强化等级加成
function AppSpellMgr:setStrengMul(val)
	return self:SetUInt32(SPELL_STRENGTHEN_ALLMUL,val)
end
-- 获取全身强化等级加成
function AppSpellMgr:getStrengMul()
	return self:GetUInt32(SPELL_STRENGTHEN_ALLMUL)
end

-- 获取部位当前宝石的等级
function AppSpellMgr:getGemCurLev(part)
	local gemid = SPELL_GEM_START + (part-1) * MAX_GEM_COUNT
	local id = self:GetUInt16(gemid + GEM_CURID_BLESS,0)
	local val = self:GetUInt32(gemid + id)
	return val
end
-- 设置当前部位宝石的等级
function AppSpellMgr:setGemCurLev(part,lev)
	local gemid = SPELL_GEM_START + (part-1) * MAX_GEM_COUNT
	local id = self:GetUInt16(gemid + GEM_CURID_BLESS,0)
	return self:SetUInt32(gemid + id,lev)
end

-- 获取部位最小的宝石等级
function AppSpellMgr:getGemMinLev(part)
	local gemid = SPELL_GEM_START + (part-1) * MAX_GEM_COUNT
	local minval = 10000

	for i=0,2 do
		local lev = self:GetUInt32(gemid + i)
		if lev < minval then
			minval = lev
		end
	end

	return minval
end
--获取所有宝石等级
function AppSpellMgr:getGemAllLev(part)
	local gemid = SPELL_GEM_START + (part-1) * MAX_GEM_COUNT
	local tab = {}
	for i=0,2 do
		local lev = self:GetUInt32(gemid + i)
		table.insert(tab,lev)
	end
	return tab
end

-- 获取部位当前宝石的祝福值
function AppSpellMgr:getGemCurBless(part)
	local gemid = SPELL_GEM_START + (part-1) * MAX_GEM_COUNT
	return self:GetUInt16(gemid + GEM_CURID_BLESS,1)
end

-- 设置当前宝石的祝福值
function AppSpellMgr:setGemCurBless(part,val)
	local gemid = SPELL_GEM_START + (part-1) * MAX_GEM_COUNT
	self:SetUInt16(gemid + GEM_CURID_BLESS,1,val)
end
-- 切换宝石的当前ID 0->1->2->0->1->...
function AppSpellMgr:setGemChgID(part)
	local gemid = SPELL_GEM_START + (part-1) * MAX_GEM_COUNT
	local cid = self:GetUInt16(gemid + GEM_CURID_BLESS,0)
	if cid == 2 then
		cid = 0
	else 
		cid = cid + 1
	end
	self:SetUInt16(gemid + GEM_CURID_BLESS,0,cid)
end

-- 设置全身宝石等级加成
function AppSpellMgr:setGemMul(val)
	return self:SetUInt32(SPELL_GEM_ALLMUL,val)
end
-- 获取全身宝石等级加成
function AppSpellMgr:getGemMul()
	return self:GetUInt32(SPELL_GEM_ALLMUL)
end
----------------------------------------------强化宝石结束-------------------------------------------


-------------------------------装备养成开始-------------------------------------------
--
--强化lv
function AppSpellMgr:GetEquipDevelopStrengthLv(index)
	return self:GetUInt16(SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_STRENGTH_LV,0)
end

function AppSpellMgr:SetEquipDevelopStrengthLv(index,value)
	self:SetUInt16(SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_STRENGTH_LV,0,value)
end

--精炼阶数
function AppSpellMgr:GetEquipDevelopRefineRank(index)--技能管理器
	return self:GetUInt16(SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_REFINE_LV,0)
end

function AppSpellMgr:SetEquipDevelopRefineRank(index,value)
	self:SetUInt16(SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_REFINE_LV,0,value)
end

--精炼星数
function AppSpellMgr:GetEquipDevelopRefineStar(index)
	return self:GetUInt16(SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_REFINE_LV,1)
end

function AppSpellMgr:SetEquipDevelopRefineStar(index,value)
	self:SetUInt16(SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_REFINE_LV,1,value)
end


--镶嵌等级
function AppSpellMgr:GetEquipDevelopGemLv(index,gem_index)
	local res1 = gem_index%2
	local res2 = math.floor(gem_index/2)
	
	local i = SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_GEM_LV_START + res2
	
	return self:GetUInt16(SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_GEM_LV_START + res2,res1)
end

function AppSpellMgr:SetEquipDevelopGemLv(index,gem_index,value)
	local res1 = gem_index%2
	local res2 = math.floor(gem_index/2)
	
	local i = SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_GEM_LV_START + res2
	
	self:SetUInt16(SPELL_INT_FIELD_EQUIPDEVELOP_START + index * MAX_EQUIPDEVELOP_COUNT + EQUIPDEVELOP_GEM_LV_START + res2,res1,value)
end

--奖励lv
function AppSpellMgr:GetEquipDevelopBonusStrengthLv()
	return self:GetByte(SPELL_INT_FIELD_EQUIPDEVELOP_BONUS_LV,0)
end

function AppSpellMgr:SetEquipDevelopBonusStrengthLv(value)
	self:SetByte(SPELL_INT_FIELD_EQUIPDEVELOP_BONUS_LV,0,value)
end

function AppSpellMgr:GetEquipDevelopBonusRefineLv()
	return self:GetByte(SPELL_INT_FIELD_EQUIPDEVELOP_BONUS_LV,1)
end

function AppSpellMgr:SetEquipDevelopBonusRefineLv(value)
	self:SetByte(SPELL_INT_FIELD_EQUIPDEVELOP_BONUS_LV,1,value)
end

function AppSpellMgr:GetEquipDevelopBonusGemLv()
	return self:GetByte(SPELL_INT_FIELD_EQUIPDEVELOP_BONUS_LV,2)
end

function AppSpellMgr:SetEquipDevelopBonusGemLv(value)
	self:SetByte(SPELL_INT_FIELD_EQUIPDEVELOP_BONUS_LV,2,value)
end


--洗炼属性结果 guid | num | attr_id | value | qua
function AppSpellMgr:GetEquipDevelopWashAttrsInfo()
	return self:GetStr(SPELL_STRING_FIELD_EQUIPDEVELOP_WASHATTRS_INFO)
end

function AppSpellMgr:SetEquipDevelopWashAttrsInfo(value)
	self:SetStr(SPELL_STRING_FIELD_EQUIPDEVELOP_WASHATTRS_INFO,value)
end

----------------------------------------------装备养成结束-------------------------------------------

--------------------------------------

function AppSpellMgr:activeAppearance(id)
	for i = SPELL_INT_FIELD_APPEARANCE_START, SPELL_INT_FIELD_APPEARANCE_END-1 do
		if self:GetUInt32(i) == 0 then
			self:SetUInt32(i, id)
			break
		end
	end
end

function AppSpellMgr:GetActiveAppearance()
	local dict = {}
	for i = SPELL_INT_FIELD_APPEARANCE_START, SPELL_INT_FIELD_APPEARANCE_END-1 do
		local id = self:GetUInt32(i)
		if id > 0 then
			dict[ id ] = 1
		end
	end
	
	return dict
end

function AppSpellMgr:calcAppearanceAttr(attrs, attrsRate, id1, id2)
	local player = self:getOwner()
	
	CalAppearanceAttrByApprId(attrsRate, id1)
	CalAppearanceAttrByApprId(attrsRate, id2)
	
	local apprDict	= self:GetActiveAppearance()
	local gender	= player:GetGender()
	local pokedice	= tb_char_info[gender].waiguantj
	for _, pokedex in ipairs(pokedice) do
		if isPokedexActive(apprDict, pokedex) then
			CalAppearanceAttrByPokedexId(attrs, pokedex)
		end
	end
end

function isPokedexActive(apprDict, id)
	local exterior = tb_appearance_pokedex[ id ].exterior
	for _, apprId in ipairs(exterior) do
		if not apprDict[apprId] then
			return false
		end
	end
	
	return true
end

function CalAppearanceAttrByApprId(dict, id)
	if id == 0 then
		return
	end
	
	local config = tb_appearance_info[id]
	if not config then
		outFmtDebug("!!!!!!!!!!!!!CalAppearanceAttrByApprId id %d",id)
		return
	end
	local list = MergeAttrKeysAndValues(config.attrKeys, config.attrValues)
	
	for _, info in ipairs(list) do
		local key = info[ 1 ]
		local val = info[ 2 ]
		if not dict[key] then
			dict[key] = 0
		end
		dict[key] = dict[key] + val
	end
end

function CalAppearanceAttrByPokedexId(dict, id)
	if id == 0 then
		return
	end
	
	local config = tb_appearance_pokedex[ id ]
	local list = MergeAttrKeysAndValues(config.attrKeys, config.attrValues)
	
	for _, info in ipairs(list) do
		local key = info[ 1 ]
		local val = info[ 2 ]
		if not dict[key] then
			dict[key] = 0
		end
		dict[key] = dict[key] + val
	end
end

-----------------------------------------------


-- 获得玩家guid
function AppSpellMgr:getPlayerGuid()
	--物品管理器guid转玩家guid
	if not self.owner_guid then
		self.owner_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypePlayer)
	end
	return self.owner_guid
end

--获得技能管理器的拥有者
function AppSpellMgr:getOwner()
	local playerguid = self:getPlayerGuid()
	return app.objMgr:getObj(playerguid)
end

return AppSpellMgr