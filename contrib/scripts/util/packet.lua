
local Packet = {}

local packet_meta = {
	__index = function(self, key)
		if key == 'optcode' then
			return packet.optcode(self.ptr)
		else
			return rawget(Packet, key)
		end
	end,
	__gc = function ( self )
		self:delete()
	end
}

--构造函数,当有提供包号的时候则不用提供C指针
function Packet.new( optcode, ptr  )
	local obj = {}
	obj.ptr = ptr
	if not obj.ptr then
		obj.ptr = packet.init(assert(optcode))
		obj.needfree = true
	end
	return setmetatable(obj, packet_meta) 
end

--TODO:本来指望垃圾自动回收,奈何5.1不会自动触发
function Packet:delete( )
	if self.needfree and self.ptr then
		packet.free(self.ptr)
		self.ptr = nil
	end
end

function Packet:writeUTF( s )
	if (type(s) ~= 'string') then
		outFmtError(debug.traceback())
	end
	assert(type(s) == 'string')
	packet.write_str(self.ptr, s)
end

function Packet:writeUTFByLen(s, len)
	assert(type(s) == 'string' and type(len) == 'number')
	packet.write_str_by_len(self.ptr, s, len)
end

function Packet:readUTF( )
	return packet.read_str(self.ptr)
end

function Packet:readUTFByLen( len )
	assert(type(len) == 'number')
	return packet.read_str_by_len(self.ptr, len)
end
 
function Packet:readU32( )
	return packet.read_u32(self.ptr)
end

function Packet:readI32( )
	return packet.read_i32(self.ptr)
end

function Packet:readU16( )
	return packet.read_u16(self.ptr)
end

function Packet:readI16( )
	return packet.read_i16(self.ptr)
end

function Packet:readByte( )
	return packet.read_u8(self.ptr)
end

function Packet:readFloat( )
	return packet.read_float(self.ptr)
end

function Packet:readDouble( )
	return packet.read_double(self.ptr)
end

function Packet:writeI16( val )
	assert(type(val) == 'number')
	packet.write_i16(self.ptr, val)
end

function Packet:writeU32( val )
	assert(type(val) == 'number')
	packet.write_u32(self.ptr, val)
end

function Packet:writeI32( val )
	assert(type(val) == 'number')
	packet.write_i32(self.ptr, val)
end

function Packet:writeU16( val )
	assert(type(val) == 'number')
	packet.write_u16(self.ptr, val)
end

function Packet:writeI16( val )
	assert(type(val) == 'number')
	packet.write_i16(self.ptr, val)
end

function Packet:writeByte( val )
	assert(type(val) == 'number')
	packet.write_u8(self.ptr,  val)
end

function Packet:writeFloat( val )
	assert(type(val) == 'number')
	packet.write_float(self.ptr,  val)
end

function Packet:writeDouble( val )
	assert(type(val) == 'number')
	packet.write_double(self.ptr,  val)
end

function Packet:writeMap( val )
	assert(type(val) == 'table')
	self:writeU16(table.nums(val))
	for k,v in pairs(val) do
		self:writeUTF(k)
		self:writeUTF(v)
	end
end

function Packet:readMap( )
	local map = {}
	local len = self:readU16()
	while(len > 0) do
		map[self:readUTF()] = self:readUTF()
		map[self:readUTF()] = self:readUTF()
		len = len -1
	end
	return map
end

--将数据流写入self
--@start_pos 如果未提供则从当前数据包的可读位置开始
--@size 从开始写入开始算的字节数
-- function Packet:writeBytes( from_pkt, start_pos, size )
-- 	print(type(from_pkt))
-- 	local from_pkt_ptr = type(from_pkt) == 'userdata' and from_pkt or assert(from_pkt.ptr)
-- 	local start_pos = start_pos or -1
-- 	local size = size or -1
-- 	return packet.write_bytes(self.ptr, from_pkt_ptr, start_pos, size)
-- end

function Packet.run_tests( )
	local pkt = Packet.new()
	--常规测试
	pkt:writeUTF('hello')
	local ret, s = pkt:readUTF()
	assert(ret and s == 'hello')
	
	pkt:writeU32(1)
	local ret, u = pkt:readU32()
	assert(ret and u == 1)
	
	pkt:writeI32(-1)
	local ret, i = pkt:readI32()
	assert( ret and i == -1)

	pkt:writeU16(1)
	local ret, u = pkt:readU16()
	assert(ret and u == 1)

	pkt:writeI16(-1)
	local ret, i = pkt:readI16()
	assert(ret and i == -1)

	pkt:writeByte(255)
	local ret, byte = pkt:readByte()
	assert(ret and byte == 255)

	--边界测试
	pkt:writeU16(-1)
	local ret, u = pkt:readU16()
	assert(ret and u == 65535)

	--local pkt2 = Packet.new()
	--assert(pkt2:writeBytes(pkt, 0))
	--local ret, s = pkt2:readUTF()
	--assert(ret and s == 'hello')
end

return Packet
