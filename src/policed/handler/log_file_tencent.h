#ifndef _LOG_FILE_TENCENT_
#define _LOG_FILE_TENCENT_

#include <comm/common_stl.h>
#include "log_file_stream.h"

#define RETRY_MAX_COUNT 10

class LogFileTencent
{
	typedef struct log_packet_t
	{
		packet *pkt;		//日志包
		uint32 retry_count;	//重试次数
	}log_packet;
	typedef map<uint32, LogFileStream*> FildMap;
public:
	LogFileTencent();
	~LogFileTencent();
public:
	void Update(uint32 diff);
	void WriteLog(packet *pkt, account_info *a_info = NULL, PolicedContext *player = NULL);
	void PostTXJinFen(string ext_url,string data);//通过外部服务器接口post经分数据
	void PostTXJinFen(string ext_url,string account,string user_guid,string pf,double modifyfee = 0,uint32 onlinetime = 0);//通过外部服务器接口post经分数据

private:
	void WriteBaseLog(uint32 type, LogFileStream& file, time_t sec, time_t usec, account_info *a_info, PolicedContext *player);
	void ResetMap(struct tm *local);
	void ReWriteLog(TimerHolder& th);
private:
	FildMap m_file_map;						//所有日志文件

	char m_time[50];						//用来做秒和微秒的组合
	uint32 m_today;							//今日的日期
	uint32 m_host;							//今日的小时
	
private:
	typedef void (LogFileTencent::*handle)(uint32 log_type, packet*, LogFileStream*, time_t, time_t, account_info*, PolicedContext*);
	handle m_tx_handle_map[MAX_LOG_FILE_TENCENT_TYPE];
	string GetLastString(string name,const char &spit);

private:
	void HandlerInit();
	void Handle_Write_Tencent_User(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
	void Handle_Write_Tencent_Player(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
	void Handle_Write_Tencent_Recharge(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
	void Handle_Write_Tencent_UpgradeLog(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
	void Handle_Write_Tencent_Newtask(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
	void Handle_Write_Tencent_Login(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
	void Handle_Write_Tencent_Offline(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
	void Handle_Write_Tencent_Online(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
	void Handle_Write_Tencent_MoneyLog(uint32, packet *, LogFileStream *, time_t, time_t, account_info*, PolicedContext*);
};

#endif
