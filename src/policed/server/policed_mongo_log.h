#ifndef __POLICED_MONGO_LOG__
#define __POLICED_MONGO_LOG__

#include "policed_app.h"
#include "policed_context.h"
#include "policed_app.h"
#include "policed_object_manager.h"
#include "object/PlayerBase.h"

#ifdef WIN32
#include "db_wrap/mongo_wrap.h"
#define MongoWrap3 MongoWrap
#else
#include <db_wrap/mongo3_wrap.h>
#endif // WIN32


class PolicedMongoLog:public MongoWrap3{
public:
	PolicedMongoLog(log_holder_t *log);
	virtual ~PolicedMongoLog();

	string GetLogNS(string ns){return "logs."+ns;};
	string GetDailyNS(string ns){return "ht_daily_info.ht_daily_info"+ns;};
	string GetForgeNS(string ns){return "ht_forge_info."+ns;};
	string GetMoneyNS(string ns){return "log_gold_ingot."+ns;};
	string GetLogBaseNS(string ns){return "logs.ht_basic_info_"+ns;};
	string GetLogSystemNS(string ns){return "logs.ht_system_info_"+ns;};
	string GetServerName(string char_guid);

	virtual void Close();

	bool DecodeConnectionString(string conn_str,string& host,string& port,string& user, string& passwd, string &db_prefix, log_holder_t *logger/* = nullptr*/);
	bool Open(string conn_str,int log_level, string log_path);

	//根据玩家GUID获取account
	string GetServerNameFromAccount(string account);
	
	void UpdatePlayerLockStatus(string account_id,uint32 status);

	void UpdatePlayerGagStatus(string player_guid,uint32 end_time);

	//更新所有在线状态
	void UpdateAllHTBasicOnline(bool is_online);
	//获取basicInfo中在线玩家列表
	void GetBasicInfoOnlinePlayer(set<string> &);
	//更新某玩家为不在线
	void UpdateHTBasicOffline(string player_guid);
	//保存玩家后台基本信息
	void  SaveHTBasicInfo(PlayerBase* player,bool online);
	//保存后台锻造升级信息
	void  SaveHTForgeInfo(string guid, uint32 forge_type, uint32 forge_level);

	//保存玩家各个系统基本信息
	void  SaveHTSystemInfo(PlayerBase* player);

	//保存家族后台基本信息
	void SaveHTCorpsInfo(BinLogObject* corps, string manager_guid, string manager_name, uint32 count);

	//保存排行榜
	void  SaveHTTopInfo();
	//获取玩家排行榜数据
	string GetTopInfoFromType(uint32 top_type);
	//保存玩家首冲记录
	void SaveFirstRechargeInfo(map<string,string>& values);
	//记录GM命令
	void WriteGmCommand( const string & gm_id, string& gm_command );

	string GetCharNS(string ns){return "char."+ns;};

	void SaveBackupInfo(string guid,string save_time,uint32 level);
	//保存元宝消费信息
	void SaveGoldLog( const string &account, const string &player_id, const string &player_name, uint16 money_type, uint16 oper_type, double v,double old_value, double new_value, const string &trace_id, uint32 item_id, uint32 count, double unit_price, uint16 item_bind, uint16 item_del, uint32 level, uint32 quest);

	template<typename T>
	string toString(T t)
	{
		stringstream ss;
		ss << std::fixed << t;
		return ss.str();
	}

	template<typename T>
	void fromString(T& t,string v)
	{
		stringstream ss;
		ss << v;
		ss >> t;
	}	
private:
	log_holder_t *log_;
	string server_name_;
	string host_;
	int port_;
	string user_;
	string password_;
//	AsyncMongoDB *async_db_;
	string db_host_;
	int db_port_;
	string db_user_;
	string db_passwd_;	
	string db_prefix_;
	
};

#endif