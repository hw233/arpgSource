local protocols = require('share.protocols')

-- 切换战斗模式
function ScenedContext:Hanlde_Change_Battle_Mode( pkt )
	
	local mode = pkt.mode
	local prev = self:GetBattleMode()
	
	local mapid = unitLib.GetMapID(self.ptr)
	-- 是否允许切换战斗
	if tb_map[mapid].allowChangeMode == 0 then
		return
	end
	
	-- 判断传过来的传过来的参数合法性
	if mode < 0 or mode >= MAX_BATTLE_MODE or mode == prev then
		return
	end
	
	-- 判断要切换的模式是否在切换列表中
	if not table.find(config.can_change_mode, mode) then
		return
	end
	
	-- 判断原来的模式是否在切换列表中
	if not table.find(config.can_change_mode, prev) then
		return
	end
	
	-- 如果在战斗中不能切换和平模式
	local status = playerLib.GetPlayeCurFightStatus(self.ptr)
	if status == COMBAT_STATE_ENTER and mode == PEACE_MODE then
		self:CallOptResult(OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE, BATTLE_MODE_OPERTE_PEACE_MODE_DENY)
		return
	end
	
	-- 如果是切换到和平模式, 判断CD
	if mode == PEACE_MODE then
		local curr = os.time()
		local cd = self:GetPeaceModeCD()
		if curr < cd then
			self:CallOptResult(OPERTE_TYPE_CHANGE_BATTLE_MODE_LOSE, BATTLE_MODE_OPERTE_PEACE_MODE_IN_CD)
			return
		end
	end
		
	self:SetBattleMode(mode)
end

--进入打坐
function ScenedContext:Hanlde_Dazuo_Start( pkt )
	if not self:IsAlive() then --死亡不允许打坐
		return
	end
	
	-- 是否能够打坐
	local mapid = unitLib.GetMapID(self.ptr)
	if tb_map[mapid].is_sit == 0 then
		outFmtError("current map cannot sit")
		return
	end
	
	--在跨服地图不允许打坐
	local mapid = self:GetMapID()
	if IsKuafuMapID(mapid) then
		return	
	end
	if self:GetDaZuoTime() == 0 then	--不在打坐，则打坐
		self:StartDaZuo()
	else
		self:CancelDaZuo()
	end
end

--开始挂机
function ScenedContext:Hanlde_Start_Hung_Up(pkt)
	if self:IsAlive() then 		
		--记录挂机开始时间
		if not self:GetHungUp() then
			self:SetHungUpStartTime(os.time())
			self:SetHungUp()
			--playerLib.SendToAppdDoSomething(player_ptr, SCENED_APPD_FUXINGJIANGLIN)
		end
	else
		if self:GetHungUp() then
			self:UnSetHungUp() 
		end
	end
end

--停止挂机
function ScenedContext:Hanlde_Stop_Hung_Up(pkt)
	if self:GetHungUp() then
		self:UnSetHungUp()
	end
	
	--记录挂机时长
	if self:GetHungUpStartTime() ~= 0 then
		local times = math.floor((os.time() - self:GetHungUpStartTime())/60)
		if(times > 0)then
			self:AddHungUpTime(times)
			self:AddHungUpDailyTime(times)
		end
		self:SetHungUpStartTime(0)
	end
end

--副本挂机操作
function ScenedContext:Hanlde_Fuben_Hung_Up(packet)	
	self:DoFubenHungUp(packet.ntype, packet.count, packet.param)
end

-- 查询副本信息
function ScenedContext:Hanlde_Query_Fuben_Info(packet)
	local type = packet.type
	local level = packet.level
	if type == 1 then --飞升之路排名前三
		local result = ""
		for i=1,MAX_FSZL_RANK do
			local guid = globalCounter:GetFSZLPlayerGuid(level,i)
			local name = globalCounter:GetFSZLPlayerName(level,i)
			result = result..guid..";"..name..";"
		end
		-- 下发数据给客户端显示		
		self:call_query_fuben_info_result(type, result)		
	elseif type == 2 then --副本前十杀怪数排名
		local result = ""
		for i = 1,MAX_FUBEN_RANK do
			local guid = globalCounter:GetFubenPlayerGuid(i)
			local name = globalCounter:GetFubenPlayerName(i)
			local killnum =  globalCounter:GetFubenPlayerKill(i)
			if guid then
				result = result..guid..";"..name..";"..killnum..";"	
			end
		end
		-- 下发数据给客户端显示		
		self:call_query_fuben_info_result(type, result)
	end
end

-- 开始使用游戏对象
function ScenedContext:Hanlde_Use_Gameobject_Start(packet)	
	self:StartUseGameObject()
end

