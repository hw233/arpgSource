#ifndef _SCENED_MAP_H
#define _SCENED_MAP_H

#include <shared/lua_helper.h>
#include <object/ScenedObjectManager.h>
#include <object/SharedDef.h>
#include <object/BinLogObject.h>

class MapTemplate;
class ScenedContext;
class Unit;
class Creature;
class GridManager;
class Player;
class GameObject;
struct TimerHolder;

//地图包的类型
enum EMapPktTypes
{
	MAP_PKT_TYPES_CREATURE_TELEPORT			= 0, //生物传送
};

//由于同步性问题,所以需要由心跳来检测（实例ID是否等于回调序号）来判断数据到达
typedef struct wait_joiningt
{
	uint8	teleport_sign;		//回调序号
	char	player_guid[50];
	uint32	connection_id;
	int		to_map_id;
	int		to_instance_id;
	float	to_x;
	float	to_y;
	uint32	create_tm;			//开始传送时间,便于调试
}wait_joining;

typedef UNORDERED_MAP<string,Creature*>  CreaturesMap;
typedef std::list<std::pair<int32,string> > GameObjectRespawnList;
typedef std::list<uint32> PlayerRespawnList;
typedef std::multimap<std::string,Unit*> AliasObjectMMap; 	//怪物表
typedef std::vector<Unit*>	WorldObjectVec;
typedef std::set<Creature*>	CreatureSet;
typedef UNORDERED_MAP<string,GameObject*> GameObjectMap;
typedef std::map<string, Player*> PlayerMap;
typedef std::map<uint32, std::set<string>> HitMap;
typedef vector<wait_joining> WaitJoiningVec;

class Map
{
public:
	static map<uint32, BinLogObject*> all_parent_map_info;		//父级地图变量
	static void UpdateParentInfo();
	static BinLogObject fighting_info_binlog;					//战斗信息binlog，如伤害结果，施法附加信息，buff附加信息等等
	static void BroadcastFightingInfo(Unit *unit);
	// 重置战斗binlog信息
	static void FightingInfoBinlogRest() {
		string guid = fighting_info_binlog.GetGuid();
		fighting_info_binlog.Reset();
		fighting_info_binlog.SetGuid(guid);
	}
public:	
	typedef std::multimap<uint32,Map*> MapInstances;	//所有地图系列集合
	static MapInstances map_instances;
	static WaitJoiningVec wait_joing_vec;				//等待传送进入地图

	static Map *CreateInstance(uint32 instanceid,uint32 mapid,uint32 lineno,const string &general_id);	//创建地图实例
	static void DelMap(uint32 instanceid); 						//删除地图实例
	static Map *FindInstance(uint32 instance_id,uint32 map_id);	//根据实例ID取得地图实例,如果不存在返回空
	static std::vector<Map *> FindFieldBossInstance(std::set<uint32> s_mapid);	// 根据一系列的mapid获得对应的一系列的地图实例对象
	//static Map *FindInstance(uint32 map_id, uint8 lineNo=1);	//根据地图ID取得地图实例,如果不存在返回空，副本千万不能用哦不能用哦
	static void ClearInstance();
	static void CheckMapInstanceLastTime(TimerHolder& th);		//每分钟检测副本实例是否关闭	
	static void CheckWaitJoingVec(uint32 diff);					//检测玩家传送是否可以正常收工
	static void PrintAllMap();									//打印内存中所有的地图信息
	//lua缓存
	typedef struct script_instance_var{
		uint32	type;				//值的类型 对应LUA_DATA_TYPE枚举
		union
		{
			lua_Integer	value;		//值
			lua_Number l_val;		//浮点值
			void * ptr;				//某指针
			char str[50];			//某字符串
		};		
	}script_instance_var;

	//脚本定时器
	struct script_timer_callback{
		int id;							//回调主键
		char s_callback[50];			//函数名
		script_instance_var param1[10];	//回调保留参数
		IntervalTimer timer;			//定时器
		int invalid;					//是否失效
		int is_new;						//是否新增定时器
	};
	typedef list<script_timer_callback*> ScriptCBList;

