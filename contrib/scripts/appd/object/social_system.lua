SocialSystem = class('SocialSystem', BinLogObject)

--将好友信息设置进入社交对象的下标当中
function SocialSystem:SetFriendInfo( index, friendInfo, is_online, type, tm )
	local name = friendInfo:GetName()
	local guid	= friendInfo:GetGuid()
	local level = friendInfo:GetLevel()
	local faction = friendInfo:GetFaction()
	local gender = friendInfo:GetGender()
	local onlinetime = friendInfo:GetOnlineTime()
	local force	= friendInfo:GetForce()

	local k = index * MAX_FRIEND_INT_MEMBER;
	self:SetUInt32(k + FRIEND_INT_LEVEL, level);	--设置等级
	self:SetByte(k + FRIEND_INT_ATTRIBUTE, 1, is_online)--设置在线
	self:SetByte(k + FRIEND_INT_ATTRIBUTE, 2, faction)	--设置阵营
	self:SetByte(k + FRIEND_INT_ATTRIBUTE, 3, gender)	--设置性别
	self:SetUInt32(k + FRIEND_INT_FLAG, setBit(0, type))
	self:SetUInt32(k + FRIEND_INT_ONLINE_TM, onlinetime)	--设置在线时长
	self:SetDouble(k + FRIEND_INT_FORCE, force)			--设置战斗力
	self:SetUInt32(k + FRIEND_INT_LAST_CHAT_TM, tm)		--设置最近聊天时间
	self:SetUInt32(k + FRIEND_INT_ADD_TM,os.time())
	k = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + index * MAX_FRIEND_STRING_MEMBER;
	self:SetStr(k + FRIEND_STRING_GUID, guid)
	self:SetStr(k + FRIEND_STRING_NAME, name)
end

-- 设置好友关系类型
function SocialSystem:SetFriendType(j, type, tm, playerInfo, friendInfo)
	if (type == FRIEND_PLAYER_TYPE_FRIENDLIST)then
		--好友和黑名单不能共存
		if(self:GetFlags(j, FRIEND_PLAYER_TYPE_BLACKLIST))then
			self:UnSetFlags(j, FRIEND_PLAYER_TYPE_BLACKLIST)
		end
		self:SetUInt32(j + FRIEND_INT_ADD_TM,os.time())
	elseif(type == FRIEND_PLAYER_TYPE_ENEMYLIST)then
	
	elseif(type == FRIEND_PLAYER_TYPE_RECENTLIST)then
		if(tm ~= 0 and self:GetUInt32(j * MAX_FRIEND_INT_MEMBER + FRIEND_INT_LAST_CHAT_TM) ~= tm)then
			--更新最近聊天时间
			self:SetUInt32(j * MAX_FRIEND_INT_MEMBER + FRIEND_INT_LAST_CHAT_TM, tm); 
		end
	
	elseif(type == FRIEND_PLAYER_TYPE_BLACKLIST)then
		if(self:GetFlags(j, FRIEND_PLAYER_TYPE_FRIENDLIST))then
			self:UnSetFlags(j, FRIEND_PLAYER_TYPE_FRIENDLIST)
			--判断是否在托管操作中
			--申请方删除
			if(playerInfo:GetPlayerHostingGuid() == friendInfo:GetGuid())then
				if(playerInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY))then
					--托管申请重置
					self:ResetHostingOpt(playerInfo,RESET_HOSTING_APPLY)
				else
					--托管重置
					self:ResetHostingOpt(playerInfo,RESET_HOSTING_HOSTING)
					--好友托管重置
					self:ResetFriendHosting(friendInfo:GetGuid())
				end
			end
			--管理方删除
			if(self:GetBit(j * MAX_FRIEND_INT_MEMBER + FRIEND_INT_FLAG,FRIEND_PLAYER_TYPE_HOSTING_FLAG))then
				self:UnSetBit(j * MAX_FRIEND_INT_MEMBER + FRIEND_INT_FLAG,FRIEND_PLAYER_TYPE_HOSTING_FLAG)
				self:SetUInt32(j * MAX_FRIEND_INT_MEMBER + FRIEND_INT_HOSTING_END_TM,0)
				--托管重置
				local friend_socialSysInfo = friendInfo:getSocialSystem()
				if friend_socialSysInfo then
					friend_socialSysInfo:ResetHostingOpt(friendInfo,RESET_HOSTING_HOSTING)
				end
			end
		end
	end

	if(self:GetFlags(j, type) == false)then
		self:SetFlags(j, type)
	end
