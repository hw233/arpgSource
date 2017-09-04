#include "PathFinding.h"
#include "robotd_app.h"
#include "robotd_unit.h"
#include <shared/offset_path.h>
#include "robotd_context.h"

using namespace std;

const int MaxDistance = 9999;

vector<StarNode*> StarNode::AllNode;

StarNode::StarNode():main_road_point(nullptr), x(0), y(0), g(0), h(0), f(0), father(nullptr), child(nullptr)
{
	AllNode.push_back(this);
}

void StarNode::FreeNode()
{
	for (auto it:AllNode)
	{
		safe_delete(it);
	}
	AllNode.clear();
}

PathFinding::PathFinding()
	:m_map(NULL),m_startX(0),m_startY(0),m_endX(0),m_endY(0),m_start_node(NULL),m_end_node(NULL),m_steps(0)
{

}

PathFinding::~PathFinding()
{
	FreeNode();
}

void PathFinding::SetMapTemplate(uint32 mapid)
{
	const MapTemplate *templ = MapTemplate::GetMapTempalte(mapid);
	ASSERT(templ);
	m_map = templ;
}

//设置开始点和结束点
void PathFinding::SetStartEndPoint(uint16 startX, uint16 startY, uint16 endX, uint16 endY)
{
	m_startX = startX;
	m_startY = startY;
	m_endX = endX;
	m_endY = endY;

	m_start_node = new StarNode;
	m_start_node->x = m_startX;
	m_start_node->y = m_startY;
	m_start_node->g = 0;
	m_start_node->h = judge(m_start_node->x,m_start_node->y);
	m_start_node->f = m_start_node->g + m_start_node->h;

	m_end_node = new StarNode;
	m_end_node->x = m_endX;
	m_end_node->y = m_endY;
}

int PathFinding::AstarCalculate(StarNode *node, MainRoadPoint *point)
{
	uint16 nextX = point->pos_x;
	uint16 nextY = point->pos_y;
	if(!m_map->IsCanRun(node->x, node->y, nextX, nextY))
	{
		return 0;
	}
	//计算此子节点的g值
	int newGVal = node->g + GetDistance(point->pos_x, point->pos_y, nextX, nextY);

	//搜索OPEN表，判断此点是否在OPEN表中
	vector<StarNode*>::iterator open_table_result;
	for (open_table_result = m_open_table.begin();
		open_table_result != m_open_table.end();++open_table_result)
	{
		if ((*open_table_result)->x == nextX &&
			(*open_table_result)->y == nextY)
		{
			break;
		}
	}

	//此子节点已经存在于OPEN表中
	if (open_table_result != m_open_table.end())
	{
		//OPEN表中节点的g值已经是最优的，则跳过此节点
		if ((*open_table_result)->g <= newGVal)
		{
			return 0;
		}
	}

	//搜索CLOSED表，判断此节点是否已经存在于其中
	vector<StarNode*>::iterator m_close_tableResult;
	for (m_close_tableResult = m_close_table.begin();
		m_close_tableResult != m_close_table.end();++m_close_tableResult)
	{
		if ((*m_close_tableResult)->x == nextX &&
			(*m_close_tableResult)->y == nextY)
		{
			break;
		}
	}

	//此节点已经存在于CLOSED表中
	if (m_close_tableResult != m_close_table.end())
	{
		//CLOSED表中的节点已经是最优的，则跳过
		if ((*m_close_tableResult)->g <= newGVal)
		{
			return 0;
		}
	}

	//此节点是迄今为止的最优节点
	StarNode *bestNode = new StarNode;
	bestNode->main_road_point = point;
	bestNode->x = nextX;
	bestNode->y = nextY;
	bestNode->father = node;
	bestNode->g = newGVal;
	bestNode->h = judge(nextX,nextY);
	bestNode->f = bestNode->g + bestNode->h;

	//如果已经存在于CLOSED表中，将其移除
	if (m_close_tableResult != m_close_table.end())
	{
		m_close_table.erase(m_close_tableResult);
	}

	//如果已经存在于OPEN表，更新
	if (open_table_result != m_open_table.end())
	{
		m_open_table.erase(open_table_result);

		//重新建堆，实现排序。注意不能用sort_heap，因为如果容器为空的话会出现bug
		make_heap(m_open_table.begin(),m_open_table.end(),HeapCompare_f());
	}

	m_open_table.push_back(bestNode);//将最优节点放入OPEN表

	push_heap(m_open_table.begin(),m_open_table.end(),HeapCompare_f());//重新排序

	return 0;
}

