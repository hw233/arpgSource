--[[
装备部位操作
]]

--强化
function PlayerInfo:strength (part)
	--部位数据是否非法
	if part <= 0 or part > EQUIPMENT_COUNT then
		outFmtError("strength error part %d",part)
		return
	end

	local spellMgr = self:getSpellMgr()
	
	--最大强化等级
	local maxLev = #tb_strengthen_bless
	local curlev = spellMgr:getStrengLev(part)
	if curlev >= maxLev then
		outFmtError("strength maxlev %d,%d",curlev,part)
		return
	end

	local config = tb_strengthen_bless[curlev + 1]


	--是否有足够的资源
	if not self:checkMoneyEnoughs(config.cost) then
		outFmtError("resouce not enough")
		return
	end

	--扣除相应资源 + 祝福值
	if self:costMoneys(MONEY_CHANGE_FORGE_STRENG,config.cost) then
	 	local baseBless = spellMgr:getStrengBlessExp(part)
	 	local bless = GetRandomExp(config.blessrate)
	 	local now = baseBless + bless
		
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_BLESS_EXP, {bless})
	 	--祝福值满，升星
	 	if now >= config.bless then
	 		spellMgr:setStrengLev(part,curlev + 1)
	 		spellMgr:setStrengBlessExp(part,now-config.bless)
	 		self:setStrengthMul()
	 		self:RecalcAttrAndBattlePoint()
			
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_BLESS_SUCCESS)
	 	else
	 		spellMgr:setStrengBlessExp(part,now)
	 	end
	 	
		-- 加任务
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_STRENGTH_SUIT, {part})
	end

end
-- 设置全身强化加成
function PlayerInfo:setStrengthMul()
	local minLev = 1000
	local spellMgr = self:getSpellMgr()
	for i = 1, EQUIPMENT_COUNT do
		local lev = spellMgr:getStrengLev(i)
		if lev < minLev then
			minLev = lev
		end
	end
	
	local allmulID = 0
	for _, strenmul in ipairs(tb_strengthen_mul) do
		if strenmul.allstrent <= minLev then
			allmulID = strenmul.id
		end
	end

	local curMul = spellMgr:getStrengMul()
	if curMul ~= allmulID then
		spellMgr:setStrengMul(allmulID)
	end

end

--宝石升级
function PlayerInfo:gem (pkt)
	local part = pkt.part

	--outFmtInfo("gem message %d",part)


	--部位数据是否非法
	if part <= 0 or part > EQUIPMENT_COUNT then
		outFmtError("gem error part %d",part)
		return
	end

	local spellMgr = self:getSpellMgr()

	local gemtype = tb_gem_pos[part].gemtype
	local maxLev = #tb_gem_cost

	local gemCurLev = spellMgr:getGemCurLev(part)

	--宝石升满
	if gemCurLev >= maxLev then
		outFmtError("gem maxlev %d,%d",gemCurLev,part)
		return
	end

	--outFmtInfo("gem type %d",gemtype)

	local gemCurBless = spellMgr:getGemCurBless(part)
	--local config = tb_gem_base[(gemtype-1)*maxLev + gemCurLev + 1]
	local costConfig = tb_gem_cost[gemCurLev + 1]

	--是否有足够的资源
	if not self:checkMoneyEnoughs(costConfig.cost) then
		outFmtError("gem resouce not enough")
		return
	end

	--扣除资源加宝石祝福值
	if self:costMoneys(MONEY_CHANGE_FORGE_GEM,costConfig.cost) then
		local bless = costConfig.exp 
		--随机暴率
		local mul = GetRandomExp(tb_gem_rate[1].rate)
		bless = bless * mul
		local now = gemCurBless + bless
		self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_GEM_EXP, {bless})
		--outFmtInfo("gem %d,%d",bless,costConfig.maxexp)

		if now >= costConfig.maxexp then 
			-- 设置等级 祝福值 切换id 宝石加成
			spellMgr:setGemCurLev(part,gemCurLev+1)
			spellMgr:setGemCurBless(part,now-costConfig.maxexp)
			spellMgr:setGemChgID(part)
			self:setGemMul()
			self:RecalcAttrAndBattlePoint()
			
			self:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_GEM_SUCCESS)
		else
			-- 添加祝福值
			spellMgr:setGemCurBless(part,now)
		end
		
		if mul > 1 then
			--self:call_operate_result_tip(MODULE_MIX,MODULE_MIX_GEM,0,0,mul)
			self:CallOptResult(OPRATE_TYPE_GEM, GEM_TYPE_CRIT,mul)
		end
		
	
		-- 加任务
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_STRENGTH_GEM, {part})
	end



end

-- 设置全身宝石加成
function PlayerInfo:setGemMul()
	
	local minLev = 10000
	local spellMgr = self:getSpellMgr()
	for i = 1, EQUIPMENT_COUNT do
		local lev = spellMgr:getGemMinLev(i)
		outFmtInfo("gem clev %d",lev)
		if lev < minLev then
			minLev = lev
		end
	end

	--outFmtInfo("gem mul %d",minLev)
	
	local allmulID = 0
	for _, gemmul in ipairs(tb_gem_mul) do
		if gemmul.allgem <= minLev then
			allmulID = gemmul.id
		end
	end

	--outFmtInfo("gem mul -- %d",allmulID)

	local curMul = spellMgr:getGemMul()
	if curMul ~= allmulID then
		spellMgr:setGemMul(allmulID)
	end
	
end
-- function PlayerInfo:strength (part, isUseMaterial)
-- 	if part < 0 or part >= MAX_EQUIP_PART_NUMBER then
-- 		return
-- 	end

-- 	--TODO: 从配置表读取数据
-- 	--TODO: 依次判断是否达到最高等级 -> 判断金币够不 -> 判断是否成功
-- 	local success = false
-- 	if isUseMaterial then
-- 		--TODO 判断包裹里是否有材料, 有的话扣除并置为true
-- 	else
-- 		--TODO 通过概率来确定是否成功
-- 	end

-- 	--TODO 由于测试需要, 把他置为成功
-- 	success = true
-- 	--TODO 如果成功, 先扣金币
-- 	if success then
-- 		self:getLogicalMgr():addStrengthLevel(part, 1)
-- 		--self:getLogicalMgr():setGemLevel(part, 0, 12)
-- 		--self:getLogicalMgr():setGemExp(part, 0, -23)		
-- 		--self:getLogicalMgr():SetStr(4, "CCCBB")		

-- 		self:getLogicalMgr():SetInt16 (200, 0, -9)

-- 		-- 发送属性改变
-- 		playerLib.SendAttr(self.ptr)
-- 	else
-- 		--TODO 如果失败, 扣一半金币
-- 		self:CallOptResult(OPERTE_TYPE_STRENGTH, STRENGTH_OPERTE_FAIL)
-- 	end
-- end