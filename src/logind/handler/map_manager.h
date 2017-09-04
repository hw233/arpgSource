#ifndef _MAP_MANAGER_H_
#define _MAP_MANAGER_H_
#include <object/BinLogObject.h>

class logind_player;
class MapTemplate;

enum EMapInstanceType
{
	MAP_INSTANCE_INT_TYPE_INSTANCE_ID		= 0,	//地图实例ID
	MAP_INSTANCE_INT_TYPE_MAP_ID			= 1,	//地图父ID
	MAP_INSTANCE_INT_TYPE_LINENO			= 2,	//线号
	MAP_INSTANCE_INT_TYPE_SCENED_CONN		= 3,	//所属场景服连接ID
	MAX_MAP_INSTANCE_INT_TYPE,

	MAP_INSTANCE_STRING_TYPE_GENERAL_ID		= 0,	//地图玩家归属？
	MAX_MAP_INSTANCE_STRING_TYPE,
};

#define MAP_INSTANCE_PLAYER_INFO_START_FIELD BINLOG_STRING_FIELD_OWNER + 1	//地图内玩家信息的起始下标

//服务器初始化时，需要实例化的基础地图信息
class InitMapInfo
{
	uint32 map_id;
	string general_id;
};

//地图管理
class MapManager : public BinLogObject
{
	//玩家副本信息,用于保存玩家退出后暂存副本信息
	struct PlayerInstanceInfo
	{
		string guid;			//玩家低位guid
		uint32 expire;			//失效时间
		uint32 instanceid;		//玩家副本ID
		int32 mapid;			//地图ID
		float  x;
		float  y;
	};
	typedef std::map<string,PlayerInstanceInfo> PlayerInstanceInfoMap;

public:
	typedef std::map<uint32, std::map<uint16, uint32>> LineInfo;
	static LineInfo lineInfo;
	
	static void lineCreated(uint32 mapid, uint16 lineNo, uint32 instanceId) {
		
		std::map<uint16, uint32> ln;
		if (lineInfo.find(mapid) == lineInfo.end()) {
			lineInfo[mapid] = ln;
		}
		lineInfo[mapid][lineNo] = instanceId;
	}

	static std::map<uint16, uint32> GetMapLines(uint32 mapid) {
		return lineInfo[mapid];
	}

	static void lineRemoved(uint32 mapid, uint16 lineNo) {
		if (lineInfo.find(mapid) == lineInfo.end()) {
			return;
		}
		lineInfo[mapid].erase(lineNo);
	}

	static bool isMapLineCreated(uint32 mapid, uint16 lineNo) {
		
		auto it = lineInfo.find(mapid);
		if (it == lineInfo.end()) {
			return false;
		}

		std::map<uint16, uint32> ln = it->second;
		if (ln.find(lineNo) == ln.end()) {
			return false;
		}

		return true;
	}

public:
	MapManager();
	~MapManager();
	//管理器心跳
	void Update(uint32 diff);
	//玩家传送心跳
	void UpdateTeleport(LogindContext *session);
	//关闭地图管理器,一般发生在没有场景服的情况
	void Close();
	//玩家登录时的逻辑
	void PlayerLogin(logind_player *player);
	//玩家登出时的逻辑
	void PlayerLogout(logind_player *player);
	//登录服重启修复
	void Restart();
	//地图包路由
	void HandleMapPkt(packet *pkt);
	//尝试关闭某一地图
	bool TryClose(uint32 instance_id);
	//查找地图实例
	int32 FindInstance(uint32 instanceid);
	int32 FindInstance(uint32 mapid, const string ganeral_id, uint32 lineo);
	int32 FindInstance(const string ganeral_id);
	//计算玩家数量
	uint32 PlayerCount(uint32 index);
protected:
	//执行传送玩家
	void TeleportTo(LogindContext *session);
	//创建地图实例
	int32 CreateInstance(uint32 mapid,const string &general_id,uint32 lineno = 0);
	//当场景服关闭时
	bool OnScenedClosed(int32 scened_id);	
	//记录玩家副本记录
	void RecordInstance(logind_player *player);
	//删除玩家副本记录信息
	void DelRecordInstance(logind_player *player);
	//玩家加入地图实例
	void AddPlayer(uint32 index, logind_player *player);
	//玩家加入地图
	bool PlayerJoin(logind_player *player);
	//从地图中删除玩家，返回地图实例所在位置
	int32 DelPlayer(logind_player *player);
public:
	inline uint32 GetInstanceID(int32 index)
	{
		ASSERT(index >= 0);
		return GetUInt32(index * MAX_MAP_INSTANCE_INT_TYPE + MAP_INSTANCE_INT_TYPE_INSTANCE_ID);
	}

