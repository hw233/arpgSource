--技能管理器

local AppSocialMgr = class("AppSocialMgr", BinLogObject)

function AppSocialMgr:ctor()
	
end

----------------------------------------基础技能部分------------------------------------
--添加一条数据
function AppSocialMgr:addSocialItem(player,index,fam)
	fam = fam or 0
	local name = player:GetName()
	local guid	= player:GetGuid()
	local level = player:GetLevel()
	local gender = player:GetGender()
	local vip = player:GetVIP()
	local faction = player:GetFactionName()
	local force = player:GetForce()

	--图标 vip 等级
	self:SetByte(index,0,gender)
	self:SetByte(index,1,vip)
	self:SetUInt16(index,1,level)

	--亲密度设置
	self:SetUInt16(index+1,0,fam)
	self:SetByte(index+1,2,1)
	self:SetByte(index+1,3,1)
	--self:SetUInt16(index+1,1,1)
	
	--战力
	self:SetDouble(index+SOCIAL_FAMILIAR_FORCE,force)
	--guid
	self:SetStr(index,guid)
	--name faction
	local str = name .. "\1" .. faction
	--outFmtDebug("addSocialItem %d",index)
	--outFmtDebug("addSocialStr %s",str)
	self:SetStr(index+1,str)
end
--删除好友
function AppSocialMgr:removeFriend(guid)
	local idx = self:getFriendIndex(guid)
	if idx ~= -1 then
		self:clearSocialItem(idx)
		self:setFriendIndex(guid,-1)
	end
end
--通知好友上线
function AppSocialMgr:broadcastFriendOnline(online)
	local owner = self:getOwner()
	local selfGuid = owner:GetGuid()
	for i=SOCIAL_FRIEND_START,SOCIAL_FRIEND_END-1,MAX_FRIENT_COUNT do
		local guid = self:getGuid(i)
		if guid ~= "" then
			--outFmtDebug("tong zhi shang xian %s",guid)
			local friend = app.objMgr:getObj(guid)
			if friend then 
				local tf = friend:FriendOnline(selfGuid,online)
				--通知好友上线但好友拒绝 说明自己已经被好友删除 自己也删除好友
				if not tf then
					owner:RemoveFriend(guid,false)
				end
			end
			
		end
	end
end
--刷新在线状态
function AppSocialMgr:resetOnlineState()
	for i=SOCIAL_FRIEND_START,SOCIAL_ENEMY_END-1,MAX_FRIENT_COUNT do
		local guid = self:getGuid(i)
		if guid ~= "" then
			if app.objMgr:getObj(guid) then
				self:SetByte(i+1,3,1)
			else
				self:SetByte(i+1,3,0)
			end
		end
	end
end

function AppSocialMgr:resetInfo()
	for i=SOCIAL_FRIEND_START,SOCIAL_FRIEND_END-1,MAX_FRIENT_COUNT do
		self:resetItemInfo(i,true)
	end
	for i=SOCIAL_ENEMY_START,SOCIAL_ENEMY_END-1,MAX_FRIENT_COUNT do
		self:resetItemInfo(i,false)
	end
end
function AppSocialMgr:resetItemInfo(index,needclear)
	local owner = self:getOwner()
	local selfGuid = owner:GetGuid()
	local guid = self:getGuid(index)
	if guid ~= "" then
		local friend = app.objMgr:getObj(guid)

		if friend then
			--判断自己还是否是对方好友
			--outFmtDebug("test friend %s",guid)
			if needclear and (not friend:isFriend(selfGuid)) then
				--outFmtDebug("time clear friend")
				owner:RemoveFriend(guid,false)
				return
			end

			local level = friend:GetLevel()
			local vip = friend:GetVIP()
			local name = friend:GetName()
			local faction = friend:GetFactionName()
			local force = friend:GetForce()
			
			local baseLev = self:GetUInt16(index,1)
			if baseLev ~= level then
				self:SetUInt16(index,1,level)
			end

			local basevip = self:GetByte(index,1)
			if basevip ~= vip then
				self:SetByte(index,1,vip)
			end
			
			local str = name .. "\1" .. faction
			local basestr = self:GetStr(index+1)
			if basestr ~= str then
				self:SetStr(index+1,str)
			end
			
			local baseforce = self:GetDouble(index+SOCIAL_FAMILIAR_FORCE)
			if baseforce ~= force then
				self:SetDouble(index+SOCIAL_FAMILIAR_FORCE,force)
			end
		end
	end
end
--维护map
function AppSocialMgr:setPlayerLib()
	--设置好友idx
	for i=SOCIAL_FRIEND_START,SOCIAL_FRIEND_END-1,MAX_FRIENT_COUNT do
		local guid = self:getGuid(i)
		if guid ~= "" then
			self:setFriendIndex(guid, i)
		end
	end
	--设置仇人idx
	for i=SOCIAL_ENEMY_START,SOCIAL_ENEMY_END-1,MAX_FRIENT_COUNT do
		local guid = self:getGuid(i)
		if guid ~= "" then
			self:setEnemyIndex(guid, i)
		end
	end

end


