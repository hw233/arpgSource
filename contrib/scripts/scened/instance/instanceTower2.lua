InstanceTower2 = class('InstanceTower2', Instance_base)

InstanceTower2.DURATION_TIME = 1800		--副本持续时间
InstanceTower2.Name = "InstanceTower2"
InstanceTower2.FloorIndex = 1

function InstanceTower2:ctor(  )
end

--初始化脚本函数
function InstanceTower2:OnInitScript(  )
	Instance_base.OnInitScript(self)					--调用基类
	self:SetMapEndTime(os.time() + self.DURATION_TIME)	--30分钟后回收
end

--当副本状态发生变化时间触发
function InstanceTower2:OnSetState(fromstate,tostate)
	if tostate == self.STATE_FINISH or tostate == self.STATE_FAIL then
		--10s后结束副本
		self:SetMapEndTime(os.time() + 5)
	end
end

--获得当前击杀数
function InstanceTower2:GetKillCount()
	return self:GetUInt32(TAGUAI_INT_FIELD_FIRST_FLOOR+self.FloorIndex)
end

--设置当前击杀数
function InstanceTower2:SetKillCount(val)
	return self:SetUInt32(TAGUAI_INT_FIELD_FIRST_FLOOR+self.FloorIndex, val)
end

--刷怪
function InstanceTower2:OnRefreshMonster()
	-- 判断副本状态是否正常
	local mapState = self:GetMapState()
	if mapState == self.STATE_FINISH or mapState == self.STATE_FAIL then
		return
	end

	-- 刷新怪物
	local tx = 7
	local ty = 12
	local entry = 6
	local config = tb_creature_template[entry]

	for i = 8, 10 do
		for j = 15, 18 do
			local bornX = i
			local bornY = j
			local creature = mapLib.AddCreature(self.ptr, 
							{templateid = entry, x = bornX, y = bornY, active_grid = true, alias_name = config.name, ainame = config.ainame, npcflag = {}})
		
		end
	end
end

--玩家加入地图
function InstanceTower2:OnJoinPlayer(player)
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


AI_taguai2 = class("AI_taguai2", AI_Base)
AI_taguai2.ainame = "AI_taguai2"
--死亡
function AI_taguai2:JustDied( map_ptr,owner,killer_ptr )
	AI_Base.JustDied(self,map_ptr,owner,killer_ptr)

	local instanceInfo = InstanceTower2:new{ptr = map_ptr}
	local prev = instanceInfo:GetKillCount()
	instanceInfo:SetKillCount(prev + 1)

	local number = #mapLib.GetAllCreature(map_ptr)
	if number == 0 then
		local prev = instanceInfo:GetUInt32(TAGUAI_INT_FIELD_FIRST_FLOOR)
		local curr = instanceInfo:GetUInt32(TAGUAI_INT_FIELD_FIRST_FLOOR+1)
		print("floor 1 killed = "..prev.." floor 2 killed = "..curr)
	end

	return 0
end

return InstanceTower2