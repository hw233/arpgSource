/*
** Lua binding: lua_mongo_wrap_binding
** Generated automatically by tolua++-1.0.92 on 09/14/15 17:00:12.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_lua_mongo_wrap_binding_open (lua_State* tolua_S);

#include <db_wrap/mongo_wrap.h>
#include "lua_mongo_wrap_binding_manual.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_MongoWrap (lua_State* tolua_S)
{
 MongoWrap* self = (MongoWrap*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"int32_t");
 tolua_usertype(tolua_S,"AsyncMongoDB");
 tolua_usertype(tolua_S,"MongoWrap");
 tolua_usertype(tolua_S,"Map");
 tolua_usertype(tolua_S,"std::vector<MongoWrap::Map>");
 tolua_usertype(tolua_S,"std::map<string,string>");
 tolua_usertype(tolua_S,"MongoDB::Map");
}

/* method: new of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_new00
static int tolua_lua_mongo_wrap_binding_MongoWrap_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MongoWrap* tolua_ret = (MongoWrap*)  Mtolua_new((MongoWrap)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MongoWrap");
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

/* method: new_local of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_new00_local
static int tolua_lua_mongo_wrap_binding_MongoWrap_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MongoWrap* tolua_ret = (MongoWrap*)  Mtolua_new((MongoWrap)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MongoWrap");
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

/* method: delete of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_delete00
static int tolua_lua_mongo_wrap_binding_MongoWrap_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Open of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Open00
static int tolua_lua_mongo_wrap_binding_MongoWrap_Open00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,1,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,1,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_iscppstring(tolua_S,8,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
  string host = ((string)  tolua_tocppstring(tolua_S,2,0));
  unsigned short port = ((unsigned short)  tolua_tonumber(tolua_S,3,0));
  string user = ((string)  tolua_tocppstring(tolua_S,4,""));
  string pwd = ((string)  tolua_tocppstring(tolua_S,5,""));
  string dbname = ((string)  tolua_tocppstring(tolua_S,6,"admin"));
  int level = ((int)  tolua_tonumber(tolua_S,7,_NO_LOG));
  string logpath = ((string)  tolua_tocppstring(tolua_S,8,""));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Open'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Open(host,port,user,pwd,dbname,level,logpath);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Open'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Insert of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Insert00
static int tolua_lua_mongo_wrap_binding_MongoWrap_Insert00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::map<string,string>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
   std::map<string,string>* values = ((  std::map<string,string>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Insert'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Insert(ns,*values);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Insert'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Update of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Update00
static int tolua_lua_mongo_wrap_binding_MongoWrap_Update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const std::map<string,string>",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"std::map<string,string>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
  const std::map<string,string>* where = ((const std::map<string,string>*)  tolua_tousertype(tolua_S,3,0));
   std::map<string,string>* values = ((  std::map<string,string>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Update'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Update(ns,*where,*values);
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
#endif //#ifndef TOLUA_DISABLE

/* method: Delete of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Delete00
static int tolua_lua_mongo_wrap_binding_MongoWrap_Delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const std::map<string,string>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
  const std::map<string,string>* where = ((const std::map<string,string>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Delete'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Delete(ns,*where);
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
#endif //#ifndef TOLUA_DISABLE

/* method: Load of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Load00
static int tolua_lua_mongo_wrap_binding_MongoWrap_Load00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const std::map<string,string>",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"std::map<string,string>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
  const std::map<string,string>* where = ((const std::map<string,string>*)  tolua_tousertype(tolua_S,3,0));
   std::map<string,string>* result = ((  std::map<string,string>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Load'", NULL);
#endif
  {
   Mongo_Result tolua_ret = (Mongo_Result)  self->Load(ns,*where,*result);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Load'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Query of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Query00
static int tolua_lua_mongo_wrap_binding_MongoWrap_Query00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const std::map<string,string>",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"std::vector<MongoWrap::Map>",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const std::map<string,string>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
  const std::map<string,string>* where = ((const std::map<string,string>*)  tolua_tousertype(tolua_S,3,0));
   std::vector<MongoWrap::Map>* results = ((  std::vector<MongoWrap::Map>*)  tolua_tousertype(tolua_S,4,0));
  const std::map<string,string>* outfield = ((const std::map<string,string>*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Query'", NULL);
#endif
  {
   Mongo_Result tolua_ret = (Mongo_Result)  self->Query(ns,*where,*results,*outfield);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Query'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Find of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_Find00
static int tolua_lua_mongo_wrap_binding_MongoWrap_Find00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const std::map<string,string>",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"std::vector<MongoWrap::Map>",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
  const std::map<string,string>* where = ((const std::map<string,string>*)  tolua_tousertype(tolua_S,3,0));
   std::vector<MongoWrap::Map>* results = ((  std::vector<MongoWrap::Map>*)  tolua_tousertype(tolua_S,4,0));
  string orderKey = ((string)  tolua_tocppstring(tolua_S,5,0));
   int bySort = ((  int)  tolua_tonumber(tolua_S,6,0));
   int limitResult = ((  int)  tolua_tonumber(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Find'", NULL);
#endif
  {
   Mongo_Result tolua_ret = (Mongo_Result)  self->Find(ns,*where,*results,orderKey,bySort,limitResult);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Find'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CheckNS of class  MongoWrap */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_MongoWrap_CheckNS00
