#ifndef _LUA_GUID_OBJCET_TABLE_H_
#define _LUA_GUID_OBJCET_TABLE_H_

#include <core_obj/GuidObject.h>
#include <core_obj/GuidObjectTable.h>
#include <core_obj/ByteArray.h>
#include <core_obj/RemoteOperatorAdapter.h>

struct lua_State;

namespace core_obj {


//////////////////////////////////////////////////////////////////////////
//for GuidObjectTable
class LuaGuidObjectTable:public GuidObjectTable
{
public:

	LuaGuidObjectTable();
	virtual ~LuaGuidObjectTable();

	void RegisterCreateObject(lua_State *tolua_S,unsigned int callback_idx);
	void RegisterAttachObject(lua_State *tolua_S,unsigned int callback_idx);
	void RegisterReleaseObject(lua_State *tolua_S,unsigned int callback_idx);

	virtual GuidObject* CreateObject(string guid);
	virtual void AttachObject(GuidObject* obj);
	virtual void ReleaseObject(string guid);

	//遍历所有对象,key为空时遍历所有 TODO:先放这里，测试ok挪到基类
	//委托返回非0则终止循环
	void ForeachIndexer(const string& key,std::function<bool(GuidObject*)> f);
private:
	int lua_create_object_callback_index_;
	int lua_attach_object_callback_index_;
	int lua_release_object_callback_index_;	
	lua_State *tolua_S_;
};


//////////////////////////////////////////////////////////////////////////
//for LuaGuidObject
class LuaGuidObject:public GuidObject{
public:
	LuaGuidObject(SyncMode m_ = SYNC_SLAVE,const string& key = "");
	virtual ~LuaGuidObject();

	bool DoCallback(uint32_t cb_idx,BinLogStru *binlog);

	//注册回调
	void RegisterHanlder(lua_State *tolua_S, int index, uint32_t callback_idx);
	void UnRegisterHanlder(uint32_t callback_idx);

	//去掉回调,如果index等于
	void UnRegisterHanlderByIndex(int index);
	void RegisterAfterUpdate(lua_State *tolua_S,uint32_t callback_idx);
	//对象更新发生以后
private:
	std::multimap<int,int> scriptHanlders_;
	lua_State *tolua_S_;
	uint32_t after_update_hanlder;
};

}

#endif
