
#include "map_template.h"
#include <fstream>
#include <iosfwd>
#include <math.h>
#include <iostream>
#include <core_obj/UpdateMask.h>

using core_obj::UpdateMask;

//////////////////////////////////////////////////////////////////////////
//for map_template static
MapTemplate::MTVector MapTemplate::mtvec;
uint32 MapTemplate::m_map_version;
stringstream temp_strline_stream;		//中间变量
float MapTemplate::safe_distance = 6.0f;

#if PLATFORM == PLATFORM_WINDOWS

bool MapTemplate::_ls(string folder, string ext, vector<string>& files)
{
    string searchstr = folder + "/*." + ext;

    HANDLE handle;
    WIN32_FIND_DATA ffd;

    if((handle = FindFirstFile(searchstr.c_str(), &ffd)) == INVALID_HANDLE_VALUE)
        return false;

    do
    {
        files.push_back(folder + "/" + ffd.cFileName);
    }
    while(FindNextFile(handle, &ffd));

    FindClose(handle);
    return true;
}
#else

#include <sys/dir.h>
#include <sys/stat.h>

//判断是否为目录

int IS_DIR(const char* path)
{
    struct stat st;
    lstat(path, &st);
    return S_ISDIR(st.st_mode);
}

bool MapTemplate::_ls(string folder, string ext, vector<string>& files)
{
    DIR *pdir;
    struct dirent *pdirent;
    string temp;

    if((pdir = opendir(folder.c_str())))
    {
        while((pdirent = readdir(pdir)))
        {
            //跳过"."和".."
            if(strcmp(pdirent->d_name, ".") == 0
                    || strcmp(pdirent->d_name, "..") == 0)
                continue;

            temp = folder + "/" + pdirent->d_name;

            //不是以该扩展名结尾
            if(temp.substr(temp.size() - ext.size(), ext.size()) != ext)
                continue;

            files.push_back(temp);

            //当temp为目录并且recursive为1的时候递归处理子目录
            if(IS_DIR(temp.c_str()))
                continue;

            //_ls(temp, ext,files);

        }
    }
    else
    {
        printf("opendir error:%s\n", folder.c_str());
        return false;
    }

    closedir(pdir);
    return true;
}

#endif

//加载版本
bool MapTemplate::Load_Version(string folder)
{
	//读素材版本号
	string map_index_file = folder  + "index.map";
	std::ifstream ifsversion(map_index_file.c_str(), std::ios::in);
	string version;
	if(!getline(ifsversion, version))
		return false;
	//地图ID
	Tokens tokens;
	StrSplit(tokens,version,',');
	if (!tokens.size())
	{
		return false;
	}
	MapTemplate::m_map_version = atoi(tokens[0].c_str());
	return true;
	//if(!ReadToken(is,m_map_version))
	//	return false;
}

