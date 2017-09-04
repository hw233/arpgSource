--服务器列表binlog
local ServerConnList = class('ServerConnList', assert(BinLogObject))

function ServerConnList:ctor( )
end

--遍历所有服务器信息
function ServerConnList:Foreach (fun)
	local len = self:GetUInt32Len()
	local i = 0
	while self:GetServerStartLoc(i) < len do
		local connid = self:GetConnInfoID(i)
		if( connid ~= 0)then
			if(fun(i, connid, self:GetServerType(i), self:GetConnOkFlag(i), self:GetScenedType(i)))then
				break
			end
		end
		i = i + 1
	end
end

--获取某个服务器起始下标位置
function ServerConnList:GetServerStartLoc(index)
	return SERVER_CONN_LIST_FIELD_SERVER_INFO_START + index * MAX_SERVER_CONN_INFO
end

--获取指定位置的fd
function ServerConnList:GetConnInfoID (index)
	return self:GetUInt32(self:GetServerStartLoc(index) + SERVER_CONN_INFO_ID)
end

--获取指定位置的服务器类型
function ServerConnList:GetServerType (index)
	return self:GetUInt32(self:GetServerStartLoc(index) + SERVER_CONN_INFO_TYPE)
end

--获取指定位置的场景服类型
function ServerConnList:GetScenedType (index)
	return self:GetUInt32(self:GetServerStartLoc(index) + SERVER_CONN_INFO_SCENED_TYPE)
end

--获取指定位置的服务器是否准备完毕
function ServerConnList:GetConnOkFlag (index)
	return self:GetBit(self:GetServerStartLoc(index) + SERVER_CONN_INFO_FLAG, SERVER_CONN_INFO_FLAG_READY_OK)
end

--添加一个server信息,中心服调用
function ServerConnList:AddServer (server_type, conn_id, pid)
	outDebug(string.format('AddServer server_type = %d  conn_id = %d',server_type,conn_id))
	local empty_pos = self:FindEmptyPos() --找个空的位置
	if(empty_pos == -1)then--找不到空位置
		outDebug('AddServer find no empty pos')
		return
	end
	--把服务器信息存到空位置上
	for j = 0, MAX_SERVER_CONN_INFO-1, 1 do
		--存之前先清零
		self:SetUInt32(self:GetServerStartLoc(empty_pos) + j, 0)
	end
	self:SetUInt32(self:GetServerStartLoc(empty_pos) + SERVER_CONN_INFO_PID, pid)			
	self:SetUInt32(self:GetServerStartLoc(empty_pos) + SERVER_CONN_INFO_TYPE, server_type)	
	--场景服需要设置自己的类型
	if(server_type == SERVER_TYPE_SCENED)then
		local type_table = {0,0,0,0,0,0}
		self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(server_type == SERVER_TYPE_SCENED)then
				type_table[scened_type] = type_table[scened_type] + 1
			end
		end)
		
		local scened_type
		local min_v = 999
		for i = 1, table.maxn(type_table) do
			if(min_v > type_table[i])then
				scened_type = i
				min_v = type_table[i]
			end
		end
		outString('this scened server type is '..scened_type)
		self:SetUInt32(self:GetServerStartLoc(empty_pos) + SERVER_CONN_INFO_SCENED_TYPE, scened_type)
	end
	--最后才给连接id
	self:SetUInt32(self:GetServerStartLoc(empty_pos) + SERVER_CONN_INFO_ID, conn_id)
	--self:PrintServerConList()
end

--移除指定类型和链接id的服务器
function ServerConnList:RemoveServer (f_type, conn_id)
	outDebug(string.format('RemoveServer server_type = %d  conn_id = %d',f_type,conn_id))

	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(fd == conn_id and server_type == f_type)then
				--找到后把数据清零
				--outDebug('scened  clean server_type = '..f_type..'  conn_id = '..conn_id)
				for k = 0, MAX_SERVER_CONN_INFO - 1, 1 do
					self:SetUInt32(self:GetServerStartLoc(index) + k , 0)
				end
				return true
			end
		end)
	--self:PrintServerConList()
end

--返回场景服的个数
function ServerConnList:GetScenedSize ()
	local size = 0;
	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(ok_flag and server_type == SERVER_TYPE_SCENED)then
				size = size + 1	--服务器类型为场景服才统计
			end
		end)
	return size
end

--返回场景服的索引
function ServerConnList:GetScenedIndex ()
	local indexs = {};
	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(ok_flag and server_type == SERVER_TYPE_SCENED)then
				table.insert(indexs, index)	--服务器类型为场景服才统计
			end
		end)
	return indexs
end

--添加场景服的玩家数量
function ServerConnList:AddScenedPlayer (conn_id)
	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(fd == conn_id)then
				self:AddUInt32(self:GetServerStartLoc(index) + SERVER_CONN_INFO_COUNT, 1)
				return true
			end	
		end)
end

--减少场景服的玩家数量
function ServerConnList:SubScenedPlayer (conn_id)
	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(fd == conn_id)then
				self:SubUInt32(self:GetServerStartLoc(index)+ SERVER_CONN_INFO_COUNT, 1)
				return true
			end	
		end)
end


