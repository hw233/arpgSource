--这是示例方法，请不要调用
--每个函数只有一个参数，参数类型是table,要在函数的上面说明参数的格式
--函数体内，声明返回结果的地方，也要在注释上写明返回格式。

--1.等级，2.基础攻击力，3.基础防御力
function sample_function(input)
	--1.攻击力，2.防御力
	local result = {}
	--这只是一个例子，不要下面的细节啊
	result[1] = input[2] + input[1] * 10
	result[2] = input[3] + input[1] * 5
	return result
end

--获取基础属性战斗力
--tParam:基础属性[属性类型，属性值...]
--返回：result result[1]=基础属性战斗力
--返回：result result[2]=固定属性战斗力
function CalForceByBaseAttr(tParam)
	local result = {0,0}
	if #tParam < 2 then
		return result
	end
	local tList = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	for i = 1, #tParam, 2 do
		tList[tParam[i]] = tParam[i+1]
	end 
	--基础属性战斗力=0.34*生命值+1*外功+1*内功+1*外防+1*内防+5*命中+5*闪避+5*暴击+5*抗暴
	result[1] = math.floor(0.34*tList[1]+1*tList[2]+1*tList[3]+1*tList[4]+1*tList[5]+5*tList[6]+5*tList[7]+5*tList[8]+5*tList[9])
	--固定属性战斗力=0.34*生命值+1*外功+1*内功+1*外防+1*内防+5*命中+5*闪避+5*暴击+5*抗暴
	result[2] = math.floor(0.34*tList[10]+1*tList[11]+1*tList[12]+1*tList[13]+1*tList[14]+5*tList[15]+5*tList[16]+5*tList[17]+5*tList[18])
	return result
end

--获取装备强化属性
--tParam[1] = 基础属性值
--tParam[2] = 装备强化等级
--返回：result result[1]=装备强化属性
function CalEquipStrongValue(tParam)
	local result = {0}
	if #tParam < 2 or tParam[2] == 0 then
		return result
	end
	--强化属性 = int(装备基础属性*(强化系数/10000))
	result[1] = math.floor(tParam[1]*equip_intensify_table[tParam[2]].value/10000)
	return result
end

--获取装备升星属性
--tParam[1] = 装备基础属性
--tParam[2] = 装备升星标示位
--返回：result result[1]=升星属性
function CalEquipStarValue(tParam)
	local result = {0}
	if #tParam < 2 or tParam[2] == 0 then
		return result
	end
	local value = 0
	for i = 1, #tb_equip_grade do
		if tParam[2] % 2 == 1 then
		value = value + tb_equip_grade[i].value
		end
		tParam[2] = math.floor(tParam[2]/2)
	end
	--升星属性 = int(装备基础属性*(已激活星级系数之和/10000))
	result[1] = math.floor(tParam[1]*value/10000)
	return result
end

--计算辅助技能属性值
--tParam[1] = 属性的初始值
--tParam[2] = 成长值
--tParam[3] = 当前等级
--返回：result result[1]=辅助技能属性值
function CalAssistSpellAttrByLevel(tParam)
	local result = {0}
	if #tParam < 3 then
		return result
	end
	--属性值 = 属性的初始值+( 技能等级 - 1 ) *成长值
	result[1] = math.floor(tParam[1] + ( tParam[3] - 1 ) * tParam[2])
	return result
end

--计算辅助技能升级消耗
--tParam[1] = 辅助技能id
--tParam[2] = 辅助技能等级
--返回：result result[1]=升级消耗
function CalAssistSpellUpgradeCost(tParam)
	local result = {0}
	if #tParam < 2 or tParam[2] == 0 then
		return result
	end
	--升级消耗的量 = 辅助技能升级表中对应等级的升级消耗*升级消耗系数/1000
	result[1] = math.floor(tb_assistspell_upgrade[tParam[2]].cost * tb_assistspell[tParam[1]].cost_value / 1000)
	return result
end

--计算主角技能附加伤害
--tParam[1] = 主角技能id
--tParam[2] = 主角技能等级
--返回：result result[1]=附加伤害
function CalSpellHurtAdd(tParam)
	local result = {0}
	if #tParam < 2 or tParam[2] == 0 then
		return result
	end
	--附加伤害 = 附加伤害系数*技能对应等级表的附加伤害/1000+附加伤害固定值
	result[1] = math.floor(tb_spell_template[tParam[1]].fight_value[1] * tb_spell_upgrade[tParam[2]].fight_value/1000 + tb_spell_template[tParam[1]].fight_value[2])
	return result
end

