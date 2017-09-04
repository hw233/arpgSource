#include "logind_app.h"
#include "map_manager.h"
#include "mobile_context.h"
#include "logind_context.h"
#include "logind_player.h"
#include "logind_script.h"
#include <shared/log_handler.h>
#include <shared/storage_loader.h>

LuaStack *gLuaStack = NULL;
lua_State *L = NULL;
const char *sdir = NULL;

static const struct luaL_reg mylib[] = 
{
	{"outString",			__outString<LogindApp>},
	{"outDebug",			__outDebug<LogindApp>},
	{"outError",			__outError<LogindApp>},

	//随机函数
	{"randInt",				__RandInt},
	{"randIntD",			__RandIntD},
	{"randIndex",			__RandIndex},

	{"getBit",				&LuaGetBit},
	{"setBit",				&LuaSetBit},
	{"loadConfig",			&LuaLoadConfig},

	{"findInstance",		&MapManager::LuaFindInstance},			//查找地图实例
	{"createInstance",		&MapManager::LuaCreateInstance},		//创建地图实例
	{"mapPlayerCount",		&MapManager::LuaMapPlayerCount},		//统计某地图内玩家数量
	{NULL, NULL} /* sentinel */ 
};

extern const struct luaL_reg LBinLog[] =
{
	LUA_BINLOG_REG_LIBNAME(BinLogObject),
	{NULL, NULL} /* sentinel */ 
};

static const struct luaL_reg mylibname[] = 
{
	{"binLogLib",			luaopen_lib<LBinLog>},
	{NULL, NULL} /* sentinel */ 
};


int scripts_init(const char *_sdir)
{
	int status;    
	char sfile[500];

	/*创建一个指向Lua解释器的指针。*/
	gLuaStack = new LuaStack;
	gLuaStack->init();
	L = gLuaStack->getLuaState();

	if(_sdir != NULL)
		sdir = _sdir;

	/*函数加载Lua库*/
	luaL_openlibs(L);		
	//对象管理器封装接口
	lua_open_objects_manger(L);

	/////////////////////////////////////////////////////////////////
	//			注册
	////////////////////////////////////////////////////////////////
	for(const luaL_reg *ptr = mylib;ptr->name != NULL;ptr++)
		lua_register(L,ptr->name,ptr->func);
	for (const luaL_reg * lib = mylibname; lib->func; lib++) {
		lua_pushcfunction(L, lib->func);
		lua_pushstring(L, lib->name);
		lua_call(L, 1, 0);
	}	
	sprintf(sfile,"package.path = package.path .. ';%s/?.lua'",_sdir);
	luaL_dostring(L,sfile);		
	sprintf(sfile,"__SCRIPTS_ROOT__='%s'",_sdir);		/*将脚本的路径注册进lua中*/
	luaL_dostring(L,sfile);
	/////////////////////////////
	//		  加载脚本
	/////////////////////////////

	/*加载脚本*/    
	sprintf(sfile, "%s/cow_logind.lua", sdir);
	if((status = luaL_dofile(L, sfile)))
	{
		tea_perror("载入脚本 %s 出错!", sfile);
		tea_perror("%s!\n", LUA_TOSTRING(L, -1)); /* 注意printf不要打印额外信息了，因为lua的出错信息已经够用了 */
		lua_pop(L, 1);
		scripts_free();
		return status;
	}	
	DoGetConfig();	
	ASSERT(lua_gettop(L) == 0);//不要有残留
	return 0;
}

int scripts_free()
{
	if(gLuaStack != NULL)
	{
		delete gLuaStack;
		gLuaStack = NULL;
	}
	return 0;
}

int scripts_reload()
{
	scripts_free();
	scripts_init(g_Config.script_folder.c_str());	
	LogindApp::g_app->RegSessionOpts();
	return 0;
}

