local AppdMongoDB = class('AppdMongoDB')


function AppdMongoDB:ctor( db, async_db )
	self.db = assert(db)
	self.async_db = async_db
end

--扩展一下数据库
function AppdMongoDB:extend( obj, dbNames )
	assert(obj)
	assert(dbNames)
	for k,v in pairs(AppdMongoDB) do
		if (k ~= 'extend') then
			obj[k] = v
		end
	end
	table.foreach(dbNames,function(_,v)
		obj[v] = obj:getDB(v)
	end)
	obj.dbNameCaches = {}
	return obj
end

--传入账号或者玩家GUID获得server_name
function AppdMongoDB:getServerName( s )
	-- '30_38_196280920' 	==> '30_38'
	-- 'p1.2_2590'			==> '2_2590'
	local startIndx = string.find(s, '%w+_%w+')
	if not startIndx then
		return
	end
	return string.sub(s, startIndx)
end

--传入表名拼接出库及表名
function AppdMongoDB:getGMIntfNs( tbl_name )
	return 'gm_intf.'.. tbl_name
end

--取得server库全路径
function AppdMongoDB:getServerNS( tbl_name )
	return 'server.'.. tbl_name
end

--配置库全路径
function AppdMongoDB:getConfigNS( tbl_name )
	return 'cow_config.' .. tbl_name
end

--根据游戏的名称返回数据库，不允许在应用服修改角色表内容
function AppdMongoDB:getCharDB( server_name )
	if not self.dbNameCaches[server_name] then
		self.dbNameCaches[server_name] = string.format('char_%s',server_name)
	end

	return  self:getDB( self.dbNameCaches[server_name] )
end

function AppdMongoDB:getCharSeverNameNs(server_name,ns)
	if(ns ~= nil and ns ~= "")then
		return string.format("char_%s.%s",server_name,ns)
	else
		return string.format("char_%s",server_name)
	end
end

function AppdMongoDB:removeTradeToChar()
	local server_table =  globalGameConfig:GetMergeServerName()
	for i = 1,#server_table
	do
		local c = self.server.trade:find({server_name = server_table[i]},{})
		while c:hasNext() do
			local r = c:next()
			if r then
				self:getCharDB(server_table[i]).trade:insert(r)
			end
		end
	end
end

--传入库名,及库特殊前缀,如果没有传则使用使用本对象的一般配置
function AppdMongoDB:getWorldDB( dbName, this_prefix )
	--真正的前缀由函数指定,没有传入则由对象配置
	--真正的库名由前缀+库名
	local realDbName, realPrefix = dbName, self.db_prefix
	if this_prefix then
		realPrefix = this_prefix
	end
	if realPrefix and string.len(realPrefix) ~= 0 then
		realDbName = string.format('%s_%s', realPrefix, realDbName)
	end
	return self:getDB(realDbName)
end

