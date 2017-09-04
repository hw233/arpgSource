#ifndef _GAME_PATROL_
#define _GAME_PATROL_

#define SMS_KEY "@xTr66s^0n:l>U=Uod*g"
#define SMS_PID "3"

enum GAME_POLIC_ALARM_TYPE
{
	GAME_POLIC_ALARM_CHAT					= 1,		//聊天警告
	GAME_POLIC_ALARM_CHECK_GOLD				= 2,		//校验元宝数与充值的冲突
	GAME_POLIC_ALARM_DEAL_GOLD_5000			= 3,		//等级＜50级，充值为0，交易获得元宝大于5千
	GAME_POLIC_ALARM_DEAL_SILVER_1000		= 4,		//等级＜50级，充值为0，交易获得银两大于1000万
	GAME_POLIC_ALARM_DEAL_SILVER_10000		= 5,		//交易获得银两大于1亿
	GAME_POLIC_ALARM_STALL_GOLD_5000		= 6,		//等级＜50级，充值为0，摊子获得元宝大于5千
	GAME_POLIC_ALARM_STALL_SILVER_1000		= 7,		//等级＜50级，充值为0，摊子获得银两大于1000万
	GAME_POLIC_ALARM_STALL_SILVER_10000		= 8,		//摊子获得银两大于1亿
};

enum GAME_POLIC_ALARM_CHAT_TYPE
{
	GAME_POLIC_ALARM_CHAT_15				= 1,		//等级小于等于15级，和3个玩家私聊
};

////踢人的提醒文字
//static const string kicking_type_content[] = {
//	"对不起，由于您的非法操作已被封禁IP，若有疑问可咨询在线客服。",
//	"对不起，由于您的非法操作已被封号，若有疑问可咨询在线客服。",
//	"对不起，由于您的非法操作已被踢线，若有疑问可咨询在线客服。",
//	"对不起，请勿使用非正常方式在矿场复活。",
//	"对不起，请勿散播不良信息影响他人正常游戏。",
//	"对不起，请勿使用非正常手段进行游戏。",
//	"对不起，系统检测到您当前账号异常，请刷新后重新进入游戏即可恢复正常。"
//};

//校验玩家行为
//检查玩家资产与实际充值的差异
class GamePolice
{
public:
	GamePolice();
	~GamePolice(){};
private:
	char m_field_data[10000];
	set<string> m_exclude_account;
	bool m_send_msg;
	vector<string> m_phone_no;
	char m_post_data[1000];
	char m_sms_key_no[1000];
	
	uint32 m_lock_db_update_tm;
public:
	void SaveHTInfo(TimerHolder& th);		//保存后台信息
	void SaveTopInfo(TimerHolder& th);		//保存排行榜信息
	
};

#endif