--获取一条数据信息
function AppSocialMgr:getSocilaItem(index)
	local i1 = self:GetUInt32(index)
	local i2 = self:GetUInt32(index+1)
	local s1 = self:GetStr(index)
	local s2 = self:GetStr(index+1)

	return i1,i2,s1,s2
end
--设置一条数据信息
function AppSocialMgr:setSocilaItem(index,i1,i2,s1,s2)
	self:SetUInt32(index,i1)
	self:SetUInt32(index+1,i2)
	self:SetStr(index,s1)
	self:SetStr(index+1,s2)
end

--是否是朋友
function AppSocialMgr:isFriend(guid)
	local idx = self:getFriendIndex(guid)
	--outFmtDebug("pan duan peng you  %d",idx)
	if idx ~= -1 then
		return true
	end
	return false
	-- for i=SOCIAL_FRIEND_START,SOCIAL_FRIEND_END,MAX_FRIENT_COUNT do
	-- 	if self:getGuid(i) == guid then
	-- 		return true
	-- 	end
	-- end
	-- return false
end
--是否在申请列表中
function AppSocialMgr:isApply(guid)
	for i=SOCIAL_APPLY_START,SOCIAL_APPLY_END-1,MAX_FRIENT_COUNT do
		if self:getGuid(i) == guid then
			return true
		end
	end
	return false
end
--根据guid获取index
function AppSocialMgr:getApplyIndex(guid)
	for i=SOCIAL_APPLY_START,SOCIAL_APPLY_END-1,MAX_FRIENT_COUNT do
		if self:getGuid(i) == guid then
			return i
		end
	end
	return -1
end

function AppSocialMgr:getFriendIndex(guid)
	local owner = self:getOwner()
	return playerLib.GetSocialFriend(owner.ptr, guid)
	-- for i=SOCIAL_FRIEND_START,SOCIAL_FRIEND_END,MAX_FRIENT_COUNT do
	-- 	if self:getGuid(i) == guid then
	-- 		return i
	-- 	end
	-- end
	-- return -1
end

function AppSocialMgr:setFriendIndex(guid,index)
 	local owner = self:getOwner()
	playerLib.SetSocialFriend(owner.ptr, guid,index)
end

function AppSocialMgr:getEnemyIndex(guid)
	local owner = self:getOwner()
	return playerLib.GetSocialEnemy(owner.ptr, guid)
	-- for i=SOCIAL_ENEMY_START,SOCIAL_ENEMY_END,MAX_FRIENT_COUNT do
	-- 	if self:getGuid(i) == guid then
	-- 		return i
	-- 	end
	-- end
	-- return -1
end
function AppSocialMgr:setEnemyIndex(guid,index)
	local owner = self:getOwner()
	playerLib.SetSocialEnemy(owner.ptr, guid,index)
end
--当前朋友的数量
function  AppSocialMgr:getFriendNum()
	local num = 0;
	for i=SOCIAL_FRIEND_START,SOCIAL_FRIEND_END-1,MAX_FRIENT_COUNT do
		if self:getGuid(i) ~= '' then
			num = num + 1
		end
	end
	return num
end
--获取一个空的朋友位
function AppSocialMgr:getEmptyFriendIndex()
	for i=SOCIAL_FRIEND_START,SOCIAL_FRIEND_END-1,MAX_FRIENT_COUNT do
		if self:getGuid(i) == '' then
			return i
		end
	end
	return -1
end
--获取一个空的申请位
function AppSocialMgr:getEmptyApplyIndex()
	for i=SOCIAL_APPLY_START,SOCIAL_ENEMY_END-1,MAX_FRIENT_COUNT do
		if self:getGuid(i) == '' then
			return i
		end
	end
	return -1
end
--获取一个空的仇人位
function AppSocialMgr:getEmptyEnemyIndex()
	for i=SOCIAL_ENEMY_START,SOCIAL_ENEMY_END-1,MAX_FRIENT_COUNT do
		--outFmtDebug("chouren ------------- %d",i)
		if self:getGuid(i) == '' then
			return i
		end
	end
	return -1
end
--下线清空申请列表
function AppSocialMgr:clearApplyList()
	for i=SOCIAL_APPLY_START,SOCIAL_APPLY_END-1,MAX_FRIENT_COUNT do
		local gid = self:GetStr(i)
		if gid ~= "" then
			self:SetUInt32(i,0)
			self:SetUInt16(i+1,0,0)
			self:SetByte(i+1,2,1)
			self:SetByte(i+1,3,0)
			self:SetStr(i,"")
			self:SetStr(i+1,"")
		end
	end
	self:SetUInt32(SOCIAL_APPLY_CLEAR_FLAG,0)
end
--清除一条数据
function AppSocialMgr:clearSocialItem(index)
	self:SetUInt32(index,0)
	self:SetUInt16(index+1,0,0)
	self:SetByte(index+1,2,1)
	self:SetByte(index+1,3,0)
	self:SetStr(index,"")
	self:SetStr(index+1,"")
