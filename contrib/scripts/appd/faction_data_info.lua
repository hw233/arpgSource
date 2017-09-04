--帮派数据管理
local FactionDataInfo = class('FactionDataInfo', BinLogObject)

local SPLIT_THINGS = "\1"	--分割多条物品
local SPLIT_THING_DETAIL = "\2"	--分割物品详细信息



function FactionDataInfo:ctor()
	
end

-- 礼物ID 设定 id  ==  index
function FactionDataInfo:SetGiftID(index,value)
	self:SetUInt32(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_ID,value)
end

function FactionDataInfo:GetGiftID(index)
	local dd = FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_ID
	
	return self:GetUInt32(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_ID)
end

-- 本次礼物总魅力值
function FactionDataInfo:SetGiftPoint(index,value)
	self:SetUInt32(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_POINT,value)
end

function FactionDataInfo:GetGiftPoint(index)
	return self:GetUInt32(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_POINT)
end

-- 感谢标识信息
function FactionDataInfo:SetGiftFlagThank(index,value)
	if value == 1 then
		self:SetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,1)
	elseif value == 0 then
		self:UnSetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,1)
	end
end

function FactionDataInfo:GetGiftFlagThank(index)
	local result = self:GetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,1)
	if result then
		return 1
	else
		return 0
	end
end

-- 女王回复标识信息
function FactionDataInfo:SetGiftFlagReply(index,value)
	if value == 1 then
		self:SetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,2)
	elseif value == 0 then
		self:UnSetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,2)
	end
end

function FactionDataInfo:GetGiftFlagReply(index)
	local result = self:GetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,2)
	if result then
		return 1
	else
		return 0
	end
end

-- 奖励领取标识信息
function FactionDataInfo:SetGiftFlagReward(index,value)
	if value == 1 then
		self:SetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,3)
	elseif value == 0 then
		self:UnSetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,3)
	end
end

function FactionDataInfo:GetGiftFlagReward(index)
	local result = self:GetBit(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_FLAG_THANK,3)
	if result then
		return 1
	else
		return 0
	end
end

-- 赠送时间
function FactionDataInfo:SetGiftTime(index,value)
	self:SetUInt32(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_TIME,value)
end

function FactionDataInfo:GetGiftTime(index)
	return self:GetUInt32(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_TIME)
end

-- 赠送者生成的礼物序号
function FactionDataInfo:SetGiftCountID(index,value)
	self:SetUInt32(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_COUNT_ID,value)
end

function FactionDataInfo:GetGiftCountID(index)
	return self:GetUInt32(FACTION_DATA_INT_FIELD_GIFT_START + MAX_FACTION_DATA_INT_GIFT * index + FACTION_DATA_INT_GIFT_COUNT_ID)
end

-- 赠送者guid
function FactionDataInfo:SetGiftGuid(index,value)
	self:SetStr(FACTION_DATA_STRING_FIELD_GIFT_START + MAX_FACTION_DATA_STRING_GIFT * index + FACTION_DATA_STRING_GIFT_GUID,value)
end

function FactionDataInfo:GetGiftGuid(index)
	return self:GetStr(FACTION_DATA_STRING_FIELD_GIFT_START + MAX_FACTION_DATA_STRING_GIFT * index + FACTION_DATA_STRING_GIFT_GUID)
end

-- 赠送者留言
function FactionDataInfo:SetGiftMsg(index,value)
	self:SetStr(FACTION_DATA_STRING_FIELD_GIFT_START + MAX_FACTION_DATA_STRING_GIFT * index + FACTION_DATA_STRING_GIFT_MSG,value)
end

function FactionDataInfo:GetGiftMsg(index)
	return self:GetStr(FACTION_DATA_STRING_FIELD_GIFT_START + MAX_FACTION_DATA_STRING_GIFT * index + FACTION_DATA_STRING_GIFT_MSG)
end

-- 礼物列表详情	物品1 \2 数量1 \1 物品2 \2 数量2
function FactionDataInfo:SetGiftItemList(index,value)
	self:SetStr(FACTION_DATA_STRING_FIELD_GIFT_START + MAX_FACTION_DATA_STRING_GIFT * index + FACTION_DATA_STRING_GIFT_ITEM_LIST,value)
end

