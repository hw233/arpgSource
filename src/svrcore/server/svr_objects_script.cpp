#include <shared/lua_helper.h>
#include "tolua++.h"
#include "tolua_fix.h"
#include "svr_object_manager.h"
#include <object/BinLogObject.h>

//////////////////////////////////////////////////////////////////////////
//LUA call c

namespace objects
{


//根据所给guid返回对象
static int luaGet(lua_State *L)
{
	const char *guid = luaL_checkstring(L,1);
	GuidObject *ptr = SvrObjectManager::self->Get(guid);
	if (ptr)
		lua_pushlightuserdata(L, (void*)ptr);
	else
		lua_pushnil(L);
	return 1;
}

//根据所给对象查询表达式遍历对象
static int luaForeach(lua_State *tolua_S)
{
	tolua_Error tolua_err; 
	
	if (!tolua_isstring(tolua_S,1, 0, &tolua_err) 
		|| !toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err))
	{
		 tolua_error(tolua_S,"#ferror in function 'luaForeach'.",&tolua_err);
		 return 0;
	}

	const char* key = luaL_checkstring(tolua_S, 1);
	int f = toluafix_ref_function(tolua_S,2,0);
	SvrObjectManager::self->ForeachIndexer(key,[&](core_obj::GuidObject *obj){
		int statePos = lua_gettop( tolua_S );
		toluafix_get_function_by_refid(tolua_S,f);
		//tolua_pushusertype(tolua_S,(void*)self,"core_obj::LuaGuidObjectTable");
		tolua_pushcppstring(tolua_S, obj->guid());
		lua_pushlightuserdata(tolua_S,(void*)obj);
		lua_call(tolua_S,2,1); 
		bool result = tolua_toboolean(tolua_S,-1,0) != 0;
		lua_settop(tolua_S,statePos);		//恢复栈区
		return result;
	});
	//去掉该函数的引用
	toluafix_remove_function_by_refid(tolua_S,f);
	return 0;
}

static int gAttachObjectt_delegate_index = 0;

// 对象创建时触发
static int luaRegisterAttachObject(lua_State *tolua_S)
{
	tolua_Error tolua_err; 
    if(!toluafix_isfunction(tolua_S, 1, "LUA_FUNCTION", 0, &tolua_err))
    {
		tolua_error(tolua_S,"#ferror in function 'RegisterAttachObjectt'.",&tolua_err);
        return 0;
    }
	if (gAttachObjectt_delegate_index)
		toluafix_remove_function_by_refid(tolua_S, gAttachObjectt_delegate_index);
	gAttachObjectt_delegate_index = toluafix_ref_function(tolua_S,1,0);
	ASSERT(gAttachObjectt_delegate_index);
    return 0;
}

static int gAfterAttachObjectt_delegate_index = 0;

