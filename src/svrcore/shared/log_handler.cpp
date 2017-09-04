#include <comm/common_stl.h>
#include <net/packet.h>
#include <protocol/WorldPacket.h>
#include "protocol/internal.h"
#include "log_handler.h"

std::function<bool(WorldPacket& pkt)> fp_send_to_policed;

set<uint32> m_heal;
set<uint32> m_loot;

struct InitLogHandler
{
	InitLogHandler()
	{

		m_heal.insert(3);
		m_heal.insert(4);
		m_heal.insert(28);
		m_heal.insert(29);
		m_heal.insert(30);
		m_heal.insert(31);
		m_heal.insert(32);
		m_heal.insert(33);
		m_heal.insert(34);
		m_heal.insert(35);
		m_heal.insert(36);
		m_heal.insert(37);
		m_heal.insert(38);
		m_heal.insert(39);
		m_heal.insert(40);
		m_heal.insert(41);
		m_heal.insert(1919);
		m_heal.insert(1920);
		m_heal.insert(1921);
		m_heal.insert(1922);
		m_heal.insert(1923);
		m_heal.insert(1924);

		m_loot.insert(1095);
		m_loot.insert(1096);
		m_loot.insert(1100);
		m_loot.insert(1101);
		m_loot.insert(1102);
		m_loot.insert(1103);
		m_loot.insert(1104);
		m_loot.insert(1105);
		m_loot.insert(1106);
	}	
} __InitLogHandler;

void InitLogPacket(WorldPacket &pkt,uint32 log_type, const string & account, const string &player_id)
{
	uint32 sec, usec;
	struct timeval tmnow;
	gettimeofday(&tmnow, NULL) ;
	sec = tmnow.tv_sec;
	usec = tmnow.tv_usec;
	pkt << log_type << sec << usec << account << player_id;
}



//写入聊天log
void WriteChatLog(uint32 chat_type, const string & account, const string &player_from, const string &player_to, const char* content, uint32 level, int gmlevel, const string &name_to)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_CHAT,account, player_from);	
	_pkt << chat_type << player_to << (const char*)content << level << name_to;
	if(gmlevel != -1)
		_pkt << gmlevel;
	fp_send_to_policed(_pkt);
}

//记录用户充值行为
void WriteRecharge(const string & account, const string &player_id, double amount, double money, uint32 balance, uint32 level, const string &order)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_RECHARGE,account, player_id);	
	_pkt  << balance << amount << money << balance << level << order;
	fp_send_to_policed(_pkt);
}

//记录用户的交易行为
void WriteDeal(const string & account, const string &player_id, uint32 type, uint32 buy_mapid, uint32 buy_level, uint32 buy_cp, uint32 buy_yd, vector<deal_goods> *buy_goods,
	uint32 sell_sid, const string & sell_roleid, const string & sell_user, uint32 sell_mapid, uint32 sell_level, uint32 sell_cp, uint32 sell_yd, vector<deal_goods> *sell_goods)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_DEAL, account, player_id);
	_pkt << type << buy_mapid << buy_level << buy_cp << buy_yd;
	if(buy_goods == NULL)
		_pkt << 0;
	else
	{
		_pkt<< (uint32)buy_goods->size();
		for (vector<deal_goods>::iterator it = buy_goods->begin(); it != buy_goods->end(); ++it)
		{
			_pkt << *it;
		}
	}

	_pkt << sell_sid << sell_roleid << sell_user << sell_mapid << sell_level << sell_cp << sell_yd;
	if(sell_goods == NULL)
		_pkt << 0;
	else
	{
		_pkt<< (uint32)sell_goods->size();
		for (vector<deal_goods>::iterator it = sell_goods->begin(); it != sell_goods->end(); ++it)
		{
			_pkt << *it;
		}
	}
	fp_send_to_policed(_pkt);
}

//记录额外的元宝收入
void WriteYbIncome(const string & account, const string &player_id, double amount, double balance, uint32 level, uint32 reason, double old_value, const string &trace_id, uint32 p1, uint32 p2,
				   double unit_price,uint16 item_bind, uint16 item_del, uint32 quest)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_YB_INCOME, account, player_id);
	_pkt << amount << balance << level << reason << old_value << trace_id << p1 << p2 << unit_price << item_bind << item_del << quest;
	fp_send_to_policed(_pkt);
}