--接受其他任务
function ScenedContext:Hanlde_Receive_Other_Quest(packet)	
	if(packet.type == QUEST_TYPE_TOKEN)then --江湖密令任务
		self:DoAcceptTokenQuset()
	elseif(packet.type == QUEST_TYPE_XUANSHANG)then --悬赏任务
		self:DoAcceptXuanShangQuset()
	end
end

--完成其他任务
function ScenedContext:Hanlde_Complete_Other_Quest(packet)	
	if(packet.type == QUEST_TYPE_TOKEN)then --江湖密令任务
		self:DoCompleteTokenQuset(packet.quest_id,packet.item_guid,packet.reserve)
	end
end		

-- 全屏秒杀操作
function ScenedContext:Hanlde_Instance_All_Kill_Opt(packet)
	local player_ptr = self.ptr
	local map_ptr = unitLib.GetMap(player_ptr)
	local allCreatures = mapLib.GetAllCreature(map_ptr)
	if(#allCreatures > 0)then
		playerLib.SendToAppdDoSomething(player_ptr, SCENED_APPD_ALL_KILL_SUB_MONEY)
	else
		--下发错误提示
		self:CallOptResult(OPERTE_TYPE_FUBEN, FUBEN_OPRATE_NO_CREATURES)
	end
end

JUMP_SKILL = 10001

--跳跃	
function ScenedContext:Hanlde_Jump_Start(packet)
	local player_ptr = self.ptr
	local dst_x, dst_y = packet.pos_x, packet.pos_y
	if dst_x == 0 or dst_y == 0 then
		outDebug("MSG_JUMP_START  dst_x == 0 or dst_y == 0 !!!")
		return
	end	
	if not self:IsAlive() then
		self:CallOptResult(OPRATE_TYPE_JUMP, JUMP_RESULT_DEAD)
		return
	end
	--验证目标点是不是障碍点
	local map_ptr = unitLib.GetMap(player_ptr)
	if map_ptr == nil then
		outDebug("error : do MSG_JUMP_START but map_ptr = nil ")
		--错误返回
		return
	end
	
	--看看本地图是否允许跳跃
	local mapid = mapLib.GetMapID(map_ptr)
	if tb_map[mapid].is_jump == 0 then
		self:CallOptResult(OPRATE_TYPE_JUMP, JUMP_RESULT_MAP_CANT_JUMP)
		return
	end
	
	--是否有禁止跳跃buff
	if not unitLib.IsCanJump(player_ptr) then
		outDebug("MSG_JUMP_START  player is cannot jump !!!")
		self:CallOptResult(OPRATE_TYPE_JUMP, JUMP_RESULT_CANT_JUMP)
		return
	end
	
	-- 骑乘状态下坐骑, 然后跳跃? 还是不能跳跃
	
	--[[
	--如果是2段跳，判断是否激活了2段跳技能
	if unitLib.HasBuff(player_ptr, BUFF_JUMP_JUMP) then
		if not self:IsActiveSpell(5) then
			outDebug("MSG_JUMP_START  Spell is not Active !!!")
			self:CallOptResult(OPRATE_TYPE_JUMP, JUMP_RESULT_NOT_ACTIVE_SPELL)
			return
		end
	end 
	]]
	
	--验证cd
	local cur_time = os.time()			--获取服务器运行时间
	local cd_time = self:GetPlayerJumpCd()	-- 玩家跳跃技能cd
	if cur_time < cd_time then
		self:CallOptResult(OPRATE_TYPE_JUMP, JUMP_RESULT_SPELL_CD)
		return
	end
	
	--距离验证
	local angle = self:GetAngle(dst_x, dst_y)
	local distance = self:GetDistance(dst_x, dst_y)
	local cas_x, cas_y = unitLib.GetPos(player_ptr)
	if distance > config.jump_max_distance then
		distance = config.jump_max_distance
	end
	
	while(distance > 0)do
		dst_x = cas_x + distance * math.cos(angle)
		dst_y = cas_y + distance * math.sin(angle)
		if mapLib.IsCanRun(map_ptr, dst_x, dst_y) == 1 then
			break
		else
			distance = distance - 0.3
		end
	end
	
	
	if mapLib.IsCanRun(map_ptr, dst_x, dst_y) == 1 then
		
		local cd = math.ceil(tb_skill_base[JUMP_SKILL].singleCD / 1000)
		self:SetPlayerJumpCd(cur_time + cd)
	
		--[[
		if self:isRide() then
			self:MountUnride() 
		end
		--]]
		if tb_map[mapid].type == MAP_TYPE_FIELD then
			Select_Instance_Script(self:GetMapID()):new{ptr = map_ptr}:OnStartJump(self)
		end
		--增加BUFF, 通过BUFF来控制跳跃时间
		SpelladdBuff(player_ptr, BUFF_JUMP_JUMP, player_ptr, BUFF_DEFAULT_EFFECT_ID_JUMP_JUMP,tb_buff_effect[BUFF_DEFAULT_EFFECT_ID_JUMP_JUMP].duration)
		
		--移动
		--local move_path = {}
		--table.insert(move_path,dst_x)
		--table.insert(move_path,dst_y)
		--unitLib.MoveTo(player_ptr, #move_path,table.concat(move_path,","))
		
		-- TODO: 设置目的坐标 并 广播跳跃到目标点
		unitLib.CallJumpStart(player_ptr, dst_x, dst_y)
		
		-- 加无敌buff
		unitLib.AddBuff(player_ptr, BUFF_INVINCIBLE, player_ptr, 1, 1)
	else
		outFmtError("error : MSG_JUMP_START is not can run x = %d  y = %d",dst_x, dst_y)
		self:CallOptResult(OPRATE_TYPE_JUMP, JUMP_RESULT_WRONG_COORD)
	end
end

function ScenedContext:Hanlde_Use_Jump_Point(pkt)
	local id = pkt.id

	-- 判断跳点是否存在
	local config = tb_map_jump_point_detail[id]
	if not config then
		outFmtDebug("Hanlde_Use_Jump_Point not id for %d", id)
		return
	end
	
	-- 判断玩家是否在跳点触发范围内
	if not Script_Gameobject_Pick_Check(self.ptr, config.entry, config.point[ 1 ], config.point[ 2 ]) then
		outFmtDebug("Hanlde_Use_Jump_Point out of pos")
		return
	end
	
	-- 设置目标点
	local destPoint = config.show[#config.show]
	unitLib.SetPos(self.ptr, destPoint[ 1 ], destPoint[ 2 ])
	outFmtDebug("lua set position to (%d, %d)", destPoint[ 1 ], destPoint[ 2 ])
	-- 发送广播
	local pkt = protocols.pack_use_jump_point (id)
	pkt:writeU32(self:GetIntGuid())
	app:Broadcast(self, pkt)
	pkt:delete()
	
	--增加BUFF, 通过BUFF来控制跳跃时间
	SpelladdBuff(self.ptr, BUFF_JUMP_JUMP, self.ptr, 1, config.last)
end

-- /*跳跃结束*/
function ScenedContext:Hanlde_Jump_End(packet)
	local player_ptr = self.ptr
	--如果是生化危机地图
	local map_id = unitLib.GetMapID(player_ptr)
	if map_id == SHWJ_INSTANCE_MAPID then
		local mapInfo = Select_Instance_Script(map_id):new{ptr = map_ptr}
		mapInfo:EndJump(self,packet.pos_x, packet.pos_y)
		return
	end
	
	local jump_lv = unitLib.GetBuffLevel(player_ptr,BUFF_JUMP_JUMP)
	if(jump_lv == 2)then			--若是第二次跳，那必须先移动到终点去
		local x,y = unitLib.GetMoveEndPos(player_ptr)
		if(x ~= 0 and y ~= 0)then	--为0的时候是已经走到了
			unitLib.SetPos(player_ptr, x, y, false)
		end
		--BUFF:跳跃落地后，移动速度提升50%、闪避率提升30%，持续5秒
		SpelladdBuff(player_ptr, BUFF_JUMP_DOWN, player_ptr, 1,tb_buff_template[BUFF_JUMP_DOWN].duration)
	end
	
	--设置cd
	local cur_time = os.time()
	local cd = config.jump_cd
	self:SetPlayerJumpCd(cur_time + cd)
	--移除BUFF
	unitLib.RemoveBuff(player_ptr, BUFF_JUMP_JUMP)
	unitLib.CallJumpEnd(player_ptr, packet.pos_x, packet.pos_y)
end

--坐骑骑乘
--
--S = 1000 / 人物基础数值 + Add 码/秒
--Q = 1000 / (1000 / 人物基础数值 + Add) ms/码
--
function ScenedContext:Handle_Ride(packet)
	local player_ptr = self.ptr
	
	-- 坐骑未激活
	if not self:IsMountActived() then
		outFmtError("mount not be actived")
		return
	end
	
	-- 是否能够骑乘
	local mapid = unitLib.GetMapID(player_ptr)
	if tb_map[mapid].is_ride == 0 then
		outFmtError("current map cannot ride")
		return
	end
	
	local prev = self:rideFlag()
	if prev == 0 then	--上坐骑
		--当前是战斗状态
		local status = playerLib.GetPlayeCurFightStatus(player_ptr)
		if status == COMBAT_STATE_ENTER then
			self:CallOptResult(OPRATE_TYPE_MOUNT_QICHENG, MOUNT_QICHENG_FIGHT)
			return
		end
		
		--当前是跳跃状态
		if unitLib.HasBuff(player_ptr, BUFF_JUMP_JUMP) then
			self:CallOptResult(OPRATE_TYPE_MOUNT_QICHENG, MOUNT_QICHENG_JUMP)
			return
		end
		
		self:MountRide()
	elseif prev > 0 then	--下坐骑
		self:MountUnride()
	end
end



--客户端发起传送
function ScenedContext:Hanlde_Teleport( pkt )
	Script_WorldMap_Teleport(self, pkt.map_id, pkt.pos_x, pkt.pos_y)
end

--打boss传送
function ScenedContext:Hanlde_DaBoss_Teleport( pkt )
	local player_ptr = self.ptr
	local map_ptr = unitLib.GetMap(player_ptr)
	if not map_ptr then return end
	local mapid = unitLib.GetMapID(player_ptr)
	local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
	--玩家不在世界地图,不让传
	if mapInfo:GetInstanceType() ~= 0 then
		outFmtError("CMSG_DABOSS_TELEPORT player %s not in worldmap, curmapid %d!", self:GetPlayerGuid(), mapid)
		return
	end
	--玩家必须还活着
	if not self:IsAlive() then
		outFmtError("CMSG_DABOSS_TELEPORT player %s is not alive!", self:GetPlayerGuid())
		return 
	end
	
	--pvp状态下一律不准进
	if(self:GetPVPState())then
		outFmtError("CMSG_DABOSS_TELEPORT player %s is pvp state!", self:GetPlayerGuid())
		return
	end
	
	local teleport_count = self:GetDaBossTeleportCount()
	if teleport_count >= 50 then
		outFmtError("CMSG_DABOSS_TELEPORT teleport_count is %d", self:GetDaBossTeleportCount())
		return
	end
	teleport_count = teleport_count + 1
	self:SetDaBossTeleportCount(teleport_count)
	playerLib.Teleport(player_ptr, pkt.map_id, pkt.pos_x, pkt.pos_y)
	
end


--BOSS掉落记录查询
function ScenedContext:Handle_Boss_Drop_Record_Query( pkt )
	local event = {}	
	for i = 1, MAX_DROP_RECORD_INFO_COUNT do
		local ret, player_guid, player_name, item_entry, boss_entry, map_id, tm ,boss_level ,map_cs = globalCounter:GetBossDropRecordInfo( i - 1 )
		if ret then
			local e = boss_drop_event_t.new()
			e.player_guid = player_guid
			e.player_name = player_name
			e.item_entry = item_entry
			e.boss_entry = boss_entry
			e.map_id = map_id
			e.time = tm
			e.boss_level = boss_level
			e.map_cs = map_cs
			table.insert( event, e)
		end
	end
	self:call_boss_drop_event_query_result( event )
end

--查询上次击杀BOSS的玩家信息
function ScenedContext:Handle_Query_Kill_Boss_Playername( pkt )
	local entry = pkt.boss_entry
	if entry == 0 then
		outFmtError("Handle_Query_Kill_Boss_Playername:  boss_entry:%d",entry )
		return
	end 
	--查找被杀BOSS模板位置
	local cursor = globalCounter:FindKillBossPosition( entry )
	local boss_entry, player_name
	if cursor ~= -1 then 
		boss_entry, player_name = globalCounter:GetKillBossInfo(cursor)
	else
		boss_entry, player_name = entry, ""		
	end	
	self:call_query_kill_boss_playername(boss_entry, player_name)
end

--请求BUFF处理
function ScenedContext:Hanlde_Ask_For_Buff( pkt )
	if(unitLib.HasBuff(self.ptr, pkt.data))then
		unitLib.RemoveBuff(self.ptr, pkt.data)
		ClearBuffFlags(self.ptr, pkt.data)
	end
end

--进入日常副本
function ScenedContext:Hanlde_Enter_Daily_Instance( pkt )
	local player_ptr = self.ptr
	local map_ptr = unitLib.GetMap(player_ptr)
	if not map_ptr then return end
	local mapid = unitLib.GetMapID(player_ptr)
	local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
	--玩家不在世界地图,不让传
	if mapInfo:GetInstanceType() ~= 0 then
		outFmtError("CMSG_ENTER_DAILY_INSTANCE player %s not in worldmap, curmapid %d!", self:GetPlayerGuid(), mapid)
		return
	end
	--玩家必须还活着
	if not self:IsAlive() then
		outFmtError("CMSG_ENTER_DAILY_INSTANCE player %s is not alive!", self:GetPlayerGuid())
		return 
	end
	--pvp状态下一律不准进
	if self:GetPVPState() then
		outFmtError("CMSG_ENTER_DAILY_INSTANCE player %s is pvp state!", self:GetPlayerGuid())
		return
	end
	--发到应用服其他校验、及扣除材料
	playerLib.SendToAppdDoSomething(player_ptr, SCENED_APPD_ENTER_DAILY_INSTANCE, pkt.map_id)
end

--进入塞外伏击副本
function ScenedContext:Hanlde_Enter_Swfj_Instance( pkt )
	local player_ptr = self.ptr
	local map_ptr = unitLib.GetMap(player_ptr)
	if not map_ptr then return end
	local mapid = unitLib.GetMapID(player_ptr)
	local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
	--玩家不在世界地图,不让传
	if mapInfo:GetInstanceType() ~= 0 then
		outFmtError("CMSG_ENTER_DAILY_INSTANCE player %s not in worldmap, curmapid %d!", self:GetPlayerGuid(), mapid)
		return
	end
	--玩家必须还活着
	if not self:IsAlive() then
		outFmtError("CMSG_ENTER_DAILY_INSTANCE player %s is not alive!", self:GetPlayerGuid())
		return 
	end
	--pvp状态下一律不准进
	if self:GetPVPState() then
		outFmtError("CMSG_ENTER_DAILY_INSTANCE player %s is pvp state!", self:GetPlayerGuid())
		return
	end
	
	--次数判断
	if self:GetSwfjEnterCount() >= tb_swfj[151].num then
		outFmtError("CMSG_ENTER_DAILY_INSTANCE enter count %s is max", self:GetSwfjEnterCount())
		return
	end
	
	local to_mapid = 151
	playerLib.Teleport(player_ptr, to_mapid, tb_map_info[to_mapid].into_point[1], tb_map_info[to_mapid].into_point[2])	
end

--塞外伏击领取经验
function ScenedContext:Hanlde_Get_Swfj_Instance_Reward( pkt )
	local mapid = unitLib.GetMapID(self.ptr)
	if not tb_swfj[mapid] then return end
	local map_ptr = unitLib.GetMap(self.ptr)
	if not map_ptr then return end
	local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
	
	--不在领奖励阶段
	if mapInfo:GetMapState() ~= mapInfo.STATE_GET_EXP then
		return
	end
	--发到应用服其他校验、及扣除材料
	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_SWFJ_INSTANCE_AWARD_SUB_MONEY, pkt.get_type)
end


--进入VIP副本
function ScenedContext:Hanlde_Enter_VIP_Instance( pkt )
	
	local id = pkt.id
	local hard = pkt.hard
	if tb_map_vip[id] == nil then
		return
	end
	
	if hard < 1 or hard > #tb_map_vip[id].forces then
		return
	end
	
	local map_ptr = unitLib.GetMap(self.ptr)
	if not map_ptr then 
		return
	end
	
	local toMapId = tb_map_vip[id].mapid
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		outFmtError("Hanlde_Enter_VIP_Instance player %s is not alive!", self:GetPlayerGuid())
		return 
	end

	-- 该地图是否存在
	if tb_map[toMapId] == nil then
		return
	end
	
	-- 是否允许传送
	if not self:makeEnterTest(toMapId) then
--		outFmtError("Hanlde_Enter_VIP_Instance player %s cannot tele to vip map curmapid %d!", self:GetPlayerGuid(), mapid)
		return
	end
	
	--pvp状态下一律不准进
	if self:GetPVPState() then
		self:CallOptResult(OPRATE_TYPE_TELEPORT, TELEPORT_OPRATE_PVP_STATE)
		return
	end
	
	--发到应用服进行进入判断
	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_ENTER_VIP_INSTANCE, id, ""..hard)
end

-- 进入试炼塔
function ScenedContext:Hanlde_Enter_Trial_Instance(pkt)
	
	local toMapId = tb_instance_trial[1].mapid
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		outFmtError("Hanlde_Enter_Trial_Instance player %s is not alive!", self:GetPlayerGuid())
		return 
	end

	-- 该地图是否存在
	if tb_map[toMapId] == nil then
		return
	end
	
	local mapid = unitLib.GetMapID(self.ptr)
	-- 是否允许传送
	if mapid ~= toMapId and not self:makeEnterTest(toMapId) then
--		outFmtError("Hanlde_Enter_VIP_Instance player %s cannot tele to vip map curmapid %d!", self:GetPlayerGuid(), mapid)
		return
	end
	
	--pvp状态下一律不准进
	if self:GetPVPState() then
		self:CallOptResult(OPRATE_TYPE_TELEPORT, TELEPORT_OPRATE_PVP_STATE)
		return
	end
	
	--发到应用服进行进入判断
	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_ENTER_TRIAL_INSTANCE, 0)
