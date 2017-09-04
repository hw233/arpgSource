#include "platform_post.h"
#include <net/http.h>
#include <comm/b64.h>
#include <comm/crypt.h>
#include <comm/url_encode.h>
#include "policed_script.h"
#include "policed_context.h"

PlatformPost *PlatformPost::Factory(const string &type)
{
	PlatformPost *result = NULL;
	if(type == PLATFORM_GAME2)
		result = new Game2Post();
	else
		result = new NullPost();
	ASSERT(result);
	result->Init();
	return result;
}

PlatformPost::~PlatformPost()
{

}

void PlatformPost::Init()
{
	DoGetPostUrl(GetPostType(), m_post_url_map);
}

const string &PlatformPost::GetPostUrl(EPostType type)
{
	auto it = m_post_url_map.find(type);
	if(it == m_post_url_map.end())
	{
		//如果没配，那就是默认地址的
		it = m_post_url_map.find(POST_TYPE_NONE);
		ASSERT(it != m_post_url_map.end());
	}
	return it->second;
}

void PlatformPost::Post(EPostType type,const string& data)
{
	auto url = GetPostUrl(type);
#if PLATFORM == PLATFORM_WINDOWS
	ASSERT(!url.empty());
	tea_pdebug("post data %s to %s", data.c_str(), url.c_str());	
#endif
	PolicedApp::g_app->m_post.async_post(url,data,[url,data](int status,const std::string& response){
		if (status != 200)	
			tea_perror("post data %s to %s fail,err code %d。%s", data.c_str(), url.c_str(), status, response.c_str());		
		else		
			tea_pdebug("post result : %s", response.c_str() );	
	});
}

void PlatformPost::Md5EncodeSign(const char *sign,...)
{
	ASSERT(sign);

	va_list ap;		
	va_start(ap, sign);
	vsnprintf(m_sign, POST_SIGN_STRING_MAX_LEN, sign, ap);
	va_end(ap);	

	MD5CryptToStr32(m_sign, strlen(m_sign), m_sign_md5);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//哥们网
void Game2Post::UpgradePost(account_info* a_info, PolicedContext* player,const char* post_time,uint32 level, double power)
{
	if(!g_Config.do_post)
		return;

	stringstream key_str;
	key_str << g_Config.game_code
		<< level
		<< player->GetName()
		<< player->guid()
		<< a_info->uid
		<< player->GetServerId()
		<< 0
		<< power
		<< PolicedApp::g_app->GetLoginKey();
	Md5EncodeSign(key_str.str().c_str());

	stringstream ss;
	ss << "pid=" << player->GetPlatformId()
		<< "&qid=" << a_info->uid
		<< "&server_id=" << player->GetServerId()
		<< "&playid=" << player->guid()
		<< "&gkey=" << g_Config.game_code
		<< "&level=" << level
		<< "&name=" << player->GetName()
		<<"&toptype=0&topvalue=" << power
		<< "&sign=" << m_sign_md5;
	
	Post(POST_TYPE_UPGRADE, ss.str());
}

//在线三分钟事件
void Game2Post::Online3MinsPost(account_info* a_info, PolicedContext* player)
{
	if(!g_Config.do_post)
		return;

	stringstream key_str;
	key_str << g_Config.game_code
		<< player->GetLevel()
		<< player->GetName()
		<< player->guid()
		<< a_info->uid
		<< player->GetCreateTime()
		<< player->GetServerId()
		<< 0
		<< player->GetForce()
		<< 1
		<< PolicedApp::g_app->GetLoginKey();
	Md5EncodeSign(key_str.str().c_str());

	stringstream ss;
	ss << "pid=" << player->GetPlatformId()
		<< "&qid=" << a_info->uid
		<< "&regdate=" << player->GetCreateTime()
		<< "&server_id=" << player->GetServerId()
		<< "&playid=" << player->guid()
		<< "&gkey=" << g_Config.game_code
		<< "&level=" << player->GetLevel()
		<< "&name=" << player->GetName()
		<<"&toptype=0&topvalue=" << player->GetForce()
		<<"&type=1"
		<< "&sign=" << m_sign_md5;

	Post(POST_TYPE_UPGRADE, ss.str());
}

#define GAME2_CHAT_POST_KEY "N2M6AfROHcZ0Qt1vAEVuw8KDm5RpD6sV"
void Game2Post::ChatPost(account_info* a_info, PolicedContext* player, uint32 type, string& playerto, uint32 level, uint32 gmlevel,string& content,string& nameto)
{
	if(!g_Config.post_chat)
		return;

	//聊天监控
	stringstream key_str;
	key_str<<a_info->sid<<a_info->uid<<a_info->id<<GAME2_CHAT_POST_KEY;
	Md5EncodeSign(key_str.str().c_str());

	string to_uid = "";
	if(playerto != "")
	{
		PolicedContext *to_char_info = ObjMgr.FindPlayer(playerto);
		if(to_char_info)
		{
			to_uid = to_char_info->GetPlatformId();
		}
	}
	stringstream ss;
	ss << "&pid=" << player->GetPlatformId()
		<< "&sid=" << player->GetServerId()
		<< "&uid=" << a_info->uid
		<< "&account_id=" << a_info->id
		<< "&player_id=" << player->guid()
		<< "&player_name=" << player->GetName()
		<< "&touid=" << to_uid
		<< "&toplayer_id=" << playerto 
		<< "&toplayer_name=" << nameto
		<< "&level=" << level
		<< "&ip=" << player->GetLastLoginIp()
		<< "&msg="<< content
		<< "&type=" << POST_TYPE_CHAT
		<< "&type1=" << type
		<< "&sign=" << m_sign_md5;
	Post(POST_TYPE_CHAT, ss.str());
}