//记录额外的元宝消费
void WrtieYbExpend(const string & account, const string &player_id, double amount, double balance, uint32 level, uint32 reason, double old_value, const string &trace_id, uint32 p1, uint32 p2,
				   double unit_price,uint16 item_bind, uint16 item_del, uint32 quest)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_YB_EXPEND, account, player_id);
	_pkt << amount << balance << level << reason << old_value << trace_id << p1 << p2 << unit_price << item_bind << item_del << quest;
	fp_send_to_policed(_pkt);	
}

//记录用户登录行为
void WriteLogin(const string & account, const string &player_id, uint32 level, const char *ip, uint32 map, const char *group, double power)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_LOGIN, account, player_id);	
	_pkt << level << ip << map << group << power;
	fp_send_to_policed(_pkt);
}

//记录用户登出行为
void WriteLogout(const string & account, const string &player_id, uint32 level, const char *ip, uint32 map, const char *group)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_LOGOUT, account, player_id);	
	_pkt << level << ip << map << group;
	fp_send_to_policed(_pkt);
}

//记录用户创建角色行为
void WriteCreateRole(const string & account, const string &player_id, const char *rolename, string ip)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_CREATE_ROLE, account, player_id);	
	_pkt << rolename << ip;
	fp_send_to_policed(_pkt);
}

//记录用户死亡事件
void WriteDeath(const string & account, const string &player_id, const char *reason, uint32 level, uint32 map, uint32 is_player)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_DEATH, account, player_id);	
	_pkt << reason << level << map << is_player;
	fp_send_to_policed(_pkt);
}

//记录用户接受任务的行为
void WriteAcceptTask(const string & account, const string &player_id, uint32 taskid, uint32 type, uint32 map)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_ACCEPT_TASK, account, player_id);	
	_pkt << taskid << type << map;
	fp_send_to_policed(_pkt);
}

//记录用户完成任务的行为
void WriteTask(const string & account, const string &player_id, uint32 taskid, uint32 map_id, uint32 result)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_TASK, account, player_id);	
	_pkt << taskid << map_id << result;
	fp_send_to_policed(_pkt);
}

//记录用户复活
void WriteRelive(const string & account, const string &player_id, uint32 type, uint32 map)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_RELIVE, account, player_id);	
	_pkt << type << map;
	fp_send_to_policed(_pkt);
}


//记录用户升级事件
void WriteUpgrade(const string & account, const string &player_id, uint32 level, uint32 map, double power)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_UPGRADE, account, player_id);	
	_pkt << level << map << power;
	fp_send_to_policed(_pkt);
}

//记录用户金币所得行为
void WriteSilver(const string & account, const string &player_id, double sum, uint32 status, uint32 map, double old_value, double new_value, double warehouse_value,
				 const string &trace_id, uint32 p1, uint32 p2,double unit_price,uint16 item_bind, uint16 item_del, uint32 quest,uint32 level)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_GOLD, account, player_id);	
	_pkt << sum << status << map << old_value << new_value << warehouse_value << trace_id << p1 << p2 << unit_price << item_bind << item_del << quest << level;
	fp_send_to_policed(_pkt);
}

//记录用户的绑定元宝行为
void WriteBindGold(const string & account, const string &player_id, uint16 oper_type, double v,double old_value, double new_value, 
				   const string &trace_id, uint32 item_id, uint32 count, uint32 level, uint32 map_id,
				   double unit_price,uint16 item_bind, uint16 item_del, uint32 quest)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_BIND_GOLD, account, player_id);	
	_pkt << oper_type << v << old_value << new_value << trace_id << item_id << count << level << map_id << unit_price << item_bind << item_del << quest;
	fp_send_to_policed(_pkt);
}

//记录用户领取新手卡的行为
void WriteNewCardReceive(const string & account, const string &player_id, const char * card_id, uint32 map, uint32 x, uint32 y)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_NEW_CARD_RECEIVE, account, player_id);	
	_pkt << card_id << map << x << y;
	fp_send_to_policed(_pkt);
}

