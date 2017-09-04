
#ifndef _ASYNC_MONGODB_TASK_H_
#define _ASYNC_MONGODB_TASK_H_

#include "worker_pool.h"
#include "db_wrap/mongo_wrap_def.h"
#include "db_wrap/mongo_wrap.h"

//////////////////////////////////////////////////////////////////////////
//异步工作任务
struct AsyncMongoDBTask
{
	typedef std::function<void(AsyncMongoDBTask*)> CB;
	typedef std::map<string,string> Map;
	typedef std::vector<Map> Results;

	AsyncMongoDBTask(string& s,const Map& w,Map& v,int type):ns_(s),where(w),value(v),operType(type),result_(MONGO_RES_NULL)
	{
	}
	AsyncMongoDBTask(string& s,Map& v, int type):ns_(s),value(v),operType(type)
	{
	}
	AsyncMongoDBTask():operType(0)
	{
	}

	string ns_;
	Map where;
	Map value;
	Results results;
	int operType;					//for Mongo_Operator_TYPE
	Mongo_Result result_;			//操作结果
	CB callback_;

	int limit_;						//仅针对query有效,返回记录数
	string order_field_;			//排序字段
	int sort_;						//bySort 1是升序，－1是降序
};

enum Mongo_Operator_TYPE
{
	UPDATE = 1,			//更新操作
	INSERT,			//插入操作
	LOAD,			//查询单条记录
	QUERY,			//查询多条记录,毕竟还是有一些轮询，不能让他卡主进程
	UPSERT,			//更新或者插入
};

class AsyncMongoDB 
{
public:
	AsyncMongoDB(MongoWrap *db);
	virtual ~AsyncMongoDB();
	bool Start();

	void AsyncUpdate(string ns,const MongoWrap::Map& where,MongoWrap::Map& values);
	void AsyncInsert(string ns,MongoWrap::Map& values);
	void AsyncUpsert(string ns,const MongoWrap::Map& where,MongoWrap::Map& values);
	AsyncMongoDBTask *AsyncQuery(string ns,const MongoWrap::Map& wheres,AsyncMongoDBTask::CB cb,string orderKey, int32_t bySort,int32_t limitReslut);
	void AsyncLoad(string ns,const MongoWrap::Map& wheres,AsyncMongoDBTask::CB cb);
	void Update();

protected:
	void Excute(AsyncMongoDBTask *task);
	void PushResult(AsyncMongoDBTask *task);

private:
	//MongoWrap *mongodb_;
	WorkerPool<AsyncMongoDBTask> *workers_;	
	MongoWrap* db_;

	std::mutex mutex_;
	std::vector<AsyncMongoDBTask*> results_;	
};


#endif
