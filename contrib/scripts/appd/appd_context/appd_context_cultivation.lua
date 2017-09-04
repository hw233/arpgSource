
local XIULIANCHANG_MAP_ID = 3005
local ROBOT_CHEST_NUM = 3
--C++接口
function PlayerInfo:LogoutBackupCultivation()
	playerLib.SetCultivation(self:GetGuid(), self:GetCultivationStartTime(),self:GetCultivationLostInfo(),self:GetWeapon(),self:GetAvatar(),self:GetDivine(),self:GetVIP())
end

function PlayerInfo:LoginRestoreCultivation()
	local info = playerLib.GetCultivation(self:GetGuid(),-1)
	if info ~= nil and info[1] ~= 0  then
		outFmtDebug("LoginRestoreCultivation : tiem:%d , lost: %d",info[1],info[2])
		self:SetCultivation(info[1],info[2])
	end
end


--设置自己的修炼场信息
function PlayerInfo:SetCultivation(time,lost)
	--playerLib.SetCultivation(self:GetGuid(), time,lost,self:GetWeapon(),self:GetAvatar(),self:GetDivine(),self:GetVIP())
	self:SetCultivationStartTime(time)
	self:SetCultivationLostInfo(lost)
end

function PlayerInfo:SetCultivationStartTime(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_START_TIME,value)
end

function PlayerInfo:GetCultivationStartTime()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_START_TIME)
end

function PlayerInfo:SetCultivationLostInfo(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_LOST_INFO,value)
end
function PlayerInfo:GetCultivationLostInfo()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_LOST_INFO)
end

--设置guid对应角色修炼场信息的某项 index[0~5] 0:time 1:lost 2:weapon 3:avatar 4:divine 5:vip
function PlayerInfo:SetCultivationIndex(guid,index,value)
	--playerLib.SetCultivationByIndexValue(guid,index,value)
	local player = app.objMgr:getObj(guid)
	-- 没有就加离线文件
	if not player then 
		playerLib.SetCultivationByIndexValue(guid,index,value)
	else
		if index ==0 then
			player:SetCultivationStartTime(value)
		elseif index == 1 then
			player:SetCultivationLostInfo(value)
		end
	end
	
end
--获取guid对应角色修炼场信息 暂时没有用
function PlayerInfo:GetCultivation(guid)
	--local info = playerLib.GetCultivation(guid, -1)
	--return info
end
--获取guid对应角色修炼场信息的某项
function PlayerInfo:GetCultivationIndex(guid,index)
	--[[
	local info = playerLib.GetCultivation(guid, index)
	if info ~= nil then
		return info[1]
	else
		return 0 --未开启修炼 但被别人从斗剑台抽到了 先置为0
	end
	--]]
	local player = app.objMgr:getObj(guid)
	-- 没有就加离线文件
	if not player then 
		local info = playerLib.GetCultivation(guid,index)
		if info ~= nil then
			return info[1]
		else
			return 0 --未开启修炼 但被别人从斗剑台抽到了 先置为0
		end
	else
		if index ==0 then
			return player:GetCultivationStartTime()
		elseif index == 1 then
			return player:GetCultivationLostInfo()
		elseif index ==5 then
			return player:GetVIP()
		end
	end
	
end


--binlog接口
--10条修炼记录(guid, 结果, 是否被掠夺,时间) 当前4名对手(guid)  当日累计掠夺经验 当日累计掠夺宝箱数  当前剩余挑战次数 当日购买次数 上次满体力挑战时间戳  上次刷新对手时间 本轮被掠夺成功次数 本日总掠夺次数 

--上次挑战时间戳
function PlayerInfo:SetCultivationLastPlunderTime(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_LAST_PLUNDER_TIME,value)
end

function PlayerInfo:GetCultivationLastPlunderTime()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_LAST_PLUNDER_TIME)
end

--上次刷新对手时间
function PlayerInfo:SetCultivationLastRefreshTime(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_LAST_REFRESH_TIME,value)
end

function PlayerInfo:GetCultivationLastRefreshTime()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_LAST_REFRESH_TIME)
end

--当日累计掠夺经验
function PlayerInfo:SetCultivationTodayPlunderExp(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PLUNDER_EXP,value)
end

function PlayerInfo:GetCultivationTodayPlunderExp()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PLUNDER_EXP)
end

