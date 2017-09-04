require"lfs"
require "bit"
--读取地图文本数据
local MapTemplate = class('MapTemplate')

--地图版本
MapTemplate.m_map_version = 0
--地图数据
MapTemplate.m_map_datas = {}
--加载所有地图
function MapTemplate.LoadMap(folder,only_baseinfo)
	--加载版本
	if(Load_Map_Version(folder) == false)then
		return false
	end
	
	local files = {}
	findindir(folder,"%.txt",files,false)
	--加载数据
	for i = 1,#files do
		local mt = require("MapTemplate").new()
		if(mt:LoadMapData(files[i],only_baseinfo) == false)then
			return false
		end
		m_map_datas[mt.m_baseinfo["mapid"]] = mt
	end
	
	return true
end

--加载地图版本
function MapTemplate.Load_Map_Version(folder)
	local map_index_file = folder + "index.map"
	local file = io.open(map_index_file,"r")
	if(not file)then return false end
	local info = lua_string_split(file.read(),",")
	if(not info or #info <= 0)then return false end
	self.m_map_version = toint(info[1])
	file.close()
	return true
end

--遍历某目录
function findindir (path, wefind, r_table, intofolder)
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local f = path..'\\'..file
            --print ("/t "..f)
            if string.find(f, wefind) ~= nil then
                --print("/t "..f)
                table.insert(r_table, f)
            end
            local attr = lfs.attributes (f)
            assert (type(attr) == "table")
            if attr.mode == "directory" and intofolder then
                findindir (f, wefind, r_table, intofolder)
            else
                --for name, value in pairs(attr) do
                --    print (name, value)
                --end
            end
        end
    end
end

function MapTemplate:ctor(  )
	self.MAX_MAP_CELLS_MASK = 400 * 400/32
	self.safe_distance = 6--安全距离
	--基本信息
	self.m_baseinfo = {}
	--障碍点
	self.m_obstacleMask = {}
	--复活点
	self.m_spawnpoint = {}
	--传送点
	self.m_teleport = {}
	--主干道
	self.m_main_road = {}
	--生物
	self.m_monsters = {}
	--游戏对象
	self.m_gameobjects = {}
	--怪物路线
	self.m_monsterlines = {}
	--8方向
	self.around = {{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}}
end

--载入地图数据
function MapTemplate:LoadMapData(str_url,only_baseinfo)
	--outDebug("----------------LoadMapData start "..str_url)
	local file = io.open(str_url,"r")
	if(not file)then return false end
	local info = {}
	for line in file:lines() do
		table.insert(info,line)
	end
	--基本信息1
	if(self:LoadBaseinfo(info[1]) == false)then
		file:close()
		return false
	end
	--需要读取其他信息
	if(not only_baseinfo or only_baseinfo == false)then
		--障碍点2
		self:LoadObstacle(info[2])
		--透明点(服务端无用)3
		--复活点4
		self:LoadRaised(info[4])
		--传送点5
		self:LoadTeleport(info[5])
		--主干道6
		self:LoadTrunk(info[6])
		--音乐信息(服务端无用)7
		--光源信息(服务端无用)8
		--动态信息(服务端无用)9
		--生物信息10
		self:LoadCreatrue(info[10])
		--游戏对象信息11
		self:LoadGameObject(info[11])
		--怪物移动路线信息12
		self:LoadMonsterLine(info[12])
		--水淹码(服务端无用)13
		--远景(服务端无用)14
		--水域(服务端无用)15
	end
	outDebug("------load mapdata ok, mapid= "..self.m_baseinfo["mapid"])
	file:close()
	return true
end

--载入基本信息
function MapTemplate:LoadBaseinfo(str)
	--outDebug("------load baseinfo ing "..str)
	if(not str or str == "")then
		return false
	end
	
	local info = lua_string_split(str,",")
	--地图id
	self.m_baseinfo["mapid"] = toint(info[1])
	--地图名称
	self.m_baseinfo["name"] = info[2]
	--地图创建时间
	self.m_baseinfo["create_date"] = info[3]
	--像素宽
	self.m_baseinfo["px_width"] = info[4]
	--像素高
	self.m_baseinfo["px_height"] = toint(info[5])
	--瓷砖宽
	self.m_baseinfo["unit_width"] = toint(info[6])
	--瓷砖高
	self.m_baseinfo["unit_height"] = toint(info[7])
	--逻辑宽
	self.m_baseinfo["width"] = toint(info[8])
	--逻辑高
	self.m_baseinfo["height"] = toint(info[9])
	--是否副本地图
	self.m_baseinfo["is_instance"] = toint(info[10])
	--父级地图ID
	self.m_baseinfo["parentID"] = toint(info[11])
	--场景音乐
	self.m_baseinfo["music"] = info[12]
	--影子方向
	self.m_baseinfo["shadow"] = toint(info[13])
	--副本人数
	self.m_baseinfo["count"] = toint(info[14])
	--日限制
	self.m_baseinfo["day_limit"] = toint(info[15])
	--周限制
	self.m_baseinfo["week_limit"] = toint(info[16])
	--副本类型
	self.m_baseinfo["instance_type"] = toint(info[17])
	
	--校验逻辑格数据
	if(self.m_baseinfo["width"] * self.m_baseinfo["height"] >= self.MAX_MAP_CELLS_MASK * 32)then
		return false
	end
	
	--outDebug("load baseinfo ok")
	return true
end

--载入障碍点
function MapTemplate:LoadObstacle(str)
	if(not str or str == "")then
		return false
	end
	
	local info = lua_string_split(str,",")
	local i = 0
	for i= 1,#info do
		table.insert(self.m_obstacleMask,toint(info[i]))
	end
	--outDebug("load obstacle ok")
	return true
end

--载入复活点
function MapTemplate:LoadRaised(str)
	if(not str or str == "")then
		return false
	end
	
	local info = lua_string_split(str,",")
	for i = 1,#info,3 do
		local raised = {}
		raised["x"] = toint(info[i])
		raised["y"] = toint(info[i+1])
		raised["faction"] = toint(info[i+2])
		
		table.insert(self.m_spawnpoint,raised)
	end
	--outDebug("load raised ok")
	return true
end

--载入传送点
function MapTemplate:LoadTeleport(str)
	if(not str or str == "")then
		return false
	end
	
	local info = lua_string_split(str,",")
	for i = 1,#info,7 do
		local teleport = {}
		teleport["x"] = toint(info[i])--坐标x
		teleport["y"] = toint(info[i+1])--坐标y
		teleport["temp_id"] = toint(info[i+2])--传送点模板id
		teleport["name"] = info[i+3]--传送点名称
		teleport["to_mapid"] = toint(info[i+4])--目标地图id
		teleport["to_x"] = toint(info[i+5])--目标地图坐标x
		teleport["to_y"] = toint(info[i+6])--目标地图坐标y
		
		table.insert(self.m_teleport,teleport)
	end
	--outDebug("load teleport ok")
	return true
end

--载入主干道
function MapTemplate:LoadTrunk(str)
	if(not str or str == "")then
		return false
	end
	
	local info = lua_string_split(str,",")
	local next_ps = {}
	local idx = 1
	while idx <= #info do
		local point = {}
		point["pos_x"] = info[idx]
		idx = idx +1
		point["pos_y"] = info[idx]
		idx = idx +1
		point["point"] = point["pos_x"] + self.m_baseinfo["width"] * point["pos_y"]
		point["next_ps"] = {}
		local len = info[idx]
		idx = idx +1
		local next_v = {}
		for i = 1,len do
			local n_x = info[idx]
			idx = idx +1
			local n_y = info[idx]
			idx = idx +1
			table.insert(next_v,n_x + self.m_baseinfo["width"] * n_y)
		end
		next_ps[point] = next_v
		self.m_main_road[point["point"]] = point
	end
	for key,value in pairs(next_ps) do
		local point = key
		for i=1,#value do
			local it_point
			for _k,_v in pairs(self.m_main_road) do
				if(_k == value[i])then
					it_point = _v
					break
				end
			end
			--ASSERT(it_point != this->m_main_road.end())
			if(it_point)then
				table.insert(point["next_ps"],it_point)
			end
		end
	end
	--outDebug("load trunk ok")
	return true
end

--载入生物
function MapTemplate:LoadCreatrue(str)
	if(not str or str == "")then
		return false
	end
	
	local info = lua_string_split(str,",")
	for i = 1,#info,15 do
		local monster = {}
		monster["templateid"] = toint(info[i])--模板id
		monster["x"] = toint(info[i+1])--坐标x
		monster["y"] = toint(info[i+2])--坐标y
		monster["count"] = toint(info[i+3])--数量
		monster["spawn_type"] = toint(info[i+4])--0死亡后刷新 1定时刷新
		monster["respawn_time"] = toint(info[i+5])--重生时间
		monster["spawn_time1"] = toint(info[i+6])--刷新时间1
		monster["spawn_time2"] = toint(info[i+7])--刷新时间2
		monster["spawn_time3"] = toint(info[i+8])--刷新时间3
		monster["script_name"] = info[i+9]--脚本
		monster["move_type"] = toint(info[i+10])--移动类别
		monster["line_id"] = toint(info[i+11])--线路id
		monster["flag"] = toint(info[i+12])--npc标识
		monster["toward"] = tonumber(info[i+13])--朝向
		monster["alias_name"] = info[i+14]--别名，用于副本AI脚本查询对象
		
		monster["toward"] = monster["toward"] * math.pi
		local pos = monster["x"] + self.m_baseinfo["width"] * monster["y"]
		self.m_monsters[pos] = monster
	end
	--outDebug("load creatrue ok")
	return true
end

--载入游戏对象
function MapTemplate:LoadGameObject(str)
	if(not str or str == "")then
		return false
	end
	
	local info = lua_string_split(str,",")
	for i = 1,#info,12 do
		local gameobject = {}
		gameobject["templateid"] = toint(info[i])--模板id
		gameobject["x"] = toint(info[i+1])--坐标x
		gameobject["y"] = toint(info[i+2])--坐标y
		gameobject["count"] = toint(info[i+3])--数量
		gameobject["spawn_type"] = toint(info[i+4])--0死亡后刷新 1定时刷新
		gameobject["respawn_time"] = toint(info[i+5])--重生时间
		gameobject["spawn_time1"] = toint(info[i+6])--刷新时间1
		gameobject["spawn_time2"] = toint(info[i+7])--刷新时间2
		gameobject["spawn_time3"] = toint(info[i+8])--刷新时间3
		gameobject["script_name"] = info[i+9]--脚本
		gameobject["toward"] = tonumber(info[i+10])--朝向
		gameobject["alias_name"] = info[i+11]--别名
		
		gameobject["toward"] = gameobject["toward"] * math.pi
		table.insert(self.m_gameobjects,gameobject)
	end
	--outDebug("load gameobject ok")
	return true
end

--载入怪物路线
function MapTemplate:LoadMonsterLine(str)
	if(not str or str == "")then
		return false
	end
	
	local info = lua_string_split(str,",")
	local i = 1
	while i <= #info do
		local monsterline = {}
		monsterline["id"] = info[i]
		i = i + 1
		monsterline["path"] = {}
		local p_len = info[i]
		i = i + 1
		for j=1,p_len do
			local point = {}
			point["pos_x"] = info[i]
			i = i + 1
			point["pos_y"] = info[i]
			i = i + 1
			table.insert(monsterline["path"],point)
		end
		self.m_monsterlines[monsterline["id"]] = monsterline
	end
	--outDebug("load monsterline ok")
	return true
end

--是否副本
function MapTemplate:IsInstance()
	return self.m_baseinfo["is_instance"] ~= 0
end

--是否路障
function MapTemplate:IsObstacle(p_x, p_y)
	local index = p_x + p_y * self.m_baseinfo["width"]
	return bit.band(toint(self.m_obstacleMask[index/8]),bit.lshift(1, bit.band(index , 0x7 ))) ~= 0
end

--非有效坐标
function MapTemplate:IsNotValidPos(p_x,p_y)
	return p_x ~= nil or p_y ~= nil or
		p_x < 0 or p_y < 0 or
		p_x >= tonumber(self.m_baseinfo["width"]) or p_y >= tonumber(self.m_baseinfo["height"])
end

--计算两点之间距离
function MapTemplate:GetDistance(fromX, fromY, toX, toY)
	local dx = toX - fromX
	local dy = toY - fromY
	return math.sqrt(dx*dx + dy*dy)
end

--某点是否在地图内
function MapTemplate:IsInMap(px, py)
	if(px >= self.m_baseinfo["width"] or py >= self.m_baseinfo["height"])then
		return false
	end
	return true
end

--某点是否可以通过
function MapTemplate:IsCanRun(px, py)
	if(px >= self.m_baseinfo["width"] or py >= self.m_baseinfo["height"])then
		return false
	end
	return self:IsObstacle(px + self.m_baseinfo["width"] * py) == false		
end
	
--
function MapTemplate:IsCanRun(p)
	if(p >= toint(m_baseinfo["width"] * m_baseinfo["height"]))then
		return false
	end
	return self:IsObstacle(p) == false	
end

-- // 传入给定路径，判断是否可以通过
-- bool IsCanRun(uint16 count, float *path) const;
function MapTemplate:IsCanRun(count, path)
	local tpath, x0, y0, x1, y1
    tpath = path

    if(count > 1)then
        x1 = tpath
        tpath = tpath + 1
        y1 = tpath
		tpath = tpath + 1
        local isStart = true
        x0 = x1
        y0 = y1
        x1 = tpath
        tpath = tpath + 1
        y1 = tpath
        tpath = tpath + 1

        if(IsCanRun(x0, y0, x1, y1, isStart) == false)then
			--outDebug("MapTemplate::IsCanRun can not run %f,%f to %f,%f", x0, y0, x1, y1)
            return false
        end

        --逐点判断，不能通过立刻跳出
        for i=4, count,2 do
            x0 = x1
            y0 = y1
            x1 = tpath
            tpath = tpath +1
            y1 = tpath
            tpath = tpath +1

            if(IsCanRun(x0, y0, x1, y1, isStart) == false)then
				--outDebug("MapTemplate::IsCanRun can not run %f,%f to %f,%f", x0, y0, x1, y1)
                return false
            end
            isStart = false
        end
    else
        x0 = tpath
        tpath = tpath +1
        y0 = tpath
        return IsCanRun(toint(x0), toint(y0))
    end

    return true
end

--传入给定路径，判断是否可以通过
-- bool IsCanRun(float fromX, float fromY,const vector<float>& path) const;	
function MapTemplate:IsCanRun(fromX, fromY, path)
	if(not path or #paht <= 0)then
		return self:IsCanRun(toint(fromX),toint(fromY))
	end
	
	--第一点,无视障碍
	if(IsCanRun(toint(fromX),toint(fromY)) == false)then
		return true
	end
	
	if(IsCanRun(tonumber(fromX), tonumber(fromY), tonumber(path[0]), tonumber(path[1]), true) == false)then
		return false
	end
	
	--逐点判断，不能通过立刻跳出
	for i = 1,#path,2 do
		if(IsCanRun(tonumber(fromX), tonumber(fromY), tonumber(path[i]), tonumber(path[i+1])) == false)then
			--outDebug("MapTemplate::IsCanRun can not run %f,%f to %f,%f", fromX,fromY, path[i],path[i+1])
			return false
		end
		fromX = path[i]
		fromY = path[i+1]
	end 
    return true
end

-- bool IsCanRun(float fromX, float fromY, float toX, float toY, bool isStart = false) const;	
function MapTemplate:IsCanRun(fromx, fromy, tox, toy, isStart)
	local x0 = toint(fromx) 
	local y0 = toint(fromy)
	local x1 = toint(tox)
	local y1 = toint(toy)
	local dx =  math.abs(x1 - x0)
	local sx = -1
	if(x0 < x1)then
		sx = 1
	end
	local dy = -math.abs(y1 - y0)
	local sy = -1
	if(y0 < y1)then
		sy = 1
	end
	local err = dx + dy
	local e2 --error value e_xy

	while true do
		--loop
		if(IsCanRun(x0, y0) == false and isStart == false)then
			return false
		end

		isStart = false

		if(x0 == x1 and y0 == y1)then break end

		e2 = 2 * err

		if(e2 >= dy)then
			err = err + dy    -- e_xy+e_x > 0 
			x0 = x0 + sx
		end

		if(e2 <= dx)then
			err = err + dx    --/* e_xy+e_y < 0 */
			y0 = y0 + sy
		end
	end

	return true
end

-- bool IsCanRun(float fromX, float fromY, float toX, float toY, uint16 &lastX, uint16 &lastY, bool isStart = false) const;
function MapTemplate:IsCanRun(fromX, fromY, toX, toY, lastX, lastY, isStart)
	local nx = 0
	local ny = 0
	return IsCanRun(fromX, fromY, toX, toY, lastX, lastY, nx, ny, isStart)
end

-- bool IsCanRun(float fromX, float fromY, float toX, float toY, uint16 &lastX, uint16 &lastY, uint16 &cannotx, uint16 &cannoty, bool isStart = false) 
function MapTemplate:IsCanRun(fromx, fromy, tox, toy, lastX, lastY, cannotx, cannoty, isStart)
	local x0 = toint(fromx)
	local y0 = toint(fromy)
	local x1 = toint(tox)
	local y1 = toint(toy)
	local dx =  math.abs(x1 - x0)
	local sx =  -1
	if(x0 < x1)then
		sx = 1
	end
	local dy = -math.abs(y1 - y0)
	local sy =  -1
	if(y0 < y1)then
		sy = 1
	end
	local err = dx + dy
	local e2 --/* error value e_xy */
	local lastX = 0
	local lastY = 0

	while true do
		--/* loop */
		if(IsCanRun(x0, y0) == false and isStart == false)then
			cannotx = x0
			cannoty = y0
			return false
		end

		lastX = x0
		lastY = y0
		isStart = false

		if(x0 == x1 and y0 == y1)then break end

		e2 = 2 * err

		if(e2 >= dy)then
			err = err + dy    --/* e_xy+e_x > 0 */
			x0 = x0 + sx
		end

		if(e2 <= dx)then
			err = err + dx    --/* e_xy+e_y < 0 */
			y0 = y0 + sy
		end
	end

	return true
end

--是否传送点
function MapTemplate:IsTeleportPos(px, py)
	for i = 0, #self.m_teleport do
		if(self.m_teleport[i]["x"] == px and self.m_teleport[i]["y"] == py)then
			return true
		end
	end
	return false
end

--判断朝向
function MapTemplate:FindAroundIndex(fromX, fromY, toX, toY)
	local diffx, diffy
	local around_index = 0
	--计算差异，用来获取方向
	diffx = fromX - toint(toX)
	diffy = fromY - toint(toY)
	--通过差异发现方向
	while around_index < 8 and (around[around_index][1] ~= diffx or around[around_index][2] ~= diffy) do
		around_index = around_index + 1
	end

	--没找到方向，明显程序有问题。
	if(around_index >= 8)then
		--tea_pdebug("不是临近的点，无法得到朝向。")
		return -1;
	end
	--为了让八方向数组形成圆形循环，林北加个八比较爽
	return around_index + 8
end

--获取某个复活点
function MapTemplate:GetSpwanPoint(px, py, faction)
	if(#self.m_spawnpoint <= 0)then return 0,0 end
	local min_dist = -1
	local fx = 0
	local fy = 0
	for i = 1,#self.m_spawnpoint do
		local raise = self.m_spawnpoint[i]
		if(raise["faction"] == 0 or raise["faction"] == faction)then
			local dist = self:GetDistance(px,py,raise["x"],raise["y"])
			if(min_dist == -1 or dist < min_dist)then
				min_dist = dist
				fx = raise["x"]
				fy = raise["y"]
			end
		end
	end
	return fx,fy
end

--是否在安全区域
function MapTemplate:IsInSafeArea(p_x, p_y)
	for i = 1,#self.m_spawnpoint do
		local rx = self.m_spawnpoint[i]["x"]
		local ry = self.m_spawnpoint[i]["y"]
		if(self:GetDistance(p_x,p_y,rx,ry) < self.safe_distance and self:IsInstance() == false)then
			return true
		end
	end
	return false
end

--?
function MapTemplate:GetMainRoadPoint(point)
	-- map<uint32, MainRoadPoint*>::const_iterator it = m_main_road.find(point);
	-- ASSERT(it != m_main_road.end());
	-- return it->second;
	return self.m_main_road[point]
end

return MapTemplate
