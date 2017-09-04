-- 读邮件
function PlayerInfo:Read_Mail(indx)
	
	local giftPacksInfo = self:getGiftPacksInfo()
	giftPacksInfo:mailRead(indx)
end

-- 领取邮件
function PlayerInfo:Pick_Mail(indx)

	local giftPacksInfo = self:getGiftPacksInfo()
	giftPacksInfo:pickMail(self, indx)
end

-- 删除邮件
function PlayerInfo:Remove_Mail(indx)
	--[[
	local giftPacksInfo = self:getGiftPacksInfo()
	giftPacksInfo:removeMail(indx)
	--]]
end

-- 一键领取邮件
function PlayerInfo:Pick_Mail_One_Step()
	
	local giftPacksInfo = self:getGiftPacksInfo()
	giftPacksInfo:pickMailOneStep(self)
end

-- 一键删除邮件
function PlayerInfo:Remove_Mail_One_Step()
	
	local giftPacksInfo = self:getGiftPacksInfo()
	giftPacksInfo:removeMailOneStep() 
end