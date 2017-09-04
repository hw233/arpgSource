
local ActionBase = require('robotd.action.action')
local ActionScenedKuafu = class('ActionScenedKuafu', ActionBase)

--获取类型
function ActionScenedKuafu:GetType()
	return ROBOT_ACTION_TYPE_SCENED
end

--初始化变量
function ActionScenedKuafu:Initialize(...)
	self:AddTimer("ActionScenedKuafu_update", self.Update, 5000, self)
	self.player.Kuafu_Status = nil
	self.Kuafu_3v3_Status = 0
	self.is_goto = false
	self.enemy_list = {}
	self.last_update_enemy_time = 0
	
	local skillIdInfo, skillLevelInfo = self.player:GetSkillInfo()
	self.normalAttackInfo = skillIdInfo[ 1 ]
	self.skillLevel = skillLevelInfo[ 1 ]
end

--获取类型名
function ActionScenedKuafu:GetName()
	return 'ActionScenedKuafu'
end

--心跳
ACTION_SCENE_KUAfU_STATUS_START = -1;
ACTION_SCENE_KUAfU_STATUS_GUAJI = 1;
ACTION_SCENE_KUAfU_STATUS_ISDIE = 2;


local ACTION_SCENE_KUAfU_3v3_STATUS_NONE			= 0	--初始化状态
local ACTION_SCENE_KUAfU_3v3_STATUS_GOTO			= 1	--寻路到初始挂机点
local ACTION_SCENE_KUAfU_3v3_STATUS_FIND_CREATE		= 2	--寻找生物
local ACTION_SCENE_KUAfU_3v3_STATUS_ATTACK_CREATE	= 3 --攻击生物