	//脚本时间戳定时器
	struct script_timestamp_timer_callback{
		int id;							//回调主键
		char s_callback[50];			//函数名
		script_instance_var param1[10];	//回调保留参数
		uint32 timestamp;				//定时器
		int invalid;					//是否失效
		int is_new;						//是否新增定时器
	};
	typedef list<script_timestamp_timer_callback*> ScriptTimeStampList;

	//成员
public:
	Map(uint32 mapid, uint32 instanceid, uint32 lineno, const string &generalid);
	virtual ~Map();

	//根据地图ID从模版原型表中载入地图
	bool Load();
	virtual void Update(uint32 diff);
	void UpdateRespan(uint32 diff);		//处理对象的生命周期

	//地图ID
	uint32 GetMapId(){return m_mapid;}
	const string &GetParentGuid(){return m_parent_map_info->guid();}
	//实例id
	uint32 GetInstanceID(){return m_parent_map_info->GetUInt32(MAP_INT_FIELD_INSTANCE_ID);}
	//线号
	uint32 GetLineNo(){return m_parent_map_info->GetUInt32(MAP_INT_FIELD_LINE_NO);}
	//识别id
	const string &GetGeneralID(){return m_parent_map_info->GetStr(MAP_STR_GENERAL_ID);}
	void SetGeneralID(const string &generalid){m_parent_map_info->SetStr(MAP_STR_GENERAL_ID, generalid);}
	//获得地图创建时间
	uint32 GetMapCreateTime(){return m_parent_map_info->GetUInt32(MAP_INT_FIELD_CREATE_TM);}
	//获得地图结束时间
	uint32 GetMapEndTime(){return m_parent_map_info->GetUInt32(MAP_INT_FIELD_END_TM);}
	void SetMapEndTime(uint32 val){	m_parent_map_info->SetUInt32(MAP_INT_FIELD_END_TM, val);}
	uint32 GetPlayerAutoRespanTime(){return 1000 * m_parent_map_info->GetByte(MAP_INT_FIELD_STATE, 2);}
	//获得场次id
	uint32 GetWarId(){return m_parent_map_info->GetUInt32(MAP_INT_FIELD_WARID);}
	//获得跨服类型
	uint32 GetKuafuType(){return m_parent_map_info->GetUInt32(MAP_INT_FIELD_KUAFU_TYPE);}
	//初始化场次id和跨服id
	void InitWaridAndKuafuType(ScenedContext* session);

	void JoinPlayer(Player *player);	//玩家加入
	void LeavePlayer(Player *player);		//玩家离开
	void LeaveCreature(Creature *creature);	//生物离开地图,仅仅是离开,内存不回收

	uint32 GetPlayerCount(){return m_players.size();}
	PlayerMap& GetPlayers(){return m_players;}

	void AddGameObject(GameObject *go);		//加入游戏对象
	void DeleteGameObject(GameObject *go);	//删除游戏对象
	void ReSpawnWorldObject(GameObject* wo);//加入重新刷新列表
	void SetMapSite(float x, float y);		//战利品网格设置为已用
	void UnSetMapSite(float x, float y);	//战利品网格设置为未用

	bool AddCreature(Creature *creature);
	Creature *AddCreature(uint32,float,float,float toward = 0.0f,uint32 respan_time = 0, uint32 movetype=0, uint32 npcflag = 0, const char *alias_name = NULL,bool active_grid = false, uint8 faction = 0,const char *ainame = NULL,uint32 level = 0, uint32 attackType = 0); 
	Creature * AddCreatureTravelers(string &data,float x,float y, uint32 movetype=0,const char *alias_name = NULL);		//别的图传过来的怪物

	void ToDelCreature(Creature *creature);

	//广播数据包,是否包括自己
	void Broadcast(packet& pkt,Unit *wo) const;
	void Broadcast(packet& pkt,Player *player,bool self = false) const;

	void BroadcastMap(packet& pkt) const;

