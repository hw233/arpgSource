local protocols = require('share.protocols')

function PlayerInfo:RefreshFriendInfo()
	local socialMgr = self:getSocialMgr()
	socialMgr:resetInfo()
end

function PlayerInfo:socialLogIn()
	--登录刷新好友在线状态
	local socialMgr = self:getSocialMgr()
	socialMgr:setPlayerLib()
	socialMgr:resetOnlineState()
	socialMgr:broadcastFriendOnline(true)
end
function PlayerInfo:socialLogOut()
	local socialMgr = self:getSocialMgr()
	socialMgr:clearApplyList()
	socialMgr:broadcastFriendOnline(false)
end

function PlayerInfo:FriendOnline(guid,online)
	local socialMgr = self:getSocialMgr()
	if socialMgr:isFriend(guid) then
		socialMgr:setFriendOnLine(guid,online)
		return true
	else 
		return false
	end

end

--清空申请列表
function PlayerInfo:ClearApply()
	local socialMgr = self:getSocialMgr()
	socialMgr:clearApplyList()
end

-- 删除好友 是否双向
function PlayerInfo:RemoveFriend(guid,twoway)
	local socialMgr = self:getSocialMgr()
	--outFmtDebug("shan chu hao you %s",guid)
	if not socialMgr:isFriend(guid) then
		outFmtDebug("player is not already friend, %s", guid)
		return false
	end
	socialMgr:removeFriend(guid)

	if twoway then
		local friend = app.objMgr:getObj(guid)
		--outFmtDebug("hao you shan zi ji %s",self:GetGuid())
		if friend then
			friend:RemoveFriend(self:GetGuid(),false)
		end
	end


end
--好友是否已满
function PlayerInfo:FriendIsFull()
	local lev = self:GetLevel()
	local config = tb_social_num[lev]
	if config then
		local maxnum = config.num
		local socialMgr = self:getSocialMgr()
		local curnum = socialMgr:getFriendNum()
		if curnum >= maxnum then
			return true
		else 
			return false
		end
	else 
		return true
	end
	
end
--申请添加好友
function PlayerInfo:ApplyFriend(guid)
	if guid == self:GetGuid() then
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ADD_MYSELF)
		return false
	end
	
	local friend = app.objMgr:getObj(guid)
	if not friend then 
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_NOT_FIND)
		return false
	end
	
	if self:FriendIsFull() then
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_SELF_FULL)
		return false
	end
	
	if friend:FriendIsFull() then
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_TARGET_FULL)
		return false
	end
	
	--向目标申请加好友
	friend:AddApplyFriend(self)
end
--确认添加好友
function PlayerInfo:SureApplyFriend(guid)
	local socialMgr = self:getSocialMgr()
	local idx = socialMgr:getApplyIndex(guid)
	
	--是否在好友列表中
	if idx ~= -1 then
		--对方是否还在线
		
		local friend = app.objMgr:getObj(guid)
		if not friend then 
			--清除申请对象
			socialMgr:setSocilaItem(idx,0,0,"","")
			--outFmtDebug("friend not on line or not extis, %s", guid)
			self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_NOT_FIND)
			return false
		end
		
		local addTest = friend:CanAddFriend(self)
		if addTest ~= 0 then
			if addTest == 1 then
				self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_TARGET_FULL)
			elseif addTest == 2 then
				self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ADD_MYSELF)
			elseif addTest == 3 then
				self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ALREADY_FRIEND)
			end
			socialMgr:setSocilaItem(idx,0,0,"","")
			return false
		end
		--if not friend:CanAddFriend(self) then
		--	socialMgr:setSocilaItem(idx,0,0,"","")
		--	return false
		--end
		
		addTest = self:CanAddFriend(friend)
		if addTest ~= 0 then
			if addTest == 1 then
				self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_SELF_FULL)
			elseif addTest == 2 then
				self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ADD_MYSELF)
			elseif addTest == 3 then
				self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ALREADY_FRIEND)
			end
			socialMgr:setSocilaItem(idx,0,0,"","")
			return false
		end
		
		--if not self:CanAddFriend(friend) then
		--	socialMgr:setSocilaItem(idx,0,0,"","")
		--	return false
		--end


		if friend:AddFriend(self) == false then
			--清除申请对象
			socialMgr:setSocilaItem(idx,0,0,"","")
			return false
		end
		--清理申请数据
		socialMgr:setSocilaItem(idx,0,0,"","")
		self:AddFriend(friend)
		
		self:AddAchieve(QUEST_TARGET_TYPE_FRIEND_NUM,1)
		friend:AddAchieve(QUEST_TARGET_TYPE_FRIEND_NUM,1)
		
		return true
	else
		socialMgr:setSocilaItem(idx,0,0,"","")
	end
	return false
end

