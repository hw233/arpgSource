
require('match.config.CombineExpandConfig')
require('match.config.MatchConfig')
require('match.config.MatchExpandConfig')

require('match.CombineTRExpander')
require('match.MatchManager')
require('match.MatchProcessor')
require('match.MatchProcessorNotifier')
require('match.MatchTask')
require('match.MatchTRExpander')
require('match.TeamPart')
require('match.User')

local matchProcessor

function initMatch()
	local userMax = 3
	local combineExpandConfig = CombineExpandConfig:new {combineExpandConfig = {2, 4, 6, 10, 12, 14}}
	local matchExpandConfig = MatchExpandConfig:new {matchExpandConfig = {4, 8, 12, 16, 20, 24}}
	local matchConfig = MatchConfig:new {userMax = userMax, matchExpandConfig = matchExpandConfig, combineExpandConfig = combineExpandConfig}
	matchProcessor = MatchProcessor:new {config = matchConfig}
	
	matchProcessor:setNotifier(MatchProcessorNotifier:new{})
end

--[[
	userList = {user1, user2}
--]]
function submitMatchTask(userList)
	local teamPart = TeamPart:new{userList = userList}
	local task = MatchTask:new{teamPart = teamPart}
	task:Mark()
	
	matchProcessor:submitMatch(task)
end

function cancelMatch(id)
	local task = MatchManager.getMatchTask(id)
	if task then
		matchProcessor:cancelMatch(task)
	end
end