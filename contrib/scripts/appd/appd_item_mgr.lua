--物品管理器
local ItemMgrBase = require("appd.item_mgr_base")
local AppItemMgr = class("AppItemMgr", ItemMgrBase)

function AppItemMgr:ctor()
	super(self)		--调用下基类的构造函数
	--物品cd map
	self.cool_down_map = {}			--{[entry] = cd,}
	

	--扩展物品相关的业务接口，如：物品属性重算，物品鉴定
	require('appd.appd_item_instance').extend(self)
end

function AppItemMgr:init( )
	self.super.init(self)	

	--初始化各个包裹的大小
	for _,bag in ipairs(tb_bag_size) do
		local bag_type = bag.type
		if self:getBagSize(bag_type) == 0 then
			self:setBagSize(bag_type, bag.num_init)	--初始化背包大小
		end
	end
end

--添加物品返回物品id ps：仅限主包裹且不处理叠加，直接new一个物品
--[[
@entry:物品模板
@isBind：是否绑定
@isAppaisal：是否鉴定
@fail_time：失效时间
@cb：回调函数

@return: 
]]
function AppItemMgr:addItemReturnPtr(entry, isBind, isAppaisal, fail_time, cb)
	local bag_type = BAG_TYPE_MAIN_BAG
	if type(isBind) == "number" then
		isBind = isBind ~= 0
	end
	local empty_count = self:getEmptyCount(bag_type)	--取出剩余位置个数
	if empty_count <= 0 then
		outFmtDebug("addItemReturnPtr: bag is full!")
		return -1 --没空位置了
	end
	--新建个物品
	local item = require("appd.appd_item").new()
	item:setEntry(entry)	
	item:setCount(1)
	if isBind then item:setBind() end	
	if isAppaisal and self.itemAppaisal then self:itemAppaisal(item) end
	if fail_time > 0 then item:setFailTime(fail_time) end
	if cb then cb(item) end
	self.itemMgr:Add(item.item, bag_type, -1)
end

