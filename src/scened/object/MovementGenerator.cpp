#include <shared/map_template.h>
#include "scened.h"
#include "scened_scripts.h"
#include "Grid.h"
#include "Map.h"
#include "Creature.h"
#include "Unit.h"
#include "Player.h"
#include "MovementGenerator.h"
#include "BuffManager.h"

#define EPS 1E-4

//连续寻路几次
const uint16 FIND_NUMBER = 20;
const int FOLLOW_CHECK_OWNER_TIMER = 1000;
const float FOLLOW_DISTANCE = 8.0f;			//机关兽跟随距离
const float MAX_FOLLOW_DISTANCE = 20.0f;	//机关兽最大跟随距离
const float TELEPORT_DISTANCE = 5.0f;		//机关兽传回距离
const int AUTO_CHECK_RECOVER_TIME = 1000;	//自动检测恢复血和蓝时间
//////////////////////////////////////////////////////////////////////////
//for MovementGenerator

void MovementGenerator::RandomPos(float& nx, float& ny, const Unit *wo, float range /* = 4.0f */)
{
    RandomPos(nx, ny, wo->GetPositionX(), wo->GetPositionY(), range);
}

void MovementGenerator::RandomPos(float& nx, float& ny, float fPosX, float fPosY, float range /* = 4.0f */)
{
    const float angle = float(rand_norm() * (M_PI * 2));
    GetNewPos(nx, ny, fPosX, fPosY, range, angle);
}

void MovementGenerator::RandomPath(Unit *unit,std::deque<float>& path,uint32 ms, float x, float y)
{
	//先根据移动速度算出移动距离
	float dist = float(ms)/float(unit->GetMoveSpeed());	

	float oldX,oldY;
	unit->GetPosition(oldX,oldY);

	const MapTemplate *templ = const_cast<Unit*>(unit)->GetMap()->GetMapTemp();
	ASSERT(templ);

	float nx,ny;			

	//为了防止出现死循环,只循环最多1秒5次
	for (uint32 i = 0; i < ms/200; i++ )
	{
		if (x > 0.00001 && y > 0.00001 && i == 0)
		{	
			nx = x;
			ny = y;
			float range = hypotf(nx,ny);
			//不到一个攻击单位,至少要的
			if(range < 0.5f*g_Config.nomal_attack_distance)
				range = g_Config.nomal_attack_distance;
			else if(range > 1.5f*g_Config.nomal_attack_distance)
				range = 1.5f*g_Config.nomal_attack_distance;
			dist -= range;
		}
		else
		{	//从需要移动的距离随机出一个移动点
			float range = float(rand_norm()*dist);	
			//不到一个攻击单位,至少要的
			if(range < 0.5f*g_Config.nomal_attack_distance)
				range = g_Config.nomal_attack_distance;
			else if(range > 1.5f*g_Config.nomal_attack_distance)
				range = 1.5f*g_Config.nomal_attack_distance;
			dist -= range;
			RandomPos(nx,ny,oldX,oldY,range);
		}		

		//验证一下路障,不行就继续
		if(!templ->IsCanRun(oldX,oldY,nx,ny,false)){
			tea_pdebug("MovementGenerator::RandomPath !IsCanRun");
			continue;
		}

		path.push_back(nx);
		path.push_back(ny);

		//保存上一点为出发点
		oldX = nx;
		oldY = ny;

		//如果还不到攻击距离,停止随机
		if(dist < g_Config.nomal_attack_distance)
			break;
	}

	if(path.size() == 0)
	{
		path.push_back(oldX);
		path.push_back(oldY);
	}
}

//////////////////////////////////////////////////////////////////////////
//for DISAPPEAR_MOTION_TYPE
class DisappearMovementGenerator : public MovementGenerator
{
public:
    bool Update(Unit &, const uint32 &)
    {
        return false;
    }
    MovementGeneratorType GetMovementGeneratorType()
    {
        return DISAPPEAR_MOTION_TYPE;
    }

    void Finalize(Creature &creature)
    {		
        creature.GetMap()->ToDelCreature(&creature);	//将生物从地图中移除		
    }
};

//for DEADLINE_MOTION_TYPE
class DeadlineMovementGenerator : public MovementGenerator
{
public:
	IntervalTimer m_clear_timer;

	void Initialize(Unit &,uint64 = 0 , int64 t = 0)
	{
		m_clear_timer.SetInterval(t);
		m_clear_timer.SetCurrent(0);
	}

	bool Update(Unit &, const uint32 &diff)
	{
		m_clear_timer.Update(diff);
		if (m_clear_timer.Passed()) {
			return false;
		}
		return true;
	}
	MovementGeneratorType GetMovementGeneratorType()
	{
		return DEADLINE_MOTION_TYPE;
	}

	void Finalize(Unit &unit)
	{
		if (unit.GetTypeId() == TYPEID_UNIT) {
			unit.GetMap()->ToDelCreature((Creature*)&unit);	//将生物从地图中移除
		}
	}
};

//////////////////////////////////////////////////////////////////////////
//for RandomMovementGenerator
#include "Creature.h"

//随机走动
class RandomMovementGenerator : public MovementGeneratorMedium<Creature, RandomMovementGenerator>
{
public:
    RandomMovementGenerator(): m_next_move(0){}
    void Initialize(Unit &unit, uint64 param = 0, int64 t = 0) { 
		m_next_move.Reset(urand(10000, 60000));
		unit.SetOrientation(float(rand_norm() * (M_PI * 2)));
	};
    bool Update(Creature &creature, const uint32 &diff)
    {
        //移动中,不需要重算目标点
        if(creature.IsMoving())
            return false;

        m_next_move.Update(diff);

        if(m_next_move.Passed())
        {
            if(!creature.isAlive())
                return false;

            //从模板中得到活动半径，生成随机位置
            creature_template& ct = creature.GetTemplate();
            float wander_distance = (float)ct.visionradius * g_Config.monster_random_radius_percent;

			const float angle = float(rand_norm() * (M_PI * 2));
			const float range = float(rand_norm() * wander_distance);
			const float distanceX = range * cos(angle);
			const float distanceY = range * sin(angle);

			float bornX, bornY;
			creature.GetBornPos(bornX, bornY);

			float nx = bornX + distanceX;
			float ny = bornY + distanceY;

			if (creature.GetMap()->GetMapTemp()->IsCanRun(creature.GetPositionX(),creature.GetPositionY(),nx,ny, true))
			{
				creature.MoveTo(nx, ny);
			}

            m_next_move.Reset(urand(30000, 60000));
        }

        return true;
    }
    void Finalize(Creature &) {};
    void Reset(Creature &) {};