function FactionDataInfo:GetGiftItemList(index)
	return self:GetStr(FACTION_DATA_STRING_FIELD_GIFT_START + MAX_FACTION_DATA_STRING_GIFT * index + FACTION_DATA_STRING_GIFT_ITEM_LIST)
end

-- 回复信息列表		玩家guid \2 信息 \2 回复类型 \2 时间 \1 玩家guid \2 信息 \2 回复类型 \2 时间
function FactionDataInfo:SetGiftReplyList(index,value)
	self:SetStr(FACTION_DATA_STRING_FIELD_GIFT_START + MAX_FACTION_DATA_STRING_GIFT * index + FACTION_DATA_STRING_GIFT_REPLY_LIST,value)
end

function FactionDataInfo:GetGiftReplyList(index)
	return self:GetStr(FACTION_DATA_STRING_FIELD_GIFT_START + MAX_FACTION_DATA_STRING_GIFT * index + FACTION_DATA_STRING_GIFT_REPLY_LIST)
end

-- 记录礼包Rank index 为名次 value 为礼包ID
function FactionDataInfo:SetGiftRank(index,value)
	self:SetUInt32(FACTION_DATA_INT_FIELD_GIFT_RANK_START + index,value)
end

function FactionDataInfo:GetGiftRank(index)
	return self:GetUInt32(FACTION_DATA_INT_FIELD_GIFT_RANK_START + index)
end


function FactionDataInfo:CheckCanSendGift(guid)
	local count = 0
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		if self:GetGiftGuid(index) == guid then
			count = count + 1
		end
	end
	if count < MAX_FACTION_DATA_GIFT_COUNT then
		return true
	else
		return false
	end
end

--添加礼包  遍历搜索空位  插入排行    (...)
function FactionDataInfo:AddGift(guid,point,item,msg,count_id)
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		if self:GetGiftGuid(index) == "" or string.len (self:GetGiftGuid(index)) == 0 then
			self:SetGiftID(index,index)
			self:SetGiftPoint(index,point)
			self:SetGiftFlagThank(index,0)
			self:SetGiftFlagReply(index,0)
			self:SetGiftFlagReward(index,0)
			self:SetGiftTime(index,os.time())
			self:SetGiftCountID(index,count_id)
			self:SetGiftGuid(index,guid)
			self:SetGiftMsg(index,msg)
			local item_list = ""
			for k,v in pairs(item) do
				if item_list == "" then
					item_list = v[1]..SPLIT_THING_DETAIL..v[2]
				else
					item_list = item_list..SPLIT_THINGS..v[1]..SPLIT_THING_DETAIL..v[2]
				end
			end
			self:SetGiftItemList(index,item_list)
			self:SetGiftReplyList(index,"")
			
			self:InsertGiftRank(index,point)
			return index, item_list
		end
	end
	return nil
end
--添加礼包留言 (id)
function FactionDataInfo:AddReply(id,guid,reply_type,reply,from_queen)
	if self:GetGiftGuid(id) == "" or string.len (self:GetGiftGuid(id)) == 0 then
		return false
	end
	local reply_list = self:GetGiftReplyList(id)
	
	if from_queen then --女王保留1回复空间
		if string.charCount(reply_list,1) >= MAX_FACTION_DATA_GIFT_REPLY_COUNT - 1 then
			return false
		end
	else
		if string.charCount(reply_list,1) >= MAX_FACTION_DATA_GIFT_REPLY_COUNT - 2 then
			return false
		end
	end
	
	if reply_list == "" then
		reply_list = guid ..SPLIT_THING_DETAIL.. reply ..SPLIT_THING_DETAIL.. reply_type ..SPLIT_THING_DETAIL.. os.time()
	else
		reply_list = reply_list ..SPLIT_THINGS..guid ..SPLIT_THING_DETAIL.. reply ..SPLIT_THING_DETAIL.. reply_type ..SPLIT_THING_DETAIL.. os.time()
	end
	self:SetGiftReplyList(id,reply_list)
	return true
	
end

--修改礼包感谢标识 (id)
function FactionDataInfo:ChangeGiftFlagThank(id)
	if self:GetGiftGuid(id) == "" or string.len (self:GetGiftGuid(id)) == 0 then
		return false
	end
	
	self:SetGiftFlagThank(id,1)
	return true
