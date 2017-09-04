#include "LuaGuidObjectTable.h"
#include <tolua++.h>
#include <tolua_fix.h>

namespace core_obj {


//////////////////////////////////////////////////////////////////////////
//for GuidObjectTable
LuaGuidObjectTable::LuaGuidObjectTable():GuidObjectTable(SYNC_SLAVE),lua_create_object_callback_index_(0),lua_attach_object_callback_index_(0),lua_release_object_callback_index_(0),tolua_S_(0)
{
}

LuaGuidObjectTable::~LuaGuidObjectTable(){
	if (tolua_S_ && lua_create_object_callback_index_)
	{
		toluafix_remove_function_by_refid(tolua_S_,lua_create_object_callback_index_);
		lua_create_object_callback_index_ = 0;
	}
	objs_.clear();	//内存由lua进行管理
};

void LuaGuidObjectTable::RegisterCreateObject(lua_State *tolua_S,unsigned int callback_idx)
{
	tolua_S_ = tolua_S;
	lua_create_object_callback_index_ = callback_idx;
}

void LuaGuidObjectTable::RegisterAttachObject(lua_State *tolua_S,unsigned int callback_idx)
{
	tolua_S_ = tolua_S;
	lua_attach_object_callback_index_ = callback_idx;
}

void LuaGuidObjectTable::RegisterReleaseObject(lua_State *tolua_S,unsigned int callback_idx)
{	
	tolua_S_ = tolua_S;
	lua_release_object_callback_index_ = callback_idx;
} 

GuidObject* LuaGuidObjectTable::CreateObject(string guid) 
{     
	GuidObject *obj = nullptr; 
	if(tolua_S_ && lua_create_object_callback_index_){
		int statePos = lua_gettop(tolua_S_);
		toluafix_get_function_by_refid(tolua_S_,lua_create_object_callback_index_);
		if(lua_isfunction(tolua_S_,-1)){ 
			//只有一个参数是包
			tolua_pushusertype(tolua_S_,(void*)this,"core_obj::LuaGuidObjectTable");
			tolua_pushcppstring(tolua_S_,guid);

			//TODO:这里应该使用安全调用
			lua_call(tolua_S_,2,1); 
			obj = (GuidObject *)tolua_tousertype(tolua_S_,-1,NULL);
			if (obj)
			{
				obj->guid(guid);
				AttachObject(obj);
			}
		}
		//把栈还原了
		lua_settop(tolua_S_,statePos);	
	}		

	return obj;
}

void LuaGuidObjectTable::AttachObject(GuidObject* obj)
{
	GuidObjectTable::AttachObject(obj);
	if(tolua_S_ && lua_create_object_callback_index_){
		int statePos = lua_gettop( tolua_S_ );
		toluafix_get_function_by_refid(tolua_S_,lua_attach_object_callback_index_);
		if(lua_isfunction(tolua_S_,-1)){
			//TODO:这里应该使用安全调用	
			tolua_pushusertype(tolua_S_,(void*)this,"core_obj::LuaGuidObjectTable");
			tolua_pushusertype(tolua_S_,(void*)obj,"core_obj::GuidObject");
			lua_call(tolua_S_,2,0);
		}
		lua_settop(tolua_S_,statePos);
	}	
}

void LuaGuidObjectTable::ReleaseObject(string guid)
{ 
	if(tolua_S_ && lua_release_object_callback_index_){
		int statePos = lua_gettop( tolua_S_ );
		toluafix_get_function_by_refid(tolua_S_,lua_release_object_callback_index_);
		if(lua_isfunction(tolua_S_,-1)){
			//TODO:这里应该使用安全调用	
			tolua_pushusertype(tolua_S_,(void*)this,"core_obj::LuaGuidObjectTable");
			tolua_pushcppstring(tolua_S_,guid);
			lua_call(tolua_S_,2,0);
		}
		//把栈还原了 
		lua_settop(tolua_S_,statePos);		 
	}
	//GuidObjectTable::ReleaseObject(guid);
	//从库的内存还是交给lua来管理吧
	auto *p = Get(guid);
	if(!p)
		return;
	DetachObject(p);
}


//遍历所有对象
void LuaGuidObjectTable::ForeachIndexer(const string& key,std::function<bool(GuidObject*)> f)
{
	//如果key为空,则简单遍历所有对象
	if(key.empty())
	{
		for (auto it = objs_.begin();it != objs_.end();++it)
		{
			if(f(it->second))
				return;		
		}
		return;
	}

	auto idx = indexer_.get(key);
	if(idx)
	{
		for (auto it = idx->begin();it != idx->end();++it)
		{
			if(f(it->second))
				return;
		}
	}
	else if(objs_.count(key) == 1)
	{
		f(Get(key));
	}
	else
	{		
		Regex e(key);	
		for (ValueHashMap::iterator it = objs_.begin();it != objs_.end();++it)
		{
			if(e.Search(it->first) && f(Get(it->first)))
				return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//for LuaGuidObject
LuaGuidObject::LuaGuidObject(SyncMode m_,const string& key ):GuidObject(SYNC_SLAVE),tolua_S_(0),after_update_hanlder(0)
{
}
LuaGuidObject::~LuaGuidObject()
{
	for (auto it = scriptHanlders_.begin();it != scriptHanlders_.end();++it) {
		toluafix_remove_function_by_refid(tolua_S_,it->second);
	}
	scriptHanlders_.clear();
	if (after_update_hanlder) {
		toluafix_remove_function_by_refid(tolua_S_,after_update_hanlder);
		after_update_hanlder = 0;
	}
}	

bool LuaGuidObject::DoCallback(uint32_t cb_idx,BinLogStru *binlog)
{
	bool ret = false;
	int statePos = lua_gettop( tolua_S_ );
	toluafix_get_function_by_refid(tolua_S_,cb_idx);
	if(lua_isfunction(tolua_S_,-1)){
		//只有一个参数是包
		tolua_pushusertype(tolua_S_,(void*)binlog,"core_obj::BinLogStru");
		//TODO:这里应该使用安全调用
		lua_call(tolua_S_,1,1);
		ret = lua_toboolean(tolua_S_,-1)!=0;
	}
	//把栈还原了
	lua_settop(tolua_S_,statePos);	
	return ret;
}

//注册回调
void LuaGuidObject::RegisterHanlder(lua_State *tolua_S, int index, uint32_t callback_idx)
{
	ASSERT(tolua_S_ == nullptr || tolua_S_ == tolua_S);
	auto f = [this](core_obj::BinLogStru *binlog){
		int index = binlog->index;
		//如果是字符串则转成负数
		if(binlog->typ == TYPE_STRING)
			index *= -1;
		auto it = scriptHanlders_.lower_bound(index);
		auto end = scriptHanlders_.upper_bound(index);
		while (it != end)
		{
			//如果执行的结果等于true则移除
			if(DoCallback(it->second,binlog)){
				toluafix_remove_function_by_refid(tolua_S_,it->second);
				scriptHanlders_.erase(it++);
			}else ++it;
		}			
	};

	//如果是第一次加入这个下标的更新则需要加入回调，否则不需要
	if(scriptHanlders_.count(index) == 0){
		if(index < 0 )
			events_str_value_.AddCallback(-1*index,f);
		else
			events_value_.AddCallback(index,f);
	}
	scriptHanlders_.insert(std::make_pair(index,callback_idx));
}

void  LuaGuidObject::UnRegisterHanlder(uint32_t callback_idx)
{		
	auto it = scriptHanlders_.begin();
	auto end = scriptHanlders_.end();
	while (it != end) {
		//如果执行的结果等于true则移除
		if(it->second == callback_idx) {				
			toluafix_remove_function_by_refid(tolua_S_,it->second);
			//如果是最后一个则删除
			if(scriptHanlders_.count(it->first) == 1){
				if(it->first < 0 )
					events_str_value_.DelCallback(-1*it->first);
				else
					events_value_.DelCallback(it->first);
			}				
			scriptHanlders_.erase(it++);
			break;
		}else ++it;
	}
}

//去掉回调,如果index等于
void LuaGuidObject::UnRegisterHanlderByIndex(int index)
{
	//如果index等于0则删除callback_idx		
	auto it = scriptHanlders_.lower_bound(index);
	auto end = scriptHanlders_.upper_bound(index);
	while (it != end) {
		//如果执行的结果等于true则移除				
		toluafix_remove_function_by_refid(tolua_S_,it->second);	
		scriptHanlders_.erase(it++);
	}
	if(index<0)
		events_str_value_.DelCallback(-1*index);
	else
		events_value_.DelCallback(index);
}

void LuaGuidObject::RegisterAfterUpdate(lua_State *tolua_S,uint32_t callback_idx)
{
	ASSERT(tolua_S_ == nullptr || tolua_S_ == tolua_S);
	tolua_S_ = tolua_S;

	//如果callback_idx等于0则化身为删除
	if(callback_idx == 0){
		if (after_update_hanlder) {
			toluafix_remove_function_by_refid(tolua_S_,after_update_hanlder);
			after_update_hanlder = 0;
		}
		after_update(nullptr);
		return;
	}

	after_update_hanlder = callback_idx;
	this->after_update([this](core_obj::SyncEventRecorder* obj,int flags,core_obj::UpdateMask& intMask,core_obj::UpdateMask& strMask){
		int statePos = lua_gettop( tolua_S_ );
		toluafix_get_function_by_refid(tolua_S_,after_update_hanlder);
		if(lua_isfunction(tolua_S_,-1)){ 
			tolua_pushusertype(tolua_S_,(void*)obj,"core_obj::LuaGuidObject");
			tolua_pushnumber(tolua_S_, flags);
			tolua_pushusertype(tolua_S_,(void*)&intMask,"core_obj::UpdateMask");
			tolua_pushusertype(tolua_S_,(void*)&strMask,"core_obj::UpdateMask");
			//TODO:这里应该使用安全调用
			lua_call(tolua_S_,4,0);
		}
		//把栈还原了
		lua_settop(tolua_S_,statePos);
	});		
}


}
