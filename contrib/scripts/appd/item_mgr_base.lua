--[[
	物品管理器binlog操作基类
]]
local ItemMgrBase = class("ItemMgrBase", BinLogObject)

function ItemMgrBase:ctor()	
	self.itemMgr = ItemManager()
	self.default_bag_type = BAG_TYPE_MAIN_BAG	--默认包裹类型
	self.default_max_bag = MAX_BAG 				--默认最大包裹类型数量

end

function ItemMgrBase:init( )
	self.itemMgr:Init(self.ptr)

end

--添加物品
--[[
@entry:模板
@count:数量
@isBind:是否绑定(可选参数)
@isAppaisal:是否鉴定(可选参数)
@isSystem:包裹满后放系统包裹(可选参数)
@strong_lv:强化等级(可选参数)
@fail_time:失效时间(可选参数)
@bag_type:包裹类型（没传的话就默认放到主包裹）(可选参数)
@pos:包裹位置
@isOverlay:是否叠加

@return:错误码
]]
function ItemMgrBase:addItem(entry, count, isBind, isAppaisal, isSystem, strong_lv, fail_time, bag_type, pos, isOverlay)
	if count <= 0 then return 1 end
	bag_type = bag_type or self.default_bag_type
	fail_time = fail_time or 0
	strong_lv = strong_lv or 0
	isBind = isBind or false
	if type(isBind) == "number" then
		isBind = isBind ~= 0
	end
	isAppaisal = isAppaisal or true		--默认都鉴定
	isSystem = isSystem or true
	pos = pos or -1
	if isOverlay == nil then isOverlay = true end		--默认不传就处理叠加
	
	if pos ~= -1 and self:getBagItemByPos(bag_type, pos) then
		--要添加包裹位置上已经有物品了
		outFmtError("addItem: bag %d pos %d has item!", bag_type, pos)
		return 2
	end
	
	--包裹满后放系统包裹
	if isSystem and not self:canHold(bag_type, entry, count, isBind, fail_time) then
		bag_type = BAG_TYPE_SYSTEM
	end
	
	local item_tempate = tb_item_template[entry]
	if not item_tempate then return 3 end			--物品模板表里没有这个物品
	
	local max_over_lay = item_tempate.max_overlap			--取出最大叠加数
	if isOverlay then
		--先处理看下能不能叠加
		local canoverlay_items = self:getCanOverlayItems(bag_type, entry, isBind, fail_time, max_over_lay)
		for _, item in ipairs(canoverlay_items) do
			--处理叠加
			local lack = max_over_lay - item:getCount()
			local min_count = lack
			if count < lack then min_count = count end
			item:setCount( item:getCount() + min_count )
			count = count - min_count
			self.itemMgr:SavePtr(item.item)
			if count <= 0 then
				return 0
			end
		end
	end
	
	--执行到这,生成一个新的物品
	while(count > 0)do 
		local empty_count = self:getEmptyCount(bag_type)	--取出剩余位置个数
		if empty_count <= 0 then
			outFmtDebug("addItem: bag is full!")
			return 4 --没空位置了
		end
		--开始新建个物品
		local item = require("appd.appd_item").new()
		item:setEntry(entry)
		local overlay = count
		if overlay > max_over_lay then
			overlay = max_over_lay
		end
		item:setCount(overlay)
		if isBind then item:setBind() end
		if fail_time > 0 then item:setFailTime(fail_time) end
		if strong_lv > 0 then item:setStrongLv(strong_lv) end	
		if isAppaisal and self.itemAppaisal then self:itemAppaisal(item) end
		
		
		local bag_pos = self.itemMgr:Add(item.item, bag_type, pos)
		
		if item_tempate.type == ITEM_TYPE_EQUIP then --and table.find(item_tempate.availableGender, self:getOwner():GetGender()) then
			-- 与背包对应位置装备战力比较
			local equiped_item = self:getBagItemByPos(BAG_TYPE_EQUIP,item_tempate.pos)
	
			if not equiped_item or item:getForce() > equiped_item:getForce() then
				--outFmtInfo("addItem: find better equip %d %d %d ",entry,bag_pos, item:getForce())
				--local item_guid = string.format("%s;%d",self:GetGuid(), result_pos)
				self:getOwner():call_bag_find_equip_better(entry, bag_pos)
			end
		end
		
		count = count - overlay
	end
	return 0
end


--通过字符串构造一个新的物品对象
--@return  true 成功 false 失败
function ItemMgrBase:addItemByStr(bag_type, str, pos)
	if str == "" then return false end
	bag_type = bag_type or self.default_bag_type
	pos = pos or self:getEmptyPos(bag_type)
	local item = require("appd.appd_item").new()
	if not item:fromString(str) then return false end	
	local id = self.itemMgr:Add(item.item, bag_type, pos)	--找个空位放到包裹
	if id >= 0 then return true end	
	return false
