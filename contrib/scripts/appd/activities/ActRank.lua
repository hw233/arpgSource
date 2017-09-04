local ActRank = class("ActRank", ActivityBase)

IActRank = ActRank:new{}

ACT_RANK_TYPE_MERIDIAN = 1
ACT_RANK_TYPE_STRENGTH = 2
ACT_RANK_TYPE_GEM = 3
ACT_RANK_TYPE_DEFINE = 4
ACT_RANK_TYPE_MOUNT = 5
ACT_RANK_TYPE_TALISMAN = 6
ACT_RANK_TYPE_WING = 7

function ActRank:ctor()
	
end

-- 系统初始化
function ActRank:init(actId)
	
	--初始化排行榜
	local act_config = tb_activity_time[actId]
	
	if  act_config.offset <= 0  or  act_config.offset > MAX_ACT_RANK_COUNT then
		outFmtError("!!!!!!!!!!!!!!!!!!!!!!!!!!ActRank init fail bad offset %d",act_config.offset)
	end
	
	local offset = act_config.offset - 1
	
	for index = 0,MAX_ACT_RANK_INFO_COUNT - 1 do
		globalCounter:SetUInt32(GLOBALCOUNTER_INT_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + index,0)
		globalCounter:SetStr(GLOBALCOUNTER_STRING_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + index,"")
	end
	
end

-- 系统结束
function ActRank:finish(actId)
	
	--发送排行榜奖励
	local act_config = tb_activity_time[actId]
	local base_config = tb_activity_rank_base[act_config.params[1]]
	local rank_table = self:GetRankTable(actId)
	
	for index,rank_reward_id in ipairs(base_config.rank_reward) do
		local config = tb_activity_rank_rank_reward[rank_reward_id] 
		if config then
			local rewardStr = ""
			for s,d in ipairs(config.item) do
				if s > 1 then
					rewardStr = rewardStr .. ","
				end
				rewardStr = rewardStr..d[1]..","..d[2]
			end
			if rank_table[index] then
				AddGiftPacksData(rank_table[index][1],0,GIFT_PACKS_TYPE_ACT_RANK,os.time(),os.time() + 86400*30, config.mail_name, config.mail_desc, rewardStr, SYSTEM_NAME)
			end
		end
	end
	
end


-- 更新(目标, 领取状态)
function ActRank:update(playerInfo, actId, params)
	
	--领取奖励
	if params[1] == 1 then
		self:OnGetProcessReward(playerInfo,actId,params[2])
		
	--获取排行榜
	elseif params[1] == 2 then
		self:OnGetRankList(playerInfo,actId)
	end
	
end

-- 玩家每日重置
function ActRank:reset(playerInfo, actId)
	
end



-- 系统更新
function ActRank:systemUpdate(actId, params)
	local update_type = params[1]
	local player = params[2]
	
	local act_config = tb_activity_time[actId]
	local base_config = tb_activity_rank_base[act_config.params[1]]
	
	if update_type ~= base_config.type then
		return
	end
	
	local guid = player:GetGuid()
	local name = player:GetName()
	local value = 0
	
	if update_type == 1 then
		--经脉等级
		local spellMgr = player:getSpellMgr()
		value = spellMgr:getMeridianLevel()
		
	elseif update_type == 2 then
		--装备强化等级
		local spellMgr = player:getSpellMgr()
		for index = 0,EQUIPMENT_COUNT - 1 do
			value = value + spellMgr:GetEquipDevelopStrengthLv(index)
		end
	elseif update_type == 3 then
		--装备宝石等级
		local spellMgr = player:getSpellMgr()
		for index = 0,EQUIPMENT_COUNT - 1 do
			local gem_part_config = tb_equipdevelop_gem_part[index + 1]
			local gem_count = #(gem_part_config.gem_array)
			for gem_index = 0,gem_count -1 do
				value = value + spellMgr:GetEquipDevelopGemLv(index,gem_index)
			end
		end
		
	elseif update_type == 4 then
		--装备精炼
		local spellMgr = player:getSpellMgr()
		for index = 0,EQUIPMENT_COUNT - 1 do
			value = value + spellMgr:GetEquipDevelopRefineRank(index) * 1000 + spellMgr:GetEquipDevelopRefineStar(index)
		end
		
	elseif update_type == 5 then
		--坐骑阶级
		local spellMgr = player:getSpellMgr()
		value = spellMgr:getMountLevel() * 1000 + spellMgr:getMountStar()
	elseif update_type == 6 then
		--法宝阶级
		local spellMgr = player:getSpellMgr()
		local tab = spellMgr:getTalismanList()
		--Ttab(attrs)
		for k,v in pairs(tab) do
			value = value + v
		end
	elseif update_type == 7 then
		--翅膀阶级
		value = player:GetUInt32(PLAYER_INT_FIELD_WINGS_RANK) * 1000 + player:GetUInt32(PLAYER_INT_FIELD_WINGS_STAR)
	end
	
	--更新个人 当前值
	player:SetActivityDataUInt32(actId,0,value)
	
	--更新排行榜
	self:RankInsert(actId,guid,name,value)
	
