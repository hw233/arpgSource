--帮派管理
local FactionInfo = class('FactionInfo', BinLogObject)

local protocols = require('share.protocols')

local SPLIT_THINGS = "\1"	--分割多条物品
local SPLIT_THING_DETAIL = "\2"	--分割物品详细信息

--获取帮派标志位
function FactionInfo:GetFactionFlags(index)
	return self:GetBit(FACTION_INT_FIELD_FLAG,index)
end

--设置帮派标志位
function FactionInfo:SetFactionFlags(index)
	if not self:GetFactionFlags(index) then
		self:SetBit(FACTION_INT_FIELD_FLAG,index)
	end
end

--取消帮派标志位
function FactionInfo:UnSetFactionFlags(index)
	if self:GetFactionFlags(index) then
		self:UnSetBit(FACTION_INT_FIELD_FLAG,index)
	end
end

--获取帮派成员人数
function FactionInfo:GetMemberCount()
	return self:GetUInt32(FACTION_INT_FIELD_PLAYER_NOW_INDEX)
end

--设置帮派成员人数
function FactionInfo:SetMemberCount(val)
	self:SetUInt32(FACTION_INT_FIELD_PLAYER_NOW_INDEX, val)
end

--添加帮派成员人数
function FactionInfo:AddMemberCount(val)
	self:AddUInt32(FACTION_INT_FIELD_PLAYER_NOW_INDEX, val)
end

--减少帮派成员人数
function FactionInfo:SubMemberCount(val)
	self:SubUInt32(FACTION_INT_FIELD_PLAYER_NOW_INDEX, val)
end
--帮派最大成员人数
function FactionInfo:GetMemberMaxCount()
	local lev = self:GetFactionLevel()
	return tb_faction_base[lev].maxnum
end

--获取帮派申请人数
function FactionInfo:GetFactionApplyCount()
	return self:GetUInt32(FACTION_INT_FIELD_APPLY_PLAYER_COUNT_INDEX)
end

--设置帮派申请人数
function FactionInfo:SetFactionApplyCount(val)
	self:SetUInt32(FACTION_INT_FIELD_APPLY_PLAYER_COUNT_INDEX, val)
end

--添加帮派申请人数
function FactionInfo:AddFactionApplyCount(val)
	self:AddUInt32(FACTION_INT_FIELD_APPLY_PLAYER_COUNT_INDEX, val)
end

--减少帮派申请人数
function FactionInfo:SubFactionApplyCount(val)
	self:SubUInt32(FACTION_INT_FIELD_APPLY_PLAYER_COUNT_INDEX, val)
end
---------------------帮派事件相关---------------------
--获取单条事件类型
function FactionInfo:GetFactionEventType(pos)
	return self:GetUInt16(FACTION_INT_FIELD_EVENT+pos*MAX_FACTION_INT_EVENT+FACTION_INT_EVENT_TYPE_VALUE,0)
end
--设置单条事件类型
function FactionInfo:SetFactionEventType(pos,val)
	self:SetUInt16(FACTION_INT_FIELD_EVENT+pos*MAX_FACTION_INT_EVENT+FACTION_INT_EVENT_TYPE_VALUE,0,val)
end

--获取单条事件值
function FactionInfo:GetFactionEventValue(pos)
	return self:GetUInt16(FACTION_INT_FIELD_EVENT+pos*MAX_FACTION_INT_EVENT+FACTION_INT_EVENT_TYPE_VALUE,1)
end
--设置单条事件值
function FactionInfo:SetFactionEventValue(pos,val)
	self:SetUInt16(FACTION_INT_FIELD_EVENT+pos*MAX_FACTION_INT_EVENT+FACTION_INT_EVENT_TYPE_VALUE,1,val)
end

--获取单条事件名称
function FactionInfo:GetFactionEventName(pos)
	return self:GetStr(FACTION_STRING_FIELD_EVENT+pos)
end
--设置单条事件名称
function FactionInfo:SetFactionEventName(pos,val)
	self:SetStr(FACTION_STRING_FIELD_EVENT+pos,val)
end

--设置事件标记
function FactionInfo:SetFactionEventFlag(val)
	self:SetUInt32(FACTION_INT_FIELD_EVENT_FALG,val)
end
--获取事件标记
function FactionInfo:GetFactionEventFlag()
	return self:GetUInt32(FACTION_INT_FIELD_EVENT_FALG)
end



---------------------成员信息相关---------------------
--获取成员战斗力
function FactionInfo:GetFactionMemberForce(pos)
	return self:GetDouble(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_FORCE)
end

--设置成员战斗力
function FactionInfo:SetFactionMemberForce(pos,val)
	local old_val = self:GetFactionMemberForce(pos)
	self:SubDouble(FACTION_INT_FIELD_TOTAL_FORCE,old_val)
	self:AddDouble(FACTION_INT_FIELD_TOTAL_FORCE,val)
	rankInsertTask(self:GetGuid(),RANK_TYPE_FACTION)
	
	outFmtDebug("faction total force %d",self:GetDouble(FACTION_INT_FIELD_TOTAL_FORCE))
	self:SetDouble(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_FORCE,val)
end

--获取成员等级
function FactionInfo:GetFactionMemberLevel(pos)
	return self:GetUInt16(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 0)
end

--设置成员等级
function FactionInfo:SetFactionMemberLevel(pos,val)
	self:SetUInt16(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 0, val)
end

--获取成员今日贡献
function FactionInfo:GetFactionMemberDayGongXian(pos)
	return self:GetUInt16(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 1)
end

--设置成员今日贡献
function FactionInfo:SetFactionMemberDayGongXian(pos,val)
	self:SetUInt16(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 1, val)
end

--增加成员今日贡献
function FactionInfo:AddFactionMemberDayGongXian(pos,val)
	self:AddUInt16(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 1, val)
end

--获取成员今日金币捐献次数
function FactionInfo:GetFactionMemberGoldDonation(pos)
	return self:GetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 2)
end

--设置成员今日金币捐献次数
function FactionInfo:SetFactionMemberGoldDonation(pos,val)
	self:SetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 2, val)
end

--增加成员今日金币捐献次数
function FactionInfo:AddFactionMemberGoldDonation(pos,val)
	self:AddByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 2, val)
end

--获取成员今日元宝捐献次数
function FactionInfo:GetFactionMemberYbDonation(pos)
	return self:GetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 3)
end

--设置成员今日元宝捐献次数
function FactionInfo:SetFactionMemberYbDonation(pos,val)
	self:SetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 3, val)
end

--增加成员今日元宝捐献次数
function FactionInfo:AddFactionMemberYbDonation(pos,val)
	self:AddByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 3, val)
end

--获取成员标志位
function FactionInfo:GetFactionMemberFlags(pos,val)
	return self:GetBit(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_FLAGS, val)
end

--设置成员标志位
function FactionInfo:SetFactionMemberFlags(pos,val)
	self:SetBit(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_FLAGS, val)
end

--重设成员标志位
function FactionInfo:UnSetFactionMemberFlags(pos,val)
	self:UnSetBit(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_FLAGS, val)
end

--获取成员是否在线
function FactionInfo:GetFactionMemberIsOnline(pos)
	return self:GetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 0)
end

--设置成员是否在线
function FactionInfo:SetFactionMemberIsOnline(pos,val)
	self:SetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 0, val)
end

--获取成员身份
function FactionInfo:GetFactionMemberIdentity(pos)
	return self:GetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 1)
end

--设置成员身份
function FactionInfo:SetFactionMemberIdentity(pos,val)
	self:SetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 1, val)
end

--获取VipLevel
function FactionInfo:GetFactionMemberVipLevel(pos)
	return self:GetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 2)
end

--设置VipLevel
function FactionInfo:SetFactionMemberVipLevel(pos,val)
	self:SetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 2, val)
end

--获取今日活跃度
function FactionInfo:GetFactionMemberActive(pos)
	return self:GetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 3)
end

--设置今日活跃度
function FactionInfo:SetFactionMemberActive(pos,val)
	self:SetByte(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 3, val)
end

--获取成员在线时长
function FactionInfo:GetFactionMemberOnlineTime(pos)
	return self:GetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_ONLINE_TIME)
end

--设置成员在线时长
function FactionInfo:SetFactionMemberOnlineTime(pos,val)
	self:SetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_ONLINE_TIME,val)
end


--获取成员离线时间
function FactionInfo:GetFactionMemberLogoutTime(pos)
	return self:GetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_LOGOUT_TIME)
end

--设置成员离线时间
function FactionInfo:SetFactionMemberLogoutTime(pos,val)
	self:SetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_LOGOUT_TIME,val)
end

--获取成员历史贡献
function FactionInfo:GetFactionMemberTotalGongXian(pos)
	return self:GetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOTAL_CONTRIBUTION)
end

--设置成员历史贡献
function FactionInfo:SetFactionMemberTotalGongXian(pos,val)
	self:SetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOTAL_CONTRIBUTION,val)
end

--增加成员历史贡献
function FactionInfo:AddFactionMemberTotalGongXian(pos,val)
	self:AddUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOTAL_CONTRIBUTION,val)
end

--获取成员魅力
function FactionInfo:GetFactionMemberCharm(pos)
	return self:GetDouble(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_CHARM)
end

--设置成员魅力
function FactionInfo:SetFactionMemberCharm(pos,val)
	self:SetDouble(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_CHARM,val)
end


--获取成员总魅力贡献
function FactionInfo:GetFactionMemberTotalSendCharm(pos)
	return self:GetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOTAL_SEND_CHARM)
end

--设置成员总魅力贡献
function FactionInfo:SetFactionMemberTotalSendCharm(pos,val)
	self:SetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOTAL_SEND_CHARM,val)
end

--增加成员总魅力贡献
function FactionInfo:AddFactionMemberTotalSendCharm(pos,val)
	self:AddUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOTAL_SEND_CHARM,val)
end

--获取成员GUID
function FactionInfo:GetFactionMemberGuid(pos)
	return self:GetStr(FACTION_STRING_FIELD_PLAYER + pos*MAX_FACTION_STRING_MEMBER+FACTION_STRING_MEMBER_GUID)
end

--设置成员GUID
function FactionInfo:SetFactionMemberGuid(pos,val)
	self:SetStr(FACTION_STRING_FIELD_PLAYER + pos*MAX_FACTION_STRING_MEMBER+FACTION_STRING_MEMBER_GUID,val)
end

--获取成员名字
function FactionInfo:GetFactionMemberName(pos)
	return self:GetStr(FACTION_STRING_FIELD_PLAYER + pos*MAX_FACTION_STRING_MEMBER+FACTION_STRING_MEMBER_NAME)
end

--设置成员名字
function FactionInfo:SetFactionMemberName(pos,val)
	self:SetStr(FACTION_STRING_FIELD_PLAYER + pos*MAX_FACTION_STRING_MEMBER+FACTION_STRING_MEMBER_NAME,val)
end

--------------------------------------成员信息end-------------------------------

--获取帮派排名
function FactionInfo:GetFactionRank()
	return self:GetUInt32(FACTION_INT_FIELD_RANK)
end
--设置帮派排名
function FactionInfo:SetFactionRank(val)
	if self:GetFactionRank() ~= val then
		self:SetUInt32(FACTION_INT_FIELD_RANK,val)
	end
end

--获取帮派等级
function FactionInfo:GetFactionLevel()
	return self:GetUInt32(FACTION_INT_FIELD_LEVEL)
end

--设置帮派等级
function FactionInfo:SetFactionLevel(val)
	self:SetUInt32(FACTION_INT_FIELD_LEVEL,val)
end


--获取帮派活跃度
function FactionInfo:GetFactionActive()
	return self:GetUInt32(FACTION_INT_FIELD_ACTIVITY)
end

--设置帮派活跃度
function FactionInfo:SetFactionActive(val)
	self:SetUInt32(FACTION_INT_FIELD_ACTIVITY,val)
end

--获取帮派资产
function FactionInfo:GetFactionMoney()
	return self:GetDouble(FACTION_INT_FIELD_MONEY)
end

--设置帮派资产
function FactionInfo:SetFactionMoney(val)
	self:SetDouble(FACTION_INT_FIELD_MONEY,val)
end
--增加帮派资产
function FactionInfo:AddFactionMoney(val)
	self:AddDouble(FACTION_INT_FIELD_MONEY,val)
end


--获取帮派战斗力
function FactionInfo:GetFactionForce()
	return self:GetDouble(FACTION_INT_FIELD_FORCE)
end

--设置帮派战斗力
function FactionInfo:SetFactionForce(val)
	if self:GetFactionForce() ~= val then
		self:SetDouble(FACTION_INT_FIELD_FORCE,val)
	end
end

--获取帮派当前选择的旗帜ID
function FactionInfo:GetFactionCurFlagId()
	return self:GetByte(FACTION_INT_FIELD_FLAGS_ID,0)
end

--设置帮派当前选择的旗帜ID
function FactionInfo:SetFactionCurFlagId(val)
	self:SetByte(FACTION_INT_FIELD_FLAGS_ID,0,val)
end

--获取帮派神明等级
function FactionInfo:GetFactionGodLevel()
	return self:GetByte(FACTION_INT_FIELD_FLAGS_ID,1)
end

--设置帮派神明等级
function FactionInfo:SetFactionGodLevel(val)
	self:SetByte(FACTION_INT_FIELD_FLAGS_ID,1,val)
end

--获取帮派灵气
function FactionInfo:GetFactionLingQi()
	return self:GetUInt16(FACTION_INT_FIELD_FLAGS_ID,1)
end

--设置帮派灵气
function FactionInfo:SetFactionLingQi(val)
	self:SetUInt16(FACTION_INT_FIELD_FLAGS_ID,1,val)
end

--增加帮派灵气
function FactionInfo:AddFactionLingQi(val)
	self:AddUInt16(FACTION_INT_FIELD_FLAGS_ID,1,val)
end
--设置帮派最小等级
function FactionInfo:SetFactionMinLev(val)
	self:SetUInt16(FACTION_INT_FIELD_MINLEV,0,val)
end
--获取帮派最小等级
function FactionInfo:GetFactionMinLev(val)
	return self:GetUInt32(FACTION_INT_FIELD_MINLEV,0)
end

--设置帮派公告
function FactionInfo:SetFactionNotice(notice)
	self:SetStr(FACTION_STRING_FIELD_GONGGAO,notice)
end

--设置帮派招募公告
function FactionInfo:SetFactionZhaoMuNotice(notice)
	self:SetStr(FACTION_STRING_FIELD_ZHAOMU_GONGGAO,notice)
end

--设置帮派公告修改时间
function FactionInfo:SetFactionNoticeTime(val)
	self:SetUInt32(FACTION_STRING_INT_GONGGAO_TIME,val)
end


--------------------------------------红包-------------------------------
--元宝数
function FactionInfo:GetFactionHongBao(index)
	return self:GetUInt32(FACTION_INT_FIELD_HONGBAO+index*MAX_FACTION_INT_HONGBAO+FACTION_INT_HONGBAO_MONEY)
end
--设置元宝数
function FactionInfo:SetFactionHongBao(index,val)
	self:SetUInt32(FACTION_INT_FIELD_HONGBAO+index*MAX_FACTION_INT_HONGBAO+FACTION_INT_HONGBAO_MONEY,val)
end
--个数
function FactionInfo:GetFactionHongBaoCount(index)
	return self:GetUInt32(FACTION_INT_FIELD_HONGBAO+index*MAX_FACTION_INT_HONGBAO+FACTION_INT_HONGBAO_COUNT)
end
--设置个数
function FactionInfo:SetFactionHongBaoCount(index,val)
	self:SetUInt32(FACTION_INT_FIELD_HONGBAO+index*MAX_FACTION_INT_HONGBAO+FACTION_INT_HONGBAO_COUNT,val)
end
--时间
function FactionInfo:GetFactionHongBaoTime(index)
	return self:GetUInt32(FACTION_INT_FIELD_HONGBAO+index*MAX_FACTION_INT_HONGBAO+FACTION_INT_HONGBAO_TIME)
end
--设置时间
function FactionInfo:SetFactionHongBaoTime(index,val)
	self:SetUInt32(FACTION_INT_FIELD_HONGBAO+index*MAX_FACTION_INT_HONGBAO+FACTION_INT_HONGBAO_TIME,val)
end
--发红包的GUID
function FactionInfo:GetFactionHongBaoGuid(index)
	return self:GetStr(FACTION_STRING_FIELD_HONGBAO+index*MAX_FACTION_STRING_HONGBAO+FACTION_STRING_HONGBAO_GUID)
end
--设置发红包的GUID
function FactionInfo:SetFactionHongBaoGuid(index,str)
	self:SetStr(FACTION_STRING_FIELD_HONGBAO+index*MAX_FACTION_STRING_HONGBAO+FACTION_STRING_HONGBAO_GUID,str)
end
--发红包的名字
function FactionInfo:GetFactionHongBaoName(index)
	return self:GetStr(FACTION_STRING_FIELD_HONGBAO+index*MAX_FACTION_STRING_HONGBAO+FACTION_STRING_HONGBAO_NAME)
end
--设置发红包的名字
function FactionInfo:SetFactionHongBaoName(index,val)
	self:SetStr(FACTION_STRING_FIELD_HONGBAO+index*MAX_FACTION_STRING_HONGBAO+FACTION_STRING_HONGBAO_NAME,val)
end

--查找红包位置
function FactionInfo:FindFactionHongBaoPos(guid)
	local hongbao_table = {}
	for i = 0,MAX_FACTION_HONGBAO_COUNT - 1 do
		if self:GetFactionHongBaoGuid(i) == guid then
			table.insert(hongbao_table,{i,self:GetFactionHongBaoTime()})
		end
	end
	--没找到
	if #hongbao_table == 0 then
		return
	end
	--如果这个玩家发了>1个红包。优先选择最后面发的红包
	if #hongbao_table > 1 then
		table.sort(hongbao_table,function(a,b) return a[2] > b[2] end)
	end
	
	return hongbao_table[1][1]
end

--获取红包空位置
function FactionInfo:GetFactionHongBaoEmptyPos()
	for i = 0,MAX_FACTION_HONGBAO_COUNT - 1 do
		if self:GetFactionHongBaoCount(i) == 0 then
			return i
		end
	end
	return
end

--删除红包
function FactionInfo:DelFactionHongBao(index)
	self:SetFactionHongBao(index,0)
	self:SetFactionHongBaoCount(index,0)
	self:SetFactionHongBaoTime(index,0)
	self:SetFactionHongBaoGuid(index,"")
	self:SetFactionHongBaoName(index,"")
end

--添加一个红包
function FactionInfo:AddFactionHongBao(money,count,guid,name)
	local index = self:GetFactionHongBaoEmptyPos()
	if index then
		self:SetFactionHongBao(index,money)
		self:SetFactionHongBaoCount(index,count)
		self:SetFactionHongBaoTime(index,os.time())
		self:SetFactionHongBaoGuid(index,guid)
		self:SetFactionHongBaoName(index,name)
	end
end

--------------------------------------红包end-------------------------------


--------------------------------------申请加入帮派成员信息---------------------------------------------
--获取申请成员战斗力
function FactionInfo:GetFactionApplyForce(pos)
	return self:GetDouble(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_FORCE)
end

--设置申请成员战斗力
function FactionInfo:SetFactionApplyForce(pos,val)
	self:SetDouble(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_FORCE,val)
end

--获取申请成员等级
function FactionInfo:GetFactionApplyLevel(pos)
	return self:GetUInt16(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 0)
end

--设置申请成员等级
function FactionInfo:SetFactionApplyLevel(pos,val)
	self:SetUInt16(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_UINT16, 0, val)
end

--获取申请成员VipLevel
function FactionInfo:GetFactionApplyVipLevel(pos)
	return self:GetByte(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 2)
end

--设置申请成员VipLevel
function FactionInfo:SetFactionApplyVipLevel(pos,val)
	self:SetByte(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 2, val)
end

--设置成员是否在线
function FactionInfo:SetFactionApplyIsOnline(pos,val)
	self:SetByte(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 0, val)
end

--获取成员身份
function FactionInfo:GetFactionApplyIdentity(pos)
	return self:GetByte(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_BYTE, 1)
end

--获取申请成员在线时长
function FactionInfo:GetFactionApplyOnlineTime(pos)
	return self:GetUInt32(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_ONLINE_TIME)
end

--设置申请成员在线时长
function FactionInfo:SetFactionApplyOnlineTime(pos,val)
	self:SetUInt32(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_ONLINE_TIME,val)
end

--设置申请成员申请结束时间
function FactionInfo:GetFactionApplyEndTime(pos)
	return self:GetUInt32(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_XIAOHEIWU_END_TIME)
end

--获取申请成员申请结束时间
function FactionInfo:SetFactionApplyEndTime(pos,val)
	self:SetUInt32(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_XIAOHEIWU_END_TIME,val)
