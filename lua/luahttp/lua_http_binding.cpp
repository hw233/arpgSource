/*
** Lua binding: lua_http_binding
** Generated automatically by tolua++-1.0.92 on 01/05/16 20:29:01.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_lua_http_binding_open (lua_State* tolua_S);

#include "lua_http_binding_manual.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_http__server__server (lua_State* tolua_S)
{
 http::server::server* self = (http::server::server*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_http__client__client (lua_State* tolua_S)
{
 http::client::client* self = (http::client::client*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"http::server::request_handler");
 tolua_usertype(tolua_S,"http::server::request");
 tolua_usertype(tolua_S,"http::client::client");
 tolua_usertype(tolua_S,"http::server::reply");
 tolua_usertype(tolua_S,"http::server::server");
}

/* method: new of class  http::client::client */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_client_client_new00
static int tolua_lua_http_binding_http_client_client_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"http::client::client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   http::client::client* tolua_ret = (http::client::client*)  Mtolua_new((http::client::client)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"http::client::client");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  http::client::client */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_client_client_new00_local
static int tolua_lua_http_binding_http_client_client_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"http::client::client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   http::client::client* tolua_ret = (http::client::client*)  Mtolua_new((http::client::client)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"http::client::client");
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
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  http::client::client */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_client_client_delete00
static int tolua_lua_http_binding_http_client_client_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"http::client::client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  http::client::client* self = (http::client::client*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: async_post of class  http::client::client */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_client_client_async_post00
static int tolua_lua_http_binding_http_client_client_async_post00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"http::client::client",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  http::client::client* self = (http::client::client*)  tolua_tousertype(tolua_S,1,0);
  string url = ((string)  tolua_tocppstring(tolua_S,2,0));
  string data = ((string)  tolua_tocppstring(tolua_S,3,0));
  int cb = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'async_post'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->async_post(url,data,cb);
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
#endif //#ifndef TOLUA_DISABLE

