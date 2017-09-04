MatchTask = class('MatchTask')

-- IMatchTask instance = MatchTask:new {teamPart = teamPart}
--构造函数
function MatchTask:ctor()
	self.maxUser = 0
	self.startTime = 0
	self.matchCount = 0
	
	self.matchRangeRecords = {0}
	self.combineRangeRecords = {0}
	self.partList = {}
end

function MatchTask:getMaxUser()
	return self.maxUser
end

function MatchTask:setMaxUser(maxUser)
	self.maxUser = maxUser
end

function MatchTask:getStartTime()
	return self.startTime
end

function MatchTask:setStartTime(startTime)
	self.startTime = startTime
end

function MatchTask:isActive()
	return startTime + MatchConfig.MATCH_SECONDS > os.time()
end

function MatchTask:isFull()
	return self:size() == self.maxUser
end

function MatchTask:size()
	local cnt = 0
	for _, teamPart in pairs(self.partList) do
		cnt = cnt + teamPart:size()
	end
	return cnt
end

function MatchTask:getMatchRangeRecords()
	return self.matchRangeRecords
end
	
function MatchTask:getCombineRangeRecords()
	return self.combineRangeRecords
end

function MatchTask:isSameCamp(task)
	return false
end

function MatchTask:playersSizeEquals(task)
	return self:size() == task:size()
end

function MatchTask:incrementMatchCount()
	self.matchCount = self.matchCount + 1
end

function MatchTask:compareTo(task)
	local aver1 = self:getAverageScore()
	local aver2 = task:getAverageScore()
	return aver1 - aver2
end

function MatchTask:getAverageScore()
	local scores = 0

	for _, teamPart in pairs(self.partList) do
		scores = scores + teamPart:getScores()
	end

	return math.floor(scores / self:size())
end

--[[
做某些事
--]]
function MatchTask:completed()
		
end

function MatchTask:canCombine(position, campActive)
	return self:size() + position:size() <= self.maxUser
end

function MatchTask:combine(position, campActive)
	table.foreach(position.partList, function(i, v)
		table.insert(self.partList, v)
	end)
	self:Mark()
end

--[[
	 * 分离除自己以外的
	 **/
--]]
function MatchTask:separate()
	local taskList = {}
	
	for i = 2, #self.partList do
		local teamPart = self.partList[ i ]
		local task = MatchTask:new {teamPart = teamPart}
		task:setMaxUser(self:getMaxUser())
		task:setStartTime(self:getStartTime())
		table.insert(taskList, task)
		task:Mark();
		self.partList[ i ] = nil
	end

	return taskList
end

function MatchTask:Mark()
	for _, teamPart in pairs(self.partList) do
		local userIdList = teamPart:getUserIdList()
		for _, id in pairs(userIdList) do
			MatchManager.MarkUserMatchTask(id, self)
		end
	end
end

function MatchTask:Unmark()
	for _, teamPart in pairs(self.partList) do
		local userIdList = teamPart:getUserIdList()
		for _, id in pairs(userIdList) do
			MatchManager.UnmarkUserMatchTask(id)
		end
	end
end

function MatchTask:getPartList()
	return self.partList
end