--插入gm充值表
function AppdMongoDB:setQQRechargeNum( account, amount, type, order_id )
	--self.db:Insert(self:getGMIntfNs('gm_rechage'), {
	mongoInsert(self:getGMIntfNs('gm_recharge'), {
		_id = assert(order_id),
		account = assert(account),
		i_type = tostring(type),
		i_amount = tostring(amount),
		u_adddate = tostring(os.time()),
		i_status = "0",
		server_name = self:getServerName(account)
	})
end

--查询激活码是否存在
function AppdMongoDB:queryJHMInfo(code,jhm_type)
	local wheres = {}
	local values = {}
	wheres["code"] = code
	wheres["i_type"] = tostring(jhm_type)
	local result = mongoQuery(self:getGMIntfNs("jhm_code"), wheres)
	if(result ~= nil and #result > 0)then
		return 1
	end
	return 0
end

--保存激活码
function AppdMongoDB:saveJHMInfo(code,jhm_type)
	local wheres = {}
	wheres["code"] = code
	wheres["i_type"] = tostring(jhm_type)
	mongoInsert(self:getGMIntfNs("jhm_code"), wheres)
end

----------------------------------摊位相关start------------------------------------------
-- 加载摊位数据
function AppdMongoDB:loadTempStallItemData()
	self:checkStallItemData(0)
end

--加载摊位物品信息
function AppdMongoDB:loadStallItemInfo(server_list)
	local results = {}
	for i = 1,#server_list do
		local cond = {}
		cond["server_name"] = server_list[i]
		local load_result = mongoQuery(self:getCharSeverNameNs(server_list[i],"trade"),cond)
		if(load_result ~= nil and #load_result > 1)then
			for j = 1,#load_result do
				table.insert(results,load_result[j])
			end
		end
	end
	return results
end

-- 判断是否是表里面的物品
function AppdMongoDB:checkStallItemData(item_type)
	if self.trade_data[item_type] ~= nil then
		return
	end
	-- trade_data[0] 用于保存本服所有摊位物品 所有物品列表数据

	if self.trade_data[0] == nil then
		self.trade_data[0] = {}
		local stall_list = self:loadStallItemInfo(globalGameConfig:GetMergeServerName())
		local temp
		for k,v in pairs(stall_list) do
			temp = self:setValueToNumber(v)
			table.insert(self.trade_data[0], temp)
		end
	end

	local tItemType = self:getTradeBigType()
	for k,item_type in pairs(tItemType) do
		self.trade_data[item_type] = {}
	end
	for k,v in pairs(self.trade_data[0]) do
		if v and v.i_pos and v.i_entry then
			local item_type = self:getItemType(tonumber(v.i_entry), tonumber(v.i_pos))
			table.insert(self.trade_data[item_type], v)
		end
	end

end

--[[
表结构
s_item_guid		string		物品GUID
s_item_name		string		物品名字
owner_guid		string		拥有者guid
owner_name		string		拥有者名字
i_type			int			物品类型（暂时没有用）
i_entry			int			物品模版
i_count			int			物品数量
i_price			int			出售总价格
i_quality		int			装备品质
i_level			int			装备等级
u_force			int64		装备战斗力
i_gender		int			装备性别
i_pos			int			装备部位
i_callcd		int			吆喝cd
i_money			int			出售单价
data_str		string		物品data_str
]]
--添加物品
function AppdMongoDB:addStallItem(s_item_guid,s_item_name,owner_guid,owner_name,i_type,i_entry,i_count,i_price,
i_quality,i_level,u_force,i_gender,i_pos,data_str)
	if self:isExistItemGuid(s_item_guid) == true then
		-- outDebug("---addStallItem has item guid = "..s_item_guid)
		return false
	end

	local values = {}
	values['s_item_guid'] = s_item_guid or ""
	values['s_item_name'] = s_item_name or ""
	values['owner_guid'] = owner_guid or ""
	values['owner_name'] = owner_name or ""
	values['i_type'] = i_type or 0
	values['i_entry'] = i_entry or 0
	values['i_count'] = i_count or 0
	values['i_price'] = i_price or 0
	values['i_quality'] = i_quality or 0
	values['i_level'] = i_level or 0
	values['u_force'] = u_force or 0
	values['i_gender'] = i_gender or 0
	values['i_pos'] = i_pos or 0
	values['data_str'] = data_str or ""
	values['i_callcd'] = 0
	values['i_money'] = i_price/i_count
	values['server_name'] = self:getServerName(owner_guid)
	--self:getCharDB(self:getServerName(owner_guid)).trade:insert(values)
	local item_type = self:getItemType(i_entry, i_pos)
	self:addTempTradeItemData(item_type, values)
	return true
end

--获取交易大分类型
function AppdMongoDB:getTradeBigType()
	local need_table = {}
	--[[
	for _,trade in ipairs(tb_trade) do
		local isExist = false
		for _,typed in ipairs(need_table) do
			if typed == trade.paging_big then
				isExist = true
				break
			end
		end		
		if not isExist then
			table.insert(need_table,trade.paging_big)
		end
	end
	]]
	return need_table
end

-- 将字符串value转成number
function AppdMongoDB:setValueToNumber(tItem)
	local temp = {}
	for k,v in pairs(tItem) do
		if(tonumber(v))then
			temp[k] = tonumber(v)
		else
			temp[k] = v
		end
	end
	-- 单价重新算下，不然数据库读出来的都是整数了
	if temp['i_price'] and temp['i_count'] and temp['i_count']~=0 then
		temp['i_money'] = temp['i_price']/temp['i_count']
	end
	return temp
end

-- 判断是否是表里面的物品
function AppdMongoDB:isStallItemByEntry(tItem, entry)
	for k,v in pairs(tItem) do
		if v==entry then
			return true
		end
	end
	return false
end

-- 添加物品
function AppdMongoDB:addTempTradeItemData(item_type, value)
	self:checkStallItemData(item_type)
	self:checkStallItemData(0)
	table.insert(self.trade_data[item_type], value)
	table.insert(self.trade_data[0], value)
end

-- 删除物品
function AppdMongoDB:deleteTempTradeItemData(item_type, item_guid)
	self:checkStallItemData(item_type)
	self:checkStallItemData(0)
	for k,v in pairs(self.trade_data[item_type]) do
		if v['s_item_guid'] == item_guid then
			table.remove(self.trade_data[item_type], k)
			break
		end
	end
	for k,v in pairs(self.trade_data[0]) do
		if v['s_item_guid'] == item_guid then
			table.remove(self.trade_data[0], k)
			break
		end
	end
end

-- 获取物品在哪个表里面
function AppdMongoDB:getItemType(entry, pos)
	if pos > 0 and pos ~= 65535 then
		return 1
	end

	local item_template = tb_item_template[entry]
	if not item_template then
		return 1
	end
	
	local category = item_template.category
	for _,trade in pairs(tb_trade) do
		local isexit = false
		for _,item in ipairs(trade.clan) do
			if item == category then
				isexit = true
				break
			end
		end
		if isexit then
			return trade.paging_big
		end
	end

	return 4
end

-- 根据物品族 获取物品
function AppdMongoDB:getItemByClan(clan)
	local need_item_table = {}
	for _,item_config in pairs(tb_item_template) do
		for _,category in ipairs(clan) do
			if item_config.category == category	then
				table.insert(need_item_table, item_config.id)
			end
		end		
	end

	return need_item_table
end

--[[
	item_type,uint32,False,/*物品类型*/
	item_quality,uint8,False,/*装备品质*/
	item_level,uint16,False,/*装备等级*/
	item_gender,uint8,False,/*装备性别*/
	find_str,String,False,/*搜索字符串*/
	page_size,uint32,False,/*页数大小*/
	page_index,uint32,False,/*页数索引*/
]]
--查询物品,index从1开始(c调lua)
function AppdMongoDB:searchTradeItem(item_type, item_quality, item_level, item_gender, find_str, pageSize, pageIndex, item_sort)
	--outFmtError("--------%u,%u,%u,%u,%s,%u,%u", item_type, item_quality, item_level, item_gender, find_str, pageSize, pageIndex)
	local min_index = pageSize*pageIndex
	local max_index = min_index + pageSize
	local pageCount = 0
	local results = {}
	-- 临时表
	local trade_temp = {}
	-- 条件表
	local trade_select = {}
	if item_quality ~= 0 then
		trade_select['i_quality'] = item_quality - 1
	end
	if item_level ~= 0 then
		trade_select['i_level'] = {(item_level-1)*10, item_level*10}
	end
	if item_gender ~= 0 then
		trade_select['i_gender'] = item_gender - 1
	end
	if find_str ~= "" then
		trade_select['s_item_name'] = find_str
	end

	--大分类
	if item_type < 0 then
		item_type = -item_type
	elseif(item_type > 0 and tb_trade[item_type] ~= nil)then--小分类
		--根据物品族去获取物品
		local clan = tb_trade[item_type].clan
		item_type = tb_trade[item_type].paging_big
		if(item_type == 1)then--装备类
			trade_select['i_pos'] = clan[1] - 30001
		else--非装备
			trade_select['i_entry'] = self:getItemByClan(clan)
		end
	end

	--大类型是否存在
	local isExistBigType = false
	local big_type_table = self:getTradeBigType()
	for i = 1, #big_type_table,1 do
		if(big_type_table[i] ~= nil and item_type == big_type_table[i])then
			isExistBigType = true
			break
		end
	end

	-- 这边先把库数据，按大类型和价格排序，保存在表中
	if item_type == 0 or isExistBigType == true then
		self:checkStallItemData(item_type)

		-- 根据条件查找
		local bSel = true
		local config = self.trade_data[item_type]
		for i=1, #config, 1 do
			bSel = true
			for k,v in pairs(trade_select) do
				if config[i] and config[i][k] and v then
					if k == 'i_level' then		-- 等级
						if config[i][k] <= v[1] or config[i][k] > v[2] then
							bSel = false
							break
						end
					elseif k == 's_item_name' then		-- 名称搜索
						if string.find(config[i][k], v) == nil then
							bSel = false
							break
						end
					elseif k == 'i_entry' then		-- 物品模板
						if self:isStallItemByEntry(v, config[i][k]) == false then
							bSel = false
							break
						end

					else
						if config[i][k] ~= v then
							bSel = false
							break
						end
					end
				else
					bSel = false
					break
				end
			end
			if bSel then
				table.insert(trade_temp, config[i])
			end
		end
		pageCount = math.ceil(#trade_temp/pageSize)

		--排序
		if #trade_temp > 1 then
			if item_sort == 0 then
				table.sort(trade_temp, function(a,b) return a.i_price > b.i_price end)
			elseif item_sort == 1 then
				table.sort(trade_temp, function(a,b) return a.i_price < b.i_price end)
			elseif item_sort == 2 then
				table.sort(trade_temp, function(a,b) return a.i_money > b.i_money end)
			elseif item_sort == 3 then
				table.sort(trade_temp, function(a,b) return a.i_money < b.i_money end)
			end
		end
		for i=min_index+1, max_index, 1 do
			if trade_temp[i] then
				local list = stall_list_info_t:new {
					['s_item_guid']=trade_temp[i]['s_item_guid'],
					['s_item_name']=trade_temp[i]['s_item_name'],
					['owner_guid']=trade_temp[i]['owner_guid'],
					['owner_name']=trade_temp[i]['owner_name'],
					['i_entry']=trade_temp[i]['i_entry'],
					['i_count']=trade_temp[i]['i_count'],
					['i_price']=trade_temp[i]['i_price'],
					['u_force']=trade_temp[i]['u_force'],}
				table.insert(results, list)
			end
		end
	end
	return {pageCount, results}

end

-- 获取我的摊位
function AppdMongoDB:getMyTradeItems(guid)
	local pageCount = 0
	local results = {}
	-- 临时表
	local trade_temp = {}
	-- 条件表
	local trade_select = {}
	trade_select['owner_guid'] = guid

	-- 这边先把库数据，按大类型和价格排序，保存在表中
	self:checkStallItemData(0)

	-- 根据条件查找
	local bSel = true
	local config = self.trade_data[0]
	for i=1, #config, 1 do
		bSel = true
		for k,v in pairs(trade_select) do
			if config[i] and config[i][k] and v then
				if config[i][k] ~= v then
					bSel = false
					break
				end
			else
				bSel = false
				break
			end
		end
		if bSel then
			table.insert(trade_temp, config[i])
		end
	end

	for i=1, #trade_temp, 1 do
		if trade_temp[i] then
			local list = stall_goods_t:new {
				['s_item_guid']= trade_temp[i]['s_item_guid'],
				['i_entry']=trade_temp[i]['i_entry'],
				['i_count']=trade_temp[i]['i_count'],
				['i_price']=trade_temp[i]['i_price'],
				['u_force']=trade_temp[i]['u_force'],
				['i_callcd']=trade_temp[i]['i_callcd'],}
			table.insert(results, list)
		end
	end

	return results
end

-- 获取我的摊位物品数(c调lua)
function AppdMongoDB:getMyTradeItemCount(guid)
	self:checkStallItemData(0)
	local count = 0
	for k,v in pairs(self.trade_data[0]) do
		if v and v["owner_guid"] == guid then
			count = count + 1
		end
	end
	return count

end

--验证物品guid是否存在
function AppdMongoDB:isExistItemGuid( guid)
	self:checkStallItemData(0)
	for k,v in pairs(self.trade_data[0]) do
		if v and v["s_item_guid"] == guid then
			return true
		end
	end
	return false

end

--删除物品
function AppdMongoDB:deleteStallItem(item_guid,player_guid)
	local values = {}
	values["s_item_guid"] = item_guid
	values["owner_guid"] = player_guid

	mongoDelete(self:getCharSeverNameNs(self:getServerName(player_guid),"trade"),values)
end

--通过guid获取物品
function AppdMongoDB:getTradeItemByGuid(guid)
	self:checkStallItemData(0)
	for k,v in pairs(self.trade_data[0]) do
		if v and v["s_item_guid"] == guid then
			return v
		end
	end
	return nil

end

-- 摊位物品上架
function AppdMongoDB:doStallAddItem(playerInfo, item_guid, item_num, item_price)
	local player_guid = playerInfo:GetGuid()
	if player_guid == "" then
		return
	end

	--校验数量
	if(item_num <= 0)then
		outDebug("AppdContext::Handle_Stall_Add_Item: item_num error")
		return
	end
	--校验金额
	if(item_price > MAX_MY_STALL_TRADE_PRICE or item_price < MIN_MY_STALL_TRADE_PRICE)then
		outDebug("AppdContext::Handle_Stall_Add_Item: item_price error")
		return
	end

	--检查下摊位包裹是否已满
	local itemMgr = playerInfo:getItemMgr( )
	local stall_bag_empty_num = itemMgr:getEmptyCount(BAG_TYPE_STALL)
	if(stall_bag_empty_num == 0)then
		--怎么会出现摊位背包满了呢？
		--this->GetBags()->GetStallBag()->Clear();
		outDebug("AppdContext::Handle_Stall_Add_Item: stall item full,stall_bag_empty_num = 0")
		return
	end
	local count = self:getMyTradeItemCount(player_guid)
	if(count >= MAX_MY_STALL_TRADE_NUM)then
		outDebug("AppdContext::Handle_Stall_Add_Item: stall is full,count="..count)
		return
	end
	--校验
	local item = itemMgr:getItemByGuid(item_guid,BAG_TYPE_MAIN_BAG)
	if (item == nil)then
		outDebug("AppdContext::Handle_Stall_Add_Item: item = NULL")
		return
	end

	local max_num = item:getCount()
	if (item:isBind())then
		outDebug("AppdContext::Handle_Stall_Add_Item: item isbind")
		return
	end
	local item_entry = item:getEntry()
	local temp = tb_item_template[item_entry]
	if(not temp)then
		outDebug("AppdContext::Handle_Stall_Add_Item: temp is null")
		return
	end
	--不可交易
	if(temp.is_transaction ~= 1)then
		outDebug("AppdContext::Handle_Stall_Add_Item: item not trade")
		return
	end
	if(item_num < 0 or item_num > max_num)then
		outDebug("AppdContext::Handle_Stall_Add_Item: item count error")
		return
	end
	local pos = itemMgr:getEmptyPos(BAG_TYPE_STALL)
	if(pos == -1)then
		outDebug("AppdContext::Handle_Stall_Add_Item: stall full")
		return
	end
	local src_pos = itemMgr:getItemBagPosByGuid(BAG_TYPE_MAIN_BAG,item:getGuid())
	if(src_pos == -1)then
		outDebug("AppdContext::Handle_Stall_Add_Item: main bag pos error")
		return
	end
	if(itemMgr:splitItem(BAG_TYPE_MAIN_BAG,src_pos,BAG_TYPE_STALL,pos,item_num) == false)then
		outDebug("AppdContext::Handle_Stall_Add_Item: item SplitItem fail ")
		return
	end
	local item = itemMgr:getBagItemByPos(BAG_TYPE_STALL,pos)
	if( not item)then
		outDebug("AppdContext::Handle_Stall_Add_Item: item getBagItemByPos null ")
		return
	end
	local data_str, item_name, s_item_guid
	local item_type, item_count, item_qual, item_level, item_force, item_gender, item_pos
	data_str = item:toString()
	s_item_guid = item:getGuid()
	item_name = temp.name
	item_count = item:getCount()
	item_force = item:getForce()
	item_price = item_price
	item_type = 0
	item_qual = temp.quality
	item_level = temp.level
	item_gender = temp.sex
	item_pos = temp.pos

	local success = self:addStallItem(s_item_guid, item_name, player_guid, playerInfo:GetName(), item_type, item_entry, item_count,
		item_price, item_qual, item_level, item_force, item_gender, item_pos, data_str)
	--添加成功
	if (success)then
		self:saveStallItemInfo(s_item_guid, item_name, player_guid, playerInfo:GetName(), item_type, item_entry, item_count,
			item_price, item_qual, item_level, item_force, item_gender, item_pos, data_str)
		--写下日志
		--WriteStall(GetAccount(),player_guid,item:getGuid(), item:getEntry(),item_count, item_price, 0, 0)
		playerInfo:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_SHANGJIA_SUCCESS)
	end
end

--保存摊位物品信息
function AppdMongoDB:saveStallItemInfo(item_guid, item_name, owner_guid, owner_name, item_type, item_entry, item_count,
			item_price, item_qual, item_level, item_force, item_gender, item_pos, data_str)
	local values = {}
	values["s_item_guid"] = item_guid
	values["s_item_name"] = item_name
	values["owner_guid"] = owner_guid
	values["owner_name"] = owner_name
	values["i_type"] = tostring(item_type)
	values["i_entry"] = tostring(item_entry)
	values["i_count"] = tostring(item_count)
	values["i_price"] = tostring(item_price)
	values["i_quality"] = tostring(item_qual)
	values["i_level"] = tostring(item_level)
	values["u_force"] = tostring(item_force)
	values["i_gender"] = tostring(item_gender)
	values["i_pos"] = tostring(item_pos)
	values["data_str"] = data_str
	values["i_callcd"] = tostring(0);
	values["i_money"] = tostring(item_price/item_count)
	values["server_name"] = self:getServerName(owner_guid)
	mongoInsert(self:getCharSeverNameNs(self:getServerName(owner_guid),"trade"),values)
end

-- 摊位物品下架
function AppdMongoDB:doStallClose(playerInfo, item_guid)
	local player_guid = playerInfo:GetGuid()
	if player_guid == "" then
		return
	end
	local itemMgr = playerInfo:getItemMgr( )
	if (itemMgr:getEmptyCount() < 1) then	--是否装得下物品
		playerInfo:CallOptResult(OPRATE_TYPE_BAG,BAG_RESULT_BAG_FULL)
		return
	end

	local c = self:getTradeItemByGuid(item_guid)
	if c then
		self:deleteStallItem(item_guid, player_guid)
		if itemMgr:addItemByStr(BAG_TYPE_MAIN_BAG, c.data_str) then
			playerInfo:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_XIAJIA_SUCCESS)
		end
		local item_type = self:getItemType(c.i_entry, c.i_pos)
		self:deleteTempTradeItemData(item_type, item_guid)
	else
		playerInfo:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_NO_ITEM)
	end
end

-- 摊位购买物品
function AppdMongoDB:doStallBuy(player, item_guid, reserve)
	local itemMgr = player:getItemMgr( )
	if (itemMgr:getEmptyCount() < 1) then	--是否装得下物品
		player:CallOptResult(OPRATE_TYPE_BAG,BAG_RESULT_BAG_FULL)
		return
	end

	local c = self:getTradeItemByGuid(item_guid)
	if not c then
		player:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_NO_ITEM)
		return
	end
	if player:GetGuid() == c.owner_guid then
		outDebug("doStallBuy do not buy self item")
		return
	end
	local owner_guid = c.owner_guid
	local price = c.i_price
	assert(price > 0)
	if player:SubMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_TYPE_STALL_BUY, price, owner_guid, c.i_entry, c.i_count) == false then
		return
	end
	self:deleteStallItem(item_guid, owner_guid)
	local record_name = reserve == "" and player:GetName() or TEXT_SHENMIDAKA
	local record_guid = reserve == "" and player:GetGuid() or "0"
	itemMgr:addItemByStr(BAG_TYPE_MAIN_BAG, c.data_str)
	WriteItemLog(player, LOG_ITEM_OPER_TYPE_STALL_BUY, c.i_entry, c.i_count, ITEM_BIND_NONE)
	player:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_BUY_SUCCESS)
	local owner, online = playerLib.GetPlayer(owner_guid)
	if owner == nil then
		local data = {}
		data.my_guid = player:GetGuid()
		data.callback_guid = owner_guid
		data.target_guid = owner_guid
		data.record_name = record_name
		data.record_guid = record_guid
		data.name = "doStallBuy"
		function data.fun(data,objs)
			price = math.ceil(price * 0.8)
			local ownerPlayer = objs[data.callback_guid]
			if ownerPlayer then				
				ownerPlayer:AddStallBillsNum(price)
				local ownerStallInfo = ownerPlayer:getStallTradeRecordInfo()
				if ownerStallInfo then
					ownerStallInfo:AddStallTradeRecord(data.record_guid, data.record_name, c.i_entry, c.i_count, 0, c.i_price, STALL_TRADE_EVENT_TYPE_SELL, 0)
				end
			end
		end
		GetObjects(data)
	else
		local ownerInfo = app.objMgr:fromPtr(owner)
		price = math.ceil(price * 0.8)
		ownerInfo:AddStallBillsNum(price)
		local ownerStallInfo = ownerInfo:getStallTradeRecordInfo()
		if ownerStallInfo then
			ownerStallInfo:AddStallTradeRecord(record_guid, record_name, c.i_entry, c.i_count, 0, c.i_price, STALL_TRADE_EVENT_TYPE_SELL, 0)
		end
		ownerInfo:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_SALE_SUCCESS,{c.i_entry,price})
	end

	local playerStallInfo = player:getStallTradeRecordInfo()
	if(playerStallInfo)then
		playerStallInfo:AddStallTradeRecord(c.owner_guid, c.owner_name, c.i_entry, c.i_count, 0, c.i_price, STALL_TRADE_EVENT_TYPE_BUY, 0)
	end
	writeStallTradeLog(player.ptr, c.s_item_guid, c.owner_guid, c.owner_name, c.i_entry, c.i_count, c.i_price)
	local item_type = self:getItemType(c.i_entry, c.i_pos)
	self:deleteTempTradeItemData(item_type, item_guid)
