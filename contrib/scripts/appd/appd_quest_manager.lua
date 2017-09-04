--技能管理器

local AppQuestMgr = class("AppQuestMgr", BinLogObject)

function AppQuestMgr:ctor()
	
end
---成就--------------------------------------------------
--获取成就进度
function AppQuestMgr:getAchieve(id)
	local idx = QUEST_FIELD_ACHIEVE_START + (id - 1) * MAX_ACHIEVE_FIELD + ACHIEVE_FIELD_CURRENT
	return self:GetUInt32(idx)
end
--设置成就进度
function AppQuestMgr:setAchieve(id,num)
	local idx = QUEST_FIELD_ACHIEVE_START + (id - 1) * MAX_ACHIEVE_FIELD + ACHIEVE_FIELD_CURRENT
	self:SetUInt32(idx,num)
end
--是否领取成就奖励
function AppQuestMgr:getAchieveReward(id)
	local idx = QUEST_FIELD_ACHIEVE_START + (id - 1) * MAX_ACHIEVE_FIELD + ACHIEVE_FIELD_REWARD
	return self:GetByte(idx,0)
end
--领取成就奖励
function AppQuestMgr:setAchieveReward(id)
	local idx = QUEST_FIELD_ACHIEVE_START + (id - 1) * MAX_ACHIEVE_FIELD + ACHIEVE_FIELD_REWARD
	self:SetByte(idx,0,1)
end
--是否达成成就
function AppQuestMgr:getHasAchieve(id)
	local idx = QUEST_FIELD_ACHIEVE_START + (id - 1) * MAX_ACHIEVE_FIELD + ACHIEVE_FIELD_REWARD
	return self:GetByte(idx,1)
end
--达成成就
function AppQuestMgr:setHasAchieve(id)
	local idx = QUEST_FIELD_ACHIEVE_START + (id - 1) * MAX_ACHIEVE_FIELD + ACHIEVE_FIELD_REWARD
	self:SetByte(idx,1,1)
end

--添加总成就点数
function AppQuestMgr:addAchieveAll(num)
	self:AddUInt32(QUEST_FIELD_ACHIEVE_ALL,num)
end
--获取总成就点数
function AppQuestMgr:getAchieveAll()
	return self:GetUInt32(QUEST_FIELD_ACHIEVE_ALL)
end

--总成就奖励ID
function AppQuestMgr:getAchieveAllReward()
	return self:GetUInt32(QUEST_FIELD_ACHIEVE_REWARD)
end
--设置总成就奖励ID
function AppQuestMgr:addAchieveAllReward()
	self:AddUInt32(QUEST_FIELD_ACHIEVE_REWARD,1)
end

---称号--------------------------------------------------
--添加称号ID
function AppQuestMgr:addTitle(id)
	local config = tb_title_base[id]
	if not config then 
		return false
	end
	
	local hast,tid = self:hasTitle(id)
	if hast then
		if config.limtime > 0 then
			self:SetUInt32(tid + TITLE_FIELD_TIME,os.time() + config.limtime * 60)
		else
			self:SetUInt32(tid + TITLE_FIELD_TIME,0)
		end
		local owner = self:getOwner()
		owner:RecalcAttrAndBattlePoint()
		return true
	end
	
	for i=QUEST_FIELD_TITLE_START,QUEST_FIELD_TITLE_END-1,MAX_TITLE_FIELD do
		if self:GetUInt16(i,0) == 0 then
			self:SetUInt16(i,0,id)
			self:SetUInt16(i,1,0)
			if config.limtime > 0 then
				self:SetUInt32(i + TITLE_FIELD_TIME,os.time() + config.limtime * 60)
			else
				self:SetUInt32(i + TITLE_FIELD_TIME,0)
			end
			local owner = self:getOwner()
			owner:RecalcAttrAndBattlePoint()
			return true
		end
	end
	
	return false
end

--是否拥有某个称号
function AppQuestMgr:hasTitle(id)
	for i=QUEST_FIELD_TITLE_START,QUEST_FIELD_TITLE_END-1,MAX_TITLE_FIELD do
		if self:GetUInt16(i,0) == id then
			return true,i
		end
	end
	return false,0
end

--初始化称号
function AppQuestMgr:initTitle(idx)
	if self:GetUInt16(idx,1) == 0 then
		self:SetUInt16(idx,1,1)
	end
end