--计算主角技能无视防御伤害
--tParam[1] = 主角技能id
--tParam[2] = 主角技能等级
--返回：result result[1]=无视防御伤害
function CalSpellHurtPassDefense(tParam)
	local result = {0}
	if #tParam < 2 or tParam[2] == 0 then
		return result
	end
	--无视防御伤害 = 无视防御伤害系数*技能对应等级表的无视防御伤害/1000+无视防御伤害固定值
	result[1] = math.floor(tb_spell_template[tParam[1]].cannot_defence_hure[1] * tb_spell_upgrade[tParam[2]].cannot_defence_hure/1000 + tb_spell_template[tParam[1]].cannot_defence_hure[2])
	return result
end

--计算主角技能升级消耗
--tParam[1] = 主角技能id
--tParam[2] = 主角技能等级
--返回：result result[1]=升级消耗
function CalSpellUpgradeCost(tParam)
	local result = {0}
	if #tParam < 2 or tParam[2] == 0 then
		return result
	end
	--升级消耗 = 升级消耗系数*技能对应等级表的升级消耗/1000+升级消耗固定值
	result[1] = math.floor(tb_spell_template[tParam[1]].cost_value[1] * tb_spell_upgrade[tParam[2]].cost / 1000 + tb_spell_template[tParam[1]].cost_value[2])
	return result
end


--计算玩家移动速度
--tParam[1] = 服务端下发的速度值
--返回：result result[1]= 客户端速度
function CalMoveSpeed(tParam)
	local result = {0}
	if #tParam ~= 1 then
		return result
	end
	--移动速度 = int(1000/v服)*10 + 20
	result[1] = math.floor(1000/tParam[1])*10 + 20
	return result
end

--获取红颜玩家当前季节
--tParam[1] = 玩家出游次数
--返回：result result[1]= 玩家当前季节
function CalHongYanJiJieValue(tParam)
	local result = {0}
	if #tParam < 1 then
		return result
	end
	--季节 = int(出游次数/30)%4
	result[1] = math.floor(tParam[1]/30)%4
	return result
end

--获取红颜玩家当前时辰
--tParam[1] = 玩家出游次数
--返回：result result[1]= 玩家当前时辰
function CalHongYanShiChenValue(tParam)
	local result = {0}
	if #tParam < 1 then
		return result
	end
	--时辰 = 出游次数%3
	result[1] = tParam[1]%3
	return result
end

--获取红颜美人人气值
--tParam[1] = 美人id
--tParam[2] = 美人被点赞次数
--返回：result result[1]= 美人人气值
function CalHongYanRenQiValue(tParam)
	local result = {0}
	if #tParam < 2 then
		return result
	end
	--实际人气值 = 初始人气值 + 点赞次数*7
	result[1] = tb_peri_base[tParam[1]].popularity_init + tParam[2] * 7
	return result
end

--获取红颜美人美貌值
--tParam[1] = 美人id
--tParam[2] = 美人评分
--返回：result result[1]= 美人美貌值
function CalHongYanMeiMaoValue(tParam)
	local result = {0}
	if #tParam < 2 then
		return result
	end
	--实际美貌值 = int((初始评分 + 美人评分)/2 + 0.5)
	result[1] = math.floor((tb_peri_base[tParam[1]].beautiful_init + tParam[2]) / 2 + 0.5)
	return result
end

--获取红颜美人结识的玩家数量
--tParam[1] = 真实数量
--返回：result result[1]= 玩家数量
function CalHongYanJieShiValue(tParam)
	local result = {0}
	if #tParam < 1 then
		return result
	end
	--玩家数量 = int(真实数量 * (1 + 0.2 * 真实数量 / 10))
	result[1] = math.floor(tParam[1] * (1 + 0.2 * tParam[1] / 10))
	return result
end

--获取红颜玩家活力恢复时间
--tParam[1] = 当前活力点
--tParam[2] = 玩家下一次活力恢复时间戳
--返回：result result[1]= 活力恢复时间
function CalHongYanHuoLiRestoreTime(tParam)
	local result = {0}
	if #tParam < 1 then
		return result
	end
	--所需恢复时间 = int((活力上限 - 当前活力点) * 恢复时间 / 每次恢复活力点)
	result[1] = math.floor((tb_game_set[21].value[3] - tParam[1]) * tb_game_set[21].value[1] / tb_game_set[21].value[2])
	if #tParam > 1 then
		--活力回满时间戳 = 所需恢复时间 + 下一次活力恢复时间戳
		result[1] = result[1] + tParam[2]
	end
	return result
end

