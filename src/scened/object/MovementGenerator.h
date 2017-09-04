#ifndef _MOVEMENT_GENERATOR_H_
#define _MOVEMENT_GENERATOR_H_

#include <comm/timer.h>
#include <shared/FactoryHolder.h>
#include "Unit.h"
#include "GameObject.h"

//#define TRAVELLER_UPDATE_INTERVAL  100

class Creature;

enum WayfindingType
{
	WAYFINDINGTYPE_NONE = 0,								//无寻路方式
	WAYFINDINGTYPE_WAYPOINT = 1,							//干道寻路
	WAYFINDINGTYPE_CONTINUOUS = 2,							//连续寻路
	WAYFINDINGTYPE_POINT_BY_POINT = 3,						//逐点寻路
	WAYFINDINGTYPE_MAX
};

enum FOLLOW_MOVE_TYPE
{
	FOLLOW_MOVE_TYPE_FOREVER		= 0,	//永远跟随
	FOLLOW_MOVE_TYPE_LIMIT			= 1,	//限时跟随
	FOLLOW_MOVE_TYPE_LIMIT_CLEAR	= 2,	//限时跟随，时间到以后消失
	FOLLOW_MOVE_TYPE_MAX,
};

//////////////////////////////////////////////////////////////////////////
//
typedef struct war_robot_distance_threat_t
{
	Unit *unit;
	float dis_threat;
}war_robot_distance_threat;

class MovementGenerator
{
public:
	virtual ~MovementGenerator(){};

	virtual void Initialize(Unit &,uint64  = 0, int64 = 0){};
	virtual void Finalize(Unit &) {};
	virtual void Reset(Unit &) {};
	virtual bool Update(Unit &, const uint32 &time_diff) = 0;
	virtual MovementGeneratorType GetMovementGeneratorType() = 0;
	virtual void unitSpeedChanged() { }
	virtual bool GetDestination(float& /*x*/, float& /*y*/) const { return false; }
		
	static void RandomPath(Unit *unit,std::deque<float>& path,uint32 ms,float x,float y);		//随机路径
	static void RandomPos(float& nx,float& ny,float fPosX,float fPosY,float range = 4.0f);	
protected:
	static void RandomPos(float& nx,float& ny,const Unit *wo,float range = 4.0f);	

	static inline void GetNewPos(float& nx,float& ny,float fPosX,float fPosY,float range,float angle)
	{
		const float distanceX = range * cos(angle);
		const float distanceY = range * sin(angle);

		nx = fPosX + distanceX;
		ny = fPosY + distanceY;
	}
};


//////////////////////////////////////////////////////////////////////////
//for 静止,
class IdleMovementGenerator : public MovementGenerator
{
public:
	//光个静止太薄弱了,至少也是定时静止
	IntervalTimer m_timer;

	void Initialize(Unit &,uint64 = 0 , int64 t = 0)
	{
		m_timer.SetInterval(t);
		m_timer.SetCurrent(0);
	}

	bool Update(Unit &unit, const uint32 &diff)
	{
		//如果设置的时间为0则一直静止
		if(m_timer.GetInterval()==0)
			return true;

		//如果超时了,那么返回
		m_timer.Update(diff);
		if (m_timer.Passed())
		{
			unit.SetTarget(NULL);		//不然仇恨管理无法指引他去砍人
			return false;
		}
		return true;
	}
	MovementGeneratorType GetMovementGeneratorType()
	{
		return IDLE_MOTION_TYPE;
	}
};

