
#ifndef __TOLUA_FIX_H_
#define __TOLUA_FIX_H_

//////////////////////////////////////////////////////////////////////////
//linbc add on 20140718
#include <assert.h>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <stdint.h>
#include <typeinfo>
#include <string.h>


#define CCLOG printf

//#if COCOS2D_DEBUG
//extern bool CC_DLL cc_assert_script_compatible(const char *msg);
#define CCASSERT(cond, msg) do {                              \
	if (!(cond)) {                                          \
	if (/*!cc_assert_script_compatible(msg) && */strlen(msg)) \
	CCLOG("Assert failed: %s", msg);             \
	assert(cond);                                      \
	} \
} while (0)
#define CC_ASSERT assert
//#endif  // CCASSERT

//////////////////////////////////////////////////////////////////////////
#include "tolua++.h"
    
#define TOLUA_REFID_PTR_MAPPING "toluafix_refid_ptr_mapping"
#define TOLUA_REFID_TYPE_MAPPING "toluafix_refid_type_mapping"
#define TOLUA_REFID_FUNCTION_MAPPING "toluafix_refid_function_mapping"
    
TOLUA_API void toluafix_open(lua_State* L);
/*TOLUA_API int toluafix_pushusertype_ccobject(lua_State* L,
                                             int uid,
                                             int* p_refid,
                                             void* ptr,
                                             const char* type);
TOLUA_API int toluafix_remove_ccobject_by_refid(lua_State* L, int refid);*/
TOLUA_API int toluafix_ref_function(lua_State* L, int lo, int def);
TOLUA_API void toluafix_get_function_by_refid(lua_State* L, int refid);
TOLUA_API void toluafix_remove_function_by_refid(lua_State* L, int refid);
TOLUA_API int toluafix_isfunction(lua_State* L, int lo, const char* type, int def, tolua_Error* err);
TOLUA_API int toluafix_totable(lua_State* L, int lo, int def);
TOLUA_API int toluafix_istable(lua_State* L, int lo, const char* type, int def, tolua_Error* err);
TOLUA_API void toluafix_stack_dump(lua_State* L, const char* label);


#endif // __TOLUA_FIX_H_
