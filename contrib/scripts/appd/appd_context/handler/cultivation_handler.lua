--ÐÞÁ¶³¡handle²¿·Ö

function PlayerInfo:Hanlde_Get_Cultivation_Info(pkt)
	self:GetCultivationInfo()
end
function PlayerInfo:Hanlde_Get_Cultivation_Rivals_Info(pkt)
	self:GetCultivationRivalsInfo()
end
function PlayerInfo:Hanlde_Get_Cultivation_Reward(pkt)
	self:GetCultivationReward()
end
function PlayerInfo:Hanlde_Refresh_Cultivation_Rivals(pkt)
	self:RefreshCultivationRivals()
end
function PlayerInfo:Hanlde_Plunder_Cultivation_Rival(pkt)
	local index = pkt.index
	if index < 0 or index > 3 then
		return
	end
	self:PlunderCultivationRival(index)
end
function PlayerInfo:Hanlde_Revenge_Cultivation_Rival(pkt)
	local index = pkt.index
	if index < 0 or index > 9 then
		return
	end
	self:RevengeCultivationRival(index)
end
function PlayerInfo:Hanlde_Buy_Cultivation_Left_Plunder_Count(pkt)
	local count = pkt.count
	if count <= 0 then
		return
	end
	self:BuyCultivationLeftPlunderCount(count)
end