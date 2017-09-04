#ifndef __UNIT_H__
#define __UNIT_H__

#include <object/std.h>
#include <comm/hash_list.h>
#include <protocol/external.h>
#include <object/SharedDef.h>
#include <object/ScenedObjectManager.h>

class Map;
class BufferManager;
struct Grid;

extern vector<uint16> g_cant_attack_buff;
extern vector<uint16> g_cant_cast_buff;
extern vector<uint16> g_cant_move_buff;
extern vector<uint16> g_cant_jump_buff;
extern vector<uint16> g_cant_teleport_buff;
extern vector<uint16> g_cant_use_item_buff;
extern vector<uint16> g_cant_see_buff;
extern vector<uint16> g_invincible_buff;
extern vector<uint16> g_cant_fight_with_player;
extern vector<uint16> g_cant_attack_be_player;
extern vector<uint16> g_cant_recovery_buff;
extern vector<uint16> g_ignore_the_obstacles;


//由于弹道技能需要一个过程，所以，增加一个触发器用于延迟产生伤害
//技能分为,施法、触发、伤害
//技能触发器
typedef struct spell_trigger_t
{
	//uint32 self_guid;		//发技能的Unit GUID
	uint32 target_guid;	//目标的Guid
	int spell_id;			//技能ID
	int spell_lv;			//技能等级
	float dst_x;			//目标点坐标x
	float dst_y;			//目标点坐标y
	IntervalTimer timer;	//定时触发时间（ms）
	int count;				//触发次数
	char data[500];			//预留参数
}spell_trigger;

typedef std::map<uint16,uint8> PassiveSpellLevelMap;
typedef std::map<uint32,uint16> PassiveSpellBinLogIndexMap;

//////////////////////////////////////////////////////////////////////////
class Unit:public BinLogObject
{		
public:
	static const uint32_t OBJECT_MODE = core_obj::SYNC_MASTER | core_obj::SYNC_UPDATEDATA;		
	typedef std::set<uint32> UnitSet;
	typedef std::unordered_map<size_t,int> FriendlyMap;
	static Unit* FindUnit(uint32_t guid);	
public:
	Unit();
	virtual ~Unit();
protected:
	void _Create (const string& guid, int entry);
public:
	void WriteCreateBlock(ByteArray &,UpdateMask *int_mask = nullptr,UpdateMask *str_mask = nullptr);
	bool WriteMovePath(ByteArray &);
public:
	void SetStatFloat( uint16 index, float value);
	void SetStatInt32( uint16 index, int32 value);

	virtual void Update( uint32 time );		
	virtual void Calcul_Attr();						//计算属性
	void SetCalculAttrStage(bool need)
	{
		m_recalcul_Attr = need;
	} //设置属性重算
		
	virtual void UpdateLiveStatus(uint32 diff) = 0;	//更新生存状态
	bool UpdateLocate(uint32 diff);					//更新位置	
	void UpdateSpellTrigger(uint32 diff);				//技能触发器更新

	virtual void OnKilled(Unit* killer = NULL, uint16 id = 0);			//被杀死
	void Respawn(float x=0.0f,float y=0.0f);		//重生

	const string &GetAliasName(){return GetStr(UNIT_STRING_FIELD_ALIAS_NAME);};
	void SetAliasName(const char*val){SetStr(UNIT_STRING_FIELD_ALIAS_NAME, val);}

	//属性接口
	int GetEntry() const { return GetUInt16(UNIT_FIELD_UINT16_0, 0); }
	void SetEntry(uint16 entry) { SetUInt16(UNIT_FIELD_UINT16_0, 0, entry); }

	uint8 GetTypeId() const { return GetByte(UNIT_FIELD_BYTE_0, 0); }
	void SetTypeId(uint8 type) {SetByte(UNIT_FIELD_BYTE_0, 0, type);}
	uint32 GetLevel() const {return GetUInt16(UNIT_FIELD_UINT16_0, 1);}
	void  SetLevel(uint16 level){ SetUInt16(UNIT_FIELD_UINT16_0, 1,level);}
	
