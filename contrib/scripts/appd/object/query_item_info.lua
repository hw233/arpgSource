--查看物品信息
local QueryItemInfo = class('QueryItemInfo', BinLogObject)

--------------------------查看物品----------------------------------
function QueryItemInfo:ctor(ptr)
	self.ptr = ptr
end

--添加一个查看信息
function QueryItemInfo:addItemQueryInfo( str )
	self:SetStr(ITEM_QUERY_STR_FIELD_START + self:GetUInt32(ITEM_QUERY_INT_FIELD_COUNT),str)
	
	self:AddUInt32(ITEM_QUERY_INT_FIELD_COUNT, 1)
end

return QueryItemInfo