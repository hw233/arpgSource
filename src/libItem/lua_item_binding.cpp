/*
** Lua binding: lua_item_binding
** Generated automatically by tolua++-1.0.92 on 04/14/17 15:36:53.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_lua_item_binding_open (lua_State* tolua_S);

#include "lua_item_binding_manual.h"
#include "Item.h"
#include "ItemManager.h"
#include <string>
#include <map>
#include <stdint.h>
#include <core_obj/GuidObject.h>
using core_obj::GuidObject;
using std::string;

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Item (lua_State* tolua_S)
{
 Item* self = (Item*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_ItemManager (lua_State* tolua_S)
{
 ItemManager* self = (ItemManager*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"GuidObject");
 tolua_usertype(tolua_S,"Item");
 tolua_usertype(tolua_S,"std::function<void(Item*)>");
 tolua_usertype(tolua_S,"ItemManager");
 tolua_usertype(tolua_S,"std::function<void(int, double)>");
}

/* method: new of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_new00
static int tolua_lua_item_binding_Item_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Item* tolua_ret = (Item*)  Mtolua_new((Item)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Item");
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

/* method: new_local of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_new00_local
static int tolua_lua_item_binding_Item_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Item* tolua_ret = (Item*)  Mtolua_new((Item)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Item");
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

/* method: delete of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_delete00
static int tolua_lua_item_binding_Item_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
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

/* method: GetId of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_GetId00
static int tolua_lua_item_binding_Item_GetId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetId'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetId();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEntry of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_GetEntry00
static int tolua_lua_item_binding_Item_GetEntry00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEntry'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetEntry();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEntry'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetEntry of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_SetEntry00
static int tolua_lua_item_binding_Item_SetEntry00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  int value = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetEntry'", NULL);
#endif
  {
   self->SetEntry(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetEntry'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCount of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_GetCount00
static int tolua_lua_item_binding_Item_GetCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetCount();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCount of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_SetCount00
static int tolua_lua_item_binding_Item_SetCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  int value = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCount'", NULL);
#endif
  {
   self->SetCount(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBit of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_GetBit00
static int tolua_lua_item_binding_Item_GetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBit'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->GetBit(pos);
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

/* method: SetBit of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_SetBit00
static int tolua_lua_item_binding_Item_SetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBit'", NULL);
#endif
  {
   self->SetBit(pos);
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

/* method: UnSetBit of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_UnSetBit00
static int tolua_lua_item_binding_Item_UnSetBit00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  int pos = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'UnSetBit'", NULL);
#endif
  {
   self->UnSetBit(pos);
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

/* method: SetAttr of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_SetAttr00
static int tolua_lua_item_binding_Item_SetAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  const string key = ((const string)  tolua_tocppstring(tolua_S,2,0));
  double values = ((double)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetAttr'", NULL);
#endif
  {
   self->SetAttr(key,values);
   tolua_pushcppstring(tolua_S,(const char*)key);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetAttr of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_GetAttr00
static int tolua_lua_item_binding_Item_GetAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  string key = ((string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetAttr'", NULL);
#endif
  {
   double tolua_ret = (double)  self->GetAttr(key);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ClearAttr of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_ClearAttr00
static int tolua_lua_item_binding_Item_ClearAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ClearAttr'", NULL);
#endif
  {
   self->ClearAttr();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBaseAttr of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_SetBaseAttr00
static int tolua_lua_item_binding_Item_SetBaseAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  unsigned int pos = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
  int key = ((int)  tolua_tonumber(tolua_S,3,0));
  double val = ((double)  tolua_tonumber(tolua_S,4,0));
  int qua = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBaseAttr'", NULL);
#endif
  {
   self->SetBaseAttr(pos,key,val,qua);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBaseAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddBaseAttr of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_AddBaseAttr00
static int tolua_lua_item_binding_Item_AddBaseAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  int key = ((int)  tolua_tonumber(tolua_S,2,0));
  double val = ((double)  tolua_tonumber(tolua_S,3,0));
  int qua = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddBaseAttr'", NULL);
#endif
  {
   self->AddBaseAttr(key,val,qua);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddBaseAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ClearBaseAttr of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_ClearBaseAttr00
static int tolua_lua_item_binding_Item_ClearBaseAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ClearBaseAttr'", NULL);
#endif
  {
   self->ClearBaseAttr();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ClearBaseAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ForEachBaseAttr of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_ForEachBaseAttr00
static int tolua_lua_item_binding_Item_ForEachBaseAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"std::function<void(int, double)>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  std::function<void(int, double)> func = *((std::function<void(int, double)>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ForEachBaseAttr'", NULL);
#endif
  {
   self->ForEachBaseAttr(func);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ForEachBaseAttr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetGuid of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_GetGuid00
static int tolua_lua_item_binding_Item_GetGuid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGuid'", NULL);
#endif
  {
   string tolua_ret = (string)  self->GetGuid();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGuid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FromString of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_FromString00
static int tolua_lua_item_binding_Item_FromString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
  const string s = ((const string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FromString'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->FromString(s);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)s);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FromString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ToString of class  Item */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_Item_ToString00
