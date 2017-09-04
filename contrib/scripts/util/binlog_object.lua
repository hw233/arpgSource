--------------------------------------------------------------------------------------------------
--BinLog封装
require ('util.functions')

local BinLogObject = class('BinLogObject')

function BinLogObject:GetBit(index, offset)
	checkNil(index)
	checkNil(offset)
	return binLogLib.GetBit(self.ptr, index, offset)
end
	
function BinLogObject:SetBit (index, offset)
	checkNil(index)
	checkNil(offset)
	if self:GetBit(index, offset) then
		return
	end
	binLogLib.SetBit(self.ptr, index, offset)
end

function BinLogObject:UnSetBit (index, offset)
	checkNil(index)
	checkNil(offset)
	if not self:GetBit(index, offset) then
		return
	end
	binLogLib.UnSetBit(self.ptr, index, offset)
end

function BinLogObject:GetByte (index, offset)
	checkNil(index)
	checkNil(offset)
	return binLogLib.GetByte(self.ptr, index, offset)
end

function BinLogObject:AddByte (index, offset, value)
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.AddByte(self.ptr, index, offset, value)
end

function BinLogObject:SubByte (index, offset, value)
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.SubByte(self.ptr, index, offset, value)
end

function BinLogObject:SetByte (index, offset, value)
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	if self:GetByte(index, offset) == value then
		return
	end
	binLogLib.SetByte(self.ptr, index, offset, value)
end

function BinLogObject:GetUInt16 (index, offset)
	checkNil(index)
	checkNil(offset)
	return binLogLib.GetUInt16(self.ptr, index, offset)
end

function BinLogObject:AddUInt16 (index, offset, value)
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.AddUInt16(self.ptr, index, offset, value)
end

function BinLogObject:SubUInt16 (index, offset, value)
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.SubUInt16(self.ptr, index, offset, value)
end

function BinLogObject:SetUInt16 (index, offset, value)
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	if self:GetUInt16(index, offset) == value then
		return
	end
	binLogLib.SetUInt16(self.ptr, index, offset, value)
end

function BinLogObject:SetInt16 (index, offset, value)
	checkNil(index)
	checkNil(offset)
	checkNil(value)
	binLogLib.SetInt16(self.ptr, index, offset, value)
end

function BinLogObject:GetUInt32 (index)
	checkNil(index)
	return binLogLib.GetUInt32(self.ptr, index)
end

function BinLogObject:SetUInt32 (index, value)
	checkNil(index)
	checkNil(value)
	if self:GetUInt32(index) == value then
		return
	end
	binLogLib.SetUInt32(self.ptr, index, value)
end

function BinLogObject:AddUInt32 (index, value)
	checkNil(index)
	checkNil(value)
	binLogLib.AddUInt32(self.ptr, index, value)
end

function BinLogObject:SubUInt32 (index, value)
	checkNil(index)
	checkNil(value)
	binLogLib.SubUInt32(self.ptr, index, value)
end

function BinLogObject:GetInt32 (index)
	checkNil(index)
	return binLogLib.GetInt32(self.ptr, index)
end

function BinLogObject:SetInt32 (index, value)
	checkNil(index)
	checkNil(value)
	if self:GetInt32(index) == value then
		return
	end
	binLogLib.SetInt32(self.ptr, index, value)
end

function BinLogObject:AddInt32 (index, value)
	checkNil(index)
	checkNil(value)
	binLogLib.AddInt32(self.ptr, index, value)
end

function BinLogObject:SubInt32 (index, value)
	checkNil(index)
	checkNil(value)
	binLogLib.SubInt32(self.ptr, index, value)
end

function BinLogObject:GetDouble (index)
	checkNil(index)
	return binLogLib.GetDouble(self.ptr, index)
end

function BinLogObject:SetDouble (index, value)
	checkNil(index)
	checkNil(value)
	if self:GetDouble(index) == value then
		return
	end
	binLogLib.SetDouble(self.ptr, index, value)
end

function BinLogObject:AddDouble (index, value)
	checkNil(index)
	checkNil(value)
	binLogLib.AddDouble(self.ptr, index, value)
end

function BinLogObject:SubDouble (index, value)
	checkNil(index)
	checkNil(value)
	binLogLib.SubDouble(self.ptr, index, value)
end

function BinLogObject:GetFloat (index)
	checkNil(index)
	return binLogLib.GetFloat(self.ptr, index)
end

function BinLogObject:SetFloat (index, value)
	checkNil(index)
	checkNil(value)
	if self:GetFloat(index) == value then
		return
	end
	binLogLib.SetFloat(self.ptr, index, value)
end

function BinLogObject:SetStr (index,value)
	checkNil(index)
	checkNil(value)
	local prev = self:GetStr(index)
	if prev == value or string.len(prev) == 0 and string.len(value) == 0 then
		return
	end
	binLogLib.SetStr(self.ptr, index,value)
end

function BinLogObject:GetStr (index)
	checkNil(index)
	return binLogLib.GetStr(self.ptr, index)
end

--获取当前整数下标数量
function BinLogObject:GetUInt32Len ()
	return binLogLib.GetUInt32Len(self.ptr)
end

--获取当前字符串下标数量
function BinLogObject:GetStrLen ()
	return binLogLib.GetStrLen(self.ptr)
end

--获得GUID
function BinLogObject:GetGuid ()
	return self:GetStr(BINLOG_STRING_FIELD_GUID)
end

--获取名称
function BinLogObject:GetName ()
	return self:GetStr(BINLOG_STRING_FIELD_NAME)
end
--设置名称
function BinLogObject:SetName (name)
	self:SetStr(BINLOG_STRING_FIELD_NAME, name)
end

--获取所有者
function BinLogObject:GetOwner ()
	return self:GetStr(BINLOG_STRING_FIELD_OWNER)
end
--设置所有者
function BinLogObject:SetOwner (owner)
	self:SetStr(BINLOG_STRING_FIELD_OWNER, owner)
end

--获取版本号
function BinLogObject:GetVersion ()
	return self:GetStr(BINLOG_STRING_FIELD_VERSION)
end
--设置版本号
function BinLogObject:SetVersion (version)
	self:SetStr(BINLOG_STRING_FIELD_VERSION, version)
end

--设置binlog size
function BinLogObject:SetBinlogMaxSize( value )
	binLogLib.SetBinlogMaxSize(self.ptr, value)
end

function checkNil(value)
	if not value then
		outFmtError(debug.traceback())
	end
end

function BinLogObject:swapUInt (indx1, indx2)
	local val1 = self:GetUInt32(indx1)
	local val2 = self:GetUInt32(indx2)
	va11, val2 = swap(val1, val2)
	self:SetUInt32(indx1, va11)
	self:SetUInt32(indx2, val2)
end

function BinLogObject:swapStr (indx1, indx2)
	local val1 = self:GetStr(indx1)
	local val2 = self:GetStr(indx2)
	va11, val2 = swap(val1, val2)
	self:SetStr(indx1, va11)
	self:SetStr(indx2, val2)
end

return BinLogObject