--称号装备重算 修改为计算所有拥有称号属性
function AppQuestMgr:calculTitleAttr(attrs)
	local allForce = 0
	local owner = self:getOwner()
	for i=QUEST_FIELD_TITLE_START,QUEST_FIELD_TITLE_END-1,MAX_TITLE_FIELD do
		local title_id = self:GetUInt16(i,0)
		if title_id ~= 0 then
			local config = tb_title_base[title_id].prop
			local baseForce = DoAnyOneCalcForceByAry(config, owner:GetGender())
			allForce = allForce + baseForce
			for i=1,#config do
				attrs[config[i][1]] = attrs[config[i][1]] + config[i][2]
			end
		end
	end
	
	owner:SetTitleForce(allForce)
end
--检测失效称号
function AppQuestMgr:removeExpireTitle()
	local owner = self:getOwner()
	local cur = owner:GetTitle()
	for i=QUEST_FIELD_TITLE_START,QUEST_FIELD_TITLE_END-1,MAX_TITLE_FIELD do
		local id = self:GetUInt16(i,0)
		if id ~= 0 then
			local time  = self:GetUInt32(i + TITLE_FIELD_TIME)
			if time > 0 and time < os.time() then
				self:SetUInt16(i,0,0) 
				self:SetUInt32(i + TITLE_FIELD_TIME,0)
				
				if cur == id then
					owner:SetTitle(0)
				end
				owner:RecalcAttrAndBattlePoint()
			end
		end
	end
end
-------------------------------福利-------------------------------
--是否领取首冲奖励
function AppQuestMgr:getWelfareShouchong()
	return self:GetUInt32(QUEST_FIELD_WELFARE_SHOUCHONG)
end
--领取首冲奖励标记
function AppQuestMgr:setWelfareShouchong()
	self:SetUInt32(QUEST_FIELD_WELFARE_SHOUCHONG,1)
end

--每日签到奖励是否已领取
function AppQuestMgr:getWelfareCheckIn(day)
	return self:GetBit(QUEST_FIELD_WELFARE_CHECKIN,day-1)
end
--领取每日签到奖励
function AppQuestMgr:setWelfareCheckIn(day)
	return self:SetBit(QUEST_FIELD_WELFARE_CHECKIN,day-1)
end
--本月累积签到次数
function AppQuestMgr:getWelfareCheckInDayNum()
	local num = 0
	for i = 0,31 do
		if self:GetBit(QUEST_FIELD_WELFARE_CHECKIN,i) then
			num = num + 1
		end
	end
	return num
end

--累积每日签到奖励是否已领取
function AppQuestMgr:getWelfareCheckInAll(id)
	return self:GetBit(QUEST_FIELD_WELFARE_CHECKIN_ALL,id)
end

--领取累积每日签到奖励
function AppQuestMgr:setWelfareCheckInAll(id)
	return self:SetBit(QUEST_FIELD_WELFARE_CHECKIN_ALL,id)
end

--等级是否已领取
function AppQuestMgr:getWelfareLev(id)
	return self:GetBit(QUEST_FIELD_WELFARE_LEVEL,id-1)
end
--领取等级奖励
function AppQuestMgr:setWelfareLev(id)
	return self:SetBit(QUEST_FIELD_WELFARE_LEVEL,id-1)
end

--type类型所有可以找回的次数
function AppQuestMgr:setWelfareBackAllNum(type,allnum)
	local idx = QUEST_FIELD_WELFARE_BACK_START + (type-1) * MAX_WELFA_BACK_ITEM + WELFA_BACK_ITEM_NUM
	self:SetUInt32(idx,allnum)
end
function AppQuestMgr:getWelfareBackAllNum(type)
	local idx = QUEST_FIELD_WELFARE_BACK_START + (type-1) * MAX_WELFA_BACK_ITEM + WELFA_BACK_ITEM_NUM
	return self:GetUInt32(idx)
end
--type类型下指定时间的已使用次数
function AppQuestMgr:getWelfareBackNum(type,time)
	local startIdx = QUEST_FIELD_WELFARE_BACK_START + (type-1) * MAX_WELFA_BACK_ITEM
	
	for i=WELFA_BACK_ITEM,WELFA_BACK_ITEM_END,2 do
		local idx = startIdx + i
		local dtime = self:GetUInt32(idx)
		if time == dtime then
			return self:GetUInt32(idx+1)
		end
	end
	
	return 0
end

--添加type类型下指定时间的已使用次数
function AppQuestMgr:addWelfareBackNum(type,time,num)
	
	local targetIdx,outTimeIdx = self:getWelfareTimeIdx(type,time)

	if targetIdx ~= 0 then
		local curnum = self:GetUInt32(targetIdx+1)
		curnum = curnum + num
		self:SetUInt32(targetIdx+1,curnum)
	elseif outTimeIdx ~=0 then
		self:SetUInt32(outTimeIdx,time)
		self:SetUInt32(outTimeIdx+1,num)
	end
	
