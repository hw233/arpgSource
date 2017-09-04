
--注意事项
--不支持跨地图使用游戏对象，若要跨地图，请先寻路至此地图。

local ActionBase = require('robotd.action.action')
local ActionScenedDoSomething = class('ActionScenedDoSomething', ActionBase)

--获取类型
function ActionScenedDoSomething:GetType()
	return ROBOT_ACTION_TYPE_SCENED
end

--初始化变量
function ActionScenedDoSomething:Initialize(mapid, x, y, callback)	
	self.to_mapid = mapid
	self.to_x = x
	self.to_y = y
	self.is_goto = false
	self.action_finish = false
	self.callback = callback
	self.isClose = false
end

--获取类型名
function ActionScenedDoSomething:GetName()
	return 'ActionScenedDoSomething'
end

--心跳
function ActionScenedDoSomething:Update(diff)
	--已经完成所有动作
	if(self.action_finish)then
		return false, 1
	end

	local mapid = self.player:GetMapID()
	local x, y = self.player:GetPos()
	--先寻路
	if(self.is_goto == false)then
		--已经在这里了就不用再寻路了
		if(mapid == self.to_mapid and self.player.my_unit:GetDistanceByPos(self.to_x,self.to_y)<=2)then
			--outFmtDebug("ActionScenedDoSomething:Update %s use object fail", self:ToString())
			outFmtDebug("ActionScenedDoSomething:Update is already here")
			self.is_goto = true
			self.isClose = true
			return true
		end
		
		-- 到达目的地的callback
		local closeCallback = function()
			self.isClose = true
		end

		if(mapid == self.to_mapid)then
			self:PushAction('robotd.action.scened.action_scened_goto', self.to_mapid, self.to_x, self.to_y, closeCallback)
		else
			self:PushAction('robotd.action.scened.action_scened_pathfinding', self.to_mapid, self.to_x, self.to_y, closeCallback)
		end
		self.is_goto = true
		return true
	end
	
	if not self.isClose then
		return true
	end
	
	--寻路没成功，使用失败
	if(mapid ~= self.to_mapid or self.player.my_unit:GetDistanceByPos(self.to_x,self.to_y)>2)then
		outFmtDebug("ActionScenedDoSomething:Update %s dosomething fail", self:ToString())
		return false, 2
	end

	if self.callback then
		self.callback()
	end
	--等待一秒以后退出
	self:SetWaitTimeInterval(1000)
	self.action_finish = true
	
	return true
end


return ActionScenedDoSomething
