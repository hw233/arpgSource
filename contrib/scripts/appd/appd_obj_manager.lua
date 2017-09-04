require 'util.functions'

local ObjectManager = require('util.object_manager')
local AppObjectManager = class('AppObjectManager', ObjectManager)
local AppItemMgr = require('appd.appd_item_mgr')
local AppLogicalMgr = require('appd.appd_logical_mgr')
local FactionInfo = require('appd.faction_info')
local FactionDataInfo = require('appd.faction_data_info')
local AppSpellMgr = require('appd.appd_spell_manager')
local AppInstanceMgr = require('appd.appd_instance_manager')
local AppSocialMgr = require('appd.appd_social_manager')
local AppQuestMgr = require('appd.appd_quest_manager')

function AppObjectManager:ctor( )
	self.binlogTypes = {}

	--binlog对象对照类型前缀索引
	self.binlogTypes[guidMgr.ObjectTypePlayer]   = PlayerInfo	--玩家
	self.binlogTypes[guidMgr.ObjectTypeSocial] = AppSocialMgr		--社交
	self.binlogTypes[guidMgr.ObjectTypeGiftPacks] = GiftPacksInfo	--礼包对象
	--self.binlogTypes[guidMgr.ObjectTypeLimit] = LimitActivityBase	--限时活动
	self.binlogTypes[guidMgr.ObjectTypeItemMgr]  = AppItemMgr 	--道具
	self.binlogTypes[guidMgr.ObjectTypeLogical]  = AppLogicalMgr --业务逻辑
	self.binlogTypes[guidMgr.ObjectTypeFaction]  = FactionInfo	--帮派
	self.binlogTypes[guidMgr.ObjectTypeFactionData]  = FactionDataInfo	--帮派数据
	self.binlogTypes[guidMgr.ObjectTypeSpell]    = AppSpellMgr	--技能
	self.binlogTypes[guidMgr.ObjectTypeInstance] = AppInstanceMgr	--副本
	self.binlogTypes[guidMgr.ObjectTypeQuest]    = AppQuestMgr	--任务
	
	--由于基类会构建实例，所有需要在调用基类构造函数前给类型表赋值
	super(self)	
	self.account2guid = {}    
end

--远程创建新对象后触发
function AppObjectManager:afterAttachObject( o )
	if o.class == AppItemMgr then
		o:init()
	end
end

function AppObjectManager:getByAccount( account )
	local guid = self.account2guid[account]
	if not guid then
		objects.foreach("^p[[:digit:]]", function ( g )
			local o = self:getObj(g)
			if o and o:GetAccount() == account then
				self.account2guid[o:GetAccount()] = o:GetGuid()
				guid = o:GetGuid()
				return true		--返回true的话就不会在继续遍历后面的了
			end
		end)
	end
	--如果存在guid则返回
	return guid and self:getObj(guid) or nil
end

--遍历所有玩家
function AppObjectManager:foreachAllPlayer(func)
	objects.foreach("^p[[:digit:]]", function ( g, o )
		local player = self:fromPtr(o)
		if player then return func(player) end
	end)	
end

--遍历所有帮派
function AppObjectManager:foreachAllFaction(func)
	objects.foreach("^L[[:digit:]]", function ( g )
		local faction = self:getObj(g)
		if faction then return func(faction) end
	end)
end

--遍历所有队伍
function AppObjectManager:foreachAllGroup(func)
	objects.foreach("^T[[:digit:]]", function ( g )
		local faction = self:getObj(g)
		if faction then return func(faction) end
	end)
end

-- 判断GUID是否是帮派的guid
function AppObjectManager:IsFactionGuid(guid)
	return string.sub(guid, 1, 1) == guidMgr.ObjectTypeFaction
end

-- 判断GUID是否是帮派的guid
function AppObjectManager:IsPlayerGuid(guid)
	return string.sub(guid, 1, 1) == guidMgr.ObjectTypePlayer
end

return AppObjectManager