end

--在好友列表中查找好友位置，返回位置索引（-1则表示没找到）
function SocialSystem:GetFriendGuidIndex(guid)
	--查找玩家是否已在社交系统中
	for j = 0, MAX_SOCIAL_SYSTEM_COUNT-1, 1 do
		if(self:GetSocialPlayerGuid(j) == '')then
			--没找到
			return false,j
		elseif(self:GetSocialPlayerGuid(j) == guid)then
			return true, j
		end
	end
	return false, -1
end

--添加某人到社交系统中
function SocialSystem:AddPlayerInfo(playerInfo, type, friendInfo, is_online, tm)
	if(type >= MAX_FRIEND_PLAYER_TYPE)then
		return -1
	end
	--查找玩家是否已在社交系统中
	local b, index = self:GetFriendGuidIndex(friendInfo:GetGuid())
	if not b then
		if index ~= -1 then
			self:SetFriendInfo(index, friendInfo, is_online, type, tm)
			if(type == FRIEND_PLAYER_TYPE_FRIENDLIST)then
				--通知添加成功
				if is_online == PLAYER_ONLINE_STATE_ONLINE then
					local str = string.format("%s %s %d", playerInfo:GetName(), playerInfo:GetGuid(), playerInfo:GetLevel())
					friendInfo:CallOptResult(OPRATE_TYPE_FRIEND, FRIEND_OPT_ADDFRIENDSUCCESS, str)
				end
			end
		end
	else
		self:SetFriendType(index, type, tm, playerInfo, friendInfo)
	end
	
	return index
end

--删除社交系统中某个玩家信息
function SocialSystem:DelSocialSystemPlayerInfo (playerInfo, type, player_id)
	if(type >= MAX_FRIEND_PLAYER_TYPE)then
		outDebug("error: type >= MAX_FRIEND_PLAYER_TYPE")
		return
	end

	--查找玩家是否已在社交系统中
	local b, index = self:GetFriendGuidIndex(player_id)
	if not b then
		return
	end
	
	self:UnSetFlags(index, type)
	--如果是好友，还要判断下托管情况
	if(type == FRIEND_PLAYER_TYPE_FRIENDLIST)then
		--申请方删除
		if(playerInfo:GetPlayerHostingGuid() == player_id)then
			if(playerInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY))then
				--托管申请重置
				self:ResetHostingOpt(playerInfo,RESET_HOSTING_APPLY)
			else
				--托管重置
				self:ResetHostingOpt(playerInfo,RESET_HOSTING_HOSTING)
				--好友托管重置
				self:ResetFriendHosting(player_id)
			end
		end
		--管理方删除
		if(self:GetBit(index * MAX_FRIEND_INT_MEMBER + FRIEND_INT_FLAG,FRIEND_PLAYER_TYPE_HOSTING_FLAG))then
			self:UnSetBit(index * MAX_FRIEND_INT_MEMBER + FRIEND_INT_FLAG,FRIEND_PLAYER_TYPE_HOSTING_FLAG)
			self:SetUInt32(index * MAX_FRIEND_INT_MEMBER + FRIEND_INT_HOSTING_END_TM,0)
			--好友在线的话，处理下好友
			local friendInfo = app.objMgr:getObj(player_id)
			if friendInfo then
				--托管重置
				local friend_socialSysInfo = friendInfo:getSocialSystem()
				if friend_socialSysInfo then
					friend_socialSysInfo:ResetHostingOpt(friendInfo,RESET_HOSTING_HOSTING)
				end
			end
		end
	end
	
	if(self:GetUInt32(index * MAX_FRIEND_INT_MEMBER + FRIEND_INT_FLAG) ~= 0)then
		--说明该玩家既在好友列表又在仇恨列表或最近联系人所以只需清空bit就好了
		if (type == FRIEND_PLAYER_TYPE_ENEMYLIST)then
			--清空击杀该玩家的次数,和被该玩家击杀的数目
			self:SetUInt32(index * MAX_FRIEND_INT_MEMBER + FRIEND_INT_KILL, 0)
		end	
		return
	end
	
	--填补删除的空位（将最后一个好友数据移至删除的空位，如果有的话）
	self:PreMovePlayerInf(index)