//---------------------------------------------------------------------------------------------/
//获取配置信息
int DoGetConfig()
{
	{
		LuaStackAutoPopup autoPopup(L);

		lua_getglobal(L,"config");
		if(!lua_istable(L,-1))
		{
			tea_perror("lua脚本中找不到配置信息");
			return -1;
		}

		lua_readvalue(L,"external_router_map",			LogindApp::g_app->m_opts, [](lua_State *scriptL, uint16 &op_code){
			op_code = (uint16)LUA_TOINTEGER(scriptL, -1);
			ASSERT(op_code);
		});
		lua_readvalue(L,"pk_external_router_map",			LogindApp::g_app->m_pk_opts, [](lua_State *scriptL, uint16 &op_code){
			op_code = (uint16)LUA_TOINTEGER(scriptL, -1);
			ASSERT(op_code);
		});
		lua_readvalue(L,"reload_game_config_interval",	g_Config.reload_game_config_interval);
		lua_readvalue(L,"auto_save_time",g_Config.auto_save_time);
		lua_readvalue(L,"auto_backup_hdd_time",g_Config.auto_backup_hdd_time);
		lua_readvalue(L,"Save_Player_To_DB_Count",g_Config.Save_Player_To_DB_Count);
		lua_readvalue(L,"cant_make_name",g_Config.g_cant_make_name);
	}
	{
		LuaStackAutoPopup autoPopup(L);
		lua_getglobal(L,"max_player_count");
		if(lua_isnil(L, 1))
		{
			tea_pdebug("DoGetConfig max_player_count nil");
			g_Config.max_player_count = 10000; //默认一万
		}
		else
		{
			g_Config.max_player_count = (uint32)LUA_TOINTEGER(L, 1);
			tea_pdebug("DoGetConfig max_player_count %u", g_Config.max_player_count);
		}
	}

	{
		LuaStackAutoPopup autoPopup(L);
		lua_getglobal(L,"SCENED_COUNT");
		if(lua_isnil(L, 1))
		{
			tea_pdebug("DoGetConfig max_secend_count nil");
			g_Config.max_secend_count = 6; //默认6个
		}
		else
		{
			g_Config.max_secend_count = (uint32)LUA_TOINTEGER(L, 1);
			tea_pdebug("DoGetConfig max_secend_count %u", g_Config.max_secend_count);
		}
	}
	return 0;
}

bool LoadTemplate()
{
	tea_pinfo("\n\n load template...");	
	if(!Load_char_level(L)) 
		return false;
	return true;
}


//重新读取配置文件
int LuaLoadConfig(lua_State* scriptL)
{
	DoGetConfig();
	return 0;
}

int DoGMScripts(string& gm_commands)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "DoGMScripts");

	lua_pushstring(L,gm_commands.c_str());
	//lua_pushlightuserdata(scriptL,map);
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("DoGMScripts  ERROR %s",gm_commands.c_str());
		return 0;
	}
	string result_str = LUA_TOSTRING(L,-1);
	return 0;
}

//GM命令的KEY转换
int GetGmCommandKey(string &gm_str)
{
	//lua_State *L = itemL;
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "GetGmCommandKey");
	lua_pushstring(L, gm_str.c_str());		
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("DoWingsBonus  ERROR");
		return 0;
	}
	return (int)LUA_TONUMBER(L,-1);
}


//玩家下线做点什么
int DoPlayerLogout(logind_player* player)
{
	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "LogindPlayer", player, "Logout");
	if(LUA_PCALL(L, 1, 0, 0))
	{
		tea_perror("DoPlayerLogout %s", player->GetGuid().c_str());
	}
	return 0;
}

//重新载入游戏配置表的内容
int DoReloadGameConfigTable(const char *key, const char *value)
{
	if(key == NULL || value == NULL)
	{
		tea_perror("游戏配置表中有空值。");
		return 1;
	}

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "DoReloadGameConfigTable");
	lua_pushstring(L, key);
	lua_pushstring(L, value);
	if(LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("DoReloadGameConfigTable %s %s", key, value);
		return 1;
	}
	return 0;
}

//获取汉字(字符串)
int DoGetCharacters(uint8 pos, string & parameter)
{
	//lua_State *L = itemL;
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "GetCharacters");
	lua_pushnumber(L, pos);
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("DoGetCharacters:  ERROR");
		return 0;
	}
	parameter = lua_tostring(L,-1);
	return 0; 
}

//合服后数据处理
int DoMergeSomething()
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "DoMergeSomething");
	if(LUA_PCALL(L, 0, 0, 0))
	{
		tea_perror("DoMergeSomething:  ERROR");
		return 0;
	}
	return 0;
}