end

--获取申请成员申请时间
function FactionInfo:GetFactionApplyTime(pos)
	return self:GetUInt32(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOTAL_CONTRIBUTION)
end

--设置申请成员申请时间
function FactionInfo:SetFactionApplyTime(pos,val)
	self:SetUInt32(FACTION_INT_FIELD_APPLY_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOTAL_CONTRIBUTION,val)
end

--获取申请成员GUID
function FactionInfo:GetFactionApplyGuid(pos)
	return self:GetStr(FACTION_STRING_FIELD_APPLY_PLAYER + pos*MAX_FACTION_STRING_MEMBER+FACTION_STRING_MEMBER_GUID)
end

--设置申请成员GUID
function FactionInfo:SetFactionApplyGuid(pos,val)
	self:SetStr(FACTION_STRING_FIELD_APPLY_PLAYER + pos*MAX_FACTION_STRING_MEMBER+FACTION_STRING_MEMBER_GUID,val)
end

--获取申请成员名字
function FactionInfo:GetFactionApplyName(pos)
	return self:GetStr(FACTION_STRING_FIELD_APPLY_PLAYER + pos*MAX_FACTION_STRING_MEMBER+FACTION_STRING_MEMBER_NAME)
end

--设置申请成员名字
function FactionInfo:SetFactionApplyName(pos,val)
	self:SetStr(FACTION_STRING_FIELD_APPLY_PLAYER + pos*MAX_FACTION_STRING_MEMBER+FACTION_STRING_MEMBER_NAME,val)
end

--获取申请成员名字
function FactionInfo:GetFactionApplyFlag()
	return self:GetUInt32(FACTION_INT_FIELD_APPLY_FLAG)
end

--设置申请成员名字
function FactionInfo:SetFactionApplyFlag(val)
	self:SetUInt32(FACTION_INT_FIELD_APPLY_FLAG,val)
end

--获得玩家是否在申请列表中
function FactionInfo:IsApplyPlayer(player_guid)
	for i = 0,MAX_FACTION_APLLY_MAMBER_COUNT-1
	do
		if self:GetFactionApplyGuid(i) == player_guid then
			return true
		end
	end
	return false
end

--获取申请列表可添加的位置
function FactionInfo:GetApplyEmptyPos()
	local pos
	for i = 0,MAX_FACTION_APLLY_MAMBER_COUNT-1 do
		if self:GetFactionApplyGuid(i) == "" then
			pos = i
			break
		end
	end
	return pos
end
--获取申请列表中时间最早的
function FactionInfo:GetApplyMinTimePos()
	local minnum = 4294967295
	local pos
	for i = 0,MAX_FACTION_APLLY_MAMBER_COUNT-1 do
		local time = self:GetFactionApplyTime(i)
		--outFmtDebug("time %d",time)
		if minnum > time then
			minnum = time
			pos = i
		end
		
	end
	return pos
end

--获取邀请列表成员所在位置
function FactionInfo:GetApplyPosFromPlayer(guid)
	local pos
	for i = 0,MAX_FACTION_APLLY_MAMBER_COUNT-1
	do
		if self:GetFactionApplyGuid(i) == guid then
			pos = i
			break
		end
	end
	return pos
end

--添加申请成员
function FactionInfo:SetApplyPlayer(player)	
	--outFmtDebug("SetApplyPlayer")
	local pos = self:GetApplyEmptyPos()
	if pos == nil then
		pos = self:GetApplyMinTimePos()
		--outFmtDebug("SetApplyPlayer3 %d",pos)
	end
	
	if pos then
		self:SetFactionApplyGuid(pos, player:GetGuid())
		self:SetFactionApplyLevel(pos, player:GetLevel())
		self:SetFactionApplyForce(pos, player:GetForce())
		self:SetFactionApplyName(pos, player:GetName())
		self:SetFactionApplyVipLevel(pos,player:GetVIP())
		self:SetFactionApplyIsOnline(pos,1)
		self:SetFactionApplyTime(pos,os.time())
	end
end

--获取申请成员信息
function FactionInfo:GetApplyPlayer(guid)
	local pos = self:GetApplyPosFromPlayer(guid)
	local force,level,name,is_vip
	if pos ~= nil then
		force = self:GetFactionApplyForce(pos)
		level = self:GetFactionApplyLevel(pos)
		name = self:GetFactionApplyName(pos)
		is_vip = self:GetFactionApplyVipLevel(pos)
		--onlinetime = self:GetFactionApplyOnlineTime(pos)
	end
	return force,level,name,is_vip
end

--删除申请成员信息
function FactionInfo:DelApplyPlayer(guid)
	local pos = self:GetApplyPosFromPlayer(guid)
	if pos then
		self:SetFactionApplyGuid(pos, "")
		self:SetFactionApplyLevel(pos,0)
		self:SetFactionApplyForce(pos, 0)
		self:SetFactionApplyName(pos,"")
		self:SetFactionApplyVipLevel(pos,0)
		--self:SetFactionApplyOnlineTime(pos,0)
		--self:SetFactionApplyEndTime(pos,0)
		self:SetFactionApplyIsOnline(pos,0)
		self:SetFactionApplyTime(pos,0)
		--self:SubFactionApplyCount(1)
	end
end
--------------------------------------申请加入帮派成员信息END---------------------------------------------
--添加帮派动态
function FactionInfo:AddEvent( event_name, event_type, event_val)
	local pos = self:GetFactionEventFlag()
	--outFmtDebug("pos %d %d",pos,event_name)
	self:SetFactionEventType(pos,event_type)
	self:SetFactionEventValue(pos,event_val)
	self:SetFactionEventName(pos,event_name)
	pos = pos + 1
	if pos >= MAX_FACTION_EVENT_COUNT then
		pos = 0
	end
	self:SetFactionEventFlag(pos)
	--local factionEventInfo = self:getFactionEventsInfo()
	--if(factionEventInfo ~= nil)then
	--	factionEventInfo:AddFactionEventInfo(self:GetGuid(), player_id, player_name, os.time(), event_type, pi1, pi2, pi3, ps1)
	--end
end

--获取帮派可添加的位置
function FactionInfo:GetFactionEmptyPos()
	local pos
	for i = 0,MAX_FACTION_MAMBER_COUNT - 1 do
		if self:GetFactionMemberGuid(i) == "" then
			pos = i
			break
		end
	end
	return pos
end

--查找玩家位置
function FactionInfo:FindPlayerIndex(PlayGuid)
	local pos
	for i = 0, MAX_FACTION_MAMBER_COUNT - 1 do
		if self:GetFactionMemberGuid(i) ==  PlayGuid then
			pos = i
			break
		end
	end
	return pos
end

function FactionInfo:SetBangZhuInfo(player)
	if not player then
		outFmtDebug("SetBangZhuInfo player is nil")
		return
	end
	self:SetUInt32(FACTION_INT_FIELD_MANGER_COAT,player:GetUInt16(PLAYER_INT_FIELD_APPEARANCE,1))
	self:SetUInt32(FACTION_INT_FIELD_MANGER_WEAPON,player:GetUInt16(PLAYER_INT_FIELD_APPEARANCE,0))
	self:SetUInt32(FACTION_INT_FIELD_MANGER_VIP,player:GetVIP())
	self:SetUInt32(FACTION_INT_FIELD_MANGER_GENDER,player:GetGender())
	self:SetUInt32(FACTION_INT_FIELD_MANGER_TITLE,player:GetTitle())
	self:SetUInt32(FACTION_INT_FIELD_MANGER_LIKE,player:GetUInt32(PLAYER_FIELD_RANK_LIKE))
	self:SetUInt32(FACTION_INT_FIELD_MANGER_WING,player:GetUInt32(PLAYER_INT_FIELD_WINGS_RANK)*100 + player:GetUInt32(PLAYER_INT_FIELD_WINGS_STAR))
end

--设置帮主名字
function FactionInfo:SetBangZhuName(name)
	self:SetStr(FACTION_STRING_FIELD_MANGER_NAME,name)
end
--获得帮主名字
function FactionInfo:GetBangZhuName()
	return self:GetStr(FACTION_STRING_FIELD_MANGER_NAME)
end

--设置帮主GUID
function FactionInfo:SetBangZhuGuid(guid)
	self:SetStr(FACTION_STRING_FIELD_MANGER_GUID,guid)
end
--获得帮主GUID
function FactionInfo:GetBangZhuGuid()
	return self:GetStr(FACTION_STRING_FIELD_MANGER_GUID)
end		

--获得帮主guid
function FactionInfo:GetBangZhuGuid()
	for i=0, MAX_FACTION_MAMBER_COUNT - 1 do
		if self:GetFactionMemberGuid(i) ~= '' and self:GetFactionMemberIdentity(i) == FACTION_MEMBER_IDENTITY_BANGZHU then
			return self:GetFactionMemberGuid(i)
		end
	end
		
	return ''
end

-- priv越小权限越大
-- 检测权限是否足够
function FactionInfo:CheckEnoughPrivilege(guid, priv)
	local pos = self:FindPlayerIndex(guid)
	if pos then
		local minePriv = self:GetFactionMemberIdentity(pos)
		return minePriv > 0 and minePriv <= priv
	end
	return false
end

--获得是否管理员
function FactionInfo:IsManager(guid)
	return self:CheckEnoughPrivilege(guid, tb_faction_privilege[ 1 ].manager)
end

--获得是否核心管理员
function FactionInfo:IsCoreManager(guid)
	return self:CheckEnoughPrivilege(guid, tb_faction_privilege[ 1 ].coreManager)
end

--获得位置人数
function FactionInfo:GetZhiWeiCount(zhiwei)
	local count = 0
	for i = 0, MAX_FACTION_MAMBER_COUNT - 1 do
		if self:GetFactionMemberIdentity(i) == zhiwei then
			count = count + 1
		end
	end
	return count
end	



--帮派申请
function FactionInfo:FactionApply( player)
	if player:GetFactionId() ~= "" then
		--玩家已有帮派
		--player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_IS_HAVE)
		outFmtDebug("you cannot join other, have faction = %s",faction_guid)
		return 
	end
	--if player:GetLevel() < tb_bangpai[1].need_level then
		--玩家等级不够
	--	player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_LEVEL_LACK)
	--	return 
	--end
	if self:GetMemberCount() >= self:GetMemberMaxCount() then
		--帮派人数已满
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_MEMBER_MAX_COUNT)
		return 
	end
	if self:GetFactionApplyCount() == MAX_FACTION_MAMBER_COUNT then
		--申请列表人数已满
		player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_APPLY_MAX_COUNT)
		return
	end
	
	--否则添加到申请列表中
	if not self:IsApplyPlayer(player:GetGuid()) then-- and player:AddFactionQuest(self:GetGuid()) 
		self:SetApplyPlayer(player)
	else
		outFmtDebug("you cannot join other, have apply")
	end
end

--添加帮派成员
function FactionInfo:MemberAdd( player, isInvited)
	isInvited = isInvited or false
	local player_guid = player:GetGuid()
	if self:FindPlayerIndex(player_guid) ~= nil then
		--玩家已经在帮派了
		return false
	end
	
	if player:GetFactionId() ~= "" then
		--玩家已经有帮派了
		player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_IS_HAVE)
		return false
	end

	if self:GetMemberCount() >= self:GetMemberMaxCount() then
		--帮派人数已满
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_MEMBER_MAX_COUNT)
		return 
	end
	
	local pos = self:GetFactionEmptyPos()
	if pos == nil then
		--没有找到空位,帮派人数已满
		player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_MEMBER_MAX_COUNT)
		return false
	end
	
	local force,level,name,is_vip--,onlinetime
	--创建者 or 不需要审核
	if self:GetMemberCount() == 0 or self:GetFactionFlags(FACTION_FLAGS_AUTO) or isInvited then	
		force = player:GetForce()
		level = player:GetLevel()
		name = player:GetName()
		is_vip = player:GetVIP()
		--onlinetime = player:GetOnlineTime()
	else
		if self:IsApplyPlayer(player_guid) then
			--force,level,name,is_vip = self:GetApplyPlayer(player_guid)
			force = player:GetForce()
			level = player:GetLevel()
			name = player:GetName()
			is_vip = player:GetVIP()
			self:DelApplyPlayer(player_guid)
		else
			return false
		end
	end
	--设置相关数据
	self:SetFactionMemberForce(pos, force)
	self:SetFactionMemberLevel(pos, level)
	if self:GetMemberCount() == 0 then
		self:SetFactionMemberIdentity(pos, FACTION_MEMBER_IDENTITY_BANGZHU)
	else
		self:SetFactionMemberIdentity(pos, FACTION_MEMBER_IDENTITY_QUNZHONG)
		--player:DelFactionQuest()
		--self:AddEvent(player_guid, name, FACTION_EVENT_TYPE_ADD_MEMBER)
	end
	self:SetFactionMemberGuid(pos, player_guid)
	self:SetFactionMemberName(pos, name)
	self:SetFactionMemberVipLevel(pos,is_vip)
	--self:SetFactionMemberOnlineTime(pos,onlinetime)
	self:SetFactionMemberIsOnline(pos,1)
	--self:SetFactionMemberCharm(pos,player:GetPlayerCharmPoint())
	
	self:AddMemberCount(1)
	player:SetFactionId(self:GetGuid())
	player:SetFactionName(self:GetName())
	player:UpdateFactionSkill()
	
	-- 添加监听
	app.objMgr:callAddWatch(player:GetSessionId(), self:GetGuid())
	--app.objMgr:callAddWatch(player:GetSessionId(), self:getFactionEventsGuid())

	--rankInsertTask(self:GetGuid(), RANK_TYPE_FACTION)
	local questMgr = player:getQuestMgr()
	questMgr:OnUpdate(QUEST_TARGET_TYPE_FACTION)
	return true
end

function FactionInfo:FactionExit(player)
	local player_guid = player:GetGuid()
	local index = self:FindPlayerIndex(player_guid)
	if index == nil then
		return
	end
	--如果退出是帮主, 强制踢掉全部
	if self:GetFactionMemberIdentity(index) == FACTION_MEMBER_IDENTITY_BANGZHU then	
		self:FactionDissolution()
		self:SetUInt32(FACTION_INT_FIELD_IS_DISSOLVED, 1)
		-- 踢掉排名 (帮派等级)
		rankInsertTask(self:GetGuid(), RANK_TYPE_FACTION)
		-- TODO:魅力的踢掉
		-- rankInsertTask(self:GetGuid(), RANK_TYPE_CHARM)
	else
		self:FactionQuit(player)
	end
end

--成员退出
function FactionInfo:FactionQuit( player,is_merge)
	local player_guid = player:GetGuid()
	local index = self:FindPlayerIndex(player_guid)
	if index == nil then
		return
	end
	
	player:SetFactionId("")
	player:SetFactionName("")
	--移除监听
	app.objMgr:callDelWatch(player:GetSessionId(),self:GetGuid())

	--app.objMgr:callDelWatch(player:GetSessionId(),self:getFactionEventsGuid())
	--成员离开后的处理
	self:RemoveChallengeBossDamageRankByGuid(player_guid)
	self:RemoveChallengeBossTotalRankByGuid(player_guid)
	self:DoChangeMemberOpt(index)
	
	--self:DelGiftInfo(player_guid)
	if is_merge == nil then
		--self:AddEvent(player:GetGuid(), player:GetName(), FACTION_EVENT_TYPE_SUB_MEMBER)
	end
	player:ClearBuyedFactionShopItem()
	player:UpdateFactionSkill()
	
	-- 成员退出
	-- 如果在帮派场景的强制传送到冒险世界
	if player:GetMapId() == FACTION_MAP_ID then
		local passedSectionId = player:GetUInt32(PLAYER_INT_FIELD_TRIAL_FINISHED_SECTIONID)
		local mapid, x, y, generalId = onGetRiskTeleportInfo(player:GetGuid(), passedSectionId)
		call_appd_teleport(player:GetScenedFD(), player:GetGuid(), x, y, mapid, generalId)
	end
	
end
--离线成员移除
function FactionInfo:FactionOutlineQuit(guid)
	local index = self:FindPlayerIndex(guid)
	if index == nil then
		return
	end
	
	self:RemoveChallengeBossDamageRankByGuid(guid)
	self:RemoveChallengeBossTotalRankByGuid(guid)
	self:DoChangeMemberOpt(index)
end
--帮派解散
function FactionInfo:FactionDissolution()

	for i = 0, MAX_FACTION_MAMBER_COUNT - 1 do
		local pguid = self:GetFactionMemberGuid(i)
		if pguid ~=  "" then
			local member = app.objMgr:getObj(pguid)
			if member then
				self:FactionQuit(member,true)
			else
				self:FactionOutlineQuit(pguid)
			end
		end
	end
	
	app.objMgr:callRemoveObject(self:GetGuid())
end

--成员离开后的处理
function FactionInfo:DoChangeMemberOpt(index)
	--成员人数-1
	self:SubMemberCount(1)
	--如果帮派解散
	if self:GetMemberCount() == 0 then
		--local faction_events_guid = self:getFactionEventsGuid()
		app.objMgr:callRemoveObject(self:GetGuid())
		--app.objMgr:callRemoveObject(faction_events_guid)
		return
	end
	--清理数据
	self:SetFactionMemberForce(index, 0)
	self:SetFactionMemberLevel(index, 0)
	self:SetFactionMemberDayGongXian(index,0)
	self:SetFactionMemberIdentity(index, 0)
	self:SetFactionMemberVipLevel(index,0)
	self:SetFactionMemberTotalSendCharm(index,0)
	--self:SetFactionMemberOnlineTime(index,0)
	self:SetFactionMemberLogoutTime(index,0)
	self:SetFactionMemberTotalGongXian(index,0)
	self:SetFactionMemberGuid(index, "")
	self:SetFactionMemberName(index, "")
end


--同意加入帮派
function FactionInfo:FactionAgreeJoin( player, apply_guid)

	local applyer = app.objMgr:getObj(apply_guid)
	if applyer == nil then
		--找不到该玩家
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_PLAYER_OFFLINE)
		return 
	end
	
	if not self:IsManager(player:GetGuid()) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	
	if not self:IsApplyPlayer(apply_guid) then
		--玩家没有在申请列表
		--player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_NOT_IN_APPLY_LIST)
		outFmtDebug("apply user is not in apply list")
		return
	end
	
	if self:GetMemberCount() >= self:GetMemberMaxCount() then
		--帮派人数已满
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_MEMBER_MAX_COUNT)
		return 
	end
	
	if applyer:GetFactionId() ~= "" then
		--玩家已经有帮派了
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_IS_HAVE)
		--申请列表的数据不是最新的，这里手动清除一下数据
		self:DelApplyPlayer(apply_guid)
		return
	end
	
	if self:MemberAdd(applyer) then
		applyer:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_JOIN_SUCESS)
		--player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_JOIN_SUCCESS, {apply_guid,applyer:GetName()})
	end
end

--全部同意加入帮派
function FactionInfo:FactionAgreeJoinAll( player )
	
	if not self:IsManager(player:GetGuid()) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	
	for i = 0,MAX_FACTION_APLLY_MAMBER_COUNT-1 do
		local apply_guid = self:GetFactionApplyGuid(i)
		if apply_guid ~= "" then
			self:FactionAgreeJoin( player, apply_guid)
		end
	end

end

--拒绝加入帮派
function FactionInfo:FactionRefuseJoin(player,apply_guid)
	local applyer = app.objMgr:getObj(apply_guid)
	if applyer == nil then
		--找不到该玩家
		self:DelApplyPlayer(apply_guid)
		return 
	end
	
	if not self:IsManager(player:GetGuid()) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	
	if not self:IsApplyPlayer(apply_guid) then
		--玩家没有在申请列表
		--player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_NOT_IN_APPLY_LIST)
		outFmtDebug("player is not in apply list")
		return
	end
	self:DelApplyPlayer(apply_guid)
	--applyer:DelFactionQuest(self:GetGuid())
	applyer:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_FACTION_REFUSED_JOIN, self:GetName())
end
--拒绝所有人加入帮派
function FactionInfo:FactionRefuseJoinAll(player)
	if not self:IsManager(player:GetGuid()) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	
	for i = 0,MAX_FACTION_APLLY_MAMBER_COUNT-1 do
		local apply_guid = self:GetFactionApplyGuid(i)
		if apply_guid ~= "" then
			self:FactionRefuseJoin( player, apply_guid)
		end
	end
