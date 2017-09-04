local AppItemInstance = {
	--商品列表
	good_type_listcache = {}
}

--鉴定物品
function AppItemInstance:itemAppaisal(item)
	local entry = item:getEntry()
	local item_tempate = tb_item_template[entry]
	if not item_tempate then return end
	
	local attr_config = item_tempate.forge_pro
	local attr_length = item_tempate.forge_pro_max[1]

	if item_tempate.type == ITEM_TYPE_EQUIP then
		--生成基础属性
		--self:createBaseAtrr(item, item_tempate.type, attr_config)
		self:createAddAtrr(item,attr_config,attr_length)

		--重算下战斗力
		self:resetItemForce(item)
		
		
			
			
			
			
		
	end
	
	
	--标志为已鉴定
	if not item:isAppaisal() then item:setApparisal() end
end


--获得物品属性 ps：这里只获得pk属性相关的，不包括特殊属性
function AppItemInstance:getItemCalculAttr( item ,attrs,suitBaseAttribute)
	--先添加基础属性
	local entry = item:getEntry()
	local item_tempate = tb_item_template[entry]
	local pos = item_tempate.pos
	local ary = item_tempate.basic_properties
	
	for i=1,#ary do
		if not suitBaseAttribute[ary[i][1]] then
			suitBaseAttribute[ary[i][1]] = 0
		end
		suitBaseAttribute[ary[i][1]] = suitBaseAttribute[ary[i][1]] + ary[i][2]
	end
	
	local playerInfo = self:getOwner()
	local spellMgr = playerInfo:getSpellMgr()
	
	--Ttab(attrs)
	--[[
	-- 加强化属性
	local strongLv = spellMgr:getStrengLev(pos)
	if strongLv > 0 then
		local strongId = (pos - 1) * 100 + strongLv
		local pros = tb_strengthen_base[strongId].pros
		mergeAttrs(attrs, pros)
	end

	
	
	-- 宝石属性
	local gemtype = tb_gem_pos[pos].gemtype
	local levels = spellMgr:getGemAllLev(gemtype)
	for _, gemlevel in ipairs(levels) do
		if gemlevel > 0 then
			local gemId = (gemtype - 1) * 10 + gemlevel
			local pros = tb_gem_base[gemId].pros
			mergeAttrs(attrs, pros)
		end
	end
	--]]
	--[[--装备养成 部位属性 装备属性*精炼倍率 
	local refine_rank = spellMgr:GetEquipDevelopRefineRank(pos - 1)
	local refine_star = spellMgr:GetEquipDevelopRefineStar(pos - 1)
	local refine_config = tb_equipdevelop_refine[pos*10000+refine_rank*100+refine_star]
	local refine_scale = 1
	if refine_config then
		refine_scale = (100 + refine_config.improve) /100
	end
	
	-- 装备基础属性*精炼倍率 
	local array = item_tempate.basic_properties
	local props = {}
	for i,prop in pairs(array) do
		table.insert(props,{prop[1],math.floor(prop[2] * refine_scale)})
	end
	--]]
	--添加附加属性 *精炼倍率 
	local func = function (key,val)
		if not attrs[key] then
			attrs[key] = 0
		end
		attrs[key] = attrs[key] + val--math.floor( * refine_scale)
	end

	item:forEachBaseAttr(func)
	
	--mergeAttrs(attrs, props)

	return item_tempate.battlePoint
end

