
--所有AI的基类
AI_Base = {
	threat_motion_type = WAYFINDING_ATTACK_MOTION_TYPE,	--仇恨产生的移动类型

	ainame = 'AI_Base',	
	react_state = REACT_PASSIVE,					--是否是主动怪,默认为被动攻击
	friends = {},									--朋友们,作为变量由C读取
	chat_storys = {},								--奇闻系统内容
	react_change = false,							--怪物攻击属性是否需要等级差变化

	script_init =
		function (self,owner)
			--设置主动怪被动怪类型
			creatureLib.SetReactState(owner,self.react_state)
		end,
	MoveInLineOfSight =	--当有unit进入视线时发生
		function(self,owner,unit) 
			-- --怪物等级差变化而攻击玩家
			-- if(GetUnitTypeID(unit) == TYPEID_PLAYER)then
			-- 	local creature = UnitInfo:new{ptr = owner}
			-- 	local player = UnitInfo:new{ptr = unit}
			-- 	if(self.react_change and REACT_CHANGE_CREATURE_ID[creature:GetEntry()] == 1 and creature:GetLevel() - player:GetLevel() < 10)then
			-- 		return false
			-- 	end
			-- 	if(self.react_state == REACT_AGGRESSIVE_PLAYER)then
			-- 		return true
			-- 	end
			-- end
			return true
		end,	
	DamageTaken =
		function(self,owner,unit,damage)
			return 0
		end,	--当伤害发生前
	DamageDeal =
		function(self,owner,unit,damage)
			
		end,	--当伤害发生后
	--当生物产生伤害时 ps:unit可能是兵种
	GiveDamage =
		function(self,owner,unit,damage)
			return damage
		end,
	KilledUnit = function(self,owner,victim)		--当杀死unit时发生,ps:如果是兵种的的话，这个方法可以不用处理了，因为已经处理成玩家kill玩家了
		if(GetUnitTypeID(victim) == TYPEID_PLAYER) then
			local playerInfo = UnitInfo:new{ptr = victim}
			if(IsWorldMap(playerInfo:GetMapID()))then
				-- 帮玩家免费复活
				local count = playerInfo:GetFreeReliveCount()
				if count > 0 then
					playerInfo:SetFreeReliveCount(count - 1)
					unitLib.Respawn(victim,RESURRPCTION_HUANHUNDAN,5)
				end
			end
		end
	end,
	JustDied =									--当被别人杀死前
		function(self,map,owner,killer_ptr)
			
			-- 记录杀怪任务
			local creatureInfo = UnitInfo:new {ptr = owner}
			local entry = creatureInfo:GetEntry()
			if GetUnitTypeID(killer_ptr) == TYPEID_PLAYER then
				local isPkServer = globalGameConfig:IsPKServer()
				if not isPkServer then
					local playerPtrs = mapLib.GetPlayersAfterCreatureDead(map, creatureInfo:GetIntGuid())
					for _, player_ptr in ipairs(playerPtrs) do
						playerLib.SendToAppdDoSomething(player_ptr, SCENED_APPD_KILL_MONSTER, entry)
					end
					playerLib.SendToAppdDoSomething(killer_ptr, SCENED_APPD_KILL_MONSTER_COLLECT, entry)
				end
			end
			return 0							--返回尸体的存活时间
		end,
	--添加地图杀怪数
	AddKill =
		function (self, owner)			
			local map_ptr = unitLib.GetMap(owner)
			local map = Instance_base:new{ptr = map_ptr}
			local result = map:GetMapKillNum() + 1
			map:SetMapKillNum(result)
			
			return result
		end,
	SaySomething =
		function (self,owner)
			local ownerInfo = UnitInfo:new{ptr = owner}
			local entry = ownerInfo:GetEntry()
			local say_str = tb_creature_template[entry].dialogue
			--允许怪物不说话
			if(say_str == "")then
				return 999999999
			end			
			local next_say_timer = randIntD(30000,90000)
			creatureLib.MonsterTalk(owner, say_str)			
			return next_say_timer
		end,
	GossipMessage =
		function(self,owner,option_id)
			return '',{}
		end,		--闲聊选项
	LootAllot = 	--生成战利品
		function(self, owner, player, killer, drop_rate_multiples, boss_type, fcm)
			local playerInfo = UnitInfo:new{ptr = player}
			local player_guid = playerInfo:GetPlayerGuid()
			local player_lv = playerInfo:GetLevel()
			local player_gender = playerInfo:GetGender()		--CHAR_GENDER_MALE,CHAR_GENDER_FEMALE
			local createInfo = UnitInfo:new{ptr = owner}
			local creature_level = createInfo:GetLevel()
			local creature_entry = createInfo:GetEntry()
			local map_ptr = unitLib.GetMap(owner)
			local info = tb_creature_template[creature_entry]
			if not info then return end
			--[[
			--先处理铜钱掉落
			for i = 1, info.money_num do
				--循环处理要掉落的铜钱堆数
				local need_drop_silver = self:SilverLoot(info)
				local result_silver = need_drop_silver
				if result_silver > 0 then
					playerInfo:AddMoney(MONEY_TYPE_SILVER, MONEY_CHANGE_SELECT_LOOT, result_silver)
					--AddLootGameObject(map_ptr, owner, player_guid, Item_Loot_Silver, result_silver, fcm)
				end
			end
			
			--处理道具掉落
			local need_drop_items = self:ItemLoot(player_lv, player_gender, info,player,owner)		-- {{模板,数量,绑定与否,存在时间,保护时间}, }
			for i = 1, #need_drop_items do
				local drop_item_config = need_drop_items[i]
				local loot_entry = drop_item_config[ 1 ]
				local count      = drop_item_config[ 2 ]
				if loot_entry == Item_Loot_Silver then
					--playerInfo:AddMoney(MONEY_TYPE_SILVER, MONEY_CHANGE_SELECT_LOOT, drop_item_config[2])
					AddLootGameObject(map_ptr, owner, player_guid, loot_entry, count, fcm)
				else
					for j = 1, drop_item_config[2] do
						--playerLib.AddItem(player, loot_entry, 1, ITEM_BIND_NONE, LOG_ITEM_OPER_TYPE_LOOT)
						AddLootGameObject(map_ptr, owner, player_guid, loot_entry, 0, fcm, drop_item_config[4], drop_item_config[5], drop_item_config[6])
					end
				end
			end
			--]]
			--[[
			local drop_ids = info.reward_id
			local rewardDict = {}
			DoRandomDropTable(drop_ids, rewardDict)			
			PlayerAddRewards(player, rewardDict)
			--]]
			-- --兽魂
			-- local shouhun = self:ShouHunLoot(info) 
			-- if shouhun > 0 then
			-- 	playerInfo:AddMountShouHun(shouhun)
			-- end
		end,
	--处理兽魂掉落
	ShouHunLoot = function( self, info )
		local shouhun_min, shouhun_max, shouhun_percent = info.shouhun_min, info.shouhun_max, info.shouhun_percent
		if shouhun_min == 0 or shouhun_max == 0 or shouhun_percent == 0 then return 0 end
		if randIntD(1, 100) <= shouhun_percent then
			return randIntD(shouhun_min, shouhun_max)
		end
		return 0
	end,	
		
	--处理铜钱掉落
	SilverLoot = function( self, info )
		local min_silver, max_silver, percent = info.money_min, info.money_max, info.money_percent
		if min_silver == 0 or max_silver == 0 or percent == 0 then return 0 end
		if randIntD(1, 100) <= percent then
			return randIntD(min_silver, max_silver)
		end
		return 0
	end,
	--处理道具掉落
	ItemLoot = function( self, player_lv, player_gender, info,player,owner)
		
		local need_drop_items = {}
		local drop_ids = info.reward_id
		if #drop_ids == 0 then return need_drop_items end
		
		for i = 1, #drop_ids do
			local dict = {}
			local dropId = drop_ids[i]
			DoRandomDrop(dropId, dict)
			
			for entry, value in pairs(dict) do
				--模板,数量,绑定与否,存在时间,保护时间,强化等级
				local temp = {entry, value, 0, config.loot_exist_timer, config.loot_change_owner, 0}
				table.insert(need_drop_items, temp)
			end
		end
		return need_drop_items
		
	end,	
}

