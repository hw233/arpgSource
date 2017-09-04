-- bit operation

bit = bit or {}
bit.data32 = {}
for i=1,32 do
    bit.data32[i]=2^(32-i)
end

function    bit._b2d(arg)
    local   nr=0
    for i=1,32 do
        if arg[i] ==1 then
        nr=nr+bit.data32[i]
        end
    end
    return  nr
end

function bit._d2b(arg)
    arg = arg >= 0 and arg or (0xFFFFFFFF + arg + 1)
    local   tr={}
    for i=1,32 do
        if arg >= bit.data32[i] then
        tr[i]=1
        arg=arg-bit.data32[i]
        else
        tr[i]=0
        end
    end
    return   tr
end

function    bit._and(a,b)
    local   op1=bit._d2b(a)
    local   op2=bit._d2b(b)
    local   r={}

    for i=1,32 do
        if op1[i]==1 and op2[i]==1  then
            r[i]=1
        else
            r[i]=0
        end
    end
    return  bit._b2d(r)

end

function    bit._rshift(a,n)
    local   op1=bit._d2b(a)
    n = n <= 32 and n or 32
    n = n >= 0 and n or 0

    for i=32, n+1, -1 do
        op1[i] = op1[i-n]
    end
    for i=1, n do
        op1[i] = 0
    end

    return  bit._b2d(op1)
end

function    bit._lshift(a,n)
    local   op1=bit._d2b(a)
    n = n <= 32 and n or 32
    n = n >= 0 and n or 0

    for i=1, 32-n do
        op1[i] = op1[i+n]
    end
    for i=32-n+1, 32 do
        op1[i] = 0
    end

    return  bit._b2d(op1)
end

function    bit._not(a)
    local   op1=bit._d2b(a)
    local   r={}

    for i=1,32 do
        if  op1[i]==1   then
            r[i]=0
        else
            r[i]=1
        end
    end
    return  bit._b2d(r)
end

function bit._or(a,b)
    local   op1=bit._d2b(a)
    local   op2=bit._d2b(b)
    local   r={}

    for i=1,32 do
        if op1[i]==1 or op2[i]==1  then
            r[i]=1
        else
            r[i]=0
        end
    end
    return  bit._b2d(r)
end

-- 索引从1开始
function bit.setBit(a, i)
	if i > 32 or i < 1 then
		error('bit.setBit i cross the border')
	end
	local   op1=bit._d2b(a)
	op1[33 - i] = 1
	return bit._b2d(op1)
end

-- 索引从1开始
function bit.unSetBit(a, i)
	if i > 32 or i < 1 then
		error('bit.setBit i cross the border')
	end
	local   op1=bit._d2b(a)
	op1[33 - i] = 0
	return bit._b2d(op1)
end

bit.band = bit.band or bit._and
bit.rshift = bit.rshift or bit._rshift
bit.lshift = bit.lshift or bit._lshift
bit.bnot = bit.bnot or bit._not

------------------------------------------
-- bit碰撞检测 画布起点在左上角顺时针画圈
------------------------------------------
bitHit = {}

-- 坐标转换掩码
-- @parame x
-- @parame y
-- @return 掩码索引
function bitHit._findIdxByPoint(x, y)
	x, y = math.floor(x), math.floor(y)
	if x == 0 and y == 0 then
		return 1
	end
	local x_abs = math.abs(x)
	local y_abs = math.abs(y)
	local side_half = math.max(x_abs, y_abs)
	local side = side_half * 2
	-- 找出画布所在节点位子
	local canvas_last_idx
	local x_temp = x + side_half
	local y_temp = y + side_half
	if y_temp == 0 then
		-- print('in top')
		canvas_last_idx = side * 4 - x_temp
	elseif x_temp == side then
		-- print('in right', y_temp)
		canvas_last_idx = side * 3 - y_temp
	elseif y_temp == side then
		-- print('in bottom')
		canvas_last_idx = side + x_temp
	elseif x_temp == 0 then
		-- print('in left', y_temp)
		canvas_last_idx = y_temp
	end
	local node_side = side + 1
	-- print('canvas_last_idx', canvas_last_idx)
	local canvas_idx = node_side * node_side + 1 - canvas_last_idx
	-- print('canvas_idx', canvas_idx)
	return canvas_idx