function PlayerInfo:AddCultivationTodayPlunderExp(value)
	return self:AddUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PLUNDER_EXP,value)
end

--当日累计掠夺宝箱数
function PlayerInfo:SetCultivationTodayPlunderChest(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PLUNDER_CHEST,value)
end

function PlayerInfo:GetCultivationTodayPlunderChest()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PLUNDER_CHEST)
end

function PlayerInfo:AddCultivationTodayPlunderChest(value)
	return self:AddUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PLUNDER_CHEST,value)
end

--当日购买次数
function PlayerInfo:SetCultivationTodayPurchaseCount(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PURCHASE_COUNT,value)
end

function PlayerInfo:GetCultivationTodayPurchaseCount()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PURCHASE_COUNT)
end

function PlayerInfo:AddCultivationTodayPurchaseCount(value)
	return self:AddUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_PURCHASE_COUNT,value)
end

--当前剩余挑战次数
function PlayerInfo:SetCultivationLeftPlunderCount(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_LEFT_PLUNDER_COUNT,value)
end

function PlayerInfo:GetCultivationLeftPlunderCount()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_LEFT_PLUNDER_COUNT)
end

function PlayerInfo:AddCultivationLeftPlunderCount(value)
	return self:AddUInt32(PLAYER_INT_FIELD_CULTIVATION_LEFT_PLUNDER_COUNT,value)
end

--10条修炼记录下标
function PlayerInfo:SetCultivationPlunderRecordIndex(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_PLUNDER_RECORD_INDEX,value)
end

function PlayerInfo:GetCultivationPlunderRecordIndex()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_PLUNDER_RECORD_INDEX)
end

function PlayerInfo:AddCultivationPlunderRecordIndex(value)
	return self:AddUInt32(PLAYER_INT_FIELD_CULTIVATION_PLUNDER_RECORD_INDEX,value)
end
--本轮被掠夺成功次数

function PlayerInfo:SetCultivationTotalBeenPlunderCount(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_TOTAL_BEEN_PLUNDER_COUNT,value)
end

function PlayerInfo:GetCultivationTotalBeenPlunderCount()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_TOTAL_BEEN_PLUNDER_COUNT)
end

function PlayerInfo:AddCultivationTotalBeenPlunderCount(value)
	return self:AddUInt32(PLAYER_INT_FIELD_CULTIVATION_TOTAL_BEEN_PLUNDER_COUNT,value)
end

--本日总掠夺次数
function PlayerInfo:SetCultivationTodayTotalPlunderCount(value)
	self:SetUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_TOTAL_PLUNDER_COUNT,value)
end

function PlayerInfo:GetCultivationTodayTotalPlunderCount()
	return self:GetUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_TOTAL_PLUNDER_COUNT)
end

function PlayerInfo:AddCultivationTodayTotalPlunderCount(value)
	return self:AddUInt32(PLAYER_INT_FIELD_CULTIVATION_TODAY_TOTAL_PLUNDER_COUNT,value)
end

--当前4名对手guid
function PlayerInfo:SetCultivationRivalGuid(index,value)
	if index <0 or index >= MAX_PLAYER_CULTIVATION_RIVAL_COUNT then
		return
	end
	self:SetStr(PLAYER_STRING_INT_FIELD_CULTIVATION_RIVAL_GUID_START+index,value)
end

function PlayerInfo:GetCultivationRivalGuid(index)
	if  index >=0 and index < MAX_PLAYER_CULTIVATION_RIVAL_COUNT then
		return self:GetStr(PLAYER_STRING_INT_FIELD_CULTIVATION_RIVAL_GUID_START+index)
	else
		return nil
	end
end

--10条修炼记录
function PlayerInfo:SetCultivationPlunderRecord(index, value)
	
	self:SetStr(PLAYER_STRING_INT_FIELD_CULTIVATION_PLUNDER_RECORD_START + index,value)
end


--索引加1
function PlayerInfo:CultivationPlunderRecordNext()
	local cur_num = self:GetCultivationPlunderRecordIndex()
	if(cur_num >= MAX_PLAYER_CULTIVATION_PLUNDER_RECORD_COUNT-1)then		--最多保留MAX_PLAYER_CULTIVATION_PLUNDER_RECORD_COUNT
		self:SetCultivationPlunderRecordIndex(0)	
	else
		self:AddCultivationPlunderRecordIndex(1)
	end
