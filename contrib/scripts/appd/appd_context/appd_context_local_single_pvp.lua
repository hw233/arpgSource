
-- 匹配队列
SINGLE_PVP_QUEUE = {}

-- 初始化队列
if #SINGLE_PVP_QUEUE == 0 then
	for i = 1, #tb_single_pvp_grade do
		table.insert(SINGLE_PVP_QUEUE, {})
	end
end

function OnProcessLocalSinglePVPMatch()
	local now = os.time()
	-- 先把能匹配的匹配下来
	for i = #SINGLE_PVP_QUEUE, 1, -1 do
		local queue = SINGLE_PVP_QUEUE[ i ]
		local size = #queue
		-- 先把最后一个给留下来
		local last = nil
		if size % 2 == 1 then
			last = queue[size]
			size = size - 1
		end
		
		for j = 1, size, 2 do
			local fa = queue[ j ]
			local fb = queue[j+1]
			local key = string.format("%s#%s", fa[ 1 ], fb[ 1 ])
			OnLocalSinglePVPMatched(fa[ 1 ], key, now)
			OnLocalSinglePVPMatched(fb[ 1 ], key, now)
		end
		
		-- 把最后一个存进队列
		SINGLE_PVP_QUEUE[ i ] = {}
		if last then
			table.insert(SINGLE_PVP_QUEUE[ i ], last)
		end
	end	
	
	-- 现在每个阶段的队列最多只有1个人
	for i = #SINGLE_PVP_QUEUE, 1, -1 do
		local queue = SINGLE_PVP_QUEUE[ i ]
		if #queue > 0 then
			local fa = queue[ 1 ]
			-- 如果可以匹配其他段位了
			if fa[ 2 ] == 0 and os.time() >= fa[ 3 ] then
				fa[ 2 ] = 1
				fa[ 3 ] = os.time() + tb_single_pvp_base[ 1 ].matchLast
			end
			
			if fa[ 2 ] == 1 then
				if os.time() >= fa[ 3 ] then
					-- 和机器人比赛
					local key = string.format("%s#robot|%d", fa[ 1 ], i)
					OnLocalSinglePVPMatched(fa[ 1 ], key, now)
					SINGLE_PVP_QUEUE[ i ] = {}
				else
					-- 匹配比他段位低的
					for j = i-1, 1, -1 do
						local lowerQueue = SINGLE_PVP_QUEUE[ j ]
						if #lowerQueue > 0 then
							local fb = lowerQueue[ 1 ]
							local key = string.format("%s#%s", fa[ 1 ], fb[ 1 ])
							OnLocalSinglePVPMatched(fa[ 1 ], key, now)
							OnLocalSinglePVPMatched(fb[ 1 ], key, now)
							-- 清空数据
							SINGLE_PVP_QUEUE[ i ] = {}
							SINGLE_PVP_QUEUE[ j ] = {}
							break
						end
					end
				end
			end
		end
	end
end


function OnLocalSinglePVPMatched(guid, key, now)
	outFmtInfo("################# OnLocalSinglePVPMatched matched, guid = %s key = %s", guid, key)
	-- 进行进入比赛逻辑
	local generalId = string.format("%d|%s", now, key)
	local playerInfo = app.objMgr:getObj(guid)
	
	local kk = string.split(key, '|')
	local guidTable = string.split(kk[ 1 ], '#')
	local indx = 1
	for i = 1, #guidTable do
		if guid == guidTable[ i ] then
			indx = i
		end
	end
	local enterPos = tb_single_pvp_base[ 1 ].enterPos
	local x = enterPos[indx][ 1 ]
	local y = enterPos[indx][ 2 ]
	local mapid = tb_single_pvp_base[ 1 ].mapid
	-- 发起传送
	call_appd_teleport(playerInfo:GetScenedFD(), guid, x, y, mapid, generalId)
	
	app:SetMatchingKuafuType(guid, nil)
	
	local instMgr = playerInfo:getInstanceMgr()
	instMgr:SubQualifyDailyTimes()
	
	playerInfo:AddActiveItem(VITALITY_TYPE_SINGLE_PVP)
	playerInfo:onUpdatePlayerQuest(QUEST_TARGET_TYPE_JOIN_SINGLE_PVP, {})
end