--计算装备强化加成的固定值属性
--tParam[1]:装备的基础属性
--tParam[2]:装备强化等级
--返回：int 装备强化加成的固定值属性
function CalEquipStrongAttr(tParam)
	local result = {0}
	if #tParam < 2 then
		return result
	end
	--强化加成的固定值属性=int(装备的基础属性*强化加成属性比率/10000)
	local prop_rate
	if tParam[2] == 0 then
		prop_rate = 0
	else
		prop_rate = tb_equip_intensify[tParam[2]].prop_rate
	end
	result[1] = math.floor(tParam[1] * prop_rate / 10000)
	return result
end

--计算分解时强化等级加成的概率(银和铁)
--tParam[1]:装备模板id
--tParam[2]:装备强化等级
--返回：分解时强化等级加成的概率[1]:银的概率 [2]铁的概率
function CalEquipResolveAddProOfSilver(tParam)
	local result = {0}
	if #tParam < 2 then
		return result
	end
	--装备模板
	local equip = tb_item_template[tParam[1]]
	--装备部位分解
	local resolve
	if equip.pos == 0 then
		resolve = tb_equip_disassemble[equip.rank].weapon
	elseif equip.pos == 1 then
		resolve = tb_equip_disassemble[equip.rank].helmet
	elseif equip.pos == 2 then
		resolve = tb_equip_disassemble[equip.rank].armour_iron_num
	elseif equip.pos == 3 then
		resolve = tb_equip_disassemble[equip.rank].belt
	elseif equip.pos == 4 then
		resolve = tb_equip_disassemble[equip.rank].cuff
	elseif equip.pos == 5 then
		resolve = tb_equip_disassemble[equip.rank].trousers
	elseif equip.pos == 6 then
		resolve = tb_equip_disassemble[equip.rank].shoe
	elseif equip.pos == 7 then
		resolve = tb_equip_disassemble[equip.rank].pendant
	elseif equip.pos == 8 then
		resolve = tb_equip_disassemble[equip.rank].necklace
	elseif equip.pos == 9 then
		resolve = tb_equip_disassemble[equip.rank].ring
	elseif equip.pos == 10 then
		resolve = tb_equip_disassemble[equip.rank].cloak
	end
	local R_yin = resolve[6]
	local R_tie = resolve[3]

	--强化等级加成的概率A_银（百分比）=int(R_银*V/10000)
	local A_yin = math.floor(R_yin * tb_equip_intensify[tParam[2]].disassemble_value / 10000)
	--强化等级加成的概率A_铁（百分比）=min((100-P_银-R_铁),int(R_铁*V/10000))
	local A_tie = math.min((100 - (R_yin + A_yin) - R_tie), math.floor(R_tie * tb_equip_intensify[tParam[2]].disassemble_value / 10000))
	
	result = {A_yin , A_tie}
	return result
end

--计算武将战斗力
--tParam:基础属性[属性值...] (兵力,攻击,速度,格挡,闪避,致命)
--返回：int 武将战斗力
function CalculWuJiangForce(tParam)
	if #tParam < 6 then
		return {0}
	end
	--战斗力值 =  兵力/3 	  + 攻击*1 	    + 速度*8 	  + 格挡*8      + 闪避*8      + 致命*12
	local force = tParam[1]/3 + tParam[2]*1 + tParam[3]*8 + tParam[4]*8 + tParam[5]*8 + tParam[6]*12	
	return {math.floor(force)}
end


--[[
--神兵资质转换成百分比的表配置
ShenBing_zizhi_Coeffection = {
	[EQUIP_ATTR_FIXED_OUT_DAMAGE] = 0.002, [EQUIP_ATTR_FIXED_IN_DAMAGE] = 0.002, [EQUIP_ATTR_FIXED_OUT_DEF] = 0.0023, [EQUIP_ATTR_FIXED_IN_DEF] = 0.0023, [EQUIP_ATTR_FIXED_HP] = 0.0025,
}
]]

--副本需要刷的怪物数量
function GetFubenRefreshCreatureCount( tParam )
	return 10 + math.floor( tParam.refreshcount / 3 )
end

--获取刷出新的仙人概率
--参数：当前所有世界地图仙人数量
function GetXianRenNewFreshRate( tParam )
	return 100 - tParam.count * 20
end

--获取仙人权重
--参数：排行榜第10名玩家等级，某地图开启等级，某地图仙人数量
function GetXianRenQuanZhong(tParam)
	local qz = 0
	if(tParam.rank_lv >= tParam.open_lv)then
		 qz = 8000/math.pow(2, tParam.count)
	else
		qz = 100/math.pow(2, tParam.count)
	end	
	return qz