--交换位置
function AppItemMgr:exchangePos(src_bag, src_pos, dst_bag, dst_pos)
	--outFmtDebug("-------------exchange%d,%d,%d,%d",src_bag,src_pos,dst_bag,dst_pos)
	local owner = self:getOwner()
	if not owner then
		outFmtError("exchangePos:not find %s owner!", self.itemMgr:GetGuid())
		return false
	end
	
	if src_bag > MAX_BAG or dst_bag > MAX_BAG then
		return false
	end
	
	--校验下相互交换的包裹是否合法
	if src_bag == BAG_TYPE_MAIN_BAG then
		if dst_bag ~= BAG_TYPE_MAIN_BAG then
			outFmtError("exchangePos1:cant exchange to dst_bag %d src_bag %d",dst_bag, src_bag)
			return false
		end	
	elseif src_bag == BAG_TYPE_EQUIP then
		if dst_bag ~= BAG_TYPE_EQUIP_BAG then
			outFmtError("exchangePos3:cant exchange to dst_bag %d src_bag %d",dst_bag, src_bag)
			return false
		end
	elseif src_bag == BAG_TYPE_EQUIP_BAG then
		if dst_bag ~= BAG_TYPE_EQUIP and dst_bag ~= BAG_TYPE_EQUIP_BAG then
			outFmtError("exchangePos4:cant exchange to dst_bag %d src_bag %d",dst_bag, src_bag)
			return false
		end		
	elseif src_bag == BAG_TYPE_GEM then
		if dst_bag ~= BAG_TYPE_GEM_BAG then
			--其他包裹就只能交换到主包裹
			outFmtError("exchangePos5:cant exchange to dst_bag %d src_bag %d",dst_bag, src_bag)
			return false
		end
	elseif src_bag == BAG_TYPE_GEM_BAG then
		if dst_bag ~= BAG_TYPE_GEM then
			--其他包裹就只能交换到主包裹
			outFmtError("exchangePos5:cant exchange to dst_bag %d src_bag %d",dst_bag, src_bag)
			return false
		end
	end
	
	local src_bag_size = self:getBagSize(src_bag)
	local dst_bag_size = self:getBagSize(dst_bag)
	if src_pos >= src_bag_size then
		--位置从0开始
		outFmtError("exchangePos: src_pos %d >= max_bag_size %d", src_pos, src_bag_size)
		return false
	end
	if dst_pos >= dst_bag_size then
		--位置从0开始
		outFmtError("exchangePos: dst_pos %d >= max_bag_size %d", dst_pos, dst_bag_size)
		return false
	end
	
	local src_item = self:getBagItemByPos(src_bag, src_pos)
	local dst_item = self:getBagItemByPos(dst_bag, dst_pos)

	if not src_item then
		outFmtError("exchangePos:not find src_item src_bag %d src_pos %d", src_bag, src_pos)
		return false
	end
	local src_temp = tb_item_template[src_item:getEntry()]	
	--卸装备的时候目的包裹一定要是主包裹，且目标位置没有物品
	if src_bag == BAG_TYPE_EQUIP and dst_item then
		outFmtError("exchangePos: src_bag == BAG_TYPE_EQUIP find dst_pos has item %d", dst_item:getEntry())
		return false
	end	
	
	local isRring = false	--标志是否要穿到右戒指
	if src_bag == dst_bag then
		--同一包裹间互换
		if src_bag == BAG_TYPE_EQUIP or src_pos == dst_pos then return false end
	else
		--穿装备校验
		--ps: (src_bag == BAG_TYPE_EQUIP and dst_item )这个的判断是否可以不用呢？因为源包裹是装备包裹的话其实就是卸载装备，卸载装备就不应该在判断是否鉴定是否够负重了等等
		if dst_bag == BAG_TYPE_EQUIP then -- or (src_bag == BAG_TYPE_EQUIP and dst_item )
			--目的包裹是装备包裹			
			if dst_item then
				--戒指特殊处理
				--[[ 我们游戏不分左右戒指故不用处理
				local dst_temp = tb_item_template[dst_item:getEntry()]
				if dst_temp.pos == EQUIPMENT_TYPE_LRING then
					local litem = dst_item
					--说明已经穿了一个戒指在身上了,这样只能把戒指穿在右戒指的位置了
					local ritem = self:getBagItemByPos(dst_bag, EQUIPMENT_TYPE_RRING)
					if ritem then
						--左右戒指都装满了,判断哪个战斗力高
						if litem:getForce() > ritem:getForce() then
							isRring = true
							dst_item = ritem
						else
							isRring = false							
						end					
					else
						--右戒指位置还是空的					
						dst_item = nil
						isRring = true
					end
				end
				]]
			end
			
			--校验装备位置
			if src_temp.pos ~= dst_pos then
				--估计是模板填错了
				outFmtError("exchangePos: templete pos is invalid!")
				return false
			end
			--校验源物品是否鉴定
			if not src_item:isAppaisal() then
				outFmtError("exchangePos: src_item %d pos %d not appaisal", src_item:getEntry(), src_pos)
				return false
			end
			--校验是否是装备
			if src_temp.type ~= ITEM_TYPE_EQUIP and ITEM_TYPE_FASHION ~= src_temp.type then
				outFmtError("exchangePos:src_item %d pos %d not equip item!", src_item:getEntry(), src_pos)
				return false
			end
					
			--校验穿戴等级
			if owner:GetLevel() < src_temp.level then
				outFmtError("exchangePos: player level %d < min_level %d", owner:GetLevel(), src_temp.level)
				return false
			end
			
			--[[
			--校验性别
			if not table.find(src_temp.availableGender, owner:GetGender()) then
				outFmtError("exchangePos: player gender %d not fit", owner:GetGender())
				return false
			end
			--]]
			
			--[[ 无负重相关属性
			if not self:isCanBear(src_item, dst_item) then
				outFmtError("exchangePos: cant bear!")
				return false
			end
			]]
			if ITEM_TYPE_FASHION == src_temp.type and not owner:GetOpenMenuFlag(MODULE_FASHION,MODULE_FASHION_ALL) then
				outFmtError("exchangePos:Fashion Module not unlock !")
				return false
			end
			
		
		end		
	end