end
--修改礼包女王回复标识 (id)
function FactionDataInfo:ChangeGiftFlagReply(id)
	if self:GetGiftGuid(id) == "" or string.len (self:GetGiftGuid(id)) == 0 then
		return false
	end
	
	self:SetGiftFlagReply(id,1)
	return true
end

--修改礼包领取标识 (id)
function FactionDataInfo:ChangeGiftFlagReward(id)
	if self:GetGiftGuid(id) == "" or string.len (self:GetGiftGuid(id)) == 0 then
		return false
	end
	
	self:SetGiftFlagReward(id,1)
	return true
end


--取得礼包详情 (id)
function FactionDataInfo:GetGiftInfoByID(id)
	if self:GetGiftGuid(id) == "" or string.len (self:GetGiftGuid(id)) == 0 then
		return nil
	end
	
	local rank_info = {
		rank = 0,
		id = self:GetGiftID(id),
		point = self:GetGiftPoint(id),
		thank = self:GetGiftFlagThank(id),
		reply = self:GetGiftFlagReply(id),
		reward = self:GetGiftFlagReward(id),
		time = self:GetGiftTime(id),
		count_id = self:GetGiftCountID(id),
		guid = self:GetGiftGuid(id),
		msg = self:GetGiftMsg(id),
		item_list = self:GetGiftItemList(id),
		reply_list = self:GetGiftReplyList(id)
	}
	
	
	
	return rank_info
end


--取得礼包item详情 (id)
function FactionDataInfo:GetGiftItemListTable(id)
	if self:GetGiftGuid(id) == "" or string.len (self:GetGiftGuid(id)) == 0 then
		return nil
	end
	local item_list = self:GetGiftItemList(id)
	local item_table = {}
	local tokens = string.split(item_list,SPLIT_THINGS)
	for _,token in pairs(tokens) do
		local info_tokens = string.split(token,SPLIT_THING_DETAIL)
		table.insert(item_table,{tonumber(info_tokens[1]),tonumber(info_tokens[2])})
	end
	return item_table
end

function FactionDataInfo:DelGift(index)
	self:SetGiftID(index,0)
	self:SetGiftPoint(index,0)
	self:SetGiftFlagThank(index,0)
	self:SetGiftFlagReply(index,0)
	self:SetGiftFlagReward(index,0)
	self:SetGiftTime(index,0)
	self:SetGiftCountID(index,0)
	self:SetGiftGuid(index,"")
	self:SetGiftMsg(index,"")
	self:SetGiftItemList(index,"")
	self:SetGiftReplyList(index,"")
end

--删除某玩家的礼物  移出排行 (guid)
function FactionDataInfo:DelGiftByGuid(guid)
	local unthank_count = 0
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		if self:GetGiftGuid(index) == guid then
			if self:GetGiftFlagThank(index) == 0 then
				unthank_count = unthank_count + 1
			end
			self:DelGift(index)
			self:RemoveGiftRank(index)
		end
	end
	
	return unthank_count
end

--每日清空 ()
function FactionDataInfo:DailyClearAllGift()
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		self:DelGift(index)
	end
	self:ClearGiftRank()
end




function FactionDataInfo:SetGiftRankID(index,value)
	self:SetUInt32(FACTION_DATA_INT_FIELD_GIFT_RANK_START + index,value)
end

function FactionDataInfo:GetGiftRankID(index)
	return self:GetUInt32(FACTION_DATA_INT_FIELD_GIFT_RANK_START + index)
end

--插入今日排行
function FactionDataInfo:InsertGiftRank(id,point)
	id = id + 1 --默认值为0, 所以+1进行偏移
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		local compare_id = self:GetGiftRankID(index)
		if compare_id == 0 then
			self:Insert(id,index)
			break
		end
		if point > self:GetGiftPoint(compare_id-1) then
			self:Insert(id,index)
			break
		end
		
	end
end

function FactionDataInfo:Insert(id,index)
	if self:GetGiftRankID(index) == 0 then
		self:SetGiftRankID(index,id)
		return
	end
	repeat
		local temp = self:GetGiftRankID(index)
		self:SetGiftRankID(index,id)
		id = temp
		index = index + 1
		
	until self:GetGiftRankID(index) == 0
	self:SetGiftRankID(index,id)
