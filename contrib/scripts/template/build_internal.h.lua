
local enum_tables = {}
local __lineno_enum_tables = {}
--local note_tables = {}
local note_ansi_tables = {}

function read_ln_string(ln, ln_num)
	local key,val,note = nil,nil

	--匹配
	--键 = 键 + ( 键 + 值 + ) / 值
	if(not key) then
		for v0,v1,v2,v3,v4 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s*[+]+%s*[%(]+%s*(%w+_[%w_]+)%s*[+]+%s*([%w]+)%s*[%)]+%s*[%/]+%s*([%w]+)') do
			key = v0
			local t_val = math.modf((enum_tables[v2] + tonumber(v3)) / tonumber(v4))
			val = enum_tables[v1] + t_val
		end
	end
	--键 = 键 + 键 * 键 + 值
	if(not key) then
		for v0,v1,v2,v3,v4 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]+%s+(%w+_[%w_]+)%s+[*]*%s+(%w+_[%w_]+)%s*[+]+%s*([%w_]+)') do
			key = v0
			val = enum_tables[v1] + enum_tables[v2] * enum_tables[v3] + tonumber(v4)
		end
	end
	--匹配 MAX_W_ = MAX_W_			键 = 键 * 键
	--键 = 键 * 键
	if(not key) then
		for v1,v2,v3 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[*]*%s+(%w+_[%w_]+)%s*') do
			key = v1
			val = enum_tables[v2] * enum_tables[v3]
		end
	end
	--键 = 键 * 键 + 值
	if(not key) then
		for v1,v2,v3,v4 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[*]*%s+(%w+_[%w_]+)%s*[+]+%s*([%w_]+)') do
			key = v1
			val = enum_tables[v2] * enum_tables[v3] + tonumber(v4)
		end
	end
	--匹配 MAX_W_ = MAX_W_			键 = 键 + 键 / 键 * 键
	if(not key) then
		for v0,v1,v2,v3,v4 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]+%s+(%w+_[%w_]+)%s+[/]*%s+(%w+_[%w_]+)%s*[*]+%s*([%w_]+)') do
			key = v0
			val = enum_tables[v1] + enum_tables[v2] / enum_tables[v3] * enum_tables[v4]
		end
	end
	--匹配 MAX_W_ = MAX_W_			键 = 键 + 键 * 键 * 键
	if(not key) then
		for v0,v1,v2,v3,v4 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]+%s+(%w+_[%w_]+)%s+[*]+%s+(%w+_[%w_]+)%s+[*]+%s+(%w+_[%w_]+)%s*') do
			key = v0
			val = enum_tables[v1] + enum_tables[v2] * enum_tables[v3] * enum_tables[v4]
		end
	end
	--键 = 键 + 键 * 键
	if(not key) then
		for v0,v1,v2,v3 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]+%s+(%w+_[%w_]+)%s+[*]*%s+(%w+_[%w_]+)%s*') do
			key = v0
			val = enum_tables[v1] + enum_tables[v2] * enum_tables[v3]
		end
	end
	--键 = 键 + 键 * 值
	if(not key) then
		for v0,v1,v2,v3 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]+%s+(%w+_[%w_]+)%s+[*]*%s+([%w]+)%s*') do
			key = v0
			val = enum_tables[v1] + enum_tables[v2] * tonumber(v3)
		end
	end
	--键 = 键 + 键 - 值
	if(not key) then
		for v0,v1,v2,v3 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]+%s+(%w+_[%w_]+)%s+[-]*%s+([%w]+)%s*') do
			key = v0
			val = enum_tables[v1] + enum_tables[v2] - tonumber(v3)
		end
	end
	--键 = 键 + 键 + 值
	if(not key) then
		for v1,v2,v3,v4 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]*%s+(%w+_[%w_]+)%s*[+]+%s*([%w_]+)') do
			key = v1
			val = enum_tables[v2] + enum_tables[v3] + tonumber(v4)
		end
	end
	--键 = 键 + 键 / 值
	if(not key) then
		for v0,v1,v2,v3 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]+%s+(%w+_[%w_]+)%s+[/]*%s+(%w+_[%w_]+)%s*') do
			key = v0
			val = enum_tables[v1] + math.ceil(enum_tables[v2] / enum_tables[v3])
		end
	end
	--键 = 键 + 键 / 值
	if(not key) then
		for v0,v1,v2,v3 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s+[+]+%s+(%w+_[%w_]+)%s+[/]*%s+([%w]+)%s*') do
			key = v0
			val = enum_tables[v1] + math.ceil(enum_tables[v2] / tonumber(v3))
		end
	end
	--键 = 键 + 键(或值)
	if(not key) then
		for v1,v2 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+%s*[+]+%s*[%w_]+)') do
			key = v1
			assert(key,string.format('第 %d 行是 nil!',ln_num))
			--键 + 键
			if(not val) then
				for v1,v2 in ln:gmatch('%s*(%w+_[%w_]+)%s*[+]+%s*(%w+_[%w_]+)') do
					val = enum_tables[v1] + enum_tables[v2]
				end
			end
			--键 + 值
			if(not val) then
				for v1,v2 in ln:gmatch('%s*(%w+_[%w_]+)%s*[+]+%s*([%w]+)') do
					val = enum_tables[v1] + tonumber(v2)
				end
			end
		end
	end

	--键 = 键
	if(not key) then
		for v1,v2 in ln:gmatch('%s*(%w+_[%w_]+)%s*=%s*(%w+_[%w_]+)%s*') do
			key,val = v1,enum_tables[v2]
		end
	end
	--键 = 值
	if(not key) then
		for v1,v2 in ln:gmatch('%s*(%w+[%w_]+)%s*=%s*([%w]+)%s*') do
			key,val = v1,tonumber(v2)
		end
		for v1,v2 in ln:gmatch('%s*(%w+[%w_]+)%s*=%s*(-[%w]+)%s*') do
			key,val = v1,tonumber(v2)
		end
	end
	--宏
	if(not key) then
		for v1,v2 in ln:gmatch('%s*#define%s+([%w_]+)%s+(%w+_[%w_]+)%s*') do
			key,val = v1,enum_tables[v2]
		end
	end
	--宏
	if(not key) then
		for v1,v2 in ln:gmatch('%s*#define%s+([%w_]+)%s+([%w]+)') do
			key,val = v1,tonumber(v2)
		end
	end
	--宏
	if(not key) then
		for v1,v2 in ln:gmatch('%s*#define%s+([%w_]+)%s+"([%w]+)"') do
			key,val = v1,v2
		end
	end
	--键 = 上个键
	if(not key) then
		for v1 in ln:gmatch('%s+(MAX_[%w_]+)%s*') do
			temkey = __lineno_enum_tables[ln_num-1]
			for i=2,ln_num do --加这个循环判断是因为有的枚举间有注释或空格
				if(temkey) then
					key,val = v1,enum_tables[temkey] + 1
					break
				else
					temkey = __lineno_enum_tables[ln_num-i]
				end
			end
		end
	end
	--键 = 上个键
	if(not key) then
		for v1 in ln:gmatch('%s+(INTERNAL_[%w_]+)%s*') do
			temkey = __lineno_enum_tables[ln_num-1]
			for i=2,ln_num do --加这个循环判断是因为有的枚举间有注释或空格
				if(temkey) then
					key,val = v1,enum_tables[temkey] + 1
					break
				else
					temkey = __lineno_enum_tables[ln_num-i]
				end
			end
		end
	end
	-- 过虑函数
	for v1,v2 in ln:gmatch('%s*([%w]+%s[%w]+[%(]+)%s*') do
		key =nil
	end
	--匹配 enum ******
	if(not key) then
		for v1 in ln:gmatch('%s*enum%s+([%w_]+)') do
			--print('enum',v1)
			--is_enum = true
		end
	end
	--读取注释
	for v0 in ln:gmatch('//(.+)') do
		note_ansi = v0
		--编码转换
--TODO:		note = string.sub(lc.a2u(v0), 1, -2)
		enter = false
	end

	--保存数据到表中
	__lineno_enum_tables[ln_num] = key
	if(key)then
		--note_tables[key] = note
		note_ansi_tables[key] = note_ansi
		enum_tables[key] = val
	end
end

function main()
	--读取头文件
	local ln_num = 0
	local f = io.open('../../../src/svrcore/protocol/internal.h',"r")
	for ln in f:lines() do
		ln_num = ln_num + 1
		read_ln_string(ln, ln_num)
	end
	f:close()

	--依据行号生成
	local out_f = io.open('internal.lua', 'w')
	for i=1,ln_num do
		local enum_key = __lineno_enum_tables[i]
		if enum_key and enum_tables[enum_key] then
			out_f:write(string.format('%s = %d\n',enum_key,enum_tables[enum_key]))			
		end
	end
	out_f:close()
end

main()