	//在地图内寻找Unit对象 找传送点对象比较特殊 需要用到玩家对象
	GameObject *FindWorldObject(uint32 guid);
	Unit *FindUnit(uint32 guid);
	Player *FindPlayer(uint32 guid);
	Player *FindPlayer(const char* guid);
	Creature* FindCreature(uint32 guid);

	bool IsCanRun(float x,float y);	
	bool IsRightCoord(float x,float y);
	bool IsRightCoordNoCanRun(float x,float y);

	//创建生物和Gameobject的低位GUI
	static const string CreateNewCreatureID();

	//地图模板
	const MapTemplate *GetMapTemp(){return m_template;}

	//增加自动复活玩家
	void AddRespanPlayer(Player* player);

	//是否空位，可以放置战利品
	bool IsEnpty(uint32 x,uint32 y)
	{
		if(x >= m_loot_site_width || y >= m_loot_site_hight)
			return false;		
		return m_all_site.GetBit(x + m_loot_site_width * y) == 0;
	}

	//是否空位，可以放置战利品
	bool IsEnpty(uint32 p)
	{
		if(p >= m_loot_site_width * m_loot_site_hight)
			return false;		
		return m_all_site.GetBit(p) == 0;
	}

	bool HaveEnpty()
	{
		//保险起见，留10个空格，以免死循环		
		return m_all_enpty_count > 10;
	}
	//地图是否可跳跃
	bool isCanJumpMap(){return m_is_can_jump;}
	void SetMapIsCanJump(bool is_can ){m_is_can_jump = is_can;}
	
	void SendCreateBlock(ScenedContext *player);
	void SendCreateBlock(uint32 fd);
	void SendDeleteBlock(ScenedContext *player);
	void SendDeleteBlock(uint32 fd);
	
	void SendGridUnitFromPlayer(Player* player);	//热更时下发unit给客户端用

	void SendMapPkt(uint32 mapid, const string &general_id, uint8 typ, std::function<void(ByteArray&)>);
	void HandleMapPkt(ByteArray &bytes);
	//将怪物传送至另一个地图
	void TeleportCreature(const string &guid, uint32 mapid, float x,float y, const string &general_id);
	void OnTeleportCreature(ByteArray &bytes);
	GridManager *GetGridManager(){return m_grids;}	
	void GetAllGameObjectByEntry(vector<GameObject *> &vect, uint32 entry = 0);		//获取所有游戏对象

public:
	/** <playerGuid, damage> */
	typedef std::map<string, double> PlayerDamage;
	PlayerDamage m_fieldBossDamage;
	static int LuaClearFieldBossDamage(lua_State *scriptL);			//清理玩家伤害
	static int LuaAddFiledBossDamage(lua_State *scriptL);			//增加玩家伤害
	static int LuaGetMaxinumFieldBossDamage(lua_State *scriptL);	//获得最大伤害的玩家playerguid

	string openGuid;	// 正在开启宝箱的guid
	static int LuaOnOpenTreasure(lua_State *scriptL);	//设置正在开启宝箱的guid
	static int LuaGetOnOpenGuid(lua_State *scriptL);	//获得正在开启宝箱的guid

public:
	GameObjectMap				m_gameobjects;	
	GameObjectRespawnList		m_go_dynamic;		//动态游戏对象,到一定的时候需要消失
	PlayerRespawnList			m_player_respawn;	//玩家的自动复活	
	string m_state_script;							//地图实例脚本编号
	UpdateMask m_all_site;							//标识地图上所有坐标的使用状态

	PlayerMap m_players;		//玩家列表
	CreaturesMap m_alive_creatures;		//活着的怪物列表

	HitMap creatureHitHash;	//攻击列表

	bool m_is_can_jump;			//是否能跳跃

	bool m_can_recovry;			//是否支持恢复（生命源泉等）
	bool m_can_castspell;		//是否允许施法(暂时主要用于限制跨服准备时间内不允许打架)
	uint32 m_mapAttrBonus[MAX_EQUIP_ATTR];//地图属性加成
	
