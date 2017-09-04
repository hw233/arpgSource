MatchConfig = class('MatchConfig')

-- IMatchConfig instance = MatchConfig:new {userMax = userMax, matchExpandConfig = matchExpandConfig, combineExpandConfig = combineExpandConfig}
MatchConfig.MATCH_SECONDS = 12000
MatchConfig.matchExecutePeriod = 1000

--¹¹Ôìº¯Êý
function MatchConfig:ctor()
	
end

function MatchConfig:getUserMax()
	return self.userMax
end

function MatchConfig:getMatchExpandConfig()
	return self.matchExpandConfig
end

function MatchConfig:getCombineExpandConfig()
	return self.combineExpandConfig
end

function MatchConfig:isCampActive()
	return false
end

function MatchConfig:getMatchExecutePeriod()
	return MatchConfig.matchExecutePeriod
end