end
--踢出帮派
function FactionInfo:MemberKicked( player, member_id)
	local player_guid = player:GetGuid()
	
	--local pos = self:FindPlayerIndex(player_guid)
	--if pos == nil then return end
	
	--if self:GetFactionMemberIdentity(pos) ~= FACTION_MEMBER_IDENTITY_BANGZHU
	--	and self:GetFactionMemberIdentity(pos) ~= FACTION_MEMBER_IDENTITY_FU_BANGZHU then
		--不是帮主或者负帮主
	--	player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_NO_MANAGER)
	--	return
	--end
	
	if not self:IsManager(player:GetGuid()) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	
	if member_id == player_guid then
		--自己不能踢自己
		return
	end
	
	local member_pos = self:FindPlayerIndex(member_id)
	if member_pos == nil then
		--不是本帮派的人
		return
	end
	local member = app.objMgr:getObj(member_id)
	if member then
		--outFmtDebug("on line %s",member_id)
		self:FactionQuit(member,true)
		member:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_KICKED, self:GetName())
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_KICK_SUCCESS, {member_id,member:GetName()})
	else
		--outFmtDebug("not on line %s",member_id)
		self:FactionOutlineQuit(member_id)
	end
end

--职位任免
function FactionInfo:FactionAppoint( player, member_id,zhiwei)
	--print("zhiwei %d",zhiwei)
	if zhiwei < FACTION_MEMBER_IDENTITY_BANGZHU or zhiwei > FACTION_MEMBER_IDENTITY_QUNZHONG then
		--职位错误
		return
	end
	
	local player_guid = player:GetGuid()
	local pos = self:FindPlayerIndex(player_guid)
	if pos == nil then return end
	if not self:IsCoreManager(player_guid) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	
	if member_id == player_guid then
		--自己不能任免自己
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_APPOINT_NOSELF)
		return
	end
	
	local member_pos = self:FindPlayerIndex(member_id)
	if member_pos == nil then
		--不是本帮派的人
		return
	end
	
	if self:GetFactionMemberIdentity(member_pos) == zhiwei then
		--职位相同
		return
	end
	--outFmtDebug("zhiwei %d",zhiwei)
	
	local config = tb_faction_zhiwei[zhiwei].num
	if zhiwei ~= FACTION_MEMBER_IDENTITY_BANGZHU and config then
		if self:GetZhiWeiCount(zhiwei) >= config then
		--该职位人数已满
			player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_MAX_ZHIWEI)
			return
		end
	end
	
	
	--如果是替换帮主
	if zhiwei == FACTION_MEMBER_IDENTITY_BANGZHU  then
		if self:GetFactionMemberIdentity(pos) == FACTION_MEMBER_IDENTITY_BANGZHU then--帮主自己才能转让
			local data_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionData)
			local factionData = app.objMgr:getObj(data_guid)
			self:ThankAndReplyAllFactionGift(factionData,player)
			
			self:SetFactionMemberIdentity(pos,FACTION_MEMBER_IDENTITY_QUNZHONG)
			self:SetBangZhuName(self:GetFactionMemberName(member_pos))
			self:SetBangZhuGuid(self:GetFactionMemberGuid(member_pos))
			
			
			--[[
			--修改魅力排行
			local pos = self:FindPlayerIndex(member_id)
			
			self:SetFactionCharmPoint(self:GetFactionMemberCharm(pos))
			
			if self:GetFactionCharmPoint() > 0 then
			
				local queen_vip = self:GetFactionMemberVipLevel( self:FindPlayerIndex(self:GetBangZhuGuid()))
				local guard_guid,guard_name,guard_vip = self:GetWeekGuardInfo()
				local prev_rank, new_rank = UpdateFactionRank(self:GetFactionCharmPoint(),self:GetBangZhuGuid(),self:GetBangZhuName(),self:GetGuid(),self:GetName(),self:GetFactionCurFlagId(),os.time(),guard_guid,guard_name,queen_vip,guard_vip)
				
				if new_rank ~= 0 and (new_rank < prev_rank or prev_rank == 0) then
					--帮派广播排行变化
					local stru = faction_gift_rank_info_t .new()
					stru.rank = new_rank
					stru.point = self:GetFactionCharmPoint()
					stru.queen_name =  self:GetBangZhuName()
					stru.faction_name = self:GetName()
					stru.guard_name = guard_name
					stru.faction_flag = self:GetFactionCurFlagId()
					stru.queen_vip = queen_vip
					stru.guard_vip = guard_vip
					for pos = 0, MAX_FACTION_APLLY_MAMBER_COUNT -1 do
						local guid = self:GetFactionMemberGuid(pos)
						if guid ~= '' then
							local player = app.objMgr:getObj(guid)
							if player then
								player:call_show_faction_gift_rank_change(prev_rank,new_rank,stru)
							end
						
						end
					end
				end
			end
			--]]
		else
			--print("aaaa")
			return
		end
	end
	
	self:SetFactionMemberIdentity(member_pos,zhiwei)
	
	player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_APPOINT_SUCCESS)
	local member = app.objMgr:getObj(member_id)
	if member then
		member:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_PROMOTED,zhiwei)
		--self:AddEvent(player:GetGuid(), player:GetName(), FACTION_EVENT_TYPE_APPOINT,zhiwei,0,0,member:GetName())
	end
end

--招募设置
function FactionInfo:FactionRecruit(player,reserve_int1,reserve_int2,reserve_str1 )
	--print(reserve_int1,reserve_int2,reserve_str1,FACTION_FLAGS_AUTO)
	if reserve_int2 < 0 then
		outFmtDebug("faction Recruit level error")
		return;
	end
	if string.utf8len(reserve_str1) > 100 then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOTICE_ERR)
		return
	end
	local player_guid = player:GetGuid()
	--local pos = self:FindPlayerIndex(player_guid)
	--if pos == nil then return end
	if not self:IsManager(player_guid) then
		--不是帮主
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	
	if reserve_int1 == 1 then
		self:SetFactionFlags(FACTION_FLAGS_AUTO)
	else
		self:UnSetFactionFlags(FACTION_FLAGS_AUTO)
	end
	
	self:SetFactionMinLev(reserve_int2)
	self:SetFactionZhaoMuNotice(reserve_str1)

	--rankInsertTask(self:GetGuid(), RANK_TYPE_FACTION)
end

--帮派升级
function FactionInfo:FactionLevelUp(player )
	local player_guid = player:GetGuid()
	if not self:IsManager(player_guid) then
		--不是帮主
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	local lv = self:GetFactionLevel()
	local money = self:GetFactionMoney()
	local config = tb_faction_base[lv]
	if config == nil or lv >= #tb_faction_base then 
		return 
	end
	--判断金钱
	if money < config.cost then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPEATE_TYPE_FACTION_MONEY_ERR)
		return
	end
	self:SetFactionMoney(money - config.cost)
	self:SetFactionLevel(lv + 1)
	--self:RefreshShop()
	--升级成功
	player:CallOptResult(OPERTE_TYPE_FACTION, OPEATE_TYPE_FACTION_LEVEL_UP)
	
	rankInsertTask(self:GetGuid(), RANK_TYPE_FACTION)
end




--替换帮旗
function FactionInfo:FactionChangeFlags(player ,index)
	local player_guid = player:GetGuid()
	local pos = self:FindPlayerIndex(player_guid)
	if pos == nil then return end
	
	-- 核对权限
	if not self:CheckEnoughPrivilege(player_guid, tb_faction_privilege[ 1 ].replaceFlag) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_NO_MANAGER)
		return
	end
	local has_money = player:GetMoney(MONEY_TYPE_GOLD_INGOT)
	local need_money = tb_bangpai[1].cost_qi[1]
	local guid = globalValue:GetFactionFlagsGuid(index)
	if guid == nil then return end
	local is_rank = true
	--默认旗帜
	if index == 0 or guid == self:GetGuid() then
		self:FactionFlags(index)
		return
	--该旗帜有归属	
	elseif guid ~= "" then
		need_money = tb_bangpai[1].cost_qi[2]
		--判断一下排名
		local faction = app.objMgr:getObj(guid)
		if faction then
			if self:GetFactionRank() > faction:GetFactionRank() then
				--排名不够
				is_rank = false
			end
		end
	end
	if not is_rank then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPRATE_TYPE_FACTION_RANK_ERR)
		return
	end
	if money < need_money then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPEATE_TYPE_FACTION_MONEY_ERR)
		return
	end
	--扣钱
	if not player:SubMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_FACTION_FLAGS, need_money) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPEATE_TYPE_FACTION_MONEY_ERR)
		return
	end
	self:FactionFlags(index,guid)
end

function FactionInfo:FactionFlags(index ,guid)
	--默认旗帜
	if guid == nil then
		self:SetFactionCurFlagId(index)
		return
	end
	
	globalValue:SetFactionFlagsGuid(index,self:GetGuid())
	self:SetFactionCurFlagId(index)
	
	if guid and guid ~= "" then
		local faction = app.objMgr:getObj(guid)
		if faction then
			faction:FactionFlags(0)
		end
	end
end
--帮会公告
function FactionInfo:FactionNotice(player,notice)
	
	local player_guid = player:GetGuid()
	local pos = self:FindPlayerIndex(player_guid)
	if pos == nil then return end
	local tab = string.split(notice,"\1")
	if #tab ~= 3 then
		outFmtInfo("faction notice format error");
		return
	end
	--if #tab
	-- 核对权限
	if not self:CheckEnoughPrivilege(player_guid, tb_faction_privilege[ 1 ].notice) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOT_MANAGER)
		return
	end
	
	--帮派公告不能超过100个字符
	
	if string.utf8len(notice) > 100 then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOTICE_ERR)
		return
	end
	
	--判断是否有屏蔽词
	if fuckPingBi(notice) ~= notice then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_NOTICE_ERR_PB)
		return
	end
	print(notice,56) 
	self:SetFactionNotice(notice)
	--self:SetFactionNoticeTime(os.time())
	
	--local allPlayers = self:GetFactionAllMemberPtr()
	--for _, people in pairs(allPlayers) do
	--	people:call_faction_chat("","公告", notice)
	--end
end







--帮派成员心跳
function FactionInfo:Update( player)
	--玩家在帮派位置
	local Faction_Pos = self:FindPlayerIndex(player:GetGuid())
	if Faction_Pos == nil then
		return
	end		
	
	--玩家战斗力	
	if player:GetForce() ~= self:GetFactionMemberForce(Faction_Pos) then
		self:SetFactionMemberForce(Faction_Pos,player:GetForce())
	end
	
	--玩家等级
	if player:GetLevel() ~= self:GetFactionMemberLevel(Faction_Pos) then
		self:SetFactionMemberLevel(Faction_Pos, player:GetLevel())
	end
	
	
	--玩家VIP等级
	if player:GetVIP() ~= self:GetFactionMemberVipLevel(Faction_Pos) then
		self:SetFactionMemberVipLevel(Faction_Pos, player:GetVIP())
	end
	
	--玩家在线时长
	if player:GetOnlineTime() ~= self:GetFactionMemberOnlineTime(Faction_Pos) then
		self:SetFactionMemberOnlineTime(Faction_Pos, player:GetOnlineTime())
	end
	
	-- 更新玩家名字
	if player:GetName() ~= self:GetFactionMemberName(Faction_Pos) then
		self:SetFactionMemberName(Faction_Pos, player:GetName())
	end
	
	--更新地位
	if self:GetFactionMemberIdentity(Faction_Pos) == FACTION_MEMBER_IDENTITY_QUNZHONG then
		if self:GetFactionMemberTotalGongXian(Faction_Pos) > tb_bangpai[1].zhiwei[1] then
			self:SetFactionMemberIdentity(Faction_Pos,FACTION_MEMBER_IDENTITY_GAOSHOU)
		end
	end
	if self:GetFactionMemberIdentity(Faction_Pos) == FACTION_MEMBER_IDENTITY_GAOSHOU then
		if self:GetFactionMemberTotalGongXian(Faction_Pos) > tb_bangpai[1].zhiwei[2] then
			self:SetFactionMemberIdentity(Faction_Pos,FACTION_MEMBER_IDENTITY_JINGYING)
		end
	end
	
	--打boss加成
	if self:GetFactionFlags(FACTION_FLAGS_IS_BOSS_ADD) then
		player:GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_FACTION_BOSS_ADD)
	else
		player:UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_FACTION_BOSS_ADD)
	end
	
end
--获取商品index 和 num
function FactionInfo:GetShopItem(item)
	for i=FACTION_INT_FIELD_SHOP,FACTION_INT_FIELD_SHOP_END-1 do
		local id = self:GetUInt16(i,0)
		if id == item then
			return i,self:GetUInt16(i,1)
		end
	end
	return -1,0
end
--设置商品数量
function FactionInfo:SetShopItemNum(idx,num)
	self:SetUInt16(idx,1,num)
end
--商店购买
function FactionInfo:ShopItem(player,item,num)
	if num <= 0 then
		return
	end
	local idx,curNum = self:GetShopItem(item)
	local buyedIndex, buyedNum = player:GetBuyedFactionShopItem(item)
	if idx ~= -1 then
		if num > curNum - buyedNum then
			player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_SHOP_NUMLOW)
			return
		end
		local config = tb_faction_shop[item]
		if not config then
			return
		end
		
		if not player:checkMoneyEnoughs(config.costResource,num) then
			player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_DEVOTE)
			return
		end
	
		if player:costMoneys(MONEY_CHANGE_FACTION_SHOP,config.costResource,num) then
			curNum = curNum - num
			--self:SetShopItemNum(idx,curNum)
			player:AddBuyedFactionShopItem(item,num)
			player:AddItemByEntry(config.itemId, num, nil, 9, true)--FIXME
		end
	end
end
--商店刷新
function FactionInfo:RefreshShop()
	local lev = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_SHOP)
	--print(lev)
	local config = tb_faction_base[lev]
	if not config then
		return
	end
	local num = config.shop
	
	local list = tb_faction_shop_list[lev]
	
	local item_list = {}
	for _,group_info in pairs(num) do
		local group_id = group_info[1]
		local group_num = group_info[2]
		if group_num > #list[group_id] then
			group_num = #list[group_id]
		end
		local idxTab = GetRandomIndexTable(#list[group_id],group_num)
		
		for _,index in pairs(idxTab) do
			local randomResult = randIntD(1,100)
			for _,info in ipairs(list[group_id][index]) do
				if info.total_weight >= randomResult then
					table.insert(item_list,info.config)
					
					break
				end
			end
		end
		
	end
	
	for i=FACTION_INT_FIELD_SHOP,FACTION_INT_FIELD_SHOP_END-1 do
		local idx = i - FACTION_INT_FIELD_SHOP + 1
		
		if item_list[idx] then
			local item = item_list[idx]
			if item then
				self:SetUInt16(i,0,item.id)
				self:SetUInt16(i,1,item.num)
			else 
				self:SetUInt16(i,0,0)
				self:SetUInt16(i,1,0)
			end
		end
	end
	
	--[[
	if num > #list then
		num = #list
	end
	local idxTab = GetRandomIndexTable(#list,num)
	--for k,v in ipairs(idxTab) do
	--	print(k,v)
	--end
	for i=FACTION_INT_FIELD_SHOP,FACTION_INT_FIELD_SHOP_END-1 do
		local idx = i - FACTION_INT_FIELD_SHOP + 1
		
		if idxTab[idx] then
			local item = list[idxTab[idx]]
	--[[
			if item then
				self:SetUInt16(i,0,item.id)
				self:SetUInt16(i,1,item.num)
			else 
				self:SetUInt16(i,0,0)
				self:SetUInt16(i,1,0)
			end
		else 
			self:SetUInt16(i,0,0)
			self:SetUInt16(i,1,0)
		end
	end
	--]]
end
--帮派自身心跳
function FactionInfo:SelfUpdate()

	--local faction_force = 0
	--local is_boss_add = false
	--for iNum = 0, MAX_FACTION_MAMBER_COUNT - 1 do
		--更新战斗力
		--faction_force = faction_force + self:GetFactionMemberForce(iNum)
		--更新是否打帮派boss有加成
		--local player = app.objMgr:getObj(self:GetFactionMemberGuid(iNum))
		--if player and player:GetFactionFbCount() > 0 then
		--	is_boss_add = true
		--end
	--end	
	--self:SetFactionForce(faction_force)
	--if is_boss_add then
	--	self:SetFactionFlags(FACTION_FLAGS_IS_BOSS_ADD)
	--else
	--	self:UnSetFactionFlags(FACTION_FLAGS_IS_BOSS_ADD)
	--end
	
	--看下帮主是否已经离线超过3天
	local manager_guid = self:GetBangZhuGuid()
	local pos = self:FindPlayerIndex(manager_guid)
	--帮派成员大于1个才有必要换
	if pos ~= nil and self:GetMemberCount() > 1 then
		local logout_time = self:GetFactionMemberLogoutTime(pos)
		if logout_time > 0 and logout_time < os.time() - tb_bangpai[1].leave_time*3600 then
			local index_ass = -1
			local zhiwei = FACTION_MEMBER_IDENTITY_QUNZHONG
			for i = 0, MAX_FACTION_MAMBER_COUNT - 1 do		
				local guid = self:GetFactionMemberGuid(i)
				if guid ~= "" and guid ~= player_guid then
					if zhiwei > self:GetFactionMemberIdentity(i) then
						zhiwei = self:GetFactionMemberIdentity(i)
						index_ass = i
					end
				end
			end
			if index_ass ~= -1 then
				self:SetFactionMemberIdentity(pos,self:GetFactionMemberIdentity(index_ass))
				self:SetFactionMemberIdentity(index_ass, FACTION_MEMBER_IDENTITY_BANGZHU)
			end
		end
	end
	
	--更新申请列表
	for i = 0,MAX_FACTION_APLLY_MAMBER_COUNT - 1 do
		local apply_guid = self:GetFactionApplyGuid(i)
		if apply_guid ~= "" then
			local applyer = app.objMgr:getObj(apply_guid)
			if applyer then
				--如果玩家已经加入其它帮派，则清理一下该位置的申请数据
				if applyer:GetFactionId() ~= "" then
					self:DelApplyPlayer(apply_guid)
				end
			end
		end
	end
end

--玩家离线 
function FactionInfo:PlayerLogout(player_guid)
	local FactionId = self:FindPlayerIndex(player_guid)
	if FactionId == nil then
		return false
	end
	self:SetFactionMemberIsOnline(FactionId,0)
	self:SetFactionMemberLogoutTime(FactionId,os.time())
	return true
end
	
--玩家上线
function FactionInfo:PlayerLogin( player)
	local FactionId = self:FindPlayerIndex(player:GetGuid())
	if FactionId == nil then
		return false
	end
	self:SetFactionMemberIsOnline(FactionId,1)
	self:SetFactionMemberLogoutTime(FactionId,0)
	return true
end


--获取所有团员的GUID
function FactionInfo:DoGetFactionAllMember()
	local result = {}
	for i=0, MAX_FACTION_MAMBER_COUNT - 1 do
		local Guid = self:GetFactionMemberGuid(i)
		if Guid ~= "" and Guid ~= nil then
			table.insert(result,Guid)
		end
	end
	return result
end


--获取所有团员的对象
function FactionInfo:GetFactionAllMemberPtr()
	local result = {}
	local player
	for i=0, MAX_FACTION_MAMBER_COUNT - 1 do
		local Guid = self:GetFactionMemberGuid(i)
		if Guid ~= "" and Guid~= nil then
			player = app.objMgr:getObj(Guid)
			if player then	-- 在线帮派成员
				table.insert(result, player)
			end
		end
	end
	return result
end

--帮派解散
function FactionInfo:AllFactionPlayerQuit()	
	for i=MAX_FACTION_MAMBER_COUNT-1, 0, -1 do
		local Guid = self:GetFactionMemberGuid(i)
		if Guid ~= "" and Guid~= nil then
			local player = app.objMgr:getObj(Guid)
			if player then	-- 在线则退出
				self:FactionQuit(player,true)
			end
		end
	end
end

-- 检测下玩家是否是军团成员
function FactionInfo:CheckFactionMember(player)
	if self:FindPlayerIndex(player:GetGuid()) ~= nil then
		player:SetFactionId(self:GetGuid())
		player:SetFactionName(self:GetName())
		self:PlayerLogin(player)
		--下发binlog给客户端
		app.objMgr:callAddWatch(player:GetSessionId(), self:GetGuid())
		app.objMgr:callAddWatch(player:GetSessionId(), self:getFactionEventsGuid())
	end
end

--根据帮派查找帮派动态
function FactionInfo:getFactionEventsInfo( )
	if not self.guid_FactionEventsInfo then
		self.guid_FactionEventsInfo = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionEvents)
	end
	return app.objMgr:getObj(self.guid_FactionEventsInfo)
end

--根据帮派查找帮派动态guid
function FactionInfo:getFactionEventsGuid( )
	if not self.faction_events_guid then
		self.faction_events_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionEvents)
	end
	return self.faction_events_guid
end

--捐献
function FactionInfo:FactionJuanXian(player,pos,money_type,money_sum)
	local baseconfig = tb_faction_donation[money_type]
	if baseconfig == nil then
		return
	end
	if money_type ~= 1 and money_type ~= 2 then
		return
	end
	if money_sum <= 0 then
		return
	end
	--获取最大捐献次数
	local maxNum = 0
	local config = tb_faction_base[self:GetFactionLevel()]
	if config then
		if money_type == 1 then
			maxNum = config.golddonation
		elseif money_type == 2 then
			maxNum = config.ybdonation
		end
	else
		return
	end
	--outFmtDebug("maxNum %d",maxNum)

	local vip = player:GetVIP()
	--outFmtDebug("vip %d",vip)
	if vip > 0 then
		local vipConfig = tb_vip_base[vip]
		if money_type == 1 then
			maxNum = maxNum + vipConfig.factiondonation
		elseif money_type == 2 then
			maxNum = maxNum + vipConfig.factionybdonation
		end
	end
	--当前次数
	local currentNum
	if money_type == 1 then
		currentNum = self:GetFactionMemberGoldDonation(pos)
	elseif money_type == 2 then
		currentNum = self:GetFactionMemberYbDonation(pos)
	end
	--outFmtDebug("curNum %d",currentNum)
	
	--outFmtDebug("currentNum %d,%d",currentNum,money_sum)

	local targetNum = currentNum + money_sum
	if targetNum > maxNum then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_DONATIONMAX)
		return
	end
	
	--print(baseconfig.cost)
	if not player:checkMoneyEnoughs(baseconfig.cost,money_sum) then
		if money_type == 1 then
			player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_DONATION_GOLD)
		elseif money_type == 2 then
			player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_DONATION_YB)
		end

		return
	end
	
	if player:costMoneys(MONEY_CHANGE_FACTION_DONATION,baseconfig.cost,money_sum) then
		local devote = baseconfig.devote[2] * money_sum
		local zijin = baseconfig.money * money_sum
		self:AddFactionMoney(zijin)
		self:AddFactionMemberTotalGongXian(pos,devote)
		player:AddMoney(baseconfig.devote[1], MONEY_CHANGE_FACTION_DONATION, devote)
		
		self:AddEvent(player:GetName(),money_type,money_sum)
		if money_type == 1 then
			self:AddFactionMemberGoldDonation(pos,money_sum)
		elseif money_type == 2 then
			self:AddFactionMemberYbDonation(pos,money_sum)
		end
		
		-- 加任务
		local questMgr = player:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_FACTION_DONATION, {money_type, money_sum})
		
		player:AddActiveItem(VITALITY_TYPE_FACTION_DONATE)
	end
	

	
	
	--local cost_table = {}
	--table.insert(cost_table,money_type)
	--table.insert(cost_table,money_sum)
	--if not player:SubItemByConfig(cost_table,MONEY_CHANGE_FACTION_JUANXIAN,OPERTE_TYPE_FACTION,OPEATE_TYPE_FACTION_MONEY_ERR) then
	--	return
	--end
	
	--local gongxian = (money_sum - 2) * 6 + 10
	--local zijin = (money_sum - 2) * 10 + 15
	--if money_type == 1 then	--铜钱
	--	gongxian = (money_sum - 50000) / 10000 * 2.5 + 10
	--	zijin = (money_sum - 50000) / 10000 * 2.5 + 10
	--end
	
	--self:AddFactionMemberDayGongXian(pos,gongxian)
	--self:AddFactionMemberTotalGongXian(pos,gongxian)
	--self:AddFactionMoney(pos,zijin)

	--self:AddEvent(player:GetGuid(), player:GetName(), FACTION_EVENT_TYPE_JUANXIAN,money_type,money_sum,gongxian)
