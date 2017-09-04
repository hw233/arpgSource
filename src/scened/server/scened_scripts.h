#ifndef _SCENED_SCRIPTS_H_
#define _SCENED_SCRIPTS_H_

#include "Map.h"
#include <comm/common_stl.h>
#include <protocol/external.h>
#include <shared/lua_helper.h>

class Unit;
class Player;
class GameObject;
class Creature;
class Map;

int scripts_init(const char*);
int scripts_free();
int scripts_reload();

//读取模版数据到内存
extern bool LoadTemplate();
//---------------------------------------------------------------------------------------------/
//全局变量初始化脚本
extern int DoInitSomethingScript();
//---------------------------------------------------------------------------------------------/
//使用技能
extern int DoSpellCastScript(Unit *caster,Unit *target,float dst_x,float dst_y,uint32 spellid,uint32 spell_lv, Unit *unit, const char *data = NULL);

//---------------------------------------------------------------------------------------------/
//使用物品
extern int DoUseItemScript(Player *player,uint32 template_id,uint32 count);

//---------------------------------------------------------------------------------------------/
//跳
extern int DoJumpScript(Unit *player);

//---------------------------------------------------------------------------------------------/
//怪物AI 及 NPC闲聊脚本
extern int DoSelect_Instance_Script(Map *m);
extern int OnSetInstanceState(Map *m,int fromstate,int tostate);
extern int OnInstanceTryToNextState(Player *player, Map *m, uint16 level = 0, uint32 param = 0);
extern int OnNotifyArea(Map *m, Unit *trigger, const char *name, float x,float y);
extern int OnMapTimeBegin(Map *m);
extern int OnAfterJoinPlayer(Map *m, Player *player);
extern int OnJoinPlayer(Map *m, Player *player);
extern int OnLeavePlayer(Map *m, Player *player);
extern int OnPlayerKilled(Player *player, Player *killer, uint32& silver);
extern int OnPlayerKilledByMonster(Player *player, Unit *killer);
extern int OnPlayerHurted(Player *killer, Player *player, double damage);
extern int OnPlayerDeath(Player *player);
extern int OnPlayerUpgrage(Map *m, Player *player, uint32 prev, uint32 lv);
extern int DoIsFriendly(Map *m,const Unit *player,const Unit *target);
extern int DoMapBuffBonus(Map *m,const Unit *unit);//地图光环

extern int DoCompleteQuest(Map *_m, Player *player, uint32 questid);	//完成任务执行脚本
extern int DoIsNeedTeleportWhileMapClear(Map *_m, Player *player);			//地图需要清空人时要做的事
extern int DoIsNeedTeleportToOrigin(Map *_m);							// 是否需要回到进入的地方
extern int DoAcceptQuest(Map *_m, Player *player, uint32 questid);	//接受任务执行脚本
extern int DoBeforUseGameObjectScript(Player *player, GameObject *go, uint32 go_templateid);
extern int DoGameObjectScript(Player *player,GameObject *go,uint32 go_templateid);
extern int DoQuestItemScript(Player *player,uint32 item_templateid);
extern int DoScriptInit(const char* ainame,Creature *creature);


extern int DoGossipMessage(const char* ainame,Unit *owner,Player *player,uint32 option_id,string &text,vector<gossip_menu_option_info> &gossip_menu,string &key);
extern int DoMoveInLineOfSight(const char* ainame,Creature *self,Unit *unit);
extern int DoDamageTaken(const char* ainame,Creature *self,Unit *unit,double& damage);
extern int DoDamageDeal(const char* ainame,Creature *self,Unit *unit,double damage);
extern int DoGiveDamage(const char* ainame,Creature *self,Unit *unit,double& damage);
extern int DoKilledUnit(const char* ainame,Creature *self,Unit *victim);
extern int DoJustDied(const char* ainame,Creature *self,Unit *killer,uint32 &deadtime);
extern int DoSaySomething(const char* ainame,Creature *self);


