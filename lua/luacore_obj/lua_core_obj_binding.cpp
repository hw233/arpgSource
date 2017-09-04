/*
** Lua binding: lua_core_obj_binding
** Generated automatically by tolua++-1.0.92 on 08/18/15 23:03:48.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_lua_core_obj_binding_open (lua_State* tolua_S);

#include <assert.h>
#define BYTEARRAY_ASSERT assert
#include "UpdateMask.h"
#include "ByteArrayCompress.h"
#include "SyncEventRecorder.h"
#include "GuidObject.h"
#include "GuidObjectTable.h"
#include "BinLogStru.h"
#include "RemoteOperatorAdapter.h"
#include "lua_core_obj_binding_manual.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_core_obj__ByteArrayCompress (lua_State* tolua_S)
{
 core_obj::ByteArrayCompress* self = (core_obj::ByteArrayCompress*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_core_obj__SyncEventRecorder (lua_State* tolua_S)
{
 core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_core_obj__UpdateMask (lua_State* tolua_S)
{
 core_obj::UpdateMask* self = (core_obj::UpdateMask*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_core_obj__ByteArray (lua_State* tolua_S)
{
 core_obj::ByteArray* self = (core_obj::ByteArray*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_core_obj__LuaGuidObject (lua_State* tolua_S)
{
 core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_core_obj__GuidObjectTable (lua_State* tolua_S)
{
 core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_core_obj__LuaGuidObjectTable (lua_State* tolua_S)
{
 core_obj::LuaGuidObjectTable* self = (core_obj::LuaGuidObjectTable*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_core_obj__RemoteOperatorAdapter (lua_State* tolua_S)
{
 core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_core_obj__GuidObject (lua_State* tolua_S)
{
 core_obj::GuidObject* self = (core_obj::GuidObject*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"core_obj::ByteArrayCompress");
 tolua_usertype(tolua_S,"core_obj::SyncEventRecorder");
 tolua_usertype(tolua_S,"core_obj::UpdateMask");
 tolua_usertype(tolua_S,"core_obj::ByteArray");
 tolua_usertype(tolua_S,"core_obj::LuaGuidObject");
 tolua_usertype(tolua_S,"core_obj::GuidObjectTable");
 tolua_usertype(tolua_S,"core_obj::LuaGuidObjectTable");
 tolua_usertype(tolua_S,"core_obj::RemoteOperatorAdapter");
 tolua_usertype(tolua_S,"core_obj::GuidObject");
}

/* method: new of class  core_obj::UpdateMask */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_UpdateMask_new00
static int tolua_lua_core_obj_binding_core_obj_UpdateMask_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::UpdateMask",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   core_obj::UpdateMask* tolua_ret = (core_obj::UpdateMask*)  Mtolua_new((core_obj::UpdateMask)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::UpdateMask");
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

/* method: new_local of class  core_obj::UpdateMask */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_UpdateMask_new00_local
static int tolua_lua_core_obj_binding_core_obj_UpdateMask_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::UpdateMask",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   core_obj::UpdateMask* tolua_ret = (core_obj::UpdateMask*)  Mtolua_new((core_obj::UpdateMask)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::UpdateMask");
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