end

--帮派每日重置
function FactionInfo:ResetFaction()
	--for i = 0, MAX_FACTION_MAMBER_COUNT - 1 do
	--	if self:GetFactionMemberGuid(i) ~= "" then
	--		self:SetFactionMemberGoldDonation(i,0)
	--		self:SetFactionMemberYbDonation(i,0)
	--	end
	--end
	
	self:RefreshShop()
	
	self:SetTokenBuyCount(0)
	self:SetTokenPointsCount(0)
	
	--self:DailyResetGift()
	
	self:DailyResetTower()
	
end

function FactionInfo:ResetFactionWeek()
	
	--self:ClearGiftWeekPoint()
	
end

--帮派重置成员每日信息
function FactionInfo:ResetMember(player)
	local pos = self:FindPlayerIndex(player:GetGuid())
	if pos then
		self:SetFactionMemberGoldDonation(pos,0)
		self:SetFactionMemberYbDonation(pos,0)
	end
end
--领取福利
function FactionInfo:FactionFuLi(player,pos)
	
	if player:IsLingQuFactionFuLi() then
		--领取过了
		return
	end
	local lv = self:GetFactionLevel()
	local zhiwei = self:GetFactionMemberIdentity(pos)
	if tb_bangpai_level[lv] == nil then return end
	local free_table = tb_bangpai_level[lv].free[zhiwei]
	if free_table == nil then return end
	
	for i = 1,#free_table,2 do
		player:AddItemByEntry(free_table[i], free_table[i+1], MONEY_CHANGE_BOX_OPEN,LOG_ITEM_OPER_TYPE_FACTION_FULI, ITEM_BIND_GET)
	end
	--设置领取
	player:SetLingQuFactionFuLi(1)
end

--发红包
function FactionInfo:FactionFaHongBao(player,hongbao,hongbao_count)
	--判断元宝
	if player:GetMoney(MONEY_TYPE_GOLD_INGOT) < hongbao then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPEATE_TYPE_FACTION_MONEY_ERR)
		return
	end
	--红包元宝数小于红包个数 （1个红包最低是1元宝）
	if hongbao < hongbao_count then
		return
	end
	
	--扣钱
	if not player:SubMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_FACTION_HONGBAO, hongbao) then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPEATE_TYPE_FACTION_MONEY_ERR)
		return
	end
	
	self:AddFactionHongBao(hongbao,hongbao_count,player:GetGuid(), player:GetName())
	self:AddEvent(player:GetGuid(), player:GetName(), FACTION_EVENT_TYPE_HONGBAO,hongbao,hongbao_count)
end

--领取红包
function FactionInfo:FactionLqHongBao(player,guid)
	local index = self:FindFactionHongBaoPos(guid)
	if index == nil then
		return
	end
	local money = self:GetFactionHongBao(index)
	local count = self:GetFactionHongBaoCount(index)
	local get_money = 0
	if count == 1 then
		get_money = money
		self:DelFactionHongBao(index)
	else
		get_money = randIntD(1,money - count)
		self:SetFactionHongBaoCount(index,count-1)
		self:SetFactionHongBao(index,money-get_money)
	end
	--给元宝
	player:AddMoney(MONEY_TYPE_GOLD_INGOT, MONEY_CHANGE_FACTION_HONGBAO, get_money)
	
end

--上香
function FactionInfo:FactionShangXiang(player,pos,type)
	local config = tb_bangpai_jiu[type]
	if config == nil then return end
	--金钱不足
	if not player:SubItemByConfig(config.cost,MONEY_CHANGE_FACTION_SHANGXIANG,OPERTE_TYPE_FACTION,OPEATE_TYPE_FACTION_MONEY_ERR) then
		return
	end
	--加buff
	local lv = self:GetFactionGodLevel()
	player:CallScenedDoSomething(APPD_SCENED_FACTION_SHANGXIANG,type,tostring(lv))
	--获得基础绑元
	if config.yuanbao > 0 then
		local yuanbao = config.yuanbao * randIntD(2,4)
		player:AddMoney(MONEY_TYPE_BIND_GOLD, MONEY_CHANGE_FACTION_SHANGXIANG, yuanbao)
	end
	--增加帮贡
	local banggong = config.banggong * randIntD( math.floor((lv-1)/3+1) ,5)
	self:AddFactionMemberDayGongXian(pos,banggong)
	self:AddFactionMemberTotalGongXian(pos,banggong)
	--增加灵气
	self:AddFactionLingQi(config.add_lingqi)
	--增加事件
	self:AddEvent(player:GetGuid(), player:GetName(), FACTION_EVENT_TYPE_SHANGXIANG,type,banggong)
	--升级帮派神灵
	self:DoShenMingLevelUp(lv)
end

--升级帮派神灵
function FactionInfo:DoShenMingLevelUp(lv)
	local config = tb_bangpai_god[lv]
	if config == nil or lv >= #tb_bangpai_god then
		return
	end
	local lingqi = self:GetFactionLingQi()
	if lingqi >= config.num_lingqi then
		self:SetFactionGodLevel(lv + 1)
		self:SetFactionLingQi(0)
	end
end





---------------------------------------------------------帮派动态信息-------------------------------------------------------
--帮派动态信息个数加1
function FactionInfo:Next()
	local cur_num = self:GetAttackStrutCount()
	if(cur_num >= MAX_FACTION_EVENT_INFO_COUNT-1)then		--动态信息最多保留MAX_FACTION_EVENT_INFO_COUNT
		self:SetAttackStrutCount(0)	
	else
		self:AddUInt32(MAX_FACTION_EVENT_INFO_INT_NOW_INDEX, 1)
	end
end	

--帮派留言个数加1
function FactionInfo:NextNOtice()
	local cur_num = self:GetNoticeCount()
	if(cur_num >= MAX_FACTION_EVENT_NOTICE_COUNT-1)then		--动态信息最多保留MAX_FACTION_EVENT_INFO_COUNT
		self:SetNoticeCount(0)	
	else
		self:AddUInt32(MAX_FACTION_EVENT_INFO_INT_NOTICE_INDEX, 1)
	end
end	

--帮派动态信息int开始下标
function FactionInfo:Start()
	return FACTION_EVENT_INFO_INT_FIELD_START + self:GetAttackStrutCount() * MAX_FACTION_EVENT_INFO_INT
end

--帮派动态信息string开始下标
function FactionInfo:StringStart()
	return FACTION_EVENT_INFO_STR_FIELD_START + self:GetAttackStrutCount() * MAX_FACTION_EVENT_INFO_STRING 
end

--帮派留言int开始下标
function FactionInfo:StartNotice()
	return FACTION_EVENT_INFO_INT_NOTICE_START + self:GetNoticeCount() * MAX_FACTION_EVENT_INFO_INT
end

--帮派留言string开始下标
function FactionInfo:StringStartNotice()
	return FACTION_EVENT_INFO_STR_NOTICE_START + self:GetNoticeCount() * MAX_FACTION_EVENT_INFO_STRING 
end

--获得帮派动态信息当前索引
function FactionInfo:GetAttackStrutCount()
	return self:GetUInt32(MAX_FACTION_EVENT_INFO_INT_NOW_INDEX)
end

--设置帮派动态信息当前索引
function FactionInfo:SetAttackStrutCount (val)
	self:SetUInt32(MAX_FACTION_EVENT_INFO_INT_NOW_INDEX, val)
end

--获得帮派留言当前索引
function FactionInfo:GetNoticeCount()
	return self:GetUInt32(MAX_FACTION_EVENT_INFO_INT_NOTICE_INDEX)
end

--设置帮派留言当前索引
function FactionInfo:SetNoticeCount (val)
	self:SetUInt32(MAX_FACTION_EVENT_INFO_INT_NOTICE_INDEX, val)
end

--添加一个帮派动态信息
function FactionInfo:AddFactionInfo (corpid, playerid, playername, eventTm, eventType, reserve1, reserve2, reserve3, reserve_str)
	--str部分
	self:SetFactionGuid(eventType, corpid)
	self:SetPlayerGuid(eventType, playerid)
	self:SetPlayerName(eventType, playername)
	if(reserve_str == nil)then
		reserve_str = ''
	end
	self:SetStrReserve1(eventType, reserve_str)
	--int部分
	self:SetEventType(eventType)
	if(reserve1 == nil)then
		reserve1 = 0
	end
	self:SetIntReserve1(eventType, reserve1)
	if(reserve2 == nil)then
		reserve2 = 0
	end
	self:SetIntReserve2(eventType, reserve2)
	if(reserve3 == nil)then
		reserve3 = 0
	end
	self:SetIntReserve3(eventType, reserve3)	
	self:SetEventTm(eventType, eventTm)
	self:Next()		--下移索引
end

--设置时间戳
function FactionInfo:SetEventTm (eventtype, val)	
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_TIME, val)
end

--设置event类型
function FactionInfo:SetEventType (eventtype)
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_EVENT_TYPE, eventtype)
end

--设置保留1
function FactionInfo:SetIntReserve1 (eventtype, val)
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_RESERVE1, val)
end

--设置保留2
function FactionInfo:SetIntReserve2 (eventtype, val)
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_RESERVE2, val)
end

--设置保留3
function FactionInfo:SetIntReserve3 (eventtype, val)
	self:SetUInt32(self:Start() + FACTION_EVENT_INFO_INT_RESERVE3, val)
end

--设置帮派guid
function FactionInfo:SetFactionGuid (eventtype, val)
	self:SetStr(self:StringStart() + FACTION_EVENT_INFO_STRING_FACTION_GUID, val)
end

--设置player guid
function FactionInfo:SetPlayerGuid (eventtype, val)
	self:SetStr(self:StringStart() + FACTION_EVENT_INFO_STRING_PLAYER_GUID, val)
end

--设置玩家名字
function FactionInfo:SetPlayerName (eventtype, val)
	self:SetStr(self:StringStart() + FACTION_EVENT_INFO_STRING_PLAYER_NAME, val)
end

--设置str 保留1
function FactionInfo:SetStrReserve1 (eventtype, val)
	self:SetStr(self:StringStart() + FACTION_EVENT_INFO_STRING_RESERVE1, val)
end

--输出帮派动态信息
function FactionInfo:PrintFactionEvents (eventtype)
	for i = 0, MAX_FACTION_EVENT_INFO_COUNT-1 do
		if(self:GetStr(FACTION_EVENT_INFO_STR_FIELD_START + i * MAX_FACTION_EVENT_INFO_STRING + FACTION_EVENT_INFO_STRING_FACTION_GUID) ~= '')then
			outDebug('event time = '..self:GetUInt32(FACTION_EVENT_INFO_INT_FIELD_START + i * MAX_FACTION_EVENT_INFO_INT + FACTION_EVENT_INFO_INT_TIME))
			outDebug('event type = '..self:GetUInt32(FACTION_EVENT_INFO_INT_FIELD_START + i * MAX_FACTION_EVENT_INFO_INT + FACTION_EVENT_INFO_INT_EVENT_TYPE))
			outDebug('event corps guid = '..self:GetStr(FACTION_EVENT_INFO_STR_FIELD_START + i * MAX_FACTION_EVENT_INFO_STRING + FACTION_EVENT_INFO_STRING_FACTION_GUID))
			outDebug('event corps guid = '..self:GetStr(FACTION_EVENT_INFO_STR_FIELD_START + i * MAX_FACTION_EVENT_INFO_STRING + FACTION_EVENT_INFO_STRING_PLAYER_NAME))
			outDebug('event corps guid = '..self:GetStr(FACTION_EVENT_INFO_STR_FIELD_START + i * MAX_FACTION_EVENT_INFO_STRING + FACTION_EVENT_INFO_STRING_PLAYER_GUID))
		end
	end	
end


--帮派BOSS相关


--获取帮派令牌数
function FactionInfo:GetTokenNum()
	return self:GetUInt32(FACTION_INT_FIELD_TOKEN_NUM)
end

--设置帮派令牌数
function FactionInfo:SetTokenNum(val)
	self:SetUInt32(FACTION_INT_FIELD_TOKEN_NUM, val)
end

--获取帮派令牌积分进度
function FactionInfo:GetTokenPoints()
	return self:GetUInt32(FACTION_INT_FIELD_TOKEN_POINTS)
end

--设置帮派令牌积分进度
function FactionInfo:SetTokenPoints(val)
	self:SetUInt32(FACTION_INT_FIELD_TOKEN_POINTS, val)
end

--获取帮派令牌积分完成次数
function FactionInfo:GetTokenPointsCount()
	return self:GetUInt32(FACTION_INT_FIELD_TOKEN_POINTS_COUNT)
end

--设置帮派令牌积分完成次数
function FactionInfo:SetTokenPointsCount(val)
	self:SetUInt32(FACTION_INT_FIELD_TOKEN_POINTS_COUNT, val)
end


--获取帮派今日令牌购买次数
function FactionInfo:GetTokenBuyCount()
	return self:GetUInt32(FACTION_INT_FIELD_TOKEN_TODAY_BUY_COUNT)
end

--设置帮派今日令牌购买次数
function FactionInfo:SetTokenBuyCount(val)
	self:SetUInt32(FACTION_INT_FIELD_TOKEN_TODAY_BUY_COUNT, val)
end

--获取帮派已挑战最大boss id
function FactionInfo:GetChallengeBossIdMax()
	return self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_ID_MAX)
end

--设置帮派已挑战最大boss id
function FactionInfo:SetChallengeBossIdMax(val)
	self:SetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_ID_MAX, val)
end

--获取帮派当前挑战boss id
function FactionInfo:GetChallengeBossId()
	return self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_ID)
end

--设置帮派当前挑战boss id
function FactionInfo:SetChallengeBossId(val)
	self:SetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_ID, val)
end

--获取帮派当前挑战开始时间
function FactionInfo:GetChallengeBossStartTime()
	return self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_START_TIME)
end

--设置帮派当前挑战开始时间
function FactionInfo:SetChallengeBossStartTime(val)
	self:SetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_START_TIME, val)
end

--获取帮派当前boss血量万分比
function FactionInfo:GetChallengeBossHpRate()
	return self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_HP_RATE)
end

--设置帮派当前boss血量万分比
function FactionInfo:SetChallengeBossHpRate(val)
	self:SetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_HP_RATE, val)
end

--获取帮派当前boss pos
function FactionInfo:GetChallengeBossPos()
	return self:GetUInt16(FACTION_INT_FIELD_CHALLENGE_BOSS_POS,0),self:GetUInt16(FACTION_INT_FIELD_CHALLENGE_BOSS_POS,1)
end

--设置帮派当前boss pos
function FactionInfo:SetChallengeBossPos(x,y)
	self:SetUInt16(FACTION_INT_FIELD_CHALLENGE_BOSS_POS,0, x)
	self:SetUInt16(FACTION_INT_FIELD_CHALLENGE_BOSS_POS,1, y)

end

--获取帮派保护目标血量万分比
function FactionInfo:GetProtectTargetHpRate()
	return self:GetUInt32(FACTION_INT_FIELD_PROTECT_TARGET_HP_RATE)
end

--设置帮派保护目标血量万分比
function FactionInfo:SetProtectTargetHpRate(val)
	self:SetUInt32(FACTION_INT_FIELD_PROTECT_TARGET_HP_RATE, val)
end

--获取帮派当前boss pos
function FactionInfo:GetProtectTargetPos()
	return self:GetUInt16(FACTION_INT_FIELD_PROTECT_TARGET_POS,0),self:GetUInt16(FACTION_INT_FIELD_PROTECT_TARGET_POS,1)
end

--设置帮派当前boss pos
function FactionInfo:SetProtectTargetPos(x,y)
	self:SetUInt16(FACTION_INT_FIELD_PROTECT_TARGET_POS,0, x)
	self:SetUInt16(FACTION_INT_FIELD_PROTECT_TARGET_POS,1, y)

end

--获取当前boss输出榜 index位置 输出值和guid
function FactionInfo:GetChallengeBossDamageRankByIndex(index)
	local damage = self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_DAMAGE_RANK_START + index)
	local guid = self:GetStr(FACTION_STRING_FIELD_CHALLENGE_BOSS_DAMAGE_RANK_START + index)
	
	return damage,guid
end

--设置当前boss输出榜 index位置 输出值和guid
function FactionInfo:SetChallengeBossDamageRankByIndex(index,damage,guid)
	self:SetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_DAMAGE_RANK_START + index,damage)
	self:SetStr(FACTION_STRING_FIELD_CHALLENGE_BOSS_DAMAGE_RANK_START + index,guid)
	
end

--当前boss输出榜 增加guid 对应的输出值,guid不存在则添加记录
function FactionInfo:AddChallengeBossDamageRankDamage(damage,guid)
	local empty_pos = {}
	for i = 0 ,MAX_FACTION_MAMBER_COUNT -1  do
		local temp_damage,temp_guid = self:GetChallengeBossDamageRankByIndex(i)
		if guid == temp_guid then
			self:SetChallengeBossDamageRankByIndex(i,temp_damage + damage,guid)
			return
		end
		if temp_guid == '' then
			table.insert(empty_pos,i)
		end
	end
	if empty_pos[1] then
		self:SetChallengeBossDamageRankByIndex(empty_pos[1],damage,guid)
	end
end

--当前boss输出榜 根据guid 删除记录
function FactionInfo:RemoveChallengeBossDamageRankByGuid(guid)
	for i = 0 ,MAX_FACTION_MAMBER_COUNT -1  do
		local temp_damage,temp_guid = self:GetChallengeBossDamageRankByIndex(i)
		if guid == temp_guid then
			self:SetChallengeBossDamageRankByIndex(i,0,'')
			return
		end
	end