--物品属性重算
function AppItemInstance:itemCalculAttr( attrs ,suitBaseAttribute)
	--local attrs = {}					--汇总所有属性
	--for i = EQUIP_ATTR_HP, EQUIP_ATTR_CRIT_DEF do
	--	attrs[i] = 0
	--end
	outFmtDebug("zhuangbei chongsuan")
	--遍历装备包裹
	local suitBasePoint = 0
	local func = function( ptr )
		local item = require("appd.appd_item").new(ptr)	
		local point = self:getItemCalculAttr(item,attrs ,suitBaseAttribute)
		suitBasePoint = suitBasePoint + point
		--local temp_attrs = self:getItemCalculAttr(item)
		--self:resetItemForce(item)					
		--汇总到总属性
		--for j = EQUIP_ATTR_HP, EQUIP_ATTR_CRIT_DEF do
		--	attrs[j] = attrs[j] + temp_attrs[j]
		--end		
	end
	self.itemMgr:ForEachBagItem(BAG_TYPE_EQUIP, func)
	
	local playerInfo = self:getOwner()
	local spellMgr = playerInfo:getSpellMgr()
	--[[
	-- 全身强化属性
	local mulId = spellMgr:getStrengMul()
	if tb_strengthen_mul[mulId] then
		local pros = tb_strengthen_mul[mulId].mulpro
		mergeAttrs(attrs, pros)
	end
	
	-- 全身宝石属性
	local gemMulId = spellMgr:getGemMul()
	if tb_gem_mul[gemMulId] then
		local pros = tb_gem_mul[gemMulId].mulpro
		mergeAttrs(attrs, pros)
	end
	--]]
	
	
	--todo 新装备养成 精炼属性 强化属性 宝石属性 全身奖励
	for pos = 1,EQUIPMENT_COUNT do
		local refine_rank = spellMgr:GetEquipDevelopRefineRank(pos - 1)
		local refine_star = spellMgr:GetEquipDevelopRefineStar(pos - 1)
		local refine_config = tb_equipdevelop_refine[pos*10000+refine_rank*100+refine_star]
		local refine_scale = 1
		--[[
		if refine_config then
			refine_scale = (100 + refine_config.improve) /100
		end
		
		for index,_ in pairs(attrs) do
			attrs[index] = math.floor( attrs[index] * refine_scale)
		end
		
		local strength_lv = spellMgr:GetEquipDevelopStrengthLv(pos - 1)
		local strength_config = tb_equipdevelop_strength[pos * 1000 + strength_lv]
		if strength_config then
			local props = strength_config.props
			mergeAttrs(attrs, props)
		end
		--]]
		
		if refine_config then
			local props = refine_config["props"..getJobIndxByGender(playerInfo:GetGender())]
			mergeAttrs(attrs, props)
		end
		
		
		local strength_lv = spellMgr:GetEquipDevelopStrengthLv(pos - 1)
		local strength_config = tb_equipdevelop_strength[pos * 1000 + strength_lv]
		if strength_config then
			local props = strength_config["props"..getJobIndxByGender(playerInfo:GetGender())]
			mergeAttrs(attrs, props)
		end
		
		
		
		
		local gem_part_config = tb_equipdevelop_gem_part[pos]
		local gem_count = #(gem_part_config.gem_array)
		for gem_index = 0,gem_count -1 do
			local gem_type = gem_part_config.gem_array[gem_index + 1]
			local gem_level = spellMgr:GetEquipDevelopGemLv(pos - 1,gem_index)
			local gem_config = tb_equipdevelop_gem[gem_type * 1000 + gem_level]
			if gem_config then
				local props = gem_config.props
				mergeAttrs(attrs, props)
			end
		end
	end
	
	local bonus_strength_lv = spellMgr:GetEquipDevelopBonusStrengthLv()
	if tb_equipdevelop_bonus[1*100 + bonus_strength_lv] then
		local props = tb_equipdevelop_bonus[1*100 + bonus_strength_lv].props
		mergeAttrs(attrs, props)
	end
	
	local bonus_refine_lv = spellMgr:GetEquipDevelopBonusRefineLv()
	if tb_equipdevelop_bonus[2*100 + bonus_refine_lv] then
		local props = tb_equipdevelop_bonus[2*100 + bonus_refine_lv].props
		mergeAttrs(attrs, props)
	end
	
	local bonus_gem_lv = spellMgr:GetEquipDevelopBonusGemLv()
	if tb_equipdevelop_bonus[3*100 + bonus_gem_lv] then
		local props = tb_equipdevelop_bonus[3*100 + bonus_gem_lv].props
		mergeAttrs(attrs, props)
	end
	
	outFmtInfo("bonus %d %d %d",bonus_strength_lv,bonus_refine_lv,bonus_gem_lv)
	return suitBasePoint
end

