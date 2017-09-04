#ifndef _GLOBAL_VALUE_MANAGER_H_
#define _GLOBAL_VALUE_MANAGER_H_

#include "BinLogObject.h"
#include "ServerConnList.h"
#include "GuidManager.h"
#include <core_obj/GuidObjectTable.h>

#define GLOBAL_OBJ_GUID					"G.globalvalue"			//全局对象
#define GLOBAL_GAME_CONFIG				"G.gameconfig"			//游戏配置
#define GLOBAL_CLIENT_GAME_CONFIG		"G.ClientGameConfig"	//客户端配置信息
#define GLOBAL_GUID_MANAGER_GUID		"G.guidmanager"			//guid管理数据
#define GLOBAL_RANK_INFO				"G.rankinfo"			//排行榜数据
#define GLOBAL_RANK_INFO_RASK_POOL		"G.rankinforaskpool"	//排行榜任务池
#define GLOBAL_RIGHT_FLOAT_GUID			"G.rightfloat"			//弹窗公告
#define GLOBAL_SYSTEM_MAIL				"G.systemMail"			//系统邮件
#define GLOBAL_OFFLINE_MAIL				"G.offlineMail"			//离线邮件
#define GLOBAL_NOTICE_MESSAGE			"G.noticemessage"		//公告消息
#define GLOBAL_SAVE_TO_DB				"G.SaveToDbGuidList"	//本轮要保存到数据库的所有玩家
#define GLOBAL_COUNTER					"G.counter"				//计数器，各种世界数量计数
#define GLOBAL_ASYNC_VALUE				"G.asyncValue"			//不同步客户端的全局数据

class GlobalValueManager
{
public:
	GlobalValueManager()
		:m_globalvalue(NULL),m_map_Instance_list(NULL),m_rank_info(NULL)
		,m_rank_info_rask_pool(NULL),m_server_conn_list(NULL),m_systemmail(NULL),m_offlinemail(NULL)
		,m_rightfloat(NULL),m_gameconfig(NULL),m_clientconfig(NULL)
		,m_notice_message(NULL),m_save_to_db_guid_list(NULL),m_counter(NULL),m_asyncValue(NULL)
	{

	}

	~GlobalValueManager()
	{
	}
public:
	GuidManager	m_guid_mgr;					//所有的id生产器

