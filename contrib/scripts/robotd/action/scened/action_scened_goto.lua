local ActionBase = require('robotd.action.action')
local ActionScenedGoto = class('ActionScenedGoto', ActionBase)

--获取类型
function ActionScenedGoto:GetType()
	return ROBOT_ACTION_TYPE_SCENED
end

--初始化变量
function ActionScenedGoto:Initialize(mapid,x, y, callback)
	self.to_mapid = mapid
	self.to_x = x
	self.to_y = y
	self.send_path = false
	self.callback = callback
	self.is_jump = false
	self.last_update_time = 0
end

--获取类型名
function ActionScenedGoto:GetName()
	return 'ActionScenedGoto'
end

function ActionScenedGoto:IsInJumpPointRange(x, y)
	return self.player.my_unit:GetDistanceByPos(x, y) <= 2
end

--心跳
function ActionScenedGoto:Update(diff)
	local mapid = self.player:GetMapID()
	--不再这张地图了，无法同图寻路
	if(mapid ~= self.to_mapid)then
		outFmtDebug("ActionScenedGoto:Update mapid ~= self.to_mapid %s %u", self:ToString(),mapid)
		-- 发送消息好103
		self.player:call_instance_exit(0)
		if self.callback then
			self.callback()
		end
		return false, 1
	end
	
	local x, y = self.player:GetPos()
	--到达目的地了
	if(self.to_x == x and self.to_y == y)then
		--outFmtDebug("================222222222222222222222")
		if self.callback then
			self.callback()
		end
		return false, 2
	end
	
	if self.player:GetMapID() == 3002 then
		if(self.player.my_unit:IsMoving() == true)then
			if self.last_update_time == 0 then
				self.last_update_time = os.time()
			end
			if os.time() - self.last_update_time >= 1 then
				--self.player:call_unit_move (self.player.my_unit:GetUIntGuid(), x, y, {x, y})
				self.player:stopMoving(x,y)
				--self.player:SetPos(x,y)
				self.last_update_time = os.time()
				return true
			end
		end
	end
	
	if self.is_jump then
		return true
	end
	--[[
	--新手村才需要检测跳跃
	if(mapid == BORN_MAP)then
		--检测当前坐标是否是跳跃点
		for	i, jump_info in pairs(tb_map_jump_point_detail) do
			local v = jump_info.point
			if(mapid == jump_info.mapid and self:IsInJumpPointRange(v[ 1 ], v[ 2 ]))then
				self.player:stopMoving(x,y)
				
				self.is_jump = true
				local jumpEndCallback = function ()
					self.is_jump = false
				end
				
				self:PushAction('robotd.action.scened.action_scened_jump', i, jumpEndCallback)
				return true
			end
		end
	end
	]]
	if(self.player.my_unit:IsMoving() == false)then
		--outFmtDebug("================1111111111111111111111111111")
		--寻路发包
		local result = mapLib.PathfindingGoto(self.player.ptr, self.player.my_unit.ptr, self.to_x, self.to_y)
		if(result == false)then
			PathfindingGotoFailure(self.player,mapid)
			self:SetWaitTimeInterval(1000)
		end
	end
	return true
end


return ActionScenedGoto
