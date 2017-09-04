local ActionBase = require('robotd.action.action')
local ActionScenedPathfinding = class('ActionScenedPathfinding', ActionBase)

--获取类型
function ActionScenedPathfinding:GetType()
	return ROBOT_ACTION_TYPE_SCENED
end

TELEPORT_ENTRY = 6

--初始化变量
function ActionScenedPathfinding:Initialize(mapid, x, y, callback)
	self.to_mapid = mapid
	self.to_x = x
	self.to_y = y
	self.callback = callback
	self.isTeleport = false
	self.is_goto = false
end

--获取类型名
function ActionScenedPathfinding:GetName()
	return 'ActionScenedPathfinding'
end

--心跳
function ActionScenedPathfinding:Update(diff)
	--精灵对象还没加入地图
	if(self.player:GetMapID() == 0)then
		return true
	end
	local x,y = self.player:GetPos()
	local mapid = self.player:GetMapID()
			--同图寻路
	if(mapid == self.to_mapid)then
		if(math.floor(x) == self.to_x and math.floor(y) == self.to_y)then
			-- 到达目的地
			if self.callback then
				self.callback()
			end
			return false, 1
		end
		-- 先寻路
		if not self.is_goto then
			self.is_goto = true
			--如果是障碍点
			local is_canRun = mapLib.IsCanRun(self.to_mapid, self.to_x, self.to_y)
			if(is_canRun == false)then
				outFmtDebug("ActionScenedPathfinding:Update create pos can't run %u %u %u", self.to_mapid, self.to_x, self.to_y)
				local pos_tab = mapLib.RandomPos(self.to_mapid,1, self.to_x, self.to_y,4)
				is_canRun = mapLib.IsCanRun(self.to_mapid,pos_tab[1].x,pos_tab[1].y)
				if(is_canRun)then
					self.to_x = pos_tab[1].x
					self.to_y = pos_tab[1].y
				end
			end

			self:PushAction('robotd.action.scened.action_scened_goto', self.to_mapid, self.to_x, self.to_y)
			return true
		end
		
		return true
	end
	
	
	-- 还在找传送门
	if self.isTeleport then
		return true
	end
	self.isTeleport = true
	
	--- TODO:如果是一个长路径 如何表示
	
	--跨图寻路,先找到传送点，然后使用游戏对象
	local tele_pos = self:FindTeleObject()
	if(tele_pos == nil)then
		--outFmtInfo('ActionScenedPathfinding:Update tele_pos == nil %u  %u', self.player:GetMapID(), self.to_mapid)
		self.player:call_instance_exit(0)
		if self.callback then
			self.callback()
		end
		return false, 2
	end
	
	-- 传送完后的
	local closeCallback = function()
		self.isTeleport = false
	end
	
	self:PushAction('robotd.action.scened.action_scened_use_teleport', TELEPORT_ENTRY, mapid, tele_pos[ 1 ], tele_pos[ 2 ], closeCallback)

	return true
end

--寻找下一个要去的地图传送点
function ActionScenedPathfinding:FindTeleObject()
	local mapid = self.player:GetMapID()
	if not tb_map_teleport[mapid] then
		return
	end
	--当前地图所有的传送点
	local tele_pos = tb_map_teleport[mapid].teleports
	-- 没有返回
	if not tele_pos then return end
	for	i, v in ipairs(tele_pos) do		
		if(self:IsToMapObject(v))then
			return tb_map_teleport_detail[v].pos
		end
	end
	return nil
end

function ActionScenedPathfinding:IsToMapObject(teleportId)
	local tomapid = tb_map_teleport_detail[teleportId].tomapid
	if(tomapid == self.to_mapid)then
		return true
	end
	return false
end

return ActionScenedPathfinding
