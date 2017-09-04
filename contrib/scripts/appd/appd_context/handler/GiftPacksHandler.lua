-- 读邮件
function PlayerInfo:Handle_Read_Mail(pkt)
	local indx = pkt.indx
	-- 判断下标是否合法
	if indx < 0 or indx >= MAX_GIFTPACKS_INFO_COUNT then
		return
	end
	
	self:Read_Mail(indx)
end

-- 领取邮件
function PlayerInfo:Handle_Pick_Mail(pkt)
	local indx = pkt.indx
	-- 判断下标是否合法
	if indx < 0 or indx >= MAX_GIFTPACKS_INFO_COUNT then
		return
	end
	
	self:Pick_Mail(indx)
end

-- 删除邮件
function PlayerInfo:Handle_Remove_Mail(pkt)
	local indx = pkt.indx
	-- 判断下标是否合法
	if indx < 0 or indx >= MAX_GIFTPACKS_INFO_COUNT then
		return
	end	
	
	self:Remove_Mail(indx)
end

-- 一键领取邮件
function PlayerInfo:Handle_Pick_Mail_One_Step(pkt)
	
	self:Pick_Mail_One_Step()
end

-- 一键删除邮件
function PlayerInfo:Handle_Remove_Mail_One_Step(pkt)
	
	self:Remove_Mail_One_Step()
end