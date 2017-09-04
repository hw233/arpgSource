#ifndef _MAP_TEMPLATE_H_
#define _MAP_TEMPLATE_H_

#include "svrcore-internal.h"
#include <gtest/gtest.h>

using std::istream;

enum cell_flags
{
	CELL_FLAGS_OBSTACLE		    = 0X001,			/*是否障碍,  00000001*/
	CELL_FLAGS_TRANS		    = 0X002,			/*是否半透明,00000010*/
	CELL_FLAGS_TRANPORT		    = 0X004,			/*是否传送点 00000100*/
	CELL_FLAGS_MUSIC		    = 0X008,			/*是否声源点 00001000*/
	/*CELL_FLAGS_MOVIE		    = 0X010,*/			/*是否动态点 00010000*/
	CELL_FLAGS_RAISED			= 0X010,			/*是否复活点 00010000*/
	CELL_FLAGS_NPC			    = 0X020,			/*是否NPC点  00100000*/
	CELL_FLAGS_MONSTER		    = 0X040,			/*是否怪物点 01000000*/
	CELL_FLAGS_GAMEOBJECT		= 0X080,			/*是否其他游戏对象点 10000000*/

};

enum MapInstanceType
{
	MAP_INST_TYP_NO_INSTANCE	= 0,		//不是副本
	MAP_INST_TYP_ACTIVITY		= 1,		//活动副本,虽是副本地图,但是,只有单例,活动时间自动关闭
	MAP_INST_TYP_SINGLETON		= 2,		//单人副本，副本完成直接关闭
	MAP_INST_TYP_KUAFU			= 3,		//跨服地图
};

/*地图基本信息*/
typedef struct mt_baseinfo_t
{
	uint32	mapid;					/*地图ID*/
	char	name[255];				/*地图名称*/
	char	create_date[16];		/*地图创建时间*/
	uint32	px_width;				/*像素宽*/
	uint32	px_height;				/*像素高*/
	uint32	unit_width;				/*瓷砖宽*/
	uint32	unit_height;			/*瓷砖高*/
	uint16	width;					/*逻辑宽*/
	uint16	height;					/*逻辑高*/
	uint32	is_instance;			/*是否副本地图*/
	uint32  parentID;               /*父级地图ID*/
	char	music[255];		        /*场景音乐*/
	uint32  shadow;                 /*影子方向*/
	uint32	count;					/*副本人数*/
	uint32	day_limit;				/*日限制*/
	uint32	week_limit;				/*周限制*/
	uint16  instance_type;			/*副本类型 from enum MapInstanceType*/
} mt_baseinfo;

//传送点
typedef struct mt_teleport_t
{
	uint16 x;						//X轴坐标
	uint16 y;						//Y轴坐标
	uint32 temp_id;					//模板id
	char name[50];					//传送点名称
	uint32 to_mapid;				//地图ID
	uint16 to_x;
	uint16 to_y;
} mt_teleport;

//复活点
typedef struct mt_raised_t
{
	uint16 x;						//X轴坐标
	uint16 y;						//Y轴坐标
	uint16 faction;					//阵营
} mt_raised;

//背景音乐信息
typedef struct mt_music_t
{
	uint32 x;
	uint32 y;
	char   file[100];
} mt_music;

//背景浮动层
typedef struct mt_trans_t
{
	uint32 x;
	uint32 y;
	char  file[100];
	uint32 width;
	uint32 height;
	uint32 alpha;
} mt_trans;

//怪物刷新点
typedef struct mt_monsters_t
{
	uint32 templateid;					//ID
	float x;							//X轴坐标
	float y;							//Y轴坐标
	uint32 count;						//数量
	uint32 respawn_time;				//重生时间
	uint32 spawn_type;					//0死亡后刷新，1定时刷新
	uint32 spawn_time1;					//刷新时间1
	uint32 spawn_time2;					//刷新时间2
	uint32 spawn_time3;					//刷新时间3
	char   script_name[50];				//脚本
	uint32 move_type;					//移动类别
	uint32 line_id;                     //线路id 
	uint32 flag;                        //npc标识
	float toward;						//npc朝向
	char  alias_name[50];				//别名,用于副本AI脚本查找对象
} mt_monsters;

//游戏对象刷新点
typedef struct mt_gameobject_t
{
	uint32 templateid;					//ID
	float x;							//X轴坐标
	float y;							//Y轴坐标
	uint32 count;						//数量
	uint32 respawn_time;				//重生时间
	uint32 spawn_type;					//0死亡后刷新，1定时刷新
	uint32 spawn_time1;					//刷新时间1
	uint32 spawn_time2;					//刷新时间2
	uint32 spawn_time3;					//刷新时间3
	char   script_name[50];				//脚本
	float  toward;						//朝向
	char  alias_name[50];				//别名

} mt_gameobject;

typedef struct mt_point_t
{
	uint16 pos_x, pos_y;
	uint32 point;
} mt_point;