// 取得对象后附加到对象库以后时触发
static int luaRegisterAfterAttachObject(lua_State *tolua_S)
{
	tolua_Error tolua_err; 
	if(!toluafix_isfunction(tolua_S, 1, "LUA_FUNCTION", 0, &tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'RegisterAttachObjectt'.",&tolua_err);
		return 0;
	}
	if (gAfterAttachObjectt_delegate_index)
		toluafix_remove_function_by_refid(tolua_S, gAfterAttachObjectt_delegate_index);
	gAfterAttachObjectt_delegate_index = toluafix_ref_function(tolua_S,1,0);
	ASSERT(gAfterAttachObjectt_delegate_index);
	return 0;
}

//释放对象回调委托
static int gReleaseObject_delegate_index = 0;

//当对象被释放时触发
static int luaRegisterReleaseObject(lua_State *tolua_S)
{
	tolua_Error tolua_err; 
    if(!toluafix_isfunction(tolua_S, 1, "LUA_FUNCTION", 0, &tolua_err))
    {
		tolua_error(tolua_S,"#ferror in function 'RegisterReleaseObject'.",&tolua_err);
        return 0;
    }
	if (gReleaseObject_delegate_index)
		toluafix_remove_function_by_refid(tolua_S, gReleaseObject_delegate_index);
	gReleaseObject_delegate_index = toluafix_ref_function(tolua_S,1,0);
	ASSERT(gReleaseObject_delegate_index);
    return 0;
}

//创建一个新对象,并且通知远程创建
static int luaNewAndCallPut(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if (!tolua_isstring(tolua_S,1, 0, &tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'luaCreateObject'.",&tolua_err);
		return 0;
	}

	const char *guid = luaL_checkstring(tolua_S,1);
	const char *owner = luaL_checkstring(tolua_S,2);
	GuidObject *ptr = SvrObjectManager::self->Get(guid);
	if (ptr)
		return 0;			//不能重复创建
	if (strlen(owner) == 0)
		return 0;			//owner不能为空,因为磁盘的存储是会依赖owner进行分类
	
	BinLogObject* obj = new BinLogObject;
	obj->SetGuid(guid);
	obj->SetOwner(owner);
	SvrObjectManager::self->CallPutObject(obj);
	SvrObjectManager::self->CallSetTag(guid, owner);
	SvrObjectManager::self->InsertObjOwner(guid);		//本地创建的对象直接放进去
	return 0;
}

//通知远程删除对象
static int luaCallRemoveObject(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if (!tolua_isstring(tolua_S,1, 0, &tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'luaCallRemoveObject'.",&tolua_err);
		return 0;
	}

	const char *guid = luaL_checkstring(tolua_S,1);
	SvrObjectManager::self->CallRemoveObject(guid);
	return 0;
}

//添加监听
static int luaCallAddWatch(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if (!tolua_isnumber(tolua_S, 1, 0, &tolua_err) || !tolua_isstring(tolua_S, 2, 0, &tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'luaCallAddWatch'.",&tolua_err);
		return 0;
	}

	int fd = (int)tolua_tonumber(tolua_S, 1, 0);
	const char *guid = luaL_checkstring(tolua_S,2);
	SvrObjectManager::self->CallAddWatch(fd, guid);
	return 0;
}
	
//删除监听
static int luaCallDelWatch(lua_State *tolua_S)
{
	tolua_Error tolua_err;
	if (!tolua_isnumber(tolua_S, 1, 0, &tolua_err) || !tolua_isstring(tolua_S, 2, 0, &tolua_err))
	{
		tolua_error(tolua_S,"#ferror in function 'luaCallDelWatch'.",&tolua_err);
		return 0;
	}

	int fd = (int)tolua_tonumber(tolua_S, 1, 0);
	const char *guid = luaL_checkstring(tolua_S,2);
	SvrObjectManager::self->CallDelWatch(fd, guid);
	return 0;
}

};

//////////////////////////////////////////////////////////////////////////
// C call Lua

using namespace objects;

//当对象附加是触发
void DoOnAttachObject(GuidObject *obj)
{	
    if(L && gAttachObjectt_delegate_index != 0)
    {
		LuaStackAutoPopup autoPopup(L);
        toluafix_get_function_by_refid(L, gAttachObjectt_delegate_index );
        //TODO:验证函数是否有效
		tolua_pushcppstring(L, obj->guid());
        lua_pushlightuserdata(L, (void*)obj);
        lua_call(L, 2, 0);
    }
}

//当对象附加是触发
void DoOnAttachObject(const string &account, GuidObject *obj)
{	
	if(L && gAttachObjectt_delegate_index != 0)
	{
		LuaStackAutoPopup autoPopup(L);
		toluafix_get_function_by_refid(L, gAttachObjectt_delegate_index );
		//TODO:验证函数是否有效
		tolua_pushstring(L, account.c_str());
		tolua_pushcppstring(L, obj->guid());
		lua_pushlightuserdata(L, (void*)obj);
		lua_call(L, 3, 0);
	}
}

//当对象附加后触发
void DoOnAfterAttachObject(const string &account, GuidObject *obj)
{	
	if(L && gAfterAttachObjectt_delegate_index != 0)
	{
		LuaStackAutoPopup autoPopup(L);
		toluafix_get_function_by_refid(L, gAfterAttachObjectt_delegate_index );
		//TODO:验证函数是否有效
		tolua_pushstring(L, account.c_str());
		lua_pushlightuserdata(L, (void*)obj);
		lua_call(L, 2, 0);
	}
}

//当对象附加后触发
void DoOnAfterAttachObject(GuidObject *obj)
{	
    if(L && gAfterAttachObjectt_delegate_index != 0)
    {
		LuaStackAutoPopup autoPopup(L);
        toluafix_get_function_by_refid(L, gAfterAttachObjectt_delegate_index );
        //TODO:验证函数是否有效
        lua_pushlightuserdata(L, (void*)obj);
        lua_call(L, 1, 0);
    }
}


//当对象被释放时触发回调
void DoOnReleaseObject(GuidObject *obj)
{	
    if(L && gReleaseObject_delegate_index != 0)
    {
		LuaStackAutoPopup autoPopup(L);
        toluafix_get_function_by_refid(L, gReleaseObject_delegate_index);
        tolua_pushcppstring(L, obj->guid());
        lua_pushlightuserdata(L, (void*)obj);
        lua_call(L, 2, 0);
    }
}

//当对象被释放时触发回调
void DoOnReleaseObject(const string &account, GuidObject *obj)
{	
	if(L && gReleaseObject_delegate_index != 0)
	{
		LuaStackAutoPopup autoPopup(L);
		toluafix_get_function_by_refid(L, gReleaseObject_delegate_index);
		tolua_pushstring(L, account.c_str());
		tolua_pushcppstring(L, obj->guid());
		lua_pushlightuserdata(L, (void*)obj);
		lua_call(L, 3, 0);
	}
}

//注册对象管理lua接口
void lua_open_objects_manger(lua_State *L)
{
	int startPos = lua_gettop(L);
    luaL_Reg l[] = {
        {"get", luaGet},
        {"foreach",                 luaForeach},
        {"registerAttachObject",    luaRegisterAttachObject},
		{"registerAfterAttachObject",luaRegisterAfterAttachObject},
        {"registerReleaseObject",   luaRegisterReleaseObject},
		{"newAndCallPut",			luaNewAndCallPut},		
		{"callRemoveObject",		luaCallRemoveObject},
		{"callAddWatch",			luaCallAddWatch},
		{"callDelWatch",			luaCallDelWatch},
        {NULL, NULL}
    };
    luaL_register(L, "objects", l);	
	lua_settop(L,startPos);
}