static int tolua_lua_mongo_wrap_binding_MongoWrap_CheckNS00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MongoWrap",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MongoWrap* self = (MongoWrap*)  tolua_tousertype(tolua_S,1,0);
  const string ns = ((const string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CheckNS'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CheckNS(ns);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)ns);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CheckNS'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsyncQuery of class  AsyncMongoDB */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncQuery00
static int tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncQuery00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AsyncMongoDB",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const MongoDB::Map",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"int32_t",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,5,1,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"int32_t",1,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,7,&tolua_err) || !tolua_isusertype(tolua_S,7,"int32_t",1,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AsyncMongoDB* self = (AsyncMongoDB*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
  const MongoDB::Map* where = ((const MongoDB::Map*)  tolua_tousertype(tolua_S,3,0));
  int32_t cb = *((int32_t*)  tolua_tousertype(tolua_S,4,0));
  string orderKey = ((string)  tolua_tocppstring(tolua_S,5,""));
  int32_t bySort = *((int32_t*)  tolua_tousertype(tolua_S,6,(void*)&(const int32_t)0));
  int32_t limitReslut = *((int32_t*)  tolua_tousertype(tolua_S,7,(void*)&(const int32_t)0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsyncQuery'", NULL);
#endif
  {
   self->AsyncQuery(ns,*where,cb,orderKey,bySort,limitReslut);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsyncQuery'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsyncUpdate of class  AsyncMongoDB */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncUpdate00
static int tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AsyncMongoDB",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Map",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"Map",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AsyncMongoDB* self = (AsyncMongoDB*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
  const Map* where = ((const Map*)  tolua_tousertype(tolua_S,3,0));
  Map* values = ((Map*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsyncUpdate'", NULL);
#endif
  {
   self->AsyncUpdate(ns,*where,*values);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsyncUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AsyncInsert of class  AsyncMongoDB */
#ifndef TOLUA_DISABLE_tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncInsert00
static int tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncInsert00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"AsyncMongoDB",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Map",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  AsyncMongoDB* self = (AsyncMongoDB*)  tolua_tousertype(tolua_S,1,0);
  string ns = ((string)  tolua_tocppstring(tolua_S,2,0));
  const Map* values = ((const Map*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AsyncInsert'", NULL);
#endif
  {
   self->AsyncInsert(ns,*values);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AsyncInsert'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_lua_mongo_wrap_binding_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_constant(tolua_S,"_NO_LOG",_NO_LOG);
  tolua_constant(tolua_S,"_DEBUG_L",_DEBUG_L);
  tolua_constant(tolua_S,"_ERROR_L",_ERROR_L);
  tolua_constant(tolua_S,"MONGO_RES_ERROR",MONGO_RES_ERROR);
  tolua_constant(tolua_S,"MONGO_RES_SUCCESS",MONGO_RES_SUCCESS);
  tolua_constant(tolua_S,"MONGO_RES_NULL",MONGO_RES_NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"MongoWrap","MongoWrap","",tolua_collect_MongoWrap);
  #else
  tolua_cclass(tolua_S,"MongoWrap","MongoWrap","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"MongoWrap");
   tolua_function(tolua_S,"new",tolua_lua_mongo_wrap_binding_MongoWrap_new00);
   tolua_function(tolua_S,"new_local",tolua_lua_mongo_wrap_binding_MongoWrap_new00_local);
   tolua_function(tolua_S,".call",tolua_lua_mongo_wrap_binding_MongoWrap_new00_local);
   tolua_function(tolua_S,"delete",tolua_lua_mongo_wrap_binding_MongoWrap_delete00);
   tolua_function(tolua_S,"Open",tolua_lua_mongo_wrap_binding_MongoWrap_Open00);
   tolua_function(tolua_S,"Insert",tolua_lua_mongo_wrap_binding_MongoWrap_Insert00);
   tolua_function(tolua_S,"Update",tolua_lua_mongo_wrap_binding_MongoWrap_Update00);
   tolua_function(tolua_S,"Delete",tolua_lua_mongo_wrap_binding_MongoWrap_Delete00);
   tolua_function(tolua_S,"Load",tolua_lua_mongo_wrap_binding_MongoWrap_Load00);
   tolua_function(tolua_S,"Query",tolua_lua_mongo_wrap_binding_MongoWrap_Query00);
   tolua_function(tolua_S,"Find",tolua_lua_mongo_wrap_binding_MongoWrap_Find00);
   tolua_function(tolua_S,"CheckNS",tolua_lua_mongo_wrap_binding_MongoWrap_CheckNS00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"AsyncMongoDB","AsyncMongoDB","",NULL);
  tolua_beginmodule(tolua_S,"AsyncMongoDB");
   tolua_function(tolua_S,"AsyncQuery",tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncQuery00);
   tolua_function(tolua_S,"AsyncUpdate",tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncUpdate00);
   tolua_function(tolua_S,"AsyncInsert",tolua_lua_mongo_wrap_binding_AsyncMongoDB_AsyncInsert00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_lua_mongo_wrap_binding (lua_State* tolua_S) {
 return tolua_lua_mongo_wrap_binding_open(tolua_S);
};
#endif

