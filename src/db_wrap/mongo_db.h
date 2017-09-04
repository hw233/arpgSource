#ifndef _SHARED_MONGO_DB_H_
#define _SHARED_MONGO_DB_H_

#include <comm/common_stl.h>
#include <functional>
#include "db_wrap/mongo_wrap.h"
#include "db_wrap/AsyncMongoDBTask.h"

struct log_holder_t;
class AsyncMongoDB;

//////////////////////////////////////////////////////////////////////////
class MongoDB:public MongoWrap
{
public:
	//解密连接字符串
	static bool DecodeConnectionString(string conn_str,string& host,string& port,string& user, string& passwd, string &db_prefix, log_holder_t *logger = nullptr);

	MongoDB(log_holder_t *log);
	virtual ~MongoDB();

	enum DB_TYPE
	{
		DBT_CHAR,		//角色库
		DBT_SERVER,		//服务器信息库，帐号信息也在此
		DBT_LOG,		//日志库
		DBT_WORLD,		//世界服信息
		DBT_CONFIG,		//服务器集群配置信息
	};

	virtual bool Open(string conn_str,int log_level=-1,string log_path="",int worker_num = 0);	
	virtual void Close(){};

	static string GetCharNS(string ns){return "char."+ns;}
	static string GetServerNS(string ns){return "server."+ns;};
//	static string GetWorldNS(string ns){return "world."+ns;};
	static string GetConfigNS(string ns){return "cow_config."+ns;}
	static string GetGMIntfNs(string ns){return "gm_intf."+ns;}
	static string GetOpertoolNS(string ns){return "oper_tool."+ns;}
	static string GetCharSeverNameNS(string server_name, string ns){return "char_"+server_name+"."+ns;}

	static string GetServerName(string guid);

	//从玩家的名称里面获得pid/sid组合的servername
	//输入 "2,1,帅哥" 返回 "2_1"
	static string GetServerNameFromCharName(string name);

	//从帐户表中取得服务器名称
	//输入 2_1_123456 返回 2_1
	static string GetServerNameFromAccount(string account);

	//从玩家姓名中去除pid/sid
	//输入 "2,1,帅哥" 返回 "帅哥"
	static string GetLastNameFromCharName(string name);

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

	void fromString(char str[],string v)
	{
		strcpy(str,v.c_str());
	}

	void AsyncUpdate(string ns,const MongoDB::Map& where,MongoDB::Map& values);
	void AsyncUpsert(string ns,const MongoDB::Map& where,MongoDB::Map& values);
	void AsyncInsert(string ns,MongoDB::Map& values);

	//增加异步查询接口
	void AsyncQuery(string ns,const MongoDB::Map& where, AsyncMongoDBTask::CB cb,string orderKey = "", int32_t bySort = 0,int32_t limitReslut = 0);
	void AsyncLoad(string ns,const MongoDB::Map& where, AsyncMongoDBTask::CB cb);

	Mongo_Result QueryPlayerInfo(string ns,string server_name,MongoDB::Map& where,Results& results);
	Mongo_Result QueryPlayerInfo(string ns,string server_name,MongoDB::Map& where,Results& results,MongoDB::Map& outfield);
	Mongo_Result QueryPlayerInfo(string ns,string server_name,MongoDB::Map& where,Results& results,string orderKey, int32_t bySort,int32_t limitReslut);
	Mongo_Result LoadPlayerInfo(string ns,string server_name,MongoDB::Map& where,MongoDB::Map& values);
	string GetPlayerInfo(string ns,string server_name) {return "char_" + server_name + "." + ns;}
	
	//执行异步回调
	void UpdateAsync();

	//取得异步数据库对象
	AsyncMongoDB *get_async_db(){return async_db_;}
private:
	log_holder_t *log_;
	AsyncMongoDB *async_db_;

	string db_host_;
	int db_port_;
	string db_user_;
	string db_passwd_;	
	string db_prefix_;
};


#endif // !_SHARED_MONGO_DB_H_

