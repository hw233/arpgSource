#ifndef _LOGIND_GM_TABLE_
#define _LOGIND_GM_TABLE_

#include "logind_app.h"
#include <object/std.h>
#include <object/PlayerBase.h>
#include <object/SharedDef.h>

class LogindContext;

class LogindGMTable
{
public:
	LogindGMTable();
	~LogindGMTable();

public:
	void Send2PolicedUpdateGagStauts(const string& guid, uint32 end_time);	//发送到日志服更新禁言状态
	void Send2PolicedUpdateLockStauts(const string& account, uint32 end_time);	//发送到日志服更新封号状态
	void KickingPlayer(const string& account, uint8 kicking_type);		//T人
	uint32 LockAccount(const string& account, uint32 end_time);		//封号
	void SendOnilne(TimerHolder& th);					//在线日志
	uint32 GetLockInfo(string account);					//获取封号信息
private:
	void SetLockInfo(string account,uint32 end_time);	//设置封号信息
	LogindContext *Ready_kicking_player(const char* account, const char *content);

private:
	map<string,uint32>	m_lockInfo;				//封号信息缓存
};
#endif