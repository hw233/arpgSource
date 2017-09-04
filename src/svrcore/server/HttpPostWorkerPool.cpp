#include "HttpPostWorkerPool.h"
#include <net/http.h>

//////////////////////////////////////////////////////////////////////////
AsyncPostQuest::AsyncPostQuest(const char* url,const char* data):url_(0), data_(0), response_(0),result_(0),index_(0)
{
	url_ = (char *)malloc(strlen(url)+1);
	data_ = (char *)malloc(strlen(data)+1);
	strcpy(url_,url);
	strcpy(data_,data);
}

AsyncPostQuest::~AsyncPostQuest	()
{
	if(url_) free(url_);
	if(data_) free(data_);
	if(response_) free(response_);
}

//////////////////////////////////////////////////////////////////////////
//for HttpPostWorkerPool
HttpPostWorkerPool::HttpPostWorkerPool():callback_index_(0),run_callback_index_(0)
{
}

HttpPostWorkerPool::~HttpPostWorkerPool()
{
	for (auto it = callbacks_.begin();it!=callbacks_.end();++it)
	{
		auto *ptr = it->second;
		delete ptr;
	}
	callbacks_.clear();
}

void HttpPostWorkerPool::start(int worker_num)
{
	Start([this](AsyncPostQuest *quest){
		char *response = NULL;
		quest->result_ = http_post(quest->url_, quest->data_, &quest->response_);
		//并将已完成任务返回到准备回调的列表
		std::unique_lock<std::mutex > lck(mutex_callbacks_);
		callbacks_[quest->index_] = quest;
	},worker_num);
}

void HttpPostWorkerPool::asyn_post(const char *url,const char *data,bool isok)
{
	auto *quest = new AsyncPostQuest(url,data);	
	quest->index_ = ++callback_index_;			//回调序号

	if(isok){
		//并将已完成任务返回到准备回调的列表
		std::unique_lock<std::mutex > lck(mutex_callbacks_);
		callbacks_[quest->index_] = quest;
	}else
		Push(quest);
}

void HttpPostWorkerPool::do_callback(std::function<void(AsyncPostQuest&)> cb)
{
	//组回调任务上锁
	std::unique_lock<std::mutex > lck(mutex_callbacks_);

	auto it = callbacks_.find(run_callback_index_+1);
	while(it != callbacks_.end())
	{
		auto quest = it->second;	
		if(cb)
			cb(*quest);
		//已执行的回调序号可以了
		run_callback_index_ ++;
		//从容器中删除 
		callbacks_.erase(it);
		//释放内存
		delete quest;

		//如果下一次回调已经准备好了，可以继续
		it = callbacks_.find(run_callback_index_+1);
	}
}