static int tolua_lua_item_binding_Item_ToString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Item* self = (const Item*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ToString'", NULL);
#endif
  {
   string tolua_ret = (string)  self->ToString();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ToString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_new00
static int tolua_lua_item_binding_ItemManager_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ItemManager* tolua_ret = (ItemManager*)  Mtolua_new((ItemManager)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ItemManager");
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

/* method: new_local of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_new00_local
static int tolua_lua_item_binding_ItemManager_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ItemManager* tolua_ret = (ItemManager*)  Mtolua_new((ItemManager)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ItemManager");
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

/* method: delete of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_delete00
static int tolua_lua_item_binding_ItemManager_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
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

/* method: Init of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_Init00
static int tolua_lua_item_binding_ItemManager_Init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"GuidObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  GuidObject* obj = ((GuidObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Init'", NULL);
#endif
  {
   self->Init(obj);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Init'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetByPos of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_GetByPos00
static int tolua_lua_item_binding_ItemManager_GetByPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int bag_type = ((int)  tolua_tonumber(tolua_S,2,0));
  int pos = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetByPos'", NULL);
#endif
  {
   Item* tolua_ret = (Item*)  self->GetByPos(bag_type,pos);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Item");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetByPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Get of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_Get00
static int tolua_lua_item_binding_ItemManager_Get00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Get'", NULL);
#endif
  {
   Item* tolua_ret = (Item*)  self->Get(id);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Item");
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

/* method: GetItemByGuid of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_GetItemByGuid00
static int tolua_lua_item_binding_ItemManager_GetItemByGuid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  const string guid = ((const string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetItemByGuid'", NULL);
#endif
  {
   Item* tolua_ret = (Item*)  self->GetItemByGuid(guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Item");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetItemByGuid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Add of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_Add00
static int tolua_lua_item_binding_ItemManager_Add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Item",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  const Item* item = ((const Item*)  tolua_tousertype(tolua_S,2,0));
  int bag = ((int)  tolua_tonumber(tolua_S,3,0));
  int pos = ((int)  tolua_tonumber(tolua_S,4,-1));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Add'", NULL);
#endif
  {
   int tolua_ret = (int)  self->Add(*item,bag,pos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Del of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_Del00
static int tolua_lua_item_binding_ItemManager_Del00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Del'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Del(id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Del'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Sub of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_Sub00
static int tolua_lua_item_binding_ItemManager_Sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int entry = ((int)  tolua_tonumber(tolua_S,2,0));
  int count = ((int)  tolua_tonumber(tolua_S,3,0));
  int bag = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Sub'", NULL);
#endif
  {
   int tolua_ret = (int)  self->Sub(entry,count,bag);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Save of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_Save00
static int tolua_lua_item_binding_ItemManager_Save00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Item",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  const Item* item = ((const Item*)  tolua_tousertype(tolua_S,2,0));
  int bag = ((int)  tolua_tonumber(tolua_S,3,0));
  int pos = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Save'", NULL);
#endif
  {
   self->Save(*item,bag,pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Save'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SavePtr of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_SavePtr00
static int tolua_lua_item_binding_ItemManager_SavePtr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Item",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  const Item* item = ((const Item*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SavePtr'", NULL);
#endif
  {
   self->SavePtr(item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SavePtr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Move of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_Move00
static int tolua_lua_item_binding_ItemManager_Move00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  int to_bag = ((int)  tolua_tonumber(tolua_S,3,0));
  int to_pos = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Move'", NULL);
#endif
  {
   self->Move(id,to_bag,to_pos);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Move'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ForEachBagItem of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_ForEachBagItem00
static int tolua_lua_item_binding_ItemManager_ForEachBagItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::function<void(Item*)>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int bag = ((int)  tolua_tonumber(tolua_S,2,0));
  std::function<void(Item*)> func = *((std::function<void(Item*)>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ForEachBagItem'", NULL);
#endif
  {
   self->ForEachBagItem(bag,func);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ForEachBagItem'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetGuid of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_GetGuid00
static int tolua_lua_item_binding_ItemManager_GetGuid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetGuid'", NULL);
#endif
  {
   const string tolua_ret = (const string)  self->GetGuid();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetGuid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: CountEntey of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_CountEntey00
static int tolua_lua_item_binding_ItemManager_CountEntey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int entry = ((int)  tolua_tonumber(tolua_S,2,0));
  int bag = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CountEntey'", NULL);
#endif
  {
   int tolua_ret = (int)  self->CountEntey(entry,bag);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CountEntey'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEmptyCount of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_GetEmptyCount00
static int tolua_lua_item_binding_ItemManager_GetEmptyCount00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int bag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEmptyCount'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetEmptyCount(bag);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEmptyCount'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetBagSize of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_GetBagSize00
static int tolua_lua_item_binding_ItemManager_GetBagSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int bag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetBagSize'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetBagSize(bag);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetBagSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetBagSize of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_SetBagSize00
static int tolua_lua_item_binding_ItemManager_SetBagSize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int bag = ((int)  tolua_tonumber(tolua_S,2,0));
  int size = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetBagSize'", NULL);
#endif
  {
   self->SetBagSize(bag,size);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetBagSize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetEmptyPos of class  ItemManager */
#ifndef TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_GetEmptyPos00
static int tolua_lua_item_binding_ItemManager_GetEmptyPos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
  int bag = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetEmptyPos'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetEmptyPos(bag);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetEmptyPos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_lua_item_binding_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"Item","Item","",tolua_collect_Item);
  #else
  tolua_cclass(tolua_S,"Item","Item","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"Item");
   tolua_function(tolua_S,"new",tolua_lua_item_binding_Item_new00);
   tolua_function(tolua_S,"new_local",tolua_lua_item_binding_Item_new00_local);
   tolua_function(tolua_S,".call",tolua_lua_item_binding_Item_new00_local);
   tolua_function(tolua_S,"delete",tolua_lua_item_binding_Item_delete00);
   tolua_function(tolua_S,"GetId",tolua_lua_item_binding_Item_GetId00);
   tolua_function(tolua_S,"GetEntry",tolua_lua_item_binding_Item_GetEntry00);
   tolua_function(tolua_S,"SetEntry",tolua_lua_item_binding_Item_SetEntry00);
   tolua_function(tolua_S,"GetCount",tolua_lua_item_binding_Item_GetCount00);
   tolua_function(tolua_S,"SetCount",tolua_lua_item_binding_Item_SetCount00);
   tolua_function(tolua_S,"GetBit",tolua_lua_item_binding_Item_GetBit00);
   tolua_function(tolua_S,"SetBit",tolua_lua_item_binding_Item_SetBit00);
   tolua_function(tolua_S,"UnSetBit",tolua_lua_item_binding_Item_UnSetBit00);
   tolua_function(tolua_S,"SetAttr",tolua_lua_item_binding_Item_SetAttr00);
   tolua_function(tolua_S,"GetAttr",tolua_lua_item_binding_Item_GetAttr00);
   tolua_function(tolua_S,"ClearAttr",tolua_lua_item_binding_Item_ClearAttr00);
   tolua_function(tolua_S,"SetBaseAttr",tolua_lua_item_binding_Item_SetBaseAttr00);
   tolua_function(tolua_S,"AddBaseAttr",tolua_lua_item_binding_Item_AddBaseAttr00);
   tolua_function(tolua_S,"ClearBaseAttr",tolua_lua_item_binding_Item_ClearBaseAttr00);
   tolua_function(tolua_S,"ForEachBaseAttr",tolua_lua_item_binding_Item_ForEachBaseAttr00);
   tolua_function(tolua_S,"GetGuid",tolua_lua_item_binding_Item_GetGuid00);
   tolua_function(tolua_S,"FromString",tolua_lua_item_binding_Item_FromString00);
   tolua_function(tolua_S,"ToString",tolua_lua_item_binding_Item_ToString00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ItemManager","ItemManager","",tolua_collect_ItemManager);
  #else
  tolua_cclass(tolua_S,"ItemManager","ItemManager","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ItemManager");
   tolua_function(tolua_S,"new",tolua_lua_item_binding_ItemManager_new00);
   tolua_function(tolua_S,"new_local",tolua_lua_item_binding_ItemManager_new00_local);
   tolua_function(tolua_S,".call",tolua_lua_item_binding_ItemManager_new00_local);
   tolua_function(tolua_S,"delete",tolua_lua_item_binding_ItemManager_delete00);
   tolua_function(tolua_S,"Init",tolua_lua_item_binding_ItemManager_Init00);
   tolua_function(tolua_S,"GetByPos",tolua_lua_item_binding_ItemManager_GetByPos00);
   tolua_function(tolua_S,"Get",tolua_lua_item_binding_ItemManager_Get00);
   tolua_function(tolua_S,"GetItemByGuid",tolua_lua_item_binding_ItemManager_GetItemByGuid00);
   tolua_function(tolua_S,"Add",tolua_lua_item_binding_ItemManager_Add00);
   tolua_function(tolua_S,"Del",tolua_lua_item_binding_ItemManager_Del00);
   tolua_function(tolua_S,"Sub",tolua_lua_item_binding_ItemManager_Sub00);
   tolua_function(tolua_S,"Save",tolua_lua_item_binding_ItemManager_Save00);
   tolua_function(tolua_S,"SavePtr",tolua_lua_item_binding_ItemManager_SavePtr00);
   tolua_function(tolua_S,"Move",tolua_lua_item_binding_ItemManager_Move00);
   tolua_function(tolua_S,"ForEachBagItem",tolua_lua_item_binding_ItemManager_ForEachBagItem00);
   tolua_function(tolua_S,"GetGuid",tolua_lua_item_binding_ItemManager_GetGuid00);
   tolua_function(tolua_S,"CountEntey",tolua_lua_item_binding_ItemManager_CountEntey00);
   tolua_function(tolua_S,"GetEmptyCount",tolua_lua_item_binding_ItemManager_GetEmptyCount00);
   tolua_function(tolua_S,"GetBagSize",tolua_lua_item_binding_ItemManager_GetBagSize00);
   tolua_function(tolua_S,"SetBagSize",tolua_lua_item_binding_ItemManager_SetBagSize00);
   tolua_function(tolua_S,"GetEmptyPos",tolua_lua_item_binding_ItemManager_GetEmptyPos00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_lua_item_binding (lua_State* tolua_S) {
 return tolua_lua_item_binding_open(tolua_S);
};
#endif