--套装属性重算
function AppItemInstance:suitCalculAttr( base_attr )
	local attrs = {}					--汇总属性
	local suit_attrs = {}				--套装属性
	local other = {}					--其他属性（特殊属性之类的 MAX_EEQUIP_OTHER ）
	for i = 1, MAX_EQUIP_ATTR - 1, 1 do
		attrs[i] = 0
	end
	for i = 1, MAX_EEQUIP_OTHER - 1, 1 do
		other[i] = 0
	end
	
	local suit_id_config = {}				--记录已经处理过的套装id {[id] = 1,}
	--遍历装备包裹
	local func = function( ptr )
		local item = require("appd.appd_item").new(ptr)
		local entry = item:getEntry()
		local item_config = tb_item_template[entry]
		for _, id in ipairs(item_config.suit_id) do
			if id ~= 0 and not suit_id_config[id] then
				suit_id_config[id] = 1		--标记为已处理
				--先清空
				for j = 1, MAX_EQUIP_ATTR - 1 do
					suit_attrs[j] = 0				
				end					
				local flag = true
				--取出套装配置
				local config = tb_suit[id]
				for _, entry in ipairs(config.ingredient) do
					local items = self:getItemByEntry(BAG_TYPE_EQUIP, entry)
					if #items == 0 then
						flag = false
						break
					end
				end
				if flag then
					--给套装属性
					local attr_config = config.pro
					for j = 1, #attr_config, 2 do
						--属性值
						suit_attrs[ attr_config[j] ] = suit_attrs[ attr_config[j] ] + attr_config[j+1]				
					end
					
					--汇总到总属性
					for j = 1, MAX_EQUIP_ATTR - 1 do
						attrs[j] = attrs[j] + suit_attrs[j]
					end
				end					
			end
		
		end	
	end
	self.itemMgr:ForEachBagItem(BAG_TYPE_EQUIP, func)
	
	return attrs, other
end

--生成物品基础属性
function AppItemInstance:createBaseAtrr(item, typed, attr_config)
	self:clearBaseAtrr(item)	--清除老的基础属性
	local length = #attr_config
	if length < 1 then return end
	--[[
	if typed == 1 then
		--装备,必须有2条属性以上
		if length < 2 then return end
	end
	]]
	for i = 1,#attr_config do
		--outFmtDebug("att config %s,%s",attr_config[i][1],attr_config[i][2])
		item:setAttr(attr_config[i][1], attr_config[i][2])
	end
end

function AppItemInstance:createAddAtrr(item, attr_config, length)
	self:clearBaseAtrr(item)	--清除老的基础属性
	if length < 1 then return end
	local ary = GetRandomIndexTable(#attr_config,length)

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
		item:addBaseAttr(attrId, val, qua);
	end

end
-- 附加属性前缀字符串  标示,属性类型,品质
function AppItemInstance:getAddAttKey(type,lev)
	return  equip_attr_base .. equip_attr_spit  .. type .. equip_attr_spit .. lev
end

--清空物品基础属性
function AppItemInstance:clearBaseAtrr(item)
	item:clearBaseAttr()				--清空所有非特殊属性对	
end

--重算物品战斗力
function AppItemInstance:resetItemForce(item)
	----[[
	local temp_attrs = {}
	local force = self:getItemCalculAttr(item, temp_attrs,{})
	local playerInfo = self:getOwner()
	force = force + DoAnyOneCalcForce( temp_attrs, playerInfo:GetGender())		--计算战斗力
	local cur_force = item:getAttr(ITEM_OTHER_ATTR_FORCE)
	if cur_force == 0 or cur_force ~= force then
		item:setAttr(ITEM_OTHER_ATTR_FORCE, force)		--更新物品战斗力		
	end	
	self.itemMgr:SavePtr(item.item)
	--]]
end

--查找商品列表
function AppItemInstance:findGoodsList(type_id)
	if(self.good_type_listcache[type_id] == nil)then
		self.good_type_listcache[type_id] = {}
		for _,good in pairs(tb_goods)do
			if(type_id == good.type)then
				table.insert(self.good_type_listcache[type_id],good.id)
				
			end
		end
	end
	return self.good_type_listcache[type_id]
end

--删掉所有失效物品
function AppItemInstance:delFailTimeItem()
	local timenow = os.time()
	for bag_type = 0, MAX_BAG-1 do
		local bag_size = self:getBagSize(bag_type)
		for i = 1, bag_size do
			local item = self:getBagItemByPos(bag_type, i-1)
			if item and ( (item:getFailTime() ~= 0 and item:getFailTime() <= timenow) or item:getCount() == 0 ) then
				WriteItemLog(self:getOwner(), LOG_ITEM_OPER_TYPE_DEL_FAILTIME, item:getEntry(), item:getCount(), item:isBind())
				self:delItemObj(item)
			end
		end
	end
end

--扩展该方法
function AppItemInstance.extend(self)
	for k,v in pairs(AppItemInstance) do self[k] = v end
end

return AppItemInstance