end

--清理当前输出榜
function FactionInfo:ClearChallengeBossDamageRank()
	for i = 0 ,MAX_FACTION_MAMBER_COUNT -1  do
		self:SetChallengeBossDamageRankByIndex(i,0,'')
	end
end


--当前boss输出榜 遍历返回递减的排行
function FactionInfo:GetChallengeBossDamageRankList()
	local list = {}
	for i = 0 ,MAX_FACTION_MAMBER_COUNT -1  do
		local temp_damage,temp_guid = self:GetChallengeBossDamageRankByIndex(i)
		if temp_guid ~= '' then
			table.insert(list,{guid = temp_guid,damage = temp_damage})
		end
	end
	table.sort(list,function (a,b)
		return a.damage > b.damage
	end)
	return list
end


--获取总输出榜 index位置 输出值和guid
function FactionInfo:GetChallengeBossTotalRankByIndex(index)
	local damage = self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_TOTAL_RANK_START + index)
	local guid = self:GetStr(FACTION_STRING_FIELD_CHALLENGE_BOSS_TOTAL_RANK_START + index)
	
	return damage,guid
end

--设置总输出榜 index位置 输出值和guid
function FactionInfo:SetChallengeBossTotalRankByIndex(index,damage,guid)
	self:SetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_TOTAL_RANK_START + index,damage)
	self:SetStr(FACTION_STRING_FIELD_CHALLENGE_BOSS_TOTAL_RANK_START + index,guid)
	
end

--总输出榜 增加guid 对应的输出值,guid不存在则添加记录
function FactionInfo:AddChallengeBossTotalRankDamage(damage,guid)
	local empty_pos = {}
	for i = 0 ,MAX_FACTION_MAMBER_COUNT -1  do
		local temp_damage,temp_guid = self:GetChallengeBossTotalRankByIndex(i)
		if guid == temp_guid then
			self:SetChallengeBossTotalRankByIndex(i,temp_damage + damage,guid)
			return
		end
		if guid == '' then
			table.insert(empty_pos,i)
		end
	end
	if empty_pos[1] then
		self:SetChallengeBossTotalRankByIndex(empty_pos[1],damage,guid)
	end
end

--总输出榜 根据guid 删除记录
function FactionInfo:RemoveChallengeBossTotalRankByGuid(guid)
	for i = 0 ,MAX_FACTION_MAMBER_COUNT -1  do
		local temp_damage,temp_guid = self:GetChallengeBossTotalRankByIndex(i)
		if guid == temp_guid then
			self:SetChallengeBossTotalRankByIndex(i,0,'')
			return
		end
	end
end


--客户端调用接口
--购买令牌
function FactionInfo:BuyToken(player,num)
	if num < 1 then
		return
	end
	local level = self:GetFactionLevel()
	local count = self:GetTokenBuyCount()
	local token_num = self:GetTokenNum()
	
	outFmtDebug('-----------------before buy token nums = %d, count = %d', token_num,count)

	if count >= tb_faction_base[level].token_max_buy or count + num > tb_faction_base[level].token_max_buy or token_num + num > tb_faction_base[level].token_max_keep then
		return  --不能购买次数
	end
	local costTable = {}
	local temp = {}
	for i = count + 1,count + num do
		local price = tb_faction_base[level].token_buy_price[i]
		if price then
			if temp[price[1]] == nil then
				temp[price[1]] = price[2]
			else
				temp[price[1]] = temp[price[1]] + price[2]
			end
		end
		
	end
	for id,price in pairs(temp) do
		table.insert(costTable,{id,price})
	end
	
	if not player:costMoneys(MONEY_CHANGE_TYPE_MALL_BUY, costTable, 1) then
		player:CallOptResult(OPERTE_TYPE_NPCBUY, NPC_BUY_MONEY_NO_ENOUGH)
		return
	end

	self:SetTokenBuyCount(count + num)
	self:SetTokenNum(token_num + num)
	
	outFmtDebug('------------------after buy token nums = %d, count = %d', self:GetTokenNum(),self:GetTokenBuyCount())
end

--进行召唤boss		修改当前bossid  场景服每秒检测
function FactionInfo:ChallengeBoss(player,boss_id)
	if self:GetChallengeBossId() > 0 then
		--已经处于挑战中了 无效包
		return
	end
	local config = tb_faction_boss[boss_id]
	if not config then
		return
	end
	
	local token_num = self:GetTokenNum()
	local level = self:GetFactionLevel()
	local id_max = self:GetChallengeBossIdMax()
	local pos = self:FindPlayerIndex(player:GetGuid())
	local identity = 0 
	if pos then
		identity = self:GetFactionMemberIdentity(pos)
	end
	local flag = false
	for i,j in pairs(config.faction_zhiwei_limit) do
		if identity == j then
			flag = true
		end
	end
	if not flag then
		--权限不足
		return
	end
	
	if level < config.faction_lv_limit then
		--家族等级不足
		return
	end
	
	if token_num < config.token_cost then
		--令牌不足
		return
	end
	
	if boss_id > id_max + 1 then
		--前置boss未挑战成功
		return
	end
	
	self:SetTokenNum(token_num - config.token_cost)
	self:SetChallengeBossStartTime(os.time())
	self:SetChallengeBossId(boss_id)
	self:SetChallengeBossHpRate(10000)
	self:SetChallengeBossPos(config.born_pos[1],config.born_pos[2])
	--保护目标
	self:SetProtectTargetHpRate(10000)
	self:SetProtectTargetPos(config.target_pos[1],config.target_pos[2])
	
	self:ClearChallengeBossDamageRank()
	
	outFmtDebug('-----------ChallengeBoss:bossid = %d', boss_id)
	
	--广播
	app:SendFactionNotice(self:GetGuid(), OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_BOSS_START, {tb_creature_template[config.entry].name})
	--app:SendFactionNotice(self:GetGuid(), 23, 3, {})
end


--					scened_internal_pack 添加 发包
--场景服调用接口 appdInsternalHanlders 添加 消息处理
--击杀野区 积分变化 (playerguid,point)
function FactionInfo:AddTokenPoints(player_guid,points)
	local currPoints = self:GetTokenPoints()
	local currPointsCount = self:GetTokenPointsCount()
	local tokenNum = self:GetTokenNum()
	local config = tb_faction_base[self:GetFactionLevel()]
	if currPoints + points < config.token_points then
		if currPointsCount >= config.token_daily then
			self:SetTokenPoints(0)
		elseif tokenNum >= config.token_max_keep then
			--令牌满了 不再加积分
		else
			self:SetTokenPoints(currPoints + points)
		end
		
	else
		local temp = currPoints + points
		local count = 0
		while( temp >= config.token_points) do
			if currPointsCount < config.token_daily and tokenNum < config.token_max_keep then
				count = count + 1
				temp = temp - config.token_points
			
			elseif currPointsCount + count >= config.token_daily then
				temp = 0
			elseif tokenNum + count >= config.token_max_keep then
				if count == 0 then
					temp = currPoints
				else
					temp = config.token_points - 1
				end
			end
		end

		self:SetTokenPoints(temp)
		self:SetTokenPointsCount(currPointsCount + count)
		self:SetTokenNum(tokenNum + count)
		
	end
	
end

--对boss造成伤害 (playerguid,damage)
function FactionInfo:BossDamaged(player_guid,damage)
	self:AddChallengeBossDamageRankDamage(damage,player_guid)
end
--同步boss hp pos (hp,x,y)
function FactionInfo:UpdateBossInfo(hp_rate,x,y)
	--outFmtDebug('=========================UpdateBossInfo')
	local boss_id = self:GetChallengeBossId()
	if boss_id <= 0 then
		return
	end
	self:SetChallengeBossHpRate(hp_rate)
	self:SetChallengeBossPos(x,y)
end
--同步target hp pos (hp,x,y)
function FactionInfo:UpdateTargetInfo(hp_rate,x,y)
	local boss_id = self:GetChallengeBossId()
	if boss_id <= 0 then
		return
	end
	self:SetProtectTargetHpRate(hp_rate)
	self:SetProtectTargetPos(x,y)
end
--挑战成功 ()
function FactionInfo:ChallengeBossWin()
	local boss_id = self:GetChallengeBossId()
	if boss_id <= 0 then
		return
	end
	local config = tb_faction_boss[boss_id]
	self:AddFactionMoney(config.win_money)
	
	--成员奖励
	local damage_rank = self:GetChallengeBossDamageRankList()
	for index,info in ipairs(damage_rank) do
		local player_guid = info.guid
		local damage = info.damage
		self:AddChallengeBossTotalRankDamage(damage,player_guid)
		local reward = nil
		local mail_name = ''
		local mail_desc = ''
		if tb_faction_boss_reward_list[boss_id] and tb_faction_boss_reward_list[boss_id][index] then
			reward = tb_faction_boss_reward_list[boss_id][index].reward
			mail_name = tb_faction_boss_reward_list[boss_id][index].mail_name
			mail_desc = tb_faction_boss_reward_list[boss_id][index].mail_desc
		else
			reward = nil
		end
		
		local player = app.objMgr:getObj(player_guid)
		
		if reward then
			--player:AppdAddItems(reward,MONEY_CHANGE_FACTION_BOSS,LOG_ITEM_OPER_TYPE_FACTION_BOSS)
			
			local rewardStr = ""
			for s,d in ipairs(reward) do
				if s > 1 then
					rewardStr = rewardStr .. ","
				end
				rewardStr = rewardStr..d[1]..","..d[2]
			end
			
			AddGiftPacksData(player_guid,0,GIFT_PACKS_TYPE_FACTION_BOSS,os.time(),os.time() + 86400*30, mail_name, mail_desc, rewardStr, SYSTEM_NAME)
		end
		
		if player then
			--smsg  胜利消息
			player:call_faction_boss_send_result(1,boss_id,config.win_money)
		end
		
	end
	
	
	local boss_id_max = self:GetChallengeBossIdMax()
	if boss_id > boss_id_max then
		self:SetChallengeBossIdMax(boss_id)
	end
	
	
	
	self:SetChallengeBossId(0)
	
end
--挑战失败 (type:1目标失守 / 2超时)
function FactionInfo:ChallengedBossFail(fail_type)
	--outFmtDebug('=========================ChallengedBossFail %d',fail_type)
	local boss_id = self:GetChallengeBossId()
	if boss_id <= 0 then
		return
	end
	local config = tb_faction_boss[boss_id]
	local lose_money = config.lose_money
	if self:GetFactionMoney() < lose_money then
		lose_money = self:GetFactionMoney()
		self:SetFactionMoney(0)
	else
		self:SetFactionMoney(self:GetFactionMoney() - config.lose_money )
	end
	
	--成员奖励
	local damage_rank = self:GetChallengeBossDamageRankList()
	for index,info in ipairs(damage_rank) do
		local player_guid = info.guid
		local damage = info.damage
		self:AddChallengeBossTotalRankDamage(damage,player_guid)
		local reward = nil
		local mail_name = ''
		local mail_desc = ''
		if tb_faction_boss_reward_list[boss_id] and tb_faction_boss_reward_list[boss_id][index] then
			reward = tb_faction_boss_reward_list[boss_id][index].fail_reward
			mail_name = tb_faction_boss_reward_list[boss_id][index].fail_mail_name
			mail_desc = tb_faction_boss_reward_list[boss_id][index].fail_mail_desc
		else
			reward = nil
		end
		local player = app.objMgr:getObj(player_guid)
		
		if reward then
			--player:AppdAddItems(reward,MONEY_CHANGE_FACTION_BOSS,LOG_ITEM_OPER_TYPE_FACTION_BOSS)
			
			local rewardStr = ""
			for s,d in ipairs(reward) do
				if s > 1 then
					rewardStr = rewardStr .. ","
				end
				rewardStr = rewardStr..d[1]..","..d[2]
			end
			
			AddGiftPacksData(player_guid,0,GIFT_PACKS_TYPE_FACTION_BOSS,os.time(),os.time() + 86400*30, mail_name, mail_desc, rewardStr, SYSTEM_NAME)
		end
		if player then
			--smsg  失败消息 type_fail, boss_id, money
			player:call_faction_boss_send_result(2,boss_id,lose_money)
		end
		
	end
	
	self:SetChallengeBossId(0)
	
end




-----------------------------------------家族宝库-----------------------------------------------
-- 获得当前宝库的上限
function FactionInfo:GetStoreHouseLimit()
	local house_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_STOREHOUSE)
	if house_lv <= 0 then
		return 0
	end
	local config = tb_faction_building[FACTION_BUILDING_TYPE_STOREHOUSE*100 + house_lv]
	if not config then
		return 0
	end
	return config.params[1]
end

function FactionInfo:GetStoreHouseCount()
	return self:GetUInt32(FACTION_INT_FIELD_STOREHOUSE_ITEM_COUNT)
end

function FactionInfo:AddStoreHouseCount()
	self:AddUInt32(FACTION_INT_FIELD_STOREHOUSE_ITEM_COUNT, 1)
end

function FactionInfo:SubStoreHouseCount()
	self:SubUInt32(FACTION_INT_FIELD_STOREHOUSE_ITEM_COUNT, 1)
end

function FactionInfo:GetStoreHouseRecordCursor()
	return self:GetUInt32(FACTION_INT_FIELD_STOREHOUSE_RECORD_CURSOR)
end

function FactionInfo:SetStoreHouseRecordCursor(val)
	self:SetUInt32(FACTION_INT_FIELD_STOREHOUSE_RECORD_CURSOR, val)
end

function FactionInfo:AddStoreHouseRecord(record)
	local cursor = self:GetStoreHouseRecordCursor()
	self:SetStr(cursor + FACTION_STRING_FIELD_STOREHOUSE_RECORD_START, record)
	cursor = cursor + 1
	if cursor >= MAX_FACTION_STOREHOUSE_RECORD_COUNT then
		cursor = 0
	end
	self:SetStoreHouseRecordCursor(cursor)
end

function FactionInfo:IsExeceeded(added)
	return self:GetStoreHouseCount() + added > self:GetStoreHouseLimit()
end

--[[
·玩家将道具放入仓库，显示：【玩家名称】将【装备名称】放入仓库					0|playerName|entry
·玩家将道具从仓库取出，显示：【玩家名称】将【道具名称】从仓库取出				1|playerName|entry
·系统将道具放入仓库，显示：系统奖励 【道具名称】								2|entry
·装备销毁，显示：【玩家名称】将【道具名称】销毁 获得家族资金【N】				3|playerName|entry|resource

--]]

function FactionInfo:AddSystemReward(rewards)
	local pattern = "0;%d;1;4;{};"
	for _, info in ipairs() do
		local entry = info[ 1 ]
		local count = info[ 2 ]
		local record = string.format("2|%d", entry)
		-- 是宝箱的才让加入
		if tb_box[entry] then
			local itemStr = string.format(pattern, entry)
			for i = 1, count do
				self:AddStoreHouseItem(itemStr)
				self:AddStoreHouseRecord(record)
			end
		end
	end
end

-- 上交
function FactionInfo:HandInReward(owner, pos_str)
	local values =  string.split(pos_str, "|")
	
	-- 宝库满了
	if self:IsExeceeded(#values) then
		return
	end
	
	local rewards = {}
	local itemMgr = owner:getItemMgr()
	for _, str in pairs(values) do
		local pos = tonumber(str)
		if pos then
			local item = itemMgr:getBagItemByPos(BAG_TYPE_EQUIP_BAG, pos)
			if item then
				local itemStr = item:toString()
				local entry = item:getEntry()

				if tb_item_template[entry].quality >= tb_faction_privilege[ 1 ].handInQuality then
					itemMgr:delItemObj(item, 1)
					self:AddStoreHouseItem(itemStr)
					
					-- 奖励整合				
					local reward = tb_item_template[entry].handInReward
					for _, info in ipairs(reward) do
						if not rewards[info[ 1 ]] then
							rewards[info[ 1 ]] = 0
						end
						rewards[info[ 1 ]] = rewards[info[ 1 ]] + info[ 2 ]
					end
					local record = string.format("0|%s|%d", owner:GetName(), entry)
					self:AddStoreHouseRecord(record)
				end
			end
		end
	end
	
	local rewardDict = {}
	for entry, count in pairs(rewards) do
		table.insert(rewardDict, {entry, count})
	end
	owner:AppdAddItems(rewardDict, MONEY_CHANGE_STOREHOUSE, LOG_ITEM_OPER_TYPE_STOREHOUSE)
end

-- 兑换装备
function FactionInfo:ExchangeSoreHouseItem(owner, pos)
	if pos < 0 or pos >= self:GetStoreHouseLimit() then
		return
	end
	
	local binlogIndx = pos + FACTION_STRING_FIELD_STOREHOUSE_START
	local info = self:GetStr(binlogIndx)
	if string.len(info) == 0 then
		return
	end
	
	local params = string.split(info, ';')
	local entry = tonumber(params[ 2 ])
	local costTable = tb_item_template[entry].exchangeCost
	
	-- 花费是否满足
	if not owner:checkMoneyEnoughs(costTable) then
		return
	end
	
	local itemMgr = owner:getItemMgr()
	-- 判断有没有空位
	if itemMgr:getEmptyCount(BAG_TYPE_EQUIP_BAG) == 0 then
		return
	end
	
	-- 看看增加是否成功
	if not itemMgr:addItemByStr(BAG_TYPE_EQUIP_BAG, info) then
		return
	end
	
	owner:costMoneys(MONEY_CHANGE_STOREHOUSE, costTable)
	self:DelStoreHouseItem(binlogIndx)
	
	local record = string.format("1|%s|%d", owner:GetName(), entry)
	self:AddStoreHouseRecord(record)
end

-- 销毁装备
function FactionInfo:DestroySoreHouseItem(owner, pos)
	if pos < 0 or pos >= self:GetStoreHouseLimit() then
		return
	end
	
	local binlogIndx = pos + FACTION_STRING_FIELD_STOREHOUSE_START
	local info = self:GetStr(binlogIndx)
	if string.len(info) == 0 then
		return
	end
	
	-- 判断有无权限
	local priv = tb_faction_privilege[ 1 ].destroyItem
	if not self:CheckEnoughPrivilege(owner:GetGuid(), priv) then
		return
	end
	
	-- 看看能加多少
	local params = string.split(info, ';')
	local entry = tonumber(params[ 2 ])
	local added = tb_item_template[entry].destroyReward
	self:AddFactionMoney(added)
	
	self:DelStoreHouseItem(binlogIndx)
	
	local record = string.format("3|%s|%d|%d", owner:GetName(), entry, added)
	self:AddStoreHouseRecord(record)
end

-- 增加宝库道具
function FactionInfo:AddStoreHouseItem(itemStr)
	for i = FACTION_STRING_FIELD_STOREHOUSE_START, FACTION_STRING_FIELD_STOREHOUSE_END-1 do
		local info = self:GetStr(i)
		if string.len(info) == 0 then
			self:SetStr(i, itemStr)
			self:AddStoreHouseCount()
			break
		end
	end
end

-- 删除宝库道具
function FactionInfo:DelStoreHouseItem(binlogIndx)
	if binlogIndx < FACTION_STRING_FIELD_STOREHOUSE_START or binlogIndx >= FACTION_STRING_FIELD_STOREHOUSE_END then
		return
	end
	
	local info = self:GetStr(binlogIndx)
	if string.len(info) > 0 then
		self:SetStr(binlogIndx, '')
		self:SubStoreHouseCount()
	end
end

-----------------------------------------家族宝库-----------------------------------------------

--家族贡献礼包兑换
function FactionInfo:FactionDonateGiftExchange(player,sum)
	if sum < 1 then
		return
	end
	local house_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_STOREHOUSE)
	if house_lv <= 0 then
		return
	end
	
	local build_info = tb_faction_building[FACTION_BUILDING_TYPE_STOREHOUSE*100+house_lv]
	if not build_info then
		return
	end
	local gift_max_count = build_info.params[2]
	local gift_item_id = build_info.params[3]
	local gift_item_donate_cost = build_info.params[4]
	
	if player:GetFactionDonateGiftExchangeDailyCount() > gift_max_count or player:GetFactionDonateGiftExchangeDailyCount()+ sum > gift_max_count then
		return
	end
	
	if player:costMoneys(MONEY_CHANGE_STOREHOUSE,{{MONEY_TYPE_FACTION,gift_item_donate_cost}},sum) then
		player:AppdAddItems({{gift_item_id,1}}, nil, nil, sum, nil, nil, 1)
		player:AddFactionDonateGiftExchangeDailyCount(sum)
	end
	return
end



-----------------------------------------家族建筑-----------------------------------------------
function FactionInfo:GetBuildingId(pos)
	return self:GetUInt32(FACTION_INT_FIELD_BUILDING_ID_START + pos)