bool MapTemplate::Load(string folder,bool only_baseinfo)
{
    vector<string> files;
    if(!_ls(folder, "txt", files) || files.size() == 0)
        return false;
	
	if (!Load_Version(folder))
	{
		//tea_perror
		return false;
	}
	
    for(vector<string>::const_iterator it = files.begin();
            it != files.end();
            ++it)
    {
        std::ifstream ifs(it->c_str(), std::ios::in);	

        MapTemplate *mt = new MapTemplate();

        if(!mt->Load(ifs,only_baseinfo))
        {
            delete mt;
            return false;
        }

        if(mtvec.size() <= mt->m_baseinfo.mapid)
            mtvec.resize(mt->m_baseinfo.mapid + 1, 0);

        mtvec[mt->m_baseinfo.mapid] = mt;
        ifs.close();
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////
//for map_template

MapTemplate::MapTemplate():path_pointer(NULL), point_in_line_pointer(NULL)
{
    memset(&m_baseinfo, 0, sizeof(m_baseinfo));
}

MapTemplate::~MapTemplate()
{
	if(path_pointer != NULL)
	{
		for(vector<mt_monsterline_each*>::iterator it = all_monsterline_each.begin(); it != all_monsterline_each.end(); ++it)
		{
			delete *it;
		}
		free(point_in_line_pointer);
		free(path_pointer);
	}
	safe_delete(m_main_road);
}


// 传入给定路径，判断是否可以通过
bool MapTemplate::IsCanRun(float fromX,float fromY,const vector<float>& path) const
{
	if(path.empty())
		return IsCanRun((uint16)fromX,(uint16)fromY);

	//第一点,无视障碍
	if(!IsCanRun((uint16)fromX, (uint16)fromY))
		return true;
	
    if(!IsCanRun((float)fromX, (float)fromY, (float)path[0], (float)path[1], true))    
        return false;    
	
	//逐点判断，不能通过立刻跳出
	for (int i = 0; i < (int)path.size(); i = i + 2)
	{
		if(!IsCanRun((float)fromX,(float)fromY, (float)path[i],(float)path[i+1]))
		{
#if PLATFORM == PLATFORM_WINDOWS
			printf("MapTemplate::IsCanRun can not run %f,%f to %f,%f", fromX,fromY, path[i],path[i+1]);
#endif
			return false;
		}
		fromX = path[i];
		fromY = path[i+1];
	}  
    return true;
}

// 传入给定路径，判断是否可以通过
bool MapTemplate::IsCanRun(uint16 count, float *path) const
{
    float *tpath, x0, y0, x1, y1;
    tpath = path;

    if(count > 1)
    {
        x1 = *tpath;
        tpath++;
        y1 = *tpath;
        tpath++;
        bool isStart = true;
        x0 = x1;
        y0 = y1;
        x1 = *tpath;
        tpath++;
        y1 = *tpath;
        tpath++;

		/*if(isStart && x1 == x0 && y0 == y1)
		{
		isStart = false;
		}*/

        if(!IsCanRun(x0, y0, x1, y1, isStart))
        {
#if PLATFORM == PLATFORM_WINDOWS
			printf("MapTemplate::IsCanRun can not run %f,%f to %f,%f", x0, y0, x1, y1);
#endif
            return false;
        }

        //逐点判断，不能通过立刻跳出
        for(uint16 i = 4; i < count; i += 2)
        {
            x0 = x1;
            y0 = y1;
            x1 = *tpath;
            tpath++;
            y1 = *tpath;
            tpath++;

            if(! IsCanRun(x0, y0, x1, y1, isStart))
            {
#if PLATFORM == PLATFORM_WINDOWS
				printf("MapTemplate::IsCanRun can not run %f,%f to %f,%f", x0, y0, x1, y1);
#endif
                return false;
            }

            isStart = false;
        }
    }
    else
    {
        x0 = *tpath;
        tpath++;
        y0 = *tpath;
        return IsCanRun((uint16)x0, (uint16)y0);
    }

    return true;
}

//传入起点和终点，判断是可以通过
bool MapTemplate::IsCanRun(float fromx, float fromy, float tox, float toy, bool isStart) const
{
	int32 x0 = (int32)fromx, y0 = (int32)fromy, x1 = (int32)tox, y1 = (int32)toy;
	int32 dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int32 dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int32 err = dx + dy, e2; /* error value e_xy */	

	for(;;)
	{
		/* loop */
		if(!IsCanRun(x0, y0) && !isStart)
		{
			//#if PLATFORM == PLATFORM_WINDOWS
			//			printf("无法通过的路线(%f,%f)(%f,%f)坐标：(%d,%d)\n", fromx, fromy, tox, toy, x0, y0);
			//#endif
			return false;
		}

		isStart = false;

		if(x0 == x1 && y0 == y1) break;

		e2 = 2 * err;

		if(e2 >= dy)
		{
			err += dy;    /* e_xy+e_x > 0 */
			x0 += sx;
		}

		if(e2 <= dx)
		{
			err += dx;    /* e_xy+e_y < 0 */
			y0 += sy;
		}
	}

	return true;
}

bool MapTemplate::IsCanRun(float fromX, float fromY, float toX, float toY, uint16 &lastX, uint16 &lastY, bool isStart) const
{
	uint16 nx = 0, ny = 0;
	return IsCanRun(fromX, fromY, toX, toY, lastX, lastY, nx, ny, isStart);
}

bool MapTemplate::IsCanRun(float fromx, float fromy, float tox, float toy, uint16 &lastX, uint16 &lastY, uint16 &cannotx, uint16 &cannoty, bool isStart) const
{
	int32 x0 = (int32)fromx, y0 = (int32)fromy, x1 = (int32)tox, y1 = (int32)toy;
	int32 dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int32 dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int32 err = dx + dy, e2; /* error value e_xy */
	lastX = 0;
	lastY = 0;

	for(;;)
	{
		/* loop */
		if(!IsCanRun(x0, y0) && !isStart)
		{
			cannotx = x0;
			cannoty = y0;
//#if PLATFORM == PLATFORM_WINDOWS
//			printf("无法通过的路线(%f,%f)(%f,%f)坐标：(%d,%d)\n", fromx, fromy, tox, toy, x0, y0);
//#endif
			return false;
		}

		lastX = x0;
		lastY = y0;
		isStart = false;

		if(x0 == x1 && y0 == y1) break;

		e2 = 2 * err;

		if(e2 >= dy)
		{
			err += dy;    /* e_xy+e_x > 0 */
			x0 += sx;
		}

		if(e2 <= dx)
		{
			err += dx;    /* e_xy+e_y < 0 */
			y0 += sy;
		}
	}

	return true;
}

bool MapTemplate::Load(istream& is,bool only_baseinfo)
{
    //先载入基本信息
    if(!LoadBaseinfo(is))
        return false;

	//如果只载入基本信息则直接返回
	if(only_baseinfo)
		return true;

    //载入障碍信息
    if(!LoadObstacle(is))
        return false;


    //载入透明层
    if(!_LoadTrans(is))
        return false;

    //载入复活点信息
    if(!_LoadRaised(is))
        return false;

    //载入传送点信息
    if(!LoadTeleport(is))
        return false;

    //载入主干道信息
    if(!LoadTrunk(is))
        return false;

    //载入音乐信息
    if(!_LoadMusic(is))
        return false;

    //载入光源信息
    if(!_LoadLight(is))
        return false;

    //载入动态层
    if(!_LoadMovie(is))
        return false;

    //载入怪物信息
    if(!LoadCreatrue(is))
        return false;

    //载入游戏对象
    if(!LoadGameObject(is))
        return false;

    //载入移动线路
    if(!LoadMonsterLine(is))
        return false;

    return true;
}

//载入基本信息
bool MapTemplate::LoadBaseinfo(istream &_is)
{
    string str;		

    if(!getline(_is, str))
        return false;

	//string gbk_str;
	//utf8ToConsole(str,gbk_str);

    stringstream is(str);
    std::cout << "LoadBaseinfoing" << std::endl;

    //地图ID
    if(!ReadToken(is, m_baseinfo.mapid))
        return false;

	 std::cout << "mapid = " << m_baseinfo.mapid << std::endl;

    //地图名称
    if(!ReadToken(is, m_baseinfo.name))
        return false;

    //地图创建时间
    if(!ReadToken(is, m_baseinfo.create_date))
        return false;

    //像素宽
    if(!ReadToken(is, m_baseinfo.px_width))
        return false;

    //像素高
    if(!ReadToken(is, m_baseinfo.px_height))
        return false;

    //瓷砖宽
    if(!ReadToken(is, m_baseinfo.unit_width))
        return false;

    //瓷砖高
    if(!ReadToken(is, m_baseinfo.unit_height))
        return false;

    //逻辑宽
    if(!ReadToken(is, m_baseinfo.width))
        return false;

    //逻辑高
    if(!ReadToken(is, m_baseinfo.height))
        return false;

    //是否副本地图
    if(!ReadToken(is, m_baseinfo.is_instance))
        return false;

    //父级地图ID
    if(!ReadToken(is, m_baseinfo.parentID))
        return false;

    //场景音乐
    if(!ReadToken(is, m_baseinfo.music))
        return false;

    //影子方向
    if(!ReadToken(is, m_baseinfo.shadow))
        return false;

    //副本人数
    if(!ReadToken(is, m_baseinfo.count))
        return false;

    //日限制
    if(!ReadToken(is, m_baseinfo.day_limit))
        return false;

    //周限制
    if(!ReadToken(is, m_baseinfo.week_limit))
        return false;

	//副本类型
	if(!ReadToken(is, m_baseinfo.instance_type))
        return false;

    //初始化逻辑格子数据
    //m_obstacleMask.SetCount(m_baseinfo.width * m_baseinfo.height);	
	ASSERT(m_baseinfo.width * m_baseinfo.height <= MAX_MAP_CELLS_MASK*32);
    std::cout << "LoadBaseinfoed OK" << std::endl;

    return true;
}

bool MapTemplate::LoadObstacle(istream &_is)
{
    string str;

    if(!getline(_is, str))
        return false;

    if(str.empty())
        return true;

	char m_temp_str[100000];
	char *token[100000];
	uint32 len;
	strcpy(m_temp_str, str.c_str());
	str_split(m_temp_str, ",", token, &len, true, true, 100000);
	
	memset(m_obstacleMask,0,sizeof(m_obstacleMask));
    //障碍点
    uint32 obs, i = 0;

    while(i < len)
    {
		ReadValue(token[i], obs);
		ASSERT(m_obstacleMask);
		m_obstacleMask[i] = obs;
        i++;
    }

    return true;
}


int8 MapTemplate::around[8][2] = {{-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0}};
//判断朝向
int MapTemplate::FindAroundIndex(uint16 fromX, uint16 fromY, uint16 toX, uint16 toY)
{
	int diffx, diffy;
	int around_index;
	//计算差异，用来获取方向
	diffx = fromX - uint16(toX);
	diffy = fromY - uint16(toY);
	//通过差异发现方向
	for(around_index = 0; around_index < 8 && (around[around_index][0] != diffx || around[around_index][1] != diffy); around_index++);

	//没找到方向，明显程序有问题。
	if(around_index >= 8)
	{
		//tea_pdebug("不是临近的点，无法得到朝向。");
		return -1;
	}
	//为了让八方向数组形成圆形循环，林北加个八比较爽
	return around_index + 8;
}

//载入障碍周边路线图
bool MapTemplate::LoadObstaclePeriphery()
{
	clock_t cstart;
	cstart = clock();
	uint32 map_size = m_baseinfo.width * m_baseinfo.height, sum = 0, monster_point_sum;
	uint32 insert_sum = 0;
	UpdateMask all_site;
	map<uint32, mt_monsterline_each*> monsterline_each;

	//先弄到所有障碍旁边的点
	all_site.SetCount(map_size);
	for(uint16 x = 0; x < m_baseinfo.width; ++x)
	{
		for (uint16 y = 0; y < m_baseinfo.height; ++y)
		{
			//如果是障碍点，就看看周边有没有非障碍点
			if(IsObstacle(x,y))
			{
				//只拿上下左右四个方向，因为斜角的不需要有，否则就走不了45度角的障碍
				for(uint8 i = 1; i < 8; i += 2)
				{
					if((x == 0 && around[i][0] == -1)
						||(y == 0 && around[i][1] == -1)
						||(x == m_baseinfo.width - 1 && around[i][0] == 1)
						||(y == m_baseinfo.height - 1 && around[i][1] == 1))
						continue;
					uint16 x1 = x + around[i][0], y1 = y + around[i][1];
					uint32 point = x1 + m_baseinfo.width * y1;
					if(!IsObstacle(point) && all_site.GetBit(point) == 0)
					{
						all_site.SetBit(point);
						sum++;
					}
				}
			}			
		}
	}
	monster_point_sum = sum;
	path_pointer = malloc(sizeof(mt_point) * monster_point_sum);
	point_in_line_pointer = malloc(sizeof(mt_point_in_line) * monster_point_sum);
	uint32 load_index = 0;
	//开始把点连成线	
	while(sum > 0)
	{
		int index;
		uint16 x, y, nx, ny;
		uint32 all_site_index = 0;
		list<mt_point_in_line*> plList;//本干道所有的点在线信息
		mt_monsterline_each *eachPoint = new mt_monsterline_each;//本干道
		all_monsterline_each.push_back(eachPoint);
		//找到第一个可用点
		for(; all_site_index < map_size && all_site.GetBit(all_site_index) == 0 ; all_site_index++);
		uint32 point = all_site_index;
		all_site.UnSetBit(all_site_index);
		sum--;
		x = point % m_baseinfo.width;
		y = point / m_baseinfo.width;
		//生成点在线
		mt_point_in_line *pl = (mt_point_in_line*)point_in_line_pointer + load_index;
		pl->each = eachPoint;
		pl->point = point;
		plList.push_back(pl);
		//生成点
		mt_point *mp = (mt_point*)path_pointer + load_index++;
		mp->point = point;
		mp->pos_x = x;
		mp->pos_y = y;
		eachPoint->point_each.push_back(mp);
		eachPoint->is_join = true;
		monsterline_each[point] = eachPoint;
		while(sum > 0)
		{
			bool found = false;
			uint32 point_next = 0;
			for(uint8 i = 0; i < 8; i++)
			{
				nx = x + around[i][0];
				ny = y + around[i][1];
				point_next = nx + m_baseinfo.width * (ny);
				if(point_next >= map_size || all_site.GetBit(point_next) == 0)
					continue;
				index = FindAroundIndex(nx,ny,x,y);
				if(index != -1)
				{
					//如果是斜角，则需要左边或者右边有一个共同的障碍点
					//如果是相邻的点，则需要在同一侧有两个共同的障碍点
					//好复杂
					if	(
							(
								//如果是斜角，则需要左边或者右边有一个共同的障碍点
								index % 2 == 0 
								&& 
								(
									IsObstacle(nx - around[(index - 1) % 8][0], ny - around[(index - 1) % 8][1])
									|| IsObstacle(nx - around[(index + 1) % 8][0], ny - around[(index + 1) % 8][1])
								)
							)
							|| 
							(
								//如果是相邻的点，则需要在同一侧有两个共同的障碍点
								index % 2 == 1 
								&& 
								(
									(IsObstacle(nx - around[(index - 1) % 8][0], ny - around[(index - 1) % 8][1]) && IsObstacle(nx - around[(index - 2) % 8][0], ny - around[(index - 2) % 8][1]))
									|| (IsObstacle(nx - around[(index + 1) % 8][0], ny - around[(index + 1) % 8][1]) && IsObstacle(nx - around[(index + 2) % 8][0], ny - around[(index + 2) % 8][1]))
								)
							)
						)
					{
						//生成点在线
						mt_point_in_line *pl = (mt_point_in_line*)point_in_line_pointer + load_index;
						pl->each = eachPoint;
						pl->point = point_next;
						//生成点
						mt_point *p = (mt_point*)path_pointer + load_index++;
						p->point = point_next;
						p->pos_x = nx;
						p->pos_y = ny;
						//分布放入集合
						if(eachPoint->is_join)
						{
							plList.push_back(pl);
							eachPoint->point_each.push_back(p);
						}
						else
						{
							plList.push_front(pl);
							eachPoint->point_each.push_front(p);
							insert_sum++;
						}
						point = point_next;
						monsterline_each[point] = eachPoint;
						x = nx;
						y = ny;
						all_site.UnSetBit(point_next);
						sum--;
						found = true;
						break;
					}
				}
			}
			if(!found)
			{
				if(!eachPoint->is_join)
					break;
				point_next = eachPoint->point_each.front()->point;
				if(eachPoint->point_each.size() != 2)//太他妈巧合了，第二个
				{
					if(point_next == point)
						break;	//画地图画得也太玄乎了吧，一堆障碍里有一点可以站立的地方，真厉害
					else
					{
						nx = point_next % m_baseinfo.width;
						ny = point_next / m_baseinfo.width;
						index = FindAroundIndex(nx,ny,x,y);
						if(index != -1)
							break;
					}
				}
				point = eachPoint->point_each.front()->point;
				x = eachPoint->point_each.front()->pos_x;
				y = eachPoint->point_each.front()->pos_y;
				eachPoint->is_join = false;
			}
		}

		//OK，把这条干道的所有的点放到怪物寻路点的集合中
		uint32 pl_index = 0;
		for (list<mt_point_in_line*>::iterator pl_it = plList.begin(); pl_it != plList.end(); ++pl_it, pl_index++)
		{
			(*pl_it)->index = pl_index;
			m_all_way_point[(*pl_it)->point] = (*pl_it);
		}
		
	}
	printf("地图【%2d】:【%4d】个点，【%4d】条干道，耗时【%4d】毫秒。\n",m_baseinfo.mapid, monster_point_sum, int(all_monsterline_each.size()), int(clock() - cstart));
	return true;
}

bool MapTemplate::_LoadTrans(istream &_is)
{
    string str;

    if(!getline(_is, str))
        return false;

    return true;
}

bool MapTemplate::_LoadRaised(istream &_is)
{
    //fuhuodian
    string str;

    if(!getline(_is, str))
        return false;

    if(str.empty())
        return true;

    stringstream is(str);

    uint32 x;

	mt_raised raised;
    while(ReadToken(is, x))
    {
        memset(&raised, 0, sizeof(raised));
        raised.x = x;

        if(!ReadToken(is, raised.y))
            return false;

		if(!ReadToken(is, raised.faction))
            return false;

        m_spawnpoint.push_back(raised);
    }

    return true;
}


bool MapTemplate::LoadTeleport(istream &_is)
{
    string str;

    if(!getline(_is, str))
        return false;

    if(str.empty())
        return true;

    stringstream is(str);
    uint32 x = 0;
    mt_teleport teleport;

    while(ReadToken(is, x))
    {
        memset(&teleport, 0, sizeof(teleport));
        teleport.x = x;

        if(!ReadToken(is, teleport.y))
            return false;

        if(!ReadToken(is, teleport.temp_id))
            return false;

        if(!ReadToken(is, teleport.name))
            return false;	
		
        if(!ReadToken(is, teleport.to_mapid))
            return false;

        if(!ReadToken(is, teleport.to_x))
            return false;

        if(!ReadToken(is, teleport.to_y))
            return false;

        m_teleport.push_back(teleport);
    }

    return true;
}

bool MapTemplate::LoadTrunk(istream &_is)
{
    string str;

    if(!getline(_is, str))
        return false;

	stringstream is(str);
	uint16 x = 0, n_x = 0, n_y = 0;
	map<MainRoadPoint *, vector<uint32>> next_ps;
	while(ReadToken(is, x))
	{
		MainRoadPoint *point = new MainRoadPoint;
		point->pos_x = x;
		ReadToken(is, point->pos_y);
		point->point = point->pos_x + m_baseinfo.width * point->pos_y;
		uint32 len = 0;
		ReadToken(is, len);
		vector<uint32> next_v;
		next_v.resize(len);
		for (uint32 i = 0; i < len; i++)
		{
			ReadToken(is, n_x);
			ReadToken(is, n_y);
			next_v[i] = n_x + m_baseinfo.width * n_y;
		}
		next_ps[point] = next_v;
		this->m_main_road[point->point] = point;
	}
	for (auto it:next_ps)
	{
		MainRoadPoint *point = it.first;
		for (auto p:it.second)
		{
			auto it_point = this->m_main_road.find(p);
			ASSERT(it_point != this->m_main_road.end());
			point->next_ps.push_back(it_point->second);
		}
	}
    return true;
}

bool MapTemplate::_LoadMusic(istream &is)
{
    string str;

    if(!getline(is, str))
        return false;

    return true;
}

bool MapTemplate::_LoadLight(istream &is)
{
    string str;

    if(!getline(is, str))
        return false;

    return true;
}

bool MapTemplate::_LoadMovie(istream &is)
{
    string str;

    if(!getline(is, str))
        return false;

    return true;
}

bool MapTemplate::LoadCreatrue(istream &_is)
{
    string str;

    if(!getline(_is, str))
        return false;

    if(str.empty())
        return true;

	char m_temp_str[100000];
	char *token[100000];
	uint32 len;
	strcpy(m_temp_str, str.c_str());
	str_split(m_temp_str, ",", token, &len, true, true, 100000);


    uint32 temp_id = 0, i = 0;
    mt_monsters monsters;

    while(i < len)
    {
		ReadValue(token[i++], temp_id);

        memset(&monsters, 0, sizeof(monsters));
        monsters.templateid = temp_id;

        if(!ReadValue(token[i++], monsters.x))
            return false;

        if(!ReadValue(token[i++], monsters.y))
            return false;

        if(!ReadValue(token[i++], monsters.count))
            return false;

        if(!ReadValue(token[i++], monsters.spawn_type))
            return false;

        if(!ReadValue(token[i++], monsters.respawn_time))
            return false;

        if(!ReadValue(token[i++], monsters.spawn_time1))
            return false;

        if(!ReadValue(token[i++], monsters.spawn_time2))
            return false;

        if(!ReadValue(token[i++], monsters.spawn_time3))
            return false;

        if(!ReadValue(token[i++], monsters.script_name))
            return false;

        if(!ReadValue(token[i++], monsters.move_type))
            return false;

        if(!ReadValue(token[i++], monsters.line_id))
            return false;

        if(!ReadValue(token[i++], monsters.flag))
            return false;

        if(!ReadValue(token[i++], monsters.toward))
            return false;

		if(!ReadValue(token[i++], monsters.alias_name))
            return false;

        monsters.toward = monsters.toward * M_PI;
        uint32 pos = (uint32)monsters.x + m_baseinfo.width * (uint32)monsters.y;

        m_monsters[pos] = monsters;
    }

    return true;
}

bool MapTemplate::LoadGameObject(istream &_is)
{
    string str;

    if(!getline(_is, str))
        return false;

    if(str.empty())
        return true;

    stringstream is(str);

    uint32 temp_id = 0;
    mt_gameobject gameobject;

    while(ReadToken(is, temp_id))
    {
        memset(&gameobject, 0, sizeof(gameobject));
        gameobject.templateid = temp_id;

        if(!ReadToken(is, gameobject.x))
            return false;

        if(!ReadToken(is, gameobject.y))
            return false;

        if(!ReadToken(is, gameobject.count))
            return false;

        if(!ReadToken(is, gameobject.spawn_type))
            return false;

        if(!ReadToken(is, gameobject.respawn_time))
            return false;

        if(!ReadToken(is, gameobject.spawn_time1))
            return false;

        if(!ReadToken(is, gameobject.spawn_time2))
            return false;

        if(!ReadToken(is, gameobject.spawn_time3))
            return false;

        if(!ReadToken(is, gameobject.script_name))
            return false;

        if(!ReadToken(is, gameobject.toward))
            return false;

		if(!ReadToken(is, gameobject.alias_name))
            return false;

        gameobject.toward = gameobject.toward * M_PI;
        m_gameobjects.push_back(gameobject);
    }

    return true;
}

bool MapTemplate::LoadMonsterLine(istream &_is)
{
    string str;
    getline(_is, str);

    if(str.empty())
        return true;

    stringstream is(str);

    uint32 temp_id = 0;
    int32 point_len;
    mt_point point1;
    mt_monsterline monsterline;

    while(ReadToken(is, temp_id))
    {
        monsterline.path.clear();
        monsterline.id = temp_id;

        if(!ReadToken(is, point_len))
            return false;

        for(int j = 0; j < point_len; j++)
        {
            if(!ReadToken(is, point1.pos_x))
                return false;

            if(!ReadToken(is, point1.pos_y))
                return false;

            monsterline.path.push_back(point1);
        }

        if(m_monsterlines.size() <= monsterline.id)
            m_monsterlines.resize(monsterline.id + 1);

        m_monsterlines[monsterline.id] = monsterline;
    }

    return true;
}

bool MapTemplate::_LoadInving(istream &is)
{
    string str;

    if(!getline(is, str))
        return false;

    return true;
}

bool MapTemplate::ReadToken(std::istream &is,  std::string &str , char split)
{
    //清空
    str.clear();

    char c = 0;

    while(is.good())
    {
        c = is.get();

        if(split != c && is.good())
            str.push_back(c);
        else
            return true;

    }

    //读不到
    if(str.empty())
        return false;

    //
    return true;
}

bool MapTemplate::GetSpwanPoint(uint16 &x, uint16 &y,uint16 fation) const
{
    if(m_spawnpoint.empty())
        return false;

	uint16 min_dist = uint16(-1);		//这里这个
	uint16 fX = x,fY = y;
	for (auto it = m_spawnpoint.begin();it != m_spawnpoint.end();++it)
	{
		if(it->faction == 0 || it->faction == fation)
		{
			auto dist = GetDistance(fX,fY,it->x,it->y);
			if(dist < min_dist)
			{
				min_dist = dist;
				x = it->x;
				y = it->y;
				return true;
			}
		}		
	}
	
    return false;
}

bool MapTemplate::IsInSafeArea(uint16 x,uint16 y) const
{
	for (auto iter = m_spawnpoint.begin();iter!=m_spawnpoint.end();++iter)
	{
		if(GetDistance(x,y,iter->x,iter->y) < safe_distance && !IsInstance())
			return true;
	}
	return false;
}

MainRoadPoint *MapTemplate::GetMainRoadPoint(uint32 point) const
{
	map<uint32, MainRoadPoint*>::const_iterator it = m_main_road.find(point);
	ASSERT(it != m_main_road.end());
	return it->second;
}
