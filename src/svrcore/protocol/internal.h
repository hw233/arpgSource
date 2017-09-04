/************************************************************************
 * file	:internal_protocol.h                                                                     
 * desc	:本文件定义了所有服务器之间通讯的协议
 *			并需要实现协议级别的,数据包缓存
 *
 *
 * author:	linbc
 * date:	20110418
 * version:
 ************************************************************************/
#ifndef _SVRCOER_PROTOCOL_H_
#define _SVRCOER_PROTOCOL_H_

#include <svrcore-internal.h>

//服务器之间内部通讯协议
enum Internal_Opcode_Type
{
	INTERNAL_OPT_NULL					= 0,
	INTERNAL_OPT_REG_SERVER				= 1,		//注册服务器,服务器连接到网关服,由此确定身份

	//服务器建立连接 begin
	INTERNAL_OPT_CREATE_CONN			= 2,		//新连接建立
	INTERNAL_OPT_DESTORY_CONN			= 3,		//断开连接
    INTERNAL_OPT_PING_PONG  			= 4,		//INTERNAL_OPT_DUPLICATE_CONN,重复连接
	INTERNAL_OPT_PACKET_GATE			= 5,		//信息包转发
	INTERNAL_OPT_PACKET_GATE_LIST		= 6,		//转发包列表	
	INTERNAL_OPT_REG_TOFD				= 7,		//注册数据包的转发流向
	//服务器建立连接 end

	//内部对象更新 begin
	INTERNAL_OPT_UD_OBJECT				= 8,		//内部对象更新	
	INTERNAL_OPT_UD_CONTROL				= 9,		//内部对象更新控制协议
	INTERNAL_OPT_UD_CONTROL_RESULT		= 10,		//内部对象更新控制协议结果
	__INTERNAL_OPT_UD_UNKOWN			= 11,		//内部对象更新预留
	//内部对象更新 end

	//后台命令 begin
	INTERNAL_OPT_COMMMAND				= 12,		//后台命令
	INTERNAL_OPT_COMMMAND_RESULTS		= 13,		//后台命令执行结果
	//后台命令 end

	INTERNAL_OPT_GET_ONLINE_PLAYER		= 14,		//向中心服获取所有在线玩家列表
	INTERNAL_OPT_GET_SESSION			= 15,		//玩家get session
	INTERNAL_OPT_CENTD_DESTORY_CONN		= 16,		//通知中心服断开连接

	INTERNAL_OPT_OPEN_TIME				= 17,		//同步服务器时间

	INTERNAL_OPT_SPLICE					= 18,		//超大包的拼接
	INTERNAL_OPT_PACKET_CENTD_ROUTER	= 19,		//中心服路由包
	INTERNAL_OPT_SAVE_ALL_DATA			= 20,		//中心服发给登录服，通知保存所有数据
	INTERNAL_OPT_GM_COMMMANDS			= 21,		//GM命令
	__INTERNAL_OPT_UNKOWN1				= 23,		//把玩家传送回游戏服
	INTERNAL_OPT_PLAYER_LOGIN			= 24,		//玩家登录成功
	INTERNAL_OPT_UNSET_RELEASE_MYSELF	= 25,		//设置本对象为不主动释放自己的
	INTERNAL_OPT_LOAD_OBJECT			= 26,		//请求从数据库读取对象集合
	INTERNAL_OPT_RELEASE_OBJECT			= 27,		//通知中心服移除没有订阅的对象
	///////////////////////////////////////////邪恶的分割线////////////////////////////////////////////////
	//以上，要么中心服发出，要么发给网关服
	//以下，就是各个服务器自己定制的协议了
	//按现在的设计，都得走路由包
	
	INTERNAL_OPT_CALLBACK,					//回调协议
	INTERNAL_OPT_PLAYER_LOGOUT,				//玩家退出登录成功
	INTERNAL_OPT_LOGIND_MERGE_SERVER,		//登陆服下发合服通知
	