	//阵营
	//void  SetRace(uint8 fa){SetByte(UNIT_FIELD_BYTES_0,2,fa);}
	uint8 GetGender() const { return GetByte(UNIT_FIELD_BYTE_1, 0); }
	uint8 GetFaction() const {return GetByte(UNIT_FIELD_BYTE_1, 1);}	
	void  SetFaction(uint8 fa){SetByte(UNIT_FIELD_BYTE_1,1,fa);}
	uint8 GetRace() const{return GetByte(UNIT_FIELD_BYTE_1,2);}
	uint8 GetProfession() const{return GetByte(UNIT_FIELD_BYTE_1,3);}
	uint8 GetCreatureSkin() const{return GetByte(UNIT_FIELD_BYTE_4, 2);}

	double GetCurrCastVampiric() {
		return GetDouble(UINT_FIELD_VAMPIRIC);
	}

	void ClearCurrCastVampiric() {
		SetDouble(UINT_FIELD_VAMPIRIC, 0);
	}

	uint32 GetRecovery() {
		return GetUInt32(UNIT_FIELD_RECOVERY);
	}

	uint32 GetRecoveryRate() {
		return GetUInt32(UNIT_FIELD_RECOVERY_RATE);
	}
	////基本属性
	//uint32 GetCombatRate(Combat_Rate rage);
	//void SetCombatRate(Combat_Rate rage,uint32 val);

	/*uint32 GetOutArmor() { return GetUInt32(UNIT_FIELD_OUT_RESISTANCES); }
	uint32 GetInArmor() { return GetUInt32(UNIT_FIELD_IN_RESISTANCES); }
	void SetOutArmor(uint32 val) { SetUInt32(UNIT_FIELD_OUT_RESISTANCES, val); }
	void SetInArmor(uint32 val) { SetUInt32(UNIT_FIELD_IN_RESISTANCES, val); }*/

	uint32 GetHealth()    { return GetUInt32(UNIT_FIELD_HEALTH); }
	uint32 GetMaxHealth() { return GetUInt32(UNIT_FIELD_MAX_HEALTH); }
	void SetHealth(uint32 val, bool isBroadcast = true);
	void SetMaxHealth(uint32 val);
	double ModifyHealth(double val, bool isBroadcast = true);
	//广播生命变化
	void BroadcastHp(uint32 hp);

	int GetSP() {return this->GetUInt32(UNIT_FIELD_ANGER);}
	void SetSP(uint32 value) {this->SetUInt32(UNIT_FIELD_ANGER, value);}

	/*uint32 GetPower(Powers power) 
	{ 
	uint32 ret = 0;
	switch (power)
	{
	case POWER_MANA:			
	break;
	case POWER_ENERGY:
	ret = GetUInt32(UNIT_FIELD_POWERS_ENERGY);
	break;		
	default:
	break;
	}
	return ret;
	}*/
	//void SetPower(Powers power, uint32 val);
	//uint32 GetMaxPower(Powers power);
	//void SetMaxPower(Powers power, uint32 val);
	//
	//double ModifyPower(Powers power, double val);
	//void ApplyPowerMod(Powers power, uint32 val, bool apply);
	//void ApplyMaxPowerMod(Powers power, uint32 val, bool apply);

	/*uint32 GetOutDamage(){return GetUInt32(UNIT_FIELD_OUT_DAMAGE);}
	void SetOutDamage(uint32 val){ SetUInt32(UNIT_FIELD_OUT_DAMAGE,val);}
	uint32 GetInDamage(){return GetUInt32(UNIT_FIELD_IN_DAMAGE);}
	void SetInDamage(uint32 val){ SetUInt32(UNIT_FIELD_IN_DAMAGE,val);}*/

	uint32 GetMoveSpeed() {return GetUInt32(UNIT_FIELD_MOVE_SPEED);}
	void SetMoveSpeed(uint32 speed);

