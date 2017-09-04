#include "scened_app.h"
#include "ThreatManager.h"
#include "Unit.h"
#include "Player.h"
#include "Grid.h"
#include "Creature.h"
#include "LootManager.h"
#include "Map.h"

//////////////////////////////////////////////////////////////////////////
//for ThreatManager

void ThreatManager::Initialize(Creature *unit, uint32 _visionradius, uint32 _actionradius)
{
	me = unit;
	
	actionradius = float(_actionradius);
	visionradius = float(_visionradius);
	me->GetBornPos(bornX, bornY);
}

Unit *ThreatManager::GetTarget()
{
	Unit *target = NULL;
	uint32 threat = 0;
	for(TargetMap::iterator iter = m_targetMap.begin(); iter != m_targetMap.end(); ++iter)
	{
		if(threat < iter->second.first + iter->second.second) 
		{
			Unit *unit = me->GetMap()->FindUnit(iter->first);
			if (!unit)
				continue;
			target = unit;
			threat = iter->second.first + iter->second.second;
		}
	}
	return target;
}

//增加攻击仇恨
void ThreatManager::Add(Unit *target,uint32 v)
{ 
	if(me->GetEvadeState()) return;
	TargetMap::iterator iter = m_targetMap.find(target->GetUIntGuid());
	if(iter != m_targetMap.end())
	{
		iter->second.first += v;
	}
	else
	{
		if (!me->IsFriendlyTo(target))
			m_targetMap.insert(TargetMap::value_type(target->GetUIntGuid(), ThreatValue(v, 0)));
		//tea_pdebug("me[%s] 对[%s] 增加仇恨 ",me->GetName(),target->GetName());
		//target->AddAttacker(me);

		//初次出现在视野
		me->AI_MoveInLineOfSight(target);
	}
}

void ThreatManager::Del(Unit *target, bool isDelOwnerShip)
{
	TargetMap::iterator iter = m_targetMap.find(target->GetUIntGuid());
	if(iter != m_targetMap.end()) 
	{
		m_targetMap.erase(iter);
		target->DelAttacker(me->GetUIntGuid(), isDelOwnerShip);
	}
}

void ThreatManager::Clear()
{
	for(TargetMap::iterator iter = m_targetMap.begin(); iter != m_targetMap.end(); ++iter)
	{
		//g_obj_mgr->FindObject();
	 	Unit *unit = me->GetMap()->FindUnit(iter->first);
		if (!unit)
			continue;
		unit->DelAttacker(me->GetUIntGuid());
	}
	m_targetMap.clear();

	me->SetTarget(NULL);
}

void ThreatManager::Update(uint32 )
{
	ASSERT(me);	
	ASSERT(me->isAlive());
	//目前没有逃脱模式
	//if(me->GetEvadeState()) return;
	
	//如果是防御怪
	if(me->GetReactState() == REACT_DEFENSIVE)
		return;

	float x,y;
	me->GetBornPos(x,y);

	//超出活动范围直接返回
	if((uint32)me->GetDistance(x,y) > actionradius) 
	{
		Clear();
		//清除归属者
		me->ResetOwnerShip();
		//有待商榷
		//me->ClearAttacker();
		return;
	}
	//更新旧的仇恨列表
	UpdateThreatMap();

	//如果可能主动攻击其他生物的生物
	if(me->GetReactState() == REACT_AGGRESSIVE || me->GetReactState() == REACT_AGGRESSIVE_UNIT )
	{
		Grid *grid = me->GetGrid();
		//从周围九大格获得所有玩家
		Grid::CreatureList::iterator iter;
		Grid::GridPtrVec::iterator viter;
		for(viter = grid->notice_grid.begin(); viter != grid->notice_grid.end(); ++viter)
			for(iter = (*viter)->creatures.begin(); iter != (*viter)->creatures.end(); ++iter)
				CaluThreatByDistance(*iter);
	}

	if(me->GetReactState() == REACT_AGGRESSIVE || me->GetReactState() == REACT_AGGRESSIVE_PLAYER )
	{
		//如果可能主动攻击玩家的生物
		Grid *grid = me->GetGrid();
		//从周围九大格获得所有玩家
		PlayerSet::iterator iter;
		Grid::GridPtrVec::iterator viter;
		for(viter = grid->notice_grid.begin(); viter != grid->notice_grid.end(); ++viter)
			for(iter = (*viter)->players.begin(); iter != (*viter)->players.end(); ++iter)
				CaluThreatByDistance(*iter);
	}

	//走向最大仇恨目标
	Unit *threatTarget = GetTarget();
	if (threatTarget != me->GetTarget())
	{
		me->SetTarget(threatTarget);
		if (threatTarget)
			me->MotionMoveTarget();//移动到目标
	}

	// 如果找不到目标 则清空攻击列表
	if (threatTarget == NULL) {
		if (me->GetHealth() < me->GetMaxHealth()) {
			// 清空攻击关系列表
			me->GetMap()->ClearCreatureHitHash(me->GetUIntGuid());
		}
	}
}

void ThreatManager::CaluThreatByDistance(Unit *_target)
{
	//如果玩家已经存在仇恨则不需要再次增加距离仇恨	
	if(m_targetMap.count(_target->GetUIntGuid())>0)
		return;
	float x,y;
	me->GetBornPos(x,y);

	//玩家在视野范围内，并且在怪物活动范围内
	float distance = 0.0f;
	if( _target->isAlive() && 
		!me->IsFriendlyTo(_target) &&
		_target->GetDistance(x, y) < actionradius && 
		(distance = me->GetDistance(_target)) < visionradius&&
		_target->isCanSee())
	{
		//初次出现在视野
		if(me->AI_MoveInLineOfSight(_target) != 0)
		{
			m_targetMap.insert(TargetMap::value_type(_target->GetUIntGuid(), ThreatValue(0,  getThreat(distance))));
		}
	}
}

//更新旧的仇恨列表
void ThreatManager::UpdateThreatMap()
{
	Unit *threater = 0;
	Unit *tagert = me->GetTarget();
	float x,y;
	me->GetBornPos(x,y);
	float distance =  0.0f;
	TargetMap::iterator iter = m_targetMap.begin();
	while(iter != m_targetMap.end())
	{
		threater = me->GetMap()->FindUnit(iter->first);
		if (!threater )
		{
			me->DelOwnerShip(iter->first);
			iter = m_targetMap.erase(iter);
			continue;
		}
		if(	threater->isAlive() &&
			threater->GetDistance(x,y) < actionradius)
		{						
			distance = me->GetDistance(threater);
			if(distance < visionradius)
				iter->second.second = getThreat(distance);
			else 
				iter->second.second = 0;
			++iter;
			continue;
		}
		m_targetMap.erase(iter++);
		threater->DelAttacker(me->GetUIntGuid());
		if (threater->GetTypeId() == TYPEID_PLAYER)
			me->DelOwnerShip(threater->GetUIntGuid());
		//TODO:这里可能出现死循环，修改时要注意
		if(tagert == threater) me->SetTarget(NULL);
	}
}