	INTERNAL_OPT_JOIN_MAP,					//玩家移动到地图
	INTERNAL_OPT_LEAVE_MAP,					//玩家离开地图
	INTERNAL_OPT_APPD_TELEPORT,				//应用服发起传送
	INTERNAL_OPT_ADD_MAP_INSTANCE,			//通知创建地图实例
	INTERNAL_OPT_DEL_MAP_INSTANCE,			//通知删除地图实例
	INTERNAL_OPT_ADD_TAG_WATCH,				//制作补全

	INTERNAL_OPT_USER_ITEM,					//使用物品
	INTERNAL_OPT_USER_ITEM_RESULT,			//使用物品结果	
	INTERNAL_OPT_QUEST_ADD_REW_ITEM,		//生成任务奖励物品
	INTERNAL_OPT_WRITE_LOG,					//通知日志服写日志
	INTERNAL_OPT_NOTICE,					//发公告
	INTERNAL_OPT_CHAT,						//聊天接口
	INTERNAL_OPT_ADD_GIFT_PACKS,			//通知登陆服添加礼包
	INTERNAL_OPT_UPDATE_EQUIP_INFO,			//应用服更新玩家装备/坐骑属性到场景服
	INTERNAL_OPT_RECALCULATION_ATTR,		//通知应用服重算属性	 
	INTERNAL_OPT_DEL_CHAR,					//删除角色
	INTERNAL_OPT_USER_KILLED,				//玩家被玩家杀死
	INTERNAL_OPT_LOOT_SELECT,				//玩家拾取战利品
	INTERNAL_OPT_ADD_EXP,					//应用服通知场景服增加经验
	INTERNAL_OPT_DELETE_BINLOG,				//通知数据服删除binlog

	INTERNAL_OPT_ADD_GOLD_LOG = 51,				//新增元宝操作记录
	INTERNAL_OPT_ADD_FORGE_LOG,				//新增锻造记录
	INTERNAL_OPT_SCENED_CONSUME_MONEY,		//使用元宝,铜钱通知场景服做些什么
	INTERNAL_OPT_APPD_ADD_MONEY,			//通知应用服增加money
	INTERNAL_OPT_APPD_SUB_MONEY,			//通知应用服扣money
	
	INTERNAL_OPT_SYNC_GUID_MAX = 57,				//合服时同步guid管理器最大的累加值
	
	INTERNAL_OPT_HT_FORGE_UP = 62,				//发给后台锻造升级日志
	
	INTERNAL_OPT_MAP_ROUTER_PKT,			//地图实例之间的包

	INTERNAL_OPT_CLOSE_SCENED,				//通知场景服，你出问题了，自己关掉自己吧	
	INTERNAL_OPT_OFFLINE_OPER,				//离线操作
	INTERNAL_OPT_OFFLINE_OPER_RESULT,		//离线操作,binlog操作结果包	
	INTERNAL_OPT_SAVE_BACKUP,				//保存备份数据记录	
	INTERNAL_OPT_LOGIN_LOCK_INFO,			//通知登陆服封号信息
	INTERNAL_OPT_TENCENT_LOG,				//通知日志服写腾讯日志
	INTERNAL_OPT_LOGIND_HOSTING,			//通知登陆服托管登录
	INTERNAL_OPT_LOGS_FIRST_RECHARGE,		//应用服发包给日志服保存首冲记录

	INTERNAL_OPT_ADD_MAP_WATHER = 73,			//添加地图观察者

	INTERNAL_OPT_UPGRADE_LEVEL,				//玩家升级

	INTERNAL_OPT_APPD_TO_SEND_DO_SOMETHING = 75,
	INTERNAL_OPT_SEND_TO_APPD_DO_SOMETHING = 76,
	
	INTERNAL_OPT_UPDATE_GAG_STATUS,			//通知日志服更新禁言状态
	INTERNAL_OPT_UPDATE_LOCK_STATUS,		//通知日志服更新封号状态

