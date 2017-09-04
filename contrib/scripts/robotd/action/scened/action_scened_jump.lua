local ActionBase = require('robotd.action.action')
local ActionJump = class('ActionJump', ActionBase)

--获取类型
function ActionJump:GetType()
	return ROBOT_ACTION_TYPE_SCENED
end

--初始化变量
function ActionJump:Initialize(indx, callback)
	self.indx = indx
	self.callback = callback
	self.toJump = false
end

--获取类型名
function ActionJump:GetName()
	return 'ActionJump'
end

--心跳
function ActionJump:Update(diff)
	if(self.toJump == false)then
		local x, y = self.player:GetPos()
		-- 先停止
		self.player:stopMoving(x,y)
		
		self.player:call_use_jump_point(self.indx)
		local jump_info = tb_map_jump_point_detail[self.indx]
		local len = #jump_info.show
		local to = jump_info.show[len]
		
		outFmtDebug("~~~~~~~~~~~ActionJump~~~~~~~~~~~~~guid:%s fromx:%s fromy:%s tox:%s toy:%s", self.player.my_unit:GetGuid(), x,y,to[1] ,to[2])


		self.player:SetPos(to[ 1 ], to[ 2 ])
		self:SetWaitTimeInterval(jump_info.last * 1000)
		
		self.toJump = true
		if self.callback then
			self.callback()
		end
		return true
	end
	
	--到达目的地了
	return false, 1

end


return ActionJump