end

function AppQuestMgr:setWelfareBackNum(type,time,num)
	
	local targetIdx,outTimeIdx = self:getWelfareTimeIdx(type,time)

	if targetIdx ~= 0 then
		self:SetUInt32(targetIdx+1,num)
	elseif outTimeIdx ~=0 then
		self:SetUInt32(outTimeIdx,time)
		self:SetUInt32(outTimeIdx+1,num)
	end
	
end


function AppQuestMgr:getWelfareTimeIdx(type,time)
	local startIdx = QUEST_FIELD_WELFARE_BACK_START + (type-1) * MAX_WELFA_BACK_ITEM
	local targetIdx = 0
	local outTimeIdx = 0
	local curTime = GetTodayStartTimestamp(0)
	for i=WELFA_BACK_ITEM,WELFA_BACK_ITEM_END-1,2 do
		--outFmtDebug("welf %d",i)
		local idx = startIdx + i
		local dtime = self:GetUInt32(idx)
		if time == dtime then
			targetIdx = idx
			break
		elseif dtime == 0 then
			outTimeIdx = idx
		elseif self:testOutTime(curTime,dtime) then
			outTimeIdx = idx
		end
	end
	return targetIdx,outTimeIdx
end

function AppQuestMgr:testOutTime(curTime,targetTime)
	local num = curTime - targetTime
	--outFmtDebug("chazhi %d",num)
	if num >= 345600 then
		return true
	end
	return false
end

--累计充值奖励领取状态
function AppQuestMgr:GetRechargeRewardFlag(index)
	return self:GetBit(QUEST_FIELD_WELFARE_RECHARGE_REWARD_FLAG,index)
end

function AppQuestMgr:SetRechargeRewardFlag(index)
	return self:SetBit(QUEST_FIELD_WELFARE_RECHARGE_REWARD_FLAG,index)
end

--累计消费奖励领取状态
function AppQuestMgr:GetConsumeRewardFlag(index)
	return self:GetBit(QUEST_FIELD_WELFARE_CONSUME_REWARD_FLAG,index)
end

function AppQuestMgr:SetConsumeRewardFlag(index)
	return self:SetBit(QUEST_FIELD_WELFARE_CONSUME_REWARD_FLAG,index)
end

--七日大礼进度
function AppQuestMgr:GetSevenDayProcess()
	return self:GetUInt32(QUEST_FIELD_WELFARE_SEVEN_DAY_PROCESS)
end

function AppQuestMgr:AddSevenDayProcess(val)
	return self:AddUInt32(QUEST_FIELD_WELFARE_SEVEN_DAY_PROCESS,val)
end

--七日大礼奖励领取状态
function AppQuestMgr:GetSevenDayFlag(index)
	return self:GetBit(QUEST_FIELD_WELFARE_SEVEN_DAY_FLAG,index)
end

function AppQuestMgr:SetSevenDayFlag(index)
	return self:SetBit(QUEST_FIELD_WELFARE_SEVEN_DAY_FLAG,index)
end



-------------------------------下面是任务-------------------------------
--[[
// 最多可领取任务个数
#define MAX_QUEST_COUNT 10
// 任务最多目标
#define MAX_QUEST_TARGET_COUNT 5
QUEST_TARGET_INFO_SHORT0	= 0,	//0:状态, 1:目标值
QUEST_TARGET_INFO_PROCESS	= 1,	//进度
MAX_QUEST_TARGET_INFO_COUNT		

QUEST_INFO_ID				= 0,					//任务id
QUEST_INFO_STEP_START		= QUEST_INFO_ID + 1,	//任务分步骤开始
QUEST_INFO_STEP_END			= QUEST_INFO_STEP_START + MAX_QUEST_STEP_COUNT,	//任务分步骤结束
MAX_QUEST_INFO_COUNT		= QUEST_INFO_STEP_END,

QUEST_FIELD_QUEST_START			//任务开始
QUEST_FIELD_QUEST_END			//任务结束
--]]

-- 每完成一个每日任务判断是否需要领奖
function AppQuestMgr:OnGetDailyQuestRewards()
	local playerInfo = self:getOwner()
	local finished = playerInfo:GetDailyQuestFinished()
	local config = tb_quest_daily[finished]
	if config then
		local rewards = config.rewards
		-- 判断背包格子是否足够
		local itemMgr = playerInfo:getItemMgr()
		local emptys  = itemMgr:getEmptyCount(BAG_TYPE_MAIN_BAG)
		if emptys < #rewards then
			playerInfo:CallOptResult(OPRATE_TYPE_BAG, BAG_RESULT_BAG_FULL)
			return
		end
		playerInfo:AppdAddItems(rewards, MONEY_CHANGE_QUEST, LOG_ITEM_OPER_TYPE_QUEST, 1, 0, ITEM_SHOW_TYPE_MINI_QUEST_BAR)
	end