	GameObject *GetGameObjectByGuid(uint32 target);

public:
	void ClearCreatureHitHash(uint32 uintguid);
	void PlayerHitCreature(uint32 creatureUintGuid, string playerGuid);	//某个人攻击了野怪

protected:
	void AddAliasCreature(Unit *unit);
	void DelAliasCreature(Unit *unit);
protected:
	uint32 m_mapid;						//地图模板ID
	BinLogObject *m_parent_map_info;	//父地图信息

	const MapTemplate *m_template;	//地图模版
	GridManager *m_grids;		//网格管理者
		
	CreatureSet	m_to_del_creature;		//需要从地图删除的生物(好累啊)
	
	int			 m_script_callback_key;	
	ScriptCBList m_script_callbacks;	//脚本回调
	ScriptTimeStampList m_script_timestamp_callbacks;	//时间戳定时器回调

	std::multimap<int, script_instance_var>  m_luaVar;//lua脚本关联变量

	uint32 m_loot_area;				//战利品占地面积的边长
	uint32 m_loot_site_width;			//根据战利品占地边长缩小后的地图宽度
	uint32 m_loot_site_hight;			//根据战利品占地边长缩小后的地图高度
	uint32 m_all_enpty_count;			//所有空位数量

	AliasObjectMMap m_aliasObjs; 	//根据别名查找对象
	WorldObjectVec m_worldObject_toadd;	//待加入的
	Map* m_parent;					//父级地图	

	ByteArray m_byte_buf;

	uint32 m_end_time;
public:
	bool m_broadcast_nogrid;		//无格子式，全图广播
	bool m_is_close_respawn;		//是否关闭复活
	//////////////////////////////////////////////////////////////////////////
	//static for lua script
	static int LuaGetMapId(lua_State *scriptL);					//获取地图模板ID
	
	static int LuaSetMapAttrBonus(lua_State *scriptL);			//设置地图加成
	static int LuaSetCanRecovey(lua_State *scriptL);			//设置地图是否支持回血
	static int LuaGetCanRecovey(lua_State *scriptL);			//获取地图是否支持回血
	static int LuaSetCanCastSpell(lua_State *scriptL);			//设置是否可以施法
	static int LuaGetCanCastSpell(lua_State *scriptL);			//获得是否可以施法

	static int LuaFindSatisfiedFieldBossInstance(lua_State *scriptL);	//获得获得副本实例

	static int LuaAddCreature(lua_State *scriptL);
	
	static int LuaAddGameObject(lua_State *scriptL);
	static int LuaConsLootPoint(lua_State *scriptL);			//计算战利品点
	static int LuaConsLootPointByPos(lua_State *scriptL);		//通过坐标点获取掉落坐标
	static int LuaRandConsLootPoint(lua_State *scriptL);		//获取全图随机掉落点
	static int LuaRemoveObject(lua_State *scriptL);
	static int LuaQueryCreature(lua_State *scriptL);		//根据模板ID找到相应生物
	static int LuaGetCreatureEntryCount(lua_State *scriptL);//获得怪物不同entry的数量
	static int LuaAliasCreature(lua_State *scriptL); 		//根据生物的别名查找对象
	static int LuaAliasCreatureTable(lua_State *scriptL);	//根据生物的别名查找对象,返回一个表
	
