
#include "lua_helper.h"
#include "util.h"
#include "probability_rand.h"
#include "object/BinLogObject.h"
#include <db_wrap/mongo_db.h>

int windows_lua_pcall(lua_State *L,  int nargs, int nresults, int p)
{
	//如果这里断言了，那肯定是在lua里没找到这个方法
	ASSERT(lua_type(L, lua_gettop(L) - nargs) == LUA_TFUNCTION);
	lua_call(L, nargs, nresults);
	return 0;
}

void check_lua_print_error(lua_State *L)
{
	int top = lua_gettop(L);
	for(int j = 1; j <= top; j ++)
	{
		if(lua_isnil(L, j))
		{
			printf("--接口的第%u个参数为nil值\n", j);
		}
		else if(lua_isstring(L, j))
		{
			printf("--接口的第%u个参数为字符串 %s\n", j, lua_tostring(L, j));
		}
		else if(lua_islightuserdata(L, j))
		{
			printf("--接口的第%u个参数为指针 %p\n", j, lua_touserdata(L, j));
		}
		else
		{
			printf("--接口的第%u个参数为数字 %u\n", j, lua_tonumber(L, j));
		}
	}

	ASSERT(false);
}

void check_lua_nil_parameter(lua_State *L)
{
	int top = lua_gettop(L);
	bool b = false;
	for(int i = 1; i <= top; i ++)
	{
		if(lua_isnil(L, i))
		{
			check_lua_print_error(L);
		}
	}
}

lua_Number lua_check_tonumber(lua_State *L, int n)
{
	if(lua_type(L, n) != LUA_TNUMBER && lua_type(L, n) != LUA_TNIL)
	{
		printf("---------------------------------------第%d个参数的类型是 %s", n, lua_typename(L, lua_type(L, n)));
		check_lua_print_error(L);
	}
	return lua_tonumber(L, n);
}
lua_Integer lua_check_tointeger(lua_State *L, int n)
{
	if(lua_type(L, n) != LUA_TNUMBER && lua_type(L, n) != LUA_TNIL)
	{
		printf("---------------------------------------第%d个参数的类型是 %s", n, lua_typename(L, lua_type(L, n)));
		check_lua_print_error(L);
	}
	return lua_tointeger(L, n);
}
int lua_check_toboolean(lua_State *L, int n)
{
	if(lua_type(L, n) != LUA_TBOOLEAN && lua_type(L, n) != LUA_TNIL)
	{
		printf("---------------------------------------第%d个参数的类型是 %s", n, lua_typename(L, lua_type(L, n)));
		check_lua_print_error(L);
	}
	return lua_toboolean(L, n);
}
const char *lua_check_tolstring(lua_State *L, int n, size_t *len)
{
	if(lua_type(L, n) != LUA_TSTRING && lua_type(L, n) != LUA_TNIL)
	{
		printf("---------------------------------------第%d个参数的类型是 %s", n, lua_typename(L, lua_type(L, n)));
		check_lua_print_error(L);
	}
	return lua_tolstring(L, n, len);
}
const char *lua_check_tostring(lua_State *L, int n)
{
	if(lua_type(L, n) != LUA_TSTRING && lua_type(L, n) != LUA_TNIL)
	{
		printf("---------------------------------------第%d个参数的类型是 %s", n, lua_typename(L, lua_type(L, n)));
		check_lua_print_error(L);
	}
	return lua_tostring(L, n);
}

void *lua_check_touserdata(lua_State *L, int n, EObjectTypePrefix t)
{
	if(lua_type(L, n) != LUA_TLIGHTUSERDATA && lua_type(L, n) != LUA_TNIL)
	{
		printf("---------------------------------------第%d个参数的类型是 %s", n, lua_typename(L, lua_type(L, n)));
		check_lua_print_error(L);
	}
	void *result = lua_touserdata(L, n);
	if(!result) 
		check_lua_print_error(L);
	if(t != ObjectTypeNone)
	{
		BinLogObject *binlog = (BinLogObject *)result;
		if(GuidManager::GetPrefix(binlog->GetGuid()) != t)
		{
			check_lua_print_error(L);
		}
	}
	return result;
}


