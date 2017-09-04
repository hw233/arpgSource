
--客户端完成当前引导
function PlayerInfo:Hanlde_Finish_Now_Guide(pkt)
	self:SetGuideIdLast(self:GetGuideIdNow())
end

-- 完成非强制引导步骤
function PlayerInfo:Handle_Optional_Guide_Step(pkt)
	self:OptionalGuideClicked(pkt.guide_id, pkt.step)
end