//怪物路线
typedef struct mt_monsterline_t
{
	uint32 id;                  //ID
	vector<mt_point> path;      //路点集合
} mt_monsterline;

//路障周边路线
typedef struct mt_monsterline_each_t
{
	list<mt_point*> point_each;		//所有点的集合
	bool is_join;					//是否首尾相连
} mt_monsterline_each;

typedef struct mt_point_in_line_t
{
	mt_monsterline_each *each;
	int index;
	uint32 point;
} mt_point_in_line;

//主干道节点
class MainRoadPoint
{
public:
	MainRoadPoint():pos_x(0),pos_y(0),point(0){}
	~MainRoadPoint(){}
public:
	uint16 pos_x;
	uint16 pos_y;
	uint32 point;
	vector<MainRoadPoint*> next_ps;	//我的所有子节点
};


//地图实例保留时间 秒
#define INSTANCE_DEL_TIME		600

//最大的单元格子数
const int MAX_MAP_CELLS_MASK = 400 * 400/32;

//计算两点之间距离
template<typename T>
inline T GetDistance(T fromX,T fromY,T toX,T toY)
{
	float dx = static_cast<float>(toX) - static_cast<float>(fromX);
	float dy = static_cast<float>(toY) - static_cast<float>(fromY);
	return (T)sqrt(dx*dx + dy*dy);
}

//地图模版
class TEA_SVRCORE_SPEC MapTemplate
{
	friend class Map;
	//静态
public:
	FRIEND_TEST(MapTemplate, ReadToken);
	FRIEND_TEST(MapTemplate, ReadToken2);
	FRIEND_TEST(MapTemplate, LoadBaseinfo);
	FRIEND_TEST(MapTemplate, LoadObstacle);
	FRIEND_TEST(MapTemplate, _LoadTrans);
	FRIEND_TEST(MapTemplate, _LoadRaised);
	FRIEND_TEST(MapTemplate, LoadTeleport);
	FRIEND_TEST(MapTemplate, _LoadTrunk);
	FRIEND_TEST(MapTemplate, _LoadMusic);
	FRIEND_TEST(MapTemplate, _LoadLight);
	FRIEND_TEST(MapTemplate, _LoadMovie);
	FRIEND_TEST(MapTemplate, _LoadNPC);
	FRIEND_TEST(MapTemplate, LoadCreatrue);
	FRIEND_TEST(MapTemplate, LoadGameObject);
	FRIEND_TEST(MapTemplate, LoadMonsterLine);
	FRIEND_TEST(MapTemplate, IsCanRun);
	FRIEND_TEST(MapTemplate, _LoadInving);

	typedef vector<MapTemplate*> MTVector;
	static MTVector mtvec;					//所有地图模版
	static uint32 m_map_version;			//地图版本号
	static float safe_distance;			//安全区域半径,默认为6

	static bool _ls(string folder, string ext, vector<string>& files);	//遍历指定文件夹
	static bool Load_Version(string folder);
	static bool Load(string folder,bool only_baseinfo=false);
	static void UnLoad()
	{
		for_each(mtvec.begin(), mtvec.end(), safe_delete);
		mtvec.clear();
	}

	static const MapTemplate* GetMapTempalte(uint32 mapid)
	{
		if(mtvec.size() <= mapid)
		{
			return NULL;
		}

		return mtvec[mapid];
	}

	//八方向
	static int8 around[8][2];
	//判断朝向
	static int FindAroundIndex(uint16 cnx, uint16 cny, uint16 toX, uint16 toY);
public:
	MapTemplate();
	~MapTemplate();
	bool Load(istream& is,bool only_baseinfo=false);
	bool Load(const char *str,bool only_baseinfo=false);
	bool LoadObstaclePeriphery();			//载入障碍周边路线图	
	bool IsCanRun(uint16 x, uint16 y) const
	{
		if(x >= m_baseinfo.width || y >= m_baseinfo.height)
			return false;

		return !IsObstacle(x + m_baseinfo.width * y);		
	}	
	bool IsInMap(uint16 x, uint16 y) const
	{
		if(x >= m_baseinfo.width || y >= m_baseinfo.height)
			return false;
		return true;
	}
	bool IsCanRun(uint32 p) const
	{
		if(p >= uint32(m_baseinfo.width * m_baseinfo.height))
			return false;

		return !IsObstacle(p);		
	}

	// 传入给定路径，判断是否可以通过
	bool IsCanRun(uint16 count, float *path) const;
	bool IsCanRun(float fromX, float fromY,const vector<float>& path) const;	
	bool IsCanRun(float fromX, float fromY, float toX, float toY, bool isStart = false) const;	
	bool IsCanRun(float fromX, float fromY, float toX, float toY, uint16 &lastX, uint16 &lastY, bool isStart = false) const;
	bool IsCanRun(float fromX, float fromY, float toX, float toY, uint16 &lastX, uint16 &lastY, uint16 &cannotx, uint16 &cannoty, bool isStart = false)  const;