function ActionScenedKuafu:Update(diff)
	if(self.player == nil or self.player.my_unit == nil or self.player.mapInfo == nil)then
		return true
	end
	if(self.player.Kuafu_Status == nil) then
		self.player.Kuafu_Status = ACTION_SCENE_KUAfU_STATUS_START
		outFmtDebug("in xianfu init")
	end
	
	-- 3v3 PK
	if self.player:GetMapID() == 3002 then
		if self.is_goto then
			return true
		end
			
		if self.last_update_enemy_time == 0 or os.time() - self.last_update_enemy_time >= 1 then
			self.last_update_enemy_time = os.time()
		
			self.enemy_list = self.player:Find3v3Enemy()
			
		end
		if #self.enemy_list == 0 then
			return true
		end
		local targetUnit = self.enemy_list[1]
		if(self.enemy_list[2] ~= nil and  self.player.my_unit:GetDistance(self.enemy_list[2]) < self.player.my_unit:GetDistance(targetUnit))then
			targetUnit = self.enemy_list[2]
		end
		if(self.enemy_list[3] ~= nil and  self.player.my_unit:GetDistance(self.enemy_list[3]) < self.player.my_unit:GetDistance(targetUnit))then
			targetUnit = self.enemy_list[3]
		end

		local to_x,to_y = targetUnit:GetPos()
		--local to_x,to_y =19+ randInt(-2,2), 63+ randInt(-2,2)
		outFmtDebug(' x:%d y:%d',to_x,to_y)
		
		local caster = self.player.my_unit:GetUIntGuid()
		local target = targetUnit:GetUIntGuid()
		
		local skillId = self.normalAttackInfo[ 1 ]
		local skillLevelIndx = tb_skill_base[skillId].uplevel_id[ 1 ] + self.skillLevel - 1
		local range = tb_skill_uplevel[skillLevelIndx].distance
		--已经到攻击范围内，攻击
		--if(self.player.my_unit:GetDistance(targetUnit) <= range)then
		if(self.player.my_unit:GetDistanceByPos(to_x,to_y) <= range)then
			local p = randInt(0,100)
			if p > 20 then
				self.is_goto = false
				self.Kuafu_3v3_Status = ACTION_SCENE_KUAfU_3v3_STATUS_NONE
				
				local skillId = self.normalAttackInfo[ 1 ]
				table.remove(self.normalAttackInfo, 1)
				table.insert(self.normalAttackInfo, skillId)
				self.player:CastSpell(skillId, to_x, to_y, caster, target)
				self:SetWaitTimeInterval(900)
				return true
			end
			to_x = to_x+ randInt(-2,2)
			to_y = to_y+ randInt(-2,2)
			
		elseif (self.player.my_unit:GetDistanceByPos(to_x,to_y) > 50)then
			to_x = 19+ randInt(-4,4)
			to_y = 63+ randInt(-4,4)
		end
		if(self.Kuafu_3v3_Status <= ACTION_SCENE_KUAfU_3v3_STATUS_GOTO)then

			self.is_goto = true
			local closeCallback = function ()
				self.is_goto = false
				self.Kuafu_3v3_Status = ACTION_SCENE_KUAfU_3v3_STATUS_GOTO
			end
			self:PushAction('robotd.action.scened.action_scened_pathfinding', self.player:GetMapID(), to_x, to_y, closeCallback)
			--self:SetWaitTimeInterval(800)
			return true
		end
		return true
	end
	
	-- 如果在仙府夺宝
	if self.player:GetMapID() == 3003 then
		local info = self.player:PickTreasure()
		if info then
			self:PushAction('robotd.action.scened.action_scened_dosomething', 3003, info[ 1 ], info[ 2 ], function()
				self.player:call_loot_select(info[ 1 ], info[ 2 ])
				outFmtInfo("%s call_loot_select at (%d, %d)", self.player:GetGuid(), info[ 1 ], info[ 2 ])
			end)
			return true
		end
		
		local params = self.player:FindBossFirst()
		if params then
			self:PushAction('robotd.action.scened.action_scened_quest_killmonster', params[ 1 ], params[ 2 ], params[ 3 ], params[ 4 ],  params[ 5 ], params[ 6 ])
			self:SetWaitTimeInterval(1000)
		end
		return true
	end
	
	--[[
	if self.player:GetMapID() == KUAFU_MILLION_WAR_MAPID then
		if self.player.Kuafu_Status == ACTION_SCENE_KUAfU_STATUS_START then
			local t = os.time()
			if t > self.player.mapInfo:GetMillion_War_Ready_End_Time() then
				self.player.Kuafu_Status = ACTION_SCENE_KUAfU_STATUS_GUAJI;
				self:PushAction('robotd.action.scened.action_scened_zhuiji',TYPE_PEPLE)
				outFmtDebug("in million_war player guaji start")
			end
		end
		
		if self.player.Kuafu_Status == ACTION_SCENE_KUAfU_STATUS_GUAJI then
			if self.player:GetMillionWarLifeNum() == 0 or self.player.mapInfo:GetMillionWinGuid() ~= '' then
				self.player.Kuafu_Status = ACTION_SCENE_KUAfU_STATUS_ISDIE;
				outFmtDebug("in million_war player is Die goback")
				outFmtDebug("------------------------%s",self.player.mapInfo:GetMillionWinGuid())
				
				self.player:call_instance_exit()
			--买一次命
			--self.player:call_million_war_buy(0,0)
				return true
			end
		end
		if self.player.Kuafu_Status == ACTION_SCENE_KUAfU_STATUS_ISDIE then
			outFmtDebug("in million_war player is Die")
		end
	end
	
	if self.player:GetMapID() == 204001 then
		if self.player.Kuafu_Status == ACTION_SCENE_KUAfU_STATUS_START then
			self.player.Kuafu_Status = ACTION_SCENE_KUAfU_STATUS_GUAJI;
			--creatures_1={{110102,83,57},{110103,53,100},{110104,80,167},{110105,122,179},{110106,162,119},{110107,123,66},{110101,107,112}
			self:PushAction('robotd.action.scened.action_scened_zhuiji',TYPE_PEPLE)		
		end
		
		if self.player.Kuafu_Status == ACTION_SCENE_KUAfU_STATUS_GUAJI then
			if self.player.my_unit:IsDie() or self.player.mapInfo:GetBossCount() == 0 then
				self.player.Kuafu_Status = ACTION_SCENE_KUAfU_STATUS_ISDIE;
				outFmtDebug("in huangjin_war player is Die goback")
				self.player:call_instance_exit()
				return true
			end
		end
		if self.player.Kuafu_Status == ACTION_SCENE_KUAfU_STATUS_ISDIE then
			outFmtDebug("in huangjin_war player is Die")
		end
	end
	--]]
	
	return true
end

return ActionScenedKuafu