//A星干道
bool PathFinding::AstarMainRoad()
{
	m_open_table.push_back(m_start_node);//起始点装入OPEN表

	//对vector中元素进行排序：将最后一个元素加入原本已序的heap内
	push_heap(m_open_table.begin(),m_open_table.end(),HeapCompare_f());

	StarNode *tempNode = nullptr;

	//开始遍历
	while (true)
	{
		if (m_open_table.empty())//判断OPEN表是否为空
		{
			return false;
		}

		tempNode = m_open_table.front();//注意：OPEN表为空会导致未定义行为
		++m_steps;
		//将第一个元素移到最后，并将剩余区间重新排序，组成新的heap
		pop_heap(m_open_table.begin(),m_open_table.end(),HeapCompare_f());
		m_open_table.pop_back();//删除最后一个元素

		//判断是否已经搜寻到目标节点
		if(m_map->IsCanRun(tempNode->x, tempNode->y, m_endX, m_endY))
		{
			m_end_node->father = tempNode;
			generatePath();
			return true;
		}

		if(tempNode == m_start_node)
		{
			for (auto it:m_map->m_main_road)
			{
				AstarCalculate(tempNode, it.second);
			}
		}
		else
		{
			MainRoadPoint *point = tempNode->main_road_point;
			for (auto it:point->next_ps)//针对每个子节点
			{
				if(AstarCalculate(tempNode, it))
					break;
			}
		}
		m_close_table.push_back(tempNode);
	}
}

//寻路
bool PathFinding::Finding()
{
	//判断起始点和目标点是否是同一点
	if (!(m_startX != m_endX || m_startY != m_endY))
	{
		return false;
	}

	m_steps = 0;
	m_path.clear();

	bool result = false;
	if(m_map->IsCanRun(m_startX, m_startY, m_endX, m_endY))
	{
		m_path.push_back(m_endX);
		m_path.push_back(m_endY);
		result = true;
	}
	else
	{
		result = AstarMainRoad();
		FreeNode();
	}
	return result;
}

bool PathFinding::pathFinding(uint32 mapid, uint16 startX, uint16 startY, uint16 endX, uint16 endY)
{
	SetMapTemplate(mapid);

	////缓存里有就用缓存的
	//sprintf(s_key, "%4u%4u%4u%4u%4u", m_map->GetMapBaseInfo().mapid, startX, startY, endX, endY);
	//map<string, vector<float>>::iterator path_it = m_path_data.find(s_key);
	//if(path_it != m_path_data.end())
	//{
	//	m_path = path_it->second;
	//	return !m_path.empty();
	//}

	//终点是否可到达(掩码)
	if(!m_map->IsCanRun(endX, endY))
	{
		tea_pdebug("error: astar find path fail. end can't run, mapid = %u, start %u %u, end %u %u!", mapid, startX, startY, endX, endY);
		return false;
	}

	bool result = false;
	
	if(m_map->IsCanRun(startX, startY))
	{
		m_startX = startX;
		m_startY = startY;
		SetStartEndPoint(m_startX, m_startY, endX, endY);
		result = Finding();
	}
	else
	{
		//如果起始点是障碍点，那就在周边找一个不是障碍点作为起始点
		tea_pdebug("PathFinding::pathFinding !m_map->IsCanRun(startX, startY) %u %u %u", mapid, startX, startY);
		for (int i = 0; i < 8; i++)
		{
			m_startX = startX + MapTemplate::around[i][0];
			m_startY = startY + MapTemplate::around[i][1];
			if(m_map->IsCanRun(m_startX, m_startY))
			{
				SetStartEndPoint(m_startX, m_startY, endX, endY);
				result = Finding();
				if(result)
					break;
			}
		}
	}

	if (result)
	{
		ASSERT(!m_path.empty());
		//因为起始点是障碍点，寻路是找周边的非障碍点作为起始点。
		if(m_startX != startX || m_startY != startY)
		{
			m_path.insert(m_path.begin(), m_startY);
			m_path.insert(m_path.begin(), m_startX);
		}
		//m_path_data[s_key] = m_path;
	}
	else
	{
		tea_pdebug("error: astar find path fail. mapid = %u, start %u %u %u %u, end %u %u!", mapid, startX, startY, m_startX, m_startY, endX, endY);
	}

	return result;
}