    MovementGeneratorType GetMovementGeneratorType()
    {
        return RANDOM_MOTION_TYPE;
    }

private:
    //destination m_dest;
    TimeTrackerSmall m_next_move;	
};

////////////////////////////////////////////////////////////////////////////////////////////
//寻路走向目标
WayFindingMovementGenerator::WayFindingMovementGenerator():targetPoxX(0), targetPoxY(0), find_type(WAYFINDINGTYPE_NONE), recallpos(0), basenumber(1)
{
}

void WayFindingMovementGenerator::Initialize(Creature &creature, uint64 param, int64 t)
{
	if(!creature.isAlive() || !creature.isCanMove())
		return;

	//计算移动位置
	Unit *target = creature.GetTarget();
	if(!target)
		return;

	const MapTemplate *mt = creature.GetMap()->GetMapTemp();
	//float angle = creature.GetAngle(target);
	//初始新目标点坐标
	GetTargetPos(creature, target, mt);

	uint16 lastx = 0, lasty = 0, cnx = 0, cny = 0;
	
	//若沿路无障碍，当然OK
	if(mt->IsCanRun(creature.GetPositionX(), creature.GetPositionY(), targetPoxX, targetPoxY, lastx, lasty, cnx, cny, true))
	{
// 		printf("生物[%s] %f:%f 移动到 %f:%f", creature.GetName(), creature.GetPositionX(), creature.GetPositionY(), targetPoxX, targetPoxY);
		creature.MoveTo(targetPoxX, targetPoxY);
	}
	else
	{
		//若寻路最后一个无障碍坐标与怪物目前坐标一致或者干脆根本没发现无障碍坐标
		if((lastx == (uint16)creature.GetPositionX() && lasty == (uint16)creature.GetPositionY()) || (lastx == 0 && lasty == 0))
		{
			//tea_pdebug("【%s】遇到障碍，开始寻路。", creature.GetName());
			if(find_type == WAYFINDINGTYPE_NONE)
			{
				//干道寻路
				if(WayPointFindPath(creature, target, cnx, cny))
					return;
				/*if(ContinuousFindPath(creature, target, cnx, cny))
					return;*/
			}
			return;
			//逐点寻路
			if(!FindPath(creature, target, cnx, cny))
			{
				//寻路失败，将过往路径清除。
				basenumber = 1;
				recallpos = 0;
				return;
			}
		}
		else
		{
			basenumber = 1;
			recallpos = 0;
			creature.MoveTo(lastx, lasty);
		}

		//tea_pdebug("生物[%s] %f:%f 移动到 %f:%f", creature.GetName(), creature.GetPositionX(), creature.GetPositionY(), targetPoxX, targetPoxY);

	}

	//creature.MoveTo(targetPoxX,targetPoxY,creature.GetAttackDistance(target));
}

void WayFindingMovementGenerator::GetTargetPos(Creature &creature, Unit *target, const MapTemplate *mt)
{
	float angle = creature.GetAngle(target);		
	uint32 cguid = creature.GetUIntGuid();

	target->removePositionInfo(cguid);

	float dis = creature.GetDistance(target);
	float attackRange = creature.GetAttackDistance(NULL);
	
	if (dis <= attackRange) {
		targetPoxX = creature.GetPositionX();
		targetPoxY = creature.GetPositionY();

		point_t p;
		p.pos_x = creature.GetPositionX();
		p.pos_y = creature.GetPositionY();
		target->positionInfo[cguid] = p;
		return;
	}

	if (!target->getNearestPositionByAngleAndAttackRange(angle, attackRange, target->GetPositionX(), target->GetPositionY(), targetPoxX, targetPoxY) || !mt->IsCanRun((uint16)targetPoxX, (uint16)targetPoxY)) {
		targetPoxX = target->GetPositionX();
		targetPoxY = target->GetPositionY();
	}
	//const float offset_angle = float(rand_norm() * (M_PI * 2));
	//GetNewPos(targetPoxX, targetPoxY, target->GetPositionX(), target->GetPositionY(), range, offset_angle);
	else {
		point_t p;
		p.pos_x = targetPoxX;
		p.pos_y = targetPoxY;
		target->positionInfo[cguid] = p;
	}
}

void WayFindingMovementGenerator::Finalize(Creature &creature)
{
	creature.StopMoving();
}

bool WayFindingMovementGenerator::Update(Creature &creature, const uint32 &diff)
{
	//如果被限制移动移动
	if(!creature.isCanMove())
	{
		creature.SetTarget(NULL);
		return true;        
	}

	if(!creature.GetTarget()->isAlive())
	{
		creature.SetTarget(NULL);
		return true;
	}

	//如果生物是移动状态，并且目标的方向相同则不用处理，目标移动类型设置了目标
	float creatureX = creature.GetTarget()->GetPositionX();
	float creatureY = creature.GetTarget()->GetPositionY();

	if(targetPoxX != creatureX
		|| targetPoxY != creatureY
		|| !creature.IsMoving()/// toPoxX == uint16(creature.GetPositionX()) && toPoxY == uint16(creature.GetPositionY()))
		|| creature.GetIsBlowFly())
	{
		if(targetPoxX != creatureX || targetPoxY != creatureY)
		{
			basenumber = 1;
			recallpos = 0;
			find_type = WAYFINDINGTYPE_NONE;
		}

		//怪物的小数点在障碍里面，让他再多走一会儿，下次再寻路
		if(creature.IsMoving() 
			&& !creature.GetMap()->GetMapTemp()->IsCanRun((uint16)creature.GetPositionX(), (uint16)creature.GetPositionY()))
			return true;
		creature.StopMoving();
		Initialize(creature);
		return true;
	}
	return true;
}

MovementGeneratorType WayFindingMovementGenerator::GetMovementGeneratorType()
{
	return WAYFINDING_MOTION_TYPE;
}

