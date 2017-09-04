--[[
	KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START
	KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_END
	MAX_KUAFU_XIANFU_BOSS_INT_COUNT
	
	KUAFU_XIANFU_BOSS_SHOW_INFO		= 0,									// 2 shorts(0:entry, 1:死亡状态(0:未刷,1:刷新,2:死亡))
	KUAFU_XIANFU_BOSS_BORN_INFO		= KUAFU_XIANFU_BOSS_SHOW_INFO + 1,		// 是否刷新
	KUAFU_XIANFU_BOSS_BORN_TIME		= KUAFU_XIANFU_BOSS_BORN_INFO + 1,		// 刷新时间戳
--]]

function PlayerInfo:PickTreasure()
		-- 有宝箱的先捡起来
	if self.lootMgr then
		local lootInfo = self.lootMgr:ReadData()
		return lootInfo
	end
	
	return
end


function PlayerInfo:FindBossFirst()
	-- 怪物是否已经
	if self.bossObserve then
		return
	end
	
	local mapId = self:GetMapID()
	-- 先找BOSS
	local start = KUAFU_XIANFU_FIELDS_INT_BOSS_INFO_START
	for i = 1, #tb_kuafu_xianfu_boss do
		local state = self.mapInfo:GetUInt16(start + KUAFU_XIANFU_BOSS_SHOW_INFO, 1)
		if state == 1 or state == 0 then
			-- 去BOSS刷新地
			local config = tb_kuafu_xianfu_boss[ i ]
			local entry = config.bossEntry
			self.bossObserve = true
			local indx = start
			return 
			{
				entry, 
				mapId, 
				config.bossPos[ 1 ], 
				config.bossPos[ 2 ], 
				-- 是否完成
				function()
					local state = self.mapInfo:GetUInt16(indx + KUAFU_XIANFU_BOSS_SHOW_INFO, 1)
					return state == 2
				end, 
				-- 完成以后做的事
				function()
					self.bossObserve = false
				end
			}
		end
		start = start + MAX_KUAFU_XIANFU_BOSS_INT_COUNT
	end
	
	-- 再找人
	return
end

function PlayerInfo:Find3v3Enemy()
	local self_guid = self:GetGuid()
	local self_camp = 0
	local playerInfo_list = {}
	local enemy_list = {}
	
	--outFmtDebug('Find3v3Enemy: self_guid: %s',self_guid)
	
	for emptyIndex = 0,5 do
		local strStart = KUAFU_3V3_FIELDS_STR_INFO_START + emptyIndex * MAX_KUAFU_3V3_STR_COUNT
		local intStart = KUAFU_3V3_FIELDS_INT_INFO_START + emptyIndex * MAX_KUAFU_3V3_INT_COUNT
		
		local guid = self.mapInfo:GetStr(strStart + KUAFU_3V3_PLAYER_GUID)
		local camp = self.mapInfo:GetByte(intStart + KUAFU_3V3_PLAYER_SETTLEMENT, 2)
		local isDead = self.mapInfo:GetByte(intStart + KUAFU_3V3_PLAYER_SETTLEMENT, 1)
		
		--outFmtDebug('Find3v3Enemy:mapInfo guid: %s camp:%d dead:%d',guid,camp,isDead)
		if guid == self_guid then
			if isDead == 1 then
				return {}
			end
			self_camp = camp
		else
			table.insert(playerInfo_list,{guid,camp,isDead})
		end
	end
	
	if self_camp == 0 then
		return enemy_list
	end
	
	for unit_guid,unit in pairs(self.all_unit) do
		for i,info in ipairs(playerInfo_list) do
			local index = string.find(unit_guid, '%.')
			if index ~= nil then
				local guid = string.sub(unit_guid, index + 1, string.len(unit_guid))
				if(guid == info[1])then
				
					if info[2] ~= self_camp and info[3] ~= 1 then
						if unit ~= nil then 
							--outFmtDebug('Find3v3Enemy enemy list added  guid: %s camp:%d',info[1],info[2])
							table.insert(enemy_list,unit)
						end
					end
				end
			end
			
			
		end
	end
	
	
	
	return enemy_list
end