template<class T, class D>
class MovementGeneratorMedium : public MovementGenerator
{
public:
	void Initialize(Unit &u,uint64 param = 0, int64 t = 0)
	{
		//u->AssertIsType<T>();
		(static_cast<D*>(this))->Initialize(*((T*)&u),param,t);
	}
	void Finalize(Unit &u)
	{
		//u->AssertIsType<T>();
		(static_cast<D*>(this))->Finalize(*((T*)&u));
	}
	void Reset(Unit &u)
	{
		//u->AssertIsType<T>();
		(static_cast<D*>(this))->Reset(*((T*)&u));
	}
	bool Update(Unit &u, const uint32 &time_diff)
	{
		//u->AssertIsType<T>();
		return (static_cast<D*>(this))->Update(*((T*)&u), time_diff);
	}
public:
	// will not link if not overridden in the generators
	void Initialize(T &u,uint64 = 0, int64 t = 0);
	void Finalize(T &u);
	void Reset(T &u);
	bool Update(T &u, const uint32 &time_diff);
};

//////////////////////////////////////////////////////////////////////////
//沿路线走动
class WayPointMovementGenerator : public MovementGeneratorMedium<Creature,WayPointMovementGenerator>
{
public:
	WayPointMovementGenerator(){}
	void Initialize(Creature &,uint64 param = 0,int64 t = 0);
	void Reset(Creature &);
	bool Update(Creature &, const uint32 &);
	void Finalize(Creature &);

	MovementGeneratorType GetMovementGeneratorType() { return WAYPOINT_MOTION_TYPE; }
	vector<mt_point> path;
private:	
	uint32 wayid;
	uint32 point_index;	
	//是否需要回退状态
	bool m_revert_active_grid;;
};

//寻路走向目标
class WayFindingMovementGenerator : public MovementGeneratorMedium<Creature,WayFindingMovementGenerator>
{
public:
	WayFindingMovementGenerator();

	virtual void Initialize(Creature &creature, uint64 param = 0, int64 t = 0);
	virtual void Reset(Creature &) {};
	virtual void Finalize(Creature &creature);
	virtual bool Update(Creature &creature, const uint32 &diff);

	virtual MovementGeneratorType GetMovementGeneratorType();
	static vector<float> find_link;		//寻路得到的路线图
protected:
	float targetPoxX, targetPoxY;		//目标坐标
	WayfindingType find_type;			//寻路类型
	float recall[4][2];					//记录每一次寻路，四次一组，若两两想同则扩大搜索范围
	int recallpos;						//recall的当前指标
	uint16 basenumber;					//搜索范围
	uint32 offset_angle,offset_radius;	//为了让怪分散一点
	bool WayPointFindPath(Creature &creature, Unit *target, uint16 cnx, uint16 cny);	
	//连续寻路
	bool ContinuousFindPath(Creature &creature, Unit *target, uint16 cnx, uint16 cny);
	//尝试通过
	bool TestGo(Unit *target, uint16 &poxX, uint16 &poxY, uint16 p, bool &complete);
	bool FindPath(Creature &creature, Unit *target, uint16 cnx, uint16 cny);
	//尝试通过
	bool TestGo(Creature &creature, Unit *target, uint16 p, uint16 &toPoxX, uint16 &toPoxY);
	virtual void GetTargetPos(Creature &creature, Unit *target, const MapTemplate *mt);
};

//寻路追杀
class WayFindingAndAttackMovementGenerator:public WayFindingMovementGenerator
{
public:
	WayFindingAndAttackMovementGenerator();

	virtual void Initialize(Creature & creature, uint64 param = 0, int64 t = 0);
    virtual bool Update(Creature &creature, const uint32 &diff);
	virtual MovementGeneratorType GetMovementGeneratorType()
	{
		return WAYFINDING_ATTACK_MOTION_TYPE;
	}
protected:
	bool m_is_attack_target;
	uint32 m_target_dis;						//追杀时允许与目标的最近距离
	//IntervalTimer m_recover_timer;				//自动恢复定时器
private:
	struct spell_cast_probability_t *spell;		//目前随机到的技能，如果是NULL，则是普通攻击
	bool AttackTarge(Creature &creature, const uint32 &diff);

	//立即攻击
	void Attack(Creature &creature, const uint32 &diff);