--根据规则选取场景服
function ServerConnList:SelectScenedFDByType (scened_type, mapid)

	--地图选场景配置
	local map_config = {
		[1] = {1, 5, 15, 32},
		[2] = {3, 9, 29, 39},
		[3] = {2, 17, 30, 59},
		[4] = {4, 18, 31},
		[5] = {61, 10, 12, 14, 28, 33, 34, 35, 62},		-- 活动副本地图
		[6] = {19, 20, 21, 22, 23, 24, 25, 26, 27, 40, 58, 81, 84, 85, 86},	--单人副本地图
	}

	local conn_id = 0
	local idx = -1
	for k, val in pairs(map_config) do
		for i, v in pairs(val) do
			if(v == mapid)then
				idx = k
				break
			end
		end
		if(idx ~= -1)then
			break
		end
	end
	
	if(idx ~= -1)then
		self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(server_type == SERVER_TYPE_SCENED and ok_flag)then
				if(scened_type == idx)then
					conn_id = fd
					return true
				end
			end	
		end)
		
		if(conn_id ~= 0)then
			return conn_id
		end
	end
	
	--找不到对应场景服,或者index为-1，那就找一个人少的放一下
	local player_count = 0
	
	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(server_type == SERVER_TYPE_SCENED and ok_flag)then
				local player_count_ = self:GetUInt32(self:GetServerStartLoc(index) + SERVER_CONN_INFO_COUNT)
				if(conn_id == 0 or player_count > player_count_)then
					--outDebug('------------ conn_id = '..conn_id..'  player_count = '..player_count)
					conn_id = fd
					player_count = player_count_
				end
			end	
		end)
	
	return conn_id
end

--根据一定的规则获得场景服连接 inst_type:副本类型（0：不是副本 1：活动副本 2：单人副本）
function ServerConnList:GetScenedFDByType (inst_type, mapid)
	local conn_id = 0
	if(inst_type == MAP_INST_TYP_SINGLETON)then		--单人副本
		conn_id = self:SelectScenedFDByType(SCENED_TYPE_INST, mapid)
	elseif(inst_type == MAP_INST_TYP_ACTIVITY)then	--活动副本
		conn_id = self:SelectScenedFDByType(SCENED_TYPE_ACTIVI, mapid)
	elseif(inst_type == MAP_INST_TYP_NO_INSTANCE)then	--不是副本
		conn_id = self:SelectScenedFDByType(SCENED_TYPE_NO_INST, mapid)
	end
	--如果相应的服务器没有找到	
	if(conn_id == 0)then
		conn_id = self:SelectScenedFDByType(MAX_SCENED_TYPE);
	end
	outDebug('tip: SelectScenedFDByType:mapid = '..mapid..' select scened connection = '..conn_id)
	return conn_id	
end

--设置服务器准备好的flag
function ServerConnList:SetServerReadyOKFlag (conn_id)
	local index = self:FindServerByConnid(conn_id)
	if(index ~= -1)then
		self:SetBit(self:GetServerStartLoc(index) + SERVER_CONN_INFO_FLAG, SERVER_CONN_INFO_FLAG_READY_OK)
	end
end

--获得服务器准备好的flag
function ServerConnList:GetServerReadyOKFlag (conn_id)
	local index = self:FindServerByConnid(conn_id)
	if(index ~= -1)then
		return self:GetConnOkFlag(index)
	else
		return false
	end
end

--找个空的位置
function ServerConnList:FindEmptyPos ()
	--找个空位置存起来
	local i = 0
	while true do
		if(self:GetConnInfoID(i) == 0)then
			return i
		end	
		i = i + 1
	end
	return -1
end

--查找指定类型的服务器,除场景服
function ServerConnList:FindServerByType (server_type)	
	local bl = false
	local idx = nil
	local connId = nil
	local connOkFlag = nil
	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(server_type == SERVER_TYPE_SCENED)then
				bl = true
				idx = index
				connId = fd
				connOkFlag = ok_flag
				return true
			end	
		end)

	return bl, idx, connId, connOkFlag
end

--找到指定链接的服务器 
function ServerConnList:FindServerByConnid (conn_id)
	local idx = -1
	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(fd == conn_id)then
				idx = index
				return true
			end	
		end)
		
	return idx
end

function ServerConnList:PrintServerConList ()
	outDebug('-------------------- print server list -------------------')

	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			outDebug('index = '..i..' conn_id = '..fd..' server_type = '..server_type..' scened_type = '..self:GetUInt32(self:GetServerStartLoc(index) + SERVER_CONN_INFO_SCENED_TYPE))
		end)
		
	outDebug('-------------------- print server list end-------------------')	
end

--------------------------------------
--拷贝自showhand

--获取指定位置的服务器是否准备完毕
function ServerConnList:getServerReadyOKFlagByIndex( i )
	return self:GetBit(self:GetServerStartLoc(i) + SERVER_CONN_INFO_FLAG, SERVER_CONN_INFO_FLAG_READY_OK)
end

function ServerConnList:getServerFD( serverType, must_readok )
	local i = self:findConn(function (index, fd, type )
		if (type ~= serverType) then
			return false
		elseif (not must_readok) then
			return true
		else
			return self:getServerReadyOKFlagByIndex(index)
		end
	end)

	if(i == -1)then
		return 0
	else
		return self:GetConnInfoID(i)
	end
end

function ServerConnList:findConn( f )
	local i, len = 0, self:GetUInt32Len()
	while self:GetServerStartLoc(i) < len do
		local connid = self:GetConnInfoID(i)
		if(connid ~= 0)then
			if(f(i, connid, self:GetServerType(i)))then
				return i
			end
		end
		i = i + 1
	end	
	return -1
end

function ServerConnList:getLogindFD( must_readok )	
	return self:getServerFD(SERVER_TYPE_LOGIND,must_readok )
end

function ServerConnList:getAppdFD( ... )
	return self:getServerFD(SERVER_TYPE_APPD)
end

function ServerConnList:getPolicedFD( ... )
	return self:getServerFD(SERVER_TYPE_POLICED)
end

--遍历获取所有场景服FD
function ServerConnList:forEachScenedFD(f)
	self:Foreach(function(index, fd, server_type, ok_flag, scened_type)
			if(ok_flag and server_type == SERVER_TYPE_SCENED)then
				f(fd)
			end
		end)
end

return ServerConnList