end

--填补删除的空位
function SocialSystem:PreMovePlayerInf (index)
	--删除的是最后一个(即下一个的GUID为空)则直接清除
	if(self:GetStr(PLAYER_SOCIAL_SYSTEM_STRING_FIELD + (index + 1) * MAX_FRIEND_STRING_MEMBER) == '')then
		local _index = index*MAX_FRIEND_INT_MEMBER
		for t = 0, MAX_FRIEND_INT_MEMBER - 1 do
			--清除数据
			self:SetUInt32(_index + t, 0)
		end
		--string数据
		_index = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + index * MAX_FRIEND_STRING_MEMBER
		for t = 0, MAX_FRIEND_STRING_MEMBER-1 do
			--清除数据
			self:SetStr(_index + t, "")
		end	
		return
	end
	--拿最后一个不为空的玩家数据来填补
	for i = index, MAX_SOCIAL_SYSTEM_COUNT-1 do
		local k = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + (i + 1)*MAX_FRIEND_STRING_MEMBER
		--第i+1个玩家数据为空则说明i就是要拿的玩家数据啦
		if(self:GetStr(k + FRIEND_STRING_GUID) == '')then
			--int数据
			local j = i * MAX_FRIEND_INT_MEMBER
			for t = 0, MAX_FRIEND_INT_MEMBER - 1 do
				--取数据
				self:SetUInt32(index*MAX_FRIEND_INT_MEMBER + t, self:GetUInt32(j + t))
				--清除数据
				self:SetUInt32(j + t, 0)
			end
			--string数据
			j = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + i * MAX_FRIEND_STRING_MEMBER
			for t = 0, MAX_FRIEND_STRING_MEMBER-1 do
				--取数据
				self:SetStr(PLAYER_SOCIAL_SYSTEM_STRING_FIELD + index*MAX_FRIEND_STRING_MEMBER + t, self:GetStr(j + t))
				--清除数据
				self:SetStr(j + t, "")
			end	
			break
		end
	end
end

--在好友列表中查找好友位置，返回位置索引（-1则表示没找到）
function SocialSystem:FindFriendByGuid(type, guid)
	if(type >= MAX_FRIEND_PLAYER_TYPE)then
		return -1
	end
	
	--查找玩家是否已在社交系统中
	local index = -1;
	for j = 0, MAX_SOCIAL_SYSTEM_COUNT-1, 1 do
		if(self:GetSocialPlayerGuid(j) == '')then
			--没找到
			return -1
		elseif(self:GetSocialPlayerGuid(j) == guid)then
			if(self:GetFlags(j, type))then
				 return j
			end
		end
	end
	return index;
end

--设置flag
function SocialSystem:SetFlags(index, off)
	self:SetBit(index*MAX_FRIEND_INT_MEMBER + FRIEND_INT_FLAG, off)
end

--获得flag
function SocialSystem:GetFlags(index, off)
	return self:GetBit(index*MAX_FRIEND_INT_MEMBER + FRIEND_INT_FLAG, off)
end

--Un设置flag
function SocialSystem:UnSetFlags (index, off)
	self:UnSetBit(index*MAX_FRIEND_INT_MEMBER + FRIEND_INT_FLAG, off)
end

--获得指定位置玩家guid
function SocialSystem:GetSocialPlayerGuid (index)
	return self:GetStr(PLAYER_SOCIAL_SYSTEM_STRING_FIELD + index * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID)
end

--set指定位置玩家guid
function SocialSystem:SetSocialPlayerGuid (index, val)
	self:SetStr(PLAYER_SOCIAL_SYSTEM_STRING_FIELD + index * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID, val)
end

--获得某个位置上好友亲密度
function SocialSystem:GetQinmiByPos (pos)
	local index2 = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + pos * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID;
	if(self:GetStr(index2) ~= '' and self:GetFlags(pos, FRIEND_PLAYER_TYPE_FRIENDLIST))then
		--在好友列表
		return self:GetUInt32(pos * MAX_FRIEND_INT_MEMBER + FRIEND_INT_QINMI)		
	end
	return 0;