	void RandSpell(Creature &creature, float dis);
	bool IsInCastRange(Creature &creature, float dis);
	bool lessThan(float dis, float dis2);
};

//木偶式的宠物移动方式，跟随玩家并攻击玩家所选择的目标
class MercenaryMovementGenerator : public WayFindingAndAttackMovementGenerator
{
public:
	MercenaryMovementGenerator();
	virtual void Initialize(Creature &,uint64 param = 0,int64 t = 0);
	virtual void Reset(Creature &creature){ Initialize(creature);}
	virtual bool Update(Creature &, const uint32 &);
	virtual void Finalize(Creature &creature);
	MovementGeneratorType GetMovementGeneratorType()
	{
		return MERCENARY_MOTION_TYPE;
	}
protected:
	virtual Unit *GetAttackTarget(Creature &creature,Unit *owner);
protected:
	//是否会攻击目标
	bool m_canAttack;
	//停止下来后是否会随机移动
	bool m_canRandomMove;
	uint8 follow_type;
	//是否需要回退状态
	bool m_revert_active_grid;
	//跟随的对象
	uint32 m_owner;	
	//随机移动定时器
	TimeTrackerSmall m_next_random_move;
	//是否随机移动中
	bool m_is_random_move;	
	//跟随时限
	IntervalTimer m_timer;
};

//寻路走向指定地点
class WayFindingDstMovementGenerator : public MovementGeneratorMedium<Creature,WayFindingDstMovementGenerator>
{
public:
	WayFindingDstMovementGenerator();

	virtual void Initialize(Creature &creature, uint64 param = 0, int64 t = 0);
	virtual void Reset(Creature &) {};
	virtual void Finalize(Creature &creature);
	 virtual bool Update(Creature &creature, const uint32 &diff);
	virtual MovementGeneratorType GetMovementGeneratorType();
	static vector<float> find_link;		//寻路得到的路线图
protected:
	float targetPoxX, targetPoxY;		//目标坐标
	WayfindingType find_type;			//寻路类型
	float recall[4][2];					//记录每一次寻路，四次一组，若两两想同则扩大搜索范围
	int recallpos;						//recall的当前指标
	uint16 basenumber;					//搜索范围
	uint32 offset_angle,offset_radius;	//为了让怪分散一点
	bool WayPointFindPath(Creature &creature, uint16 cnx, uint16 cny);	
	//连续寻路
	bool ContinuousFindPath(Creature &creature, uint16 cnx, uint16 cny);
	//尝试通过
	bool TestGo(Creature &creature, uint16 &poxX, uint16 &poxY, uint16 p, bool &complete);
	bool FindPath(Creature &creature);
};

//////////////////////////////////////////////////////////////////////////
//抽象工厂

namespace Tea
{

struct SelectableMovement : public FactoryHolder<MovementGenerator,MovementGeneratorType>
{
	SelectableMovement(MovementGeneratorType mgt) : FactoryHolder<MovementGenerator,MovementGeneratorType>(mgt) {}
};

template<class REAL_MOVEMENT>
struct MovementGeneratorFactory : public SelectableMovement
{
	MovementGeneratorFactory(MovementGeneratorType mgt) : SelectableMovement(mgt) {}

	MovementGenerator* Create(void *) const
	{
		return (new REAL_MOVEMENT());
	}
};

typedef FactoryHolder<MovementGenerator,MovementGeneratorType> MovementGeneratorCreator;
typedef FactoryHolder<MovementGenerator,MovementGeneratorType>::FactoryHolderRegistry MovementGeneratorRegistry;
typedef FactoryHolder<MovementGenerator,MovementGeneratorType>::FactoryHolderRepository MovementGeneratorRepository;

MovementGenerator* SelectMovementGenerator(Creature *);
MovementGenerator* SelectMovementGenerator(Creature *,MovementGeneratorType type,int param1 = 0);

void InitializeMovement();

}

#endif