end	

--添加一个记录信息 guid |名称 |结果(0:己方战斗胜利-反击 1:己方战斗失败-复仇) |标记(0:未被掠夺 1~n:n号宝箱被掠夺) |时间戳 |反击标识(0:未反击 1:已反击)
function PlayerInfo:AddCultivationPlunderRecord(guid,robot_id, name, lv, result, flag, time, revenge)
	local info = guid..'|'..robot_id..'|'..name..'|'..lv..'|'..result..'|'..flag..'|'..time..'|'..revenge
	local index = self:GetCultivationPlunderRecordIndex()
	self:SetCultivationPlunderRecord(index, info)
	self:CultivationPlunderRecordNext()
	
	outFmtDebug("AddCultivationPlunderRecord:%s",info)
	return 1
end

--取得一个记录信息
function PlayerInfo:GetCultivationPlunderRecord(index)
	if  index >=0 and index < MAX_PLAYER_CULTIVATION_PLUNDER_RECORD_COUNT then
		local info = self:GetStr(PLAYER_STRING_INT_FIELD_CULTIVATION_PLUNDER_RECORD_START + index)
		return info
	else
		return nil
	end
end

--修改记录复仇标记为1
function PlayerInfo:SetCultivationPlunderRecordAfterRevenge(index)
	local info = self:GetCultivationPlunderRecord(index)
	local values = string.split(info, "|")

	local new_info = values[1]..'|'..values[2]..'|'..values[3]..'|'..values[4]..'|'..values[5]..'|'..values[6]..'|'..values[7]..'|'..'1'
	
	self:SetCultivationPlunderRecord(index,new_info)
	
end




--业务接口

--判断该功能模块是否开启  依据刷新对手时间判断
function PlayerInfo:IsCultivationUnlocked()
	if self:GetCultivationLastRefreshTime() == 0 then
		return false
	else
		return true
	end
end

--模块解锁时初始化
function PlayerInfo:InitCultivation()
	local now_time = os.time()
	--刷新对手
	self:SetCultivationLastRefreshTime(1)
	self:RefreshCultivationRivals()
	--重置可用次数为最大
	local max_count = tb_xiulianchang_base[1].max_plunder_recover_count
	self:SetCultivationLeftPlunderCount(max_count)
	--设置c++数据 开始修炼时间 宝箱未被强夺
	self:SetCultivation(now_time,0)
end

--每日重置 挑战次数 本日记录
function PlayerInfo:DailyResetCultivation()
	if not self:IsCultivationUnlocked() then
		return
	end
	--清空本日相关数据
	self:SetCultivationTodayPlunderExp(0)
	self:SetCultivationTodayPlunderChest(0)
	self:SetCultivationTodayPurchaseCount(0)
	self:SetCultivationTodayTotalPlunderCount(0)
end


--每1s更新/上线时更新 挑战次数次数
function PlayerInfo:UpdateCultivation()
	if not self:IsCultivationUnlocked() then
		
		return
	end
	--检测时间 符合条件则增加可用掠夺次数
	local start_time =  self:GetCultivationLastPlunderTime()
	local current_count = self:GetCultivationLeftPlunderCount()
	local recover_time = tb_xiulianchang_base[1].plunder_recover_time
	local max_recover_count = tb_xiulianchang_base[1].max_plunder_recover_count
	local now_time = os.time()
	if current_count >= max_recover_count then
		return
	end
	local recover = 0
	while (now_time - start_time >= recover_time) do
		recover = recover + 1
		start_time = start_time + recover_time
		current_count = current_count + 1
		if current_count >= max_recover_count then
			self:AddCultivationLeftPlunderCount(recover)
			self:SetCultivationLastPlunderTime(start_time)
			return
		end
	end
	if recover > 0 then
		self:AddCultivationLeftPlunderCount(recover)
		self:SetCultivationLastPlunderTime(start_time)
	end
	
end

--获取个人修炼状态 返回客户端  (修炼开始时间,被抢夺状态)
function PlayerInfo:GetCultivationInfo()
	if not self:IsCultivationUnlocked() then
		return
	end
	local start_time = self:GetCultivationIndex(self:GetGuid(),0)
	local lost_info = self:GetCultivationIndex(self:GetGuid(),1)
	self:call_update_cultivation_info(start_time, lost_info)
