--战斗信息
FightingInfo = class('FightingInfo', assert(BinLogObject))
FightingInfo.ptr = getFightingInfo()

	--下一个战斗伤害结构体
function FightingInfo:Next()
	self:AddUInt32(MAX_FIGHTING_INFO_INT_NOW_INDEX, 1)
end

--本伤害结构体int开始下标
function FightingInfo:Start()
	return MAX_FIGHTING_INFO_INT_START + self:GetAttackStrutCount() * MAX_FIGHTING_INFO_INT
end
--本伤害结构体string开始下标
function FightingInfo:StringStart()
	return FIGHTING_INFO_STRING_FIELD_START + self:GetAttackStrutCount() * MAX_FIGHTING_INFO_STRING
end

--获得伤害结构体个数
function FightingInfo:GetAttackStrutCount()
	return self:GetUInt32(MAX_FIGHTING_INFO_INT_NOW_INDEX)
end

--设置施法者的guid
function FightingInfo:SetCasterGuid( val)
	self:SetStr(self:StringStart() + FIGHTING_INFO_STRING_CASTER, val)
end
--设置目标的guid
function FightingInfo:SetTargetGuid( val)
	self:SetStr(self:StringStart() + FIGHTING_INFO_STRING_TARGET, val)
end
--设置伤害
function FightingInfo:SetDamage( val)
	self:SetDouble(self:Start() + FIGHTING_INFO_INT_VALUES, val)
end
--根据索引取伤害
function FightingInfo:GetDamageByOffset( off)
	return self:GetDouble(MAX_FIGHTING_INFO_INT_START + off * MAX_FIGHTING_INFO_INT + FIGHTING_INFO_INT_VALUES)
end	
--根据索引取伤害
function FightingInfo:SetDamageByOffset( off, val)
	self:SetDouble(MAX_FIGHTING_INFO_INT_START + off * MAX_FIGHTING_INFO_INT + FIGHTING_INFO_INT_VALUES, val)
end	
--设置攻击类型（普通、暴击、躲避、格挡、致命一击、击飞）
function FightingInfo:SetAttackType( val)
	self:SetByte(self:Start() + FIGHTING_INFO_INT_UINT8, 3, val)
end
--获得攻击类型
function FightingInfo:GetAttackTypeByOffset( off)
	return self:GetByte(MAX_FIGHTING_INFO_INT_START + off * MAX_FIGHTING_INFO_INT + FIGHTING_INFO_INT_UINT8, 3)
end
--设置是否被杀标志非0表示本次伤害没有把对方杀死
function FightingInfo:SetIsKill( val)
	self:SetByte(self:Start() + FIGHTING_INFO_INT_UINT8, 0, val)
end
--设置施法者生物类型
function FightingInfo:SetCasterType( val)
	self:SetByte(self:Start() + FIGHTING_INFO_INT_UINT8, 1, val)
end
--设置目标生物类型
function FightingInfo:SetTargetType( val)
	self:SetByte(self:Start() + FIGHTING_INFO_INT_UINT8, 2, val)
end
--设置技能id
function FightingInfo:SetSpellId( val)
	self:SetUInt32(self:Start() + FIGHTING_INFO_INT_SPELL_ID, val)
end
--设置预留0
function FightingInfo:SetReserve0( val)
	self:SetUInt16(self:Start() + FIGHTING_INFO_INT_RESERVE_0, 0, val)
end
--设置预留1
function FightingInfo:SetReserve1(val)
	self:SetUInt16(self:Start() + FIGHTING_INFO_INT_RESERVE_0, 1, val)
end
--设置预留1
function FightingInfo:SetReserve2( val)
	self:SetUInt16(self:Start() + FIGHTING_INFO_INT_RESERVE_1, 0, val)
end
--设置预留1
function FightingInfo:SetReserve3( val)
	self:SetUInt16(self:Start() + FIGHTING_INFO_INT_RESERVE_1, 1, val)
end

--设置预留2
function FightingInfo:SetCasterIntGuid( val)
	self:SetUInt32(self:Start() + FIGHTING_INFO_INT_RESERVE_2, val)
end
--设置预留3
function FightingInfo:SetTargetIntGuid( val)
	self:SetUInt32(self:Start() + FIGHTING_INFO_INT_RESERVE_3, val)
end


return FightingInfo