	inline int GetMapID(int32 index)
	{
		ASSERT(index >= 0);
		return GetInt32(index * MAX_MAP_INSTANCE_INT_TYPE + MAP_INSTANCE_INT_TYPE_MAP_ID);
	}

	inline uint32 GetLineNo(int32 index)
	{
		ASSERT(index >= 0);
		return GetUInt32(index * MAX_MAP_INSTANCE_INT_TYPE + MAP_INSTANCE_INT_TYPE_LINENO);
	}

	inline uint32 GetScenedConn(int32 index)
	{
		ASSERT(index >= 0);
		return GetUInt32(index * MAX_MAP_INSTANCE_INT_TYPE + MAP_INSTANCE_INT_TYPE_SCENED_CONN);
	}

	inline const string &GetGeneralID(int32 index)const
	{
		ASSERT(index >= 0);
		return GetStr(BINLOG_STRING_FIELD_OWNER + 1 + index * MAX_MAP_INSTANCE_STRING_TYPE + MAP_INSTANCE_STRING_TYPE_GENERAL_ID);
	}

	inline void SetMapInstanceInfo(uint32 index, uint32 instance_id, int parent_id, uint32 lineno, uint32 conn, const string &general_id)
	{
		SetUInt32(index * MAX_MAP_INSTANCE_INT_TYPE + MAP_INSTANCE_INT_TYPE_INSTANCE_ID, instance_id);
		SetInt32(index * MAX_MAP_INSTANCE_INT_TYPE + MAP_INSTANCE_INT_TYPE_MAP_ID, parent_id);
		SetUInt32(index * MAX_MAP_INSTANCE_INT_TYPE + MAP_INSTANCE_INT_TYPE_LINENO, lineno);
		SetUInt32(index * MAX_MAP_INSTANCE_INT_TYPE + MAP_INSTANCE_INT_TYPE_SCENED_CONN, conn);
		SetStr(BINLOG_STRING_FIELD_OWNER + 1 + index * MAX_MAP_INSTANCE_STRING_TYPE + MAP_INSTANCE_STRING_TYPE_GENERAL_ID, general_id);
	}
private:
	//当前帧场景服连接数量
	uint32 m_scened_size;
	//第一次发现场景服崩溃的时候
	time_t m_scened_collapse_time;
	//第一次初始化静态地图最长等待时间定时器
	TimeTrackerSmall m_timer;
	//玩家副本信息,用于保存玩家退出后暂存副本信息,如果服务器崩溃或者重启，本数据将丢失
	PlayerInstanceInfoMap	m_playerInstInfo;
	uint8 m_teleport_callback_index;

	inline uint32 GetNewInstanceID(){return g_GuidMgr.NewIndex(ObjectTypeMapInstance);}	
	inline const char *GetInstancePlayerInfoID(uint32 instance_id){return g_GuidMgr.MAKE_NEW_GUID(ObjectTypeMapPlayerInfo, instance_id, "");}
	//玩家加入线路ID的某地图模板,没有则返回空
	int32 HandleGetInstance(logind_player *player,const MapTemplate *parent_mt,uint32 lineno = 0, uint32 mapid = 0);		
	//遍历所有
	int32 ForEach(std::function<bool(uint32 index)>);
	//通知场景服，玩家加入地图	
	void Call_Join_Map(uint32 index, logind_player *player);
	void CallDelMapInstance(uint32 index);
	//检查场景服状态，为崩溃的场景服玩家重新传送
	void UpdateScenedStatus();
	//清空某地图实例，并传送玩家到新的同地图id的实例
	void ClearInstanceAndTelePlayer(uint32 scened_id, uint32 index);
	//重新传送玩家
	void ReTelePlayer(logind_player *player, uint32 scened_id);
	//场景服全部崩光了以后，清理所有的地图信息。
	void ClearMapInstance();
public:
	static int LuaFindInstance(lua_State* scriptL);				//查找地图实例
	static int LuaCreateInstance(lua_State* scriptL);			//创建地图实例
	static int LuaMapPlayerCount(lua_State *scriptL);			//统计某地图内玩家数量
};

#endif

