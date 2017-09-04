#include "logind_player.h"
#include "logind_context.h"
#include "map_manager.h"
#include "logind_script.h"
#include "logind_mongodb.h"
#include "logind_cache.h"
#include <object/SharedDef.h>
#include <shared/log_handler.h>
#include <shared/util.h>
#include <cstdlib>

logind_player::logind_player():PlayerBase(),m_session(0),m_mobile_session(0),m_zibao(false),m_auto_save_time(0)
{
}

logind_player::~logind_player()
{

}

void logind_player::InitNewPlayer(const std::string& name,uint8 faction,uint8 gender,uint8 head_id,uint8 hair_id,uint8 race,bool is_FCM)
{
	//创建基本属性
	SetName(name);
	SetOwner(GetGuid());
	SetGender(gender);
	SetRace(race);
	SetHead(head_id);
	SetLevel(BORN_LV);	
	SetHair(hair_id);	
	SetCreateTime((uint32)time(NULL));

	//初始给点银两和真气
	/**
	this->SetDouble(PLAYER_EXPAND_INT_MONEY + MONEY_TYPE_SILVER * 2, 1000000);
	this->SetDouble(PLAYER_EXPAND_INT_MONEY + MONEY_TYPE_GOLD_INGOT * 2, 1000000);
	*/
	this->SetByte(PLAYER_FIELD_HOOK_BYTE0, 0, 1);
	this->SetByte(PLAYER_FIELD_HOOK_BYTE0, 1, 1);
	this->SetByte(PLAYER_FIELD_HOOK_BYTE0, 2, 1);
	this->SetByte(PLAYER_FIELD_HOOK_BYTE0, 3, 1);
	this->SetByte(PLAYER_FIELD_HOOK_BYTE1, 0, 1);
	this->SetByte(PLAYER_FIELD_HOOK_BYTE1, 1, 1);
	this->SetUInt16(PLAYER_FIELD_HOOK_SHORT, 0, 50);

	//坐标朝向
	//新手村稍微随机一下，不然相当不带劲啊
	// TODO: 这里要稍微改下
	const MapTemplate *mt = MapTemplate::GetMapTempalte(BORN_MAP);
	ASSERT(mt);		//新手村都没有，搞屁啊
	uint16 born_x = BORN_X + irand(-8,8);
	uint16 born_y = BORN_Y + irand(-8,8);
	if(!mt->IsCanRun(born_x,born_y))
	{
		born_x = BORN_X;
		born_y = BORN_Y;
	}
	SetMapId(BORN_MAP);//地图
	Relocate(born_x,born_y,BORN_OR);

	SetByte(PLAYER_FIELD_BYTES_3,3,ATTACK_MODE_PEACE);			//初始攻击模式	
	SetUInt32(PLAYER_FIELD_LEVEL, 1);							//等级	
	
	SetDouble(PLAYER_FIELD_MOVESPEED, INIT_MOVE_SPEED);			//移动速度
	//默认启用时装
	SetBit(PLAYER_EXPAND_INT_USE_FASHION,EQUIPMENT_TYPE_COAT);			//主武器
	SetBit(PLAYER_EXPAND_INT_USE_FASHION,EQUIPMENT_TYPE_MAIN_WEAPON);	//衣服
	//到lua初始化玩家属性
	DoSetNewPlayerInfo(this);

	//防沉迷
	SetFCMLoginTime(is_FCM ? 0 : -1);
}

bool logind_player::InitDatabase()
{
	SetScenedFD(0);
	if (GetDouble(PLAYER_EXPAND_INT_MONEY + MONEY_TYPE_SILVER*2) > MAX_MONEY)
		SetDouble(PLAYER_EXPAND_INT_MONEY + MONEY_TYPE_SILVER*2, MAX_MONEY);

	SetUInt32(PLAYER_FIELD_ANGER, 0);

	//校验下等级
	uint32 lv = GetLevel();
	char_level *level_info =  char_level_db[lv];
	if (!level_info)
	{
		tea_perror("player %s data invalid  player_level %u", guid().c_str(), lv);
		return false;
	}
	
	//清空装备标识
	for (uint16 index = PLAYER_FIELD_EQUIPMENT; index < PLAYER_FIELD_EQUIPMENT + EQUIPMENT_TYPE_MAX_POS; index++)
		SetUInt32(index, 0);

	return true;
}

//计算防沉迷累计离线时间
void logind_player::CalculFCMLogoutTime()
{
	//非防沉迷玩家直接不处理
	if (GetFCMLoginTime()==(uint32)-1)
		return;
	uint32 time_now = (uint32)time(NULL);
	//这次登陆-上次退出+原累计退出时间 = 总累计离线时间
	uint32 cur_logouttime = time_now - GetLastLogoutTime() + GetFCMLogoutTime();
	//累计离线五个小时，清空累计在线和累计离线
	if (cur_logouttime>=18000)
	{
		SetFCMLoginTime(0);
		cur_logouttime = 0;
	}
	SetFCMLogoutTime(cur_logouttime);
}

void logind_player::SendPacket(packet& pkt)
{
	if (GetSession())
		GetSession()->SendPacket(pkt);
	else
		tea_pdebug("玩家 ID：%s Session不存在试图发包",GetGuid().c_str());
}