end
--获取匹配对手角色信息 返回客户端 自定义结构rival_info:(index  姓名 等级 外观 武器 神兵 战力 宝箱数量)
function PlayerInfo:GetCultivationRivalsInfo()
	if not self:IsCultivationUnlocked() then
		return
	end
	local list = {}
	for i=0,3 do
		local info = 	self:GetCultivationRivalGuid(i)
		local temp = string.split(info,'@')
		if #temp ~= 2 then
			return
		end
		local token = string.split(temp[1],'|')
		if #token ~= 2 then
			return
		end
		local rank = 0
		local guid = token[1]
		local robot_id = tonumber(token[2])
		local content = temp[2]

		if rank ~= 0 then --斗剑台排行对应人物 --此if部分已无用
			local config = globalCounter:GetRankInfo(rank)
			local stru = cultivation_rivals_info_t .new()
			stru.index	= i
			stru.name 	= config.name
			stru.level	= config.level
			stru.weapon	= config.weapon
			stru.avatar	= config.avatar
			stru.divine	= config.divine
			stru.force	= config.force
			if string.len(config.guid) <= 0 then
				stru.chest = ROBOT_CHEST_NUM
			else
				local vip = config.vip
				local start_time = self:GetCultivationIndex(config.guid,0)  --若未开启 则生成假人
				local lost_info = self:GetCultivationIndex(config.guid,1)
				if start_time == 0 and lost_info == 0 then
					stru.chest = 1
				else
					local lost_list = self:LostInfoToList(lost_info)
					local vip_info = tb_xiulianchang_vip[vip]
					local time_limit = vip_info.time_limit
					--计算修炼时间
					local cultivation_time = os.time() - start_time
					
					if cultivation_time < tb_xiulianchang_base[1].get_reward_time_limit then
						stru.chest	= 0
					else
						if cultivation_time > time_limit then
							cultivation_time = time_limit
						end
						local count = 0
						for index,info in ipairs(tb_xiulianchang_reward) do
							if cultivation_time >= info.time and not IsKeyInTable(index,lost_list) then
								count = count + 1
								
							end
						end
						stru.chest = count 
					end
				end
				
				
			end
			stru.gender = config.gender
			table.insert(list, stru)
			
		elseif robot_id ~= 0 then --修炼场机器人
			local config = tb_xiulianchang_dummy[robot_id]
			local stru = cultivation_rivals_info_t .new()
			stru.index	= i
			stru.name 	= config.name
			stru.level	= config.level
			stru.weapon	= config.weapon
			stru.avatar	= config.avatar
			stru.divine	= config.divine
			stru.force	= config.force
			stru.chest	= ROBOT_CHEST_NUM 
			stru.gender	= config.gender
			table.insert(list, stru)
		else --从content中获取
			local config = GetDummyInfoFromContent(content)
			local tempcontent = ''
			if string.len(config.guid) > 0 then --玩家则更新数据
				local data = {}
				data.name = 'Handle_GetCultivationRivalsInfo'
				data.callback_guid = config.guid
				data.my_guid = self:GetGuid()
				function data.fun (data, objs)
					outFmtDebug("callbacked ===================")
					local targetPlayer = objs[data.callback_guid]
					if not targetPlayer then return end
					outFmtDebug("target player = %s", targetPlayer:GetGuid())
					
					local tempconfig = targetPlayer:GetDummyInfo()
					tempcontent =  JoinDummyInfoIntoContent(data.callback_guid,tempconfig)
				end
				GetObjects(data)
			end
			if tempcontent ~= '' then
				config = GetDummyInfoFromContent(tempcontent)
			end
			local stru = cultivation_rivals_info_t .new()
			stru.index	= i
			stru.name 	= config.name
			stru.level	= config.level
			stru.weapon	= config.weapon
			stru.avatar	= config.avatar
			stru.divine	= config.divine
			stru.force	= config.force
			if string.len(config.guid) <= 0 then
				stru.chest = ROBOT_CHEST_NUM
			else
				local vip = config.vip
				local start_time = self:GetCultivationIndex(config.guid,0)  --若未开启 则生成假人
				local lost_info = self:GetCultivationIndex(config.guid,1)
				if start_time == 0 and lost_info == 0 then
					stru.chest = 1
				else
					local lost_list = self:LostInfoToList(lost_info)
					local vip_info = tb_xiulianchang_vip[vip]
					local time_limit = vip_info.time_limit
					--计算修炼时间
					local cultivation_time = os.time() - start_time
					
					if cultivation_time < tb_xiulianchang_base[1].get_reward_time_limit then
						stru.chest	= 0
					else
						if cultivation_time > time_limit then
							cultivation_time = time_limit
						end
						local count = 0
						for index,info in ipairs(tb_xiulianchang_reward) do
							if cultivation_time >= info.time and not IsKeyInTable(index,lost_list) then
								count = count + 1
								
							end
						end
						stru.chest = count 
					end
				end
				
				
			end
			stru.gender = config.gender
			table.insert(list, stru)
		end
	end

	self:call_update_cultivation_rivals_info_list(list)