end

-- 随机生成每日任务
function AppQuestMgr:RandomGenerateDailyQuest()
	local playerInfo = self:getOwner()
	-- 如果已经做完了就不接了
	local finished = playerInfo:GetDailyQuestFinished()
	if finished >= tb_quest_daily_base[ 1 ].dailyLimit then
		return
	end
	
	-- 接下一个任务
	local level = playerInfo:GetLevel()
	local questList = tb_quest_daily_list[level]
	if questList then
		local indx = randInt(1, #questList)
		local questId = questList[indx]
		self:OnAddQuest(questId)
	else
		outFmtError("designer not set daily quest in tb_char_level or programer not init")
	end
end

function AppQuestMgr:OnDailyQuestReset()
	local playerInfo = self:getOwner()
	playerInfo:ClearDailyQuestFinished()
	
	-- 环任务
	self:OnAddFirstCircleQuest(tb_quest_daily_base[ 1 ].npcQuest)
	
	-- 日常任务
	self:OnAddFirstDaily2Quest(tb_quest_daily2_base[ 1 ].npcQuest)
end

--[[
	在每日重置 和 角色升级的时候可能触发
--]]
function AppQuestMgr:OnAddFirstCircleQuest(firstQuestId)
	if not tb_quest[firstQuestId] then
		return
	end
	
	local playerInfo = self:getOwner()
	-- 等级满足才能接取
	if playerInfo:GetLevel() < tb_quest[firstQuestId].level then
		return
	end
	
	-- 看看等级是否满足条件
	-- 删除原来的任务
	for start = QUEST_FIELD_QUEST_START, QUEST_FIELD_QUEST_END - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)

		if questId > 0 and tb_quest[questId].type == QUEST_TYPE_DAILY then
			self:OnRemoveQuest(start)
		end
	end
	-- 给每日任务的第一个任务
	self:OnAddQuest(firstQuestId)
end

--[[
	在每日重置 和 角色升级的时候可能触发
--]]
function AppQuestMgr:OnAddFirstDaily2Quest(firstQuestId)
	if not tb_quest[firstQuestId] then
		return
	end
	
	local playerInfo = self:getOwner()
	
	-- 等级满足才能接取
	if playerInfo:GetLevel() < tb_quest[firstQuestId].level then
		return
	end
	
	-- 删除原来的任务
	for start = QUEST_FIELD_QUEST_START, QUEST_FIELD_QUEST_END - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)

		if questId > 0 and tb_quest[questId].type == QUEST_TYPE_DAILY2 then
			self:OnRemoveQuest(start)
		end
	end
	
	-- 日常任务
	self:OnAddQuest(firstQuestId)
	
	-- 重置日常任务个数
	self:ClearDaily2Finished()
	
	-- 重置日常任务提交情况
	self:ClearDaily2Submit()
	
	-- 清空日常任务
	for start = QUEST_FIELD_DAILY2_QUEST_START, QUEST_FIELD_DAILY2_QUEST_END - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)

		if questId > 0 and tb_quest[questId].type == QUEST_TYPE_DAILY2 then
			self:OnRemoveQuest(start)
		end
	end
	
end


