--礼包信息
GiftPacksInfo = class('GiftPacksInfo', BinLogObject)

--礼包信息个数加1
function GiftPacksInfo:Next()
	local cur_num = self:GetAttackStrutCount()
	if(cur_num >= MAX_GIFTPACKS_INFO_COUNT-1)then		--礼包信息最多保留MAX_GIFTPACKS_INFO_COUNT
		self:SetAttackStrutCount(0)	
	else
		self:AddUInt32(MAX_GIFTPACKS_INFO_INT_NOW_INDEX, 1)
	end
end	

--礼包信息int开始下标
function GiftPacksInfo:Start()
	return GIFTPACKS_INT_FIELD_BEGIN + self:GetAttackStrutCount() * MAX_GIFTPACKS_INFO_INT
end

--礼包信息string开始下标
function GiftPacksInfo:StringStart()
	return GIFTPACKS_STRING_FIELD_BEGIN + self:GetAttackStrutCount() * MAX_GIFTPACKS_INFO_STRING 
end

--获得礼包信息当前索引
function GiftPacksInfo:GetAttackStrutCount()
	return self:GetUInt32(MAX_GIFTPACKS_INFO_INT_NOW_INDEX)
end

--设置礼包信息当前索引
function GiftPacksInfo:SetAttackStrutCount( val)
	self:SetUInt32(MAX_GIFTPACKS_INFO_INT_NOW_INDEX, val)
end

--获取礼包ID
function GiftPacksInfo:GetGiftPacksID()
	return self:GetUInt32(self:Start() + GIFTPACKS_INFO_INT_ID)
end

--设置礼包ID
function GiftPacksInfo:SetGiftPacksID(val)
	self:SetUInt32(self:Start() + GIFTPACKS_INFO_INT_ID, val)
end

--获取礼包发放时间
function GiftPacksInfo:GetGiftPacksStartTime()
	return self:GetUInt32(self:Start() + GIFTPACKS_INFO_INT_START_TIME)
end

--设置礼包领取时间
function GiftPacksInfo:SetGiftPacksStartTime(val)
	self:SetUInt32(self:Start() + GIFTPACKS_INFO_INT_START_TIME, val)
end

--获取礼包结束时间
function GiftPacksInfo:GetGiftPacksEndTime()
	return self:GetUInt32(self:Start() + GIFTPACKS_INFO_INT_END_TIME)
end

--设置礼包结束时间
function GiftPacksInfo:SetGiftPacksEndTime(val)
	self:SetUInt32(self:Start() + GIFTPACKS_INFO_INT_END_TIME, val)
end

--获取礼包类型
function GiftPacksInfo:GetGiftPacksType()
	return self:GetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE,0)
end

--设置礼包类型
function GiftPacksInfo:SetGiftPacksType(val)
	self:SetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE, 0 ,val)
end

--获取礼包领取状态
function GiftPacksInfo:GetGiftPacksReceive()
	return self:GetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE,1)
end

--设置礼包领取状态
function GiftPacksInfo:SetGiftPacksReceive(val)
	self:SetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE, 1 ,val)
end

--获取礼包阅读状态
function GiftPacksInfo:GetGiftPacksRead()
	return self:GetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE,2)
end

--设置礼包阅读状态
function GiftPacksInfo:SetGiftPacksRead(val)
	self:SetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE, 2 ,val)
end

--获取礼包是否删除
function GiftPacksInfo:GetGiftPacksIsDelete()
	return self:GetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE,3)
end

--设置礼包是否删除
function GiftPacksInfo:SetGiftPacksIsDelete(val)
	self:SetByte(self:Start() + GIFTPACKS_INFO_INT_BYTE, 3 ,val)
end

--获取礼包名称
function GiftPacksInfo:GetGiftPacksName()
	return self:GetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_NAME)
end

--设置礼包名称
function GiftPacksInfo:SetGiftPacksName(val)
	self:SetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_NAME,val)
end

--获取礼包说明
function GiftPacksInfo:GetGiftPacksDesc()
	return self:GetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_DESC)
end

--设置礼包说明
function GiftPacksInfo:SetGiftPacksDesc(val)
	self:SetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_DESC,val)
end

--获取礼包来源
function GiftPacksInfo:GetGiftPacksFrom()
	return self:GetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_FROM)
end

--设置礼包来源
function GiftPacksInfo:SetGiftPacksFrom(val)
	self:SetStr(self:StringStart() + GIFTPACKS_INFO_STRING_GIFT_FROM,val)
end

--获取礼包物品
function GiftPacksInfo:GetGiftPacksItem(pos)
	local start
	if(pos == nil)then
		start = self:StringStart()
	else
		start = GIFTPACKS_STRING_FIELD_BEGIN + pos * MAX_GIFTPACKS_INFO_STRING
	end
	
	return self:GetStr(start + GIFTPACKS_INFO_STRING_GIFT_ITEM)
