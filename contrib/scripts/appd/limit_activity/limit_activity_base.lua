--限时活动封装
LimitActivityBase = class('LimitActivityBase', BinLogObject)

--//////////////////////////////////////////////////////////////////
--//限时活动领取类型枚举
LIMIT_ACTIVITY_CONFIG_MONEY			= 1		--钱
LIMIT_ACTIVITY_CONFIG_ITEM			= 2		--物品
LIMIT_ACTIVITY_CONFIG_NUMBER		= 3		--可领取次数
LIMIT_ACTIVITY_CONFIG_BAG_EMPTY		= 4		--需求背包空格
LIMIT_ACTIVITY_CONFIG_LEVEL			= 5		--需求等级
LIMIT_ACTIVITY_CONFIG_RECHARGE		= 6		--累计充值

--//////////////////////////////////////////////////////////////////
--//限时活动下标
LimitActivityBase.INT_FIELD_VERSION						= 0		--活动版本号
LimitActivityBase.INT_FIELD_BEGIN						= 1		--活动开始时间
LimitActivityBase.INT_FIELD_END							= 2		--活动结束时间
LimitActivityBase.INT_FIELD_VERSION_NUM					= 3		--客户端活动版本号
LimitActivityBase.INT_FIELD_RECHARGE_SUM				= 4		--活动充值总数
LimitActivityBase.INT_FIELD_RECHARGE_TIME				= 5		--最后充值时间
	
LimitActivityBase.INT_FILED_BIT							= 80	--setBit,getBit,用来保存一次性的东西
LimitActivityBase.INT_FILED_BIT_MAX						= 87	--给8个uint32,这样就ok了，最大的bit下标是255

LimitActivityBase.INT_FIELD_REWARD_COUNT				= 100	--活动领取下标开始

LimitActivityBase.STRING_FIELD_RECHARGE_ID				= 4		--最后充值ID
LimitActivityBase.STRING_FIELD_CLIENT_CONFIG			= 5		--客户端配置

--//////////////////////////////////////////////////////////////////
--//限时活动下标
LimitActivityBase.Name = 'LimitActivityBase'		--活动类名
LimitActivityBase.Version = 0		--活动版本号
LimitActivityBase.BeginTime = 0		--开始时间
LimitActivityBase.EndTime = 0		--结束时间
LimitActivityBase.ClientConfigVersion = 1		--客户端用的判断规则版本号
LimitActivityBase.ClientConfig = ''		--客户端用的判断规则
LimitActivityBase.ResetDailyTable = {}		--需要每日重置的下标
LimitActivityBase.ReceiveTable = {}		--领取规则


-- 获取活动版本
function LimitActivityBase:GetActivityVersion()
	return self:GetUInt32(self.INT_FIELD_VERSION)
end
-- 设置活动版本
function LimitActivityBase:SetActivityVersion(val)
	self:SetUInt32(self.INT_FIELD_VERSION, val)
end

-- 获取活动开始时间
function LimitActivityBase:GetActivityStartTime()
	return self:GetUInt32(self.INT_FIELD_BEGIN)
end
-- 设置活动开始时间
function LimitActivityBase:SetActivityStartTime(val)
	self:SetUInt32(self.INT_FIELD_BEGIN, val)
end

-- 获取活动结束时间
function LimitActivityBase:GetActivityEndTime()
	return self:GetUInt32(self.INT_FIELD_END)
end
-- 设置活动结束时间
function LimitActivityBase:SetActivityEndTime(val)
	self:SetUInt32(self.INT_FIELD_END, val)
end

-- 获取活动充值数
function LimitActivityBase:GetActivityRechargeSum()
	return self:GetUInt32(self.INT_FIELD_RECHARGE_SUM)
end
-- 设置活动充值数
function LimitActivityBase:SetActivityRechargeSum(val)
	self:SetUInt32(self.INT_FIELD_RECHARGE_SUM, val)
end
-- 添加活动充值数
function LimitActivityBase:AddActivityRechargeSum(val)
	self:AddUInt32(self.INT_FIELD_RECHARGE_SUM, val)
end

-- 获取活动充值时间
function LimitActivityBase:GetActivityRechargeTime()
	return self:GetUInt32(self.INT_FIELD_RECHARGE_TIME)
end
-- 设置活动充值时间
function LimitActivityBase:SetActivityRechargeTime(val)
	self:SetUInt32(self.INT_FIELD_RECHARGE_TIME, val)
