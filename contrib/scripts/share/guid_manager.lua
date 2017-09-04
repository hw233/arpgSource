--GUID管理器
local GuidManager = class('GuidManager', assert(BinLogObject))

--//////////////////////////////////////////////////////////////////////////
--//所有对象的类型枚举
GuidManager.ObjectTypeNone					= ' '	--//无效的前缀
GuidManager.ObjectTypeAccount				= 'A'	--//账户信息
GuidManager.ObjectTypeGlobalValue			= 'G'	--//全局对象
GuidManager.ObjectTypeUnit					= 'U'	--//生物
GuidManager.ObjectTypePlayer				= 'p'	--//玩家
GuidManager.ObjectTypeItemMgr				= 'I'	--//物品

GuidManager.ObjectTypeQuest 				= 'Q'	--//任务
GuidManager.ObjectTypeInstance				= 'C'	--//副本
GuidManager.ObjectTypeFaction				= 'L'	--//帮派
GuidManager.ObjectTypeFactionData 			= 'D'	--//帮派数据
GuidManager.ObjectTypeSpell					= 'X'	--//技能
GuidManager.ObjectTypeLimit					= 'l'	--//限时活动对象
GuidManager.ObjectTypeSocial				= 's'	--//社交
GuidManager.ObjectTypeLogical 				= 'B'	--//业务逻辑
GuidManager.ObjectTypeGiftPacks				= 'g'	--//礼包
GuidManager.ObjectTypeMap					= 'M'	--//地图变量
GuidManager.ObjectTypeMapPlayerInfo			= 'u'	--//任意地图实例信息(目前主要是在线玩家列表),前缀被用光了，这个将就点用吧
GuidManager.ObjectTypeGridLoot				= 'O'	--//战利品对象

--//以下纯粹是为了借用机制，所以搞些符号代替
GuidManager.ObjectTypeMapInstance			= '#'	--//地图实例的id规则
GuidManager.ObjectTypeGMCommand				= '@'	--//GM命令产生

-------------------------------------------------
--//关于binlog数量大小类型枚举
GuidManager.MAX_BINLOG_SIZE_1		= 100
GuidManager.MAX_BINLOG_SIZE_2		= 1000
GuidManager.MAX_BINLOG_SIZE_3		= 3000
GuidManager.MAX_BINLOG_SIZE_4		= 5000
GuidManager.MAX_BINLOG_SIZE_5		= 10000
GuidManager.MAX_BINLOG_SIZE_UNLIME	= 999999999	--//给个大数，怎么也到不了了吧
--//

GuidManager.ObjectTypeCount = {}

function GuidManager:ctor( )
	--初始化
	for i = 0,127 do
		self.ObjectTypeCount[i] = self:GetUInt32(i)
	end
end

--产生新的GUID
function GuidManager:Make_New_Guid(t,index,suffix)
	if(not index)then
		index = self:NewIndex(t)
	end
	if(suffix)then
		return string.format("%s%d.%s",t,index,suffix)
	else
		return string.format("%s%d",t,index)
	end
end

--新增标号
function GuidManager:NewIndex(t)
	local index = string.byte(t)
	self:AddUInt32(index,1)
	self.ObjectTypeCount[index] = self.ObjectTypeCount[index] + 1
	return self.ObjectTypeCount[index]
end

--返回GUID前缀
function GuidManager.get( guid )
	if guid == '' or guid == nil then
		return ''
	else
		return string.sub(guid, 1, 1)
	end
end

--返回GUID后缀
function GuidManager.getSuffix( guid )
	error('未实行')
end

--测试类型
local function test_findType( ... )
	local guid,guid2 = '1234.linbc.dahei.xm', '324.qihei.com'
	local t = {
		['linbc'] = {
			['dahei'] = class('abc'),
			['niu'] = class('bcd')
		},
		['qihei'] = class('cvb'),
	}
	assert(GuidManager.findType(t, guid).__cname == 'abc')
end

function GuidManager.findType( types, guid )
	--以.作切割
	local ar = {}
	for k,_ in guid:gmatch('[%w_]+') do
		table.insert(ar,k)
	end
	--忽略掉第一个类型信息如：z2.fengliuzheng.
	local i, len = 2, #ar
	while i <= len do
		local t = types[ ar[i] ]
		--print(t, ar[i])
		if t == nil then
			return nil
		end
		if type(t) == 'table' and t.__cname then
			return t
		else
			--进入下一个子分类
			i = i + 1
			types = t
		end
	end

	return nil
end

function GuidManager.replace( guid, t )
	assert(guid)
	assert(t)
	if (guid == nil or string.len(guid) == 0) then
		return ""
	end
	return t .. string.sub(guid, 2, string.len(guid))
end

return GuidManager