-- 随机生成日常任务
function AppQuestMgr:RandomGenerateDaily2Quest()
	local indx = self:GetDaily2QuestIndx()
	if indx > 0 then
		local config = tb_quest_daily2[indx]
		for i, questSetId in ipairs(config.questSet) do
			local num = config.questSelectNum[ i ]
			if num then
				local questSet = tb_quest_daily2_set[questSetId].questSet
				local rdDict = GetRandomIndexTable(#questSet, num)
				
				for _, indx in ipairs(rdDict) do
					local questId = questSet[indx]
					self:OnAddQuest(questId, QUEST_FIELD_DAILY2_QUEST_START, QUEST_FIELD_DAILY2_QUEST_END)
				end
			end
		end
	end
end

-- 随机生成日常任务
function AppQuestMgr:GetDaily2QuestIndx()
	local playerInfo = self:getOwner()
	local level = playerInfo:GetLevel()
	
	for indx, info in ipairs(tb_quest_daily2) do
		local levelrange = info.levelrange
		if levelrange[ 1 ] <= level and level <= levelrange[ 2 ] then
			return indx
		end
	end
	
	return 0
end

-- 激活下一个关联的任务
function AppQuestMgr:ActiveFlowingQuests(questId)
	local config = tb_quest[questId]
	local playerInfo = self:getOwner()
	
	playerInfo:UnlockModuleByTaskId(questId)
	
	-- 每日任务
	if config.type == QUEST_TYPE_DAILY then
		if config.start == 0 then
			-- 完成数+1
			playerInfo:AddDailyQuestFinished()
		end
		self:OnGetDailyQuestRewards()
		self:RandomGenerateDailyQuest()
		return
	end
	
	-- 日常任务
	if config.type == QUEST_TYPE_DAILY2 then
		if config.start == 1 then
			self:RandomGenerateDaily2Quest()
		end
		return
	end
	
	-- 是否有下一个主线
	if config.nextid > 0 then
		if tb_quest[config.nextid] then
			self:OnAddQuest(config.nextid)
		end
	end
	
	-- 支线任务
	if #config.acitveIds then
		for _, id in pairs(config.acitveIds) do
			if tb_quest[id] then
				self:OnAddQuest(id)
			end
		end
	end
end

-- 领取章节奖励
function AppQuestMgr:OnPickQuestChapterReward(indx)
	local rewards = tb_quest_chapter[indx].items
	local playerInfo = self:getOwner()
	playerInfo:AppdAddItems(rewards, MONEY_CHANGE_QUEST, LOG_ITEM_OPER_TYPE_QUEST)
	--playerInfo:SetQuestChapterPicked(indx)
end

-- 清空日常任务完成个数
function AppQuestMgr:ClearDaily2Finished()
	self:SetUInt32(QUEST_FIELD_DAILY2_FINISHED, 0)
end

-- 获得日常任务完成个数
function AppQuestMgr:GetDaily2Finished()
	return self:GetUInt32(QUEST_FIELD_DAILY2_FINISHED)
end

-- 增加日常任务完成个数
function AppQuestMgr:AddDaily2Finished()
	self:AddUInt32(QUEST_FIELD_DAILY2_FINISHED, 1)
end

-- 清空日常任务提交情况
function AppQuestMgr:ClearDaily2Submit()
	self:SetUInt32(QUEST_FIELD_DAILY2_SUBMIT, 0)
end

-- 设置日常任务已提交
function AppQuestMgr:Daily2Submit()
	self:SetUInt32(QUEST_FIELD_DAILY2_SUBMIT, 1)
end

-- 查询日常任务是否已提交
function AppQuestMgr:IsDaily2Submited()
	return self:GetUInt32(QUEST_FIELD_DAILY2_SUBMIT) > 0
end

function AppQuestMgr:OnExecuteQuestCmdAfterAccepted(indx)
	local start = QUEST_FIELD_QUEST_START + indx * MAX_QUEST_INFO_COUNT
	
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
	local state   = self:GetUInt16(start + QUEST_INFO_ID, 1)
	if questId > 0 and state == QUEST_STATUS_INCOMPLETE then
		local playerInfo = self:getOwner()
		playerInfo:AfterQuestDoing(tb_quest[questId].afterAccept)
	end
end

-- 领取奖励
function AppQuestMgr:OnPickQuest(indx)
	local start = QUEST_FIELD_QUEST_START + indx * MAX_QUEST_INFO_COUNT
	
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
	local state   = self:GetUInt16(start + QUEST_INFO_ID, 1)
	if questId > 0 and state == QUEST_STATUS_COMPLETE then
		self:OnInnerPickQuest(start)
	end
end

-- 领取日常任务
function AppQuestMgr:OnPickDailyQuest(indx)
	local start = QUEST_FIELD_DAILY2_QUEST_START + indx * MAX_QUEST_INFO_COUNT
	
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
	local state   = self:GetUInt16(start + QUEST_INFO_ID, 1)
	if questId > 0 then
		if state == QUEST_STATUS_COMPLETE then
			self:OnInnerPickQuest(start)
		elseif state == QUEST_STATUS_INCOMPLETE then
			self:CheckIfTheTurnItemInQuest(start)
		end
	end
end

-- 上交任务的判断
function AppQuestMgr:CheckIfTheTurnItemInQuest(start)
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
	local playerInfo = self:getOwner()
	
	local config = tb_quest[questId]
	local target = config.targets[ 1 ]
	local targetType = target[ 1 ]
	local entry = target[ 2 ]
	local count = target[ 3 ]
	
	-- 如果不是上交任务返回
	if targetType ~= QUEST_TARGET_TYPE_TURN_ITEM_IN then
		return
	end
	
	-- 扣道具成功
	if playerInfo:useMulItem({{entry, count}}) then
		-- 设置进度信息
		local qtIndx = GetOneQuestTargetStartIndx(start, 0)
		self:SetUInt32(qtIndx + QUEST_TARGET_INFO_PROCESS, count)
		self:SetUInt16(qtIndx + QUEST_TARGET_INFO_SHORT0, 0, 1)
		
		self:OnInnerPickQuest(start)
	end
end

-- 获取日常任务的binlog索引
function AppQuestMgr:GetQuestDaily2Indice()
	local indice = {}
	for start = QUEST_FIELD_DAILY2_QUEST_START, QUEST_FIELD_DAILY2_QUEST_END - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
		if questId > 0 then
			table.insert(indice, start)
		end
	end
	
	return indice
end

-- 提交日常任务
function AppQuestMgr:OnSubmitQuestDaily2()
	-- 已经领取过了, 不需要领取了
	if self:IsDaily2Submited() then
		return
	end
	
	local indice = self:GetQuestDaily2Indice()
	local all = #indice
	if all == 0 then
		return
	end
	
	local belongLvRangeId = 0
	for _, start in ipairs(indice) do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
		--[[
		local state = self:GetUInt16(start + QUEST_INFO_ID, 1)

		if state == QUEST_STATUS_COMPLETE then
			self:OnInnerPickQuest(start)
		elseif state == QUEST_STATUS_INCOMPLETE then
			self:CheckIfTheTurnItemInQuest(start)
		end
		--]]
		-- 所属等级段
		belongLvRangeId = tb_quest[questId].belongLvRangeId
	end
	
	-- 所属区间范围有误
	if not tb_quest_daily2[belongLvRangeId] then
		return
	end
	
	local config = tb_quest_daily2[belongLvRangeId]
	local finished = self:GetDaily2Finished()
	-- 保底完成数不足
	if finished < config.finishQuestsNum[ 1 ] then
		return
	end
	
	self:Daily2Submit()

	local playerInfo = self:getOwner()

	-- 给完成奖励

	if config.rewardsSelect[finished] then
		local rewards = tb_quest_daily2_finish_reward[config.rewardsSelect[finished]].rewards
		playerInfo:AppdAddItems(rewards, MONEY_CHANGE_QUEST, LOG_ITEM_OPER_TYPE_QUEST, 1, 0, ITEM_SHOW_TYPE_MINI_QUEST_DAILY2)
	end
	
	-- 给全部完成奖励
	if finished >= all then
		local rewards = config.allFinishrewards
		playerInfo:AppdAddItems(rewards, MONEY_CHANGE_QUEST, LOG_ITEM_OPER_TYPE_QUEST, 1, 0, ITEM_SHOW_TYPE_MINI_QUEST_DAILY2)
	end
	
	playerInfo:onUpdatePlayerQuest(QUEST_TARGET_TYPE_DAILY_TASK, {})
end

--[[
-- 领取某个日常任务的奖励
function AppQuestMgr:OnPickingDaily2Quest(questId)
	local playerInfo = self:getOwner()
	local questSetId = tb_quest[questId].belongSet
	
	if tb_quest_daily2_set[questSetId] then
		local rewards = tb_quest_daily2_set[questSetId].rewards
		playerInfo:AppdAddItems(rewards, MONEY_CHANGE_QUEST, LOG_ITEM_OPER_TYPE_QUEST, 1, 0, ITEM_SHOW_TYPE_MINI_QUEST_DAILY2)
	end
end
--]]

-- 内部领取奖励
function AppQuestMgr:OnInnerPickQuest(start)
	local playerInfo = self:getOwner()
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)

	-- 设一个已领取状态
	self:OnQuestPicked(start)
	
	-- 日常任务不能删除
	if tb_quest[questId].type == QUEST_TYPE_DAILY2 and tb_quest[questId].start == 0 then
		-- 由于策划保证多个任务会配一样的任务奖励, 那就走任务那套奖励
		--self:OnPickingDaily2Quest(questId)
		-- 未提交任务才需要加计数
		if not self:IsDaily2Submited() then
			self:AddDaily2Finished()
		end
	else
		self:OnRemoveQuest(start)
	end
	
	-- 如果是主线任务 当前主线任务id + 1000000 表示完成
	if tb_quest[questId].type == QUEST_TYPE_MAIN then
		playerInfo:SetMainQuestID(1000000 + questId)
	end
	
	self:ActiveFlowingQuests(questId)
	
	-- 领取奖励
	if #tb_quest[questId].rewards > 0 then
		local gender = playerInfo:GetGender()
		if #tb_quest[questId].rewards == 1 then
			gender = 1
		end
		local rewards = tb_quest[questId].rewards[gender]
		-- 判断背包格子是否足够
		local itemMgr = playerInfo:getItemMgr()
		local emptys  = itemMgr:getEmptyCount(BAG_TYPE_MAIN_BAG)
		if emptys < #rewards then
			playerInfo:CallOptResult(OPRATE_TYPE_BAG, BAG_RESULT_BAG_FULL)
			return
		end
		playerInfo:AppdAddItems(rewards, MONEY_CHANGE_QUEST, LOG_ITEM_OPER_TYPE_QUEST, 1, 0, ITEM_SHOW_TYPE_MINI_QUEST_BAR)
	end
	
	-- 如果是章节最后一个任务 自动领取章节奖励
	if tb_quest[questId].chapterLast == 1 then
		local chapterIndex = tb_quest[questId].chapter
		self:OnPickQuestChapterReward(chapterIndex)
	end
	
	playerInfo:AfterQuestDoing(tb_quest[questId].afterFinish)