end
--移出今日排行
function FactionDataInfo:RemoveGiftRank(id)
	id = id + 1 --默认值为0, 所以+1进行偏移
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		local compare_id = self:GetGiftRankID(index)
		if compare_id == 0 then
			break
		end
		if id == compare_id then
			self:Remove(index)
			break
		end
	end
end

function FactionDataInfo:Remove(index)
	if self:GetGiftRankID(index) == 0 or index == MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 then
		self:SetGiftRankID(index,0)
	end
	repeat
		
		self:SetGiftRankID(index,self:GetGiftRankID(index + 1))
		index = index + 1
		
	until index == MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 or self:GetGiftRankID(index + 1) == 0
	self:SetGiftRankID(index,0)
	
end
--清空今日排行
function FactionDataInfo:ClearGiftRank()
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		if self:GetGiftRankID(index) == 0 then
			break
		end
		self:SetGiftRankID(index,0)
	end
end

--按排行取得详情列表 (begin,tail)
function FactionDataInfo:GetGiftRankList(begin,tail)
	local rank_list = {}
	for index = begin -1, tail -1 do
		local id2 =  self:GetGiftRankID(index)
		if id2 ~= 0 then
			local info = {rank = index + 1,
				id = self:GetGiftID(id2-1),
				point = self:GetGiftPoint(id2-1),
				thank = self:GetGiftFlagThank(id2-1),
				reply = self:GetGiftFlagReply(id2-1),
				reward = self:GetGiftFlagReward(id2-1),
				time = self:GetGiftTime(id2-1),
				count_id = self:GetGiftCountID(id2-1),
				guid = self:GetGiftGuid(id2-1),
				msg = self:GetGiftMsg(id2-1),
				item_list = self:GetGiftItemList(id2-1),
				reply_list = self:GetGiftReplyList(id2-1)}
			table.insert(rank_list,info)
			
		end
	end
	return rank_list
end

--女王只显示未感谢的 
function FactionDataInfo:GetGiftRankListUnthank(begin,tail)
	local rank_list = {}
	local rank = 1
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		local id = self:GetGiftRankID(index)
		if id == 0 then --已到达末尾
			break
		end
		
		if self:GetGiftFlagThank(id-1) == 0 then
			if rank < begin then
				rank = rank + 1
			elseif rank > tail then
				break
			else
				table.insert(rank_list,{
					rank = index + 1,
					id = self:GetGiftID(id-1),
					point = self:GetGiftPoint(id-1),
					thank = self:GetGiftFlagThank(id-1),
					reply = self:GetGiftFlagReply(id-1),
					reward = self:GetGiftFlagReward(id-1),
					time = self:GetGiftTime(id-1),
					count_id = self:GetGiftCountID(id-1),
					guid = self:GetGiftGuid(id-1),
					msg = self:GetGiftMsg(id-1),
					item_list = self:GetGiftItemList(id-1),
					reply_list = self:GetGiftReplyList(id-1)
				})
				rank = rank + 1
				if rank > tail then
					break
				end
			end
		
		end
	end
	return rank_list
end

--女王 已感谢的在后,未感谢的在前
function FactionDataInfo:GetGiftRankListHistory(begin,tail)
	local rank_list = {}
	local rank = 1
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		local id = self:GetGiftRankID(index)
		if id == 0 then --已到达末尾
			break
		end
		
		if self:GetGiftFlagThank(id-1) == 0 then
			if rank < begin then
				rank = rank + 1
			elseif rank > tail then
				break
			else
				table.insert(rank_list,{
					rank = index + 1,
					id = self:GetGiftID(id-1),
					point = self:GetGiftPoint(id-1),
					thank = self:GetGiftFlagThank(id-1),
					reply = self:GetGiftFlagReply(id-1),
					reward = self:GetGiftFlagReward(id-1),
					time = self:GetGiftTime(id-1),
					count_id = self:GetGiftCountID(id-1),
					guid = self:GetGiftGuid(id-1),
					msg = self:GetGiftMsg(id-1),
					item_list = self:GetGiftItemList(id-1),
					reply_list = self:GetGiftReplyList(id-1)
				})
				rank = rank + 1
				if rank > tail then
					break
				end
			end
		
		end
	end
	if rank <= tail then
		for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
			local id = self:GetGiftRankID(index)
			if id == 0 then --已到达末尾
				break
			end
			
			if self:GetGiftFlagThank(id-1) == 1 then
				if rank < begin then
					rank = rank + 1
				elseif rank > tail then
					break
				else
					table.insert(rank_list,{
						rank = index + 1,
						id = self:GetGiftID(id-1),
						point = self:GetGiftPoint(id-1),
						thank = self:GetGiftFlagThank(id-1),
						reply = self:GetGiftFlagReply(id-1),
						reward = self:GetGiftFlagReward(id-1),
						time = self:GetGiftTime(id-1),
						count_id = self:GetGiftCountID(id-1),
						guid = self:GetGiftGuid(id-1),
						msg = self:GetGiftMsg(id-1),
						item_list = self:GetGiftItemList(id-1),
						reply_list = self:GetGiftReplyList(id-1)
					})
					rank = rank + 1
					if rank > tail then
						break
					end
				end
			
			end
		end
	end
	return rank_list
