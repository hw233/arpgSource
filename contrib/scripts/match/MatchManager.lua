MatchManager = class('MatchManager')

--¹¹Ôìº¯Êý
function MatchManager:ctor()
	self.taskHash = {}
	self.removed = {}
end


function MatchManager:MarkUserMatchTask(id, task)
	if (self.removed[id] == 1) then
		outFmtError("Mark error !! userid = %d has already removed", id)
	end
	self.taskHash[id] = task
end


function MatchManager:UnmarkUserMatchTask(id)
	if (not self.taskHash[id]) then
		outFmtError("repeat remove !! userid = %d has already removed", id)
	end
	
	self.taskHash[id] = nil
	self.removed[id] = 1
end

function MatchManager:getMatchTask(id)
	return self.taskHash[id]
end