	void SetDeathState(uint8 state){
		SetByte(UNIT_FIELD_BYTE_0, 1,state);
	}

	uint8 GetDeathState(){return GetByte(UNIT_FIELD_BYTE_0, 1);}

	//-----------------------------------------------------------------------------------------------
	/*移动相关*/		
	void StopMoving(bool need_send=false);	
	void StopMoving(uint16 x,uint16 y);	
	//void MoveTurnAround(float ori,bool toself=false);	//转向
	void Jump(float x,float y,uint32 time);				//跳
	
	virtual void RandomMove(uint32 ms, float x=-1, float y=-1);	
	void MoveTo(float to_x,float to_y)
	{
		float points[] = {to_x,to_y};
		MoveTo(2,points,true);
	}	
    void MoveTo(uint16 size,float *path,bool synchronous = false);	//移动路径	
	bool IsMoving(){ return m_moving_ms != 0;}						//如果毫秒数不等于0说明是移动状态

	//转向目标
	void TurnToTarget(Unit *target);
	//最后一次跳跃时间
	uint32 GetLastJump(){return m_last_jump;}
	void SetLastJump(uint32 time){m_last_jump = time;}

	//目标操作
	uint32 GetTargetGuid() const {return m_target_guid;}
	void  SetTarget(Unit* target);	
	Unit* GetTarget();

	//最后被我攻击的生物
	void SetLastVictim(uint32 target_id) 
	{
		m_last_victim_target = target_id;
	}
	uint32 GetLastVictim()const{return m_last_victim_target;}
	//最后攻击我的生物

	//--------------------------------------------------------------------------------------------------------
	//行动判定
	bool isCanMove();			//能否移动
	bool isCanTeleport();		//能否传送
	bool isCanUseItem();		//是否能使用物品
	bool isInvincible();		//是否无敌
	bool isCanSee();			//是否可见
	bool isCantAttackByPlayer()const;//是否可被玩家攻击
	bool isCanRecovery();		//能否恢复

	bool isIgnoreTheObstacles();//无视障碍
	bool isCanJump();			//能否可以跳跃

	//--------------------------------------------------------------------------------------------------------
	//战斗相关 物理+技能

	//重置状态,将所有仇恨状态全部清空重置
	void ClearAttacker(bool destructor = false);
	//添加攻击者
	void AddAttacker(Unit *unit);
	//删除攻击者
	void DelAttacker(uint32 unit, bool isDelOwnerShip = false);
	//取得攻击者列表
	const UnitSet &GetPlayerAttackSet() const
	{
		return m_attacker_player;
	}
	//取得攻击者列表
	const UnitSet &GetUnitAttackSet() const
	{
		return m_attacker_unit;
	}
	//玩家被怪物攻击时相关处理
	void DoPVEEvent(Player* target);
	int Acting(Unit* act_target);			//开始表演
	int StopActing();						//停止表演
	BOOL isActing(){return m_isActing;}		//是否在表演状态

	//状态管理
	bool IsFriendlyTo(Unit const* unit);									//是否是友好的目标	
	void InvalidFriendlyCache(Unit *unit = nullptr);						//友好缓存失效,以我为目标的要重建,我的目标也要重建
	void InvalidFriendlyCache(Grid *grid);									//友好缓存失效,所有的格子里面的对象
	bool hasUnitState(const uint32 f) const { return ((m_state & f) !=0); }	//是否有标识,暂时只用于战斗
	void addUnitState(uint32 f) { m_state |= f; }							//增加标识战斗,暂时只用于战斗
	void clearUnitState(uint32 f) { m_state &= ~f; }						//删除标识战斗,暂时只用于战斗
	//攻击距离
	virtual float GetAttackDistance (Unit const* )const;

