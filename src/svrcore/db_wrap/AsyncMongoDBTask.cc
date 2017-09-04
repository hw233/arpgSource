#include "AsyncMongoDBTask.h"

#include "worker_pool.h"
#include "mongo_wrap.h"

//////////////////////////////////////////////////////////////////////////
//for AsyncMongoDB


AsyncMongoDB::AsyncMongoDB(MongoWrap *db):workers_(nullptr),db_(db)
{
	workers_ = new WorkerPool<AsyncMongoDBTask>();
}

AsyncMongoDB::~AsyncMongoDB()
{
	if(workers_)
	{
		workers_->Stop();
		delete workers_;
		workers_ = nullptr;
	}	
}

bool AsyncMongoDB::Start()
{				
	workers_->Start(std::bind(&AsyncMongoDB::Excute,this,std::placeholders::_1)/*,worker_num*/);//只允许一个连接
	return true;
}

void AsyncMongoDB::Excute(AsyncMongoDBTask *task)
{
	switch (task->operType)
	{
	case UPDATE:
		{
			db_->Update(task->ns_,task->where,task->value);
			delete task;
			break;
		}
	case UPSERT:
		{
			db_->Upsert(task->ns_,task->where,task->value);
			delete task;
			break;
		}
	case INSERT:
		{
			db_->Insert(task->ns_,task->value);
			delete task;
			break;
		}
	case LOAD:
		{
			task->result_ = db_->Load(task->ns_,task->where,task->value);
			PushResult(task);
			break;
		}
	case QUERY:
		{
			task->result_ = db_->Query(task->ns_,task->where,task->results,task->order_field_,task->sort_,task->limit_);
			PushResult(task);
			break;
		}
	}
}

void AsyncMongoDB::AsyncUpdate(string ns,const MongoWrap::Map& where,MongoWrap::Map& values)
{
	auto *t = new AsyncMongoDBTask(ns,where,values, UPDATE);
	workers_->Push(t);
}

void AsyncMongoDB::AsyncInsert(string ns,MongoWrap::Map& values)
{
	auto *t = new AsyncMongoDBTask(ns,values, INSERT);
	workers_->Push(t);
}	

void AsyncMongoDB::AsyncUpsert(string ns,const MongoWrap::Map& where,MongoWrap::Map& values)
{
	auto *t = new AsyncMongoDBTask(ns,where, values, UPSERT);
	workers_->Push(t);
}	

AsyncMongoDBTask *AsyncMongoDB::AsyncQuery(string ns,const MongoWrap::Map& wheres,AsyncMongoDBTask::CB cb,string orderKey, int32_t bySort,int32_t limitReslut)
{
	auto *t = new AsyncMongoDBTask();
	t->ns_ = ns;
	t->where = wheres;
	t->operType = QUERY;
	t->callback_ = cb;
	t->order_field_ = orderKey;
	t->sort_ = bySort;
	t->limit_ = limitReslut;
	workers_->Push(t);
	return t;
}

void AsyncMongoDB::AsyncLoad(string ns,const MongoWrap::Map& wheres,AsyncMongoDBTask::CB cb)
{
	auto *t = new AsyncMongoDBTask();
	t->ns_ = ns;
	t->where = wheres;
	t->operType = LOAD;
	t->callback_ = cb;
	workers_->Push(t);
}

void AsyncMongoDB::PushResult(AsyncMongoDBTask *task)
{
	std::unique_lock < std::mutex > lck(mutex_);
	results_.push_back(task);
}

//在主线程跑,跑完就进入结果队列
void AsyncMongoDB::Update()
{
	std::unique_lock < std::mutex > lck(mutex_);
	for (auto it:results_)
	{
		if(it->callback_)
			it->callback_(it);
		delete it;
	}
	results_.clear();
}