end

--获得能继续叠加的物品
function ItemMgrBase:getCanOverlayItems(bag_type, entry, isBind, fail_time, max_over_lay)
	local items = {}
	if type(isBind) == "number" then
		isBind = isBind ~= 0
	end
	self.itemMgr:ForEachBagItem(bag_type, function(ptr)
		local item = require("appd.appd_item").new(ptr)
		if item:getEntry() == entry and item:isBind() == isBind and item:getFailTime() == fail_time and item:getCount() < max_over_lay then
			--todo lhs这里实际还需要判断这个物品是否在交易中
			table.insert(items, item)
		end
	end)
	
	return items
end

--通过模板获得物品
function ItemMgrBase:getItemByEntry(bag_type, entry)
	local items = {}
	self.itemMgr:ForEachBagItem(bag_type, function(ptr)
		local item = require("appd.appd_item").new(ptr)
		if item:getEntry() == entry then			
			table.insert(items, item)
		end
	end)
	return items
end


--获得指定背包的所有物品
function ItemMgrBase:getBagAllItem(bag_type)
	local items = {}
	if not bag_type then return items end
	self.itemMgr:ForEachBagItem(bag_type, function(ptr, pos)
		local item = require("appd.appd_item").new(ptr)
		items[pos] = item	--按照位置存,这样遍历的时候就不能用#和ipairs	--到底要不要存位置呢，根据业务来吧先存着，todo lhs
	end)
	return items
end

--通过模板获得物品个数
--isBind和fail_time为nil的话（即参数不传）则不校验
function ItemMgrBase:countItem(bag_type, entry, isBind, fail_time)
	bag_type = bag_type or self.default_bag_type
	if type(isBind) == "number" then
		isBind = isBind ~= 0
	end
	
	local count = 0
	self.itemMgr:ForEachBagItem(bag_type, function(ptr)
		local item = require("appd.appd_item").new(ptr)
		if entry == item:getEntry() and ( isBind == nil or item:isBind() == isBind )  and (fail_time == nil or item:getFailTime() == fail_time) then
			count = count + item:getCount()
		end		
	end)
	return count
end

--统计所有包裹里物品个数
function ItemMgrBase:countAllItem(entry, isBind, fail_time)
	local count = 0
	for bag = 0, self.default_max_bag-1 do
		count = count + self:countItem(bag, entry, isBind, fail_time)
	end		
	return count
end

--删除指定模板物品
function ItemMgrBase:delItem( entry, count, bag_type )
	bag_type = bag_type or self.default_bag_type
	local has_count = self:countItem(bag_type, entry)
	local bind = SUB_ITEM_NOT_BIND
	if has_count < count then
		--不够扣
		outFmtDebug("delItem: item %d has_count %d need_count %d is not enough!", entry, has_count, count)
		return SUB_ITEM_FAIL
	end
	local need_count = count
	local func = function( item )
		local overlay_count = item:getCount()
		if overlay_count > need_count then
			--够扣，修改下叠加数量
			item:setCount(overlay_count - need_count)
			self.itemMgr:SavePtr(item.item)
			need_count = 0			
		elseif overlay_count <= need_count then
			--移除这个物品
			self.itemMgr:Del(item:getId())
			need_count = need_count - overlay_count			
		end
		if need_count <= 0 then		
			return true
		end
		return false
	end
	
	if need_count > 0 then
		local items = self:getItemByEntry(bag_type, entry)
		--优先扣限时物品
		for _, item in ipairs(items) do
			if item:getFailTime() ~= 0 and func(item) then
				bind = SUB_ITEM_HAVE_FAILTIME
				break
			end
		end
	end		
	
	--次优先扣绑定,非限时的绑定物品
	if need_count > 0 then
		local items = self:getItemByEntry(bag_type, entry)	--每次都得重新获取,因为中途有可能已经删掉了某个物品
		for _, item in ipairs(items) do
			if item:getFailTime() == 0 and item:isBind() and func(item) then
				bind = SUB_ITEM_HAVE_BIND
				break
			end
		end
	end
	--最后在扣非绑定的，非限时非绑定的物品
	if need_count > 0 then
		local items = self:getItemByEntry(bag_type, entry)
		for _, item in ipairs(items) do
			if item:getFailTime() == 0 and not item:isBind() and func(item) then
				break
			end
		end
	end
	if need_count <= 0 then		
		return bind
	end
	return SUB_ITEM_FAIL