end

--领取修炼奖励
function PlayerInfo:GetCultivationReward()
	if not self:IsCultivationUnlocked() then
		return
	end
	--取得修炼开始时间 宝箱状态
	local start_time = self:GetCultivationIndex(self:GetGuid(),0)
	local lost_info = self:GetCultivationIndex(self:GetGuid(),1)
	local lost_list = self:LostInfoToList(lost_info)
	
	--IsKeyInTable(1,lost_list)
	--判断修炼时间是否足够进行领取
	--根据角色等级 vip等级 修炼时间 被掠夺次数 宝箱情况  产生奖励
	local vip = self:GetVIP()
	local vip_info = tb_xiulianchang_vip[vip]
	local time_limit = vip_info.time_limit
	
	

	--计算修炼时间
	local cultivation_time = os.time() - start_time
	if cultivation_time < tb_xiulianchang_base[1].get_reward_time_limit then
		return
	end

	if cultivation_time > time_limit then
		cultivation_time = time_limit
	end
	
	local next_exp = tb_char_level[self:GetLevel()].next_exp
	local extend_exp_reward  = vip_info.extend_exp_reward
	local base_exp_reward = tb_xiulianchang_base[1].base_exp_reward
	local base_exp_time_unit = tb_xiulianchang_base[1].base_exp_time_unit
	local plunder_exp_lost = tb_xiulianchang_base[1].plunder_exp_lost
	local been_plunder_count = self:GetCultivationTotalBeenPlunderCount()
	local max_been_plunder_count = tb_xiulianchang_base[1].max_been_plunder_count
	if been_plunder_count > max_been_plunder_count then
		been_plunder_count = max_been_plunder_count
	end
	
	local exp_reward = math.floor(math.floor(cultivation_time / base_exp_time_unit)* math.floor(next_exp * (base_exp_reward/10000))*(extend_exp_reward/100 + 1)*(1 - been_plunder_count * plunder_exp_lost / 10000))
	
	local rewards = {}
	table.insert(rewards,{Item_Loot_Exp,exp_reward}) --加经验
	for index,info in ipairs(tb_xiulianchang_reward) do
		if cultivation_time >= info.time and not IsKeyInTable(index,lost_list) then
			for k,v in ipairs(info.reward) do
				table.insert(rewards,v)
			end
			
		end
	end
	self:AppdAddItems(rewards,MONEY_CHANGE_CULTIVATION_REWARD,LOG_ITEM_OPER_TYPE_CULTIVATION_REWARD)
	
	--重置修炼开始时间
	--设置c++数据 开始修炼时间 宝箱未被强夺
	self:SetCultivation(os.time(),0)
	--清零被掠夺成功次数
	self:SetCultivationTotalBeenPlunderCount(0)
	
	self:GetCultivationInfo()
	
end

function PlayerInfo:LostInfoToList(info)
	local list = {}
	local i = 1
	while info ~= 0 do
		local flag  = info % 2
		info = math.floor(info / 2)
		if flag == 1 then
			list[i] = flag
		end
		i = i + 1
	end
	return list
end

function PlayerInfo:ListToLostInfo(list)
	local info = 0
	for i,flag in pairs(list) do
		info = info + flag * 2^(i-1)
	end
	return info
end