end

--获取仙人某地图刷出的概率
--参数：某地图权重，所有世界地图权重
function GetXianRenMapFreshRate(tParam)
	return tParam.map_qz/tParam.total_qz
end

--获得仙人属性（公式:生物属性*仙人属性系数/100)
--参数：生物属性，仙人属性系数
function GetXianRenSomeAttr(tParam)
	return tParam.selfAttr * tParam.xx/100
end

equip_attr_base = "a"
equip_attr_spit = ","

local equip_attr_type = {

	[EQUIP_ATTR_MAX_HEALTH] = "max_health",	--最大生命
	[EQUIP_ATTR_DAMAGE] = "damage",	--攻击力
	[EQUIP_ATTR_ARMOR] = "armor",	--防御力
	[EQUIP_ATTR_HIT] = "hit",	--命中
	[EQUIP_ATTR_MISS] = "miss",	--闪避
	[EQUIP_ATTR_CRIT] = "crit",	--暴击
	[EQUIP_ATTR_TOUGH] = "tough",	--坚韧
	[EQUIP_ATTR_ATTACK_SPEED] = "attack_speed",	--攻击速度
	[EQUIP_ATTR_MOVE_SPEED] = "move_speed",	--移动速度
	[EQUIP_ATTR_IGNORE_ARMOR] = "ignore_armor",	--忽视防御
	[EQUIP_ATTR_IGNORE_MISS] = "ignore_miss",	--忽视闪避
	[EQUIP_ATTR_RECOVERY] = "recovery",	--生命值回复
	[EQUIP_ATTR_DAMAGE_AMPLIFY_RATE] = "damage_amplify_rate",	--伤害加深(万分比)
	[EQUIP_ATTR_DAMAGE_RESIST_RATE] = "damage_resist_rate",	--伤害减免(万分比)
	[EQUIP_ATTR_DAMAGE_RETURN_RATE] = "damage_return_rate",	--反弹伤害(万分比)
	[EQUIP_ATTR_VAMPIRIC_RATE] = "vampiric_rate",	--吸血光环(万分比)
	[EQUIP_ATTR_RECOVERY_RATE] = "recovery_rate",	--回复效率(万分比)
	[EQUIP_ATTR_CRIT_RATE] = "crit_rate",	--暴击率(万分比)
	[EQUIP_ATTR_CRIT_RESIST_RATE] = "crit_resist_rate",	--抗暴率(万分比)
	[EQUIP_ATTR_CRIT_DAM_RATE] = "crit_dam_rate",	--暴击伤害倍数(万分比)
	[EQUIP_ATTR_CRIT_RESIST_DAM_RATE] = "crit_resist_dam_rate",	--降暴伤害倍数(万分比)
	[EQUIP_ATTR_HIT_RATE] = "hit_rate",	--命中率(万分比)
	[EQUIP_ATTR_MISS_RATE] = "miss_rate",	--闪避率(万分比)
	[EQUIP_ATTR_STUN_RATE] = "stun_rate",	--眩晕
	[EQUIP_ATTR_ROOTS_RATE] = "roots_rate",	--定身
	[EQUIP_ATTR_SILENCE_RATE] = "silence_rate",	--沉默
	[EQUIP_ATTR_CHAOS_RATE] = "chaos_rate",	--混乱
	[EQUIP_ATTR_CHARM_RATE] = "charm_rate",	--魅惑
	[EQUIP_ATTR_CONTROL_ENHANCE_RATE] = "control_enhance_rate",	--控制增强
	[EQUIP_ATTR_CONTROL_RESIST_RATE] = "control_resist_rate",	--控制减免
	[EQUIP_ATTR_STRENGTH_ARMOR] = "strength_armor",	--强化护甲
	
	[ITEM_OTHER_ATTR_FAIL_TIME 	] = "ifailtm",
	[ITEM_OTHER_ATTR_STRONG_LV 	] = "istronglv",
	[ITEM_OTHER_ATTR_STRONG_FAIL_COUNT] = "isfcount",
	[ITEM_OTHER_ATTR_FORCE		] = "force",	
	
}

--通过属性类型获得对应的key
function GetAttrKey(tParam)
	return {equip_attr_type[tParam[1]] or ""}
end

--通过属性类型获得对应的index
function GetAttrIndex(tParam)
	local result = {0}
	for i=1,#equip_attr_type do
		if equip_attr_type[i] == tParam[1] then
			result[1] = i
		end
	end
	return result
end

--获取属性类型数量
function GetAttrLength()
	return {#equip_attr_type}
end
