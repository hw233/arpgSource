local ObjectManager = require('util.object_manager')
local ScenedObjectManager = class('ScenedObjectManager', ObjectManager)
local ScenedFaction = require('scened.context.scened_faction')

function ScenedObjectManager:ctor()
	--binlog对象对照类型前缀索引
	self.binlogTypes = {}
	--self.binlogTypes[guidMgr.ObjectTypeUnit] = ScenedContext --生物
	self.binlogTypes[guidMgr.ObjectTypeFaction] = ScenedFaction --帮派
	super(self)
end

--远程创建新对象后触发
function ScenedObjectManager:afterAttachObject( o )
	--outFmtInfo("ScenedObjectManager %s afterAttachObject", tostring(o.class))
end

return ScenedObjectManager