//记录摆摊行为
void WriteStall(const string & account, const string & player_id, const string & item_id, uint32 entry, uint32 amount, uint32 ingot_price, uint32 silver_price, uint32 stall_type)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_STALL, account, player_id);
	_pkt << item_id << entry << amount << ingot_price << silver_price << stall_type;
	fp_send_to_policed(_pkt);	
}

//记录通缉名单包行为
void WritePacket(const string & account, const string &player_id, packet *pkt)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_WANTED_PACKET, account, player_id);	
	_pkt << time(NULL) << ms_time() << pkt->head->optcode << pkt->content;
	fp_send_to_policed(_pkt);
}

//记录所有物品日志
void WriteItemLog(const string & account, const string &player_id, uint32 itemid, uint32 amount, uint32 new_amount, uint32 opid, uint32 level, int32 bind_mode, double balance, double money, uint32 map)
{
	//药品不写
	if(opid == LOG_ITEM_OPER_TYPE_USE && m_heal.find(itemid) != m_heal.end())
		return;
	//战利品披风不写
	if(opid == LOG_ITEM_OPER_TYPE_LOOT && m_loot.find(itemid) != m_loot.end())
		return;
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_ITEM_LOG, account, player_id);	
	_pkt << itemid << amount << new_amount << opid << level << bind_mode << balance << money << map;
	fp_send_to_policed(_pkt);
}

//记录用户切换地图的行为
void WriteMap(const string & account, const string &player_id, uint32 level, uint32 nonce_map, uint32 to_map)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_MAP, account, player_id);	
	_pkt << level << nonce_map << to_map;
	fp_send_to_policed(_pkt);
}

//记录在线人数
void WriteOnline(uint32 account_count, uint32 player_count)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_ONLINE, "", "");
	_pkt << account_count << player_count;
	fp_send_to_policed(_pkt);
}


//记录用户进入游戏主界面(未建角色)
void WriteGameEnter(const string & account, const char *ip)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_GAME_ENTER, account, "");
	
	_pkt << ip;

	fp_send_to_policed(_pkt);
}

//记录玩家选择阵营的情况
void WriteChooseFaction(const string & account, const string &player_id, uint32 oper_type, uint32 result)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_CHOOSE_FACTION, account, player_id);

	_pkt << oper_type << result;

	fp_send_to_policed(_pkt);
}

//记录运营活动奖励信息
void WriteActivityInfo(const string & account, const string &player_id,uint32 type, uint32 acti_version, uint32 acti_id)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_ACTIVITY_INFO, account, player_id);

	_pkt << type << acti_version << acti_id;
	fp_send_to_policed(_pkt);
}

//记录用户打副本的行为
void WriteTranscript(const string & account, const string &player_id, uint32 starttime, uint32 timelong, uint32 num, uint32 level, uint32 map_id)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_TRANSCRIPT, account, player_id);

	_pkt << starttime << timelong << num << level << map_id;

	fp_send_to_policed(_pkt);
}

//记录客户端信息
void WriteClientInfo(const string &player_id, string& client, string& browser, string& isdebug, string& language, string& version, string& flash_version, string& x, string& y, string& os)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_CLIENT_INFO, "", player_id);

	_pkt << client << browser << isdebug << language << version << flash_version << x << y << os;
	fp_send_to_policed(_pkt);
}

static char log_string_content[LOG_STRING_MAX_LEN];
//记录攻击包
void WriteAttackPacker(const string & account, const string &player_id, uint32 p_id, uint32 type, const char *msg,...)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_ATTACK_PACKET, account, player_id);

	_pkt << p_id << type;
	
	if (msg)
	{
		va_list ap;		
		va_start(ap, msg);
		vsnprintf(log_string_content, LOG_STRING_MAX_LEN, msg, ap);
		va_end(ap);
	}
	else
	{
		memset(log_string_content, 0, LOG_STRING_MAX_LEN);
	}
	
	_pkt << (string)log_string_content;

	fp_send_to_policed(_pkt);
}

