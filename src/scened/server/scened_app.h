/************************************************************************
 * file	: scened_app.h                                                                     
 * desc	: 场景服务应用程序
 *		场景服保持着地图实例集合,所以需要应中心服要求对这些地图进行管理
 *
 *
 * author:	linbc
 * date:	20110518
 * version:
 ************************************************************************/
#ifndef _SCENED_APP_H_
#define _SCENED_APP_H_

#include <svrcore.h>
#include <comm/timer.h>
#include <protocol/external.h>
#include "ScenedObjectManager.h"

class Player;
class Creature;

//游戏配置
struct TEA_SVRCORE_SPEC ScenedConfig:public GameConfig
{
	virtual ~ScenedConfig(){}
	float error_distance;					//客户端服务端允许的偏差距离（普通速度下允许单拐弯点误差值）

	//以下数据从lua脚本载入
	uint32 max_player_level;				//最大等级
	uint32 peace_mode_cd;					//和平模式CD(秒)
	uint32 dead_protect_CD;					//死亡保护时间(秒)
	uint32 nomal_attack_time; 				//默认攻击时间
	uint32 nomal_move_speed; 				//默认移动速度	
	uint32 update_ownership_time;			//更新怪物所有者的间隔
	uint32 left_fighting_time;				//脱离战斗时间
	uint32 heathy_auto_recovery_cycle;		//自动回血蓝时间
	uint32 heathy_auto_recovery_percent; 	//自动回血蓝比例
	uint32 energy_auto_recovery_cycle;		//自动回体时间
	uint32 energy_auto_recovery_values;		//自动回体力值
	uint32 endurance_auto_recovery_cycle;	//自动回精力时间
	uint32 endurance_auto_recovery_values;	//自动回精力值	
	uint32 jump_cd; 						//跳cd
	float jump_max_distance;				//最大跳跃距离
	int jump_need_energy;					//跳跃消耗体力
	float notice_distance; 					//地图触发点通知距离
	float nomal_attack_distance;			//默认攻击距离
	uint32 loot_area;						//战利品占地面积的边长
	uint32 loot_exist_timer;				//战利品地图存在时间
	uint32 loot_change_owner;				//战利品改变所有者的定时器
	uint32 loot_money_entry;				//战利品钱的gameobject模板ID
	uint32 respawn_max_level;				//原地复活等级,小于该等级可以原地复活

	uint32 sub_weapon_durability;			//每次扣除的武器耐久的基本单位
	uint32 sub_other_durability;			//每次扣除的装备(除了武器和无限耐久装备)耐久的基本单位
	uint32 new_bird_protect_lv;				//新手保护等级

	float monster_random_radius_percent;	//怪物随机移动半径
	uint32 gear_exist_time;					//机关停止后消失时间
	uint8  red_name_time;					//砍人红名时间
	float safe_distance;					//复活点的安全半径
	uint32 use_gameobject_diff;				//使用需要读条的GO允许的时间差
	uint32 max_npc_distance;				//最大服务器距离
	vector<uint16> can_move_dis_map;		//允许坐标不同步的地图
	string map_data_hdd_path;				//地图数据硬盘保存路径
protected:
	virtual bool _Load(config_loader *loader);
}; 


#if COMPILER == COMPILER_GNU
#  define tea_perror(str,...)	out_error(ScenedApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pdebug(str,...)	out_debug(ScenedApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pwarn(str,...)	    out_warn(ScenedApp::g_app->get_logger(),str,##__VA_ARGS__)
#  define tea_pinfo(str,...)	    out_str(ScenedApp::g_app->get_logger(),str,##__VA_ARGS__)
#else
#  define tea_perror(str,...)	out_error(ScenedApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pdebug(str,...)	out_debug(ScenedApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pwarn(str,...)	    out_warn(ScenedApp::g_app->get_logger(),str,__VA_ARGS__)
#  define tea_pinfo(str,...)	    out_str(ScenedApp::g_app->get_logger(),str,__VA_ARGS__)
#endif


class ScenedApp:public SvrCoreApp<ScenedApp>
{
	friend class SvrCoreApp<ScenedApp>;
	static const int WORLD_SLEEP_CONST = 1000/8;			//8帧
public:
	ScenedApp(SvrCoreParams& params,SvrCoreConfig& config);
	virtual ~ScenedApp();

	virtual bool Open();
	void Close();

	ScenedConfig& GetConfig()
	{
		return dynamic_cast<ScenedConfig&>(m_config);
	}	

	void Update(uint32 diff);

	ScenedObjectManager	*m_obj_mgr;				//对象管理器
	void AddWatchGlobalValueOk();

protected:
	SvrObjectManager *GetObjMgr()
	{
		return dynamic_cast<SvrObjectManager *>(m_obj_mgr);
	}

private:
	//////////////////////////////////////////////////////////////////////////
	//	
	const static int SERVER_TYPE = SERVER_TYPE_SCENED;

