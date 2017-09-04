RobotdUnit = class('RobotdUnit', BinLogObject)

--构造函数
function RobotdUnit:ctor()
	
end

--获取整数guid
function RobotdUnit:GetUIntGuid()
	if(self.u_guid == nil)then
		local guid = self:GetGuid()
		local index = string.find(guid, '%.') or 0
		guid = string.sub(guid, 2, index - 1)
		self.u_guid = tonumber(guid)
	end
	return self.u_guid
end

--获取精灵的类型
function RobotdUnit:GetTypeID()
	return self:GetByte(UNIT_FIELD_BYTE_0, 0)
end

--是否游戏对象
function RobotdUnit:IsGameObject()
	return self:GetTypeID() == TYPEID_GAMEOBJECT
end

--是否战利品
function RobotdUnit:IsLoot()
	return self:IsGameObject() and self:GetBit(UNIT_FIELD_GO_FLAGS, GO_FLAG_LOOT)
end    

--是否玩家精灵
function RobotdUnit:IsPlayer()
	return self:GetTypeID() == TYPEID_PLAYER
end

--是否普通生物
function RobotdUnit:IsUnit()
	return self:GetTypeID() == TYPEID_UNIT
end

--获取模板ID
function RobotdUnit:GetEntry()
	return self:GetUInt16(UNIT_FIELD_UINT16_0, 0)
end

--获取所在地图模板ID
function RobotdUnit:GetMapID()
	return self:GetUInt16(UNIT_FIELD_UINT16_1, 0)
end

--获取所在地图实例ID
function RobotdUnit:GetInstanceID()
	return self:GetUInt32(UNIT_FIELD_INSTANCE_ID)
end

function RobotdUnit:IsAlive()
	return self:GetByte(UNIT_FIELD_BYTE_0, 1) == DEATH_STATE_ALIVE
end

--获取当前坐标
function RobotdUnit:GetPos()
	return unitLib.getUnitPos(self.ptr)
end

--设置当前坐标
function RobotdUnit:SetPos(x, y)
	return unitLib.setUnitPos(self.ptr, x, y)
end

--获取距离
function RobotdUnit:GetDistance(unit)
	if(unit:GetInstanceID() ~= self:GetInstanceID())then
		return 999999999
	end
	return unitLib.getDistance(self.ptr, unit.ptr)
end

--获取距离
function RobotdUnit:GetDistanceByPos(x, y)
	return unitLib.getDistanceByPos(self.ptr, x, y)
end

--获取生存状态
function RobotdUnit:GetDeathState()
	return self:GetByte(UNIT_FIELD_BYTE_0, 1)
end

--是否死亡状态
function RobotdUnit:IsDie()
	return self:GetDeathState() ~= DEATH_STATE_ALIVE
end

--是否移动状态
function RobotdUnit:IsMoving()
	return unitLib.isMoving(self.ptr)
end

--停止移动状态
function RobotdUnit:stopMoving(x,y)
	return unitLib.stopMoving(self.ptr,x,y)
end

--判断是否NPC
function RobotdUnit:IsNpc()
	return self:IsUnit() and self:GetUInt32(UNIT_FIELD_NPC_FLAG) ~= 0
end

--是否传送点
function RobotdUnit:IsTeleteport()
	return self:IsGameObject() and self:GetBit(UNIT_FIELD_GO_FLAGS, GO_FLAG_TELE)
end

--传送点传送到的地图id
function RobotdUnit:GetToMapId()
	assert(self:IsTeleteport())
	return self:GetUInt32(GO_TELETEPORTER_TO_MAPID)
end

--获取当前内力值
function RobotdUnit:GetCurMagic()
	return self:GetUInt32(UNIT_FIELD_MAGIC)
end

--获取最大内力值
function RobotdUnit:GetMaxMagic()
	return self:GetUInt32(UNIT_FIELD_MAXMAGIC)
end

--获取当前HP
function RobotdUnit:GetCurHealth()
	return self:GetUInt32(UNIT_FIELD_HEALTH)
end

--获取最大HP
function RobotdUnit:GetMaxHealth()
	return self:GetUInt32(UNIT_FIELD_MAX_HEALTH)
end

--是否不在障碍点上
function RobotdUnit:IsCanRun()
	return mapLib.IsCanRun(self:GetMapID(), self:GetPos())
end

return RobotdUnit