end

-- 如果需要初始化进度的
function AppQuestMgr:ProcessInit(start)
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
	local config = tb_quest[questId]
	local targets = config.targets
	local size = math.min(#targets, MAX_QUEST_TARGET_COUNT)
	local playerInfo = self:getOwner()
	
	for i = 1, size do
		local targetInfo = targets[ i ]
		local targetType = targetInfo[ 1 ]
		
		if QUEST_UPDATE_CALLBACK[targetType] then
			local dest = QUEST_UPDATE_CALLBACK[targetType]:GetTargetValue(targetInfo)
			local qtIndx = GetOneQuestTargetStartIndx(start, i-1)
			self:SetUInt16(qtIndx + QUEST_TARGET_INFO_SHORT0, 1, dest)
			
			QUEST_UPDATE_CALLBACK[targetType]:OnInit (playerInfo, start, i-1)
		end
	end
end

-- 升级了判断主线任务是否解锁
function AppQuestMgr:OnCheckMainQuestActive(currLevel)
	for start = QUEST_FIELD_QUEST_START, QUEST_FIELD_QUEST_END - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
		local state   = self:GetUInt16(start + QUEST_INFO_ID, 1)
		if questId > 0 and state == QUEST_STATUS_UNAVAILABLE then
			if tb_quest[questId].level <= currLevel then
				local state = QUEST_STATUS_INCOMPLETE
				self:SetUInt16(start + QUEST_INFO_ID, 1, state)
				self:ProcessInit(start)
				self:CheckQuestFinish(start)
			end
			return
		end
	end
end

-- 增加任务
function AppQuestMgr:OnAddQuest(addQuestId, binlogStart, binlogEnd)
	local playerInfo = self:getOwner()
	binlogStart = binlogStart or QUEST_FIELD_QUEST_START
	binlogEnd = binlogEnd or QUEST_FIELD_QUEST_END
	
	for start = binlogStart, binlogEnd - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)

		if questId == 0 then
			local state = QUEST_STATUS_INCOMPLETE
			if tb_quest[addQuestId].level > playerInfo:GetLevel() then
				state = QUEST_STATUS_UNAVAILABLE
			end
			self:SetUInt16(start + QUEST_INFO_ID, 0, addQuestId)
			self:SetUInt16(start + QUEST_INFO_ID, 1, state)
			if state == QUEST_STATUS_INCOMPLETE then
				self:ProcessInit(start)
				self:CheckQuestFinish(start)
			end
			if tb_quest[addQuestId].type == QUEST_TYPE_MAIN then
				playerInfo:SetMainQuestID(addQuestId)
			end
			
			playerInfo:UpdateGuideIdByTaskId(addQuestId)
			
			playerInfo:AfterQuestDoing(tb_quest[addQuestId].afterAccept)
			return
		end
	end
	
	outFmtDebug("quest max count excceed for add quest %d", addQuestId)
