-- 扫荡vip副本
function PlayerInfo:Handle_Sweep_Vip_Instance(pkt)
	local id = pkt.id
	
	if tb_map_vip[id] == nil then
		return
	end
	
	self:sweepVIP(id)
end


-- 扫荡试炼塔
function PlayerInfo:Handle_Sweep_Trial(pkt)
	self:sweepTrial()
end

-- 重置试炼塔
function PlayerInfo:Handle_Reset_Trial(pkt)
	self:resetTrial()
end

-- 扫荡资源副本
function PlayerInfo:Handle_Sweep_Res(pkt)
	local id = pkt.id
	if tb_instance_res[id] == nil then
		return
	end
	self:sweepResInstance(id)
end


-- 世界BOSS报名
function PlayerInfo:Handle_World_Boss_Enroll(pkt)
	-- 模块没开 不让进
	if not self:GetOpenMenuFlag(MODULE_BOSS, MODULE_BOSS_WORLD_BOSS) then
		return
	end
	onEnrole(self)
end


function PlayerInfo:Handle_Buy_Mass_Boss_Times(pkt)
	local cnt = pkt.cnt
	if cnt <= 0 then
		return
	end
	
	-- 购买次数不能超过上限
	local buyed = self:GetMassBossBuyedTimes()
	if buyed + cnt > #tb_mass_boss_times then
		return
	end
	
	-- 次数满了不能购买
	local curr = self:getMassBossTimes()
	if curr + cnt > tb_mass_boss_base[ 1 ].dailytimes then
		return
	end
		
	local cost = {}
	for i = 1, cnt do
		local config = tb_mass_boss_times[buyed+i]
		for _, rinfo in pairs(config.cost) do
			AddTempInfoIfExist(cost, rinfo[ 1 ], rinfo[ 2 ])
		end
	end
	
	if not self:costMoneys(MONEY_CHANGE_MASS_BOSS_BUY_TIMES, cost, 1) then
		return
	end
	
	self:AddMassBossBuyedTimes(cnt)
	self:AddMassBossTimes(cnt)
end

function PlayerInfo:Handle_Buy_Group_Instance_Times(pkt)
	local count = pkt.count
	if count < 1 then
		return
	end
	
	self:OnBuyGroupInstanceTicket(count)
end

function PlayerInfo:Handle_Get_Risk_Reward(pkt)
	globalCounter:getRiskRank(self)
end