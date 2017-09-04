
local ActionBase = require('robotd.action.action')
local ActionScenedQuestKillMonster = class('ActionScenedQuestKillMonster', ActionBase)

local STATUS_NONE			= 0	--初始化状态
local STATUS_GOTO			= 1	--寻路到初始挂机点
local STATUS_FIND_CREATE	= 2	--寻找生物
local STATUS_ATTACK_CREATE	= 3 --攻击生物

--获取类型
function ActionScenedQuestKillMonster:GetType()
	return ROBOT_ACTION_TYPE_SCENED
end

--初始化变量
function ActionScenedQuestKillMonster:Initialize(object_id, mapid, x, y, IsFinishMethod, callback)
	self.object_id = object_id
	self.mapid = mapid
	self.to_x = x
	self.to_y = y
	self.IsFinishMethod = IsFinishMethod
	self.status = STATUS_NONE
	self.callback = callback
	
	self.is_goto = false
	
	local skillIdInfo, skillLevelInfo = self.player:GetSkillInfo()
	self.normalAttackInfo = skillIdInfo[ 1 ]
	self.skillLevel = skillLevelInfo[ 1 ]
end

--获取类型名
function ActionScenedQuestKillMonster:GetName()
	return 'ActionScenedQuestKillMonster'
end

--获取当前技能
function ActionScenedQuestKillMonster:GetCurrentSkillId()
	return self.normalAttackInfo[ 1 ]
end

function ActionScenedQuestKillMonster:SkillMoveToTail()
	local skillId = self.normalAttackInfo[ 1 ]
	table.remove(self.normalAttackInfo, 1)
	table.insert(self.normalAttackInfo, skillId)
end

--寻路到挂机点
function ActionScenedQuestKillMonster:GoTo()
	--先寻路
	if(self.status == STATUS_NONE)then
		local closeCallback = function ()
			self.is_goto = false
			self.status = STATUS_GOTO
		end
		self:PushAction('robotd.action.scened.action_scened_pathfinding', self.mapid, self.to_x, self.to_y, closeCallback)
		
		return true
	end
	
	local mapid = self.player:GetMapID()
	local x, y = self.player:GetPos()
	--寻路没成功，使用失败
	if(mapid ~= self.mapid or self.player.my_unit:GetDistanceByPos(self.to_x,self.to_y)>4)then
		outFmtDebug("ActionScenedQuestKillMonster:Update %s goto destination fail,from: %s %s %s", self:ToString(), mapid, x, y)
		return false
	else
		self.status = STATUS_FIND_CREATE
		return true
	end
end

--攻击生物
function ActionScenedQuestKillMonster:AttectCreate(spellId, attack_create)
	local x, y = attack_create:GetPos()
	local caster = self.player.my_unit:GetUIntGuid()
	local target = attack_create:GetUIntGuid()
	self.player:CastSpell(spellId, x, y, caster, target)
end

local oldtime = 0

--心跳
function ActionScenedQuestKillMonster:Update(diff)
	--杀怪数够了
	if self.IsFinishMethod() then
		if self.callback then
			self.callback()
		end
		return false, 1
	end
	
	--寻路到挂机点
	if(self.status <= STATUS_GOTO)then
		-- 在寻路中就不需要寻路了
		if self.is_goto then
			return true
		end
		self.is_goto = true
	
		return self:GoTo(), 4
	end
	
	--到这里了，那就校验一下地图吧
	local mapid = self.player:GetMapID()
	--不再目标地图了，一切都难以继续了，本次挂机算了吧
	if(mapid ~= self.mapid)then
		return false, 2
	end
	
	--先找一下打哪只怪
	if(self.status == STATUS_FIND_CREATE)then
		--object_id
		local unit = self.player:FindUnitByTemplateID(self.object_id)
		if(unit == nil)then
			outFmtDebug("there is no unit for entry %d wait 1 second", self.object_id)
			self:SetWaitTimeInterval(1000)
			return true
		end
		self.attack_create_guid = unit:GetGuid()
		self.status = STATUS_ATTACK_CREATE
		return true
	end
	
	--根据保存的guid搜索精灵对象，如果找不到或者精灵已死，则各种处理
	local attack_create = self.player:FindUnitByGuid(self.attack_create_guid)
	if(attack_create == nil or attack_create:IsDie())then
		self.attack_create_guid = ""
		self.status = STATUS_FIND_CREATE
		return true
	end
	
	-- 进行简单的普通攻击

	local skillId = self:GetCurrentSkillId()
	local skillLevelIndx = tb_skill_base[skillId].uplevel_id[ 1 ] + self.skillLevel - 1
	local range = tb_skill_uplevel[skillLevelIndx].distance
	--已经到攻击范围内，攻击
	if(self.player.my_unit:GetDistance(attack_create) <= range)then
		self:SkillMoveToTail()
		self:AttectCreate(skillId, attack_create)
		self:SetWaitTimeInterval(900)
		return true
	end
	
	--移动中就先不调节位置了
	if(self.player.my_unit:IsMoving())then
		return true
	end
	
	--塞个移动栈？
	--算了，直接这里移动吧
	local x, y = attack_create:GetPos()

	--判断如果是障碍点的话，随机10次
	local is_canRun = mapLib.IsCanRun(attack_create:GetMapID(),x,y)
	if(is_canRun == false)then
		outFmtDebug("ActionScenedQuestKillMonster:Update create pos can't run %u %u %u", attack_create:GetMapID(),x,y)
		local pos_tab = mapLib.RandomPos(attack_create:GetMapID(),1,x,y,3)
		is_canRun = mapLib.IsCanRun(attack_create:GetMapID(),pos_tab[1].x,pos_tab[1].y)
		if(is_canRun)then
			x = pos_tab[1].x
			y = pos_tab[1].y
		end
	end

	--仍然是障碍点
	if(is_canRun == false)then
		outFmtInfo("%s  -----Can`t Run %f %u %u %u %u", self:ToString(), self.player.my_unit:GetDistance(attack_create), x, y, self.player:GetPos())
		assert(false)
		return false,4
	end

	local p_x,p_y = self.player:GetPos()
	--如果目标点和玩家同个坐标就不用再跑了
	if(x == math.floor(p_x) and y == math.floor(p_y))then
		return true
	end

	local result = mapLib.PathfindingGoto(self.player.ptr, self.player.my_unit.ptr, x, y)
	if(result == false)then
		outFmtInfo("%s  -----  %f %u %u %u %u", self:ToString(), self.player.my_unit:GetDistance(attack_create), x, y, self.player:GetPos())
		if(result == false)then
			PathfindingGotoFailure(self.player,mapid)
			self:SetWaitTimeInterval(1000)
		end
	end	
	return true
end


return ActionScenedQuestKillMonster