end

-- 通过位置移除任务
function AppQuestMgr:OnQuestPicked(start)
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
	self:SetUInt16(start + QUEST_INFO_ID, 1, QUEST_STATUS_END)
end

-- 通过位置移除任务
function AppQuestMgr:OnRemoveQuest(start)
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)

	for i = 0, MAX_QUEST_INFO_COUNT-1 do
		self:SetUInt32(start + i, 0)
	end
end

-- 选择主线任务
function AppQuestMgr:OnSelectMainQuest(id)
	-- 换的不是主线任务 不让换
	if tb_quest[id].type ~= QUEST_TYPE_MAIN then
		return
	end
	
	-- 删除原来的主线任务
	for start = QUEST_FIELD_QUEST_START, QUEST_FIELD_QUEST_END - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)

		if questId > 0 then
			if tb_quest[questId].type == QUEST_TYPE_MAIN then
				self:OnRemoveQuest(start)
				break
			end
		end
	end
	
	-- 加入新的主线任务
	self:OnAddQuest(id)
end

-- 完成当前主线
function AppQuestMgr:FinishCurrentMainQuest()
	for start = QUEST_FIELD_QUEST_START, QUEST_FIELD_QUEST_END - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
		if questId > 0 then
			if tb_quest[questId].type == QUEST_TYPE_MAIN then
				self:SetUInt16(start + QUEST_INFO_ID, 1, QUEST_STATUS_COMPLETE)
				return
			end
		end
	end
