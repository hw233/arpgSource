InstanceWorldBossP = class("InstanceWorldBossP", Instance_base)

InstanceWorldBossP.Name = "InstanceWorldBossP"


function InstanceWorldBossP:ctor(  )
	
end

--玩家加入地图
function InstanceWorldBossP:OnJoinPlayer(player)
	InstanceInstBase.OnJoinPlayer(self, player)
	local playerInfo = UnitInfo:new{ptr = player}
	if not playerInfo:IsAlive() or os.time() >= self:GetMapQuestEndTime() then
		--死亡了还进来，直接弹出去
		mapLib.ExitInstance(self.ptr, player)
		return
	end
end

--当玩家离开时触发
function InstanceWorldBossP:OnLeavePlayer( player, is_offline)
	
end

--初始化脚本函数
function InstanceWorldBossP:OnInitScript(  )
	-- 不让重复初始化
	if self:isInstanceInit() then
		return
	end
	self:SetInstanceInited()
	
	Instance_base.OnInitScript(self) --调用基类
	
	local endTime = os.time() + globalValue:GetEnrollCountdown()
	
	self:SetMapQuestEndTime(endTime)
	-- 设置结束时间
	self:SetMapEndTime(endTime + 10)
end

return InstanceWorldBossP