end

-- 清空系统回购数据
function AppdMongoDB:doClearSystenBuy()
	globalCounter:SetSystemBuyPlayerGuid("")
	globalCounter:SetSystemBuyMoney(0)
end

-- 摊位系统回购
function AppdMongoDB:doSystenBuyStallItem()
	self:checkStallItemData(0)
	local money = 1000 - globalCounter:GetSystemBuyMoney()
	if money <= 0 then
		return
	end
	local tPlayerGuid = {}
	local str = globalCounter:GetSystemBuyPlayerGuid()
	if str ~= "" then
		local temp = lua_string_split(str, "|")
		for k,guid in pairs(temp) do
			tPlayerGuid[guid] = 1
		end
	end
	local count = 0
	for k,c in pairs(self.trade_data[0]) do
		-- 大前提、上架物品数量<=2且总价<=50元宝
		if c and c.i_count <= 2 and c.i_price <= 50 then
			if tPlayerGuid[c.owner_guid] == nil then
				local owner= app.objMgr:getObj(c.owner_guid)		-- 玩家必须在线
				if owner ~= nil then
					local bCanBuy = false
					local ownerInfo = app.objMgr:fromPtr(owner)
					local rechange_sum = ownerInfo:GetRechageSum()
					-- 充值金额<1元
					if rechange_sum < 10 then
						bCanBuy = true
					-- 30元>=充值金额>=1元，则上架物品数量<=2且总价<=20元宝
					elseif rechange_sum <= 300 and c.i_price <= 20 then
						bCanBuy = true
					end
					-- 符合购买条件
					if bCanBuy then
						-- 将顽疾guid存起来
						tPlayerGuid[c.owner_guid] = 1
						-- 本次购买次数+1
						count = count + 1
						local price = c.i_price
						-- 今天购买钱数扣掉
						money = money - price
						-- 删除物品
						self:deleteStallItem(c.s_item_guid, c.owner_guid)
						-- 最后要给玩家money
						price = math.ceil(price * 0.8)
						ownerInfo:AddStallBillsNum(price)
						local ownerStallInfo = ownerInfo:getStallTradeRecordInfo()
						if ownerStallInfo then
							ownerStallInfo:AddStallTradeRecord("0", TEXT_SHENMIDAKA, c.i_entry, c.i_count, 0, c.i_price, STALL_TRADE_EVENT_TYPE_SELL, 0)
						end
						ownerInfo:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_SALE_SUCCESS,{c.i_entry,price})
						-- 删除缓存数据
						local item_type = self:getItemType(c.i_entry, c.i_pos)
						table.remove(self.trade_data[0], k)
						for i,v in pairs(self.trade_data[item_type]) do
							if v['s_item_guid'] == c.s_item_guid then
								table.remove(self.trade_data[item_type], i)
								break
							end
						end
						if money <= 0 or count >= 10 then
							break
						end
					end
				end
			end
		end
	end
	globalCounter:SetSystemBuyMoney(1000 - money)
	local temp = {}
	for k,v in pairs(tPlayerGuid) do
		table.insert(temp, k)
	end
	local str = table.concat(temp, "|")
	globalCounter:SetSystemBuyPlayerGuid(str)