	BinLogObject *m_globalvalue;			//世界变量
	BinLogObject *m_map_Instance_list;		//地图实例列表
	BinLogObject *m_rank_info;				//排行榜数据
	BinLogObject *m_rank_info_rask_pool;	//排行榜任务池
	ServerConnList *m_server_conn_list;		//内部服务器列表
	BinLogObject *m_rightfloat;				//弹窗公告
	BinLogObject *m_systemmail;				//系统邮件
	BinLogObject *m_offlinemail;			//离线邮件
	BinLogObject *m_gameconfig;				//游戏配置
	BinLogObject *m_clientconfig;			//客户端配置信息
	BinLogObject *m_notice_message;			//公告信息
	BinLogObject *m_save_to_db_guid_list;	//本轮要保存到数据库的所有玩家
	BinLogObject *m_counter;				//计数器
	BinLogObject *m_asyncValue;				//不同步客户端的全局数据

public:
	BinLogObject *CreateGlobalValues(core_obj::GuidObjectTable *objMgr, const string &guid);
public:
	//平台id
	const string &GetPlatformID(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_PLATFORM_ID);}
	void SetPlatformID(const string &pid){m_gameconfig->SetStr(GAME_CONFIG_STRING_FIELD_PLATFORM_ID, pid);}
	//服务器id
	const string &GetServerID(uint32 i = 0){return m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_SERVER + i);}
	void SetServerID(uint32 i, const string &server_name){m_gameconfig->SetStr(GAME_CONFIG_STRING_FIELD_SERVER + i, server_name);}
	void AddServerID(const string &server_name);
	//开服时间
	uint32 GetKaiFuShiJian(){return m_gameconfig->GetUInt32(GAME_CONFIG_INT_FIELD_KAIFUSHIJIAN);}
	void SetKaiFuShiJian(uint32 t){
		m_gameconfig->SetUInt32(GAME_CONFIG_INT_FIELD_KAIFUSHIJIAN, t);
	}
	//数据库连接串
	const string &GetDbConnStr(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_DB_CONN);}
	void SetDbConnStr(const string &str){m_gameconfig->SetStr(GAME_CONFIG_STRING_FIELD_DB_CONN, str);}
	//日志数据库连接串
	const string &GetLogDbConnStr(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_LOG_DB_CONN);}
	void SetLogDbConnStr(const string &str){m_gameconfig->SetStr(GAME_CONFIG_STRING_FIELD_LOG_DB_CONN, str);}
	//登录秘钥
	const string &GetLoginKey(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_LOGIN_KEY);}
	void SetLoginKey(const string &str){m_gameconfig->SetStr(GAME_CONFIG_STRING_FIELD_LOGIN_KEY, str);}
	//本地硬盘数据保存地址
	const string &GetLocalHddPath(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_LOCAL_HDD_PATH);}
	void SetLocalHddPath(const string &str){m_gameconfig->SetStr(GAME_CONFIG_STRING_LOCAL_HDD_PATH, str);}
	//外部web接口配置
	const string &GetExtWebInterface(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_EXT_WEB_INTERFACE);}
	void SetExtWebInterface(const string &str){m_gameconfig->SetStr(GAME_CONFIG_STRING_EXT_WEB_INTERFACE, str);}
	//配置服url地址
	const string &GetConfSvrUrl(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_CONF_SVR_URL);}
	void SetConfSvrUrl(const string &str){m_gameconfig->SetStr(GAME_CONFIG_STRING_CONF_SVR_URL, str);}
	//世界服url配置
	const string &GetWorldServerUrl(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_WORLD_SERVER_URL);}
	void SetWorldServerUrl(const string &str){m_gameconfig->SetStr(GAME_CONFIG_STRING_WORLD_SERVER_URL, str);}
	//设置QQ信息
	void SetQQInfo(const string &str)
	{
		if(str != m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_GM_QQ))
			m_gameconfig->SetStr(GAME_CONFIG_STRING_FIELD_GM_QQ,str);
	}
	//获得QQ信息
	const string& GetQQInfo(){return m_gameconfig->GetStr(GAME_CONFIG_STRING_FIELD_GM_QQ);}
	//活动版本号
	uint32 GetActivityVersion(){return m_globalvalue->GetUInt32(GLOBALVALUE_INT_FIELD_LIMIT_ACTIVITY_VERSION);}
	const string &GetActivityScript(){return m_globalvalue->GetStr(GLOBALVALUE_STRING_FIELD_LIMIT_ACTIVITY_SCRIPT);}
	//合服时间
	uint32 GetAddTime(){return m_gameconfig->GetUInt32(GAME_CONFIG_INT_FIELD_ADDTIME);}
	void SetAddTime(uint32 i){m_gameconfig->SetUInt32(GAME_CONFIG_INT_FIELD_ADDTIME, i);}
	//排行榜
	const string &GetRankInfoData(uint32 index){return m_rank_info->GetStr(index);}
	void SetRankInfoData(uint32 index, const string &data){m_rank_info->SetStr(index, data);}
	//客户端是否显示服务器id
	void SetShowServerID(){m_gameconfig->SetBit(GAME_CONFIG_INT_FIELD_FLAGS,GAME_CONFIG_FIELD_FLAGS_SHOW_SERVER_ID);}
	bool GetShowServerID(){return m_gameconfig->GetBit(GAME_CONFIG_INT_FIELD_FLAGS,GAME_CONFIG_FIELD_FLAGS_SHOW_SERVER_ID);}
	//服务器跨服类型
	void SetServerType(uint32 t){m_gameconfig->SetUInt32(GAME_CONFIG_INT_FIELD_WORLD_SVR_TYPE, t);}
	uint32 GetServerType(){return m_gameconfig->GetUInt32(GAME_CONFIG_INT_FIELD_WORLD_SVR_TYPE);}
	bool IsGameServer(){return GetServerType() == WORLD_SERVER_TYPE_GAME;}
	bool IsPKServer(){return GetServerType() == WORLD_SERVER_TYPE_PK;}	
	//获取活动版本号
	uint32 GetLimitActivityVersion(){return m_globalvalue->GetUInt32(GLOBALVALUE_INT_FIELD_LIMIT_ACTIVITY_VERSION);}
	//获得世界服连接状态
	uint32 GetServerConnWorldStatus(){return m_globalvalue->GetUInt32(GLOBALVALUE_INT_FIELD_WORLD_CONNECTION_STATUS);}
	void SetServerConnWorldStatus(uint32 status){m_globalvalue->SetUInt32(GLOBALVALUE_INT_FIELD_WORLD_CONNECTION_STATUS, status);}
	//获取所有合服server_name
	void GetMergeServerName(vector<string>& server_names);
	bool IsMyServer(const string &server_name);
	//设置每日绑定元宝获得
	void SetDailyAddBindGold(double val){m_counter->SetDouble(GLOBALCOUNTER_INT_FIELD_DAILY_ADD_BIND_GOLD,val);}
	//设置每日绑定元宝消耗
	void SetDailySubBindGold(double val){m_counter->SetDouble(GLOBALCOUNTER_INT_FIELD_DAILY_SUB_BIND_GOLD,val);}
	//设置每日元宝获得
	void SetDailyAddGoldIngot(double val){m_counter->SetDouble(GLOBALCOUNTER_INT_FIELD_DAILY_ADD_GOLD_INGOT,val);}
	//设置每日元宝消耗
	void SetDailySubGoldIngot(double val){m_counter->SetDouble(GLOBALCOUNTER_INT_FIELD_DAILY_SUB_GOLD_INGOT,val);}
	//设置每日银子获得
	void SetDailyAddSilver(double val){m_counter->SetDouble(GLOBALCOUNTER_INT_FIELD_DAILY_ADD_SILVER,val);}
	//设置每日银子消耗
	void SetDailySubSilver(double val){m_counter->SetDouble(GLOBALCOUNTER_INT_FIELD_DAILY_SUB_SILVER,val);}

protected:
	virtual void AfterCreateGlobalValue(BinLogObject *binlog);
	BinLogObject *CreateObject(const string &guid, BinLogObject **ptr, std::function<BinLogObject*(const string &)> fun = nullptr);
};

#endif
