#ifndef _CENT_PLAYER_H_
#define _CENT_PLAYER_H_

#include "logind_app.h"
#include <object/std.h>
#include <object/PlayerBase.h>
#include <object/SharedDef.h>
#include <object/QuestDef.h>

class LogindContext;
class MobileContext;

#define INIT_MOVE_SPEED 135 //135毫秒移动一格

//登录服玩家对象
class logind_player:public PlayerBase
{
	friend class LogindContext;
public:
	
	//创建一个新对象	
	//static logind_player *CreateChar(const string &guidlow, const std::string& name,uint8 faction,uint8 gender,uint8 head_id,uint8 hair_id,uint8 race);
	logind_player();
	virtual ~logind_player();	

	LogindContext *GetSession(){return m_session;}
	MobileContext *GetMobileSession(){return m_mobile_session;}
	void SetSession(LogindContext *context){m_session = context;}	
	void SetMobileSession(MobileContext *context){m_mobile_session = context;}	

	void SendPacket(packet& pkt);
public:
	void InitNewPlayer(const std::string& name,uint8 faction,uint8 gender,uint8 head_id,uint8 hair_id,uint8 race,bool is_FCM = false);								//初始化新玩家
	bool InitDatabase();								//初始化从数据库load出的数据
public:
	//////////////////////////////////////////////////////
	//防沉迷s
	void CalculFCMLogoutTime();//计算处理防沉迷累计离线时间
	void SetZiBao(){m_zibao = true;}	
	time_t GetAutoSaveTime(){return m_auto_save_time;}
	void SetAutoSavetime(time_t t){m_auto_save_time = t;}
protected:	
	LogindContext *m_session;					//页游会话
	MobileContext *m_mobile_session;			//手游会话
private:
	bool m_zibao;					//是否被@自爆了
	time_t m_auto_save_time;		//自动保存时间戳
};

#endif

