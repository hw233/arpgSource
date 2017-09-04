
--弹窗公告信息
local GlobalRightFloat = class('GlobalRightFloat', assert(BinLogObject ))

function GlobalRightFloat:ctor( ... )
end

--弹窗公告信息个数加1
function GlobalRightFloat:Next()
	local cur_num = self:GetAttackStrutCount()
	if(cur_num == MAX_RIGHTFLOAT_INFO_COUNT-1)then		--弹窗信息最多保留MAX_RIGHTFLOAT_INFO_INT_NOW_INDEX
		self:SetAttackStrutCount(0)	
	else
		self:AddUInt32(MAX_RIGHTFLOAT_INFO_INT_NOW_INDEX, 1)
	end
end	

--获得弹窗公告信息当前索引
function GlobalRightFloat:GetAttackStrutCount()
	return self:GetUInt32(MAX_RIGHTFLOAT_INFO_INT_NOW_INDEX)
end

--设置弹窗公告信息当前索引
function GlobalRightFloat:SetAttackStrutCount( val)
	self:SetUInt32(MAX_RIGHTFLOAT_INFO_INT_NOW_INDEX, val)
end

--判断弹窗公告是否已存在
function GlobalRightFloat:IsRightFloatInfo(id)
	for i = 0, MAX_RIGHTFLOAT_INFO_COUNT -1
	do
		local index = RIGHTFLOAT_INT_FIELD_START + MAX_RIGHTFLOAT_INFO_INT * i + RIGHTFLOAT_INFO_INT_ID
		if(self:GetUInt32(index) ~= 0 and self:GetUInt32(index) == id)then
			return i
		end
	end
	return -1
end

--添加一个弹窗公告信息
function GlobalRightFloat:AddRightFloatInfo( id, status, zone1, zone2, zone3, mode, ftime, ttime, adddate, subject, content, link)
	local pos = self:GetAttackStrutCount()
	local is_new = true
	if(self:IsRightFloatInfo(id) ~= -1)then	--判断是否已经有弹窗公告了
		pos = self:IsRightFloatInfo(id)
		is_new = false
	end
	local index = pos * MAX_RIGHTFLOAT_INFO_INT
	local index_str = RIGHTFLOAT_STRING_FIELD_START + pos * MAX_RIGHTFLOAT_INFO_STRING
	self:SetUInt32(index + RIGHTFLOAT_INFO_INT_ID, id)
	self:SetUInt32(index + RIGHTFLOAT_INFO_INT_ZONE_1, zone1)
	self:SetUInt32(index + RIGHTFLOAT_INFO_INT_ZONE_2, zone2)
	self:SetUInt32(index + RIGHTFLOAT_INFO_INT_ZONE_3, zone3)
	self:SetUInt32(index + RIGHTFLOAT_INFO_INT_START_TIME, ftime)
	self:SetUInt32(index + RIGHTFLOAT_INFO_INT_END_TIME, ttime)
	self:SetUInt32(index + RIGHTFLOAT_INFO_INT_ADD_TIME, adddate)
	self:SetByte(index + RIGHTFLOAT_INFO_INT_BYTE, 0 ,status)
	self:SetByte(index + RIGHTFLOAT_INFO_INT_BYTE, 1 ,mode)
	self:SetStr(index_str + RIGHTFLOAT_INFO_STRING_SUBJECT,subject)
	self:SetStr(index_str + RIGHTFLOAT_INFO_STRING_CONTENT,content)
	self:SetStr(index_str + RIGHTFLOAT_INFO_STRING_LINK,link)
	if(is_new)then
		self:Next()
	end
	return true
end		
	

return GlobalRightFloat
