outString('load lua script')
--先载入一些常量
require("const")
outString(__SCRIPTS_ROOT__..'tea_centd.lua')
outString(__LOG_FOLDER__)
outString(__ETC_FOLDER__)
outString(__BIN_FOLDER__)

outString('load share/serverconnlist')
ServerConnList = require("share/serverconnlist")

outString('load server_mgr')
require("centd/server_mgr")
-------------------------------------------------------------------------------
--配置文件信息
config = {
	--进程守护定时器
	process_method_timer = 2000,
	--远程获取tag对象失效重获的时间间隔
	get_objects_timer = 10,
	--中心服命令表
	external_router_map = {
	},
}

--获得场景类型
function SelectScened(tSceneds)
	if(tSceneds[SCENED_TYPE_NO_INST] < SCENED_COUNT - 2)then	--4个野外场景
		return SCENED_TYPE_NO_INST
	end
	if(tSceneds[SCENED_TYPE_ACTIVI] == 0)then			--1个活动场景
		return SCENED_TYPE_ACTIVI
	end
	if(tSceneds[SCENED_TYPE_INST] == 0)then				--1个副本场景
		return SCENED_TYPE_INST
	end
	
	return SCENED_TYPE_NO_INST	
end
