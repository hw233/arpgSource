#ifndef _SVR_OBJECT_MANAGER_H_
#define _SVR_OBJECT_MANAGER_H_

#include <core_obj/GuidObjectTable.h>
#include <core_obj/GuidObject.h>
#include <core_obj/RemoteOperatorAdapter.h>
#include <core_obj/ByteArrayCompress.h>
#include <object/BinLogObject.h>
#include <shared/lua_helper.h>

using core_obj::GuidObjectTable;
using core_obj::RemoteOperatorAdapter;
using core_obj::ByteArray;
using core_obj::GuidObject;
using core_obj::ByteArrayCompress;
using core_obj::AsyncByteArrayCompress;
using core_obj::CompressCallback;

class SvrCoreAppImpl;
struct TimerHolder;

#define GLOBAL_VALUE_OWNER_STRING "global_value"
#define FACTION_BINLOG_OWNER_STRING "faction"
#define FACTION_DATA_OWNER_STRING	"faction_data"
#define WORLD_BINLOG_OWNER_STRING "world"

//某所有者下所有数据
class OwnerDataSet
{
public:
public:
	OwnerDataSet(const string &owner)
		:m_owner_guid(owner)
	{
		
	}
	~OwnerDataSet(){}
public:
	const string &GetGuid() const
	{
		return m_owner_guid;
	}
	set<string> &GetAllData()
	{
		return m_all_data;
	}
private:
	//所有者guid
	string m_owner_guid;
	//所有对象
	set<string> m_all_data;
};

namespace objects{
	static int luaGet(lua_State *L);
	static int luaForeach(lua_State *L);
	static int luaNewAndCallPut(lua_State *L);	
	static int luaCallRemoveObject(lua_State *L);
	static int luaCallAddWatch(lua_State *L);
	static int luaCallDelWatch(lua_State *L);
}


//数据管理器基类
class SvrObjectManager : public GuidObjectTable, public RemoteOperatorAdapter
{
public:
	friend int objects::luaGet(lua_State *L);
	friend int objects::luaForeach(lua_State *L);
	friend int objects::luaNewAndCallPut(lua_State *L);	
	friend int objects::luaCallRemoveObject(lua_State *L);
	friend int objects::luaCallAddWatch(lua_State *L);
	friend int objects::luaCallDelWatch(lua_State *tolua_S);

	SvrObjectManager(SvrCoreAppImpl *app);
	virtual ~SvrObjectManager();
private:
	static SvrObjectManager *self;

	SvrCoreAppImpl *m_app;
private:
	virtual void Send(uint32_t connid,ByteArray&) = 0;

	bool AddToOwnerMap(const string &owner, GuidObject *obj);		//添加一个owner-guid
	//创建一个新的数据集
	inline OwnerDataSet *NewOwnerDataSet(const string &guid);
	
public:
	virtual void Update();
	virtual void AttachObject(GuidObject* obj);
	virtual void AfterAttachObject(GuidObject *obj);
	virtual void ReleaseObject(string guid);
	//对象工厂
	virtual GuidObject *ObjectFactor(const string &guid) = 0;
	//创建完binlog对象后的回调
	virtual void AfterCreateObjOwner(BinLogObject *binlog);
	//把对象插入到数据集中
	void InsertObjOwner(const string &guid);
	//从数据集中删除某个对象
	bool DelOwnerMap(const string owner, GuidObject *obj);
	//获取某数据集下面所有的对象
	void GetDataSetAllObject(const string &guid, vector<GuidObject*> &result);
	//获得指定玩家的所有数据集
	OwnerDataSet *FindDataSetByOwnerGuid(const string &owner);
	//根据正则表达式打印搜索的对象
	void PrintObjectByRegex(const string &regex_str, bool print_data);
	//byteArray内存池回收
	void ByteArrayPoolClear();

	//遍历所有对象,key为空时遍历所有 TODO:先放这里，测试ok挪到基类
	//委托返回非0则终止循环
	void ForeachIndexer(const string& key,std::function<bool(GuidObject*)> f);

	//本地创建的对象添加到远程，会触发Attach和AfterAttach事件
	void CallPutObjects(const string& tag,vector<GuidObject *> &objs);

public:
	bool Compress(ByteArray& bytes){return m_compress_tool.Compress(bytes);}
	bool UnCompress(ByteArray& bytes,ByteArray& out_bytes){return m_compress_tool.UnCompress(bytes,out_bytes);};
protected:
	//以binlog的owner guid为key，保存相关的所有数据
	std::map<string, OwnerDataSet*> m_all_obj_owner_idx;
	AsyncByteArrayCompress m_compress_tool;
};

#endif