	bool isAlive() const { return (GetByte(UNIT_FIELD_BYTE_0, 1) == DEATH_STATE_ALIVE); }
	bool isCasing()const {return hasUnitState(UNIT_STAT_CAST_SPELL);}
	bool isSpellProcess()const{return hasUnitState(UNIT_STAT_SPELL_PROCESS);}

	//技能相关
	void CastSpell(uint32 spell_id,float x,float y);				    //开始施法
	void SpellStop(bool clear_auto_spell = true,bool to_self = true);						//施法停止
	void CalSpellCastInfo();			    //计算技能伤害
	void AfterCalSpellCastInfo();			//伤害结果以后

	//当前技能管理
	uint32 GetCurSpell() const{return m_cur_spell;}						    //获取玩家当前技能
	virtual void SetCurSpell(uint32 id);
	uint32 GetCurSpellLevel() const{return m_cur_spell_lv;}
	void SetCurSpellLevel(uint32 level) {m_cur_spell_lv = level;}

	virtual bool HasSpell(uint32 ) {return true;};
	virtual bool IsSpellCD(uint32){return false;};				//技能是否在CD			
	virtual void SetSpellCD(uint32 id) = 0;				//技能进入CD

	//技能目标区域管理
	void SetTargetPos(float x,float y){m_target_x = x; m_target_y = y;}	
	void GetTargetPos(float &x,float &y){x = m_target_x; y = m_target_y;}
	float GetTargetX(){return m_target_x;}
	float GetTargetY(){return m_target_y;}

	BufferManager *GetBuffManager(){return m_buff_manager;}

	bool GetIsBlowFly()
	{
		if(m_isBlowFly)
		{
			m_isBlowFly = false;
			return true;
		}
		return false;
	}

	//被击飞
	void BlowFly(Unit* attacker,uint32 space, uint16 &destX, uint16 &destY);

	//----------------------------------------------------------------------//
	//坐标朝向操作接口

	//变换对象位置和朝向
	void Relocate(float x, float y, float orientation)
	{
		SetPosition(x,y);
		m_orientation = orientation;
	}

	float GetPositionX( ) const { return m_positionX; }			//获取X坐标
	float GetPositionY( ) const { return m_positionY; }			//获取Y坐标

	void GetPosition( float &x, float &y ) const				//获取XY坐标
	{ x = m_positionX; y = m_positionY;}

	void SetPosition( float x, float y) 
	{ 
		ASSERT(x == x);
		ASSERT(y == y);
		m_positionX = x; 
		m_positionY = y;
	}

	//变换对象朝向
	void SetOrientation(float orientation) { m_orientation = orientation; }
	float GetOrientation( ) const { return m_orientation; }		//获取朝向

	uint16 GetMapId()const {return GetUInt16(UNIT_FIELD_UINT16_1, 0);}
	void SetMapId(uint16 id)
	{
		SetUInt16(UNIT_FIELD_UINT16_1, 0, id);
	}

	uint32 GetInstanceId() const{return GetUInt32(UNIT_FIELD_INSTANCE_ID);}	
	void SetInstanceId(uint32 id){SetUInt32(UNIT_FIELD_INSTANCE_ID, id);}

	//得到朝向角度
	float GetAngle( const Unit* obj ) const;
	float GetAngle( const float x, const float y ) const;

	//得到距离
	float GetDistance(const Unit *obj) const;
	float GetDistance(const float x,const float y) const;

	//添加技能触发器
	void AddSpellTrigger(spell_trigger str);

	Grid *GetGrid(){return m_grid;}
	void SetGrid(Grid *_g){m_grid = _g;}

	Map *GetMap()const{ return m_map;};
	void SetMap(Map* _m){m_map = _m;}

	//是否可以导致Grid处于活动状态
	bool CanActiveGrid() {return m_canActiveGrid;}
	void SetCanActiveGrid(bool status) { m_canActiveGrid =  status;}

	//判断是否可以施法
	bool IsCantCast();