end

function FactionInfo:SetBuildingId(pos,value)
	self:SetUInt32(FACTION_INT_FIELD_BUILDING_ID_START + pos,value)
end

function FactionInfo:GetBuildingLvUpId()
	return self:GetUInt32(FACTION_INT_FIELD_BUILDING_LVUP_ID)
end

function FactionInfo:SetBuildingLvUpId(value)
	self:SetUInt32(FACTION_INT_FIELD_BUILDING_LVUP_ID,value)
end

function FactionInfo:GetBuildingLvUpFinishTime()
	return self:GetUInt32(FACTION_INT_FIELD_BUILDING_LVUP_FINISH_TIME)
end

function FactionInfo:SetBuildingLvUpFinishTime(value)
	self:SetUInt32(FACTION_INT_FIELD_BUILDING_LVUP_FINISH_TIME,value)
end

function FactionInfo:SubBuildingLvUpFinishTime(value)
	self:SubUInt32(FACTION_INT_FIELD_BUILDING_LVUP_FINISH_TIME,value)
end

function FactionInfo:AddBuildingId(id)
	for pos = 0,MAX_FACTION_BUILDING_COUNT - 1  do
		if self:GetBuildingId(pos) == 0 then
			self:SetBuildingId(pos,id)
			return true
		end
	end
	return false
end

function FactionInfo:ReplaceBuildingId(from_id,to_id)
	for pos = 0,MAX_FACTION_BUILDING_COUNT - 1  do
		if self:GetBuildingId(pos) == from_id then
			self:SetBuildingId(pos,to_id)
			return true
		end
	end
	return false
end

function FactionInfo:GetBuildingLvByType(building_type)
	for pos = 0,MAX_FACTION_BUILDING_COUNT - 1  do
		local id = self:GetBuildingId(pos)
		if id - id % 100 == building_type * 100 then
			return id % 100
		end
	end
	return 0
	
end

function FactionInfo:GetBuildingPos(id)
	for pos = 0,MAX_FACTION_BUILDING_COUNT - 1  do
		if self:GetBuildingId(pos) == id then
			return pos
		end
	end
	return -1
	
end

function FactionInfo:GetBuildingMainHallType()
	return FACTION_BUILDING_TYPE_MAINHALL
end

function FactionInfo:GetDefaultBuildingMainHallId()
	return FACTION_BUILDING_TYPE_MAINHALL *100 + 1
end

function FactionInfo:GetBuildingMainHallLv()
	return self:GetBuildingLvByType(self:GetBuildingMainHallType())
end

function FactionInfo:PrintAllBuildings()
	for pos = 0,MAX_FACTION_BUILDING_COUNT - 1  do
		if self:GetBuildingId(pos) ~= 0 then
			outFmtDebug('PrintAllBuildings==========id: %d', self:GetBuildingId(pos))
		end
	end
end



--升级建筑
function FactionInfo:UpgradeBuilding(player, id)
	--self:PrintAllBuildings()
	--检测升级条件
	local building_info = tb_faction_building[id]
	if not building_info then
		return
	end
	--检测权限
	local baseconfig = tb_faction_building_base[1]
	local index = self:FindPlayerIndex(player:GetGuid())
	if not index then
		return
	end
	local player_zhiwei =  self:GetFactionMemberIdentity(index)
	local flag = false
	for _,zhiwei in pairs(baseconfig.zhiwei_limit) do
		if zhiwei == player_zhiwei then
			flag = true
		end
	end
	
	if not flag then
		return
	end
	
	if self:GetBuildingLvUpId() ~= 0 then
		return
	end
	
	local pos = self:GetBuildingPos(id)
	if pos == -1 then
		return
	end
	
	if building_info.can_lvup == 0 then
		return
	end
	
	for _,info in pairs(building_info.need_buildinglv) do
		if self:GetBuildingLvByType(info[1]) < info[2] then
			return
		end
	end
	
	--扣除资金
	if self:GetFactionMoney() < building_info.cost then
		outFmtDebug('=====================UpgradeBuilding money not enough!!')
		return
	end
	self:SetFactionMoney(self:GetFactionMoney() - building_info.cost)
	
	--设置升级id 结束时间
	self:SetBuildingLvUpId(id)
	self:SetBuildingLvUpFinishTime(os.time() + building_info.time * 60)
	
	outFmtDebug('==============================UpgradeBuilding buildid: %d',id)
	self:PrintAllBuildings()
	--家族广播
end



--加速升级建筑
function FactionInfo:UpgradeBuildingSpeedUp(player, count)
	--检测玩家剩余次数
	if count < 1 then
		return
	end
	local pos = self:FindPlayerIndex(player:GetGuid())
	if pos == nil then return end
	
	local curr_count = player:GetFactionBuildingSpeedUpDailyCount()
	
	--检测建筑可升级次数
	local baseconfig = tb_faction_building_base[1]
	
	if curr_count + count > baseconfig.speedup_limit then
		return
	end
	
	local max_count = math.ceil((self:GetBuildingLvUpFinishTime() - os.time())/(baseconfig.speedup_time*60))
	
	if max_count <= 0 or max_count <count then
		return
	end

	
	--检测消耗足够 扣除消耗
	if not player:costMoneys(MONEY_CHANGE_FACTION_DONATION,baseconfig.speedup_cost,count) then
		return
	end
	
	player:AddFactionBuildingSpeedUpDailyCount(count)
	
	--设置结束时间
	self:SubBuildingLvUpFinishTime(baseconfig.speedup_time*60*count)
	
	
	--发放奖励
	--self:AddFactionMemberDayGongXian(pos,baseconfig.speedup_donate * count)
	self:AddFactionMemberTotalGongXian(pos,baseconfig.speedup_donate * count)
	for index = curr_count + 1,curr_count + count do
		local reward_id = baseconfig.speedup_reward[index]
		if reward_id and reward_id ~= 0 then
			local reward = tb_faction_building_reward[reward_id]
			if #reward > 0 then
				player:AppdAddItems(reward, nil, nil, count, nil, nil, 1)
			end
		end
	end
	outFmtDebug('=====================================UpgradeBuildingSpeedUp guid: %s count: %d',player:GetGuid(),count)
	--添加家族日志
end



--每秒检测是否到达完成时间,处理建筑升级
function FactionInfo:UpdateBuildingProcess()
	
	--检测id 结束时间
	if self:GetBuildingLvUpFinishTime() > os.time() then
		return
	end
	
	local building_id = self:GetBuildingLvUpId()
	if building_id == 0 then
		return
	end
	--可完成 修改建筑列表对应建筑id  lv部分+1 
	local building_info = tb_faction_building[building_id]
	if not building_info or building_info.can_lvup == 0 then
		self:SetBuildingLvUpId(0)
		self:SetBuildingLvUpFinishTime(0)
	end
	
	self:ReplaceBuildingId(building_id,building_id + 1)
	
	
	local building_lv =  (building_id + 1) % 100
	local building_type = (building_id + 1 - building_lv)/100
	
	--主殿升级处理
	if building_type == self:GetBuildingMainHallType() then
		self:OnMainHallLvUp(building_lv)
		self:SetFactionLevel(building_lv)
		--self:RefreshShop()
		rankInsertTask(self:GetGuid(), RANK_TYPE_FACTION)
	end
	--家族技能建筑升级
	if building_type == FACTION_BUILDING_TYPE_SKILL then
		self:ResetAllMemberFactionSkill()
	end
	
	--家族商店升级
	if building_type == FACTION_BUILDING_TYPE_SHOP then
		self:RefreshShop()
	end
	
	--其他...
	
	----清理id = 0 time = 0
	self:SetBuildingLvUpId(0)
	self:SetBuildingLvUpFinishTime(0)
	
	outFmtDebug('============================OnUpdateBuildingProcess from_id: %d, to_id:%d',building_id,building_id+1)
	self:PrintAllBuildings()
	--家族通知
	
	
end


--主殿升级后解锁新建筑
function FactionInfo:OnMainHallLvUp(level)
	--遍历是否有新建筑解锁
	for id,info in pairsByKeys(tb_faction_building) do
		if info.unlock == level then
			self:AddBuildingId(id)
			
			--活动大厅解锁时处理
			if math.floor(id/100) == FACTION_BUILDING_TYPE_EVENT then
				self:ResetAllBossDenfense()
			end
			if math.floor(id/100) == FACTION_BUILDING_TYPE_SKILL then
				self:ResetAllMemberFactionSkill()
			end
			
			if math.floor(id/100) == FACTION_BUILDING_TYPE_SHOP then
				self:RefreshShop()
			end
		end
	end
	
	
end




-----------------------------------------家族建筑-----------------------------------------------

-----------------------------------------家族礼物-----------------------------------------------
--删除家族礼物功能 faction部分注释 faction_handle注释 faction_data部分保留 global_value部分注释 cow_appd部分注释 appd_context部分注释 sharedef定义保留
--[[
--礼物周榜   --guid ,周计数, 未处理礼物计数
function FactionInfo:SetGiftWeekPoint(index,value)
	self:SetUInt32(FACTION_INT_FIELD_GIFT_WEEK_POINT_START + index,value)
end

function FactionInfo:AddGiftWeekPoint(index,value)
	self:AddUInt32(FACTION_INT_FIELD_GIFT_WEEK_POINT_START + index,value)
end

function FactionInfo:GetGiftWeekPoint(index)
	return self:GetUInt32(FACTION_INT_FIELD_GIFT_WEEK_POINT_START + index)
end

--女王未处理礼物计数
function FactionInfo:SetGiftQueenUncheckCount(value)
	self:SetUInt32(FACTION_INT_FIELD_GIFT_QUEEN_UNCHECK_COUNT,value)
end

function FactionInfo:AddGiftQueenUncheckCount(value)
	self:AddUInt32(FACTION_INT_FIELD_GIFT_QUEEN_UNCHECK_COUNT,value)
end

function FactionInfo:SubGiftQueenUncheckCount(value)
	if self:GetGiftQueenUncheckCount() > value then
		self:SubUInt32(FACTION_INT_FIELD_GIFT_QUEEN_UNCHECK_COUNT,value)
	else
		self:SetGiftQueenUncheckCount(0)
	end
end

function FactionInfo:GetGiftQueenUncheckCount()
	return self:GetUInt32(FACTION_INT_FIELD_GIFT_QUEEN_UNCHECK_COUNT)
end

--未处理礼物计数
function FactionInfo:SetGiftUncheckCount(index,value)
	self:SetUInt32(FACTION_INT_FIELD_GIFT_UNCHECK_COUNT_START + index,value)
end

function FactionInfo:AddGiftUncheckCount(index,value)
	self:AddUInt32(FACTION_INT_FIELD_GIFT_UNCHECK_COUNT_START + index,value)
end

function FactionInfo:SubGiftUncheckCount(index,value)
	if self:GetGiftUncheckCount(index) > value then
		self:SubUInt32(FACTION_INT_FIELD_GIFT_UNCHECK_COUNT_START + index,value)
	else
		self:SetGiftUncheckCount(index,0)
	end
end

function FactionInfo:GetGiftUncheckCount(index)
	return self:GetUInt32(FACTION_INT_FIELD_GIFT_UNCHECK_COUNT_START + index)
end

--赠送礼物计数
function FactionInfo:SetGiftSendCount(index,value)
	self:SetUInt32(FACTION_INT_FIELD_GIFT_SEND_COUNT_START + index,value)
end

function FactionInfo:AddGiftSendCount(index,value)
	self:AddUInt32(FACTION_INT_FIELD_GIFT_SEND_COUNT_START + index,value)
end

function FactionInfo:SubGiftSendCount(index,value)
	if self:GetGiftSendCount(index) > value then
		self:SubUInt32(FACTION_INT_FIELD_GIFT_SEND_COUNT_START + index,value)
	else
		self:SetGiftSendCount(index,0)
	end
end

function FactionInfo:GetGiftSendCount(index)
	return self:GetUInt32(FACTION_INT_FIELD_GIFT_SEND_COUNT_START + index)
end

--guid
function FactionInfo:SetGiftPlayerGuid(index,value)
	self:SetStr(FACTION_STRING_FIELD_GIFT_PLAYER_GUID_START + index,value)
end

function FactionInfo:GetGiftPlayerGuid(index)
	return self:GetStr(FACTION_STRING_FIELD_GIFT_PLAYER_GUID_START + index)
end

--魅力值
function FactionInfo:SetFactionCharmPoint(value)
	self:SetDouble(FACTION_INT_FIELD_CHARM_POINT,value)
end

function FactionInfo:AddFactionCharmPoint(value)
	self:AddDouble(FACTION_INT_FIELD_CHARM_POINT,value)
end

function FactionInfo:GetFactionCharmPoint()
	return self:GetDouble(FACTION_INT_FIELD_CHARM_POINT)
end

--最后赠送时间
function FactionInfo:SetFactionGiftLastSendTime(value)
	self:SetDouble(FACTION_INT_FIELD_GIFT_LAST_SEND_TIME,value)
end

function FactionInfo:GetFactionGiftLastSendTime()
	return self:GetDouble(FACTION_INT_FIELD_GIFT_LAST_SEND_TIME)
end

--取得guid对应下标,不存在则创建
function FactionInfo:GetGiftPlayerGuidIndex(guid)
	for index = 0 , MAX_FACTION_MAMBER_COUNT -1 do
		if self:GetGiftPlayerGuid(index) == guid and string.len(self:GetGiftPlayerGuid(index)) ~= 0 then
			return index
		end
	end
	
	for index = 0 , MAX_FACTION_MAMBER_COUNT -1 do
		if string.len(self:GetGiftPlayerGuid(index)) == 0 then
			self:SetGiftPlayerGuid(index,guid)
			self:SetGiftWeekPoint(index,0)
			self:SetGiftUncheckCount(index,0)
			self:SetGiftSendCount(index,0)
			return index
		end
	end
	
	return nil
end

function FactionInfo:GetWeekGuardInfo()
	local guard_point = 0
	local guard_guid = ""
	local guard_name = ""
	local queen_guid = self:GetBangZhuGuid()
	for index = 0 , MAX_FACTION_MAMBER_COUNT -1 do
		local point = self:GetGiftWeekPoint(index)
		local guid = self:GetGiftPlayerGuid(index)
		if point > guard_point and guid ~= queen_guid then
			guard_point = point
			guard_guid = guid
		end
	end

	local Faction_Pos = self:FindPlayerIndex(guard_guid)
	if Faction_Pos == nil then
		return "","",""
	end		
	
	return guard_guid,self:GetFactionMemberName(Faction_Pos),self:GetFactionMemberVipLevel(Faction_Pos)
end

--清理周榜
function FactionInfo:ClearGiftWeekPoint()
	for index = 0 , MAX_FACTION_MAMBER_COUNT -1 do
		self:SetGiftWeekPoint(index,0)
		self:SetGiftPlayerGuid(index,0)
	end
	
end

--清理每日计数
function FactionInfo:ClearGiftDayCount()
	for index = 0 , MAX_FACTION_MAMBER_COUNT -1 do
		self:SetGiftUncheckCount(index,0)
		self:SetGiftSendCount(index,0)
	end
	self:SetGiftQueenUncheckCount(0)
end

--删除记录
function FactionInfo:DelGiftInfo(guid)
	for index = 0 , MAX_FACTION_MAMBER_COUNT -1 do
		if self:GetGiftPlayerGuid(index) == guid and string.len(self:GetGiftPlayerGuid(index)) ~= 0 then
			local data_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionData)
			local factionData = app.objMgr:getObj(data_guid)
			if factionData then
				local unthank_count = factionData:DelGiftByGuid(guid)
				if unthank_count > 0 then
					self:SubGiftQueenUncheckCount(unthank_count)
				end
			end
			self:SetGiftPlayerGuid(index,"")
			self:SetGiftWeekPoint(index,0)
			self:SetGiftUncheckCount(index,0)
		end
	end
	
end

function FactionInfo:DailyResetGift()
	local data_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionData)
	local factionData = app.objMgr:getObj(data_guid)
	if factionData then
		factionData:DailyClearAllGift()
	end
	
	self:ClearGiftDayCount()
	
end

--factionData相关

--赠送礼物 (item_table)
function FactionInfo:SendFactionGift(factionData,player,item_table,msg,msg_type)
	local index = self:GetGiftPlayerGuidIndex(player:GetGuid())
	if not index  then 
		outFmtError("get index GetGiftPlayerGuidIndex fail")
		return
	end
	
	if self:GetGiftSendCount(index) >= MAX_FACTION_DATA_GIFT_COUNT then
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_GIFT_SEND_FAIL)
		outFmtError("SendFactionGift can not send more than 150")
		return
	end
	
	
	--检测物品数量
	if not player:hasMulItem(item_table) then
		outFmtError("item not enough")
		return
	end
	
	--扣除物品
	if #item_table > 0 then
		if factionData:CheckCanSendGift(player:GetGuid()) then
			if not player:useMulItem(item_table) then
				outFmtError("use item fail")
				return
			end
		end
	else
		outFmtError("no item to send")
		return
	end
	
	--计算魅力值 奖励
	local point = 0
	local reward_table = {}
	local faction_money = 0
	for _,v in pairs(item_table) do
		if tb_faction_gift[v[1] ] then
			point = point + tb_faction_gift[v[1] ].point * v[2]
			faction_money = faction_money +tb_faction_gift[v[1] ].money * v[2]
			for _,reward_info in pairs(tb_faction_gift[v[1] ].reward) do
				table.insert(reward_table, {reward_info[1],reward_info[2]*v[2]})
				
			end
		end
		
		-- 贡献道具个数
		local questMgr = player:getQuestMgr()
		questMgr:OnUpdate(QUEST_TARGET_TYPE_CONTRIBUTE_ITEM, {v[ 1 ], v[ 2 ]})
	end
	
	local count_id = player:GetPlayerGiftCountID() + 1 --从1开始
	--添加gift信息
	msg = player:GetGuid() ..SPLIT_THING_DETAIL.. msg ..SPLIT_THING_DETAIL.. msg_type ..SPLIT_THING_DETAIL.. os.time()
	
	local id,item_list = factionData:AddGift(player:GetGuid(),point,item_table,msg,count_id)
	if not id then
		outFmtError("add gift fail")
		return
	end
	
	--player添加送礼记录(id,point,item_list,count_id)
	player:AddGiftInfo(id,point,item_list,count_id,msg)
	player:AddPlayerGiftCountID(1) -- 计数+1
	
	player:AddFactionGiftSendCount(1)
	player:AddFactionGiftPointCount(point)
	
	--修改统计数值 女王 家族 魅力值 周榜 未感谢计数
	local index_sender = self:GetGiftPlayerGuidIndex(player:GetGuid())
	local index_queen = self:GetGiftPlayerGuidIndex(self:GetBangZhuGuid())
	if not index_sender  then 
		outFmtError("get index_sender GetGiftPlayerGuidIndex fail")
		return
	end
	
	if not index_queen  then 
		outFmtError("get index_queen GetGiftPlayerGuidIndex fail")
		return
	end
	
	self:AddFactionCharmPoint(point)
	local pos = self:FindPlayerIndex(self:GetBangZhuGuid())
	self:SetFactionMemberCharm(pos,self:GetFactionCharmPoint())
	self:SetFactionGiftLastSendTime(os.time())
	local queen = app.objMgr:getObj(self:GetBangZhuGuid())
	if queen then
		queen:SetPlayerCharmPoint(self:GetFactionCharmPoint())
	end
	outFmtDebug("========SendFactionGift CharmPoint: %d",self:GetFactionCharmPoint())
	--rankInsertTask(self:GetGuid(), RANK_TYPE_CHARM)
	--self:AddGiftUncheckCount(index_queen,1)
	self:AddGiftQueenUncheckCount(1)
	outFmtDebug("=============count: %d",self:GetGiftQueenUncheckCount())
	self:AddGiftWeekPoint(index_sender,point)
	self:AddGiftSendCount(index_sender,1)
	
	local pos = self:FindPlayerIndex(player:GetGuid())
	self:AddFactionMemberTotalSendCharm(pos,point)
	
	--发送奖励
	player:AppdAddItems(reward_table,MONEY_CHANGE_FACTION_GIFT,LOG_ITEM_OPER_TYPE_FACTION_GIFT)
	
	self:AddFactionMoney(faction_money)
	
	if player:GetGuid() == self:GetBangZhuGuid() then
		self:ThankAndReplyFactionGift(factionData,player,id,FACTION_DATA_REPLY_TYPE_TEXT,"")
		player:GetFactionGiftExreward(count_id)
	end
	
	outFmtInfo("============SendFactionGift finish ")
	
	--返回确认消息
	player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_GIFT_SEND)
	
	
	--跑马灯
	for _,v in pairs(item_table) do
		if tb_faction_gift[v[1] ] then
			if tb_faction_gift[v[1] ].notice_type == 1 then
				player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_GIFT_NOTICE,{player:GetName(),self:GetBangZhuName(),tb_item_template[v[1] ].name,v[2]})
			end
		end
	end
	
	--更新排行
	local queen_vip = self:GetFactionMemberVipLevel( self:FindPlayerIndex(self:GetBangZhuGuid()))
	local guard_guid,guard_name,guard_vip = self:GetWeekGuardInfo()
	local prev_rank, new_rank = UpdateFactionRank(self:GetFactionCharmPoint(),self:GetBangZhuGuid(),self:GetBangZhuName(),self:GetGuid(),self:GetName(),self:GetFactionCurFlagId(),os.time(),guard_guid,guard_name,queen_vip,guard_vip)
	
	if new_rank ~= 0 and (new_rank < prev_rank or prev_rank == 0) then
		--帮派广播排行变化
		local stru = faction_gift_rank_info_t .new()
		stru.rank = new_rank
		stru.point = self:GetFactionCharmPoint()
		stru.queen_name =  self:GetBangZhuName()
		stru.faction_name = self:GetName()
		stru.guard_name = guard_name
		stru.faction_flag = self:GetFactionCurFlagId()
		stru.queen_vip = queen_vip
		stru.guard_vip = guard_vip
		for pos = 0, MAX_FACTION_APLLY_MAMBER_COUNT -1 do
			local guid = self:GetFactionMemberGuid(pos)
			if guid ~= '' then
				local player = app.objMgr:getObj(guid)
				if player then
					player:call_show_faction_gift_rank_change(prev_rank,new_rank,stru)
				end
			
			end
		end
	end
	
	--printAllFactionRank()
