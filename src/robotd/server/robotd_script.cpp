#include "robotd_app.h"
#include "robotd_script.h"
#include <shared/probability_rand.h>
#include <shared/storage_loader.h>
#include "robotd_context.h"
#include <shared/add_memdb_index.h>
#include <shared/lua_packet_binding.h>
#include "robotd_unit.h"
#include "tolua_fix.h"
#include "shared/lua_helper.h"

LuaStack *gLuaStack = NULL;
lua_State *L = NULL;

int ___outString(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	/* get number of arguments */
	int n = lua_gettop(formulaL);
	ASSERT(n == 1);	//暂不支持日志多参数输出
	const char *str = LUA_TOSTRING(formulaL, 1);
	tea_pinfo(str);
	return 0;
}

int ___outDebug(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	/* get number of arguments */
	int n = lua_gettop(formulaL);
	ASSERT(n == 1);	//暂不支持日志多参数输出
	const char *str = LUA_TOSTRING(formulaL, 1);
	tea_pdebug(str);
	return 0;
}

int ___outError(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	/* get number of arguments */
	int n = lua_gettop(formulaL);
	ASSERT(n == 1);	//暂不支持日志多参数输出
	const char *str = LUA_TOSTRING(formulaL, 1);
	tea_perror(str);
	return 0;
}

RobotdContext *___lua_update_robot = nullptr;
int LuaSetUpdateRobot(lua_State *scriptL)
{
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	___lua_update_robot = (RobotdContext*)lua_touserdata(scriptL, 1);
	return 0;
}

static const struct luaL_reg mylib[] = { 

	{"outString",				___outString},
	{"outDebug",				___outDebug},
	{"outError",				___outError},
	{"randInt",					__RandInt}, 
	{"randIntD",				__RandIntD}, 
	{"randIndex",				__RandIndex}, 

	{"getBit",					&LuaGetBit},
	{"setBit",					&LuaSetBit},
	//{"open_robert_debugger",	&luaopen_robert_debugger},					//打开调试支持

	{"setUpdateRobot",			&LuaSetUpdateRobot},
	{NULL, NULL} /* sentinel */ 
};

extern const struct luaL_reg LBinLog[] =
{
	LUA_BINLOG_REG_LIBNAME(BinLogObject),
	{NULL, NULL} /* sentinel */ 
};

extern const struct luaL_reg LPlayer[] =
{
	{"close",					&RobotdContext::LuaClose},
	{"isLoginOk",				&RobotdContext::LuaIsLoginOk},
	{NULL, NULL} /* sentinel */ 
};

extern const struct luaL_reg LUnit[] =
{
	{"getUnitPos",				&RobotdUnit::LuaGetPos},
	{"setUnitPos",				&RobotdUnit::LuaSetPos},
	{"getDistance",				&RobotdUnit::LuaGetDistance},
	{"isMoving",				&RobotdUnit::LuaIsMoving},
	{"stopMoving",				&RobotdUnit::LuaStopMoving},
	{"getDistanceByPos",		&RobotdUnit::LuaGetDistanceByPos},

	{NULL, NULL} /* sentinel */ 
};

extern const struct luaL_reg LMap[] =
{
	{"PathfindingGoto",			&PathFinding::LuaPathfindingGoto},
	{"RandomPos",				&PathFinding::LuaRandomPos},
	{"IsCanRun",				&PathFinding::LuaIsCanRun},
	{NULL, NULL} /* sentinel */ 
};

static const struct luaL_reg mylibname[] = 
{
	{"binLogLib",		luaopen_lib<LBinLog>},
	{"playerLib",		luaopen_lib<LPlayer>},
	{"unitLib",			luaopen_lib<LUnit>},
	{"mapLib",			luaopen_lib<LMap>},
	{NULL, NULL} /* sentinel */ 
};

lua_State *__script_init(const char* path)
{
	gLuaStack = new LuaStack;
	gLuaStack->init();
	L = gLuaStack->getLuaState();

	for(const luaL_reg *ptr = mylib;ptr->name != NULL && ptr->func != NULL;ptr++)
		lua_register(L,ptr->name,ptr->func);

	for (const luaL_reg * lib = mylibname; lib->func; lib++) {
		lua_pushcfunction(L, lib->func);
		lua_pushstring(L, lib->name);
		lua_call(L, 1, 0);
	}
	
	//加载http接口
	luaopen_lua_http_binding(L);
	//对象管理器封装接口
	lua_open_objects_manger(L);

	//包处理函数
	lua_open_packet_binding_init_delegate([](uint32_t cid,packet* pkt)->bool{
		//没有这个功能
		ASSERT(false);
		return true;
	},[](void* self, packet* pkt)->bool{		
		auto *ctx = (RobotdContext*)self;
		ASSERT(ctx);
		static ByteArray bytes;
		bytes.clear();
		bytes.writeShort(pkt->head->optcode);
		bytes.writeBytes(pkt->content, pkt->head->len);
		ctx->SendToServer(bytes);
		return true;
	});
	lua_open_packet_binding(L);

	/*设置包路径*/	
	char tmp[500];
	sprintf(tmp,"package.path = '%s/?.lua'",path);
	luaL_dostring(L,tmp);
	sprintf(tmp,"__SCRIPTS_ROOT__='%s'",path);		/*将脚本的路径注册进lua中*/
	luaL_dostring(L,tmp);

	/*加载脚本*/
	int status;
	sprintf(tmp, "%s/cow_robotd.lua", path);
	if((status = luaL_dofile(L, tmp)))
	{
		tea_perror("Load the application script [%s] ERROR!", tmp);
		tea_perror("%s!\n", LUA_TOSTRING(L, -1));
		lua_pop(L, 1);
		scripts_free();
		return NULL;
	}
	ASSERT(lua_gettop(L) == 0);//不要有残留
	return L;
}


