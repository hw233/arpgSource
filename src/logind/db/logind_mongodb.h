#ifndef _LOGIND_MONGODB_H_
#define _LOGIND_MONGODB_H_

#include <db_wrap/mongo_db.h>

class LogindObjectManager;
class LogindConfig;
class PlayerBase;

class LogindMongoDB:public MongoDB
{
	friend class GuidManager;
public:
	LogindMongoDB(LogindObjectManager *mgr,log_holder_t *log);
	virtual ~LogindMongoDB();

	void SetLocaldb(bool b){is_localdb = b;}
	//////////////////////////////////////////////////////////////////////////
	//保存一个对象
	void SaveBinlog(BinLogObject *obj, bool fsync = false);
	//保存一个集合
	void SaveDataset(const string &guid, bool fsync);
	/////////////////////////////////////////////////////////////////////////
	//世界变量
	bool Load_GlobalValue();
	//////////////////////////////////////////////////////////////////////////
	//账户表
	//把所有账户表LOAD进内存
	//插入一个新的账户
	account_table *UpsertAccount(string account,string pid, string sid,string uid, int is_FCM, string ip, string platdata = "",int gmlevel = -1);		

	//////////////////////////////////////////////////////////////////////////
	//角色表
	//获取主玩家对象
	void LoadPlayer(const string& account, const string &player_guid, vector<GuidObject*> &vec, std::function<void(bool,vector<GuidObject*>&)> fun);
	//用同步的方式load取玩家数据,仅用于web接口方式访问,本接口不允许其他地方调用，如若万不得已，需要linbc签字画押方可使用。
	bool LoadPlayer(const string& account, const string &player_guid, vector<GuidObject*> &vec);

	//根据角色名称取得玩家guid
	string FindGuidByName(string name, bool &has_err);
	//插入角色名称
	void InsertGuidByName(string guid,string name);

	typedef std::function<void(bool,vector<char_create_info>&)> CharListDelegate;
	//根据帐号获取角色列表
	void GetCharsList(string account, CharListDelegate  func);

	//新增角色
	void AddChars(string guid, string account, string name, string ip, string data, string data_str);	
	//更新角色在线状态
	//如果guid为空,则更新全部
	void UpdateCharsOnline(string guid, uint8 status);
	//删号
	void DelChars(string guid);
	//根据服务器load角色名
	void LoadNameFromServer(string server_name);

	//根据玩家GUID获取account
	string FindAccountByGuid(string guid);

	//重新载入服务器配置从配置表
	void ReloadSeverNameByConfig();

	//读取合服状态
	bool LoadMergeServerStatus(int32 opt_status,int32 merge_status,std::function<void(merge_info*)> cb);
	//设置合服状态
	void UpdateMergeServerStatus(int32 new_status,int32 merge_status,string merge_server);
	//更新server_info合服状态
	void UpdateSeverInfoMergeStatus();
	//保存本服最大GUID
	void SaveServerMaxGuid();
	//获取服务器最大GUID
	void GetServerMaxGuid(string server_name,uint32 &player_max);

	//根据服务器名更新服务器列表
	bool UpdateServerListInfo(string server_name);

	//根据服务器名load最大GUID
	uint32 LoadMaxGuidFromServerName(string server_name, EObjectTypePrefix type);
	
	//读取开服时间
	uint32 LoadKaiFuShiJian();
	//设置开服时间
	void UpdateKaiFuShiJian(uint32 open_time);
	//设置合服对照表
	void UpdateMergeServerList(string server_name);
	//////////////////////////////////////////////////////////////////////////
	//获取客户端版本需求
	void GetVersionsRequirements(std::function<void(uint32,const string&,const string&,uint32)>);
		
	void DbOperGameConfig(std::function<void(string,string)> fun);
	//////////////////////////////////////////////////////////////////////////
	//GM命令充值
	void SetGmRechargeNum(const char* account,uint32 amount, uint32 type);

	//判断玩家是否存在
	bool FindFriendByGuid(string guid, PlayerBase *player);

	//GM命令更新服务端
	void ServerUpdate(uint32 server_type);

	//获取排行榜数据
	void LoadRankListInfo(string server_name, string &data, string &data_str);
	//全量更新玩家所有信息
	void UpdatePlayerDBInfo(const string &guid);
	//更新account的gm_level
	void UpdateAccountGMLevel(uint32 gm_level,string account,string server_name);

	//获取统计数据
	void LoadCounterInfo(string server_name, string &data, string &data_str);

	//删除binlog
	void DeleteBinlog(const string &guid,const string &owner);
	//根据玩家GUID获取account
	string GetAccountFromGuid(string str);
protected:
	//保存一批对象
	void SaveBinlogVec(vector<GuidObject*> vec, bool fsync = false);
	//保存玩家数据
	void SavePlayer(BinLogObject *player, bool fsync = false);
	//保存其他玩家
	void SavePlayerOtherInfo(BinLogObject *blobj, bool fsync = false);
	//保存世界变量
	void SaveGlobalValue(BinLogObject *binlog);
	//指定数据库保存binlogobject
	void SaveBinlog(string data_name,BinLogObject *blobj, bool fsync = false);
	//删除帮派信息
	void DelFaction(const string &guid);
	//是否拥有完整数据
	bool is_localdb;									
private:
	void LoadMaxGuid(BinLogObject *binlog, const char *table_name, EObjectTypePrefix type);
	void LoadOtherMaxGuid(BinLogObject *binlog, EObjectTypePrefix type);
	void Load_GlobalValue(BinLogObject* binlog, const char *guid, bool load_db = true, std::function<void()> = nullptr);

	LogindObjectManager *m_obj_mgr;
	vector<string>				m_loaded_servername;	//已经载入玩家名称的服务器	
};


#endif // !_LOGIND_MONGODB_H_