end

function FactionInfo:SendGiftRankPacket(player,rank_table,op_type,page)
	local output = {}
	if op_type == 1 then
		output = protocols.pack_show_faction_gift_page ()
	elseif op_type == 2 then
		output = protocols.pack_show_faction_gift_info ()
	elseif op_type == 3 then
		output = protocols.pack_show_faction_gift_unthank_page ()
	elseif op_type == 4 then
		output = protocols.pack_show_faction_gift_history_page ()
	end
	local size = #rank_table
	output:writeU16(page)
	output:writeU16(size)
	for _,info in ipairs(rank_table) do
		output:writeU32(info.rank)
		output:writeU32(info.id)
		output:writeU32(info.point)
		output:writeU32(info.thank)
		output:writeU32(info.reply)
		output:writeU32(info.reward)
		output:writeU32(info.time)
		output:writeU32(info.count_id)
		output:writeUTF(info.guid)
		output:writeUTF(info.msg)
		output:writeUTF(info.item_list)
		output:writeUTF(info.reply_list)
		
	end
	player:SendPacket(output)
	output:delete()
	
	--测试
	--printAllRankTable(rank_table)
end

--查看礼物列表
function FactionInfo:ShowFactionGiftPage(factionData,player,page)
	--页数转换为名次范围
	if page < 1 or page > MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT / tb_faction_base[self:GetFactionLevel()].gift_view_rows then
		outFmtError("find ShowFactionGiftPage page: %d out of range",page)
		return
	end
	
	local rows = tb_faction_base[self:GetFactionLevel()].gift_view_rows
	local begin = page * rows - rows + 1
	local tail = page * rows
	--查询排名信息 
	local rank_table = factionData:GetGiftRankList(begin,tail)
	
	--打包发回客户端 smsg 1
	self:SendGiftRankPacket(player,rank_table,1,page)
end

--查看礼物信息
function FactionInfo:ShowFactionGiftInfo(factionData,player,id)
	--查询礼包信息
	if id < 0 or id > MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT - 1 then
		outFmtError("find ShowFactionGiftInfo id: %d out of range",id)
		return
	end
	
	local rank_info = factionData:GetGiftInfoByID(id)
	if not rank_info then
		outFmtError("find ShowFactionGiftInfo GetGiftInfoByID: %d fail",id)
		return
	end
	--打包发回客户端 smsg 2
	self:SendGiftRankPacket(player,{rank_info},2,1)
end

--女王查看未感谢礼物
function FactionInfo:ShowFactionGiftUnthankPage(factionData,player,page)
	--页数转换为名次范围
	
	if page < 1 or page > MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT / tb_faction_base[self:GetFactionLevel()].gift_view_rows then
		outFmtError("find ShowFactionGiftUnthankPage page: %d out of range",page)
		return
	end
	
	local rows = tb_faction_base[self:GetFactionLevel()].gift_view_rows
	local begin = page * rows - rows + 1
	local tail = page * rows
	--查询排名信息 GetGiftRankListUnthank(begin,tail)
	local rank_table = factionData:GetGiftRankListUnthank(begin,tail)
	
	
	--打包发回客户端 smsg 3
	
	self:SendGiftRankPacket(player,rank_table,3,page)
end

--女王查看历史记录
function FactionInfo:ShowFactionGiftHistoryPage(factionData,player,page)
	--页数转换为名次范围
	if page < 1 or page > MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT / tb_faction_base[self:GetFactionLevel()].gift_view_rows then
		outFmtError("find ShowFactionGiftHistoryPage page: %d out of range",page)
		return
	end
	
	local rows = tb_faction_base[self:GetFactionLevel()].gift_view_rows
	local begin = page * rows - rows + 1
	local tail = page * rows
	--查询排名信息 GetGiftRankListHistory(begin,tail)
	local rank_table = factionData:GetGiftRankListHistory(begin,tail)
	
	--打包发回客户端 smsg 4
	
	self:SendGiftRankPacket(player,rank_table,4,page)
	
end

--女王感谢
function FactionInfo:ThankFactionGift(factionData,player,id,refresh_page )
	--获取礼物信息
	if id < 0 or id > MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT - 1 then
		outFmtError("find ThankFactionGift id: %d out of range",id)
		return
	end
	local rank_info = factionData:GetGiftInfoByID(id)
	if not rank_info then
		--outFmtError("find ThankFactionGift GetGiftInfoByID: %d fail",id)
		return
	end
	
	--检测能否感谢  是否必须回复
	if rank_info.point >= tb_faction_base[self:GetFactionLevel()].gift_points_must_reply then
		outFmtInfo("ThankFactionGift high point must reply")
		return
	end
	if rank_info.thank == 1 then
		outFmtInfo("ThankFactionGift gift already thank")
		return
	end
	
	--修改礼物信息 修改thank
	if not factionData:ChangeGiftFlagThank(id) then
		outFmtError("find ThankFactionGift ChangeGiftFlagThank: %d fail",id)
		return
	end
	
	local sender = app.objMgr:getObj(rank_info.guid)
	if sender then
		local index = sender:GetGiftIndexByCountID(rank_info.count_id)
		if index then
			
			sender:SetGiftReplyList(index,"")
			sender:SetGiftFlagThank(index,1)
		end
		
	else
		factionData:AddGiftCache(id,"")
	end
	
	--修改统计数值 未感谢计数-1 玩家未领取计数+1
	local index_sender = self:GetGiftPlayerGuidIndex(rank_info.guid)
	local index_queen = self:GetGiftPlayerGuidIndex(self:GetBangZhuGuid())
	if not index_sender  then 
		outFmtError("get index_sender GetGiftPlayerGuidIndex fail")
		return
	end
	
	if not index_queen  then 
		outFmtError("get index_queen GetGiftPlayerGuidIndex fail")
		return
	end
	
	--self:SubGiftUncheckCount(index_queen,1)
	self:SubGiftQueenUncheckCount(1)
	outFmtDebug("=============count: %d",self:GetGiftQueenUncheckCount())
	self:AddGiftUncheckCount(index_sender,1)
	
	outFmtInfo("============ThankFactionGift finish ")
	if refresh_page then
		--添加提示
		
		player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_GIFT_THANK)
		self:ShowFactionGiftUnthankPage(factionData,player,1)
	end
end

--女王全部感谢
function FactionInfo:ThankAllFactionGift(factionData,player)
	for id=0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		self:ThankFactionGift(factionData,player,id,false)
	end
	
	outFmtInfo("============ThankAllFactionGift finish ")
	--添加提示
	player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_GIFT_THANK_ALL)
	self:ShowFactionGiftUnthankPage(factionData,player,1)
end

--女王全部感谢并回复空
function FactionInfo:ThankAndReplyAllFactionGift(factionData,player)
	for id=0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		self:ThankAndReplyFactionGift(factionData,player,id,FACTION_DATA_REPLY_TYPE_TEXT,"")
	end
	
	outFmtInfo("============ThankAndReplyAllFactionGift finish ")
	--添加提示
	--player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_GIFT_THANK_ALL)
	--self:ShowFactionGiftUnthankPage(factionData,player,1)
end

--女王感谢并回复
function FactionInfo:ThankAndReplyFactionGift(factionData,player,id,reply_type,reply)
	--获取礼物信息
	if id < 0 or id > MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT - 1 then
		outFmtError("find ThankAndReplyFactionGift id: %d out of range",id)
		return
	end
	
	local rank_info = factionData:GetGiftInfoByID(id)
	if not rank_info then
		--outFmtError("find ThankFactionGift GetGiftInfoByID: %d fail",id)
		return
	end
	--检测能否感谢
	if rank_info.thank == 1 then
		outFmtInfo("ThankFactionGift gift already thank")
		return
	end
	--修改礼物信息  添加回复  修改thank  修改标识reply
	if not factionData:ChangeGiftFlagThank(id) then
		outFmtError("find ThankFactionGift ChangeGiftFlagThank: %d fail",id)
		return
	end
	
	
	if rank_info.guid == self:GetBangZhuGuid() then --女王自己礼物处理
		local index = player:GetGiftIndexByCountID(rank_info.count_id)
		if index then
			player:SetGiftReplyList(index,"")
			player:SetGiftFlagThank(index,1)
		end
		self:SubGiftQueenUncheckCount(1)
		outFmtDebug("=============count: %d",self:GetGiftQueenUncheckCount())
		return
	end
	
	if factionData:AddReply(id,player:GetGuid(),reply_type,reply) then
		outFmtInfo("find ThankFactionGift AddReply: %d success",id)
		if not factionData:ChangeGiftFlagReply(id) then
			outFmtError("find ThankFactionGift ChangeGiftFlagThank: %d fail",id)
			return
		end
	end
	local msg = player:GetGuid() ..SPLIT_THING_DETAIL.. reply ..SPLIT_THING_DETAIL.. reply_type ..SPLIT_THING_DETAIL.. os.time()
	
	local sender = app.objMgr:getObj(rank_info.guid)
	if sender then
		local index = sender:GetGiftIndexByCountID(rank_info.count_id)
		if index then
			sender:SetGiftReplyList(index,msg)
			sender:SetGiftFlagThank(index,1)
		end
	else
		factionData:AddGiftCache(id,msg)
	end
	
	
	--修改统计数值 未感谢计数-1 玩家未领取计数+1
	local index_sender = self:GetGiftPlayerGuidIndex(rank_info.guid)
	local index_queen = self:GetGiftPlayerGuidIndex(self:GetBangZhuGuid())
	if not index_sender  then 
		outFmtError("get index_sender GetGiftPlayerGuidIndex fail")
		return
	end
	
	if not index_queen  then 
		outFmtError("get index_queen GetGiftPlayerGuidIndex fail")
		return
	end
	
	--self:SubGiftUncheckCount(index_queen,1)
	self:SubGiftQueenUncheckCount(1)
	outFmtDebug("=============count: %d",self:GetGiftQueenUncheckCount())
	self:AddGiftUncheckCount(index_sender,1)
	
	outFmtInfo("============ThankAndReplyFactionGift finish ")
	
	
	player:CallOptResult(OPERTE_TYPE_FACTION, OPERTE_TYPE_FACTION_GIFT_THANK)
	
	self:ShowFactionGiftUnthankPage(factionData,player,1)
end

--进行回复
function FactionInfo:ReplyFactionGift(factionData,player,id,reply_type,reply)
	--获取礼物信息
	if id < 0 or id > MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT - 1 then
		outFmtError("find ReplyFactionGift id: %d out of range",id)
		return
	end
	
	local rank_info = factionData:GetGiftInfoByID(id)
	if not rank_info then
		outFmtError("find ReplyFactionGift GetGiftInfoByID: %d fail",id)
		return
	end
	
	--修改礼物信息 添加回复 女王回复额外修改标识reply
	if factionData:AddReply(id,player:GetGuid(),reply_type,reply) then
		outFmtInfo("find ReplyFactionGift AddReply: %d success",id)
		if player:GetGuid() == self:GetBangZhuGuid() then
			if not factionData:ChangeGiftFlagReply(id) then
				outFmtError("find ThankFactionGift ChangeGiftFlagThank: %d fail",id)
				return
			end
		end
	end
	
	--返回回复结果 
	self:ShowFactionGiftInfo(factionData,player,id)
end


--查看女王感谢自己的记录
function FactionInfo:ShowFactionGiftThankPage(factionData,player,page)
	--页数转换为名次范围
	local rows = tb_faction_base[self:GetFactionLevel()].gift_view_rows
	local begin = page * rows - rows + 1
	local tail = page * rows
	--查询排名信息 GetGiftRankListThank(guid,begin,tail)
	local rank_table = factionData:GetGiftRankListThank(player:GetGuid(),begin,tail)
	
	--打包发回客户端 smsg 5
	添加smsg 5
end

--查看自己所有赠送记录
function FactionInfo:ShowFactionGiftSendPage(factionData,player,page)
	--页数转换为名次范围
	local rows = tb_faction_base[self:GetFactionLevel()].gift_view_rows
	local begin = page * rows - rows + 1
	local tail = page * rows
	--查询排名信息 GetGiftRankListSelf(guid,begin,tail)
	local rank_table = factionData:GetGiftRankListSelf(player:GetGuid(),begin,tail)
	
	--打包发回客户端 smsg 6
	添加smsg 6
end

--领取额外奖励
function FactionInfo:GetFactionGiftExreward(factionData,player,id)
	--获取礼物信息
	local rank_info = factionData:GetGiftInfoByID(id)
	if not rank_info then
		outFmtError("find GetFactionGiftExreward GetGiftInfoByID: %d fail",id)
		return
	end
	
	--检查领取标识 是否是自己的
	if rank_info.guid ~= player:GetGuid() then
		outFmtError("find GetFactionGiftExreward gift guid not same")
		return
	end
	
	if rank_info.thank == 0 then
		outFmtError("find GetFactionGiftExreward gift not thank!!")
		return
	end
	
	if rank_info.reward == 1 then
		outFmtError("find GetFactionGiftExreward gift reward already get")
		return
	end
	
	--修改礼包奖励标识
	if not factionData:ChangeGiftFlagReward(id) then
		outFmtError("find GetFactionGiftExreward ChangeGiftFlagReward: %d fail",id)
		return
	end
	
	--取得道具列表 获得奖励结果
	local item_table = factionData:GetGiftItemListTable(id)
	
	local reward_table = {}
	for _,v in pairs(item_table) do
		if tb_faction_gift[v[1] ] then
			point = point + tb_faction_gift[v[1] ].point * v[2]
			for _,reward_info in pairs(tb_faction_gift[v[1] ].ex_reward) do
				table.insert(reward_table, {reward_info[1],reward_info[2]*v[2]})
			end
		end
	end
	
	
	--发送奖励
	player:AppdAddItems(reward_table,MONEY_CHANGE_FACTION_GIFT,LOG_ITEM_OPER_TYPE_FACTION_GIFT)
	
	--修改统计数值  未领取数量
	local index_sender = self:GetGiftPlayerGuidIndex(rank_info.guid)
	if not index_sender  then 
		outFmtError("get index_sender GetGiftPlayerGuidIndex fail")
		return
	end
	
	self:SubGiftUncheckCount(index_sender,1)
	
	outFmtInfo("============GetFactionGiftExreward finish ")
end

--领取所有额外奖励
function FactionInfo:GetAllFactionGiftExreward(factionData,player)
	for id=0, MAX_FACTION_DATA_GIFT_COUNT * MAX_FACTION_MAMBER_COUNT-1 do
		self:GetFactionGiftExreward(factionData,player,id)
	end
	
	outFmtInfo("============GetAllFactionGiftExreward finish ")
	
end


--帮助方法 打印列表
function printAllRankTable(rank_table)
	outFmtInfo("=========================================================")
	
	for _,info in ipairs(rank_table) do
		outFmtInfo("---------------------------------------------")
		outFmtInfo("rank: %d ",info.rank)
		outFmtInfo("id: %d ",info.id)
		outFmtInfo("point: %d ",info.point)
		outFmtInfo("thank: %d ",info.thank)
		outFmtInfo("reply: %d ",info.reply)
		outFmtInfo("reward: %d ",info.reward)
		outFmtInfo("time: %d ",info.time)
		outFmtInfo("count_id: %d ",info.count_id)
		if info.guid then
			outFmtInfo("guid: %s ",info.guid)
		end
		outFmtInfo("msg: %s ",info.msg)
		outFmtInfo("item_list: %s ",info.item_list)
		outFmtInfo("reply_list: %s ",info.reply_list)
		outFmtInfo("---------------------------------------------")
	end
	
	outFmtInfo("=========================================================")

end
--]]
-----------------------------------------家族礼物-----------------------------------------------

-----------------------------------------家族首领挑战-----------------------------------------------
--怪物池id
function FactionInfo:GetBossDenfensePoolId(index)
	return self:GetUInt32(FACTION_INT_FIELD_BOSSDENFENSE_START + index * MAX_FACTION_BOSSDEFENSE + FACTION_INT_BOSSDEFENSE_POOL_ID)
end

function FactionInfo:SetBossDenfensePoolId(index,value)
	self:SetUInt32(FACTION_INT_FIELD_BOSSDENFENSE_START + index * MAX_FACTION_BOSSDEFENSE + FACTION_INT_BOSSDEFENSE_POOL_ID,value)
end

--怪物当前Hp
function FactionInfo:GetBossDenfenseHp(index)
	return self:GetUInt32(FACTION_INT_FIELD_BOSSDENFENSE_START + index * MAX_FACTION_BOSSDEFENSE + FACTION_INT_BOSSDEFENSE_HP)
end

function FactionInfo:SetBossDenfenseHp(index,value)
	self:SetUInt32(FACTION_INT_FIELD_BOSSDENFENSE_START + index * MAX_FACTION_BOSSDEFENSE + FACTION_INT_BOSSDEFENSE_HP,value)
end

--怪物最大Hp
function FactionInfo:GetBossDenfenseMaxHp(index)
	return self:GetUInt32(FACTION_INT_FIELD_BOSSDENFENSE_START + index * MAX_FACTION_BOSSDEFENSE + FACTION_INT_BOSSDEFENSE_MAX_HP)
end

function FactionInfo:SetBossDenfenseMaxHp(index,value)
	self:SetUInt32(FACTION_INT_FIELD_BOSSDENFENSE_START + index * MAX_FACTION_BOSSDEFENSE + FACTION_INT_BOSSDEFENSE_MAX_HP,value)
end

--怪物状态
function FactionInfo:GetBossDenfenseStatus(index)
	return self:GetUInt32(FACTION_INT_FIELD_BOSSDENFENSE_START + index * MAX_FACTION_BOSSDEFENSE + FACTION_INT_BOSSDEFENSE_STATUS)
end

function FactionInfo:SetBossDenfenseStatus(index,value)
	self:SetUInt32(FACTION_INT_FIELD_BOSSDENFENSE_START + index * MAX_FACTION_BOSSDEFENSE + FACTION_INT_BOSSDEFENSE_STATUS,value)
end

--怪物输出记录
function FactionInfo:GetBossDenfenseMemberDamage(factionData,index,member_index)
	return factionData:GetUInt32(FACTION_DATA_INT_FIELD_BOSSDEFENSE_DAMAGE_START + index * MAX_FACTION_MAMBER_COUNT + member_index)
end

function FactionInfo:SetBossDenfenseMemberDamage(factionData,index,member_index,value)
	factionData:SetUInt32(FACTION_DATA_INT_FIELD_BOSSDEFENSE_DAMAGE_START + index * MAX_FACTION_MAMBER_COUNT + member_index,value)
end

function FactionInfo:AddBossDenfenseMemberDamage(factionData,index,member_index,value)
	factionData:AddUInt32(FACTION_DATA_INT_FIELD_BOSSDEFENSE_DAMAGE_START + index * MAX_FACTION_MAMBER_COUNT + member_index,value)
end

--local data_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionData)
--local factionData = app.objMgr:getObj(data_guid)

