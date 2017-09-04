local security = require("base/Security")
		
-- 定时器的检测跨服匹配情况
function PlayerInfo:QueryKuafuMatchInfo()
	self:OnCheckWorld3v3Match()
	self:OnCheckWorldXianfuMatch()
	self:OnCheckGroupInstanceMatch()
end

-- 人物离线取消所有匹配
function PlayerInfo:OnCancelKuafuMatch()
	self:OnCancelWorld3v3MatchBeforeOffline()
	self:OnCancelWorldXianfuMatchBeforeOffline()
	self:OnCancelGroupInstanceMatchBeforeOffline()
end

-- 进行奖励检查
function PlayerInfo:CheckMatchReward()
	-- 跨服不检查
	local isPkServer = globalGameConfig:IsPKServer()
	if isPkServer then
		return
	end
	
	--self:CheckWorld3v3Reward()
	--[[
	self:CheckWorldXianfuReward()
	self:OnSyncMoney()
	--]]
	self:CheckGroupInstanceReward()
end

-- 匹配取消
function PlayerInfo:OnCancelMatch(kuafu_type)
	if app:IsKuafuTypeMatching(self:GetGuid(), kuafu_type) then
		app:SetMatchingKuafuType(self:GetGuid(), nil)
		self:call_kuafu_3v3_cancel_match(kuafu_type)
	end
end
