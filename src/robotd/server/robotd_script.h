#ifndef _ROBOTD_SCRIPT_H_
#define _ROBOTD_SCRIPT_H_

#include <shared/lua_helper.h>
#include "robotd_context.h"

int scripts_init(const char* path);
int scripts_free();
int scripts_reload();

//取随机名字
extern int GetRandName(string &name);

//机器人登录
extern int DoRobotLoginOk(const string &account, uint32 fd, RobotdContext *robot);

//机器人复活以后
extern int DoRobotRespawn(RobotdContext *robot);

//机器人登出
extern int DoRobotLogout(const string &account, uint32 fd);

//机器人连接跨服成功
extern int DoRobotWarConn(const string &account, uint32 fd);

//机器人断开跨服连接
extern int DoRobotWarClose(const string &account, uint32 fd);

//网络包处理
extern int DoPacketHandle(const string &account, uint32 fd, packet *pkt);

//机器人心跳
extern int DoRobotUpdate(uint32 diff);

//取栈信息
extern int DoGetStack(const string &account,string &info);
#endif