function PlayerInfo:CanRefreshCultivationRivals()
	local last_time = self:GetCultivationLastRefreshTime()
	if os.time() - last_time > tb_xiulianchang_base[1].rival_refresh_cd then
		return true
	else
		return false
	end
end

--刷新匹配对手
function PlayerInfo:RefreshCultivationRivals()
	if not self:IsCultivationUnlocked() then
		return
	end
	if not self:CanRefreshCultivationRivals() then
		return
	end
	--匹配抽取 玩家guid或机器人id 
	local player_rank = self:GetDoujiantaiRank()
	local indx = #tb_xiulianchang_fight_range
	
	for i = 1, #tb_xiulianchang_fight_range do
		local config = tb_xiulianchang_fight_range[i]
		if config.ra <= player_rank and player_rank <= config.rb then
			indx = i
			break
		end
	end
	ranges = tb_xiulianchang_fight_range[indx].chooseRange
	robot_count = tb_xiulianchang_fight_range[indx].robot_count
	
	local dict = {}
	for _, range in pairs(ranges) do
		local rank = player_rank
		if rank == 0 then
			rank = MAX_DOUJIANTAI_RANK_COUNT + 1
		end
		
		local l = rank - range[ 2 ]
		local r = rank - range[ 1 ]
		if l <=0 then
			l = 1
		end
		if r >= MAX_DOUJIANTAI_RANK_COUNT +1 then
			r = MAX_DOUJIANTAI_RANK_COUNT
		end
		
		local rank = randInt(l, r)
		--rank = 924 --测试固定rank敌人
		
		if  rank == player_rank then
			outFmtDebug('RefreshCultivationRivals: error rank == player_rank')
			return
		end
		if rank > 0 then
			local config = globalCounter:GetRankInfo(rank)
			if string.len(config.guid) > 0 then
				local content =  JoinDummyInfoIntoContent(config.guid,config)
				table.insert(dict, config.guid .. '|0@'..content ) --第一段记录斗剑台人物guid 第二段为0 第三段为玩家content
			else
				local content =  JoinDummyInfoIntoContent('',config)
				table.insert(dict,'|0@'..content ) --第一段记录空串 第二段为0 第三段为机器人content
			end
			
		end
	end
	
	robot_ranges = {{1,250},{251,500},{501,750},{751,1000}}
	for i = 1, robot_count do
		local robot_range = robot_ranges[i]
		local robot_index = randInt(robot_range[1], robot_range[2])
		table.insert(dict, '|' .. robot_index..'@') --第一段为空串 第二段为修炼机器人id 第三段为空串
	end
	
	
	--dict[1] = '966|0' --测试固定rank敌人
	for i = 1, #dict do
		outFmtDebug("RefreshCultivationRivals: new rival_%d guid : %s",i,dict[i])
		self:SetCultivationRivalGuid(i-1,dict[i])  
	end
	self:SetCultivationLastRefreshTime(os.time())
	self:GetCultivationRivalsInfo()
end

--进行掠夺
function PlayerInfo:PlunderCultivationRival(index)
	if not self:IsCultivationUnlocked() then
		return
	end
	
	--检测剩余次数 最大次数 
	--若是满体力时挑战,设置开始回复
	if self:GetCultivationLeftPlunderCount() <= 0 then
		return
	end
	if self:GetCultivationLeftPlunderCount() == tb_xiulianchang_base[1].max_plunder_recover_count then
		self:SetCultivationLastPlunderTime(os.time())

	end
	self:AddCultivationLeftPlunderCount(-1)
	--取得对手guid 获取角色信息
	--进战斗相关处理
	local info = self:GetCultivationRivalGuid(index)
	local temp = string.split(info,'@')
	if #temp ~= 2 then
		return
	end
	local token = string.split(temp[1],'|')
	
	if #token ~= 2 then
		return
	end
	local rank = 0
	local guid = token[1]
	local robot_id = tonumber(token[2])
	local content = temp[2]
	local generalId = string.format("%d|%s|%d|%d|%s@%s", os.time(), self:GetGuid(), rank, robot_id,guid,content)
	local x = tb_xiulianchang_base[ 1 ].bornPos[ 1 ][ 1 ]
	local y = tb_xiulianchang_base[ 1 ].bornPos[ 1 ][ 2 ]
	-- 发起传送
	call_appd_teleport(self:GetScenedFD(), self:GetGuid(), x, y, XIULIANCHANG_MAP_ID, generalId)
	--刷新对手
	self:RefreshCultivationRivals()