end

--set某个位置上好友亲密度
function SocialSystem:SetQinmiByPos(pos, qinmi)
	local index2 = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + pos * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID;
	if(self:GetStr(index2) ~= '' and self:GetFlags(pos, FRIEND_PLAYER_TYPE_FRIENDLIST))then
		--在好友列表
		self:SetUInt32(pos * MAX_FRIEND_INT_MEMBER + FRIEND_INT_QINMI, qinmi);
	end
end

--add某个位置上好友亲密度
function SocialSystem:AddQinmiByPos( pos, qinmi)
	local index2 = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + pos * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID;
	if(self:GetStr(index2) ~= '' and self:GetFlags(pos, FRIEND_PLAYER_TYPE_FRIENDLIST))then
		--在好友列表
		self:AddUInt32(pos * MAX_FRIEND_INT_MEMBER + FRIEND_INT_QINMI, qinmi);
	end
end

--获得某个位置上成为好友时间
function SocialSystem:GetAddFriendTMByPos( pos)
	local index2 = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + pos * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID;
	if(self:GetStr(index2) ~= '')then
		--在好友列表
		return self:GetUInt32(pos * MAX_FRIEND_INT_MEMBER + FRIEND_INT_ADD_TM)		
	end
	return 0;
end

--set某个位置上成为好友时间
function SocialSystem:SetAddFriendTMByPos(pos, val)
	local index2 = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + pos * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID;
	if(self:GetStr(index2) ~= '')then
		--在好友列表
		self:SetUInt32(pos * MAX_FRIEND_INT_MEMBER + FRIEND_INT_ADD_TM, val);
	end
end

--获得某个位置上托管结束时间
function SocialSystem:GetHostingTMPos(pos)
	local index2 = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + pos * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID;
	if(self:GetStr(index2) ~= '')then
		--在好友列表
		return self:GetUInt32(pos * MAX_FRIEND_INT_MEMBER + FRIEND_INT_HOSTING_END_TM)		
	end
	return 0;
end

--set某个位置上托管结束时间
function SocialSystem:SetHostingTMPos(pos, val)
	local index2 = PLAYER_SOCIAL_SYSTEM_STRING_FIELD + pos * MAX_FRIEND_STRING_MEMBER + FRIEND_STRING_GUID;
	if(self:GetStr(index2) ~= '')then
		--在好友列表
		self:SetUInt32(pos * MAX_FRIEND_INT_MEMBER + FRIEND_INT_HOSTING_END_TM, val);
	end
end

