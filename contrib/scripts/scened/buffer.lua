-------------------------------------------------------------------------------
----------------------------buff相关-------------------------------------------
--周期性buff
function DoBuffTriggerScript(unit, buff_id, buffEffectId)
	local unitInfo = UnitInfo:new{ptr = unit}
	local giver_ptr = unitLib.GetBuffGiverUnit(unit, buff_id)
	
	local config = tb_buff_template[buff_id]
	if not config then
		return
	end
	
	if buff_id == BUFF_HEAL then --百分比回血
		local prevHP = binLogLib.GetUInt32(unit, UNIT_FIELD_HEALTH)
		local value = tb_buff_effect[buffEffectId].value
		local max_hp = unitInfo:GetMaxHealth()
		local add_hp = math.floor(max_hp * value / 100)
		unitInfo:ModifyHealth(add_hp)
		
		local mapid = unitLib.GetMapID(unit)
		local map_ptr = unitLib.GetMap(unit)
		local mapInfo = Select_Instance_Script(mapid):new {ptr = map_ptr}
		mapInfo:OnPlayerHurt(unit, unit, -add_hp)
		if add_hp < 0 then
			-- 周期性掉血触发
			OnHpChanged(unit, math.ceil(prevHP/max_hp*100))
		end
	end
end

--BUFF结束时需要做的一些事情
function DoBuffOverScript(unit, buff_id, buffEffectId)
	local unitInfo = UnitInfo:new{ptr = unit}
	--反击护盾
	if buff_id == BUFF_FANJI_HUDUN then
		local cast_x, cast_y = unitLib.GetPos(unit)
		local targets = mapLib.GetCircleTargets(cast_x, cast_y, 8, unit, TARGET_TYPE_ENEMY)	
		for key, attack_target in pairs(targets) do
			--目标不能是自己				--不能是友好的
			if attack_target ~= nil and attack_target ~= unit then
				AddSpellCastinfo(unit, attack_target, 5000, HITINFO_NORMALSWING, tb_buff_template[buff_id].skill_id)
			end
		end
	--战斗状态结束后
	elseif buff_id == BUFF_ZHANDOU_STATE then
		SpelladdBuff(unit, BUFF_LINGTI, unit, 1, tb_buff_template[BUFF_LINGTI].duration)
	--妖术结束后
	elseif buff_id == BUFF_HUANHUA then
		unitInfo:SetCreatureSkin(0)
	end
	
	
	if buff_id >= BUFF_FANTAN then	--有buff
		local config = tb_buff_template[buff_id]
		if config and config.recalculation == 1 then	-- 有结束重算标识
			unitLib.SetCalculAttr(unit)
		end
		ClearBuffFlags(unit, buff_id)
	end
end

--清除buff对应的flag标识
function ClearBuffFlags(unit, buff_id)
	local unitInfo = UnitInfo:new{ptr = unit}
	---------------清空限制性buff的flag----------------------
	local cant_casts = config.cant_cast		--取出配置
	for k, buff in pairs(cant_casts) do
		if buff_id == buff then
			local flag = false
			for k1, buff1 in pairs(cant_casts) do
				if buff1 ~= buff_id and unitLib.HasBuff(unit, buff1)  then
					flag = true		--如果玩家身上还有类似的buff，则暂不清flag
					break	
				end		
			end
			if not flag then
				unitInfo:UnSetUnitFlags(UNIT_FIELD_FLAGS_CANT_CAST)
			end	
			break
		end
	end
	local cant_moves = config.cant_move		--取出配置
	for k, buff in pairs(cant_moves) do
		if buff_id == buff then
			local flag = false
			for k1, buff1 in pairs(cant_moves) do
				if buff1 ~= buff_id and unitLib.HasBuff(unit, buff1) then
					flag = true		--如果玩家身上还有类似的buff，则暂不清flag
					break	
				end		
			end
			if not flag then
				unitInfo:UnSetUnitFlags(UNIT_FIELD_FLAGS_CANT_MOVE)
			end	
			break
		end
	end
	-------------------清空限制性buff的flag end-----------------	
end

