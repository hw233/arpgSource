--回调集合
local callback_table = {
}

--获取通过owner某一批对象
function GetObjects(data)
	local server_name = app.dbAccess:getServerName(data.callback_guid)
	if(server_name == nil)then
		outDebug("GetObjects is error player guid is "..data.callback_guid)
		return
	end
	if(globalGameConfig:IsMyServer(server_name) == false)then
		outString(string.format("GetObjects not my server , %s %s %s", data.callback_guid, data.my_guid and data.my_guid or "", data.name))
		return
	end
	
	assert(data.name)
	assert(data.callback_guid)
	outDebug(string.format('GetObjects guid : %s , name : %s', data.callback_guid, data.name))
	
	local data_cb = callback_table[data.callback_guid]
	if(data_cb == nil)then
		data_cb = {callback_guid = data.callback_guid, data = {data}}
		callback_table[data_cb.callback_guid] = data_cb
		playerLib.OfflineOper(data_cb.callback_guid, data.name)
	else		
		--同一个玩家只能同时进行一次回调
		if(data.lock_fun ~= nil)then
			data.lock_fun()
			return 0
		end
		table.insert(data_cb.data, data)
	end
end

--执行回调
function DoGetObjectsCallBack(callback_guid, objs)
	local data_cb = callback_table[callback_guid]
	if data_cb == nil then
		outDebug("DoGetObjectsCallBack data is nil "..callback_guid)
		return false
	end

	local infoObjs = {}
	for k,v in pairs(objs) do
		--调用工厂方法创建对象实例 
		--todo linbc 只考虑playerInfo的时候才需要infoObjs
		local o = app.objMgr:newObject(k, v, infoObjs)
		infoObjs[k]	= o
		--如果存在初始化函数则自动调用
		if o.init then
			o:init()
		end
	end
	
	callback_table[callback_guid] = nil
	for k,data in pairs(data_cb.data) do
		outDebug(string.format('DoGetObjectsCallBack uid : %s, name : %s',data.callback_guid,data.name))
		--todo 如果这里业务代码出错，那么剩下的所有回调都将失效。
		--但是如果不断言，不好找问题
		local ret = doxpcall(data.fun, data, infoObjs)
		assert(ret)
	end
end