end

-- 进入世界冒险
function ScenedContext:Hanlde_Enter_Risk_Instance(pkt)
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		outFmtError("Hanlde_Enter_Trial_Instance player %s is not alive!", self:GetPlayerGuid())
		return 
	end

	local mapid = unitLib.GetMapID(self.ptr)
	-- 是否允许传送
	local subType = tb_map[mapid].inst_sub_type
	if subType > 0 and subType ~= INSTANCE_SUB_TYPE_RISK then
--		outFmtError("Hanlde_Enter_VIP_Instance player %s cannot tele to vip map curmapid %d!", self:GetPlayerGuid(), mapid)
		return
	end
	
	--pvp状态下一律不准进
	if self:GetPVPState() then
		self:CallOptResult(OPRATE_TYPE_TELEPORT, TELEPORT_OPRATE_PVP_STATE)
		return
	end
	
	-- 系统未激活
	if not self:GetOpenMenuFlag(MODULE_WORLD_RISK,MODULE_WORLD_RISK_MAIN) then
		return
	end

	self:teleportWorldRisk()
end

--进入资源副本
function ScenedContext:Hanlde_Enter_Res_Instance( pkt )
	
	local id = pkt.id
	--outFmtDebug("instance id %d",id)
	if tb_instance_res[id] == nil then
		return
	end
	
	local map_ptr = unitLib.GetMap(self.ptr)
	if not map_ptr then 
		return
	end
	
	local toMapId = tb_instance_res[id].mapid
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		outFmtError("Hanlde_Enter_VIP_Instance player %s is not alive!", self:GetPlayerGuid())
		return 
	end

	-- 该地图是否存在
	if tb_map[toMapId] == nil then
		return
	end
	
	-- 是否允许传送
	if not self:makeEnterTest(toMapId) then
