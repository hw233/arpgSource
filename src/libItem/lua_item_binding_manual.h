#ifndef _LUA_ITEM_BINDING_MANUAL_H_
#define _LUA_ITEM_BINDING_MANUAL_H_

#include "tolua++.h"
#include "tolua_fix.h"
#include "ItemManager.h"
#include "Item.h"

#define TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_ForEachBagItem00
#define TOLUA_DISABLE_tolua_lua_item_binding_ItemManager_Init00
#define TOLUA_DISABLE_tolua_lua_item_binding_Item_ForEachBaseAttr00

static int tolua_lua_item_binding_ItemManager_ForEachBagItem00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
//	tolua_Error tolua_err;
// 	if (
// 		!tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
// 		!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
// 		(tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"std::function<void(Item*)>",0,&tolua_err)) ||
// 		!tolua_isnoobj(tolua_S,4,&tolua_err)
// 		)
// 		goto tolua_lerror;
// 	else
#endif
	{
		ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
		int bag = ((int)  tolua_tonumber(tolua_S,2,0));
		int cb = ((int)  toluafix_ref_function(tolua_S,3,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ForEachBagItem'", NULL);
#endif
		{
			self->ForEachBagItem(bag,[cb,tolua_S](Item* item, int pos)->bool{
				int statePos = lua_gettop(tolua_S);
				//½«»Øµ÷º¯Êý»Ö¸´µ½Õ»¶¥
				toluafix_get_function_by_refid(tolua_S,cb);
				bool ret = false;
				if(lua_isfunction(tolua_S,-1)){
					tolua_pushusertype(tolua_S,(void*)item,"Item"); //lua_pushlightuserdata(tolua_S,item);
					tolua_pushnumber(tolua_S,(lua_Number)pos);		//lua_pushnumber(tolua_S, pos);
					lua_call(tolua_S,2,1);
					ret = tolua_toboolean(tolua_S, -1, 0) != 0;
				}
				//»Ö¸´luaÕ»×´Ì¬
				lua_settop(tolua_S, statePos);
				return ret;
			});
			//»Øµ÷ºóÒÆ³ý×¢²á
			toluafix_remove_function_by_refid(tolua_S,cb);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
//tolua_lerror:
//	tolua_error(tolua_S,"#ferror in function 'ForEachBagItem'.",&tolua_err);
	return 0;
#endif
}

static int tolua_lua_item_binding_ItemManager_Init00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
//	tolua_Error tolua_err;
// 	if (
// 		!tolua_isusertype(tolua_S,1,"ItemManager",0,&tolua_err) ||
// 		!tolua_isusertype(tolua_S,2,"GuidObject",0,&tolua_err) ||
// 		!tolua_isnoobj(tolua_S,3,&tolua_err)
// 		)
// 		goto tolua_lerror;
// 	else
#endif
	{
		ItemManager* self = (ItemManager*)  tolua_tousertype(tolua_S,1,0);
		GuidObject* obj = ((GuidObject*)  lua_touserdata(tolua_S,2));//tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Init'", NULL);
#endif
		{
			self->Init(obj);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
//tolua_lerror:
//	tolua_error(tolua_S,"#ferror in function 'Init'.",&tolua_err);
	return 0;
#endif
}



static int tolua_lua_item_binding_Item_ForEachBaseAttr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	//tolua_Error tolua_err;
	//if (
	//	!tolua_isusertype(tolua_S,1,"Item",0,&tolua_err) ||
	//	(tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"std::function<void(int, double)>",0,&tolua_err)) ||
	//	!tolua_isnoobj(tolua_S,3,&tolua_err)
	//	)
	//	goto tolua_lerror;
	//else
#endif
	{
		Item* self = (Item*)  tolua_tousertype(tolua_S,1,0);
		int cb = ((int)  toluafix_ref_function(tolua_S,2,0));

		//std::function<void(int, double)> func = *((std::function<void(int, double)>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
		if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ForEachBaseAttr'", NULL);
#endif
		{
			//self->ForEachBaseAttr(func);

			self->ForEachBaseAttr([cb,tolua_S](int key, double value)->void{
				int statePos = lua_gettop(tolua_S);
				//½«»Øµ÷º¯Êý»Ö¸´µ½Õ»¶¥
				toluafix_get_function_by_refid(tolua_S,cb);
				
				if(lua_isfunction(tolua_S,-1)){
					tolua_pushnumber(tolua_S, (lua_Number)key);		//lua_pushnumber(tolua_S, pos);
					//tolua_pushusertype(tolua_S,(void*)item,"Item"); //lua_pushlightuserdata(tolua_S,item);
					tolua_pushnumber(tolua_S,(lua_Number)value);		//lua_pushnumber(tolua_S, pos);
					lua_call(tolua_S,2,0);
					//ret = tolua_toboolean(tolua_S, -1, 0) != 0;
				}
				//»Ö¸´luaÕ»×´Ì¬
				lua_settop(tolua_S, statePos);
			});
			//»Øµ÷ºóÒÆ³ý×¢²á
			toluafix_remove_function_by_refid(tolua_S,cb);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
//tolua_lerror:
//	tolua_error(tolua_S,"#ferror in function 'ForEachBaseAttr'.",&tolua_err);
	return 0;
#endif
}




#endif