end

--只显示自己被感谢的 未领取的
function FactionDataInfo:GetGiftRankListThank(guid,begin,tail)
	local rank_list = {}
	local rank = 1
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		local id = self:GetGiftRankID(index)
		if id == 0 then --已到达末尾
			break
		end
		
		if self:GetGiftGuid(id-1) == guid and self:GetGiftFlagThank(id-1) == 1 and self:GetGiftFlagReward(id-1) == 0 then
			if rank < begin then
				rank = rank + 1
			elseif rank > tail then
				break
			else
				table.insert(rank_list,{
					rank = index + 1,
					id = self:GetGiftID(id-1),
					point = self:GetGiftPoint(id-1),
					thank = self:GetGiftFlagThank(id-1),
					reply = self:GetGiftFlagReply(id-1),
					reward = self:GetGiftFlagReward(id-1),
					time = self:GetGiftTime(id-1),
					count_id = self:GetGiftCountID(id-1),
					guid = self:GetGiftGuid(id-1),
					msg = self:GetGiftMsg(id-1),
					item_list = self:GetGiftItemList(id-1),
					reply_list = self:GetGiftReplyList(id-1)
				})
				rank = rank + 1
				if rank > tail then
					break
				end
			end
		
		end
	end
	return rank_list
	
end

--只显示自己所有的的 ,已感谢在前,未感谢的在前
function FactionDataInfo:GetGiftRankListSelf(guid,begin,tail)
	local rank_list = {}
	local rank = 1
	for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		local id = self:GetGiftRankID(index)
		if id == 0 then --已到达末尾
			break
		end
		
		if self:GetGiftGuid(id-1) == guid and self:GetGiftFlagThank(id-1) == 0 then
			if rank < begin then
				rank = rank + 1
			elseif rank > tail then
				break
			else
				table.insert(rank_list,{
					rank = index + 1,
					id = self:GetGiftID(id-1),
					point = self:GetGiftPoint(id-1),
					thank = self:GetGiftFlagThank(id-1),
					reply = self:GetGiftFlagReply(id-1),
					reward = self:GetGiftFlagReward(id-1),
					time = self:GetGiftTime(id-1),
					count_id = self:GetGiftCountID(id-1),
					guid = self:GetGiftGuid(id-1),
					msg = self:GetGiftMsg(id-1),
					item_list = self:GetGiftItemList(id-1),
					reply_list = self:GetGiftReplyList(id-1)
				})
				rank = rank + 1
				if rank > tail then
					break
				end
			end
		
		end
	end
	if rank <= tail then
		for index = 0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
			local id = self:GetGiftRankID(index)
			if id == 0 then --已到达末尾
				break
			end
			
			if self:GetGiftGuid(id-1) == guid and self:GetGiftFlagThank(id-1) == 1 then
				if rank < begin then
					rank = rank + 1
				elseif rank > tail then
					break
				else
					table.insert(rank_list,{
						rank = index + 1,
						id = self:GetGiftID(id-1),
						point = self:GetGiftPoint(id-1),
						thank = self:GetGiftFlagThank(id-1),
						reply = self:GetGiftFlagReply(id-1),
						reward = self:GetGiftFlagReward(id-1),
						time = self:GetGiftTime(id-1),
						count_id = self:GetGiftCountID(id-1),
						guid = self:GetGiftGuid(id-1),
						msg = self:GetGiftMsg(id-1),
						item_list = self:GetGiftItemList(id-1),
						reply_list = self:GetGiftReplyList(id-1)
					})
					rank = rank + 1
					if rank > tail then
						break
					end
				end
			
			end
		end
	end
	return rank_list