end

--删除指定物品对象的物品
function ItemMgrBase:delItemObj( item, count )
	local overlay_count = item:getCount()
	count = count or overlay_count	--不指定要扣的数量就直接整个物品删掉
	if count > overlay_count then
		--不够扣
		outFmtDebug("delItemObj: entry %d count %d overlay_count %d not enough!", item:getEntry(), count, overlay_count)
		return false
	elseif count < overlay_count then
		--直接扣叠加数量就好了
		item:setCount(overlay_count - count)
		self.itemMgr:SavePtr(item.item)
		return true
	elseif count == overlay_count then
		--移除这个物品
		self.itemMgr:Del(item:getId())
		return true
	end
end

--删除指定包裹位置的物品(直接整个物品删掉,比如：销毁物品)
function ItemMgrBase:delItemByPos(bag_type, pos, count)
	local item = self:getBagItemByPos(bag_type, pos)
	if not item then 
		outFmtDebug("delItemByPos: not find item int bag %d pos %d", bag_type, pos)
		return false 
	end
	count = count or item:getCount()	
	return self:delItemObj(item,count)
end

--清空包裹所有物品
function ItemMgrBase:clearBag(bag_type)
	local bag_size = self:getBagSize(bag_type)
	for i = 1, bag_size do
		local item = self:getBagItemByPos(bag_type, i-1)
		if item then
			self:delItemObj(item)
		end
	end
end

--通过物品guid获得物品
function ItemMgrBase:getItemByGuid( guid, bag_type )
	if not bag_type then
		--直接认guid
		local ptr = self.itemMgr:GetItemByGuid(guid)
		if ptr then
			return require("appd.appd_item").new(ptr)
		end
	else
		local item
		--在指定的包裹找
		self.itemMgr:ForEachBagItem(bag_type, function(ptr)
			local item_tmp = require("appd.appd_item").new(ptr)
			if item_tmp:getGuid() == guid then
				item = item_tmp
				return true
			end
		end)
		return item
	end
end

--获得指定包裹的指定位置物品
function ItemMgrBase:getBagItemByPos( bag_type, pos )
	local ptr = self.itemMgr:GetByPos(bag_type, pos)
	local item
	if ptr then
		item = require("appd.appd_item").new(ptr)
	end
	return item
end

--获得背包大小
function ItemMgrBase:getBagSize(bag_type)
	return self.itemMgr:GetBagSize(bag_type)
end

--设置背包大小
function ItemMgrBase:setBagSize(bag_type, size)
	self.itemMgr:SetBagSize(bag_type, size)
end

--获得空位置个数
function ItemMgrBase:getEmptyCount(bag_type)
	bag_type = bag_type or self.default_bag_type
	return self.itemMgr:GetEmptyCount(bag_type)
end

--获得包裹空位置 -1表示没找到
function ItemMgrBase:getEmptyPos(bag_type)
	return self.itemMgr:GetEmptyPos(bag_type)
end

--获得指定物品guid的物品所在的包裹位置 -1表示没找到
function ItemMgrBase:getItemBagPosByGuid(bag_type, guid)
	local bag_size = self:getBagSize(bag_type)
	for i = 1, bag_size do
		local item = self:getBagItemByPos(bag_type, i-1)
		if item and item:getGuid() == guid then
			return i-1
		end
	end
	return -1
end

--校验是否能放下
function ItemMgrBase:canHold(bag_type, entry, count, isBind, fail_time)
	isBind = isBind or false
	if type(isBind) == "number" then
		isBind = isBind ~= 0
	end
	fail_time = fail_time or 0
	local item_tempate = tb_item_template[entry]
	local max_over_lay = item_tempate.max_overlap	
	local can_hold = 0
	--计算还能叠加的
	self.itemMgr:ForEachBagItem(bag_type, function(ptr)
		local item = require("appd.appd_item").new(ptr)
		if item and item:getEntry() == entry and item:isBind() == isBind and item:getFailTime() == fail_time then
			can_hold = can_hold + max_over_lay - item:getCount()			
		end
	end)
	--计算空位置	
	local empty_count = self:getEmptyCount(bag_type)
	can_hold = can_hold + max_over_lay * empty_count	
	return can_hold >= count
end


--获得物品管理器的拥有者
function ItemMgrBase:getOwner()
	--物品管理器guid转玩家guid
	if not self.owner_guid then
		self.owner_guid = guidMgr.replace(self.itemMgr:GetGuid(), guidMgr.ObjectTypePlayer)
	end
	return app.objMgr:getObj(self.owner_guid)	
end


return ItemMgrBase
