
local ActionBase = require('robotd.action.action')
local ActionScenedGuaJi = class('ActionScenedGuaJi', ActionBase)

local STATUS_NONE			= 0	--初始化状态
local STATUS_GOTO			= 1	--寻路到初始挂机点
local STATUS_FIND_CREATE	= 2	--寻找生物
local STATUS_ATTACK_CREATE	= 3 --攻击生物

--获取类型
function ActionScenedGuaJi:GetType()
	return ROBOT_ACTION_TYPE_SCENED
end

--初始化变量
function ActionScenedGuaJi:Initialize(object_id, mapid, x, y, count)
	self.object_id = object_id
	self.mapid = mapid
	self.to_x = x
	self.to_y = y
	self.init_count = self.player:GetKillMonster()
	self.count = count
	self.status = STATUS_NONE	
end

--获取类型名
function ActionScenedGuaJi:GetName()
	return 'ActionScenedGuaJi'
end

--寻路到挂机点
function ActionScenedGuaJi:GoTo()	
	--世界BOSS副本
	if(self.mapid ~= self.player:GetMapID() and bossInfo[toint(self.mapid)])then
		self.player:call_enter_dynamic_map(self.mapid,0,self.object_id)
		self:SetWaitTimeInterval(2000)
		return true
	end

	--先寻路
	if(self.status == STATUS_NONE)then
		self:PushAction('robotd.action.scened.action_scened_pathfinding', self.mapid, self.to_x, self.to_y)
		self.status = STATUS_GOTO
		return true
	end
	
	local mapid = self.player:GetMapID()
	local x, y = self.player:GetPos()
	--寻路没成功，使用失败
	if(mapid ~= self.mapid or self.player.my_unit:GetDistanceByPos(self.to_x,self.to_y)>3)then
		outFmtDebug("ActionScenedGuaJi:Update %s use object fail,from: %s %s %s", self:ToString(), mapid, x, y)
		return false
	else
		self.status = STATUS_FIND_CREATE
		return true
	end
end

--攻击生物
function ActionScenedGuaJi:AttectCreate(attack_create)
	local x, y = attack_create:GetPos()
	local caster = self.player.my_unit:GetUIntGuid()
	local target = attack_create:GetUIntGuid()
	self.player:CastSpell(x, y, caster, target)
	--self.player:call_spell_start(1500, x, y, caster, target)
	--self.player:call_spell_start(1600, x, y, caster, target)
	--self.player:call_spell_start(1700, x, y, caster, target)
end

local oldtime = 0

--心跳
function ActionScenedGuaJi:Update(diff)
	--杀怪数够了
	if(self.player:GetKillMonster() >= self.count + self.init_count)then
		return false, 1
	end
	
	--寻路到挂机点
	if(self.status <= STATUS_GOTO)then
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
		local unit, dis = self.player:FindCreate()
		if(unit == nil)then
			return false, 3
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
	



	--到这里，已经到挂机点附近了，并且已经找好要打哪只怪了
	--现在需要的是靠近怪物，攻击。

	--已经到攻击范围内，攻击
	if(self.player.my_unit:GetDistance(attack_create) <= 4)then
		if(oldtime ~= 0 and (os.time() - oldtime) > 600 and self.player:GetLevel()>100)then
		 	--outFmtDebug("action_scened_zhuiji ===================== 0 start %u",(os.time() - oldtime))
		 	
		 	if(randIntD(1,100)<=10)then
		 		--outFmtDebug("action_scened_zhuiji ===================== start")
		 		self:PushAction('robotd.action.scened.action_scened_zhuiji',TYPE_PLAYER)
		 		return true
		 	end
		end
		--print("========================================= zhuiji",oldtime)
		--print("========================================= zhuiji=",os.time())
		if(oldtime == 0)then
			oldtime = os.time()
		end



		local x, y = attack_create:GetPos()
		local j, k = self.player.my_unit:GetPos()
		--outFmtDebug("==================Fight~~~~~~target:%s,%s  self:%s,%s",x,y,j,k)
		--停止移动，攻击
		self:AttectCreate(attack_create)
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
		outFmtDebug("ActionScenedGuaJi:Update create pos can't run %u %u %u", attack_create:GetMapID(),x,y)
		local pos_tab = mapLib.RandomPos(attack_create:GetMapID(),1,x,y,4)
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


return ActionScenedGuaJi