/* method: delete of class  core_obj::UpdateMask */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_UpdateMask_delete00
static int tolua_lua_core_obj_binding_core_obj_UpdateMask_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::UpdateMask",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::UpdateMask* self = (core_obj::UpdateMask*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetBit of class  core_obj::UpdateMask */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_UpdateMask_GetBit00
static int tolua_lua_core_obj_binding_core_obj_UpdateMask_GetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::UpdateMask",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::UpdateMask* self = (const core_obj::UpdateMask*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBit'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetBit(i);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBit of class  core_obj::UpdateMask */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_UpdateMask_SetBit00
static int tolua_lua_core_obj_binding_core_obj_UpdateMask_SetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::UpdateMask",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::UpdateMask* self = (core_obj::UpdateMask*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBit'", NULL);
#endif
  {
   self->SetBit(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UnSetBit of class  core_obj::UpdateMask */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_UpdateMask_UnSetBit00
static int tolua_lua_core_obj_binding_core_obj_UpdateMask_UnSetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::UpdateMask",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::UpdateMask* self = (core_obj::UpdateMask*)  tolua_tousertype(tolua_S,1,0);
  int i = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UnSetBit'", NULL);
#endif
  {
   self->UnSetBit(i);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UnSetBit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteTo of class  core_obj::UpdateMask */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_UpdateMask_WriteTo00
static int tolua_lua_core_obj_binding_core_obj_UpdateMask_WriteTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::UpdateMask",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::UpdateMask* self = (const core_obj::UpdateMask*)  tolua_tousertype(tolua_S,1,0);
  core_obj::ByteArray* tolua_var_1 = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteTo'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->WriteTo(*tolua_var_1);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadFrom of class  core_obj::UpdateMask */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_UpdateMask_ReadFrom00
static int tolua_lua_core_obj_binding_core_obj_UpdateMask_ReadFrom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::UpdateMask",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::UpdateMask* self = (core_obj::UpdateMask*)  tolua_tousertype(tolua_S,1,0);
  core_obj::ByteArray* tolua_var_2 = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadFrom'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ReadFrom(*tolua_var_2);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadFrom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_new00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   core_obj::ByteArray* tolua_ret = (core_obj::ByteArray*)  Mtolua_new((core_obj::ByteArray)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::ByteArray");
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

/* method: new_local of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_new00_local
static int tolua_lua_core_obj_binding_core_obj_ByteArray_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   core_obj::ByteArray* tolua_ret = (core_obj::ByteArray*)  Mtolua_new((core_obj::ByteArray)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::ByteArray");
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

/* method: delete of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_delete00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_delete00(lua_State* tolua_S)
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

/* method: clear of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_clear00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_clear00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear'", NULL);
#endif
  {
   self->clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: data of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_data00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  int offset = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'data'", NULL);
#endif
  {
   void* tolua_ret = (void*)  self->data(offset);
   tolua_pushuserdata(tolua_S,(void*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: length of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_get_core_obj__ByteArray_length
static int tolua_get_core_obj__ByteArray_length(lua_State* tolua_S)
{
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'length'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->length());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: length of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_set_core_obj__ByteArray_length
static int tolua_set_core_obj__ByteArray_length(lua_State* tolua_S)
{
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'length'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->length(((int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: position of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_get_core_obj__ByteArray_position
static int tolua_get_core_obj__ByteArray_position(lua_State* tolua_S)
{
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->position());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: position of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_set_core_obj__ByteArray_position
static int tolua_set_core_obj__ByteArray_position(lua_State* tolua_S)
{
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'position'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->position(((int)  tolua_tonumber(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: bytesAvailable of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_bytesAvailable00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_bytesAvailable00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::ByteArray* self = (const core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'bytesAvailable'", NULL);
#endif
  {
   int tolua_ret = (int)  self->bytesAvailable();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bytesAvailable'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readByte of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readByte00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readByte00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readByte'", NULL);
#endif
  {
    char tolua_ret = (  char)  self->readByte();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readDouble of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readDouble00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readDouble00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readDouble'", NULL);
#endif
  {
   double tolua_ret = (double)  self->readDouble();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readFloat of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readFloat00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readFloat00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readFloat'", NULL);
#endif
  {
   float tolua_ret = (float)  self->readFloat();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readInt of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readInt00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readInt00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readInt'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->readInt();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readShort of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readShort00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readShort00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readShort'", NULL);
#endif
  {
    short tolua_ret = (  short)  self->readShort();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readShort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readShort of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readShort01
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readShort01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readShort'", NULL);
#endif
  {
    short tolua_ret = (  short)  self->readShort(pos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_lua_core_obj_binding_core_obj_ByteArray_readShort00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: readUnsignedByte of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedByte00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedByte00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readUnsignedByte'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->readUnsignedByte();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readUnsignedByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readUnsignedInt of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedInt00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedInt00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readUnsignedInt'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->readUnsignedInt();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readUnsignedInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readUnsignedShort of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedShort00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedShort00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readUnsignedShort'", NULL);
#endif
  {
   unsigned short tolua_ret = ( unsigned short)  self->readUnsignedShort();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readUnsignedShort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readUnsignedShort of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedShort01
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedShort01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readUnsignedShort'", NULL);
#endif
  {
    short tolua_ret = (  short)  self->readUnsignedShort(pos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedShort00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: readUTF of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readUTF00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readUTF00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readUTF'", NULL);
#endif
  {
   string tolua_ret = (string)  self->readUTF();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readUTF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readUTFCheckLen of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readUTFCheckLen00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readUTFCheckLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
   char is_enough_long = ((  char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readUTFCheckLen'", NULL);
#endif
  {
   string tolua_ret = (string)  self->readUTFCheckLen(is_enough_long);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readUTFCheckLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: readStringByLen of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_readStringByLen00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_readStringByLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  int len = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'readStringByLen'", NULL);
#endif
  {
   string tolua_ret = (string)  self->readStringByLen(len);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'readStringByLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeDouble of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeDouble00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeDouble00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  double val = ((double)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeDouble'", NULL);
#endif
  {
   self->writeDouble(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeFloat of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeFloat00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  float val = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeFloat'", NULL);
#endif
  {
   self->writeFloat(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeInt of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeInt00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
   int val = ((  int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeInt'", NULL);
#endif
  {
   self->writeInt(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeShort of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeShort00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeShort00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  short val = ((short)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeShort'", NULL);
#endif
  {
   self->writeShort(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeShort'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeByte of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeByte00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
   char val = ((  char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeByte'", NULL);
#endif
  {
   self->writeByte(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeUnsignedInt of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeUnsignedInt00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeUnsignedInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  unsigned int val = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeUnsignedInt'", NULL);
#endif
  {
   self->writeUnsignedInt(val);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeUnsignedInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeUTF of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeUTF00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeUTF00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  string str = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeUTF'", NULL);
#endif
  {
   self->writeUTF(str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeUTF'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeStringByLen of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeStringByLen00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeStringByLen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  string s = ((string)  tolua_tocppstring(tolua_S,2,0));
  int len = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeStringByLen'", NULL);
#endif
  {
   self->writeStringByLen(s,len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeStringByLen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: writeBytes of class  core_obj::ByteArray */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArray_writeBytes00
static int tolua_lua_core_obj_binding_core_obj_ByteArray_writeBytes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArray",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArray* self = (core_obj::ByteArray*)  tolua_tousertype(tolua_S,1,0);
  const core_obj::ByteArray* bytes_ = ((const core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
  int offset = ((int)  tolua_tonumber(tolua_S,3,0));
  int len = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'writeBytes'", NULL);
#endif
  {
   core_obj::ByteArray& tolua_ret = (core_obj::ByteArray&)  self->writeBytes(*bytes_,offset,len);
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"core_obj::ByteArray");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'writeBytes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncMode m_ = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  {
   core_obj::SyncEventRecorder* tolua_ret = (core_obj::SyncEventRecorder*)  Mtolua_new((core_obj::SyncEventRecorder)(m_));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::SyncEventRecorder");
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

/* method: new_local of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00_local
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncMode m_ = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  {
   core_obj::SyncEventRecorder* tolua_ret = (core_obj::SyncEventRecorder*)  Mtolua_new((core_obj::SyncEventRecorder)(m_));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::SyncEventRecorder");
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

/* method: new of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new01
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  core_obj::SyncMode m_ = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  const string key = ((const string)  tolua_tocppstring(tolua_S,3,0));
  {
   core_obj::SyncEventRecorder* tolua_ret = (core_obj::SyncEventRecorder*)  Mtolua_new((core_obj::SyncEventRecorder)(m_,key));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::SyncEventRecorder");
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new01_local
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  core_obj::SyncMode m_ = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  const string key = ((const string)  tolua_tocppstring(tolua_S,3,0));
  {
   core_obj::SyncEventRecorder* tolua_ret = (core_obj::SyncEventRecorder*)  Mtolua_new((core_obj::SyncEventRecorder)(m_,key));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::SyncEventRecorder");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_delete00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: guid of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_get_core_obj__SyncEventRecorder_guid
static int tolua_get_core_obj__SyncEventRecorder_guid(lua_State* tolua_S)
{
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'guid'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->guid());
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: guid of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_set_core_obj__SyncEventRecorder_guid
static int tolua_set_core_obj__SyncEventRecorder_guid(lua_State* tolua_S)
{
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'guid'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->guid(((string)  tolua_tocppstring(tolua_S,2,0))
)
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteCreateBlock of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteCreateBlock00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteCreateBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
  core_obj::ByteArray* tolua_var_3 = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteCreateBlock'", NULL);
#endif
  {
   self->WriteCreateBlock(*tolua_var_3);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteCreateBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteReleaseBlock of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteReleaseBlock00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteReleaseBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
  core_obj::ByteArray* tolua_var_4 = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteReleaseBlock'", NULL);
#endif
  {
   self->WriteReleaseBlock(*tolua_var_4);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteReleaseBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: WriteUpdateBlock of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteUpdateBlock00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteUpdateBlock00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
  core_obj::ByteArray* tolua_var_5 = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'WriteUpdateBlock'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->WriteUpdateBlock(*tolua_var_5);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'WriteUpdateBlock'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadFrom of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_ReadFrom00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_ReadFrom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
  int flags = ((int)  tolua_tonumber(tolua_S,2,0));
  core_obj::ByteArray* bytes = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadFrom'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->ReadFrom(flags,*bytes);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadFrom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToString of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_ToString00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_ToString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
  string ints = ((string)  tolua_tocppstring(tolua_S,2,0));
  string strs = ((string)  tolua_tocppstring(tolua_S,3,0));
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
#endif //#ifndef TOLUA_DISABLE

/* method: FromString of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_FromString00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_FromString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
  string ints = ((string)  tolua_tocppstring(tolua_S,2,0));
  string strs = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FromString'", NULL);
#endif
  {
   self->FromString(ints,strs);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FromString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBinlogMaxSize of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_SetBinlogMaxSize00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_SetBinlogMaxSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
  int size = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBinlogMaxSize'", NULL);
#endif
  {
   self->SetBinlogMaxSize(size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBinlogMaxSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setMode of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_setMode00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_setMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
  unsigned int v = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setMode'", NULL);
#endif
  {
   self->setMode(v);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_Clear00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'", NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Reset of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_Reset00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_Reset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Reset'", NULL);
#endif
  {
   self->Reset();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Reset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: uint32_values_size of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_uint32_values_size00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_uint32_values_size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'uint32_values_size'", NULL);
#endif
  {
   int tolua_ret = (int)  self->uint32_values_size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'uint32_values_size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: str_values_size of class  core_obj::SyncEventRecorder */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_str_values_size00
static int tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_str_values_size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::SyncEventRecorder",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncEventRecorder* self = (core_obj::SyncEventRecorder*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'str_values_size'", NULL);
#endif
  {
   int tolua_ret = (int)  self->str_values_size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'str_values_size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_new00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncMode m_ = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  {
   core_obj::GuidObject* tolua_ret = (core_obj::GuidObject*)  Mtolua_new((core_obj::GuidObject)(m_));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::GuidObject");
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

/* method: new_local of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_new00_local
static int tolua_lua_core_obj_binding_core_obj_GuidObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncMode m_ = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  {
   core_obj::GuidObject* tolua_ret = (core_obj::GuidObject*)  Mtolua_new((core_obj::GuidObject)(m_));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::GuidObject");
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

/* method: new of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_new01
static int tolua_lua_core_obj_binding_core_obj_GuidObject_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  core_obj::SyncMode m_ = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  const string key = ((const string)  tolua_tocppstring(tolua_S,3,0));
  {
   core_obj::GuidObject* tolua_ret = (core_obj::GuidObject*)  Mtolua_new((core_obj::GuidObject)(m_,key));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::GuidObject");
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_lua_core_obj_binding_core_obj_GuidObject_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_new01_local
static int tolua_lua_core_obj_binding_core_obj_GuidObject_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  core_obj::SyncMode m_ = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  const string key = ((const string)  tolua_tocppstring(tolua_S,3,0));
  {
   core_obj::GuidObject* tolua_ret = (core_obj::GuidObject*)  Mtolua_new((core_obj::GuidObject)(m_,key));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::GuidObject");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_lua_core_obj_binding_core_obj_GuidObject_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_delete00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetUInt32 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetUInt3200
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetUInt3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::GuidObject* self = (const core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetUInt32'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->GetUInt32(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetUInt32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddUInt32 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_AddUInt3200
static int tolua_lua_core_obj_binding_core_obj_GuidObject_AddUInt3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned int value = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddUInt32'", NULL);
#endif
  {
   self->AddUInt32(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddUInt32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SubUInt32 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SubUInt3200
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SubUInt3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned int value = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SubUInt32'", NULL);
#endif
  {
   self->SubUInt32(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SubUInt32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUInt32 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetUInt3200
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetUInt3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned int value = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUInt32'", NULL);
#endif
  {
   self->SetUInt32(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUInt32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetInt32 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetInt3200
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetInt3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::GuidObject* self = (const core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetInt32'", NULL);
#endif
  {
    int tolua_ret = (  int)  self->GetInt32(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInt32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddInt32 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_AddInt3200
static int tolua_lua_core_obj_binding_core_obj_GuidObject_AddInt3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
   int value = ((  int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddInt32'", NULL);
#endif
  {
   self->AddInt32(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddInt32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SubInt32 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SubInt3200
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SubInt3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
   int value = ((  int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SubInt32'", NULL);
#endif
  {
   self->SubInt32(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SubInt32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetInt32 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetInt3200
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetInt3200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
   int value = ((  int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetInt32'", NULL);
#endif
  {
   self->SetInt32(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetInt32'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetFloat of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetFloat00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::GuidObject* self = (const core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetFloat'", NULL);
#endif
  {
   float tolua_ret = (float)  self->GetFloat(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddFloat of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_AddFloat00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_AddFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  float value = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddFloat'", NULL);
#endif
  {
   self->AddFloat(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SubFloat of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SubFloat00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SubFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  float value = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SubFloat'", NULL);
#endif
  {
   self->SubFloat(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SubFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFloat of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetFloat00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetFloat00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  float value = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFloat'", NULL);
#endif
  {
   self->SetFloat(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFloat'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetDouble of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetDouble00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetDouble00(lua_State* tolua_S)
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
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetDouble'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetDouble(index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDouble of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetDouble00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetDouble00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  double value = ((double)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDouble'", NULL);
#endif
  {
   self->SetDouble(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddDouble of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_AddDouble00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_AddDouble00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  double value = ((double)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddDouble'", NULL);
#endif
  {
   self->AddDouble(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SubDouble of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SubDouble00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SubDouble00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  double value = ((double)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SubDouble'", NULL);
#endif
  {
   self->SubDouble(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SubDouble'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetUInt16 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetUInt1600
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetUInt1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::GuidObject* self = (const core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetUInt16'", NULL);
#endif
  {
   unsigned short tolua_ret = ( unsigned short)  self->GetUInt16(index,offset);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetUInt16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddUInt16 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_AddUInt1600
static int tolua_lua_core_obj_binding_core_obj_GuidObject_AddUInt1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned short value = (( unsigned short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddUInt16'", NULL);
#endif
  {
   self->AddUInt16(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddUInt16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SubUInt16 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SubUInt1600
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SubUInt1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned short value = (( unsigned short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SubUInt16'", NULL);
#endif
  {
   self->SubUInt16(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SubUInt16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetUInt16 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetUInt1600
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetUInt1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned short value = (( unsigned short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetUInt16'", NULL);
#endif
  {
   self->SetUInt16(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetUInt16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetInt16 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetInt1600
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetInt1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::GuidObject* self = (const core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetInt16'", NULL);
#endif
  {
    short tolua_ret = (  short)  self->GetInt16(index,offset);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInt16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddInt16 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_AddInt1600
static int tolua_lua_core_obj_binding_core_obj_GuidObject_AddInt1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
   short value = ((  short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddInt16'", NULL);
#endif
  {
   self->AddInt16(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddInt16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SubInt16 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SubInt1600
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SubInt1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
   short value = ((  short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SubInt16'", NULL);
#endif
  {
   self->SubInt16(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SubInt16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetInt16 of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetInt1600
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetInt1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
   short value = ((  short)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetInt16'", NULL);
#endif
  {
   self->SetInt16(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetInt16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetByte of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetByte00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::GuidObject* self = (const core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetByte'", NULL);
#endif
  {
   unsigned char tolua_ret = ( unsigned char)  self->GetByte(index,offset);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddByte of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_AddByte00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_AddByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char value = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddByte'", NULL);
#endif
  {
   self->AddByte(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SubByte of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SubByte00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SubByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char value = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SubByte'", NULL);
#endif
  {
   self->SubByte(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SubByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetByte of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetByte00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetByte00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned char offset = (( unsigned char)  tolua_tonumber(tolua_S,3,0));
  unsigned char value = (( unsigned char)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetByte'", NULL);
#endif
  {
   self->SetByte(index,offset,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetByte'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBit of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetBit00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::GuidObject* self = (const core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned short offset = (( unsigned short)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBit'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetBit(index,offset);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBit of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetBit00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned short offset = (( unsigned short)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBit'", NULL);
#endif
  {
   self->SetBit(index,offset);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UnSetBit of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_UnSetBit00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_UnSetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned short offset = (( unsigned short)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UnSetBit'", NULL);
#endif
  {
   self->UnSetBit(index,offset);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UnSetBit'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetStr of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetStr00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetStr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const core_obj::GuidObject* self = (const core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetStr'", NULL);
#endif
  {
   const string tolua_ret = (const string)  self->GetStr(index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetStr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetStr of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_SetStr00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_SetStr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  const string str = ((const string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetStr'", NULL);
#endif
  {
   self->SetStr(index,str);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetStr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHashCode of class  core_obj::GuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObject_GetHashCode00
static int tolua_lua_core_obj_binding_core_obj_GuidObject_GetHashCode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObject* self = (core_obj::GuidObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHashCode'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->GetHashCode();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHashCode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncMode m = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,core_obj::SYNC_SLAVE));
  {
   core_obj::LuaGuidObject* tolua_ret = (core_obj::LuaGuidObject*)  Mtolua_new((core_obj::LuaGuidObject)(m));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::LuaGuidObject");
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

/* method: new_local of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00_local
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncMode m = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,core_obj::SYNC_SLAVE));
  {
   core_obj::LuaGuidObject* tolua_ret = (core_obj::LuaGuidObject*)  Mtolua_new((core_obj::LuaGuidObject)(m));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::LuaGuidObject");
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

/* method: new of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new01
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  core_obj::SyncMode m = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  const string key = ((const string)  tolua_tocppstring(tolua_S,3,0));
  {
   core_obj::LuaGuidObject* tolua_ret = (core_obj::LuaGuidObject*)  Mtolua_new((core_obj::LuaGuidObject)(m,key));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::LuaGuidObject");
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new01_local
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  core_obj::SyncMode m = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  const string key = ((const string)  tolua_tocppstring(tolua_S,3,0));
  {
   core_obj::LuaGuidObject* tolua_ret = (core_obj::LuaGuidObject*)  Mtolua_new((core_obj::LuaGuidObject)(m,key));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::LuaGuidObject");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 2;
tolua_lerror:
 return tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_delete00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*)  tolua_tousertype(tolua_S,1,0);
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

/* method: after_update of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_after_update00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_after_update00(lua_State* tolua_S)
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
  int f = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'after_update'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->after_update(f);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'after_update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddListen of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListen00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int scb = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddListen'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->AddListen(index,scb);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddListen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddListenString of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListenString00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListenString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::LuaGuidObject* self = (core_obj::LuaGuidObject*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
  int scb = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddListenString'", NULL);
#endif
  {
   unsigned int tolua_ret = ( unsigned int)  self->AddListenString(index,scb);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddListenString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DelListen of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelListen00
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
   self->DelListen(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DelListen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: DelScriptHanlder of class  core_obj::LuaGuidObject */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelScriptHanlder00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelScriptHanlder00(lua_State* tolua_S)
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
  int hanlder = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'DelScriptHanlder'", NULL);
#endif
  {
   self->DelScriptHanlder(hanlder);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DelScriptHanlder'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_new00
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncMode m = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  {
   core_obj::GuidObjectTable* tolua_ret = (core_obj::GuidObjectTable*)  Mtolua_new((core_obj::GuidObjectTable)(m));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::GuidObjectTable");
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

/* method: new_local of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_new00_local
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::SyncMode m = ((core_obj::SyncMode) (int)  tolua_tonumber(tolua_S,2,0));
  {
   core_obj::GuidObjectTable* tolua_ret = (core_obj::GuidObjectTable*)  Mtolua_new((core_obj::GuidObjectTable)(m));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::GuidObjectTable");
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

/* method: delete of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_delete00
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Get of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_Get00
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
  const string key = ((const string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Get'", NULL);
#endif
  {
   core_obj::GuidObject* tolua_ret = (core_obj::GuidObject*)  self->Get(key);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::GuidObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Get'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CreateIndex of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_CreateIndex00
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_CreateIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
  string exp = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CreateIndex'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->CreateIndex(exp);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CreateIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReleaseIndex of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_ReleaseIndex00
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_ReleaseIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
  string exp = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReleaseIndex'", NULL);
#endif
  {
   self->ReleaseIndex(exp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReleaseIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ApplyBlock of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_ApplyBlock00
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
   bool tolua_ret = (bool)  self->ApplyBlock(bytes);
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
#endif //#ifndef TOLUA_DISABLE

/* method: LuaRegisterCreateEvent of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterCreateEvent00
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterCreateEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
  string guid = ((string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int cb_idx = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LuaRegisterCreateEvent'", NULL);
#endif
  {
   self->LuaRegisterCreateEvent(guid,cb_idx);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LuaRegisterCreateEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LuaRegisterReleaseEvent of class  core_obj::GuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterReleaseEvent00
static int tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterReleaseEvent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::GuidObjectTable* self = (core_obj::GuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
  string guid = ((string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int cb_idx = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LuaRegisterReleaseEvent'", NULL);
#endif
  {
   self->LuaRegisterReleaseEvent(guid,cb_idx);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LuaRegisterReleaseEvent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  core_obj::LuaGuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_new00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   core_obj::LuaGuidObjectTable* tolua_ret = (core_obj::LuaGuidObjectTable*)  Mtolua_new((core_obj::LuaGuidObjectTable)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::LuaGuidObjectTable");
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

/* method: new_local of class  core_obj::LuaGuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_new00_local
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   core_obj::LuaGuidObjectTable* tolua_ret = (core_obj::LuaGuidObjectTable*)  Mtolua_new((core_obj::LuaGuidObjectTable)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::LuaGuidObjectTable");
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

/* method: delete of class  core_obj::LuaGuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_delete00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::LuaGuidObjectTable* self = (core_obj::LuaGuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
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

/* method: LuaRegisterCreateObject of class  core_obj::LuaGuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterCreateObject00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterCreateObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::LuaGuidObjectTable* self = (core_obj::LuaGuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
  unsigned int cb_idx = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LuaRegisterCreateObject'", NULL);
#endif
  {
   self->LuaRegisterCreateObject(cb_idx);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LuaRegisterCreateObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: LuaRegisterReleaseObject of class  core_obj::LuaGuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterReleaseObject00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterReleaseObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::LuaGuidObjectTable* self = (core_obj::LuaGuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
  unsigned int cb_idx = (( unsigned int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'LuaRegisterReleaseObject'", NULL);
#endif
  {
   self->LuaRegisterReleaseObject(cb_idx);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'LuaRegisterReleaseObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ForeachIndexer of class  core_obj::LuaGuidObjectTable */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_ForeachIndexer00
static int tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_ForeachIndexer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::LuaGuidObjectTable",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::LuaGuidObjectTable* self = (core_obj::LuaGuidObjectTable*)  tolua_tousertype(tolua_S,1,0);
  string exp = ((string)  tolua_tocppstring(tolua_S,2,0));
  unsigned int f = (( unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ForeachIndexer'", NULL);
#endif
  {
   self->ForeachIndexer(exp,f);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ForeachIndexer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: inst of class  core_obj::ByteArrayCompress */
#ifndef TOLUA_DISABLE_tolua_get_core_obj__ByteArrayCompress_inst
static int tolua_get_core_obj__ByteArrayCompress_inst(lua_State* tolua_S)
{
   tolua_pushusertype(tolua_S,(void*)&core_obj::ByteArrayCompress::inst,"core_obj::ByteArrayCompress");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: inst of class  core_obj::ByteArrayCompress */
#ifndef TOLUA_DISABLE_tolua_set_core_obj__ByteArrayCompress_inst
static int tolua_set_core_obj__ByteArrayCompress_inst(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"core_obj::ByteArrayCompress",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  core_obj::ByteArrayCompress::inst = *((core_obj::ByteArrayCompress*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  core_obj::ByteArrayCompress */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_new00
static int tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::ByteArrayCompress",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   core_obj::ByteArrayCompress* tolua_ret = (core_obj::ByteArrayCompress*)  Mtolua_new((core_obj::ByteArrayCompress)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::ByteArrayCompress");
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

/* method: new_local of class  core_obj::ByteArrayCompress */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_new00_local
static int tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::ByteArrayCompress",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   core_obj::ByteArrayCompress* tolua_ret = (core_obj::ByteArrayCompress*)  Mtolua_new((core_obj::ByteArrayCompress)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"core_obj::ByteArrayCompress");
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

/* method: delete of class  core_obj::ByteArrayCompress */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_delete00
static int tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArrayCompress",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArrayCompress* self = (core_obj::ByteArrayCompress*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Compress of class  core_obj::ByteArrayCompress */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_Compress00
static int tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_Compress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArrayCompress",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArrayCompress* self = (core_obj::ByteArrayCompress*)  tolua_tousertype(tolua_S,1,0);
  core_obj::ByteArray* bytes = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Compress'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Compress(*bytes);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Compress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: UnCompress of class  core_obj::ByteArrayCompress */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_UnCompress00
static int tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_UnCompress00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::ByteArrayCompress",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"core_obj::ByteArray",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::ByteArrayCompress* self = (core_obj::ByteArrayCompress*)  tolua_tousertype(tolua_S,1,0);
  core_obj::ByteArray* bytes = ((core_obj::ByteArray*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UnCompress'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->UnCompress(*bytes);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'UnCompress'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
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
  int send_func = ((int)  tolua_tonumber(tolua_S,3,0));
  int opt_ctl = ((int)  tolua_tonumber(tolua_S,4,0));
  int opt_up = ((int)  tolua_tonumber(tolua_S,5,0));
  int opt_ctl_ret = ((int)  tolua_tonumber(tolua_S,6,0));
  {
   core_obj::RemoteOperatorAdapter* tolua_ret = (core_obj::RemoteOperatorAdapter*)  Mtolua_new((core_obj::RemoteOperatorAdapter)(t,send_func,opt_ctl,opt_up,opt_ctl_ret));
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
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00_local
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"core_obj::GuidObjectTable",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
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
  int send_func = ((int)  tolua_tonumber(tolua_S,3,0));
  int opt_ctl = ((int)  tolua_tonumber(tolua_S,4,0));
  int opt_up = ((int)  tolua_tonumber(tolua_S,5,0));
  int opt_ctl_ret = ((int)  tolua_tonumber(tolua_S,6,0));
  {
   core_obj::RemoteOperatorAdapter* tolua_ret = (core_obj::RemoteOperatorAdapter*)  Mtolua_new((core_obj::RemoteOperatorAdapter)(t,send_func,opt_ctl,opt_up,opt_ctl_ret));
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
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_delete00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
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

/* method: InitNoticeInfo of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_InitNoticeInfo00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_InitNoticeInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  core_obj::GuidObject* obj = ((core_obj::GuidObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'InitNoticeInfo'", NULL);
#endif
  {
   self->InitNoticeInfo(obj);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'InitNoticeInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallPutObject of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallPutObject00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallPutObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"core_obj::GuidObject",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  core_obj::GuidObject* obj = ((core_obj::GuidObject*)  tolua_tousertype(tolua_S,2,0));
  int cb = ((int)  tolua_tonumber(tolua_S,3,0));
  bool add_notice = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallPutObject'", NULL);
#endif
  {
   self->CallPutObject(obj,cb,add_notice);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallPutObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallRemoveObject of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallRemoveObject00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallRemoveObject00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string key = ((string)  tolua_tocppstring(tolua_S,2,0));
  int cb = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallRemoveObject'", NULL);
#endif
  {
   self->CallRemoveObject(key,cb);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallRemoveObject'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallAddWatch of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallAddWatch00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallAddWatch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string key = ((string)  tolua_tocppstring(tolua_S,2,0));
  int cb_func = ((int)  tolua_tonumber(tolua_S,3,0));
  bool createblock = ((bool)  tolua_toboolean(tolua_S,4,0));
  unsigned int fd = (( unsigned int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallAddWatch'", NULL);
#endif
  {
   self->CallAddWatch(key,cb_func,createblock,fd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallAddWatch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallDelWatch of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallDelWatch00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallDelWatch00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string key = ((string)  tolua_tocppstring(tolua_S,2,0));
  int cb_func = ((int)  tolua_tonumber(tolua_S,3,0));
  unsigned int fd = (( unsigned int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallDelWatch'", NULL);
#endif
  {
   self->CallDelWatch(key,cb_func,fd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallDelWatch'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallWatchObjects of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallWatchObjects00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallWatchObjects00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string query_str = ((string)  tolua_tocppstring(tolua_S,2,0));
  int cb_func = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallWatchObjects'", NULL);
#endif
  {
   self->CallWatchObjects(query_str,cb_func);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallWatchObjects'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallDelWatchObjs of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallDelWatchObjs00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallDelWatchObjs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string wol = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallDelWatchObjs'", NULL);
#endif
  {
   self->CallDelWatchObjs(wol);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallDelWatchObjs'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallCreateIndex of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallCreateIndex00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallCreateIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string exp = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallCreateIndex'", NULL);
#endif
  {
   self->CallCreateIndex(exp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallCreateIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallReleaseIndex of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallReleaseIndex00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallReleaseIndex00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string exp = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallReleaseIndex'", NULL);
#endif
  {
   self->CallReleaseIndex(exp);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallReleaseIndex'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallSetTag of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallSetTag00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallSetTag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string guid = ((string)  tolua_tocppstring(tolua_S,2,0));
  string tag = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallSetTag'", NULL);
#endif
  {
   self->CallSetTag(guid,tag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallSetTag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CallUnSetTAg of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallUnSetTAg00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallUnSetTAg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
  string guid = ((string)  tolua_tocppstring(tolua_S,2,0));
  string tag = ((string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CallUnSetTAg'", NULL);
#endif
  {
   self->CallUnSetTAg(guid,tag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CallUnSetTAg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Update of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_Update00
static int tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_Update00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"core_obj::RemoteOperatorAdapter",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  core_obj::RemoteOperatorAdapter* self = (core_obj::RemoteOperatorAdapter*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Update'", NULL);
#endif
  {
   self->Update();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Update'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: HandlePacket of class  core_obj::RemoteOperatorAdapter */
#ifndef TOLUA_DISABLE_tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_HandlePacket00
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
   self->HandlePacket(cid,opt,buff);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'HandlePacket'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_lua_core_obj_binding_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"core_obj",0);
  tolua_beginmodule(tolua_S,"core_obj");
   tolua_constant(tolua_S,"SYNC_NONE",core_obj::SYNC_NONE);
   tolua_constant(tolua_S,"SYNC_MASTER",core_obj::SYNC_MASTER);
   tolua_constant(tolua_S,"SYNC_SLAVE",core_obj::SYNC_SLAVE);
   tolua_constant(tolua_S,"SYNC_MIRROR",core_obj::SYNC_MIRROR);
   tolua_constant(tolua_S,"OPT_SET",core_obj::OPT_SET);
   tolua_constant(tolua_S,"OPT_UNSET",core_obj::OPT_UNSET);
   tolua_constant(tolua_S,"OPT_ADD",core_obj::OPT_ADD);
   tolua_constant(tolua_S,"OPT_SUB",core_obj::OPT_SUB);
   tolua_constant(tolua_S,"__OPT_NEW",core_obj::__OPT_NEW);
   tolua_constant(tolua_S,"__OPT_DELETE",core_obj::__OPT_DELETE);
   tolua_constant(tolua_S,"__OPT_UPDATE",core_obj::__OPT_UPDATE);
   tolua_constant(tolua_S,"TYPE_UINT32",core_obj::TYPE_UINT32);
   tolua_constant(tolua_S,"TYPE_UINT16",core_obj::TYPE_UINT16);
   tolua_constant(tolua_S,"TYPE_UINT8",core_obj::TYPE_UINT8);
   tolua_constant(tolua_S,"TYPE_BIT",core_obj::TYPE_BIT);
   tolua_constant(tolua_S,"TYPE_UINT64",core_obj::TYPE_UINT64);
   tolua_constant(tolua_S,"TYPE_INT32",core_obj::TYPE_INT32);
   tolua_constant(tolua_S,"TYPE_STRING",core_obj::TYPE_STRING);
   tolua_constant(tolua_S,"TYPE_INT16",core_obj::TYPE_INT16);
   tolua_constant(tolua_S,"TYPE_FLOAT",core_obj::TYPE_FLOAT);
   tolua_constant(tolua_S,"ATOMIC_OPT_RESULT_NO",core_obj::ATOMIC_OPT_RESULT_NO);
   tolua_constant(tolua_S,"ATOMIC_OPT_RESULT_TRY",core_obj::ATOMIC_OPT_RESULT_TRY);
   tolua_constant(tolua_S,"ATOMIC_OPT_RESULT_OK",core_obj::ATOMIC_OPT_RESULT_OK);
   tolua_constant(tolua_S,"ATOMIC_OPT_RESULT_FAILED",core_obj::ATOMIC_OPT_RESULT_FAILED);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"UpdateMask","core_obj::UpdateMask","",tolua_collect_core_obj__UpdateMask);
   #else
   tolua_cclass(tolua_S,"UpdateMask","core_obj::UpdateMask","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"UpdateMask");
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_UpdateMask_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_UpdateMask_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_UpdateMask_new00_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_UpdateMask_delete00);
    tolua_function(tolua_S,"GetBit",tolua_lua_core_obj_binding_core_obj_UpdateMask_GetBit00);
    tolua_function(tolua_S,"SetBit",tolua_lua_core_obj_binding_core_obj_UpdateMask_SetBit00);
    tolua_function(tolua_S,"UnSetBit",tolua_lua_core_obj_binding_core_obj_UpdateMask_UnSetBit00);
    tolua_function(tolua_S,"WriteTo",tolua_lua_core_obj_binding_core_obj_UpdateMask_WriteTo00);
    tolua_function(tolua_S,"ReadFrom",tolua_lua_core_obj_binding_core_obj_UpdateMask_ReadFrom00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ByteArray","core_obj::ByteArray","",tolua_collect_core_obj__ByteArray);
   #else
   tolua_cclass(tolua_S,"ByteArray","core_obj::ByteArray","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ByteArray");
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_ByteArray_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_ByteArray_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_ByteArray_new00_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_ByteArray_delete00);
    tolua_function(tolua_S,"clear",tolua_lua_core_obj_binding_core_obj_ByteArray_clear00);
    tolua_function(tolua_S,"data",tolua_lua_core_obj_binding_core_obj_ByteArray_data00);
    tolua_variable(tolua_S,"length",tolua_get_core_obj__ByteArray_length,tolua_set_core_obj__ByteArray_length);
    tolua_variable(tolua_S,"position",tolua_get_core_obj__ByteArray_position,tolua_set_core_obj__ByteArray_position);
    tolua_function(tolua_S,"bytesAvailable",tolua_lua_core_obj_binding_core_obj_ByteArray_bytesAvailable00);
    tolua_function(tolua_S,"readByte",tolua_lua_core_obj_binding_core_obj_ByteArray_readByte00);
    tolua_function(tolua_S,"readDouble",tolua_lua_core_obj_binding_core_obj_ByteArray_readDouble00);
    tolua_function(tolua_S,"readFloat",tolua_lua_core_obj_binding_core_obj_ByteArray_readFloat00);
    tolua_function(tolua_S,"readInt",tolua_lua_core_obj_binding_core_obj_ByteArray_readInt00);
    tolua_function(tolua_S,"readShort",tolua_lua_core_obj_binding_core_obj_ByteArray_readShort00);
    tolua_function(tolua_S,"readShort",tolua_lua_core_obj_binding_core_obj_ByteArray_readShort01);
    tolua_function(tolua_S,"readUnsignedByte",tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedByte00);
    tolua_function(tolua_S,"readUnsignedInt",tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedInt00);
    tolua_function(tolua_S,"readUnsignedShort",tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedShort00);
    tolua_function(tolua_S,"readUnsignedShort",tolua_lua_core_obj_binding_core_obj_ByteArray_readUnsignedShort01);
    tolua_function(tolua_S,"readUTF",tolua_lua_core_obj_binding_core_obj_ByteArray_readUTF00);
    tolua_function(tolua_S,"readUTFCheckLen",tolua_lua_core_obj_binding_core_obj_ByteArray_readUTFCheckLen00);
    tolua_function(tolua_S,"readStringByLen",tolua_lua_core_obj_binding_core_obj_ByteArray_readStringByLen00);
    tolua_function(tolua_S,"writeDouble",tolua_lua_core_obj_binding_core_obj_ByteArray_writeDouble00);
    tolua_function(tolua_S,"writeFloat",tolua_lua_core_obj_binding_core_obj_ByteArray_writeFloat00);
    tolua_function(tolua_S,"writeInt",tolua_lua_core_obj_binding_core_obj_ByteArray_writeInt00);
    tolua_function(tolua_S,"writeShort",tolua_lua_core_obj_binding_core_obj_ByteArray_writeShort00);
    tolua_function(tolua_S,"writeByte",tolua_lua_core_obj_binding_core_obj_ByteArray_writeByte00);
    tolua_function(tolua_S,"writeUnsignedInt",tolua_lua_core_obj_binding_core_obj_ByteArray_writeUnsignedInt00);
    tolua_function(tolua_S,"writeUTF",tolua_lua_core_obj_binding_core_obj_ByteArray_writeUTF00);
    tolua_function(tolua_S,"writeStringByLen",tolua_lua_core_obj_binding_core_obj_ByteArray_writeStringByLen00);
    tolua_function(tolua_S,"writeBytes",tolua_lua_core_obj_binding_core_obj_ByteArray_writeBytes00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"SyncEventRecorder","core_obj::SyncEventRecorder","",tolua_collect_core_obj__SyncEventRecorder);
   #else
   tolua_cclass(tolua_S,"SyncEventRecorder","core_obj::SyncEventRecorder","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"SyncEventRecorder");
    tolua_constant(tolua_S,"OBJ_OPT_NEW",core_obj::SyncEventRecorder::OBJ_OPT_NEW);
    tolua_constant(tolua_S,"OBJ_OPT_DELETE",core_obj::SyncEventRecorder::OBJ_OPT_DELETE);
    tolua_constant(tolua_S,"OBJ_OPT_UPDATE",core_obj::SyncEventRecorder::OBJ_OPT_UPDATE);
    tolua_constant(tolua_S,"OBJ_OPT_BINLOG",core_obj::SyncEventRecorder::OBJ_OPT_BINLOG);
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new00_local);
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new01);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new01_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_new01_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_delete00);
    tolua_variable(tolua_S,"guid",tolua_get_core_obj__SyncEventRecorder_guid,tolua_set_core_obj__SyncEventRecorder_guid);
    tolua_function(tolua_S,"WriteCreateBlock",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteCreateBlock00);
    tolua_function(tolua_S,"WriteReleaseBlock",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteReleaseBlock00);
    tolua_function(tolua_S,"WriteUpdateBlock",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_WriteUpdateBlock00);
    tolua_function(tolua_S,"ReadFrom",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_ReadFrom00);
    tolua_function(tolua_S,"ToString",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_ToString00);
    tolua_function(tolua_S,"FromString",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_FromString00);
    tolua_function(tolua_S,"SetBinlogMaxSize",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_SetBinlogMaxSize00);
    tolua_function(tolua_S,"setMode",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_setMode00);
    tolua_function(tolua_S,"Clear",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_Clear00);
    tolua_function(tolua_S,"Reset",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_Reset00);
    tolua_function(tolua_S,"uint32_values_size",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_uint32_values_size00);
    tolua_function(tolua_S,"str_values_size",tolua_lua_core_obj_binding_core_obj_SyncEventRecorder_str_values_size00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"GuidObject","core_obj::GuidObject","core_obj::SyncEventRecorder",tolua_collect_core_obj__GuidObject);
   #else
   tolua_cclass(tolua_S,"GuidObject","core_obj::GuidObject","core_obj::SyncEventRecorder",NULL);
   #endif
   tolua_beginmodule(tolua_S,"GuidObject");
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_GuidObject_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_GuidObject_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_GuidObject_new00_local);
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_GuidObject_new01);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_GuidObject_new01_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_GuidObject_new01_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_GuidObject_delete00);
    tolua_function(tolua_S,"GetUInt32",tolua_lua_core_obj_binding_core_obj_GuidObject_GetUInt3200);
    tolua_function(tolua_S,"AddUInt32",tolua_lua_core_obj_binding_core_obj_GuidObject_AddUInt3200);
    tolua_function(tolua_S,"SubUInt32",tolua_lua_core_obj_binding_core_obj_GuidObject_SubUInt3200);
    tolua_function(tolua_S,"SetUInt32",tolua_lua_core_obj_binding_core_obj_GuidObject_SetUInt3200);
    tolua_function(tolua_S,"GetInt32",tolua_lua_core_obj_binding_core_obj_GuidObject_GetInt3200);
    tolua_function(tolua_S,"AddInt32",tolua_lua_core_obj_binding_core_obj_GuidObject_AddInt3200);
    tolua_function(tolua_S,"SubInt32",tolua_lua_core_obj_binding_core_obj_GuidObject_SubInt3200);
    tolua_function(tolua_S,"SetInt32",tolua_lua_core_obj_binding_core_obj_GuidObject_SetInt3200);
    tolua_function(tolua_S,"GetFloat",tolua_lua_core_obj_binding_core_obj_GuidObject_GetFloat00);
    tolua_function(tolua_S,"AddFloat",tolua_lua_core_obj_binding_core_obj_GuidObject_AddFloat00);
    tolua_function(tolua_S,"SubFloat",tolua_lua_core_obj_binding_core_obj_GuidObject_SubFloat00);
    tolua_function(tolua_S,"SetFloat",tolua_lua_core_obj_binding_core_obj_GuidObject_SetFloat00);
    tolua_function(tolua_S,"GetDouble",tolua_lua_core_obj_binding_core_obj_GuidObject_GetDouble00);
    tolua_function(tolua_S,"SetDouble",tolua_lua_core_obj_binding_core_obj_GuidObject_SetDouble00);
    tolua_function(tolua_S,"AddDouble",tolua_lua_core_obj_binding_core_obj_GuidObject_AddDouble00);
    tolua_function(tolua_S,"SubDouble",tolua_lua_core_obj_binding_core_obj_GuidObject_SubDouble00);
    tolua_function(tolua_S,"GetUInt16",tolua_lua_core_obj_binding_core_obj_GuidObject_GetUInt1600);
    tolua_function(tolua_S,"AddUInt16",tolua_lua_core_obj_binding_core_obj_GuidObject_AddUInt1600);
    tolua_function(tolua_S,"SubUInt16",tolua_lua_core_obj_binding_core_obj_GuidObject_SubUInt1600);
    tolua_function(tolua_S,"SetUInt16",tolua_lua_core_obj_binding_core_obj_GuidObject_SetUInt1600);
    tolua_function(tolua_S,"GetInt16",tolua_lua_core_obj_binding_core_obj_GuidObject_GetInt1600);
    tolua_function(tolua_S,"AddInt16",tolua_lua_core_obj_binding_core_obj_GuidObject_AddInt1600);
    tolua_function(tolua_S,"SubInt16",tolua_lua_core_obj_binding_core_obj_GuidObject_SubInt1600);
    tolua_function(tolua_S,"SetInt16",tolua_lua_core_obj_binding_core_obj_GuidObject_SetInt1600);
    tolua_function(tolua_S,"GetByte",tolua_lua_core_obj_binding_core_obj_GuidObject_GetByte00);
    tolua_function(tolua_S,"AddByte",tolua_lua_core_obj_binding_core_obj_GuidObject_AddByte00);
    tolua_function(tolua_S,"SubByte",tolua_lua_core_obj_binding_core_obj_GuidObject_SubByte00);
    tolua_function(tolua_S,"SetByte",tolua_lua_core_obj_binding_core_obj_GuidObject_SetByte00);
    tolua_function(tolua_S,"GetBit",tolua_lua_core_obj_binding_core_obj_GuidObject_GetBit00);
    tolua_function(tolua_S,"SetBit",tolua_lua_core_obj_binding_core_obj_GuidObject_SetBit00);
    tolua_function(tolua_S,"UnSetBit",tolua_lua_core_obj_binding_core_obj_GuidObject_UnSetBit00);
    tolua_function(tolua_S,"GetStr",tolua_lua_core_obj_binding_core_obj_GuidObject_GetStr00);
    tolua_function(tolua_S,"SetStr",tolua_lua_core_obj_binding_core_obj_GuidObject_SetStr00);
    tolua_function(tolua_S,"GetHashCode",tolua_lua_core_obj_binding_core_obj_GuidObject_GetHashCode00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"LuaGuidObject","core_obj::LuaGuidObject","core_obj::GuidObject",tolua_collect_core_obj__LuaGuidObject);
   #else
   tolua_cclass(tolua_S,"LuaGuidObject","core_obj::LuaGuidObject","core_obj::GuidObject",NULL);
   #endif
   tolua_beginmodule(tolua_S,"LuaGuidObject");
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new00_local);
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new01);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new01_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_new01_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_delete00);
    tolua_function(tolua_S,"after_update",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_after_update00);
    tolua_function(tolua_S,"AddListen",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListen00);
    tolua_function(tolua_S,"AddListenString",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_AddListenString00);
    tolua_function(tolua_S,"DelListen",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelListen00);
    tolua_function(tolua_S,"DelScriptHanlder",tolua_lua_core_obj_binding_core_obj_LuaGuidObject_DelScriptHanlder00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"GuidObjectTable","core_obj::GuidObjectTable","",tolua_collect_core_obj__GuidObjectTable);
   #else
   tolua_cclass(tolua_S,"GuidObjectTable","core_obj::GuidObjectTable","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"GuidObjectTable");
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_new00_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_delete00);
    tolua_function(tolua_S,"Get",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_Get00);
    tolua_function(tolua_S,"CreateIndex",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_CreateIndex00);
    tolua_function(tolua_S,"ReleaseIndex",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_ReleaseIndex00);
    tolua_function(tolua_S,"ApplyBlock",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_ApplyBlock00);
    tolua_function(tolua_S,"LuaRegisterCreateEvent",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterCreateEvent00);
    tolua_function(tolua_S,"LuaRegisterReleaseEvent",tolua_lua_core_obj_binding_core_obj_GuidObjectTable_LuaRegisterReleaseEvent00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"LuaGuidObjectTable","core_obj::LuaGuidObjectTable","core_obj::GuidObjectTable",tolua_collect_core_obj__LuaGuidObjectTable);
   #else
   tolua_cclass(tolua_S,"LuaGuidObjectTable","core_obj::LuaGuidObjectTable","core_obj::GuidObjectTable",NULL);
   #endif
   tolua_beginmodule(tolua_S,"LuaGuidObjectTable");
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_new00_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_delete00);
    tolua_function(tolua_S,"LuaRegisterCreateObject",tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterCreateObject00);
    tolua_function(tolua_S,"LuaRegisterReleaseObject",tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_LuaRegisterReleaseObject00);
    tolua_function(tolua_S,"ForeachIndexer",tolua_lua_core_obj_binding_core_obj_LuaGuidObjectTable_ForeachIndexer00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"ByteArrayCompress","core_obj::ByteArrayCompress","",tolua_collect_core_obj__ByteArrayCompress);
   #else
   tolua_cclass(tolua_S,"ByteArrayCompress","core_obj::ByteArrayCompress","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"ByteArrayCompress");
    tolua_variable(tolua_S,"inst",tolua_get_core_obj__ByteArrayCompress_inst,tolua_set_core_obj__ByteArrayCompress_inst);
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_new00_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_delete00);
    tolua_function(tolua_S,"Compress",tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_Compress00);
    tolua_function(tolua_S,"UnCompress",tolua_lua_core_obj_binding_core_obj_ByteArrayCompress_UnCompress00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"RemoteOperatorAdapter","core_obj::RemoteOperatorAdapter","",tolua_collect_core_obj__RemoteOperatorAdapter);
   #else
   tolua_cclass(tolua_S,"RemoteOperatorAdapter","core_obj::RemoteOperatorAdapter","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"RemoteOperatorAdapter");
    tolua_function(tolua_S,"new",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00);
    tolua_function(tolua_S,"new_local",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00_local);
    tolua_function(tolua_S,".call",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_new00_local);
    tolua_function(tolua_S,"delete",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_delete00);
    tolua_function(tolua_S,"InitNoticeInfo",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_InitNoticeInfo00);
    tolua_function(tolua_S,"CallPutObject",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallPutObject00);
    tolua_function(tolua_S,"CallRemoveObject",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallRemoveObject00);
    tolua_function(tolua_S,"CallAddWatch",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallAddWatch00);
    tolua_function(tolua_S,"CallDelWatch",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallDelWatch00);
    tolua_function(tolua_S,"CallWatchObjects",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallWatchObjects00);
    tolua_function(tolua_S,"CallDelWatchObjs",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallDelWatchObjs00);
    tolua_function(tolua_S,"CallCreateIndex",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallCreateIndex00);
    tolua_function(tolua_S,"CallReleaseIndex",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallReleaseIndex00);
    tolua_function(tolua_S,"CallSetTag",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallSetTag00);
    tolua_function(tolua_S,"CallUnSetTAg",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_CallUnSetTAg00);
    tolua_function(tolua_S,"Update",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_Update00);
    tolua_function(tolua_S,"HandlePacket",tolua_lua_core_obj_binding_core_obj_RemoteOperatorAdapter_HandlePacket00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_lua_core_obj_binding (lua_State* tolua_S) {
 return tolua_lua_core_obj_binding_open(tolua_S);
};
#endif

