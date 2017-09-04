#ifndef _ROBOTD_OBJECT_MANAGER_H_
#define _ROBOTD_OBJECT_MANAGER_H_

#include "robotd_app.h"
#include <core_obj/GuidObjectTable.h>
#include <core_obj/GuidObject.h>
#include <core_obj/RemoteOperatorAdapter.h>
#include <core_obj/ByteArrayCompress.h>
#include <server/svr_object_manager.h>

using core_obj::GuidObjectTable;
using core_obj::RemoteOperatorAdapter;
using core_obj::ByteArray;
using core_obj::GuidObject;
using core_obj::ByteArrayCompress;

class RobotdContext;
class RobotdUnit;

class RobotdObjectManager : public SvrObjectManager
{
public:
	RobotdObjectManager(SvrCoreAppImpl *app, RobotdContext *);
	virtual ~RobotdObjectManager();

	GuidObject *ObjectFactor(const string &guid);
	GuidObject *CreateObject(string guid);
	void AttachObject(GuidObject* obj);
	void AfterAttachObject(GuidObject *obj);
	void ReleaseObject(string guid);

	string GetByUGuid(uint32 guid){return GetByUGUID(guid);};
	bool Compress(ByteArray& bytes){return m_compress_tool.Compress(bytes);}
	bool UnCompress(ByteArray& bytes,ByteArray& out_bytes){return m_compress_tool.UnCompress(bytes,out_bytes);}
	
	void ClearUnit();
public:
	set<RobotdUnit*> m_all_player_unit;
	set<RobotdUnit*> m_all_unit;
private:
	void Send(uint32_t connid,ByteArray&);
	RobotdContext *m_owner;
	ByteArrayCompress m_compress_tool;
};

#endif