-- 校验玩家信息
function SocialSystem:CheckFriendInfo(index, playerInfo)
	local guid = self:GetSocialPlayerGuid(index)
	if guid == "" then
		return false
	end
	local k = index * MAX_FRIEND_INT_MEMBER
	local friendInfo = app.objMgr:getObj(guid)
	if friendInfo then
		local level = friendInfo:GetLevel()
		local online = PLAYER_ONLINE_STATE_OUTLINE
		if(friendInfo:GetBit(PLAYER_APPD_INT_FIELD_FLAG,PLAYER_APPD_INT_FIELD_FLAGS_YEYOU_ONLINE))then
			online = PLAYER_ONLINE_STATE_ONLINE
		end
		local faction = friendInfo:GetFaction()    --获取阵营
		local sex = friendInfo:GetGender()
		local is_kill = self:GetUInt16(k + FRIEND_INT_KILL,1)
		if(self:GetUInt32(k + FRIEND_INT_LEVEL) ~= level)then
			self:SetUInt32(k + FRIEND_INT_LEVEL, level)
		end
		if(self:GetByte(k + FRIEND_INT_ATTRIBUTE, 1) ~= online)then
			self:SetByte(k + FRIEND_INT_ATTRIBUTE, 1, online)
			if(is_kill ~= 0)then
				playerInfo:CallOptResult(OPRATE_TYPE_FRIEND, FRIEND_OPT_KILLER_IS_ONLINE, friendInfo:GetName())
			end
		end
		if(self:GetByte(k + FRIEND_INT_ATTRIBUTE, 2) ~= faction)then
			self:SetByte(k + FRIEND_INT_ATTRIBUTE, 2, faction)
		end
		if(self:GetByte(k + FRIEND_INT_ATTRIBUTE, 3) ~= sex)then
			self:SetByte(k + FRIEND_INT_ATTRIBUTE, 3, sex)
		end
		local onlinetime = friendInfo:GetOnlineTime()
		if(self:GetUInt32(k + FRIEND_INT_ONLINE_TM) ~= onlinetime)then
			self:SetUInt32(k + FRIEND_INT_ONLINE_TM, onlinetime)
		end
		local force = friendInfo:GetForce()
		if(self:GetDouble(k + FRIEND_INT_FORCE) ~= force)then
			self:SetDouble(k + FRIEND_INT_FORCE, force)
		end
		
		--托管相关
		--托管者取消托管时，被托管玩家不在线
		local hosting_guid = friendInfo:GetPlayerHostingGuid()
		if(hosting_guid == nil or hosting_guid == "")then
			if(self:GetUInt32(k + FRIEND_INT_HOSTING_END_TM) > 0)then
				self:SetUInt32(k + FRIEND_INT_HOSTING_END_TM, 0)
			end
			if(self:GetBit(k + FRIEND_INT_FLAG,FRIEND_PLAYER_TYPE_HOSTING_FLAG))then
				self:UnSetBit(k + FRIEND_INT_FLAG,FRIEND_PLAYER_TYPE_HOSTING_FLAG)
			end
		else	--被托管者取消时，托管者不在线
			if(self:GetUInt32(k + FRIEND_INT_HOSTING_END_TM) == 0 and friendInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY) == false
				and friendInfo:GetPlayerHostingGuid() == playerInfo:GetGuid())then
				--托管申请重置
				local friend_socialSysInfo = friendInfo:getSocialSystem()
				if friend_socialSysInfo then
					friend_socialSysInfo:ResetHostingOpt(friendInfo,RESET_HOSTING_APPLY)
				end
			end
		end
		
		--我托管给的这个玩家
		if(self:GetBit(k + FRIEND_INT_FLAG, FRIEND_PLAYER_TYPE_HOSTING_THIS))then
			--判断对方是否还有加我的好友
			local socialSysInfo = friendInfo:getSocialSystem()
			if(socialSysInfo ~= nil)then
				local index = socialSysInfo:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,playerInfo:GetGuid())
				if(index == -1)then
					--已经不是好友了，清理下托管关系
					if(playerInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY))then
						self:ResetHostingOpt(playerInfo,RESET_HOSTING_APPLY)		--如果是托管申请的话，重置申请
					else
						self:ResetHostingOpt(playerInfo,RESET_HOSTING_HOSTING)	--重置托管
					end
				end					
			end
		end
	else
		--申请中，对方下线处理
		if(playerInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY) and playerInfo:GetPlayerHostingGuid() == guid)then
			--托管申请重置
			self:ResetHostingOpt(playerInfo,RESET_HOSTING_APPLY)
		end
		if(self:GetByte(k + FRIEND_INT_ATTRIBUTE, 1) ~= PLAYER_ONLINE_STATE_OUTLINE)then
			self:SetByte(k + FRIEND_INT_ATTRIBUTE, 1, PLAYER_ONLINE_STATE_OUTLINE)
		end
	end
	return true
end

--心跳更新社交系统玩家信息
function SocialSystem:UpdateFirendInfo(playerInfo)
	for j = 0, MAX_SOCIAL_SYSTEM_COUNT-1, 1 do
		-- 校验玩家信息
		if(not self:CheckFriendInfo(j, playerInfo))then
			break
		end
	end
	
	--获取托管玩家的GUID
	local hosting_guid = playerInfo:GetPlayerHostingGuid()
	if(hosting_guid ~= "" and hosting_guid ~= nil)then
		--托管人员不在好友中了
		if(not self:GetFriendGuidIndex(hosting_guid))then
			playerInfo:UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY)
			playerInfo:SetPlayerHostingApplyTM(0)
			playerInfo:SetPlayerHostingGuid("")
			playerInfo:SetPlayerHostingName("")
			playerInfo:SetPlayerHostingTime(0)
		end
	end
	