	uint32 GetUIntGuid() const{return m_uint_guid;}
	// 获得精灵下标的flag标志
	bool GetUnitFlags(uint32 index){return GetBit(UNIT_FIELD_FLAGS, index);}
	// 设置精灵下标的flag标志
	void SetUnitFlags(uint32 index){SetBit(UNIT_FIELD_FLAGS, index);}
	// 设置精灵下标的flag标志
	void UnSetUnitFlags(uint32 index){UnSetBit(UNIT_FIELD_FLAGS, index);}

	hashlist_node m_hash_node;	//用于哈希表
protected:	
	uint32 m_uint_guid;			//整形guid
	float m_positionX;			//x坐标				
	float m_positionY;			//y坐标
	float m_orientation;		//朝向
	bool m_canActiveGrid;		//能激活Grid
	Grid *m_grid;
	Map *m_map;

	uint32 m_moving_ms;					//移动完当前点需要消耗,等于0时处于静止状态	
	std::deque<float> m_moving_path;	//移动路径
	uint32 m_moving_last_diff;			//累计时间差,用于减小精度误差

	uint32 m_last_victim_target;		//最后被我攻击的生物
	UnitSet m_attacker_unit;					//生物攻击者
	UnitSet m_attacker_player;					//玩家攻击者
	Unit*  m_attacking;	
	uint32 m_state;						//战斗状态	
	uint32 m_cur_spell;					//当前魔法
	uint32 m_cur_spell_lv;				//当前魔法等级
	uint32 m_auto_spell;				//自动施法技能
	float m_target_x;					//魔法释放区域
	float m_target_y;
	uint32 m_last_jump;					//上次跳跃时间

	BufferManager *m_buff_manager;		//BUFF管理器

	TimeTracker			m_live_timer;	//生存状态计时器
	CycleTimer	m_spell_process;		//施法过程定时器

	BOOL m_isActing;					//是否在表演状态
	bool m_recalcul_Attr;				//是否需要重算属性
	//bool m_is_del_threat;				//是否需清楚仇恨
	
	bool m_isBlowFly;					//是否被击飞

	uint32 m_update_tick_num;

	uint32 m_target_guid;				//选中目标的guid
	bool m_need_sync_pos;				//是否需要同步坐标到主玩家binlog

	vector<spell_trigger> m_spell_trigger;
	FriendlyMap			m_friendly_cache;//可以攻击的目标

	PassiveSpellBinLogIndexMap m_index_spellId;	//binlogIndex对应的被动技能

	PassiveSpellLevelMap m_passive_spell_level;	//被动技能和等级

public:
	typedef std::map<uint32, point_t> HitMeUnitPositionInfo;
	#define RANGE 1
	HitMeUnitPositionInfo positionInfo;

	void removePositionInfo(uint32 cguid) {
		if (positionInfo.find(cguid) != positionInfo.end()) {
			positionInfo.erase(cguid);
		}
	}

	bool getNearestPositionByAngleAndAttackRange(float angle, float attackRange, float tarx, float tary, float& posx, float& posy) {
		
		angle += M_PI;
		uint32 ar = (uint32)attackRange * 2;
		// 外层循环
		for (uint32 i = 0; i <= 180; i += 30) {

			float offsetAngle;

			offsetAngle = float(angle + i * M_PI / 180.0);
			for (uint32 range = ar; range >= 3; -- range) {
				const float distanceX = float(range * cos(offsetAngle) / 2.0);
				const float distanceY = float(range * sin(offsetAngle) / 2.0);
				posx = tarx + distanceX;
				posy = tary + distanceY;

				if (!isExist(posx, posy)) {
					return true;
				}
			}

			offsetAngle = float(angle - i * M_PI / 180.0);
			for (uint32 range = ar; range >= 2; -- range) {
				const float distanceX = float(range * cos(offsetAngle) / 2.0);
				const float distanceY = float(range * sin(offsetAngle) / 2.0);
				posx = tarx + distanceX;
				posy = tary + distanceY;

				if (!isExist(posx, posy)) {
					return true;
				}
			}
		}

		return false;
	}