//---------------------------------------------------------------------------------------------/
//复活脚本
extern int DoRespawn(Player *player,Map *map,uint32 respwan_type,uint32 respwan_map,uint16 respwan_x,uint16 respwan_y);

//传送时检验合法性
extern bool DoCheckTeleport(Player *player, uint32 entry, uint16 posX, uint16 posY);
//---------------------------------------------------------------------------------------------/

//玩家世界地图传送时调用
extern int DOWorldMapTeleportScript(Player *player,uint32 map_id,float pos_x,float pos_y);
//---------------------------------------------------------------------------------------------/

//获取特殊战利品
extern int DoLootAllot(Player *player,Creature *owner,  Unit *unit, int boss_type);

//---------------------------------------------------------------------------------------------/
//BUFF触发，周期性BUFF调用
extern int DoBuffTriggerScript(Unit *unit,uint16 buff_id,uint32 buffEffectId);


//---------------------------------------------------------------------------------------------/
//BUFF结束,持续性BUFF调用
extern int DoBuffOverScript(Unit *unit,uint16 buff_id,uint32 buffEffectId);

extern int GetBuffFrequency(uint32 buffEffectId);

//---------------------------------------------------------------------------------------------/
//装备发生变化，重新计算BUFF增加的二级属性
extern int DOComputeBuffAttr(Unit *unit,uint16 buff_id,uint32 buffEffectId);

//---------------------------------------------------------------------------------------------/
//清除一些相应的BUFF
int DoOnlineClearBuff(Player *player);

//---------------------------------------------------------------------------------------------/
//玩家脱离战斗之后干点什么事情
extern int DosometingScript(Player *player);
//PVP攻击玩家
extern int DOPlayerPvPScript(Player *player,Player* target);
//PVP攻击玩家死亡
extern int DOPlayerPvPKilledScript(Player *killer, Player* target);
//玩家被怪物攻击
extern int DOPlayerPVEScript(Unit* creature, Player* target);
//PVE玩家攻击boss
extern int DOPlayerPvBScript(Player* player, Unit*creature);
//---------------------------------------------------------------------------------------------/
//计算玩家获得经验加成
extern int DOComputeExpBonusScript(Player *player, Creature *creature, uint32 exp,int percent,double &vip_exp);

//---------------------------------------------------------------------------------------------/
//执行地图脚本的初始化事件
extern int DoInitMapScript(Map* map);
//执行地图的世界对象初始化 不会被重新加载重置
extern int DoInitMapWorldObject(Map* map);
//执行地图硬盘数据初始化
extern int DoInitReadHddMapData(Map* map);
//执行所有地图脚本的初始化事件
extern int DoInitAllMapScript();
//对活动进行相应操作
extern int DoActivityOperAttrScript(Player *player, uint32 id);
//获得地图脚本的死亡复活时间
extern int DoGetMapSingleRespawnScript(Map* map, Player* player, uint32& res);
//---------------------------------------------------------------------------------------------/

//定时器回调函数
extern int DOTimerTickCallBack(Map *map, const char *s_callback, Map::script_instance_var param[10]);

//处理任务奖励，经验，银子，绑定元宝
extern int DoQuestRewardScript(Player *player, int32 quest_id = 0, int32 xp = 0,int32 silver = 0,int32 bind_gold = 0, int32 taolue = 0, uint8 all_flag = 0, uint32 special_flag = 0);

int LuaCallChat(lua_State *scriptL);				//lua发送聊天信息

//取得战斗信息binlog
extern int LuaGetFightingInfo(lua_State *scriptL);
//执行luaGM命令
extern int DoGMScripts(lua_State *scriptL, Player *player, string &gm_commands);

//移动之前的需要处理的逻辑
extern bool OnMoveTo(Map *m, Player *player);
//要开始攻击之前需要处理的逻辑
extern bool OnStartAttack(Map *m, Player *player);

//GM命令的KEY转换
extern int GetGmCommandKey(string &gm_str);

