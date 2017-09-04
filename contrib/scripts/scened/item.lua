------------使用物品
----------------------------------------------------

----------------------------------------------------
--以下为函数
function DoUseItemScript(user, item_entry, count)
	local userInfo = UnitInfo:new{ptr = user}
	local mapid = userInfo:GetMapID()
	local map_ptr = unitLib.GetMap(user)
	if map_ptr == nil then
		outFmtError("player %s is not alive or map_ptr == nil! %s %s", userInfo:GetPlayerGuid(), tostring(userInfo:IsAlive()), tostring(map_ptr))
		return false	--失败
	end

	--特殊处理的道具
	if ScenedUseItem[item_entry] then		
		return ScenedUseItem[item_entry](ScenedUseItem, userInfo, item_entry, count)
	end
	
	local config = tb_item_template[item_entry]
	if not config then return false end
	local item_type = config.type
	--药品
	if item_type == ITEM_TYPE_MEDICINE then
		--否禁用了恢复
		if mapLib.GetCanRecovey(map_ptr) == false then
			outFmtError(" error : DoUseItemScript use item fail map donot recovey, mapid= %d", mapid)
			return false
		end
		--玩家死了就不能用了
		if(userInfo:IsAlive() == false)then
			outFmtError(" error : DoUseItemScript use item fail player id die ")
			return false
		end
		local info = config.using_effect
		for i = 1, #info, 2 do
			if info[i][1] == 1 then
				--outFmtDebug("use yao %d",info[i][2])
				userInfo:ModifyHealth(info[i][2])
			end
			--if info[i] == 1 then			--加生命
			--	userInfo:ModifyHealth(info[i+1])
			--elseif info[i] == 2 then		--加法力
			--	userInfo:ModifyPower(POWER_MANA, info[i+1])
			--elseif info[i] == 3 then		--加体力
			--	userInfo:ModifyPower(POWER_ENERGY, info[i+1])
			--end
		end
		return true
	--PK药
	elseif item_type == ITEM_TYPE_PK_MEDICINE then
		
	--宠物药
	elseif item_type == ITEM_TYPE_PET_MEDICINE then
	
	--获得buff
	elseif item_type == ITEM_TYPE_BUFF then
		local using_effect = config.using_effect
		for i = 1,#using_effect,2 do
			local buff_id = using_effect[i]
			SpelladdBuff(user, buff_id, user, 1, tb_buff_template[buff_id].duration)
		end
	end

	return true
end

ScenedUseItem = {
	--还魂丹
	[50003] = function(self, userInfo, item_entry, count)
		if(userInfo:IsAlive() == false and userInfo:GetLevel() > 0)then
			local map_ptr = unitLib.GetMap(userInfo.ptr)
			if not map_ptr then return false end
			-- 非野外地图不能使用复活
			local mapid = unitLib.GetMapID(userInfo.ptr)
			if tb_map[mapid].type ~= MAP_TYPE_FIELD then
				return false
			end
			
			local mapid = unitLib.GetMapID(userInfo.ptr)
			userInfo:SetUseRespawnMapId(mapid)
			unitLib.Respawn(userInfo.ptr, RESURRPCTION_HUANHUNDAN, 100)	--原地复活
			
			return true
		end
		return false
	end,
	--生命源泉
	[171] = function(self, userInfo, item_entry, count)
		local val = userInfo:GetAutoHp()
		userInfo:SetAutoHp(val + 100000*count)
		return true
	end,
	--1.5倍经验丹 BUFF_ONEPOINTFIVE_JINGYAN
	[201] = function(self, userInfo, item_entry, count)
		SpelladdBuff(userInfo.ptr, BUFF_ONEPOINTFIVE_JINGYAN, userInfo.ptr, 1, 3600)
		return true
	end,
	--2倍经验丹 BUFF_TOW_JINGYAN
	[202] = function(self, userInfo, item_entry, count)
		SpelladdBuff(userInfo.ptr, BUFF_TOW_JINGYAN, userInfo.ptr, 1, 3600)
		return true
	end,
	--3倍经验丹 BUFF_THREE_JINGYAN
	[203] = function(self, userInfo, item_entry, count)
		SpelladdBuff(userInfo.ptr, BUFF_THREE_JINGYAN, userInfo.ptr, 1, 3600)
		return true
	end,
	--5倍经验丹 BUFF_FIVE_JINGYAN
	[204] = function(self, userInfo, item_entry, count)
		SpelladdBuff(userInfo.ptr, BUFF_FIVE_JINGYAN, userInfo.ptr, 1, 3600)
		return true
	end,
	--女儿红
	[613] = function(self, userInfo, item_entry, count)
		userInfo:ModifyHealth(userInfo:GetMaxHealth()*0.3)
		return true
	end,
	--九花玉露丸
	[620] = function(self, userInfo, item_entry, count)
		userInfo:ModifyHealth(userInfo:GetMaxHealth())
		return true
	end,
	--随机符
	[601] = function(self, userInfo, item_entry, count)
		local map_ptr = unitLib.GetMap(userInfo.ptr)
		if map_ptr == nil then return false end
		--传送到随机点
		local randX, randY = mapLib.RandConsLootPoint(map_ptr)
		unitLib.SetPos(userInfo.ptr, randX, randY, true)
		return true
	end,
}