void PathFinding::generatePath()
{
	//先把倒序的路径弄正。
	StarNode *node = m_end_node;
	uint32 nodesize = 1;
	do 
	{
		node->father->child = node;
		node = node->father;	
		++nodesize;
	} while (node != m_start_node);

	//把路径填入vector
	m_path.clear();
	node = m_start_node;
#if 1
	////压入起点
	//m_path.push_back((float)node->x);
	//m_path.push_back((float)node->y);
	if (nodesize > 2)
	{
		while (node != m_end_node)
		{
			uint16 x = node->x, y = node->y, lastX = node->x, lastY = node->y;			
			StarNode *temp_node = node->child;
			if (temp_node != m_end_node)
			{
				GetCangoPoint(x, y, temp_node);	
				if(lastX == x && lastY == y)
				{
					//蹦这说明找出来的路径有问题
					ASSERT(lastX != x || lastY != y);			
				}
				else
				{
					m_path.push_back((float)x);
					m_path.push_back((float)y);
					node = temp_node->father;	//回到上一个不可达的节点				
				}	
			}
			else
			{
				break;
			}
		}
	}	
	//压入终点
	m_path.push_back((float)m_end_node->x);
	m_path.push_back((float)m_end_node->y);
#else
	//压入起点
	while (node != m_end_node)
	{
		m_path.push_back((float)node->x);
		m_path.push_back((float)node->y);
		node = node->child;	
	}
	//压入终点
	m_path.push_back((float)m_end_node->x);
	m_path.push_back((float)m_end_node->y);	
#endif
	ASSERT(m_map->IsCanRun(m_startX,m_startY,m_path));
}

//获得可达坐标点
void PathFinding::GetCangoPoint(uint16 &startx, uint16 &starty, StarNode *&next_node)
{
	uint16 lastX = startx, lastY = starty;
	while (next_node != m_end_node)
	{
		if (m_map->IsCanRun((float)startx, (float)starty, (float)next_node->x, (float)next_node->y))
		{
			lastX = next_node->x;
			lastY = next_node->y;
			next_node =  next_node->child;
		}
		else
		{
			break;
		}
	}
	startx = lastX;
	starty = lastY;	
}

void PathFinding::FreeNode()
{
	m_open_table.clear();
	m_close_table.clear();
	StarNode::FreeNode();
}

int PathFinding::judge(int x, int y)
{
	return (10 * (abs(m_endX - x) + abs(m_endY - y)));
}

int PathFinding::LuaPathfindingGoto(lua_State* scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 4);
	RobotdContext *context = (RobotdContext*)LUA_TOUSERDATA(scriptL, 1, ObjectTypePlayer);
	RobotdUnit *unit = (RobotdUnit*)LUA_TOUSERDATA(scriptL, 2, ObjectTypeUnit);
	uint16 to_x = (uint16)LUA_TOINTEGER(scriptL, 3);
	uint16 to_y = (uint16)LUA_TOINTEGER(scriptL, 4);

	ASSERT(unit);

	static PathFinding pf;
	tea_pdebug("%s LuaPathfindingGoto from (%d, %d) to (%d, %d)", unit->GetGuid().c_str(), (uint16)unit->GetPosX(), (uint16)unit->GetPosY(), to_x, to_y);
	bool result = pf.pathFinding(unit->GetMapId(), (uint16)unit->GetPosX(), (uint16)unit->GetPosY(), to_x, to_y);
	
	if(!result)
	{
		lua_pushboolean(scriptL, FALSE);
		return 1;
	}

	static offset_path ofpath;
	ofpath.path_.clear();
	ofpath.set_origin((uint16)unit->GetPosX(), (uint16)unit->GetPosY());
#if 0		
	for (int i = 0; i < (int)m_moving_path.size(); i = i + 2)
		ofpath._add(int(m_moving_path[i]), int(m_moving_path[i+1]));
