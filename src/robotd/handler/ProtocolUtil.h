#ifndef _PROTOCOL_UTIL_H_
#define _PROTOCOL_UTIL_H_

#include "robotd_app.h"
#include "encipher.h"

class ProtocolUtil
{
public:
	ProtocolUtil():m_conn(0),m_war_conn(0){}
	~ProtocolUtil(){}
public:
	static void SetWarOptCode(vector<int> &);
private:
	core_socket::TcpConnection *m_conn;
	core_socket::c2sencipher m_encipher;

	core_socket::TcpConnection *m_war_conn;
	core_socket::c2sencipher m_war_encipher;
	static UpdateMask m_war_optcode;

	ByteArray m_bytes;
public:
	void SendToServer(ByteArray &);
	void ResetConn(core_socket::TcpConnection *conn);
	void ResetWarConn(core_socket::TcpConnection *conn);
	/*无效动作*/
	int unpack_null_action (ByteArray &bytes );
	int send_null_action ();
	/*测试连接状态*/
	int unpack_ping_pong (ByteArray &bytes );
	int send_ping_pong ();
	/*踢掉在线的准备强制登陆*/
	int unpack_forced_into (ByteArray &bytes );
	int send_forced_into ();
	/*获得Session对象*/
	int unpack_get_session (ByteArray &bytes ,string &sessionkey,string &account,string &version);
	int send_get_session (char const*sessionkey,char const*account,char const*version);
	/*网关服数据包路由测试*/
	int unpack_route_trace (ByteArray &bytes ,string &val);
	int send_route_trace (char const*val);
	/*记录客户端日志*/
	int unpack_write_client_log (ByteArray &bytes ,uint32 &type,string &uid,string &guid,string &log);
	int send_write_client_log (uint32 type,char const*uid,char const*guid,char const*log);
	/*操作失败*/
	int unpack_operation_failed (ByteArray &bytes ,uint16 &type,uint16 &reason,string &data);
	int send_operation_failed (uint16 type,uint16 reason,char const*data);
	/*同步时间*/
	int unpack_sync_mstime (ByteArray &bytes ,uint32 &mstime_now,uint32 &time_now,uint32 &open_time);
	int send_sync_mstime (uint32 mstime_now,uint32 time_now,uint32 open_time);
	/*对象更新*/
	int unpack_ud_object (ByteArray &bytes );
	int send_ud_object ();
	/*对象更新控制协议*/
	int unpack_ud_control (ByteArray &bytes );
	int send_ud_control ();
	/*对象更新控制协议结果*/
	int unpack_ud_control_result (ByteArray &bytes );
	int send_ud_control_result ();
	/*GRID的对象更新*/
	int unpack_grid_ud_object (ByteArray &bytes );
	int send_grid_ud_object ();
	/*GRID的对象更新*/
	int unpack_grid_ud_object_2 (ByteArray &bytes );
	int send_grid_ud_object_2 ();
	/*告诉客户端，目前自己排在登录队列的第几位*/
	int unpack_login_queue_index (ByteArray &bytes ,uint32 &index);
	int send_login_queue_index (uint32 index);
	/*踢人原因*/
	int unpack_kicking_type (ByteArray &bytes ,uint32 &k_type);
	int send_kicking_type (uint32 k_type);
	/*获取角色列表*/
	int unpack_get_chars_list (ByteArray &bytes );
	int send_get_chars_list ();
	/*角色列表*/
	int unpack_chars_list (ByteArray &bytes , vector< char_create_info > &list,string &faction_name,string &queen_name,uint8 &icon);
	int send_chars_list (const vector< char_create_info > &list ,char const*faction_name,char const*queen_name,uint8 icon);
	/*检查名字是否可以使用*/
	int unpack_check_name (ByteArray &bytes ,string &name);
	int send_check_name (char const*name);
	/*检查名字结果*/
	int unpack_check_name_result (ByteArray &bytes ,uint8 &result);
	int send_check_name_result (uint8 result);
	/*创建角色*/
	int unpack_char_create (ByteArray &bytes , char_create_info **info);
	int send_char_create (char_create_info *info);
	/*角色创建结果*/
	int unpack_char_create_result (ByteArray &bytes ,uint8 &result);
	int send_char_create_result (uint8 result);
	/*删除角色*/
	int unpack_delete_char (ByteArray &bytes ,uint32 &id);
	int send_delete_char (uint32 id);
	/*角色删除结果*/
	int unpack_delete_char_result (ByteArray &bytes ,uint8 &result);
	int send_delete_char_result (uint8 result);
	/*玩家登录*/
	int unpack_player_login (ByteArray &bytes ,string &guid);
	int send_player_login (char const*guid);
	/*玩家退出*/
	int unpack_player_logout (ByteArray &bytes );
	int send_player_logout ();
	/*临时账号转正规*/
	int unpack_regularise_account (ByteArray &bytes ,string &uid);
	int send_regularise_account (char const*uid);
	/*角色配置信息*/
	int unpack_char_remotestore (ByteArray &bytes ,uint32 &key,uint32 &value);
	int send_char_remotestore (uint32 key,uint32 value);
	/*角色配置信息*/
	int unpack_char_remotestore_str (ByteArray &bytes ,uint32 &key,string &value);
	int send_char_remotestore_str (uint32 key,char const*value);
	/*传送，如果是C->S，mapid变量请填成传送点ID*/
	int unpack_teleport (ByteArray &bytes ,uint32 &intGuid);
	int send_teleport (uint32 intGuid);
	/*停止移动*/
	int unpack_move_stop (ByteArray &bytes ,uint32 &guid,uint16 &pos_x,uint16 &pos_y);
	int send_move_stop (uint32 guid,uint16 pos_x,uint16 pos_y);
	/*unit对象移动*/
	int unpack_unit_move (ByteArray &bytes ,uint32 &guid,uint16 &pos_x,uint16 &pos_y, vector< int8 > &path);
	int send_unit_move (uint32 guid,uint16 pos_x,uint16 pos_y,const vector< int8 > &path );
	/*使用游戏对象*/
	int unpack_use_gameobject (ByteArray &bytes ,uint32 &target);
	int send_use_gameobject (uint32 target);
	/*包裹操作-交换位置*/
	int unpack_bag_exchange_pos (ByteArray &bytes ,uint32 &src_bag,uint32 &src_pos,uint32 &dst_bag,uint32 &dst_pos);
	int send_bag_exchange_pos (uint32 src_bag,uint32 src_pos,uint32 dst_bag,uint32 dst_pos);
	/*包裹操作-销毁物品*/
	int unpack_bag_destroy (ByteArray &bytes ,string &item_guid,uint32 &num,uint32 &bag_id);
	int send_bag_destroy (char const*item_guid,uint32 num,uint32 bag_id);
	/*分割物品*/
	int unpack_bag_item_split (ByteArray &bytes ,uint8 &bag_id,uint16 &src_pos,uint32 &count,uint16 &dst_pos,uint8 &dst_bag);
	int send_bag_item_split (uint8 bag_id,uint16 src_pos,uint32 count,uint16 dst_pos,uint8 dst_bag);
	/*使用物品*/
	int unpack_bag_item_user (ByteArray &bytes ,string &item_guid,uint32 &count);
	int send_bag_item_user (char const*item_guid,uint32 count);
	/*下发物品冷却*/
	int unpack_bag_item_cooldown (ByteArray &bytes , vector< item_cooldown_info > &list);
	int send_bag_item_cooldown (const vector< item_cooldown_info > &list );
	/*grid中的unit移动整体打包*/
	int unpack_grid_unit_move (ByteArray &bytes );
	int send_grid_unit_move ();
	/*grid中的unit移动整体打包2*/
	int unpack_grid_unit_move_2 (ByteArray &bytes );
	int send_grid_unit_move_2 ();
	/*兑换物品*/
	int unpack_exchange_item (ByteArray &bytes ,uint32 &entry,uint32 &count,uint32 &tar_entry);
	int send_exchange_item (uint32 entry,uint32 count,uint32 tar_entry);
	/*背包扩展*/
	int unpack_bag_extension (ByteArray &bytes ,uint8 &bag_id,uint8 &extension_type,uint32 &bag_pos);
	int send_bag_extension (uint8 bag_id,uint8 extension_type,uint32 bag_pos);
	/*请求NPC商品列表*/
	int unpack_npc_get_goods_list (ByteArray &bytes ,uint32 &npc_id);
	int send_npc_get_goods_list (uint32 npc_id);
	/*Npc商品列表*/
	int unpack_npc_goods_list (ByteArray &bytes , vector< uint32 > &goods,uint32 &npc_id);
	int send_npc_goods_list (const vector< uint32 > &goods ,uint32 npc_id);
	/*购买商品*/
	int unpack_store_buy (ByteArray &bytes ,uint32 &id,uint32 &count);
	int send_store_buy (uint32 id,uint32 count);
	/*出售物品*/
	int unpack_npc_sell (ByteArray &bytes ,uint32 &npc_id,string &item_guid,uint32 &num);
	int send_npc_sell (uint32 npc_id,char const*item_guid,uint32 num);
	/*回购物品*/
	int unpack_npc_repurchase (ByteArray &bytes ,string &item_id);
	int send_npc_repurchase (char const*item_id);
	/*时装是否启用*/
	int unpack_avatar_fashion_enable (ByteArray &bytes ,uint8 &pos);
	int send_avatar_fashion_enable (uint8 pos);
	/*任务对话选项*/
	int unpack_questhelp_talk_option (ByteArray &bytes ,uint32 &quest_id,uint32 &option_id,int32 &value0,int32 &value1);
	int send_questhelp_talk_option (uint32 quest_id,uint32 option_id,int32 value0,int32 value1);
	/*与NPC对话获得传送点列表*/
	int unpack_taxi_hello (ByteArray &bytes ,uint32 &guid);
	int send_taxi_hello (uint32 guid);
	/*发送传送点列表*/
	int unpack_taxi_stations_list (ByteArray &bytes ,uint32 &npcid, vector< taxi_menu_info > &stations);
	int send_taxi_stations_list (uint32 npcid,const vector< taxi_menu_info > &stations );
	/*选择传送点*/
	int unpack_taxi_select_station (ByteArray &bytes ,uint32 &station_id,uint32 &guid);
	int send_taxi_select_station (uint32 station_id,uint32 guid);
	/*与NPC交流选择选项*/
	int unpack_gossip_select_option (ByteArray &bytes ,uint32 &option,uint32 &guid,string &unknow);
	int send_gossip_select_option (uint32 option,uint32 guid,char const*unknow);
	/*与NPC闲聊获取选项*/
	int unpack_gossip_hello (ByteArray &bytes ,uint32 &guid);
	int send_gossip_hello (uint32 guid);
	/*发送闲聊信息和选项*/
	int unpack_gossip_message (ByteArray &bytes ,uint32 &npcid,uint32 &npc_entry,uint32 &option_id,string &option_text, vector< gossip_menu_option_info > &gossip_items);
	int send_gossip_message (uint32 npcid,uint32 npc_entry,uint32 option_id,char const*option_text,const vector< gossip_menu_option_info > &gossip_items );
	/*任务发布者状态查询*/
	int unpack_questgiver_status_query (ByteArray &bytes ,uint32 &guid);
	int send_questgiver_status_query (uint32 guid);
	/*任务NPC状态*/
	int unpack_questgiver_status (ByteArray &bytes ,uint32 &guid,uint8 &status);
	int send_questgiver_status (uint32 guid,uint8 status);
	/*查询任务状态*/
	int unpack_query_quest_status (ByteArray &bytes , vector< quest_status > &quest_array);
	int send_query_quest_status (const vector< quest_status > &quest_array );
	/*可接任务*/
	int unpack_questhelp_get_canaccept_list (ByteArray &bytes );
	int send_questhelp_get_canaccept_list ();
	/*下发可接任务列表*/
	int unpack_questhelp_canaccept_list (ByteArray &bytes , vector< uint32 > &quests);
	int send_questhelp_canaccept_list (const vector< uint32 > &quests );
	/*任务失败*/
	int unpack_questupdate_faild (ByteArray &bytes ,uint8 &reason);
	int send_questupdate_faild (uint8 reason);
	/*任务条件完成*/
	int unpack_questupdate_complete (ByteArray &bytes ,uint32 &quest_id);
	int send_questupdate_complete (uint32 quest_id);
	/*放弃任务*/
	int unpack_questlog_remove_quest (ByteArray &bytes ,uint8 &slot,uint64 &reserve);
	int send_questlog_remove_quest (uint8 slot,uint64 reserve);
	/*完成任务*/
	int unpack_questgiver_complete_quest (ByteArray &bytes ,uint32 &guid,uint32 &quest_id,uint8 &reward);
	int send_questgiver_complete_quest (uint32 guid,uint32 quest_id,uint8 reward);
	/*完成任务后通知任务链的下个任务*/
	int unpack_questhelp_next (ByteArray &bytes ,uint32 &current_id,uint32 &next_id,uint32 &guid);
	int send_questhelp_next (uint32 current_id,uint32 next_id,uint32 guid);
	/*任务系统强制完成任务*/
	int unpack_questhelp_complete (ByteArray &bytes ,uint32 &quest_id,uint8 &quest_statue,uint8 &reserve);
	int send_questhelp_complete (uint32 quest_id,uint8 quest_statue,uint8 reserve);
	/*接受任务成功*/
	int unpack_questupdate_accept (ByteArray &bytes ,uint32 &quest_id);
	int send_questupdate_accept (uint32 quest_id);
	/*更新任务进度_下标数量*/
	int unpack_questhelp_update_status (ByteArray &bytes ,uint32 &quest_id,uint32 &slot_id,uint32 &num);
	int send_questhelp_update_status (uint32 quest_id,uint32 slot_id,uint32 num);
	/*任务已完成*/
	int unpack_questgetter_complete (ByteArray &bytes ,uint32 &quest_id);
	int send_questgetter_complete (uint32 quest_id);
	/*接任务*/
	int unpack_questgiver_accept_quest (ByteArray &bytes ,uint32 &npcid,uint32 &quest_id);
	int send_questgiver_accept_quest (uint32 npcid,uint32 quest_id);
	/*任务使用物品*/
	int unpack_questupdate_use_item (ByteArray &bytes ,uint32 &item_id);
	int send_questupdate_use_item (uint32 item_id);
	/*查询天书任务*/
	int unpack_questhelp_query_book (ByteArray &bytes ,uint32 &dynasty);
	int send_questhelp_query_book (uint32 dynasty);
	/*下发可接天书任务*/
	int unpack_questhelp_book_quest (ByteArray &bytes ,uint32 &quest_id);
	int send_questhelp_book_quest (uint32 quest_id);
	/*玩家使用游戏对象以后的动作*/
	int unpack_use_gameobject_action (ByteArray &bytes ,uint32 &player_id,uint32 &gameobject_id);
	int send_use_gameobject_action (uint32 player_id,uint32 gameobject_id);
	/*设置攻击模式*/
	int unpack_set_attack_mode (ByteArray &bytes ,uint8 &mode,uint64 &reserve);
	int send_set_attack_mode (uint8 mode,uint64 reserve);
	/*选择目标*/
	int unpack_select_target (ByteArray &bytes ,uint32 &id);
	int send_select_target (uint32 id);
	/*进入战斗*/
	int unpack_combat_state_update (ByteArray &bytes ,uint8 &cur_state);
	int send_combat_state_update (uint8 cur_state);
	/*经验更新*/
	int unpack_exp_update (ByteArray &bytes ,int32 &exp,uint8 &type,int32 &vip_exp);
	int send_exp_update (int32 exp,uint8 type,int32 vip_exp);
	/*客户端释放技能*/
	int unpack_spell_start (ByteArray &bytes ,uint32 &spell_id,uint16 &target_pos_x,uint16 &target_pos_y,uint32 &caster,uint32 &target);
	int send_spell_start (uint32 spell_id,uint16 target_pos_x,uint16 target_pos_y,uint32 caster,uint32 target);
	/*施法停止*/
	int unpack_spell_stop (ByteArray &bytes ,string &guid);
	int send_spell_stop (char const*guid);
	/*跳*/
	int unpack_jump (ByteArray &bytes ,uint32 &guid,float &pos_x,float &pos_y);
	int send_jump (uint32 guid,float pos_x,float pos_y);
	/*复活*/
	int unpack_resurrection (ByteArray &bytes ,uint8 &type,uint64 &reserve);
	int send_resurrection (uint8 type,uint64 reserve);
	/*交易发出请求*/
	int unpack_trade_request (ByteArray &bytes ,string &guid);
	int send_trade_request (char const*guid);
	/*交易请求答复*/
	int unpack_trade_reply (ByteArray &bytes ,string &guid,uint8 &reply);
	int send_trade_reply (char const*guid,uint8 reply);
	/*交易开始*/
	int unpack_trade_start (ByteArray &bytes ,string &guid);
	int send_trade_start (char const*guid);
	/*交易确认物品*/
	int unpack_trade_decide_items (ByteArray &bytes ,const string &items, uint16 *len_1,int32 &gold_ingot,int32 &silver);
	int send_trade_decide_items (char const*items,int32 gold_ingot,int32 silver);
	/*交易完成*/
	int unpack_trade_finish (ByteArray &bytes );
	int send_trade_finish ();
	/*交易取消*/
	int unpack_trade_cancel (ByteArray &bytes );
	int send_trade_cancel ();
	/*交易准备好*/
	int unpack_trade_ready (ByteArray &bytes );
	int send_trade_ready ();
	/*生物讲话*/
	int unpack_chat_unit_talk (ByteArray &bytes ,uint32 &guid,uint32 &content,string &say);
	int send_chat_unit_talk (uint32 guid,uint32 content,char const*say);
	/*就近聊天*/
	int unpack_chat_near (ByteArray &bytes ,string &content);
	int send_chat_near (char const*content);
	/*私聊*/
	int unpack_chat_whisper (ByteArray &bytes ,string &guid,string &content);
	int send_chat_whisper (char const*guid,char const*content);
	/*阵营聊天*/
	int unpack_chat_faction (ByteArray &bytes ,string &guid,string &name,string &content,uint8 &faction);
	int send_chat_faction (char const*guid,char const*name,char const*content,uint8 faction);
	/*世界*/
	int unpack_chat_world (ByteArray &bytes ,string &guid,uint8 &faction,string &name,string &content);
	int send_chat_world (char const*guid,uint8 faction,char const*name,char const*content);
	/*喇叭*/
	int unpack_chat_horn (ByteArray &bytes ,string &guid,uint8 &faction,string &name,string &content);
	int send_chat_horn (char const*guid,uint8 faction,char const*name,char const*content);
	/*公告*/
	int unpack_chat_notice (ByteArray &bytes ,uint32 &id,string &content,string &data);
	int send_chat_notice (uint32 id,char const*content,char const*data);
	/*查询玩家信息*/
	int unpack_query_player_info (ByteArray &bytes ,string &guid,uint32 &flag,uint32 &callback_id);
	int send_query_player_info (char const*guid,uint32 flag,uint32 callback_id);
	/*查询信息对象更新*/
	int unpack_query_result_update_object (ByteArray &bytes );
	int send_query_result_update_object ();
	/*领取礼包*/
	int unpack_receive_gift_packs (ByteArray &bytes );
	int send_receive_gift_packs ();
	/*地图对象更新*/
	int unpack_map_update_object (ByteArray &bytes );
	int send_map_update_object ();
	/*战斗信息binlog*/
	int unpack_fighting_info_update_object (ByteArray &bytes );
	int send_fighting_info_update_object ();
	/*战斗信息binlog*/
	int unpack_fighting_info_update_object_2 (ByteArray &bytes );
	int send_fighting_info_update_object_2 ();
	/*进入副本*/
	int unpack_instance_enter (ByteArray &bytes ,uint32 &instance_id);
	int send_instance_enter (uint32 instance_id);
	/*向服务端发送副本进入下一阶段指令*/
	int unpack_instance_next_state (ByteArray &bytes ,uint16 &level,uint32 &param);
	int send_instance_next_state (uint16 level,uint32 param);
	/*副本退出*/
	int unpack_instance_exit (ByteArray &bytes ,uint32 &reserve);
	int send_instance_exit (uint32 reserve);
	/*限时活动领取*/
	int unpack_limit_activity_receive (ByteArray &bytes ,uint32 &id,uint32 &type);
	int send_limit_activity_receive (uint32 id,uint32 type);
	/*杀人啦~~！！！！*/
	int unpack_kill_man (ByteArray &bytes ,string &killer,string &killer_name,string &victim,string &victim_name);
	int send_kill_man (char const*killer,char const*killer_name,char const*victim,char const*victim_name);
	/*玩家升级*/
	int unpack_player_upgrade (ByteArray &bytes ,uint32 &guid);
	int send_player_upgrade (uint32 guid);
	/*仓库存钱*/
	int unpack_warehouse_save_money (ByteArray &bytes ,int32 &money,int32 &money_gold,int32 &money_bills);
	int send_warehouse_save_money (int32 money,int32 money_gold,int32 money_bills);
	/*仓库取钱*/
	int unpack_warehouse_take_money (ByteArray &bytes ,int32 &money,int32 &money_gold,int32 &money_bills);
	int send_warehouse_take_money (int32 money,int32 money_gold,int32 money_bills);
	/*使用元宝操作某事*/
	int unpack_use_gold_opt (ByteArray &bytes ,uint8 &type,string &param);
	int send_use_gold_opt (uint8 type,char const*param);
	/*使用铜钱操作某事*/
	int unpack_use_silver_opt (ByteArray &bytes ,uint8 &type);
	int send_use_silver_opt (uint8 type);
	/*后台弹窗*/
	int unpack_gm_rightfloat (ByteArray &bytes ,uint32 &id,uint32 &end_time,uint32 &zone1,uint32 &zone2,uint32 &zone3,string &subject,string &content,string &link,uint8 &mode);
	int send_gm_rightfloat (uint32 id,uint32 end_time,uint32 zone1,uint32 zone2,uint32 zone3,char const*subject,char const*content,char const*link,uint8 mode);
	/*删除某条后台弹窗*/
	int unpack_del_gm_rightfloat (ByteArray &bytes ,uint32 &id);
	int send_del_gm_rightfloat (uint32 id);
	/*应用服同步时间*/
	int unpack_sync_mstime_app (ByteArray &bytes ,uint32 &mstime_now,uint32 &time_now,uint32 &open_time);
	int send_sync_mstime_app (uint32 mstime_now,uint32 time_now,uint32 open_time);
	/*玩家打开某个窗口*/
	int unpack_open_window (ByteArray &bytes ,uint32 &window_type);
	int send_open_window (uint32 window_type);
	/*禁言操作*/
	int unpack_player_gag (ByteArray &bytes ,string &player_id,uint32 &end_time,string &content);
	int send_player_gag (char const*player_id,uint32 end_time,char const*content);
	/*踢人操作*/
	int unpack_player_kicking (ByteArray &bytes ,string &player_id);
	int send_player_kicking (char const*player_id);
	/*合服通知*/
	int unpack_merge_server_msg (ByteArray &bytes ,string &merge_host,uint32 &merge_port,string &merge_key,uint32 &merge_type,uint32 &reserve,uint32 &reserve2);
	int send_merge_server_msg (char const*merge_host,uint32 merge_port,char const*merge_key,uint32 merge_type,uint32 reserve,uint32 reserve2);
	/*获取排行信息*/
	int unpack_rank_list_query (ByteArray &bytes ,uint32 &call_back_id,uint8 &rank_list_type,uint16 &start_index,uint16 &end_index);
	int send_rank_list_query (uint32 call_back_id,uint8 rank_list_type,uint16 start_index,uint16 end_index);
	/*客户端获取排行榜返回结果*/
	int unpack_rank_list_query_result (ByteArray &bytes );
	int send_rank_list_query_result ();
	/*客户端热更场景模块后获取uint*/
	int unpack_client_update_scened (ByteArray &bytes );
	int send_client_update_scened ();
	/*数值包*/
	int unpack_num_lua (ByteArray &bytes );
	int send_num_lua ();
	/*战利品拾取*/
	int unpack_loot_select (ByteArray &bytes ,uint16 &x,uint16 &y);
	int send_loot_select (uint16 x,uint16 y);
	/*通知登录服把玩家传回游戏服*/
	int unpack_goback_to_game_server (ByteArray &bytes );
	int send_goback_to_game_server ();
	/*客户端把比赛人员数据传给比赛服*/
	int unpack_world_war_CS_player_info (ByteArray &bytes );
	int send_world_war_CS_player_info ();
	/*玩家加入或者离开某服务器*/
	int unpack_join_or_leave_server (ByteArray &bytes ,uint8 &join_or_leave,uint8 &server_type,uint32 &server_pid,uint32 &server_fd,uint32 &time);
	int send_join_or_leave_server (uint8 join_or_leave,uint8 server_type,uint32 server_pid,uint32 server_fd,uint32 time);
	/*客户端请求跨服人员数据*/
	int unpack_world_war_SC_player_info (ByteArray &bytes );
	int send_world_war_SC_player_info ();
	/*客户端订阅信息*/
	int unpack_clientSubscription (ByteArray &bytes ,uint32 &guid);
	int send_clientSubscription (uint32 guid);
	/*服务端下发lua脚本*/
	int unpack_lua_script (ByteArray &bytes );
	int send_lua_script ();
	/*角色更改信息*/
	int unpack_char_update_info (ByteArray &bytes , char_create_info **info);
	int send_char_update_info (char_create_info *info);
	/*通知客户端观察者的视角*/
	int unpack_notice_watcher_map_info (ByteArray &bytes ,string &wather_guid,uint32 &map_id,uint32 &instance_id);
	int send_notice_watcher_map_info (char const*wather_guid,uint32 map_id,uint32 instance_id);
	/*客户端订阅对象信息*/
	int unpack_modify_watch (ByteArray &bytes ,uint8 &opt,uint32 &cid,string &key);
	int send_modify_watch (uint8 opt,uint32 cid,char const*key);
	/*跨服传送*/
	int unpack_kuafu_chuansong (ByteArray &bytes ,string &str_data,string &watcher_guid,uint32 &reserve);
	int send_kuafu_chuansong (char const*str_data,char const*watcher_guid,uint32 reserve);
	/*显示当前装备*/
	int unpack_show_suit (ByteArray &bytes ,uint8 &position);
	int send_show_suit (uint8 position);
	/*显示当前坐标*/
	int unpack_show_position (ByteArray &bytes ,uint8 &channel);
	int send_show_position (uint8 channel);
	/*元宝复活*/
	int unpack_gold_respawn (ByteArray &bytes ,uint8 &useGold);
	int send_gold_respawn (uint8 useGold);
	/*野外死亡倒计时*/
	int unpack_field_death_cooldown (ByteArray &bytes ,uint8 &type,uint32 &value,string &killername,uint16 &cooldown,string &params);
	int send_field_death_cooldown (uint8 type,uint32 value,char const*killername,uint16 cooldown,char const*params);
	/*商城购买*/
	int unpack_mall_buy (ByteArray &bytes ,uint32 &id,uint32 &count,uint32 &time);
	int send_mall_buy (uint32 id,uint32 count,uint32 time);
	/*强化*/
	int unpack_strength (ByteArray &bytes ,uint8 &part);
	int send_strength (uint8 part);
	/*强化成功*/
	int unpack_strength_success (ByteArray &bytes ,uint16 &level);
	int send_strength_success (uint16 level);
	/*强制进入*/
	int unpack_forceInto (ByteArray &bytes );
	int send_forceInto ();
	/*创建帮派*/
	int unpack_create_faction (ByteArray &bytes ,string &name,uint8 &icon);
	int send_create_faction (char const*name,uint8 icon);
	/*升级帮派*/
	int unpack_faction_upgrade (ByteArray &bytes );
	int send_faction_upgrade ();
	/*申请加入帮派*/
	int unpack_faction_join (ByteArray &bytes ,string &id);
	int send_faction_join (char const*id);
	/*申请升级技能*/
	int unpack_raise_base_spell (ByteArray &bytes ,uint8 &raiseType,uint16 &spellId);
	int send_raise_base_spell (uint8 raiseType,uint16 spellId);
	/*申请升阶愤怒技能*/
	int unpack_upgrade_anger_spell (ByteArray &bytes ,uint16 &spellId);
	int send_upgrade_anger_spell (uint16 spellId);
	/*申请升级坐骑*/
	int unpack_raise_mount (ByteArray &bytes );
	int send_raise_mount ();
	/*申请升阶坐骑*/
	int unpack_upgrade_mount (ByteArray &bytes ,uint8 &useItem);
	int send_upgrade_mount (uint8 useItem);
	/*申请一键升阶坐骑*/
	int unpack_upgrade_mount_one_step (ByteArray &bytes ,uint8 &useItem);
	int send_upgrade_mount_one_step (uint8 useItem);
	/*申请解锁幻化坐骑*/
	int unpack_illusion_mount_active (ByteArray &bytes ,uint16 &illuId);
	int send_illusion_mount_active (uint16 illuId);
	/*申请幻化坐骑*/
	int unpack_illusion_mount (ByteArray &bytes ,uint16 &illuId);
	int send_illusion_mount (uint16 illuId);
	/*申请骑乘*/
	int unpack_ride_mount (ByteArray &bytes );
	int send_ride_mount ();
	/*grid中的unit跳跃*/
	int unpack_grid_unit_jump (ByteArray &bytes );
	int send_grid_unit_jump ();
	/*宝石*/
	int unpack_gem (ByteArray &bytes ,uint8 &part);
	int send_gem (uint8 part);
	/*请求切换模式*/
	int unpack_change_battle_mode (ByteArray &bytes ,uint8 &mode);
	int send_change_battle_mode (uint8 mode);
	/*和平模式CD*/
	int unpack_peace_mode_cd (ByteArray &bytes ,uint8 &mode);
	int send_peace_mode_cd (uint8 mode);
	/*激活神兵*/
	int unpack_divine_active (ByteArray &bytes ,uint8 &id);
	int send_divine_active (uint8 id);
	/*激活神兵*/
	int unpack_divine_uplev (ByteArray &bytes ,uint8 &id);
	int send_divine_uplev (uint8 id);
	/*切换神兵*/
	int unpack_divine_switch (ByteArray &bytes ,uint8 &id);
	int send_divine_switch (uint8 id);
	/*请求跳跃*/
	int unpack_jump_start (ByteArray &bytes ,uint16 &pos_x,uint16 &pos_y);
	int send_jump_start (uint16 pos_x,uint16 pos_y);
	/*请求进入vip副本*/
	int unpack_enter_vip_instance (ByteArray &bytes ,uint16 &id,uint8 &hard);
	int send_enter_vip_instance (uint16 id,uint8 hard);
	/*请求扫荡vip副本*/
	int unpack_sweep_vip_instance (ByteArray &bytes ,uint16 &id);
	int send_sweep_vip_instance (uint16 id);
	/*进行挂机*/
	int unpack_hang_up (ByteArray &bytes );
	int send_hang_up ();
	/*进行挂机设置*/
	int unpack_hang_up_setting (ByteArray &bytes ,uint32 &value0,uint32 &value1,uint32 &value2,uint32 &value3);
	int send_hang_up_setting (uint32 value0,uint32 value1,uint32 value2,uint32 value3);
	/*请求进入试炼塔副本*/
	int unpack_enter_trial_instance (ByteArray &bytes );
	int send_enter_trial_instance ();
	/*扫荡试炼塔副本*/
	int unpack_sweep_trial_instance (ByteArray &bytes );
	int send_sweep_trial_instance ();
	/*重置试炼塔*/
	int unpack_reset_trial_instance (ByteArray &bytes );
	int send_reset_trial_instance ();
	/*扫荡副本奖励*/
	int unpack_sweep_instance_reward (ByteArray &bytes ,uint8 &inst_sub_type,uint8 &data1,uint8 &data2,uint8 &data3, vector< item_reward_info > &list);
	int send_sweep_instance_reward (uint8 inst_sub_type,uint8 data1,uint8 data2,uint8 data3,const vector< item_reward_info > &list );
	/*重进副本*/
	int unpack_reenter_instance (ByteArray &bytes );
	int send_reenter_instance ();
	/*走马灯信息*/
	int unpack_merry_go_round (ByteArray &bytes );
	int send_merry_go_round ();
	/*添加好友*/
	int unpack_social_add_friend (ByteArray &bytes ,string &guid);
	int send_social_add_friend (char const*guid);
	/*同意添加好友*/
	int unpack_social_sureadd_friend (ByteArray &bytes ,string &guid);
	int send_social_sureadd_friend (char const*guid);
	/*赠送礼物*/
	int unpack_social_gift_friend (ByteArray &bytes ,string &guid, vector< item_reward_info > &gift);
	int send_social_gift_friend (char const*guid,const vector< item_reward_info > &gift );
	/*推荐好友列表*/
	int unpack_social_recommend_friend (ByteArray &bytes );
	int send_social_recommend_friend ();
	/*推荐好友列表*/
	int unpack_social_get_recommend_friend (ByteArray &bytes , vector< social_friend_info > &list);
	int send_social_get_recommend_friend (const vector< social_friend_info > &list );
	/*复仇*/
	int unpack_social_revenge_enemy (ByteArray &bytes ,string &guid);
	int send_social_revenge_enemy (char const*guid);
	/*删除好友*/
	int unpack_social_del_friend (ByteArray &bytes ,string &guid);
	int send_social_del_friend (char const*guid);
	/*回城*/
	int unpack_teleport_main_city (ByteArray &bytes );
	int send_teleport_main_city ();
	/*不同频道聊天*/
	int unpack_chat_by_channel (ByteArray &bytes ,uint8 &channel,string &content);
	int send_chat_by_channel (uint8 channel,char const*content);
	/*发送聊天*/
	int unpack_send_chat (ByteArray &bytes ,uint8 &channel,string &guid,uint8 &title,string &name,uint8 &vip,uint8 &zs,uint8 &lvl,uint8 &gender,string &content,string &to_guid,string &faction_guid);
	int send_send_chat (uint8 channel,char const*guid,uint8 title,char const*name,uint8 vip,uint8 zs,uint8 lvl,uint8 gender,char const*content,char const*to_guid,char const*faction_guid);
	/*清空申请列表*/
	int unpack_social_clear_apply (ByteArray &bytes );
	int send_social_clear_apply ();
	/*设置拒绝接受消息*/
	int unpack_msg_decline (ByteArray &bytes ,uint32 &value0,uint32 &value1);
	int send_msg_decline (uint32 value0,uint32 value1);
	/*帮派列表*/
	int unpack_faction_get_list_result (ByteArray &bytes , vector< faction_info > &list,uint8 &curpag,uint8 &maxpag);
	int send_faction_get_list_result (const vector< faction_info > &list ,uint8 curpag,uint8 maxpag);
	/*获取帮派列表*/
	int unpack_faction_getlist (ByteArray &bytes ,uint8 &page,uint8 &num,uint8 &grep);
	int send_faction_getlist (uint8 page,uint8 num,uint8 grep);
	/*帮派管理*/
	int unpack_faction_manager (ByteArray &bytes ,uint8 &opt_type,uint16 &reserve_int1,uint16 &reserve_int2,string &reserve_str1,string &reserve_str2);
	int send_faction_manager (uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2);
	/*帮派成员操作*/
	int unpack_faction_member_operate (ByteArray &bytes ,uint8 &opt_type,uint16 &reserve_int1,uint16 &reserve_int2,string &reserve_str1,string &reserve_str2);
	int send_faction_member_operate (uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2);
	/*快速加入帮派*/
	int unpack_faction_fast_join (ByteArray &bytes );
	int send_faction_fast_join ();
	/*通过名字添加好友*/
	int unpack_social_add_friend_byname (ByteArray &bytes ,string &name);
	int send_social_add_friend_byname (char const*name);
	/*读邮件*/
	int unpack_read_mail (ByteArray &bytes ,uint16 &indx);
	int send_read_mail (uint16 indx);
	/*领取邮件*/
	int unpack_pick_mail (ByteArray &bytes ,uint16 &indx);
	int send_pick_mail (uint16 indx);
	/*删除邮件*/
	int unpack_remove_mail (ByteArray &bytes ,uint16 &indx);
	int send_remove_mail (uint16 indx);
	/*一键领取邮件*/
	int unpack_pick_mail_one_step (ByteArray &bytes );
	int send_pick_mail_one_step ();
	/*一键删除邮件*/
	int unpack_remove_mail_one_step (ByteArray &bytes );
	int send_remove_mail_one_step ();
	/*屏蔽某人*/
	int unpack_block_chat (ByteArray &bytes ,string &guid);
	int send_block_chat (char const*guid);
	/*取消屏蔽*/
	int unpack_cancel_block_chat (ByteArray &bytes ,uint8 &indx);
	int send_cancel_block_chat (uint8 indx);
	/*使用需要广播的游戏对象*/
	int unpack_use_broadcast_gameobject (ByteArray &bytes ,uint32 &target);
	int send_use_broadcast_gameobject (uint32 target);
	/*世界BOSS报名*/
	int unpack_world_boss_enroll (ByteArray &bytes );
	int send_world_boss_enroll ();
	/*世界BOSS挑战*/
	int unpack_world_boss_fight (ByteArray &bytes );
	int send_world_boss_fight ();
	/*换线*/
	int unpack_change_line (ByteArray &bytes ,uint32 &lineNo);
	int send_change_line (uint32 lineNo);
	/*roll世界BOSS箱子*/
	int unpack_roll_world_boss_treasure (ByteArray &bytes );
	int send_roll_world_boss_treasure ();
	/*roll点结果*/
	int unpack_roll_result (ByteArray &bytes ,uint8 &point,string &name,uint8 &isHighest,uint32 &timestamp,uint8 &rollid);
	int send_roll_result (uint8 point,char const*name,uint8 isHighest,uint32 timestamp,uint8 rollid);
	/*当前BOSS伤害排名*/
	int unpack_boss_rank (ByteArray &bytes ,uint8 &rankType, vector< rank_info > &rankList,uint8 &mine);
	int send_boss_rank (uint8 rankType,const vector< rank_info > &rankList ,uint8 mine);
	/*排行榜点赞*/
	int unpack_rank_add_like (ByteArray &bytes ,uint8 &type,string &guid);
	int send_rank_add_like (uint8 type,char const*guid);
	/*排行榜点赞结果*/
	int unpack_rank_add_like_result (ByteArray &bytes ,uint8 &type,string &guid,uint32 &num);
	int send_rank_add_like_result (uint8 type,char const*guid,uint32 num);
	/*进入资源副本*/
	int unpack_res_instance_enter (ByteArray &bytes ,uint8 &id);
	int send_res_instance_enter (uint8 id);
	/*扫荡资源副本*/
	int unpack_res_instance_sweep (ByteArray &bytes ,uint8 &id);
	int send_res_instance_sweep (uint8 id);
	/*查看本地图的分线号*/
	int unpack_show_map_line (ByteArray &bytes );
	int send_show_map_line ();
	/*返回本地图的分线号信息*/
	int unpack_send_map_line (ByteArray &bytes , vector< line_info > &info);
	int send_send_map_line (const vector< line_info > &info );
	/*获得奖励提示*/
	int unpack_item_notice (ByteArray &bytes ,uint8 &showType, vector< item_reward_info > &list);
	int send_item_notice (uint8 showType,const vector< item_reward_info > &list );
	/*传送到某个世界地图*/
	int unpack_teleport_map (ByteArray &bytes ,uint32 &mapid,uint32 &lineNo);
	int send_teleport_map (uint32 mapid,uint32 lineNo);
	/*传送到野外boss旁边*/
	int unpack_teleport_field_boss (ByteArray &bytes ,uint32 &mapid,uint32 &lineNo);
	int send_teleport_field_boss (uint32 mapid,uint32 lineNo);
	/*活跃度奖励*/
	int unpack_get_activity_reward (ByteArray &bytes ,uint8 &id,uint8 &vip);
	int send_get_activity_reward (uint8 id,uint8 vip);
	/*成就奖励*/
	int unpack_get_achieve_reward (ByteArray &bytes ,uint8 &id);
	int send_get_achieve_reward (uint8 id);
	/*总成就奖励*/
	int unpack_get_achieve_all_reward (ByteArray &bytes );
	int send_get_achieve_all_reward ();
	/*装备称号*/
	int unpack_set_title (ByteArray &bytes ,uint8 &id);
	int send_set_title (uint8 id);
	/*初始化称号*/
	int unpack_init_title (ByteArray &bytes ,uint8 &id);
	int send_init_title (uint8 id);
	/*领取首充奖励*/
	int unpack_welfare_shouchong_reward (ByteArray &bytes );
	int send_welfare_shouchong_reward ();
	/*每日签到奖励*/
	int unpack_welfare_checkin (ByteArray &bytes );
	int send_welfare_checkin ();
	/*累积签到奖励*/
	int unpack_welfare_checkin_all (ByteArray &bytes ,uint8 &id);
	int send_welfare_checkin_all (uint8 id);
	/*补签奖励*/
	int unpack_welfare_checkin_getback (ByteArray &bytes ,uint8 &id);
	int send_welfare_checkin_getback (uint8 id);
	/*等级奖励*/
	int unpack_welfare_level (ByteArray &bytes ,uint8 &id);
	int send_welfare_level (uint8 id);
	/*活动找回奖励*/
	int unpack_welfare_active_getback (ByteArray &bytes ,uint8 &id,uint8 &best,uint16 &num);
	int send_welfare_active_getback (uint8 id,uint8 best,uint16 num);
	/*领取任务奖励*/
	int unpack_pick_quest_reward (ByteArray &bytes ,uint8 &indx);
	int send_pick_quest_reward (uint8 indx);
	/*和npc对话*/
	int unpack_talk_with_npc (ByteArray &bytes ,uint16 &entry,uint16 &questId);
	int send_talk_with_npc (uint16 entry,uint16 questId);
	/*使用虚拟物品*/
	int unpack_use_virtual_item (ByteArray &bytes ,uint16 &entry);
	int send_use_virtual_item (uint16 entry);
	/*领取任务章节奖励*/
	int unpack_pick_quest_chapter_reward (ByteArray &bytes ,uint8 &indx);
	int send_pick_quest_chapter_reward (uint8 indx);
	/*3v3跨服匹配*/
	int unpack_kuafu_3v3_match (ByteArray &bytes );
	int send_kuafu_3v3_match ();
	/*跨服开始匹配*/
	int unpack_kuafu_match_start (ByteArray &bytes ,uint8 &indx);
	int send_kuafu_match_start (uint8 indx);
	/*3v3购买次数*/
	int unpack_kuafu_3v3_buytimes (ByteArray &bytes ,uint8 &num);
	int send_kuafu_3v3_buytimes (uint8 num);
	/*3v3每日活跃奖励*/
	int unpack_kuafu_3v3_dayreward (ByteArray &bytes ,uint8 &id);
	int send_kuafu_3v3_dayreward (uint8 id);
	/*请求3v3排行榜*/
	int unpack_kuafu_3v3_getranlist (ByteArray &bytes );
	int send_kuafu_3v3_getranlist ();
	/*3v3排行榜结果列表*/
	int unpack_kuafu_3v3_ranlist (ByteArray &bytes ,string &list);
	int send_kuafu_3v3_ranlist (char const*list);
	/*福利所有奖励列表*/
	int unpack_welfare_getalllist_getback (ByteArray &bytes ,uint8 &best);
	int send_welfare_getalllist_getback (uint8 best);
	/*奖励列表*/
	int unpack_welfare_rewardlist_getback (ByteArray &bytes ,string &list,string &cost);
	int send_welfare_rewardlist_getback (char const*list,char const*cost);
	/*一键领取所有福利*/
	int unpack_welfare_getall_getback (ByteArray &bytes ,uint8 &best);
	int send_welfare_getall_getback (uint8 best);
	/*请求3v3排行榜自己的名次*/
	int unpack_kuafu_3v3_getmyrank (ByteArray &bytes );
	int send_kuafu_3v3_getmyrank ();
	/*3v3排行榜自己的名次结果*/
	int unpack_kuafu_3v3_myrank (ByteArray &bytes ,uint8 &rank);
	int send_kuafu_3v3_myrank (uint8 rank);
	/*击杀数据*/
	int unpack_kuafu_3v3_kill_detail (ByteArray &bytes ,uint32 &indx1,uint32 &indx2);
	int send_kuafu_3v3_kill_detail (uint32 indx1,uint32 indx2);
	/*跨服匹配等待数据*/
	int unpack_kuafu_3v3_wait_info (ByteArray &bytes , vector< wait_info > &list);
	int send_kuafu_3v3_wait_info (const vector< wait_info > &list );
	/*取消匹配*/
	int unpack_kuafu_3v3_cancel_match (ByteArray &bytes ,uint32 &type);
	int send_kuafu_3v3_cancel_match (uint32 type);
	/*匹配到人&接受或者拒绝*/
	int unpack_kuafu_3v3_match_oper (ByteArray &bytes ,uint32 &oper);
	int send_kuafu_3v3_match_oper (uint32 oper);
	/*拒绝比赛*/
	int unpack_kuafu_3v3_decline_match (ByteArray &bytes ,uint32 &type);
	int send_kuafu_3v3_decline_match (uint32 type);
	/*仙府夺宝跨服匹配*/
	int unpack_kuafu_xianfu_match (ByteArray &bytes ,uint8 &indx);
	int send_kuafu_xianfu_match (uint8 indx);
	/*单方跨服匹配等待*/
	int unpack_kuafu_match_wait (ByteArray &bytes ,uint8 &type,uint8 &target,uint8 &count,uint32 &data,string &params);
	int send_kuafu_match_wait (uint8 type,uint8 target,uint8 count,uint32 data,char const*params);
	/*仙府夺宝小地图信息*/
	int unpack_kuafu_xianfu_minimap_info (ByteArray &bytes );
	int send_kuafu_xianfu_minimap_info ();
	/*购买仙府进入券*/
	int unpack_buy_xianfu_item (ByteArray &bytes ,uint8 &type,uint8 &indx,uint16 &count);
	int send_buy_xianfu_item (uint8 type,uint8 indx,uint16 count);
	/*随机复活*/
	int unpack_xianfu_random_respawn (ByteArray &bytes );
	int send_xianfu_random_respawn ();
	/*斗剑台挑战*/
	int unpack_doujiantai_fight (ByteArray &bytes ,uint16 &rank);
	int send_doujiantai_fight (uint16 rank);
	/*斗剑台购买次数*/
	int unpack_doujiantai_buytime (ByteArray &bytes ,uint8 &num);
	int send_doujiantai_buytime (uint8 num);
	/*斗剑台清理CD*/
	int unpack_doujiantai_clearcd (ByteArray &bytes );
	int send_doujiantai_clearcd ();
	/*斗剑台首胜奖励*/
	int unpack_doujiantai_first_reward (ByteArray &bytes ,uint8 &id);
	int send_doujiantai_first_reward (uint8 id);
	/*斗剑台挑战对手信息*/
	int unpack_doujiantai_get_enemys_info (ByteArray &bytes );
	int send_doujiantai_get_enemys_info ();
	/*斗剑台排行榜*/
	int unpack_doujiantai_get_rank (ByteArray &bytes ,uint16 &startIdx,uint16 &endIdx);
	int send_doujiantai_get_rank (uint16 startIdx,uint16 endIdx);
	/*斗剑台刷新对手*/
	int unpack_doujiantai_refresh_enemys (ByteArray &bytes );
	int send_doujiantai_refresh_enemys ();
	/*斗剑台三甲*/
	int unpack_doujiantai_top3 (ByteArray &bytes );
	int send_doujiantai_top3 ();
	/*使用跳点*/
	int unpack_use_jump_point (ByteArray &bytes ,uint32 &id);
	int send_use_jump_point (uint32 id);
	/*出售物品*/
	int unpack_bag_item_sell (ByteArray &bytes ,string &item_guid,uint32 &count);
	int send_bag_item_sell (char const*item_guid,uint32 count);
	/*整理物品*/
	int unpack_bag_item_sort (ByteArray &bytes );
	int send_bag_item_sort ();
	/*提交日常任务*/
	int unpack_submit_quest_daily2 (ByteArray &bytes );
	int send_submit_quest_daily2 ();
	/*属性改变*/
	int unpack_attribute_changed (ByteArray &bytes );
	int send_attribute_changed ();
	/*背包有更强装备*/
	int unpack_bag_find_equip_better (ByteArray &bytes ,uint32 &item_id,uint32 &pos);
	int send_bag_find_equip_better (uint32 item_id,uint32 pos);
	/*模块解锁*/
	int unpack_module_active (ByteArray &bytes ,uint32 &moduleId);
	int send_module_active (uint32 moduleId);
	/*领取日常任务奖励*/
	int unpack_pick_daily2_quest_reward (ByteArray &bytes ,uint8 &indx);
	int send_pick_daily2_quest_reward (uint8 indx);
	/*完成当前引导*/
	int unpack_finish_now_guide (ByteArray &bytes );
	int send_finish_now_guide ();
	/*取得修炼场信息*/
	int unpack_get_cultivation_info (ByteArray &bytes );
	int send_get_cultivation_info ();
	/*返回修炼场信息*/
	int unpack_update_cultivation_info (ByteArray &bytes ,uint32 &start_time,uint32 &lost);
	int send_update_cultivation_info (uint32 start_time,uint32 lost);
	/*取得当前所有修炼场对手信息*/
	int unpack_get_cultivation_rivals_info (ByteArray &bytes );
	int send_get_cultivation_rivals_info ();
	/*返回修炼场对手信息*/
	int unpack_update_cultivation_rivals_info_list (ByteArray &bytes , vector< cultivation_rivals_info > &list);
	int send_update_cultivation_rivals_info_list (const vector< cultivation_rivals_info > &list );
	/*领取修炼场奖励*/
	int unpack_get_cultivation_reward (ByteArray &bytes );
	int send_get_cultivation_reward ();
	/*刷新修炼场对手*/
	int unpack_refresh_cultivation_rivals (ByteArray &bytes );
	int send_refresh_cultivation_rivals ();
	/*掠夺修炼场对手*/
	int unpack_plunder_cultivation_rival (ByteArray &bytes ,uint32 &index);
	int send_plunder_cultivation_rival (uint32 index);
	/*反击复仇修炼场对手*/
	int unpack_revenge_cultivation_rival (ByteArray &bytes ,uint32 &index);
	int send_revenge_cultivation_rival (uint32 index);
	/*增加修炼场剩余挑战次数*/
	int unpack_buy_cultivation_left_plunder_count (ByteArray &bytes ,uint32 &count);
	int send_buy_cultivation_left_plunder_count (uint32 count);
	/*返回修炼场战斗结果*/
	int unpack_show_cultivation_result_list (ByteArray &bytes ,int32 &result,string &name, vector< item_reward_info > &list);
	int send_show_cultivation_result_list (int32 result,char const*name,const vector< item_reward_info > &list );
	/*领取登录大礼奖励*/
	int unpack_get_login_activity_reward (ByteArray &bytes ,uint32 &id);
	int send_get_login_activity_reward (uint32 id);
	/*通知客户端释放蓄力技能*/
	int unpack_cast_spell_start (ByteArray &bytes ,uint32 &caster_guid,uint32 &target_guid,uint16 &spellid,string &data);
	int send_cast_spell_start (uint32 caster_guid,uint32 target_guid,uint16 spellid,char const*data);
	/*完成非强制引导的步骤*/
	int unpack_finish_optional_guide_step (ByteArray &bytes ,uint32 &guide_id,uint8 &step);
	int send_finish_optional_guide_step (uint32 guide_id,uint8 step);
	/*执行接到任务以后的命令*/
	int unpack_execute_quest_cmd_after_accepted (ByteArray &bytes ,uint16 &indx);
	int send_execute_quest_cmd_after_accepted (uint16 indx);
	/*通知客户端显示属性*/
	int unpack_show_unit_attribute (ByteArray &bytes );
	int send_show_unit_attribute ();
	/*返回家族*/
	int unpack_back_to_famity (ByteArray &bytes );
	int send_back_to_famity ();
	/*返回家族boss结果*/
	int unpack_faction_boss_send_result (ByteArray &bytes ,uint32 &result,uint32 &boss_id,uint32 &money);
	int send_faction_boss_send_result (uint32 result,uint32 boss_id,uint32 money);
	/*挑战boss*/
	int unpack_challange_boss (ByteArray &bytes );
	int send_challange_boss ();
	/*领取离线奖励*/
	int unpack_pick_offline_reward (ByteArray &bytes );
	int send_pick_offline_reward ();
	/*离线奖励结果*/
	int unpack_offline_reward_result (ByteArray &bytes ,uint32 &reserve,uint32 &reserve2,uint32 &reserve3,uint32 &reserve4, vector< item_reward_info > &list);
	int send_offline_reward_result (uint32 reserve,uint32 reserve2,uint32 reserve3,uint32 reserve4,const vector< item_reward_info > &list );
	/*熔炼装备*/
	int unpack_smelting_equip (ByteArray &bytes ,string &pos_str);
	int send_smelting_equip (char const*pos_str);
	/*上交装备*/
	int unpack_storehouse_hand_in (ByteArray &bytes ,string &pos_str);
	int send_storehouse_hand_in (char const*pos_str);
	/*兑换装备*/
	int unpack_storehouse_exchange (ByteArray &bytes ,uint32 &pos);
	int send_storehouse_exchange (uint32 pos);
	/*销毁装备*/
	int unpack_storehouse_destroy (ByteArray &bytes ,uint32 &pos);
	int send_storehouse_destroy (uint32 pos);
	/*赠送礼物*/
	int unpack_send_faction_gift (ByteArray &bytes , vector< item_reward_info > &list,string &msg,uint32 &msg_type);
	int send_send_faction_gift (const vector< item_reward_info > &list ,char const*msg,uint32 msg_type);
	/*领取额外奖励*/
	int unpack_get_faction_gift_exreward (ByteArray &bytes ,uint32 &count_id);
	int send_get_faction_gift_exreward (uint32 count_id);
	/*领取所有额外奖励*/
	int unpack_get_all_faction_gift_exreward (ByteArray &bytes );
	int send_get_all_faction_gift_exreward ();
	/*返回礼物列表*/
	int unpack_show_faction_gift_page (ByteArray &bytes );
	int send_show_faction_gift_page ();
	/*返回礼物信息*/
	int unpack_show_faction_gift_info (ByteArray &bytes );
	int send_show_faction_gift_info ();
	/*返回女王未感谢礼物*/
	int unpack_show_faction_gift_unthank_page (ByteArray &bytes );
	int send_show_faction_gift_unthank_page ();
	/*返回女王历史记录*/
	int unpack_show_faction_gift_history_page (ByteArray &bytes );
	int send_show_faction_gift_history_page ();
	/*请求家族魅力排行*/
	int unpack_get_faction_gift_rank_page (ByteArray &bytes ,uint32 &page);
	int send_get_faction_gift_rank_page (uint32 page);
	/*返回家族魅力排行*/
	int unpack_show_faction_gift_rank_result_list (ByteArray &bytes , vector< faction_gift_rank_info > &list, faction_gift_rank_info **info,uint32 &page);
	int send_show_faction_gift_rank_result_list (const vector< faction_gift_rank_info > &list ,faction_gift_rank_info *info,uint32 page);
	/*返回家族魅力排行变化*/
	int unpack_show_faction_gift_rank_change (ByteArray &bytes ,uint32 &old_rank,uint32 &new_rank, faction_gift_rank_info **info);
	int send_show_faction_gift_rank_change (uint32 old_rank,uint32 new_rank,faction_gift_rank_info *info);
	/*返回本家族魅力排行*/
	int unpack_show_faction_gift_rank_info (ByteArray &bytes , faction_gift_rank_info **info);
	int send_show_faction_gift_rank_info (faction_gift_rank_info *info);
	/*神兵强化*/
	int unpack_divine_forge (ByteArray &bytes ,uint32 &id,uint32 &count);
	int send_divine_forge (uint32 id,uint32 count);
	/*神兵升阶*/
	int unpack_divine_advance (ByteArray &bytes ,uint32 &id);
	int send_divine_advance (uint32 id);
	/*神兵铸魂*/
	int unpack_divine_spirit (ByteArray &bytes ,uint32 &id,uint32 &protect,uint32 &improve);
	int send_divine_spirit (uint32 id,uint32 protect,uint32 improve);
	/*查询全民boss信息*/
	int unpack_query_mass_boss_info (ByteArray &bytes ,uint8 &id);
	int send_query_mass_boss_info (uint8 id);
	/*全民boss信息结果*/
	int unpack_mass_boss_info_ret (ByteArray &bytes ,uint32 &count,uint8 &percent);
	int send_mass_boss_info_ret (uint32 count,uint8 percent);
	/*查询全民boss排行榜*/
	int unpack_query_mass_boss_rank (ByteArray &bytes ,uint8 &id);
	int send_query_mass_boss_rank (uint8 id);
	/*全民boss排行结果*/
	int unpack_mass_boss_rank_result (ByteArray &bytes , vector< mass_boss_rank_info > &info);
	int send_mass_boss_rank_result (const vector< mass_boss_rank_info > &info );
	/*挑战全民boss*/
	int unpack_try_mass_boss (ByteArray &bytes ,uint8 &id);
	int send_try_mass_boss (uint8 id);
	/*购买挑战全民boss次数*/
	int unpack_buy_mass_boss_times (ByteArray &bytes ,uint8 &cnt);
	int send_buy_mass_boss_times (uint8 cnt);
	/*组队副本跨服匹配*/
	int unpack_group_instance_match (ByteArray &bytes ,uint8 &indx);
	int send_group_instance_match (uint8 indx);
	/*组队副本跨服次数购买*/
	int unpack_buy_group_instance_times (ByteArray &bytes ,uint8 &count);
	int send_buy_group_instance_times (uint8 count);
	/*法宝激活*/
	int unpack_talisman_active (ByteArray &bytes ,uint32 &id);
	int send_talisman_active (uint32 id);
	/*法宝注灵*/
	int unpack_talisman_lvup (ByteArray &bytes ,uint32 &id);
	int send_talisman_lvup (uint32 id);
	/*神羽激活*/
	int unpack_wings_active (ByteArray &bytes );
	int send_wings_active ();
	/*神羽祝福*/
	int unpack_wings_bless (ByteArray &bytes );
	int send_wings_bless ();
	/*神羽升阶*/
	int unpack_wings_rankup (ByteArray &bytes );
	int send_wings_rankup ();
	/*神羽强化*/
	int unpack_wings_strength (ByteArray &bytes );
	int send_wings_strength ();
	/*经脉修炼*/
	int unpack_meridian_practise (ByteArray &bytes );
	int send_meridian_practise ();
	/*加经脉修炼经验值*/
	int unpack_add_meridian_exp (ByteArray &bytes ,uint8 &id);
	int send_add_meridian_exp (uint8 id);
	/*提升坐骑等级*/
	int unpack_raise_mount_level_base (ByteArray &bytes );
	int send_raise_mount_level_base ();
	/*解锁坐骑*/
	int unpack_active_mount (ByteArray &bytes );
	int send_active_mount ();
	/*家族领主挑战输出排行*/
	int unpack_show_faction_bossdefense_damage_list (ByteArray &bytes , vector< mass_boss_rank_info > &list);
	int send_show_faction_bossdefense_damage_list (const vector< mass_boss_rank_info > &list );
	/*家族无尽远征扫荡结果*/
	int unpack_show_faction_tower_sweep_list (ByteArray &bytes , vector< item_reward_info > &list);
	int send_show_faction_tower_sweep_list (const vector< item_reward_info > &list );
	/*副本结果*/
	int unpack_send_instance_result (ByteArray &bytes ,uint8 &state,uint8 &cd, vector< item_reward_info > &list,uint8 &type,string &data);
	int send_send_instance_result (uint8 state,uint8 cd,const vector< item_reward_info > &list ,uint8 type,char const*data);
	/*匹配单人pvp*/
	int unpack_match_single_pvp (ByteArray &bytes );
	int send_match_single_pvp ();
	/*购买单人pvp次数*/
	int unpack_buy_match_single_pvp_times (ByteArray &bytes ,uint8 &cnt);
	int send_buy_match_single_pvp_times (uint8 cnt);
	/*领取单人pvp额外奖励*/
	int unpack_pick_match_single_pvp_extra_reward (ByteArray &bytes ,uint8 &id);
	int send_pick_match_single_pvp_extra_reward (uint8 id);
	/*装备养成操作*/
	int unpack_equipdevelop_operate (ByteArray &bytes ,uint8 &opt_type,uint16 &reserve_int1,uint16 &reserve_int2,string &reserve_str1,string &reserve_str2);
	int send_equipdevelop_operate (uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2);
	/*激活外观*/
	int unpack_active_appearance (ByteArray &bytes ,uint16 &id);
	int send_active_appearance (uint16 id);
	/*装备外观*/
	int unpack_equip_appearance (ByteArray &bytes ,uint16 &id);
	int send_equip_appearance (uint16 id);
	/*取消装备外观*/
	int unpack_cancel_equip_appearance (ByteArray &bytes ,uint8 &type);
	int send_cancel_equip_appearance (uint8 type);
	/*改名*/
	int unpack_rename (ByteArray &bytes ,string &name);
	int send_rename (char const*name);
	/*道具解锁称号*/
	int unpack_unlock_title (ByteArray &bytes ,uint8 &indx);
	int send_unlock_title (uint8 indx);
	/*购买复仇次数*/
	int unpack_social_buy_revenge_times (ByteArray &bytes ,uint8 &count);
	int send_social_buy_revenge_times (uint8 count);
	/*请求进入世界冒险副本*/
	int unpack_enter_risk_instance (ByteArray &bytes );
	int send_enter_risk_instance ();
	/*删除仇人*/
	int unpack_social_remove_enemy (ByteArray &bytes ,string &guid);
	int send_social_remove_enemy (char const*guid);
	/*查看玩家详情*/
	int unpack_get_player_overview (ByteArray &bytes ,string &guid);
	int send_get_player_overview (char const*guid);
	/*返回玩家详情*/
	int unpack_show_player_overview (ByteArray &bytes ,string &guid,string &name,uint32 &force,uint32 &vip,uint32 &title,uint32 &gender,uint32 &coat,uint32 &weapon);
	int send_show_player_overview (char const*guid,char const*name,uint32 force,uint32 vip,uint32 title,uint32 gender,uint32 coat,uint32 weapon);
	/*邀请加入帮派*/
	int unpack_send_faction_invite (ByteArray &bytes ,string &guid);
	int send_send_faction_invite (char const*guid);
	/*显示邀请*/
	int unpack_show_faction_invite (ByteArray &bytes ,string &guid,string &name,string &faction_guid,string &faction_name);
	int send_show_faction_invite (char const*guid,char const*name,char const*faction_guid,char const*faction_name);
	/*购买vip礼包*/
	int unpack_buy_vipgift (ByteArray &bytes ,uint32 &id);
	int send_buy_vipgift (uint32 id);
	/*购买活动每日礼包*/
	int unpack_activity_opt_buy_dailygift (ByteArray &bytes ,uint32 &act_id,uint32 &index);
	int send_activity_opt_buy_dailygift (uint32 act_id,uint32 index);
	/*抽奖*/
	int unpack_draw_lottery (ByteArray &bytes ,uint32 &actId,uint8 &type);
	int send_draw_lottery (uint32 actId,uint8 type);
	/*获取活动开服排行进度奖励*/
	int unpack_activity_opt_get_rank_process_reward (ByteArray &bytes ,uint32 &act_id,uint32 &index);
	int send_activity_opt_get_rank_process_reward (uint32 act_id,uint32 index);
	/*获取活动开服排行榜*/
	int unpack_activity_opt_get_rank_list (ByteArray &bytes ,uint32 &act_id);
	int send_activity_opt_get_rank_list (uint32 act_id);
	/*返回活动开服排行榜*/
	int unpack_activity_opt_show_rank_list (ByteArray &bytes ,uint32 &act_id, vector< act_rank_info > &list);
	int send_activity_opt_show_rank_list (uint32 act_id,const vector< act_rank_info > &list );
	/*购买活动限定礼包*/
	int unpack_activity_opt_buy_limitgift (ByteArray &bytes ,uint32 &act_id,uint32 &index);
	int send_activity_opt_buy_limitgift (uint32 act_id,uint32 index);
	/*领取累计充值奖励*/
	int unpack_welfare_get_recharge_reward (ByteArray &bytes ,uint8 &id);
	int send_welfare_get_recharge_reward (uint8 id);
	/*领取累计消费奖励*/
	int unpack_welfare_get_consume_reward (ByteArray &bytes ,uint8 &id);
	int send_welfare_get_consume_reward (uint8 id);
	/*领取七日大礼奖励*/
	int unpack_welfare_get_sevenday_reward (ByteArray &bytes ,uint8 &id);
	int send_welfare_get_sevenday_reward (uint8 id);
	/*服务器开服时间*/
	int unpack_send_server_open_time (ByteArray &bytes ,uint32 &open_time);
	int send_send_server_open_time (uint32 open_time);
	/*请求世界冒险排行榜*/
	int unpack_risk_get_rank (ByteArray &bytes );
	int send_risk_get_rank ();
	/*世界冒险排行榜信息 */
	int unpack_risk_get_rank_result (ByteArray &bytes , vector< act_rank_info > &list);
	int send_risk_get_rank_result (const vector< act_rank_info > &list );
	/*设置朝向*/
	int unpack_set_orient (ByteArray &bytes ,uint16 &angle);
	int send_set_orient (uint16 angle);
};

#endif