-------------------------------------------------
--普通怪物ai
AI_guaiwu = class("AI_guaiwu", AI_Base)
AI_guaiwu.ainame = "AI_guaiwu"

-- 宠物
AI_PET = class("AI_PET", AI_Base)
AI_PET.ainame = "AI_PET"
-------------------------------------------------
--场景物件怪物ai
AI_scence_obj = class("AI_scence_obj", AI_Base)
AI_scence_obj.ainame = "AI_scence_obj"
AI_scence_obj.react_state = REACT_DEFENSIVE

--受到的伤害
function AI_scence_obj:DamageTaken(owner,unit,damage)
	return randIntD(290,310) - damage
end

--被击杀后
function AI_scence_obj:JustDied(map,owner,killer_ptr)
	AI_Base.JustDied(self, map_ptr, owner, killer_ptr)

	--先随机一个效果
	local rand_id = tb_game_set[125].value[ randIntD(1,#tb_game_set[125].value) ]
	if rand_id == 0 then --加怒气
		local killer = UnitInfo:new{ptr = killer_ptr}
		if killer:GetAnger() ~= 100 then
			killer:SetAnger(100)
			killer:SetUnitAnger(100)
		end
	else --加buff
		--先移除旧的buff，如果有的话
		for _, id in ipairs(tb_game_set[125].value) do
			RemoveNormalBuff(killer_ptr, id)
		end
		--再加新buff
		AddNormalBuff(killer_ptr, rand_id, owner, 1)
	end
	return 0 --返回尸体的存活时间
end

-------------------------------------------------
--bossai
AI_boss = class("AI_boss", AI_Base)
AI_boss.ainame = "AI_boss"
-- 有BUG 需要处理
function AI_boss:DamageTaken( owner,unit,damage )
	local map_ptr = unitLib.GetMap(owner)
	local createInfo = UnitInfo:new{ptr = owner}	
	--当被玩家攻击时
	if(GetUnitTypeID(unit) == TYPEID_PLAYER)then
		local playerInfo = UnitInfo:new{ptr = unit}
		-- if randIntD(1,2)==1 and createInfo:triggerPassiveSpell(BOSS_PASSIVESPELL_FANTAN,15) then	--反弹
		-- 	SpelladdBuff(owner, BUFF_FANTAN, owner, 1, tb_buff_template[BUFF_FANTAN].duration)
		-- elseif randIntD(1,2)==1 and createInfo:triggerPassiveSpell(BOSS_PASSIVESPELL_LIANJIE,15) then	--连接
		-- 	SpelladdBuff(owner, BUFF_LIANJIE, owner, 1, tb_buff_template[BUFF_LIANJIE].duration)
		-- 	SpelladdBuff(unit, BUFF_BEILIANJIE, owner, 1, tb_buff_template[BUFF_BEILIANJIE].duration)
		-- elseif randIntD(1,2)==1 then	--钢刺
		-- 	local tar_x, tar_y = unitLib.GetPos(owner)
		-- 	local targets = mapLib.GetCircleTargets(tar_x, tar_y, 8, owner, TARGET_TYPE_ENEMY)	
		-- 	for key, attack_target in pairs(targets) do
		-- 		if(attack_target ~= nil)then
		-- 			SpelladdBuff(attack_target, BUFF_GANGCI, owner, 1, tb_buff_template[BUFF_GANGCI].duration,1)
		-- 		end
		-- 	end
		-- elseif randIntD(1,2)==1 and createInfo:triggerPassiveSpell(BOSS_PASSIVESPELL_HUIFUHUDUN,15) then	--恢复护盾
		-- 	SpelladdBuff(owner, BUFF_HUIFU_HUDUN, owner, 1, tb_buff_template[BUFF_HUIFU_HUDUN].duration)
		-- elseif randIntD(1,2)==1 and createInfo:triggerPassiveSpell(BOSS_PASSIVESPELL_FANJIHUDUN,15) then	--反击护盾
		-- 	SpelladdBuff(owner, BUFF_FANJI_HUDUN, owner, 1, tb_buff_template[BUFF_FANJI_HUDUN].duration,5000)	
		-- elseif randIntD(1,2)==1 and createInfo:triggerPassiveSpell(BOSS_PASSIVESPELL_KUANGBAO,15) then	--狂暴
		-- 	SpelladdBuff(owner, BUFF_KUANGBAO, owner, 1, tb_buff_template[BUFF_KUANGBAO].duration)
		-- elseif randIntD(1,2)==1 and createInfo:triggerPassiveSpell(BOSS_PASSIVESPELL_XUECHI,60) then	--血池
		-- 	--刷普通怪
		-- 	local creature_entry = tb_creature_template[1].id
			
		-- 	local pos_x, pos_y = unitLib.GetPos(owner)
		-- 	local randX, randY = GetRandPosByRadius(map_ptr, pos_x, pos_y, 15)
		-- 	local creature = mapLib.AddCreature(map_ptr, {templateid = creature_entry, x = randX, y = randY, ainame = 'AI_guaiwu', active_grid = true, npcflag = {}})
		-- 	if creature~=nil then
		-- 		--攻击模式
		-- 		creatureLib.SetReactState(creature,REACT_DEFENSIVE)
		-- 		--设置主人
		-- 		creatureLib.SetMonsterHost(creature,owner)
		-- 		SpelladdBuff(creature, BUFF_XUECHI, creature, 1, tb_buff_template[BUFF_XUECHI].duration)
		--		creatureLib.MonsterMove(creature,DISAPPEAR_ONE_MOTION_TYPE,tb_buff_template[BUFF_XUECHI].duration*1000)
		-- 	end
		-- elseif randIntD(1,2)==1 and createInfo:triggerPassiveSpell(BOSS_PASSIVESPELL_WAIGONGMIANYI,15) then	--外攻免疫
		-- 	SpelladdBuff(owner, BUFF_WAIGONGMIANYI, owner, 1, tb_buff_template[BUFF_WAIGONGMIANYI].duration)
		-- elseif randIntD(1,2)==1 and createInfo:triggerPassiveSpell(BOSS_PASSIVESPELL_NEIGONGMIANYI,15) then	--内攻免疫
		-- 	SpelladdBuff(owner, BUFF_NEIGONGMIAN, owner, 1, tb_buff_template[BUFF_NEIGONGMIAN].duration)
		-- end

		local creature_entry = createInfo:GetEntry()
		local drop_belong = tb_creature_template[creature_entry].drop_belong 
		if drop_belong == 3 and createInfo:GetLootOwnerGuid() == "" then
			--首次攻击归属,有队伍就把归属设成队伍guid，没有就设成玩家guid
			local group_guid = playerInfo:GetGroupId()
			local group_ptr = objectLib.GetGroupInfo(group_guid)
			if group_ptr then
				--有队伍	
				local groupInfo = ScenedGroupInfo:new{ptr = group_ptr}			
				createInfo:SetLootOwnerGuid(group_guid)
				createInfo:SetLootOwnerName(groupInfo:GetGroupLeaderName())
			else
				--没队伍属于个人
				createInfo:SetLootOwnerGuid(playerInfo:GetPlayerGuid())
				createInfo:SetLootOwnerName(playerInfo:GetName())				
			end
		elseif drop_belong == 1 then
			--累计伤害统计		
			InstanceBoss:AddDamageCache(creature_entry, playerInfo:GetPlayerGuid(), damage)
		end		
	end

	return 0
end

--世界boss被杀了
function AI_boss:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self, map_ptr, owner, killer_ptr)
	local creatureInfo = UnitInfo:new{ptr = owner}
	local creature_entry = creatureInfo:GetEntry()
	local drop_belong = tb_creature_template[creature_entry].drop_belong
	local map_id = mapLib.GetMapID(map_ptr)
	local mapInfo = Select_Instance_Script(map_id):new{ptr = map_ptr}
	--清理BOSS正在释放的技能
	creatureInfo:ClearCurSpell(true)	
	
	--不管怎样先清掉，伤害统计
	if drop_belong == 1 then
		InstanceBoss:ClearDamageCache(creature_entry)
	end

	if(GetUnitTypeID(killer_ptr) == TYPEID_PLAYER)then
		local killInfo = UnitInfo:new{ptr = killer_ptr}		
		if drop_belong == 2 and creatureInfo:GetLootOwnerGuid() == "" then
			--最后一击归属，有队伍就把归属设成队伍guid，没有就设成玩家guid
			local group_guid = killInfo:GetGroupId()
			local group_ptr = objectLib.GetGroupInfo(group_guid)
			if group_ptr then
				--有队伍	
				local groupInfo = ScenedGroupInfo:new{ptr = group_ptr}			
				creatureInfo:SetLootOwnerGuid(group_guid)
				creatureInfo:SetLootOwnerName(groupInfo:GetGroupLeaderName())
			else
				--没队伍属于个人
				creatureInfo:SetLootOwnerGuid(killInfo:GetPlayerGuid())
				creatureInfo:SetLootOwnerName(killInfo:GetName())				
			end
		end
		--处理boss掉落
		self:BossLoot(mapInfo, creatureInfo, killInfo)
		--成就系统
		if creatureInfo:GetLevel() >= 80 then
			killInfo:AddKill80Boss(1)
		end
		--成就系统
		for key,val in ipairs(tb_cj_day[ACHIEVEMENT_KILL_SINGLE_BOSS + 1].reward) do
			for i = 1,#val[1] do
				local is_find = false
				if creature_entry == val[1][i] then
					killInfo:SetKillSingleBoss(key)
					is_find = true
					break
				end
				if is_find then
					break
				end
			end
		end
	end
end

--boss掉落处理
function AI_boss:BossLoot( mapInfo, creatureInfo, killInfo )
	local player_guid = killInfo:GetPlayerGuid()
	local player_name = killInfo:GetName()
	local player_lv = killInfo:GetLevel()
	local player_gender = killInfo:GetGender()		--CHAR_GENDER_MALE,CHAR_GENDER_FEMALE
	local fcm = killInfo:GetFCMLoginTime()
	local owner_guid = creatureInfo:GetLootOwnerGuid()	
	local creature_level = creatureInfo:GetLevel()
	local creature_entry = creatureInfo:GetEntry()	

	local info = tb_creature_template[creature_entry]
	if not info then return end
	--先处理铜钱掉落
	for i = 1, info.money_num do
		--循环处理要掉落的铜钱堆数
		local need_drop_silver = self:SilverLoot(info)
		local result_silver = need_drop_silver
		local attr_ptr = playerLib.GetEquipAttrs(killInfo.ptr)
		if attr_ptr then
			local attrInfo = ReCalculate:new{ptr = attr_ptr}
			result_silver = result_silver + need_drop_silver * attrInfo:GetOthers(EEQUIP_OTHER_KILL_CREATURE_SILVER) * 0.0001
		end
		if result_silver > 0 then
			AddLootGameObject(mapInfo.ptr, creatureInfo.ptr, owner_guid, Item_Loot_Silver, result_silver, fcm)
		end
	end

	--处理道具掉落
	
	--如果是随机boss
	self:SuiJiBossLoot(creature_entry,creature_level,info)
	
	local need_drop_items = self:ItemLoot(player_lv, player_gender, info,killInfo.ptr,creatureInfo.ptr)		-- {{模板,数量,绑定与否,存在时间,保护时间}, }
	for i = 1, #need_drop_items do
		local drop_item_config = need_drop_items[i]
		local loot_entry = drop_item_config[1]
		if loot_entry == Item_Loot_Silver or loot_entry == Item_Loot_Wood or loot_entry == Item_Loot_Mineral or loot_entry == Item_Loot_Coal 
					or loot_entry == Item_Loot_Leather or loot_entry == Item_Loot_Pearl or loot_entry == Item_Loot_Crystal then
			AddLootGameObject(mapInfo.ptr, creatureInfo.ptr, owner_guid, loot_entry, drop_item_config[2], fcm)
		else
			for j = 1, drop_item_config[2] do				
				AddLootGameObject(mapInfo.ptr, creatureInfo.ptr, owner_guid, loot_entry, drop_item_config[3], fcm, drop_item_config[4], drop_item_config[5],drop_item_config[6])				
				--添加掉宝记录
				local map_cs = 0
				--如果是奇遇地图
				if mapInfo:GetMapId() == QIYU_MORE_MAPID and mapInfo.getQiYuId then
					map_cs = mapInfo:getQiYuId()
				end
				globalCounter:AddBossDropRecordInfo( player_guid, player_name, loot_entry, creature_entry, mapInfo:GetMapId(),creature_level,map_cs )
			end
		end
	end		
end


--生成战利品
function AI_boss:LootAllot(owner, player, killer, drop_rate_multiples, boss_type, fcm)
	--boss掉落不走正常掉落

end

function AI_boss:SuiJiBossLoot(creature_entry,creature_level,info)
	local is_ok = false
	for i = 1,#tb_suiji_level[1].creature_id do
		if creature_entry == tb_suiji_level[1].creature_id[i] then
			is_ok = true
			break
		end
	end
	if is_ok then
		for _,val in ipairs(tb_suiji_level)do
			if val.level == creature_level then
				info.reward_id = val.reward
			end
		end
	end
end

--周围20码的玩家下发错误消息提示复仇boss来袭
function AI_boss:CallFuChouBossComing(owner)
	local cast_x, cast_y = unitLib.GetPos(owner)
	local players = mapLib.GetCircleTargets(cast_x, cast_y, 20, owner, TARGET_TYPE_ALL,true)
	for _, player in pairs(players) do
		if player ~= nil and GetUnitTypeID(player) == TYPEID_PLAYER then
			local attackInfo = UnitInfo:new{ptr = player}
			attackInfo:CallOptResult(OPERTE_TYPE_DA_BOSS, DABOSS_OPERATE_FUCHOU_BOSS)
		end
	end
end

--随机掉落物品的强化等级
function RandLootItemStrengLv(player,entry,owner)
	local streng_lv = 0
	local playerInfo = UnitInfo:new{ptr = player}
	if GetUnitTypeID(player) ~= TYPEID_PLAYER then
		return streng_lv
	end
	local config = tb_item_template[entry]
	--如果是装备
	if config.type == 1 then
		local add_12 = globalValue:GetNoAdd12EquipCount()
		--+12第一次初始值500
		if add_12 == 0 then 
			globalValue:SetNoAdd12EquipCount(500) 
			add_12 = globalValue:GetNoAdd12EquipCount()
		end
		local add_9 = globalValue:GetNoAdd9EquipCount()	
		local pay_lv = playerInfo:GetPayLevel()
		local add_12_pro = math.max(add_12-800,0)*10*(1-pay_lv/30)
		local add_9_pro = math.max(add_9-320,0)*20*(1-pay_lv/30)	
		-- + 12
		if randIntD(1,10000) <= add_12_pro then
			streng_lv = 12
			globalValue:SetNoAdd12EquipCount(1)	
		-- + 9 ~ + 11	
		elseif randIntD(1,10000) <= add_9_pro then
			streng_lv = randIntD(9,11)
			globalValue:SetNoAdd9EquipCount(100)	
		else
			local rand = randIntD(1,10000)
			for i = 1,8 do
				local true_pro = 0
				if i >= 1 and i <= 4 then
					true_pro = GetStrengLvPro(i,owner)[1] - ( GetStrengLvPro(i+4,owner)[2]-GetStrengLvPro(i+4,owner)[1] )
				else
					true_pro = GetStrengLvPro(i,owner)[2]
				end
				if rand <= true_pro then
					streng_lv = i
					break
				else
					rand = rand - true_pro
				end
			end
			globalValue:AddNoAdd9EquipCount()
			globalValue:AddNoAdd12EquipCount()
		end
	end
	return streng_lv
end

--获取装备强化等级的掉落概率 (@result[1] == 基础概率 result[2] == 实际概率)
function GetStrengLvPro(lv,owner)
	local creatureInfo = UnitInfo:new{ptr = owner}
	local creature_entry = creatureInfo:GetEntry()
	local creature_level = creatureInfo:GetLevel()
	local result = {0,0}
	for i = 1,#tb_equip_intensify[lv].drop_prob do
		local a_table = tb_equip_intensify[lv].drop_prob[i]
		for j = 1,#a_table-1 do
			if a_table[j] == creature_entry then
				result[1] = a_table[#a_table]
				if lv >= 5 and lv <= 8 then
					result[2] = a_table[#a_table] * creature_level / tb_game_set[122].value[1]
				end
				return result
			end
		end
	end
	return result
end


