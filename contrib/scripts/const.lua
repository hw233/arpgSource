----------------------------------------------------------------------
--继承
function __inherit__(sub,super)	
	if(super == nil)then
		outDebug("----------------------------------------")
		outDebug("LUA ERROR: ")
		outDebug(debug.traceback())
		outDebug("----------------------------------------")
	end
	setmetatable(sub,{ __index=function(t,k)
			sub[k]=super[k]
            return super[k]
	end })
end

--如果这几个函数都不存在，没办法检查语法
if(outString == nil) then
	--outString=print
end
if(getGlobalValue == nil) then
	getGlobalValue = loadstring('return nil')
end
if(__SCRIPTS_ROOT__ == nil) then
	__SCRIPTS_ROOT__ = './'
end

outString('开始载入枚举脚本')
require("template/SharedDef")
--------------------------------------------------------------------
--针对每个服务器都需要生成一些不同的配置
function dofile_config()
	--先检测一下脚本根目录下面的
	local lua_config_file = __SCRIPTS_ROOT__.."config.lua"
	local file = io.open(lua_config_file,"r")
	if(file == nil) then
		file = io.open(lua_config_file,"a")
		file:write("wulongju_test = true\n")
		file:write("post_chat = false\n")
		file:write("changan_robot_open = false\n")
		file:write("max_player_count = 10000\n")
	end
	file:close()
	dofile(lua_config_file)
end
dofile_config()

require('template.internal')

-- 服务器连接信息下标
-- 前面16位是连接次数，后面16位是成功连接次数
SERVER_CONN_LIST_FIELD_CONN_NUM = 0	-- 各种服务器连接次数，先预留个20种服务器吧
SERVER_CONN_LIST_FIELD_SERVER_INFO_START = 20	-- 服务器信息开始下标

SCENED_COUNT = 2			--场景服个数

--服务器信息枚举
SERVER_CONN_INFO_ID				= 0	--连接ID
SERVER_CONN_INFO_TYPE			= 1	--服务器类型
SERVER_CONN_INFO_LINENO			= 2	--线路
SERVER_CONN_INFO_COUNT			= 3	--玩家数量
SERVER_CONN_INFO_PING_PONG_MS	= 4	--响应时间
SERVER_CONN_INFO_SCENED_TYPE	= 5	--场景服分类 0)替补地图 1)普通地图 2)活动地图 3)副本地图；仅对场景服有效
SERVER_CONN_INFO_PID			= 6	--远程的进程号
SERVER_CONN_INFO_FLAG			= 7	--一些标志
MAX_SERVER_CONN_INFO = 8
--服务器标志位，准备好了
SERVER_CONN_INFO_FLAG_READY_OK = 0
--场景服类型
SCENED_TYPE_NONE = 0
SCENED_TYPE_NO_INST = 1
SCENED_TYPE_ACTIVI = 2
SCENED_TYPE_INST = 3
MAX_SCENED_TYPE = 4

-- web服公共密钥
WEB_SERVER_AUTH_SIGN = "c9cfac2dbbacf4f55f8411e463eecfba"
WEB_SEND_LOCK_TIME = 2				-- web协议发送锁定时间2s
--地图副本类型
MAP_INST_TYP_NO_INSTANCE	= 0		--不是副本
MAP_INST_TYP_ACTIVITY		= 1		--活动副本,虽是副本地图,但是,只有单例,活动时间自动关闭
MAP_INST_TYP_SINGLETON		= 2		--单人副本，副本完成直接关闭
MAP_INST_TYP_KUAFU			= 3		--跨服地图

DELETE_GENERALS_ID = 9999
MAX_UINT32_NUM = 4294967295			--32位最大值
MAX_UINT16_NUM = 65535				--16位最大值
MAX_UINT8_NUM = 255					--8位最大值

outString('load util.BinlogObject script')
BinLogObject = require("util.binlog_object")

outString('load OprateResult script')
require("template/OprateResult")

outString('load log_handler script')
require("log_handler")