end

-- 场景服通知应用服玩家升级了
function SocialSystem:ProceedUpgrade( playerInfo, upgrade_type, level)
	-- 在这边处理下玩家潜力点
	local qianlidian = tb_char_level[level].potential + playerInfo:GetAddQianLiValue()	--目前最大潜力点等于玩家等级
	for i=0, MAX_QIANLI_TYPE-1 do
		local value = playerInfo:GetQianLiTypeValue(i)
		qianlidian = qianlidian - value
	end

	if qianlidian > 0 and qianlidian > playerInfo:GetQianLiValue() then
		playerInfo:SetQianLiValue(qianlidian)
	end
	
	if level%5 ~= 0 then
		return
	end
	--查找玩家是否已在社交系统中
	for j = 0, MAX_SOCIAL_SYSTEM_COUNT-1, 1 do
		local guid = self:GetSocialPlayerGuid(j)
		if(guid == '')then
			--没找到
			return
		end

		local friendInfo = app.objMgr:getObj(guid)
		if friendInfo and self:GetFlags(j, FRIEND_PLAYER_TYPE_FRIENDLIST) then
			if(friendInfo:GetLevel() < 45)then   --玩家升阶，不通知30级以上的好友
				local str = string.format('%s %s %d', playerInfo:GetGuid(), playerInfo:GetName(), level)
				if upgrade_type == FRIEND_OPT_PLAYER_UPGRADE then 
					local socialSysInfo = friendInfo:getSocialSystem()
					if(socialSysInfo ~= nil)then
						local index = socialSysInfo:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST, playerInfo:GetGuid())
						if index ~= -1 and socialSysInfo:GetFlags(index, FRIEND_PLAYER_TYPE_ZHUFU_FLAG) == false then
							friendInfo:CallOptResult(OPRATE_TYPE_FRIEND, upgrade_type, str)
						end
					end
				else
					friendInfo:CallOptResult(OPRATE_TYPE_FRIEND, upgrade_type, str)
				end
			end
		end
	end
end

--托管申请
function SocialSystem:FriendHostingApply (player, player_guid)
	--自己无法托管自己
	if(player:GetGuid() == player_guid)then
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_IS_SELF)		
	end
	--判断下等级
	if(player:GetLevel() < 50)then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_LEVEL_NOT)
	end
	--判断是否已经处于托管中
	if(player:GetPlayerHostingTime() > os.time())then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_IS_HOSTING)
	end
	--判断是否在托管申请中
	if(player:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY))then
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_IS_HOSTING_APPLY)
	end
	
	--判断是否是好友
	local index = self:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,player_guid)
	if(index == -1)then
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_NOT)
	end
	--判断成为好友是否已经超过24小时了
	if(self:GetAddFriendTMByPos(index) + 86400 > os.time())then
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_HAS_NOT_24H)
	end
	--判断对方是否在线
	local friendInfo = app.objMgr:getObj(player_guid)
	if friendInfo == nil then
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_OUTLINE)
	end	
	--将玩家的好友列表托管设置处理下
	local socialSysInfo = friendInfo:getSocialSystem()
	if(socialSysInfo ~= nil)then
		--判断是否是好友
		local friend_index = socialSysInfo:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,player:GetGuid())
		if(friend_index == -1)then			
			return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_NOT_HAS_YOU)
		end
		--判断成为好友是否已经超过24小时了
		if(socialSysInfo:GetAddFriendTMByPos(friend_index) + 86400 > os.time())then
			return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_HAS_NOT_24H)
		end
	end		
	
	--判断对方是否托管登录
	if(friendInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_LOGIN))then
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_HOSTING_LOGIN)
	end
	--给对方发起申请	
	player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_APPLY, string.format("%s %s",player:GetGuid(),player:GetName()))
	--设置下托管申请中标志
	player:SetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY)
	--设置下托管对象
	player:SetPlayerHostingGuid(player_guid)
	player:SetPlayerHostingName(friendInfo:GetName())
	player:SetPlayerHostingApplyTM(os.time() + 60)	--托管申请结束时间
	self:SetFlags(index,FRIEND_PLAYER_TYPE_HOSTING_THIS)