//记录乱七八糟的自定义日志
void WriteFormatLog(const char *msg)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_FORMAT_LOG, "", "");
	
	/*if (msg)
	{
		vsnprintf(log_string_content, LOG_STRING_MAX_LEN, msg, va);	
	}
	else
	{
		memset(log_string_content, 0, LOG_STRING_MAX_LEN);
	}*/

	//*_pkt << (string)log_string_content;
	_pkt << (string)msg;

	fp_send_to_policed(_pkt);
}

//记录客户端日志
void WriteClientLog(const string & account, const string &player_id,const char *msg)
{
	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(_pkt, (uint32)LOG_FILE_TYPE_CLIENT_LOG, account, player_id);

	_pkt << (string)msg;

	fp_send_to_policed(_pkt);
}

////lua日志统一接口
//int LuaWriteLog(lua_State* scriptL)
//{
//	CHECK_LUA_NIL_PARAMETER(scriptL);
//	int n = lua_gettop(scriptL);
//	ASSERT(n >= 3);
//	uint32 log_type = (uint32)LUA_TONUMBER(scriptL, 1);
//	const char *player_id = LUA_TOSTRING(scriptL, 2);
//	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
//	InitLogPacket(_pkt, log_type, "", player_id);
//	for (int i = 3; i <= n; i++)
//	{
//		if(lua_type(scriptL, i) == LUA_TNUMBER)
//		{
//			_pkt << (uint32)LUA_TONUMBER(scriptL, i);
//		}
//		else
//		{
//			_pkt << LUA_TOSTRING(scriptL, i);
//		}
//	}
//	fp_send_to_policed(_pkt);
//	return 0;
//}

////lua道具所有行为日志
//int LuaWriteItemLog(lua_State* scriptL)
//{
//	CHECK_LUA_NIL_PARAMETER(scriptL);
//	int n = lua_gettop(scriptL);
//	ASSERT(n >= 3);
//	uint32 log_type = (uint32)LUA_TONUMBER(scriptL, 1);
//	const char *player_id = LUA_TOSTRING(scriptL, 2);
//	const char *account = LUA_TOSTRING(scriptL,12);
//	WorldPacket _pkt(INTERNAL_OPT_WRITE_LOG);
//	InitLogPacket(_pkt, log_type, account, player_id);
//	uint32 entry = (uint32)LUA_TONUMBER(scriptL, 3);
//	uint32 count = (uint32)LUA_TONUMBER(scriptL, 4);
//	uint32 new_count = (uint32)LUA_TONUMBER(scriptL, 5);
//	uint32 income_type = (uint32)LUA_TONUMBER(scriptL, 6);
//	uint32 level = (uint32)LUA_TONUMBER(scriptL, 7);
//	uint32 bing = (uint32)LUA_TONUMBER(scriptL, 8);
//	double balance = (double)LUA_TONUMBER(scriptL, 9);
//	double money = (double)LUA_TONUMBER(scriptL, 10);
//	uint32 map_id = (uint32)LUA_TONUMBER(scriptL, 11);
//	_pkt << entry << count << new_count << income_type << level << bing << balance << money << map_id;
//	fp_send_to_policed(_pkt);
//	return 0;
//}

//攻击包日志
int LuaWriteAttackPacker(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n >= 4);
	const string &player_id = LUA_TOSTRING(scriptL, 1);
	uint32 opcode = (uint32)LUA_TONUMBER(scriptL, 2);
	uint32 accack_packet_type = (uint32)LUA_TONUMBER(scriptL, 3);
	const char *str = LUA_TOSTRING(scriptL, 4);
	//能进到lua的，肯定有角色了
	WriteAttackPacker("", player_id, opcode, accack_packet_type, str);
	return 0;
}

//副本日志
int LuaWriteFubenLog(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 6);
	const string &player_id = LUA_TOSTRING(scriptL, 1);
	uint32 cur_time = (uint32)LUA_TONUMBER(scriptL, 2);
	uint32 all_time = (uint32)LUA_TONUMBER(scriptL, 3);
	uint32 kill_num = (uint32)LUA_TONUMBER(scriptL, 4);
	uint32 player_lv = (uint32)LUA_TONUMBER(scriptL, 5);
	uint32 map_id = (uint32)LUA_TONUMBER(scriptL, 6);
	WriteTranscript("", player_id, cur_time, all_time, kill_num, player_lv, map_id);
	return 0;
}

