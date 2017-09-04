--tb_system_base_task_list
--tb_system_base_level_list


--记录已开启功能
--获得索引
function PlayerInfo:GetOpenMenuIndex()
	return self:GetUInt32(PLAYER_INT_FIELD_OPEN_MENU_INDEX)
end

--设置索引
function PlayerInfo:SetOpenMenuIndex(val)
	return self:SetUInt32(PLAYER_INT_FIELD_OPEN_MENU_INDEX,val)
end

--int开始下标
function PlayerInfo:OpenMenuStart()
	return PLAYER_INT_FIELD_OPEN_MENU_START + self:GetOpenMenuIndex() * MAX_OPEN_MENU_ATTR_COUNT
end

--索引加1
function PlayerInfo:OpenMenuNext()
	local cur_num = self:GetOpenMenuIndex()
	if(cur_num >= MAX_PLAYER_OPEN_MENU_COUNT-1)then		--礼包信息最多保留MAX_GIFTPACKS_INFO_COUNT
		self:SetOpenMenuIndex(0)	
	else
		self:AddUInt32(PLAYER_INT_FIELD_OPEN_MENU_INDEX, 1)
	end
end

--查询当前所有开放功能主id{id: index:}
function PlayerInfo:GetAllOpenMenuIds()
	local allIds = {}
	for i=0,self:GetOpenMenuIndex() -1 do
		local id = self:GetUInt32(PLAYER_INT_FIELD_OPEN_MENU_START + i * MAX_OPEN_MENU_ATTR_COUNT + OPEN_MENU_MAIN_ID)
		allIds[id] = i
	end
	return allIds
end

--查询目标功能id
function PlayerInfo:GetOpenMenuFlag(id,subid)
	local allIds = self:GetAllOpenMenuIds()
	if allIds[id] == nil then
		return false
	end
	return self:GetBit(PLAYER_INT_FIELD_OPEN_MENU_START + allIds[id] * MAX_OPEN_MENU_ATTR_COUNT + OPEN_MENU_SUB_FLAG,subid)
end

--添加功能id
function PlayerInfo:AddOpenMenuFlag(id,subid)
	if self:GetOpenMenuFlag(id,subid) then
		--outFmtInfo("AddOpenMenuFlag: already set flag true: %d %d",id,subid)
		return 0
	end

	--测试
	if id == MODULE_PRACTICE and subid == MODULE_PRACTICE_EXP then
		self:InitCultivation()
	end
	
	self:call_module_active (id * 10 + subid)
	
	local allIds = self:GetAllOpenMenuIds()
	if allIds[id] == nil then
		if self:GetOpenMenuIndex() == MAX_PLAYER_OPEN_MENU_COUNT then
			outFmtInfo("AddOpenMenuFlag: can not add more OpenMenuId!")
			return 0
		end
		self:SetUInt32(self:OpenMenuStart() + OPEN_MENU_MAIN_ID,id)
		self:SetBit(self:OpenMenuStart() + OPEN_MENU_SUB_FLAG,subid)
		self:OpenMenuNext()
		
		self:UpdateGuideIdByModuleId(id*10 + subid)
		return 1
	else
		self:SetBit(PLAYER_INT_FIELD_OPEN_MENU_START + allIds[id] * MAX_OPEN_MENU_ATTR_COUNT + OPEN_MENU_SUB_FLAG,subid)
		self:UpdateGuideIdByModuleId(id*10 + subid)
		return 1
	end
end


function PlayerInfo:UnlockModuleByTaskId(taskId)
	local flag = 0
	local list = tb_system_base_task_list[taskId]
	if list ==	nil then
		--outFmtInfo("UnlockModuleByTaskId: no module can unlock  with task:%d ",taskId)
		return flag
	else
		for i,id in ipairs(list) do
			if self:AddOpenMenuFlag(math.floor(id/10),id%10) == 1 then
			
				flag = 1 
			end
			--outFmtInfo("UnlockModuleByTaskId: unlock module %d ",id)
		end
	end
	
	--self:printAllUnlock()

	return flag
end

function PlayerInfo:UnlockModuleByLevel(startLv,endLv)
	local flag = 0
	for i = startLv+1,endLv do
		
		local list = tb_system_base_level_list[i]
		if list then
			
			for j,id in ipairs(list) do
				if self:AddOpenMenuFlag(math.floor(id/10),id%10) == 1 then
				
					flag = 1
				end
				--outFmtInfo("UnlockModuleByLevel: unlock module %d ",id)
			end
		end
	end
	--self:printAllUnlock()
	return flag
end


function PlayerInfo:printAllUnlock()
	local allIds = self:GetAllOpenMenuIds()
	for id,index in pairs(allIds) do
		outFmtInfo("id: %d index:%d ",id,index)
		for i = 0, 9 do
			outFmtInfo("subid:%d flag:%s",i,self:GetOpenMenuFlag(id,i))
			
		end
	
	end
end