outString('load share/gm_command_key script')
require("share.gm_command_key")

require('share.protocols')

--加载语言版配置
if(lang ~= nil)then
	outString('load gamestring_'..lang)
	require("lang/game_string_"..lang)
else
	outString('load gamestring_CN')
	require("lang/game_string_CN")
end

-----------------------------------------------------------------------
MAX_BAG_MAIN_SIZE_EXTENSION = 140		--背包能扩展的最大
RESET_HOSTING_APPLY	= 0	--托管申请
RESET_HOSTING_HOSTING =1	--托管中

TIME_ZONE_HOUR = 8		--时区 todo目前写死


KUAFU_MAP_ID_CONFIG = {}
--根据地图id判断所在地图是否为跨服地图
function IsKuafuMapID(map_id)
	for k,val in pairs(KUAFU_MAP_ID_CONFIG) do
		if map_id == val then
			return true
		end
	end
	return false
end

--是否在世界地图
function IsWorldMap(mapid)
	return tb_map[mapid].inst_type == 0
end

-- 判断这个副本的分线号 是不是常驻分线
function IsMapLineStable(mapid, lineNo)
	return IsWorldMap(mapid) and 1 <= lineNo and lineNo <= MAX_DEFAULT_LINE_COUNT 
end

--数值表字符切割
function number_table_string_split(str, split_char) 
	local sub_str_tab = {}
	if str == "" then
		return sub_str_tab
	end
	local temp
	while (true) do
		local pos = string.find(str, split_char)
		if (not pos) then
			temp = tonumber(str)
			if(temp ~= nil)then
				table.insert(sub_str_tab, temp)
			end
			break
		end
		local sub_str = string.sub(str, 1, pos - 1)
		temp = tonumber(sub_str)
		if(temp ~= nil)then
			table.insert(sub_str_tab, temp)
		end
		str = string.sub(str, pos + 1, #str)
	end
	return sub_str_tab
end

--获取下一次时间点时间戳
function GetNextTimeStamp(h)
	local time_table = os.date("*t")
	time_table.hour = h
	time_table.min = 0
	time_table.sec = 0
	local reset_time = os.time(time_table)
	return reset_time
end

--获取下个月1号0点整
function GetNextTimeStampByMonths()
	local time_table = os.date("*t")
	local month = time_table.month
	time_table.month = month + 1
	time_table.day = 1
	time_table.hour = 0
	time_table.min = 0
	time_table.sec = 0
	local reset_time = os.time(time_table)
	return reset_time
end

function outFmtDebug(...)
    return outDebug(string.format(...))
end

-- cclog
function outFmtError(...)
    return outError(string.format(...))
end

function outFmtInfo(...)
    return outString(string.format(...))
end

--切割字符
function lua_string_split(str, split_char) 
   local sub_str_tab = {};   
   while (true) do      
		local pos = string.find(str, split_char);      
		if (not pos) then           
			sub_str_tab[#sub_str_tab + 1] = str;          
			break;        
		end      
		local sub_str = string.sub(str, 1, pos - 1);       
		sub_str_tab[#sub_str_tab + 1] = sub_str;       
		str = string.sub(str, pos + 1, #str); 
   end   
   return sub_str_tab;
 end

--检查协议路由表是否有空协议
function CheckRouterMap(router_map)
	local before = -1
	local beforeIndx = -1
	for i = 1, #router_map do
		local v = router_map[i]
		if(v == nil)then
			outString("CheckRouterMap index " .. i .. " is nil, before value is " .. before .. " beforeIndx is ".. beforeIndx)
			assert(false)
		end
		before = v
		beforeIndx = i
	end
end

-- 获取本月的日期几月（返回201603）
function GetMonthNum(val)
	if not val then val = os.time() end
	return tonumber(os.date("%Y%m", val))
end

-- 获取第几周（返回1）
function GetWeeklyCount(val)
	if not val then val = os.time() end
	val = val - 1457193599		-- 03/06/15 23:59:59（作为起始时间（周日-周六为一周））
	assert(val > 0)
	return math.ceil(val/604800)
end

-- 获取本周周日的日期几周（返回20160306）（周日-周六为一周）
function GetWeeklyNum(val)
	if not val then val = os.time() end
	local cur_tb = os.date("*t", val)
	local old_tm = val - (cur_tb.wday - 1)*86400
	return tonumber(os.date("%Y%m%d", old_tm))
end


CHAT_MSG_FILTER_SING0 = '{'
CHAT_MSG_FILTER_SING1 = '}'
GM_SING = "{GM}"
ZB_SING = "{ZB}"

--//过滤聊天字符串
function ChatMsgFilter(str)
	if(str == "" or str == nil)then return "" end
	local strlen = string.len(str)
	local start_pos = string.find(str, CHAT_MSG_FILTER_SING0)
	if string.sub(str, strlen, strlen) == CHAT_MSG_FILTER_SING1 and start_pos then
		return string.sub(str, start_pos+1, strlen-1)
	end
	return str
end

--//添加特殊标示
function ChatMsgAddSing(str, falseGmLevel, girlGmLevel)
	if(str == "" or str == nil)then return "" end
	if falseGmLevel > 0 then
		str = str..GM_SING
	elseif girlGmLevel == 1 then
		str = str..ZB_SING
	end
	return str
end

POSITION_REGEX = "{T=1}"
SUIT_REGEX = "{T=2,G=\"(%a%d+%.%w+)\"}"

CHAT_REGEX_INFO = {
	[ 1 ] = {POSITION_REGEX, ""},
	[ 2 ] = {SUIT_REGEX, "%1"},
}

-- 聊天转义解析
function ChatMsgParser(playerInfo, str, forbidden)
	local sort = {}
	for T, regexInfo in pairs(CHAT_REGEX_INFO) do
		local ret = DoFind(str, regexInfo[ 1 ], regexInfo[ 2 ])
		-- 某一段
		for _, seq in pairs(ret) do
			local a = seq[ 1 ]
			local b = seq[ 2 ]
			local s = seq[ 3 ]
			table.insert(sort, {T = T, a = a, b = b, s = s})
		end
	end
	
	if #sort == 0 then
		return true, str
	end
	
	-- 当前频道不能发送转义内容
	if forbidden then
		return false, ""
	end
	
	-- 从小达到排序
	table.sort(sort, function(a, b) return a.a < b.a end)
	
	-- 如果有正则匹配有交叉, 从头开始取合法的
	local tmp = sort[ 1 ]
	local valid = {tmp}
	for i = 2, #sort do
		local ele = sort[ i ]
		if tmp.b >= ele.a then
			sort[ i ] = nil
		else
			tmp = ele
			table.insert(valid, ele)
		end
	end
	
	-- 进行替换
	local st = 1
	local chatmsg = ""
	for i = 1, #valid do
		ed = valid[ i ].a
		if st < ed then
			chatmsg = chatmsg..string.sub(str, st, ed-1)
		end
		-----------这里进行解析----------
		chatmsg = chatmsg..msgRealContent(playerInfo, valid[ i ]);
		---------------------
		st = valid[ i ].b+1
	end
	
	if st < #str then
		chatmsg = chatmsg..string.sub(str, st, #str)
	end
	
	return true, chatmsg
end


-- {T = T, a = a, b = b, s = s}
function msgRealContent(playerInfo, info)
	-- TODO: 进行转义
	if info.T == TEXT_TYPE_POSITION then
		-- {\"T\":{T},\"M\":{M},\"L\":{L},\"X\":{X},\"Y\":{Y}}
		local str = tb_msg_text_type[info.T].content
		str = string.gsub(str, "{T}", info.T)
		str = string.gsub(str, "{M}", playerInfo:GetMapId())
		str = string.gsub(str, "{L}", 1)
		local x, y = playerInfo:GetPosition ()
		str = string.gsub(str, "{X}", x)
		str = string.gsub(str, "{Y}", y)
		
		return str
	end
	return ""
end

SYSTEM_NAME = '系统'