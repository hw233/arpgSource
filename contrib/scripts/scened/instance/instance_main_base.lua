InstanceMainBase = class("InstanceMainBase", Instance_base)

InstanceMainBase.Name = "InstanceMainBase"
InstanceMainBase.player_auto_respan = 1

function InstanceMainBase:ctor(  )
	
end

--初始化脚本函数
function InstanceMainBase:OnInitScript(  )
	Instance_base.OnInitScript(self) --调用基类
end

--玩家加入地图
function InstanceMainBase:OnJoinPlayer(player)
	Instance_base.OnJoinPlayer(self, player)
	local playerInfo = UnitInfo:new{ptr = player}
	-- 进城修改模式
	playerInfo:ChangeToPeaceModeAfterTeleport()
end