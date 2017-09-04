#ifndef _POLICED_OBJECT_MANAGER_H_
#define _POLICED_OBJECT_MANAGER_H_

#include "policed_app.h"
#include <server/svr_object_manager.h>
#include <core_obj/GuidObjectTable.h>
#include <core_obj/GuidObject.h>
#include <core_obj/RemoteOperatorAdapter.h>
#include <core_obj/ByteArrayCompress.h>

class PolicedContext;

class PolicedObjectManager : public SvrObjectManager
{
public:
	PolicedObjectManager(SvrCoreAppImpl *app);
	virtual ~PolicedObjectManager();

	//对象工厂
	GuidObject *ObjectFactor(const string &guid);
	//实现GuidObjectTable的创建及销毁方法，根据对象的前缀进行选择相应的对象
	GuidObject* CreateObject(string guid);
	void ReleaseObject(string guid);

	//遍历所有的玩家
	void ForEachPlayer(std::function<void(PolicedContext*)> func);

	//取得玩家
	PolicedContext* FindPlayer(const string& guid);
	//对象更新以后触发
	void OnAfterPlayerDataUpdate(core_obj::SyncEventRecorder *,int,UpdateMask&,UpdateMask&);
public:
	PolicedContext *m_null_player;
private:
	void Send(uint32_t connid,ByteArray&);
};

#endif