end

-- 设置吆喝cd
function AppdMongoDB:doUpdateYaoHeTime(player, item_guid)
	--先等级判断
	if(player:GetLevel() < 30)then
		return
	end
	--先验证一下发言屏蔽规则
	if(player:CheckChatLimit(CHAT_TYPE_TRADE, "    ") == false)then
		return
	end
	local c = self:getTradeItemByGuid(item_guid)
	if not c then
		-- outDebug("doStallBuy not item guid = "..item_guid)
		player:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_NO_ITEM)
		return
	end
	if player:GetGuid() ~= c.owner_guid then
		return
	end
	local cur_time = os.time()
	if c.i_callcd > cur_time then
		return
	end
	local count = 0
	for i=0, #self.trade_data[0], 1 do
		if self.trade_data[0][i] and self.trade_data[0][i]["owner_guid"] == c.owner_guid then
			self.trade_data[0][i]["i_callcd"] = cur_time + 10
			count = count + 1
			if count >= 10 then
				break
			end
		end
	end
	-- self.server.trade:update({ s_item_guid = item_guid},
		-- {['$set'] = { i_callcd = cur_time + 10}},
		-- false --如果不存在则不插入
	-- )
	app:CallOptResult(OPRATE_TYPE_STALL, STALL_OPT_YAOHE_SUCCESS, string.format("%d|%d|%d|%s|%s|%s", c.i_entry, c.i_count, c.i_price, c.owner_guid, c.owner_name, c.s_item_guid))