end

--托管同意
function SocialSystem:FriendHostingAgree(player, player_guid)
	--自己无法托管自己
	if(player:GetGuid() == player_guid)then
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_IS_SELF)
	end
	
	--判断对方是否在线
	local friendInfo = app.objMgr:getObj(player_guid)
	if not friendInfo then
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_OUTLINE)
	end
	
	--判断对方是否托管申请中并且托管人是自己	
	if(friendInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY) == false or friendInfo:GetPlayerHostingGuid() ~= player:GetGuid())then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_ERR)
	end
	
	--判断是否是好友
	local index = self:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,player_guid)
	if(index == -1)then
		player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_NOT)
		--好友取消掉了,视为拒绝
		--托管申请重置
		self:ResetHostingOpt(player,RESET_HOSTING_APPLY)
		return
	else
		local socialSysInfo = friendInfo:getSocialSystem()
		if socialSysInfo then
			local friend_index = socialSysInfo:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,player:GetGuid())
			--对方把好友删了
			if(friend_index == -1)then
				player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_NOT)
				--好友取消掉了,视为拒绝
				--托管申请重置
				self:ResetHostingOpt(player,RESET_HOSTING_APPLY)
				return
			end					
		end
	end
	local now_time = os.time()
	--设置下托管结束时间
	friendInfo:SetPlayerHostingTime(now_time + 86400 * 3)
	--friendInfo:SetPlayerHostingTime(now_time + 600)
	
	--重新设置下托管申请
	friendInfo:UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY)
	friendInfo:SetPlayerHostingApplyTM(0)
	--在自己的好友里面设置下
	self:SetHostingTMPos(index,now_time + 86400 * 3)
	--self:SetHostingTMPos(index,now_time + 600)
	self:SetFlags(index,FRIEND_PLAYER_TYPE_HOSTING_FLAG)
	friendInfo:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_SUCCESS, player:GetName())
	player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_SUCCESS)
end

--托管拒绝
function SocialSystem:FriendHostingRefuse(player, player_guid)
	--自己无法托管自己
	if(player:GetGuid() == player_guid)then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_IS_SELF)
	end
	
	--判断是否是好友
	local index = self:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,player_guid)
	if(index == -1)then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_NOT)
	end
	
	--判断对方是否在线
	local friendInfo = app.objMgr:getObj(player_guid)
	if not friendInfo then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_OUTLINE)
	end
	--判断对方是否托管申请中并且托管人是自己			
	if(friendInfo:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY) == false or friendInfo:GetPlayerHostingGuid() ~= player:GetGuid())then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_ERR)
	end
	
	--托管申请重置
	local friend_socialSysInfo = friendInfo:getSocialSystem()
	if friend_socialSysInfo then
		friend_socialSysInfo:ResetHostingOpt(friendInfo,RESET_HOSTING_APPLY)
	end
	friendInfo:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_REFUSED, player:GetName())
end

--托管取消
function SocialSystem:FriendHostingCancel(player, player_guid)
	--自己取消的
	if(player:GetGuid() == player_guid)then
		--判断是否已经处于托管中
		if(player:GetPlayerHostingTime() < os.time())then			
			return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_NOT_HOSTING)
		end

		--托管重置
		local friend_guid = player:GetPlayerHostingGuid()
		self:ResetHostingOpt(player,RESET_HOSTING_HOSTING)
		
		--重设好友下标中托管字段
		--看下好友是否在线
		local friendInfo = app.objMgr:getObj(friend_guid)
		if friendInfo then
			local friend_socialSysInfo = friendInfo:getSocialSystem()
			if friend_socialSysInfo then
				friend_socialSysInfo:ResetFriendHosting(player:GetGuid())
			end
		end
	else		--对方取消托管的	
	
		--判断是否是好友
		local index = self:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,player_guid)
		if(index ~= -1)then
			--清空下
			self:SetHostingTMPos(index,0)
			self:UnSetFlags(index,FRIEND_PLAYER_TYPE_HOSTING_FLAG)
		end
		
		--看下好友是否在线
		local friendInfo = app.objMgr:getObj(player_guid)
		if friendInfo then
			--托管重置
			local friend_socialSysInfo = friendInfo:getSocialSystem()
			if friend_socialSysInfo then
				friend_socialSysInfo:ResetHostingOpt(friendInfo ,RESET_HOSTING_HOSTING)
			end
			friendInfo:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_CANCEL, player:GetName())
		end
	end
