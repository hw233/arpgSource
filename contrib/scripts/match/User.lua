User = class('User')

-- IUser instance = User:new {id = id, score = score}
--¹¹Ôìº¯Êý
function User:ctor()
	
end

function User:getScore()
	return self.score
end

function User:getId()
	return self.id
end