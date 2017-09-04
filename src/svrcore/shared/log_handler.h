#ifndef _LOG_HANDLER_H_
#define _LOG_HANDLER_H_

#include <comm/common_stl.h>
#include <net/packet.h>
#include "object/SharedDef.h"
#include "lua_helper.h"
#include <functional>

#define LOG_STRING_MAX_LEN 10000

extern std::function<bool(WorldPacket& pkt)> fp_send_to_policed;

extern void InitLogPacket(WorldPacket &pkt, uint32 log_type, const string & account, const string &player_id);

//写入聊天log
extern void WriteChatLog(uint32 chat_type, const string & account, const string &player_from, const string &player_to, const char* content, uint32 level, int gmlevel, const string &name_to);

//记录用户充值行为
extern void WriteRecharge(const string & account, const string &player_id, double amount, double money, uint32 balance, uint32 level, const string &order);

//记录用户的交易行为
extern void WriteDeal(const string & account, const string &player_id, uint32 type, uint32 buy_mapid, uint32 buy_level, uint32 buy_cp, uint32 buy_yd, vector<deal_goods> *buy_goods,
	uint32 sell_sid, const string & sell_roleid, const string & sell_user, uint32 sell_mapid, uint32 sell_level, uint32 sell_cp, uint32 sell_yd, vector<deal_goods> *sell_goods);

//记录额外的元宝收入
extern void WriteYbIncome(const string & account, const string &player_id, double amount, double balance, uint32 level, uint32 reason,
						  double old_value, const string &trace_id, uint32 p1, uint32 p2,
						  double unit_price,uint16 item_bind, uint16 item_del, uint32 quest);

//记录额外的元宝消费
extern void WrtieYbExpend(const string & account, const string &player_id, double amount, double balance, uint32 level, uint32 reason, 
						  double old_value, const string &trace_id, uint32 p1, uint32 p2,
						  double unit_price,uint16 item_bind, uint16 item_del, uint32 quest);

//记录用户登录行为
extern void WriteLogin(const string & account, const string &player_id, uint32 level, const char *ip, uint32 map, const char *group, double power);

//记录用户登出行为
extern void WriteLogout(const string & account, const string &player_id, uint32 level, const char *ip, uint32 map, const char *group);

//记录用户创建角色行为
extern void WriteCreateRole(const string & account, const string &player_id, const char *rolename, string ip);

//记录用户死亡事件
extern void WriteDeath(const string & account, const string &player_id, const char *reason, uint32 level, uint32 map, uint32 is_player);

//记录用户接受任务的行为
extern void WriteAcceptTask(const string & account, const string &player_id, uint32 taskid, uint32 type, uint32 map);

//记录用户完成任务的行为
extern void WriteTask(const string & account, const string &player_id, uint32 taskid, uint32 map_id, uint32 result);

//记录用户复活
extern void WriteRelive(const string & account, const string &player_id, uint32 type, uint32 map);

//记录用户升级事件
extern void WriteUpgrade(const string & account, const string &player_id, uint32 level, uint32 map, double power);

//记录用户金币所得行为
extern void WriteSilver(const string & account, const string &player_id, double sum, uint32 status, uint32 map, double old_value, double new_value, double warehouse_value,
						const string &trace_id, uint32 p1, uint32 p2,double unit_price,uint16 item_bind, uint16 item_del, uint32 quest,uint32 level);

//记录用户的绑定元宝行为
extern void WriteBindGold(const string & account, const string &player_id, uint16 oper_type, double v,double old_value, double new_value, 
						  const string &trace_id, uint32 item_id, uint32 count, uint32 level, uint32 map_id,
						  double unit_price,uint16 item_bind, uint16 item_del, uint32 quest);

//记录用户领取新手卡的行为
extern void WriteNewCardReceive(const string & account, const string &player_id, const char * card_id, uint32 map, uint32 x, uint32 y);

//记录摆摊行为
extern void WriteStall(const string & account, const string & player_id, const string & item_id, uint32 entry, uint32 amount, uint32 ingot_price, uint32 silver_price, uint32 stall_type);

//记录通缉名单包行为
extern void WritePacket(const string & account, const string &player_id, packet *pkt);

