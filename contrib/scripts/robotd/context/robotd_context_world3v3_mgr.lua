--[[
		MAX_KUAFU_3V3_COUNT

		local strStart = KUAFU_3V3_FIELDS_STR_INFO_START + emptyIndex * MAX_KUAFU_3V3_STR_COUNT
		local intStart = KUAFU_3V3_FIELDS_INT_INFO_START + emptyIndex * MAX_KUAFU_3V3_INT_COUNT
		
		self:SetStr(strStart + KUAFU_3V3_PLAYER_NAME, playerInfo:GetName())
		self:SetStr(strStart + KUAFU_3V3_PLAYER_GUID, playerInfo:GetPlayerGuid())
		
		self:SetByte(intStart + KUAFU_3V3_PLAYER_SHOW_INFO, 0, playerInfo:GetGender())
		self:SetByte(intStart + KUAFU_3V3_PLAYER_SHOW_INFO, 1, 100)
		self:SetUInt16(intStart + KUAFU_3V3_PLAYER_SHOW_INFO, 1, playerInfo:GetLevel())
		self:SetByte(intStart + KUAFU_3V3_PLAYER_SETTLEMENT, 2, playerInfo:GetVirtualCamp())
--]]

function PlayerInfo:FindEnemy()
	
	self:FindUnitByGuid(guid)
end