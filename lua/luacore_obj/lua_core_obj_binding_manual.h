#ifndef _LUA_CORE_OBJ_BINDING_MANUAL_H_
#define _LUA_CORE_OBJ_BINDING_MANUAL_H_

#include "tolua++.h"
#include "tolua_fix.h"
#include "LuaGuidObjectTable.h"

#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallAddWatch00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallDelWatch00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallWatchObjects00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallRemoveObject00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallPutObject00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00_local
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_HandlePacket00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterCreateEvent00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterReleaseEvent00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_ForeachIndexer00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterCreateObject00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterReleaseObject00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_ApplyBlock00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_ToString00

#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_after_update00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListen00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListenString00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelListen00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelScriptHanlder00
#define TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readUTFCheckLen00


//void lua_get_string_map(lua_State* tolua_S,std::map<string,string>& _map)
//{
//	_map.clear();
//
//	int t_idx = lua_gettop(tolua_S);
//	lua_pushnil(tolua_S);
//	while(lua_next(tolua_S, t_idx))
//	{
//		string  key = lua_tostring(tolua_S, -2);
//		string values = lua_tostring(tolua_S, -1);
//		lua_pop(tolua_S, 1);
//		_map[key] = values;
//	}
//	lua_pop(tolua_S, 1);  
//}
//
//extern void lua_push_string_map(lua_State* tolua_S,const std::map<string,string>& _map);
//{
//
//	lua_newtable(tolua_S);  
//	uint32_t i = 1;
//	for(auto iter = _map.begin();iter!=_map.end();++iter)
//	{
//		//key
//		lua_pushstring(tolua_S, iter->first.c_str());
//		//values
//		lua_pushstring(tolua_S,iter->second.c_str());	
//		lua_rawset(tolua_S, -3); 
//	}	
//}