--		outFmtError("Hanlde_Enter_VIP_Instance player %s cannot tele to vip map curmapid %d!", self:GetPlayerGuid(), mapid)
		return
	end
	
	--pvp状态下一律不准进
	if self:GetPVPState() then
		self:CallOptResult(OPRATE_TYPE_TELEPORT, TELEPORT_OPRATE_PVP_STATE)
		return
	end
	
	--发到应用服进行进入判断
	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_ENTER_RES_INSTANCE, id)
end
--end 资源副本-----------


-- 
function ScenedContext:Hanlde_Teleport_Map(pkt)
	local mapid = pkt.mapid
	local lineNo = pkt.lineNo
	
	-- 地图id不对
	if not tb_map[mapid] or tb_map[mapid].inst_type ~= 0 then
		return
	end
	
	-- 该地图还未处理
	if not INSTANCE_SCRIPT_TABLE[mapid] then
		return
	end
	--[[
	-- 分线不对
	if lineNo < 0 or lineNo > MAX_DEFAULT_LINE_COUNT then
		return
	end
	--]]
	local teleInfo = tb_map[mapid].tele
	lineNo = 0
	if #teleInfo >= 2 then
		playerLib.Teleport(self.ptr, mapid, teleInfo[1], teleInfo[2], lineNo)
	else
		outFmtDebug("has no teleportinfo for mapid = %u", mapid)
	end
