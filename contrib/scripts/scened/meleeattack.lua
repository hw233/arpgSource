----------------------------------------------------------
----------------------------------------------------------
--- 闪避率（万份比）=8000*(1-1/(1+MAX(防御方闪避-攻击方命中,0)*0.0002))+属性闪避率				
function GetLastEva(attacker, target) --获取命中率
	local attackInfo = UnitInfo:new{ptr = attacker}
	local targetInfo = UnitInfo:new{ptr = target}
	local eva = targetInfo:GetEva()
	local hit = attackInfo:GetHit()
	local gap_eva = eva - hit
	if gap_eva < 0 then  gap_eva = 0 end

	local last_hit = 8000 * (1 - 1 / (1 + gap_eva*0.0002)) + targetInfo:GetEvaRate()
	
	return last_hit
end

----暴击率（万份比）=10000*(1-1/(1+MAX(攻击方暴击-防御方抗暴,0)*0.0002))+属性暴击率
function GetLastCrit(attacker, target) 	--获取暴击率
	local attackInfo = UnitInfo:new{ptr = attacker}
	local targetInfo = UnitInfo:new{ptr = target}
	local crit = attackInfo:GetCrit()
	local crit_def = targetInfo:GetCritDef()
	local gap_crit = crit - crit_def
	if gap_crit < 0 then gap_crit = 0 end

	local crit_rate = 10000 * ( 1 - 1/(1 + gap_crit*0.0002 )) + attackInfo:GetCritRate()
	
	return crit_rate
end

--计算伤害值 (普通攻击和技能攻击伤害的计算都是用这个接口)
function calculDamage(attacker, target, spell_id, spell_lv) 
	local calcDamage = 0
	local attackInfo = UnitInfo:new{ptr = attacker}
	local targetInfo = UnitInfo:new{ptr = target}
	local attack_typeid = GetUnitTypeID(attacker)	
	local index = tb_skill_base[spell_id].uplevel_id[1] + spell_lv - 1	
	local g_min = attackInfo:GetMinDamage()
	local g_max = attackInfo:GetMaxDamage()
	local f = targetInfo:GetArmor()
	local p = tb_skill_uplevel[index].hurt_percent * 0.01
	local e = tb_skill_uplevel[index].hurt_add
	local s_add = attackInfo:GetAddDamage() * 0.0001
	local s_sub = targetInfo:GetSubDamage() * 0.0001
	local l_g = attackInfo:GetLevel()
	local l_f = targetInfo:GetLevel()
	local s_wushi = tb_skill_uplevel[index].cannot_defence_hure	
	--玩家攻击
	if attack_typeid == TYPEID_PLAYER then
		calcDamage = ((math.max(randIntD(g_min,g_max)-f,0) * p + e) * (1 + s_add - s_sub)) * (1 + math.min(math.max(l_g - l_f - 4,0),25) * 0.02) + s_wushi + 1
	--怪物攻击
	else
		local entry = attackInfo:GetEntry()
		local monster_type = tb_creature_template[entry].monster_type
		if monster_type == 0 then	--普通怪物
			calcDamage = ((math.max(randIntD(g_min,g_max)-f,0) * p + e) * (1 + s_add - s_sub)) + math.max((l_g - l_f - 4),0) * (0.15 * l_f + 10) + s_wushi + 1
		else	--boss
			calcDamage = ((math.max(randIntD(g_min,g_max)-f,0) * p + e) * (1 + s_add - s_sub)) + s_wushi + 1
		end
	end
	return calcDamage
end
