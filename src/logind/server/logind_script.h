#ifndef _LOGIND_SCRIPT_H_
#define _LOGIND_SCRIPT_H_

#include <comm/common_stl.h>
#include <shared/lua_helper.h>

int scripts_init(const char*);
int scripts_free();
int scripts_reload();

//获取配置信息
extern int DoGetConfig();

//读取模版数据到内存
extern bool LoadTemplate();

//重新读取配置
extern int LuaLoadConfig(lua_State* scriptL);
//GM命令的KEY转换
extern int GetGmCommandKey(string &gm_str);
//重新载入游戏配置表的内容
extern int DoReloadGameConfigTable(const char *key, const char *value);
//玩家下线做点什么
extern int DoPlayerLogout(logind_player* player);

//获取汉字(字符串)
extern int DoGetCharacters(uint8 pos, string & parameter);

//修复玩家数据
extern int DoRepairPlayerData(const string &account, vector<GuidObject*> &data, bool &b);

//获得场景服的个数
extern int DoGetScenedSize(uint32 &result);

//添加场景服的玩家数量
extern int DoAddScenedPlayer(uint32 conn_id);

//减少场景服的玩家数量
extern int DoSubScenedPlayer(uint32 conn_id);

//根据一定的规则获得场景服连接
extern int DoGetScenedFDByType(uint16 inst_type, uint32 mapid);

//玩家下线的时候判断是否记录玩家所在地图的信息，最多保留10分钟
extern int DoIsRecordIntanceInfo(logind_player* player, uint32 map_id, uint32 isfuben, uint32 fuben_type);

//合服后数据处理
extern int DoMergeSomething();
//pk服，根据跨服类型选择要传送到的地图id
extern bool DoSelectKuafuMapid(logind_player* player, uint32 warid, uint8 kuafutype, uint32 number, uint32 reverse, string reverse_str);
//根据地图id判断所在地图是否为跨服地图
extern bool DoIsKuafuMapID(uint32 map_id);
//计算玩家付费等级
extern int DoPlayerPayLevel(logind_player* player);
//是否需要提供传送generalid
extern int DoIsNeedGeneralid(uint32 mapid, bool &result);
extern bool IsInstanceOfflineReenter(uint32 mapid);
// 获得冒险地图的传送信息
extern int DoGetRiskTeleportInfo(logind_player* player, int &mapid, float &x, float &y, string &generalId);
// 获得地图配置人数
extern int OnGetMapLineCount(uint32 mapid, uint8 &result);
//查找或者新建地图实例
extern int DoFindOrCreateMap(uint32 mapid, uint16 inst_type, const string &general_id, uint32 lineno, int &index);
//根据地图id判断所在地图是否为世界地图
extern bool DoIsWorldMapID(uint32 map_id);
//获取下次保存数据库的时间
extern int DoGetNextSaveTime(uint32 &t);
//初始化玩家信息
extern void DoSetNewPlayerInfo(logind_player* player);
#endif	