	static int on_centd_open_time(tcp_connection *,server_packet *pkt);			//中心服同步服务器时间到场景服
	static int on_centd_guid_object_table(tcp_connection *,server_packet *pkt);
	static int on_join_map(tcp_connection *,server_packet *pkt);			//玩家加入地图
	static int on_leave_map(tcp_connection *,server_packet *pkt);			//玩家离开地图
	static int on_create_world_map(tcp_connection *,server_packet *pkt);	//创建地图
	static int on_del_map(tcp_connection *,server_packet *pkt);				//通知释放地图	
	static int on_close_scened(tcp_connection *,server_packet *pkt);			//关闭自己
	static int on_internal_user_item(tcp_connection *,server_packet *pkt);		//使用物品
	static int on_internal_quest_load_result(tcp_connection *,server_packet *pkt);	//加载任务数据
		
	static int on_gm_commands(tcp_connection *,server_packet *pkt);		//GM命令
	
	static int on_appd_teleport(tcp_connection *,server_packet *pkt);	//应用服发起传送
	static int on_logind_teleport(tcp_connection *,server_packet *pkt);	//登录服发起传送
	static int on_internal_update_equip_info(tcp_connection *, server_packet *pkt); //应用服更新玩家装备到场景服
	static int on_scened_addexp(tcp_connection *,server_packet *pkt);	//应用服通知场景服增加经验
	static int on_scened_consume_money(tcp_connection *conn,server_packet *pkt);	//应用服通知场景服消耗元宝或铜钱做些什么	
	static int on_logind_sync_maxguid(tcp_connection* coon, server_packet* pkt);	//登录服同步guid最大累加数值
	static int on_logind_merge_server(tcp_connection* coon, server_packet* pkt);	//登陆服通知场景服合服了
	static int on_map_router_pkt(tcp_connection* coon, server_packet* pkt);			//地图给地图发包
	static int on_logind_to_scened_add_map_watcher(tcp_connection *conn,server_packet *pkt);	//通知场景服添加地图观察者
	static int on_netgd_destory_conn(tcp_connection *conn, server_packet *pkt);			//关闭连接

	static int on_update_spell_info(tcp_connection* coon, server_packet* pkt);		//通知场景服更新技能信息
	static int on_update_change_divine_info(tcp_connection* coon, server_packet* pkt);		//通知场景服更新装备的技能信息
	static int on_replace_equiped_spell(tcp_connection* coon, server_packet* pkt);			//通知场景服装备槽技能改变
	static int on_appd_send_scened_do_something(tcp_connection* coon, server_packet* pkt);	//通知场景服做些事情

public:	
	void call_appd_user_item(const char *guid,uint32 template_id,uint8 result,uint32 count);
	void call_user_killed(const string &guid, const string &killer_id, uint32 silver);
	void call_loot_select(const string &guid,uint32 entry,uint32 strong,uint32 count,uint32 fail_time = 0);	//通知玩家拾取物品
	void call_del_map(uint32 instanceid);								//删除实例
	void call_player_addmoney(const string& player_id, const uint8 money_type, const uint8 opt_type, const double val, string &p1, int32 p2, int32 p3, uint8 p4, uint8 p5);//通知应用服增加money
	void call_player_submoney(const string& player_id, const uint8 money_type, const uint8 opt_type, const double val, string &p1, int32 p2, int32 p3, uint8 p4, uint8 p5);//通知应用服扣money	
	void call_player_do_something(const string& player_id, uint32 type, uint32 id, string& str);//通知应用服做什些事情
	void call_recalculate(const string& player_id);	//通知应用服重算属性
	void call_add_gift_packs(string& player, int id, int gift_type, uint32_t start_time, uint32_t end_time, string gift_name, string gift_desc, string item_config, string item_from);//发礼包
	void call_player_upgrade(const string& player_id, const uint32 prevLevel, const uint32 level);//通知应用服玩家升级了
	//////////////////////////////////////////////////////////////////////////
	//后台命令
	static int on_command_reload_scripts(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_reload_template(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_reset_daily(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_hello_world(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_complete_quest(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_add_quest(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_del_quest_item(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_teleport(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);
	static int on_command_reset_spell_cd(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);

	static int on_command_ls_session(tcp_connection *, server_packet *,uint32 guid, uint32, Tokens& tokens);

};

#define g_Config ScenedApp::g_app->GetConfig()
#define Pingbi(str) ScenedApp::g_app->FuckPingbi(str)
#define RegTimer(cb,t,param) ScenedApp::g_app->RegisterTimer((cb),(t),(param));
#define CancleTimer(cb) ScenedApp::g_app->CancleTimer((cb));
#define globalValue (ScenedApp::g_app->m_globalvalue)
#define g_GuidMgr (ScenedApp::g_app->m_guid_mgr)
#define ObjMgr (*ScenedApp::g_app->m_obj_mgr)
#define ServerList (*ScenedApp::g_app->m_server_conn_list)
#define globalNoticeMessage (ScenedApp::g_app->m_notice_message)
//#define globalCounter (ScenedApp::g_app->m_counter)

#endif
