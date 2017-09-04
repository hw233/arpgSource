require 'util.functions'

local ObjectManager = class('ObjectManager')

GLOBAL_VALUE_OWNER_STRING = "global_value"
WORLD_BINLOG_OWNER_STRING = "world"
FACTION_BINLOG_OWNER_STRING = "faction"
FACTION_DATA_OWNER_STRING = "faction_data"
GROUP_OWNER_STRING = "group"

function ObjectManager:ctor( )
	self.__tags = require('util.tags_manager').new()
	self.__objs = {}
	self.__ptr2obj = {}
	--所有的binlogod
	self.binlogTypes = self.binlogTypes or {}
	--所有的全局对象
	self.binlogInstances = require 'share.global_manager'
	local function onNewObj( guid, ptr )
		local o = self:newObject(guid, ptr)
		self.__objs[guid] = o
		self.__ptr2obj[ptr] = o
	end

	--当新对象来的时候
	objects.registerAttachObject(onNewObj)
	--当对象释放的时候
	objects.registerReleaseObject(function ( guid, ptr )
		--TODO:触发事件
		self.__objs[guid] = nil
		self.__tags:remove(guid)
		self.__ptr2obj[ptr] = nil
	end)
	--当@脚本的时候需要所有的对象重新添加一遍
	objects.foreach('',onNewObj)
	
	-----------------------------------------
	--远程创建新对象后触发
    --当新对象已经应用到对象更新并且被附加到对象表之后触发
	if self.afterAttachObject then
		objects.registerAfterAttachObject( function(ptr)
			local o = self:fromPtr(ptr)       
			--如果发现是物品则调用一下初始化		
			self:afterAttachObject(o)		
		end)

		--当@脚本的时候需要所有的对象重新afterAttachObject
		objects.foreach('',function ( _, ptr )
			local o = self:fromPtr(ptr)
			--如果发现是物品则调用一下初始化              
			self:afterAttachObject(o)
		end)
	end
end

function ObjectManager:fromPtr( ptr )
	return self.__ptr2obj[ptr]
end

--根据guid规则创建对象,允许子类重载
--设置对象标签就在这里了
function ObjectManager:newObject( guid, ptr, ... )
	--先从对象实例表获得一下
	local inst = self.binlogInstances[guid]
	if not inst then
		--使用前缀去类型对照表里面找看看
		local prefix = guidMgr.get (guid)
		local cls = self.binlogTypes[prefix]
		--递归查找类型
		if cls and cls.__cname == nil then
			cls = guidMgr.findType(cls, guid)
		end
		cls = cls or BinLogObject	--默认是binlog类型
		inst = cls:new({ptr = ptr}, ...)
	end	
	return inst
end

function ObjectManager:getObj( guid )
	assert(guid)
	local o = self.__objs[guid]	
	if not o then 
		--正常不会走到这一步
		local ptr = objects.get(guid)
		if not ptr then return nil end
		o = self:newObject(guid, ptr)
		self.__objs[guid] = o--应急需要不进缓存
	end
	return o 
end

--遍历所有tag中对象
function ObjectManager:everyTag( tag, func )
	local function cb( guid )
		local obj = self:getObj(guid)
		if obj then
			func(obj)
		end
	end
	return self.__tags:forEach(tag, cb)
end


--创建一个新对象
function ObjectManager:newAndCallPut(guid, owner)
	objects.newAndCallPut(guid, owner)
	return self:getObj(guid)
end

--删除一个对象
function ObjectManager:callRemoveObject(guid)
	objects.callRemoveObject(guid)
end

--添加监听
function ObjectManager:callAddWatch(fd, guid)
	if fd == 0 then return end
	objects.callAddWatch(fd, guid)
end

--移除监听
function ObjectManager:callDelWatch(fd, guid)
	if fd == 0 or guid == "" then return end
	objects.callDelWatch(fd, guid)
end


return ObjectManager
