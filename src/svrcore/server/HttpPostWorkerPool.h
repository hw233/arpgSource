#ifndef _HTTP_POST_WORKDER_POOL_H_
#define _HTTP_POST_WORKDER_POOL_H_

#include <worker_pool.h>
#include <map>
#include <functional>

//异步任务post
struct AsyncPostQuest
{
	AsyncPostQuest():url_(0), data_(0), response_(0),result_(0),index_(0)
	{
	}
	AsyncPostQuest(const char* url,const char* data);
	~AsyncPostQuest();
	char *url_;
	char *data_;
	char *response_;
	int result_;
	uint64_t index_;
};

class HttpPostWorkerPool:public WorkerPool<AsyncPostQuest>
{
public:
	HttpPostWorkerPool();
	virtual ~HttpPostWorkerPool();

	//开始工作吧
	void start(int worker_num);

	//生成异步任务,是否已完成，如果已经完成直接回调而不用压缩
	void asyn_post(const char *url,const char *data,bool isok = false);	

	//执行回调并清空
	void do_callback(std::function<void(AsyncPostQuest&)> cb);

protected:
	//当前队列的序号
	uint64_t callback_index(){return callback_index_;}	

	//已经执行的回调序号
	uint64_t run_callback_index(){return run_callback_index_;}

	//这里是准备给回调的任务列表
	std::map<uint64_t,AsyncPostQuest*> callbacks_;
	std::mutex mutex_callbacks_;

	//最后一个进入队列的回调号
	uint64_t callback_index_;
	//已完成的回调号
	uint64_t run_callback_index_;
};

#endif