end

local GIFT_CACHE_INDEX_START = MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT --开始位置

--感谢后将礼包信息添加到缓存
function FactionDataInfo:AddGiftCache(gift_id,reply)
	local cache_index = self:GetGiftCacheEmptyIndex()
	self:SetGiftID(cache_index,self:GetGiftID(gift_id))
	self:SetGiftPoint(cache_index,self:GetGiftPoint(gift_id))
	self:SetGiftFlagThank(cache_index,self:GetGiftFlagThank(gift_id))
	self:SetGiftFlagReply(cache_index,self:GetGiftFlagReply(gift_id))
	self:SetGiftFlagReward(cache_index,self:GetGiftFlagReward(gift_id))
	self:SetGiftTime(cache_index,self:GetGiftTime(gift_id))
	self:SetGiftCountID(cache_index,self:GetGiftCountID(gift_id))
	self:SetGiftGuid(cache_index,self:GetGiftGuid(gift_id))
	self:SetGiftMsg(cache_index,reply)
	self:SetGiftItemList(cache_index,self:GetGiftItemList(gift_id))
	self:SetGiftReplyList(cache_index,"")
	
end
--移出缓存
function FactionDataInfo:DelGiftCache(index)
	self:SetGiftID(index,0)
	self:SetGiftPoint(index,0)
	self:SetGiftFlagThank(index,0)
	self:SetGiftFlagReply(index,0)
	self:SetGiftFlagReward(index,0)
	self:SetGiftTime(index,0)
	self:SetGiftCountID(index,0)
	self:SetGiftGuid(index,"")
	self:SetGiftMsg(index,"")
	self:SetGiftItemList(index,"")
	self:SetGiftReplyList(index,"")
	
end

--取得离线感谢信息缓存 index 空位  没有则返回时间最早的
function FactionDataInfo:GetGiftCacheEmptyIndex()
	local oldest_time = os.time()
	local oldest_index = GIFT_CACHE_INDEX_START
	
	for index = GIFT_CACHE_INDEX_START, GIFT_CACHE_INDEX_START + MAX_FACTION_DATA_GIFT_CACHE_COUNT - 1 do
		if self:GetGiftGuid(index) == "" or string.len (self:GetGiftGuid(index)) == 0 then
			return index
		elseif self:GetGiftTime(index) < oldest_time then
			oldest_time = self:GetGiftTime(index)
			oldest_index = index
		end
	end
	return oldest_index
end

--取得玩家对应离线感谢信息缓存
function FactionDataInfo:GetGiftCacheByGuid(guid)
	local gift_table = {}
	for index = GIFT_CACHE_INDEX_START, GIFT_CACHE_INDEX_START + MAX_FACTION_DATA_GIFT_CACHE_COUNT - 1 do
		if self:GetGiftGuid(index) == guid then
			table.insert(gift_table,{
				--rank = 0,
				--id = self:GetGiftID(index),
				--point = self:GetGiftPoint(index),
				thank = self:GetGiftFlagThank(index),
				--reply = self:GetGiftFlagReply(index),
				--reward = self:GetGiftFlagReward(index),
				--time = self:GetGiftTime(index),
				count_id = self:GetGiftCountID(index),
				--guid = self:GetGiftGuid(index),
				msg = self:GetGiftMsg(index),
				--item_list = self:GetGiftItemList(index),
				--reply_list = self:GetGiftReplyList(index)
			})
			self:DelGiftCache(index)
		end
	end
	return gift_table
end

--[[
local output = protocols.pack_show_unit_attribute ()
		local size = GetAttrSize()
		output:writeU16(size)
		foreachAttr(function (attrId, binlogIndx)
			local val = playerInfo:GetUInt32(binlogIndx)
			output:writeU32(attrId)
			output:writeU32(val)
		end)
		playerInfo:SendPacket(output)
		output:delete()
		
--]]
return FactionDataInfo