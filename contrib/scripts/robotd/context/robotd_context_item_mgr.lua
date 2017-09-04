--初始化精灵管理器
function PlayerInfo:ItemMgrInit()
	
end

local itemMakeDict = {{10001, 10120}, {1000, 9999}, {101, 103}, {20000, 49999}, {50006, 50010}}
-- 随机加道具
function PlayerInfo:GmAddItem()
	local indx = randInt(1, #itemMakeDict)
	local config = itemMakeDict[indx]
	local entry = randInt(config[ 1 ], config[ 2 ])
	--entry = 50001
	local command = string.format("@制造 %d 1", entry)
	outFmtDebug("GmAddItem %s", command)
	self:SendGmCommand(command)
	
	return true
end

-- 选择一个物品  返回item_guid 和 count (item_guid 可能为空, count 可以是任意数)
function PlayerInfo:ChooseItem()
	
	if self.itemMgr == nil then
		return '', -1
	end
	
	local item_guid = ''
	local indx = randInt(0, 1)
	
	--[[
	if (indx == 0) then
		return item_guid, self:RandomInt()
	end
	]]
	
	local items = {}
	self.itemMgr:foreach(function(item)
		table.insert(items, item)
	end)
	
	if #items == 0 then
		outFmtDebug("no item in bag")
		return item_guid, self:RandomInt()
	end
	indx = randInt(1, #items)
	
	local item = items[indx]
	item_guid = item.item_guid
	local count = item.count
	count = self:RandomInt(count)
	
	return item_guid, count
end

-- 交换物品 （穿装备）
function PlayerInfo:Send_Bag_Exchange_Pos()
	local src_bag = self:RandomInt(1)
	local src_pos = self:RandomInt(100)
	local dst_bag = self:RandomInt(1)
	local dst_pos = self:RandomInt(100)
	self:call_bag_exchange_pos (src_bag ,src_pos ,dst_bag ,dst_pos)
	outFmtDebug("Send_Bag_Exchange_Pos %d %d %d %d", src_bag ,src_pos ,dst_bag ,dst_pos)
	
	return true
end

-- 销毁物品
function PlayerInfo:Send_Bag_Destroy()
	local item_guid, num = self:ChooseItem()
	local bag_id = self:RandomInt(1)
	outFmtDebug("Send_Bag_Destroy %s %d %d", item_guid, num, bag_id)
	self:call_bag_destroy (item_guid, num, bag_id)
	
	return true
end

-- 使用道具
function PlayerInfo:Send_Use_Item()
	local item_guid, count = self:ChooseItem()
	outFmtDebug("Send_Use_Item %s %d", item_guid, count)
	self:call_bag_item_user (item_guid ,count)
	
	return true
end

-- 购买商品
function PlayerInfo:Send_Mall_Buy()
	local id = self:RandomInt(#tb_shop)
	local count = self:RandomInt(1)
	local time = 0
	if tb_shop[ id ] then
		count = self:RandomInt(tb_shop[id].count)
		time = self:RandomInt(#tb_shop[id].timeCostResource)
	end
	self:call_mall_buy (id ,count ,time)
end

-- 强化部位
function PlayerInfo:Send_Strength()
	local part = self:RandomInt(10)
	self:call_strength (part)
end