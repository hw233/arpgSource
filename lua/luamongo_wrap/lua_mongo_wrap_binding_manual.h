
#include <map>
#include <string>
#include <db_wrap/AsyncMongoDBTask.h>
#include <luacocos/tolua_fix.h>

using std::string;

void lua_get_string_map(lua_State* tolua_S,std::map<string,string>& _map)
{
	_map.clear();

	int t_idx = lua_gettop(tolua_S);
	lua_pushnil(tolua_S);
	while(lua_next(tolua_S, t_idx))
	{
		string  key = lua_tostring(tolua_S, -2);
		string values = lua_tostring(tolua_S, -1);
		lua_pop(tolua_S, 1);
		_map[key] = values;
	}
	lua_pop(tolua_S, 1);  
}

void lua_push_string_map(lua_State* tolua_S,const std::map<string,string>& _map)
{

	lua_newtable(tolua_S);  
	uint32_t i = 1;
	for(auto iter = _map.begin();iter!=_map.end();++iter)
	{
		//key
		lua_pushstring(tolua_S, iter->first.c_str());
		//values
		lua_pushstring(tolua_S,iter->second.c_str());	
		lua_rawset(tolua_S, -3); 
	}	
}

void luaPushResult(lua_State* tolua_S,const MongoWrap::Results& results)
{
	lua_newtable(tolua_S);//返回值，返回一个二维的table
	for (uint32_t i = 0;i<results.size();++i)
	{
		lua_pushnumber(tolua_S,i+1); //key
		lua_newtable(tolua_S);//压入value,也是一个table
		//map里面的元素
		uint32_t j=1;
		for (auto iter = results[i].begin();iter!= results[i].end();++iter)
		{
			//key
			lua_pushstring(tolua_S, iter->first.c_str());
			lua_rawseti(tolua_S,-2,j);
			j++;
			//values
			lua_pushstring(tolua_S,iter->second.c_str());
			lua_rawseti(tolua_S,-2,j);
			j++;
		}
		lua_settable(tolua_S, -3); 
	}
}

#define TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Insert00
/* method: Insert of class  MongoWrap */
static int tolua_lua_mongo_wrap_binding_MongoWrap_Insert00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_istable(tolua_S,3,0,&tolua_err)||//这边变为验证是否table(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::map<string,string>",0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
		std::map<string,string> values;
		//取出语句
		lua_get_string_map(tolua_S,values);

#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Insert'", NULL);
#endif
		{
			bool tolua_ret = (bool)  self->Insert(ns,values);
			tolua_pushboolean(tolua_S,(bool)tolua_ret); 
			//如需返回结果调用lua_push_string_map(tolua_S,push_map）
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Insert'.",&tolua_err);
	return 0;
#endif
}


#define TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Update00
/* method: Update of class  MongoWrap */
static int tolua_lua_mongo_wrap_binding_MongoWrap_Update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_istable(tolua_S,3,0,&tolua_err) ||
		!tolua_istable(tolua_S,4,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));

		std::map<string,string> values;
		//取出语句
		lua_get_string_map(tolua_S,values);

		std::map<string,string> where;
		//取出语句
		lua_get_string_map(tolua_S,where);

#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Update'", NULL);
#endif
		{
			bool tolua_ret = (bool)  self->Update(ns,where,values);
			tolua_pushboolean(tolua_S,(bool)tolua_ret);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Update'.",&tolua_err);
	return 0;
#endif
}

#define TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Delete00
/* method: Delete of class  MongoWrap */
static int tolua_lua_mongo_wrap_binding_MongoWrap_Delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_istable(tolua_S,3,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
		std::map<string,string> where;
		//取出语句
		lua_get_string_map(tolua_S,where);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Delete'", NULL);
#endif
		{
			bool tolua_ret = (bool)  self->Delete(ns,where);
			tolua_pushboolean(tolua_S,(bool)tolua_ret);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Delete'.",&tolua_err);
	return 0;
#endif
}

#define TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Load00
/* method: Load of class  MongoWrap */
static int tolua_lua_mongo_wrap_binding_MongoWrap_Load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_istable(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err) 
		)
		goto tolua_lerror;
	else
#endif
	{
		MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
		std::map<string,string> where;   
		//取出语句  
		lua_get_string_map(tolua_S,where);

		
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Load'", NULL);
#endif
		{
			std::map<string,string> result;
			Mongo_Result tolua_ret = (Mongo_Result)  self->Load(ns,where,result);
			tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
			lua_push_string_map(tolua_S,result);
		}
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Load'.",&tolua_err);
	return 0;
#endif
}