bool WayFindingMovementGenerator::WayPointFindPath(Creature &creature, Unit *target, uint16 cnx, uint16 cny)
{
	if(creature.GetTarget() && creature.GetDistance(creature.GetTarget()) > 50)
	{
		//tea_pdebug("距离大于50，放弃寻路");
		return false;
	}
	uint16 lastx = 0, lasty = 0;
	uint16 px = uint16(creature.GetPositionX()), py = uint16(creature.GetPositionY());
	const MapTemplate *mt = creature.GetMap()->GetMapTemp();
	uint32 point = px + mt->GetMapBaseInfo().width * py;
	uint32 point2 = 0;
	uint32 point_target;
	int me_size;
	int index_creature = -1, index_target = -1;
	const mt_point_in_line *me = mt->GetMonsterLineEach(point);
	if(me == NULL)
	{
		int8 index = MapTemplate::FindAroundIndex(cnx, cny, px, py);
		point2 = px + MapTemplate::around[(index - 1) % 8][0] + mt->GetMapBaseInfo().width * (py + MapTemplate::around[(index - 1) % 8][1]);
		me = mt->GetMonsterLineEach(point2);
		if(me == NULL)
		{
			point2 = px + MapTemplate::around[(index + 1) % 8][0] + mt->GetMapBaseInfo().width * (py + MapTemplate::around[(index + 1) % 8][1]);
			me = mt->GetMonsterLineEach(point2);
			if(me == NULL)
				return false;
		}
	}

	mt->IsCanRun(targetPoxX, targetPoxY, creature.GetPositionX(), creature.GetPositionY(), lastx, lasty, cnx, cny);
	int8 index = MapTemplate::FindAroundIndex(cnx, cny, lastx == 0 ? uint16(targetPoxX) : lastx, lasty == 0 ? uint16(targetPoxY) : lasty);
	if(index == -1)
	{
		//tea_pdebug("看来，玩家进入障碍了。");
		return false;
	}
	if(index % 2 == 0)
	{
		index = (index + 9) % 8;
		uint16 nlx = lastx + MapTemplate::around[index][0];
		uint16 nly = lasty + MapTemplate::around[index][1];
		if(mt->IsCanRun(nlx,nly))
		{
			lastx = nlx;
			lasty = nly;
		}
	}
	point_target = lastx + mt->GetMapBaseInfo().width * lasty;
	const mt_point_in_line *me_t = mt->GetMonsterLineEach(point_target);
	//不在同一个干道
	if(me_t == NULL || me_t->each != me->each)
	{
		tea_pdebug("不在同一个干道。从%d,%d到%d,%d", py, px, (uint16)targetPoxX, (uint16)targetPoxY);
		return false;
	}

	//tea_pdebug("进入干道");
	me_size = me->each->point_each.size();
	index_target = me_t->index;
	index_creature = me->index;

	if(index_creature == -1 || index_target == -1 || index_creature == index_target)
	{
		tea_perror("干道点没画好。从%d,%d到%d,%d", py, px, (uint16)targetPoxX, (uint16)targetPoxY);
		return false;
	}

	//1顺向，2逆向
	uint8 direction = index_target > index_creature ? 1 : 2;
	if(me->each->is_join)
	{
		if(direction == 1)
			direction = index_target - index_creature < index_creature + me_size - index_target ? 1 : 2;
		else
			direction = index_creature - index_target> index_target + me_size - index_creature ? 1 : 2;
	}

	int max_i = (direction == 1 ? index_target + me_size - index_creature : index_creature + me_size - index_target) % me_size;		
	int i = point2 == 0 ? 0 : 1;
	if(point2 != 0)
	{
		find_link[0] = float(point2 % mt->GetMapBaseInfo().width);
		find_link[1] = float(point2 / mt->GetMapBaseInfo().width);
		max_i++;
	}
	int max_size = (max_i + 1) * 2;
	uint32 array_len = 0;
	index_creature += me_size;
	if(find_link.size() < uint32(max_size))
		find_link.resize(uint32(max_size));
	do
	{
		if(direction == 1)
			index_creature++;
		else
			index_creature--;
		uint32 index = index_creature % me_size;
		mt_monsterline_each *each = NULL;
		list<mt_point*>::iterator list_it = me->each->point_each.begin();
		for (uint32 t_i = 0; t_i < index; ++list_it, t_i++);			
		find_link[i * 2] = (*list_it)->pos_x;
		find_link[i * 2 + 1] = (*list_it)->pos_y;
		array_len += 2;
		if(mt->IsCanRun(find_link[i * 2], find_link[i * 2 + 1], targetPoxX, targetPoxY))
		{
			i++;
			find_link[i * 2] = targetPoxX;
			find_link[i * 2 + 1] = targetPoxY;
			array_len += 2;
			break;
		}
		i++;
		if(i > 50)
		{
			//tea_pdebug("寻路超过50步，放弃。");
			return false;
		}
	}
	while(i < max_i);

	for (uint32 fi = 0; fi < array_len; fi+=2)
	{
		if(fi > 2)
		{
			if(!creature.GetMap()->GetMapTemp()->IsCanRun(find_link[fi-2],find_link[fi-1],find_link[fi],find_link[fi+1]))
			{
				tea_perror("way finding err map:%u line no:%u, creature X %u, creature Y %u to target X %u,target Y %u",
					creature.GetMapId(), me->index, px, py, (uint16)targetPoxX, (uint16)targetPoxY); 
#ifdef WIN32
ASSERT(false);
#endif //windows下面的测试
				return false;
			}
		}
	}

	creature.MoveTo(array_len, &find_link[0]);

	find_type = WAYFINDINGTYPE_WAYPOINT;
	return true;
}
	
//连续寻路
bool WayFindingMovementGenerator::ContinuousFindPath(Creature &creature, Unit *target, uint16 cnx, uint16 cny)
{
	uint16 find_number = 20;
	uint16 c_r_x, c_r_y, c_l_x, c_l_y;
	uint16 last_l_x = cnx, last_l_y = cny, last_r_x = cnx, last_r_y = cny;
	int find_index = 0;
	int left_number = 0, right_number = 0;
	bool left = false, right = false;
	int i;

	c_r_x = (uint16)creature.GetPositionX();
	c_r_y = (uint16)creature.GetPositionY();
	c_l_x = c_r_x;
	c_l_y = c_r_y;
	for(; find_index < find_number; find_index++)
	{
		//探寻这一步从那一个朝向开始寻路
		int8 p_l = MapTemplate::FindAroundIndex(last_l_x, last_l_y, c_l_x, c_l_y);
		int8 p_r = MapTemplate::FindAroundIndex(last_r_x, last_r_y, c_r_x, c_r_y);
		if(find_index != 0)
		{
			//左右置一下朝向，因为FindAroundIndex是反向计算的
			p_l -= 2;
			p_r += 2;
		}
		bool left_find = false, right_find = false;

		//保存上一步
		last_r_x = c_r_x;
		last_r_y = c_r_y;
		last_l_x = c_l_x;
		last_l_y = c_l_y;

		for(i = 1; i < 8; i++)
		{
			p_r ++;
			//尝试通过
			if(!right_find && TestGo(target, c_r_x, c_r_y, p_r, right))
			{
				find_link[find_index * 2] = c_r_x;
				find_link[find_index * 2 + 1] = c_r_y;
				if(right) break; //寻到直达的路了
				right_find = true;//本次寻路成功，但是只限于这一步
			}
			p_l --;
			//尝试通过
			if(!left_find && TestGo(target, c_l_x, c_l_y, p_l, left))
			{
				find_link[(find_index + find_number) * 2] = c_l_x;
				find_link[(find_index + find_number) * 2 + 1] = c_l_y;
				if(left) break;//寻到直达的路了
				left_find = true;//本次寻路成功，但是只限于这一步
			}
			if(right_find && left_find)
				break;//左右都寻到了
			if(i == 7)
			{
				//tea_perror("寻路有问题，一圈都没出口，明显是错的,或者怪物进入障碍，然后重新寻路。");
				//处理
				return false;
			}
		}
		if (right || left)
		{
			if(right)
				creature.MoveTo((find_index + 1) * 2, &find_link[0]);
			else
				creature.MoveTo((find_index + 1) * 2, &find_link[find_number * 2]);
			find_type = WAYFINDINGTYPE_CONTINUOUS;
			//tea_pdebug("连续寻路OK");
			return true;
		}
	}
	return false;
}

