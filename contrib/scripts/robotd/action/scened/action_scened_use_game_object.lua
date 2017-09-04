
--注意事项
--不支持跨地图使用游戏对象，若要跨地图，请先寻路至此地图。

local ActionBase = require('robotd.action.action')
local ActionScenedUseGameObject = class('ActionScenedUseGameObject', ActionBase)

--获取类型
function ActionScenedUseGameObject:GetType()
	return ROBOT_ACTION_TYPE_SCENED
end

--初始化变量
function ActionScenedUseGameObject:Initialize(object_id, mapid, x, y, callback)	
	self.to_mapid = mapid
	self.to_x = x
	self.to_y = y
	self.object_id = object_id
	self.is_goto = false
	self.action_finish = false
	self.callback = callback
end

--获取类型名
function ActionScenedUseGameObject:GetName()
	return 'ActionScenedUseGameObject'
end

--心跳
function ActionScenedUseGameObject:Update(diff)
	--已经完成所有动作
	if(self.action_finish)then
		if self.callback then
			self.callback()
		end
		return false, 1
	end

	local mapid = self.player:GetMapID()
	local x, y = self.player:GetPos()
	--先寻路
	if(self.is_goto == false)then
		--已经在这里了就不用再寻路了
		if(mapid == self.to_mapid and self.player.my_unit:GetDistanceByPos(self.to_x,self.to_y)<=3)then
			outFmtDebug("ActionScenedUseGameObject:Update is already exist")
			self.is_goto = true
			return true
		end

		if(self.to_mapid > 0)then
			--访问GO随机点计算
			local pos_tab = mapLib.RandomPos(self.to_mapid,1,self.to_x, self.to_y,3)
			local is_canRun = mapLib.IsCanRun(self.to_mapid,pos_tab[1].x,pos_tab[1].y)
			if(is_canRun)then
				self.to_x = pos_tab[1].x
				self.to_y = pos_tab[1].y
			end
		end

		if(mapid == self.to_mapid)then
			self:PushAction('robotd.action.scened.action_scened_goto', self.to_mapid, self.to_x, self.to_y)
		else
			self:PushAction('robotd.action.scened.action_scened_pathfinding', self.to_mapid, self.to_x, self.to_y)
		end
		self.is_goto = true
		return true
	end
	
	--寻路没成功，使用失败
	if(mapid ~= self.to_mapid or self.player.my_unit:GetDistanceByPos(self.to_x,self.to_y)>3)then
		outFmtDebug("ActionScenedUseGameObject:Update %s use object fail", self:ToString())
		return false, 2
	end

	local unit = self.player:FindGameObject(self.object_id)
	if(unit == nil)then
		outFmtDebug("ActionScenedUseGameObject:Update object not find, %s", self:ToString())
		return false, 3
	else
		self.player:call_use_gameobject(unit:GetUIntGuid())
		--等待一秒以后退出
		self:SetWaitTimeInterval(1000)
		self.action_finish = true
	end
	
	return true
end


return ActionScenedUseGameObject