end


function ScenedContext:Hanlde_Teleport_Field_Boss(pkt)
	local mapid = pkt.mapid
	local lineNo = pkt.lineNo
	
	-- 地图id不对
	if not tb_map[mapid] or tb_map[mapid].type ~= MAP_TYPE_FIELD then
		return
	end
	
	-- 该地图还未处理
	if not INSTANCE_SCRIPT_TABLE[mapid] then
		return
	end
	
	-- 分线不对
	if lineNo < 1 or lineNo > MAX_DEFAULT_LINE_COUNT then
		return
	end
	
	local pos = tb_field_boss[mapid].bossPosi
	local x = randInt(-5, 5) + pos[ 1 ]
	local y = randInt(-5, 5) + pos[ 2 ]
	playerLib.Teleport(self.ptr, mapid, x, y, lineNo)
end

function ScenedContext:Handle_ForceInto(pkt)
	DoForceInto(self)
end

-- 回到主城
function ScenedContext:Hanlde_Teleport_Main_City(pkt)
	local mapid = unitLib.GetMapID(self.ptr)
	if mapid == ZHUCHENG_DITU_ID then
		return
	end
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		return 
	end
	
	local map_ptr = unitLib.GetMap(self.ptr)
	mapLib.ExitInstance(map_ptr, self.ptr)