/* method: poll of class  http::client::client */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_client_client_poll00
static int tolua_lua_http_binding_http_client_client_poll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"http::client::client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  http::client::client* self = (http::client::client*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'poll'", NULL);
#endif
  {
   self->poll();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'poll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: method of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_get_http__server__request_method
static int tolua_get_http__server__request_method(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'method'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->method);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: method of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_set_http__server__request_method
static int tolua_set_http__server__request_method(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'method'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->method = ((string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: uri of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_get_http__server__request_uri
static int tolua_get_http__server__request_uri(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uri'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->uri);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: uri of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_set_http__server__request_uri
static int tolua_set_http__server__request_uri(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'uri'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->uri = ((string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: query_string of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_get_http__server__request_query_string
static int tolua_get_http__server__request_query_string(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'query_string'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->query_string);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: query_string of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_set_http__server__request_query_string
static int tolua_set_http__server__request_query_string(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'query_string'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->query_string = ((string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: content of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_get_http__server__request_content
static int tolua_get_http__server__request_content(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'content'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->content);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: content of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_set_http__server__request_content
static int tolua_set_http__server__request_content(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'content'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->content = ((string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: remote_ip of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_get_http__server__request_remote_ip
static int tolua_get_http__server__request_remote_ip(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'remote_ip'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->remote_ip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: remote_ip of class  http::server::request */
#ifndef TOLUA_DISABLE_tolua_set_http__server__request_remote_ip
static int tolua_set_http__server__request_remote_ip(lua_State* tolua_S)
{
  http::server::request* self = (http::server::request*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'remote_ip'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->remote_ip = ((string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: status of class  http::server::reply */
#ifndef TOLUA_DISABLE_tolua_get_http__server__reply_status
static int tolua_get_http__server__reply_status(lua_State* tolua_S)
{
  http::server::reply* self = (http::server::reply*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'status'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->status);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: status of class  http::server::reply */
#ifndef TOLUA_DISABLE_tolua_set_http__server__reply_status
static int tolua_set_http__server__reply_status(lua_State* tolua_S)
{
  http::server::reply* self = (http::server::reply*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'status'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->status = ((http::server::reply::status_type) (int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: content of class  http::server::reply */
#ifndef TOLUA_DISABLE_tolua_get_http__server__reply_content
static int tolua_get_http__server__reply_content(lua_State* tolua_S)
{
  http::server::reply* self = (http::server::reply*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'content'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->content);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: content of class  http::server::reply */
#ifndef TOLUA_DISABLE_tolua_set_http__server__reply_content
static int tolua_set_http__server__reply_content(lua_State* tolua_S)
{
  http::server::reply* self = (http::server::reply*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'content'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->content = ((string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_hanlde of class  http::server::request_handler */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_server_request_handler_add_hanlde00
static int tolua_lua_http_binding_http_server_request_handler_add_hanlde00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"http::server::request_handler",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  http::server::request_handler* self = (http::server::request_handler*)  tolua_tousertype(tolua_S,1,0);
  string uri = ((string)  tolua_tocppstring(tolua_S,2,0));
  int callback = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_hanlde'", NULL);
#endif
  {
   self->add_hanlde(uri,callback);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_hanlde'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  http::server::server */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_server_server_new00
static int tolua_lua_http_binding_http_server_server_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"http::server::server",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string address = ((string)  tolua_tocppstring(tolua_S,2,0));
  string port = ((string)  tolua_tocppstring(tolua_S,3,0));
  string doc_root = ((string)  tolua_tocppstring(tolua_S,4,0));
  {
   http::server::server* tolua_ret = (http::server::server*)  Mtolua_new((http::server::server)(address,port,doc_root));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"http::server::server");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  http::server::server */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_server_server_new00_local
static int tolua_lua_http_binding_http_server_server_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"http::server::server",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  string address = ((string)  tolua_tocppstring(tolua_S,2,0));
  string port = ((string)  tolua_tocppstring(tolua_S,3,0));
  string doc_root = ((string)  tolua_tocppstring(tolua_S,4,0));
  {
   http::server::server* tolua_ret = (http::server::server*)  Mtolua_new((http::server::server)(address,port,doc_root));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"http::server::server");
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
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  http::server::server */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_server_server_delete00
static int tolua_lua_http_binding_http_server_server_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"http::server::server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  http::server::server* self = (http::server::server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: poll of class  http::server::server */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_server_server_poll00
static int tolua_lua_http_binding_http_server_server_poll00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"http::server::server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  http::server::server* self = (http::server::server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'poll'", NULL);
#endif
  {
   self->poll();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'poll'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_request_handler of class  http::server::server */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_server_server_get_request_handler00
static int tolua_lua_http_binding_http_server_server_get_request_handler00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"http::server::server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  http::server::server* self = (http::server::server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_request_handler'", NULL);
#endif
  {
   http::server::request_handler& tolua_ret = (http::server::request_handler&)  self->get_request_handler();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"http::server::request_handler");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_request_handler'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_port of class  http::server::server */
#ifndef TOLUA_DISABLE_tolua_lua_http_binding_http_server_server_get_port00
static int tolua_lua_http_binding_http_server_server_get_port00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"http::server::server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  http::server::server* self = (http::server::server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_port'", NULL);
#endif
  {
   unsigned short tolua_ret = (unsigned short)  self->get_port();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_port'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_lua_http_binding_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"http",0);
  tolua_beginmodule(tolua_S,"http");
   tolua_module(tolua_S,"client",0);
   tolua_beginmodule(tolua_S,"client");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"client","http::client::client","",tolua_collect_http__client__client);
    #else
    tolua_cclass(tolua_S,"client","http::client::client","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"client");
     tolua_function(tolua_S,"new",tolua_lua_http_binding_http_client_client_new00);
     tolua_function(tolua_S,"new_local",tolua_lua_http_binding_http_client_client_new00_local);
     tolua_function(tolua_S,".call",tolua_lua_http_binding_http_client_client_new00_local);
     tolua_function(tolua_S,"delete",tolua_lua_http_binding_http_client_client_delete00);
     tolua_function(tolua_S,"post",tolua_lua_http_binding_http_client_client_post00);
     tolua_function(tolua_S,"async_post",tolua_lua_http_binding_http_client_client_async_post00);
     tolua_function(tolua_S,"poll",tolua_lua_http_binding_http_client_client_poll00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
   tolua_module(tolua_S,"server",0);
   tolua_beginmodule(tolua_S,"server");
    tolua_cclass(tolua_S,"request","http::server::request","",NULL);
    tolua_beginmodule(tolua_S,"request");
     tolua_variable(tolua_S,"method",tolua_get_http__server__request_method,tolua_set_http__server__request_method);
     tolua_variable(tolua_S,"uri",tolua_get_http__server__request_uri,tolua_set_http__server__request_uri);
     tolua_variable(tolua_S,"query_string",tolua_get_http__server__request_query_string,tolua_set_http__server__request_query_string);
     tolua_variable(tolua_S,"content",tolua_get_http__server__request_content,tolua_set_http__server__request_content);
     tolua_variable(tolua_S,"remote_ip",tolua_get_http__server__request_remote_ip,tolua_set_http__server__request_remote_ip);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"reply","http::server::reply","",NULL);
    tolua_beginmodule(tolua_S,"reply");
     tolua_constant(tolua_S,"ok",http::server::reply::ok);
     tolua_constant(tolua_S,"created",http::server::reply::created);
     tolua_constant(tolua_S,"accepted",http::server::reply::accepted);
     tolua_constant(tolua_S,"no_content",http::server::reply::no_content);
     tolua_constant(tolua_S,"multiple_choices",http::server::reply::multiple_choices);
     tolua_constant(tolua_S,"moved_permanently",http::server::reply::moved_permanently);
     tolua_constant(tolua_S,"moved_temporarily",http::server::reply::moved_temporarily);
     tolua_constant(tolua_S,"not_modified",http::server::reply::not_modified);
     tolua_constant(tolua_S,"bad_request",http::server::reply::bad_request);
     tolua_constant(tolua_S,"unauthorized",http::server::reply::unauthorized);
     tolua_constant(tolua_S,"forbidden",http::server::reply::forbidden);
     tolua_constant(tolua_S,"not_found",http::server::reply::not_found);
     tolua_constant(tolua_S,"internal_server_error",http::server::reply::internal_server_error);
     tolua_constant(tolua_S,"not_implemented",http::server::reply::not_implemented);
     tolua_constant(tolua_S,"bad_gateway",http::server::reply::bad_gateway);
     tolua_constant(tolua_S,"service_unavailable",http::server::reply::service_unavailable);
     tolua_variable(tolua_S,"status",tolua_get_http__server__reply_status,tolua_set_http__server__reply_status);
     tolua_variable(tolua_S,"content",tolua_get_http__server__reply_content,tolua_set_http__server__reply_content);
    tolua_endmodule(tolua_S);
    tolua_cclass(tolua_S,"request_handler","http::server::request_handler","",NULL);
    tolua_beginmodule(tolua_S,"request_handler");
     tolua_function(tolua_S,"add_hanlde",tolua_lua_http_binding_http_server_request_handler_add_hanlde00);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"server","http::server::server","",tolua_collect_http__server__server);
    #else
    tolua_cclass(tolua_S,"server","http::server::server","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"server");
     tolua_function(tolua_S,"new",tolua_lua_http_binding_http_server_server_new00);
     tolua_function(tolua_S,"new_local",tolua_lua_http_binding_http_server_server_new00_local);
     tolua_function(tolua_S,".call",tolua_lua_http_binding_http_server_server_new00_local);
     tolua_function(tolua_S,"delete",tolua_lua_http_binding_http_server_server_delete00);
     tolua_function(tolua_S,"poll",tolua_lua_http_binding_http_server_server_poll00);
     tolua_function(tolua_S,"get_request_handler",tolua_lua_http_binding_http_server_server_get_request_handler00);
     tolua_function(tolua_S,"get_port",tolua_lua_http_binding_http_server_server_get_port00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_lua_http_binding (lua_State* tolua_S) {
 return tolua_lua_http_binding_open(tolua_S);
};
#endif

