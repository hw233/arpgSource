#ifndef _CREATURE_H_
#define _CREATURE_H_

#include <stack>
#include "MovementGenerator.h"
#include "Unit.h"
#include "ThreatManager.h"

//仇恨管理更新定时器
#define UPDATE_THREAT_TIMER 1200
class Map;
class Player;
class Unit;
class LootManager;

//enum Friendly_To_Type
//{
//	FRIENDLY_TO_PLAYER
//	FRIENDLY_TO_MONSTER
//	FRIENDLY_TO_ALL
//	FRIENDLY_TO_NONE
//};

//怪物类型
enum BossFlag
{
	BOSS_FLAG_GENERAL		= 0,		//一般怪物
	BOSS_FLAG_PINK			= 1,		//粉色怪物
	BOSS_FLAG_GREEN			= 2,		//绿色怪物
	BOSS_FLAG_VIOLET		= 3,		//紫色怪物
	BOSS_FLAG_RED			= 4,		//红色怪物
	MAX_BOSS_FLAG
};

//怪物法术概率
typedef struct spell_cast_probability_t
{
	uint32 spell_id;				//法术ID
	uint32 spell_probability;	//施法概率
	uint32 spell_distance;		//施法距离
	uint32 spell_groupCD;		//组CD
	uint32 spell_singleCD;		//CD时长
	uint32 spell_nowCDtime;		//释放技能需要等待的时间
	uint32 target_type;			//目标类型
	uint32 spell_level;			//技能等级
} spell_cast_probability;

//怪物归属列表，存玩家ID
typedef std::vector<uint32>	OwnershipVec;

typedef std::map<uint32, std::vector<spell_cast_probability*>> SpellAIMap;

//生物类,包括所有的NPC以及怪物
class Creature:public Unit
{
	friend class ThreatManager;
	typedef std::stack<MovementGenerator *> MotionImpl;
	typedef std::vector<MovementGenerator *> MotionExpirezList;

public:
	Creature();
	virtual ~Creature();

	void Update(uint32 p_time);
	void OnKilled(Unit* killer, uint16 id = 0);
	void Calcul_Attr();		
	bool Create(Map *map, const string &guidlow, uint32 tempid,uint32 respan_tm =0,uint32 movetype=0,const char *ainame = 0,uint32 level = 0, uint32 attackType = 0);
	void Initialize();
	void SetCanActiveGrid(bool status);
	
	void UpdateLiveStatus(uint32 diff);			//更新生存状态
	void UpdateSaySomething(uint32 diff);		//随便说点什么
	void UpdateOwnership(uint32 diff);			//更新怪物所有者
	void UpdateMotion(uint32 diff);				//更新怪物行为
	bool CheckCreateTimeOutDel(uint32 diff);	//检查删除需要到时删除的怪物
	
	void MotionMoveTarget();							//走到目标点	

//	void SetScriptAI(const char *scriptName){m_script_name = scriptName;}	
	void SetReactState(ReactStates st) { m_reactState = st; }	//生物的 仇恨反应类型
	ReactStates GetReactState() { return m_reactState; }
	void SetMoveType(uint32 type){m_move_type = type;}
	uint32 GetMoveType(){return m_move_type;}
	void GetBornPos(float& x,float& y){x = m_bornX;y = m_bronY;}	//出生点坐标
	void SetBornPos(float x,float y){m_bornX = x;m_bronY = y;}	

	inline creature_template& GetTemplate() const
	{
		ASSERT(GetEntry()!=0);
		ASSERT(creature_template_db[GetEntry()]);
		ASSERT(creature_template_db[GetEntry()]->id == (int)GetEntry());
		return *creature_template_db[GetEntry()];
	}
	