	bool isExist(float x, float y) {

		for (auto it : positionInfo) {
			if (abs(it.second.pos_x - x) <= RANGE && abs(it.second.pos_y - y) <= RANGE) {
				return true;
			}
		}

		return false;
	}

	/*int getNearestAngleIndexWithoutAngles(float angle) {

	std::map<uint8, uint8> vist;

	for (auto it : positionInfo) {
	vist[it.second] = 1;
	}

	if (vist.size() == PIECES) {
	vist.clear();
	}

	float angleInterval = float(M_PI * 2.0 / PIECES);

	int index = -1;
	float diff = 360;
	float curr = -angleInterval;
	for (int i = 0; i < PIECES; ++ i) {
	curr += angleInterval;
	if (vist.find(i) != vist.end())
	continue;

	float absAngle = fabs(curr - angle);
	if (diff > absAngle) {
	diff = absAngle;
	index = i;
	}
	}

	return index;
	}*/

	//////////////////////////////////////////////////////////////////////////
	//for lua script
public:	
	static int LuaGetTarget(lua_State* scriptL);	//取生物目标
	static int LuaSetTarget(lua_State* scriptL);	//设置生物目标
	static int LuaCastSpell(lua_State* scriptL);
	static int LuaSetCalculAttr(lua_State* scriptL);//计算属性

	static int LuaGetPos(lua_State* scriptL);
	static int LuaSetPos(lua_State* scriptL);
	
	static int LuaKillUnit(lua_State* scriptL);		//秒杀生物

	static int LuaIsFriendlyTo(lua_State* scriptL);
	static int LuaInvalidFriendlyCache(lua_State* scriptL);	//友好度缓存失效
	static int LuaIsCanHit(lua_State* scriptL);
	
	static int LuaIsActing(lua_State* scriptL);

	static int LuaIsInvincible(lua_State* scriptL);
	static int LuaDelThreat(lua_State* scriptL);
	static int LuaSetActing(lua_State* scriptL);
	static int LuaGetMap(lua_State* scriptL);
	static int LuaGetMapID(lua_State* scriptL);

	static int LuaBlowFly(lua_State* scriptL);
	static int LuaRespawn(lua_State* scriptL);
	static int LuaRandomMove(lua_State* scriptL);
	static int LuaSpellStop(lua_State* scriptL);				//停止施法
	static int LuaGetCurSpell(lua_State* scriptL);
	static int LuaSetCurSpell(lua_State* scriptL);
	static int LuaHasUnitState(lua_State* scriptL);
	static int LuaIsMoving(lua_State* scriptL);
	static int LuaGetMoveSpeed(lua_State* scriptL);
	static int LuaSetMoveSpeed(lua_State* scriptL);
	static int LuaGetOrientation(lua_State* scriptL);	//返回生物移动的方向(即角度)
	static int LuaSetOrientation(lua_State* scriptL);	//设置生物移动的方向(即角度)
	static int LuaStopMoving(lua_State* scriptL);		//生物停止移动	
	static int LuaMoveTo(lua_State* scriptL);			//生物移动到
	static int LuaGetIntGuid(lua_State* scriptL);		//获得精灵的int guid

	static int LuaGetMoveEndPos(lua_State *scriptL);	//获取生物移动终点坐标
	static int LuaIsCanJump(lua_State* scriptL);		//生物是否可以跳跃

	static int LuaAddSpellTrigger(lua_State* scriptL);	//增加一个技能触发器
	static int LuaDelSpellTrigger(lua_State* scriptL);	//删除一个技能触发器
	static int LuaSetHealth(lua_State* scriptL);	//设置血量
	static int LuaModifyHealth(lua_State* scriptL);	//改变血量

	static int LuaCallJumpStart(lua_State* scriptL);	//广播开始跳跃

	static int LuaSetOwner(lua_State* scriptL);		//设置拥有者，方法调用时需要上报
};

#endif

