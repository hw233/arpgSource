local protocols = require('share.protocols')

--场景服
function  DoGMScripts(player_ptr, gm_commands, runtime)
	local result = ""
	
	if(gm_commands == "@hello")then
		result = "ni ye hello"
		return result
	end
	
	local tokens = lua_string_split(gm_commands," ")
	if(#tokens == 0) then
		--闹哪样啊??
		return result
	end
	local paras = {0}
	for i = 2, #tokens do
		paras[i] = tonumber(tokens[i])
		if(paras[i] == nil)then
			paras[i] = 0
		end
	end
	if(tokens[1] == "@开启所有地图")then
		if(player_ptr == nil)then
			return result
		end
		local player = UnitInfo:new{ptr = player_ptr}
		player:SetPlayerUInt32(PLAYER_EXPAND_INT_MAP_STATUS, 255)
	elseif(tokens[1] == "@消除冷却")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		for j = PLAYER_SCENED_INT_FIELD_SLOT_SPELL_0 + SLOT_SPELL_CD, PLAYER_SCENED_INT_FIELD_SLOT_SPELL_END - 1,  MAX_SLOT_ATTR_COUNT do				
			playerInfo:SetUInt32(j, 0)
		end
	elseif(tokens[1] == '@double')then
		if(player_ptr == nil)then
			return result
		end
		if(#paras < 3)then
			result = '参数数量不够'
			return result
		end
		if(paras[2] >= MAX_PLAYER_EXPAND_INT_FIELD)then
			result = '越界了'
			return result
		end
		local target_ptr = unitLib.GetTarget(player_ptr)
		if(target_ptr == nil)then
			binLogLib.SetDouble( playerLib.GetSession(player_ptr), paras[2], paras[3])
		else
			if(GetUnitTypeID(target_ptr) == TYPEID_PLAYER)then
				binLogLib.SetDouble( playerLib.GetSession(target_ptr), paras[2], paras[3])
			end
		end
	elseif(tokens[1] == '@下标')then
		if(player_ptr == nil)then
			return result
		end
		if(#paras < 3)then
			result = '参数数量不够'
			return result
		end
		if(paras[2] >= MAX_PLAYER_EXPAND_INT_FIELD)then
			result = '越界了'
			return result
		end
		local target_ptr = unitLib.GetTarget(player_ptr)
		if(target_ptr == nil)then
			binLogLib.SetUInt32( playerLib.GetSession(player_ptr), paras[2], paras[3])
		else
			if(GetUnitTypeID(target_ptr) == TYPEID_PLAYER)then
				binLogLib.SetUInt32( playerLib.GetSession(target_ptr), paras[2], paras[3])
			end
		end
	elseif(tokens[1] == '@精灵下标')then
		if(player_ptr == nil)then
			return result
		end
		if(#paras < 3)then
			result = '参数数量不够'
			return result
		end
		if(paras[2] >= MAX_PLAYER_EXPAND_INT_FIELD)then
			result = '越界了'
			return result
		end
		local target_ptr = unitLib.GetTarget(player_ptr)
		if(target_ptr == nil)then
			binLogLib.SetUInt32(player_ptr, paras[2], paras[3])
		else
			binLogLib.SetUInt32(target_ptr, paras[2], paras[3])
		end
	elseif(tokens[1] == "@技能")then
		if(player_ptr == nil)then
			return result
		end
		if(#paras >= 2)then
			local jn = paras[2]
			local level = 1
			if(#paras >= 3)then
				level = paras[3]
			end
			local playerInfo = UnitInfo:new{ptr = player_ptr}
			-- for j = PLAYER_SCENED_INT_FIELD_SLOT_SPELL_0, PLAYER_SCENED_INT_FIELD_SLOT_SPELL_END - 1 do				
				-- playerInfo:SetUInt32(j, 0)
			-- end
			playerInfo:SetPlayerUInt32(PLAYER_SCENED_INT_FIELD_SLOT_SPELL_0 + MAX_SLOT_ATTR_COUNT + SLOT_SPELL_ID, jn)
			playerInfo:SetPlayerUInt32(PLAYER_SCENED_INT_FIELD_SLOT_SPELL_0 + MAX_SLOT_ATTR_COUNT + SLOT_SPELL_CD, 0)
			playerInfo:SetPlayerUInt32(PLAYER_SCENED_INT_FIELD_SLOT_SPELL_0 + MAX_SLOT_ATTR_COUNT + SLOT_SPELL_LV, level)
		end
	elseif(tokens[1] == '@buff')then
		if(player_ptr == nil)then
			return result
		end
		if(#paras >= 2)then
			local buff = tonumber(paras[2])
			local level = 1
			if(#paras >= 3)then
				level = paras[3]
			end
			local buff_time = 65535
			if(#paras >= 4)then
				buff_time = paras[4]
			end
			local reserve = 0
			if(#paras >= 5)then
				reserve = paras[5]
			end
			if(buff > 100)then
				SpelladdBuff(player_ptr, buff, player_ptr, level, buff_time, reserve)
			end
		end
	elseif(tokens[1] == '@removebuff')then
		if(player_ptr == nil)then
			return result
		end
		if(#paras >= 2)then
			local buff = paras[2]
			unitLib.RemoveBuff(player_ptr, buff)
			ClearBuffFlags(player_ptr,buff)
		end		
	elseif(tokens[1] == "@clearbuff")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		for i = 0, MAX_BUFF-1, 1 do
			local buff_id = playerInfo:GetBuffByPos(i)
			if buff_id >= 101 then	--有buff
				unitLib.RemoveBuff(player_ptr, buff_id)	
				ClearBuffFlags(player_ptr,buff_id)
			end
		end
	elseif(tokens[1] == '@系数')then
		if(player_ptr == nil)then
			return result
		end
		if(#tokens >= 3)then
			local str = ''
			if(tokens[2] == '速度')then
				str = 'Speed'
			elseif(tokens[2] == '经验')then
				str = 'EXP'
			elseif(tokens[2] == '爆率')then
				str = 'BurstRate'
			elseif(tokens[2] == '攻击')then
				str = 'Damage'
			elseif(tokens[2] == '防御')then
				str = 'DEF'
			elseif(tokens[2] == '身法')then
				str = 'Hit'
			elseif(tokens[2] == '生命')then
				str = 'HP'
			elseif(tokens[2] == '内力')then
				str = 'MP'
			elseif(tokens[2] == '暴击')then
				str = 'Crit'
			elseif(tokens[2] == '闪避')then
				str = 'Eva'
			elseif(tokens[2] == '攻速')then
				str = 'AttackSpeed'	
			elseif(tokens[2] == '全属性')then
				str = 'FullProperty'
			else
				return '未指定属性'
			end
			playerLib.SetCoefficient(player_ptr, str, paras[3])
			unitLib.SetCalculAttr(player_ptr)
		end
	elseif(tokens[1] == '@格挡')then
		if(player_ptr == nil)then
			return result
		end
		
		if(#tokens == 2 )then
			local map = unitLib.GetMap(player_ptr)
			mapLib.DelTimer(map, 'OnRecoverGedang')
			mapLib.AddTimer(map, 'OnRecoverGedang', paras[2])
		end		
	elseif(tokens[1] == '@spellcd') then	
		if (player_ptr == nil) then
			return result
		end
		
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		for i = PLAYER_SCENED_INT_FIELD_SLOT_SPELL_0, PLAYER_SCENED_INT_FIELD_SLOT_SPELL_END-1, MAX_SLOT_ATTR_COUNT do
			playerInfo:SetPlayerUInt32(i + SLOT_SPELL_CD, 0)	--设置公共cd			
		end
	elseif(tokens[1] == '@在线时长') then
		if(#paras >= 2)then
			local onlinetime = paras[2]
			local playerInfo = UnitInfo:new{ptr = player_ptr}
			playerInfo:SetPlayerUInt32(PLAYER_FIELD_ONLINE_TIME, onlinetime)
		end
	elseif(tokens[1] == '@技能信息')then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		--打印技能槽信息
		for i = PLAYER_SCENED_INT_FIELD_SLOT_SPELL_0, PLAYER_SCENED_INT_FIELD_SLOT_SPELL_END-1, MAX_SLOT_ATTR_COUNT do
			if(playerInfo:GetPlayerUInt32(i + SLOT_SPELL_ID) ~= 0)then
				outDebug('-------------------------------------------')
				outDebug('playerguid = '..playerInfo:GetPlayerGuid()..'		name = '..playerInfo:GetName())
				outDebug('runtime = '..runtime)
				outDebug('spell_id = '..playerInfo:GetPlayerUInt32(i + SLOT_SPELL_ID))
				outDebug('spell_cd = '..playerInfo:GetPlayerUInt32(i + SLOT_SPELL_CD))			
				outDebug('-------------------------------------------')				
				
			end
		end
	elseif(tokens[1] == '@掉落物品')then
		local item_entry = 101
		local num = 1
		local item_num = 1
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local map = unitLib.GetMap(player_ptr)
		local isbind = ITEM_BIND_NONE
		local fcm = playerInfo:GetFCMLoginTime()
		if(#paras >= 2 and paras[2] > 0)then
			item_entry = paras[2]
		end
		if(#paras >= 3 and paras[3] > 0)then
			num = paras[3]
		end
		if(#paras >= 4 and paras[4] > 0)then
			item_num = paras[4]
		end

		if item_entry <= 100 then
			isbind = 1
		end
		
		for i=1, num do				
			AddLootGameObject(map, player_ptr, "", item_entry, 1, fcm)
		end
		
	elseif(tokens[1] == "@无双乱舞")then
		local num = 50
		if(#tokens >= 2 and tonumber(tokens[2]) > 0)then
			num = tonumber(tokens[2])
		end
		local recalcu_ptr = playerLib.GetEquipAttrs(player_ptr)
		if(recalcu_ptr)then
			local recalcuInfo = ReCalculate:new{ptr = recalcu_ptr}
			recalcuInfo:SetWSLWTarget(num)
		end
	elseif(tokens[1] == "@全图秒杀")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local map_ptr = unitLib.GetMap(player_ptr)
		local mapid = unitLib.GetMapID(player_ptr)
		local allPlayers = mapLib.GetAllPlayer(map_ptr)
		if #allPlayers == 1 then
			local player = allPlayers[1]
			local allCreatures = mapLib.GetAllCreature(map_ptr)
			for key, creature in pairs(allCreatures) do
				local creatureInfo = UnitInfo:new{ptr = creature}
				if creatureInfo then
					unitLib.KillUnit(player, creature) --秒杀生物
				end
			end	
		end
	elseif(tokens[1] == '@全服杀怪数')then
		globalValue:SetWorldKillCreatureNum(paras[2] or 1)
	elseif(tokens[1] == '@打印魔将')then
		outDebug("kill creature num = "..globalValue:GetKillCreatureNum())
		outDebug("creature mapid = "..globalValue:GetCurSuperBossMapId())
		outDebug("creature posx = "..globalValue:GetCurSuperBossPosX())
		outDebug("creature posy = "..globalValue:GetCurSuperBossPosY())
		outDebug("boss num = "..globalValue:GetCurSuperBossNum())
		outDebug("boss string = "..globalValue:GetCurSuperBossInfo())
	elseif(tokens[1] == '@精英怪')then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local num = 72000
		if(#paras >= 2 and paras[2] > 0)then
			num = paras[2]
		end
		globalValue:SetKillCreatureNum(0)
		globalValue:SetCurSuperBossMapId(0)
		globalValue:SetCurSuperBossNum(0)
		globalValue:SetCurSuperBossInfo("")
	elseif(tokens[1] == '@武斗')then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		if(#paras < 2)then
			return result
		end
		local type_ = paras[2]
		if(type_ ~= 0 and #paras < 3)then
			return result
		end
		local val = paras[3]
		if(type_ == 0)then --重置
			playerInfo:Set32he1TotalCount(0)
			playerInfo:Set32he1FinalCount(0)
			playerInfo:Set32he1ChampionCount(0)
			playerInfo:Set32he1EliminateCount(0)
			playerInfo:Set32he1JifenNum(0)
		elseif(type_ == 1)then
			playerInfo:Set32he1TotalCount(val)
		elseif(type_ == 2)then
			playerInfo:Set32he1FinalCount(val)
		elseif(type_ == 3)then
			playerInfo:Set32he1ChampionCount(val)
		elseif(type_ == 4)then
			playerInfo:Set32he1EliminateCount(val)
		elseif(type_ == 5)then
			playerInfo:Set32he1JifenNum(val)
		end
	elseif(tokens[1] == '@副本令')then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local num = 0
		if(#paras >= 2 and paras[2] >= 0)then
			num = paras[2]
		end
		playerInfo:SetPlayerCurEnergyCount(num)	
	elseif(tokens[1] == '@穿云箭')then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		for i = 1, MAX_ZBTX_FACTION - 1 do
			ZbtxFactionInfo:SetZbtxFactionCyjCD(i, 0)
		end
	elseif(tokens[1] == "@printbuff")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local num = 0
		if(#paras >= 2 and paras[2] >= 1)then
			local target = unitLib.GetTarget(player_ptr)
			if target then
				playerInfo = UnitInfo:new{ptr = target}
			end
		end
		if playerInfo:GetTypeID() == TYPEID_PLAYER then
			outDebug(string.format("----------player guid is %s ------------- ", playerInfo:GetPlayerGuid()))
		else
			outDebug(string.format("----------target guid is %s ------------- ", playerInfo:GetGuid()))
		end
		if playerInfo:GetUnitFlags(UNIT_FIELD_FLAGS_CANT_CAST) then
			outDebug("----------unit is cannot cast state------------- ")
		end
		if playerInfo:GetUnitFlags(UNIT_FIELD_FLAGS_CANT_MOVE) then
			outDebug("----------unit is cannot move state------------- ")
		end
		outDebug("-----print buff start------")
		for i = 0, MAX_UNIT_BUFF - 1, 1 do
			local buffid = playerInfo:GetUInt16(UNIT_FIELD_BUFF + math.floor(i/2) ,math.fmod(i,2))
			local tm = playerInfo:GetUInt16(UNIT_FIELD_BUFF_TM + math.floor(i/2) ,math.fmod(i,2))
			local lv = playerInfo:GetByte(UNIT_FIELD_BUFF_LEVEL + math.floor(i/4),math.fmod(i,4))
			local guid = playerInfo:GetUInt32(UNIT_FIELD_BUFF_CASTER_GUID + i)
			local reserve = playerInfo:GetUInt32(UNIT_FIELD_BUFF_RESERVE + i)
			if buffid > 0 then
				outDebug("i = "..i.."  buffid = "..buffid.."  time = "..tm.."  lv = "..lv.."  guid = "..guid.."  reserve = "..reserve)
			end
		end
		outDebug("-----print buff over------")
		
	elseif(tokens[1] == '@打宝塔传送')then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local num = 1
		if(#paras >= 2 and paras[2] >= 1 and paras[2] <= 100)then
			num = paras[2]
		end
		-- 设置进入打宝塔层数
		playerInfo:SetEnterDabaotaLevel(num)
		playerInfo:SetEnterDabaotaEndTm(os.time() + 3600)
		local general_id = string.format("dabaota_%03d", num)
		if num <= 20 then
			playerLib.Teleport(player_ptr,61,14,48, 0, general_id, false)
		else
			playerLib.Teleport(player_ptr,33,19,53, 0, general_id, false)
		end
	elseif(tokens[1] == "@闯塔令")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		if(#paras >= 2 and paras[2] >= 0 and paras[2] <= 6)then
			playerInfo:SetPlayerCurDabaotaCount(paras[2])
		end
	elseif(tokens[1] == "@打老鼠")then
		local num = 30
		if(#paras >= 2 and paras[2] >= 0 and paras[2] <= 6)then
			num = paras[2]
		end
		local start_time = os.time() + num
		globalValue:SetDaLaoShuEnterTm(start_time - 60)
		globalValue:SetDaLaoShuStartTm(start_time)
		globalValue:SetDaLaoShuEndTm(start_time + 600)
	elseif(tokens[1] == "@打印血量")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		outDebug("-----------hp = "..playerInfo:GetHealth())
	elseif(tokens[1] == "@打老鼠传送")then
		local faction = app.objMgr:getObj(player:GetFactionId())
		if (faction ~= nil) then
			local guid = faction:GetGuid()
			if guid ~= nil then
				playerLib.Teleport(player_ptr, DALAOSHU_MAPID, 59, 56, guid)
			end
		end
	elseif(tokens[1] == "@战斗力")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		if #paras >= 2 and paras[2] >= 0 then
			playerInfo:SetForce(paras[2])
		end
	
	elseif(tokens[1] == "@机关兽")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local id = 1
		if #paras >= 2 and paras[2] >= 1 and paras[2] <= 9 then
			id = paras[2]
		end
		playerInfo:RefreshPets(id)
	elseif(tokens[1] == "@屋舍c")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local str = playerInfo:GetPlayerStr(PLAYER_STRING_FIELD_TELEPORT_GUID)
		print('===========================wushe str: '..str)
	elseif(tokens[1] == "@使用技能")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		unitLib.CastSpell(player_ptr, SPELL_ID_TIANSHU1, 0, 0)
	elseif(tokens[1] == "@激活天书")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local id = 0
		if #paras >= 2 and paras[2] >=0 and paras[2] <= 3 then
			id = paras[2]
		end
		if #paras >= 3 and paras[3] >=0 then
			if paras[3] < OPEN_TIANSHU_SPELL_CONFIG[id][2] then
				playerInfo:SetPlayerByte(PLAYER_SCENED_INT_FIELD_TSJNJ_INFO_START + id*MAX_SLOT_TSJNJ_SPELL + SLOT_TSJNJ_SPELL_COUNT, 0, paras[3])
			elseif paras[3] == OPEN_TIANSHU_SPELL_CONFIG[id][2] then
				playerInfo:SetTsjnjSpellLevel(id, 1)
				playerInfo:SetPlayerByte(PLAYER_SCENED_INT_FIELD_TSJNJ_INFO_START + id*MAX_SLOT_TSJNJ_SPELL + SLOT_TSJNJ_SPELL_COUNT, 0, OPEN_TIANSHU_SPELL_CONFIG[id][2])
			end
		end
	elseif(tokens[1] == "@被咬信息")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		outDebug("-----------"..playerInfo:GetDogBiteInfo())
	elseif(tokens[1] == "@技能特效")then
		local ntype = 1
		if #paras >= 2 and paras[2] >=1 and paras[2] <= 3 then
			ntype = paras[2]
		end
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local config = active_wslw_config[ntype]
		if config == nil or config[1] == 0 then		-- 没有这个特效id
			outDebug(" error : CMSG_ACTIVE_WSLW_SPELL config == nil ntype = "..ntype)
			return result
		end
		if playerInfo:GetPlayerScenedFlags(config[1]) then	-- 已激活
			outDebug(" error : CMSG_ACTIVE_WSLW_SPELL spell has active ntype = "..ntype)
			return result
		end
		if playerInfo:GetLevel() < config[2] then	-- 等级不足
			outDebug(" error : CMSG_ACTIVE_WSLW_SPELL player level bu zu, need level = "..config[2])
			return result
		end
		playerInfo:SetPlayerScenedFlags(config[1])	-- 成功激活了
	elseif(tokens[1] == "@测试函数")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local cas_x, cas_y = unitLib.GetPos(player_ptr)
		dst_x, dst_y = cas_x + 5, cas_y + 10
		local target = unitLib.GetTarget(player_ptr)	--获得攻击目标
		if target then
			dst_x, dst_y = unitLib.GetPos(target)
		end
		local targets = mapLib.GetObliqueRectTargets(cas_x + 5, cas_y + 7, cas_x - 5, cas_y - 7, player_ptr, TARGET_TYPE_ENEMY, 7)
		--local targets = mapLib.GetRectTargets(cas_x-7, cas_y+7, cas_x+7, cas_y-7, player_ptr, TARGET_TYPE_ENEMY)
		outDebug("-----------------target num = "..#targets)
		for k, unit in pairs(targets) do
			local dst_x, dst_y = unitLib.GetPos(unit)
			outDebug("------k = "..k.." --x = "..dst_x.." ---y = "..dst_y)
			unitLib.KillUnit(player_ptr, unit)
		end
	elseif(tokens[1] == "@测试技能")then
		local target = unitLib.GetTarget(player_ptr)	--获得攻击目标
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local dst_x, dst_y = unitLib.GetPos(player_ptr)
		if target then
			dst_x, dst_y = unitLib.GetPos(target)
		end
		local spellid = 43
		if #paras >= 2 and paras[2] >=1 and paras[2] <= 200 then
			spellid = paras[2]
		end
		DoSpellCastScript(player_ptr, target, dst_x, dst_y, spellid, 1, nil,"")
	elseif(tokens[1] == '@兵魂')then
		if #tokens >= 3 then
			local condition, count = tonumber(tokens[2]), tonumber(tokens[3])
			local playerInfo = UnitInfo:new{ptr = player_ptr}
			if(condition == 1)then
				playerInfo:SetPlayerUInt32(PLAYER_APPD_INT_FIELD_BINGHUN_ZDZZQ1, count)
				--print('------------condition1='..playerInfo:GetPlayerUInt32(PLAYER_APPD_INT_FIELD_BINGHUN_ZDZZQ1))
			elseif(condition == 2)then
				playerInfo:SetPlayerUInt32(PLAYER_APPD_INT_FIELD_BINGHUN_ZDZZQ2, count)
				--print('------------condition2='..playerInfo:GetPlayerUInt32(PLAYER_APPD_INT_FIELD_BINGHUN_ZDZZQ2))
			elseif(condition == 3)then
				playerInfo:SetPlayerUInt32(PLAYER_APPD_INT_FIELD_BINGHUN_ZDZZQ3, count)
				--print('------------condition3='..playerInfo:GetPlayerUInt32(PLAYER_APPD_INT_FIELD_BINGHUN_ZDZZQ3))
			end
		end
	elseif(tokens[1] == "@清空大沼泽信息")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		playerInfo:UnSetPlayerScenedFlags(PLAYER_SCENED_FLAS_DAZHAOZE_LOOT_FIRST)
		playerInfo:SetDazhaozeLootKillNum(0)
	elseif(tokens[1] == "@无敌")then
		if not unitLib.HasBuff(player_ptr, BUFF_WUDI) then
			SpelladdBuff(player_ptr, BUFF_WUDI, player_ptr, 1, 65535, 0)
		else
			unitLib.RemoveBuff(player_ptr, BUFF_WUDI)
		end
	elseif(tokens[1] == "@32he1")then
		local mapId = unitLib.GetMapID(player_ptr)
		local map_ptr = unitLib.GetMap(player_ptr)
		if IsFubenMap(mapId) then
			local mapInfo = Select_Instance_Script(mapId):new{ptr = map_ptr}
			if(tokens[2] == nil or (tokens[2] == "print" and tonumber(tokens[3])))then
				mapInfo:PrintAllInfo(tonumber(tokens[3]))
			else
				local old_lunshu = mapInfo:GetCurLunshu()
				local new_lunshu = tonumber(tokens[2])
				if(new_lunshu and not mapInfo.index_config[new_lunshu])then
					return result
				end
				mapInfo:AddCurLunshu(new_lunshu)
				local pre_str_start, str_start = mapInfo.index_config[old_lunshu][3], mapInfo.index_config[new_lunshu][3]
				local start, ends = mapInfo.index_config[new_lunshu][1], mapInfo.index_config[new_lunshu][2]
				for i = start, ends-1, 2 do
					local str_idx = pre_str_start + i - start
					local player_ptr1 = mapLib.GetPlayer(mapInfo.ptr, mapInfo:GetStr(str_idx))
					local set_index = str_start + i - start
					if(player_ptr1)then
						local playerInfo = UnitInfo:new{ptr = player_ptr1}
						mapInfo:SetByte(i, 1, playerInfo:GetKuafuNumber())
						mapInfo:SetStr(set_index, playerInfo:GetGuid())
					end
					local player_ptr2 = mapLib.GetPlayer(mapInfo.ptr, mapInfo:GetStr(str_idx+1))
					if(player_ptr2)then
						local playerInfo = UnitInfo:new{ptr = player_ptr2}
						mapInfo:SetByte(i+1, 1, playerInfo:GetKuafuNumber())
						mapInfo:SetStr(set_index+1, playerInfo:GetGuid())
					end
				end
			end
		end
	elseif(tokens[1] == "@打印被动技能")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		playerInfo:PrintPassiveSpellSlot()
	elseif(tokens[1] == "@当前地图玩家数")then
		local mapId = unitLib.GetMapID(player_ptr)
		local map_ptr = unitLib.GetMap(player_ptr)
		local mapInfo = Select_Instance_Script(mapId):new{ptr = map_ptr}
		local num = mapInfo:GetMapOnLinePlayerNum()
		outDebug("cur map on line player num ："..num.." map id："..mapId)
	elseif(tokens[1] == "@打印移速")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		outDebug("------move speed = "..playerInfo:GetByte(UNIT_FIELD_BYTE_0, 2))
	elseif(tokens[1] == "@重置世界boss")then
		for index=0, 9 do
			globalValue:SetWorldBossRefreshTm(index, 0)
			globalValue:SetWorldBossKillLog(index, "")
			for offset=0, 9 do
				globalValue:SetWorldBossRankDamNum(index, offset, 0)
				globalValue:SetWorldBossRankGuid(index, offset, "")
				globalValue:SetWorldBossRankName(index, offset, "")
			end
		end
	elseif(tokens[1] == "@重置公告信息")then
		for i=1, 101 do
			globalNoticeMessage:SetNoticeMessage(NOTICE_MESSAGE_INFO_TYPE_WORLD_BOSS,"")
		end
	elseif(tokens[1] == "@扣血")then
		local target = unitLib.GetTarget(player_ptr)	--获得攻击目标
		if target == nil then
			return result
		end
		local targetInfo = UnitInfo:new{ptr = target}
		if #paras >= 2 and paras[2] > 0 then
			if targetInfo:GetHealth() > paras[2] then
				 targetInfo:ModifyHealth(-paras[2])
			end
		end
	elseif(tokens[1] == "@世界boss")then
		local mapid = unitLib.GetMapID(player_ptr)
		local config = world_boss_pos_config[mapid]
		if config == nil then
			outError("OnTimer_WorldBossRefresh config == nil mapid = "..mapid)
			return result
		end
		local map_ptr = unitLib.GetMap(player_ptr)
		local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
		globalValue:SetWorldBossRefreshTm(config[6], os.time())
		mapLib.DelTimeStampTimer(map_ptr, 'OnTimer_WorldBossRefresh')
		mapInfo:OnTimer_WorldBossRefresh()
	elseif(tokens[1] == "@军团公告")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		sendFactionNoticeInfo(playerInfo:GetPlayerGuid(), 2, 100)
	elseif(tokens[1] == "@世界事件时间")then
		outDebug("----tm = "..os.date("%c",globalValue:GetCurWorldEventEndTm()))
	elseif(tokens[1] == "@重置打宝塔特殊次数")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		playerInfo:SetDBTSpecialCount(0)
	elseif(tokens[1] == "@精英怪杀怪数")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local mapid = unitLib.GetMapID(player_ptr)
		local offset = JINGYINGGUA_CONFIG[mapid]
		local num = 0
		if(#paras >= 2)then
			num = paras[2]
		end
		if offset then
			globalCounter:SetWorldKilledCount(offset, num)
		end
	elseif(tokens[1] == "@打印炸弹cd")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		outDebug("-----------run time = "..getMsTime())
		for i=0,  MAX_ZHADAN_NUM - 1, 1 do
			outDebug("-------- i = "..i.."---cd = "..playerInfo:GetAnFangZhaDanCd(i))
		end
	elseif(tokens[1] == "@打印打老鼠")then
		print(os.date("%c",globalValue:GetDaLaoShuEnterTm()),os.date("%c",globalValue:GetDaLaoShuStartTm()),os.date("%c",globalValue:GetDaLaoShuEndTm()))
	elseif(tokens[1] == "@清空铜钱副本排行")then
		for i=0, MAX_TONGQIAN_FUBEN_RANK_NUM-1, 1 do
			globalValue:SetTqFubenMoneyNum(i, 0)
			globalValue:SetTqFubenPlayerGuid(i, "")
			globalValue:SetTqFubenPlayerName(i, "")
		end
	elseif(tokens[1] == "@打印铜钱副本排行")then
		for i=0, MAX_TONGQIAN_FUBEN_RANK_NUM-1, 1 do
			outDebug("--num = "..globalValue:GetTqFubenMoneyNum(i).."--"..globalValue:GetTqFubenPlayerGuid(i)
			.."--"..globalValue:GetTqFubenPlayerName(i))
		end
	elseif(tokens[1] == "@英雄帖")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local ntype = 0
		if(#paras >= 2)then
			ntype = paras[2]
		end
		local val = 0
		if(#paras >= 3)then
			val = paras[3]
		end
		if ntype == 0 then
			playerInfo:UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_ENTER_YXT)
		elseif ntype == 1 then			-- 设置开启擂台数
			playerInfo:SetYxtOpenPlayerCount(val > 10 and 10 or val)
		elseif ntype == 2 then			-- 设置今日剩余次数
			playerInfo:SetYxtDailyPkCount(val > 255 and 255 or val)
		elseif ntype == 3 then			-- 打印应战者获胜奖励
			local config = YINGXIONGTIE_AWARD_CONFIG[val]
			if(config == nil)then
				return result
			end
			--保底奖励=0.25*接受者胜利总奖励/挑战人数+1
			local add_jf = 0	-- 累计已发放奖励
			local add_rs = 0	-- 已抽取人数
			local min_jf = math.floor(0.25*config[5]/config[1]+1)		-- 保底积分
			local max_jf, get_jf		-- 积分上限，玩家获得积分
			for i=1, config[1] do
		--不同抽取顺序的奖励上限=1.9*(接受者胜利总奖励-累积已发放奖励-挑战人数+已抽取人数)/(挑战人数-已抽取人数)
				local max_jf = math.floor(1.9*(config[5]-add_jf-config[1]+add_rs)/(config[1]-add_rs))
				if max_jf < min_jf then
					max_jf = min_jf
				end
				if add_rs == config[1] -1 then
					get_jf = config[5] - add_jf
				else
					get_jf = randIntD(min_jf, max_jf)
				end
				outDebug("------i--"..add_rs.."---add_jf = "..add_jf.."--get_jf = "..get_jf)
				add_jf = add_jf + get_jf
				add_rs = add_rs + 1
			end
		end
	elseif(tokens[1] == "@打印攻击者信息")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		for i=0, MAX_ATTACKER_INFO_NUM-1, 1 do
			outDebug("------guid = "..playerInfo:GetAttackerGuid(i).."  name = "..playerInfo:GetAttackerName(i))
		end
	elseif(tokens[1] == "@战神之路")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local ntype = 0
		if(#paras >= 2)then
			ntype = paras[2]
		end
		local val = 0
		if(#paras >= 3)then
			val = paras[3]
		end
		if ntype == 0 then		-- 设置战神之路等级
			playerInfo:SetZszlLevel(val)
		elseif ntype == 1 then		-- 清空战神之路排名
			for level=1, 100 do
				globalCounter:SetZszlPlayerGuid(level, "")
				globalCounter:SetZszlPlayerName(level, "")
				globalCounter:SetZszlPlayerForce(level, 0)
				globalCounter:SetZszlPlayerDam(level, 0)
				globalCounter:SetZszlPlayerTm(level, 0)
			end
		end
	elseif(tokens[1] == "@复活")then
		unitLib.Respawn(player_ptr,RESURRPCTION_HUANHUNDAN,100)
	elseif(tokens[1] == "@打印精灵下标")then
		if(#paras >= 2)then
			if unitLib.GetTarget(player_ptr) then
				player_ptr = unitLib.GetTarget(player_ptr)
			end
		end
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		for i=0, 4 do
			for j=0, 3 do
				outFmtError("------index = %d off = %d value = %d", i, j, playerInfo:GetByte(i, j))
			end
		end
		for i=5, 7 do
			for j=0, 1 do
				outFmtError("------index = %d off = %d value = %d", i, j, playerInfo:GetUInt16(i, j))
			end
		end
		outFmtError("------index = %d flags value = %d", 8, playerInfo:GetUInt32(8))
		if(GetUnitTypeID(player_ptr) == TYPEID_PLAYER)then
			outFmtError("------player index = %d flags value = %d", 210, playerInfo:GetPlayerUInt32(210))
		end
		for i=19, 32 do
			outFmtError("------index = %d  value = %d", i, playerInfo:GetUInt32(i))
		end
		for i=196, 211 do
			outFmtError("------index = %d  value = %d", i, playerInfo:GetUInt32(i))
		end
	elseif(tokens[1] == "@清空法力")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		outDebug("-----------00000---------"..playerInfo:GetPlayerUInt32(PLAYER_FIELD_DAZUO_TIME))
		if playerInfo:GetUnitFlags(UNIT_FIELD_FLAGS_BUFF_DAZUO) then
			outDebug("-----------11111---------")
		else
			outDebug("-----------222222---------")
		end
	elseif(tokens[1] == "@1v1")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local type_ = 1
		if(#paras < 3)then
			local mapId = unitLib.GetMapID(player_ptr)
			local map_ptr = unitLib.GetMap(player_ptr)
			if(IsFubenMap(mapId))then
				local mapInfo = Select_Instance_Script(mapId):new{ptr = map_ptr}
				mapInfo:PrintMapInfo(player_ptr)
			end
			return result
		end
		type_ = paras[2]
		val = paras[3]
		if(type_ == 0)then --设置重进1V1
			playerInfo:UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_ENTER_1V1)
		elseif(type_ == 1)then  --设置当天报名次数
			playerInfo:Set1v1TodaySignCount(val)
		elseif(type_ == 2)then --设置当前连胜场次
			playerInfo:Set1v1CurWinCount(val)
		elseif(type_ == 3)then	--最大连胜场次
			playerInfo:Set1v1MaxWinCount(val)
		elseif(type_ == 4)then --总胜利场次
			playerInfo:Set1v1TotalWinCount(val)
		elseif(type_ == 5)then --总参与次数
			playerInfo:Set1v1TotalCount(val)
		elseif(type_ == 6)then --玩家当前威望
			playerInfo:Set1v1CurWeiwang(val)
		elseif(type_ == 7)then --玩家当天威望
			playerInfo:Set1v1TodWeiwang(val)
		end
	elseif(tokens[1] == "@逐鹿中原传送")then
		local num = 0
		if #paras >= 2 then
			num = paras[2]
		end
		local general_id = string.format("zlzy_%d", num)
		playerLib.Teleport(player_ptr,KUAFU_ZLZY_DITU, 42, 80, 0, general_id, false)
	elseif(tokens[1] == "@lhc")then		-- 测试用
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local bang = objectLib.GetBangInfo(playerInfo:GetBangId())
		if bang then
			outDebug("====11111=======")
		else
			outDebug("====22222=======")
		end
	elseif(tokens[1] == "@游戏对象")then
		local map_ptr = unitLib.GetMap(player_ptr)
		local posx, posy = unitLib.GetPos(player_ptr)
		local entry = 0
		if #paras >= 2 then
			entry = paras[2]
		end
		mapLib.AddGameObject(map_ptr, entry, posx, posy)
	elseif(tokens[1] == "@转生")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local zhuan = paras[2] or 1
		local max_level = playerInfo:GetMaxLevel()
		if max_level ~= playerInfo:GetLevel() then
			outFmtError("@zhuansheng %d ~= %d", max_level, playerInfo:GetLevel())
			return result
		end
		if zhuan < 5 then
			playerInfo:SetMaxLevel( tb_grade_up[zhuan+1].need_level )
		end
		if not playerLib.Upgrade(player_ptr, 1) then		-- 转生失败
			playerInfo:SetMaxLevel(max_level)
		end			
		playerInfo:CallOptResult(OPERTE_TYPE_ZHUANSHENG, ZHUANSHENG_OPERATE_BREAK_SUCCESS)
	elseif(tokens[1] == "@经验")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		playerLib.AddExp(player_ptr, paras[2] or 1000)
	elseif(tokens[1] == "@飞升之路")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		playerInfo:SetFszlLevel(paras[2] or 0)
	elseif(tokens[1] == "@飞升之路重置排行")then
		for i = GLOBALCOUNTER_STR_FIELD_FSZL_PLAYER_INFO_START, GLOBALCOUNTER_STR_FIELD_FSZL_PLAYER_INFO_END -1 do
			if globalCounter:GetStr(i) ~= "" then globalCounter:SetStr(i, "") end
		end
		for i = GLOBALCOUNTER_INT_FIELD_FSZL_PLAYER_INFO_START, GLOBALCOUNTER_INT_FIELD_FSZL_PLAYER_INFO_END-1 do
			if globalCounter:GetUInt32(i) ~= 0 then globalCounter:SetUInt32(i, 0) end
		end	
	elseif(tokens[1] == "@天地灵矿")then
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapid = unitLib.GetMapID(player_ptr)
		local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
		local px = 0
		local py = 0
		local pc = 0
		if(#tokens >= 4)then
			px = tonumber(tokens[2])
			py = tonumber(tokens[3])
			pc = tonumber(tokens[4])
		end
		--刷矿
		mapInfo:OnTimer_TianDiLingKuangRefresh(px,py,pc)
	elseif(tokens[1] == "@天地灵矿时间")then
		if(#tokens >= 2)then
			local map_ptr = unitLib.GetMap(player_ptr)
			if not map_ptr then return result end
			local mapid = unitLib.GetMapID(player_ptr)
			local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
			globalValue:SetTianDiLingKuangFreshTime(tonumber(tokens[2]))
			--print("555555555555555555555 "..globalValue:GetTianDiLingKuangFreshTime())
			mapInfo:InitTianDiLingKuangTime()
		end
	elseif(tokens[1] == "@重置天地灵矿")then
		globalValue:SetTianDiLingKuangFreshTime(0)
	elseif(tokens[1] == "@打印天地灵矿")then
		local v = 0
		if(#tokens >= 2)then v = toint(tokens[2]) end
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapid = unitLib.GetMapID(player_ptr)
		local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
		local allGameObjects = mapLib.GetAllGameObject(map_ptr,v)
		local t_count = 0
		for i = 1,#allGameObjects do
			local go = UnitInfo:new{ptr = allGameObjects[i]}
			local posx, posy = unitLib.GetPos(allGameObjects[i])
			if(go ~= nil )then
				if(v == 0)then
					for j= 1,#mapInfo.TIANDILINGKUANG_ENTRY_TABLE do
						if(go:GetEntry() == mapInfo.TIANDILINGKUANG_ENTRY_TABLE[j])then
							t_count = t_count + 1
							outDebug(string.format("[Print TianDiLingKuang]go entry:%d,px:%d,py:%d",go:GetEntry(),posx,posy))
							break
						end
					end
				else
					t_count = t_count + 1
					outDebug(string.format("[Print TianDiLingKuang]go entry:%d,px:%d,py:%d",go:GetEntry(),posx,posy))
				end
			end
		end
		outDebug(string.format("[Print TianDiLingKuang]go mapid:%d entry:%d,count:%d",mapid,v,t_count))
	elseif(tokens[1] == "@清除天地灵矿")then
		local v = 0
		if(#tokens >= 2)then v = toint(tokens[2]) end
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapid = unitLib.GetMapID(player_ptr)
		local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
		local allGameObjects = mapLib.GetAllGameObject(map_ptr,v)
		outDebug(string.format("[Remove TianDiLingKuang]go entry:%d,count:%d",v,#allGameObjects))
		for i = 1,#allGameObjects do
			local go = UnitInfo:new{ptr = allGameObjects[i]}
			if(go ~= nil )then
				if(v == 0)then
					for j= 1,#mapInfo.TIANDILINGKUANG_ENTRY_TABLE do
						if(go:GetEntry() == mapInfo.TIANDILINGKUANG_ENTRY_TABLE[j])then
							mapLib.RemoveWorldObject(map_ptr,allGameObjects[i])
							break
						end
					end
				else
					mapLib.RemoveWorldObject(map_ptr,allGameObjects[i])
				end
			end
		end
	elseif(tokens[1] == "@地图")then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapid = unitLib.GetMapID(player_ptr)
		local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
		outFmtDebug("player %s in mapid %d", playerInfo:GetPlayerGuid(), mapInfo:GetMapId())	
	elseif(tokens[1] == "@蟠桃")then
		local time_now = os.time()
		for i = GLOBALVALUE_INT_FIELD_PANTAO_TIME_START, GLOBALVALUE_INT_FIELD_PANTAO_TIME_END-1 do
			globalValue:SetUInt32(i, time_now)
		end
	elseif(tokens[1] == "@观察")then
		local watcher_guid = ""
		if #paras >= 2 then
			watcher_guid = tokens[2]
		end
		playerLib.AddWatcher(player_ptr, watcher_guid)
	elseif(tokens[1] == "@职业")then
		local val = paras[2] or 1
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		playerInfo:SetProfession(val)	--设置职业
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapInfo = Instance_liudaolunhui:new{ptr = map_ptr}
		mapInfo:SetLDFaction(playerInfo,val)	--设置六道势力
		--如果是鬼 增加灵体buff
		if(val == 3)then
			SpelladdBuff(player_ptr, BUFF_LINGTI, player_ptr, 1, tb_buff_template[BUFF_LINGTI].duration)
		--如果是修罗 增加修罗力BUFF	
		elseif(val == 6)then
			SpelladdBuff(player_ptr, BUFF_XIULUOLI, player_ptr, 1, tb_buff_template[BUFF_XIULUOLI].duration)
		end
		unitLib.SetCalculAttr(player_ptr)
	elseif(tokens[1] == "@风流镇酒馆刷新")then	
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapid = unitLib.GetMapID(player_ptr)
		local mapInfo = Instance_fengliuzhen_pub:new{ptr = map_ptr}
		if( not mapInfo )then return result end
		local add_tb,remove_tb = mapInfo:DoFreshFengLiuZhenPub(paras[2])
		mapInfo:DoFreshCreatures(add_tb,remove_tb)
	elseif(tokens[1] == "@重置风流镇酒馆")then
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapid = unitLib.GetMapID(player_ptr)
		local mapInfo = Instance_fengliuzhen_pub:new{ptr = map_ptr}
		if( not mapInfo )then return result end
		mapInfo:DoResetFengLiuZhenPubWujiangInfoAll()
		mapInfo:DoRemoveAllCreatures()
	elseif(tokens[1] == "@进入风流镇酒馆")then
		local to_mapid = 33
		playerLib.Teleport(player_ptr, to_mapid, tb_map_info[to_mapid].into_point[1], tb_map_info[to_mapid].into_point[2])
	elseif(tokens[1] == "@打印风流镇酒馆位置信息")then
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapid = unitLib.GetMapID(player_ptr)
		local mapInfo = Instance_fengliuzhen_pub:new{ptr = map_ptr}
		if( not mapInfo )then return result end
		for i= 1,10 do
			print("-----Instance FLZ PUB POS:"..toint(i-1)..","..mapInfo:GetMapFengLiuZhenPubPosData(i-1))
		end
	elseif(tokens[1] == "@增加副本死亡次数")then
		local val = paras[2] or 1
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local mapid = unitLib.GetMapID(player_ptr)
		local mapInfo = Instance_liudaolunhui:new{ptr = map_ptr}
		if(mapid == 31 and mapInfo)then
			mapInfo:SetMapRefreshXiuLuoCount(val)
		end
	elseif(tokens[1] == "@增加个人死亡次数")then
		local val = paras[2] or 1
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		playerInfo:SetRefreshXiuLuoCount(val)
	elseif(tokens[1] == "@ldlhcs")then
		playerLib.Teleport(player_ptr,31,168,168, 0, "", false)
	elseif(tokens[1] == "@ldlhsg")then
		local k = paras[2] or 1
		local map_ptr = unitLib.GetMap(player_ptr)
		if not map_ptr then return result end
		local v = tb_liudaolunhui[k]
		if v then
			local creature = mapLib.AddCreature(map_ptr, {templateid = v.creature_id, x = 85 + randInt(-5, 5), y=  171 + randInt(-5, 5), ainame = "AI_LiuDaoLunHuiCreature", npcflag = {}})		
			if(creature)then
				local mapInfo = Instance_liudaolunhui:new{ptr = map_ptr}
				mapInfo:InitRobot(creature,k)	--初始化机器人
			end
		end
	elseif tokens[1] == '@清理千机阁' then
		globalValue:ClearQianjige()
	elseif tokens[1] == '@技能cd' then	
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		playerInfo:PrintSpellSlotInfo()	--打印技能槽信息
		
	elseif tokens[1] == '@清理武林秘境' then
		globalValue:ClearWulinMijing()
	elseif tokens[1] == '@清理江湖奇侠' then
		globalValue:ClearJiangHuQiXia()	
	elseif tokens[1] == '@清理奇遇' then
		globalValue:ClearQiYu()		
	elseif(tokens[1] == "@stest")then
		-- local playerInfo = UnitInfo:new{ptr = player_ptr}
		-- local mapId = unitLib.GetMapID(player_ptr)
		-- local map_ptr = unitLib.GetMap(player_ptr)
		-- local mapInfo = Select_Instance_Script(mapId):new{ptr = map_ptr}
		-- if mapId == 3 then
		-- 	local creature = mapLib.AddCreature(map_ptr, {active_grid = true,templateid = 2501, x = 110, y = 102, ainame = "AI_jianghu_qixia", npcflag = {}})
		-- 	if creature then
		-- 		local creatureInfo = UnitInfo:new{ptr = creature}
		-- 		creatureLib.SetActionRadius(creature, 40)
		-- 		creatureLib.SetVisionRadius(creature, 40)
		-- 		creatureLib.MonsterMove(creature, JHQX_MOTION_TYPE)
		-- 	end
		-- end
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		
	elseif tokens[1] == "@hdqsprint" then
		local map_ptr = unitLib.GetMap(player_ptr)
		local mapid = unitLib.GetMapID(player_ptr)
		if mapid ~= FLZ_INSTANCE_HDQS_MAPID then return result end
		local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
		mapInfo:PrintAllData()
	elseif tokens[1] == "@hdqscatch" then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local map_ptr = unitLib.GetMap(player_ptr)
		local mapid = unitLib.GetMapID(player_ptr)
		if mapid ~= FLZ_INSTANCE_HDQS_MAPID then return result end
		local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
		local index = mapInfo:FindPlayerIndex(playerInfo:GetPlayerGuid())
		if index == -1 then return result end
		local animal = paras[2]
		mapInfo:SetPlayerAnimal(index, animal)
		--被动buff
		local buff = mapInfo.GUARD_BUFF_CONFIG[animal].buff
		SpelladdBuff(player_ptr, buff, player_ptr, 1, tb_buff_template[buff].duration)
		mapInfo:RecalcPlayerAttrs(index)
		playerInfo:CallOptResult(OPRATE_TYPE_HDQS, HDQS_OPERATE_PLAYER_YOUBU_RESULT, tostring(animal))
	elseif tokens[1] == "@hdqsadd" then
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local map_ptr = unitLib.GetMap(player_ptr)
		local mapid = unitLib.GetMapID(player_ptr)
		if mapid ~= FLZ_INSTANCE_HDQS_MAPID then return result end
		local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
		local index = mapInfo:FindPlayerIndex(playerInfo:GetPlayerGuid())
		if index == -1 then return result end
		local drop_item = paras[2]
		if not drop_item then return result end
		local opt_reason = HDQS_OPERATE_PLAYER_DROP_ITEM
		if drop_item >= 2312 and drop_item <= 2316 then
			mapInfo:SetPlayerOtherEquip(index, drop_item)
		else
			local grid, is_new_grid = mapInfo:FindPlayerItemGrid(index, drop_item)
			if grid ~= -1 then
				if is_new_grid then --新格子设一下id
					mapInfo:SetPlayerItemInfo(index, grid, drop_item, 0)
					mapInfo:RecalcPlayerAttrs(index)
				end
				mapInfo:AddPlayerItemCount(index, grid, 1)
			else
				opt_reason = HDQS_OPERATE_PLAYER_DROP_NO_ITEM
			end
		end
		playerInfo:CallOptResult(OPRATE_TYPE_HDQS, opt_reason, string.format("%d|%d",drop_item,1))
	elseif tokens[1] == '@技能范围' then	
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		if not playerInfo:GetBit(PLAYER_SCENED_INT_FLAGS, PLAYER_SCENED_FLAGS_SPELL_FANWEI) then
			playerInfo:SetBit(PLAYER_SCENED_INT_FLAGS, PLAYER_SCENED_FLAGS_SPELL_FANWEI)
		else
			playerInfo:UnSetBit(PLAYER_SCENED_INT_FLAGS, PLAYER_SCENED_FLAGS_SPELL_FANWEI)
		end
	elseif(tokens[ 1 ] == "@显示属性")then
		-- /*通知客户端显示属性*/
		local playerInfo = UnitInfo:new{ptr = player_ptr}
		local output = protocols.pack_show_unit_attribute ()
		local size = GetAttrSize()
		output:writeU16(size)
		foreachAttr(function (attrId, binlogIndx)
			local val = playerInfo:GetUInt32(binlogIndx)
			output:writeU32(attrId)
			output:writeU32(val)
		end)
		playerInfo:SendPacket(output)
		output:delete()
		
	end
	
	return result
end