end

--进行复仇
function PlayerInfo:RevengeCultivationRival(index)
	if not self:IsCultivationUnlocked() then
		return
	end
	--检测能否复仇
	local info = self:GetCultivationPlunderRecord(index)
	local token = string.split(info,'|')
	if #token ~= 8 then
		return
	end
	local guid = (token[1])
	local robot_id = tonumber(token[2])
	local revenge = tonumber(token[8])
	if revenge == 1 then
		return
	end
	
	self:SetCultivationPlunderRecordAfterRevenge(index)
	
	local content = ''
	if string.len(guid) > 0 then
		local data = {}
		data.name = 'Handle_RevengeCultivationRival'
		data.callback_guid = guid
		data.my_guid = self:GetGuid()
		function data.fun (data, objs)
			outFmtDebug("callbacked ===================")
			local targetPlayer = objs[data.callback_guid]
			if not targetPlayer then return end
			outFmtDebug("target player = %s", targetPlayer:GetGuid())
			
			local config = targetPlayer:GetDummyInfo()
			content =  JoinDummyInfoIntoContent(guid,config)
		end
		GetObjects(data)
	end
	
	
	
	
	local generalId = string.format("%d|%s|%d|%d|%s@%s", os.time(), self:GetGuid(), 0, robot_id, guid,content)
	local x = tb_xiulianchang_base[ 1 ].bornPos[ 1 ][ 1 ]
	local y = tb_xiulianchang_base[ 1 ].bornPos[ 1 ][ 2 ]
	-- 发起传送
	call_appd_teleport(self:GetScenedFD(), self:GetGuid(), x, y, XIULIANCHANG_MAP_ID, generalId)
	--取得对手guid 获取角色信息
	--进战斗相关处理
end

--购买挑战次数
function PlayerInfo:BuyCultivationLeftPlunderCount(count)
	if not self:IsCultivationUnlocked() then
		return
	end
	--print('BuyCultivationLeftPlunderCount')
	--检测购买条件 vip等级 购买次数上限
	local vip = self:GetVIP()
	local vip_info = tb_xiulianchang_vip[vip]
	if vip_info == nil then
		return
	end
	local buy_limit = vip_info.buy_limit
	local buy_price = vip_info.buy_price
	local current_buy_count = self:GetCultivationTodayPurchaseCount()
	
	if current_buy_count + count > buy_limit then
		return
	end
	
	--处理 消费
	if self:costMoneys(MONEY_CHANGE_CULTIVATION_PURCHASE,buy_price,1) then
		self:AddCultivationTodayPurchaseCount(count)
		self:AddCultivationLeftPlunderCount(count)
	end
	
end