end

-- 查询摊位物品信息
function AppdMongoDB:doQueryStallItemInfo(player, item_guid, callback_id)
	local c = self:getTradeItemByGuid(item_guid)
	if c then
		local queryItem = require('appd.object.query_item_info').new(getQueryItemPtr(c.owner_guid))
		local item_str = tostring(BAG_TYPE_STALL)..';'..tostring(c.i_pos)..";"..c.data_str
		queryItem:addItemQueryInfo(item_str)
		playerLib.AddQueryBinlog(player:GetGuid(), queryItem.ptr)
		--发送查询结果
		sendQueryResultInfo(player:GetGuid(), callback_id)
	else
		-- outDebug("doStallBuy not item guid = "..item_guid)
		player:CallOptResult(OPRATE_TYPE_STALL,STALL_OPT_NO_ITEM)
	end
end

--打印信息
function AppdMongoDB:printMongodbInfo(result)
	for k,v in pairs(result) do
		outDebug("--------------k = "..k)
		if v.s_item_guid then
		outDebug("----s_item_guid = "..v.s_item_guid)
		end
		if v.s_item_name then
		outDebug("----s_item_name = "..v.s_item_name)
		end
		if v.owner_guid then
		outDebug("----owner_guid = "..v.owner_guid)
		end
		if v.owner_name then
		outDebug("----owner_name = "..v.owner_name)
		end
		if v.buyer_guid then
		outDebug("----buyer_guid = "..v.buyer_guid)
		end
		if v.buyer_name then
		outDebug("----owner_name = "..v.buyer_name)
		end
		if v.i_index then
		outDebug("----i_index = "..v.i_index)
		end
		if v.i_type then
		outDebug("----i_type = "..v.i_type)
		end
		if v.i_entry then
		outDebug("----i_entry = "..v.i_entry)
		end
		if v.i_count then
		outDebug("----i_count = "..v.i_count)
		end
		if v.i_price then
		outDebug("----i_price = "..v.i_price)
		end
		if v.i_money then
		outDebug("----i_money = "..v.i_money)
		end
		if v.i_quality then
		outDebug("----i_quality = "..v.i_quality)
		end
		if v.i_level then
		outDebug("----i_level = "..v.i_level)
		end
		if v.u_force then
		outDebug("----u_force = "..v.u_force)
		end
		if v.i_gender then
		outDebug("----i_gender = "..v.i_gender)
		end
		if v.i_pos then
		outDebug("----i_pos = "..v.i_pos)
		end
		if v.i_callcd then
		outDebug("----i_callcd = "..v.i_callcd)
		end
		--outDebug("----data_str = "..v.data_str)
	end
end

-------求购
--[[
表结构
buyer_guid		string		求购者guid
buyer_name		string		求购者名字
s_item_guid		string		物品名称
i_entry			int			物品模版
i_count			int			物品数量
i_price			int			出售总价格
i_index			int			摊位索引
]]

-- 加载求购摊位数据
function AppdMongoDB:loadTempTradeBuyItemData()
	self:checkStallBuyItemData(0)
end

-- 判断求购摊位是否是表里面的物品
function AppdMongoDB:checkStallBuyItemData(item_type)
	if self.trade_buy_data[item_type] ~= nil then
		return
	end
	-- trade_buy_data[0] 用于保存本服所有求购摊位物品 所有物品列表数据

	if self.trade_buy_data[0] == nil then
		self.trade_buy_data[0] = {}
		local temp
		for k,server_name in pairs(globalGameConfig:GetMergeServerName()) do
			local ns = self:getCharSeverNameNs(server_name,"trade_buy")
			local result = mongoQuery(ns, {})
			for k,v in pairs(result) do
				temp = self:setStallBuyValueToNumber(v)
				table.insert(self.trade_buy_data[0], temp)
			end
		end
	end
	local tItemType = self:getTradeBigType()
	for k,item_type in pairs(tItemType) do
		self.trade_buy_data[item_type] = {}
	end
	for k,v in pairs(self.trade_buy_data[0]) do
		if v and v.i_entry then
			local item_type = self:getStallBuyItemType(v.i_entry)
			table.insert(self.trade_buy_data[item_type], v)
		end
	end

end

-- 设置求购摊位信息
function AppdMongoDB:setStallBuyValueToNumber(tItem)
	local temp = {}
	for k,v in pairs(tItem) do
		if(tonumber(v))then
			temp[k] = tonumber(v)
		else
			temp[k] = v
		end
	end
	-- 单价重新算下，不然数据库读出来的都是整数了
	if temp['i_price'] and temp['i_count'] and temp['i_count']~=0 then
		temp['i_money'] = temp['i_price']/temp['i_count']
	end
	temp['i_callcd'] = 0

	return temp
end

--通过Index获取求购摊位物品
function AppdMongoDB:getTradeBuyItemByIndex(player_guid, index)
	self:checkStallBuyItemData(0)
	for k,v in pairs(self.trade_buy_data[0]) do
		if v and v['buyer_guid'] == player_guid and v['i_index'] == index then
			return v
		end
	end
	return nil

