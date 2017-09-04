--添加一个战利品到地图上，返回成功或者失败
function AddLootGameObject(map, unit, player_id, loot_entry, bind_state, fcm,exist_time, protect_time,streng_lv,fail_time, flags)
	--outString('AddLootGameObject')
	if(map == nil or unit == nil)then
		return false
	end
	
	local is_money = IsResource(loot_entry)
	if(is_money)then
		assert(fcm ~= nil)	--必须崩掉，如果是要掉钱，必须处理防沉迷

		if(fcm ~= MAX_UINT32_NUM and fcm >= 180)then
			if(fcm >= 300)then
				bind_state = 0
			else
				bind_state = bind_state / 2
			end
		end
		bind_state = math.floor(bind_state)
		-- 掉落数量不能小于1
		if bind_state < 1 then
			return false
		end
	end

	if(exist_time == nil or protect_time == nil)then
		exist_time = 40		--战利品存活时间
		protect_time = 20	--战利品拥有者保护时间
	end
	if(fail_time == nil)then
		fail_time = 0
	end
	local x,y,cons_ok = mapLib.ConsLootPoint(map, unit)
	--local pos_x, pos_y = unitLib.GetPos(unit)
	if(cons_ok)then
		--这里太频繁调用了，就不封装成类了，要不然每次都得new
		local public_obj, private_obj, index = mapLib.GetLootObject(map, x, y)
		outDebug(string.format('AddLootGameObject %f %f %d', x, y, index))
		if index < 0 then
			return false
		end
		
		local guid = binLogLib.GetStr(unit, BINLOG_STRING_FIELD_GUID)
		local name = binLogLib.GetStr(unit, BINLOG_STRING_FIELD_NAME)
		local flags_int = 0
		
		--战利品模板
		assert(loot_entry)
		binLogLib.SetUInt32(public_obj, LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_ENTRY, loot_entry)

		--金钱数量
		binLogLib.SetUInt32(public_obj, LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_MONEY_SUM, bind_state)
		
		--强化等级
		if streng_lv and streng_lv > 0 then
			binLogLib.SetUInt32(public_obj, LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_STRENG_LV, streng_lv)
		end
		--战利品存活时间			
		binLogLib.SetUInt32(private_obj, index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_EXIST_TIME, exist_time * 1000)
		--战利品拥有者保护时间
		binLogLib.SetUInt32(private_obj, index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_OWNER_TIME, protect_time * 1000)
		--拾取以后的物品失效时间
		if(fail_time ~= 0)then
			binLogLib.SetUInt32(private_obj, index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_FAIL_TIME, fail_time)
		end
		--拥有者ID
		binLogLib.SetStr(public_obj, LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PUBLIC_STR_FIELD + LOOT_PUBLIC_STR_OWNER, player_id)
		
		
		--战利品掉落怪GUID
		binLogLib.SetStr(private_obj, LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PRIVATE_STRING + LOOT_PRIVATE_STRING_CREATE_BY, guid)
		--战利品掉落怪名称		
		binLogLib.SetStr(private_obj, LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PRIVATE_STRING + LOOT_PRIVATE_STRING_CREATE_NAME, name)
		
		--标志位
		if flags then
			for k,flag in pairs(flags) do
				assert(flag < 32)--超了再说
				flags_int = flags_int + math.pow(2, flag)
			end
		end
		if(flags_int ~= 0)then
			binLogLib.SetUInt32(private_obj, index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_FLAG, flags_int)
		end
		
		--设置地图坐标不可掉落
		mapLib.SetMapSite(map, x, y)
		--将新增的战利品位置插入到列表
		mapLib.InsertLootIndex(public_obj, index)
		return true
	end
	return false
end


--添加七龙珠铜钱雨
function AddQiLongZhuTongQianYu(map, num, x, y)
	local public_obj, private_obj, index = mapLib.GetLootObject(map, x, y)		
	--outDebug(string.format('AddLootGameObject %f %f %u', x, y, index))
	if public_obj == nil then
		outDebug("AddQiLongZhuTongQianYu public_obj == nil")
		return
	end
	if private_obj == nil then
		outDebug("AddQiLongZhuTongQianYu private_obj == nil")
		return
	end
	if index == 4294967295 then
		outDebug("AddQiLongZhuTongQianYu index == 4294967295")
		return
	end
	--模板
	binLogLib.SetUInt32(public_obj, LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_ENTRY, Item_Loot_Silver)
	--数量
	binLogLib.SetUInt32(public_obj, LOOT_PUBLIC_INT_BEGIN + index * MAX_LOOT_PUBLIC_INT_FIELD + LOOT_PUBLIC_INT_MONEY_SUM, num)
	--战利品存活时间			
	binLogLib.SetUInt32(private_obj, index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_EXIST_TIME, 300 * 1000)
	--战利品拥有者保护时间
	binLogLib.SetUInt32(private_obj, index * MAX_LOOT_PRIVATE_INT_FIELD + LOOT_PRIVATE_INT_OWNER_TIME, config.loot_change_owner)
	--拥有者ID
	binLogLib.SetStr(public_obj, LOOT_STR_FIELD_BEGIN + index * MAX_LOOT_PUBLIC_STR_FIELD + LOOT_PUBLIC_STR_OWNER, "")

	--设置地图坐标不可掉落
	mapLib.SetMapSite(map, x, y)
	--将新增的战利品位置插入到列表
	mapLib.InsertLootIndex(public_obj, index)
	return true
end