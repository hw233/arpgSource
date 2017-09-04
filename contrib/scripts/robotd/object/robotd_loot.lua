RobotdLoot = class('RobotdLoot', BinLogObject)

--¹¹Ôìº¯Êý
function RobotdLoot:ctor()

end

function RobotdLoot:ReadData()
	local tx = self:GetUInt16(LOOT_PUBLIC_INT_START_POS, 0)
    local ty = self:GetUInt16(LOOT_PUBLIC_INT_START_POS, 1)
    local ex = self:GetUInt16(LOOT_PUBLIC_INT_END_POS, 0)
    local ey = self:GetUInt16(LOOT_PUBLIC_INT_END_POS, 1)

    local gridtx = tx
    local gridty = ty
    local gridWidth = math.floor((ex - tx) / 2) + 1
	local gridHeight = math.floor((ey - ty) / 2) + 1
    local lootCount = gridWidth * gridHeight
	
	--local lootInfo = {}
	for index = 0, lootCount - 1 do
        local intstart = LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD
		--if intstart < self:GetUInt32Len() then
			local entry = self:GetUInt32(intstart + LOOT_PUBLIC_INT_ENTRY)
			if (entry > 0) then
				local offsetx = index % gridWidth
				local offsety = math.floor(index / gridWidth)
				local x = (offsetx * 2) + gridtx
				local y = (offsety * 2) + gridty
				
				return {x, y}
				--table.insert(lootInfo, {x, y})
			end
		--end
	end

	return
end


return RobotdLoot
