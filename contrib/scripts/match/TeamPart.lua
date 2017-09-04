TeamPart = class('TeamPart')

-- ITeamPart instance = TeamPart:new {userList = userList}
--¹¹Ôìº¯Êý
function TeamPart:ctor()
	local value = 0
	for _, user in pairs(self.userList) do
		value = value + user:getScore()
	end
	
	self.scores = value
	self.average = math.floor(self.scores / self:size())
end

function TeamPart:size()
	return #self.userList
end

function TeamPart:getScores()
	return self.scores
end

function TeamPart:getAverage()
	return self.average
end

function TeamPart:getUserIdList()
	local userIdList = {}
	
	for _, user in pairs(self.userList) do
		table.insert(userIdList, user:getId())
	end
	
	return userIdList
end