	bool isVendor()       const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_VENDOR ); }
	bool isTrainer()      const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_TRAINER ); }
	bool isQuestGiver()   const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_QUESTGIVER ); }
	bool isGossip()       const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_GOSSIP ); }
	bool isTaxi()         const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_FLIGHTMASTER ); }
	bool isGuildMaster()  const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_PETITIONER ); }
	bool isBattleMaster() const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_BATTLEMASTER ); }
	bool isBanker()       const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_WAREHOUSE ); }
	bool isInnkeeper()    const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_INNKEEPER ); }
	bool isSpiritHealer() const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_SPIRITHEALER ); }
	bool isSpiritGuide()  const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_SPIRITGUIDE ); }
	bool isTabardDesigner()const{ return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_TABARDDESIGNER ); }
	bool isAuctioner()    const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_AUCTIONEER ); }
	bool isArmorer()      const { return GetBit( UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_VENDOR_EQUIPMENT ); }
	bool isServiceProvider() const
	{
		return GetUInt32(UNIT_FIELD_NPC_FLAG) > 0;
		/*return (GetUInt32(UNIT_FIELD_NPC_FLAG) & 
			(UNIT_NPC_FLAG_GOSSIP | UNIT_NPC_FLAG_VENDOR | UNIT_NPC_FLAG_TRAINER | UNIT_NPC_FLAG_FLIGHTMASTER |
			UNIT_NPC_FLAG_PETITIONER | UNIT_NPC_FLAG_BATTLEMASTER | UNIT_NPC_FLAG_WAREHOUSE |
			UNIT_NPC_FLAG_INNKEEPER | UNIT_NPC_FLAG_SPIRITHEALER |
			UNIT_NPC_FLAG_SPIRITGUIDE | UNIT_NPC_FLAG_TABARDDESIGNER | UNIT_NPC_FLAG_AUCTIONEER)) != 0;*/
	}

	//是否挑战怪
	bool isChallenge() const  { return GetBit(UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_CHALLENGE); }	
	//是否光棍11,无经验，没钱，没装备，没副本杀怪数
	bool isOneOne() const {return GetBit(UNIT_FIELD_NPC_FLAG, UNIT_NPC_FLAG_ONE_ONE); }

	uint32 GetCurOwnership(){return m_cur_ownership;}				//获取当前生物所有者下标
	void SetCurOwnership(uint32 owner){m_cur_ownership = owner;}		//设置怪物所有者
	float GetFollowDis(){return 0;}

	void RandomMove(uint32 ms,float x=-1,float y=-1);	
	int MonsterMove(MovementGeneratorType mov_typ, size_t param1, time_t t);//设置移动类型

	//////////////////////////////////////////////////////////////////////////
	//AI接口	
	int AI_MoveInLineOfSight(Unit *u);				//当有Unit出现在视野
	void AI_DamageTaken(Unit *u,double& damage);		//当伤害发生时
	void AI_DamageDeal(Unit *u,double damage);		//当伤害处理后
	void AI_GiveDamage(Unit *u,double& damage);		//当产生伤害时
	void AI_KilledUnit(Unit *pVictim);				//当杀死某单位时
	void AI_JustDied(Unit *killer, uint32 &deadtime);					//当被杀死时

public:		
	MotionImpl	m_impl;
	MotionExpirezList m_expire_impl;

	ThreatManager m_threatMgr;			//仇恨管理
	OwnershipVec m_ownerships;						//归属列表
	vector<uint32> m_friends_creature;				//友军的模版ID
	
	bool HasSpell(uint32 spell_id = 0);									//是否有技能是有概率可施放的	
	int GetSpellGroup();								//技能组CD结束的组号	
	void SpellGroupWait(uint32 group_id, uint32 t = 0);	//使该技能组稍等一下再继续随机
	spell_cast_probability *RandSpell(float dis);
	void ChangeSpellCD(uint32 spellID, uint32 cd);		////改变技能CD时间

	//技能相关	
	bool IsSpellCD(uint32 spellID);				
	void SetSpellCD(uint32 spellID);