--[[	
	--开始交换位置流程啦
	--如果可以叠加直接处理
	if src_item and dst_item and src_item:canOverlay(dst_item) and dst_item:getCount() < src_temp.max_overlap then
		local can_over_lay = src_temp.max_overlap - dst_item:getCount()
		if can_over_lay < src_item:getCount() then
			dst_item:setCount(dst_item:getCount() + can_over_lay)
			src_item:setCount(src_item:getCount() - can_over_lay)
			self.itemMgr:SavePtr(dst_item.item)
			self.itemMgr:SavePtr(src_item.item)
		else
			dst_item:setCount(dst_item:getCount() + src_item:getCount())
			self.itemMgr:SavePtr(dst_item.item)
			--删除源物品
			self:delItemObj(src_item)
		end
		return true
	end
	--]]
	--源位置物品移到目的位置
	if isRring and dst_pos == EQUIPMENT_TYPE_LRING then
		dst_pos = EQUIPMENT_TYPE_RRING;
	end
	self.itemMgr:Move(src_item:getId(), dst_bag, dst_pos)
	
	--目的物品移到源位置
	-- if dst_item then
	-- 	self.itemMgr:Move(dst_item:getId(), src_bag, src_pos)
	-- end
	
	--有装备包裹参与更新玩家装备下标
	if src_bag == BAG_TYPE_EQUIP then
		local update_pos = src_pos
		if src_pos >= EQUIPMENT_TYPE_MAX_POS then
			update_pos = update_pos - EQUIPMENT_TYPE_MAX_POS
		end
		owner:UnSetBit(PLAYER_EXPAND_INT_USE_FASHION, update_pos)
		owner:UpdateEquipDisplay(update_pos)
		--属性重算
		self:resetItemForce(src_item)
		playerLib.SendAttr(owner.ptr)
	end
	
	if dst_bag == BAG_TYPE_EQUIP then
		local update_pos = dst_pos
		if dst_pos >= EQUIPMENT_TYPE_MAX_POS then
			update_pos = update_pos - EQUIPMENT_TYPE_MAX_POS
		end
		owner:UnSetBit(PLAYER_EXPAND_INT_USE_FASHION, update_pos)
		owner:UpdateEquipDisplay(update_pos)
		if dst_item then self:resetItemForce(dst_item) end
		playerLib.SendAttr(owner.ptr)
		--通知客户端交换位置成功
		owner:CallOptResult(OPRATE_TYPE_BAG,BAG_RESULT_EXCHANGE_SUCCESS,dst_pos)
		
		-- 任务
		local questMgr = owner:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_SUIT, {update_pos, src_item:getEntry()})
	end
	return true
end

