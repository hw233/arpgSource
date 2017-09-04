require ('util.functions')

local RankManager = class('RankManager')

function RankManager:ctor()	
	self.minValue = 0		--入榜需要最低分数
	self.shard_value = 100	--每个分片段
	self.info = {}			--玩家信息表
	self.shard = {}			--记录分片排名分数
	self.shard_guid = {}	--分片排名对应的GUID
	self.type = 0			--排行榜类型
	self.maxShard = 1		--最大分片数
	
end

--排名
function RankManager:Compare(guid,tbl)
	--获取是否已经有数据
	if(self.info[guid])then
		--判断是否需要排名
		if(tbl.value < self.minValue)then
			self:Delete(guid)
		else
			self:Update(guid,tbl)
		end
	else
		if(tbl.value >= self.minValue)then
			self:Insert(guid,tbl)
		end
	end
end

--插入数据
function RankManager:Insert(guid,tbl)
	--根据分数算出分片位置
	local shard = math.floor((tbl.value - self.minValue)/self.shard_value) + 1
	if(shard <= 0)then
		return
	end
	if(shard > self.maxShard)then
		self.maxShard = shard
	end
	local rank = self:GetShardTable(tbl.value,shard)
	if(rank <= 0)then
		return
	end
	self.shard[shard] = self.shard[shard] or {}
	self.shard_guid[shard] = self.shard_guid[shard] or {}
	--找到位置了
	table.insert(self.shard[shard],rank,tbl.value)
	table.insert(self.shard_guid[shard],rank,guid)
	--更新数据
	--更新下信息内容
	if(self.info[guid] == nil)then
		self.info[guid] = clone(tbl)
	else
		self.info[guid].value = tbl.value
	end	
	self.info[guid].shard = shard
	self.info[guid].rank = rank
end

--更新数据
function RankManager:Update(guid,tbl)
	--先找出之前的位置移除掉
	local old_shard = self.info[guid].shard
	local old_rank = self.info[guid].rank
	table.remove(self.shard[old_shard],old_rank)
	table.remove(self.shard_guid[old_shard],old_rank)
	--调用插入
	self:Insert(guid,tbl)
end

--删除数据
function RankManager:Delete(guid)
	local rank_info = self.info[guid]
	--取出数据所在table的位置，然后移除
	if(rank_info)then
		if(rank_info.shard and rank_info.rank)then
			table.remove(self.shard[rank_info.shard],rank_info.rank)
			table.remove(self.shard_guid[rank_info.shard],rank_info.rank)
		end
		self.info[guid] = nil
	end
end

--根据分片键获取分片位置
function RankManager:GetShardTable(value,shard)
	--算下在分片的哪个位置
	local rank = -1
	if(shard > 0)then
		rank = 1
		self.shard[shard] = self.shard[shard] or {}
		local low = 1
		local high = #self.shard[shard]
		if(high >= low)then
			if(value > self.shard[shard][high])then
				rank = high + 1
			elseif(value <= self.shard[shard][low])then
				rank = 1
			else
				rank = binarySearch(self.shard[shard],value)
			end
		end	
	end
	return rank
end

--保存数据库
function RankManager:SaveDB()
	--保存前先清空
	app.dbAccess:deleteRankInfo(self.type)
	--倒序找出排行榜，只要20名就好
	local top = 1
	for i = self.maxShard,1,-1
	do
		if(self.shard_guid[i])then
			for j = #self.shard_guid[i],1,-1
			do
				local guid = self.shard_guid[i][j]
				if(self.info[guid] ~= nil)then
					app.dbAccess:saveRankInfo(guid,self.info[guid].owner,self.info[guid].value,self.info[guid].entry,top,self.type)
					top = top + 1
					if(top > 20)then
						return
					end
				end
			end
		end	
	end
end

--读取数据库
function RankManager:LoadDB()
	if (not(app and app.dbAccess)) then
		return
	end
	local result = app.dbAccess:loadRankInfo(self.type)
	if result == nil then
		return
	end
	for _k,_v in pairs(result) do
		self:Insert(_v.guid,{value = _v.u_value,entry = _v.i_entry,owner = _v.owner})
	end
end

return RankManager
