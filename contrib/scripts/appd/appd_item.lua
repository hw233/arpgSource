--物品接口封装
local AppItem = class("AppItem")

function AppItem:ctor( item )
	if item then
		self.item = item
	else
		self.item = Item()
	end
	
end

--通过字符串初始化物品
function AppItem:fromString( str )
	return self.item:FromString(str)
end

--获得物品字符串数据
function AppItem:toString(  )
	return self.item:ToString()
end

--获得物品id
function AppItem:getId( )
	return self.item:GetId( )
end

--获得物品guid
function AppItem:getGuid( )
	return self.item:GetGuid( )
end


--获得物品模板
function AppItem:getEntry( )
	return self.item:GetEntry( )
end

--设置物品模板
function AppItem:setEntry( entry )
	self.item:SetEntry( entry )
end

--获得物品叠加数
function AppItem:getCount( )
	return self.item:GetCount( )
end

--设置物品叠加数
function AppItem:setCount( count )
	self.item:SetCount( count )
end

--获得失效时间
function AppItem:getFailTime( )
	return self.item:GetAttr( GetAttrKey({[1] = ITEM_OTHER_ATTR_FAIL_TIME})[1] )
end

--设置失效时间
function AppItem:setFailTime( tm )
	self.item:SetAttr( GetAttrKey({[1] = ITEM_OTHER_ATTR_FAIL_TIME})[1], tm )
end

--获得物品强化等级
function AppItem:getStrongLv( )
	return self.item:GetAttr( GetAttrKey({[1] = ITEM_OTHER_ATTR_STRONG_LV})[1] )
end

--设置物品强化等级
function AppItem:setStrongLv( lv )
	self.item:SetAttr( GetAttrKey({[1] = ITEM_OTHER_ATTR_STRONG_LV})[1], lv )
end

--获得物品强化失败次数
function AppItem:getFailNum()
	return self.item:GetAttr( GetAttrKey({[1] = ITEM_OTHER_ATTR_STRONG_FAIL_COUNT})[1] )
end

--设置物品强化失败次数
function AppItem:setFailNum( count )
	self.item:SetAttr( GetAttrKey({[1] = ITEM_OTHER_ATTR_STRONG_FAIL_COUNT})[1], count )
end

--获得物品战斗力
function AppItem:getForce( )
	return self.item:GetAttr( GetAttrKey({[1] = ITEM_OTHER_ATTR_FORCE})[1] )
end

--设置物品战斗力
function AppItem:setForce( force )
	self.item:SetAttr( GetAttrKey({[1] = ITEM_OTHER_ATTR_FORCE})[1], force )
end

--设置物品为绑定
function AppItem:setBind()
	self.item:SetBit(ITEM_FLAGS_IS_BINDED)
end

--获得物品是否为绑定
function AppItem:isBind()
	return self.item:GetBit(ITEM_FLAGS_IS_BINDED)
end

--设置物品为已鉴定
function AppItem:setApparisal()
	self.item:SetBit(ITEM_FLAGS_APPRAISAL)
end

--获得物品是否为已鉴定
function AppItem:isAppaisal()
	return self.item:GetBit(ITEM_FLAGS_APPRAISAL)
end

--设置物品为极品装备
function AppItem:setJipin()
	self.item:SetBit(ITEM_FLAGS_IS_JIPIN)
end

--获得物品是否为极品装备
function AppItem:isJipin()
	return self.item:GetBit(ITEM_FLAGS_IS_JIPIN)
end

--获得属性值
function AppItem:getAttr( attr_type )
	return self.item:GetAttr(GetAttrKey({[1] = attr_type})[1])
end

--设置属性值
function AppItem:setAttr( attr_type, val )
	 self.item:SetAttr(GetAttrKey({[1] = attr_type})[1], val)
end
--设置附加属性
function AppItem:setAddAttr(key,val)
	 self.item:SetAttr(key, val)
end
--设置基础属性
function AppItem:setBaseAttr(pos,key,val,qua)
	self.item:SetBaseAttr(pos,key,val,qua)
end
--添加基础属性
function AppItem:addBaseAttr(key,val,qua)
	self.item:AddBaseAttr(key,val,qua)
end
--清空基础属性
function AppItem:clearBaseAttr()
	self.item:ClearBaseAttr()
end
--遍历基础属性
function AppItem:forEachBaseAttr( func )
	self.item:ForEachBaseAttr(func)
end


--遍历属性对
function AppItem:forEachAttr( func )
	for i = 1, MAX_EQUIP_ATTR-1 do
		local val = self:getAttr(i)
		if val ~= 0 then
			func(i, val)
		end
	end
end

--清空所有属性
function AppItem:clearAttr()
	--local fail_time = self:getAttr(ITEM_OTHER_ATTR_FAIL_TIME)
	--local strong_lv = self:getAttr(ITEM_OTHER_ATTR_STRONG_LV)
	self.item:ClearAttr()				--清空所有属性对	
	--把特殊属性设回去
	--if fail_time > 0 then self:setAttr(ITEM_OTHER_ATTR_FAIL_TIME, fail_time) end
	--if strong_lv > 0 then self:setAttr(ITEM_OTHER_ATTR_STRONG_LV, strong_lv) end
	
end

--判断是佛可以叠加
function AppItem:canOverlay(item)
	if self:getEntry() == item:getEntry() and self:getFailTime() == item:getFailTime() and self:isBind() == item:isBind() then
		return true
	end
	return false
end


return AppItem