end

function ActRank:RankInsert(actId,guid,name,value)
	local act_config = tb_activity_time[actId]
	
	local offset = act_config.offset - 1
	
	for index = 0,MAX_ACT_RANK_INFO_COUNT - 1 do
		local temp_int = globalCounter:GetUInt32(GLOBALCOUNTER_INT_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + index)
		local temp_str = globalCounter:GetStr(GLOBALCOUNTER_STRING_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + index)
		
		if temp_int < value then
			local in_int = value
			local in_str = guid .. "\1" .. name
			local out_int = 0
			local out_str = ""
			for temp_index = index, MAX_ACT_RANK_INFO_COUNT - 1 do
				out_int = globalCounter:GetUInt32(GLOBALCOUNTER_INT_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + temp_index)
				out_str = globalCounter:GetStr(GLOBALCOUNTER_STRING_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + temp_index)
				globalCounter:SetUInt32(GLOBALCOUNTER_INT_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + temp_index,in_int)
					globalCounter:SetStr(GLOBALCOUNTER_STRING_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + temp_index,in_str)
				in_int = out_int
				in_str = out_str
				local tokens = string.split(out_str,"\1")
				if out_str == "" or tokens[1] == guid then
					break
				end
				
			end
			break
		end
	end
end

function ActRank:GetRankTable(actId)
	local act_config = tb_activity_time[actId]
	local offset = act_config.offset - 1
	local list = {}
	for index = 0,MAX_ACT_RANK_INFO_COUNT - 1 do
		local temp_int = globalCounter:GetUInt32(GLOBALCOUNTER_INT_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + index)
		local temp_str = globalCounter:GetStr(GLOBALCOUNTER_STRING_FIELD_ACT_RANK_INFO_START + offset * MAX_ACT_RANK_INFO_COUNT + index)
		if temp_str == "" then
			break
		end
		local tokens = string.split(temp_str,"\1")
		table.insert(list,{tokens[1],tokens[2],temp_int})
	end
	
	return list
end

function ActRank:OnGetProcessReward(playerInfo,actId,index)
	local act_config = tb_activity_time[actId]
	local base_config = tb_activity_rank_base[act_config.params[1]]
	
	if index ~= playerInfo:GetActivityDataUInt32(actId,1) + 1 then
		return
	end
	
	if not base_config.process[index] or not base_config.process_reward[index] then
		return
	end
	
	if base_config.process[index] > playerInfo:GetActivityDataUInt32(actId,0) then
		return
	end
	
	local item = tb_activity_rank_process_reward[base_config.process_reward[index]].item
	
	playerInfo:AppdAddItems(item,MONEY_CHANGE_ACT_RANK,LOG_ITEM_OPER_TYPE_ACT_RANK)
	
	playerInfo:SetActivityDataUInt32(actId,1,index)
end

function ActRank:OnGetRankList(playerInfo,actId)
	
	local rank_table = self:GetRankTable(actId)
	
	local list = {}
	for _,v in ipairs(rank_table) do
	--奖励通知
		local stru = act_rank_info_t .new()
		stru.name	= v[2]
		stru.value 	= v[3]
		table.insert(list, stru)
	end
	
	playerInfo:call_activity_opt_show_rank_list(actId,list)
end

return ActRank