end

-- 回到家族
function ScenedContext:Handle_Back_To_Family(pkt)
	local factionGuid = self:GetFactionId()
	if string.len(factionGuid) == 0 then
		return
	end
	-- 玩家必须还活着
	if not self:IsAlive() then
		return 
	end
	
	local x = randInt(-2, 2)
	local y = randInt(-2, 2)
	playerLib.Teleport(self.ptr, FACTION_MAP_ID, x+FACTION_FUHUO_X, y+FACTION_FUHUO_Y, 0, factionGuid)
end

function ScenedContext:Handle_Challange_Boss(pkt)
	local mapid = unitLib.GetMapID(self.ptr)
	local passedSectionId = self:getLastPassedSectionId()
	local sectionId = onGetAvailableSectionId(passedSectionId)
	
	--[[
	-- 只有在小怪关卡才能挑战boss
	if mapid ~= tb_risk_data[sectionId].mapid then
		return
	end
	--]]
		
	-- 判断是否能挑战boss
	if not self:isCanChallengeRiskBoss() then
		return
	end
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		return 
	end
	
	local bossSectionId = tb_risk_data[sectionId].relateId
	-- 判断等级时候够
	if self:GetLevel() < tb_risk_data[bossSectionId].level then
		return
	end
	
	if bossSectionId > 0 then
		local config = tb_risk_data[bossSectionId]
		local bossMapid = config.mapid
		local x		= config.born[ 1 ]
		local y		= config.born[ 2 ]
		local generalId = string.format("%s#%d", self:GetPlayerGuid(), bossSectionId)
		playerLib.Teleport(self.ptr, bossMapid, x, y, 0, generalId)
	end
