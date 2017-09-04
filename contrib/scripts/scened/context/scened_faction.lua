local ScenedFaction = class('ScenedFaction', BinLogObject)

function ScenedFaction:ctor()

end

--function ScenedFaction:

--读取当前bossid
function ScenedFaction:GetChallengeBossId()
		return self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_ID)
end


--开始时间
function ScenedFaction:GetChallengeBossStartTime()
		return self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_START_TIME)
end
--boss hp pos
function ScenedFaction:GetChallengeBossHpRate()
		return self:GetUInt32(FACTION_INT_FIELD_CHALLENGE_BOSS_HP_RATE)
end
function ScenedFaction:GetChallengeBossPos()
		return self:GetUInt16(FACTION_INT_FIELD_CHALLENGE_BOSS_POS,0),self:GetUInt16(FACTION_INT_FIELD_CHALLENGE_BOSS_POS,1)
end
--target hp pos 
function ScenedFaction:GetProtectTargetHpRate()
		return self:GetUInt32(FACTION_INT_FIELD_PROTECT_TARGET_HP_RATE)
end
function ScenedFaction:GetProtectTargetPos()
		return self:GetUInt16(FACTION_INT_FIELD_PROTECT_TARGET_POS,0),self:GetUInt16(FACTION_INT_FIELD_PROTECT_TARGET_POS,1)
end
--家族等级
function ScenedFaction:GetFactionLevel()
	return self:GetUInt32(FACTION_INT_FIELD_LEVEL)
end
--

return ScenedFaction