end
-- 添加活动充值时间
function LimitActivityBase:AddActivityRechargeTime(val)
	self:AddUInt32(self.INT_FIELD_RECHARGE_TIME, val)
end

--专门为初始化活动的时候，设置活动BUFF定制
function LimitActivityBase:SetActivityBuffBit(buff, offset)
	if(buff)then
		self:SetBit(self.INT_FILED_BIT, offset)
	else
		self:UnSetBit(self.INT_FILED_BIT, offset)
	end
	return offset + 1
end

-- 获取活动BUFF
function LimitActivityBase:GetActivityBuffBit(buff)
	return self:GetBit(self.INT_FILED_BIT, buff)
end

-- 获取活动奖励次数
function LimitActivityBase:GetActivityAwardCount(offset)
	return self:GetUInt32(self.INT_FIELD_REWARD_COUNT + offset)
end
-- 设置活动奖励次数
function LimitActivityBase:SetActivityAwardCount(offset, val)
	self:SetUInt32(self.INT_FIELD_REWARD_COUNT + offset, val)
end
-- 添加活动奖励次数
function LimitActivityBase:AddActivityAwardCount(offset, val)
	self:AddUInt32(self.INT_FIELD_REWARD_COUNT + offset, val)
end

-- 获取客户端活动版本
function LimitActivityBase:GetClientActivityVersion()
	return self:GetUInt32(self.INT_FIELD_VERSION_NUM)
end
-- 设置客户端活动版本
function LimitActivityBase:SetClientActivityVersion(val)
	self:SetUInt32(self.INT_FIELD_VERSION_NUM, val)
end

-- 获取活动充值Id
function LimitActivityBase:GetActivityRechargeId()
	return self:GetStr(self.STRING_FIELD_RECHARGE_ID)
end
-- 设置活动充值Id
function LimitActivityBase:SetActivityRechargeId(val)
	self:SetStr(self.STRING_FIELD_RECHARGE_ID, val)
end

-- 获取客户端活动配置
function LimitActivityBase:GetClientActivityConfig()
	return self:GetStr(self.STRING_FIELD_CLIENT_CONFIG)
end
-- 设置客户端活动配置
function LimitActivityBase:SetClientActivityConfig(val)
	self:SetStr(self.STRING_FIELD_CLIENT_CONFIG, val)
end



-- 崩掉给打印下
function LimitActivityBase:Beng(msg)
	outError(msg)
	outError(string.format('活动类名 ：%s',self.Name))
end

--检查子类是否有弄错
function LimitActivityBase:CheckClass()
	--windows下崩掉, linux下面有防错，不会蹦
	if(self.Version == nil or self.Version == 0)then
		self:Beng('限时活动版本号错误')
	end
	if(self.Name == nil or self.Name == '' or self.Name == 'LimitActivityBase')then
		self:Beng('限时活动类名错误')
	end
	if(self.BeginTime == nil or self.BeginTime == 0)then
		self:Beng('限时活动没找到合适的开始时间配置')
	end
	if(self.EndTime == nil or self.EndTime == 0)then
		self:Beng('限时活动没找到合适的结束时间配置')
	end
end

--是否在活动期间
function LimitActivityBase:Between(t)
	if(t == nil)then t = os.time() end
	return t > self.BeginTime and t < self.EndTime
end

-- 登陆触发
function LimitActivityBase:OnLogin(player)

end

--玩家充值以后调用的方法
function LimitActivityBase:OnRecharge(player, amount, recharge_id, t)
	if(self:Between(t) and t > self:GetActivityRechargeTime())then
		self:AddActivityRechargeSum(amount)
		self:SetActivityRechargeTime(t)
		self:SetActivityRechargeId(recharge_id)
		return true
	end
	return false
end

--消费元宝后调用下
function LimitActivityBase:OnConsumption(player,money_type,val)

end

--每日重置
function LimitActivityBase:OnResetDaily(player)
	--每日重置的下标
	for k,v in pairs(self.ResetDailyTable) do
		if(self:GetActivityAwardCount(v) ~= 0)then
			self:SetActivityAwardCount(v, 0)
		end
	end
	
end