end

--设置礼包物品
function GiftPacksInfo:SetGiftPacksItem(val, pos)
	local start
	if(pos == nil)then
		start = self:StringStart()
	else
		start = GIFTPACKS_STRING_FIELD_BEGIN + pos * MAX_GIFTPACKS_INFO_STRING
	end
	
	self:SetStr(start + GIFTPACKS_INFO_STRING_GIFT_ITEM,val)
end

--添加一个礼包信息
function GiftPacksInfo:AddGiftPacksInfo(gift_type, start_time, end_time, gift_name, gift_desc, item_config, item_from)
	
	self:SetGiftPacksStartTime(start_time)
	self:SetGiftPacksEndTime(end_time)
	self:SetGiftPacksType(gift_type)
	self:SetGiftPacksReceive(0)
	self:SetGiftPacksRead(0)
	self:SetGiftPacksIsDelete(0)
	
	self:SetGiftPacksName(gift_name)
	self:SetGiftPacksDesc(gift_desc)
	self:SetGiftPacksItem(item_config)
	self:SetGiftPacksFrom(item_from)
	self:Next()
	return 1
end

-- 判断礼包是否存在
function GiftPacksInfo:IsIndexHasMail(indx)
	if indx < 0 or indx >= MAX_GIFTPACKS_INFO_COUNT then
		return false
	end
	local intIndex = GIFTPACKS_INT_FIELD_BEGIN + indx * MAX_GIFTPACKS_INFO_INT
	return self:GetUInt32(intIndex + GIFTPACKS_INFO_INT_START_TIME) > 0
end

-- 设置读
function GiftPacksInfo:mailRead(indx)
	-- 邮件不存在
	if not self:IsIndexHasMail(indx) then
		return
	end
	
	local intIndex = GIFTPACKS_INT_FIELD_BEGIN + indx * MAX_GIFTPACKS_INFO_INT
	-- 已经设置只读
	if self:GetByte(intIndex + GIFTPACKS_INFO_INT_BYTE, 2) == 1 then
		return
	end
	self:SetByte(intIndex + GIFTPACKS_INFO_INT_BYTE, 2, 1)
	
	local strIndex = GIFTPACKS_STRING_FIELD_BEGIN + indx * MAX_GIFTPACKS_INFO_STRING
	local days = 20
	if self:GetStr(strIndex + GIFTPACKS_INFO_STRING_GIFT_ITEM) == "" then
		days = 3
	end
	
	self:SetUInt32(intIndex + GIFTPACKS_INFO_INT_END_TIME, os.time() + days * 24 * 3600)
end

-- 领取礼包
function GiftPacksInfo:pickMail(playerInfo, indx)
	-- 邮件不存在
	if not self:IsIndexHasMail(indx) then
		return
	end
	
	local intIndex = GIFTPACKS_INT_FIELD_BEGIN + indx * MAX_GIFTPACKS_INFO_INT
	
	-- 已经领了的判断和过期/删除的判断
	if self:GetByte(intIndex + GIFTPACKS_INFO_INT_BYTE,1) > 0 or self:GetByte(intIndex + GIFTPACKS_INFO_INT_BYTE,3) > 0 or self:GetUInt32(intIndex+GIFTPACKS_INFO_INT_END_TIME) < os.time() then
		return
	end
	
	local items = self:GetGiftPacksItem(indx)
	-- 没有附件
	if items == "" or string.len(items) == 0 then
		return
	end
	
	
	local itemInfoTable = string.split(items, ",")
	local size = #itemInfoTable / 2
	
	-- 按所属背包分类
	local bagContains = {}
	
	local rewardDict = {}
	for i = 1, #itemInfoTable, 2 do
		local itemId = tonumber(itemInfoTable[ i ])
		local count  = tonumber(itemInfoTable[i+1])
		table.insert(rewardDict, {itemId, count})
		
		local bagType = tb_item_template[itemId].belong_bag
		if bagType >= 0 then
			if not bagContains[bagType] then
				bagContains[bagType] = {}
			end
			bagContains[bagType][itemId] = count
		end
	end
	
	-- 判断背包格子是否足够
	local itemMgr = playerInfo:getItemMgr()
	for bagType, dict in pairs(bagContains) do
		local need = self:GetNeedRoom(dict)
		local emptys  = itemMgr:getEmptyCount(bagType)
		if emptys < need then
			playerInfo:CallOptResult(OPRATE_TYPE_BAG, BAG_RESULT_BAG_FULL)
			return
		end
	end

	-- 设置领取标志
	self:SetByte(intIndex + GIFTPACKS_INFO_INT_BYTE, 1, 1)
	self:SetByte(intIndex + GIFTPACKS_INFO_INT_BYTE, 2, 1)
	self:SetUInt32(intIndex + GIFTPACKS_INFO_INT_END_TIME, os.time() + 3 * 24 * 3600)
	self:SetGiftPacksItem("", indx)
	
	playerInfo:AppdAddItems(rewardDict, MONEY_CHANGE_GIFT_PACKET, LOG_ITEM_OPER_TYPE_GIFT_PACKS, 1, 0, 0, 2)