end

-- 使用需要广播的
function ScenedContext:Handle_Use_Broadcast_gameobject(pkt)
	local target = pkt.target
	
	local map_ptr = unitLib.GetMap(self.ptr)
	local gameobject = mapLib.GetGameObjectByGuid(map_ptr, target)
	if not gameobject then return end
	
	-- 判断距离是否足够
	local posx, posy = unitLib.GetPos(gameobject)
	local distance = self:GetDistance(posx, posy)
	if distance > config.field_boss_pick_max_distance then
		self:CallOptResult(OPERTE_TYPE_FIELD_BOSS, FIELD_BOSS_OPERTE_TOO_FAR)
		return
	end
	
	-- 至少是需要读进度条
	local gameObjectInfo = UnitInfo:new {ptr = gameobject}
	if not gameObjectInfo:NeedUseMode() then
		return
	end
	
	-- 特殊处理的采集物
	local entry = gameObjectInfo:GetEntry()	
	if tb_gameobject_template[entry].judge < 1 then
		return
	end
	
	local mapid = unitLib.GetMapID(self.ptr)
	local mapInfo = Select_Instance_Script(mapid):new {ptr = map_ptr}
	mapInfo:OnUseBroadCastGameObject(self, gameObjectInfo)
end

-- 世界BOSS挑战
function ScenedContext:Handle_World_Boss_Fight(pkt)
	-- 不是BOSS挑战阶段不能进
	if not globalValue:IsWorldBossBorn() then
		outFmtDebug("not in IsWorldBossBorn")
		return
	end
	
	local id = globalValue:GetWorldBossTimes()
	-- 没进行过报名的不能进
	if id ~= self:GetLastJoinID() then
		outFmtDebug("current joinid = %d, but curr = %d", id, self:GetLastJoinID())
		return
	end
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		return 
	end
	
	local indx = globalValue:GetTodayWorldBossID()
	local config = tb_worldboss_base[indx]
	local toMapId = config.mapid
	local toX = randInt(config.rect[ 1 ], config.rect[ 3 ])
	local toY = randInt(config.rect[ 2 ], config.rect[ 4 ])
	local line = self:GetLastLine()
	-- 不符合线路的不能进
	local rooms = globalValue:GetTodayWorldBossRoom()
	if line > 0 and line <= rooms and not globalValue:IsWorldBossEndInLine(line) then
		playerLib.Teleport(self.ptr, toMapId, toX, toY, line, "")
	end
end


-- 和NPC对话
function ScenedContext:Handle_Talk_With_Npc(pkt)
	local entry = pkt.entry
	local questId = pkt.questId
	
	if not tb_creature_template[entry] then
		return
	end
	
	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_TALK, entry, ""..questId)
end


function ScenedContext:Handle_Gold_Respawn(pkt)
	local mapid = unitLib.GetMapID(self.ptr)
	local map_ptr = unitLib.GetMap(self.ptr)
	local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
	
	mapInfo:OnCheckIfCanCostRespawn(self.ptr)
end

function ScenedContext:Handle_random_Respawn(pkt)
	local mapid = unitLib.GetMapID(self.ptr)
	local map_ptr = unitLib.GetMap(self.ptr)
	local mapInfo = Select_Instance_Script(mapid):new{ptr = map_ptr}
	
	mapInfo:OnCheckIfCanRandomRespawn(self)
end

--[[-- 换线
function ScenedContext:Hanlde_Change_Line(pkt)
	local lineNo = pkt.lineNo
	local mapid	 = unitLib.GetMapID(self.ptr)
	
	if not DoIsMapLineCreated(mapid, lineNo) then
		outFmtDebug("cannot change line mapid = %d, lineNo = %d", mapid, lineNo)
		return
	end
	
	local toX, toY = unitLib.GetPos(self.ptr)
	playerLib.Teleport(self.ptr, mapid, toX, toY, lineNo)
end--]]

function ScenedContext:Handle_Roll_WorldBoss_Treasure(pkt)
	
end