//1001日志
void WriteObjectLoss(const string &account, const string &player_id, const string &obj_guid, uint32 op_type)
{
	WorldPacket pkt(INTERNAL_OPT_WRITE_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TYPE_OBJECT_LOSS, account, player_id);
	pkt << obj_guid << op_type;

	fp_send_to_policed(pkt);
}
/////////////////////////////////////////////////////////
//腾讯专用接口
//根据平台数据，获取个别信息
string GetPlatInfo(const string &platdata,const string &key)
{
	Tokens tokens;
	StrSplit(tokens, platdata, "#_#");
	if(tokens.empty() ||tokens[0].empty())
		return "";
	std::map<string,string> plat_map;
	for(size_t i = 0; i < tokens.size();)
	{
		plat_map[tokens[i]] = tokens[i+1];
		i += 2;
	}
	return plat_map[key];
}

//TX用户记录日志
void WriteTXUserLog(const string &account,const string &player_id,const string &player_name,uint32 player_level,const string &platform,uint32 create_time)
{
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_USER, account, player_id);
	pkt << player_name << player_level << platform << create_time;

	fp_send_to_policed(pkt);
}

//TX角色记录日志
void WriteTXPlayerLog(const string &account,const string &player_id,const string &player_name,uint32 player_level,const string &platform,uint32 create_time)
{
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_PLAYER, account, player_id);
	pkt << player_name << player_level << platform << create_time;

	fp_send_to_policed(pkt);
}


//TX用户升级记录日志
void WriteTXUpradeLog(const string &account,const string &player_id,const string &player_name,uint32 old_level,uint32 new_level,uint32 sucess,uint8 upgradeType,const string &params,uint32 create_time)
{
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_UPGRADELOG, account, player_id);
	pkt << player_name << old_level << new_level << sucess << upgradeType << params << create_time;

	fp_send_to_policed(pkt);
}

//TX用户新手日志
void WriteTXNewTaskLog(const string &account,const string &player_id,const string &player_name,uint32 taskid,uint32 status,uint32 quality,uint32 create_time)
{
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_NEWTASK, account, player_id);
	pkt << player_name << taskid << status << quality << create_time;

	fp_send_to_policed(pkt);
}

//TX用户登录日志
void WriteTXLoginLog(const string &account,const string &player_id,const string &player_name,uint32 player_level,const string &siteUrl,const string &adFrom,const string &platform,const string &ip,uint32 create_time)
{
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_LOGIN, account, player_id);
	pkt << player_name << player_level << siteUrl << adFrom << platform << ip << create_time;

	fp_send_to_policed(pkt);
}

//TX用户离线日志
void WriteTXOffineLog(const string &account,const string &player_id,const string &player_name,uint32 player_level,const string &siteUrl,const string &adFrom,
							 const string &platform,const string &ip,uint32 online_time,uint32 history_time,uint32 create_time)
{
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_OFFLINE, account, player_id);
	pkt << player_name << player_level << siteUrl << adFrom << platform << ip << online_time << history_time << create_time;

	fp_send_to_policed(pkt);
}

//TX用户在线日志
void WriteTXOnlineLog(uint32 allcount,uint32 cashcount)
{
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_ONLINE, "", "");
	pkt << allcount << cashcount;

	fp_send_to_policed(pkt);
}

//TX用户元宝日志
void WriteTXMoneyLog(const string &account,const string &player_id,const string &player_name,uint32 rootid,uint32 type_id,double old_value, double new_value,double gold,uint32 gold_ticke,uint32 goods_id,uint32 num,uint32 create_time,const string &platform)
{
	WorldPacket pkt(INTERNAL_OPT_TENCENT_LOG);
	InitLogPacket(pkt, (uint32)LOG_FILE_TENCENT_MONEYLOG, account, player_id);
	pkt << player_name << rootid << type_id << old_value << new_value << gold << gold_ticke << goods_id << num << create_time << platform;

	fp_send_to_policed(pkt);
}