//修复玩家数据
int DoRepairPlayerData(const string &account, vector<GuidObject*> &data, bool &b)
{
	ASSERT(!data.empty());
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "RepairPlayerData");
	lua_pushstring(L, account.c_str());
	lua_newtable(L);
	int i = 1;
	std::for_each(data.begin(),data.end(),[&i](GuidObject* it){
		ASSERT(it);
		lua_pushnumber(L, i);
		lua_pushlightuserdata(L, it);
		lua_rawset(L, -3);
		i++;
	});
	if(LUA_PCALL(L, 2, 1, 0))
	{
		tea_perror("DoRepairPlayerData:  ERROR  %s", account.c_str());
	}
	b = LUA_TOBOOLEAN(L, -1) != FALSE;

	return 0;
}

//获得场景服的个数
int DoGetScenedSize(uint32 &result)
{
	result = 0;
	if (!LogindApp::g_app->m_server_conn_list)
		return 0;

	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "ServerConnList", LogindApp::g_app->m_server_conn_list, "GetScenedSize");

	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("lua_error: DoGetScenedSize  ERROR!!!");
		return 0;
	}
	result = uint32(LUA_TONUMBER(L, -1));
	return 0;
}

//添加场景服的玩家数量
int DoAddScenedPlayer(uint32 conn_id)
{
	if (!LogindApp::g_app->m_server_conn_list)
		return 0;

	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "ServerConnList", LogindApp::g_app->m_server_conn_list, "AddScenedPlayer");
	lua_pushnumber(L, conn_id);
	if(LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("lua_error: DoAddScenedPlayer  ERROR!!!");
		return 0;
	}
	return 0;
}

//减少场景服的玩家数量
int DoSubScenedPlayer(uint32 conn_id)
{
	if (!LogindApp::g_app->m_server_conn_list)
		return 0;

	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "ServerConnList", LogindApp::g_app->m_server_conn_list, "SubScenedPlayer");
	lua_pushnumber(L, conn_id);
	if(LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("lua_error: DoSubScenedPlayer  ERROR!!!");
		return 0;
	}
	return 0;
}

//根据一定的规则获得场景服连接
int DoGetScenedFDByType(uint16 inst_type, uint32 mapid)
{
	if (!LogindApp::g_app->m_server_conn_list)
		return 0;

	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "ServerConnList", LogindApp::g_app->m_server_conn_list, "GetScenedFDByType");
	lua_pushnumber(L, inst_type);
	lua_pushnumber(L, mapid);
	if(LUA_PCALL(L, 3, 1, 0))
	{
		tea_perror("lua_error: DoGetScenedFDByType  ERROR!!!");
		return 0;
	}
	return int(LUA_TONUMBER(L, -1));
}

//玩家下线的时候判断是否记录玩家所在地图的信息，最多保留10分钟
int DoIsRecordIntanceInfo(logind_player* player, uint32 map_id, uint32 isfuben, uint32 fuben_type)
{
	if (!player)
	{
		tea_pdebug("error:DoIsRecordIntanceInfo  player == null!!!");
		return 0;
	}

	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "DoIsRecordIntanceInfo");
	lua_pushlightuserdata(L, player);
	lua_pushnumber(L, map_id);
	lua_pushnumber(L, isfuben);
	lua_pushnumber(L, fuben_type);
	if(LUA_PCALL(L, 4, 1, 0))
	{
		tea_perror("DoGetCharacters:  ERROR");
		return 0;
	}
	return (int)LUA_TOBOOLEAN(L,-1);
}

//pk服，根据跨服类型选择要传送到的地图id
bool DoSelectKuafuMapid(logind_player* player, uint32 warid, uint8 kuafutype, uint32 number, uint32 reverse, string reverse_str)
{
	if (!player)
	{
		tea_pdebug("DoSelectKuafuMapid player == null !!!");
		return false;
	}
	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "LogindPlayer", player, "SelectKuafuMapid");
	lua_pushnumber(L, warid);
	lua_pushnumber(L, kuafutype);
	lua_pushnumber(L, number);
	lua_pushnumber(L, reverse);
	lua_pushstring(L, reverse_str.c_str());
	if(LUA_PCALL(L, 6, 1, 0))
	{
		tea_perror("lua_error DoSelectKuafuMapid %s kuafutype %u", player->GetGuid().c_str(), kuafutype);
		return false;
	}

	return (BOOL)LUA_TOBOOLEAN(L, -1)==1?true:false;
}