end

-- 碰撞测试
-- @parame x
-- @parame y
-- @parame 掩码块数据
-- @return 是否碰撞
function bitHit.hitTest(x, y, ...)
	--print(x, y)
	local isHit = false
	local bit_idx = bitHit._findIdxByPoint(x, y)
	--print ('index for canvas', bit_idx)
	-- 要比较的掩码块位置
	local compare_idx = 1
	if bit_idx > 32 then
		local d = bit_idx / 32
		compare_idx = math.ceil(d)
		bit_idx = (bit_idx - 1) % 32 + 1
	end
	--print ('index 32bit', bit_idx, 'compare_idx', compare_idx)
	-- print('要比较的掩码块位置', compare_idx - 1, '坐标在掩码块中的索引', bit_idx)
	local compare_t = {...}
	if compare_t[compare_idx] ~= nil then
		local compare_v = bit.setBit(0, bit_idx)
		--print('compare_v', compare_v, 'compare_t[compare_idx]', compare_t[compare_idx])
		isHit = bit.band(compare_t[compare_idx], compare_v) ~= 0
	end
	return isHit
end

-- 创建碰撞区域数据
-- @parame array坐标数据x,y,x,y,...
function bitHit.makeAreaData(...)
	local values = {...}
	--print(table.concat(values, ','))
	local size = math.floor(#values / 2)
	local datas = {}
	for i = 1, size do
		local x = values[i * 2 - 1]
		local y = values[i * 2]
		if math.abs(x) > 15 then
			error('bitHit.makeAreaData x cross the border')

		end
		if math.abs(y) > 15 then
			error('bitHit.makeAreaData y cross the border')
		end
		local bit_idx = bitHit._findIdxByPoint(x, y)
		--print('bit_idx', bit_idx)
		local compare_idx = 1
		if bit_idx > 32 then
			local d = bit_idx / 32
			compare_idx = math.ceil(d)
			bit_idx = (bit_idx - 1) % 32 + 1
		end
		datas[compare_idx] = datas[compare_idx] or 0
		datas[compare_idx] = bit.setBit(datas[compare_idx], bit_idx)
	end
	return datas
end

--获得技能区域数据
--tParam:目标偏移坐标值对组{x0,y0,x1,y1,...}
--返回：区域数据
function CalMakeAreaData(tParam)
	if #tParam < 2 then
		return {0}
	end
	local datas = bitHit.makeAreaData(unpack(tParam))
	local max_len = table.maxn(datas)
	for i = 1, max_len do
		datas[i] = datas[i] or 0
	end
	return datas
end

--获得碰撞测试结果
--tParam[1]:坐标x
--tParam[2]:坐标y
--tParam[3~n]:区域数据
--返回：碰撞测试结果
function CalHitTest(tParam)
	if #tParam < 3 then
		return {false}
	end
	return {bitHit.hitTest(tParam[1], tParam[2], unpack(tParam, 3))}
end

------------------------------------------
-- postion坐标系转换工具函数
------------------------------------------
postion = {}
-- 坐标旋转角度
-- @parame x
-- @parame y
-- @parame angle
-- @return 旋转后坐标
function postion.rotate(x, y, angle)
	return x * math.cos(angle) - y * math.sin(angle), x * math.sin(angle) + y * math.cos(angle)
end

-- 坐标平移
-- @parame x
-- @parame y
-- @parame tx
-- @parame ty
-- @return 平移后坐标
function postion.translate(x, y, tx, ty)
	return x + tx, y + ty
end

-- 坐标缩放
-- @parame x
-- @parame y
-- @parame scaleX
-- @parame scaleY
-- @return 缩放后坐标
function postion.scale(x, y, scaleX, scaleY)
	return x * scaleX, y * scaleY
end

-- 获取角度
-- @parame x0
-- @parame y0
-- @parame x1
-- @parame y1
-- @return 角度
function postion.angle(x0, y0, x1, y1)
	local angle = math.atan2(y1 - y0, x1 - x0)
	if angle < 0 then
		angle = angle + 2 * math.pi
	end
	return angle
end

-- 获取距离
-- @parame x0
-- @parame y0
-- @parame x1
-- @parame y1
-- @return 距离
function postion.distance(x0, y0, x1, y1)
	local dx = x1 - x0
	local dy = y1 - y0
	return math.sqrt(dx*dx + dy*dy)
end

--获得目标点技能区域坐标
--tParam[1]:发出点x
--tParam[2]:发出点y
--tParam[3]:施法点x
--tParam[4]:施法点y
--tParam[5]:目标点x
--tParam[6]:目标点y
--tParam[7]:备选朝向（发出点施法点重合时选用）
--tParam[8]:固定朝向（可选）
--返回：目标点技能区域坐标
function GetHitAreaPostion(tParam)
	if #tParam < 7 then
		return {50, 50}	-- 返回一个打不到的点
	end
	local angle
	if tParam[8] ~= nil then
		angle = tParam[8]	-- 固定朝向
	elseif tParam[1] == tParam[3] and tParam[2] == tParam[4] then
		angle = tParam[7]	-- 备选朝向
	else
		-- 发出点到施法点朝向
		angle = postion.angle(tParam[1], tParam[2], tParam[3], tParam[4])
	end
--	print('======================angle', angle, angle * 180/math.pi)
	local posx, posy = postion.translate(tParam[5], tParam[6], -tParam[3], -tParam[4])
--	print("translate posx", posx, "posy", posy)
	posx, posy = postion.rotate(posx, posy,- angle)
--	print("rotate posx", posx, "posy", posy)
	posx, posy = math.floor(posx), math.floor(posy)
--	print("ret posx", posx, "posy", posy)
	return {posx, posy}
end

--获得转换后的技能区域坐标组
--tParam[1]:发出点x
--tParam[2]:发出点y
--tParam[3]:施法点x
--tParam[4]:施法点y
--tParam[5]:区域宽度
--tParam[6]:区域高度
--tParam[7]:备选朝向（发出点施法点重合时选用）
--tParam[8]:是否固定朝向（0 自动，1 固定朝向）
--tParam[9~n]:技能数据
--返回：技能区域坐标组
function GetAreaHitTest(tParam)
	if #tParam < 9 then
		return {0}	-- 数据不够返回0
	end
	--目标点转换基本数据
	local data = {tParam[1], tParam[2], tParam[3], tParam[4], 0, 0, tParam[7]}
	if tParam[8] == 1 then
		data[8] = 0
	end
	--碰撞测试结果基本数据
	local area = {0, 0}
	for i = 1, #tParam-8 do
		area[i+2] = tParam[i+8]
	end
	local half_x, half_y = math.floor(tParam[5] / 2), math.floor(tParam[6] / 2)
	local result = {}
	for i = 0, tParam[5]-1 do
		for j = 0, tParam[6]-1 do
			data[5], data[6] = data[3] + i - half_x, data[4] + j - half_y
			local postion = GetHitAreaPostion(data)
			area[1], area[2] = postion[1], postion[2]
			if CalHitTest(area)[1] then
				table.insert(result, i)
				table.insert(result, j)
			end
		end
	end
	if #result < 1 then
		result[1] = 0
	end
	return result
end

function bitHit.demo()
	
	local dstx = 5
	local dsty = 0
	local castx = 2
	local casty = 10
	
	--[[local angle = math.atan2(dsty-casty, dstx-castx)
	if angle < 0 then
		angle = 2 * math.pi + angle;
	end--]]
	local angle = math.pi * 7 / 4
	
	local fix = nil
	local ret = GetHitAreaPostion({castx, casty, castx, casty, dstx, dsty, angle, fix})
	local x = ret[ 1 ]
	local y = ret[ 2 ]
	local idx = bitHit._findIdxByPoint(x, y)
	print("x = ", x, "y = ", y, "idx = ", idx)
	
	
	
end

--bitHit.demo()