end

-- 获取求购摊位物品在哪个表里面
function AppdMongoDB:getStallBuyItemType(entry)
	local tem = tb_item_template[entry]
	local pos = -1
	if(tem ~= nil)then
		pos = tem.pos
	end
	return self:getItemType(entry,pos)
end

-- 添加求购摊位物品
function AppdMongoDB:addTempTradeBuyItemData(item_type, value)
	local temp = self:setStallBuyValueToNumber(value)
	self:checkStallBuyItemData(item_type)
	self:checkStallBuyItemData(0)
	table.insert(self.trade_buy_data[item_type], temp)
	table.insert(self.trade_buy_data[0], temp)
end

-- 删除求购摊位物品
function AppdMongoDB:deleteTempTradeBuyItemData(item_type, player_guid, index)
	self:checkStallBuyItemData(item_type)
	self:checkStallBuyItemData(0)
	for k,v in pairs(self.trade_buy_data[item_type]) do
		if v['buyer_guid'] == player_guid and v['i_index'] == index then
			table.remove(self.trade_buy_data[item_type], k)
			break
		end
	end
	for k,v in pairs(self.trade_buy_data[0]) do
		if v['buyer_guid'] == player_guid and v['i_index'] == index then
			table.remove(self.trade_buy_data[0], k)
			break
		end
	end
end

-- 获取我的求购摊位物品数
function AppdMongoDB:doGetTradeBuyItemCount(player_guid)
	self:checkStallBuyItemData(0)
	local count = 0
	for k,v in pairs(self.trade_buy_data[0]) do
		if v and v["buyer_guid"] == player_guid then
			count = count + 1
		end
	end
	return count
end

--查询求购摊位物品,index从1开始
function AppdMongoDB:doSearchTradeBuyItem(player, item_type, find_str, pageSize, pageIndex, item_sort)
	--outFmtError("--------%u,%s,%u,%u", item_type, find_str, pageSize, pageIndex)
	local pageCount = 0
	local results = {}
	local min_index = pageSize*pageIndex
	local max_index = min_index + pageSize
	-- 临时表
	local trade_temp = {}
	-- 条件表
	local trade_select = {}
	if find_str ~= "" then
		trade_select['s_item_name'] = find_str
	end

	--大分类
	if item_type < 0 then
		item_type = -item_type
	elseif(item_type > 0 and tb_trade[item_type] ~= nil)then--小分类
		--根据物品族去获取物品
		local clan = tb_trade[item_type].clan
		item_type = tb_trade[item_type].paging_big
		if(item_type ~= 1)then--非装备
			trade_select['i_entry'] = self:getItemByClan(clan)
		end
	end

	--大类型是否存在
	local isExistBigType = false
	local big_type_table = self:getTradeBigType()
	for i = 1, #big_type_table,1 do
		if(big_type_table[i] ~= nil and item_type == big_type_table[i])then
			isExistBigType = true
			break
		end
	end

	-- 这边先把库数据，按大类型和价格排序，保存在表中
	if item_type == 0 or isExistBigType == true then
		self:checkStallBuyItemData(item_type)

		-- 根据条件查找
		local bSel = true
		local config = self.trade_buy_data[item_type]
		for i=1, #config, 1 do
			bSel = true
			for k,v in pairs(trade_select) do
				if config[i] and config[i][k] and v then
					if k == 's_item_name' then		-- 名称搜索
						if string.find(config[i][k], v) == nil then
							bSel = false
							break
						end
					elseif k == 'i_entry' then		-- 物品模板
						if self:isStallItemByEntry(v, config[i][k]) == false then
							bSel = false
							break
						end
					else
						if config[i][k] ~= v then
							bSel = false
							break
						end
					end
				else
					bSel = false
					break
				end
			end
			if bSel then
				table.insert(trade_temp, config[i])
			end
		end
		pageCount = math.ceil(#trade_temp/pageSize)
		--排序
		if #trade_temp > 1 then
			if item_sort == 0 then
				table.sort(trade_temp, function(a,b) return a.i_price > b.i_price end)
			elseif item_sort == 1 then
				table.sort(trade_temp, function(a,b) return a.i_price < b.i_price end)
			elseif item_sort == 2 then
				table.sort(trade_temp, function(a,b) return a.i_money > b.i_money end)
			elseif item_sort == 3 then
				table.sort(trade_temp, function(a,b) return a.i_money < b.i_money end)
			end
		end
		for i=min_index+1, max_index, 1 do
			if trade_temp[i] then
				table.insert(results, {
					['i_index']=trade_temp[i]['i_index'],
					['buyer_guid']=trade_temp[i]['buyer_guid'],
					['buyer_name']=trade_temp[i]['buyer_name'],
					['i_entry']=trade_temp[i]['i_entry'],
					['i_count']=trade_temp[i]['i_count'],
					['i_price']=trade_temp[i]['i_price'],}
					)
			end
		end
	end
	--下发查询结果给客服端 
	--ps:因为求购的没有搬过来，所以这里就没法改成新的发包方式了，所以先注释掉
	--packetLib.Call_stall_buy_get_list_result(player.ptr, {list = results, page_count = pageCount, page_index = pageIndex})
end

-- 获取我的求购摊位
function AppdMongoDB:doMyTradeBuyItem(player, guid)
	if guid == "" then
		return
	end
	local pageCount = 0
	local results = {}
	-- 临时表
	local trade_temp = {}
	-- 条件表
	local trade_select = {}
	trade_select['buyer_guid'] = guid

	-- 这边先把库数据，按大类型和价格排序，保存在表中
	self:checkStallBuyItemData(0)

	-- 根据条件查找
	local bSel = true
	local config = self.trade_buy_data[0]
	for i=1, #config, 1 do
		bSel = true
		for k,v in pairs(trade_select) do
			if config[i] and config[i][k] and v then
				if config[i][k] ~= v then
					bSel = false
					break
				end
			else
				bSel = false
				break
			end
		end
		if bSel then
			table.insert(trade_temp, config[i])
		end
	end

	for i=1, #trade_temp, 1 do
		if trade_temp[i] then
			table.insert(results, {
				['i_index']=trade_temp[i]['i_index'],
				['i_entry']=trade_temp[i]['i_entry'],
				['i_count']=trade_temp[i]['i_count'],
				['i_price']=trade_temp[i]['i_price'],}
				)
		end
	end
	--下发查询结果给客服端
	--ps:因为求购的没有搬过来，所以这里就没法改成新的发包方式了，所以先注释掉
	--packetLib.Call_stall_buy_see_result(player.ptr, {list = results, player_guid = guid})

end

-- 求购摊位设置吆喝cd
function AppdMongoDB:doUpdateStallBuyYaoHeTime(player, index)
	if index == nil then
		return
	end
	--先等级判断
	local player_guid = player:GetGuid()

	if(player:GetLevel() < 30)then
		return
	end
	--先验证一下发言屏蔽规则
	if(playerLib.CheckChatLimit(player.ptr, CHAT_TYPE_TRADE, "    ") == false)then
		return
	end
	-- 没有找到该求购摊位信息
	local c = self:getTradeBuyItemByIndex(player_guid, index)
	if not c then
		player:CallOptResult(OPRATE_TYPE_STALL,STALL_BUY_OPT_NO_ITEM)
		return
	end

	local cur_time = os.time()
	if c.i_callcd > cur_time then
		return
	end
	local count = 0
	for i=0, #self.trade_buy_data[0], 1 do
		if self.trade_buy_data[0][i] and self.trade_buy_data[0][i]["buyer_guid"] == c.buyer_guid then
			self.trade_buy_data[0][i]["i_callcd"] = cur_time + 10
			count = count + 1
			if count >= 10 then
				break
			end
		end
	end

	app:CallOptResult(OPRATE_TYPE_STALL, STALL_BUY_OPT_YAOHE_SUCCESS, string.format("%d|%d|%d|%d|%s|%s", c.i_index, c.i_entry, c.i_count, c.i_price, c.buyer_guid, c.buyer_name))
end

-- 添加我的求购物品
function AppdMongoDB:addStallBuyItem(player, i_entry, i_count, i_price)
	if i_entry <= 0 then
		return
	end
	if i_count <= 0 or i_count > 999 then
		return
	end
	if i_price > MAX_MY_STALL_TRADE_PRICE or i_price < MIN_MY_STALL_TRADE_PRICE then
		return
	end
	local player_guid = player:GetGuid()
	local player_name = player:GetName()
	local server_name = self:getServerName(player_guid)
	local index = playerLib.GetStallBuyNum(player.ptr)
	if index == nil then
		outFmtError("addStallBuyItem item is not can trade index == nil, player guid = %s entry = %d", player_guid, i_entry)
		return
	end
	local max_count = 1
	--黄金vip可发布5个求购信息
	if player:IsHasVipPrivilege(STALL_BUY_ADD_ITEM) then
		max_count = 5
	end
	-- 发布条数是否达到上限
	if self:doGetTradeBuyItemCount(player_guid) >= max_count then
		outFmtError("addStallBuyItem add item has max, player guid = %s max_count = %d", player_guid, max_count)
		return
	end
	-- 在判断一下该物品是否可交易
	local config = tb_item_template[i_entry]
	if config == nil or config.is_transaction ~= 1 then
		outFmtError("addStallBuyItem item is not can trade, player guid = %s entry = %d", player_guid, i_entry)
		return
	end
	-- 交易数量上限不能超过叠加数
	if i_count > config.max_overlap then
		outFmtError("addStallBuyItem item is not can trade count has max, player guid = %s entry = %d", player_guid, i_entry)
		return
	end
	-- 如果已经上架了，说明index 有错了
	if self:getTradeBuyItemByIndex(player_guid, index) then
		outFmtError("addStallBuyItem index error , player guid = %s entry = %d index = %u", player_guid, i_entry, index)
		return
	end
	-- 钱够不够
	if player:GetMoney(MONEY_TYPE_GOLD_INGOT) < i_price then
		outFmtError("addStallBuyItem  money not enought, player guid = %s entry = %d i_price = %u", player_guid, i_entry, i_price)
		return
	end
	-- 扣钱
	if (player:SubMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_TYPE_STALL_BUY_ADD_ITEM, i_price, player_guid, i_entry, i_count) == false) then
		return
	end
	local values = {}
	values['i_index'] = tostring(index)
	values['buyer_guid'] = player_guid
	values['buyer_name'] = player_name
	values['i_entry'] = tostring(i_entry)
	values['i_count'] = tostring(i_count)
	values['i_price'] = tostring(i_price)
	values['s_item_name'] = config.name
	-- 插入数据库
	local ns = self:getCharSeverNameNs(server_name,"trade_buy")
	mongoInsert(ns, values)
	-- 插入缓存
	local item_type = self:getStallBuyItemType(i_entry)
	self:addTempTradeBuyItemData(item_type, values)
	player:CallOptResult(OPRATE_TYPE_STALL,STALL_BUY_OPT_SHANGJIA_SUCCESS)

	-- 写下求购上架日志
	WriteStallBuyLog(player_guid, "", "", LOG_STALL_BUY_SHANGJIA, i_entry, i_count, i_price, index)
