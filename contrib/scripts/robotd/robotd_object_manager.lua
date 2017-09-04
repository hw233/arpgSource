require 'util.functions'

local ObjectManager = require('util.object_manager')
local RobotdObjectManager = class('RobotdObjectManager', ObjectManager)
local guidMgr = require('share.guid_manager')

GLOBAL_GAME_CONFIG = "G.gameconfig"

function RobotdObjectManager:ctor( player )
	self.player = player
	--所有的binlogod
	self.binlogTypes = {}
	self.binlogInstances = {}
	self.binlogTypes[guidMgr.ObjectTypeUnit] = require('robotd.object.robotd_unit')		--精灵对象
	self.binlogTypes[guidMgr.ObjectTypeMap] = require('robotd.object.robotd_map_info')	--地图对象
	self.binlogTypes[guidMgr.ObjectTypeItemMgr] = require('robotd.object.robotd_item')	--物品对象
	self.binlogTypes[guidMgr.ObjectTypeQuest] = require('robotd.object.robotd_quest')	--任务对象
	self.binlogTypes[guidMgr.ObjectTypeSpell] = require('robotd.object.robotd_spell')	--技能对象
	self.binlogTypes[guidMgr.ObjectTypeGridLoot] = require('robotd.object.robotd_loot')--战利品对象
	
	self.__tags = require('util.tags_manager').new()
	self.__objs = {}
	self.__ptr2obj = {}

	self.account2guid = {}
end

--远程创建新对象后触发
function RobotdObjectManager:afterAttachObject( obj ) 
	if(self:IsOnline() == false)then
		outFmtDebug("RobotdObjectManager:afterAttachObject offline, %s", obj:GetGuid())
		return
	end
	local prefix = guidMgr.get(obj:GetGuid())
	if(prefix == guidMgr.ObjectTypeUnit)then
		self.player:AddUnit(obj)
		return
	elseif(prefix == guidMgr.ObjectTypeMap)then
		self.player.mapInfo = obj
		return
	elseif(prefix == guidMgr.ObjectTypeItemMgr)then
		self.player.itemMgr = obj
		return
	-- 任务对象
	elseif (prefix == guidMgr.ObjectTypeQuest) then
		self.player.quest = obj
		return
	elseif (prefix == guidMgr.ObjectTypeSpell) then
		self.player.spellMgr = obj
		return
	elseif (prefix == guidMgr.ObjectTypeGridLoot) then
		self.player.lootMgr = obj
		return
	elseif(obj:GetGuid() == GLOBAL_GAME_CONFIG)then
		self.player.gameconfig = obj
		return
	end
end


function RobotdObjectManager:onNewObj( guid, ptr )
	local o = self:newObject(guid, ptr)
	self.__objs[guid] = o
	self.__ptr2obj[ptr] = o
end

function RobotdObjectManager:onReleaseObject(guid, ptr)
	--outDebug('RobotdObjectManager:onReleaseObject ' .. guid)
	local obj = self.__objs[guid]
	if(obj == nil)then
		--outFmtDebug('RobotdObjectManager:onReleaseObject %s not found',guid)
		return
	end
	local prefix = guidMgr.get(obj:GetGuid())
	if(prefix == guidMgr.ObjectTypeUnit)then
		self.player:DelUnit(obj)
	elseif(prefix == guidMgr.ObjectTypeItem)then
		self.player:DelItem(obj)
	elseif (prefix == guidMgr.ObjectTypeGridLoot) then
		self.player.lootMgr = nil
	elseif(prefix == guidMgr.ObjectTypeMap)then
		if(self.player.mapInfo == obj)then
			self.player.mapInfo = nil
		end
		return
	end
	
	--TODO:触发事件
	self.__objs[guid] = nil
	self.__tags:remove(guid)
	self.__ptr2obj[ptr] = nil
end

--玩家登出以后
function RobotdObjectManager:Logout( ... )
	self.player = nil
end

function RobotdObjectManager:IsOnline( ... )
	return self.player ~= nil
end

return RobotdObjectManager