--分割物品
function AppItemMgr:splitItem(src_bag, src_pos, dst_bag, dst_pos, count)
	local src_item = self:getBagItemByPos(src_bag, src_pos)
	local dst_item = self:getBagItemByPos(dst_bag, dst_pos)
	
	if not src_item then
		outFmtDebug("splitItem: not find src item %d %d", src_bag, src_pos)
		return false
	end
	
	if dst_item and dst_item:getEntry() == src_item:getEntry() and dst_item:getFailTime() == src_item:getFailTime()
		and dst_item:isBind() == src_item:isBind() then
		--目标位置有该类型物品
		local max_over_lay = tb_item_template[dst_item:getEntry()].max_overlap
		local dst_over_lay = dst_item:getCount()
		local can_over_lay = max_over_lay - dst_over_lay
		if count < can_over_lay then
			can_over_lay = count
		end
		if can_over_lay < src_item:getCount() then
			--处理叠加
			dst_item:setCount(dst_item:getCount() + can_over_lay)
			src_item:setCount(src_item:getCount() - can_over_lay)
			self.itemMgr:SavePtr(dst_item.item)
			self.itemMgr:SavePtr(src_item.item)
		else
			--原位置物品删掉
			dst_item:setCount(dst_item:getCount() + src_item:getCount())
			self:delItemObj(src_item)
			self.itemMgr:SavePtr(dst_item.item)			
		end
		return true		
	end
	--目标位置有物品，还执行到这，估计是模板位置和原位置物品不一致
	if dst_item then
		outFmtDebug("splitItem:split item fail!")
		return false
	end
	
	--到这说明目标位置没物品
	local src_over_lay = src_item:getCount()	
	if src_over_lay == count then
		--走交换位置流程
		self:exchangePos(src_bag, src_pos, dst_bag, dst_pos)
	else
		if src_over_lay < count then
			count = src_over_lay
		end		
		--目的位置添加物品
		self:addItem(src_item:getEntry(), count, src_item:isBind(), src_item:isAppaisal(), true, src_item:getStrongLv(), src_item:getFailTime(), dst_bag, dst_pos, false)		
		--原位置删物品
		self:delItemObj(src_item, count)
	end
	return true
end

--是否超负重
function AppItemMgr:isCanBear(src_item, dst_item)
	local owner = self:getOwner()
	if not owner then return false end
	local cur_bear = 0
	--汇总装备所有负重
	self.itemMgr:ForEachBagItem(BAG_TYPE_EQUIP, function(ptr)
		local item = require("appd.appd_item").new(ptr)
		if item:getId() ~= src_item:getId() then			
			cur_bear = cur_bear + item:getAttr(EQUIP_ATTR_BEAR)
		end
	end)
	--加上要穿的装备负重
	if dst_item then  
		cur_bear = cur_bear + dst_item:getAttr(EQUIP_ATTR_BEAR)
	end
	
	return cur_bear <= owner:GetBear()
end

--获得物品cd
function AppItemMgr:getCoolDown( entry )
	local mstime = getMsTime()
	local tm = self.cool_down_map[entry]
	if tm and tm > mstime then
		return tm - mstime
	end
	return 0
end

--设置物品cd
function AppItemMgr:setCoolDown( entry, cd )
	local mstime = getMsTime()
	self.cool_down_map[entry] = mstime + cd
end

--处理物品冷切
function AppItemMgr:handleCoolDown( entry )
	local item_tempate = tb_item_template[entry]
	--设置物品本身的cd
	self:setCoolDown(entry, item_tempate.cooldown)			
	--处理族cd
	for _, item in pairs(tb_item_template) do
		if item.id ~= entry and item.category ~= 0 and item.category == item_tempate.category then
			self:setCoolDown(item.id, item_tempate.category_cooldown)	
		end
	end
	local list = item_cooldown_info_t:new {["item"] = entry, ["cooldown"] = self:getCoolDown(entry)}
	self:getOwner():call_bag_item_cooldown({list})
end



--判断一组物品的数量是否够{{entry,count},{entry,count} ...} 
function AppItemMgr:hasMulItem(costItemTable,multiple)
	multiple = multiple or 1
	local map = {}

	--判断key是否重复
	for _, costItem in pairs(costItemTable) do

		if map[costItem[1]] ~= nil then
			outFmtError("count mulItem key repeat %d", costItem[1])
			return false
		else 
			map[costItem[1]] = 1
		end
	end

	for _, costItem in pairs(costItemTable) do
		local count = self:countItem(self.default_bag_type,costItem[1])

		if count < (costItem[2]*multiple) then
			return false
		end
	end

	return true

end

