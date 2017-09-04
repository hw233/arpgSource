#ifndef _LOGIND_OBJECT_MANAGER_H_
#define _LOGIND_OBJECT_MANAGER_H_

#include "logind_app.h"
#include <server/svr_object_manager.h>
#include <core_obj/GuidObjectTable.h>
#include <core_obj/GuidObject.h>
#include <core_obj/RemoteOperatorAdapter.h>
#include <core_obj/ByteArrayCompress.h>

class logind_player;


//好难描述的对象啊，地图内的信息
#define MAP_INSTANCE_PLAYER_INFO_OWNER_STRING "map_player_info"

//GLOBAL_SAVE_TO_DB的保存起始下标
#define SAVE_PLAYER_GUID_STR_TO_SERVER_NAME BINLOG_STRING_FIELD_OWNER + 1
#define SAVE_PLAYER_GUID_STR_BEGIN BINLOG_STRING_FIELD_OWNER + 2
//
#define SAVE_PLAYER_GUID_INT_TODAY	0		//本次保存关心的变动日期
#define SAVE_PLAYER_GUID_INT_COUNT	1		//保存个数
#define SAVE_PLAYER_GUID_INT_INDEX	2		//保存的索引
#define SAVE_PLAYER_GUID_INT_FLAG	3		//一些标志位

enum ESavePlayerGuidFlag
{
	SAVE_PLAYER_GUID_FLAG_POST	= 0,		//是否使用Post的方式推送数据
	MAX_SAVE_PLAYER_GUID_INT_FLAG,
};
class SavePlayerGuidObj: public BinLogObject
{
public:
	SavePlayerGuidObj():BinLogObject(core_obj::SYNC_LOCAL){
		SetBinlogMaxSize(MAX_BINLOG_SIZE_UNLIME);
	}
	~SavePlayerGuidObj(){}
public:
	const string &GetToServerName(){return GetStr(SAVE_PLAYER_GUID_STR_TO_SERVER_NAME);}
	void SetToServerName(const string &server_name){SetStr(SAVE_PLAYER_GUID_STR_TO_SERVER_NAME, server_name);}

	const string &GetSavePlayerGuid(int i){return GetStr(SAVE_PLAYER_GUID_STR_BEGIN + i);}
	void SetSavePlayerGuid(uint32 i, const string &guid){SetStr(SAVE_PLAYER_GUID_STR_BEGIN + i, guid);}
	
	uint32 GetToday(){return GetUInt32(SAVE_PLAYER_GUID_INT_TODAY);}
	void SetToday(uint32 today){SetUInt32(SAVE_PLAYER_GUID_INT_TODAY, today);}

	uint32 GetCount(){return GetUInt32(SAVE_PLAYER_GUID_INT_COUNT);}
	void SetCount(uint32 count){SetUInt32(SAVE_PLAYER_GUID_INT_COUNT, count);}

	uint32 GetIndex(){return GetUInt32(SAVE_PLAYER_GUID_INT_INDEX);}
	void AddIndex(){AddUInt32(SAVE_PLAYER_GUID_INT_INDEX, 1);}
	void SetIndex(uint32 count){SetUInt32(SAVE_PLAYER_GUID_INT_INDEX, count);}

	void SetPost(){SetBit(SAVE_PLAYER_GUID_INT_FLAG, SAVE_PLAYER_GUID_FLAG_POST);}
	void UnSetPost(){UnSetBit(SAVE_PLAYER_GUID_INT_FLAG, SAVE_PLAYER_GUID_FLAG_POST);}
	bool GetPost(){return GetBit(SAVE_PLAYER_GUID_INT_FLAG, SAVE_PLAYER_GUID_FLAG_POST);}
};

class LogindObjectManager : public SvrObjectManager
{
public:
	LogindObjectManager(SvrCoreAppImpl *app);
	virtual ~LogindObjectManager();

	//对象工厂
	GuidObject *ObjectFactor(const string &guid);
	//实现GuidObjectTable的创建及销毁方法，根据对象的前缀进行选择相应的对象
	GuidObject* CreateObject(string guid);
	void ReleaseObject(string guid);

	//克隆一个binlog
	BinLogObject *Clone(BinLogObject *obj);

	//遍历所有的玩家
	void ForEachPlayer(std::function<void(logind_player*)> func);	
	//取得玩家
	logind_player* FindPlayer(const string& guid);
	
	void CollapseReset();
	//读取玩家对象
	bool LoadPlayer(const string &guid, logind_player **player, vector<GuidObject*> &vec);
	//从中心服删除该玩家所有数据
	void RemovePlayerData(const string &guid);
private:
	void Send(uint32_t connid,ByteArray&);

	ByteArrayCompress m_compress_tool;
};

#endif