end

function GiftPacksInfo:GetNeedRoom(dict)
	local room = 0
	for itemId, count in pairs(dict) do
		room = room + count / tb_item_template[itemId].max_overlap
		if (count % tb_item_template[itemId].max_overlap) > 0 then
			room = room + 1
		end
	end
	
	return room
end

-- 删除礼包
function GiftPacksInfo:removeMail(indx)
	-- 邮件不存在
	if not self:IsIndexHasMail(indx) then
		return
	end
	
	local intIndex = GIFTPACKS_INT_FIELD_BEGIN + indx * MAX_GIFTPACKS_INFO_INT
	
	-- 已经删除的判断
	if self:GetByte(intIndex + GIFTPACKS_INFO_INT_BYTE,3) > 0  then
		return
	end
	
	-- 没有读过不让删
	if self:GetByte(intIndex + GIFTPACKS_INFO_INT_BYTE, 2) == 0  then
		return
	end
	
	-- 有附件的不让删除
	if self:GetGiftPacksItem(indx) ~= "" then
		return
	end
	
	-- 设置删除标志
	self:SetByte(intIndex + GIFTPACKS_INFO_INT_BYTE, 3, 1)
	
	-- 如果本条是最后挑数据, 清空本条数据
	local cursor = self:GetAttackStrutCount()
	-- 最后的位置
	local lastPos = (cursor - 1 + MAX_GIFTPACKS_INFO_COUNT) % MAX_GIFTPACKS_INFO_COUNT
	
	-- 领取的就是最后一条
	if indx == lastPos then
		-- 直接清掉
		self:reset(indx)
	else
		self.movable = true
		self:MoveDataAndReset(lastPos, indx)
	end
	
	-- 游标移动到上面
	self:SetAttackStrutCount(lastPos)
end

function GiftPacksInfo:reset(indx)
	local intIndex = GIFTPACKS_INT_FIELD_BEGIN + indx * MAX_GIFTPACKS_INFO_INT
	for i = 0, MAX_GIFTPACKS_INFO_INT-1 do
		self:SetUInt32(intIndex + i, 0)
	end
	
	local strIndex = GIFTPACKS_STRING_FIELD_BEGIN + indx * MAX_GIFTPACKS_INFO_STRING
	for i = 0, MAX_GIFTPACKS_INFO_STRING-1 do
		self:SetStr(strIndex + i, '')
	end
end

function GiftPacksInfo:MoveDataAndReset(indx1, indx2)
	-- 只有标记的才能move
	if not self.movable or indx1 == indx2 then
		return
	end
	self.movable = nil
	
	local intIndex1 = GIFTPACKS_INT_FIELD_BEGIN + indx1 * MAX_GIFTPACKS_INFO_INT
	local intIndex2 = GIFTPACKS_INT_FIELD_BEGIN + indx2 * MAX_GIFTPACKS_INFO_INT
	for i = 0, MAX_GIFTPACKS_INFO_INT-1 do
		self:SetUInt32(intIndex2 + i, self:GetUInt32(intIndex1 + i))
		self:SetUInt32(intIndex1 + i, 0)
	end
	
	local strIndex1 = GIFTPACKS_STRING_FIELD_BEGIN + indx1 * MAX_GIFTPACKS_INFO_STRING
	local strIndex2 = GIFTPACKS_STRING_FIELD_BEGIN + indx2 * MAX_GIFTPACKS_INFO_STRING
	for i = 0, MAX_GIFTPACKS_INFO_STRING-1 do
		self:SetStr(strIndex2 + i, self:GetStr(strIndex1 + i))
		self:SetStr(strIndex1 + i, '')
	end
end

-- 一键领取
function GiftPacksInfo:pickMailOneStep(playerInfo)
	for i = 0, MAX_GIFTPACKS_INFO_COUNT-1 do
		self:pickMail(playerInfo, i)
	end
end

-- 一键删除
function GiftPacksInfo:removeMailOneStep()
	
	-- 如果本条是最后挑数据, 清空本条数据
	local cursor = self:GetAttackStrutCount()
	-- 最后的位置
	local real = cursor - 1 + MAX_GIFTPACKS_INFO_COUNT
	
	for i = real, cursor, -1 do
		local indx = i % MAX_GIFTPACKS_INFO_COUNT
		self:removeMail(indx)
	end
end

return GiftPacksInfo