	static int LuaGetCircleTargets(lua_State *scriptL);		//取圆形区域目标 center_x,center_y,radius
	static int LuaGetCircleTargetsByPos(lua_State *scriptL);		//取圆形区域目标2
	static int LuaGetRectTargets(lua_State *scriptL);		//取矩形区域目标 leftdown_x,leftdown_y,rightup_x,rightup_y
	static int LuaGetFanshapedTargets(lua_State *scriptL);	//取扇形区域目标 center_x,center_y,radius,radian,orientation
	static int LuaGetObliqueRectTargets(lua_State *scriptL);	//获取斜矩形的目标
	static float __GetAngle(const float x1, const float y1, const float x2, const float y2);
	static int __LuaAddUnit(lua_State *scriptL, vector<Unit *>&units);
	static int __LuaAddUnit2(lua_State *scriptL, vector<Unit *>&units);
	static int	__LuaAddPlayer(lua_State *scriptL, vector<Player *>&players);
	static void GetCircleTargets(float center_x, float center_y, float radius, Unit* unit, vector<Unit *> &units, Spell_Target_Type = TARGET_TYPE_ALL, bool dis_sort = false);
	static void GetCircleTargetsByPos(Map *map,float center_x, float center_y, float radius,vector<Unit *> &units, Spell_Target_Type = TARGET_TYPE_ALL);
	static void GetRectTargets(Unit* unit,float start_x, float start_y, float end_x, float end_y, vector<Unit *> &units, Spell_Target_Type TargetType); 	//获取矩形目标
	static int LuaGetPlayersCounts(lua_State *scriptL);	//获取地图人数
	static int LuaAddTimer(lua_State *scriptL);
	static int LuaAddTimeStampTimer(lua_State *scriptL);
	static int LuaDelTimer(lua_State *scriptL);
	static int LuaDelTimeStampTimer(lua_State *scriptL);
	static int LuaRandomPos(lua_State *scriptL);			//在地图中随机
	static int LuaGuidToPtr(lua_State *scriptL);
	static int LuaGetAllPlayer(lua_State *scriptL);	//获取所有玩家
	static int LuaGetAllCreature(lua_State *scriptL);	//获取所有怪物
	static int LuaGetGameOject(lua_State *scriptL);		//获得指定模板的游戏对象
	static int LuaGetAllGameObject(lua_State *scriptL);	//获取所有游戏对象
	static int LuaIsCanRun(lua_State *scriptL);				//判断坐标是否合法
	static int LuaIsInMap(lua_State *scriptL);				//是不是在地图之内
	static int LuaSetMapSite(lua_State* scriptL);		// 设置地图坐标点是否可以掉落
	static int LuaGetRandPosByRadius(lua_State *scriptL);		////获取点半径随机可到坐标

	static int LuaGetLootObject(lua_State *scriptL);		//根据坐标获取对应的战利品指针
	static int LuaInsertLootIndex(lua_State *scriptL);		//将新增的战利品位置插入到列表
	static int LuaClearAllLoot(lua_State *scriptL);			//清空地图上所有战力品
	static int LuaCreateMapDataHddDirectory(lua_State *scriptL);//创建地图硬盘数据保存文件夹
	static int LuaIsHaveFile(lua_State *scriptL);			//判断某文件是否存在
	static int LuaGetParentPtr(lua_State *scriptL);			//获取父级地图信息
	static int LuaParentGetAllPlayer(lua_State *scriptL);	//获取系列地图内所有玩家信息
	static int LuaBroadcast(lua_State *scriptL);	//grid广播包
	static int LuaBroadcastMap(lua_State *scriptL);	//MAP广播包

	static int LuaGetGameObjectByGuid(lua_State *scriptL);		//通过uintguid获取游戏对象
	static int LuaGetGameObjectByEntry(lua_State *scriptL);	//通过entry获得游戏对象
	static int LuaRemoveGameObjectByEntry(lua_State *scriptL);	//通过entry删除游戏对象

	static int LuaGetPlayerByPlayerGuid(lua_State *scriptL);

	static int LuaIsMapLineCreated(lua_State *scriptL);

	static int LuaGetPlayersAfterCreatureDead(lua_State *scriptL);	//野怪死了, 找曾经攻击过的人
};


class InstanceMap :public Map
{
public:
	InstanceMap(uint32 mapid, uint32 instanceid, uint32 lineno, const string &generalid);
	virtual	~InstanceMap();
public:
	void InheritOldMap(InstanceMap *old_map);				//继承一部分数据
	void ExitInstance(Player *player = NULL);				//离开副本,如果没有给定玩家则退出全部
	
private:
	uint8 m_score;											//副本评级
	void OnePlayerExitInstance(Player *player);				//某个玩家离开副本

public:
	static int LuaGetPlayer(lua_State *scriptL);			//获得副本玩家。暂时为单人
	static int LuaExitInstance(lua_State *scriptL);			//离开副本
};
#endif

