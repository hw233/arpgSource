MatchProcessor = class('MatchProcessor')

--[[IMatchProcessor instance = MatchProcessor:new {
	config = config,
}
--]]
--构造函数
function MatchProcessor:ctor()
	if self.config:getMatchExpandConfig() then
		self.matchRangeExpander = MatchTRExpander:new {
			matchExpandConfig = config:getMatchExpandConfig()
		}
	end
	
	if self.config:getCombineExpandConfig() then
		self.combineRangeExpander = CombineTRExpander:new {
			combineExpandConfig = config:getCombineExpandConfig()
		}
	end
	
	self.totalMatched = 0
	
	self.waitAddTaskQueue = {}
	self.taskQueue = {}
end

function MatchProcessor:submitMatch(task)
	task:setMaxUser(self.config:getUserMax())
	task:setStartTime(os.time())
	table.insert(waitAddTaskQueue, task)
end

function MatchProcessor:cancelMatch(task)
	table.removeItem(self.waitAddTaskQueue, task)
	table.removeItem(self.taskQueue		  , task)
	
	--把后来匹配的玩家重新打散排到队尾
	local externalPlayerTasks = task:separate()
	for _, externalTask in pairs(externalPlayerTasks) do
		table.insert(taskQueue, externalTask)
	end
	notifier:cancel(task)
end

function MatchProcessor:fillTaskQueue()
	if (#self.waitAddTaskQueue > 0) then
		for i = #self.waitAddTaskQueue, 1, -1 do
			local task = self.waitAddTaskQueue[ i ]
			table.insert(self.taskQueue, task)
			table.remove(self.waitAddTaskQueue, i)
		end
	end
end

--[[
     * 移到末尾位置
     */
--]]
function MatchProcessor:headToTail()
	local t = self.taskQueue[ 1 ];
	table.remove(self.taskQueue, 1)
	table.insert(self.taskQueue, t)
end

function MatchProcessor:expandMatchRange(task)
	if self.matchRangeExpander then
		if self.matchRangeExpander:isDifficutMatch(task) then
            self.matchRangeExpander:expandMatchRange(task)
		end
	end
end

function MatchProcessor:expandCombineRange(task)
	if self.combineRangeExpander then
		if self.combineRangeExpander:isDifficutMatch(task) then
            self.combineRangeExpander:expandMatchRange(task)
		end
	end
end

function MatchProcessor:matchTask(task)
	task:incrementMatchCount()
    self:expandMatchRange(task)
    
	local matchRangeRecords = task:getMatchRangeRecords()
	-- 是否必须对立阵营
	local campActive = config:isCampActive()
	
	for _, record in pairs(matchRangeRecords) do
		local i = 1
		while (i <= #self.taskQueue) do
			local position = self.taskQueue[ i ]
			if not position:isActive() then
				self:timeout(position)
				table.remove(self.taskQueue, i)
			else
				local baseCondition = (position ~= task and task:playersSizeEquals(position))
				if campActive then --如果必须是对立阵营才能成为对手，则判断
                    baseCondition = baseCondition and not task:isSameCamp(position)
                end
				
				if (baseCondition) then
                    local cpr = position:compareTo(task)
                    if (math.abs(cpr) <= record) then --找到合适的队伍
                        task:completed()
                        notifier:completed(task, position);
                        totalMatched = totalMatched + task:size() + position:size()
						table.remove(self.taskQueue, i) -- 先删除这个
						table.remove(self.taskQueue, 1) -- 匹配成功则移除头并且移除position
                        return position
                    end
				end
				i = i + 1
			end
		end
	end
	
	return nil
end

--[[
	组队
--]]
function MatchProcessor:organizeTeam()
    self:fillTaskQueue()
    local task = self.taskQueue[ 1 ]
            
    if (not task) then
		return
	end
            
    if (not task:isActive()) then -- 判断task是否失效
		table.remove(self.taskQueue, 1)
		self:timeout(task);
		return
	end

	if (not task:isFull()) then -- 组队未满人，先寻找平均值范围内的组成完整队伍
		self:expandCombineRange(task)
        self:combineTeam(task)
	end

    -- 遍历所有组合仍未能组成完整队伍，则将匹配请求放入尾部
	if (not task:isFull()) then --// 在此时出现线程安全问题，如玩家取消匹配
        -- // 一次全局遍历无法组成完整队伍，则分离队伍
		local externalPlayerTasks = task:separate()
		
		for _, externalTask in pairs(externalPlayerTasks) do
			table.insert(self.taskQueue, externalTask)
		end
		self:headToTail()
		return
    end

	local result = self:matchTask(task)
	if (not result) then
		-- // 未匹配成功，则将头移到尾部
		self:headToTail()
	else
		local remain = 0
		for _, t in pairs(self.taskQueue) do
			remain = remain + t:size()
		end
		--print("Complete a team match (remain = %d, totalMatched = %d)\n", remain, totalMatched);
	end
    
end

function MatchProcessor:combineTeam(task)
	-- 循环遍历比较匹配范围集合,从小的范围到大的范围开始比较
	local combineRangeRecords = task:getCombineRangeRecords()
			
	for _, record in pairs(combineRangeRecords) do --//范围扩张记录遍历，由小到大取最优
		
		local i = 1
		while (i <= #self.taskQueue) do -- //剩余所有Task遍历，用于组合完整队伍
			local position = self.taskQueue[ i ]
			
			if not position:isActive() then	--//判断position是否失效
				self:timeout(position)
				table.remove(self.taskQueue, i)
			else
				local campActive = self.config:isCampActive() --//是否需要阵营判断
				if (position ~= task and task:canCombine(position, campActive)) then --// 判断是否能够合并两个组
					local cpr = position:compareTo(task)
					--符合匹配要求
					if (math.abs(cpr) <= record) then
						task:combine(position, campActive) --// 合并
						table.remove(self.taskQueue, i)
					end
					if (task:isFull()) then
						return
					end
				end
				i = i + 1
			end
		end
	end
end

function MatchProcessor:timeout(task)
	task:Unmark()
end
  
function MatchProcessor:getNotifier()
	return self.notifier
end

function MatchProcessor:setNotifier(notifier)
	self.notifier = notifier
end