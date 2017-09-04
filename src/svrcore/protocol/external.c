
#include "protocol/external.h"
#include "svrcore.h"
#include "shared/util.h"

/*int   pack_update_object(packet**dst,list_t *block_list)
{
	list_t *pos = NULL;
	uint16 block_num=0;

	ASSERT(dst);
	*dst = external_protocol_new_packet(SMSG_UPDATE_OBJECT);
	packet_write(*dst,(char*)&block_num,sizeof(block_num));
	list_for_each(pos,block_list){
		packet *pkt = list_entry(pos,packet,s);
		packet_write(*dst,pkt->content,pkt->wpos);
		(*((uint16*)(*dst)->content))++;
	}	
	update_packet_len(*dst);
	return 0;
}*/

char* OpcodeName[NUM_MSG_TYPES+1] = {
    "MSG_NULL_ACTION",
    "MSG_PING_PONG",
    "CMSG_FORCED_INTO",
    "CMSG_GET_SESSION",
    "MSG_ROUTE_TRACE",
    "CMSG_WRITE_CLIENT_LOG",
    "SMSG_OPERATION_FAILED",
    "MSG_SYNC_MSTIME",
    "SMSG_UD_OBJECT",
    "CMSG_UD_CONTROL",
    "SMSG_UD_CONTROL_RESULT",
    "SMSG_GRID_UD_OBJECT",
    "SMSG_GRID_UD_OBJECT_2",
    "SMSG_LOGIN_QUEUE_INDEX",
    "SMSG_KICKING_TYPE",
    "CMSG_GET_CHARS_LIST",
    "SMSG_CHARS_LIST",
    "CMSG_CHECK_NAME",
    "SMSG_CHECK_NAME_RESULT",
    "CMSG_CHAR_CREATE",
    "SMSG_CHAR_CREATE_RESULT",
    "CMSG_DELETE_CHAR",
    "SMSG_DELETE_CHAR_RESULT",
    "CMSG_PLAYER_LOGIN",
    "CMSG_PLAYER_LOGOUT",
    "CMSG_REGULARISE_ACCOUNT",
    "CMSG_CHAR_REMOTESTORE",
    "CMSG_CHAR_REMOTESTORE_STR",
    "CMSG_TELEPORT",
    "MSG_MOVE_STOP",
    "MSG_UNIT_MOVE",
    "CMSG_USE_GAMEOBJECT",
    "CMSG_BAG_EXCHANGE_POS",
    "CMSG_BAG_DESTROY",
    "CMSG_BAG_ITEM_SPLIT",
    "CMSG_BAG_ITEM_USER",
    "SMSG_BAG_ITEM_COOLDOWN",
    "SMSG_GRID_UNIT_MOVE",
    "SMSG_GRID_UNIT_MOVE_2",
    "CMSG_EXCHANGE_ITEM",
    "CMSG_BAG_EXTENSION",
    "CMSG_NPC_GET_GOODS_LIST",
    "SMSG_NPC_GOODS_LIST",
    "CMSG_STORE_BUY",
    "CMSG_NPC_SELL",
    "CMSG_NPC_REPURCHASE",
    "CMSG_AVATAR_FASHION_ENABLE",
    "CMSG_QUESTHELP_TALK_OPTION",
    "CMSG_TAXI_HELLO",
    "SMSG_TAXI_STATIONS_LIST",
    "CMSG_TAXI_SELECT_STATION",
    "CMSG_GOSSIP_SELECT_OPTION",
    "CMSG_GOSSIP_HELLO",
    "SMSG_GOSSIP_MESSAGE",
    "CMSG_QUESTGIVER_STATUS_QUERY",
    "SMSG_QUESTGIVER_STATUS",
    "MSG_QUERY_QUEST_STATUS",
    "CMSG_QUESTHELP_GET_CANACCEPT_LIST",
    "SMSG_QUESTHELP_CANACCEPT_LIST",
    "SMSG_QUESTUPDATE_FAILD",
    "SMSG_QUESTUPDATE_COMPLETE",
    "CMSG_QUESTLOG_REMOVE_QUEST",
    "CMSG_QUESTGIVER_COMPLETE_QUEST",
    "SMSG_QUESTHELP_NEXT",
    "CMSG_QUESTHELP_COMPLETE",
    "SMSG_QUESTUPDATE_ACCEPT",
    "CMSG_QUESTHELP_UPDATE_STATUS",
    "SMSG_QUESTGETTER_COMPLETE",
    "CMSG_QUESTGIVER_ACCEPT_QUEST",
    "CMSG_QUESTUPDATE_USE_ITEM",
    "CMSG_QUESTHELP_QUERY_BOOK",
    "SMSG_QUESTHELP_BOOK_QUEST",
    "SMSG_USE_GAMEOBJECT_ACTION",
    "CMSG_SET_ATTACK_MODE",
    "MSG_SELECT_TARGET",
    "SMSG_COMBAT_STATE_UPDATE",
    "SMSG_EXP_UPDATE",
    "MSG_SPELL_START",
    "MSG_SPELL_STOP",
    "MSG_JUMP",
    "CMSG_RESURRECTION",
    "MSG_TRADE_REQUEST",
    "MSG_TRADE_REPLY",
    "SMSG_TRADE_START",
    "MSG_TRADE_DECIDE_ITEMS",
    "SMSG_TRADE_FINISH",
    "MSG_TRADE_CANCEL",
    "MSG_TRADE_READY",
    "SMSG_CHAT_UNIT_TALK",
    "CMSG_CHAT_NEAR",
    "CMSG_CHAT_WHISPER",
    "MSG_CHAT_FACTION",
    "MSG_CHAT_WORLD",
    "MSG_CHAT_HORN",
    "MSG_CHAT_NOTICE",
    "CMSG_QUERY_PLAYER_INFO",
    "SMSG_QUERY_RESULT_UPDATE_OBJECT",
    "CMSG_RECEIVE_GIFT_PACKS",
    "SMSG_MAP_UPDATE_OBJECT",
    "SMSG_FIGHTING_INFO_UPDATE_OBJECT",
    "SMSG_FIGHTING_INFO_UPDATE_OBJECT_2",
    "CMSG_INSTANCE_ENTER",
    "CMSG_INSTANCE_NEXT_STATE",
    "CMSG_INSTANCE_EXIT",
    "CMSG_LIMIT_ACTIVITY_RECEIVE",
    "SMSG_KILL_MAN",
    "SMSG_PLAYER_UPGRADE",
    "CMSG_WAREHOUSE_SAVE_MONEY",
    "CMSG_WAREHOUSE_TAKE_MONEY",
    "CMSG_USE_GOLD_OPT",
    "CMSG_USE_SILVER_OPT",
    "SMSG_GM_RIGHTFLOAT",
    "SMSG_DEL_GM_RIGHTFLOAT",
    "MSG_SYNC_MSTIME_APP",
    "CMSG_OPEN_WINDOW",
    "CMSG_PLAYER_GAG",
    "CMSG_PLAYER_KICKING",
    "SMSG_MERGE_SERVER_MSG",
    "CMSG_RANK_LIST_QUERY",
    "SMSG_RANK_LIST_QUERY_RESULT",
    "CMSG_CLIENT_UPDATE_SCENED",
    "SMSG_NUM_LUA",
    "CMSG_LOOT_SELECT",
    "CMSG_GOBACK_TO_GAME_SERVER",
    "CMSG_WORLD_WAR_CS_PLAYER_INFO",
    "SMSG_JOIN_OR_LEAVE_SERVER",
    "MSG_WORLD_WAR_SC_PLAYER_INFO",
    "MSG_CLIENTSUBSCRIPTION",
    "SMSG_LUA_SCRIPT",
    "CMSG_CHAR_UPDATE_INFO",
    "SMSG_NOTICE_WATCHER_MAP_INFO",
    "CMSG_MODIFY_WATCH",
    "CMSG_KUAFU_CHUANSONG",
    "CMSG_SHOW_SUIT",
    "CMSG_SHOW_POSITION",
    "CMSG_GOLD_RESPAWN",
    "SMSG_FIELD_DEATH_COOLDOWN",
    "CMSG_MALL_BUY",
    "CMSG_STRENGTH",
    "SMSG_STRENGTH_SUCCESS",
    "CMSG_FORCEINTO",
    "CMSG_CREATE_FACTION",
    "CMSG_FACTION_UPGRADE",
    "CMSG_FACTION_JOIN",
    "CMSG_RAISE_BASE_SPELL",
    "CMSG_UPGRADE_ANGER_SPELL",
    "CMSG_RAISE_MOUNT",
    "CMSG_UPGRADE_MOUNT",
    "CMSG_UPGRADE_MOUNT_ONE_STEP",
    "CMSG_ILLUSION_MOUNT_ACTIVE",
    "CMSG_ILLUSION_MOUNT",
    "CMSG_RIDE_MOUNT",
    "SMSG_GRID_UNIT_JUMP",
    "CMSG_GEM",
    "CMSG_CHANGE_BATTLE_MODE",
    "SMSG_PEACE_MODE_CD",
    "CMSG_DIVINE_ACTIVE",
    "CMSG_DIVINE_UPLEV",
    "CMSG_DIVINE_SWITCH",
    "CMSG_JUMP_START",
    "CMSG_ENTER_VIP_INSTANCE",
    "CMSG_SWEEP_VIP_INSTANCE",
    "CMSG_HANG_UP",
    "CMSG_HANG_UP_SETTING",
    "CMSG_ENTER_TRIAL_INSTANCE",
    "CMSG_SWEEP_TRIAL_INSTANCE",
    "CMSG_RESET_TRIAL_INSTANCE",
    "SMSG_SWEEP_INSTANCE_REWARD",
    "CMSG_REENTER_INSTANCE",
    "SMSG_MERRY_GO_ROUND",
    "CMSG_SOCIAL_ADD_FRIEND",
    "CMSG_SOCIAL_SUREADD_FRIEND",
    "CMSG_SOCIAL_GIFT_FRIEND",
    "CMSG_SOCIAL_RECOMMEND_FRIEND",
    "SMSG_SOCIAL_GET_RECOMMEND_FRIEND",
    "CMSG_SOCIAL_REVENGE_ENEMY",
    "CMSG_SOCIAL_DEL_FRIEND",
    "CMSG_TELEPORT_MAIN_CITY",
    "CMSG_CHAT_BY_CHANNEL",
    "SMSG_SEND_CHAT",
    "CMSG_SOCIAL_CLEAR_APPLY",
    "CMSG_MSG_DECLINE",
    "SMSG_FACTION_GET_LIST_RESULT",
    "CMSG_FACTION_GETLIST",
    "CMSG_FACTION_MANAGER",
    "CMSG_FACTION_MEMBER_OPERATE",
    "CMSG_FACTION_FAST_JOIN",
    "CMSG_SOCIAL_ADD_FRIEND_BYNAME",
    "CMSG_READ_MAIL",
    "CMSG_PICK_MAIL",
    "CMSG_REMOVE_MAIL",
    "CMSG_PICK_MAIL_ONE_STEP",
    "CMSG_REMOVE_MAIL_ONE_STEP",
    "CMSG_BLOCK_CHAT",
    "CMSG_CANCEL_BLOCK_CHAT",
    "CMSG_USE_BROADCAST_GAMEOBJECT",
    "CMSG_WORLD_BOSS_ENROLL",
    "CMSG_WORLD_BOSS_FIGHT",
    "CMSG_CHANGE_LINE",
    "CMSG_ROLL_WORLD_BOSS_TREASURE",
    "SMSG_ROLL_RESULT",
    "SMSG_BOSS_RANK",
    "CMSG_RANK_ADD_LIKE",
    "SMSG_RANK_ADD_LIKE_RESULT",
    "CMSG_RES_INSTANCE_ENTER",
    "CMSG_RES_INSTANCE_SWEEP",
    "CMSG_SHOW_MAP_LINE",
    "SMSG_SEND_MAP_LINE",
    "SMSG_ITEM_NOTICE",
    "CMSG_TELEPORT_MAP",
    "CMSG_TELEPORT_FIELD_BOSS",
    "CMSG_GET_ACTIVITY_REWARD",
    "CMSG_GET_ACHIEVE_REWARD",
    "CMSG_GET_ACHIEVE_ALL_REWARD",
    "CMSG_SET_TITLE",
    "CMSG_INIT_TITLE",
    "CMSG_WELFARE_SHOUCHONG_REWARD",
    "CMSG_WELFARE_CHECKIN",
    "CMSG_WELFARE_CHECKIN_ALL",
    "CMSG_WELFARE_CHECKIN_GETBACK",
    "CMSG_WELFARE_LEVEL",
    "CMSG_WELFARE_ACTIVE_GETBACK",
    "CMSG_PICK_QUEST_REWARD",
    "CMSG_TALK_WITH_NPC",
    "CMSG_USE_VIRTUAL_ITEM",
    "CMSG_PICK_QUEST_CHAPTER_REWARD",
    "CMSG_KUAFU_3V3_MATCH",
    "SMSG_KUAFU_MATCH_START",
    "CMSG_KUAFU_3V3_BUYTIMES",
    "CMSG_KUAFU_3V3_DAYREWARD",
    "CMSG_KUAFU_3V3_GETRANLIST",
    "SMSG_KUAFU_3V3_RANLIST",
    "CMSG_WELFARE_GETALLLIST_GETBACK",
    "SMSG_WELFARE_REWARDLIST_GETBACK",
    "CMSG_WELFARE_GETALL_GETBACK",
    "CMSG_KUAFU_3V3_GETMYRANK",
    "SMSG_KUAFU_3V3_MYRANK",
    "SMSG_KUAFU_3V3_KILL_DETAIL",
    "SMSG_KUAFU_3V3_WAIT_INFO",
    "MSG_KUAFU_3V3_CANCEL_MATCH",
    "CMSG_KUAFU_3V3_MATCH_OPER",
    "SMSG_KUAFU_3V3_DECLINE_MATCH",
    "CMSG_KUAFU_XIANFU_MATCH",
    "SMSG_KUAFU_MATCH_WAIT",
    "SMSG_KUAFU_XIANFU_MINIMAP_INFO",
    "CMSG_BUY_XIANFU_ITEM",
    "CMSG_XIANFU_RANDOM_RESPAWN",
    "CMSG_DOUJIANTAI_FIGHT",
    "CMSG_DOUJIANTAI_BUYTIME",
    "CMSG_DOUJIANTAI_CLEARCD",
    "CMSG_DOUJIANTAI_FIRST_REWARD",
    "MSG_DOUJIANTAI_GET_ENEMYS_INFO",
    "CMSG_DOUJIANTAI_GET_RANK",
    "CMSG_DOUJIANTAI_REFRESH_ENEMYS",
    "MSG_DOUJIANTAI_TOP3",
    "MSG_USE_JUMP_POINT",
    "CMSG_BAG_ITEM_SELL",
    "CMSG_BAG_ITEM_SORT",
    "CMSG_SUBMIT_QUEST_DAILY2",
    "SMSG_ATTRIBUTE_CHANGED",
    "SMSG_BAG_FIND_EQUIP_BETTER",
    "SMSG_MODULE_ACTIVE",
    "CMSG_PICK_DAILY2_QUEST_REWARD",
    "CMSG_FINISH_NOW_GUIDE",
    "CMSG_GET_CULTIVATION_INFO",
    "SMSG_UPDATE_CULTIVATION_INFO",
    "CMSG_GET_CULTIVATION_RIVALS_INFO",
    "SMSG_UPDATE_CULTIVATION_RIVALS_INFO_LIST",
    "CMSG_GET_CULTIVATION_REWARD",
    "CMSG_REFRESH_CULTIVATION_RIVALS",
    "CMSG_PLUNDER_CULTIVATION_RIVAL",
    "CMSG_REVENGE_CULTIVATION_RIVAL",
    "CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT",
    "SMSG_SHOW_CULTIVATION_RESULT_LIST",
    "CMSG_GET_LOGIN_ACTIVITY_REWARD",
    "SMSG_CAST_SPELL_START",
    "CMSG_FINISH_OPTIONAL_GUIDE_STEP",
    "CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED",
    "SMSG_SHOW_UNIT_ATTRIBUTE",
    "CMSG_BACK_TO_FAMITY",
    "SMSG_FACTION_BOSS_SEND_RESULT",
    "CMSG_CHALLANGE_BOSS",
    "CMSG_PICK_OFFLINE_REWARD",
    "SMSG_OFFLINE_REWARD_RESULT",
    "CMSG_SMELTING_EQUIP",
    "CMSG_STOREHOUSE_HAND_IN",
    "CMSG_STOREHOUSE_EXCHANGE",
    "CMSG_STOREHOUSE_DESTROY",
    "CMSG_SEND_FACTION_GIFT",
    "CMSG_GET_FACTION_GIFT_EXREWARD",
    "CMSG_GET_ALL_FACTION_GIFT_EXREWARD",
    "SMSG_SHOW_FACTION_GIFT_PAGE",
    "SMSG_SHOW_FACTION_GIFT_INFO",
    "SMSG_SHOW_FACTION_GIFT_UNTHANK_PAGE",
    "SMSG_SHOW_FACTION_GIFT_HISTORY_PAGE",
    "CMSG_GET_FACTION_GIFT_RANK_PAGE",
    "SMSG_SHOW_FACTION_GIFT_RANK_RESULT_LIST",
    "SMSG_SHOW_FACTION_GIFT_RANK_CHANGE",
    "SMSG_SHOW_FACTION_GIFT_RANK_INFO",
    "CMSG_DIVINE_FORGE",
    "CMSG_DIVINE_ADVANCE",
    "CMSG_DIVINE_SPIRIT",
    "CMSG_QUERY_MASS_BOSS_INFO",
    "SMSG_MASS_BOSS_INFO_RET",
    "CMSG_QUERY_MASS_BOSS_RANK",
    "SMSG_MASS_BOSS_RANK_RESULT",
    "CMSG_TRY_MASS_BOSS",
    "CMSG_BUY_MASS_BOSS_TIMES",
    "CMSG_GROUP_INSTANCE_MATCH",
    "CMSG_BUY_GROUP_INSTANCE_TIMES",
    "CMSG_TALISMAN_ACTIVE",
    "CMSG_TALISMAN_LVUP",
    "CMSG_WINGS_ACTIVE",
    "CMSG_WINGS_BLESS",
    "CMSG_WINGS_RANKUP",
    "CMSG_WINGS_STRENGTH",
    "CMSG_MERIDIAN_PRACTISE",
    "CMSG_ADD_MERIDIAN_EXP",
    "CMSG_RAISE_MOUNT_LEVEL_BASE",
    "CMSG_ACTIVE_MOUNT",
    "SMSG_SHOW_FACTION_BOSSDEFENSE_DAMAGE_LIST",
    "SMSG_SHOW_FACTION_TOWER_SWEEP_LIST",
    "SMSG_SEND_INSTANCE_RESULT",
    "CMSG_MATCH_SINGLE_PVP",
    "CMSG_BUY_MATCH_SINGLE_PVP_TIMES",
    "CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD",
    "CMSG_EQUIPDEVELOP_OPERATE",
    "CMSG_ACTIVE_APPEARANCE",
    "CMSG_EQUIP_APPEARANCE",
    "CMSG_CANCEL_EQUIP_APPEARANCE",
    "CMSG_RENAME",
    "CMSG_UNLOCK_TITLE",
    "CMSG_SOCIAL_BUY_REVENGE_TIMES",
    "CMSG_ENTER_RISK_INSTANCE",
    "CMSG_SOCIAL_REMOVE_ENEMY",
    "CMSG_GET_PLAYER_OVERVIEW",
    "SMSG_SHOW_PLAYER_OVERVIEW",
    "CMSG_SEND_FACTION_INVITE",
    "SMSG_SHOW_FACTION_INVITE",
    "CMSG_BUY_VIPGIFT",
    "CMSG_ACTIVITY_OPT_BUY_DAILYGIFT",
    "CMSG_DRAW_LOTTERY",
    "CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD",
    "CMSG_ACTIVITY_OPT_GET_RANK_LIST",
    "SMSG_ACTIVITY_OPT_SHOW_RANK_LIST",
    "CMSG_ACTIVITY_OPT_BUY_LIMITGIFT",
    "CMSG_WELFARE_GET_RECHARGE_REWARD",
    "CMSG_WELFARE_GET_CONSUME_REWARD",
    "CMSG_WELFARE_GET_SEVENDAY_REWARD",
    "SMSG_SEND_SERVER_OPEN_TIME",
    "CMSG_RISK_GET_RANK",
    "SMSG_RISK_GET_RANK_RESULT",
    "CMSG_SET_ORIENT",
"NUM_MSG_TYPES"
};

const char *GetOpcodeName(uint16 opt)
{
	if(opt>=NUM_MSG_TYPES)
		return "";
	return OpcodeName[opt];
}