function ScenedContext:Handle_Pick_Offline_Reward(pkt)
	--[[
	local passedSectionId = self:GetPlayerUInt32(PLAYER_INT_FIELD_TRIAL_FINISHED_SECTIONID)
	local sectionId = onGetAvailableSectionId(passedSectionId)
	
	-- 已经领取的就不领了
	if self:GetPlayerUInt32(PLAYER_INT_FILED_LEAVE_RISK_PICKED) == 1 then
		return
	end
	
	local mapid = unitLib.GetMapID(self.ptr)
	-- 不在冒险世界的就不可以领取
	if not isRiskMap(mapid) then
		return
	end
	
	self:SetPlayerUInt32(PLAYER_INT_FILED_LEAVE_RISK_PICKED, 1)
	
	local limit = 1440
	local last = self:GetPlayerUInt32(PLAYER_INT_FILED_LEAVE_RISK_TIME)
	
	self:SetPlayerUInt32(PLAYER_INT_FILED_LEAVE_RISK_TIME, os.time())
	
	local diff = math.floor((os.time() - last) / 60)
	if diff > limit then diff = limit end
	
	local config = tb_risk_data[sectionId]
	local dict = {}
	
	-- 随机装备
	local suitCount = math.floor(diff / config.suitCount)
	local dropid = config.dropid

	for i = 1, suitCount do
		DoRandomDrop(dropid, dict)
	end
	
	PlayerAddRewards(self.ptr, dict, MONEY_CHANGE_OFFLINE, LOG_ITEM_OPER_TYPE_OFFLINE)
	
	local list = Change_To_Item_Reward_Info(dict)
	self:call_offline_reward_result (0, list)
	--]]
end

function ScenedContext:Handle_Try_Mass_Boss (pkt)
	local id = pkt.id
	
	if not tb_mass_boss_info[ id ] then
		return
	end
	
	local map_ptr = unitLib.GetMap(self.ptr)
	if not map_ptr then 
		return
	end
	
	-- boss未刷新
	if not globalValue:isMassBossAlive(id) then
		return
	end
	
	local toMapId = tb_mass_boss_info[ id ].mapId
	
	-- 玩家必须还活着
	if not self:IsAlive() then
		outFmtError("Handle_Try_Mass_Boss player %s is not alive!", self:GetPlayerGuid())
		return 
	end

	-- 该地图是否存在
	if not tb_map[toMapId] then
		return
	end
	
	-- 人数是否超过上限
	local currentCount = InstanceMassBoss.enterCount[ id ]
	if currentCount > tb_mass_boss_info[ id ].permitCount then
		return
	end
	
	-- 是否允许传送
	if not self:makeEnterTest(toMapId) then
		return
	end
	
	--pvp状态下一律不准进
	if self:GetPVPState() then
		self:CallOptResult(OPRATE_TYPE_TELEPORT, TELEPORT_OPRATE_PVP_STATE)
		return
	end
	
	--发到应用服进行进入判断
	playerLib.SendToAppdDoSomething(self.ptr, SCENED_APPD_ENTER_MASS_BOSS_INSTANCE, id)
end

function ScenedContext:Handle_Query_Mass_Boss_Info(pkt)
	local id = pkt.id
	if not tb_mass_boss_info[ id ] then
		return
	end
	
	local rankList = InstanceMassBoss.globalRankList[ id ]
	local count = 0
	local rate = 100
	if rankList then
		count = #rankList
	end
	
	if InstanceMassBoss.bossHpRate[ id ] then
		rate = InstanceMassBoss.bossHpRate[ id ]
	end
	
	self:call_mass_boss_info_ret (count, rate)
end

function ScenedContext:Handle_Query_Mass_Boss_Rank(pkt)
	local id = pkt.id
	if not tb_mass_boss_info[ id ] then
		return
	end
	
	local rankList = InstanceMassBoss.globalRankList[ id ]
	local info = {}
	if rankList then
		local maxHP = InstanceMassBoss.bossMaxHp[id]
		local len = math.min(10, #rankList)
		for i = 1, len do
			local datainfo = rankList[ i ]
			local rank_info = mass_boss_rank_info_t .new()
			rank_info.name = datainfo[ 1 ]
			rank_info.dam = datainfo[ 2 ] * 100 / maxHP
			table.insert(info, rank_info)
		end
	end
	
	self:call_mass_boss_rank_result (info)
end

local OpcodeHandlerFuncTable = require 'scened.context.scened_context_handler_map'

--网络包处理方法
packet.register_on_external_packet(function ( player_ptr, pkt )
	local _player = UnitInfo:new{ptr = player_ptr}	
	local optcode = packet.optcode(pkt)
	local succeed, args = protocols.unpack_packet(optcode, pkt)
	
	--解包失败记一下日志
	if not succeed then
		logLib.WriteAttackPacker(_player:GetGuid(), optcode, ACCACK_PACKET_TYPE_UNPACK, '')
	else
		args.__optcode = optcode		
		if OpcodeHandlerFuncTable[optcode] then
			doxpcall(OpcodeHandlerFuncTable[optcode], _player, args)
		end
	end
end)
