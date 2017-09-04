#ifndef _MONGO3_WRAP_H_
#define _MONGO3_WRAP_H_

#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <fstream>
#include <mutex>
#include "db_wrap/mongo_wrap_def.h"

using std::string;

struct MogoWrapLogImpl;
struct MongoDBPtr;

//c++对mmongo操作接口包装
//约定key: i开头为整型 u为无符串整理 d为number类型 其他默认全部为字符串类型
class MongoWrap3
{
public:
	typedef std::map<string,string> Map;
	typedef std::vector<uint32_t> Values;
	typedef std::vector<string> StrValues;
	typedef std::vector<Map> Results;

	//bson与基本类型之间的转换	
public:

	MongoWrap3();
	virtual ~MongoWrap3();

	//打开数据连接
	bool Open(string host,unsigned short port,string user = "",string pwd = "",string dbname = "admin",int level=_NO_LOG, string logpath="");
//	//关闭连接
//	void Close();

	//保存数据到指定的记录当中
	bool Insert(string ns,Map& values,bool fsync = false);
	bool Upsert(string ns,const Map& where,Map& values);

	//更新数据到指定的记录当中
	bool Update(string ns,int _id,Map& values);
	bool Update(string ns,const Map& where,Map& values, bool upsert = false, bool fsync = false);

	//读数据库中读取数据
	Mongo_Result Load(string ns,const Map& where,Map& result);

	//查询多条记录
	Mongo_Result Query(string ns,const Map& where,Results& results);
	Mongo_Result Query(string ns,const Map& where,Results& results,const Map& outfield);
	Mongo_Result Query( string ns,const Map& where,Results& results,string orderKey, int32_t bySort,int32_t limitResult = 0 );

	//用于获取大于或者小于指定值，bGrate=true,大于等于；bGrate=false，小于等于。
	Mongo_Result Query(string ns,const Map& where,Results& results,bool bGrate,string compareKey, uint32_t uValue);

	bool Delete(string ns,const Map& where);

	bool CheckNS(const string& ns);

	//为了减连接数,让异步与同步共用连接,所以允许上锁
	std::mutex mutex_;
protected:
			
	string host_;
	int port_;
	string dbname_;			//貌似没啥用?
	
	MongoDBPtr *mongo_;
	string user_;
	string password_;

	MogoWrapLogImpl *log_;
};

#endif