--计算BUFF属性 --加属性的BUFF在这搞就可以了
function DOComputeBuffAttr(unit,buff_id,buffEffectId)
	local unitInfo = UnitInfo:new{ptr = unit}
	
	local config = tb_buff_template[buff_id]
	if not config then
		return
	end
	
	-- 不需要计算
	if buffEffectId == 0 then
		return
	end	
	local value = tb_buff_effect[buffEffectId].value
	
	-- 改变属性的
	if #config.attr_id > 0 then
		for _, attrId in ipairs(config.attr_id) do
			local binlogIndx = GetAttrUnitBinlogIndex(attrId)
			local curr = binLogLib.GetUInt32(unit, binlogIndx)
			binLogLib.SetUInt32(unit, binlogIndx, math.floor(curr * (100 + value) / 100))
		end
	elseif buff_id == BUFF_ALLATTR then
		unitInfo:addAllAttrRate(value)
	end
end

-----------------------------------------------------------------
--脚本调用脚本

--------------------------------系统给玩家增加buff------------------------------------
function SystemAddBuff(unit, buffId, buffEffectId, bonus_time)
	unitLib.SystemAddBuff(unit, buffId, buffEffectId, bonus_time)
end

-------------------增加Buff----------------------
function SpelladdBuff(unit, buff_id, buff_giver, buffEffectId, bonus_time, reserve)
	if unit == nil then
		outFmtError("SpelladdBuff unit == nil buff_id = %d",buff_id)
		return
	end
	local unitInfo = UnitInfo:new{ptr = unit}
	if unitInfo:IsAlive() == false then
		outFmtError("SpelladdBuff unit %d is not alive buff_id = %d", unitInfo:GetEntry(), buff_id)
		return
	end
	
	if buff_id == BUFF_ONEPOINTFIVE_JINGYAN or buff_id == BUFF_TOW_JINGYAN or buff_id == BUFF_THREE_JINGYAN or buff_id == BUFF_FIVE_JINGYAN then
		--经验丹		
		if unitLib.HasBuff(unit, buff_id) then
			--已经有这个buff了就处理叠加
			local duration = unitLib.GetBuffDuration(unit, buff_id)
			if duration + bonus_time >= 65535 then
				duration = 65534
			else
				duration = duration + bonus_time
			end
			unitLib.SetBuffDuration(unit, buff_id, duration)
		else
			--添加buff
			unitLib.AddBuff(unit, buff_id, buff_giver, buffEffectId, bonus_time)
			--添加了buff的同时移除其他经验buff
			local jingyan = {BUFF_ONEPOINTFIVE_JINGYAN, BUFF_TOW_JINGYAN, BUFF_THREE_JINGYAN, BUFF_FIVE_JINGYAN}
			for _, id in ipairs(jingyan) do
				if id ~= buff_id and unitLib.HasBuff(unit, id) then
					unitLib.RemoveBuff(unit, id)
				end
			end
		end

	elseif buff_id == BUFF_GANGCI then				--钢刺
		if unitLib.HasBuff(unit, buff_id) == false then
			unitLib.AddBuff(unit,buff_id,buff_giver,buffEffectId,bonus_time,reserve)
			AddSpellCastinfo(buff_giver, unit, 100 + unitInfo:GetHealth()*0.01, HITINFO_NORMALSWING, tb_buff_template[buff_id].skill_id)
		else
			unitLib.SetBuffDuration(unit, buff_id, tb_buff_template[buff_id].duration)
			unitLib.SetBuffReserve(unit, buff_id, unitLib.GetBuffReserve(unit,buff_id) + reserve)
			AddSpellCastinfo(buff_giver, unit, 100 + unitInfo:GetHealth()*unitLib.GetBuffReserve(unit,buff_id)/100, HITINFO_NORMALSWING, tb_buff_template[buff_id].skill_id)
		end
	
	elseif buff_id == BUFF_FANJI_HUDUN then				--反击护盾
		if unitLib.HasBuff(unit, buff_id) == false then
			unitLib.AddBuff(unit,buff_id,buff_giver,buffEffectId,bonus_time,reserve)
		end	
	elseif buff_id == BUFF_JUMP_JUMP then		--连跳BUFF
		if unitLib.HasBuff(unit, buff_id) == false then
			unitLib.AddBuff(unit,buff_id,buff_giver,1,bonus_time)
		else
			unitLib.RemoveBuff(unit, buff_id)
			unitLib.AddBuff(unit,buff_id,buff_giver,2,bonus_time)
		end
	elseif buff_id == BUFF_SHIHUA 	--石化
			or buff_id == BUFF_YUNXUAN 	--晕眩
			or buff_id == BUFF_YINCHANG	--吟唱
			or buff_id == BUFF_ZHIKONG	--滞空
			or buff_id == BUFF_XUANYUN_BUFF 	--晕眩
		 then		
		if unitLib.HasBuff(unit, buff_id) == false then
			unitLib.AddBuff(unit,buff_id,buff_giver,0,bonus_time,reserve or 0)
		else
			unitLib.RemoveBuff(unit, buff_id)
			unitLib.AddBuff(unit,buff_id,buff_giver,0,bonus_time,reserve or 0)
		end	
		
		--限制施法
		if unitInfo:GetUnitFlags(UNIT_FIELD_FLAGS_CANT_CAST)==false then
			unitInfo:SetUnitFlags(UNIT_FIELD_FLAGS_CANT_CAST)
		end
		
		--[[
		--如果没有施法移动BUFF限制移动
		if unitInfo:GetUnitFlags(UNIT_FIELD_FLAGS_CANT_MOVE)==false then
			unitInfo:SetUnitFlags(UNIT_FIELD_FLAGS_CANT_MOVE)
		end
		--]]
	elseif buff_id == BUFF_DINGSHEN or buff_id == BUFF_HDQS_NET 
			or buff_id == BUFF_DINGSHEN_BUFF then --定身、网
		if unitLib.HasBuff(unit, buff_id) == false then
			unitLib.AddBuff(unit,buff_id,buff_giver,1,bonus_time)
		else
			unitLib.RemoveBuff(unit, buff_id)
			unitLib.AddBuff(unit,buff_id,buff_giver,1,bonus_time)
		end	
		--限制移动
		if unitInfo:GetUnitFlags(UNIT_FIELD_FLAGS_CANT_MOVE)==false then
			unitInfo:SetUnitFlags(UNIT_FIELD_FLAGS_CANT_MOVE)
		end
	elseif buff_id == BUFF_CHENMO then		--沉默
		if unitLib.HasBuff(unit, buff_id) == false then
			unitLib.AddBuff(unit,buff_id,buff_giver,1,bonus_time)
		else
			unitLib.RemoveBuff(unit, buff_id)
			unitLib.AddBuff(unit,buff_id,buff_giver,1,bonus_time)
		end	
		--限制施法
		if unitInfo:GetUnitFlags(UNIT_FIELD_FLAGS_CANT_CAST)==false then
			unitInfo:SetUnitFlags(UNIT_FIELD_FLAGS_CANT_CAST)
		end
	elseif buff_id == BUFF_HUDUN_BUFF then		--护盾buff
		local targetInfo = unitInfo
		local monster = creatureLib.GetMonsterHost(buff_giver)
		if monster then
			targetInfo = UnitInfo:new{ptr = monster}
		end
		reserve = targetInfo:GetMaxHealth() * reserve / 100
		if unitLib.HasBuff(unit, buff_id) == false then
			unitLib.AddBuff(unit,buff_id,buff_giver,buffEffectId,bonus_time,reserve or 0)
		else
			unitLib.RemoveBuff(unit,buff_id)
			unitLib.AddBuff(unit,buff_id,buff_giver,buffEffectId,bonus_time,reserve or 0)
		end
	--如果不需要特殊处理就这么弄
	else
		if unitLib.HasBuff(unit, buff_id) == false then
			unitLib.AddBuff(unit,buff_id,buff_giver,buffEffectId,bonus_time,reserve or 0)
		else
			unitLib.RemoveBuff(unit,buff_id)
			unitLib.AddBuff(unit,buff_id,buff_giver,buffEffectId,bonus_time,reserve or 0)
		end
	end
	
	-- 判断是不是被吼中了
	if GetUnitTypeID(unit) == TYPEID_UNIT and buff_id == BUFF_ROAR then
		creatureLib.ModifyThreat(unit, buff_giver, 9999999)
	end
	
	OnPassiveEffect(unit, buff_giver, nil, PASSIVE_DISPATCH_TYPE_ON_BUFF, 0, {buff_id})
end

function getBuffFrequency(buffEffectId)
	if tb_buff_effect[buffEffectId] then
		return tb_buff_effect[buffEffectId].frequency
	end
	
	return 0
end