	INTERNAL_OPT_UPDATE_SPELL_INFO			= 81,		//通知场景服更新技能信息
	INTERNAL_OPT_CHANGE_DIVINE_INFO			= 82,		//通知场景服神兵替换
	INTERNAL_OPT_REPLACE_EQUIPED_SPELL		= 83,		//通知场景服装备槽技能替换
	INTERNAL_OPT_ILLUSION					= 84,		//通知场景服更换幻化
	INTERNAL_OPT_ADD_ITEMS					= 85,		//通知应用服加多个道具
	INTERNAL_OPT_LOGIND_TELEPORT			= 86,		//登录服发起传送

	INTERNAL_OPT_KUAFU_ENTER				= 90,		//应用服通知登录服跨服信息
	INTERNAL_OPT_KUAFU_BACK					= 91,		//场景服服通知登录服让玩家回游戏服

	INTERNAL_OPT_INVITE_RET					= 92,		//邀请的处理结果

	INTERNAL_OPT_FACTION_BOSS_WIN			= 93,		//挑战boss成功
	INTERNAL_OPT_FACTION_BOSS_FAIL			= 94,		//挑战boss失败
	INTERNAL_OPT_FACTION_ADD_TOKEN_POINTS	= 95,		//击杀野怪点数
	INTERNAL_OPT_FACTION_BOSS_DAMAGED		= 96,		//对boss造成伤害
	INTERNAL_OPT_FACTION_UPDATE_BOSS_INFO	= 97,		//更新boss信息
	INTERNAL_OPT_FACTION_UPDATE_TARGET_INFO	= 98,		//更新保护目标信息


	INTERNAL_OPT_FACTION_BOSSDEFENSE_WIN	= 99,		//家族首领挑战成功
	INTERNAL_OPT_FACTION_BOSSDEFENSE_LEAVE	= 100,		//家族首领挑战玩家离开场景
	INTERNAL_OPT_RENAME_CHECK				= 101,		//修改名称检测
	INTERNAL_OPT_RENAME_CHECK_RESULT		= 102,		//修改名称检测结果
	INTERNAL_OPT_UPDATE_CHAR_NAME			= 103,		//更新名字

	MAX_INTERNAL_OPT						= 9999,//内部包最大包号
};

//服务器类型
enum ServerType
{
	SERVER_TYPE_NETGD	= 0,		//网关服
	SERVER_TYPE_CENTD	= 1,		//中心服
	SERVER_TYPE_LOGIND	= 2,		//登录服
	SERVER_TYPE_APPD	= 3,		//应用服
	SERVER_TYPE_POLICED	= 4,		//日志服
//	SERVER_TYPE_ROBOTD	= 5,		//机器人服
	SERVER_TYPE_SCENED	= 6,		//场景服
};

// 服务器连接信息下标
// 前面16位是连接次数，后面16位是成功连接次数
#define SERVER_CONN_LIST_FIELD_CONN_NUM 0	// 各种服务器连接次数，先预留个20种服务器吧
#define SERVER_CONN_LIST_FIELD_WORLD_STATUS 19		// 世界服链接状态
#define SERVER_CONN_LIST_FIELD_SERVER_INFO_START 20	// 服务器信息开始下标

//服务器信息枚举
enum EServerConnInfo
{
	SERVER_CONN_INFO_ID				= 0,	//连接ID
	SERVER_CONN_INFO_TYPE			= 1,	//服务器类型
	SERVER_CONN_INFO_LINENO			= 2,	//线路
	SERVER_CONN_INFO_COUNT			= 3,	//玩家数量
	SERVER_CONN_INFO_PING_PONG_MS	= 4,	//响应时间
	SERVER_CONN_INFO_SCENED_TYPE	= 5,	//场景服分类 0)替补地图 1)普通地图 2)活动地图 3)副本地图；仅对场景服有效
	SERVER_CONN_INFO_PID			= 6,	//远程的进程号
	SERVER_CONN_INFO_FLAG			= 7,	//一些标志
	MAX_SERVER_CONN_INFO,
};

//服务器标志位，准备好了
#define SERVER_CONN_INFO_FLAG_READY_OK 0