--添加申请好友
function PlayerInfo:AddApplyFriend(friend)
	local socialMgr = self:getSocialMgr()
	local guid = friend:GetGuid();

	--判断是不是好友
	if socialMgr:isFriend(guid) then
		--outFmtDebug("player is already friend, %s", guid)
		friend:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ALREADY_FRIEND)
		return false
	end
	--判断是不是自己
	if self:GetName() == guid then
		--outFmtDebug("you self is a friend, %s", guid)
		friend:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ADD_MYSELF)
		return false
	end

	--判断是不是已经在列表中
	if socialMgr:isApply(guid) then
		--outFmtDebug("player is already in apply, %s", guid)
		friend:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_HAS_SEND_ADD)
		return false
	end

	socialMgr:addApplyPlayer(friend)

end
--直接添加好友
function PlayerInfo:AddFriend(friend)
	local socialMgr = self:getSocialMgr()
	local guid = friend:GetGuid()

	--判断是不是好友
	if socialMgr:isFriend(guid) then
		--outFmtDebug("player is already friend, %s", guid)
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ALREADY_FRIEND)
		return false
	end
	--判断是不是自己
	if self:GetName() == guid then
		--outFmtDebug("you self is a friend, %s", guid)
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ADD_MYSELF)
		return false
	end

	
	
	if self:FriendIsFull() then
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_SELF_FULL)
		return false
	end
	
	local tf = socialMgr:addFriendPlayer(friend)
	
	--if tf == false then
	--	outFmtDebug("target friend is full")
	--end

	return tf

end
-- 0 可添加 1 列表已满 2 添加自己 3 已经是好友
function PlayerInfo:CanAddFriend(friend)
	local socialMgr = self:getSocialMgr()
	local guid = friend:GetGuid()

	--判断是不是好友
	if socialMgr:isFriend(guid) then
		--outFmtDebug("player is already friend, %s", guid)
		--self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ALREADY_FRIEND)
		return 3
	end
	--判断是不是自己
	if self:GetName() == guid then
		--outFmtDebug("you self is a friend, %s", guid)
		--self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_ADD_MYSELF)
		return 2
	end

	if self:FriendIsFull() then
		--self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_SELF_FULL)
		return 1
	end
	
	local idx = socialMgr:getEmptyFriendIndex()
	if idx == -1 then
		return 1
	end

	return 0
end
--赠送礼物
function PlayerInfo:AddGiftFriend(guid,gift)
	local friend = app.objMgr:getObj(guid)
	if not friend then 
		--outFmtDebug("friend not on line or not extis, %s", guid)
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_NOT_FIND)
		return
	end
	local famNum = 0
	local money = {}
	for _,v in pairs(gift) do
		local config = tb_social_shop[v.item_id]
		if not config then
			outFmtDebug("cannot find tb_social_shop data, %d", v.item_id)
			return
		end
		famNum = famNum + config.familiay
		local costTab = config.costResource
		for _, res in pairs(costTab) do
			AddTempInfoIfExist(money,res[1],res[2])
		end
	end
	

	--是否有足够的资源
	if not self:checkMoneyEnoughs(money) then
		--outFmtError("resouce not enough")
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_NO_MONEY)
		return
	end

	if self:costMoneys(MONEY_CHANGE_SOCIAL_GIFT,money) then
		--同时增加双方好友度
		self:AddFamiliay(famNum,friend:GetGuid())
		friend:AddFamiliay(famNum,self:GetGuid())

		--给东西
		for _,v in pairs(gift) do
			local config = tb_social_shop[v.item_id]
			friend:AddItemByEntry(config.itemId, v.num, nil, 8, true)--FIXME
		end
		
		-- 加任务
		local questMgr = self:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_FRIEND_DONATION)
	end

end

function PlayerInfo:RecommendFriend()
	playerLib.GetAndSendRecommendFriends(self:GetGuid())
end
--增加亲密度
function PlayerInfo:AddFamiliay(num,guid)
	local socialMgr = self:getSocialMgr()
	socialMgr:addFamiliay(guid,num)
end
--增加仇恨度 仇人id 仇恨度
function PlayerInfo:AddEnemy(guid,num)
	local isPkServer = globalGameConfig:IsPKServer()
	if isPkServer then
		return
	end
	
	local socialMgr = self:getSocialMgr()
	--socialMgr:addFamiliay(guid,num)
	local enemy = app.objMgr:getObj(guid)
	if not enemy then 
		--outFmtDebug("enemy not on line or not extis, %s", guid)
		self:CallOptResult(OPERTE_TYPE_SOCIAL, OPERTE_TYPE_SOCIAL_NOT_FIND)
		return
	end
	socialMgr:addEnemyPlayerNum(enemy,guid,num)
end

--是否为好友
function PlayerInfo:isFriend(guid)
	local socialMgr = self:getSocialMgr()
	return socialMgr:isFriend(guid)
end

-- 删除好友
function PlayerInfo:RemoveEnemy(guid)
	local socialMgr = self:getSocialMgr()
	if not socialMgr:isEnemy(guid) then
		outFmtDebug("player is not enemy, %s", guid)
		return false
	end
	socialMgr:removeEnemy(guid)
end