public:
	//对对象类型判断决定是否增加BUFF  
	static void AddTheOwnership(Unit *target,Unit *buff_giver);
	//TODO 怪物对某玩家因为死亡消除仇恨的同时清除这个玩家归属者			

	void SetEvadeState(bool state);					//设置逃脱状态
	bool GetEvadeState() {return evade_state;}	
	const char* GetAIName(){return m_script_name.c_str();}
	float GetAttackDistance(Unit const* ) const;

	//TODO:所有者GUID
	uint32 GetHostGUID()const{return shortGUID(GetOwner().c_str());}
	void SetHostGUID(Unit *unit);
	Unit *GetOwnerUnit();

	//任务品相关
	void DelOwnerShip(uint32 lguid);				//删除一个归属者
	void AddTheOwnership(uint32 lguid);				//V增加一个玩家到归属列表
	Player *GetOwnship();							//获取当前合法所有者
	void ResetOwnerShip();
	void GetOwnshipTable(vector<Player *> &players);						//获取当前合法所有者列表

	//经验
	uint32 GetFinallyExp(Unit *unit);				//这只怪基础经验	


	//设置为限时怪
	void SetClearTimer(time_t t);
	time_t GetClearTimer();

	uint32 GetBallisticSpellId(){return  GetUInt32(GO_FIELD_DATA);};
	uint32 GetBallisticDamage(){return  GetUInt32(GO_FIELD_DATA+1);};

	uint32 m_threat_move_type;						//生物仇恨移动类型

	uint32 GetNextSpellCD() {
		return nextSpellCD;
	}
private:	
	//for AI
	ReactStates m_reactState;                       // for AI, not charmInfo
	uint32 m_move_type;	
	string m_script_name;							//scriptai 
	bool evade_state ;								//逃脱状态
	TimeTracker m_timer_say;
	TimeTracker m_timer_threat;						//仇恨更新计时器
	TimeTrackerSmall m_update_ownership_timer;
	float m_bornX,m_bronY;							//出生坐标
	uint32 m_respawn_time;							//重生时间
	vector<uint32> m_spells_cd;					//技能CD
	SpellAIMap m_spells;

	//下个技能动作的CD
	uint32 nextSpellCD;
	
	IntervalTimer m_clear_timer;			//消失时限
	uint32 m_cur_ownership;					//生物死后爆出的战利品拥有者
	//////////////////////////////////////////////////////////////////////////
	//for motion
	IntervalTimer m_motion_timer;
	uint32 m_motion_timer_diff;
public:	
	static int LuaSetMonsterHost(lua_State *scriptL);	//设置主人
	static int LuaGetMonsterHost(lua_State *scriptL);	//获取主人
	static int LuaSetReactState(lua_State* scriptL);
	static int LuaSetScriptAI(lua_State* scriptL);

	static int LuaModifyThreat(lua_State* scriptL);	
	static int LuaMonsterTalk(lua_State* scriptL);
	static int LuaMonsterDeadTalk(lua_State* scriptL);			//怪物死亡对白
	static int LuaMonsterMove(lua_State* scriptL);
	static int LuaMonsterAddSpell(lua_State* scriptL);
	static int LuaIsAIFriends(lua_State* scriptL);				//获取AI中的Friends
	static int LuaActiveGrid(lua_State *scriptL);			//激活生物的grid
	static int LuaSetActionRadius(lua_State* scriptL);	//设置活动半径
	static int LuaSetVisionRadius(lua_State* scriptL);	//设置活动半径
	static int LuaGetOwnShipTable(lua_State* scriptL);	//获得归属者列表
	static int LuaChangeSpellCD(lua_State* scriptL);	//改变某个技能cd时间
	static int LuaGetLootOwner(lua_State* scriptL);		//当前合法所有者
	static int LuaRemoveMonster(lua_State* scriptL);	//移除怪物

};

#endif
