--[[
-- 商店购买物品
function PlayerInfo:Handle_Store_Buy(pkt)
	local itemId	= pkt.id
	local count		= pkt.count
	local timeID 	= pkt.time
	
	-- 判断道具是否存在
	if not tb_store[itemId] then
		return
	end
	
	-- 判断一次购买上限
	if count < 1 or count > tb_store[itemId].limit then
		return
	end
	
	self:storeBuyItem(itemId, count)
	
end
--]]

-- 商城购买物品
function PlayerInfo:Handle_Mall_Buy(pkt)
	local id	= pkt.id
	local count	= pkt.count
	local timeid = pkt.time
	
	-- 判断道具是否存在
	if not tb_shop[id] then
		return
	end
	
	-- 判断一次购买上限
	if count < 1 or count > tb_shop[id].limit then
		return
	end
	
	self:shopBuyItem(id, count,timeid)
	
end


--[[
-- 元宝复活
function PlayerInfo:Handle_Gold_Respawn(pkt)
	local useGold = pkt.useGold
	self:goldRespawn(useGold)
end
--]]