#else
	std::deque<float> p;
	for (auto it:pf.m_path)
	{
		p.push_back(it);
	}

	if(!CreateOffsetPath(ofpath.origin_x_,ofpath.origin_y_,p,ofpath.path_))
	{
		ASSERT(false);
		return 0;
	}
#endif
	//vector<char> *__path = nullptr;
#if 1
	//vector<char> temp_path;
	//temp_path.push_back(ofpath.path_[0]);
	//temp_path.push_back(ofpath.path_[1]);
	context->send_unit_move(unit->GetUIntGuid(), ofpath.origin_x_, ofpath.origin_y_, ofpath.path_);
	//__path = &pf.m_path;
#else
	context->send_unit_move(unit->GetUIntGuid(), ofpath.origin_x_, ofpath.origin_y_, ofpath.path_);
	__path = &ofpath.path_;
#endif
	unit->StopMoving((uint16)unit->GetPosX(), (uint16)unit->GetPosY());
	for (auto it : p)
	{
		unit->m_moving_path.push_back(it);
	}
	unit->StartMoving(true, "LuaPathfindingGoto");
	lua_pushboolean(scriptL, TRUE);
	return 1;
}

//能否通行
int PathFinding::LuaIsCanRun(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	uint32 mapid = (uint32)LUA_TOINTEGER(scriptL,1);
	const MapTemplate *mt = NULL;
	mt = MapTemplate::GetMapTempalte(mapid);
	float x = (float)LUA_TONUMBER(scriptL,2);
	float y = (float)LUA_TONUMBER(scriptL,3);
	if (!mt)
	{
		lua_pushboolean(scriptL, FALSE);
		return 1;
	}
	lua_pushboolean(scriptL, mt->IsCanRun(uint16(x),uint16(y)) ? TRUE : FALSE);
	return 1;
}

//随机
int PathFinding::LuaRandomPos(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);

	const MapTemplate *mt = NULL;

	uint32 mapid = (uint32)LUA_TOINTEGER(scriptL,1);
	mt = MapTemplate::GetMapTempalte(mapid);

	if(!mt)
	{
		tea_perror("LuaRandomPos map is NULL");
		return 0;
	}

	//需要随机的点数量
	int pos_num = 1;
	if(n > 1)
		pos_num = (int)LUA_TOINTEGER(scriptL,2);	

	//在此坐标附近
	bool need_check_pos = (n >= 5);
	uint16 fPosX,fPosY,fDist;
	if(need_check_pos)
	{
		fPosX = (uint16)LUA_TONUMBER(scriptL,3);
		fPosY = (uint16)LUA_TONUMBER(scriptL,4);
		fDist = (uint16)LUA_TONUMBER(scriptL,5);		
	}else ASSERT(0);


	vector<point_t> points;

	point_t point;
	for (int dist = fDist; dist > 0; dist--)
	{
		for(int angle = 0; angle < 628; angle += 2)
		{

			point.pos_x = uint16(fPosX + fDist * cos(angle/100.0f));
			point.pos_y = uint16(fPosY + fDist * sin(angle/100.0f));

			if (mt->IsCanRun(uint16(point.pos_x),uint16(point.pos_y)))			
				points.push_back(point);
		}

		if (!points.empty())
		{
			break;
		}
	}

	if (points.empty() || int(points.size()) < pos_num)
	{
		for(int i=0; i < pos_num - int(points.size()); i++)
		{
			point.pos_x = fPosX;
			point.pos_y = fPosY;
			points.push_back(point);
		}
	}

	std::random_shuffle(points.begin(), points.end());

	int max_pos = int(points.size()) > pos_num ? pos_num : int(points.size());
	lua_newtable(scriptL);    /* We will pass a table */
	for(int i=0; i < max_pos; i++)
	{
		lua_pushnumber(scriptL, i+1);   /* Push the table index */

		lua_newtable(scriptL);
		lua_pushnumber(scriptL, points[i].pos_x);
		lua_setfield(scriptL, -2, "x");
		lua_pushnumber(scriptL, points[i].pos_y);
		lua_setfield(scriptL, -2, "y");              

		lua_rawset(scriptL, -3); 
	}

	return 1;
}
