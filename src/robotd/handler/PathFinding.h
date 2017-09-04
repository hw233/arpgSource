#ifndef _A_STAR_PATH_FNDING_
#define _A_STAR_PATH_FNDING_

#include <svrcore.h>
#include <stack>
#include <vector>
#include <shared/map_template.h>

//迷宫地图中节点类型标记
enum{
	NODE_EMPTY,//可以通过的节点
	NODE_OBSTACLE,//障碍物，不可通过
	NODE_PATH//路径上的点
};

//记录路径上的点的坐标
typedef struct tagpathNode{
	int x,y;
}PathNode;

//节点数据结构定义
class StarNode
{
public:
	StarNode();
	~StarNode(){}
public:
	static vector<StarNode*> AllNode;
	static void FreeNode();
public:
	MainRoadPoint *main_road_point;//干道点
	uint16 x,y;//当前点在迷宫中的位置坐标
	int g;//起始点到当前点实际代价
	int h;//当前节点到目标节点最佳路径的估计代价
	int f;//估计函数：f = g + h。
	StarNode *father;//指向其父节点的指针
	StarNode *child;//指向其父节点的指针
};

//定义STL优先队列的排序方式
class HeapCompare_f{
public:
	bool operator()(StarNode* x,StarNode* y) const
	{
		return x->f > y->f;//依据估价函数进行排序：升序排列
	}
};

//迷宫寻路：A*算法
class PathFinding{
public:
	PathFinding();
	~PathFinding();
private:
	const MapTemplate *m_map;
	uint16 m_startX,m_startY;//起始点坐标
	uint16 m_endX,m_endY;//目标点坐标

	uint32 point_start,	dis_start,	point_end, dis_end;//干道寻路用的变量
	set<uint32> m_point_set;

	char s_key[30];

	StarNode *m_start_node,*m_end_node,*m_main_start_node,*m_main_end_node;//起始节点和目标节点
	int m_steps;//搜索所花费的总步数

	//OPEN表：采用C++ STL中vector实现优先级队列功能
	//注意：存储的是StarNode*指针
	std::vector<StarNode*> m_open_table;
	//CLOSED表：存储的也是StarNode*指针
	std::vector<StarNode*> m_close_table;
	map<string, std::vector<float>> m_path_data;
private:
	//设置地图模版
	void SetMapTemplate(uint32 mapid);
	//设置开始点和结束点
	void SetStartEndPoint(uint16 startX, uint16 startY, uint16 endX, uint16 endY);
	//产生路径信息
	void generatePath();
	//估计当前点到目标点的距离：曼哈顿距离
	int judge(int x,int y);
	//释放
	void FreeNode();
	//获得可达坐标点
	void GetCangoPoint(uint16 &startx, uint16 &starty, StarNode *&next_node);
	//A星干道
	bool AstarMainRoad();
	//A星计算两点的值
	int AstarCalculate(StarNode *node, MainRoadPoint *point);
	//寻路
	bool Finding();
public:
	vector<float> m_path;
public:
	//寻路主函数
	bool pathFinding(uint32 mapid, uint16 startX, uint16 startY, uint16 endX, uint16 endY);
public:
	static int LuaPathfindingGoto(lua_State* scriptL);	//寻路前往
	static int LuaRandomPos(lua_State* scriptL);		//获取随机坐标
	static int LuaIsCanRun(lua_State *scriptL);			//判断坐标是否合法	
};


#endif //_A_STAR_PATH_FNDING_