-- 取消匹配组队副本
function PlayerInfo:OnCancelLocalSinglePVPMatchBeforeOffline()
	if not app:IsKuafuTypeMatching(self:GetGuid(), MATCH_TYPE_LOCAL_SINGLE_PVP) then
		return
	end
	
	local indx = self:calcQueueIndx()
	local queue = SINGLE_PVP_QUEUE[indx]
	
	for i = 1, #queue do
		local fa = queue[ i ]
		if fa[ 1 ] == self:GetGuid() then
			table.remove(queue, i)
			break
		end
	end
	
	app:SetMatchingKuafuType(self:GetGuid(), nil)
end

-- 匹配组队副本
function PlayerInfo:OnLocalSinglePVPMatch()
	-- 已经在跨服了
	if self:IsKuafuing() then
		return false
	end
	
	if app:IsInKuafuTypeMatching(self:GetGuid()) then
		return false
	end
	
	app:SetMatchingKuafuType(self:GetGuid(), MATCH_TYPE_LOCAL_SINGLE_PVP)
	
	local indx = self:calcQueueIndx()
	if self:isMatchRobot() then
		local key = string.format("%s#robot|%d", self:GetGuid(), indx)
		OnLocalSinglePVPMatched(self:GetGuid(), key, os.time())
	else
		-- 加入匹配队列
		table.insert(SINGLE_PVP_QUEUE[indx], {self:GetGuid(), 0, os.time() + tb_single_pvp_base[ 1 ].matchLast})
		-- {guid, matchTimes, 本队列时间戳}
	end
	
	return true
end

function PlayerInfo:calcQueueIndx()
	local score = self:GetQualifyScore()
	local indx = #tb_single_pvp_grade	
	for i = 1, #tb_single_pvp_grade do
		local range = tb_single_pvp_grade[ i ].range
		if score >= range[ 1 ] and score <= range[ 2 ] then
			indx = i
			break
		end
	end
	
	return indx
end

function PlayerInfo:isMatchRobot()
	local score = self:GetQualifyScore()
	return score <= tb_single_pvp_base[ 1 ].robotScore
end

-- 购买进入次数
function PlayerInfo:OnBuyLocalSinglePVPTicket(count)
	
end


-- 重置单人匹配副本
function PlayerInfo:OnResetLocalSinglePVPDayTimes()
	local instMgr = self:getInstanceMgr()
	instMgr:ResetQualifyDaily()
end

function PlayerInfo:OnResetLocalSinglePVPWeekInfo()
	self:SetInitQualifyScore()
end

QUALIFY_RECORD_FORMAT = '%s|%d|%d|%d' -- fightName|result|score|timestamp

function PlayerInfo:OnProcessSingleMatchResult(result, fightName)
	local indx = self:calcQueueIndx()
	local gradeConfig = tb_single_pvp_grade[indx]
	local instMgr = self:getInstanceMgr()
	local score
	
	if result == GlobalCounter.WIN then
		-- 加次数
		instMgr:AddQualifyWins(1)
		
		-- 胜利计算额外奖励
		local wins = instMgr:GetQualifyWins()
		for i = 1, #tb_single_pvp_extra do
			local config = tb_single_pvp_extra[ i ]
			if config.wins == wins then
				instMgr:SetQualifyExtraObtain(i-1)
			end
		end
		score = gradeConfig.win
		self:AddQualifyScore(score)
		
		self:onUpdatePlayerQuest(QUEST_TARGET_TYPE_SINGLE_PVP_WINS, {})
	else
		score = gradeConfig.lose
		self:SubQualifyScore(score)
	end
	
	-- 排名
	rankInsertTask(self:GetGuid(), RANK_TYPE_SINGLE_PVP)
	local record = string.format(QUALIFY_RECORD_FORMAT, fightName, result, score, os.time())
	instMgr:AddQaulifyRecord(record)
end


-- 获得排位赛积分
function PlayerInfo:GetQualifyScore()
	return self:GetUInt32(PLAYER_INT_FIELD_QUALIFY_SCORE)
end

-- 增加排位赛积分
function PlayerInfo:AddQualifyScore(val)
	self:AddUInt32(PLAYER_INT_FIELD_QUALIFY_SCORE, val)
end

-- 减少排位赛积分
function PlayerInfo:SubQualifyScore(val)
	local curr = self:GetQualifyScore()
	-- 已经扣到底了
	if curr == 0 then
		return
	end
	val = math.min(curr, val)
	self:SubUInt32(PLAYER_INT_FIELD_QUALIFY_SCORE, val)
end

-- 设置排位赛积分
function PlayerInfo:SetInitQualifyScore()
	local val = tb_single_pvp_base[ 1 ].initScore
	self:SetUInt32(PLAYER_INT_FIELD_QUALIFY_SCORE, val)
end