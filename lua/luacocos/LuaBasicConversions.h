/****************************************************************************
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#ifndef __COCOS2DX_SCRIPTING_LUA_COCOS2DXSUPPORT_LUABAISCCONVERSIONS_H__
#define __COCOS2DX_SCRIPTING_LUA_COCOS2DXSUPPORT_LUABAISCCONVERSIONS_H__

extern "C" {
#include "lua.h"
#include "tolua++.h"
}
#include "tolua_fix.h"

typedef long ssize_t;
//#include "cocos2d.h"

//using namespace cocos2d;

extern std::unordered_map<std::string, std::string>  g_luaType;
extern std::unordered_map<std::string, std::string>  g_typeCast;

#if COCOS2D_DEBUG >=1
void luaval_to_native_err(lua_State* L,const char* msg,tolua_Error* err);
#endif

#define LUA_PRECONDITION( condition, ...) if( ! (condition) ) {														\
printf("lua: ERROR: File %s: Line: %d, Function: %s", __FILE__, __LINE__, __FUNCTION__ );                                                         \
printf(__VA_ARGS__);                                                  \
}                                                                           \

extern bool luaval_is_usertype(lua_State* L,int lo,const char* type, int def);
// to native
extern bool luaval_to_ushort(lua_State* L, int lo, unsigned short* outValue);
extern bool luaval_to_int32(lua_State* L,int lo,int* outValue);
extern bool luaval_to_uint32(lua_State* L, int lo, unsigned int* outValue);
extern bool luaval_to_uint16(lua_State* L,int lo,uint16_t* outValue);
extern bool luaval_to_boolean(lua_State* L,int lo,bool* outValue);
extern bool luaval_to_number(lua_State* L,int lo,double* outValue);
extern bool luaval_to_long_long(lua_State* L,int lo,long long* outValue);
extern bool luaval_to_std_string(lua_State* L, int lo, std::string* outValue);
extern bool luaval_to_long(lua_State* L,int lo, long* outValue);
extern bool luaval_to_ssize(lua_State* L,int lo, ssize_t* outValue);

//extern bool luaval_to_point(lua_State* L,int lo,Point* outValue);
//extern bool luaval_to_size(lua_State* L,int lo,Size* outValue);
//extern bool luaval_to_rect(lua_State* L,int lo,Rect* outValue);
//extern bool luaval_to_color3b(lua_State* L,int lo,Color3B* outValue);
//extern bool luaval_to_color4b(lua_State* L,int lo,Color4B* outValue);
//extern bool luaval_to_color4f(lua_State* L,int lo,Color4F* outValue);
//extern bool luaval_to_physics_material(lua_State* L,int lo, cocos2d::PhysicsMaterial* outValue);
//extern bool luaval_to_affinetransform(lua_State* L,int lo, AffineTransform* outValue);
//extern bool luaval_to_fontdefinition(lua_State* L, int lo, FontDefinition* outValue );
//extern bool luaval_to_array(lua_State* L,int lo, __Array** outValue);
//extern bool luaval_to_dictionary(lua_State* L,int lo, __Dictionary** outValue);
//extern bool luaval_to_array_of_Point(lua_State* L,int lo,Point **points, int *numPoints);
//extern bool luavals_variadic_to_array(lua_State* L,int argc, __Array** ret);
//extern bool luavals_variadic_to_ccvaluevector(lua_State* L, int argc, cocos2d::ValueVector* ret);

template <class T>
bool luavals_variadic_to_vector( lua_State* L, int argc, std::vector<T>* ret)
{
    if (nullptr == L || argc == 0 )
        return false;
    
    bool ok = true;
    
    for (int i = 0; i < argc; i++)
    {
        if (lua_isuserdata(L, i + 2))
        {
            tolua_Error err;
            
            //if (!tolua_isusertype(L, i + 2, "cc.Ref", 0, &err))
            //{
            //    ok = false;
            //    break;
            //}
            T obj = static_cast<T>(tolua_tousertype(L, i + 2, nullptr));
            ret->push_back(obj);
        }
    }
    
    return ok;
}

template <class T>
bool luaval_to_vector(lua_State* L, int lo , std::vector<T>* ret)
{
    if (nullptr == L || nullptr == ret)
        return false;
    
    bool ok = true;
    
    tolua_Error tolua_err;
    if (!tolua_istable(L, lo, 0, &tolua_err) )
        ok = false;
    
    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        for (int i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L, lo);
            
            if (lua_isnil(L, -1) || !lua_isuserdata(L, -1))
            {
                lua_pop(L, 1);
                continue;
            }
            

            T cobj = static_cast<T>(tolua_tousertype(L, -1, NULL) );
            if (NULL != cobj)
                ret->push_back(cobj);
            
            lua_pop(L, 1);
        }
    }
    
    return ok;
}

bool luaval_to_std_vector_string(lua_State* L, int lo, std::vector<std::string>* ret);
bool luaval_to_std_vector_int(lua_State* L, int lo, std::vector<int>* ret);

template <class T>
bool luaval_to_map_string_key(lua_State* L, int lo, std::map<std::string, T>* ret)
{
    if(nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;
    
    tolua_Error tolua_err;
    bool ok = true;
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err);
#endif
        ok = false;
    }
    
    if (ok)
    {
        std::string stringKey = "";
        lua_pushnil(L);                                             /* first key L: lotable ..... nil */
        while ( 0 != lua_next(L, lo ) )                             /* L: lotable ..... key value */
        {
            if (!lua_isstring(L, -2))
            {
                lua_pop(L, 1);                                      /* removes 'value'; keep 'key' for next iteration*/
                continue;
            }
            
            if (lua_isnil(L, -1) || !lua_isuserdata(L, -1))
            {
                lua_pop(L, 1);
                continue;
            }
            
            luaval_to_std_string(L, -2, &stringKey);
            T obj = static_cast<T>(tolua_tousertype(L, -1, NULL) );
            if (nullptr != obj)
                ret->insert(stringKey, obj);
                
            lua_pop(L, 1);                                          /* L: lotable ..... key */
        }
    }
    
    return ok;
}