end

--托管登录
function SocialSystem:FriendHostingLogin(player, player_guid)
	--自己无法登录自己
	if(player:GetGuid() == player_guid)then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_IS_SELF)
	end
	
	--判断是否是好友
	local index = self:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,player_guid)
	if(index == -1)then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_NOT)
	end
	
	--判断是否在托管中
	if(self:GetHostingTMPos(index) < os.time())then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_NOT_HOSTING)
	end
	
	--判断对方是否在线				
	local friendInfo = app.objMgr:getObj(player_guid)
	if friendInfo then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_FRIEND_ONLINE)
	end
	
	--通知登录服给KEY
	playerLib.SendLoginHosting(player.ptr,player_guid)
end

--托管到期
function SocialSystem:FriendHostingDueTo(player, player_guid)
	--判断下托管时间是不是已经到期了
	--判断是否在托管中
	if(player:GetPlayerHostingTime() > os.time())then		
		return player:CallOptResult(OPERTE_TYPE_HOSTING, HOSTING_OPERTE_NOT_DUE_TO)
	end
	--清理玩家身上下标
	local friend_guid = player:GetPlayerHostingGuid()
	--托管重置
	self:ResetHostingOpt(player,RESET_HOSTING_HOSTING)
	--重设好友下标中托管字段
	--看下好友是否在线
	local friendInfo = app.objMgr:getObj(friend_guid)
	if friendInfo then
		local friend_socialSysInfo = friendInfo:getSocialSystem()
		if friend_socialSysInfo then
			friend_socialSysInfo:ResetFriendHosting(player:GetGuid())
		end
	end
	--判断下是不是托管登录中
	if(player:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_LOGIN))then
		--把人踢了
		player:Close(PLAYER_CLOSE_OPERTE_APPD_ONE6, "")
	end
end


--托管操作重置
function SocialSystem:ResetHostingOpt(player,hosting_type)
	local friend_guid = player:GetPlayerHostingGuid()
	if(player:GetPlayerHostingGuid() ~= "")then
		player:SetPlayerHostingGuid("")
	end
	if(player:GetPlayerHostingName() ~= "")then
		player:SetPlayerHostingName("")
	end
	if(hosting_type == RESET_HOSTING_APPLY and player:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY))then
		player:UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_APPLY)
		player:SetPlayerHostingApplyTM(0)
	elseif(hosting_type == RESET_HOSTING_HOSTING and player:GetPlayerHostingTime() ~= 0)then
		player:SetPlayerHostingTime(0)
	end
	
	local index = self:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,friend_guid)
	if(index ~= -1)then
		--清空下
		if(self:GetFlags(index,FRIEND_PLAYER_TYPE_HOSTING_THIS))then
			self:UnSetFlags(index,FRIEND_PLAYER_TYPE_HOSTING_THIS)
		end
	end

end

--重设好友下标中托管字段
function SocialSystem:ResetFriendHosting(guid)
	--判断好友是否在线
	local index = self:FindFriendByGuid(FRIEND_PLAYER_TYPE_FRIENDLIST,guid)
	if(index ~= -1)then
		--清空下
		if(self:GetHostingTMPos(index) ~= 0)then
			self:SetHostingTMPos(index,0)
		end
		if(self:GetFlags(index,FRIEND_PLAYER_TYPE_HOSTING_FLAG))then
			self:UnSetFlags(index,FRIEND_PLAYER_TYPE_HOSTING_FLAG)
		end
	end
end

--击杀该玩家次数
function SocialSystem:AddKillPlayerCount(index, val)
	self:AddUInt16(index * MAX_FRIEND_INT_MEMBER + FRIEND_INT_KILL, 0, val)
end

--被该玩家击杀次数
function SocialSystem:AddKilledPlayerCount(index, val)
	self:AddUInt16(index * MAX_FRIEND_INT_MEMBER + FRIEND_INT_KILL, 1, val)
end

return SocialSystem