InstanceTower1 = class('InstanceTower1', Instance_base)

InstanceTower1.DURATION_TIME = 1800		--副本持续时间
InstanceTower1.Name = "InstanceTower1"
InstanceTower1.FloorIndex = 0

function InstanceTower1:ctor(  )
end

--初始化脚本函数
function InstanceTower1:OnInitScript( )
	Instance_base.OnInitScript(self)					--调用基类
	self:SetMapEndTime(os.time() + self.DURATION_TIME)	--30分钟后回收
end

--当副本状态发生变化时间触发
function InstanceTower1:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		--10s后结束副本
		self:SetMapEndTime(os.time() + 5)
	end
end

--获得当前击杀数
function InstanceTower1:GetKillCount()
	return self:GetUInt32(TAGUAI_INT_FIELD_FIRST_FLOOR+self.FloorIndex)
end

--设置当前击杀数
function InstanceTower1:SetKillCount(val)
	return self:SetUInt32(TAGUAI_INT_FIELD_FIRST_FLOOR+self.FloorIndex, val)
end

--刷怪
function InstanceTower1:OnRefreshMonster()
	-- 判断副本状态是否正常
	local mapState = self:GetMapState()
	if mapState == self.STATE_FINISH or mapState == self.STATE_FAIL then
		return
	end

	-- 刷新怪物
	local tx = 55
	local ty = 17
	local entry = 5
	local config = tb_creature_template[entry]

	for i = 53, 54 do
		for j = 15, 18 do
			local bornX = i
			local bornY = j
			local creature = mapLib.AddCreature(self.ptr, 
							{templateid = entry, x = bornX, y = bornY, active_grid = true, alias_name = config.name, ainame = config.ainame, npcflag = {}})
		
		end
	end
end


--玩家加入地图
function InstanceTower1:OnJoinPlayer(player)
	Instance_base.OnJoinPlayer(self, player)
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() then
		--死亡了还进来，直接弹出去
		unitLib.Respawn(player, RESURRECTION_SPAWNPOINT, 100)
		mapLib.ExitInstance(self.ptr, player)
		self:SetMapState(self.STATE_FAIL)
	end

	self:SetKillCount(0)
	self:OnRefreshMonster()
end


AI_taguai1 = class("AI_taguai1", AI_Base)
AI_taguai1.ainame = "AI_taguai1"
--死亡
function AI_taguai1:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)

	local instanceInfo = InstanceTower1:new{ptr = map_ptr}
	local prev = instanceInfo:GetKillCount()
	instanceInfo:SetKillCount(prev + 1)

	local number = #mapLib.GetAllCreature(map_ptr)
	if number == 0 then
		playerLib.Teleport(killer_ptr, 102, 6, 10, 0, "")
	end

	return 0
end


return InstanceTower1