//---------------------------------------------------------------------------------------------/
//获取配置信息
int DoGetConfig()
{
	LuaStackAutoPopup autoPopup(L);

	lua_getglobal(L,"config");
	if(!lua_istable(L,-1))
	{
		tea_perror("lua脚本中找不到配置信息");
		return -1;
	}

	vector<int> opt_set;
	lua_readvalue(L,"war_server_optcode",			opt_set, [](lua_State *scriptL, int &op_code){
		op_code = (int)LUA_TOINTEGER(scriptL, -1);
		ASSERT(op_code);
	});
	ProtocolUtil::SetWarOptCode(opt_set);
	return 0;
}

int scripts_init(const char* path)
{
	/*创建一个指向Lua解释器的指针。*/
	tea_pinfo("初始化默认LUA栈");
	L = __script_init(path);
	ASSERT(L);
	DoGetConfig();	
	return 0;
}

int scripts_free()
{
	safe_delete(gLuaStack);
	if(L != NULL)
		L = NULL;
	return 0;
}

int scripts_reload()
{
	scripts_free();
	scripts_init(g_Config.script_folder.c_str());
	return 0;
}

//取随机名字
int GetRandName(string &name)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "get_rand_name");	
	lua_pushnumber(L, 1);
	if(LUA_PCALL(L, 1, 1, 0))
	{
		tea_perror("get_rand_name  ERROR");
		return 1;
	}
	name = LUA_TOSTRING(L, 1);
	return 0;
}


//机器人登录
int DoRobotLoginOk(const string &account, uint32 fd, RobotdContext *robot)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "app");
	lua_getfield(L, -1, "RobotLoginOk");
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushstring(L, account.c_str());
	lua_pushinteger(L, fd);
	lua_pushlightuserdata(L, robot);
	if(LUA_PCALL(L, 4, 0, 0))
	{
		tea_perror("RobotLoginOk  ERROR %s %u ", account.c_str(), fd);
		ASSERT(false);
		return 1;
	}
	return 0;
}

int DoRobotRespawn(RobotdContext *robot) {
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "app");
	lua_getfield(L, -1, "RobotRespawn");
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushstring(L, robot->GetAccount().c_str());
	if(LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("DoRobotRespawn %s ERROR ", robot->GetGuid().c_str());
		return 1;
	}
	return 0;
}

//机器人登出
int DoRobotLogout(const string &account, uint32 fd)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "app");
	lua_getfield(L, -1, "RobotLogout");
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushstring(L, account.c_str());
	lua_pushinteger(L, fd);
	if(LUA_PCALL(L, 3, 0, 0))
	{
		tea_perror("RobotLogout  ERROR %s %u ", account.c_str(), fd);
		ASSERT(false);
		return 1;
	}
	return 0;
}

//机器人连接跨服成功
int DoRobotWarConn(const string &account, uint32 fd)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "app");
	lua_getfield(L, -1, "RobotWarConn");
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushstring(L, account.c_str());
	lua_pushinteger(L, fd);
	if(LUA_PCALL(L, 3, 0, 0))
	{
		tea_perror("RobotLogout  ERROR %s %u ", account.c_str(), fd);
		ASSERT(false);
		return 1;
	}
	return 0;
}

//机器人断开跨服连接
int DoRobotWarClose(const string &account, uint32 fd)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "app");
	lua_getfield(L, -1, "RobotWarClose");
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushstring(L, account.c_str());
	lua_pushinteger(L, fd);
	if(LUA_PCALL(L, 3, 0, 0))
	{
		tea_perror("RobotLogout  ERROR %s %u ", account.c_str(), fd);
		ASSERT(false);
		return 1;
	}
	return 0;
}

//网络包处理
int DoPacketHandle(const string &account, uint32 fd, packet *pkt)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "app");
	lua_getfield(L, -1, "PacketHandle");
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushstring(L, account.c_str());
	lua_pushinteger(L, fd);
	lua_pushlightuserdata(L, pkt);
	if(LUA_PCALL(L, 4, 0, 0))
	{
		tea_perror("PacketHandle  ERROR %s %u ", account.c_str(), fd);
		ASSERT(false);
		return 1;
	}
	return 0;
}

//机器人心跳
int DoRobotUpdate(uint32 diff)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "app");
	lua_getfield(L, -1, "RobotUpdate");
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushinteger(L, diff);
	if(LUA_PCALL(L, 2, 0, 0))
	{
		tea_perror("RobotUpdate  ERROR ");
		ASSERT(false);
		return 1;
	}
	return 0;
}

//取当前栈信息
int DoGetStack(const string &account,string &info)
{
	lua_getglobal(L, "app");
	lua_getfield(L, -1, "GetRobotStack");
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushstring(L, account.c_str());
	if(LUA_PCALL(L, 2, 1, 0))
	{
		tea_perror("DoGetStack  ERROR %s", account.c_str());;
		return 1;
	}
	info = (string)LUA_TOSTRING(L, -1);
	return 0;
}