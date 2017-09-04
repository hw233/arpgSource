#ifndef _LOGIND_SCRIPT_H_
#define _LOGIND_SCRIPT_H_

#include <comm/common_stl.h>
#include <shared/lua_helper.h>

int scripts_init(const char*);
int scripts_free();
int scripts_reload();

//获取配置信息
extern int DoGetConfig();

extern int DoSendSms(const char* tele_no,const char* context);

extern bool GetPlatFormGameCode();

//GM命令的KEY转换
extern int GetGmCommandKey(string &gm_str);

extern int GetPlatfromPostType(vector<string> &vec);
extern int DoGetPostUrl(const string &plat_id, map<uint32,string> &m);
//读取模版数据到内存
extern bool LoadTemplate();
#endif