//取随机数
int __RandInt(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	int n = lua_gettop(formulaL);
	ASSERT(n == 2);

	//	uint32 max		= (uint32)LUA_TONUMBER(formulaL,1);
	int32 mins = (int32)LUA_TOINTEGER(formulaL, 1);
	int32 maxs = (int32)LUA_TOINTEGER(formulaL, 2);
	if(mins < maxs)
	{
		int32 rand_num = irand(mins,maxs);
		lua_pushinteger(formulaL, rand_num);		
	}
	else
	{
		lua_pushinteger(formulaL, mins);
	}
	return 1;
}
//取随机数
int __RandIntD(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	uint32 mins = (uint32)LUA_TONUMBER(formulaL, 1);
	uint32 max  = (uint32)LUA_TONUMBER(formulaL, 2);
	if(mins < max)
	{
		uint32 rand_num = urand(mins, max);
		lua_pushnumber(formulaL, rand_num);
	}
	else
	{
		lua_pushinteger(formulaL, mins);
	}
	
	return 1;
}

//第一个参数是需要随机出来的数量
//第二个是table{存在很多概率}
//索引table
int __RandIndex(lua_State *formulaL)
{
	CHECK_LUA_NIL_PARAMETER(formulaL);
	ProbabilityRand<uint32> rand_by_probability;
	//需要随机出几个
	uint32 need_rand = (uint32)LUA_TONUMBER(formulaL, -2);
	uint32 it_idx = (uint32)lua_gettop(formulaL);
	lua_pushnil(formulaL);

	while(lua_next(formulaL, it_idx))
	{
		uint32 a = (uint32)LUA_TONUMBER(formulaL, -1);
		uint32 index = (uint32)LUA_TONUMBER(formulaL, -2);
		rand_by_probability.AddRandPossibility(a, index);		
		lua_pop(formulaL, 1);
	}
	
	lua_newtable(formulaL);    /* We will pass a table */
	if(rand_by_probability.empty())
		return 1;
	for(uint32 i = 0; i < need_rand; i++)
	{
		lua_pushnumber(formulaL, i);   /* Push the table index */
		lua_pushnumber(formulaL, rand_by_probability.Rand(true));
		lua_rawset(formulaL, -3);      /* Stores the pair in the table */
	}

	return 1;
}

void lua_readvalue(lua_State *scriptL,const char* key,const char *& val)
{
	lua_getfield(scriptL,-1,key);
	val = LUA_TOSTRING(scriptL,-1);
	lua_pop(scriptL,1);
}

void lua_readvalue(lua_State *scriptL,const char* key,char *val)
{
	lua_getfield(scriptL,-1,key);
	strcpy (val ,LUA_TOSTRING(scriptL,-1));
	lua_pop(scriptL,1);
}

void lua_readvalue(lua_State *scriptL,const int index,const char *& val)
{
	lua_rawgeti(scriptL, -1, index);
	val = LUA_TOSTRING(scriptL,-1);
	lua_pop(scriptL,1);
}

void lua_readvalue(lua_State *scriptL,const int index,string& val)
{
	lua_rawgeti(scriptL, -1, index);
	val = LUA_TOSTRING(scriptL,-1);
	lua_pop(scriptL,1);
}

void lua_readvalue(lua_State *scriptL,const int index,char *val)
{
	lua_rawgeti(scriptL, -1, index);
	const char *s = LUA_TOSTRING(scriptL,-1);
	strcpy(val , s);
	lua_pop(scriptL,1);
}

void lua_readvalue(lua_State *scriptL,const char* key,bool& b)
{
	lua_getfield(scriptL,-1,key);
	b = (LUA_TOBOOLEAN(scriptL,-1)!=0);
	lua_pop(scriptL,1);
}

void lua_readvalue(lua_State *scriptL,const char* key,string& val)
{
	lua_getfield(scriptL,-1,key);
	val = LUA_TOSTRING(scriptL,-1);
	lua_pop(scriptL,1);
}

void lua_getvector(lua_State *scriptL,vector<string>& vec)
{
	vec.clear();
	int it_idx = lua_gettop(scriptL);
	lua_pushnil(scriptL);	
	while(lua_next(scriptL,it_idx))
	{
		vec.push_back(LUA_TOSTRING(scriptL,-1));
		lua_pop(scriptL,1);
	}
}

