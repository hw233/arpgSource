
--设置上次引导id
function PlayerInfo:SetGuideIdLast(id)
	self:SetUInt32(PLAYER_INT_FIELD_GUIDE_ID_LAST,id)
end
--获得上次引导id
function PlayerInfo:GetGuideIdLast()
	return self:GetUInt32(PLAYER_INT_FIELD_GUIDE_ID_LAST,id)

end
--设置当前引导id 更新上次引导id
function PlayerInfo:SetGuideIdNow(id)
	if id < 1 then
		return
	end
	local now_id = self:GetGuideIdNow()
	self:SetGuideIdLast(now_id)
	self:SetUInt32(PLAYER_INT_FIELD_GUIDE_ID_NOW,id)
end
--获得当前引导id
function PlayerInfo:GetGuideIdNow()
	return self:GetUInt32(PLAYER_INT_FIELD_GUIDE_ID_NOW,id)

end


function PlayerInfo:UpdateGuideIdByTaskId (task_id)
	local taskInfo = tb_quest[task_id]
	if not taskInfo then
		return
	end
	local guide_id =  taskInfo.guide_id
	if guide_id > 0 then
		self:SetGuideIdNow(guide_id)
	end
end

function PlayerInfo:UpdateGuideIdByModuleId (module_id)
	local moduleInfo = tb_system_base[module_id]
	if not moduleInfo then
		return
	end
	-- 强制引导
	local guide_id =  moduleInfo.guide_id
	if guide_id > 0 then
		self:SetGuideIdNow(guide_id)
	end
	
	-- 非强制引导
	if moduleInfo.optional_guide_id then
		self:AddOptionalGuideId(moduleInfo.optional_guide_id)
	end
end

-- 添加非强制引导
function PlayerInfo:AddOptionalGuideId(guide_id)

	for i = PLAYER_INT_FIELD_OPTIONAL_GUIDE_START, PLAYER_INT_FIELD_OPTIONAL_GUIDE_END-1, 2 do
		if self:GetUInt32(i) == 0 then
			self:SetUInt32(i, guide_id)
			break
		end
	end
end

-- 一个非强制的某个步骤完成了
function PlayerInfo:OptionalGuideClicked(guide_id, step)
	if guide_id == 0 then
		return
	end
	for i = PLAYER_INT_FIELD_OPTIONAL_GUIDE_START, PLAYER_INT_FIELD_OPTIONAL_GUIDE_END-1, 2 do
		if self:GetUInt32(i) == guide_id then
			if step >= self:GetUInt32(i+1) then
				step = math.min(step, 12)
				self:SetUInt32(i+1, step+1)
			end
			return
		end
	end
end