end

-- 求购摊位物品下架
function AppdMongoDB:doStallBuyClose(player, index)
	local player_guid = player:GetGuid()
	-- 物品存在？
	local c = self:getTradeBuyItemByIndex(player_guid, index)
	if c then
		-- 删除求购摊位信息
		local server_name = self:getServerName(player_guid)
		local ns = self:getCharSeverNameNs(server_name,"trade_buy")
		local wheres = {}
		wheres['i_index'] = tostring(index)
		wheres['buyer_guid'] = player_guid
		-- 从数据库中删除
		mongoDelete(ns, wheres)
		local item_type = self:getStallBuyItemType(c.i_entry)
		-- 从缓存中删除
		self:deleteTempTradeBuyItemData(item_type, player_guid, index)
		-- 钱还给你
		player:AddMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_TYPE_STALL_BUY_CLOSE_ITEM, c.i_price, player_guid, c.i_entry, c.i_count)
		player:CallOptResult(OPRATE_TYPE_STALL,STALL_BUY_OPT_XIAJIA_SUCCESS)

		-- 写下求购下架日志
		WriteStallBuyLog(player_guid, "", "", LOG_STALL_BUY_CLOSE, c.i_entry, c.i_count, c.i_price, c.i_index)
	else
		player:CallOptResult(OPRATE_TYPE_STALL,STALL_BUY_OPT_NO_ITEM)
	end

end