//根据地图id判断所在地图是否为跨服地图
bool DoIsKuafuMapID(uint32 map_id)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "IsKuafuMapID");
	lua_pushnumber(L, map_id);
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("lua_error: DoIsKuafuMapID  ERROR!!!");
		return false;
	}
	
	return LUA_TOBOOLEAN(L,-1) == TRUE;
}

//计算玩家付费等级
int DoPlayerPayLevel(logind_player* player)
{
	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "LogindPlayer", player, "DoPayLevel");
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("DoPlayerPayLevel %s", player->GetGuid().c_str());
		return 0;
	}
	return (int)LUA_TONUMBER(L,-1);
}

//是否需要提供传送generalid
int DoIsNeedGeneralid(uint32 mapid, bool &result)
{
	result = false;
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "DoIsNeedGeneralid");
	lua_pushnumber(L, mapid);
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("lua_error: DoIsNeedGeneralid  ERROR!!! %u", mapid);
		return 0;
	}
	
	result = LUA_TOBOOLEAN(L, -1) != FALSE;
	return 0;
}

//是否需要提供传送generalid
bool IsInstanceOfflineReenter(uint32 mapid)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "DoIsOfflineReenter");
	lua_pushnumber(L, mapid);
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("lua_error: IsInstanceOfflineReenter  ERROR!!! %u", mapid);
		return 0;
	}

	return LUA_TOBOOLEAN(L, -1) != FALSE;
}

// 获得冒险地图的传送信息
int DoGetRiskTeleportInfo(logind_player* player, int &mapid, float &x, float &y, string &generalId) {

	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "LogindPlayer", player, "GetRiskTeleportInfo");
	if(LUA_PCALL(L, 1, 4, 0)) {
		tea_perror("lua_error: DoGetRiskTeleportInfo  ERROR!!! %s", player->GetGuid().c_str());
		return 1;
	}

	generalId = (string)LUA_TOSTRING(L, -1);
	y = (float)LUA_TONUMBER(L, -2);
	x = (float)LUA_TONUMBER(L, -3);
	mapid = (int)LUA_TONUMBER(L, -4);

	return 0;
}

//获得配置人数
int OnGetMapLineCount(uint32 mapid, uint8 &result)
{
	result = false;
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "OnGetMapLineCount");
	lua_pushnumber(L, mapid);
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("lua_error: DoIsNeedGeneralid  ERROR!!! %u", mapid);
		return 0;
	}
	
	result = (uint8)LUA_TONUMBER(L, -1);

	return 0;
}

//查找或者新建地图实例
int DoFindOrCreateMap(uint32 mapid, uint16 inst_type, const string &general_id, uint32 lineno, int &index)
{
	index = -1;
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "FindOrCreateMap");
	lua_pushnumber(L, mapid);
	lua_pushnumber(L, inst_type);
	lua_pushstring(L, general_id.c_str());
	lua_pushnumber(L, lineno);

	if(LUA_PCALL(L, 4, 1, 0))
	{
		tea_perror("lua_error: DoFindOrCreateMap  ERROR!!! %u %u %s %u", mapid, inst_type, general_id.c_str(), lineno);
		return 0;
	}

	index = LUA_TOINTEGER(L, -1);
	return 0;
}

//根据地图id判断所在地图是否为世界地图
bool DoIsWorldMapID(uint32 map_id)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "IsWorldMap");
	lua_pushnumber(L, map_id);
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("lua_error: IsWorldMap  ERROR!!!");
		return false;
	}

	return LUA_TOBOOLEAN(L,-1) == TRUE;
}

//获取下次保存数据库的时间
int DoGetNextSaveTime(uint32 &t)
{
	t = 0;
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "GetNextSaveTime");
	if(LUA_PCALL(L, 0, 1, 0))
	{
		tea_perror("lua_error: IsWorldMap  ERROR!!!");
		return 1;
	}

	t = (uint32)LUA_TOINTEGER(L, -1);
	return 0;
}

//设置玩家初始信息
void DoSetNewPlayerInfo(logind_player* player)
{
	LuaStackAutoPopup autoPopup(L);
	NewLuaTable(L, "LogindPlayer", player, "SetNewPlayerInfo");
	if(LUA_PCALL(L, 1, 0, 0))
	{
		tea_perror("lua_error: DoSetNewPlayerInfo  ERROR!!! %u");
		return;
	}
	return;
}