//场景服类型
enum EScenedType
{
	SCENED_TYPE_NONE = 0,
	SCENED_TYPE_NO_INST = 1,
	SCENED_TYPE_ACTIVI = 2,
	SCENED_TYPE_INST = 3,
	MAX_SCENED_TYPE
};

//服务器注册类型
enum EServerRegTypes
{
	SERVER_REG_TYPE_SINGLETON	= 0,	//只能有一个连接的服务器
	SERVER_REG_TYPE_PLURAL		= 1,	//可以很多个连接并存的服务器
};

/*************************************************************************************/
/**/
/*************************************************************************************/

/*
 *	call_packet_gate - 单包转发
 *	@dst :返回新包指针
 *	@pkt :需要转发数据包
 *
 *	@return	: 0成功 -1失败
 *  
 */
TEA_SVRCORE_API int pack_packet_gate(server_packet **dst,packet *pkt);

/*
 *	on_packet_gate - 当 单包转发
 *	@src :	数据包
 *	@lppkt :返回需转发数据包
 *
 *	@return	: 0成功 -1失败
 *  
 */
TEA_SVRCORE_API int unpack_packet_gate(server_packet *src,packet *lppkt);

/*
 *	call_packet_gate_list - 批量转发数据包
 *	@dst :返回新包指针
 *	@pkt :需要转发数据包列表
 *
 *	@return	: 0成功 -1失败
 *  
 */
TEA_SVRCORE_API int pack_packet_gate_list(server_packet **dst,list_t *pkt_head);

/*
 *	on_packet_gate_list - 当批量转发数据包
 *	@src :需要解开数据包
 *	@pkt_head :解开包后放入队列
 *
 *	@return	: 0成功 -1失败
 *  
 */
TEA_SVRCORE_API int unpack_packet_gate_list(server_packet *src,list_t *pkt_head);

/*
 *	pack_reg_to_fd - 注册数据包流向
 *	@dst :返回新包指针
 *	@player_fd :玩家 文件描述符
 *	@to_fd :转发到  文件描述符
 *	@num:数量
 *	@opts:包号数组
 *
 *	@return	:$return$
 *  
 */
TEA_SVRCORE_API int pack_reg_to_fd(server_packet **dst,int player_fd,int to_fd,uint16 num,const uint16 *opts);

/*
 *	unpack_reg_to_fd - 
 *	@dst :返回新包指针
 *	@player_fd :玩家 文件描述符
 *	@to_fd :转发到  文件描述符
 *	@num:数量
 *	@opts:包号数组
 *
 *	@return	: 0成功 -1失败
 *  
 */
TEA_SVRCORE_API int unpack_reg_to_fd(server_packet *src,int *player_fd,int *to_fd,uint16 *num,uint16 **opts);

/*
 *	pack_reg_server - 注册服务器 打包
 *	@dst : 新包
 *	@server_type : 服务器类型
 *
 *	@return	: 0成功 -1失败
 *  
 *	其他服务器连接到网关服以后
 *	需要由此进行注册,让网关服可以确认包的转发去向
 *	目前比较简单,所以中心服发送此包即可
 */
TEA_SVRCORE_API int pack_reg_server(server_packet **dst,int fd,int server_type,int pid, int reg_type);

/*
 *	unpacket_reg_server - 注册服务器 解包
 *	@src :
 *	@server_type :服务器类型	0 网关服 1 中心服 2 场景服 3 应用服
 *
 *	@return	: 0成功 -1失败
 *  
 */

TEA_SVRCORE_API int unpacket_reg_server(server_packet *src,int* fd,int *server_type,int *pid);

//服务器启动时间
TEA_SVRCORE_API int pack_internal_open_time(packet **dst,uint32 open_time,struct timeval *tv);

TEA_SVRCORE_API int unpack_internal_open_time(packet *src,uint32 *open_time,struct timeval *tv);

//通知日志服写日志
TEA_SVRCORE_API int unpack_internal_write_log(packet *src, uint32 *log_type, time_t *sec, time_t *usec, char **user, char **roleid);


#endif