end
--添加申请列表
function AppSocialMgr:addApplyPlayer(player)
	local idx = self:getEmptyApplyIndex()

	if idx == -1 then
		local flag = self:GetUInt32(SOCIAL_APPLY_CLEAR_FLAG)
		idx = SOCIAL_APPLY_START + flag * MAX_FRIENT_COUNT
		flag = flag + 1
		if flag >= SOCIAL_APPLY_MAX_NUM then
			flag = 0;
		end
		self:SetUInt32(SOCIAL_APPLY_CLEAR_FLAG,flag)
	end

	self:addSocialItem(player,idx)

end
--添加朋友
function AppSocialMgr:addFriendPlayer(player)
	local idx = self:getEmptyFriendIndex()
	if idx == -1 then
		return false;
	end
	self:addSocialItem(player,idx)
	self:setFriendIndex(player:GetGuid(), idx)
	return true
end
--添加敌人仇恨值
function AppSocialMgr:addEnemyPlayerNum(player,guid,num)
	local idx = self:getEnemyIndex(guid)
	--如果在仇人列表
	if idx ~= -1 then
		local hatred = self:GetUInt16(idx + 1,0)
		hatred = hatred + num
		self:SetUInt16(idx + 1,0,hatred)
		self:setEnemyTime(idx)
	else
		--outFmtDebug("begin chouren kong wei")
		local emIdx = self:getEmptyEnemyIndex(guid)
		--outFmtDebug("chou ren kong wei %d",emIdx)
		--如果有空位则添加
		if emIdx ~= -1 then
			self:addSocialItem(player,emIdx,num)
			self:setEnemyIndex(guid, emIdx)
			self:setEnemyTime(emIdx)
		else--如果没有空位找到时间最少并且仇恨值最少的
			local fuIdx = self:minHatredTimeIndex()
			--清空原来的仇人
			local fulguid = self:GetStr(fuIdx)
			self:setEnemyIndex(fulguid, -1)
			--设置新的仇人
			self:addSocialItem(player,fuIdx,num)
			self:setEnemyIndex(guid,fuIdx)
			self:setEnemyTime(fuIdx)
		end
	end
end
--仇人时间戳
function AppSocialMgr:setEnemyTime(index)
	local idx = (index - SOCIAL_ENEMY_START) / MAX_FRIENT_COUNT + SOCIAL_ENEMY_TIME_START
	self:SetUInt32(idx,os.time())
end
function AppSocialMgr:getEnemyTime(index)
	local idx = (index - SOCIAL_ENEMY_START) / MAX_FRIENT_COUNT + SOCIAL_ENEMY_TIME_START
	return self:GetUInt32(idx)
end
function AppSocialMgr:minHatredTimeIndex()
	local minnum = 1073741824
	local minTime = os.time()
	local idx = -1

	for i=SOCIAL_ENEMY_START, SOCIAL_ENEMY_END-1, MAX_FRIENT_COUNT do
		local hatred = self:GetUInt16(i + 1,0)
		local time = self:getEnemyTime(i)
		if hatred < minnum or (hatred == minnum and  time < minTime) then
			minnum = hatred
			minTime = time
			idx = i
		end
	end

	return idx

end

function AppSocialMgr:isEnemy(guid)
	local idx = self:getEnemyIndex(guid)
	if idx ~= -1 then
		return true
	end
	return false
end

function AppSocialMgr:removeEnemy(guid)
	local idx = self:getEnemyIndex(guid)
	if idx ~= -1 then
		self:clearSocialItem(idx)
		self:setEnemyIndex(guid,-1)
	end
end


--设置好友在线状态
function AppSocialMgr:setFriendOnLine(guid,online)
	local idx = self:getFriendIndex(guid)

	if idx == -1 then
		return
	end

	if online then
		self:SetByte(idx + 1,3,1)
	else
		self:SetByte(idx + 1,3,0)
	end

end

function AppSocialMgr:getGuid(index)
	return self:GetStr(index)
end
--添加好友度
function AppSocialMgr:addFamiliay(guid,num)
	local idx = self:getFriendIndex(guid)

	if idx == -1 then
		return
	end
	--outFmtDebug("idx %d",idx)
	local fam = self:GetUInt16(idx+1,0)
	local famlev = self:GetByte(idx+1,2)
	--outFmtDebug("current fam %d,%d",fam,num)
	fam = fam + num
	--outFmtDebug("current fam %d,%d,%d",fam,num,famlev)
	for i=famlev,#tb_social_familiay do
		local config = tb_social_familiay[i]
		if fam >= config.exp then
			fam = fam - config.exp
			famlev = famlev + 1
		else
			break
		end
	end

	self:SetUInt16(idx+1,0,fam)
	--self:SetUInt16(idx+1,1,famlev)
	self:SetByte(idx+1,2,famlev)
	--self:SetByte(index+1,3,0)

end

-- 获得玩家guid
function AppSocialMgr:getPlayerGuid()
	--物品管理器guid转玩家guid
	if not self.owner_guid then
		self.owner_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypePlayer)
	end
	return self.owner_guid
end

--获得技能管理器的拥有者
function AppSocialMgr:getOwner()
	local playerguid = self:getPlayerGuid()
	return app.objMgr:getObj(playerguid)
end


return AppSocialMgr