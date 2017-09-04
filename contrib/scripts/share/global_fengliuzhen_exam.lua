
--风流镇主考官相关数据
local GlobalFengLiuZhenExam = class('GlobalFengLiuZhenExam', assert(BinLogObject))

function GlobalFengLiuZhenExam:ctor( )
	
end

--获取风流镇主考官某玩家得分
function GlobalFengLiuZhenExam:getPlayerScore(off)
	off = off -1
	return self:GetUInt32(FENGLIUZHEN_EXAM_INT_PLAYER_SCORE_BEGIN + off)
end
--设置风流镇主考官某玩家得分
function GlobalFengLiuZhenExam:setPlayerScore(off,value)
	off = off -1
	return self:SetUInt32(FENGLIUZHEN_EXAM_INT_PLAYER_SCORE_BEGIN + off,value)
end

--获取风流镇主考官某玩家加入时间
function GlobalFengLiuZhenExam:getPlayerAddTime(off)
	off = off -1
	return self:GetUInt32(FENGLIUZHEN_EXAM_INT_PLAYER_TIME_BEGIN + off)
end
--设置风流镇主考官某玩家加入时间
function GlobalFengLiuZhenExam:setPlayerAddTime(off,value)
	off = off -1
	return self:SetUInt32(FENGLIUZHEN_EXAM_INT_PLAYER_TIME_BEGIN + off,value)
end

--获取风流镇主考官某玩家GUID
function GlobalFengLiuZhenExam:getPlayerGuid(off)
	off = off -1
	return self:GetStr(FENGLIUZHEN_STRING_FIELD_PLAYER_GUID_BEGIN + off)
end
--设置风流镇主考官某玩家GUID
function GlobalFengLiuZhenExam:setPlayerGuid(off,value)
	off = off -1
	return self:SetStr(FENGLIUZHEN_STRING_FIELD_PLAYER_GUID_BEGIN + off,value)
end

--获取风流镇主考官某玩家名字
function GlobalFengLiuZhenExam:getPlayerName(off)
	off = off -1
	return self:GetStr(FENGLIUZHEN_STRING_FIELD_PLAYER_NAME_BEGIN + off)
end
--设置风流镇主考官某玩家名字
function GlobalFengLiuZhenExam:setPlayerName(off,value)
	off = off -1
	return self:SetStr(FENGLIUZHEN_STRING_FIELD_PLAYER_NAME_BEGIN + off,value)
end

--随机风流镇主考官考试开始时间
function GlobalFengLiuZhenExam:randExamStartTime()
	--清理排行榜数据
	self:clearRankPlayerData()
	local rand_time = randIntD(3600,9000)
	--从1-2.5小时随机一个时间  当大于2小时就不刷了
	if(rand_time <= 7200)then
		globalValue:setFengLiuZhenExamStartTime(os.time() + rand_time)
	end
end

--添加风流镇主考官排名数据
function GlobalFengLiuZhenExam:addRankPlayerData(player_guid,player_name,player_score)
	--比对下 分数是否比榜上的人 还高
	--如果有空位 直接塞进去  没空位 取出当前最小分数时间最迟的玩家
	local min_pos = 0
	local min_score = 0
	local min_time = 0
	for i = 1,MAX_FENGLIUZHEN_EXAM_PLAYER_NUM do
		if(self:getPlayerGuid(i) == "")then--有空位
			min_pos = i
			break
		else--没有空位
			local score = self:getPlayerScore(i)
			local add_time = self:getPlayerAddTime(i)
			--得分最小 或 得分一样 比对下时间
			if(min_pos == 0 or score < min_score or (score == min_score and add_time < min_time))then
				min_score = score
				min_time = add_time
				min_pos = i
			end
		end
	end
	--玩家进入排名
	if(min_pos > 0)then
		self:setPlayerGuid(min_pos,player_guid)
		self:setPlayerName(min_pos,player_name)
		self:setPlayerScore(min_pos,player_score)
		self:setPlayerAddTime(min_pos,os.time())
	end
end

--获取风流镇主考官 某玩家的排名
function GlobalFengLiuZhenExam:getPlayerRank(player_guid)
	local rank = 0
	for i = 1,MAX_FENGLIUZHEN_EXAM_PLAYER_NUM do
		if(self:getPlayerGuid(i) == player_guid)then
			rank = i
			break
		end
	end
	return rank
end

--清理风流镇主考官排名数据
function GlobalFengLiuZhenExam:clearRankPlayerData()
	for i = 1,MAX_FENGLIUZHEN_EXAM_PLAYER_NUM do
		self:setPlayerGuid(i,"")
		self:setPlayerName(i,"")
		self:setPlayerScore(i,0)
		self:setPlayerAddTime(i,0)
	end
end

return GlobalFengLiuZhenExam