	bool IsTeleportPos(uint16 x, uint16 y)
	{
		for(uint32 i = 0; i < m_teleport.size(); i++)
		{
			if(m_teleport[i].x == x && m_teleport[i].y == y)
				return true;
		}

		return false;
	}

	bool FindTeleportPos(mt_teleport& teleport, uint16 x, uint16 y) const
	{
		for(vector<mt_teleport>::const_iterator iter = m_teleport.begin();
			iter != m_teleport.end();
			++iter)
		{
			if(iter->x == x && iter->y == y)
			{
				teleport = *iter;
				return true;
			}
		}

		return false;
	}
	uint32 GetParentMapid() const
	{
		return m_baseinfo.parentID;
	}
	bool IsInstance() const
	{
		return m_baseinfo.is_instance != 0;
	}
	//非有效坐标
	bool IsNotValidPos(float x,float y) const
	{
		return x != x || y != y ||
			x < 0.0f || y < 0.0f ||
			x >= float(m_baseinfo.width) || y >= float(m_baseinfo.height);
	}

	MainRoadPoint *GetMainRoadPoint(uint32 point) const;
public:
	//vector<mt_monsters>
	//获取复活点
	bool GetSpwanPoint(uint16 &x, uint16 &y,uint16 fation) const;
	bool IsInSafeArea(uint16 x,uint16 y) const;	

	//通过坐标获取当前可用的怪物路线图
	const mt_point_in_line *GetMonsterLineEach(uint32 point) const
	{
		mt_point_in_line *result = NULL;
		map<uint32, mt_point_in_line*>::const_iterator it = m_all_way_point.find(point);
		if(it != m_all_way_point.end())
			result = it->second;
		return result;
	}

	const mt_baseinfo& GetMapBaseInfo() const
	{
		return m_baseinfo;
	}
protected:
	bool LoadBaseinfo(istream &is);			//载入基本信息
	bool LoadObstacle(istream &is);			//载入障碍信息
	bool _LoadTrans(istream &is);			//载入半透明信息
	bool _LoadRaised(istream &is);			//载入复活点信息
	bool LoadTeleport(istream &is);			//载入传送点信息
	bool LoadTrunk(istream &is);			//载入主干道
	bool _LoadMusic(istream &is);			//载入音乐信息
	bool _LoadLight(istream &is);			//载入光源
	bool _LoadMovie(istream &is);			//载入动态层
	bool _LoadNPC(istream &is);				//载入NPC信息
	bool LoadCreatrue(istream &is);			//载入怪物信息
	bool LoadGameObject(istream &is);		//载入其他游戏对象
	bool LoadMonsterLine(istream &is);      //载入怪物路线
	bool _LoadInving(istream &is);			//载入水倒信息
private:
	bool ReadValue(const char *str, char *value)
	{
		strcpy(value, str);
		return true;
	}
	bool ReadValue(const char *str, float& value)
	{
		sscanf(str, "%f", &value);	
		return true;
	}
	template<typename T>
	bool ReadValue(const char *str, T& value)
	{
		int32 temp_int;
		sscanf(str, "%d", &temp_int);
		value = (T)temp_int;
		return true;
	}

	bool ReadToken(istream& is, string& str, char split = ',');

	template<typename T>
	bool ReadToken(istream& is, T& value, char split = ',')
	{
		string str ;
		stringstream ss;

		if(!ReadToken(is, str, split))
			return false;
		ReadValue(str.c_str(), value);
		return true;
	}

	inline bool IsObstacle(uint16 x, uint16 y) const
	{
		return IsObstacle(x + y * m_baseinfo.width);
	}

	inline bool IsObstacle (uint32 index) const
	{
		return ( ( (uint8 *)m_obstacleMask)[ index >> 3 ] & ( 1 << ( index & 0x7 ) )) != 0;
	}
private:
	mt_baseinfo		m_baseinfo;
	vector<uint8>	m_data;			//根据位获得该坐标下的格子性质
	//UpdateMask 
	uint32 m_obstacleMask[MAX_MAP_CELLS_MASK];		//障碍点信息掩码
	map<uint32, mt_monsters> m_monsters;	//刷怪点
	vector<mt_gameobject> m_gameobjects;	//刷游戏对象点
	vector<mt_raised> m_spawnpoint;		//复活点	

	void *path_pointer;					//路线的总指针，在类析构时释放
	void *point_in_line_pointer;		//点在线的总指针，在类析构时释放
	vector<mt_monsterline_each*> all_monsterline_each;	//所有路线，在类析构时释放
public:
	vector<mt_teleport> m_teleport;	//传送点
	map<uint32,MainRoadPoint*> m_main_road;//主干道
	vector<mt_monsterline> m_monsterlines;	//刷怪物路线
	map<uint32, mt_point_in_line*> m_all_way_point;//所有怪物寻路点的集合
};

#endif
