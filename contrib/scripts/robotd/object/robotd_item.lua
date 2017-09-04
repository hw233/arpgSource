RobotdItem = class('RobotdItem', BinLogObject)

--构造函数
function RobotdItem:ctor()
	
end

-- 遍历背包的数据
function RobotdItem:foreach(callback)
	for i = 128, 1000 do
		local itemData = self:GetNeededItemData(i)
		if itemData and callback then
			callback(itemData)
		end
	end
end

function RobotdItem:GetSuitEntryAndBagPosBySuitPos(suitpos, suitEntry)
	suitEntry = suitEntry or 0
	local entry, bagpos

	self:foreach(function(item)
		local config = tb_item_template[item.entry]
		if config.pos > 0 and (suitpos == 0 or suitpos == config.pos) and (suitEntry == 0 or suitEntry == item.entry) then
			entry = item.entry
			bagpos = item.pos
			return
		end
	end)

	return entry, bagpos
end

function RobotdItem:GetItemPosByEntry(entry)
	local pos

	self:foreach(function(item)
		if item.entry == entry then
			pos = item.pos
			return
		end
	end)

	return pos
end

function RobotdItem:GetItemGuidByEntry(entry)
	local item_guid
	self:foreach(function(item)
		if item.entry == entry then
			item_guid = item.item_guid
			return 
		end
	end)
	
	return item_guid
end

function RobotdItem:GetNeededItemData (indx)
	if indx >= self:GetUInt32Len() then
		return
	end
	local itemStr = self:GetStr(indx)
	if (string.len(itemStr) == 0) then
		return
	end
	
	-- 装备位的不用
	local pos = self:GetUInt32(indx)
	if pos > 1000 then
		return
	end
	
	local ary = string.split(itemStr, ";")
	
	local bgItem = {}
	bgItem.pos 	 = pos
	bgItem.id	 = tonumber(ary[ 1 ])
	bgItem.entry = tonumber(ary[ 2 ])
	bgItem.count = tonumber(ary[ 3 ])
	bgItem.dataIndex = indx
	bgItem.item_guid = self:GetGuid() .. ";" .. bgItem.id
	
	return bgItem
end

return RobotdItem