-- 生成一种能买材料的扣资源和钱方案
-- 返回
--	true, realCostItemTable, costMoney
-- false, nil, 0
function AppItemMgr:costMoneyEnabledSolution(costItemTable, multiple)
	multiple = multiple or 1
	local map = {}

	--判断key是否重复
	for _, costItem in pairs(costItemTable) do

		if map[costItem[1]] ~= nil then
			outFmtError("count mulItem key repeat %d", costItem[1])
			return false, nil, 0
		else 
			map[costItem[1]] = 1
		end
	end
	
	local costMoney = 0
	local realCostItem = {}
	for _, costItem in pairs(costItemTable) do
		local count = self:countItem(self.default_bag_type, costItem[ 1 ])
		if count < (costItem[ 2 ] * multiple) then
			local buyed = costItem[ 2 ] * multiple - count
			local id = GetShopId(MONEY_TYPE_BIND_GOLD, costItem[ 1 ])
			-- 是否在商城可以买到
			if not tb_shop[id] then
				return false, nil, 0
			end
			-- 先计算需要购买几次
			local gtCount = tb_shop[id].count
			local buys = math.ceil(buyed / gtCount)
			-- 单价不能 < 0
			local price = tb_shop[id].costResource[ 1 ][ 2 ]
			if price < 0 then
				return false, nil, 0
			end 
			costMoney = costMoney + buys * price
			if count > 0 then
				AddTempInfoIfExist(realCostItem, costItem[ 1 ], count)
			end
		else
			AddTempInfoIfExist(realCostItem, costItem[ 1 ], costItem[ 2 ] * multiple)
		end
	end

	return true, realCostItem, costMoney
end




--使用一组物品{{entry,count},{entry,count} ...} 
function AppItemMgr:useMulItem(costItemTable,multiple)
	multiple = multiple or 1

	if not self:hasMulItem(costItemTable,multiple) then
		return false
	end
	
	for _, costItem in pairs(costItemTable) do
		if self:delItem(costItem[1],costItem[2]*multiple,self.default_bag_type) == 0 then
			return false
		end
	end

	return true
end


--熔炼装备
function AppItemMgr:smeltingEquip(pos_str)
	local owner = self:getOwner()
	if not owner then
		outFmtError("exchangePos:not find %s owner!", self.itemMgr:GetGuid())
		return false
	end
	
	local values =  string.split(pos_str, "|")
	local cnt = 0
	local rewardList = {}
	
	for i, str in pairs(values) do
		local pos = tonumber(str)
		if pos then
			local item = self:getBagItemByPos(BAG_TYPE_EQUIP_BAG, pos)
			if item then
				local entry = item:getEntry()
				if (entry) then
					local config = tb_item_template[entry]
					if config then
						
						--[[local drop_id = config.drop_id
						if drop_id and drop_id ~= 0 then
							local dict = {}
							DoRandomDrop(drop_id, dict)
							
							--local rewardDict = {}
							for itemId, value in pairs(dict) do
								if rewardList[itemId] then
									rewardList[itemId] = rewardList[itemId] + value
								else
									rewardList[itemId] = value
								end
							end
							self:delItemObj(item,1)
							--owner:AppdAddItems(rewardDict, MONEY_CHANGE_BOX_RANDOM, LOG_ITEM_OPER_TYPE_OPEN_BOX)
							cnt = cnt + 1
						end--]]
						cnt = cnt + 1
						self:delItemObj(item,1)
						local drop_item = config.drop_item
						mergeListToList(rewardList,drop_item)
					end
				end
			end
		end
	end

	owner:AppdAddItems(rewardList, MONEY_CHANGE_BOX_RANDOM, LOG_ITEM_OPER_TYPE_OPEN_BOX)
	SortItem(owner,true)--强制整理
	-- 加任务
	local questMgr = owner:getQuestMgr()
	questMgr:OnUpdate(QUEST_TARGET_TYPE_SMELT, {cnt})
	
	owner:AddActiveItem(VITALITY_TYPE_SMELT_EQUIP)
end

return AppItemMgr