end

-- 遍历任务是否需要更新
function AppQuestMgr:OnUpdate(questTargetType, params)
	-- 普通任务
	self:OnInnerUpdate(questTargetType, params, QUEST_FIELD_QUEST_START, QUEST_FIELD_QUEST_END)
	
	-- 日常任务
	self:OnInnerUpdate(questTargetType, params, QUEST_FIELD_DAILY2_QUEST_START, QUEST_FIELD_DAILY2_QUEST_END)
end

function AppQuestMgr:OnInnerUpdate(questTargetType, params, binlogStart, binlogEnd)
	for start = binlogStart, binlogEnd - 1, MAX_QUEST_INFO_COUNT do
		local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
		local state   = self:GetUInt16(start + QUEST_INFO_ID, 1)

		local config = tb_quest[questId]
		if config and QUEST_STATUS_INCOMPLETE == state then
			local updated = self:CheckQuestUpdate(questTargetType, start, params)
			-- 所有任务是否完成
			if updated then
				self:CheckQuestFinish(start)
			end
		end
	end
end

-- 检查是否更新任务
function AppQuestMgr:CheckQuestUpdate(questTargetType, start, params)
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
	local config = tb_quest[questId]
	local targets = config.targets
	local size = math.min(#targets, MAX_QUEST_TARGET_COUNT)
	local playerInfo = self:getOwner()
	
	local updated = false
	for i = 1, size do
		local target = targets[ i ]
		local targetType = target[ 1 ]
		local qtIndx = GetOneQuestTargetStartIndx(start, i-1)
		-- 未完成的才需要更新
		if targetType == questTargetType and self:GetUInt16(qtIndx + QUEST_TARGET_INFO_SHORT0, 0) == 0 then
			if QUEST_UPDATE_CALLBACK[targetType] then
				if QUEST_UPDATE_CALLBACK[targetType]:OnUpdate(playerInfo, start, i-1, params) then
					updated = true
				end
			end
		end
	end
	
	return updated
end

-- 检查任务是否完成
function AppQuestMgr:CheckQuestFinish(start)
	local questId = self:GetUInt16(start + QUEST_INFO_ID, 0)
	local state   = self:GetUInt16(start + QUEST_INFO_ID, 1)
	local config = tb_quest[questId]
	local targets = config.targets
	local size = math.min(#targets, MAX_QUEST_TARGET_COUNT)
	
	for i = 1, size do
		local qtIndx = GetOneQuestTargetStartIndx(start, i-1)
		if self:GetUInt16(qtIndx + QUEST_TARGET_INFO_SHORT0, 0) == 0 then
			return
		end
	end
	
	-- 不是自动跳过的对话的需要这样
	if --[[targets[ 1 ][ 1 ] ~= QUEST_TARGET_TYPE_TALK and--]] config.popup ~= 0 then
		self:SetUInt16(start + QUEST_INFO_ID, 1, QUEST_STATUS_COMPLETE)
		return
	end
	
	-- 是对话的直接跳到下一步
	self:OnInnerPickQuest(start)
end

-------------------------------上面是任务-------------------------------
-- 获得玩家guid
function AppQuestMgr:getPlayerGuid()
	--物品管理器guid转玩家guid
	if not self.owner_guid then
		self.owner_guid = guidMgr.replace(self:GetGuid(), guidMgr.ObjectTypePlayer)
	end
	return self.owner_guid
end

--获得技能管理器的拥有者
function AppQuestMgr:getOwner()
	local playerguid = self:getPlayerGuid()
	return app.objMgr:getObj(playerguid)
end


return AppQuestMgr