//尝试通过
bool WayFindingMovementGenerator::TestGo(Unit *target, uint16 &poxX, uint16 &poxY, uint16 p, bool &complete)
{
	complete = false;
	if(target->GetMap()->GetMapTemp()->IsCanRun(poxX, poxY, target->GetPositionX(), target->GetPositionY()))
	{
		poxX = (uint16)target->GetPositionX();
		poxY = (uint16)target->GetPositionY();
		complete = true;
		return true;
	}

	uint16 index = p & 7;
	uint16 tx, ty;
	tx = uint16(poxX + MapTemplate::around[index][0] * basenumber);
	ty = uint16(poxY + MapTemplate::around[index][1] * basenumber);

	if(target->GetMap()->GetMapTemp()->IsCanRun(poxX, poxY, tx, ty, true))
	{
		poxX = tx;
		poxY = ty;
		return true;
	}

	return false;
}

bool WayFindingMovementGenerator::FindPath(Creature &creature, Unit *target, uint16 cnx, uint16 cny)
{
	int i = 1;

	//目标相同但是寻路已经超过四次，则开始比较是否走回头路
	if(recallpos > 3)
	{
		if(recall[0][0] == recall[2][0] && recall[0][1] == recall[2][1] && recall[1][0] == recall[3][0] && recall[1][1] == recall[3][1])
		{
			//丫回头了，扩大搜索范围
			recallpos = 0;
			basenumber ++;
		}
		else
		{
			//丫木有回头，OK，算你第一次寻路
			recallpos = 0;
			basenumber = 1;
		}
	}

	//把怪物目前坐标先保存下来
	uint16 toPoxX = uint16(creature.GetPositionX());
	uint16 toPoxY = uint16(creature.GetPositionY());
	int8 around_index = MapTemplate::FindAroundIndex(cnx, cny, toPoxX, toPoxY);

	for(; i < 8; i++)
	{
		//若林北是奇数，林北变成负数。
		if((i & 1) == 1)
		{
			i = 0 - i;
		}
		//顺序是，左前，右前，左，右，左后，右后，后
		around_index += i;
		i = abs(i);
		//尝试通过
		if(TestGo(creature, target, around_index, toPoxX, toPoxY))
		{
			//保存走过的路径，并让下标+1
			recall[recallpos % 4][0] = toPoxX;
			recall[recallpos % 4][1] = toPoxY;
			recallpos++;
			find_type = WAYFINDINGTYPE_POINT_BY_POINT;
			creature.MoveTo(toPoxX, toPoxY);
			return true;
		}
	}

	return false;
}

//尝试通过
bool WayFindingMovementGenerator::TestGo(Creature &creature, Unit *target, uint16 p, uint16 &toPoxX, uint16 &toPoxY)
{
	uint16 index = p & 7;
	uint16 tx, ty;
	tx = uint16(toPoxX + MapTemplate::around[index][0] * basenumber);
	ty = uint16(toPoxY + MapTemplate::around[index][1] * basenumber);

	if(target->GetMap()->GetMapTemp()->IsCanRun(toPoxX, toPoxY, tx, ty, true))
	{
		toPoxX = tx;
		toPoxY = ty;
		return true;
	}

	return false;
}

//寻路追杀
WayFindingAndAttackMovementGenerator::WayFindingAndAttackMovementGenerator(): 
	spell(NULL), m_is_attack_target(true), m_target_dis(4)
{
	//m_recover_timer.SetCurrent(0);
	//m_recover_timer.SetInterval(AUTO_CHECK_RECOVER_TIME);
}

void WayFindingAndAttackMovementGenerator::Initialize(Creature & creature, uint64 param, int64 t)
{
	WayFindingMovementGenerator::Initialize(creature, param,t);
}


bool WayFindingAndAttackMovementGenerator::Update(Creature &creature, const uint32 &diff)
{	
	if(creature.GetTarget() == NULL)
		return false;
	if(AttackTarge(creature, diff))
		return true;
	
	return WayFindingMovementGenerator::Update(creature,diff);
}

void WayFindingAndAttackMovementGenerator::RandSpell(Creature &creature, float dis)
{
	//若当前没有需要释放的技能，开始随技能
	if(spell == NULL && creature.HasSpell())
	{
		spell = creature.RandSpell(dis);
	}
}

bool WayFindingAndAttackMovementGenerator::AttackTarge(Creature &creature, const uint32 &diff)
{
	if(m_is_attack_target)
	{
		//正在技能引导或者持续施法
		if(creature.isCasing() || creature.isSpellProcess())
			return true;

		if (ms_time() < creature.GetNextSpellCD()) {
			return true;
		}

		float dis = creature.GetDistance(creature.GetTarget());
		RandSpell(creature, dis);
		
		//如果在攻击距离以内，直接攻击就好不做移动;
		if(spell && (spell->target_type == 0 || dis <= spell->spell_distance))
		{
			if(creature.IsMoving()) {
				//return true;
				creature.StopMoving(true);
			}

			if(creature.isCasing() &&
				(targetPoxX != creature.GetTarget()->GetPositionX()
				|| targetPoxY != creature.GetTarget()->GetPositionY()))
			{
				int ori1 = int(creature.GetOrientation() * 100 / 25);
				float ang = creature.GetAngle(creature.GetTarget());
				int ori2 = int(ang * 100 / 25);
				if(ori1 != ori2)
				{
					creature.SpellStop();
					return true;
				}
			}

			const float angle = creature.GetAngle(creature.GetTarget()->GetPositionX(), creature.GetTarget()->GetPositionY());

			if(angle != creature.GetOrientation())
			{
				//creature.MoveTurnAround(angle);
				creature.SetOrientation(angle);
			}

			//creature.MoveStop();
			if(!creature.isCasing() && !creature.isSpellProcess())
				Attack(creature, diff);
			
			return true;
		}
	
		if(IsInCastRange(creature, dis))
		{
			if(creature.IsMoving())
				creature.StopMoving(true);
			return true;
		}		
	}

	return false;
}