--重置
function FactionInfo:ResetAllBossDenfense()
	local building_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_EVENT)
	if building_lv == 0 then
		outFmtDebug("ResetAllBossDenfense building lv is 0")
		return
	end
	local base_config = tb_faction_bossdefense_base[building_lv]
	if not base_config then
		outFmtDebug("ResetAllBossDenfense building lv:%d config not exist",building_lv)
		return
	end
	local data_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionData)
	local factionData = app.objMgr:getObj(data_guid)
	for index = 0, MAX_FACTION_BOSSDEFENSE_COUNT - 1 do
		local point_config = tb_faction_bossdefense_point[index + 1]
		if not base_config then
			outFmtDebug("ResetAllBossDenfense point id:%d config not exist",index + 1)
			return
		end
		local pool_id = 0
		if point_config.type == 1 then
			local pool = base_config.boss_pool
			pool_id = pool[randInt(1,#pool)]
		elseif point_config.type == 2 then
			local pool = base_config.elite_pool
			pool_id = pool[randInt(1,#pool)]
		end
		local pool_config = tb_faction_bossdefense_pool[pool_id]
		if not pool_config then
			outFmtDebug("ResetAllBossDenfense pool id:%d config not exist",pool_id)
			return
		end
		local entry_config = tb_creature_template[pool_config.entry]
		if not entry_config then
			outFmtDebug("ResetAllBossDenfense creature entry:%d config not exist",pool_config.entry)
			return
		end
		
		
		
		local max_hp = 0
		for _,v in pairs(entry_config.pro) do
			if v[1] == 1 then
				max_hp = v[2]
			end
		end
		
		if max_hp <= 0 then
			outFmtDebug("ResetAllBossDenfense creature hp:%d <= 0",max_hp)
			return
		end
		
		self:SetBossDenfensePoolId(index,pool_id)
		self:SetBossDenfenseHp(index,max_hp)
		self:SetBossDenfenseMaxHp(index,max_hp)
		self:SetBossDenfenseStatus(index,0)
		
		for member_index = 0,MAX_FACTION_MAMBER_COUNT - 1 do
			self:SetBossDenfenseMemberDamage(factionData,index,member_index,0)
		end
	end
	self:printAllBoss()
end

function FactionInfo:printAllBoss()
	for index = 0, MAX_FACTION_BOSSDEFENSE_COUNT - 1 do
		local id = self:GetBossDenfensePoolId(index)
		local hp = self:GetBossDenfenseHp(index)
		local maxhp = self:GetBossDenfenseMaxHp(index)
		local status = self:GetBossDenfenseStatus(index)
		
		outFmtDebug("index:%d,pool_id:%d, hp:%d, maxhp:%d,status:%d",index,id,hp,maxhp,status)
	end
	outFmtDebug("end")
end

--开始挑战(player,index)
function FactionInfo:BossDenfenseChallenge(player,index)
	if index < 0 or index > MAX_FACTION_BOSSDEFENSE_COUNT - 1 then
		outFmtDebug("BossDenfenseChallenge bad index %d",index)
		return
	end
	if player:GetFactionBossDefenseTickets() <= 0 then
		outFmtDebug("BossDenfenseChallenge player tickets not enough")
		return
	end
	if self:GetBuildingLvByType(FACTION_BUILDING_TYPE_EVENT) <= 0 then
		outFmtDebug("BossDenfenseChallenge building  not active")
		return
	end
	local status = self:GetBossDenfenseStatus(index)
	if status == 1 or status == 2 then
		outFmtDebug("BossDenfenseChallenge status:%d can not challenge",status)
		return
	end
	if tonumber(os.date("%H")) == 11 and tonumber(os.date("%M")) >= 50 then
		player:CallOptResult(OPERTE_TYPE_FACTION,OPERTE_TYPE_FACTION_BOSSDEFENSE_TIME_ERROR)
		outFmtDebug("BossDenfenseChallenge time not avaliable")
		return
	end
	--传送 参数 guid index entry hp
	local point_config = tb_faction_bossdefense_point[index + 1]
	local map_id = point_config.map_id
	local x = point_config.born_pos[1]
	local y = point_config.born_pos[2]
	local pool_id = self:GetBossDenfensePoolId(index)
	local hp = self:GetBossDenfenseHp(index)
	local builfing_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_EVENT)
	local generalId = string.format("%s|%s|%d|%d|%d|%d",player:GetGuid(),self:GetGuid(),builfing_lv,index,pool_id,hp)
	call_appd_teleport(player:GetScenedFD(), player:GetGuid(), x, y, map_id, generalId)
	
	player:SubFactionBossDefenseTickets(1)
	--outFmtDebug("BossDenfenseChallenge Tickets %d",player:GetFactionBossDefenseTickets(1))
	self:SetBossDenfenseStatus(index,1)
	
	player:AddActiveItem(VITALITY_TYPE_FACTION_BOSS)
	
	--QUEST_TARGET_TYPE_FACTION_ACTIVITY
	player:onUpdatePlayerQuest(QUEST_TARGET_TYPE_FACTION_ACTIVITY, {1})
end

--击杀结算(player_guid,index,hp)
function FactionInfo:OnBossDenfenseChallengeKill(player_guid,pool_id)
	--[[
	local player = app.objMgr:getObj(player_guid)
	if not player then
		outFmtError("player offline !!!")
		return
	end
	 
	local pool_config = tb_faction_bossdefense_pool[pool_id]
	local drop_id = pool_config.kill_drop
	local dict = {}
	DoRandomDrop(drop_id, dict)
	local rewardDict = {}
	for itemId, value in pairs(dict) do
		table.insert(rewardDict, {itemId, value})
	end
	
	player:AppdAddItems(rewardDict, MONEY_CHANGE_FACTION_BOSSDEFENSE, LOG_ITEM_OPER_TYPE_FACTION_BOSSDEFENSE)
	--]]
end

--结束结算(player_guid,index,hp)
function FactionInfo:OnBossDenfenseChallengeFinish(player_guid,index,pool_id,hp)
	local member_index = self:FindPlayerIndex(player_guid)
	local data_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionData)
	local factionData = app.objMgr:getObj(data_guid)
	--[[
	--发送参与奖励 邮件
	local pool_config = tb_faction_bossdefense_pool[pool_id]
	local drop_id = pool_config.join_drop
	local dict = {}
	
	DoRandomDrop(drop_id, dict)
	local item_str = ""
	for itemId, value in pairs(dict) do
		if item_str == "" then
			item_str = item_str..itemId..','..value
		else
			item_str = item_str..','..itemId..','..value
		end
	end
	local mail_name = pool_config.mail_name
	local mail_desc = pool_config.mail_desc
	AddGiftPacksData(player_guid,0,4,os.time(),os.time() + 86400*30, mail_name, mail_desc,item_str, SYSTEM_NAME)
	--]]
	
	
	--统计伤害
	local cur_hp = self:GetBossDenfenseHp(index)
	local damage = cur_hp - hp
	if damage > 0 then
		self:AddBossDenfenseMemberDamage(factionData,index,member_index,damage)
	end
	--变更boss状态--检测是否全部击杀
	if hp == 0 then
		self:SetBossDenfenseHp(index,0)
		self:SetBossDenfenseStatus(index,2)
		self:CheckBossDenfenseAllClear()
	else
		self:SetBossDenfenseHp(index,hp)
		self:SetBossDenfenseStatus(index,0)
	end
	
	
end


--检测是否全部击杀
function FactionInfo:CheckBossDenfenseAllClear()
	for index = 0, MAX_FACTION_BOSSDEFENSE_COUNT - 1 do
		local status = self:GetBossDenfenseStatus(index)
		if status ~= 2 then
			return
		end
	end
	local building_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_EVENT)
	local base_config = tb_faction_bossdefense_base[building_lv]
	local clear_reward = base_config.clear_reward
	local item_str = ""
	for _, v in pairs(clear_reward) do
		if item_str == "" then
			item_str = item_str..v[1]..','..v[2]
		else
			item_str = item_str..','..v[1]..','..v[2]
		end
	end
	local mail_name = base_config.mail_name
	local mail_desc = base_config.mail_desc
	--全体成员发送全清奖励 邮件
	for i=0, MAX_FACTION_MAMBER_COUNT - 1 do
		local Guid = self:GetFactionMemberGuid(i)
		if Guid ~= "" and Guid ~= nil then
			AddGiftPacksData(Guid,0,4,os.time(),os.time() + 86400*30, mail_name, mail_desc,item_str, SYSTEM_NAME)
		end
	end
	
end

--清除玩家输出信息(player_guid)
function FactionInfo:ClearBossDenfensePlayerDamage(player_guid)
	local member_index = self:FindPlayerIndex(player_guid)
	local data_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionData)
	local factionData = app.objMgr:getObj(data_guid)
	for index = 0, MAX_FACTION_BOSSDEFENSE_COUNT - 1 do
		self:SetBossDenfenseMemberDamage(factionData,index,member_index,0)
	end
end

--获取输出排行榜
function FactionInfo:GetBossDenfenseDamageList(player,index)
	if index < 0 or index > MAX_FACTION_BOSSDEFENSE_COUNT - 1 then
		outFmtDebug("GetBossDenfenseDamageList bad index %d",index)
		return
	end
	local data_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypeFactionData)
	local factionData = app.objMgr:getObj(data_guid)
	local damage_table = {}
	for member_index = 0, MAX_FACTION_MAMBER_COUNT - 1 do
		local damage = self:GetBossDenfenseMemberDamage(factionData,index,member_index)
		if damage > 0 then
			table.insert(damage_table,{member_index,damage})
		end
	end
	table.sort(damage_table, function(a, b) return a[2] < b[2] end)
	
	--返回结果
	local list = {}
	for _,v in ipairs(damage_table) do
	--奖励通知
		local stru = mass_boss_rank_info_t.new()
		stru.name	= self:GetFactionMemberName(v[1])
		stru.dam 		= v[2]
		table.insert(list, stru)
	end
	player:call_show_faction_bossdefense_damage_list(list)
end

-----------------------------------------家族首领挑战结束-----------------------------------------------

-----------------------------------------家族无尽远征开始-----------------------------------------------

--成员今日层数
function FactionInfo:GetFactionMemberTowerTodayFloor(pos)
	return self:GetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOWER_TODAY_FLOOR)
end


function FactionInfo:SetFactionMemberTowerTodayFloor(pos,val)
	self:SetUInt32(FACTION_INT_FIELD_PLAYER+pos*MAX_FACTION_INT_MEMBER+FACTION_INT_MEMBER_TOWER_TODAY_FLOOR,val)
end

--今日最高通关者 层数
function FactionInfo:GetFactionTowerTodayTopFloor()
	return self:GetUInt32(FACTION_INT_FIELD_TOWER_TODAY_TOP_FLOOR)
end


function FactionInfo:SetFactionTowerTodayTopFloor(val)
	self:SetUInt32(FACTION_INT_FIELD_TOWER_TODAY_TOP_FLOOR,val)
end

--今日最高通关者 头像
function FactionInfo:GetFactionTowerTodayTopIcon()
	return self:GetUInt32(FACTION_INT_FIELD_TOWER_TODAY_TOP_ICON)
end


function FactionInfo:SetFactionTowerTodayTopIcon(val)
	self:SetUInt32(FACTION_INT_FIELD_TOWER_TODAY_TOP_ICON,val)
end

--今日最高通关者 战力
function FactionInfo:GetFactionTowerTodayTopForce()
	return self:GetDouble(FACTION_INT_FIELD_TOWER_TODAY_TOP_FORCE)
end


function FactionInfo:SetFactionTowerTodayTopForce(val)
	self:SetDouble(FACTION_INT_FIELD_TOWER_TODAY_TOP_FORCE,val)
end

--今日最高通关者 名称
function FactionInfo:GetFactionTowerTodayTopName()
	return self:GetStr(FACTION_STRING_FIELD_TOWER_TODAY_TOP_NAME)
end


function FactionInfo:SetFactionTowerTodayTopName(val)
	self:SetStr(FACTION_STRING_FIELD_TOWER_TODAY_TOP_NAME,val)
end

--每日重置
function FactionInfo:DailyResetTower()
	for index = 0,MAX_FACTION_MAMBER_COUNT - 1 do
		self:SetFactionMemberTowerTodayFloor(index,0)
	end
	self:SetFactionTowerTodayTopFloor(0)
	self:SetFactionTowerTodayTopIcon(0)
	self:SetFactionTowerTodayTopForce(0)
	self:SetFactionTowerTodayTopName("")
end

--挑战 (player )
--传送参数(floor,buff,debuff)
function FactionInfo:TowerChallenge(player)
	local building_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_EVENT)
	if building_lv <= 0 then
		outFmtDebug("BossDenfenseChallenge building  not active")
		return
	end
	local base_config = tb_faction_tower_base[building_lv]
	if not base_config then
		outFmtDebug("TowerChallenge building lv %d not exist",building_lv)
		return
	end
	local floor = player:GetFactionTowerClearFloor() + 1
	local floor_config = tb_faction_tower_floor[floor]
	if not floor_config then
		outFmtDebug("TowerChallenge floor %d not exist",floor)
		return
	end
	local clear_count = self:GetTowerFloorClearCount(floor)
	local buff_id = base_config.buffeffect_id
	local debuff_id = 0
	if clear_count > 0 then
		debuff_id = base_config.enemy_weaken_buffeffect_id[clear_count]
		if not debuff_id then
			debuff_id = base_config.enemy_weaken_buffeffect_id[#(base_config.enemy_weaken_buffeffect_id)]
		end
	end
	local map_id = floor_config.map_id
	local x = floor_config.born_pos[1]
	local y = floor_config.born_pos[2]
	local generalId = string.format("%d|%d|%d|%d|%s",floor,buff_id,debuff_id,os.time(),player:GetGuid())
	
	call_appd_teleport(player:GetScenedFD(), player:GetGuid(), x, y, map_id, generalId)
	
	player:AddActiveItem(VITALITY_TYPE_FACTION_TOWER)
	
	player:onUpdatePlayerQuest(QUEST_TARGET_TYPE_FACTION_ACTIVITY, {2})
end

--统计某层通关人数
function FactionInfo:GetTowerFloorClearCount(floor)
	local count = 0
	for index = 0, MAX_FACTION_MAMBER_COUNT -1 do
		if self:GetFactionMemberTowerTodayFloor(index) >= floor then
			count = count + 1
		end
	end
	return count
end

--扫荡 (player )
function FactionInfo:TowerSweep(player)
	if player:GetFactionTowerSweepFlag() then
		outFmtDebug("TowerSweep sweep flag true already sweeped")
		return
	end
	local building_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_EVENT)
	if building_lv <= 0 then
		outFmtDebug("BossDenfenseChallenge building  not active")
		return
	end
	local base_config = tb_faction_tower_base[building_lv]
	if not base_config then
		outFmtDebug("TowerChallenge building lv %d not exist",building_lv)
		return
	end
	
	local clear_floor = player:GetFactionTowerClearFloor()
	if clear_floor == 0 then
		outFmtDebug("TowerSweep clear floor = 0")
		return
	end
	
	player:SetFactionTowerSweepFlag()
	self:OnTowerTodayFloorUpdate(player,clear_floor)
	local rewardDict = {}
	for floor = 1,clear_floor do
		local floor_config = tb_faction_tower_floor[floor]
		if floor_config then
			local drop_id = floor_config.sweep_dropid
			local dict = {}
			DoRandomDrop(drop_id, dict)
			for item_id,num in pairs(dict) do
				table.insert(rewardDict,{item_id,num})
			end
		end
	end
	
	player:AppdAddItems(rewardDict, MONEY_CHANGE_FACTION_TOWER, LOG_ITEM_OPER_TYPE_FACTION_TOWER)
	--发送rewardDict给前端
	local list = {}
	for _,v in ipairs(rewardDict) do
	--奖励通知
		local stru = item_reward_info_t.new()
		stru.item_id	= v[1]
		stru.num 		= v[2]
		table.insert(list, stru)
	end
	--player:call_show_faction_tower_sweep_list(list)
	protocols.call_sweep_instance_reward ( player, INSTANCE_SUB_TYPE_FACTION_TOWER, 0, 0, 0, list)
	
	player:AddActiveItem(VITALITY_TYPE_FACTION_TOWER)
end

--挑战结束 更新排行 (player, floor)
function FactionInfo:OnTowerTodayFloorUpdate(player,floor)
	local clear_floor = player:GetFactionTowerClearFloor()
	if floor > clear_floor then
		player:SetFactionTowerClearFloor(floor)
	end
	
	local index = self:FindPlayerIndex(player:GetGuid())
	local today_floor = self:GetFactionMemberTowerTodayFloor(index)
	if floor > today_floor then
		self:SetFactionMemberTowerTodayFloor(index,floor)
	end
	
	local today_max_floor = self:GetFactionTowerTodayTopFloor()
	if floor > today_max_floor then
		self:SetFactionTowerTodayTopFloor(floor)
		self:SetFactionTowerTodayTopIcon(player:GetHead())
		self:SetFactionTowerTodayTopForce(player:GetForce())
		self:SetFactionTowerTodayTopName(player:GetName())
	end
end

--领取福利宝箱 (player, chest_index)
function FactionInfo:OpenTowerChest(player, chest_index)
	if index <= 0 or index >= 32 then
		return
	end
	if player:GetFactionTowerChestFlag(chest_index) then
		outFmtDebug("OpenTowerChest chest flag true already opened")
		return
	end
	
	local building_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_EVENT)
	if building_lv <= 0 then
		outFmtDebug("BossDenfenseChallenge building  not active")
		return
	end
	local base_config = tb_faction_tower_base[building_lv]
	if not base_config then
		outFmtDebug("TowerChallenge building lv %d not exist",building_lv)
		return
	end
	local need_floor = base_config.chest_floor[chest_index]
	if not need_floor then
		outFmtDebug("TowerChallenge top floor small %d than need %d",self:GetFactionTowerTodayTopFloor(),need_floor)
		return
	end
	if self:GetFactionTowerTodayTopFloor() < need_floor then
		outFmtDebug("TowerChallenge top floor small %d than need %d",self:GetFactionTowerTodayTopFloor(),need_floor)
		return
	end
	
	local rewardDict = {}
	local drop_id = base_config.chest_drop[chest_index]
	if not drop_id then
		outFmtDebug("TowerChallenge drop_id not exist in chest index %d",chest_index)
		return
	end
	
	local dict = {}
	DoRandomDrop(drop_id, dict)
	for item_id,num in pairs(dict) do
		table.insert(rewardDict,{item_id,num})
	end
	
	player:SetFactionTowerChestFlag(chest_index)
	
	player:AppdAddItems(rewardDict, MONEY_CHANGE_FACTION_TOWER, LOG_ITEM_OPER_TYPE_FACTION_TOWER)
	
end

-----------------------------------------家族无尽远征结束-----------------------------------------------

-----------------------------------------家族技能开始-----------------------------------------------

--成员学习技能
function FactionInfo:FactionSkillLvup(player,id)
	if not tb_faction_skill_base[id] then
		return
	end
	local building_lv = self:GetBuildingLvByType(FACTION_BUILDING_TYPE_SKILL)
	local level_limit = tb_faction_skill_building[building_lv].level_limit
	
	local buildIng_need_lv = tb_faction_skill_base[id].unlock_level
	local skill_id = tb_faction_skill_base[id].skill_id
	local learn_lv = player:GetFactionSkillLearnLv(id)
	
	if buildIng_need_lv > building_lv then
		outFmtDebug("FactionSkillLvup building lv not enough")
		return
	end
	
	if learn_lv >= level_limit then
		outFmtDebug("FactionSkillLvup skill lv can not lvup more")
		return
	end
	
	local config = tb_faction_skill_lvup[id*1000 + learn_lv + 1]
	if not config then
		outFmtDebug("FactionSkillLvup skill lv max  can not lvup")
		return
	end
	local cost = config.cost
	if not player:hasAllItems(cost) then
		outFmtDebug("FactionSkillLvup resouse not enough")
		return
	end
	
	if player:useAllItems(MONEY_CHANGE_FACTIONSKILL,cost) then
		player:SetFactionSkillLearnLv(id,learn_lv + 1)
		player:SetFactionSkillNowLv(id,learn_lv + 1)
		player:updatePassive(skill_id,learn_lv + 1)
		
		player:CallOptResult(OPRATE_TYPE_UPGRADE, UPGRADE_OPRATE_FACTIONSKILL_LVUP)
		
		outFmtDebug("OnFactionSkillBuildingLvChange skill %d lv %d",skill_id,learn_lv + 1)
	end
	
	
end

function FactionInfo:ResetAllMemberFactionSkill()
	local allPlayer = self:GetFactionAllMemberPtr()
	for _,player in pairs(allPlayer) do
		if player then
			player:UpdateFactionSkill()
		end
	end
end



-----------------------------------------家族技能结束-----------------------------------------------

function encodeSimpleInfo(faction_guid)
	
end

function decodeSimpleInfo(simpleString)
	
end

function encodeFightInfo(faction_guid)
	
end

function decodeFightInfo(fightString)
	
end


return FactionInfo