//记录所有物品日志，bind_mode 1为不绑定，2是绑定
extern void WriteItemLog(const string & account, const string &player_id, uint32 itemid, uint32 amount, uint32 new_amount, uint32 opid, uint32 level, int32 bind_mode, double balance = 0, double money = 0, uint32 map = 0);

//记录用户切换地图的行为
extern void WriteMap(const string & account, const string &player_id, uint32 level, uint32 nonce_map, uint32 to_map);

//记录在线人数
extern void WriteOnline(uint32 account_count, uint32 player_count);

//记录用户进入游戏主界面(未建角色)
extern void WriteGameEnter(const string & account, const char *ip);

//记录玩家选择阵营的情况
extern void WriteChooseFaction(const string & account, const string &player_id, uint32 oper_type, uint32 result);

//记录运营活动奖励信息
extern void WriteActivityInfo(const string & account, const string &player_id,uint32 type, uint32 acti_version, uint32 acti_id);

//记录用户打副本的行为
extern void WriteTranscript(const string & account, const string &player_id, uint32 starttime, uint32 timelong, uint32 num, uint32 level, uint32 map_id);

//记录客户端信息
extern void WriteClientInfo(const string &player_id, string& client, string& browser, string& isdebug, string& language, string& version, string& flash_version, string& x, string& y, string& os);

//记录攻击包
extern void WriteAttackPacker(const string & account, const string &player_id, uint32 p_id, uint32 type, const char *msg,...);

//记录生肖唤醒记录
//extern void WriteAttackPacker(const string & account, const string &player_id, uint32 old_pro, uint32 new_pro,);

//记录乱七八糟的自定义日志
extern void WriteFormatLog(const char *msg);

//记录客户端日志
extern void WriteClientLog(const string & account, const string &player_id,const char *msg);

//攻击包日志
int LuaWriteAttackPacker(lua_State *scriptL);

//lua日志统一接口
//int LuaWriteLog(lua_State* scriptL);

//lua道具所有行为日志
//int LuaWriteItemLog(lua_State* scriptL);

//副本日志
int LuaWriteFubenLog(lua_State *scriptL);

/////////////////////////////////////////////////////////
//腾讯专用接口
//根据平台数据，获取个别信息
string GetPlatInfo(const string &platdata,const string &key);
//TX用户记录日志
extern void WriteTXUserLog(const string &account,const string &player_id,const string &player_name,uint32 player_level,const string &platform,uint32 create_time);

//TX角色记录日志
extern void WriteTXPlayerLog(const string &account,const string &player_id,const string &player_name,uint32 player_level,const string &platform,uint32 create_time);


//TX用户升级记录日志
extern void WriteTXUpradeLog(const string &account,const string &player_id,const string &player_name,uint32 old_level,uint32 new_level,uint32 sucess,uint8 upgradeType,const string &params,uint32 create_time);

//TX用户新手日志
extern void WriteTXNewTaskLog(const string &account,const string &player_id,const string &player_name,uint32 taskid,uint32 status,uint32 quality,uint32 create_time);

//TX用户登录日志
extern void WriteTXLoginLog(const string &account,const string &player_id,const string &player_name,uint32 player_level,const string &siteUrl,const string &adFrom,const string &platform,const string &ip,uint32 create_time);

//TX用户离线日志
extern void WriteTXOffineLog(const string &account,const string &player_id,const string &player_name,uint32 player_level,const string &siteUrl,const string &adFrom,
							 const string &platform,const string &ip,uint32 online_time,uint32 history_time,uint32 create_time);

//TX用户在线日志
extern void WriteTXOnlineLog(uint32 allcount,uint32 cashcount);

//TX用户元宝日志
extern void WriteTXMoneyLog(const string &account,const string &player_id,const string &player_name,uint32 rootid,uint32 type_id,double old_value, double new_value,double gold,uint32 gold_ticke,uint32 goods_id,uint32 num,uint32 create_time,const string &platform);

//1001日志
extern void WriteObjectLoss(const string &account, const string &player_id, const string &obj_guid, uint32 op_type);

/////////////////////////////////////////////////////////
#endif
