#ifndef _LUA_CORE_OBJ_BINDING_MANUAL_H_
#define _LUA_CORE_OBJ_BINDING_MANUAL_H_

#include "tolua++.h"
#include "tolua_fix.h"

#include "server/server.hpp"
#include "client/http_post.h"
#include "server/request_handler.hpp"

//////////////////////////////////////////////////////////////////////////
#define TOLUA_DISABLE_tolua_lua_http_binding_http_client_client_async_post00

/* method: async_post of class  http::client::client */
static int tolua_lua_http_binding_http_client_client_async_post00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"http::client::client",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,4,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,5,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		http::client::client* self = (http::client::client*)  tolua_tousertype(tolua_S,1,0);
		string url = ((string)  tolua_tocppstring(tolua_S,2,0));
		string data = ((string)  tolua_tocppstring(tolua_S,3,0));
		int cb = ((int)  toluafix_ref_function(tolua_S,4,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'async_post'", NULL);
#endif
		{
			//TODO:是否应该把回调记录下来,如果lua栈已被释放,必须将回调删除
			bool tolua_ret = (bool) self->async_post(url,data,[cb,tolua_S](int status_code,const std::string& data){
				int statePos = lua_gettop( tolua_S );

				//将回调函数恢复到栈顶
				toluafix_get_function_by_refid(tolua_S,cb);
				if(lua_isfunction(tolua_S,-1)){
					//只有一个参数是包
					tolua_pushnumber(tolua_S, status_code);
					tolua_pushcppstring(tolua_S, data);
					//TODO:这里应该使用安全调用
					lua_call(tolua_S,2,0);
				}

				//回调后移除注册
				toluafix_remove_function_by_refid(tolua_S,cb);

				//恢复lua栈状态
				lua_settop(tolua_S,statePos);	
			});
			tolua_pushboolean(tolua_S,(bool)tolua_ret);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'async_post'.",&tolua_err);
	return 0;
#endif
}

/* method: post of class  http::client::client */
#define TOLUA_DISABLE_tolua_lua_http_binding_http_client_client_post00

static int tolua_lua_http_binding_http_client_client_post00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"http::client::client",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		http::client::client* self = (http::client::client*)  tolua_tousertype(tolua_S,1,0);
		string url = ((string)  tolua_tocppstring(tolua_S,2,0));
		string data = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'post'", NULL);
#endif
		{
			string reponse;
			int tolua_ret = (int)  self->post(url,data,reponse);
			tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
			tolua_pushcppstring(tolua_S,reponse);
		}
	}
	return 2;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'post'.",&tolua_err);
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////


/* method: add_hanlde of class  http::server::request_handler */
#define  TOLUA_DISABLE_tolua_lua_http_binding_http_server_request_handler_add_hanlde00

static int tolua_lua_http_binding_http_server_request_handler_add_hanlde00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(tolua_S,1,"http::server::request_handler",0,&tolua_err) ||
		!tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
		!toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err) ||
		!tolua_isnoobj(tolua_S,4,&tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		http::server::request_handler* self = (http::server::request_handler*)  tolua_tousertype(tolua_S,1,0);
		string uri = ((string)  tolua_tocppstring(tolua_S,2,0));
		int cb = ((int)  toluafix_ref_function(tolua_S,3,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_hanlde'", NULL);
#endif
		{
			using http::server::request;
			using http::server::reply;

			self->add_hanlde(uri,[tolua_S,cb](const request& req,reply& rep){
				int statePos = lua_gettop( tolua_S );
				//将回调函数恢复到栈顶
				toluafix_get_function_by_refid(tolua_S,cb);
				if(lua_isfunction(tolua_S,-1)){
					//只有一个参数是包
					tolua_pushusertype(tolua_S, (void*)&req, "http::server::request");
					tolua_pushusertype(tolua_S, (void*)&rep, "http::server::reply");			
					//TODO:这里应该使用安全调用
					lua_call(tolua_S,2,0);
				}
				//恢复lua栈状态
				lua_settop(tolua_S,statePos);	
			});
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'add_hanlde'.",&tolua_err);
	return 0;
#endif
}

#endif