function PlayerInfo:PlunderFinish(data, info)
	local token = string.split(info,'|')
	local guid = token[1]
	local robot_id = tonumber(token[2])
	local level = tonumber(token[3]) 
	local result = tonumber(token[4]) 
	local enemy_name = token[5]
	
	if result == GlobalCounter.WIN then
		
		
		if string.len(guid) > 0 then--战胜玩家
			local start_time = self:GetCultivationIndex(guid,0)
			local lost_info = self:GetCultivationIndex(guid,1)
			
			local vip = self:GetCultivationIndex(guid,5)
			local vip_info = tb_xiulianchang_vip[vip]
			local time_limit = vip_info.time_limit
			local lost_list = self:LostInfoToList(lost_info)
			
			local next_exp = tb_char_level[level].next_exp
			local plunder_exp_reward = tb_xiulianchang_base[1].plunder_exp_reward
			
			
			local exp_reward = math.floor(next_exp * plunder_exp_reward / 10000)
			
			local rewards = {}
			local lost_chest = 0
			table.insert(rewards,{Item_Loot_Exp,exp_reward}) --加经验
			
			--计算修炼时间
			local cultivation_time = os.time() - start_time
			
			if cultivation_time > time_limit then
				cultivation_time = time_limit
			end
			if cultivation_time < tb_xiulianchang_base[1].get_reward_time_limit then
				--no chest
			else
				
				for i = #tb_xiulianchang_reward,1,-1 do
					local info = tb_xiulianchang_reward[i]
					if cultivation_time >= info.time and not IsKeyInTable(i,lost_list) then
						local random = randInt(1,10000)
						if random <= info.lost_rate then
							lost_chest = i
							lost_list[i] = 1
							if start_time ~= 0 then-- 对方已开启修炼 才产生影响
								self:SetCultivationIndex(guid,1, self:ListToLostInfo(lost_list))
							end
							for k,v in ipairs(info.reward) do
								table.insert(rewards,v)
							end
							break
						end
							
					end
				end
			end
			
			self:AppdAddItems(rewards,MONEY_CHANGE_CULTIVATION_PLUNDER_REWARD,LOG_ITEM_OPER_TYPE_CULTIVATION_PLUNDER_REWARD)
			
			--发送结果
			local list = {}
			for _,v in ipairs(rewards) do
			--奖励通知
				local stru = item_reward_info_t .new()
				stru.item_id	= v[1]
				stru.num 		= v[2]
				table.insert(list, stru)
			end
			self:call_show_cultivation_result_list(result,enemy_name,list)
			
			
			self:AddCultivationTodayPlunderExp(exp_reward)
			if lost_chest > 0 then
				self:AddCultivationTodayPlunderChest(1)
			end
			
			if start_time ~= 0 then-- 对方已开启修炼 才产生印象
				
			
				local data = {}
				data.name = 'Handle_PlunderFinish'
				data.callback_guid = guid
				data.my_guid = self:GetGuid()
				function data.fun (data, objs)
					outFmtDebug("callbacked ===================")
					local targetPlayer = objs[data.callback_guid]
					if not targetPlayer then return end
					outFmtDebug("target player = %s", targetPlayer:GetGuid())
					
					targetPlayer:AddCultivationTotalBeenPlunderCount(1)
					
					targetPlayer:AddCultivationPlunderRecord(self:GetGuid(),0, self:GetName(), self:GetLevel(), result, lost_chest, os.time(), 0)
					
					
				end
				GetObjects(data)
			end
		else --战胜机器人
			local next_exp = tb_char_level[level].next_exp
			local plunder_exp_reward = tb_xiulianchang_base[1].plunder_exp_reward
			
			
			local exp_reward = math.floor(next_exp * plunder_exp_reward / 10000)
			
			local rewards = {}
			local lost_chest = 0
			table.insert(rewards,{Item_Loot_Exp,exp_reward}) --加经验
			
			for i = ROBOT_CHEST_NUM,1,-1 do
				local info = tb_xiulianchang_reward[i]
				
				local random = randInt(1,10000)
				if random <= info.lost_rate then
					lost_chest = i
					for k,v in ipairs(info.reward) do
						table.insert(rewards,v)
					end
					break
				end

			end
			
			
			self:AppdAddItems(rewards,MONEY_CHANGE_CULTIVATION_PLUNDER_REWARD,LOG_ITEM_OPER_TYPE_CULTIVATION_PLUNDER_REWARD)
			
			--发送结果
			local list = {}
			for _,v in ipairs(rewards) do
			--奖励通知
				local stru = item_reward_info_t .new()
				stru.item_id	= v[1]
				stru.num 		= v[2]
				table.insert(list, stru)
			end
			self:call_show_cultivation_result_list(result,enemy_name,list)
			
			self:AddCultivationTodayPlunderExp(exp_reward)
			if lost_chest > 0 then
				self:AddCultivationTodayPlunderChest(1)
			end
		end
		
	elseif result == GlobalCounter.LOSE then
		local list = {}
		self:call_show_cultivation_result_list(result,enemy_name,list)
		if string.len(guid) > 0 then--给对方玩家添加记录
			local data = {}
			data.name = 'Handle_PlunderFinish'
			data.callback_guid = guid
			data.my_guid = self:GetGuid()
			function data.fun (data, objs)
				outFmtDebug("PlunderFinish callbacked ===================")
				local targetPlayer = objs[data.callback_guid]
				if not targetPlayer then return end
				outFmtDebug("PlunderFinish target player = %s", targetPlayer:GetGuid())
				
				targetPlayer:AddCultivationPlunderRecord(self:GetGuid(),0, self:GetName(), self:GetLevel(), result, 0, os.time(), 0)
			end
			GetObjects(data)
		end
		
	end
end