/******************************************************************/
/*?????Э??*/
/******************************************************************/
/*无效动作*/
int   pack_null_action (packet**dst )
{
	*dst = external_protocol_new_packet(MSG_NULL_ACTION);
	ASSERT((*dst)->head->optcode == MSG_NULL_ACTION);	
		
	update_packet_len(*dst);
	return 0;	
}
/*测试连接状态*/
int   pack_ping_pong (packet**dst )
{
	*dst = external_protocol_new_packet(MSG_PING_PONG);
	ASSERT((*dst)->head->optcode == MSG_PING_PONG);	
		
	update_packet_len(*dst);
	return 0;	
}
/*踢掉在线的准备强制登陆*/
int   pack_forced_into (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_FORCED_INTO);
	ASSERT((*dst)->head->optcode == CMSG_FORCED_INTO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*获得Session对象*/
int   pack_get_session (packet**dst ,char const*sessionkey,char const*account,char const*version)
{
	*dst = external_protocol_new_packet(CMSG_GET_SESSION);
	ASSERT((*dst)->head->optcode == CMSG_GET_SESSION);	
	packet_write_str(*dst,sessionkey);		
	packet_write_str(*dst,account);		
	packet_write_str(*dst,version);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_get_session (packet *src ,char **sessionkey,char **account,char **version)
{	
	int ret=0;
	ret = packet_read_str(src,sessionkey);
	if(ret) return -1;
	ret = packet_read_str(src,account);
	if(ret) return -1;
	ret = packet_read_str(src,version);
	if(ret) return -1;
	return ret;
}
/*网关服数据包路由测试*/
int   pack_route_trace (packet**dst ,char const*val)
{
	*dst = external_protocol_new_packet(MSG_ROUTE_TRACE);
	ASSERT((*dst)->head->optcode == MSG_ROUTE_TRACE);	
	packet_write_str(*dst,val);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_route_trace (packet *src ,char **val)
{	
	int ret=0;
	ret = packet_read_str(src,val);
	if(ret) return -1;
	return ret;
}
/*记录客户端日志*/
int   pack_write_client_log (packet**dst ,uint32 type,char const*uid,char const*guid,char const*log)
{
	*dst = external_protocol_new_packet(CMSG_WRITE_CLIENT_LOG);
	ASSERT((*dst)->head->optcode == CMSG_WRITE_CLIENT_LOG);	
	packet_write(*dst,(char *)&type,sizeof(uint32));
	packet_write_str(*dst,uid);		
	packet_write_str(*dst,guid);		
	packet_write_str(*dst,log);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_write_client_log (packet *src ,uint32 *type,char **uid,char **guid,char **log)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,uid);
	if(ret) return -1;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read_str(src,log);
	if(ret) return -1;
	return ret;
}
/*操作失败*/
int   pack_operation_failed (packet**dst ,uint16 type,uint16 reason,char const*data)
{
	*dst = external_protocol_new_packet(SMSG_OPERATION_FAILED);
	ASSERT((*dst)->head->optcode == SMSG_OPERATION_FAILED);	
	packet_write(*dst,(char *)&type,sizeof(uint16));
	packet_write(*dst,(char *)&reason,sizeof(uint16));
	packet_write_str(*dst,data);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_operation_failed (packet *src ,uint16 *type,uint16 *reason,char **data)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reason),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read_str(src,data);
	if(ret) return -1;
	return ret;
}
/*同步时间*/
int   pack_sync_mstime (packet**dst ,uint32 mstime_now,uint32 time_now,uint32 open_time)
{
	*dst = external_protocol_new_packet(MSG_SYNC_MSTIME);
	ASSERT((*dst)->head->optcode == MSG_SYNC_MSTIME);	
	packet_write(*dst,(char *)&mstime_now,sizeof(uint32));
	packet_write(*dst,(char *)&time_now,sizeof(uint32));
	packet_write(*dst,(char *)&open_time,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_sync_mstime (packet *src ,uint32 *mstime_now,uint32 *time_now,uint32 *open_time)
{	
	int ret=0;
	ret = packet_read(src,(char*)(mstime_now),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(time_now),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(open_time),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*对象更新*/
int   pack_ud_object (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_UD_OBJECT);
	ASSERT((*dst)->head->optcode == SMSG_UD_OBJECT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*对象更新控制协议*/
int   pack_ud_control (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_UD_CONTROL);
	ASSERT((*dst)->head->optcode == CMSG_UD_CONTROL);	
		
	update_packet_len(*dst);
	return 0;	
}
/*对象更新控制协议结果*/
int   pack_ud_control_result (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_UD_CONTROL_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_UD_CONTROL_RESULT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*GRID的对象更新*/
int   pack_grid_ud_object (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_GRID_UD_OBJECT);
	ASSERT((*dst)->head->optcode == SMSG_GRID_UD_OBJECT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*GRID的对象更新*/
int   pack_grid_ud_object_2 (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_GRID_UD_OBJECT_2);
	ASSERT((*dst)->head->optcode == SMSG_GRID_UD_OBJECT_2);	
		
	update_packet_len(*dst);
	return 0;	
}
/*告诉客户端，目前自己排在登录队列的第几位*/
int   pack_login_queue_index (packet**dst ,uint32 index)
{
	*dst = external_protocol_new_packet(SMSG_LOGIN_QUEUE_INDEX);
	ASSERT((*dst)->head->optcode == SMSG_LOGIN_QUEUE_INDEX);	
	packet_write(*dst,(char *)&index,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_login_queue_index (packet *src ,uint32 *index)
{	
	int ret=0;
	ret = packet_read(src,(char*)(index),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*踢人原因*/
int   pack_kicking_type (packet**dst ,uint32 k_type)
{
	*dst = external_protocol_new_packet(SMSG_KICKING_TYPE);
	ASSERT((*dst)->head->optcode == SMSG_KICKING_TYPE);	
	packet_write(*dst,(char *)&k_type,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kicking_type (packet *src ,uint32 *k_type)
{	
	int ret=0;
	ret = packet_read(src,(char*)(k_type),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*获取角色列表*/
int   pack_get_chars_list (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_GET_CHARS_LIST);
	ASSERT((*dst)->head->optcode == CMSG_GET_CHARS_LIST);	
		
	update_packet_len(*dst);
	return 0;	
}
/*角色列表*/
int   pack_chars_list (packet**dst , char_create_info *list , uint16 len_1,char const*faction_name,char const*queen_name,uint8 icon)
{
	*dst = external_protocol_new_packet(SMSG_CHARS_LIST);
	ASSERT((*dst)->head->optcode == SMSG_CHARS_LIST);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(char_create_info) * len_1);
	packet_write_str(*dst,faction_name);		
	packet_write_str(*dst,queen_name);		
	packet_write(*dst,(char *)&icon,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chars_list (packet *src , char_create_info **list , uint16 *len_1,char **faction_name,char **queen_name,uint8 *icon)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (char_create_info *)(src->content+src->rpos);
	src->rpos += sizeof(char_create_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	ret = packet_read_str(src,faction_name);
	if(ret) return -1;
	ret = packet_read_str(src,queen_name);
	if(ret) return -1;
	ret = packet_read(src,(char*)(icon),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*检查名字是否可以使用*/
int   pack_check_name (packet**dst ,char const*name)
{
	*dst = external_protocol_new_packet(CMSG_CHECK_NAME);
	ASSERT((*dst)->head->optcode == CMSG_CHECK_NAME);	
	packet_write_str(*dst,name);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_check_name (packet *src ,char **name)
{	
	int ret=0;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	return ret;
}
/*检查名字结果*/
int   pack_check_name_result (packet**dst ,uint8 result)
{
	*dst = external_protocol_new_packet(SMSG_CHECK_NAME_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_CHECK_NAME_RESULT);	
	packet_write(*dst,(char *)&result,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_check_name_result (packet *src ,uint8 *result)
{	
	int ret=0;
	ret = packet_read(src,(char*)(result),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*创建角色*/
int   pack_char_create (packet**dst , char_create_info *info)
{
	*dst = external_protocol_new_packet(CMSG_CHAR_CREATE);
	ASSERT((*dst)->head->optcode == CMSG_CHAR_CREATE);	
	packet_write(*dst,(char*)info,sizeof(char_create_info));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_char_create (packet *src , char_create_info **info)
{	
	int ret=0;
	*info = (char_create_info*)(src->content+src->rpos);
	src->rpos += sizeof(char_create_info);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*角色创建结果*/
int   pack_char_create_result (packet**dst ,uint8 result)
{
	*dst = external_protocol_new_packet(SMSG_CHAR_CREATE_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_CHAR_CREATE_RESULT);	
	packet_write(*dst,(char *)&result,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_char_create_result (packet *src ,uint8 *result)
{	
	int ret=0;
	ret = packet_read(src,(char*)(result),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*删除角色*/
int   pack_delete_char (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(CMSG_DELETE_CHAR);
	ASSERT((*dst)->head->optcode == CMSG_DELETE_CHAR);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_delete_char (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*角色删除结果*/
int   pack_delete_char_result (packet**dst ,uint8 result)
{
	*dst = external_protocol_new_packet(SMSG_DELETE_CHAR_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_DELETE_CHAR_RESULT);	
	packet_write(*dst,(char *)&result,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_delete_char_result (packet *src ,uint8 *result)
{	
	int ret=0;
	ret = packet_read(src,(char*)(result),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*玩家登录*/
int   pack_player_login (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_PLAYER_LOGIN);
	ASSERT((*dst)->head->optcode == CMSG_PLAYER_LOGIN);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_player_login (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*玩家退出*/
int   pack_player_logout (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_PLAYER_LOGOUT);
	ASSERT((*dst)->head->optcode == CMSG_PLAYER_LOGOUT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*临时账号转正规*/
int   pack_regularise_account (packet**dst ,char const*uid)
{
	*dst = external_protocol_new_packet(CMSG_REGULARISE_ACCOUNT);
	ASSERT((*dst)->head->optcode == CMSG_REGULARISE_ACCOUNT);	
	packet_write_str(*dst,uid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_regularise_account (packet *src ,char **uid)
{	
	int ret=0;
	ret = packet_read_str(src,uid);
	if(ret) return -1;
	return ret;
}
/*角色配置信息*/
int   pack_char_remotestore (packet**dst ,uint32 key,uint32 value)
{
	*dst = external_protocol_new_packet(CMSG_CHAR_REMOTESTORE);
	ASSERT((*dst)->head->optcode == CMSG_CHAR_REMOTESTORE);	
	packet_write(*dst,(char *)&key,sizeof(uint32));
	packet_write(*dst,(char *)&value,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_char_remotestore (packet *src ,uint32 *key,uint32 *value)
{	
	int ret=0;
	ret = packet_read(src,(char*)(key),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(value),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*角色配置信息*/
int   pack_char_remotestore_str (packet**dst ,uint32 key,char const*value)
{
	*dst = external_protocol_new_packet(CMSG_CHAR_REMOTESTORE_STR);
	ASSERT((*dst)->head->optcode == CMSG_CHAR_REMOTESTORE_STR);	
	packet_write(*dst,(char *)&key,sizeof(uint32));
	packet_write_str(*dst,value);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_char_remotestore_str (packet *src ,uint32 *key,char **value)
{	
	int ret=0;
	ret = packet_read(src,(char*)(key),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,value);
	if(ret) return -1;
	return ret;
}
/*传送，如果是C->S，mapid变量请填成传送点ID*/
int   pack_teleport (packet**dst ,uint32 intGuid)
{
	*dst = external_protocol_new_packet(CMSG_TELEPORT);
	ASSERT((*dst)->head->optcode == CMSG_TELEPORT);	
	packet_write(*dst,(char *)&intGuid,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_teleport (packet *src ,uint32 *intGuid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(intGuid),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*停止移动*/
int   pack_move_stop (packet**dst ,uint32 guid,uint16 pos_x,uint16 pos_y)
{
	*dst = external_protocol_new_packet(MSG_MOVE_STOP);
	ASSERT((*dst)->head->optcode == MSG_MOVE_STOP);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
	packet_write(*dst,(char *)&pos_x,sizeof(uint16));
	packet_write(*dst,(char *)&pos_y,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_move_stop (packet *src ,uint32 *guid,uint16 *pos_x,uint16 *pos_y)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(pos_x),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(pos_y),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*unit对象移动*/
int   pack_unit_move (packet**dst ,uint32 guid,uint16 pos_x,uint16 pos_y, int8 *path , uint16 len_4)
{
	*dst = external_protocol_new_packet(MSG_UNIT_MOVE);
	ASSERT((*dst)->head->optcode == MSG_UNIT_MOVE);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
	packet_write(*dst,(char *)&pos_x,sizeof(uint16));
	packet_write(*dst,(char *)&pos_y,sizeof(uint16));
	packet_write(*dst,(char *)&len_4, sizeof(uint16));
	packet_write(*dst,(char *)path, sizeof(int8) * len_4);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_unit_move (packet *src ,uint32 *guid,uint16 *pos_x,uint16 *pos_y, int8 **path , uint16 *len_4)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(pos_x),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(pos_y),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)len_4,sizeof(uint16));
	if(ret) return -1;
	*path = (int8 *)(src->content+src->rpos);
	src->rpos += sizeof(int8) * (*len_4);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*使用游戏对象*/
int   pack_use_gameobject (packet**dst ,uint32 target)
{
	*dst = external_protocol_new_packet(CMSG_USE_GAMEOBJECT);
	ASSERT((*dst)->head->optcode == CMSG_USE_GAMEOBJECT);	
	packet_write(*dst,(char *)&target,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_use_gameobject (packet *src ,uint32 *target)
{	
	int ret=0;
	ret = packet_read(src,(char*)(target),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*包裹操作-交换位置*/
int   pack_bag_exchange_pos (packet**dst ,uint32 src_bag,uint32 src_pos,uint32 dst_bag,uint32 dst_pos)
{
	*dst = external_protocol_new_packet(CMSG_BAG_EXCHANGE_POS);
	ASSERT((*dst)->head->optcode == CMSG_BAG_EXCHANGE_POS);	
	packet_write(*dst,(char *)&src_bag,sizeof(uint32));
	packet_write(*dst,(char *)&src_pos,sizeof(uint32));
	packet_write(*dst,(char *)&dst_bag,sizeof(uint32));
	packet_write(*dst,(char *)&dst_pos,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_bag_exchange_pos (packet *src ,uint32 *src_bag,uint32 *src_pos,uint32 *dst_bag,uint32 *dst_pos)
{	
	int ret=0;
	ret = packet_read(src,(char*)(src_bag),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(src_pos),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(dst_bag),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(dst_pos),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*包裹操作-销毁物品*/
int   pack_bag_destroy (packet**dst ,char const*item_guid,uint32 num,uint32 bag_id)
{
	*dst = external_protocol_new_packet(CMSG_BAG_DESTROY);
	ASSERT((*dst)->head->optcode == CMSG_BAG_DESTROY);	
	packet_write_str(*dst,item_guid);		
	packet_write(*dst,(char *)&num,sizeof(uint32));
	packet_write(*dst,(char *)&bag_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_bag_destroy (packet *src ,char **item_guid,uint32 *num,uint32 *bag_id)
{	
	int ret=0;
	ret = packet_read_str(src,item_guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(num),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(bag_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*分割物品*/
int   pack_bag_item_split (packet**dst ,uint8 bag_id,uint16 src_pos,uint32 count,uint16 dst_pos,uint8 dst_bag)
{
	*dst = external_protocol_new_packet(CMSG_BAG_ITEM_SPLIT);
	ASSERT((*dst)->head->optcode == CMSG_BAG_ITEM_SPLIT);	
	packet_write(*dst,(char *)&bag_id,sizeof(uint8));
	packet_write(*dst,(char *)&src_pos,sizeof(uint16));
	packet_write(*dst,(char *)&count,sizeof(uint32));
	packet_write(*dst,(char *)&dst_pos,sizeof(uint16));
	packet_write(*dst,(char *)&dst_bag,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_bag_item_split (packet *src ,uint8 *bag_id,uint16 *src_pos,uint32 *count,uint16 *dst_pos,uint8 *dst_bag)
{	
	int ret=0;
	ret = packet_read(src,(char*)(bag_id),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(src_pos),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(dst_pos),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(dst_bag),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*使用物品*/
int   pack_bag_item_user (packet**dst ,char const*item_guid,uint32 count)
{
	*dst = external_protocol_new_packet(CMSG_BAG_ITEM_USER);
	ASSERT((*dst)->head->optcode == CMSG_BAG_ITEM_USER);	
	packet_write_str(*dst,item_guid);		
	packet_write(*dst,(char *)&count,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_bag_item_user (packet *src ,char **item_guid,uint32 *count)
{	
	int ret=0;
	ret = packet_read_str(src,item_guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*下发物品冷却*/
int   pack_bag_item_cooldown (packet**dst , item_cooldown_info *list , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_BAG_ITEM_COOLDOWN);
	ASSERT((*dst)->head->optcode == SMSG_BAG_ITEM_COOLDOWN);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(item_cooldown_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_bag_item_cooldown (packet *src , item_cooldown_info **list , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (item_cooldown_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_cooldown_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*grid中的unit移动整体打包*/
int   pack_grid_unit_move (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_GRID_UNIT_MOVE);
	ASSERT((*dst)->head->optcode == SMSG_GRID_UNIT_MOVE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*grid中的unit移动整体打包2*/
int   pack_grid_unit_move_2 (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_GRID_UNIT_MOVE_2);
	ASSERT((*dst)->head->optcode == SMSG_GRID_UNIT_MOVE_2);	
		
	update_packet_len(*dst);
	return 0;	
}
/*兑换物品*/
int   pack_exchange_item (packet**dst ,uint32 entry,uint32 count,uint32 tar_entry)
{
	*dst = external_protocol_new_packet(CMSG_EXCHANGE_ITEM);
	ASSERT((*dst)->head->optcode == CMSG_EXCHANGE_ITEM);	
	packet_write(*dst,(char *)&entry,sizeof(uint32));
	packet_write(*dst,(char *)&count,sizeof(uint32));
	packet_write(*dst,(char *)&tar_entry,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_exchange_item (packet *src ,uint32 *entry,uint32 *count,uint32 *tar_entry)
{	
	int ret=0;
	ret = packet_read(src,(char*)(entry),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(tar_entry),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*背包扩展*/
int   pack_bag_extension (packet**dst ,uint8 bag_id,uint8 extension_type,uint32 bag_pos)
{
	*dst = external_protocol_new_packet(CMSG_BAG_EXTENSION);
	ASSERT((*dst)->head->optcode == CMSG_BAG_EXTENSION);	
	packet_write(*dst,(char *)&bag_id,sizeof(uint8));
	packet_write(*dst,(char *)&extension_type,sizeof(uint8));
	packet_write(*dst,(char *)&bag_pos,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_bag_extension (packet *src ,uint8 *bag_id,uint8 *extension_type,uint32 *bag_pos)
{	
	int ret=0;
	ret = packet_read(src,(char*)(bag_id),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(extension_type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(bag_pos),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*请求NPC商品列表*/
int   pack_npc_get_goods_list (packet**dst ,uint32 npc_id)
{
	*dst = external_protocol_new_packet(CMSG_NPC_GET_GOODS_LIST);
	ASSERT((*dst)->head->optcode == CMSG_NPC_GET_GOODS_LIST);	
	packet_write(*dst,(char *)&npc_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_npc_get_goods_list (packet *src ,uint32 *npc_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(npc_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*Npc商品列表*/
int   pack_npc_goods_list (packet**dst , uint32 *goods , uint16 len_1,uint32 npc_id)
{
	*dst = external_protocol_new_packet(SMSG_NPC_GOODS_LIST);
	ASSERT((*dst)->head->optcode == SMSG_NPC_GOODS_LIST);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)goods, sizeof(uint32) * len_1);
	packet_write(*dst,(char *)&npc_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_npc_goods_list (packet *src , uint32 **goods , uint16 *len_1,uint32 *npc_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*goods = (uint32 *)(src->content+src->rpos);
	src->rpos += sizeof(uint32) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	ret = packet_read(src,(char*)(npc_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*购买商品*/
int   pack_store_buy (packet**dst ,uint32 id,uint32 count)
{
	*dst = external_protocol_new_packet(CMSG_STORE_BUY);
	ASSERT((*dst)->head->optcode == CMSG_STORE_BUY);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
	packet_write(*dst,(char *)&count,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_store_buy (packet *src ,uint32 *id,uint32 *count)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*出售物品*/
int   pack_npc_sell (packet**dst ,uint32 npc_id,char const*item_guid,uint32 num)
{
	*dst = external_protocol_new_packet(CMSG_NPC_SELL);
	ASSERT((*dst)->head->optcode == CMSG_NPC_SELL);	
	packet_write(*dst,(char *)&npc_id,sizeof(uint32));
	packet_write_str(*dst,item_guid);		
	packet_write(*dst,(char *)&num,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_npc_sell (packet *src ,uint32 *npc_id,char **item_guid,uint32 *num)
{	
	int ret=0;
	ret = packet_read(src,(char*)(npc_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,item_guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(num),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*回购物品*/
int   pack_npc_repurchase (packet**dst ,char const*item_id)
{
	*dst = external_protocol_new_packet(CMSG_NPC_REPURCHASE);
	ASSERT((*dst)->head->optcode == CMSG_NPC_REPURCHASE);	
	packet_write_str(*dst,item_id);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_npc_repurchase (packet *src ,char **item_id)
{	
	int ret=0;
	ret = packet_read_str(src,item_id);
	if(ret) return -1;
	return ret;
}
/*时装是否启用*/
int   pack_avatar_fashion_enable (packet**dst ,uint8 pos)
{
	*dst = external_protocol_new_packet(CMSG_AVATAR_FASHION_ENABLE);
	ASSERT((*dst)->head->optcode == CMSG_AVATAR_FASHION_ENABLE);	
	packet_write(*dst,(char *)&pos,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_avatar_fashion_enable (packet *src ,uint8 *pos)
{	
	int ret=0;
	ret = packet_read(src,(char*)(pos),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*任务对话选项*/
int   pack_questhelp_talk_option (packet**dst ,uint32 quest_id,uint32 option_id,int32 value0,int32 value1)
{
	*dst = external_protocol_new_packet(CMSG_QUESTHELP_TALK_OPTION);
	ASSERT((*dst)->head->optcode == CMSG_QUESTHELP_TALK_OPTION);	
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
	packet_write(*dst,(char *)&option_id,sizeof(uint32));
	packet_write(*dst,(char *)&value0,sizeof(int32));
	packet_write(*dst,(char *)&value1,sizeof(int32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questhelp_talk_option (packet *src ,uint32 *quest_id,uint32 *option_id,int32 *value0,int32 *value1)
{	
	int ret=0;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(option_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(value0),sizeof(int32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(value1),sizeof(int32));
	if(ret) return -1;
	return ret;
}
/*与NPC对话获得传送点列表*/
int   pack_taxi_hello (packet**dst ,uint32 guid)
{
	*dst = external_protocol_new_packet(CMSG_TAXI_HELLO);
	ASSERT((*dst)->head->optcode == CMSG_TAXI_HELLO);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_taxi_hello (packet *src ,uint32 *guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*发送传送点列表*/
int   pack_taxi_stations_list (packet**dst ,uint32 npcid, taxi_menu_info *stations , uint16 len_2)
{
	*dst = external_protocol_new_packet(SMSG_TAXI_STATIONS_LIST);
	ASSERT((*dst)->head->optcode == SMSG_TAXI_STATIONS_LIST);	
	packet_write(*dst,(char *)&npcid,sizeof(uint32));
	packet_write(*dst,(char *)&len_2, sizeof(uint16));
	packet_write(*dst,(char *)stations, sizeof(taxi_menu_info) * len_2);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_taxi_stations_list (packet *src ,uint32 *npcid, taxi_menu_info **stations , uint16 *len_2)
{	
	int ret=0;
	ret = packet_read(src,(char*)(npcid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)len_2,sizeof(uint16));
	if(ret) return -1;
	*stations = (taxi_menu_info *)(src->content+src->rpos);
	src->rpos += sizeof(taxi_menu_info) * (*len_2);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*选择传送点*/
int   pack_taxi_select_station (packet**dst ,uint32 station_id,uint32 guid)
{
	*dst = external_protocol_new_packet(CMSG_TAXI_SELECT_STATION);
	ASSERT((*dst)->head->optcode == CMSG_TAXI_SELECT_STATION);	
	packet_write(*dst,(char *)&station_id,sizeof(uint32));
	packet_write(*dst,(char *)&guid,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_taxi_select_station (packet *src ,uint32 *station_id,uint32 *guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(station_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*与NPC交流选择选项*/
int   pack_gossip_select_option (packet**dst ,uint32 option,uint32 guid,char const*unknow)
{
	*dst = external_protocol_new_packet(CMSG_GOSSIP_SELECT_OPTION);
	ASSERT((*dst)->head->optcode == CMSG_GOSSIP_SELECT_OPTION);	
	packet_write(*dst,(char *)&option,sizeof(uint32));
	packet_write(*dst,(char *)&guid,sizeof(uint32));
	packet_write_str(*dst,unknow);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_gossip_select_option (packet *src ,uint32 *option,uint32 *guid,char **unknow)
{	
	int ret=0;
	ret = packet_read(src,(char*)(option),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,unknow);
	if(ret) return -1;
	return ret;
}
/*与NPC闲聊获取选项*/
int   pack_gossip_hello (packet**dst ,uint32 guid)
{
	*dst = external_protocol_new_packet(CMSG_GOSSIP_HELLO);
	ASSERT((*dst)->head->optcode == CMSG_GOSSIP_HELLO);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_gossip_hello (packet *src ,uint32 *guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*发送闲聊信息和选项*/
int   pack_gossip_message (packet**dst ,uint32 npcid,uint32 npc_entry,uint32 option_id,char const*option_text, gossip_menu_option_info *gossip_items , uint16 len_5)
{
	*dst = external_protocol_new_packet(SMSG_GOSSIP_MESSAGE);
	ASSERT((*dst)->head->optcode == SMSG_GOSSIP_MESSAGE);	
	packet_write(*dst,(char *)&npcid,sizeof(uint32));
	packet_write(*dst,(char *)&npc_entry,sizeof(uint32));
	packet_write(*dst,(char *)&option_id,sizeof(uint32));
	packet_write_str(*dst,option_text);		
	packet_write(*dst,(char *)&len_5, sizeof(uint16));
	packet_write(*dst,(char *)gossip_items, sizeof(gossip_menu_option_info) * len_5);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_gossip_message (packet *src ,uint32 *npcid,uint32 *npc_entry,uint32 *option_id,char **option_text, gossip_menu_option_info **gossip_items , uint16 *len_5)
{	
	int ret=0;
	ret = packet_read(src,(char*)(npcid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(npc_entry),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(option_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,option_text);
	if(ret) return -1;
	ret = packet_read(src,(char*)len_5,sizeof(uint16));
	if(ret) return -1;
	*gossip_items = (gossip_menu_option_info *)(src->content+src->rpos);
	src->rpos += sizeof(gossip_menu_option_info) * (*len_5);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*任务发布者状态查询*/
int   pack_questgiver_status_query (packet**dst ,uint32 guid)
{
	*dst = external_protocol_new_packet(CMSG_QUESTGIVER_STATUS_QUERY);
	ASSERT((*dst)->head->optcode == CMSG_QUESTGIVER_STATUS_QUERY);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questgiver_status_query (packet *src ,uint32 *guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*任务NPC状态*/
int   pack_questgiver_status (packet**dst ,uint32 guid,uint8 status)
{
	*dst = external_protocol_new_packet(SMSG_QUESTGIVER_STATUS);
	ASSERT((*dst)->head->optcode == SMSG_QUESTGIVER_STATUS);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
	packet_write(*dst,(char *)&status,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questgiver_status (packet *src ,uint32 *guid,uint8 *status)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(status),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*查询任务状态*/
int   pack_query_quest_status (packet**dst , quest_status *quest_array , uint16 len_1)
{
	*dst = external_protocol_new_packet(MSG_QUERY_QUEST_STATUS);
	ASSERT((*dst)->head->optcode == MSG_QUERY_QUEST_STATUS);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)quest_array, sizeof(quest_status) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_query_quest_status (packet *src , quest_status **quest_array , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*quest_array = (quest_status *)(src->content+src->rpos);
	src->rpos += sizeof(quest_status) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*可接任务*/
int   pack_questhelp_get_canaccept_list (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_QUESTHELP_GET_CANACCEPT_LIST);
	ASSERT((*dst)->head->optcode == CMSG_QUESTHELP_GET_CANACCEPT_LIST);	
		
	update_packet_len(*dst);
	return 0;	
}
/*下发可接任务列表*/
int   pack_questhelp_canaccept_list (packet**dst , uint32 *quests , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_QUESTHELP_CANACCEPT_LIST);
	ASSERT((*dst)->head->optcode == SMSG_QUESTHELP_CANACCEPT_LIST);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)quests, sizeof(uint32) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questhelp_canaccept_list (packet *src , uint32 **quests , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*quests = (uint32 *)(src->content+src->rpos);
	src->rpos += sizeof(uint32) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*任务失败*/
int   pack_questupdate_faild (packet**dst ,uint8 reason)
{
	*dst = external_protocol_new_packet(SMSG_QUESTUPDATE_FAILD);
	ASSERT((*dst)->head->optcode == SMSG_QUESTUPDATE_FAILD);	
	packet_write(*dst,(char *)&reason,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questupdate_faild (packet *src ,uint8 *reason)
{	
	int ret=0;
	ret = packet_read(src,(char*)(reason),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*任务条件完成*/
int   pack_questupdate_complete (packet**dst ,uint32 quest_id)
{
	*dst = external_protocol_new_packet(SMSG_QUESTUPDATE_COMPLETE);
	ASSERT((*dst)->head->optcode == SMSG_QUESTUPDATE_COMPLETE);	
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questupdate_complete (packet *src ,uint32 *quest_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*放弃任务*/
int   pack_questlog_remove_quest (packet**dst ,uint8 slot,uint64 reserve)
{
	*dst = external_protocol_new_packet(CMSG_QUESTLOG_REMOVE_QUEST);
	ASSERT((*dst)->head->optcode == CMSG_QUESTLOG_REMOVE_QUEST);	
	packet_write(*dst,(char *)&slot,sizeof(uint8));
	packet_write(*dst,(char *)&reserve,sizeof(uint64));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questlog_remove_quest (packet *src ,uint8 *slot,uint64 *reserve)
{	
	int ret=0;
	ret = packet_read(src,(char*)(slot),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve),sizeof(uint64));
	if(ret) return -1;
	return ret;
}
/*完成任务*/
int   pack_questgiver_complete_quest (packet**dst ,uint32 guid,uint32 quest_id,uint8 reward)
{
	*dst = external_protocol_new_packet(CMSG_QUESTGIVER_COMPLETE_QUEST);
	ASSERT((*dst)->head->optcode == CMSG_QUESTGIVER_COMPLETE_QUEST);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
	packet_write(*dst,(char *)&reward,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questgiver_complete_quest (packet *src ,uint32 *guid,uint32 *quest_id,uint8 *reward)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reward),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*完成任务后通知任务链的下个任务*/
int   pack_questhelp_next (packet**dst ,uint32 current_id,uint32 next_id,uint32 guid)
{
	*dst = external_protocol_new_packet(SMSG_QUESTHELP_NEXT);
	ASSERT((*dst)->head->optcode == SMSG_QUESTHELP_NEXT);	
	packet_write(*dst,(char *)&current_id,sizeof(uint32));
	packet_write(*dst,(char *)&next_id,sizeof(uint32));
	packet_write(*dst,(char *)&guid,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questhelp_next (packet *src ,uint32 *current_id,uint32 *next_id,uint32 *guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(current_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(next_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*任务系统强制完成任务*/
int   pack_questhelp_complete (packet**dst ,uint32 quest_id,uint8 quest_statue,uint8 reserve)
{
	*dst = external_protocol_new_packet(CMSG_QUESTHELP_COMPLETE);
	ASSERT((*dst)->head->optcode == CMSG_QUESTHELP_COMPLETE);	
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
	packet_write(*dst,(char *)&quest_statue,sizeof(uint8));
	packet_write(*dst,(char *)&reserve,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questhelp_complete (packet *src ,uint32 *quest_id,uint8 *quest_statue,uint8 *reserve)
{	
	int ret=0;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(quest_statue),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*接受任务成功*/
int   pack_questupdate_accept (packet**dst ,uint32 quest_id)
{
	*dst = external_protocol_new_packet(SMSG_QUESTUPDATE_ACCEPT);
	ASSERT((*dst)->head->optcode == SMSG_QUESTUPDATE_ACCEPT);	
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questupdate_accept (packet *src ,uint32 *quest_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*更新任务进度_下标数量*/
int   pack_questhelp_update_status (packet**dst ,uint32 quest_id,uint32 slot_id,uint32 num)
{
	*dst = external_protocol_new_packet(CMSG_QUESTHELP_UPDATE_STATUS);
	ASSERT((*dst)->head->optcode == CMSG_QUESTHELP_UPDATE_STATUS);	
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
	packet_write(*dst,(char *)&slot_id,sizeof(uint32));
	packet_write(*dst,(char *)&num,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questhelp_update_status (packet *src ,uint32 *quest_id,uint32 *slot_id,uint32 *num)
{	
	int ret=0;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(slot_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(num),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*任务已完成*/
int   pack_questgetter_complete (packet**dst ,uint32 quest_id)
{
	*dst = external_protocol_new_packet(SMSG_QUESTGETTER_COMPLETE);
	ASSERT((*dst)->head->optcode == SMSG_QUESTGETTER_COMPLETE);	
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questgetter_complete (packet *src ,uint32 *quest_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*接任务*/
int   pack_questgiver_accept_quest (packet**dst ,uint32 npcid,uint32 quest_id)
{
	*dst = external_protocol_new_packet(CMSG_QUESTGIVER_ACCEPT_QUEST);
	ASSERT((*dst)->head->optcode == CMSG_QUESTGIVER_ACCEPT_QUEST);	
	packet_write(*dst,(char *)&npcid,sizeof(uint32));
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questgiver_accept_quest (packet *src ,uint32 *npcid,uint32 *quest_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(npcid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*任务使用物品*/
int   pack_questupdate_use_item (packet**dst ,uint32 item_id)
{
	*dst = external_protocol_new_packet(CMSG_QUESTUPDATE_USE_ITEM);
	ASSERT((*dst)->head->optcode == CMSG_QUESTUPDATE_USE_ITEM);	
	packet_write(*dst,(char *)&item_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questupdate_use_item (packet *src ,uint32 *item_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(item_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*查询天书任务*/
int   pack_questhelp_query_book (packet**dst ,uint32 dynasty)
{
	*dst = external_protocol_new_packet(CMSG_QUESTHELP_QUERY_BOOK);
	ASSERT((*dst)->head->optcode == CMSG_QUESTHELP_QUERY_BOOK);	
	packet_write(*dst,(char *)&dynasty,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questhelp_query_book (packet *src ,uint32 *dynasty)
{	
	int ret=0;
	ret = packet_read(src,(char*)(dynasty),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*下发可接天书任务*/
int   pack_questhelp_book_quest (packet**dst ,uint32 quest_id)
{
	*dst = external_protocol_new_packet(SMSG_QUESTHELP_BOOK_QUEST);
	ASSERT((*dst)->head->optcode == SMSG_QUESTHELP_BOOK_QUEST);	
	packet_write(*dst,(char *)&quest_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_questhelp_book_quest (packet *src ,uint32 *quest_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(quest_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*玩家使用游戏对象以后的动作*/
int   pack_use_gameobject_action (packet**dst ,uint32 player_id,uint32 gameobject_id)
{
	*dst = external_protocol_new_packet(SMSG_USE_GAMEOBJECT_ACTION);
	ASSERT((*dst)->head->optcode == SMSG_USE_GAMEOBJECT_ACTION);	
	packet_write(*dst,(char *)&player_id,sizeof(uint32));
	packet_write(*dst,(char *)&gameobject_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_use_gameobject_action (packet *src ,uint32 *player_id,uint32 *gameobject_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(player_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(gameobject_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*设置攻击模式*/
int   pack_set_attack_mode (packet**dst ,uint8 mode,uint64 reserve)
{
	*dst = external_protocol_new_packet(CMSG_SET_ATTACK_MODE);
	ASSERT((*dst)->head->optcode == CMSG_SET_ATTACK_MODE);	
	packet_write(*dst,(char *)&mode,sizeof(uint8));
	packet_write(*dst,(char *)&reserve,sizeof(uint64));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_set_attack_mode (packet *src ,uint8 *mode,uint64 *reserve)
{	
	int ret=0;
	ret = packet_read(src,(char*)(mode),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve),sizeof(uint64));
	if(ret) return -1;
	return ret;
}
/*选择目标*/
int   pack_select_target (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(MSG_SELECT_TARGET);
	ASSERT((*dst)->head->optcode == MSG_SELECT_TARGET);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_select_target (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*进入战斗*/
int   pack_combat_state_update (packet**dst ,uint8 cur_state)
{
	*dst = external_protocol_new_packet(SMSG_COMBAT_STATE_UPDATE);
	ASSERT((*dst)->head->optcode == SMSG_COMBAT_STATE_UPDATE);	
	packet_write(*dst,(char *)&cur_state,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_combat_state_update (packet *src ,uint8 *cur_state)
{	
	int ret=0;
	ret = packet_read(src,(char*)(cur_state),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*经验更新*/
int   pack_exp_update (packet**dst ,int32 exp,uint8 type,int32 vip_exp)
{
	*dst = external_protocol_new_packet(SMSG_EXP_UPDATE);
	ASSERT((*dst)->head->optcode == SMSG_EXP_UPDATE);	
	packet_write(*dst,(char *)&exp,sizeof(int32));
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write(*dst,(char *)&vip_exp,sizeof(int32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_exp_update (packet *src ,int32 *exp,uint8 *type,int32 *vip_exp)
{	
	int ret=0;
	ret = packet_read(src,(char*)(exp),sizeof(int32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(vip_exp),sizeof(int32));
	if(ret) return -1;
	return ret;
}
/*客户端释放技能*/
int   pack_spell_start (packet**dst ,uint32 spell_id,uint16 target_pos_x,uint16 target_pos_y,uint32 caster,uint32 target)
{
	*dst = external_protocol_new_packet(MSG_SPELL_START);
	ASSERT((*dst)->head->optcode == MSG_SPELL_START);	
	packet_write(*dst,(char *)&spell_id,sizeof(uint32));
	packet_write(*dst,(char *)&target_pos_x,sizeof(uint16));
	packet_write(*dst,(char *)&target_pos_y,sizeof(uint16));
	packet_write(*dst,(char *)&caster,sizeof(uint32));
	packet_write(*dst,(char *)&target,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_spell_start (packet *src ,uint32 *spell_id,uint16 *target_pos_x,uint16 *target_pos_y,uint32 *caster,uint32 *target)
{	
	int ret=0;
	ret = packet_read(src,(char*)(spell_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(target_pos_x),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(target_pos_y),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(caster),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(target),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*施法停止*/
int   pack_spell_stop (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(MSG_SPELL_STOP);
	ASSERT((*dst)->head->optcode == MSG_SPELL_STOP);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_spell_stop (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*跳*/
int   pack_jump (packet**dst ,uint32 guid,float pos_x,float pos_y)
{
	*dst = external_protocol_new_packet(MSG_JUMP);
	ASSERT((*dst)->head->optcode == MSG_JUMP);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
	packet_write(*dst,(char *)&pos_x,sizeof(float));
	packet_write(*dst,(char *)&pos_y,sizeof(float));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_jump (packet *src ,uint32 *guid,float *pos_x,float *pos_y)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(pos_x),sizeof(float));
	if(ret) return -1;
	ret = packet_read(src,(char*)(pos_y),sizeof(float));
	if(ret) return -1;
	return ret;
}
/*复活*/
int   pack_resurrection (packet**dst ,uint8 type,uint64 reserve)
{
	*dst = external_protocol_new_packet(CMSG_RESURRECTION);
	ASSERT((*dst)->head->optcode == CMSG_RESURRECTION);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write(*dst,(char *)&reserve,sizeof(uint64));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_resurrection (packet *src ,uint8 *type,uint64 *reserve)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve),sizeof(uint64));
	if(ret) return -1;
	return ret;
}
/*交易发出请求*/
int   pack_trade_request (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(MSG_TRADE_REQUEST);
	ASSERT((*dst)->head->optcode == MSG_TRADE_REQUEST);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_trade_request (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*交易请求答复*/
int   pack_trade_reply (packet**dst ,char const*guid,uint8 reply)
{
	*dst = external_protocol_new_packet(MSG_TRADE_REPLY);
	ASSERT((*dst)->head->optcode == MSG_TRADE_REPLY);	
	packet_write_str(*dst,guid);		
	packet_write(*dst,(char *)&reply,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_trade_reply (packet *src ,char **guid,uint8 *reply)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(reply),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*交易开始*/
int   pack_trade_start (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(SMSG_TRADE_START);
	ASSERT((*dst)->head->optcode == SMSG_TRADE_START);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_trade_start (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*交易确认物品*/
int   pack_trade_decide_items (packet**dst ,const char **items, uint16 len_1,int32 gold_ingot,int32 silver)
{
	uint16 i = 0;
	*dst = external_protocol_new_packet(MSG_TRADE_DECIDE_ITEMS);
	ASSERT((*dst)->head->optcode == MSG_TRADE_DECIDE_ITEMS);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	for(;i < len_1; i++)
	{
		packet_write_str(*dst,*(items + i));	
	}
	packet_write(*dst,(char *)&gold_ingot,sizeof(int32));
	packet_write(*dst,(char *)&silver,sizeof(int32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_trade_decide_items (packet *src ,const char **items, uint16 *len_1,int32 *gold_ingot,int32 *silver)
{	
	int ret=0;
	uint16 i = 0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	if(*len_1 > EXTERNAL_PACKET_STR_ARRAY_MAX_LEN)
		return -1;
	for(; i < *len_1; i++)
	{
		char* p ;
		ret = packet_read_str(src,&p);
		if(ret) break;
		items[i]=p;
	}
	ret = packet_read(src,(char*)(gold_ingot),sizeof(int32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(silver),sizeof(int32));
	if(ret) return -1;
	return ret;
}
/*交易完成*/
int   pack_trade_finish (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_TRADE_FINISH);
	ASSERT((*dst)->head->optcode == SMSG_TRADE_FINISH);	
		
	update_packet_len(*dst);
	return 0;	
}
/*交易取消*/
int   pack_trade_cancel (packet**dst )
{
	*dst = external_protocol_new_packet(MSG_TRADE_CANCEL);
	ASSERT((*dst)->head->optcode == MSG_TRADE_CANCEL);	
		
	update_packet_len(*dst);
	return 0;	
}
/*交易准备好*/
int   pack_trade_ready (packet**dst )
{
	*dst = external_protocol_new_packet(MSG_TRADE_READY);
	ASSERT((*dst)->head->optcode == MSG_TRADE_READY);	
		
	update_packet_len(*dst);
	return 0;	
}
/*生物讲话*/
int   pack_chat_unit_talk (packet**dst ,uint32 guid,uint32 content,char const*say)
{
	*dst = external_protocol_new_packet(SMSG_CHAT_UNIT_TALK);
	ASSERT((*dst)->head->optcode == SMSG_CHAT_UNIT_TALK);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
	packet_write(*dst,(char *)&content,sizeof(uint32));
	packet_write_str(*dst,say);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chat_unit_talk (packet *src ,uint32 *guid,uint32 *content,char **say)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(content),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,say);
	if(ret) return -1;
	return ret;
}
/*就近聊天*/
int   pack_chat_near (packet**dst ,char const*content)
{
	*dst = external_protocol_new_packet(CMSG_CHAT_NEAR);
	ASSERT((*dst)->head->optcode == CMSG_CHAT_NEAR);	
	packet_write_str(*dst,content);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chat_near (packet *src ,char **content)
{	
	int ret=0;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	return ret;
}
/*私聊*/
int   pack_chat_whisper (packet**dst ,char const*guid,char const*content)
{
	*dst = external_protocol_new_packet(CMSG_CHAT_WHISPER);
	ASSERT((*dst)->head->optcode == CMSG_CHAT_WHISPER);	
	packet_write_str(*dst,guid);		
	packet_write_str(*dst,content);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chat_whisper (packet *src ,char **guid,char **content)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	return ret;
}
/*阵营聊天*/
int   pack_chat_faction (packet**dst ,char const*guid,char const*name,char const*content,uint8 faction)
{
	*dst = external_protocol_new_packet(MSG_CHAT_FACTION);
	ASSERT((*dst)->head->optcode == MSG_CHAT_FACTION);	
	packet_write_str(*dst,guid);		
	packet_write_str(*dst,name);		
	packet_write_str(*dst,content);		
	packet_write(*dst,(char *)&faction,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chat_faction (packet *src ,char **guid,char **name,char **content,uint8 *faction)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	ret = packet_read(src,(char*)(faction),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*世界*/
int   pack_chat_world (packet**dst ,char const*guid,uint8 faction,char const*name,char const*content)
{
	*dst = external_protocol_new_packet(MSG_CHAT_WORLD);
	ASSERT((*dst)->head->optcode == MSG_CHAT_WORLD);	
	packet_write_str(*dst,guid);		
	packet_write(*dst,(char *)&faction,sizeof(uint8));
	packet_write_str(*dst,name);		
	packet_write_str(*dst,content);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chat_world (packet *src ,char **guid,uint8 *faction,char **name,char **content)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(faction),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	return ret;
}
/*喇叭*/
int   pack_chat_horn (packet**dst ,char const*guid,uint8 faction,char const*name,char const*content)
{
	*dst = external_protocol_new_packet(MSG_CHAT_HORN);
	ASSERT((*dst)->head->optcode == MSG_CHAT_HORN);	
	packet_write_str(*dst,guid);		
	packet_write(*dst,(char *)&faction,sizeof(uint8));
	packet_write_str(*dst,name);		
	packet_write_str(*dst,content);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chat_horn (packet *src ,char **guid,uint8 *faction,char **name,char **content)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(faction),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	return ret;
}
/*公告*/
int   pack_chat_notice (packet**dst ,uint32 id,char const*content,char const*data)
{
	*dst = external_protocol_new_packet(MSG_CHAT_NOTICE);
	ASSERT((*dst)->head->optcode == MSG_CHAT_NOTICE);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
	packet_write_str(*dst,content);		
	packet_write_str(*dst,data);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chat_notice (packet *src ,uint32 *id,char **content,char **data)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	ret = packet_read_str(src,data);
	if(ret) return -1;
	return ret;
}
/*查询玩家信息*/
int   pack_query_player_info (packet**dst ,char const*guid,uint32 flag,uint32 callback_id)
{
	*dst = external_protocol_new_packet(CMSG_QUERY_PLAYER_INFO);
	ASSERT((*dst)->head->optcode == CMSG_QUERY_PLAYER_INFO);	
	packet_write_str(*dst,guid);		
	packet_write(*dst,(char *)&flag,sizeof(uint32));
	packet_write(*dst,(char *)&callback_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_query_player_info (packet *src ,char **guid,uint32 *flag,uint32 *callback_id)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(flag),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(callback_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*查询信息对象更新*/
int   pack_query_result_update_object (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_QUERY_RESULT_UPDATE_OBJECT);
	ASSERT((*dst)->head->optcode == SMSG_QUERY_RESULT_UPDATE_OBJECT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*领取礼包*/
int   pack_receive_gift_packs (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_RECEIVE_GIFT_PACKS);
	ASSERT((*dst)->head->optcode == CMSG_RECEIVE_GIFT_PACKS);	
		
	update_packet_len(*dst);
	return 0;	
}
/*地图对象更新*/
int   pack_map_update_object (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_MAP_UPDATE_OBJECT);
	ASSERT((*dst)->head->optcode == SMSG_MAP_UPDATE_OBJECT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*战斗信息binlog*/
int   pack_fighting_info_update_object (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_FIGHTING_INFO_UPDATE_OBJECT);
	ASSERT((*dst)->head->optcode == SMSG_FIGHTING_INFO_UPDATE_OBJECT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*战斗信息binlog*/
int   pack_fighting_info_update_object_2 (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_FIGHTING_INFO_UPDATE_OBJECT_2);
	ASSERT((*dst)->head->optcode == SMSG_FIGHTING_INFO_UPDATE_OBJECT_2);	
		
	update_packet_len(*dst);
	return 0;	
}
/*进入副本*/
int   pack_instance_enter (packet**dst ,uint32 instance_id)
{
	*dst = external_protocol_new_packet(CMSG_INSTANCE_ENTER);
	ASSERT((*dst)->head->optcode == CMSG_INSTANCE_ENTER);	
	packet_write(*dst,(char *)&instance_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_instance_enter (packet *src ,uint32 *instance_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(instance_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*向服务端发送副本进入下一阶段指令*/
int   pack_instance_next_state (packet**dst ,uint16 level,uint32 param)
{
	*dst = external_protocol_new_packet(CMSG_INSTANCE_NEXT_STATE);
	ASSERT((*dst)->head->optcode == CMSG_INSTANCE_NEXT_STATE);	
	packet_write(*dst,(char *)&level,sizeof(uint16));
	packet_write(*dst,(char *)&param,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_instance_next_state (packet *src ,uint16 *level,uint32 *param)
{	
	int ret=0;
	ret = packet_read(src,(char*)(level),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(param),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*副本退出*/
int   pack_instance_exit (packet**dst ,uint32 reserve)
{
	*dst = external_protocol_new_packet(CMSG_INSTANCE_EXIT);
	ASSERT((*dst)->head->optcode == CMSG_INSTANCE_EXIT);	
	packet_write(*dst,(char *)&reserve,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_instance_exit (packet *src ,uint32 *reserve)
{	
	int ret=0;
	ret = packet_read(src,(char*)(reserve),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*限时活动领取*/
int   pack_limit_activity_receive (packet**dst ,uint32 id,uint32 type)
{
	*dst = external_protocol_new_packet(CMSG_LIMIT_ACTIVITY_RECEIVE);
	ASSERT((*dst)->head->optcode == CMSG_LIMIT_ACTIVITY_RECEIVE);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
	packet_write(*dst,(char *)&type,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_limit_activity_receive (packet *src ,uint32 *id,uint32 *type)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(type),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*杀人啦~~！！！！*/
int   pack_kill_man (packet**dst ,char const*killer,char const*killer_name,char const*victim,char const*victim_name)
{
	*dst = external_protocol_new_packet(SMSG_KILL_MAN);
	ASSERT((*dst)->head->optcode == SMSG_KILL_MAN);	
	packet_write_str(*dst,killer);		
	packet_write_str(*dst,killer_name);		
	packet_write_str(*dst,victim);		
	packet_write_str(*dst,victim_name);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kill_man (packet *src ,char **killer,char **killer_name,char **victim,char **victim_name)
{	
	int ret=0;
	ret = packet_read_str(src,killer);
	if(ret) return -1;
	ret = packet_read_str(src,killer_name);
	if(ret) return -1;
	ret = packet_read_str(src,victim);
	if(ret) return -1;
	ret = packet_read_str(src,victim_name);
	if(ret) return -1;
	return ret;
}
/*玩家升级*/
int   pack_player_upgrade (packet**dst ,uint32 guid)
{
	*dst = external_protocol_new_packet(SMSG_PLAYER_UPGRADE);
	ASSERT((*dst)->head->optcode == SMSG_PLAYER_UPGRADE);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_player_upgrade (packet *src ,uint32 *guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*仓库存钱*/
int   pack_warehouse_save_money (packet**dst ,int32 money,int32 money_gold,int32 money_bills)
{
	*dst = external_protocol_new_packet(CMSG_WAREHOUSE_SAVE_MONEY);
	ASSERT((*dst)->head->optcode == CMSG_WAREHOUSE_SAVE_MONEY);	
	packet_write(*dst,(char *)&money,sizeof(int32));
	packet_write(*dst,(char *)&money_gold,sizeof(int32));
	packet_write(*dst,(char *)&money_bills,sizeof(int32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_warehouse_save_money (packet *src ,int32 *money,int32 *money_gold,int32 *money_bills)
{	
	int ret=0;
	ret = packet_read(src,(char*)(money),sizeof(int32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(money_gold),sizeof(int32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(money_bills),sizeof(int32));
	if(ret) return -1;
	return ret;
}
/*仓库取钱*/
int   pack_warehouse_take_money (packet**dst ,int32 money,int32 money_gold,int32 money_bills)
{
	*dst = external_protocol_new_packet(CMSG_WAREHOUSE_TAKE_MONEY);
	ASSERT((*dst)->head->optcode == CMSG_WAREHOUSE_TAKE_MONEY);	
	packet_write(*dst,(char *)&money,sizeof(int32));
	packet_write(*dst,(char *)&money_gold,sizeof(int32));
	packet_write(*dst,(char *)&money_bills,sizeof(int32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_warehouse_take_money (packet *src ,int32 *money,int32 *money_gold,int32 *money_bills)
{	
	int ret=0;
	ret = packet_read(src,(char*)(money),sizeof(int32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(money_gold),sizeof(int32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(money_bills),sizeof(int32));
	if(ret) return -1;
	return ret;
}
/*使用元宝操作某事*/
int   pack_use_gold_opt (packet**dst ,uint8 type,char const*param)
{
	*dst = external_protocol_new_packet(CMSG_USE_GOLD_OPT);
	ASSERT((*dst)->head->optcode == CMSG_USE_GOLD_OPT);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write_str(*dst,param);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_use_gold_opt (packet *src ,uint8 *type,char **param)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,param);
	if(ret) return -1;
	return ret;
}
/*使用铜钱操作某事*/
int   pack_use_silver_opt (packet**dst ,uint8 type)
{
	*dst = external_protocol_new_packet(CMSG_USE_SILVER_OPT);
	ASSERT((*dst)->head->optcode == CMSG_USE_SILVER_OPT);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_use_silver_opt (packet *src ,uint8 *type)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*后台弹窗*/
int   pack_gm_rightfloat (packet**dst ,uint32 id,uint32 end_time,uint32 zone1,uint32 zone2,uint32 zone3,char const*subject,char const*content,char const*link,uint8 mode)
{
	*dst = external_protocol_new_packet(SMSG_GM_RIGHTFLOAT);
	ASSERT((*dst)->head->optcode == SMSG_GM_RIGHTFLOAT);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
	packet_write(*dst,(char *)&end_time,sizeof(uint32));
	packet_write(*dst,(char *)&zone1,sizeof(uint32));
	packet_write(*dst,(char *)&zone2,sizeof(uint32));
	packet_write(*dst,(char *)&zone3,sizeof(uint32));
	packet_write_str(*dst,subject);		
	packet_write_str(*dst,content);		
	packet_write_str(*dst,link);		
	packet_write(*dst,(char *)&mode,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_gm_rightfloat (packet *src ,uint32 *id,uint32 *end_time,uint32 *zone1,uint32 *zone2,uint32 *zone3,char **subject,char **content,char **link,uint8 *mode)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(end_time),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(zone1),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(zone2),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(zone3),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,subject);
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	ret = packet_read_str(src,link);
	if(ret) return -1;
	ret = packet_read(src,(char*)(mode),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*删除某条后台弹窗*/
int   pack_del_gm_rightfloat (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(SMSG_DEL_GM_RIGHTFLOAT);
	ASSERT((*dst)->head->optcode == SMSG_DEL_GM_RIGHTFLOAT);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_del_gm_rightfloat (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*应用服同步时间*/
int   pack_sync_mstime_app (packet**dst ,uint32 mstime_now,uint32 time_now,uint32 open_time)
{
	*dst = external_protocol_new_packet(MSG_SYNC_MSTIME_APP);
	ASSERT((*dst)->head->optcode == MSG_SYNC_MSTIME_APP);	
	packet_write(*dst,(char *)&mstime_now,sizeof(uint32));
	packet_write(*dst,(char *)&time_now,sizeof(uint32));
	packet_write(*dst,(char *)&open_time,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_sync_mstime_app (packet *src ,uint32 *mstime_now,uint32 *time_now,uint32 *open_time)
{	
	int ret=0;
	ret = packet_read(src,(char*)(mstime_now),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(time_now),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(open_time),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*玩家打开某个窗口*/
int   pack_open_window (packet**dst ,uint32 window_type)
{
	*dst = external_protocol_new_packet(CMSG_OPEN_WINDOW);
	ASSERT((*dst)->head->optcode == CMSG_OPEN_WINDOW);	
	packet_write(*dst,(char *)&window_type,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_open_window (packet *src ,uint32 *window_type)
{	
	int ret=0;
	ret = packet_read(src,(char*)(window_type),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*禁言操作*/
int   pack_player_gag (packet**dst ,char const*player_id,uint32 end_time,char const*content)
{
	*dst = external_protocol_new_packet(CMSG_PLAYER_GAG);
	ASSERT((*dst)->head->optcode == CMSG_PLAYER_GAG);	
	packet_write_str(*dst,player_id);		
	packet_write(*dst,(char *)&end_time,sizeof(uint32));
	packet_write_str(*dst,content);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_player_gag (packet *src ,char **player_id,uint32 *end_time,char **content)
{	
	int ret=0;
	ret = packet_read_str(src,player_id);
	if(ret) return -1;
	ret = packet_read(src,(char*)(end_time),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	return ret;
}
/*踢人操作*/
int   pack_player_kicking (packet**dst ,char const*player_id)
{
	*dst = external_protocol_new_packet(CMSG_PLAYER_KICKING);
	ASSERT((*dst)->head->optcode == CMSG_PLAYER_KICKING);	
	packet_write_str(*dst,player_id);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_player_kicking (packet *src ,char **player_id)
{	
	int ret=0;
	ret = packet_read_str(src,player_id);
	if(ret) return -1;
	return ret;
}
/*合服通知*/
int   pack_merge_server_msg (packet**dst ,char const*merge_host,uint32 merge_port,char const*merge_key,uint32 merge_type,uint32 reserve,uint32 reserve2)
{
	*dst = external_protocol_new_packet(SMSG_MERGE_SERVER_MSG);
	ASSERT((*dst)->head->optcode == SMSG_MERGE_SERVER_MSG);	
	packet_write_str(*dst,merge_host);		
	packet_write(*dst,(char *)&merge_port,sizeof(uint32));
	packet_write_str(*dst,merge_key);		
	packet_write(*dst,(char *)&merge_type,sizeof(uint32));
	packet_write(*dst,(char *)&reserve,sizeof(uint32));
	packet_write(*dst,(char *)&reserve2,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_merge_server_msg (packet *src ,char **merge_host,uint32 *merge_port,char **merge_key,uint32 *merge_type,uint32 *reserve,uint32 *reserve2)
{	
	int ret=0;
	ret = packet_read_str(src,merge_host);
	if(ret) return -1;
	ret = packet_read(src,(char*)(merge_port),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,merge_key);
	if(ret) return -1;
	ret = packet_read(src,(char*)(merge_type),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve2),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*获取排行信息*/
int   pack_rank_list_query (packet**dst ,uint32 call_back_id,uint8 rank_list_type,uint16 start_index,uint16 end_index)
{
	*dst = external_protocol_new_packet(CMSG_RANK_LIST_QUERY);
	ASSERT((*dst)->head->optcode == CMSG_RANK_LIST_QUERY);	
	packet_write(*dst,(char *)&call_back_id,sizeof(uint32));
	packet_write(*dst,(char *)&rank_list_type,sizeof(uint8));
	packet_write(*dst,(char *)&start_index,sizeof(uint16));
	packet_write(*dst,(char *)&end_index,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_rank_list_query (packet *src ,uint32 *call_back_id,uint8 *rank_list_type,uint16 *start_index,uint16 *end_index)
{	
	int ret=0;
	ret = packet_read(src,(char*)(call_back_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(rank_list_type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(start_index),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(end_index),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*客户端获取排行榜返回结果*/
int   pack_rank_list_query_result (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_RANK_LIST_QUERY_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_RANK_LIST_QUERY_RESULT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*客户端热更场景模块后获取uint*/
int   pack_client_update_scened (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_CLIENT_UPDATE_SCENED);
	ASSERT((*dst)->head->optcode == CMSG_CLIENT_UPDATE_SCENED);	
		
	update_packet_len(*dst);
	return 0;	
}
/*数值包*/
int   pack_num_lua (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_NUM_LUA);
	ASSERT((*dst)->head->optcode == SMSG_NUM_LUA);	
		
	update_packet_len(*dst);
	return 0;	
}
/*战利品拾取*/
int   pack_loot_select (packet**dst ,uint16 x,uint16 y)
{
	*dst = external_protocol_new_packet(CMSG_LOOT_SELECT);
	ASSERT((*dst)->head->optcode == CMSG_LOOT_SELECT);	
	packet_write(*dst,(char *)&x,sizeof(uint16));
	packet_write(*dst,(char *)&y,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_loot_select (packet *src ,uint16 *x,uint16 *y)
{	
	int ret=0;
	ret = packet_read(src,(char*)(x),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(y),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*通知登录服把玩家传回游戏服*/
int   pack_goback_to_game_server (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_GOBACK_TO_GAME_SERVER);
	ASSERT((*dst)->head->optcode == CMSG_GOBACK_TO_GAME_SERVER);	
		
	update_packet_len(*dst);
	return 0;	
}
/*客户端把比赛人员数据传给比赛服*/
int   pack_world_war_CS_player_info (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WORLD_WAR_CS_PLAYER_INFO);
	ASSERT((*dst)->head->optcode == CMSG_WORLD_WAR_CS_PLAYER_INFO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*玩家加入或者离开某服务器*/
int   pack_join_or_leave_server (packet**dst ,uint8 join_or_leave,uint8 server_type,uint32 server_pid,uint32 server_fd,uint32 time)
{
	*dst = external_protocol_new_packet(SMSG_JOIN_OR_LEAVE_SERVER);
	ASSERT((*dst)->head->optcode == SMSG_JOIN_OR_LEAVE_SERVER);	
	packet_write(*dst,(char *)&join_or_leave,sizeof(uint8));
	packet_write(*dst,(char *)&server_type,sizeof(uint8));
	packet_write(*dst,(char *)&server_pid,sizeof(uint32));
	packet_write(*dst,(char *)&server_fd,sizeof(uint32));
	packet_write(*dst,(char *)&time,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_join_or_leave_server (packet *src ,uint8 *join_or_leave,uint8 *server_type,uint32 *server_pid,uint32 *server_fd,uint32 *time)
{	
	int ret=0;
	ret = packet_read(src,(char*)(join_or_leave),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(server_type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(server_pid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(server_fd),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(time),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*客户端请求跨服人员数据*/
int   pack_world_war_SC_player_info (packet**dst )
{
	*dst = external_protocol_new_packet(MSG_WORLD_WAR_SC_PLAYER_INFO);
	ASSERT((*dst)->head->optcode == MSG_WORLD_WAR_SC_PLAYER_INFO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*客户端订阅信息*/
int   pack_clientSubscription (packet**dst ,uint32 guid)
{
	*dst = external_protocol_new_packet(MSG_CLIENTSUBSCRIPTION);
	ASSERT((*dst)->head->optcode == MSG_CLIENTSUBSCRIPTION);	
	packet_write(*dst,(char *)&guid,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_clientSubscription (packet *src ,uint32 *guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guid),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*服务端下发lua脚本*/
int   pack_lua_script (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_LUA_SCRIPT);
	ASSERT((*dst)->head->optcode == SMSG_LUA_SCRIPT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*角色更改信息*/
int   pack_char_update_info (packet**dst , char_create_info *info)
{
	*dst = external_protocol_new_packet(CMSG_CHAR_UPDATE_INFO);
	ASSERT((*dst)->head->optcode == CMSG_CHAR_UPDATE_INFO);	
	packet_write(*dst,(char*)info,sizeof(char_create_info));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_char_update_info (packet *src , char_create_info **info)
{	
	int ret=0;
	*info = (char_create_info*)(src->content+src->rpos);
	src->rpos += sizeof(char_create_info);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*通知客户端观察者的视角*/
int   pack_notice_watcher_map_info (packet**dst ,char const*wather_guid,uint32 map_id,uint32 instance_id)
{
	*dst = external_protocol_new_packet(SMSG_NOTICE_WATCHER_MAP_INFO);
	ASSERT((*dst)->head->optcode == SMSG_NOTICE_WATCHER_MAP_INFO);	
	packet_write_str(*dst,wather_guid);		
	packet_write(*dst,(char *)&map_id,sizeof(uint32));
	packet_write(*dst,(char *)&instance_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_notice_watcher_map_info (packet *src ,char **wather_guid,uint32 *map_id,uint32 *instance_id)
{	
	int ret=0;
	ret = packet_read_str(src,wather_guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(map_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(instance_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*客户端订阅对象信息*/
int   pack_modify_watch (packet**dst ,uint8 opt,uint32 cid,char const*key)
{
	*dst = external_protocol_new_packet(CMSG_MODIFY_WATCH);
	ASSERT((*dst)->head->optcode == CMSG_MODIFY_WATCH);	
	packet_write(*dst,(char *)&opt,sizeof(uint8));
	packet_write(*dst,(char *)&cid,sizeof(uint32));
	packet_write_str(*dst,key);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_modify_watch (packet *src ,uint8 *opt,uint32 *cid,char **key)
{	
	int ret=0;
	ret = packet_read(src,(char*)(opt),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(cid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,key);
	if(ret) return -1;
	return ret;
}
/*跨服传送*/
int   pack_kuafu_chuansong (packet**dst ,char const*str_data,char const*watcher_guid,uint32 reserve)
{
	*dst = external_protocol_new_packet(CMSG_KUAFU_CHUANSONG);
	ASSERT((*dst)->head->optcode == CMSG_KUAFU_CHUANSONG);	
	packet_write_str(*dst,str_data);		
	packet_write_str(*dst,watcher_guid);		
	packet_write(*dst,(char *)&reserve,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_chuansong (packet *src ,char **str_data,char **watcher_guid,uint32 *reserve)
{	
	int ret=0;
	ret = packet_read_str(src,str_data);
	if(ret) return -1;
	ret = packet_read_str(src,watcher_guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*显示当前装备*/
int   pack_show_suit (packet**dst ,uint8 position)
{
	*dst = external_protocol_new_packet(CMSG_SHOW_SUIT);
	ASSERT((*dst)->head->optcode == CMSG_SHOW_SUIT);	
	packet_write(*dst,(char *)&position,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_suit (packet *src ,uint8 *position)
{	
	int ret=0;
	ret = packet_read(src,(char*)(position),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*显示当前坐标*/
int   pack_show_position (packet**dst ,uint8 channel)
{
	*dst = external_protocol_new_packet(CMSG_SHOW_POSITION);
	ASSERT((*dst)->head->optcode == CMSG_SHOW_POSITION);	
	packet_write(*dst,(char *)&channel,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_position (packet *src ,uint8 *channel)
{	
	int ret=0;
	ret = packet_read(src,(char*)(channel),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*元宝复活*/
int   pack_gold_respawn (packet**dst ,uint8 useGold)
{
	*dst = external_protocol_new_packet(CMSG_GOLD_RESPAWN);
	ASSERT((*dst)->head->optcode == CMSG_GOLD_RESPAWN);	
	packet_write(*dst,(char *)&useGold,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_gold_respawn (packet *src ,uint8 *useGold)
{	
	int ret=0;
	ret = packet_read(src,(char*)(useGold),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*野外死亡倒计时*/
int   pack_field_death_cooldown (packet**dst ,uint8 type,uint32 value,char const*killername,uint16 cooldown,char const*params)
{
	*dst = external_protocol_new_packet(SMSG_FIELD_DEATH_COOLDOWN);
	ASSERT((*dst)->head->optcode == SMSG_FIELD_DEATH_COOLDOWN);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write(*dst,(char *)&value,sizeof(uint32));
	packet_write_str(*dst,killername);		
	packet_write(*dst,(char *)&cooldown,sizeof(uint16));
	packet_write_str(*dst,params);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_field_death_cooldown (packet *src ,uint8 *type,uint32 *value,char **killername,uint16 *cooldown,char **params)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(value),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,killername);
	if(ret) return -1;
	ret = packet_read(src,(char*)(cooldown),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read_str(src,params);
	if(ret) return -1;
	return ret;
}
/*商城购买*/
int   pack_mall_buy (packet**dst ,uint32 id,uint32 count,uint32 time)
{
	*dst = external_protocol_new_packet(CMSG_MALL_BUY);
	ASSERT((*dst)->head->optcode == CMSG_MALL_BUY);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
	packet_write(*dst,(char *)&count,sizeof(uint32));
	packet_write(*dst,(char *)&time,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_mall_buy (packet *src ,uint32 *id,uint32 *count,uint32 *time)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(time),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*强化*/
int   pack_strength (packet**dst ,uint8 part)
{
	*dst = external_protocol_new_packet(CMSG_STRENGTH);
	ASSERT((*dst)->head->optcode == CMSG_STRENGTH);	
	packet_write(*dst,(char *)&part,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_strength (packet *src ,uint8 *part)
{	
	int ret=0;
	ret = packet_read(src,(char*)(part),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*强化成功*/
int   pack_strength_success (packet**dst ,uint16 level)
{
	*dst = external_protocol_new_packet(SMSG_STRENGTH_SUCCESS);
	ASSERT((*dst)->head->optcode == SMSG_STRENGTH_SUCCESS);	
	packet_write(*dst,(char *)&level,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_strength_success (packet *src ,uint16 *level)
{	
	int ret=0;
	ret = packet_read(src,(char*)(level),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*强制进入*/
int   pack_forceInto (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_FORCEINTO);
	ASSERT((*dst)->head->optcode == CMSG_FORCEINTO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*创建帮派*/
int   pack_create_faction (packet**dst ,char const*name,uint8 icon)
{
	*dst = external_protocol_new_packet(CMSG_CREATE_FACTION);
	ASSERT((*dst)->head->optcode == CMSG_CREATE_FACTION);	
	packet_write_str(*dst,name);		
	packet_write(*dst,(char *)&icon,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_create_faction (packet *src ,char **name,uint8 *icon)
{	
	int ret=0;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read(src,(char*)(icon),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*升级帮派*/
int   pack_faction_upgrade (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_FACTION_UPGRADE);
	ASSERT((*dst)->head->optcode == CMSG_FACTION_UPGRADE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*申请加入帮派*/
int   pack_faction_join (packet**dst ,char const*id)
{
	*dst = external_protocol_new_packet(CMSG_FACTION_JOIN);
	ASSERT((*dst)->head->optcode == CMSG_FACTION_JOIN);	
	packet_write_str(*dst,id);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_faction_join (packet *src ,char **id)
{	
	int ret=0;
	ret = packet_read_str(src,id);
	if(ret) return -1;
	return ret;
}
/*申请升级技能*/
int   pack_raise_base_spell (packet**dst ,uint8 raiseType,uint16 spellId)
{
	*dst = external_protocol_new_packet(CMSG_RAISE_BASE_SPELL);
	ASSERT((*dst)->head->optcode == CMSG_RAISE_BASE_SPELL);	
	packet_write(*dst,(char *)&raiseType,sizeof(uint8));
	packet_write(*dst,(char *)&spellId,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_raise_base_spell (packet *src ,uint8 *raiseType,uint16 *spellId)
{	
	int ret=0;
	ret = packet_read(src,(char*)(raiseType),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(spellId),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*申请升阶愤怒技能*/
int   pack_upgrade_anger_spell (packet**dst ,uint16 spellId)
{
	*dst = external_protocol_new_packet(CMSG_UPGRADE_ANGER_SPELL);
	ASSERT((*dst)->head->optcode == CMSG_UPGRADE_ANGER_SPELL);	
	packet_write(*dst,(char *)&spellId,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_upgrade_anger_spell (packet *src ,uint16 *spellId)
{	
	int ret=0;
	ret = packet_read(src,(char*)(spellId),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*申请升级坐骑*/
int   pack_raise_mount (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_RAISE_MOUNT);
	ASSERT((*dst)->head->optcode == CMSG_RAISE_MOUNT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*申请升阶坐骑*/
int   pack_upgrade_mount (packet**dst ,uint8 useItem)
{
	*dst = external_protocol_new_packet(CMSG_UPGRADE_MOUNT);
	ASSERT((*dst)->head->optcode == CMSG_UPGRADE_MOUNT);	
	packet_write(*dst,(char *)&useItem,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_upgrade_mount (packet *src ,uint8 *useItem)
{	
	int ret=0;
	ret = packet_read(src,(char*)(useItem),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*申请一键升阶坐骑*/
int   pack_upgrade_mount_one_step (packet**dst ,uint8 useItem)
{
	*dst = external_protocol_new_packet(CMSG_UPGRADE_MOUNT_ONE_STEP);
	ASSERT((*dst)->head->optcode == CMSG_UPGRADE_MOUNT_ONE_STEP);	
	packet_write(*dst,(char *)&useItem,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_upgrade_mount_one_step (packet *src ,uint8 *useItem)
{	
	int ret=0;
	ret = packet_read(src,(char*)(useItem),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*申请解锁幻化坐骑*/
int   pack_illusion_mount_active (packet**dst ,uint16 illuId)
{
	*dst = external_protocol_new_packet(CMSG_ILLUSION_MOUNT_ACTIVE);
	ASSERT((*dst)->head->optcode == CMSG_ILLUSION_MOUNT_ACTIVE);	
	packet_write(*dst,(char *)&illuId,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_illusion_mount_active (packet *src ,uint16 *illuId)
{	
	int ret=0;
	ret = packet_read(src,(char*)(illuId),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*申请幻化坐骑*/
int   pack_illusion_mount (packet**dst ,uint16 illuId)
{
	*dst = external_protocol_new_packet(CMSG_ILLUSION_MOUNT);
	ASSERT((*dst)->head->optcode == CMSG_ILLUSION_MOUNT);	
	packet_write(*dst,(char *)&illuId,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_illusion_mount (packet *src ,uint16 *illuId)
{	
	int ret=0;
	ret = packet_read(src,(char*)(illuId),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*申请骑乘*/
int   pack_ride_mount (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_RIDE_MOUNT);
	ASSERT((*dst)->head->optcode == CMSG_RIDE_MOUNT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*grid中的unit跳跃*/
int   pack_grid_unit_jump (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_GRID_UNIT_JUMP);
	ASSERT((*dst)->head->optcode == SMSG_GRID_UNIT_JUMP);	
		
	update_packet_len(*dst);
	return 0;	
}
/*宝石*/
int   pack_gem (packet**dst ,uint8 part)
{
	*dst = external_protocol_new_packet(CMSG_GEM);
	ASSERT((*dst)->head->optcode == CMSG_GEM);	
	packet_write(*dst,(char *)&part,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_gem (packet *src ,uint8 *part)
{	
	int ret=0;
	ret = packet_read(src,(char*)(part),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*请求切换模式*/
int   pack_change_battle_mode (packet**dst ,uint8 mode)
{
	*dst = external_protocol_new_packet(CMSG_CHANGE_BATTLE_MODE);
	ASSERT((*dst)->head->optcode == CMSG_CHANGE_BATTLE_MODE);	
	packet_write(*dst,(char *)&mode,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_change_battle_mode (packet *src ,uint8 *mode)
{	
	int ret=0;
	ret = packet_read(src,(char*)(mode),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*和平模式CD*/
int   pack_peace_mode_cd (packet**dst ,uint8 mode)
{
	*dst = external_protocol_new_packet(SMSG_PEACE_MODE_CD);
	ASSERT((*dst)->head->optcode == SMSG_PEACE_MODE_CD);	
	packet_write(*dst,(char *)&mode,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_peace_mode_cd (packet *src ,uint8 *mode)
{	
	int ret=0;
	ret = packet_read(src,(char*)(mode),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*激活神兵*/
int   pack_divine_active (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_DIVINE_ACTIVE);
	ASSERT((*dst)->head->optcode == CMSG_DIVINE_ACTIVE);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_divine_active (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*激活神兵*/
int   pack_divine_uplev (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_DIVINE_UPLEV);
	ASSERT((*dst)->head->optcode == CMSG_DIVINE_UPLEV);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_divine_uplev (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*切换神兵*/
int   pack_divine_switch (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_DIVINE_SWITCH);
	ASSERT((*dst)->head->optcode == CMSG_DIVINE_SWITCH);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_divine_switch (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*请求跳跃*/
int   pack_jump_start (packet**dst ,uint16 pos_x,uint16 pos_y)
{
	*dst = external_protocol_new_packet(CMSG_JUMP_START);
	ASSERT((*dst)->head->optcode == CMSG_JUMP_START);	
	packet_write(*dst,(char *)&pos_x,sizeof(uint16));
	packet_write(*dst,(char *)&pos_y,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_jump_start (packet *src ,uint16 *pos_x,uint16 *pos_y)
{	
	int ret=0;
	ret = packet_read(src,(char*)(pos_x),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(pos_y),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*请求进入vip副本*/
int   pack_enter_vip_instance (packet**dst ,uint16 id,uint8 hard)
{
	*dst = external_protocol_new_packet(CMSG_ENTER_VIP_INSTANCE);
	ASSERT((*dst)->head->optcode == CMSG_ENTER_VIP_INSTANCE);	
	packet_write(*dst,(char *)&id,sizeof(uint16));
	packet_write(*dst,(char *)&hard,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_enter_vip_instance (packet *src ,uint16 *id,uint8 *hard)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(hard),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*请求扫荡vip副本*/
int   pack_sweep_vip_instance (packet**dst ,uint16 id)
{
	*dst = external_protocol_new_packet(CMSG_SWEEP_VIP_INSTANCE);
	ASSERT((*dst)->head->optcode == CMSG_SWEEP_VIP_INSTANCE);	
	packet_write(*dst,(char *)&id,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_sweep_vip_instance (packet *src ,uint16 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*进行挂机*/
int   pack_hang_up (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_HANG_UP);
	ASSERT((*dst)->head->optcode == CMSG_HANG_UP);	
		
	update_packet_len(*dst);
	return 0;	
}
/*进行挂机设置*/
int   pack_hang_up_setting (packet**dst ,uint32 value0,uint32 value1,uint32 value2,uint32 value3)
{
	*dst = external_protocol_new_packet(CMSG_HANG_UP_SETTING);
	ASSERT((*dst)->head->optcode == CMSG_HANG_UP_SETTING);	
	packet_write(*dst,(char *)&value0,sizeof(uint32));
	packet_write(*dst,(char *)&value1,sizeof(uint32));
	packet_write(*dst,(char *)&value2,sizeof(uint32));
	packet_write(*dst,(char *)&value3,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_hang_up_setting (packet *src ,uint32 *value0,uint32 *value1,uint32 *value2,uint32 *value3)
{	
	int ret=0;
	ret = packet_read(src,(char*)(value0),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(value1),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(value2),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(value3),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*请求进入试炼塔副本*/
int   pack_enter_trial_instance (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_ENTER_TRIAL_INSTANCE);
	ASSERT((*dst)->head->optcode == CMSG_ENTER_TRIAL_INSTANCE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*扫荡试炼塔副本*/
int   pack_sweep_trial_instance (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_SWEEP_TRIAL_INSTANCE);
	ASSERT((*dst)->head->optcode == CMSG_SWEEP_TRIAL_INSTANCE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*重置试炼塔*/
int   pack_reset_trial_instance (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_RESET_TRIAL_INSTANCE);
	ASSERT((*dst)->head->optcode == CMSG_RESET_TRIAL_INSTANCE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*扫荡副本奖励*/
int   pack_sweep_instance_reward (packet**dst ,uint8 inst_sub_type,uint8 data1,uint8 data2,uint8 data3, item_reward_info *list , uint16 len_5)
{
	*dst = external_protocol_new_packet(SMSG_SWEEP_INSTANCE_REWARD);
	ASSERT((*dst)->head->optcode == SMSG_SWEEP_INSTANCE_REWARD);	
	packet_write(*dst,(char *)&inst_sub_type,sizeof(uint8));
	packet_write(*dst,(char *)&data1,sizeof(uint8));
	packet_write(*dst,(char *)&data2,sizeof(uint8));
	packet_write(*dst,(char *)&data3,sizeof(uint8));
	packet_write(*dst,(char *)&len_5, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(item_reward_info) * len_5);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_sweep_instance_reward (packet *src ,uint8 *inst_sub_type,uint8 *data1,uint8 *data2,uint8 *data3, item_reward_info **list , uint16 *len_5)
{	
	int ret=0;
	ret = packet_read(src,(char*)(inst_sub_type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(data1),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(data2),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(data3),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)len_5,sizeof(uint16));
	if(ret) return -1;
	*list = (item_reward_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_reward_info) * (*len_5);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*重进副本*/
int   pack_reenter_instance (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_REENTER_INSTANCE);
	ASSERT((*dst)->head->optcode == CMSG_REENTER_INSTANCE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*走马灯信息*/
int   pack_merry_go_round (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_MERRY_GO_ROUND);
	ASSERT((*dst)->head->optcode == SMSG_MERRY_GO_ROUND);	
		
	update_packet_len(*dst);
	return 0;	
}
/*添加好友*/
int   pack_social_add_friend (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_ADD_FRIEND);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_ADD_FRIEND);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_add_friend (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*同意添加好友*/
int   pack_social_sureadd_friend (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_SUREADD_FRIEND);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_SUREADD_FRIEND);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_sureadd_friend (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*赠送礼物*/
int   pack_social_gift_friend (packet**dst ,char const*guid, item_reward_info *gift , uint16 len_2)
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_GIFT_FRIEND);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_GIFT_FRIEND);	
	packet_write_str(*dst,guid);		
	packet_write(*dst,(char *)&len_2, sizeof(uint16));
	packet_write(*dst,(char *)gift, sizeof(item_reward_info) * len_2);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_gift_friend (packet *src ,char **guid, item_reward_info **gift , uint16 *len_2)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)len_2,sizeof(uint16));
	if(ret) return -1;
	*gift = (item_reward_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_reward_info) * (*len_2);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*推荐好友列表*/
int   pack_social_recommend_friend (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_RECOMMEND_FRIEND);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_RECOMMEND_FRIEND);	
		
	update_packet_len(*dst);
	return 0;	
}
/*推荐好友列表*/
int   pack_social_get_recommend_friend (packet**dst , social_friend_info *list , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_SOCIAL_GET_RECOMMEND_FRIEND);
	ASSERT((*dst)->head->optcode == SMSG_SOCIAL_GET_RECOMMEND_FRIEND);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(social_friend_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_get_recommend_friend (packet *src , social_friend_info **list , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (social_friend_info *)(src->content+src->rpos);
	src->rpos += sizeof(social_friend_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*复仇*/
int   pack_social_revenge_enemy (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_REVENGE_ENEMY);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_REVENGE_ENEMY);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_revenge_enemy (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*删除好友*/
int   pack_social_del_friend (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_DEL_FRIEND);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_DEL_FRIEND);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_del_friend (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*回城*/
int   pack_teleport_main_city (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_TELEPORT_MAIN_CITY);
	ASSERT((*dst)->head->optcode == CMSG_TELEPORT_MAIN_CITY);	
		
	update_packet_len(*dst);
	return 0;	
}
/*不同频道聊天*/
int   pack_chat_by_channel (packet**dst ,uint8 channel,char const*content)
{
	*dst = external_protocol_new_packet(CMSG_CHAT_BY_CHANNEL);
	ASSERT((*dst)->head->optcode == CMSG_CHAT_BY_CHANNEL);	
	packet_write(*dst,(char *)&channel,sizeof(uint8));
	packet_write_str(*dst,content);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_chat_by_channel (packet *src ,uint8 *channel,char **content)
{	
	int ret=0;
	ret = packet_read(src,(char*)(channel),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	return ret;
}
/*发送聊天*/
int   pack_send_chat (packet**dst ,uint8 channel,char const*guid,uint8 title,char const*name,uint8 vip,uint8 zs,uint8 lvl,uint8 gender,char const*content,char const*to_guid,char const*faction_guid)
{
	*dst = external_protocol_new_packet(SMSG_SEND_CHAT);
	ASSERT((*dst)->head->optcode == SMSG_SEND_CHAT);	
	packet_write(*dst,(char *)&channel,sizeof(uint8));
	packet_write_str(*dst,guid);		
	packet_write(*dst,(char *)&title,sizeof(uint8));
	packet_write_str(*dst,name);		
	packet_write(*dst,(char *)&vip,sizeof(uint8));
	packet_write(*dst,(char *)&zs,sizeof(uint8));
	packet_write(*dst,(char *)&lvl,sizeof(uint8));
	packet_write(*dst,(char *)&gender,sizeof(uint8));
	packet_write_str(*dst,content);		
	packet_write_str(*dst,to_guid);		
	packet_write_str(*dst,faction_guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_send_chat (packet *src ,uint8 *channel,char **guid,uint8 *title,char **name,uint8 *vip,uint8 *zs,uint8 *lvl,uint8 *gender,char **content,char **to_guid,char **faction_guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(channel),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(title),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read(src,(char*)(vip),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(zs),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(lvl),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(gender),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,content);
	if(ret) return -1;
	ret = packet_read_str(src,to_guid);
	if(ret) return -1;
	ret = packet_read_str(src,faction_guid);
	if(ret) return -1;
	return ret;
}
/*清空申请列表*/
int   pack_social_clear_apply (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_CLEAR_APPLY);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_CLEAR_APPLY);	
		
	update_packet_len(*dst);
	return 0;	
}
/*设置拒绝接受消息*/
int   pack_msg_decline (packet**dst ,uint32 value0,uint32 value1)
{
	*dst = external_protocol_new_packet(CMSG_MSG_DECLINE);
	ASSERT((*dst)->head->optcode == CMSG_MSG_DECLINE);	
	packet_write(*dst,(char *)&value0,sizeof(uint32));
	packet_write(*dst,(char *)&value1,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_msg_decline (packet *src ,uint32 *value0,uint32 *value1)
{	
	int ret=0;
	ret = packet_read(src,(char*)(value0),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(value1),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*帮派列表*/
int   pack_faction_get_list_result (packet**dst , faction_info *list , uint16 len_1,uint8 curpag,uint8 maxpag)
{
	*dst = external_protocol_new_packet(SMSG_FACTION_GET_LIST_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_FACTION_GET_LIST_RESULT);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(faction_info) * len_1);
	packet_write(*dst,(char *)&curpag,sizeof(uint8));
	packet_write(*dst,(char *)&maxpag,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_faction_get_list_result (packet *src , faction_info **list , uint16 *len_1,uint8 *curpag,uint8 *maxpag)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (faction_info *)(src->content+src->rpos);
	src->rpos += sizeof(faction_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	ret = packet_read(src,(char*)(curpag),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(maxpag),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*获取帮派列表*/
int   pack_faction_getlist (packet**dst ,uint8 page,uint8 num,uint8 grep)
{
	*dst = external_protocol_new_packet(CMSG_FACTION_GETLIST);
	ASSERT((*dst)->head->optcode == CMSG_FACTION_GETLIST);	
	packet_write(*dst,(char *)&page,sizeof(uint8));
	packet_write(*dst,(char *)&num,sizeof(uint8));
	packet_write(*dst,(char *)&grep,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_faction_getlist (packet *src ,uint8 *page,uint8 *num,uint8 *grep)
{	
	int ret=0;
	ret = packet_read(src,(char*)(page),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(num),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(grep),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*帮派管理*/
int   pack_faction_manager (packet**dst ,uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2)
{
	*dst = external_protocol_new_packet(CMSG_FACTION_MANAGER);
	ASSERT((*dst)->head->optcode == CMSG_FACTION_MANAGER);	
	packet_write(*dst,(char *)&opt_type,sizeof(uint8));
	packet_write(*dst,(char *)&reserve_int1,sizeof(uint16));
	packet_write(*dst,(char *)&reserve_int2,sizeof(uint16));
	packet_write_str(*dst,reserve_str1);		
	packet_write_str(*dst,reserve_str2);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_faction_manager (packet *src ,uint8 *opt_type,uint16 *reserve_int1,uint16 *reserve_int2,char **reserve_str1,char **reserve_str2)
{	
	int ret=0;
	ret = packet_read(src,(char*)(opt_type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve_int1),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve_int2),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read_str(src,reserve_str1);
	if(ret) return -1;
	ret = packet_read_str(src,reserve_str2);
	if(ret) return -1;
	return ret;
}
/*帮派成员操作*/
int   pack_faction_member_operate (packet**dst ,uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2)
{
	*dst = external_protocol_new_packet(CMSG_FACTION_MEMBER_OPERATE);
	ASSERT((*dst)->head->optcode == CMSG_FACTION_MEMBER_OPERATE);	
	packet_write(*dst,(char *)&opt_type,sizeof(uint8));
	packet_write(*dst,(char *)&reserve_int1,sizeof(uint16));
	packet_write(*dst,(char *)&reserve_int2,sizeof(uint16));
	packet_write_str(*dst,reserve_str1);		
	packet_write_str(*dst,reserve_str2);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_faction_member_operate (packet *src ,uint8 *opt_type,uint16 *reserve_int1,uint16 *reserve_int2,char **reserve_str1,char **reserve_str2)
{	
	int ret=0;
	ret = packet_read(src,(char*)(opt_type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve_int1),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve_int2),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read_str(src,reserve_str1);
	if(ret) return -1;
	ret = packet_read_str(src,reserve_str2);
	if(ret) return -1;
	return ret;
}
/*快速加入帮派*/
int   pack_faction_fast_join (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_FACTION_FAST_JOIN);
	ASSERT((*dst)->head->optcode == CMSG_FACTION_FAST_JOIN);	
		
	update_packet_len(*dst);
	return 0;	
}
/*通过名字添加好友*/
int   pack_social_add_friend_byname (packet**dst ,char const*name)
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_ADD_FRIEND_BYNAME);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_ADD_FRIEND_BYNAME);	
	packet_write_str(*dst,name);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_add_friend_byname (packet *src ,char **name)
{	
	int ret=0;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	return ret;
}
/*读邮件*/
int   pack_read_mail (packet**dst ,uint16 indx)
{
	*dst = external_protocol_new_packet(CMSG_READ_MAIL);
	ASSERT((*dst)->head->optcode == CMSG_READ_MAIL);	
	packet_write(*dst,(char *)&indx,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_read_mail (packet *src ,uint16 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*领取邮件*/
int   pack_pick_mail (packet**dst ,uint16 indx)
{
	*dst = external_protocol_new_packet(CMSG_PICK_MAIL);
	ASSERT((*dst)->head->optcode == CMSG_PICK_MAIL);	
	packet_write(*dst,(char *)&indx,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_pick_mail (packet *src ,uint16 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*删除邮件*/
int   pack_remove_mail (packet**dst ,uint16 indx)
{
	*dst = external_protocol_new_packet(CMSG_REMOVE_MAIL);
	ASSERT((*dst)->head->optcode == CMSG_REMOVE_MAIL);	
	packet_write(*dst,(char *)&indx,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_remove_mail (packet *src ,uint16 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*一键领取邮件*/
int   pack_pick_mail_one_step (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_PICK_MAIL_ONE_STEP);
	ASSERT((*dst)->head->optcode == CMSG_PICK_MAIL_ONE_STEP);	
		
	update_packet_len(*dst);
	return 0;	
}
/*一键删除邮件*/
int   pack_remove_mail_one_step (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_REMOVE_MAIL_ONE_STEP);
	ASSERT((*dst)->head->optcode == CMSG_REMOVE_MAIL_ONE_STEP);	
		
	update_packet_len(*dst);
	return 0;	
}
/*屏蔽某人*/
int   pack_block_chat (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_BLOCK_CHAT);
	ASSERT((*dst)->head->optcode == CMSG_BLOCK_CHAT);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_block_chat (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*取消屏蔽*/
int   pack_cancel_block_chat (packet**dst ,uint8 indx)
{
	*dst = external_protocol_new_packet(CMSG_CANCEL_BLOCK_CHAT);
	ASSERT((*dst)->head->optcode == CMSG_CANCEL_BLOCK_CHAT);	
	packet_write(*dst,(char *)&indx,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_cancel_block_chat (packet *src ,uint8 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*使用需要广播的游戏对象*/
int   pack_use_broadcast_gameobject (packet**dst ,uint32 target)
{
	*dst = external_protocol_new_packet(CMSG_USE_BROADCAST_GAMEOBJECT);
	ASSERT((*dst)->head->optcode == CMSG_USE_BROADCAST_GAMEOBJECT);	
	packet_write(*dst,(char *)&target,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_use_broadcast_gameobject (packet *src ,uint32 *target)
{	
	int ret=0;
	ret = packet_read(src,(char*)(target),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*世界BOSS报名*/
int   pack_world_boss_enroll (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WORLD_BOSS_ENROLL);
	ASSERT((*dst)->head->optcode == CMSG_WORLD_BOSS_ENROLL);	
		
	update_packet_len(*dst);
	return 0;	
}
/*世界BOSS挑战*/
int   pack_world_boss_fight (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WORLD_BOSS_FIGHT);
	ASSERT((*dst)->head->optcode == CMSG_WORLD_BOSS_FIGHT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*换线*/
int   pack_change_line (packet**dst ,uint32 lineNo)
{
	*dst = external_protocol_new_packet(CMSG_CHANGE_LINE);
	ASSERT((*dst)->head->optcode == CMSG_CHANGE_LINE);	
	packet_write(*dst,(char *)&lineNo,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_change_line (packet *src ,uint32 *lineNo)
{	
	int ret=0;
	ret = packet_read(src,(char*)(lineNo),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*roll世界BOSS箱子*/
int   pack_roll_world_boss_treasure (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_ROLL_WORLD_BOSS_TREASURE);
	ASSERT((*dst)->head->optcode == CMSG_ROLL_WORLD_BOSS_TREASURE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*roll点结果*/
int   pack_roll_result (packet**dst ,uint8 point,char const*name,uint8 isHighest,uint32 timestamp,uint8 rollid)
{
	*dst = external_protocol_new_packet(SMSG_ROLL_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_ROLL_RESULT);	
	packet_write(*dst,(char *)&point,sizeof(uint8));
	packet_write_str(*dst,name);		
	packet_write(*dst,(char *)&isHighest,sizeof(uint8));
	packet_write(*dst,(char *)&timestamp,sizeof(uint32));
	packet_write(*dst,(char *)&rollid,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_roll_result (packet *src ,uint8 *point,char **name,uint8 *isHighest,uint32 *timestamp,uint8 *rollid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(point),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read(src,(char*)(isHighest),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(timestamp),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(rollid),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*当前BOSS伤害排名*/
int   pack_boss_rank (packet**dst ,uint8 rankType, rank_info *rankList , uint16 len_2,uint8 mine)
{
	*dst = external_protocol_new_packet(SMSG_BOSS_RANK);
	ASSERT((*dst)->head->optcode == SMSG_BOSS_RANK);	
	packet_write(*dst,(char *)&rankType,sizeof(uint8));
	packet_write(*dst,(char *)&len_2, sizeof(uint16));
	packet_write(*dst,(char *)rankList, sizeof(rank_info) * len_2);
	packet_write(*dst,(char *)&mine,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_boss_rank (packet *src ,uint8 *rankType, rank_info **rankList , uint16 *len_2,uint8 *mine)
{	
	int ret=0;
	ret = packet_read(src,(char*)(rankType),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)len_2,sizeof(uint16));
	if(ret) return -1;
	*rankList = (rank_info *)(src->content+src->rpos);
	src->rpos += sizeof(rank_info) * (*len_2);
	if(src->rpos > src->wpos) return -1;
	ret = packet_read(src,(char*)(mine),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*排行榜点赞*/
int   pack_rank_add_like (packet**dst ,uint8 type,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_RANK_ADD_LIKE);
	ASSERT((*dst)->head->optcode == CMSG_RANK_ADD_LIKE);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_rank_add_like (packet *src ,uint8 *type,char **guid)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*排行榜点赞结果*/
int   pack_rank_add_like_result (packet**dst ,uint8 type,char const*guid,uint32 num)
{
	*dst = external_protocol_new_packet(SMSG_RANK_ADD_LIKE_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_RANK_ADD_LIKE_RESULT);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write_str(*dst,guid);		
	packet_write(*dst,(char *)&num,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_rank_add_like_result (packet *src ,uint8 *type,char **guid,uint32 *num)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(num),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*进入资源副本*/
int   pack_res_instance_enter (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_RES_INSTANCE_ENTER);
	ASSERT((*dst)->head->optcode == CMSG_RES_INSTANCE_ENTER);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_res_instance_enter (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*扫荡资源副本*/
int   pack_res_instance_sweep (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_RES_INSTANCE_SWEEP);
	ASSERT((*dst)->head->optcode == CMSG_RES_INSTANCE_SWEEP);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_res_instance_sweep (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*查看本地图的分线号*/
int   pack_show_map_line (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_SHOW_MAP_LINE);
	ASSERT((*dst)->head->optcode == CMSG_SHOW_MAP_LINE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回本地图的分线号信息*/
int   pack_send_map_line (packet**dst , line_info *info , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_SEND_MAP_LINE);
	ASSERT((*dst)->head->optcode == SMSG_SEND_MAP_LINE);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)info, sizeof(line_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_send_map_line (packet *src , line_info **info , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*info = (line_info *)(src->content+src->rpos);
	src->rpos += sizeof(line_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*获得奖励提示*/
int   pack_item_notice (packet**dst ,uint8 showType, item_reward_info *list , uint16 len_2)
{
	*dst = external_protocol_new_packet(SMSG_ITEM_NOTICE);
	ASSERT((*dst)->head->optcode == SMSG_ITEM_NOTICE);	
	packet_write(*dst,(char *)&showType,sizeof(uint8));
	packet_write(*dst,(char *)&len_2, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(item_reward_info) * len_2);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_item_notice (packet *src ,uint8 *showType, item_reward_info **list , uint16 *len_2)
{	
	int ret=0;
	ret = packet_read(src,(char*)(showType),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)len_2,sizeof(uint16));
	if(ret) return -1;
	*list = (item_reward_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_reward_info) * (*len_2);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*传送到某个世界地图*/
int   pack_teleport_map (packet**dst ,uint32 mapid,uint32 lineNo)
{
	*dst = external_protocol_new_packet(CMSG_TELEPORT_MAP);
	ASSERT((*dst)->head->optcode == CMSG_TELEPORT_MAP);	
	packet_write(*dst,(char *)&mapid,sizeof(uint32));
	packet_write(*dst,(char *)&lineNo,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_teleport_map (packet *src ,uint32 *mapid,uint32 *lineNo)
{	
	int ret=0;
	ret = packet_read(src,(char*)(mapid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(lineNo),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*传送到野外boss旁边*/
int   pack_teleport_field_boss (packet**dst ,uint32 mapid,uint32 lineNo)
{
	*dst = external_protocol_new_packet(CMSG_TELEPORT_FIELD_BOSS);
	ASSERT((*dst)->head->optcode == CMSG_TELEPORT_FIELD_BOSS);	
	packet_write(*dst,(char *)&mapid,sizeof(uint32));
	packet_write(*dst,(char *)&lineNo,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_teleport_field_boss (packet *src ,uint32 *mapid,uint32 *lineNo)
{	
	int ret=0;
	ret = packet_read(src,(char*)(mapid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(lineNo),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*活跃度奖励*/
int   pack_get_activity_reward (packet**dst ,uint8 id,uint8 vip)
{
	*dst = external_protocol_new_packet(CMSG_GET_ACTIVITY_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_GET_ACTIVITY_REWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
	packet_write(*dst,(char *)&vip,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_get_activity_reward (packet *src ,uint8 *id,uint8 *vip)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(vip),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*成就奖励*/
int   pack_get_achieve_reward (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_GET_ACHIEVE_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_GET_ACHIEVE_REWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_get_achieve_reward (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*总成就奖励*/
int   pack_get_achieve_all_reward (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_GET_ACHIEVE_ALL_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_GET_ACHIEVE_ALL_REWARD);	
		
	update_packet_len(*dst);
	return 0;	
}
/*装备称号*/
int   pack_set_title (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_SET_TITLE);
	ASSERT((*dst)->head->optcode == CMSG_SET_TITLE);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_set_title (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*初始化称号*/
int   pack_init_title (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_INIT_TITLE);
	ASSERT((*dst)->head->optcode == CMSG_INIT_TITLE);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_init_title (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*领取首充奖励*/
int   pack_welfare_shouchong_reward (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_SHOUCHONG_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_SHOUCHONG_REWARD);	
		
	update_packet_len(*dst);
	return 0;	
}
/*每日签到奖励*/
int   pack_welfare_checkin (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_CHECKIN);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_CHECKIN);	
		
	update_packet_len(*dst);
	return 0;	
}
/*累积签到奖励*/
int   pack_welfare_checkin_all (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_CHECKIN_ALL);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_CHECKIN_ALL);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_checkin_all (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*补签奖励*/
int   pack_welfare_checkin_getback (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_CHECKIN_GETBACK);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_CHECKIN_GETBACK);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_checkin_getback (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*等级奖励*/
int   pack_welfare_level (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_LEVEL);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_LEVEL);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_level (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*活动找回奖励*/
int   pack_welfare_active_getback (packet**dst ,uint8 id,uint8 best,uint16 num)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_ACTIVE_GETBACK);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_ACTIVE_GETBACK);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
	packet_write(*dst,(char *)&best,sizeof(uint8));
	packet_write(*dst,(char *)&num,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_active_getback (packet *src ,uint8 *id,uint8 *best,uint16 *num)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(best),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(num),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*领取任务奖励*/
int   pack_pick_quest_reward (packet**dst ,uint8 indx)
{
	*dst = external_protocol_new_packet(CMSG_PICK_QUEST_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_PICK_QUEST_REWARD);	
	packet_write(*dst,(char *)&indx,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_pick_quest_reward (packet *src ,uint8 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*和npc对话*/
int   pack_talk_with_npc (packet**dst ,uint16 entry,uint16 questId)
{
	*dst = external_protocol_new_packet(CMSG_TALK_WITH_NPC);
	ASSERT((*dst)->head->optcode == CMSG_TALK_WITH_NPC);	
	packet_write(*dst,(char *)&entry,sizeof(uint16));
	packet_write(*dst,(char *)&questId,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_talk_with_npc (packet *src ,uint16 *entry,uint16 *questId)
{	
	int ret=0;
	ret = packet_read(src,(char*)(entry),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(questId),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*使用虚拟物品*/
int   pack_use_virtual_item (packet**dst ,uint16 entry)
{
	*dst = external_protocol_new_packet(CMSG_USE_VIRTUAL_ITEM);
	ASSERT((*dst)->head->optcode == CMSG_USE_VIRTUAL_ITEM);	
	packet_write(*dst,(char *)&entry,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_use_virtual_item (packet *src ,uint16 *entry)
{	
	int ret=0;
	ret = packet_read(src,(char*)(entry),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*领取任务章节奖励*/
int   pack_pick_quest_chapter_reward (packet**dst ,uint8 indx)
{
	*dst = external_protocol_new_packet(CMSG_PICK_QUEST_CHAPTER_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_PICK_QUEST_CHAPTER_REWARD);	
	packet_write(*dst,(char *)&indx,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_pick_quest_chapter_reward (packet *src ,uint8 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*3v3跨服匹配*/
int   pack_kuafu_3v3_match (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_KUAFU_3V3_MATCH);
	ASSERT((*dst)->head->optcode == CMSG_KUAFU_3V3_MATCH);	
		
	update_packet_len(*dst);
	return 0;	
}
/*跨服开始匹配*/
int   pack_kuafu_match_start (packet**dst ,uint8 indx)
{
	*dst = external_protocol_new_packet(SMSG_KUAFU_MATCH_START);
	ASSERT((*dst)->head->optcode == SMSG_KUAFU_MATCH_START);	
	packet_write(*dst,(char *)&indx,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_match_start (packet *src ,uint8 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*3v3购买次数*/
int   pack_kuafu_3v3_buytimes (packet**dst ,uint8 num)
{
	*dst = external_protocol_new_packet(CMSG_KUAFU_3V3_BUYTIMES);
	ASSERT((*dst)->head->optcode == CMSG_KUAFU_3V3_BUYTIMES);	
	packet_write(*dst,(char *)&num,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_buytimes (packet *src ,uint8 *num)
{	
	int ret=0;
	ret = packet_read(src,(char*)(num),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*3v3每日活跃奖励*/
int   pack_kuafu_3v3_dayreward (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_KUAFU_3V3_DAYREWARD);
	ASSERT((*dst)->head->optcode == CMSG_KUAFU_3V3_DAYREWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_dayreward (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*请求3v3排行榜*/
int   pack_kuafu_3v3_getranlist (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_KUAFU_3V3_GETRANLIST);
	ASSERT((*dst)->head->optcode == CMSG_KUAFU_3V3_GETRANLIST);	
		
	update_packet_len(*dst);
	return 0;	
}
/*3v3排行榜结果列表*/
int   pack_kuafu_3v3_ranlist (packet**dst ,char const*list)
{
	*dst = external_protocol_new_packet(SMSG_KUAFU_3V3_RANLIST);
	ASSERT((*dst)->head->optcode == SMSG_KUAFU_3V3_RANLIST);	
	packet_write_str(*dst,list);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_ranlist (packet *src ,char **list)
{	
	int ret=0;
	ret = packet_read_str(src,list);
	if(ret) return -1;
	return ret;
}
/*福利所有奖励列表*/
int   pack_welfare_getalllist_getback (packet**dst ,uint8 best)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_GETALLLIST_GETBACK);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_GETALLLIST_GETBACK);	
	packet_write(*dst,(char *)&best,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_getalllist_getback (packet *src ,uint8 *best)
{	
	int ret=0;
	ret = packet_read(src,(char*)(best),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*奖励列表*/
int   pack_welfare_rewardlist_getback (packet**dst ,char const*list,char const*cost)
{
	*dst = external_protocol_new_packet(SMSG_WELFARE_REWARDLIST_GETBACK);
	ASSERT((*dst)->head->optcode == SMSG_WELFARE_REWARDLIST_GETBACK);	
	packet_write_str(*dst,list);		
	packet_write_str(*dst,cost);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_rewardlist_getback (packet *src ,char **list,char **cost)
{	
	int ret=0;
	ret = packet_read_str(src,list);
	if(ret) return -1;
	ret = packet_read_str(src,cost);
	if(ret) return -1;
	return ret;
}
/*一键领取所有福利*/
int   pack_welfare_getall_getback (packet**dst ,uint8 best)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_GETALL_GETBACK);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_GETALL_GETBACK);	
	packet_write(*dst,(char *)&best,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_getall_getback (packet *src ,uint8 *best)
{	
	int ret=0;
	ret = packet_read(src,(char*)(best),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*请求3v3排行榜自己的名次*/
int   pack_kuafu_3v3_getmyrank (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_KUAFU_3V3_GETMYRANK);
	ASSERT((*dst)->head->optcode == CMSG_KUAFU_3V3_GETMYRANK);	
		
	update_packet_len(*dst);
	return 0;	
}
/*3v3排行榜自己的名次结果*/
int   pack_kuafu_3v3_myrank (packet**dst ,uint8 rank)
{
	*dst = external_protocol_new_packet(SMSG_KUAFU_3V3_MYRANK);
	ASSERT((*dst)->head->optcode == SMSG_KUAFU_3V3_MYRANK);	
	packet_write(*dst,(char *)&rank,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_myrank (packet *src ,uint8 *rank)
{	
	int ret=0;
	ret = packet_read(src,(char*)(rank),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*击杀数据*/
int   pack_kuafu_3v3_kill_detail (packet**dst ,uint32 indx1,uint32 indx2)
{
	*dst = external_protocol_new_packet(SMSG_KUAFU_3V3_KILL_DETAIL);
	ASSERT((*dst)->head->optcode == SMSG_KUAFU_3V3_KILL_DETAIL);	
	packet_write(*dst,(char *)&indx1,sizeof(uint32));
	packet_write(*dst,(char *)&indx2,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_kill_detail (packet *src ,uint32 *indx1,uint32 *indx2)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx1),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(indx2),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*跨服匹配等待数据*/
int   pack_kuafu_3v3_wait_info (packet**dst , wait_info *list , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_KUAFU_3V3_WAIT_INFO);
	ASSERT((*dst)->head->optcode == SMSG_KUAFU_3V3_WAIT_INFO);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(wait_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_wait_info (packet *src , wait_info **list , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (wait_info *)(src->content+src->rpos);
	src->rpos += sizeof(wait_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*取消匹配*/
int   pack_kuafu_3v3_cancel_match (packet**dst ,uint32 type)
{
	*dst = external_protocol_new_packet(MSG_KUAFU_3V3_CANCEL_MATCH);
	ASSERT((*dst)->head->optcode == MSG_KUAFU_3V3_CANCEL_MATCH);	
	packet_write(*dst,(char *)&type,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_cancel_match (packet *src ,uint32 *type)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*匹配到人&接受或者拒绝*/
int   pack_kuafu_3v3_match_oper (packet**dst ,uint32 oper)
{
	*dst = external_protocol_new_packet(CMSG_KUAFU_3V3_MATCH_OPER);
	ASSERT((*dst)->head->optcode == CMSG_KUAFU_3V3_MATCH_OPER);	
	packet_write(*dst,(char *)&oper,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_match_oper (packet *src ,uint32 *oper)
{	
	int ret=0;
	ret = packet_read(src,(char*)(oper),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*拒绝比赛*/
int   pack_kuafu_3v3_decline_match (packet**dst ,uint32 type)
{
	*dst = external_protocol_new_packet(SMSG_KUAFU_3V3_DECLINE_MATCH);
	ASSERT((*dst)->head->optcode == SMSG_KUAFU_3V3_DECLINE_MATCH);	
	packet_write(*dst,(char *)&type,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_3v3_decline_match (packet *src ,uint32 *type)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*仙府夺宝跨服匹配*/
int   pack_kuafu_xianfu_match (packet**dst ,uint8 indx)
{
	*dst = external_protocol_new_packet(CMSG_KUAFU_XIANFU_MATCH);
	ASSERT((*dst)->head->optcode == CMSG_KUAFU_XIANFU_MATCH);	
	packet_write(*dst,(char *)&indx,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_xianfu_match (packet *src ,uint8 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*单方跨服匹配等待*/
int   pack_kuafu_match_wait (packet**dst ,uint8 type,uint8 target,uint8 count,uint32 data,char const*params)
{
	*dst = external_protocol_new_packet(SMSG_KUAFU_MATCH_WAIT);
	ASSERT((*dst)->head->optcode == SMSG_KUAFU_MATCH_WAIT);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write(*dst,(char *)&target,sizeof(uint8));
	packet_write(*dst,(char *)&count,sizeof(uint8));
	packet_write(*dst,(char *)&data,sizeof(uint32));
	packet_write_str(*dst,params);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_kuafu_match_wait (packet *src ,uint8 *type,uint8 *target,uint8 *count,uint32 *data,char **params)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(target),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(data),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read_str(src,params);
	if(ret) return -1;
	return ret;
}
/*仙府夺宝小地图信息*/
int   pack_kuafu_xianfu_minimap_info (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_KUAFU_XIANFU_MINIMAP_INFO);
	ASSERT((*dst)->head->optcode == SMSG_KUAFU_XIANFU_MINIMAP_INFO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*购买仙府进入券*/
int   pack_buy_xianfu_item (packet**dst ,uint8 type,uint8 indx,uint16 count)
{
	*dst = external_protocol_new_packet(CMSG_BUY_XIANFU_ITEM);
	ASSERT((*dst)->head->optcode == CMSG_BUY_XIANFU_ITEM);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write(*dst,(char *)&indx,sizeof(uint8));
	packet_write(*dst,(char *)&count,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_buy_xianfu_item (packet *src ,uint8 *type,uint8 *indx,uint16 *count)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*随机复活*/
int   pack_xianfu_random_respawn (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_XIANFU_RANDOM_RESPAWN);
	ASSERT((*dst)->head->optcode == CMSG_XIANFU_RANDOM_RESPAWN);	
		
	update_packet_len(*dst);
	return 0;	
}
/*斗剑台挑战*/
int   pack_doujiantai_fight (packet**dst ,uint16 rank)
{
	*dst = external_protocol_new_packet(CMSG_DOUJIANTAI_FIGHT);
	ASSERT((*dst)->head->optcode == CMSG_DOUJIANTAI_FIGHT);	
	packet_write(*dst,(char *)&rank,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_doujiantai_fight (packet *src ,uint16 *rank)
{	
	int ret=0;
	ret = packet_read(src,(char*)(rank),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*斗剑台购买次数*/
int   pack_doujiantai_buytime (packet**dst ,uint8 num)
{
	*dst = external_protocol_new_packet(CMSG_DOUJIANTAI_BUYTIME);
	ASSERT((*dst)->head->optcode == CMSG_DOUJIANTAI_BUYTIME);	
	packet_write(*dst,(char *)&num,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_doujiantai_buytime (packet *src ,uint8 *num)
{	
	int ret=0;
	ret = packet_read(src,(char*)(num),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*斗剑台清理CD*/
int   pack_doujiantai_clearcd (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_DOUJIANTAI_CLEARCD);
	ASSERT((*dst)->head->optcode == CMSG_DOUJIANTAI_CLEARCD);	
		
	update_packet_len(*dst);
	return 0;	
}
/*斗剑台首胜奖励*/
int   pack_doujiantai_first_reward (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_DOUJIANTAI_FIRST_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_DOUJIANTAI_FIRST_REWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_doujiantai_first_reward (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*斗剑台挑战对手信息*/
int   pack_doujiantai_get_enemys_info (packet**dst )
{
	*dst = external_protocol_new_packet(MSG_DOUJIANTAI_GET_ENEMYS_INFO);
	ASSERT((*dst)->head->optcode == MSG_DOUJIANTAI_GET_ENEMYS_INFO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*斗剑台排行榜*/
int   pack_doujiantai_get_rank (packet**dst ,uint16 startIdx,uint16 endIdx)
{
	*dst = external_protocol_new_packet(CMSG_DOUJIANTAI_GET_RANK);
	ASSERT((*dst)->head->optcode == CMSG_DOUJIANTAI_GET_RANK);	
	packet_write(*dst,(char *)&startIdx,sizeof(uint16));
	packet_write(*dst,(char *)&endIdx,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_doujiantai_get_rank (packet *src ,uint16 *startIdx,uint16 *endIdx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(startIdx),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(endIdx),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*斗剑台刷新对手*/
int   pack_doujiantai_refresh_enemys (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_DOUJIANTAI_REFRESH_ENEMYS);
	ASSERT((*dst)->head->optcode == CMSG_DOUJIANTAI_REFRESH_ENEMYS);	
		
	update_packet_len(*dst);
	return 0;	
}
/*斗剑台三甲*/
int   pack_doujiantai_top3 (packet**dst )
{
	*dst = external_protocol_new_packet(MSG_DOUJIANTAI_TOP3);
	ASSERT((*dst)->head->optcode == MSG_DOUJIANTAI_TOP3);	
		
	update_packet_len(*dst);
	return 0;	
}
/*使用跳点*/
int   pack_use_jump_point (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(MSG_USE_JUMP_POINT);
	ASSERT((*dst)->head->optcode == MSG_USE_JUMP_POINT);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_use_jump_point (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*出售物品*/
int   pack_bag_item_sell (packet**dst ,char const*item_guid,uint32 count)
{
	*dst = external_protocol_new_packet(CMSG_BAG_ITEM_SELL);
	ASSERT((*dst)->head->optcode == CMSG_BAG_ITEM_SELL);	
	packet_write_str(*dst,item_guid);		
	packet_write(*dst,(char *)&count,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_bag_item_sell (packet *src ,char **item_guid,uint32 *count)
{	
	int ret=0;
	ret = packet_read_str(src,item_guid);
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*整理物品*/
int   pack_bag_item_sort (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_BAG_ITEM_SORT);
	ASSERT((*dst)->head->optcode == CMSG_BAG_ITEM_SORT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*提交日常任务*/
int   pack_submit_quest_daily2 (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_SUBMIT_QUEST_DAILY2);
	ASSERT((*dst)->head->optcode == CMSG_SUBMIT_QUEST_DAILY2);	
		
	update_packet_len(*dst);
	return 0;	
}
/*属性改变*/
int   pack_attribute_changed (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_ATTRIBUTE_CHANGED);
	ASSERT((*dst)->head->optcode == SMSG_ATTRIBUTE_CHANGED);	
		
	update_packet_len(*dst);
	return 0;	
}
/*背包有更强装备*/
int   pack_bag_find_equip_better (packet**dst ,uint32 item_id,uint32 pos)
{
	*dst = external_protocol_new_packet(SMSG_BAG_FIND_EQUIP_BETTER);
	ASSERT((*dst)->head->optcode == SMSG_BAG_FIND_EQUIP_BETTER);	
	packet_write(*dst,(char *)&item_id,sizeof(uint32));
	packet_write(*dst,(char *)&pos,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_bag_find_equip_better (packet *src ,uint32 *item_id,uint32 *pos)
{	
	int ret=0;
	ret = packet_read(src,(char*)(item_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(pos),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*模块解锁*/
int   pack_module_active (packet**dst ,uint32 moduleId)
{
	*dst = external_protocol_new_packet(SMSG_MODULE_ACTIVE);
	ASSERT((*dst)->head->optcode == SMSG_MODULE_ACTIVE);	
	packet_write(*dst,(char *)&moduleId,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_module_active (packet *src ,uint32 *moduleId)
{	
	int ret=0;
	ret = packet_read(src,(char*)(moduleId),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*领取日常任务奖励*/
int   pack_pick_daily2_quest_reward (packet**dst ,uint8 indx)
{
	*dst = external_protocol_new_packet(CMSG_PICK_DAILY2_QUEST_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_PICK_DAILY2_QUEST_REWARD);	
	packet_write(*dst,(char *)&indx,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_pick_daily2_quest_reward (packet *src ,uint8 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*完成当前引导*/
int   pack_finish_now_guide (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_FINISH_NOW_GUIDE);
	ASSERT((*dst)->head->optcode == CMSG_FINISH_NOW_GUIDE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*取得修炼场信息*/
int   pack_get_cultivation_info (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_GET_CULTIVATION_INFO);
	ASSERT((*dst)->head->optcode == CMSG_GET_CULTIVATION_INFO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回修炼场信息*/
int   pack_update_cultivation_info (packet**dst ,uint32 start_time,uint32 lost)
{
	*dst = external_protocol_new_packet(SMSG_UPDATE_CULTIVATION_INFO);
	ASSERT((*dst)->head->optcode == SMSG_UPDATE_CULTIVATION_INFO);	
	packet_write(*dst,(char *)&start_time,sizeof(uint32));
	packet_write(*dst,(char *)&lost,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_update_cultivation_info (packet *src ,uint32 *start_time,uint32 *lost)
{	
	int ret=0;
	ret = packet_read(src,(char*)(start_time),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(lost),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*取得当前所有修炼场对手信息*/
int   pack_get_cultivation_rivals_info (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_GET_CULTIVATION_RIVALS_INFO);
	ASSERT((*dst)->head->optcode == CMSG_GET_CULTIVATION_RIVALS_INFO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回修炼场对手信息*/
int   pack_update_cultivation_rivals_info_list (packet**dst , cultivation_rivals_info *list , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_UPDATE_CULTIVATION_RIVALS_INFO_LIST);
	ASSERT((*dst)->head->optcode == SMSG_UPDATE_CULTIVATION_RIVALS_INFO_LIST);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(cultivation_rivals_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_update_cultivation_rivals_info_list (packet *src , cultivation_rivals_info **list , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (cultivation_rivals_info *)(src->content+src->rpos);
	src->rpos += sizeof(cultivation_rivals_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*领取修炼场奖励*/
int   pack_get_cultivation_reward (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_GET_CULTIVATION_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_GET_CULTIVATION_REWARD);	
		
	update_packet_len(*dst);
	return 0;	
}
/*刷新修炼场对手*/
int   pack_refresh_cultivation_rivals (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_REFRESH_CULTIVATION_RIVALS);
	ASSERT((*dst)->head->optcode == CMSG_REFRESH_CULTIVATION_RIVALS);	
		
	update_packet_len(*dst);
	return 0;	
}
/*掠夺修炼场对手*/
int   pack_plunder_cultivation_rival (packet**dst ,uint32 index)
{
	*dst = external_protocol_new_packet(CMSG_PLUNDER_CULTIVATION_RIVAL);
	ASSERT((*dst)->head->optcode == CMSG_PLUNDER_CULTIVATION_RIVAL);	
	packet_write(*dst,(char *)&index,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_plunder_cultivation_rival (packet *src ,uint32 *index)
{	
	int ret=0;
	ret = packet_read(src,(char*)(index),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*反击复仇修炼场对手*/
int   pack_revenge_cultivation_rival (packet**dst ,uint32 index)
{
	*dst = external_protocol_new_packet(CMSG_REVENGE_CULTIVATION_RIVAL);
	ASSERT((*dst)->head->optcode == CMSG_REVENGE_CULTIVATION_RIVAL);	
	packet_write(*dst,(char *)&index,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_revenge_cultivation_rival (packet *src ,uint32 *index)
{	
	int ret=0;
	ret = packet_read(src,(char*)(index),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*增加修炼场剩余挑战次数*/
int   pack_buy_cultivation_left_plunder_count (packet**dst ,uint32 count)
{
	*dst = external_protocol_new_packet(CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT);
	ASSERT((*dst)->head->optcode == CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT);	
	packet_write(*dst,(char *)&count,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_buy_cultivation_left_plunder_count (packet *src ,uint32 *count)
{	
	int ret=0;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*返回修炼场战斗结果*/
int   pack_show_cultivation_result_list (packet**dst ,int32 result,char const*name, item_reward_info *list , uint16 len_3)
{
	*dst = external_protocol_new_packet(SMSG_SHOW_CULTIVATION_RESULT_LIST);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_CULTIVATION_RESULT_LIST);	
	packet_write(*dst,(char *)&result,sizeof(int32));
	packet_write_str(*dst,name);		
	packet_write(*dst,(char *)&len_3, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(item_reward_info) * len_3);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_cultivation_result_list (packet *src ,int32 *result,char **name, item_reward_info **list , uint16 *len_3)
{	
	int ret=0;
	ret = packet_read(src,(char*)(result),sizeof(int32));
	if(ret) return -1;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read(src,(char*)len_3,sizeof(uint16));
	if(ret) return -1;
	*list = (item_reward_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_reward_info) * (*len_3);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*领取登录大礼奖励*/
int   pack_get_login_activity_reward (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(CMSG_GET_LOGIN_ACTIVITY_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_GET_LOGIN_ACTIVITY_REWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_get_login_activity_reward (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*通知客户端释放蓄力技能*/
int   pack_cast_spell_start (packet**dst ,uint32 caster_guid,uint32 target_guid,uint16 spellid,char const*data)
{
	*dst = external_protocol_new_packet(SMSG_CAST_SPELL_START);
	ASSERT((*dst)->head->optcode == SMSG_CAST_SPELL_START);	
	packet_write(*dst,(char *)&caster_guid,sizeof(uint32));
	packet_write(*dst,(char *)&target_guid,sizeof(uint32));
	packet_write(*dst,(char *)&spellid,sizeof(uint16));
	packet_write_str(*dst,data);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_cast_spell_start (packet *src ,uint32 *caster_guid,uint32 *target_guid,uint16 *spellid,char **data)
{	
	int ret=0;
	ret = packet_read(src,(char*)(caster_guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(target_guid),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(spellid),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read_str(src,data);
	if(ret) return -1;
	return ret;
}
/*完成非强制引导的步骤*/
int   pack_finish_optional_guide_step (packet**dst ,uint32 guide_id,uint8 step)
{
	*dst = external_protocol_new_packet(CMSG_FINISH_OPTIONAL_GUIDE_STEP);
	ASSERT((*dst)->head->optcode == CMSG_FINISH_OPTIONAL_GUIDE_STEP);	
	packet_write(*dst,(char *)&guide_id,sizeof(uint32));
	packet_write(*dst,(char *)&step,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_finish_optional_guide_step (packet *src ,uint32 *guide_id,uint8 *step)
{	
	int ret=0;
	ret = packet_read(src,(char*)(guide_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(step),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*执行接到任务以后的命令*/
int   pack_execute_quest_cmd_after_accepted (packet**dst ,uint16 indx)
{
	*dst = external_protocol_new_packet(CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED);
	ASSERT((*dst)->head->optcode == CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED);	
	packet_write(*dst,(char *)&indx,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_execute_quest_cmd_after_accepted (packet *src ,uint16 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*通知客户端显示属性*/
int   pack_show_unit_attribute (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_SHOW_UNIT_ATTRIBUTE);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_UNIT_ATTRIBUTE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回家族*/
int   pack_back_to_famity (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_BACK_TO_FAMITY);
	ASSERT((*dst)->head->optcode == CMSG_BACK_TO_FAMITY);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回家族boss结果*/
int   pack_faction_boss_send_result (packet**dst ,uint32 result,uint32 boss_id,uint32 money)
{
	*dst = external_protocol_new_packet(SMSG_FACTION_BOSS_SEND_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_FACTION_BOSS_SEND_RESULT);	
	packet_write(*dst,(char *)&result,sizeof(uint32));
	packet_write(*dst,(char *)&boss_id,sizeof(uint32));
	packet_write(*dst,(char *)&money,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_faction_boss_send_result (packet *src ,uint32 *result,uint32 *boss_id,uint32 *money)
{	
	int ret=0;
	ret = packet_read(src,(char*)(result),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(boss_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(money),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*挑战boss*/
int   pack_challange_boss (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_CHALLANGE_BOSS);
	ASSERT((*dst)->head->optcode == CMSG_CHALLANGE_BOSS);	
		
	update_packet_len(*dst);
	return 0;	
}
/*领取离线奖励*/
int   pack_pick_offline_reward (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_PICK_OFFLINE_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_PICK_OFFLINE_REWARD);	
		
	update_packet_len(*dst);
	return 0;	
}
/*离线奖励结果*/
int   pack_offline_reward_result (packet**dst ,uint32 reserve,uint32 reserve2,uint32 reserve3,uint32 reserve4, item_reward_info *list , uint16 len_5)
{
	*dst = external_protocol_new_packet(SMSG_OFFLINE_REWARD_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_OFFLINE_REWARD_RESULT);	
	packet_write(*dst,(char *)&reserve,sizeof(uint32));
	packet_write(*dst,(char *)&reserve2,sizeof(uint32));
	packet_write(*dst,(char *)&reserve3,sizeof(uint32));
	packet_write(*dst,(char *)&reserve4,sizeof(uint32));
	packet_write(*dst,(char *)&len_5, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(item_reward_info) * len_5);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_offline_reward_result (packet *src ,uint32 *reserve,uint32 *reserve2,uint32 *reserve3,uint32 *reserve4, item_reward_info **list , uint16 *len_5)
{	
	int ret=0;
	ret = packet_read(src,(char*)(reserve),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve2),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve3),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve4),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)len_5,sizeof(uint16));
	if(ret) return -1;
	*list = (item_reward_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_reward_info) * (*len_5);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*熔炼装备*/
int   pack_smelting_equip (packet**dst ,char const*pos_str)
{
	*dst = external_protocol_new_packet(CMSG_SMELTING_EQUIP);
	ASSERT((*dst)->head->optcode == CMSG_SMELTING_EQUIP);	
	packet_write_str(*dst,pos_str);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_smelting_equip (packet *src ,char **pos_str)
{	
	int ret=0;
	ret = packet_read_str(src,pos_str);
	if(ret) return -1;
	return ret;
}
/*上交装备*/
int   pack_storehouse_hand_in (packet**dst ,char const*pos_str)
{
	*dst = external_protocol_new_packet(CMSG_STOREHOUSE_HAND_IN);
	ASSERT((*dst)->head->optcode == CMSG_STOREHOUSE_HAND_IN);	
	packet_write_str(*dst,pos_str);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_storehouse_hand_in (packet *src ,char **pos_str)
{	
	int ret=0;
	ret = packet_read_str(src,pos_str);
	if(ret) return -1;
	return ret;
}
/*兑换装备*/
int   pack_storehouse_exchange (packet**dst ,uint32 pos)
{
	*dst = external_protocol_new_packet(CMSG_STOREHOUSE_EXCHANGE);
	ASSERT((*dst)->head->optcode == CMSG_STOREHOUSE_EXCHANGE);	
	packet_write(*dst,(char *)&pos,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_storehouse_exchange (packet *src ,uint32 *pos)
{	
	int ret=0;
	ret = packet_read(src,(char*)(pos),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*销毁装备*/
int   pack_storehouse_destroy (packet**dst ,uint32 pos)
{
	*dst = external_protocol_new_packet(CMSG_STOREHOUSE_DESTROY);
	ASSERT((*dst)->head->optcode == CMSG_STOREHOUSE_DESTROY);	
	packet_write(*dst,(char *)&pos,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_storehouse_destroy (packet *src ,uint32 *pos)
{	
	int ret=0;
	ret = packet_read(src,(char*)(pos),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*赠送礼物*/
int   pack_send_faction_gift (packet**dst , item_reward_info *list , uint16 len_1,char const*msg,uint32 msg_type)
{
	*dst = external_protocol_new_packet(CMSG_SEND_FACTION_GIFT);
	ASSERT((*dst)->head->optcode == CMSG_SEND_FACTION_GIFT);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(item_reward_info) * len_1);
	packet_write_str(*dst,msg);		
	packet_write(*dst,(char *)&msg_type,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_send_faction_gift (packet *src , item_reward_info **list , uint16 *len_1,char **msg,uint32 *msg_type)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (item_reward_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_reward_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	ret = packet_read_str(src,msg);
	if(ret) return -1;
	ret = packet_read(src,(char*)(msg_type),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*领取额外奖励*/
int   pack_get_faction_gift_exreward (packet**dst ,uint32 count_id)
{
	*dst = external_protocol_new_packet(CMSG_GET_FACTION_GIFT_EXREWARD);
	ASSERT((*dst)->head->optcode == CMSG_GET_FACTION_GIFT_EXREWARD);	
	packet_write(*dst,(char *)&count_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_get_faction_gift_exreward (packet *src ,uint32 *count_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(count_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*领取所有额外奖励*/
int   pack_get_all_faction_gift_exreward (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_GET_ALL_FACTION_GIFT_EXREWARD);
	ASSERT((*dst)->head->optcode == CMSG_GET_ALL_FACTION_GIFT_EXREWARD);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回礼物列表*/
int   pack_show_faction_gift_page (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_GIFT_PAGE);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_GIFT_PAGE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回礼物信息*/
int   pack_show_faction_gift_info (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_GIFT_INFO);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_GIFT_INFO);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回女王未感谢礼物*/
int   pack_show_faction_gift_unthank_page (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_GIFT_UNTHANK_PAGE);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_GIFT_UNTHANK_PAGE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*返回女王历史记录*/
int   pack_show_faction_gift_history_page (packet**dst )
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_GIFT_HISTORY_PAGE);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_GIFT_HISTORY_PAGE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*请求家族魅力排行*/
int   pack_get_faction_gift_rank_page (packet**dst ,uint32 page)
{
	*dst = external_protocol_new_packet(CMSG_GET_FACTION_GIFT_RANK_PAGE);
	ASSERT((*dst)->head->optcode == CMSG_GET_FACTION_GIFT_RANK_PAGE);	
	packet_write(*dst,(char *)&page,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_get_faction_gift_rank_page (packet *src ,uint32 *page)
{	
	int ret=0;
	ret = packet_read(src,(char*)(page),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*返回家族魅力排行*/
int   pack_show_faction_gift_rank_result_list (packet**dst , faction_gift_rank_info *list , uint16 len_1, faction_gift_rank_info *info,uint32 page)
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_GIFT_RANK_RESULT_LIST);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_GIFT_RANK_RESULT_LIST);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(faction_gift_rank_info) * len_1);
	packet_write(*dst,(char*)info,sizeof(faction_gift_rank_info));
	packet_write(*dst,(char *)&page,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_faction_gift_rank_result_list (packet *src , faction_gift_rank_info **list , uint16 *len_1, faction_gift_rank_info **info,uint32 *page)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (faction_gift_rank_info *)(src->content+src->rpos);
	src->rpos += sizeof(faction_gift_rank_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	*info = (faction_gift_rank_info*)(src->content+src->rpos);
	src->rpos += sizeof(faction_gift_rank_info);
	if(src->rpos > src->wpos) return -1;
	ret = packet_read(src,(char*)(page),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*返回家族魅力排行变化*/
int   pack_show_faction_gift_rank_change (packet**dst ,uint32 old_rank,uint32 new_rank, faction_gift_rank_info *info)
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_GIFT_RANK_CHANGE);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_GIFT_RANK_CHANGE);	
	packet_write(*dst,(char *)&old_rank,sizeof(uint32));
	packet_write(*dst,(char *)&new_rank,sizeof(uint32));
	packet_write(*dst,(char*)info,sizeof(faction_gift_rank_info));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_faction_gift_rank_change (packet *src ,uint32 *old_rank,uint32 *new_rank, faction_gift_rank_info **info)
{	
	int ret=0;
	ret = packet_read(src,(char*)(old_rank),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(new_rank),sizeof(uint32));
	if(ret) return -1;
	*info = (faction_gift_rank_info*)(src->content+src->rpos);
	src->rpos += sizeof(faction_gift_rank_info);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*返回本家族魅力排行*/
int   pack_show_faction_gift_rank_info (packet**dst , faction_gift_rank_info *info)
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_GIFT_RANK_INFO);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_GIFT_RANK_INFO);	
	packet_write(*dst,(char*)info,sizeof(faction_gift_rank_info));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_faction_gift_rank_info (packet *src , faction_gift_rank_info **info)
{	
	int ret=0;
	*info = (faction_gift_rank_info*)(src->content+src->rpos);
	src->rpos += sizeof(faction_gift_rank_info);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*神兵强化*/
int   pack_divine_forge (packet**dst ,uint32 id,uint32 count)
{
	*dst = external_protocol_new_packet(CMSG_DIVINE_FORGE);
	ASSERT((*dst)->head->optcode == CMSG_DIVINE_FORGE);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
	packet_write(*dst,(char *)&count,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_divine_forge (packet *src ,uint32 *id,uint32 *count)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*神兵升阶*/
int   pack_divine_advance (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(CMSG_DIVINE_ADVANCE);
	ASSERT((*dst)->head->optcode == CMSG_DIVINE_ADVANCE);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_divine_advance (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*神兵铸魂*/
int   pack_divine_spirit (packet**dst ,uint32 id,uint32 protect,uint32 improve)
{
	*dst = external_protocol_new_packet(CMSG_DIVINE_SPIRIT);
	ASSERT((*dst)->head->optcode == CMSG_DIVINE_SPIRIT);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
	packet_write(*dst,(char *)&protect,sizeof(uint32));
	packet_write(*dst,(char *)&improve,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_divine_spirit (packet *src ,uint32 *id,uint32 *protect,uint32 *improve)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(protect),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(improve),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*查询全民boss信息*/
int   pack_query_mass_boss_info (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_QUERY_MASS_BOSS_INFO);
	ASSERT((*dst)->head->optcode == CMSG_QUERY_MASS_BOSS_INFO);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_query_mass_boss_info (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*全民boss信息结果*/
int   pack_mass_boss_info_ret (packet**dst ,uint32 count,uint8 percent)
{
	*dst = external_protocol_new_packet(SMSG_MASS_BOSS_INFO_RET);
	ASSERT((*dst)->head->optcode == SMSG_MASS_BOSS_INFO_RET);	
	packet_write(*dst,(char *)&count,sizeof(uint32));
	packet_write(*dst,(char *)&percent,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_mass_boss_info_ret (packet *src ,uint32 *count,uint8 *percent)
{	
	int ret=0;
	ret = packet_read(src,(char*)(count),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(percent),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*查询全民boss排行榜*/
int   pack_query_mass_boss_rank (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_QUERY_MASS_BOSS_RANK);
	ASSERT((*dst)->head->optcode == CMSG_QUERY_MASS_BOSS_RANK);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_query_mass_boss_rank (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*全民boss排行结果*/
int   pack_mass_boss_rank_result (packet**dst , mass_boss_rank_info *info , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_MASS_BOSS_RANK_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_MASS_BOSS_RANK_RESULT);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)info, sizeof(mass_boss_rank_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_mass_boss_rank_result (packet *src , mass_boss_rank_info **info , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*info = (mass_boss_rank_info *)(src->content+src->rpos);
	src->rpos += sizeof(mass_boss_rank_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*挑战全民boss*/
int   pack_try_mass_boss (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_TRY_MASS_BOSS);
	ASSERT((*dst)->head->optcode == CMSG_TRY_MASS_BOSS);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_try_mass_boss (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*购买挑战全民boss次数*/
int   pack_buy_mass_boss_times (packet**dst ,uint8 cnt)
{
	*dst = external_protocol_new_packet(CMSG_BUY_MASS_BOSS_TIMES);
	ASSERT((*dst)->head->optcode == CMSG_BUY_MASS_BOSS_TIMES);	
	packet_write(*dst,(char *)&cnt,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_buy_mass_boss_times (packet *src ,uint8 *cnt)
{	
	int ret=0;
	ret = packet_read(src,(char*)(cnt),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*组队副本跨服匹配*/
int   pack_group_instance_match (packet**dst ,uint8 indx)
{
	*dst = external_protocol_new_packet(CMSG_GROUP_INSTANCE_MATCH);
	ASSERT((*dst)->head->optcode == CMSG_GROUP_INSTANCE_MATCH);	
	packet_write(*dst,(char *)&indx,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_group_instance_match (packet *src ,uint8 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*组队副本跨服次数购买*/
int   pack_buy_group_instance_times (packet**dst ,uint8 count)
{
	*dst = external_protocol_new_packet(CMSG_BUY_GROUP_INSTANCE_TIMES);
	ASSERT((*dst)->head->optcode == CMSG_BUY_GROUP_INSTANCE_TIMES);	
	packet_write(*dst,(char *)&count,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_buy_group_instance_times (packet *src ,uint8 *count)
{	
	int ret=0;
	ret = packet_read(src,(char*)(count),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*法宝激活*/
int   pack_talisman_active (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(CMSG_TALISMAN_ACTIVE);
	ASSERT((*dst)->head->optcode == CMSG_TALISMAN_ACTIVE);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_talisman_active (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*法宝注灵*/
int   pack_talisman_lvup (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(CMSG_TALISMAN_LVUP);
	ASSERT((*dst)->head->optcode == CMSG_TALISMAN_LVUP);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_talisman_lvup (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*神羽激活*/
int   pack_wings_active (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WINGS_ACTIVE);
	ASSERT((*dst)->head->optcode == CMSG_WINGS_ACTIVE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*神羽祝福*/
int   pack_wings_bless (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WINGS_BLESS);
	ASSERT((*dst)->head->optcode == CMSG_WINGS_BLESS);	
		
	update_packet_len(*dst);
	return 0;	
}
/*神羽升阶*/
int   pack_wings_rankup (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WINGS_RANKUP);
	ASSERT((*dst)->head->optcode == CMSG_WINGS_RANKUP);	
		
	update_packet_len(*dst);
	return 0;	
}
/*神羽强化*/
int   pack_wings_strength (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_WINGS_STRENGTH);
	ASSERT((*dst)->head->optcode == CMSG_WINGS_STRENGTH);	
		
	update_packet_len(*dst);
	return 0;	
}
/*经脉修炼*/
int   pack_meridian_practise (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_MERIDIAN_PRACTISE);
	ASSERT((*dst)->head->optcode == CMSG_MERIDIAN_PRACTISE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*加经脉修炼经验值*/
int   pack_add_meridian_exp (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_ADD_MERIDIAN_EXP);
	ASSERT((*dst)->head->optcode == CMSG_ADD_MERIDIAN_EXP);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_add_meridian_exp (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*提升坐骑等级*/
int   pack_raise_mount_level_base (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_RAISE_MOUNT_LEVEL_BASE);
	ASSERT((*dst)->head->optcode == CMSG_RAISE_MOUNT_LEVEL_BASE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*解锁坐骑*/
int   pack_active_mount (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_ACTIVE_MOUNT);
	ASSERT((*dst)->head->optcode == CMSG_ACTIVE_MOUNT);	
		
	update_packet_len(*dst);
	return 0;	
}
/*家族领主挑战输出排行*/
int   pack_show_faction_bossdefense_damage_list (packet**dst , mass_boss_rank_info *list , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_BOSSDEFENSE_DAMAGE_LIST);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_BOSSDEFENSE_DAMAGE_LIST);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(mass_boss_rank_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_faction_bossdefense_damage_list (packet *src , mass_boss_rank_info **list , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (mass_boss_rank_info *)(src->content+src->rpos);
	src->rpos += sizeof(mass_boss_rank_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*家族无尽远征扫荡结果*/
int   pack_show_faction_tower_sweep_list (packet**dst , item_reward_info *list , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_TOWER_SWEEP_LIST);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_TOWER_SWEEP_LIST);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(item_reward_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_faction_tower_sweep_list (packet *src , item_reward_info **list , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (item_reward_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_reward_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*副本结果*/
int   pack_send_instance_result (packet**dst ,uint8 state,uint8 cd, item_reward_info *list , uint16 len_3,uint8 type,char const*data)
{
	*dst = external_protocol_new_packet(SMSG_SEND_INSTANCE_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_SEND_INSTANCE_RESULT);	
	packet_write(*dst,(char *)&state,sizeof(uint8));
	packet_write(*dst,(char *)&cd,sizeof(uint8));
	packet_write(*dst,(char *)&len_3, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(item_reward_info) * len_3);
	packet_write(*dst,(char *)&type,sizeof(uint8));
	packet_write_str(*dst,data);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_send_instance_result (packet *src ,uint8 *state,uint8 *cd, item_reward_info **list , uint16 *len_3,uint8 *type,char **data)
{	
	int ret=0;
	ret = packet_read(src,(char*)(state),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(cd),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)len_3,sizeof(uint16));
	if(ret) return -1;
	*list = (item_reward_info *)(src->content+src->rpos);
	src->rpos += sizeof(item_reward_info) * (*len_3);
	if(src->rpos > src->wpos) return -1;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read_str(src,data);
	if(ret) return -1;
	return ret;
}
/*匹配单人pvp*/
int   pack_match_single_pvp (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_MATCH_SINGLE_PVP);
	ASSERT((*dst)->head->optcode == CMSG_MATCH_SINGLE_PVP);	
		
	update_packet_len(*dst);
	return 0;	
}
/*购买单人pvp次数*/
int   pack_buy_match_single_pvp_times (packet**dst ,uint8 cnt)
{
	*dst = external_protocol_new_packet(CMSG_BUY_MATCH_SINGLE_PVP_TIMES);
	ASSERT((*dst)->head->optcode == CMSG_BUY_MATCH_SINGLE_PVP_TIMES);	
	packet_write(*dst,(char *)&cnt,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_buy_match_single_pvp_times (packet *src ,uint8 *cnt)
{	
	int ret=0;
	ret = packet_read(src,(char*)(cnt),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*领取单人pvp额外奖励*/
int   pack_pick_match_single_pvp_extra_reward (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_pick_match_single_pvp_extra_reward (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*装备养成操作*/
int   pack_equipdevelop_operate (packet**dst ,uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2)
{
	*dst = external_protocol_new_packet(CMSG_EQUIPDEVELOP_OPERATE);
	ASSERT((*dst)->head->optcode == CMSG_EQUIPDEVELOP_OPERATE);	
	packet_write(*dst,(char *)&opt_type,sizeof(uint8));
	packet_write(*dst,(char *)&reserve_int1,sizeof(uint16));
	packet_write(*dst,(char *)&reserve_int2,sizeof(uint16));
	packet_write_str(*dst,reserve_str1);		
	packet_write_str(*dst,reserve_str2);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_equipdevelop_operate (packet *src ,uint8 *opt_type,uint16 *reserve_int1,uint16 *reserve_int2,char **reserve_str1,char **reserve_str2)
{	
	int ret=0;
	ret = packet_read(src,(char*)(opt_type),sizeof(uint8));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve_int1),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read(src,(char*)(reserve_int2),sizeof(uint16));
	if(ret) return -1;
	ret = packet_read_str(src,reserve_str1);
	if(ret) return -1;
	ret = packet_read_str(src,reserve_str2);
	if(ret) return -1;
	return ret;
}
/*激活外观*/
int   pack_active_appearance (packet**dst ,uint16 id)
{
	*dst = external_protocol_new_packet(CMSG_ACTIVE_APPEARANCE);
	ASSERT((*dst)->head->optcode == CMSG_ACTIVE_APPEARANCE);	
	packet_write(*dst,(char *)&id,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_active_appearance (packet *src ,uint16 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*装备外观*/
int   pack_equip_appearance (packet**dst ,uint16 id)
{
	*dst = external_protocol_new_packet(CMSG_EQUIP_APPEARANCE);
	ASSERT((*dst)->head->optcode == CMSG_EQUIP_APPEARANCE);	
	packet_write(*dst,(char *)&id,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_equip_appearance (packet *src ,uint16 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint16));
	if(ret) return -1;
	return ret;
}
/*取消装备外观*/
int   pack_cancel_equip_appearance (packet**dst ,uint8 type)
{
	*dst = external_protocol_new_packet(CMSG_CANCEL_EQUIP_APPEARANCE);
	ASSERT((*dst)->head->optcode == CMSG_CANCEL_EQUIP_APPEARANCE);	
	packet_write(*dst,(char *)&type,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_cancel_equip_appearance (packet *src ,uint8 *type)
{	
	int ret=0;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*改名*/
int   pack_rename (packet**dst ,char const*name)
{
	*dst = external_protocol_new_packet(CMSG_RENAME);
	ASSERT((*dst)->head->optcode == CMSG_RENAME);	
	packet_write_str(*dst,name);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_rename (packet *src ,char **name)
{	
	int ret=0;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	return ret;
}
/*道具解锁称号*/
int   pack_unlock_title (packet**dst ,uint8 indx)
{
	*dst = external_protocol_new_packet(CMSG_UNLOCK_TITLE);
	ASSERT((*dst)->head->optcode == CMSG_UNLOCK_TITLE);	
	packet_write(*dst,(char *)&indx,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_unlock_title (packet *src ,uint8 *indx)
{	
	int ret=0;
	ret = packet_read(src,(char*)(indx),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*购买复仇次数*/
int   pack_social_buy_revenge_times (packet**dst ,uint8 count)
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_BUY_REVENGE_TIMES);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_BUY_REVENGE_TIMES);	
	packet_write(*dst,(char *)&count,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_buy_revenge_times (packet *src ,uint8 *count)
{	
	int ret=0;
	ret = packet_read(src,(char*)(count),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*请求进入世界冒险副本*/
int   pack_enter_risk_instance (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_ENTER_RISK_INSTANCE);
	ASSERT((*dst)->head->optcode == CMSG_ENTER_RISK_INSTANCE);	
		
	update_packet_len(*dst);
	return 0;	
}
/*删除仇人*/
int   pack_social_remove_enemy (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_SOCIAL_REMOVE_ENEMY);
	ASSERT((*dst)->head->optcode == CMSG_SOCIAL_REMOVE_ENEMY);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_social_remove_enemy (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*查看玩家详情*/
int   pack_get_player_overview (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_GET_PLAYER_OVERVIEW);
	ASSERT((*dst)->head->optcode == CMSG_GET_PLAYER_OVERVIEW);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_get_player_overview (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*返回玩家详情*/
int   pack_show_player_overview (packet**dst ,char const*guid,char const*name,uint32 force,uint32 vip,uint32 title,uint32 gender,uint32 coat,uint32 weapon)
{
	*dst = external_protocol_new_packet(SMSG_SHOW_PLAYER_OVERVIEW);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_PLAYER_OVERVIEW);	
	packet_write_str(*dst,guid);		
	packet_write_str(*dst,name);		
	packet_write(*dst,(char *)&force,sizeof(uint32));
	packet_write(*dst,(char *)&vip,sizeof(uint32));
	packet_write(*dst,(char *)&title,sizeof(uint32));
	packet_write(*dst,(char *)&gender,sizeof(uint32));
	packet_write(*dst,(char *)&coat,sizeof(uint32));
	packet_write(*dst,(char *)&weapon,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_player_overview (packet *src ,char **guid,char **name,uint32 *force,uint32 *vip,uint32 *title,uint32 *gender,uint32 *coat,uint32 *weapon)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read(src,(char*)(force),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(vip),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(title),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(gender),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(coat),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(weapon),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*邀请加入帮派*/
int   pack_send_faction_invite (packet**dst ,char const*guid)
{
	*dst = external_protocol_new_packet(CMSG_SEND_FACTION_INVITE);
	ASSERT((*dst)->head->optcode == CMSG_SEND_FACTION_INVITE);	
	packet_write_str(*dst,guid);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_send_faction_invite (packet *src ,char **guid)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	return ret;
}
/*显示邀请*/
int   pack_show_faction_invite (packet**dst ,char const*guid,char const*name,char const*faction_guid,char const*faction_name)
{
	*dst = external_protocol_new_packet(SMSG_SHOW_FACTION_INVITE);
	ASSERT((*dst)->head->optcode == SMSG_SHOW_FACTION_INVITE);	
	packet_write_str(*dst,guid);		
	packet_write_str(*dst,name);		
	packet_write_str(*dst,faction_guid);		
	packet_write_str(*dst,faction_name);		
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_show_faction_invite (packet *src ,char **guid,char **name,char **faction_guid,char **faction_name)
{	
	int ret=0;
	ret = packet_read_str(src,guid);
	if(ret) return -1;
	ret = packet_read_str(src,name);
	if(ret) return -1;
	ret = packet_read_str(src,faction_guid);
	if(ret) return -1;
	ret = packet_read_str(src,faction_name);
	if(ret) return -1;
	return ret;
}
/*购买vip礼包*/
int   pack_buy_vipgift (packet**dst ,uint32 id)
{
	*dst = external_protocol_new_packet(CMSG_BUY_VIPGIFT);
	ASSERT((*dst)->head->optcode == CMSG_BUY_VIPGIFT);	
	packet_write(*dst,(char *)&id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_buy_vipgift (packet *src ,uint32 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*购买活动每日礼包*/
int   pack_activity_opt_buy_dailygift (packet**dst ,uint32 act_id,uint32 index)
{
	*dst = external_protocol_new_packet(CMSG_ACTIVITY_OPT_BUY_DAILYGIFT);
	ASSERT((*dst)->head->optcode == CMSG_ACTIVITY_OPT_BUY_DAILYGIFT);	
	packet_write(*dst,(char *)&act_id,sizeof(uint32));
	packet_write(*dst,(char *)&index,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_activity_opt_buy_dailygift (packet *src ,uint32 *act_id,uint32 *index)
{	
	int ret=0;
	ret = packet_read(src,(char*)(act_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(index),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*抽奖*/
int   pack_draw_lottery (packet**dst ,uint32 actId,uint8 type)
{
	*dst = external_protocol_new_packet(CMSG_DRAW_LOTTERY);
	ASSERT((*dst)->head->optcode == CMSG_DRAW_LOTTERY);	
	packet_write(*dst,(char *)&actId,sizeof(uint32));
	packet_write(*dst,(char *)&type,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_draw_lottery (packet *src ,uint32 *actId,uint8 *type)
{	
	int ret=0;
	ret = packet_read(src,(char*)(actId),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(type),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*获取活动开服排行进度奖励*/
int   pack_activity_opt_get_rank_process_reward (packet**dst ,uint32 act_id,uint32 index)
{
	*dst = external_protocol_new_packet(CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD);	
	packet_write(*dst,(char *)&act_id,sizeof(uint32));
	packet_write(*dst,(char *)&index,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_activity_opt_get_rank_process_reward (packet *src ,uint32 *act_id,uint32 *index)
{	
	int ret=0;
	ret = packet_read(src,(char*)(act_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(index),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*获取活动开服排行榜*/
int   pack_activity_opt_get_rank_list (packet**dst ,uint32 act_id)
{
	*dst = external_protocol_new_packet(CMSG_ACTIVITY_OPT_GET_RANK_LIST);
	ASSERT((*dst)->head->optcode == CMSG_ACTIVITY_OPT_GET_RANK_LIST);	
	packet_write(*dst,(char *)&act_id,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_activity_opt_get_rank_list (packet *src ,uint32 *act_id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(act_id),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*返回活动开服排行榜*/
int   pack_activity_opt_show_rank_list (packet**dst ,uint32 act_id, act_rank_info *list , uint16 len_2)
{
	*dst = external_protocol_new_packet(SMSG_ACTIVITY_OPT_SHOW_RANK_LIST);
	ASSERT((*dst)->head->optcode == SMSG_ACTIVITY_OPT_SHOW_RANK_LIST);	
	packet_write(*dst,(char *)&act_id,sizeof(uint32));
	packet_write(*dst,(char *)&len_2, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(act_rank_info) * len_2);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_activity_opt_show_rank_list (packet *src ,uint32 *act_id, act_rank_info **list , uint16 *len_2)
{	
	int ret=0;
	ret = packet_read(src,(char*)(act_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)len_2,sizeof(uint16));
	if(ret) return -1;
	*list = (act_rank_info *)(src->content+src->rpos);
	src->rpos += sizeof(act_rank_info) * (*len_2);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*购买活动限定礼包*/
int   pack_activity_opt_buy_limitgift (packet**dst ,uint32 act_id,uint32 index)
{
	*dst = external_protocol_new_packet(CMSG_ACTIVITY_OPT_BUY_LIMITGIFT);
	ASSERT((*dst)->head->optcode == CMSG_ACTIVITY_OPT_BUY_LIMITGIFT);	
	packet_write(*dst,(char *)&act_id,sizeof(uint32));
	packet_write(*dst,(char *)&index,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_activity_opt_buy_limitgift (packet *src ,uint32 *act_id,uint32 *index)
{	
	int ret=0;
	ret = packet_read(src,(char*)(act_id),sizeof(uint32));
	if(ret) return -1;
	ret = packet_read(src,(char*)(index),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*领取累计充值奖励*/
int   pack_welfare_get_recharge_reward (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_GET_RECHARGE_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_GET_RECHARGE_REWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_get_recharge_reward (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*领取累计消费奖励*/
int   pack_welfare_get_consume_reward (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_GET_CONSUME_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_GET_CONSUME_REWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_get_consume_reward (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*领取七日大礼奖励*/
int   pack_welfare_get_sevenday_reward (packet**dst ,uint8 id)
{
	*dst = external_protocol_new_packet(CMSG_WELFARE_GET_SEVENDAY_REWARD);
	ASSERT((*dst)->head->optcode == CMSG_WELFARE_GET_SEVENDAY_REWARD);	
	packet_write(*dst,(char *)&id,sizeof(uint8));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_welfare_get_sevenday_reward (packet *src ,uint8 *id)
{	
	int ret=0;
	ret = packet_read(src,(char*)(id),sizeof(uint8));
	if(ret) return -1;
	return ret;
}
/*服务器开服时间*/
int   pack_send_server_open_time (packet**dst ,uint32 open_time)
{
	*dst = external_protocol_new_packet(SMSG_SEND_SERVER_OPEN_TIME);
	ASSERT((*dst)->head->optcode == SMSG_SEND_SERVER_OPEN_TIME);	
	packet_write(*dst,(char *)&open_time,sizeof(uint32));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_send_server_open_time (packet *src ,uint32 *open_time)
{	
	int ret=0;
	ret = packet_read(src,(char*)(open_time),sizeof(uint32));
	if(ret) return -1;
	return ret;
}
/*请求世界冒险排行榜*/
int   pack_risk_get_rank (packet**dst )
{
	*dst = external_protocol_new_packet(CMSG_RISK_GET_RANK);
	ASSERT((*dst)->head->optcode == CMSG_RISK_GET_RANK);	
		
	update_packet_len(*dst);
	return 0;	
}
/*世界冒险排行榜信息 */
int   pack_risk_get_rank_result (packet**dst , act_rank_info *list , uint16 len_1)
{
	*dst = external_protocol_new_packet(SMSG_RISK_GET_RANK_RESULT);
	ASSERT((*dst)->head->optcode == SMSG_RISK_GET_RANK_RESULT);	
	packet_write(*dst,(char *)&len_1, sizeof(uint16));
	packet_write(*dst,(char *)list, sizeof(act_rank_info) * len_1);
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_risk_get_rank_result (packet *src , act_rank_info **list , uint16 *len_1)
{	
	int ret=0;
	ret = packet_read(src,(char*)len_1,sizeof(uint16));
	if(ret) return -1;
	*list = (act_rank_info *)(src->content+src->rpos);
	src->rpos += sizeof(act_rank_info) * (*len_1);
	if(src->rpos > src->wpos) return -1;
	return ret;
}
/*设置朝向*/
int   pack_set_orient (packet**dst ,uint16 angle)
{
	*dst = external_protocol_new_packet(CMSG_SET_ORIENT);
	ASSERT((*dst)->head->optcode == CMSG_SET_ORIENT);	
	packet_write(*dst,(char *)&angle,sizeof(uint16));
		
	update_packet_len(*dst);
	return 0;	
}
int   unpack_set_orient (packet *src ,uint16 *angle)
{	
	int ret=0;
	ret = packet_read(src,(char*)(angle),sizeof(uint16));
	if(ret) return -1;
	return ret;
}

TEA_SVRCORE_API	int pack_rand_send_msg( packet**dst ,const char* message )
{
	int type = irand(MSG_NULL_ACTION,NUM_MSG_TYPES);
	//int type = irand(MSG_NULL_ACTION,CMSG_ITEM_UPGRADE-1);
	*dst = external_protocol_new_packet(type);
	ASSERT((*dst)->head->optcode == type);	
	packet_write_str(*dst,message);		
	update_packet_len(*dst);
	return type;	
}