--初始化活动
function LimitActivityBase:InitFunc(player)
	--根据时间判断活动是不是过期了
	if(self:GetActivityEndTime() ~= globalValue:GetLimitActivityEndTime())then
		if(self:GetActivityVersion() ~= self.Version)then
			self:SetActivityVersion(self.Version)
		end
		if(self:GetActivityStartTime() ~= self.BeginTime)then
			self:SetActivityStartTime(self.BeginTime)
		end
		if(self:GetActivityEndTime() ~= self.EndTime)then
			self:SetActivityEndTime(self.EndTime)
		end
		if(self:GetClientActivityVersion() ~= self.ClientConfigVersion)then
			self:SetClientActivityVersion(self.ClientConfigVersion)
		end
		if(self:GetClientActivityConfig() ~= self.ClientConfig)then
			self:SetClientActivityConfig(self.ClientConfig)
		end
		if(self:GetActivityRechargeId() ~= "")then
			self:SetActivityRechargeId("")
		end
	end
	--下面处理buff
	
end

-- 限时活动领取
function LimitActivityBase:ReceiveFunc(player, gift_id, gift_type)
	if(gift_type == 0)then
		--非法包，没有这个领取方式
		outString(string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u'
					, player:GetName(), player:GetGuid(), gift_id, gift_type))
		return false
	end
	local rt = self.ReceiveTable[gift_id]
	if(rt == nil)then
		--非法包，没有这个领取方式
		outString(string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: gift_id is nil'
					, player:GetName(), player:GetGuid(), gift_id, gift_type))
		return false
	end
	local check = true
	if(rt.func == nil)then
		if(rt.condition == nil)then
			self:Beng('limit activity is error，receive fun is nil,condition is nil, id:'..gift_id..',type:'..gift_type)
			return false
		end
		--礼包内容
		local gift_tab = rt[0]
		--是否批量
		local batch = gift_tab ~= nil
		local rece_count = 1
		--如果不是批量，那就取真正的类型吧
		if(batch)then
			rece_count = gift_type
		else
			gift_tab = rt[gift_type]
		end
		if(gift_tab == nil)then
			outString(string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: gift_type is no'
						, player:GetName(), player:GetGuid(), gift_id, gift_type))
			return false
		end
		
		--先取已领取次数
		local count
		count = self:GetActivityAwardCount(gift_id)
		--加上本次的领取次数
		count = count + rece_count
		
		--校验条件
		local error_str = ''
		for i = 1, #rt.condition do
			local condition = rt.condition[i]
			if(condition[1] == LIMIT_ACTIVITY_CONFIG_MONEY)then			--扣钱
				if(player:GetMoney(condition[2]) < condition[3] * rece_count)then
					error_str = string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: ,money Not enough'
						, player:GetName(), player:GetGuid(), gift_id, gift_type)
					check = false
				end
			elseif(condition[1] == LIMIT_ACTIVITY_CONFIG_ITEM)then		--扣物品
				local check_sai_ok = false
				for sai = 2, #condition, 2 do
					if(player:CountItem(condition[sai]) < condition[sai + 1] * rece_count)then
						check_sai_ok = true
						break
					end
				end
				if(check_sai_ok)then
					error_str = string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: ,item Not enough'
						, player:GetName(), player:GetGuid(), gift_id, gift_type)
					check = false
				end
			elseif(condition[1] == LIMIT_ACTIVITY_CONFIG_NUMBER)then		--可领取次数
				if(condition[2] ~= 0 and count > condition[2])then
					error_str = string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: ,receive count Not enough'
						, player:GetName(), player:GetGuid(), gift_id, gift_type)
					check = false
				end
			elseif(condition[1] == LIMIT_ACTIVITY_CONFIG_LEVEL)then		--需求等级
				if(player:GetLevel() < condition[2])then
					error_str = string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: level Not enough'
						, player:GetName(), player:GetGuid(), gift_id, gift_type)
					check = false
				end
			elseif(condition[1] == LIMIT_ACTIVITY_CONFIG_BAG_EMPTY)then	--需求背包空格
				local itemMgr = player:getItemMgr( )
				if(gift_type == condition[2] and itemMgr:getEmptyCount() < condition[3])then
					error_str = string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: bag empty Not enough'
						, player:GetName(), player:GetGuid(), gift_id, gift_type)
					check = false
				end
			elseif(condition[1] == LIMIT_ACTIVITY_CONFIG_RECHARGE)then	--累计充值
				if(self:GetActivityRechargeSum() < condition[2])then
					error_str = string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: recharge no enough'
						, player:GetName(), player:GetGuid(), gift_id, gift_type)
					check = false
				end
			end
		end

		if(check == false)then
			outString(error_str)
			return false
		end
		--扣钱扣东西,check再次发挥作用，当然只是为了以防万一
		for i = 1, #rt.condition do
			local condition = rt.condition[i]
			if(condition[1] == LIMIT_ACTIVITY_CONFIG_MONEY)then			--扣钱
				if(player:SubMoney(condition[2],MONEY_CHANGE_ACTIVITY_BUY,condition[3] * rece_count) == false)then
					error_str = string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: submoney is error'
						, player:GetName(), player:GetGuid(), gift_id, gift_type)
					check = false
				end
			elseif(condition[1] == LIMIT_ACTIVITY_CONFIG_ITEM)then		--扣物品
				local check_sai_ok = true
				local itemMgr = player:getItemMgr()
				for sai = 2, #condition, 2 do
					if(itemMgr:delItem(condition[sai], condition[sai + 1] * rece_count) == SUB_ITEM_FAIL)then
						check_sai_ok = false
						break
					end
				end
				if(check_sai_ok == false)then
					error_str = string.format('limit activity player name :{%s},guid:{%s} receive error，gift_id: %u gift_type is %u: delitem is error entry is %u, count is %u'
						, player:GetName(), player:GetGuid(), gift_id, gift_type, condition[2], condition[3] * rece_count)
					check = false
				end
			end
		end

		--不通过，打印一下错误，退出
		if(check == false)then
			outString(error_str)
			return fasle
		end
		--添加次数
		self:AddActivityAwardCount(gift_id,rece_count)
		--送钱送东西
		for i = 1, #gift_tab do
			local gift = gift_tab[i]
			if(gift[1] == LIMIT_ACTIVITY_CONFIG_MONEY)then --送钱
				player:AddMoney(gift[2],MONEY_CHANGE_ACTIVITY,gift[3] * rece_count)
			elseif(gift[1] == LIMIT_ACTIVITY_CONFIG_ITEM)then		--送东西
				if(#gift < 9)then
					self:Beng('崩了')
				end
				local itemMgr = player:getItemMgr()
				itemMgr:addItem(gift[2], gift[3] * rece_count, gift[4], gift[6], gift[7], gift[8], gift[9](self))						
				WriteItemLog(player, LOG_ITEM_OPER_TYPE_ACTIVITY, gift[2], gift[3] * rece_count, gift[4])
			end
		end

		--最后，调一个函数
		if(rt.after_fun ~= nil)then
			rt.after_fun(self, player, gift_id, gift_type)
		end
	elseif(rt.func ~= nil)then
		local count = self:GetActivityAwardCount(gift_id)
		local bOK, num = rt.func(self, player, gift_id, gift_type, count)
		
		if (bOK == true and num > count) then
			self:SetActivityAwardCount(gift_id, num)
		end
		--最后，调一个函数
		if(rt.after_fun ~= nil)then
			rt.after_fun(self, player, gift_id, gift_type)
		end
	end
end

--获取今日限时活动对应的table
function GetToDayLimitActivity(version)
	if(version == nil)then	--不是指定版本的话，去世界变量下取
		version = globalValue:GetLimitActivityVersion()
	end
	local la = OPERATORS_ACTIVITY_VERSION_CONFIG[version]
	if(la and la.Class and la.Class:Between())then
		return la.Class
	end
	return nil
end

--初始化一些活动配置
function InitLimitActivityConfig()
	outString('InitLimitActivityConfig begin')
	local time_tab = os.date('*t')
	time_tab.hour = 0
	time_tab.min = 0
	time_tab.sec = 0
	--解析上结束时间
	for k,v in pairs(OPERATORS_ACTIVITY_VERSION_CONFIG) do
		if(v ~= nil and v.Class ~= nil)then
			outString('limit_'..v.Version..' parsing begin')
			--赋值
			if(v.BeginTime ~= nil)then
				time_tab.year = math.floor(v.BeginTime / 10000)
				time_tab.month = math.floor(math.fmod(v.BeginTime, 10000) / 100)
				time_tab.day = math.fmod(v.BeginTime, 100)
			end
			v.Class.BeginTime = os.time(time_tab)
			v.Class.EndTime = v.Class.BeginTime + (v.Duration * 86400 - 1)
			--校验
			v.Class:CheckClass()
			--设置一下下标
			local client_config_str = nil
			local max_i = table.maxn(v.Class.ReceiveTable)
			if(max_i >= 0)then
				for j = 0, max_i do		--直接下标顺序来循环，避免不连续的循环，顺序混乱就惨了。
					local rt = v.Class.ReceiveTable[j]
					assert(rt)	--默认不会有空下标
					--不是自定义函数的，就补一下条件
					if(rt.func == nil)then
						--没有填次数的，认为是一次
						local number_config = false
						for x = 1, #rt.condition do
							if(rt.condition[x][1] == LIMIT_ACTIVITY_CONFIG_NUMBER)then
								assert(number_config == false)
								number_config = true
							end
						end
						if(number_config == false)then
							rt.condition[#rt.condition + 1] = {LIMIT_ACTIVITY_CONFIG_NUMBER, 1}
						end
						
						--解析背包空格需求
						if(rt[0] == nil)then
							local max_x = table.maxn(rt)
							for x = 1, max_x do
								local emptypos = 0
								for y = 1, #rt[x] do
									if(rt[x][y][1] == LIMIT_ACTIVITY_CONFIG_ITEM and tb_item_template[rt[x][y][2]])then	-- 送物品
										emptypos = emptypos + math.ceil(rt[x][y][3] / tb_item_template[rt[x][y][2]].max_overlap)
									end
								end
								if(emptypos > 0)then
									table.insert(rt.condition, {LIMIT_ACTIVITY_CONFIG_BAG_EMPTY, x, emptypos})
								end
							end
						else
							assert(rt[1] == nil)
							local emptypos = 0
							for y = 1, #rt[0] do
								if(rt[0][y][1] == LIMIT_ACTIVITY_CONFIG_ITEM and tb_item_template[rt[0][y][2]])then
									emptypos = emptypos + math.ceil(rt[0][y][3] / tb_item_template[rt[0][y][2]].max_overlap)
								end
							end
							if(emptypos > 0)then
								table.insert(rt.condition, {LIMIT_ACTIVITY_CONFIG_BAG_EMPTY, 0, emptypos})
							end
						end
					end
					
					--解析给客户端用的字符串
					--当是自定义函数类型的时候，会只有一条数据，所以后面不要跟-符号
					if(client_config_str == nil)then
						if(rt.func == nil)then
							client_config_str = string.format('%u-', j)
						else
							client_config_str = string.format('%u', j)
						end
					else
						if(rt.func == nil)then
							client_config_str = string.format('%s;%u-', client_config_str, j)
						else
							client_config_str = string.format('%s;%u', client_config_str, j)
						end
					end
					--没有函数的，就得加条件了
					if(rt.func == nil)then
						for x = 1, #rt.condition do
							client_config_str = string.format('%s%s', client_config_str, table.concat(rt.condition[x], ','))
							if(x ~= #rt.condition)then
								client_config_str = string.format('%s-', client_config_str)
							end
						end
					end
				end
			end
			v.Class.ClientConfig = client_config_str
			outString('limit_'..v.Version..' parsing end')
		end
	end
end

--获取今日限时活动的版本号
function GetTodayLimitActivityVersion()
	local version
	local is_new_server = false
	for k,v in pairs(OPERATORS_ACTIVITY_VERSION_CONFIG) do
		if(v.Class:Between())then
			version = k
			break
		end
	end
	local la = GetToDayLimitActivity(version)
	if(la == nil or is_new_server == true)then
		if(globalValue:GetLimitActivityVersion() ~= 0)then
			globalValue:SetLimitActivityVersion(0)
		end
		if(globalValue:GetLimitActivityEndTime() ~= 0)then
			globalValue:SetLimitActivityEndTime(0)
		end
		if(globalValue:GetLimitActivityScript() ~= '')then
			globalValue:SetLimitActivityScript('')
		end
		return 0, ''
	else
		if(globalValue:GetLimitActivityVersion() ~= la.Version)then
			globalValue:SetLimitActivityVersion(la.Version)
		end
		if(globalValue:GetLimitActivityEndTime() ~= la.EndTime)then
			globalValue:SetLimitActivityEndTime(la.EndTime)
		end
		if(globalValue:GetLimitActivityScript() ~= la.Name)then
			globalValue:SetLimitActivityScript(la.Name)
		end
		outDebug(string.format("===========today limit is %u",la.Version))
		return la.Version, la.Name
	end
end

--运营活动相关
OPERATORS_ACTIVITY_VERSION_CONFIG = {
	[1] = {
		--版本
		Version = 1,
		--持续天数
		Duration = 2,
		BeginTime = 20160317,
	},

}

--加载活动脚本
function LoadingLimitActivityFile()
	for k,v in pairs(OPERATORS_ACTIVITY_VERSION_CONFIG) do
		v.Class = require('appd/limit_activity/limit_activity_'.. v.Version)
		outString('load file '..v.Class.Name.. ' ok')
	end
end

LoadingLimitActivityFile()


return LimitActivityBase