//extern bool luaval_to_ccvalue(lua_State* L, int lo, cocos2d::Value* ret);
//extern bool luaval_to_ccvaluemap(lua_State* L, int lo, cocos2d::ValueMap* ret);
//extern bool luaval_to_ccvaluemapintkey(lua_State* L, int lo, cocos2d::ValueMapIntKey* ret);
//extern bool luaval_to_ccvaluevector(lua_State* L, int lo, cocos2d::ValueVector* ret);

template <class T>
bool luaval_to_object(lua_State* L, int lo, const char* type, T** ret)
{
    if(nullptr == L || lua_gettop(L) < lo)
        return false;
    
    if (!luaval_is_usertype(L, lo, type, 0))
        return false;
    
    *ret = static_cast<T*>(tolua_tousertype(L, lo, 0));
    
    if (nullptr == ret)
        LUA_PRECONDITION(ret, "Invalid Native Object");
    
    return true;
}

/**
 Because all override functions wouldn't be bound,so we must use `typeid` to get the real class name
 */
template <class T>
const char* getLuaTypeName(T* ret,const char* type)
{
    if (nullptr != ret)
    {
        std::string hashName = typeid(*ret).name();
        auto iter =  g_luaType.find(hashName);
        if(g_luaType.end() != iter)
        {
            return iter->second.c_str();
        }
        else
        {
            return type;
        }
    }
    
    return nullptr;
}

/*
template <class T>
void object_to_luaval(lua_State* L,const char* type, T* ret)
{
    if(nullptr != ret)
    {
      
        cocos2d::Ref* dynObject = dynamic_cast<cocos2d::Ref *>(ret);

        if (nullptr != dynObject)
        {
            int ID = (int)(dynObject->_ID) ;
            int* luaID = &(dynObject->_luaID);
            toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,type);
        }
        else
        {
            tolua_pushusertype(L,(void*)ret,getLuaTypeName(ret, type));
        }
    }
    else
    {
        lua_pushnil(L);
    }
}
*/

#endif //__COCOS2DX_SCRIPTING_LUA_COCOS2DXSUPPORT_LUABAISCCONVERSIONS_H__
