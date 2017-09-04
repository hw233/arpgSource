#ifndef _PLATFORM_POST_
#define _PLATFORM_POST_

#include <comm/common_stl.h>
#include <object/SharedDef.h>
#include "policed_app.h"

#define POST_SIGN_STRING_MAX_LEN 1000

class PlatformPost
{
public:
	PlatformPost(){};
	~PlatformPost();
protected:
	string m_pid;
	map<uint32, string> m_post_url_map;
	map<uint32, string> m_post_key_map;
	char m_sign[POST_SIGN_STRING_MAX_LEN];
	char m_sign_md5[POST_SIGN_STRING_MAX_LEN];
protected:
	virtual string GetPostType() = 0;
	void Init();
	const string &GetPostUrl(EPostType type);
	void Post(EPostType type,const string& data);
	void Md5EncodeSign(const char *sign,...);
public:
	static PlatformPost *Factory(const string &type);
public:
	//聊天post
	virtual void ChatPost(account_info* a_info, PolicedContext* player, uint32 type, string& playerto, uint32 level, uint32 gmlevel,string& content,string& nameto){}
	//用户升级事件
	virtual void UpgradePost(account_info* a_info, PolicedContext* player,const char* post_time,uint32 level, double power){}
	//在线三分钟事件
	virtual void Online3MinsPost(account_info* a_info, PolicedContext* player){}
};

class NullPost : public PlatformPost
{
protected:
	virtual string GetPostType(){return PLATFORM_NULL;}
};

class Game2Post : public PlatformPost
{
protected:
	virtual string GetPostType(){return PLATFORM_GAME2;}
public:	
	//用户升级事件
	void UpgradePost(account_info* a_info, PolicedContext* player,const char* post_time,uint32 level, double power);
	//聊天post
	void ChatPost(account_info* a_info, PolicedContext* player, uint32 type, string& playerto, uint32 level, uint32 gmlevel,string& content,string& nameto);
	//在线三分钟事件
	void Online3MinsPost(account_info* a_info, PolicedContext* player);
};
#endif