bool WayFindingAndAttackMovementGenerator::lessThan(float dis, float dis2) {
	return  dis < dis2 || abs(dis - dis2) <= EPS;
}

//是否已经在攻击距离了
bool WayFindingAndAttackMovementGenerator::IsInCastRange(Creature &creature, float dis)
{
	return ((spell && lessThan(dis, 0.0f + spell->spell_distance)) || lessThan(dis,0.0f+m_target_dis) || lessThan(dis, creature.GetAttackDistance(NULL)));
}

//立即攻击
void WayFindingAndAttackMovementGenerator::Attack(Creature &creature, const uint32 &diff)
{
	if(!creature.isCasing() && !creature.isSpellProcess() && !creature.IsCantCast())
	{
		if(creature.GetTarget() && creature.GetTarget()->isAlive() && creature.GetBuffManager()->HasBuff(BUFF_INVINCIBLE) == 0) {
			creature.SetCurSpellLevel(spell->spell_level);
			creature.CastSpell(spell->spell_id, creature.GetTarget()->GetPositionX(), creature.GetTarget()->GetPositionY());
		}
		spell = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
//木偶式的宠物移动方式，跟随玩家并攻击玩家所选择的目标
vector<float> WayFindingMovementGenerator::find_link;
MercenaryMovementGenerator::MercenaryMovementGenerator() : m_canAttack(true), m_canRandomMove(false), follow_type(FOLLOW_MOVE_TYPE_FOREVER), m_revert_active_grid(false), m_owner(0), m_next_random_move(10000), m_is_random_move(false), m_timer(0)
{

}
	
void MercenaryMovementGenerator::Initialize(Creature & creature, uint64 param, int64 t)
{
	if(param == 0)
	{
		WayFindingMovementGenerator::Initialize(creature);
		return;
	}
		
	if(t < 0)
	{
		creature.SetClearTimer(abs(t));
		follow_type = FOLLOW_MOVE_TYPE_LIMIT_CLEAR;
	}
	else if(t > 0)
	{
		m_timer.SetInterval(t);
		m_timer.SetCurrent(0);
		follow_type = FOLLOW_MOVE_TYPE_LIMIT;
	}

	Unit *unit = (Unit*)param;
	m_owner = unit->GetUIntGuid();

	if(unit && unit->GetTypeId() == TYPEID_PLAYER) 		
	{
		creature.SetCanActiveGrid(true);
		m_revert_active_grid = true;
	}
}

bool MercenaryMovementGenerator::Update(Creature &creature, const uint32 &diff)
{
	//如果已经挂了,不用跟了
	if(!creature.isAlive())
		return false;
		
	//如果设置的时间为0则一直跟随
	if(m_timer.GetInterval() != 0 )
	{
		m_timer.Update(diff);
		//如果超时了,那么取消跟随
		if (m_timer.Passed())
		{
			creature.SetTarget(NULL);		//不然仇恨管理无法指引他去砍人
			if(creature.IsMoving())
				creature.StopMoving(true);
			return false;
		}
	}

	//找到要跟随
	Unit *owner = creature.GetMap()->FindUnit(m_owner);
	if(!owner)
		return false;

	//尝试攻击
	if(m_canAttack)
	{
		Unit *target = GetAttackTarget(creature, owner);
		if(target)
		{
			//追杀！！！
			creature.SetTarget(target);
			m_is_attack_target = true;
			return WayFindingAndAttackMovementGenerator::Update(creature, diff);
		}
	}

	//不攻击或找不到要攻击的目标，则跟随主人
	float nx, ny;
	float range = creature.GetDistance(owner);
	float angle = creature.GetAngle(owner);

	float followdis = FOLLOW_DISTANCE;
	if (creature.GetFollowDis() != 0)
	{
		followdis = creature.GetFollowDis();
	}
			
	//如果距离不是很远，先按兵不动        并且不再障碍点里面。
	if(range < followdis && creature.GetMap()->GetMapTemp()->IsCanRun((uint16)creature.GetPositionX(), (uint16)creature.GetPositionY()))
	{
		if(creature.IsMoving() && !m_is_random_move)
			creature.StopMoving(true);

		//如果被跟随着不动，那么偶尔也要动一下，不然太差了
		if(m_canRandomMove && !owner->IsMoving() && !creature.IsMoving())
		{
			m_next_random_move.Update(diff);

			if(m_next_random_move.Passed())
			{
				RandomPos(nx, ny, &creature);
				if(creature.GetMap()->GetMapTemp()->IsCanRun(creature.GetPositionX(), creature.GetPositionY(), nx, ny))
				{
					creature.MoveTo(nx, ny);
					m_is_random_move = true;
				}
				m_next_random_move.Reset(urand(3000, 8000));
			}
		}
		return true;
	}
	else if(range > MAX_FOLLOW_DISTANCE)
	{
		//直接传送
		RandomPos(nx, ny, owner, TELEPORT_DISTANCE);
		creature.Relocate(nx, ny, angle);
		creature.StopMoving();
		m_is_random_move = false;
		return true;
	}
	m_is_random_move = false;
	creature.SetTarget(owner);
	m_is_attack_target = false;
	return WayFindingAndAttackMovementGenerator::Update(creature, diff);
}

Unit *MercenaryMovementGenerator::GetAttackTarget(Creature &creature,Unit *owner)
{
	if(!creature.GetMap() || !owner->GetLastVictim())
		return NULL;

	Unit *target = NULL;
	if (owner->GetMap())
		target = owner->GetMap()->FindUnit(owner->GetLastVictim());
	
	//若怪物消失
	if(!target)
	{
		owner->SetLastVictim(0);
		return NULL;
	}
	//若怪物死亡
	if(!target->isAlive())
	{
		owner->SetLastVictim(0);
		return NULL;
	}
	return target;
}

void MercenaryMovementGenerator::Finalize(Creature &creature)
{
	//回退 是否激活Grid 状态
	if(m_revert_active_grid)
		creature.SetCanActiveGrid(false);
}

//////////////////////////////////////////////////////////////////////////
//沿路线走动
void WayPointMovementGenerator::Initialize(Creature &creature, uint64 param, int64 t)
{
    if(!creature.isAlive())
        return;

    //设置路径
    wayid = static_cast<uint32>(param);
	const vector<mt_monsterline_t>& mt_path = creature.GetMap()->GetMapTemp()->m_monsterlines;
	if(mt_path.size() <= wayid || mt_path[wayid].path.empty())
	{
		tea_pwarn("path err");
	}
	else
	{
		if(t == 0)
			path = creature.GetMap()->GetMapTemp()->m_monsterlines[wayid].path;
		else
		{
			int size = mt_path[wayid].path.size();
			for (int i = mt_path[wayid].path.size() - 1; i >= 0; i --)
			{
				path.push_back(mt_path[wayid].path[i]);
			}
		}
	}
	point_index = 0;

    //开始运行准备
    /*
    mt_point m_target = path[point_index];
    point_index++;

    creature.MoveTo(float(m_target.pos_x), float(m_target.pos_y));*/

	if(!creature.CanActiveGrid())
	{
		creature.SetCanActiveGrid(true);
		m_revert_active_grid = true;
	}
}

void WayPointMovementGenerator::Reset(Creature &creature)
{
    Initialize(creature);
}
void WayPointMovementGenerator::Finalize(Creature &creature)
{
	if(m_revert_active_grid)
		creature.SetCanActiveGrid(false);
};
bool WayPointMovementGenerator::Update(Creature &creature, const uint32 &)
{
	if(path.size() == 0)
		return false;
    //如果被限制移动移动
    if(!creature.isCanMove())
    {
        if(creature.IsMoving())
            creature.StopMoving(true);

        return true;
    }

    //如果已经到达目标
    if(!creature.IsMoving())
    {
		if(point_index !=0 && creature.GetDistance(path[point_index-1].pos_x,path[point_index-1].pos_y) > 1.0f)
		{
			creature.MoveTo(float(path[point_index-1].pos_x), float(path[point_index-1].pos_y));
		}
		else if(point_index < path.size())
		{
			creature.MoveTo(float(path[point_index].pos_x), float(path[point_index].pos_y));
			point_index ++;
		}
		else//else if (point_index >= (path.size() + 1) >> 1)
		{
			//修改出生点位置，省得走回去
			creature.SetBornPos(path[point_index - 1].pos_x, path[point_index - 1].pos_y);
			//触发一下区域通知,不想在这里触发
			char name[50];
			memset(name, 0, 50);
			//sprintf(name, "%u", wayid);
			OnNotifyArea(creature.GetMap(), &creature, name, path[point_index - 1].pos_x, path[point_index - 1].pos_y);
			return false;		//走完路线,关掉吧
		}			
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
//坐标走动
class HomeMovementGenerator : public MovementGeneratorMedium<Creature, HomeMovementGenerator>
{
public:
    HomeMovementGenerator() {}
    void Initialize(Creature &creature, uint64 param = 0, int64 t = 0)
    {
        ASSERT(creature.isAlive())
    }
    void Reset(Creature &) {};
    bool Update(Creature &creature, const uint32 &)
    {
        if(!creature.isAlive())
            return false;

        //如果被限制移动移动
        if(!creature.isCanMove())
        {
            if(creature.IsMoving())
                creature.StopMoving();

            return true;
        }

        //如果已经停下来了
        if(creature.IsMoving())
        {
            return true;
        }
        else
        {
            float x, y;
            creature.GetBornPos(x, y);

            if(x != creature.GetPositionX() || y != creature.GetPositionY())
            {
//                tea_pwarn("不该出现的情况,未回到家就停下来了!");
                creature.MoveTo(x, y);
                return true;
            }
        }

       return false;
    }
    void Finalize(Creature &) {};

    MovementGeneratorType GetMovementGeneratorType()
    {
        return HOME_MOTION_TYPE;
    }
};

//////////////////////////////////////////////////////////////////////////
//for FollowInlineMovementGenerator

//死死的跟随走动
class FollowInlineMovementGenerator : public MercenaryMovementGenerator
{
public:
	FollowInlineMovementGenerator()
	{
		m_canAttack = false;
		m_canRandomMove = false;
	}
};					

//////////////////////////////////////////////////////////////////////////
//for FollowMovementGenerator

//跟随走动
class FollowMovementGenerator :public MercenaryMovementGenerator
{
public:
	FollowMovementGenerator()
	{
		m_canAttack = false;
		m_canRandomMove = true;
	}
};



////////////////////////////////////////////////////////////////////////////////////////////
//寻路走向指定地点
vector<float> WayFindingDstMovementGenerator::find_link;
WayFindingDstMovementGenerator::WayFindingDstMovementGenerator():targetPoxX(0), targetPoxY(0), find_type(WAYFINDINGTYPE_NONE), recallpos(0), basenumber(1)
{
}

void WayFindingDstMovementGenerator::Initialize(Creature &creature, uint64 param, int64 t)
{
	if(!creature.isAlive() || !creature.isCanMove())
		return;
	targetPoxX = (float)param;
	targetPoxY = (float)t;
}


void WayFindingDstMovementGenerator::Finalize(Creature &creature)
{
	creature.StopMoving();
}
bool WayFindingDstMovementGenerator::Update(Creature &creature, const uint32 &diff)
{
	if(!creature.isAlive() || !creature.isCanMove())
		return false;
	if(targetPoxX != creature.GetPositionX()
		|| targetPoxY != creature.GetPositionY()
		|| !creature.IsMoving())
	{

		return FindPath(creature);
	}
	return false;
}

MovementGeneratorType WayFindingDstMovementGenerator::GetMovementGeneratorType()
{
	return WAYFINDDST_MOTION_TYPE;
}

bool WayFindingDstMovementGenerator::FindPath(Creature &creature)
{
	if(!creature.isAlive() || !creature.isCanMove())
		return false;

	const MapTemplate *mt = creature.GetMap()->GetMapTemp();
	//float angle = creature.GetAngle(target);

	uint16 lastx = 0, lasty = 0, cnx = 0, cny = 0;

	//若沿路无障碍，当然OK
	if(mt->IsCanRun(creature.GetPositionX(), creature.GetPositionY(), targetPoxX, targetPoxY, lastx, lasty, cnx, cny, true))
	{
		//tea_pdebug("生物[%s] %f:%f 移动到 %f:%f", creature.GetName(), creature.GetPositionX(), creature.GetPositionY(), targetPoxX, targetPoxY);
		creature.MoveTo(targetPoxX, targetPoxY);
	}
	else
	{
		//若寻路最后一个无障碍坐标与怪物目前坐标一致或者干脆根本没发现无障碍坐标
		if((lastx == (uint16)creature.GetPositionX() && lasty == (uint16)creature.GetPositionY()) || (lastx == 0 && lasty == 0))
		{
			//tea_pdebug("【%s】遇到障碍，开始寻路。", creature.GetName());
			if(find_type == WAYFINDINGTYPE_NONE)
			{
				//干道寻路
				if(WayPointFindPath(creature, cnx, cny))
					return true;
				/*if(ContinuousFindPath(creature, target, cnx, cny))
					return;*/
			}
		}
		else
		{
			basenumber = 1;
			recallpos = 0;
			creature.MoveTo(lastx, lasty);
		}

		//tea_pdebug("生物[%s] %f:%f 移动到 %f:%f", creature.GetName(), creature.GetPositionX(), creature.GetPositionY(), targetPoxX, targetPoxY);

	}

	//creature.MoveTo(targetPoxX,targetPoxY,creature.GetAttackDistance(target));
	return true;
}

bool WayFindingDstMovementGenerator::WayPointFindPath(Creature &creature, uint16 cnx, uint16 cny)
{

	uint16 lastx = 0, lasty = 0;
	uint16 px = uint16(creature.GetPositionX()), py = uint16(creature.GetPositionY());
	const MapTemplate *mt = creature.GetMap()->GetMapTemp();
	uint32 point = px + mt->GetMapBaseInfo().width * py;
	uint32 point2 = 0;
	uint32 point_target;
	int me_size;
	int index_creature = -1, index_target = -1;
	const mt_point_in_line *me = mt->GetMonsterLineEach(point);
	if(me == NULL)
	{
		int8 index = MapTemplate::FindAroundIndex(cnx, cny, px, py);
		point2 = px + MapTemplate::around[(index - 1) % 8][0] + mt->GetMapBaseInfo().width * (py + MapTemplate::around[(index - 1) % 8][1]);
		me = mt->GetMonsterLineEach(point2);
		if(me == NULL)
		{
			point2 = px + MapTemplate::around[(index + 1) % 8][0] + mt->GetMapBaseInfo().width * (py + MapTemplate::around[(index + 1) % 8][1]);
			me = mt->GetMonsterLineEach(point2);
			if(me == NULL)
				return false;
		}
	}

	mt->IsCanRun(targetPoxX, targetPoxY, creature.GetPositionX(), creature.GetPositionY(), lastx, lasty, cnx, cny);
	int8 index = MapTemplate::FindAroundIndex(cnx, cny, lastx == 0 ? uint16(targetPoxX) : lastx, lasty == 0 ? uint16(targetPoxY) : lasty);
	if(index == -1)
	{
		//tea_pdebug("看来，玩家进入障碍了。");
		return false;
	}
	if(index % 2 == 0)
	{
		index = (index + 9) % 8;
		uint16 nlx = lastx + MapTemplate::around[index][0];
		uint16 nly = lasty + MapTemplate::around[index][1];
		if(mt->IsCanRun(nlx,nly))
		{
			lastx = nlx;
			lasty = nly;
		}
	}
	point_target = lastx + mt->GetMapBaseInfo().width * lasty;
	const mt_point_in_line *me_t = mt->GetMonsterLineEach(point_target);
	//不在同一个干道
	if(me_t == NULL || me_t->each != me->each)
	{
		tea_pdebug("不在同一个干道。从%d,%d到%d,%d", py, px, (uint16)targetPoxX, (uint16)targetPoxY);
		return false;
	}

	//tea_pdebug("进入干道");
	me_size = me->each->point_each.size();
	index_target = me_t->index;
	index_creature = me->index;

	if(index_creature == -1 || index_target == -1 || index_creature == index_target)
	{
		tea_perror("干道点没画好。从%d,%d到%d,%d", py, px, (uint16)targetPoxX, (uint16)targetPoxY);
		return false;
	}

	//1顺向，2逆向
	uint8 direction = index_target > index_creature ? 1 : 2;
	if(me->each->is_join)
	{
		if(direction == 1)
			direction = index_target - index_creature < index_creature + me_size - index_target ? 1 : 2;
		else
			direction = index_creature - index_target> index_target + me_size - index_creature ? 1 : 2;
	}

	int max_i = (direction == 1 ? index_target + me_size - index_creature : index_creature + me_size - index_target) % me_size;		
	int i = point2 == 0 ? 0 : 1;
	if(point2 != 0)
	{
		find_link[0] = float(point2 % mt->GetMapBaseInfo().width);
		find_link[1] = float(point2 / mt->GetMapBaseInfo().width);
		max_i++;
	}
	int max_size = (max_i + 1) * 2;
	uint32 array_len = 0;
	index_creature += me_size;
	if(find_link.size() < uint32(max_size))
		find_link.resize(uint32(max_size));
	do
	{
		if(direction == 1)
			index_creature++;
		else
			index_creature--;
		uint32 index = index_creature % me_size;
		mt_monsterline_each *each = NULL;
		list<mt_point*>::iterator list_it = me->each->point_each.begin();
		for (uint32 t_i = 0; t_i < index; ++list_it, t_i++);			
		find_link[i * 2] = (*list_it)->pos_x;
		find_link[i * 2 + 1] = (*list_it)->pos_y;
		array_len += 2;
		if(mt->IsCanRun(find_link[i * 2], find_link[i * 2 + 1], targetPoxX, targetPoxY))
		{
			i++;
			find_link[i * 2] = targetPoxX;
			find_link[i * 2 + 1] = targetPoxY;
			array_len += 2;
			break;
		}
		i++;
		if(i > 50)
		{
			//tea_pdebug("寻路超过50步，放弃。");
			return false;
		}
	}
	while(i < max_i);

	for (uint32 fi = 0; fi < array_len; fi+=2)
	{
		if(fi > 2)
		{
			if(!creature.GetMap()->GetMapTemp()->IsCanRun(find_link[fi-2],find_link[fi-1],find_link[fi],find_link[fi+1]))
			{
				tea_perror("way finding err map:%u line no:%u, creature X %u, creature Y %u to target X %u,target Y %u",
					creature.GetMapId(), me->index, px, py, (uint16)targetPoxX, (uint16)targetPoxY); 
#ifdef WIN32
ASSERT(false);
#endif //windows下面的测试
				return false;
			}
		}
	}

	creature.MoveTo(array_len, &find_link[0]);

	find_type = WAYFINDINGTYPE_WAYPOINT;
	return true;
}
	
//连续寻路
bool WayFindingDstMovementGenerator::ContinuousFindPath(Creature &creature, uint16 cnx, uint16 cny)
{
	uint16 find_number = 20;
	uint16 c_r_x, c_r_y, c_l_x, c_l_y;
	uint16 last_l_x = cnx, last_l_y = cny, last_r_x = cnx, last_r_y = cny;
	int find_index = 0;
	int left_number = 0, right_number = 0;
	bool left = false, right = false;
	int i;

	c_r_x = (uint16)creature.GetPositionX();
	c_r_y = (uint16)creature.GetPositionY();
	c_l_x = c_r_x;
	c_l_y = c_r_y;
	for(; find_index < find_number; find_index++)
	{
		//探寻这一步从那一个朝向开始寻路
		int8 p_l = MapTemplate::FindAroundIndex(last_l_x, last_l_y, c_l_x, c_l_y);
		int8 p_r = MapTemplate::FindAroundIndex(last_r_x, last_r_y, c_r_x, c_r_y);
		if(find_index != 0)
		{
			//左右置一下朝向，因为FindAroundIndex是反向计算的
			p_l -= 2;
			p_r += 2;
		}
		bool left_find = false, right_find = false;

		//保存上一步
		last_r_x = c_r_x;
		last_r_y = c_r_y;
		last_l_x = c_l_x;
		last_l_y = c_l_y;

		for(i = 1; i < 8; i++)
		{
			p_r ++;
			//尝试通过
			if(!right_find && TestGo(creature, c_r_x, c_r_y, p_r, right))
			{
				find_link[find_index * 2] = c_r_x;
				find_link[find_index * 2 + 1] = c_r_y;
				if(right) break; //寻到直达的路了
				right_find = true;//本次寻路成功，但是只限于这一步
			}
			p_l --;
			//尝试通过
			if(!left_find && TestGo(creature, c_l_x, c_l_y, p_l, left))
			{
				find_link[(find_index + find_number) * 2] = c_l_x;
				find_link[(find_index + find_number) * 2 + 1] = c_l_y;
				if(left) break;//寻到直达的路了
				left_find = true;//本次寻路成功，但是只限于这一步
			}
			if(right_find && left_find)
				break;//左右都寻到了
			if(i == 7)
			{
				//tea_perror("寻路有问题，一圈都没出口，明显是错的,或者怪物进入障碍，然后重新寻路。");
				//处理
				return false;
			}
		}
		if (right || left)
		{
			if(right)
				creature.MoveTo((find_index + 1) * 2, &find_link[0]);
			else
				creature.MoveTo((find_index + 1) * 2, &find_link[find_number * 2]);
			find_type = WAYFINDINGTYPE_CONTINUOUS;
			//tea_pdebug("连续寻路OK");
			return true;
		}
	}
	return false;
}

//尝试通过
bool WayFindingDstMovementGenerator::TestGo(Creature &creature, uint16 &poxX, uint16 &poxY, uint16 p, bool &complete)
{
	complete = false;
	if(creature.GetMap()->GetMapTemp()->IsCanRun(poxX, poxY, targetPoxX, targetPoxY))
	{
		poxX = (uint16)targetPoxX;
		poxY = (uint16)targetPoxY;
		complete = true;
		return true;
	}

	uint16 index = p & 7;
	uint16 tx, ty;
	tx = uint16(poxX + MapTemplate::around[index][0] * basenumber);
	ty = uint16(poxY + MapTemplate::around[index][1] * basenumber);

	if(creature.GetMap()->GetMapTemp()->IsCanRun(poxX, poxY, tx, ty, true))
	{
		poxX = tx;
		poxY = ty;
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
//抽象工厂

namespace Tea
{
	//INSTANTIATE_SINGLETON_1(MovementGeneratorRegistry);
    //template class Tea::Singleton<MovementGeneratorRegistry>; 
	template<> MovementGeneratorRegistry * Tea::Singleton<MovementGeneratorRegistry  >::si_instance = 0; 
	template<> bool Tea::Singleton<MovementGeneratorRegistry >::si_destroyed = false;

	MovementGenerator* SelectMovementGenerator(Creature *creature)
	{
		MovementGeneratorRegistry &mv_registry(MovementGeneratorRepository::Instance());
		//assert(creature->GetCreatureInfo() != NULL);
		//MovementGeneratorType type = creature->GetTemplate().

		//return new RandomMovementGenerator();

		const MovementGeneratorCreator *mv_factory = mv_registry.GetRegistryItem((MovementGeneratorType)creature->GetMoveType());
		return (mv_factory == NULL ? NULL : mv_factory->Create(creature));
	}

	MovementGenerator* SelectMovementGenerator(Creature *creature, MovementGeneratorType type, int param1)
	{
		MovementGeneratorRegistry &mv_registry(MovementGeneratorRepository::Instance());

		const MovementGeneratorCreator *mv_factory = mv_registry.GetRegistryItem(type);
		return (mv_factory == NULL ? NULL : mv_factory->Create(creature));
	}

	void InitializeMovement()
	{
		(new MovementGeneratorFactory<DisappearMovementGenerator>(DISAPPEAR_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<DeadlineMovementGenerator>(DEADLINE_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<IdleMovementGenerator>(IDLE_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<RandomMovementGenerator>(RANDOM_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<WayPointMovementGenerator>(WAYPOINT_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<HomeMovementGenerator>(HOME_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<FollowMovementGenerator>(FOLLOW_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<FollowInlineMovementGenerator>(FOLLOW_INLINE_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<WayFindingMovementGenerator>(WAYFINDING_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<WayFindingAndAttackMovementGenerator>(WAYFINDING_ATTACK_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<MercenaryMovementGenerator>(MERCENARY_MOTION_TYPE))->RegisterSelf();
		(new MovementGeneratorFactory<WayFindingDstMovementGenerator>(WAYFINDDST_MOTION_TYPE))->RegisterSelf();

		//反正是初始化，随便借放一下功能。
		//初始化寻路数组
		WayFindingMovementGenerator::find_link.resize(200, 0);
		WayFindingDstMovementGenerator::find_link.resize(200, 0);
	}
}
