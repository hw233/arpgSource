RobotdMapInfo = class('RobotdMapInfo', BinLogObject)

--构造函数
function RobotdMapInfo:ctor()

end

function RobotdMapInfo:GetInstanceID()
	return self:GetUInt32(MAP_INT_FIELD_INSTANCE_ID)
end
-- 百万大战准备结束时间
function RobotdMapInfo:GetMillion_War_Ready_End_Time()
	return self:GetUInt32(MAP_FIELD_INT_MILLION_WAR_READY_END_TM)
end
function RobotdMapInfo:GetMillionWinGuid()
	return self:GetStr(MAP_FIELD_STR_MILLION_WAR_WIN_GUID)
end

function RobotdMapInfo:GetBossCount()
	return self:GetUInt32(MAP_FIELD_INT_KFGBT_BOSSCOUNT)
end

function RobotdMapInfo:GetMapState()
	return self:GetByte(MAP_INT_FIELD_STATE, 1)
end
return RobotdMapInfo
