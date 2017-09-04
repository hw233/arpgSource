/************************************************************************
* file	:object_manager.h                                                                     
* desc	:该文件定义的对象容器,包括对象的创建 销毁 缓存 
*		对象更新解包,对于副本地图当中的怪物的GUID管理是不是应该有一种策略
*		对象定时更新
*
* author:	linbc
* date:	20110518
* version:
************************************************************************/
#ifndef _SCENED_OBJECT_MANAGER_H_
#define _SCENED_OBJECT_MANAGER_H_

#include <server/svr_object_manager.h>
#include <core_obj/GuidObjectTable.h>
#include <core_obj/GuidObject.h>
#include <core_obj/RemoteOperatorAdapter.h>
#include <core_obj/ByteArrayCompress.h>

class ScenedContext;
class Player;
struct lua_State;

class ScenedObjectManager : public SvrObjectManager
{
public:
	ScenedObjectManager(SvrCoreAppImpl *app);
	virtual ~ScenedObjectManager();

	void Update();
	//对象工厂
	GuidObject *ObjectFactor(const string &guid);
	//实现GuidObjectTable的创建及销毁方法，根据对象的前缀进行选择相应的对象
	GuidObject* CreateObject(string guid);
	void ReleaseObject(string guid);
	void AsyncCompress(ByteArray& bytes, CompressCallback callback){m_compress_tool.AsyncCompress(bytes, callback);}
	void AsyncSend(uint32 cid,ByteArray *pkt){m_compress_tool.AsyncSend(cid, pkt);}

	void Send(uint32_t connid,ByteArray& bytes);
	
	//打包玩家操作日志到中心服
	void SendPlayerBinlog(ScenedContext *player);
	//取得玩家unit
	Player *FindPlayer(string guid);
		
	ByteArray *GridMallocByteArray(){return &MallocByteArray();}
	void GridFreeByteArray(ByteArray *bytes){Free(*bytes);}
};

#endif