//属性重算
extern int DoRecalculationAttrs(Player *player, bool bRecal = true);
//人物基础属性中2个等级对应血量的差值
extern int DoGetCharDiffHp(uint32 prev, uint32 curr, int32& diff);
//怪物初始化前
extern int DoBeforeInitCreature(Creature *creature, Map *map);
//按怪物等级初始化怪物信息
extern int DoInitCreatureInfo(Creature *creature, Map *m, bool bRecal = false);
//怪物初始化
extern int creatureInit(Creature *creature); 
//测试数据
extern int DoTestData(Player *player, string& str); 
//判断战利品是否需要进背包
extern bool DoCheckIfSendToAppdAfterLootSelect(Player *player, uint32 entry, uint32 count); 
//单个玩家退出地图前的
extern int DoPlayerExitInstance(Map* map, Player* player, uint32 reserve);
// 指定玩家传送到目的点
extern int SpecifyTeleportOrigin(Map* map, Player* player);
//玩家设置pk模式前的回调
extern int DoSetAttackModeCallback(Map* map, Player* player, uint8 mode);
//进入副本前的判断
extern int DoInstancEnter(Map* map, Player* player, uint32 cur_mapid, uint32 to_mapid);
//获得怪物的
extern int DoGetCreatureBaseExp(Unit* creature, Unit* owner);
//判断某个技能是否够
extern int DoIsEnoughConsumptionCast(Player *player, uint32 spell_id);
//处理某个技能的消耗
extern int DoSpellConsumption(Player *player, uint32 spell_id);
extern int DoPickRiskReward(Player *player);
//设置技能cd
extern int DoSetSpellCD(Player* player, uint32 spell_id);
//进lua判断开始施法前的逻辑
extern bool DoHandleSpellStart(Player* player, uint32 spell_slot, uint16 tar_x, uint16 tar_y, Unit* target, uint32 &spellId);
//触发被动技能
extern int DoHandlePassiveEffect(Unit* caster_ptr, Unit* target_ptr, uint32 dispatchType);
//当血量发生改变
extern int DoOnHpChanged(Unit* caster_ptr, uint32 prev);
//进lua判断采集物的逻辑
extern int DoHandlePickGameObject(Player* player, uint32 gameObjectId);
//获得地图的一些初始值
extern int DoGetInitMapData(Map *map);
//应用服通知场景服消耗元宝或铜钱做些什么
extern int DoScenedComsumeMoney(Player *player, uint8 money_type, uint8 use_type,const string &param);
//复活后干点啥
extern int DoAfterRespawn(Map* map, Unit* respawner);
//与运算(任务标志位)
extern int LuaHasFlag(lua_State* scriptL);
//获取服务器运行时间
extern int LuaGetMsTime(lua_State *scriptL);
//lua脚本释放技能
extern int LuaDoSpellCastScript(lua_State *scriptL);
//根据地图id判断所在地图是否为跨服地图
extern bool DoIsKuafuMapID(uint32 map_id);
//获取主城复活点
extern int DoGetRespawnPos(vector<uint16> &v);
//玩家传送
extern int DoPlayerTeleport(Player* player);
//场景服干点什么
extern int DoScenedDoSomething(Player *player, uint32 type, uint32 data, string str);
//根据地图id判断所在地图是否为世界地图
extern bool DoIsWorldMapID(uint32 map_id);
//判断item_entry是不是资源, 如果是 money_type 不为uint32(-1)
extern bool DoGetMoneyType(uint32 item_entry, uint32 &money_type);
// 是否是冒险地图
extern int DoIsRiskMap(uint32 mapid);
// 获得冒险地图的传送信息
extern int DoScenedGetRiskTeleportInfo(Player* player, int &mapid, float &x, float &y, string &generalId);
// 地图是否有时间限制
extern bool isMapTimeLimit(uint32 mapid);

extern void DoTerminalContinuousAction(Unit* unit);
#endif

