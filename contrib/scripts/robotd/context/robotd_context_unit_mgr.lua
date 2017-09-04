--初始化精灵管理器
function PlayerInfo:UnitMgrInit()
	self.my_unit = nil
	self.all_unit = {}
	self.all_creature = {}
	self.all_gameobject = {}
end

function PlayerInfo:CheckMyUnit(unit)
	if(unit:IsPlayer() == false)then
		return false
	end
	local guid = unit:GetGuid()
	local index = string.find(guid, '%.')
	guid = string.sub(guid, index + 1, string.len(guid))
	if(guid == self:GetGuid())then
		outFmtDebug('PlayerInfo:CheckMyUnit set my_unit  %s  %s', self:GetGuid(), unit:GetGuid())
		self.my_unit = unit
		self.unit_nil_time = 0
		return true
	end
	return false
end
--添加一个精灵到管理器中
function PlayerInfo:AddUnit(unit)
	self.all_unit[unit:GetGuid()] = unit
	if(unit:IsPlayer())then
		self:CheckMyUnit(unit)
	elseif(unit:IsGameObject())then
		self.all_gameobject[unit:GetGuid()] = unit
	else
		self.all_creature[unit:GetGuid()] = unit
	end
end

--移除一个精灵
function PlayerInfo:DelUnit(unit)
	self.all_unit[unit:GetGuid()] = nil
	if(self.my_unit == unit)then
		outFmtDebug('PlayerInfo:DelUnit set my_unit  %s  nil', self:GetGuid())
		self.my_unit = nil
	end

	if(unit:IsGameObject())then
		self.all_gameobject[unit:GetGuid()] = nil
	elseif(unit:IsPlayer() == false)then
		self.all_creature[unit:GetGuid()] = nil
	end
end

--在精灵集合中寻找自己的精灵对象
function PlayerInfo:FindMyUnit()
	for k,unit in pairs(self.all_unit) do
		if(self:CheckMyUnit(unit))then
			return
		end
	end
end

--根据模板ID搜索精灵对象
function PlayerInfo:FindUnitByTemplateID(temp_id)
	assert(temp_id)
	local result
	local dis = 9999999999
	for k,v in pairs(self.all_unit) do
		if(v:IsUnit() and v:GetEntry() == temp_id)then
			--outFmtDebug("PlayerInfo:FindUnitByTemplateID findID:%s get:%s", temp_id,v:GetName())
			if(result == nil or self.my_unit:GetDistance(v)<dis) then
				dis = self.my_unit:GetDistance(v)
				result = v
			end
		end
	end
	return result
end

--根据模板ID搜索游戏对象
function PlayerInfo:FindGameObject(object_id)
	assert(object_id)
	local tempUnit = nil
	for k,v in pairs(self.all_gameobject) do
		if(v:IsGameObject() and v:GetEntry() == object_id)then
			--用距离玩家最近的对象
			if(tempUnit == nil or self.my_unit:GetDistance(v)<self.my_unit:GetDistance(tempUnit))then
				tempUnit = v
			end
		end
	end
	return tempUnit
end

--通过guid获取精灵指针
function PlayerInfo:FindUnitByGuid(guid)
	return self.all_unit[guid]
end

--查找就近的生物
function PlayerInfo:FindCreate()
	local result = nil
	local dis = 999999999
	if(self.my_unit == nil)then
		return result, dis
	end
	
	for k,v in pairs(self.all_creature) do
		--活着的才有用
		--刚复活的生物有可能在障碍点里
		outFmtDebug('FindCreate %s ',k)
		if(v:IsDie() == false and v:IsUnit() and v:IsNpc()==false and v:IsCanRun())then
			local v_dis = self.my_unit:GetDistance(v)
			if(v_dis < dis)then
				dis = v_dis
				result = v
			end
		end
	end	
	return result, dis
end

--捡战利品
function PlayerInfo:UseLoot()

	local mapid = self:GetMapID()
	local x, y = self:GetPos()
	--outFmtDebug("PlayerInfo:UseLoot curXY %s %s %s",mapid,x,y)

	for k,v in pairs(self.all_gameobject) do
		if(v:IsLoot())then
			self:call_use_gameobject(v:GetUIntGuid(), 0)
			outFmtDebug("PlayerInfo:UseLoot %s %s %s %s", self:GetGuid(), v:GetGuid(), v:GetPos())
		end
	end
	return true
end