int LuaGetBit(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n >= 2);
	uint32 val = (uint32)LUA_TONUMBER(scriptL, 1);
	uint8 off = (uint8)LUA_TONUMBER(scriptL, 2);
	lua_pushboolean(scriptL, GET_BIT(val, off));
	return 1;
}

int LuaSetBit(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n >= 2);
	uint32 val = (uint32)LUA_TONUMBER(scriptL, 1);
	uint8 off = (uint8)LUA_TONUMBER(scriptL, 2);	
	lua_pushnumber(scriptL, SET_BIT(val, off));
	return 1;
}

int LuaUnSetBit(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n >= 2);
	uint32 val = (uint32)LUA_TONUMBER(scriptL, 1);
	uint8 off = (uint8)LUA_TONUMBER(scriptL, 2);
	lua_pushnumber(scriptL, UNSET_BIT(val, off));
	return 1;
}

int DoOpenMongodb(string connStr,string server_name)
{
	string host;
	string port;
	string user;
	string passwd;
	string db_prefix;	//数据库前缀

	if (!MongoDB::DecodeConnectionString(connStr,host,port,user,passwd, db_prefix))
	{
		//tea_perror("lua error:DoOpenMongodb/DecodeConnectionString");
		return -1;
	}	

	LuaStackAutoPopup autoPopup(L); 
	lua_getglobal(L, "app");
	if(!lua_istable(L, -1))
		return -1;
	lua_getfield(L, -1, "openMongoDB");
	if(!lua_isfunction(L, -1))
		return -2;
	lua_getglobal(L, "app");
	lua_pushstring(L, host.c_str());
	lua_pushinteger(L, atoi(port.c_str()));
	lua_pushstring(L,user.c_str());
	lua_pushstring(L, passwd.c_str());
	lua_pushstring(L,"");
	lua_pushstring(L, db_prefix.c_str());
	lua_pushstring(L,server_name.c_str());
	if(LUA_PCALL(L, 8, 1, 0))
	{
		return -3;
	}
	return 0;
}

//调用LUA心跳
int DoUpdateLua(uint32 diff)
{
	LuaStackAutoPopup autoPopup(L);
	lua_getglobal(L, "app");
	if(!lua_istable(L, -1))
		return -1;
	lua_getfield(L, -1, "update");
	if(!lua_isfunction(L, -1))
		return -2;
	lua_remove(L, -2);
	lua_getglobal(L, "app");
	lua_pushinteger(L, diff);
	if (LUA_PCALL(L, 2, 1, 0))
	{
		return -3;
	}
	return lua_tointeger(L,-1);
}

//让lua具备取得取得微秒的能力
int lua_gettimeofday(lua_State *L)
{	
	struct timeval tmnow;
	gettimeofday(&tmnow, NULL) ;
	lua_pushnumber(L, tmnow.tv_sec);
	lua_pushnumber(L, tmnow.tv_usec);
	return 2;
}

int LuaGetObj(lua_State *L, const char* guid)
{
	lua_getglobal(L, "app");
	if(!lua_istable(L, -1))
		return -1;

	lua_getfield(L, -1, "objMgr");
	if(!lua_istable(L, -1))
		return -2;
	lua_remove(L, -2);

	lua_getfield(L, -1, "getObj");
	if(!lua_isfunction(L, -1))
		return -2;

	//将objMgr拷贝到时栈顶
	lua_pushvalue(L, -2);

	lua_pushstring(L, guid);
	lua_pcall(L, 2, 1, 0);

	if(lua_isnil(L, -1))
	{
		printf("error getObj function %s", guid);
		return -1;
	}

	return 0;
}

int LuaGetObjFunc(lua_State *L, const char* guid, const char* func_name)
{
	if(LuaGetObj(L, guid))
	{
		printf("error getObj function %s %s", guid, func_name);
		//开发阶段，直接崩溃吧
		ASSERT(false);
		return -1;
	}

	lua_getfield(L, -1, func_name);
	ASSERT(lua_isfunction(L, -1));

	//把new出来的table拷贝到栈顶
	lua_pushvalue(L, -2);

	return 0;
}