static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterCreateObject00(lua_State* tolua_S) 
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err; 
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::LuaGuidObjectTable* self = (core_obj::LuaGuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
		int cb_idx = toluafix_ref_function(tolua_S,2,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LuaRegisterCreateObject'", NULL);
#endif
		{
			self->RegisterCreateObject(tolua_S,cb_idx);
			//注册回调,有一个返回值,是否注册成功
			lua_pushinteger(tolua_S,cb_idx);
			return 1;
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'LuaRegisterCreateObject'.",&tolua_err);
	return 0;
#endif
}

static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterReleaseObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err; 
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::LuaGuidObjectTable* self = (core_obj::LuaGuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
		int cb_idx = toluafix_ref_function(tolua_S,2,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LuaRegisterReleaseObject'", NULL);
#endif
		{
			self->RegisterReleaseObject(tolua_S,cb_idx);
			//注册回调,有一个返回值,是否注册成功
			lua_pushinteger(tolua_S,cb_idx);
			return 1;
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'LuaRegisterReleaseObject'.",&tolua_err);
	return 0;
#endif
}

/* method: ForeachIndexer of class  core_obj::LuaGuidObjectTable */
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_ForeachIndexer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::LuaGuidObjectTable* self = (core_obj::LuaGuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
		string exp = ((string)  tolua_tocppstring(tolua_S,2,0));
		int f = toluafix_ref_function(tolua_S,3,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ForeachIndexer'", NULL);
#endif
		{
			self->ForeachIndexer(exp,[&](core_obj::GuidObject *obj)->bool{
				int statePos = lua_gettop( tolua_S );
				toluafix_get_function_by_refid(tolua_S,f);
				//tolua_pushusertype(tolua_S,(void*)self,"core_obj::LuaGuidObjectTable");
				tolua_pushusertype(tolua_S,(void*)obj,"core_obj::GuidObject");
				lua_call(tolua_S,1,1); 
				int result = (int)tolua_tonumber(tolua_S,-1,NULL);
				lua_settop(tolua_S,statePos);		//恢复栈区
				return result != 0;
			});
		}
		//去掉该函数的引用
		toluafix_remove_function_by_refid(tolua_S,f);
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'ForeachIndexer'.",&tolua_err);
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
//for GuidObject

/* method: LuaRegisterCreateEvent of class  core_obj::GuidObjectTable */
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterCreateEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE   
	tolua_Error tolua_err; 
	if ( 
		!tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) || 
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)  ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
		string guid = ((string)  tolua_tocppstring(tolua_S,2,0));
		int cb_idx = toluafix_ref_function(tolua_S, 3, 0);;			//回调序号
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LuaRegisterCreateEvent'", NULL);
#endif
		{ 
			//self->LuaRegisterCreateEvent(guid,cb_idx);
			self->RegisterCreateEvent(guid,[tolua_S,cb_idx](core_obj::GuidObject *obj){
				int statePos = lua_gettop( tolua_S );
				toluafix_get_function_by_refid(tolua_S,cb_idx);
				if(lua_isfunction(tolua_S,-1)){ 
					//只有一个参数是包
					tolua_pushusertype(tolua_S,(void*)obj,"core_obj::GuidObject");
					//TODO:这里应该使用安全调用
					lua_call(tolua_S,1,0);
				}
				//把栈还原了
				lua_settop(tolua_S,statePos);	
			});
			lua_pushinteger(tolua_S,cb_idx);
			return 1;
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'LuaRegisterCreateEvent'.",&tolua_err);
	return 0;
#endif
}

/* method: LuaRegisterReleaseEvent of class  core_obj::GuidObjectTable */
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterReleaseEvent00(lua_State* tolua_S)
{ 
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err; 
	if (   
		!tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
		string guid = ((string)  tolua_tocppstring(tolua_S,2,0));
		int cb_idx = toluafix_ref_function(tolua_S, 3, 0);;			//回调序号
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LuaRegisterReleaseEvent'", NULL);
#endif
		{ 
			//self->LuaRegisterReleaseEvent(guid,cb_idx);
			self->RegisterReleaseEvent(guid,[tolua_S,cb_idx](core_obj::GuidObject *obj){
				int statePos = lua_gettop( tolua_S );
				toluafix_get_function_by_refid(tolua_S,cb_idx);
				if(lua_isfunction(tolua_S,-1)){
					//只有一个参数是包
					tolua_pushusertype(tolua_S,(void*)obj,"core_obj::GuidObject");
					//TODO:这里应该使用安全调用
					lua_call(tolua_S,1,0);
				}
				//把栈还原了
				lua_settop(tolua_S,statePos);	
			});
			lua_pushinteger(tolua_S,cb_idx);
			return 1;
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'LuaRegisterReleaseEvent'.",&tolua_err);
	return 0;
#endif
}

/* method: new of class  core_obj::RemoteOperatorAdapter */
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"core_obj::GuidObjectTable",0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,5,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,6,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::GuidObjectTable* t = ((core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,2,0));
		int send_func = ((int)  toluafix_ref_function(tolua_S,3,0));
		int opt_ctl = ((int)  tolua_tonumber(tolua_S,4,0));
		int opt_up = ((int)  tolua_tonumber(tolua_S,5,0));
		int opt_ctl_ret = ((int)  tolua_tonumber(tolua_S,6,0));

		{
			auto f = [send_func,tolua_S](uint32_t connid,core_obj::ByteArray& bytes){
				int statePos = lua_gettop( tolua_S );
				toluafix_get_function_by_refid(tolua_S,send_func);
				if(lua_isfunction(tolua_S,-1)){
					tolua_pushnumber(tolua_S,connid);
					tolua_pushusertype(tolua_S,(void*)&bytes,"core_obj::ByteArray");
					//TODO:这里应该使用安全调用
					lua_call(tolua_S,2,0);
				}
				//把栈还原了
				lua_settop(tolua_S,statePos);	
			};

			core_obj::RemoteOperatorAdapter* tolua_ret = (core_obj::RemoteOperatorAdapter*)  Mtolua_new((core_obj::RemoteOperatorAdapter)(t,f,opt_ctl,opt_up,opt_ctl_ret));
			tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::RemoteOperatorAdapter");
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
	return 0;
#endif
}

/* method: new_local of class  core_obj::RemoteOperatorAdapter */
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertable(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"core_obj::GuidObjectTable",0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,4,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,5,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,6,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::GuidObjectTable* t = ((core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,2,0));
		int send_func = ((int)  toluafix_ref_function(tolua_S,3,0));
		int opt_ctl = ((int)  tolua_tonumber(tolua_S,4,0));
		int opt_up = ((int)  tolua_tonumber(tolua_S,5,0));
		int opt_ctl_ret = ((int)  tolua_tonumber(tolua_S,6,0));
		{
			auto f = [send_func,tolua_S](uint32_t connid,core_obj::ByteArray& bytes){
				int statePos = lua_gettop( tolua_S );
				toluafix_get_function_by_refid(tolua_S,send_func);
				if(lua_isfunction(tolua_S,-1)){
					tolua_pushnumber(tolua_S,connid);
					tolua_pushusertype(tolua_S,(void*)&bytes,"core_obj::ByteArray");
					//TODO:这里应该使用安全调用
					lua_call(tolua_S,2,0);
				}
				//把栈还原了
				lua_settop(tolua_S,statePos);
			};

			core_obj::RemoteOperatorAdapter* tolua_ret = (core_obj::RemoteOperatorAdapter*)  Mtolua_new((core_obj::RemoteOperatorAdapter)(t,f,opt_ctl,opt_up,opt_ctl_ret));
			tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::RemoteOperatorAdapter");
			tolua_register_gc(tolua_S,lua_gettop(tolua_S));
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
	return 0;
#endif
}

/* method: CallPutObject of class  core_obj::RemoteOperatorAdapter */
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallPutObject00(lua_State* tolua_S)
{
	int n = lua_gettop(tolua_S);
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"core_obj::GuidObject",0,&tolua_err) ||
		(n > 2 && !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		(n > 3 && !tolua_isboolean(tolua_S,4,1,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,n+1,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
		core_obj::GuidObject* obj = ((core_obj::GuidObject*)  tolua_tousertype(tolua_S,2,0));
		int cb = n > 2 ? ((int)  toluafix_ref_function(tolua_S,3,0)) : 0;
		bool add_notice =  n > 3 ? ( tolua_toboolean(tolua_S,4,0) != 0) : true;
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallPutObject'", NULL);
#endif
		{
			if(n>2)
			{
				auto f = [cb,tolua_S](bool b){
					int statePos = lua_gettop( tolua_S );
					toluafix_get_function_by_refid(tolua_S,cb);
					if(lua_isfunction(tolua_S,-1)){
						tolua_pushboolean(tolua_S, b);
						//TODO:这里应该使用安全调用
						lua_call(tolua_S,1,0);
					}
					//把栈还原了
					lua_settop(tolua_S,statePos);	
					//一次性回调去掉函数引用
					toluafix_remove_function_by_refid(tolua_S,cb);
				};
				self->CallPutObject(obj,f,add_notice);
			}
			else
			{
				self->CallPutObject(obj,nullptr,add_notice);
			}
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'CallPutObject'.",&tolua_err);
	return 0;
#endif
}

/* method: CallRemoveObject of class  core_obj::RemoteOperatorAdapter */
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallRemoveObject00(lua_State* tolua_S)
{
	int n = lua_gettop(tolua_S);
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		(n > 2 && !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,n+1,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
		string key = ((string)  tolua_tocppstring(tolua_S,2,0));
		int cb = n > 2 ? ((int)  toluafix_ref_function(tolua_S,3,0)) : 0;
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallRemoveObject'", NULL);
#endif
		{
			if(n>2)
			{
				auto f = [cb,tolua_S](core_obj::GuidObject* obj){
					int statePos = lua_gettop( tolua_S );
					toluafix_get_function_by_refid(tolua_S,cb);
					if(lua_isfunction(tolua_S,-1)){
						tolua_pushusertype(tolua_S,(void*)obj,"core_obj::GuidObject");
						//TODO:这里应该使用安全调用
						lua_call(tolua_S,1,0);
					}
					//把栈还原了
					lua_settop(tolua_S,statePos);	
					//一次性回调去掉函数引用
					toluafix_remove_function_by_refid(tolua_S,cb);
				};
				self->CallRemoveObject(key,f);
			}
			else
			{
				self->CallRemoveObject(key);
			}
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'CallRemoveObject'.",&tolua_err);
	return 0;
#endif
}

/* method: CallWatchObjects of class  core_obj::RemoteOperatorAdapter */
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallWatchObjects00(lua_State* tolua_S)
{
	int n = lua_gettop(tolua_S);
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		(n > 2 && !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,n+1,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
		string query_str = ((string)  tolua_tocppstring(tolua_S,2,0));
		int cb_func = n > 2 ? ((int)  toluafix_ref_function(tolua_S,3,0)) : 0;
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallWatchObjects'", NULL);
#endif
		{
			if(n>2)
			{
				auto f = [cb_func,tolua_S](){
					int statePos = lua_gettop( tolua_S );
					toluafix_get_function_by_refid(tolua_S,cb_func);
					if(lua_isfunction(tolua_S,-1)){
						//TODO:这里应该使用安全调用
						lua_call(tolua_S,0,0);
					}
					//把栈还原了
					lua_settop(tolua_S,statePos);	
					//一次性回调去掉函数引用
					toluafix_remove_function_by_refid(tolua_S,cb_func);
				};
				self->CallWatchObjects(query_str,f);
			}
			else
			{
				self->CallWatchObjects(query_str);
			}
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'CallWatchObjects'.",&tolua_err);
	return 0;
#endif
}

/* method: CallDelWatch of class  core_obj::RemoteOperatorAdapter */
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallDelWatch00(lua_State* tolua_S)
{
	int n = lua_gettop(tolua_S);
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		(n>2 && !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
		(n>3 && !tolua_isnumber(tolua_S,4,0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,n+1,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
		string key = ((string)  tolua_tocppstring(tolua_S,2,0));
		int cb_func = ((int)  toluafix_ref_function(tolua_S,3,0));
		unsigned int fd = n > 3 ? (( unsigned int)  tolua_tonumber(tolua_S,4,0)) : 0;
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallDelWatch'", NULL);
#endif
		{
			auto f = [cb_func,tolua_S](bool b){
				int statePos = lua_gettop( tolua_S );
				toluafix_get_function_by_refid(tolua_S,cb_func);
				if(lua_isfunction(tolua_S,-1)){
					tolua_pushboolean(tolua_S,b);
					//TODO:这里应该使用安全调用
					lua_call(tolua_S,1,0);
				}
				//把栈还原了
				lua_settop(tolua_S,statePos);	
				//一次性回调去掉函数引用
				toluafix_remove_function_by_refid(tolua_S,cb_func);
			};
			if(n>3)
			{
				self->CallDelWatch(fd,key,f);
			}
			else
			{
				self->CallDelWatch(key,f);
			}
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'CallDelWatch'.",&tolua_err);
	return 0;
#endif
}

/* method: CallAddWatch of class  core_obj::RemoteOperatorAdapter */
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallAddWatch00(lua_State* tolua_S)
{
	int cb_func = 0;
	bool createblock = true;
	unsigned int fd = 0;

	int n = lua_gettop(tolua_S);
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
//		(n>2 && !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
//		(n>3 && !tolua_isboolean(tolua_S,4,0,&tolua_err)) ||
//		(n>4 && !tolua_isnumber(tolua_S,5,0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,n+1,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
		string key = ((string)  tolua_tocppstring(tolua_S,2,0));
		if(n>2 && toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err) )
			cb_func = ((int)  toluafix_ref_function(tolua_S,3,0));
		if(n>3 && tolua_isboolean(tolua_S,4,0,&tolua_err) )
			createblock = ( tolua_toboolean(tolua_S,4,0) != 0);
		if(n>4 && tolua_isnumber(tolua_S,5,0,&tolua_err))
			fd = ( unsigned int)  tolua_tonumber(tolua_S,5,0);	
		
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallAddWatch'", NULL);
#endif
		{			
			std::function<void(bool)> f;
			if(cb_func)
			{
				f = [cb_func,tolua_S](bool b){
					int statePos = lua_gettop( tolua_S );
					toluafix_get_function_by_refid(tolua_S,cb_func);
					if(lua_isfunction(tolua_S,-1)){
						tolua_pushboolean(tolua_S,b);
						//TODO:这里应该使用安全调用
						lua_call(tolua_S,1,0);
					}
					//把栈还原了
					lua_settop(tolua_S,statePos);
					//一次性回调去掉函数引用
					toluafix_remove_function_by_refid(tolua_S,cb_func);
				};
			}			

			if(n>4)
			{
				self->CallAddWatch(fd,key,f,createblock);
			}
			else
			{
				self->CallAddWatch(key,f,createblock);
			}
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'CallAddWatch'.",&tolua_err);
	return 0;
#endif
}

/* method: HandlePacket of class  core_obj::RemoteOperatorAdapter */
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_HandlePacket00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnumber(tolua_S,3,0,&tolua_err) ||
		!tolua_isusertype(tolua_S,4,"core_obj::ByteArray",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
		unsigned int cid = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
		int opt = ((int)  tolua_tonumber(tolua_S,3,0));
		core_obj::ByteArray* buff = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'HandlePacket'", NULL);
#endif
		{
			self->HandlePacket(cid,opt,*buff);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'HandlePacket'.",&tolua_err);
	return 0;
#endif
}


static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_ApplyBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
		!tolua_isusertype(tolua_S,2,"core_obj::ByteArray",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
		core_obj::ByteArray* bytes = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ApplyBlock'", NULL);
#endif
		{
			bool tolua_ret = (bool)  self->ApplyBlock(*bytes);
			tolua_pushboolean(tolua_S,(bool)tolua_ret);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'ApplyBlock'.",&tolua_err);
	return 0;
#endif
}



/* method: ToString of class  core_obj::SyncEventRecorder */
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_ToString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err)||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
		string ints ,strs;
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToString'", NULL);
#endif
		{
			self->ToString(ints,strs);
			tolua_pushcppstring(tolua_S,(const char*)ints);
			tolua_pushcppstring(tolua_S,(const char*)strs);
		}
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'ToString'.",&tolua_err);
	return 0;
#endif
}

/* method: after_update of class  core_obj::LuaGuidObject */

static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_after_update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
		//!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*)  tolua_tousertype(tolua_S,1,0);		
		int f = ((int)  toluafix_ref_function(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'after_update'", NULL);
#endif
		{
			self->RegisterAfterUpdate(tolua_S,f);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'after_update'.",&tolua_err);
	return 0;
#endif
}

/* method: AddListen of class  core_obj::LuaGuidObject */
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;  
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror; 
	else
#endif
	{
		core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*)  tolua_tousertype(tolua_S,1,0);
		int index = ((int)  tolua_tonumber(tolua_S,2,0));
		int hanlder = ((int)  toluafix_ref_function(tolua_S,3,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddListen'", NULL);
#endif
		{
			self->RegisterHanlder(tolua_S,index,hanlder);
			tolua_pushnumber(tolua_S,hanlder);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'AddListen'.",&tolua_err);
	return 0;
#endif
}

/* method: AddListenString of class  core_obj::LuaGuidObject */
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListenString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*)  tolua_tousertype(tolua_S,1,0);
		int index = -1*((int)  tolua_tonumber(tolua_S,2,0));
		int hanlder = ((int)  toluafix_ref_function(tolua_S,3,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddListenString'", NULL);
#endif
		{
			self->RegisterHanlder(tolua_S,index,hanlder);
			tolua_pushnumber(tolua_S,hanlder);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'AddListenString'.",&tolua_err);
	return 0;
#endif
}

/* method: DelListen of class  core_obj::LuaGuidObject */
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelListen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*)  tolua_tousertype(tolua_S,1,0);
		int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DelListen'", NULL);
#endif
		{
			self->UnRegisterHanlderByIndex(index);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'DelListen'.",&tolua_err);
	return 0;
#endif
}

/* method: DelScriptHanlder of class  core_obj::LuaGuidObject */

static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelScriptHanlder00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,3,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*)  tolua_tousertype(tolua_S,1,0);
		uint32_t hanlder = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DelScriptHanlder'", NULL);
#endif
		{
			self->UnRegisterHanlder(hanlder);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'DelScriptHanlder'.",&tolua_err);
	return 0;
#endif
}


static int tolua_lua_core_obj_binding_core_obj_ByteArray_readUTFCheckLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,2,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readUTFCheckLen'", NULL);
#endif
		{
			int8_t is_enough_long;
			string tolua_ret = (string)  self->readUTF(is_enough_long);
			tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
			tolua_pushnumber(tolua_S,is_enough_long);
		}
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'readUTFCheckLen'.",&tolua_err);
	return 0;
#endif
}

#endif