-- 求购摊位卖给他
function AppdMongoDB:doStallBuySell(player, item_guid, buyer_guid, index, reserve)
	local player_guid = player:GetGuid()
	-- 物品存在？
	local c = self:getTradeBuyItemByIndex(buyer_guid, index)
	if not c then
		player:CallOptResult(OPRATE_TYPE_STALL,STALL_BUY_OPT_NO_ITEM)
		return
	end
	-- 不能是自己卖给自己
	if player_guid == c.buyer_guid then
		outDebug("doStallBuySell do not sell self item")
		return
	end
	local itemMgr = player:getItemMgr()
	local item = itemMgr:getItemByGuid(item_guid, BAG_TYPE_MAIN_BAG)
	--物品不存在
	if(item == nil)then
		outFmtError("doStallBuySell item_ptr == nil item_guid = %s", item_guid)
		return
	end
	local entry = item:getEntry()
	local item_entry = c.i_entry
	local item_num = c.i_count
	-- 物品数量够吗？
	if item:getCount() < c.i_count then
		return
	end
	-- 删除成功？
	if not itemMgr:delItemObj(item,c.i_count) then
		return
	end
	local owner_guid = c.buyer_guid
	assert(c.i_price > 0)
	local price = math.ceil(c.i_price * 0.8)
	-- 删除求购摊位信息
	local server_name = self:getServerName(c.buyer_guid)
	local ns = self:getCharSeverNameNs(server_name,"trade_buy")
	local wheres = {}
	wheres['i_index'] = tostring(c.i_index)
	wheres['buyer_guid'] = c.buyer_guid
	-- 从数据库中删除
	mongoDelete(ns, wheres)
	local item_type = self:getStallBuyItemType(c.i_entry)
	-- 从缓存中删除
	self:deleteTempTradeBuyItemData(item_type, c.buyer_guid, c.i_index)
	-- 获得元宝
	player:AddMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_TYPE_STALL_BUY_SELL_ITEM, price, c.buyer_guid, c.i_entry, c.i_count)
	player:CallOptResult(OPRATE_TYPE_STALL,STALL_BUY_OPT_SALE_SUCCESS)
	local record_name = reserve == 0 and player:GetName() or TEXT_SHENMIDAKA
	local record_guid = reserve == 0 and player:GetGuid() or "0"
	local cur_tm = os.time()
	local str = TEXT_SHENMIDAKA
	if reserve == 0 then		-- 如果不是神秘大咖
		-- local sName = record_name
		-- local server_id = ""
		-- local player_name = ""
		-- if sName ~= "" then
			-- local tName = lua_string_split(sName,",")
			-- server_id = tName[2] or "1"
			-- player_name = tName[3] or ""
			-- if tName[1] == PLATFORM_GAME2 then
				-- server_id = tostring(tonumber(server_id)-1)
			-- end
		-- end
		-- str = string.format("s%s.%s", server_id, player_name)
		str = player:GetTextName()
	end
	local gift_desc = string.format(TEXT_STALL_BUY_MESSAGE, str)
	local item_config = string.format("%u,%u,0", c.i_entry, c.i_count)

	local owner = app.objMgr:getObj(owner_guid)
	if not owner then
		local data = {}
		data.my_guid = player_guid
		data.callback_guid = owner_guid
		data.target_guid = owner_guid
		data.record_name = record_name
		data.record_guid = record_guid
		data.name = "doStallBuySell"
		function data.fun(data,objs)
			-- 给邮件奖励			
			local player = objs[data.callback_guid]
			if player then
				local gift = player:getGiftPacksInfo()
				if gift then			
					gift:AddGiftPacksInfo(0,GIFT_PACKS_TYPE_STALL_BUY,cur_tm, cur_tm+86400*7,TEXT_STALL_BUY_TITLE,gift_desc,item_config,"")
					-- 写下日志
					WriteGiftPacksLog(data.callback_guid,LOG_GIFTPACKS_TYPE_OUTLINE_ADD,0,GIFT_PACKS_TYPE_STALL_BUY,cur_tm, cur_tm+86400*7,TEXT_STALL_BUY_TITLE,gift_desc,item_config,"")
				end
			end
			-- 写下记录
			local ownerStallInfo = player:getStallTradeRecordInfo()			
			if ownerStallInfo then				
				ownerStallInfo:AddStallTradeRecord(data.record_guid, data.record_name, c.i_entry, c.i_count, 0, c.i_price, STALL_BUY_TRADE_EVENT_TYPE_BUY, 0)
			end
		end
		GetObjects(data)
	else
		-- 给邮件奖励
		local gift = owner:getGiftPacksInfo()
		if gift then
			gift:AddGiftPacksInfo(0,GIFT_PACKS_TYPE_STALL_BUY,cur_tm, cur_tm+86400*7,TEXT_STALL_BUY_TITLE,gift_desc,item_config,"")
			-- 写下日志
			WriteGiftPacksLog(owner_guid,LOG_GIFTPACKS_TYPE_ONLINE_ADD,0,GIFT_PACKS_TYPE_STALL_BUY,cur_tm, cur_tm+86400*7,TEXT_STALL_BUY_TITLE,gift_desc,item_config,"")
		end
		-- 写下记录
		local ownerStallInfo = owner:getStallTradeRecordInfo()
		if ownerStallInfo then
			ownerStallInfo:AddStallTradeRecord(record_guid, record_name, c.i_entry, c.i_count, 0, c.i_price, STALL_BUY_TRADE_EVENT_TYPE_BUY, 0)
		end
		owner:CallOptResult(OPRATE_TYPE_STALL,STALL_BUY_OPT_BUY_SUCCESS,{c.i_entry,price})
	end
	-- 写下记录
	local playerStallInfo = player:getStallTradeRecordInfo()
	if playerStallInfo then
		playerStallInfo:AddStallTradeRecord(c.buyer_guid, c.buyer_name, c.i_entry, c.i_count, 0, c.i_price, STALL_BUY_TRADE_EVENT_TYPE_SELL, 0)
	end
	-- 写下求购卖给他日志
	WriteStallBuyLog(player_guid, c.buyer_guid, c.buyer_name, LOG_STALL_BUY_SELL, c.i_entry, c.i_count, c.i_price, c.i_index)
end

----------------------------------摊位相关end--------------------------------------------

--保存充值状态
function AppdMongoDB:saveRechargeStatus(begin_status,change_status,id,account,server_name)
	local values = {}
	self.gm_intf.gm_recharge:update({ i_status = begin_status,server_name = server_name,account = account,_id = id},
		{['$set'] = { i_status = change_status}},
		false --如果不存在则不插入
	)
end

--根据帐号ID查询角色ID
function AppdMongoDB:loadGuidFromAccount(account,server_name)
	local wheres = {}
	wheres.account = account
	wheres.server_name = server_name
	wheres.u_delete_time = 0
	local c =  self:getCharDB(server_name).chars:findOne(wheres)
	if c then
		return c
	end
	return nil
end

--保存第首充记录
function AppdMongoDB:saveFristRechargeInfo(guid,name,level,value,monster_kill,player_kill,quest,open_window,adddate,server_name)
	local values = {}
	values["server_name"] = server_name
	values["guid"] = guid
	local c =  self.logs.ht_first_recharge:findOne(values)
	if c then
		return
	end
	values["name"] = name
	values["i_level"] = level
	values["i_value"] = value
	values["i_monster_kill"] = monster_kill
	values["i_player_kill"] = player_kill
	values["i_quest"] = quest
	values["i_open_window"] = open_window
	values["u_adddate"] = adddate
	self.logs.ht_first_recharge:insert(values)
end

--保存特殊排行榜信息
function AppdMongoDB:saveRankInfo(guid,owner,value,entry,top,rank_type)
	local values = {}
	values["server_name"] = self.server_name
	values["guid"] = guid
	values["owner"] = owner
	values["u_value"] = value
	values["i_entry"] = entry
	values["i_rank_type"] = rank_type
	values["i_top"] = top
	self.server.rank_info:insert(values)
end

--删除特殊排行榜信息
function AppdMongoDB:deleteRankInfo(rank_type)
	self.server.rank_info:delete({server_name = self.server_name,i_rank_type = rank_type})
end

--读取特殊排行榜信息
function AppdMongoDB:loadRankInfo(rank_type)
	local wheres = {}
	wheres['i_rank_type'] = rank_type
	wheres['server_name'] = self.server_name
	local c = app.dbAccess.server.rank_info:find(wheres,{},20,0)
	local results = {}
	while c:hasNext() do
		local r = c:next()
		if r and table.nums(r) > 0 then
			table.insert(results,r)
		end
	end
	return results
end

--读取限时活动配置概率表
function AppdMongoDB:loadActivityRate()
	local wheres = {}
	wheres["server_name"] = self.server_name
	local c = app.dbAccess.server.activity_rate:find(wheres,{})
	local results = {}
	while c:hasNext() do
		local r = c:next()
		if r and table.nums(r) > 0 then
			table.insert(results,r)
		end
	end
	return results
end

return AppdMongoDB