#define TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Query00
/* method: Query of class  MongoWrap */
static int tolua_lua_mongo_wrap_binding_MongoWrap_Query00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_istable(tolua_S,3,0,&tolua_err)||
		!tolua_istable(tolua_S,4,0,&tolua_err)||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
		std::map<string,string> outfield;
		lua_get_string_map(tolua_S,outfield);
		std::map<string,string> where;
		lua_get_string_map(tolua_S,where);


#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Query'", NULL);
#endif
		{
			MongoWrap::Results results;
			Mongo_Result tolua_ret = (Mongo_Result)  self->Query(ns,where,results,outfield);
			tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);//第一个返回值是结果
			//第二个是个二维的table	
			luaPushResult(tolua_S,results);
		}
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Query'.",&tolua_err);
	return 0;
#endif
}

#define TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Find00
static int tolua_lua_mongo_wrap_binding_MongoWrap_Find00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||		//ns
		!tolua_istable(tolua_S,3,0,&tolua_err)||			//where
		!tolua_iscppstring(tolua_S,4,0,&tolua_err) ||		//orderKey
		!tolua_isnumber(tolua_S,5,0,&tolua_err) ||			//bySort
		!tolua_isnumber(tolua_S,6,0,&tolua_err) ||			//limitResult
		!tolua_isnoobj(tolua_S,7,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));

		std::map<string,string> where;
		lua_get_string_map(tolua_S,where);

		string orderKey = ((string)  tolua_tocppstring(tolua_S,4,0));
		int bySort = (int32_t) tolua_tonumber(tolua_S,5,0);
		int limitResult = (int32_t) tolua_tonumber(tolua_S,6,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Find'", NULL);
#endif
		{
			MongoWrap::Results results;
			Mongo_Result tolua_ret = (Mongo_Result)  self->Query(ns,where,results,orderKey,bySort,limitResult);
			tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
			//第二个是个二维的table	
			luaPushResult(tolua_S,results);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'Find'.",&tolua_err);
	return 0;
#endif
}


//////////////////////////////////////////////////////////////////////////
//for AsyncMongoDB


/* method: AsyncUpdate of class  AsyncMongoDB */
#define  TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncUpdate00
static int tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"AsyncMongoDB",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_istable(tolua_S,3,0,&tolua_err)) ||
		(tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_istable(tolua_S,4,0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		AsyncMongoDB* self = (AsyncMongoDB*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));

		std::map<string,string> values;
		//取出语句
		lua_get_string_map(tolua_S,values);

		std::map<string,string> where;
		//取出语句
		lua_get_string_map(tolua_S,where);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsyncUpdate'", NULL);
#endif
		{
			self->AsyncUpdate(ns,where,values);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'AsyncUpdate'.",&tolua_err);
	return 0;
#endif
}



/* method: AsyncInsert of class  AsyncMongoDB */
#define TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncInsert00
static int tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncInsert00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"AsyncMongoDB",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_istable(tolua_S,3,0,&tolua_err)) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		AsyncMongoDB* self = (AsyncMongoDB*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));

		std::map<string,string> values;
		//取出语句
		lua_get_string_map(tolua_S,values);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsyncInsert'", NULL);
#endif
		{
			self->AsyncInsert(ns,values);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'AsyncInsert'.",&tolua_err);
	return 0;
#endif
}


/* method: AsyncQuery of class  AsyncMongoDB */
#define TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncQuery00
static int tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncQuery00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"AsyncMongoDB",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_istable(tolua_S,3,0,&tolua_err)) ||
		!toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,5,1,&tolua_err) ||
		!tolua_isnumber(tolua_S,6,1,&tolua_err) ||
		!tolua_isnumber(tolua_S,7,1,&tolua_err) ||		
		!tolua_isnoobj(tolua_S,8,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		AsyncMongoDB* self = (AsyncMongoDB*)  tolua_tousertype(tolua_S,1,0);
		string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
		std::map<string,string> values;
		//取出语句
		lua_get_string_map(tolua_S,values);

		int cb = ((int)  toluafix_ref_function(tolua_S,4,0));
		string orderKey = ((string)  tolua_tocppstring(tolua_S,5,"")); 
		int32_t bySort = (int32_t)tolua_tonumber(tolua_S,6,0);
		int32_t limitReslut = (int32_t)tolua_tonumber(tolua_S,7,0);
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsyncQuery'", NULL);
#endif
		{
			auto f = [tolua_S,cb](AsyncMongoDBTask* task){

				int statePos = lua_gettop( tolua_S );
				toluafix_get_function_by_refid(tolua_S,cb);
				luaPushResult(tolua_S,task->results);				
				lua_call(tolua_S,1,0); 				
				toluafix_remove_function_by_refid(tolua_S,cb);

				lua_settop(tolua_S,statePos);		//恢复栈区								
			};
			self->AsyncQuery(ns,values,f,orderKey,bySort,limitReslut);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'AsyncQuery'.",&tolua_err);
	return 0;
#endif
}