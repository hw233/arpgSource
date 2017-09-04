#include "ProtocolUtil.h"

void ProtocolUtil::ResetConn(core_socket::TcpConnection *conn)
{
	m_encipher.init(PROTO_TEA_C2S,PROTO_TEA_S2C);
	m_conn = conn;
}

void ProtocolUtil::ResetWarConn(core_socket::TcpConnection *conn)
{
	m_war_encipher.init(PROTO_TEA_C2S,PROTO_TEA_S2C);
	m_war_conn = conn;
}

UpdateMask ProtocolUtil::m_war_optcode;
void ProtocolUtil::SetWarOptCode(vector<int> &war_optcode)
{
	m_war_optcode.Clear();
	for (auto it:war_optcode)
	{
		m_war_optcode.SetBit(it);
	}
}

void ProtocolUtil::SendToServer(ByteArray &bytes)
{
	string newKey;
	static char tmp[4096];

	bytes.position(0);
	int optcode = bytes.readUnsignedShort();
	core_socket::TcpConnection *conn;
	core_socket::c2sencipher *encipher;
	if(m_war_conn == nullptr || !m_war_optcode.GetBit(optcode))
	{
		conn = m_conn;
		encipher = &m_encipher;
	}
	else
	{
		conn = m_war_conn;
		encipher = &m_war_encipher;
	}

	if(optcode == CMSG_GET_SESSION){
		MD5CryptToStr32((char*)bytes.data(), bytes.length(), tmp);
		strcat(tmp,PROTO_MD5_KEY);			
		newKey = tmp;
	}
	
	//encipher->encode(&bytes);

	if(optcode == CMSG_GET_SESSION)
		encipher->init(newKey.c_str(),newKey.c_str());

	conn->SendPacket(bytes);
}

/*无效动作*/
int ProtocolUtil::unpack_null_action (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_null_action ()
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_NULL_ACTION);
	
	SendToServer(m_bytes);
	return 0;
}

/*测试连接状态*/
int ProtocolUtil::unpack_ping_pong (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_ping_pong ()
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_PING_PONG);
	
	SendToServer(m_bytes);
	return 0;
}

/*踢掉在线的准备强制登陆*/
int ProtocolUtil::unpack_forced_into (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_forced_into ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FORCED_INTO);
	
	SendToServer(m_bytes);
	return 0;
}

/*获得Session对象*/
int ProtocolUtil::unpack_get_session (ByteArray &bytes ,string &sessionkey,string &account,string &version)
{
	int ret=0;
	//String
	sessionkey = bytes.readUTF();
	//String
	account = bytes.readUTF();
	//String
	version = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_get_session (char const*sessionkey,char const*account,char const*version)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_SESSION);
	
	m_bytes.writeUTF(sessionkey);
	m_bytes.writeUTF(account);
	m_bytes.writeUTF(version);
	SendToServer(m_bytes);
	return 0;
}

/*网关服数据包路由测试*/
int ProtocolUtil::unpack_route_trace (ByteArray &bytes ,string &val)
{
	int ret=0;
	//String
	val = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_route_trace (char const*val)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_ROUTE_TRACE);
	
	m_bytes.writeUTF(val);
	SendToServer(m_bytes);
	return 0;
}

/*记录客户端日志*/
int ProtocolUtil::unpack_write_client_log (ByteArray &bytes ,uint32 &type,string &uid,string &guid,string &log)
{
	int ret=0;
	//uint32
	type = bytes.readUnsignedInt();
	//String
	uid = bytes.readUTF();
	//String
	guid = bytes.readUTF();
	//String
	log = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_write_client_log (uint32 type,char const*uid,char const*guid,char const*log)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WRITE_CLIENT_LOG);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint32));
	m_bytes.writeUTF(uid);
	m_bytes.writeUTF(guid);
	m_bytes.writeUTF(log);
	SendToServer(m_bytes);
	return 0;
}

/*操作失败*/
int ProtocolUtil::unpack_operation_failed (ByteArray &bytes ,uint16 &type,uint16 &reason,string &data)
{
	int ret=0;
	//uint16
	type = bytes.readUnsignedShort();
	//uint16
	reason = bytes.readUnsignedShort();
	//String
	data = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_operation_failed (uint16 type,uint16 reason,char const*data)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_OPERATION_FAILED);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&reason, sizeof(uint16));
	m_bytes.writeUTF(data);
	SendToServer(m_bytes);
	return 0;
}

/*同步时间*/
int ProtocolUtil::unpack_sync_mstime (ByteArray &bytes ,uint32 &mstime_now,uint32 &time_now,uint32 &open_time)
{
	int ret=0;
	//uint32
	mstime_now = bytes.readUnsignedInt();
	//uint32
	time_now = bytes.readUnsignedInt();
	//uint32
	open_time = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_sync_mstime (uint32 mstime_now,uint32 time_now,uint32 open_time)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_SYNC_MSTIME);
	
	m_bytes.writeBytes((uint8*)&mstime_now, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&time_now, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&open_time, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}


/*对象更新控制协议*/
int ProtocolUtil::unpack_ud_control (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_ud_control ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_UD_CONTROL);
	
	SendToServer(m_bytes);
	return 0;
}




/*告诉客户端，目前自己排在登录队列的第几位*/
int ProtocolUtil::unpack_login_queue_index (ByteArray &bytes ,uint32 &index)
{
	int ret=0;
	//uint32
	index = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_login_queue_index (uint32 index)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_LOGIN_QUEUE_INDEX);
	
	m_bytes.writeBytes((uint8*)&index, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*踢人原因*/
int ProtocolUtil::unpack_kicking_type (ByteArray &bytes ,uint32 &k_type)
{
	int ret=0;
	//uint32
	k_type = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_kicking_type (uint32 k_type)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KICKING_TYPE);
	
	m_bytes.writeBytes((uint8*)&k_type, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*获取角色列表*/
int ProtocolUtil::unpack_get_chars_list (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_get_chars_list ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_CHARS_LIST);
	
	SendToServer(m_bytes);
	return 0;
}

/*角色列表*/
int ProtocolUtil::unpack_chars_list (ByteArray &bytes , vector< char_create_info > &list,string &faction_name,string &queen_name,uint8 &icon)
{
	int ret=0;
	//char_create_info
	ASSERT(false);
	//String
	faction_name = bytes.readUTF();
	//String
	queen_name = bytes.readUTF();
	//uint8
	icon = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_chars_list (const vector< char_create_info > &list ,char const*faction_name,char const*queen_name,uint8 icon)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_CHARS_LIST);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	m_bytes.writeUTF(faction_name);
	m_bytes.writeUTF(queen_name);
	m_bytes.writeBytes((uint8*)&icon, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*检查名字是否可以使用*/
int ProtocolUtil::unpack_check_name (ByteArray &bytes ,string &name)
{
	int ret=0;
	//String
	name = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_check_name (char const*name)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHECK_NAME);
	
	m_bytes.writeUTF(name);
	SendToServer(m_bytes);
	return 0;
}

/*检查名字结果*/
int ProtocolUtil::unpack_check_name_result (ByteArray &bytes ,uint8 &result)
{
	int ret=0;
	//uint8
	result = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_check_name_result (uint8 result)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_CHECK_NAME_RESULT);
	
	m_bytes.writeBytes((uint8*)&result, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*创建角色*/
int ProtocolUtil::unpack_char_create (ByteArray &bytes , char_create_info **info)
{
	int ret=0;
	//char_create_info
	*info = (char_create_info*)bytes.cur_data();
	bytes.position(bytes.position() + sizeof(char_create_info));
	return ret;
}

int ProtocolUtil::send_char_create (char_create_info *info)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHAR_CREATE);
	
	m_bytes.writeBytes((uint8*)info, sizeof(char_create_info));
	SendToServer(m_bytes);
	return 0;
}

/*角色创建结果*/
int ProtocolUtil::unpack_char_create_result (ByteArray &bytes ,uint8 &result)
{
	int ret=0;
	//uint8
	result = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_char_create_result (uint8 result)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_CHAR_CREATE_RESULT);
	
	m_bytes.writeBytes((uint8*)&result, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*删除角色*/
int ProtocolUtil::unpack_delete_char (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_delete_char (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DELETE_CHAR);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*角色删除结果*/
int ProtocolUtil::unpack_delete_char_result (ByteArray &bytes ,uint8 &result)
{
	int ret=0;
	//uint8
	result = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_delete_char_result (uint8 result)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_DELETE_CHAR_RESULT);
	
	m_bytes.writeBytes((uint8*)&result, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*玩家登录*/
int ProtocolUtil::unpack_player_login (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_player_login (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PLAYER_LOGIN);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*玩家退出*/
int ProtocolUtil::unpack_player_logout (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_player_logout ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PLAYER_LOGOUT);
	
	SendToServer(m_bytes);
	return 0;
}

/*临时账号转正规*/
int ProtocolUtil::unpack_regularise_account (ByteArray &bytes ,string &uid)
{
	int ret=0;
	//String
	uid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_regularise_account (char const*uid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_REGULARISE_ACCOUNT);
	
	m_bytes.writeUTF(uid);
	SendToServer(m_bytes);
	return 0;
}

/*角色配置信息*/
int ProtocolUtil::unpack_char_remotestore (ByteArray &bytes ,uint32 &key,uint32 &value)
{
	int ret=0;
	//uint32
	key = bytes.readUnsignedInt();
	//uint32
	value = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_char_remotestore (uint32 key,uint32 value)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHAR_REMOTESTORE);
	
	m_bytes.writeBytes((uint8*)&key, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&value, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*角色配置信息*/
int ProtocolUtil::unpack_char_remotestore_str (ByteArray &bytes ,uint32 &key,string &value)
{
	int ret=0;
	//uint32
	key = bytes.readUnsignedInt();
	//String
	value = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_char_remotestore_str (uint32 key,char const*value)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHAR_REMOTESTORE_STR);
	
	m_bytes.writeBytes((uint8*)&key, sizeof(uint32));
	m_bytes.writeUTF(value);
	SendToServer(m_bytes);
	return 0;
}

/*传送，如果是C->S，mapid变量请填成传送点ID*/
int ProtocolUtil::unpack_teleport (ByteArray &bytes ,uint32 &intGuid)
{
	int ret=0;
	//uint32
	intGuid = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_teleport (uint32 intGuid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TELEPORT);
	
	m_bytes.writeBytes((uint8*)&intGuid, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*停止移动*/
int ProtocolUtil::unpack_move_stop (ByteArray &bytes ,uint32 &guid,uint16 &pos_x,uint16 &pos_y)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	//uint16
	pos_x = bytes.readUnsignedShort();
	//uint16
	pos_y = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_move_stop (uint32 guid,uint16 pos_x,uint16 pos_y)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_MOVE_STOP);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&pos_x, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&pos_y, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*unit对象移动*/
int ProtocolUtil::unpack_unit_move (ByteArray &bytes ,uint32 &guid,uint16 &pos_x,uint16 &pos_y, vector< int8 > &path)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	//uint16
	pos_x = bytes.readUnsignedShort();
	//uint16
	pos_y = bytes.readUnsignedShort();
	//int8
	uint16 len = bytes.readUnsignedShort();
	for (; len > 0; len--)
	{
		path. push_back(bytes.readByte());
	}
	return ret;
}

int ProtocolUtil::send_unit_move (uint32 guid,uint16 pos_x,uint16 pos_y,const vector< int8 > &path )
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_UNIT_MOVE);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&pos_x, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&pos_y, sizeof(uint16));
	m_bytes.writeShort(path .size());
	for (auto it:path)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*使用游戏对象*/
int ProtocolUtil::unpack_use_gameobject (ByteArray &bytes ,uint32 &target)
{
	int ret=0;
	//uint32
	target = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_use_gameobject (uint32 target)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_USE_GAMEOBJECT);
	
	m_bytes.writeBytes((uint8*)&target, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*包裹操作-交换位置*/
int ProtocolUtil::unpack_bag_exchange_pos (ByteArray &bytes ,uint32 &src_bag,uint32 &src_pos,uint32 &dst_bag,uint32 &dst_pos)
{
	int ret=0;
	//uint32
	src_bag = bytes.readUnsignedInt();
	//uint32
	src_pos = bytes.readUnsignedInt();
	//uint32
	dst_bag = bytes.readUnsignedInt();
	//uint32
	dst_pos = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_bag_exchange_pos (uint32 src_bag,uint32 src_pos,uint32 dst_bag,uint32 dst_pos)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BAG_EXCHANGE_POS);
	
	m_bytes.writeBytes((uint8*)&src_bag, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&src_pos, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&dst_bag, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&dst_pos, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*包裹操作-销毁物品*/
int ProtocolUtil::unpack_bag_destroy (ByteArray &bytes ,string &item_guid,uint32 &num,uint32 &bag_id)
{
	int ret=0;
	//String
	item_guid = bytes.readUTF();
	//uint32
	num = bytes.readUnsignedInt();
	//uint32
	bag_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_bag_destroy (char const*item_guid,uint32 num,uint32 bag_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BAG_DESTROY);
	
	m_bytes.writeUTF(item_guid);
	m_bytes.writeBytes((uint8*)&num, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&bag_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*分割物品*/
int ProtocolUtil::unpack_bag_item_split (ByteArray &bytes ,uint8 &bag_id,uint16 &src_pos,uint32 &count,uint16 &dst_pos,uint8 &dst_bag)
{
	int ret=0;
	//uint8
	bag_id = bytes.readUnsignedByte();
	//uint16
	src_pos = bytes.readUnsignedShort();
	//uint32
	count = bytes.readUnsignedInt();
	//uint16
	dst_pos = bytes.readUnsignedShort();
	//uint8
	dst_bag = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_bag_item_split (uint8 bag_id,uint16 src_pos,uint32 count,uint16 dst_pos,uint8 dst_bag)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BAG_ITEM_SPLIT);
	
	m_bytes.writeBytes((uint8*)&bag_id, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&src_pos, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&dst_pos, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&dst_bag, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*使用物品*/
int ProtocolUtil::unpack_bag_item_user (ByteArray &bytes ,string &item_guid,uint32 &count)
{
	int ret=0;
	//String
	item_guid = bytes.readUTF();
	//uint32
	count = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_bag_item_user (char const*item_guid,uint32 count)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BAG_ITEM_USER);
	
	m_bytes.writeUTF(item_guid);
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*下发物品冷却*/
int ProtocolUtil::unpack_bag_item_cooldown (ByteArray &bytes , vector< item_cooldown_info > &list)
{
	int ret=0;
	//item_cooldown_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_bag_item_cooldown (const vector< item_cooldown_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_BAG_ITEM_COOLDOWN);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}



/*兑换物品*/
int ProtocolUtil::unpack_exchange_item (ByteArray &bytes ,uint32 &entry,uint32 &count,uint32 &tar_entry)
{
	int ret=0;
	//uint32
	entry = bytes.readUnsignedInt();
	//uint32
	count = bytes.readUnsignedInt();
	//uint32
	tar_entry = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_exchange_item (uint32 entry,uint32 count,uint32 tar_entry)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_EXCHANGE_ITEM);
	
	m_bytes.writeBytes((uint8*)&entry, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&tar_entry, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*背包扩展*/
int ProtocolUtil::unpack_bag_extension (ByteArray &bytes ,uint8 &bag_id,uint8 &extension_type,uint32 &bag_pos)
{
	int ret=0;
	//uint8
	bag_id = bytes.readUnsignedByte();
	//uint8
	extension_type = bytes.readUnsignedByte();
	//uint32
	bag_pos = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_bag_extension (uint8 bag_id,uint8 extension_type,uint32 bag_pos)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BAG_EXTENSION);
	
	m_bytes.writeBytes((uint8*)&bag_id, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&extension_type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&bag_pos, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*请求NPC商品列表*/
int ProtocolUtil::unpack_npc_get_goods_list (ByteArray &bytes ,uint32 &npc_id)
{
	int ret=0;
	//uint32
	npc_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_npc_get_goods_list (uint32 npc_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_NPC_GET_GOODS_LIST);
	
	m_bytes.writeBytes((uint8*)&npc_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*Npc商品列表*/
int ProtocolUtil::unpack_npc_goods_list (ByteArray &bytes , vector< uint32 > &goods,uint32 &npc_id)
{
	int ret=0;
	//uint32
	uint16 len = bytes.readUnsignedShort();
	for (; len > 0; len--)
	{
		goods. push_back(bytes.readUnsignedInt());
	}
	//uint32
	npc_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_npc_goods_list (const vector< uint32 > &goods ,uint32 npc_id)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_NPC_GOODS_LIST);
	
	m_bytes.writeShort(goods .size());
	for (auto it:goods)
	{
		m_bytes.writeT(it);
	}
	m_bytes.writeBytes((uint8*)&npc_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*购买商品*/
int ProtocolUtil::unpack_store_buy (ByteArray &bytes ,uint32 &id,uint32 &count)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	//uint32
	count = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_store_buy (uint32 id,uint32 count)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_STORE_BUY);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*出售物品*/
int ProtocolUtil::unpack_npc_sell (ByteArray &bytes ,uint32 &npc_id,string &item_guid,uint32 &num)
{
	int ret=0;
	//uint32
	npc_id = bytes.readUnsignedInt();
	//String
	item_guid = bytes.readUTF();
	//uint32
	num = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_npc_sell (uint32 npc_id,char const*item_guid,uint32 num)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_NPC_SELL);
	
	m_bytes.writeBytes((uint8*)&npc_id, sizeof(uint32));
	m_bytes.writeUTF(item_guid);
	m_bytes.writeBytes((uint8*)&num, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*回购物品*/
int ProtocolUtil::unpack_npc_repurchase (ByteArray &bytes ,string &item_id)
{
	int ret=0;
	//String
	item_id = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_npc_repurchase (char const*item_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_NPC_REPURCHASE);
	
	m_bytes.writeUTF(item_id);
	SendToServer(m_bytes);
	return 0;
}

/*时装是否启用*/
int ProtocolUtil::unpack_avatar_fashion_enable (ByteArray &bytes ,uint8 &pos)
{
	int ret=0;
	//uint8
	pos = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_avatar_fashion_enable (uint8 pos)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_AVATAR_FASHION_ENABLE);
	
	m_bytes.writeBytes((uint8*)&pos, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*任务对话选项*/
int ProtocolUtil::unpack_questhelp_talk_option (ByteArray &bytes ,uint32 &quest_id,uint32 &option_id,int32 &value0,int32 &value1)
{
	int ret=0;
	//uint32
	quest_id = bytes.readUnsignedInt();
	//uint32
	option_id = bytes.readUnsignedInt();
	//int32
	value0 = bytes.readInt();
	//int32
	value1 = bytes.readInt();
	return ret;
}

int ProtocolUtil::send_questhelp_talk_option (uint32 quest_id,uint32 option_id,int32 value0,int32 value1)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTHELP_TALK_OPTION);
	
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&option_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&value0, sizeof(int32));
	m_bytes.writeBytes((uint8*)&value1, sizeof(int32));
	SendToServer(m_bytes);
	return 0;
}

/*与NPC对话获得传送点列表*/
int ProtocolUtil::unpack_taxi_hello (ByteArray &bytes ,uint32 &guid)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_taxi_hello (uint32 guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TAXI_HELLO);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*发送传送点列表*/
int ProtocolUtil::unpack_taxi_stations_list (ByteArray &bytes ,uint32 &npcid, vector< taxi_menu_info > &stations)
{
	int ret=0;
	//uint32
	npcid = bytes.readUnsignedInt();
	//taxi_menu_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_taxi_stations_list (uint32 npcid,const vector< taxi_menu_info > &stations )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_TAXI_STATIONS_LIST);
	
	m_bytes.writeBytes((uint8*)&npcid, sizeof(uint32));
	m_bytes.writeShort(stations .size());
	for (auto it:stations)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*选择传送点*/
int ProtocolUtil::unpack_taxi_select_station (ByteArray &bytes ,uint32 &station_id,uint32 &guid)
{
	int ret=0;
	//uint32
	station_id = bytes.readUnsignedInt();
	//uint32
	guid = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_taxi_select_station (uint32 station_id,uint32 guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TAXI_SELECT_STATION);
	
	m_bytes.writeBytes((uint8*)&station_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*与NPC交流选择选项*/
int ProtocolUtil::unpack_gossip_select_option (ByteArray &bytes ,uint32 &option,uint32 &guid,string &unknow)
{
	int ret=0;
	//uint32
	option = bytes.readUnsignedInt();
	//uint32
	guid = bytes.readUnsignedInt();
	//String
	unknow = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_gossip_select_option (uint32 option,uint32 guid,char const*unknow)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GOSSIP_SELECT_OPTION);
	
	m_bytes.writeBytes((uint8*)&option, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	m_bytes.writeUTF(unknow);
	SendToServer(m_bytes);
	return 0;
}

/*与NPC闲聊获取选项*/
int ProtocolUtil::unpack_gossip_hello (ByteArray &bytes ,uint32 &guid)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_gossip_hello (uint32 guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GOSSIP_HELLO);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*发送闲聊信息和选项*/
int ProtocolUtil::unpack_gossip_message (ByteArray &bytes ,uint32 &npcid,uint32 &npc_entry,uint32 &option_id,string &option_text, vector< gossip_menu_option_info > &gossip_items)
{
	int ret=0;
	//uint32
	npcid = bytes.readUnsignedInt();
	//uint32
	npc_entry = bytes.readUnsignedInt();
	//uint32
	option_id = bytes.readUnsignedInt();
	//String
	option_text = bytes.readUTF();
	//gossip_menu_option_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_gossip_message (uint32 npcid,uint32 npc_entry,uint32 option_id,char const*option_text,const vector< gossip_menu_option_info > &gossip_items )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_GOSSIP_MESSAGE);
	
	m_bytes.writeBytes((uint8*)&npcid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&npc_entry, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&option_id, sizeof(uint32));
	m_bytes.writeUTF(option_text);
	m_bytes.writeShort(gossip_items .size());
	for (auto it:gossip_items)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*任务发布者状态查询*/
int ProtocolUtil::unpack_questgiver_status_query (ByteArray &bytes ,uint32 &guid)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questgiver_status_query (uint32 guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTGIVER_STATUS_QUERY);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*任务NPC状态*/
int ProtocolUtil::unpack_questgiver_status (ByteArray &bytes ,uint32 &guid,uint8 &status)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	//uint8
	status = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_questgiver_status (uint32 guid,uint8 status)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_QUESTGIVER_STATUS);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&status, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*查询任务状态*/
int ProtocolUtil::unpack_query_quest_status (ByteArray &bytes , vector< quest_status > &quest_array)
{
	int ret=0;
	//quest_status
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_query_quest_status (const vector< quest_status > &quest_array )
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_QUERY_QUEST_STATUS);
	
	m_bytes.writeShort(quest_array .size());
	for (auto it:quest_array)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*可接任务*/
int ProtocolUtil::unpack_questhelp_get_canaccept_list (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_questhelp_get_canaccept_list ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTHELP_GET_CANACCEPT_LIST);
	
	SendToServer(m_bytes);
	return 0;
}

/*下发可接任务列表*/
int ProtocolUtil::unpack_questhelp_canaccept_list (ByteArray &bytes , vector< uint32 > &quests)
{
	int ret=0;
	//uint32
	uint16 len = bytes.readUnsignedShort();
	for (; len > 0; len--)
	{
		quests. push_back(bytes.readUnsignedInt());
	}
	return ret;
}

int ProtocolUtil::send_questhelp_canaccept_list (const vector< uint32 > &quests )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_QUESTHELP_CANACCEPT_LIST);
	
	m_bytes.writeShort(quests .size());
	for (auto it:quests)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*任务失败*/
int ProtocolUtil::unpack_questupdate_faild (ByteArray &bytes ,uint8 &reason)
{
	int ret=0;
	//uint8
	reason = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_questupdate_faild (uint8 reason)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_QUESTUPDATE_FAILD);
	
	m_bytes.writeBytes((uint8*)&reason, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*任务条件完成*/
int ProtocolUtil::unpack_questupdate_complete (ByteArray &bytes ,uint32 &quest_id)
{
	int ret=0;
	//uint32
	quest_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questupdate_complete (uint32 quest_id)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_QUESTUPDATE_COMPLETE);
	
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*放弃任务*/
int ProtocolUtil::unpack_questlog_remove_quest (ByteArray &bytes ,uint8 &slot,uint64 &reserve)
{
	int ret=0;
	//uint8
	slot = bytes.readUnsignedByte();
	//uint64
	reserve = bytes.readUnsignedLongInt();
	return ret;
}

int ProtocolUtil::send_questlog_remove_quest (uint8 slot,uint64 reserve)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTLOG_REMOVE_QUEST);
	
	m_bytes.writeBytes((uint8*)&slot, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&reserve, sizeof(uint64));
	SendToServer(m_bytes);
	return 0;
}

/*完成任务*/
int ProtocolUtil::unpack_questgiver_complete_quest (ByteArray &bytes ,uint32 &guid,uint32 &quest_id,uint8 &reward)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	//uint32
	quest_id = bytes.readUnsignedInt();
	//uint8
	reward = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_questgiver_complete_quest (uint32 guid,uint32 quest_id,uint8 reward)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTGIVER_COMPLETE_QUEST);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&reward, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*完成任务后通知任务链的下个任务*/
int ProtocolUtil::unpack_questhelp_next (ByteArray &bytes ,uint32 &current_id,uint32 &next_id,uint32 &guid)
{
	int ret=0;
	//uint32
	current_id = bytes.readUnsignedInt();
	//uint32
	next_id = bytes.readUnsignedInt();
	//uint32
	guid = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questhelp_next (uint32 current_id,uint32 next_id,uint32 guid)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_QUESTHELP_NEXT);
	
	m_bytes.writeBytes((uint8*)&current_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&next_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*任务系统强制完成任务*/
int ProtocolUtil::unpack_questhelp_complete (ByteArray &bytes ,uint32 &quest_id,uint8 &quest_statue,uint8 &reserve)
{
	int ret=0;
	//uint32
	quest_id = bytes.readUnsignedInt();
	//uint8
	quest_statue = bytes.readUnsignedByte();
	//uint8
	reserve = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_questhelp_complete (uint32 quest_id,uint8 quest_statue,uint8 reserve)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTHELP_COMPLETE);
	
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&quest_statue, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&reserve, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*接受任务成功*/
int ProtocolUtil::unpack_questupdate_accept (ByteArray &bytes ,uint32 &quest_id)
{
	int ret=0;
	//uint32
	quest_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questupdate_accept (uint32 quest_id)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_QUESTUPDATE_ACCEPT);
	
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*更新任务进度_下标数量*/
int ProtocolUtil::unpack_questhelp_update_status (ByteArray &bytes ,uint32 &quest_id,uint32 &slot_id,uint32 &num)
{
	int ret=0;
	//uint32
	quest_id = bytes.readUnsignedInt();
	//uint32
	slot_id = bytes.readUnsignedInt();
	//uint32
	num = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questhelp_update_status (uint32 quest_id,uint32 slot_id,uint32 num)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTHELP_UPDATE_STATUS);
	
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&slot_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&num, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*任务已完成*/
int ProtocolUtil::unpack_questgetter_complete (ByteArray &bytes ,uint32 &quest_id)
{
	int ret=0;
	//uint32
	quest_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questgetter_complete (uint32 quest_id)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_QUESTGETTER_COMPLETE);
	
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*接任务*/
int ProtocolUtil::unpack_questgiver_accept_quest (ByteArray &bytes ,uint32 &npcid,uint32 &quest_id)
{
	int ret=0;
	//uint32
	npcid = bytes.readUnsignedInt();
	//uint32
	quest_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questgiver_accept_quest (uint32 npcid,uint32 quest_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTGIVER_ACCEPT_QUEST);
	
	m_bytes.writeBytes((uint8*)&npcid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*任务使用物品*/
int ProtocolUtil::unpack_questupdate_use_item (ByteArray &bytes ,uint32 &item_id)
{
	int ret=0;
	//uint32
	item_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questupdate_use_item (uint32 item_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTUPDATE_USE_ITEM);
	
	m_bytes.writeBytes((uint8*)&item_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*查询天书任务*/
int ProtocolUtil::unpack_questhelp_query_book (ByteArray &bytes ,uint32 &dynasty)
{
	int ret=0;
	//uint32
	dynasty = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questhelp_query_book (uint32 dynasty)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUESTHELP_QUERY_BOOK);
	
	m_bytes.writeBytes((uint8*)&dynasty, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*下发可接天书任务*/
int ProtocolUtil::unpack_questhelp_book_quest (ByteArray &bytes ,uint32 &quest_id)
{
	int ret=0;
	//uint32
	quest_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_questhelp_book_quest (uint32 quest_id)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_QUESTHELP_BOOK_QUEST);
	
	m_bytes.writeBytes((uint8*)&quest_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*玩家使用游戏对象以后的动作*/
int ProtocolUtil::unpack_use_gameobject_action (ByteArray &bytes ,uint32 &player_id,uint32 &gameobject_id)
{
	int ret=0;
	//uint32
	player_id = bytes.readUnsignedInt();
	//uint32
	gameobject_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_use_gameobject_action (uint32 player_id,uint32 gameobject_id)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_USE_GAMEOBJECT_ACTION);
	
	m_bytes.writeBytes((uint8*)&player_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&gameobject_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*设置攻击模式*/
int ProtocolUtil::unpack_set_attack_mode (ByteArray &bytes ,uint8 &mode,uint64 &reserve)
{
	int ret=0;
	//uint8
	mode = bytes.readUnsignedByte();
	//uint64
	reserve = bytes.readUnsignedLongInt();
	return ret;
}

int ProtocolUtil::send_set_attack_mode (uint8 mode,uint64 reserve)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SET_ATTACK_MODE);
	
	m_bytes.writeBytes((uint8*)&mode, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&reserve, sizeof(uint64));
	SendToServer(m_bytes);
	return 0;
}

/*选择目标*/
int ProtocolUtil::unpack_select_target (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_select_target (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_SELECT_TARGET);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*进入战斗*/
int ProtocolUtil::unpack_combat_state_update (ByteArray &bytes ,uint8 &cur_state)
{
	int ret=0;
	//uint8
	cur_state = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_combat_state_update (uint8 cur_state)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_COMBAT_STATE_UPDATE);
	
	m_bytes.writeBytes((uint8*)&cur_state, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*经验更新*/
int ProtocolUtil::unpack_exp_update (ByteArray &bytes ,int32 &exp,uint8 &type,int32 &vip_exp)
{
	int ret=0;
	//int32
	exp = bytes.readInt();
	//uint8
	type = bytes.readUnsignedByte();
	//int32
	vip_exp = bytes.readInt();
	return ret;
}

int ProtocolUtil::send_exp_update (int32 exp,uint8 type,int32 vip_exp)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_EXP_UPDATE);
	
	m_bytes.writeBytes((uint8*)&exp, sizeof(int32));
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&vip_exp, sizeof(int32));
	SendToServer(m_bytes);
	return 0;
}

/*客户端释放技能*/
int ProtocolUtil::unpack_spell_start (ByteArray &bytes ,uint32 &spell_id,uint16 &target_pos_x,uint16 &target_pos_y,uint32 &caster,uint32 &target)
{
	int ret=0;
	//uint32
	spell_id = bytes.readUnsignedInt();
	//uint16
	target_pos_x = bytes.readUnsignedShort();
	//uint16
	target_pos_y = bytes.readUnsignedShort();
	//uint32
	caster = bytes.readUnsignedInt();
	//uint32
	target = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_spell_start (uint32 spell_id,uint16 target_pos_x,uint16 target_pos_y,uint32 caster,uint32 target)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_SPELL_START);
	
	m_bytes.writeBytes((uint8*)&spell_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&target_pos_x, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&target_pos_y, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&caster, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&target, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*施法停止*/
int ProtocolUtil::unpack_spell_stop (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_spell_stop (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_SPELL_STOP);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*跳*/
int ProtocolUtil::unpack_jump (ByteArray &bytes ,uint32 &guid,float &pos_x,float &pos_y)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	//float
	pos_x = bytes.readFloat();
	//float
	pos_y = bytes.readFloat();
	return ret;
}

int ProtocolUtil::send_jump (uint32 guid,float pos_x,float pos_y)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_JUMP);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&pos_x, sizeof(float));
	m_bytes.writeBytes((uint8*)&pos_y, sizeof(float));
	SendToServer(m_bytes);
	return 0;
}

/*复活*/
int ProtocolUtil::unpack_resurrection (ByteArray &bytes ,uint8 &type,uint64 &reserve)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	//uint64
	reserve = bytes.readUnsignedLongInt();
	return ret;
}

int ProtocolUtil::send_resurrection (uint8 type,uint64 reserve)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RESURRECTION);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&reserve, sizeof(uint64));
	SendToServer(m_bytes);
	return 0;
}

/*交易发出请求*/
int ProtocolUtil::unpack_trade_request (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_trade_request (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_TRADE_REQUEST);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*交易请求答复*/
int ProtocolUtil::unpack_trade_reply (ByteArray &bytes ,string &guid,uint8 &reply)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//uint8
	reply = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_trade_reply (char const*guid,uint8 reply)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_TRADE_REPLY);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeBytes((uint8*)&reply, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*交易开始*/
int ProtocolUtil::unpack_trade_start (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_trade_start (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_TRADE_START);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*交易确认物品*/
int ProtocolUtil::unpack_trade_decide_items (ByteArray &bytes ,const string &items, uint16 *len_1,int32 &gold_ingot,int32 &silver)
{
	int ret=0;
	uint16 i = 0;
	//String
	ASSERT(false);
	//int32
	gold_ingot = bytes.readInt();
	//int32
	silver = bytes.readInt();
	return ret;
}

int ProtocolUtil::send_trade_decide_items (char const*items,int32 gold_ingot,int32 silver)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_TRADE_DECIDE_ITEMS);
	
	m_bytes.writeUTF(items);
	m_bytes.writeBytes((uint8*)&gold_ingot, sizeof(int32));
	m_bytes.writeBytes((uint8*)&silver, sizeof(int32));
	SendToServer(m_bytes);
	return 0;
}

/*交易完成*/
int ProtocolUtil::unpack_trade_finish (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_trade_finish ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_TRADE_FINISH);
	
	SendToServer(m_bytes);
	return 0;
}

/*交易取消*/
int ProtocolUtil::unpack_trade_cancel (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_trade_cancel ()
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_TRADE_CANCEL);
	
	SendToServer(m_bytes);
	return 0;
}

/*交易准备好*/
int ProtocolUtil::unpack_trade_ready (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_trade_ready ()
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_TRADE_READY);
	
	SendToServer(m_bytes);
	return 0;
}

/*生物讲话*/
int ProtocolUtil::unpack_chat_unit_talk (ByteArray &bytes ,uint32 &guid,uint32 &content,string &say)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	//uint32
	content = bytes.readUnsignedInt();
	//String
	say = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_chat_unit_talk (uint32 guid,uint32 content,char const*say)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_CHAT_UNIT_TALK);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&content, sizeof(uint32));
	m_bytes.writeUTF(say);
	SendToServer(m_bytes);
	return 0;
}

/*就近聊天*/
int ProtocolUtil::unpack_chat_near (ByteArray &bytes ,string &content)
{
	int ret=0;
	//String
	content = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_chat_near (char const*content)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHAT_NEAR);
	
	m_bytes.writeUTF(content);
	SendToServer(m_bytes);
	return 0;
}

/*私聊*/
int ProtocolUtil::unpack_chat_whisper (ByteArray &bytes ,string &guid,string &content)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//String
	content = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_chat_whisper (char const*guid,char const*content)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHAT_WHISPER);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeUTF(content);
	SendToServer(m_bytes);
	return 0;
}

/*阵营聊天*/
int ProtocolUtil::unpack_chat_faction (ByteArray &bytes ,string &guid,string &name,string &content,uint8 &faction)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//String
	name = bytes.readUTF();
	//String
	content = bytes.readUTF();
	//uint8
	faction = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_chat_faction (char const*guid,char const*name,char const*content,uint8 faction)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_CHAT_FACTION);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeUTF(name);
	m_bytes.writeUTF(content);
	m_bytes.writeBytes((uint8*)&faction, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*世界*/
int ProtocolUtil::unpack_chat_world (ByteArray &bytes ,string &guid,uint8 &faction,string &name,string &content)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//uint8
	faction = bytes.readUnsignedByte();
	//String
	name = bytes.readUTF();
	//String
	content = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_chat_world (char const*guid,uint8 faction,char const*name,char const*content)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_CHAT_WORLD);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeBytes((uint8*)&faction, sizeof(uint8));
	m_bytes.writeUTF(name);
	m_bytes.writeUTF(content);
	SendToServer(m_bytes);
	return 0;
}

/*喇叭*/
int ProtocolUtil::unpack_chat_horn (ByteArray &bytes ,string &guid,uint8 &faction,string &name,string &content)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//uint8
	faction = bytes.readUnsignedByte();
	//String
	name = bytes.readUTF();
	//String
	content = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_chat_horn (char const*guid,uint8 faction,char const*name,char const*content)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_CHAT_HORN);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeBytes((uint8*)&faction, sizeof(uint8));
	m_bytes.writeUTF(name);
	m_bytes.writeUTF(content);
	SendToServer(m_bytes);
	return 0;
}

/*公告*/
int ProtocolUtil::unpack_chat_notice (ByteArray &bytes ,uint32 &id,string &content,string &data)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	//String
	content = bytes.readUTF();
	//String
	data = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_chat_notice (uint32 id,char const*content,char const*data)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_CHAT_NOTICE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	m_bytes.writeUTF(content);
	m_bytes.writeUTF(data);
	SendToServer(m_bytes);
	return 0;
}

/*查询玩家信息*/
int ProtocolUtil::unpack_query_player_info (ByteArray &bytes ,string &guid,uint32 &flag,uint32 &callback_id)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//uint32
	flag = bytes.readUnsignedInt();
	//uint32
	callback_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_query_player_info (char const*guid,uint32 flag,uint32 callback_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUERY_PLAYER_INFO);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeBytes((uint8*)&flag, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&callback_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}


/*领取礼包*/
int ProtocolUtil::unpack_receive_gift_packs (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_receive_gift_packs ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RECEIVE_GIFT_PACKS);
	
	SendToServer(m_bytes);
	return 0;
}




/*进入副本*/
int ProtocolUtil::unpack_instance_enter (ByteArray &bytes ,uint32 &instance_id)
{
	int ret=0;
	//uint32
	instance_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_instance_enter (uint32 instance_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_INSTANCE_ENTER);
	
	m_bytes.writeBytes((uint8*)&instance_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*向服务端发送副本进入下一阶段指令*/
int ProtocolUtil::unpack_instance_next_state (ByteArray &bytes ,uint16 &level,uint32 &param)
{
	int ret=0;
	//uint16
	level = bytes.readUnsignedShort();
	//uint32
	param = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_instance_next_state (uint16 level,uint32 param)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_INSTANCE_NEXT_STATE);
	
	m_bytes.writeBytes((uint8*)&level, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&param, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*副本退出*/
int ProtocolUtil::unpack_instance_exit (ByteArray &bytes ,uint32 &reserve)
{
	int ret=0;
	//uint32
	reserve = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_instance_exit (uint32 reserve)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_INSTANCE_EXIT);
	
	m_bytes.writeBytes((uint8*)&reserve, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*限时活动领取*/
int ProtocolUtil::unpack_limit_activity_receive (ByteArray &bytes ,uint32 &id,uint32 &type)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	//uint32
	type = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_limit_activity_receive (uint32 id,uint32 type)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_LIMIT_ACTIVITY_RECEIVE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&type, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*杀人啦~~！！！！*/
int ProtocolUtil::unpack_kill_man (ByteArray &bytes ,string &killer,string &killer_name,string &victim,string &victim_name)
{
	int ret=0;
	//String
	killer = bytes.readUTF();
	//String
	killer_name = bytes.readUTF();
	//String
	victim = bytes.readUTF();
	//String
	victim_name = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_kill_man (char const*killer,char const*killer_name,char const*victim,char const*victim_name)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KILL_MAN);
	
	m_bytes.writeUTF(killer);
	m_bytes.writeUTF(killer_name);
	m_bytes.writeUTF(victim);
	m_bytes.writeUTF(victim_name);
	SendToServer(m_bytes);
	return 0;
}

/*玩家升级*/
int ProtocolUtil::unpack_player_upgrade (ByteArray &bytes ,uint32 &guid)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_player_upgrade (uint32 guid)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_PLAYER_UPGRADE);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*仓库存钱*/
int ProtocolUtil::unpack_warehouse_save_money (ByteArray &bytes ,int32 &money,int32 &money_gold,int32 &money_bills)
{
	int ret=0;
	//int32
	money = bytes.readInt();
	//int32
	money_gold = bytes.readInt();
	//int32
	money_bills = bytes.readInt();
	return ret;
}

int ProtocolUtil::send_warehouse_save_money (int32 money,int32 money_gold,int32 money_bills)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WAREHOUSE_SAVE_MONEY);
	
	m_bytes.writeBytes((uint8*)&money, sizeof(int32));
	m_bytes.writeBytes((uint8*)&money_gold, sizeof(int32));
	m_bytes.writeBytes((uint8*)&money_bills, sizeof(int32));
	SendToServer(m_bytes);
	return 0;
}

/*仓库取钱*/
int ProtocolUtil::unpack_warehouse_take_money (ByteArray &bytes ,int32 &money,int32 &money_gold,int32 &money_bills)
{
	int ret=0;
	//int32
	money = bytes.readInt();
	//int32
	money_gold = bytes.readInt();
	//int32
	money_bills = bytes.readInt();
	return ret;
}

int ProtocolUtil::send_warehouse_take_money (int32 money,int32 money_gold,int32 money_bills)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WAREHOUSE_TAKE_MONEY);
	
	m_bytes.writeBytes((uint8*)&money, sizeof(int32));
	m_bytes.writeBytes((uint8*)&money_gold, sizeof(int32));
	m_bytes.writeBytes((uint8*)&money_bills, sizeof(int32));
	SendToServer(m_bytes);
	return 0;
}

/*使用元宝操作某事*/
int ProtocolUtil::unpack_use_gold_opt (ByteArray &bytes ,uint8 &type,string &param)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	//String
	param = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_use_gold_opt (uint8 type,char const*param)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_USE_GOLD_OPT);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeUTF(param);
	SendToServer(m_bytes);
	return 0;
}

/*使用铜钱操作某事*/
int ProtocolUtil::unpack_use_silver_opt (ByteArray &bytes ,uint8 &type)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_use_silver_opt (uint8 type)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_USE_SILVER_OPT);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*后台弹窗*/
int ProtocolUtil::unpack_gm_rightfloat (ByteArray &bytes ,uint32 &id,uint32 &end_time,uint32 &zone1,uint32 &zone2,uint32 &zone3,string &subject,string &content,string &link,uint8 &mode)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	//uint32
	end_time = bytes.readUnsignedInt();
	//uint32
	zone1 = bytes.readUnsignedInt();
	//uint32
	zone2 = bytes.readUnsignedInt();
	//uint32
	zone3 = bytes.readUnsignedInt();
	//String
	subject = bytes.readUTF();
	//String
	content = bytes.readUTF();
	//String
	link = bytes.readUTF();
	//uint8
	mode = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_gm_rightfloat (uint32 id,uint32 end_time,uint32 zone1,uint32 zone2,uint32 zone3,char const*subject,char const*content,char const*link,uint8 mode)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_GM_RIGHTFLOAT);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&end_time, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&zone1, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&zone2, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&zone3, sizeof(uint32));
	m_bytes.writeUTF(subject);
	m_bytes.writeUTF(content);
	m_bytes.writeUTF(link);
	m_bytes.writeBytes((uint8*)&mode, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*删除某条后台弹窗*/
int ProtocolUtil::unpack_del_gm_rightfloat (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_del_gm_rightfloat (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_DEL_GM_RIGHTFLOAT);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*应用服同步时间*/
int ProtocolUtil::unpack_sync_mstime_app (ByteArray &bytes ,uint32 &mstime_now,uint32 &time_now,uint32 &open_time)
{
	int ret=0;
	//uint32
	mstime_now = bytes.readUnsignedInt();
	//uint32
	time_now = bytes.readUnsignedInt();
	//uint32
	open_time = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_sync_mstime_app (uint32 mstime_now,uint32 time_now,uint32 open_time)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_SYNC_MSTIME_APP);
	
	m_bytes.writeBytes((uint8*)&mstime_now, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&time_now, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&open_time, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*玩家打开某个窗口*/
int ProtocolUtil::unpack_open_window (ByteArray &bytes ,uint32 &window_type)
{
	int ret=0;
	//uint32
	window_type = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_open_window (uint32 window_type)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_OPEN_WINDOW);
	
	m_bytes.writeBytes((uint8*)&window_type, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*禁言操作*/
int ProtocolUtil::unpack_player_gag (ByteArray &bytes ,string &player_id,uint32 &end_time,string &content)
{
	int ret=0;
	//String
	player_id = bytes.readUTF();
	//uint32
	end_time = bytes.readUnsignedInt();
	//String
	content = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_player_gag (char const*player_id,uint32 end_time,char const*content)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PLAYER_GAG);
	
	m_bytes.writeUTF(player_id);
	m_bytes.writeBytes((uint8*)&end_time, sizeof(uint32));
	m_bytes.writeUTF(content);
	SendToServer(m_bytes);
	return 0;
}

/*踢人操作*/
int ProtocolUtil::unpack_player_kicking (ByteArray &bytes ,string &player_id)
{
	int ret=0;
	//String
	player_id = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_player_kicking (char const*player_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PLAYER_KICKING);
	
	m_bytes.writeUTF(player_id);
	SendToServer(m_bytes);
	return 0;
}

/*合服通知*/
int ProtocolUtil::unpack_merge_server_msg (ByteArray &bytes ,string &merge_host,uint32 &merge_port,string &merge_key,uint32 &merge_type,uint32 &reserve,uint32 &reserve2)
{
	int ret=0;
	//String
	merge_host = bytes.readUTF();
	//uint32
	merge_port = bytes.readUnsignedInt();
	//String
	merge_key = bytes.readUTF();
	//uint32
	merge_type = bytes.readUnsignedInt();
	//uint32
	reserve = bytes.readUnsignedInt();
	//uint32
	reserve2 = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_merge_server_msg (char const*merge_host,uint32 merge_port,char const*merge_key,uint32 merge_type,uint32 reserve,uint32 reserve2)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_MERGE_SERVER_MSG);
	
	m_bytes.writeUTF(merge_host);
	m_bytes.writeBytes((uint8*)&merge_port, sizeof(uint32));
	m_bytes.writeUTF(merge_key);
	m_bytes.writeBytes((uint8*)&merge_type, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&reserve, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&reserve2, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*获取排行信息*/
int ProtocolUtil::unpack_rank_list_query (ByteArray &bytes ,uint32 &call_back_id,uint8 &rank_list_type,uint16 &start_index,uint16 &end_index)
{
	int ret=0;
	//uint32
	call_back_id = bytes.readUnsignedInt();
	//uint8
	rank_list_type = bytes.readUnsignedByte();
	//uint16
	start_index = bytes.readUnsignedShort();
	//uint16
	end_index = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_rank_list_query (uint32 call_back_id,uint8 rank_list_type,uint16 start_index,uint16 end_index)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RANK_LIST_QUERY);
	
	m_bytes.writeBytes((uint8*)&call_back_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&rank_list_type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&start_index, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&end_index, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}


/*客户端热更场景模块后获取uint*/
int ProtocolUtil::unpack_client_update_scened (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_client_update_scened ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CLIENT_UPDATE_SCENED);
	
	SendToServer(m_bytes);
	return 0;
}


/*战利品拾取*/
int ProtocolUtil::unpack_loot_select (ByteArray &bytes ,uint16 &x,uint16 &y)
{
	int ret=0;
	//uint16
	x = bytes.readUnsignedShort();
	//uint16
	y = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_loot_select (uint16 x,uint16 y)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_LOOT_SELECT);
	
	m_bytes.writeBytes((uint8*)&x, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&y, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*通知登录服把玩家传回游戏服*/
int ProtocolUtil::unpack_goback_to_game_server (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_goback_to_game_server ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GOBACK_TO_GAME_SERVER);
	
	SendToServer(m_bytes);
	return 0;
}

/*客户端把比赛人员数据传给比赛服*/
int ProtocolUtil::unpack_world_war_CS_player_info (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_world_war_CS_player_info ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WORLD_WAR_CS_PLAYER_INFO);
	
	SendToServer(m_bytes);
	return 0;
}

/*玩家加入或者离开某服务器*/
int ProtocolUtil::unpack_join_or_leave_server (ByteArray &bytes ,uint8 &join_or_leave,uint8 &server_type,uint32 &server_pid,uint32 &server_fd,uint32 &time)
{
	int ret=0;
	//uint8
	join_or_leave = bytes.readUnsignedByte();
	//uint8
	server_type = bytes.readUnsignedByte();
	//uint32
	server_pid = bytes.readUnsignedInt();
	//uint32
	server_fd = bytes.readUnsignedInt();
	//uint32
	time = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_join_or_leave_server (uint8 join_or_leave,uint8 server_type,uint32 server_pid,uint32 server_fd,uint32 time)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_JOIN_OR_LEAVE_SERVER);
	
	m_bytes.writeBytes((uint8*)&join_or_leave, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&server_type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&server_pid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&server_fd, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&time, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}


/*客户端订阅信息*/
int ProtocolUtil::unpack_clientSubscription (ByteArray &bytes ,uint32 &guid)
{
	int ret=0;
	//uint32
	guid = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_clientSubscription (uint32 guid)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_CLIENTSUBSCRIPTION);
	
	m_bytes.writeBytes((uint8*)&guid, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}


/*角色更改信息*/
int ProtocolUtil::unpack_char_update_info (ByteArray &bytes , char_create_info **info)
{
	int ret=0;
	//char_create_info
	*info = (char_create_info*)bytes.cur_data();
	bytes.position(bytes.position() + sizeof(char_create_info));
	return ret;
}

int ProtocolUtil::send_char_update_info (char_create_info *info)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHAR_UPDATE_INFO);
	
	m_bytes.writeBytes((uint8*)info, sizeof(char_create_info));
	SendToServer(m_bytes);
	return 0;
}

/*通知客户端观察者的视角*/
int ProtocolUtil::unpack_notice_watcher_map_info (ByteArray &bytes ,string &wather_guid,uint32 &map_id,uint32 &instance_id)
{
	int ret=0;
	//String
	wather_guid = bytes.readUTF();
	//uint32
	map_id = bytes.readUnsignedInt();
	//uint32
	instance_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_notice_watcher_map_info (char const*wather_guid,uint32 map_id,uint32 instance_id)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_NOTICE_WATCHER_MAP_INFO);
	
	m_bytes.writeUTF(wather_guid);
	m_bytes.writeBytes((uint8*)&map_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&instance_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*客户端订阅对象信息*/
int ProtocolUtil::unpack_modify_watch (ByteArray &bytes ,uint8 &opt,uint32 &cid,string &key)
{
	int ret=0;
	//uint8
	opt = bytes.readUnsignedByte();
	//uint32
	cid = bytes.readUnsignedInt();
	//String
	key = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_modify_watch (uint8 opt,uint32 cid,char const*key)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_MODIFY_WATCH);
	
	m_bytes.writeBytes((uint8*)&opt, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&cid, sizeof(uint32));
	m_bytes.writeUTF(key);
	SendToServer(m_bytes);
	return 0;
}

/*跨服传送*/
int ProtocolUtil::unpack_kuafu_chuansong (ByteArray &bytes ,string &str_data,string &watcher_guid,uint32 &reserve)
{
	int ret=0;
	//String
	str_data = bytes.readUTF();
	//String
	watcher_guid = bytes.readUTF();
	//uint32
	reserve = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_kuafu_chuansong (char const*str_data,char const*watcher_guid,uint32 reserve)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_KUAFU_CHUANSONG);
	
	m_bytes.writeUTF(str_data);
	m_bytes.writeUTF(watcher_guid);
	m_bytes.writeBytes((uint8*)&reserve, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*显示当前装备*/
int ProtocolUtil::unpack_show_suit (ByteArray &bytes ,uint8 &position)
{
	int ret=0;
	//uint8
	position = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_show_suit (uint8 position)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SHOW_SUIT);
	
	m_bytes.writeBytes((uint8*)&position, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*显示当前坐标*/
int ProtocolUtil::unpack_show_position (ByteArray &bytes ,uint8 &channel)
{
	int ret=0;
	//uint8
	channel = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_show_position (uint8 channel)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SHOW_POSITION);
	
	m_bytes.writeBytes((uint8*)&channel, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*元宝复活*/
int ProtocolUtil::unpack_gold_respawn (ByteArray &bytes ,uint8 &useGold)
{
	int ret=0;
	//uint8
	useGold = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_gold_respawn (uint8 useGold)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GOLD_RESPAWN);
	
	m_bytes.writeBytes((uint8*)&useGold, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*野外死亡倒计时*/
int ProtocolUtil::unpack_field_death_cooldown (ByteArray &bytes ,uint8 &type,uint32 &value,string &killername,uint16 &cooldown,string &params)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	//uint32
	value = bytes.readUnsignedInt();
	//String
	killername = bytes.readUTF();
	//uint16
	cooldown = bytes.readUnsignedShort();
	//String
	params = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_field_death_cooldown (uint8 type,uint32 value,char const*killername,uint16 cooldown,char const*params)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_FIELD_DEATH_COOLDOWN);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&value, sizeof(uint32));
	m_bytes.writeUTF(killername);
	m_bytes.writeBytes((uint8*)&cooldown, sizeof(uint16));
	m_bytes.writeUTF(params);
	SendToServer(m_bytes);
	return 0;
}

/*商城购买*/
int ProtocolUtil::unpack_mall_buy (ByteArray &bytes ,uint32 &id,uint32 &count,uint32 &time)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	//uint32
	count = bytes.readUnsignedInt();
	//uint32
	time = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_mall_buy (uint32 id,uint32 count,uint32 time)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_MALL_BUY);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&time, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*强化*/
int ProtocolUtil::unpack_strength (ByteArray &bytes ,uint8 &part)
{
	int ret=0;
	//uint8
	part = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_strength (uint8 part)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_STRENGTH);
	
	m_bytes.writeBytes((uint8*)&part, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*强化成功*/
int ProtocolUtil::unpack_strength_success (ByteArray &bytes ,uint16 &level)
{
	int ret=0;
	//uint16
	level = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_strength_success (uint16 level)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_STRENGTH_SUCCESS);
	
	m_bytes.writeBytes((uint8*)&level, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*强制进入*/
int ProtocolUtil::unpack_forceInto (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_forceInto ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FORCEINTO);
	
	SendToServer(m_bytes);
	return 0;
}

/*创建帮派*/
int ProtocolUtil::unpack_create_faction (ByteArray &bytes ,string &name,uint8 &icon)
{
	int ret=0;
	//String
	name = bytes.readUTF();
	//uint8
	icon = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_create_faction (char const*name,uint8 icon)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CREATE_FACTION);
	
	m_bytes.writeUTF(name);
	m_bytes.writeBytes((uint8*)&icon, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*升级帮派*/
int ProtocolUtil::unpack_faction_upgrade (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_faction_upgrade ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FACTION_UPGRADE);
	
	SendToServer(m_bytes);
	return 0;
}

/*申请加入帮派*/
int ProtocolUtil::unpack_faction_join (ByteArray &bytes ,string &id)
{
	int ret=0;
	//String
	id = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_faction_join (char const*id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FACTION_JOIN);
	
	m_bytes.writeUTF(id);
	SendToServer(m_bytes);
	return 0;
}

/*申请升级技能*/
int ProtocolUtil::unpack_raise_base_spell (ByteArray &bytes ,uint8 &raiseType,uint16 &spellId)
{
	int ret=0;
	//uint8
	raiseType = bytes.readUnsignedByte();
	//uint16
	spellId = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_raise_base_spell (uint8 raiseType,uint16 spellId)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RAISE_BASE_SPELL);
	
	m_bytes.writeBytes((uint8*)&raiseType, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&spellId, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*申请升阶愤怒技能*/
int ProtocolUtil::unpack_upgrade_anger_spell (ByteArray &bytes ,uint16 &spellId)
{
	int ret=0;
	//uint16
	spellId = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_upgrade_anger_spell (uint16 spellId)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_UPGRADE_ANGER_SPELL);
	
	m_bytes.writeBytes((uint8*)&spellId, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*申请升级坐骑*/
int ProtocolUtil::unpack_raise_mount (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_raise_mount ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RAISE_MOUNT);
	
	SendToServer(m_bytes);
	return 0;
}

/*申请升阶坐骑*/
int ProtocolUtil::unpack_upgrade_mount (ByteArray &bytes ,uint8 &useItem)
{
	int ret=0;
	//uint8
	useItem = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_upgrade_mount (uint8 useItem)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_UPGRADE_MOUNT);
	
	m_bytes.writeBytes((uint8*)&useItem, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*申请一键升阶坐骑*/
int ProtocolUtil::unpack_upgrade_mount_one_step (ByteArray &bytes ,uint8 &useItem)
{
	int ret=0;
	//uint8
	useItem = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_upgrade_mount_one_step (uint8 useItem)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_UPGRADE_MOUNT_ONE_STEP);
	
	m_bytes.writeBytes((uint8*)&useItem, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*申请解锁幻化坐骑*/
int ProtocolUtil::unpack_illusion_mount_active (ByteArray &bytes ,uint16 &illuId)
{
	int ret=0;
	//uint16
	illuId = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_illusion_mount_active (uint16 illuId)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ILLUSION_MOUNT_ACTIVE);
	
	m_bytes.writeBytes((uint8*)&illuId, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*申请幻化坐骑*/
int ProtocolUtil::unpack_illusion_mount (ByteArray &bytes ,uint16 &illuId)
{
	int ret=0;
	//uint16
	illuId = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_illusion_mount (uint16 illuId)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ILLUSION_MOUNT);
	
	m_bytes.writeBytes((uint8*)&illuId, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*申请骑乘*/
int ProtocolUtil::unpack_ride_mount (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_ride_mount ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RIDE_MOUNT);
	
	SendToServer(m_bytes);
	return 0;
}


/*宝石*/
int ProtocolUtil::unpack_gem (ByteArray &bytes ,uint8 &part)
{
	int ret=0;
	//uint8
	part = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_gem (uint8 part)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GEM);
	
	m_bytes.writeBytes((uint8*)&part, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*请求切换模式*/
int ProtocolUtil::unpack_change_battle_mode (ByteArray &bytes ,uint8 &mode)
{
	int ret=0;
	//uint8
	mode = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_change_battle_mode (uint8 mode)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHANGE_BATTLE_MODE);
	
	m_bytes.writeBytes((uint8*)&mode, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}


/*激活神兵*/
int ProtocolUtil::unpack_divine_active (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_divine_active (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DIVINE_ACTIVE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*激活神兵*/
int ProtocolUtil::unpack_divine_uplev (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_divine_uplev (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DIVINE_UPLEV);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*切换神兵*/
int ProtocolUtil::unpack_divine_switch (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_divine_switch (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DIVINE_SWITCH);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*请求跳跃*/
int ProtocolUtil::unpack_jump_start (ByteArray &bytes ,uint16 &pos_x,uint16 &pos_y)
{
	int ret=0;
	//uint16
	pos_x = bytes.readUnsignedShort();
	//uint16
	pos_y = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_jump_start (uint16 pos_x,uint16 pos_y)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_JUMP_START);
	
	m_bytes.writeBytes((uint8*)&pos_x, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&pos_y, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*请求进入vip副本*/
int ProtocolUtil::unpack_enter_vip_instance (ByteArray &bytes ,uint16 &id,uint8 &hard)
{
	int ret=0;
	//uint16
	id = bytes.readUnsignedShort();
	//uint8
	hard = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_enter_vip_instance (uint16 id,uint8 hard)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ENTER_VIP_INSTANCE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&hard, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*请求扫荡vip副本*/
int ProtocolUtil::unpack_sweep_vip_instance (ByteArray &bytes ,uint16 &id)
{
	int ret=0;
	//uint16
	id = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_sweep_vip_instance (uint16 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SWEEP_VIP_INSTANCE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*进行挂机*/
int ProtocolUtil::unpack_hang_up (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_hang_up ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_HANG_UP);
	
	SendToServer(m_bytes);
	return 0;
}

/*进行挂机设置*/
int ProtocolUtil::unpack_hang_up_setting (ByteArray &bytes ,uint32 &value0,uint32 &value1,uint32 &value2,uint32 &value3)
{
	int ret=0;
	//uint32
	value0 = bytes.readUnsignedInt();
	//uint32
	value1 = bytes.readUnsignedInt();
	//uint32
	value2 = bytes.readUnsignedInt();
	//uint32
	value3 = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_hang_up_setting (uint32 value0,uint32 value1,uint32 value2,uint32 value3)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_HANG_UP_SETTING);
	
	m_bytes.writeBytes((uint8*)&value0, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&value1, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&value2, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&value3, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*请求进入试炼塔副本*/
int ProtocolUtil::unpack_enter_trial_instance (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_enter_trial_instance ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ENTER_TRIAL_INSTANCE);
	
	SendToServer(m_bytes);
	return 0;
}

/*扫荡试炼塔副本*/
int ProtocolUtil::unpack_sweep_trial_instance (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_sweep_trial_instance ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SWEEP_TRIAL_INSTANCE);
	
	SendToServer(m_bytes);
	return 0;
}

/*重置试炼塔*/
int ProtocolUtil::unpack_reset_trial_instance (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_reset_trial_instance ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RESET_TRIAL_INSTANCE);
	
	SendToServer(m_bytes);
	return 0;
}

/*扫荡副本奖励*/
int ProtocolUtil::unpack_sweep_instance_reward (ByteArray &bytes ,uint8 &inst_sub_type,uint8 &data1,uint8 &data2,uint8 &data3, vector< item_reward_info > &list)
{
	int ret=0;
	//uint8
	inst_sub_type = bytes.readUnsignedByte();
	//uint8
	data1 = bytes.readUnsignedByte();
	//uint8
	data2 = bytes.readUnsignedByte();
	//uint8
	data3 = bytes.readUnsignedByte();
	//item_reward_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_sweep_instance_reward (uint8 inst_sub_type,uint8 data1,uint8 data2,uint8 data3,const vector< item_reward_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SWEEP_INSTANCE_REWARD);
	
	m_bytes.writeBytes((uint8*)&inst_sub_type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&data1, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&data2, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&data3, sizeof(uint8));
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*重进副本*/
int ProtocolUtil::unpack_reenter_instance (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_reenter_instance ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_REENTER_INSTANCE);
	
	SendToServer(m_bytes);
	return 0;
}

/*走马灯信息*/
int ProtocolUtil::unpack_merry_go_round (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_merry_go_round ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_MERRY_GO_ROUND);
	
	SendToServer(m_bytes);
	return 0;
}

/*添加好友*/
int ProtocolUtil::unpack_social_add_friend (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_social_add_friend (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_ADD_FRIEND);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*同意添加好友*/
int ProtocolUtil::unpack_social_sureadd_friend (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_social_sureadd_friend (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_SUREADD_FRIEND);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*赠送礼物*/
int ProtocolUtil::unpack_social_gift_friend (ByteArray &bytes ,string &guid, vector< item_reward_info > &gift)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//item_reward_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_social_gift_friend (char const*guid,const vector< item_reward_info > &gift )
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_GIFT_FRIEND);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeShort(gift .size());
	for (auto it:gift)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*推荐好友列表*/
int ProtocolUtil::unpack_social_recommend_friend (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_social_recommend_friend ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_RECOMMEND_FRIEND);
	
	SendToServer(m_bytes);
	return 0;
}

/*推荐好友列表*/
int ProtocolUtil::unpack_social_get_recommend_friend (ByteArray &bytes , vector< social_friend_info > &list)
{
	int ret=0;
	//social_friend_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_social_get_recommend_friend (const vector< social_friend_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SOCIAL_GET_RECOMMEND_FRIEND);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*复仇*/
int ProtocolUtil::unpack_social_revenge_enemy (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_social_revenge_enemy (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_REVENGE_ENEMY);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*删除好友*/
int ProtocolUtil::unpack_social_del_friend (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_social_del_friend (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_DEL_FRIEND);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*回城*/
int ProtocolUtil::unpack_teleport_main_city (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_teleport_main_city ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TELEPORT_MAIN_CITY);
	
	SendToServer(m_bytes);
	return 0;
}

/*不同频道聊天*/
int ProtocolUtil::unpack_chat_by_channel (ByteArray &bytes ,uint8 &channel,string &content)
{
	int ret=0;
	//uint8
	channel = bytes.readUnsignedByte();
	//String
	content = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_chat_by_channel (uint8 channel,char const*content)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHAT_BY_CHANNEL);
	
	m_bytes.writeBytes((uint8*)&channel, sizeof(uint8));
	m_bytes.writeUTF(content);
	SendToServer(m_bytes);
	return 0;
}

/*发送聊天*/
int ProtocolUtil::unpack_send_chat (ByteArray &bytes ,uint8 &channel,string &guid,uint8 &title,string &name,uint8 &vip,uint8 &zs,uint8 &lvl,uint8 &gender,string &content,string &to_guid,string &faction_guid)
{
	int ret=0;
	//uint8
	channel = bytes.readUnsignedByte();
	//String
	guid = bytes.readUTF();
	//uint8
	title = bytes.readUnsignedByte();
	//String
	name = bytes.readUTF();
	//uint8
	vip = bytes.readUnsignedByte();
	//uint8
	zs = bytes.readUnsignedByte();
	//uint8
	lvl = bytes.readUnsignedByte();
	//uint8
	gender = bytes.readUnsignedByte();
	//String
	content = bytes.readUTF();
	//String
	to_guid = bytes.readUTF();
	//String
	faction_guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_send_chat (uint8 channel,char const*guid,uint8 title,char const*name,uint8 vip,uint8 zs,uint8 lvl,uint8 gender,char const*content,char const*to_guid,char const*faction_guid)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SEND_CHAT);
	
	m_bytes.writeBytes((uint8*)&channel, sizeof(uint8));
	m_bytes.writeUTF(guid);
	m_bytes.writeBytes((uint8*)&title, sizeof(uint8));
	m_bytes.writeUTF(name);
	m_bytes.writeBytes((uint8*)&vip, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&zs, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&lvl, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&gender, sizeof(uint8));
	m_bytes.writeUTF(content);
	m_bytes.writeUTF(to_guid);
	m_bytes.writeUTF(faction_guid);
	SendToServer(m_bytes);
	return 0;
}

/*清空申请列表*/
int ProtocolUtil::unpack_social_clear_apply (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_social_clear_apply ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_CLEAR_APPLY);
	
	SendToServer(m_bytes);
	return 0;
}

/*设置拒绝接受消息*/
int ProtocolUtil::unpack_msg_decline (ByteArray &bytes ,uint32 &value0,uint32 &value1)
{
	int ret=0;
	//uint32
	value0 = bytes.readUnsignedInt();
	//uint32
	value1 = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_msg_decline (uint32 value0,uint32 value1)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_MSG_DECLINE);
	
	m_bytes.writeBytes((uint8*)&value0, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&value1, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*帮派列表*/
int ProtocolUtil::unpack_faction_get_list_result (ByteArray &bytes , vector< faction_info > &list,uint8 &curpag,uint8 &maxpag)
{
	int ret=0;
	//faction_info
	ASSERT(false);
	//uint8
	curpag = bytes.readUnsignedByte();
	//uint8
	maxpag = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_faction_get_list_result (const vector< faction_info > &list ,uint8 curpag,uint8 maxpag)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_FACTION_GET_LIST_RESULT);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	m_bytes.writeBytes((uint8*)&curpag, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&maxpag, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*获取帮派列表*/
int ProtocolUtil::unpack_faction_getlist (ByteArray &bytes ,uint8 &page,uint8 &num,uint8 &grep)
{
	int ret=0;
	//uint8
	page = bytes.readUnsignedByte();
	//uint8
	num = bytes.readUnsignedByte();
	//uint8
	grep = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_faction_getlist (uint8 page,uint8 num,uint8 grep)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FACTION_GETLIST);
	
	m_bytes.writeBytes((uint8*)&page, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&num, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&grep, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*帮派管理*/
int ProtocolUtil::unpack_faction_manager (ByteArray &bytes ,uint8 &opt_type,uint16 &reserve_int1,uint16 &reserve_int2,string &reserve_str1,string &reserve_str2)
{
	int ret=0;
	//uint8
	opt_type = bytes.readUnsignedByte();
	//uint16
	reserve_int1 = bytes.readUnsignedShort();
	//uint16
	reserve_int2 = bytes.readUnsignedShort();
	//String
	reserve_str1 = bytes.readUTF();
	//String
	reserve_str2 = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_faction_manager (uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FACTION_MANAGER);
	
	m_bytes.writeBytes((uint8*)&opt_type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&reserve_int1, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&reserve_int2, sizeof(uint16));
	m_bytes.writeUTF(reserve_str1);
	m_bytes.writeUTF(reserve_str2);
	SendToServer(m_bytes);
	return 0;
}

/*帮派成员操作*/
int ProtocolUtil::unpack_faction_member_operate (ByteArray &bytes ,uint8 &opt_type,uint16 &reserve_int1,uint16 &reserve_int2,string &reserve_str1,string &reserve_str2)
{
	int ret=0;
	//uint8
	opt_type = bytes.readUnsignedByte();
	//uint16
	reserve_int1 = bytes.readUnsignedShort();
	//uint16
	reserve_int2 = bytes.readUnsignedShort();
	//String
	reserve_str1 = bytes.readUTF();
	//String
	reserve_str2 = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_faction_member_operate (uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FACTION_MEMBER_OPERATE);
	
	m_bytes.writeBytes((uint8*)&opt_type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&reserve_int1, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&reserve_int2, sizeof(uint16));
	m_bytes.writeUTF(reserve_str1);
	m_bytes.writeUTF(reserve_str2);
	SendToServer(m_bytes);
	return 0;
}

/*快速加入帮派*/
int ProtocolUtil::unpack_faction_fast_join (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_faction_fast_join ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FACTION_FAST_JOIN);
	
	SendToServer(m_bytes);
	return 0;
}

/*通过名字添加好友*/
int ProtocolUtil::unpack_social_add_friend_byname (ByteArray &bytes ,string &name)
{
	int ret=0;
	//String
	name = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_social_add_friend_byname (char const*name)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_ADD_FRIEND_BYNAME);
	
	m_bytes.writeUTF(name);
	SendToServer(m_bytes);
	return 0;
}

/*读邮件*/
int ProtocolUtil::unpack_read_mail (ByteArray &bytes ,uint16 &indx)
{
	int ret=0;
	//uint16
	indx = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_read_mail (uint16 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_READ_MAIL);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*领取邮件*/
int ProtocolUtil::unpack_pick_mail (ByteArray &bytes ,uint16 &indx)
{
	int ret=0;
	//uint16
	indx = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_pick_mail (uint16 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PICK_MAIL);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*删除邮件*/
int ProtocolUtil::unpack_remove_mail (ByteArray &bytes ,uint16 &indx)
{
	int ret=0;
	//uint16
	indx = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_remove_mail (uint16 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_REMOVE_MAIL);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*一键领取邮件*/
int ProtocolUtil::unpack_pick_mail_one_step (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_pick_mail_one_step ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PICK_MAIL_ONE_STEP);
	
	SendToServer(m_bytes);
	return 0;
}

/*一键删除邮件*/
int ProtocolUtil::unpack_remove_mail_one_step (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_remove_mail_one_step ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_REMOVE_MAIL_ONE_STEP);
	
	SendToServer(m_bytes);
	return 0;
}

/*屏蔽某人*/
int ProtocolUtil::unpack_block_chat (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_block_chat (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BLOCK_CHAT);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*取消屏蔽*/
int ProtocolUtil::unpack_cancel_block_chat (ByteArray &bytes ,uint8 &indx)
{
	int ret=0;
	//uint8
	indx = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_cancel_block_chat (uint8 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CANCEL_BLOCK_CHAT);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*使用需要广播的游戏对象*/
int ProtocolUtil::unpack_use_broadcast_gameobject (ByteArray &bytes ,uint32 &target)
{
	int ret=0;
	//uint32
	target = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_use_broadcast_gameobject (uint32 target)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_USE_BROADCAST_GAMEOBJECT);
	
	m_bytes.writeBytes((uint8*)&target, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*世界BOSS报名*/
int ProtocolUtil::unpack_world_boss_enroll (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_world_boss_enroll ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WORLD_BOSS_ENROLL);
	
	SendToServer(m_bytes);
	return 0;
}

/*世界BOSS挑战*/
int ProtocolUtil::unpack_world_boss_fight (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_world_boss_fight ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WORLD_BOSS_FIGHT);
	
	SendToServer(m_bytes);
	return 0;
}

/*换线*/
int ProtocolUtil::unpack_change_line (ByteArray &bytes ,uint32 &lineNo)
{
	int ret=0;
	//uint32
	lineNo = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_change_line (uint32 lineNo)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHANGE_LINE);
	
	m_bytes.writeBytes((uint8*)&lineNo, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*roll世界BOSS箱子*/
int ProtocolUtil::unpack_roll_world_boss_treasure (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_roll_world_boss_treasure ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ROLL_WORLD_BOSS_TREASURE);
	
	SendToServer(m_bytes);
	return 0;
}

/*roll点结果*/
int ProtocolUtil::unpack_roll_result (ByteArray &bytes ,uint8 &point,string &name,uint8 &isHighest,uint32 &timestamp,uint8 &rollid)
{
	int ret=0;
	//uint8
	point = bytes.readUnsignedByte();
	//String
	name = bytes.readUTF();
	//uint8
	isHighest = bytes.readUnsignedByte();
	//uint32
	timestamp = bytes.readUnsignedInt();
	//uint8
	rollid = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_roll_result (uint8 point,char const*name,uint8 isHighest,uint32 timestamp,uint8 rollid)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_ROLL_RESULT);
	
	m_bytes.writeBytes((uint8*)&point, sizeof(uint8));
	m_bytes.writeUTF(name);
	m_bytes.writeBytes((uint8*)&isHighest, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&timestamp, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&rollid, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*当前BOSS伤害排名*/
int ProtocolUtil::unpack_boss_rank (ByteArray &bytes ,uint8 &rankType, vector< rank_info > &rankList,uint8 &mine)
{
	int ret=0;
	//uint8
	rankType = bytes.readUnsignedByte();
	//rank_info
	ASSERT(false);
	//uint8
	mine = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_boss_rank (uint8 rankType,const vector< rank_info > &rankList ,uint8 mine)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_BOSS_RANK);
	
	m_bytes.writeBytes((uint8*)&rankType, sizeof(uint8));
	m_bytes.writeShort(rankList .size());
	for (auto it:rankList)
	{
		m_bytes.writeT(it);
	}
	m_bytes.writeBytes((uint8*)&mine, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*排行榜点赞*/
int ProtocolUtil::unpack_rank_add_like (ByteArray &bytes ,uint8 &type,string &guid)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_rank_add_like (uint8 type,char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RANK_ADD_LIKE);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*排行榜点赞结果*/
int ProtocolUtil::unpack_rank_add_like_result (ByteArray &bytes ,uint8 &type,string &guid,uint32 &num)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	//String
	guid = bytes.readUTF();
	//uint32
	num = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_rank_add_like_result (uint8 type,char const*guid,uint32 num)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_RANK_ADD_LIKE_RESULT);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeUTF(guid);
	m_bytes.writeBytes((uint8*)&num, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*进入资源副本*/
int ProtocolUtil::unpack_res_instance_enter (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_res_instance_enter (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RES_INSTANCE_ENTER);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*扫荡资源副本*/
int ProtocolUtil::unpack_res_instance_sweep (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_res_instance_sweep (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RES_INSTANCE_SWEEP);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*查看本地图的分线号*/
int ProtocolUtil::unpack_show_map_line (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_show_map_line ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SHOW_MAP_LINE);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回本地图的分线号信息*/
int ProtocolUtil::unpack_send_map_line (ByteArray &bytes , vector< line_info > &info)
{
	int ret=0;
	//line_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_send_map_line (const vector< line_info > &info )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SEND_MAP_LINE);
	
	m_bytes.writeShort(info .size());
	for (auto it:info)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*获得奖励提示*/
int ProtocolUtil::unpack_item_notice (ByteArray &bytes ,uint8 &showType, vector< item_reward_info > &list)
{
	int ret=0;
	//uint8
	showType = bytes.readUnsignedByte();
	//item_reward_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_item_notice (uint8 showType,const vector< item_reward_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_ITEM_NOTICE);
	
	m_bytes.writeBytes((uint8*)&showType, sizeof(uint8));
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*传送到某个世界地图*/
int ProtocolUtil::unpack_teleport_map (ByteArray &bytes ,uint32 &mapid,uint32 &lineNo)
{
	int ret=0;
	//uint32
	mapid = bytes.readUnsignedInt();
	//uint32
	lineNo = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_teleport_map (uint32 mapid,uint32 lineNo)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TELEPORT_MAP);
	
	m_bytes.writeBytes((uint8*)&mapid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&lineNo, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*传送到野外boss旁边*/
int ProtocolUtil::unpack_teleport_field_boss (ByteArray &bytes ,uint32 &mapid,uint32 &lineNo)
{
	int ret=0;
	//uint32
	mapid = bytes.readUnsignedInt();
	//uint32
	lineNo = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_teleport_field_boss (uint32 mapid,uint32 lineNo)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TELEPORT_FIELD_BOSS);
	
	m_bytes.writeBytes((uint8*)&mapid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&lineNo, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*活跃度奖励*/
int ProtocolUtil::unpack_get_activity_reward (ByteArray &bytes ,uint8 &id,uint8 &vip)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	//uint8
	vip = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_get_activity_reward (uint8 id,uint8 vip)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_ACTIVITY_REWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&vip, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*成就奖励*/
int ProtocolUtil::unpack_get_achieve_reward (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_get_achieve_reward (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_ACHIEVE_REWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*总成就奖励*/
int ProtocolUtil::unpack_get_achieve_all_reward (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_get_achieve_all_reward ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_ACHIEVE_ALL_REWARD);
	
	SendToServer(m_bytes);
	return 0;
}

/*装备称号*/
int ProtocolUtil::unpack_set_title (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_set_title (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SET_TITLE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*初始化称号*/
int ProtocolUtil::unpack_init_title (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_init_title (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_INIT_TITLE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*领取首充奖励*/
int ProtocolUtil::unpack_welfare_shouchong_reward (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_welfare_shouchong_reward ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_SHOUCHONG_REWARD);
	
	SendToServer(m_bytes);
	return 0;
}

/*每日签到奖励*/
int ProtocolUtil::unpack_welfare_checkin (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_welfare_checkin ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_CHECKIN);
	
	SendToServer(m_bytes);
	return 0;
}

/*累积签到奖励*/
int ProtocolUtil::unpack_welfare_checkin_all (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_welfare_checkin_all (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_CHECKIN_ALL);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*补签奖励*/
int ProtocolUtil::unpack_welfare_checkin_getback (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_welfare_checkin_getback (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_CHECKIN_GETBACK);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*等级奖励*/
int ProtocolUtil::unpack_welfare_level (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_welfare_level (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_LEVEL);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*活动找回奖励*/
int ProtocolUtil::unpack_welfare_active_getback (ByteArray &bytes ,uint8 &id,uint8 &best,uint16 &num)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	//uint8
	best = bytes.readUnsignedByte();
	//uint16
	num = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_welfare_active_getback (uint8 id,uint8 best,uint16 num)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_ACTIVE_GETBACK);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&best, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&num, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*领取任务奖励*/
int ProtocolUtil::unpack_pick_quest_reward (ByteArray &bytes ,uint8 &indx)
{
	int ret=0;
	//uint8
	indx = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_pick_quest_reward (uint8 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PICK_QUEST_REWARD);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*和npc对话*/
int ProtocolUtil::unpack_talk_with_npc (ByteArray &bytes ,uint16 &entry,uint16 &questId)
{
	int ret=0;
	//uint16
	entry = bytes.readUnsignedShort();
	//uint16
	questId = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_talk_with_npc (uint16 entry,uint16 questId)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TALK_WITH_NPC);
	
	m_bytes.writeBytes((uint8*)&entry, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&questId, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*使用虚拟物品*/
int ProtocolUtil::unpack_use_virtual_item (ByteArray &bytes ,uint16 &entry)
{
	int ret=0;
	//uint16
	entry = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_use_virtual_item (uint16 entry)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_USE_VIRTUAL_ITEM);
	
	m_bytes.writeBytes((uint8*)&entry, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*领取任务章节奖励*/
int ProtocolUtil::unpack_pick_quest_chapter_reward (ByteArray &bytes ,uint8 &indx)
{
	int ret=0;
	//uint8
	indx = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_pick_quest_chapter_reward (uint8 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PICK_QUEST_CHAPTER_REWARD);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*3v3跨服匹配*/
int ProtocolUtil::unpack_kuafu_3v3_match (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_match ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_KUAFU_3V3_MATCH);
	
	SendToServer(m_bytes);
	return 0;
}

/*跨服开始匹配*/
int ProtocolUtil::unpack_kuafu_match_start (ByteArray &bytes ,uint8 &indx)
{
	int ret=0;
	//uint8
	indx = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_kuafu_match_start (uint8 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KUAFU_MATCH_START);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*3v3购买次数*/
int ProtocolUtil::unpack_kuafu_3v3_buytimes (ByteArray &bytes ,uint8 &num)
{
	int ret=0;
	//uint8
	num = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_buytimes (uint8 num)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_KUAFU_3V3_BUYTIMES);
	
	m_bytes.writeBytes((uint8*)&num, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*3v3每日活跃奖励*/
int ProtocolUtil::unpack_kuafu_3v3_dayreward (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_dayreward (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_KUAFU_3V3_DAYREWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*请求3v3排行榜*/
int ProtocolUtil::unpack_kuafu_3v3_getranlist (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_getranlist ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_KUAFU_3V3_GETRANLIST);
	
	SendToServer(m_bytes);
	return 0;
}

/*3v3排行榜结果列表*/
int ProtocolUtil::unpack_kuafu_3v3_ranlist (ByteArray &bytes ,string &list)
{
	int ret=0;
	//String
	list = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_ranlist (char const*list)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KUAFU_3V3_RANLIST);
	
	m_bytes.writeUTF(list);
	SendToServer(m_bytes);
	return 0;
}

/*福利所有奖励列表*/
int ProtocolUtil::unpack_welfare_getalllist_getback (ByteArray &bytes ,uint8 &best)
{
	int ret=0;
	//uint8
	best = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_welfare_getalllist_getback (uint8 best)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_GETALLLIST_GETBACK);
	
	m_bytes.writeBytes((uint8*)&best, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*奖励列表*/
int ProtocolUtil::unpack_welfare_rewardlist_getback (ByteArray &bytes ,string &list,string &cost)
{
	int ret=0;
	//String
	list = bytes.readUTF();
	//String
	cost = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_welfare_rewardlist_getback (char const*list,char const*cost)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_WELFARE_REWARDLIST_GETBACK);
	
	m_bytes.writeUTF(list);
	m_bytes.writeUTF(cost);
	SendToServer(m_bytes);
	return 0;
}

/*一键领取所有福利*/
int ProtocolUtil::unpack_welfare_getall_getback (ByteArray &bytes ,uint8 &best)
{
	int ret=0;
	//uint8
	best = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_welfare_getall_getback (uint8 best)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_GETALL_GETBACK);
	
	m_bytes.writeBytes((uint8*)&best, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*请求3v3排行榜自己的名次*/
int ProtocolUtil::unpack_kuafu_3v3_getmyrank (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_getmyrank ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_KUAFU_3V3_GETMYRANK);
	
	SendToServer(m_bytes);
	return 0;
}

/*3v3排行榜自己的名次结果*/
int ProtocolUtil::unpack_kuafu_3v3_myrank (ByteArray &bytes ,uint8 &rank)
{
	int ret=0;
	//uint8
	rank = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_myrank (uint8 rank)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KUAFU_3V3_MYRANK);
	
	m_bytes.writeBytes((uint8*)&rank, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*击杀数据*/
int ProtocolUtil::unpack_kuafu_3v3_kill_detail (ByteArray &bytes ,uint32 &indx1,uint32 &indx2)
{
	int ret=0;
	//uint32
	indx1 = bytes.readUnsignedInt();
	//uint32
	indx2 = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_kill_detail (uint32 indx1,uint32 indx2)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KUAFU_3V3_KILL_DETAIL);
	
	m_bytes.writeBytes((uint8*)&indx1, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&indx2, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*跨服匹配等待数据*/
int ProtocolUtil::unpack_kuafu_3v3_wait_info (ByteArray &bytes , vector< wait_info > &list)
{
	int ret=0;
	//wait_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_wait_info (const vector< wait_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KUAFU_3V3_WAIT_INFO);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*取消匹配*/
int ProtocolUtil::unpack_kuafu_3v3_cancel_match (ByteArray &bytes ,uint32 &type)
{
	int ret=0;
	//uint32
	type = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_cancel_match (uint32 type)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_KUAFU_3V3_CANCEL_MATCH);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*匹配到人&接受或者拒绝*/
int ProtocolUtil::unpack_kuafu_3v3_match_oper (ByteArray &bytes ,uint32 &oper)
{
	int ret=0;
	//uint32
	oper = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_match_oper (uint32 oper)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_KUAFU_3V3_MATCH_OPER);
	
	m_bytes.writeBytes((uint8*)&oper, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*拒绝比赛*/
int ProtocolUtil::unpack_kuafu_3v3_decline_match (ByteArray &bytes ,uint32 &type)
{
	int ret=0;
	//uint32
	type = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_kuafu_3v3_decline_match (uint32 type)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KUAFU_3V3_DECLINE_MATCH);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*仙府夺宝跨服匹配*/
int ProtocolUtil::unpack_kuafu_xianfu_match (ByteArray &bytes ,uint8 &indx)
{
	int ret=0;
	//uint8
	indx = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_kuafu_xianfu_match (uint8 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_KUAFU_XIANFU_MATCH);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*单方跨服匹配等待*/
int ProtocolUtil::unpack_kuafu_match_wait (ByteArray &bytes ,uint8 &type,uint8 &target,uint8 &count,uint32 &data,string &params)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	//uint8
	target = bytes.readUnsignedByte();
	//uint8
	count = bytes.readUnsignedByte();
	//uint32
	data = bytes.readUnsignedInt();
	//String
	params = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_kuafu_match_wait (uint8 type,uint8 target,uint8 count,uint32 data,char const*params)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KUAFU_MATCH_WAIT);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&target, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&count, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&data, sizeof(uint32));
	m_bytes.writeUTF(params);
	SendToServer(m_bytes);
	return 0;
}

/*仙府夺宝小地图信息*/
int ProtocolUtil::unpack_kuafu_xianfu_minimap_info (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_kuafu_xianfu_minimap_info ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_KUAFU_XIANFU_MINIMAP_INFO);
	
	SendToServer(m_bytes);
	return 0;
}

/*购买仙府进入券*/
int ProtocolUtil::unpack_buy_xianfu_item (ByteArray &bytes ,uint8 &type,uint8 &indx,uint16 &count)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	//uint8
	indx = bytes.readUnsignedByte();
	//uint16
	count = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_buy_xianfu_item (uint8 type,uint8 indx,uint16 count)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BUY_XIANFU_ITEM);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&count, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*随机复活*/
int ProtocolUtil::unpack_xianfu_random_respawn (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_xianfu_random_respawn ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_XIANFU_RANDOM_RESPAWN);
	
	SendToServer(m_bytes);
	return 0;
}

/*斗剑台挑战*/
int ProtocolUtil::unpack_doujiantai_fight (ByteArray &bytes ,uint16 &rank)
{
	int ret=0;
	//uint16
	rank = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_doujiantai_fight (uint16 rank)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DOUJIANTAI_FIGHT);
	
	m_bytes.writeBytes((uint8*)&rank, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*斗剑台购买次数*/
int ProtocolUtil::unpack_doujiantai_buytime (ByteArray &bytes ,uint8 &num)
{
	int ret=0;
	//uint8
	num = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_doujiantai_buytime (uint8 num)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DOUJIANTAI_BUYTIME);
	
	m_bytes.writeBytes((uint8*)&num, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*斗剑台清理CD*/
int ProtocolUtil::unpack_doujiantai_clearcd (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_doujiantai_clearcd ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DOUJIANTAI_CLEARCD);
	
	SendToServer(m_bytes);
	return 0;
}

/*斗剑台首胜奖励*/
int ProtocolUtil::unpack_doujiantai_first_reward (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_doujiantai_first_reward (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DOUJIANTAI_FIRST_REWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*斗剑台挑战对手信息*/
int ProtocolUtil::unpack_doujiantai_get_enemys_info (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_doujiantai_get_enemys_info ()
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_DOUJIANTAI_GET_ENEMYS_INFO);
	
	SendToServer(m_bytes);
	return 0;
}

/*斗剑台排行榜*/
int ProtocolUtil::unpack_doujiantai_get_rank (ByteArray &bytes ,uint16 &startIdx,uint16 &endIdx)
{
	int ret=0;
	//uint16
	startIdx = bytes.readUnsignedShort();
	//uint16
	endIdx = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_doujiantai_get_rank (uint16 startIdx,uint16 endIdx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DOUJIANTAI_GET_RANK);
	
	m_bytes.writeBytes((uint8*)&startIdx, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&endIdx, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*斗剑台刷新对手*/
int ProtocolUtil::unpack_doujiantai_refresh_enemys (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_doujiantai_refresh_enemys ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DOUJIANTAI_REFRESH_ENEMYS);
	
	SendToServer(m_bytes);
	return 0;
}

/*斗剑台三甲*/
int ProtocolUtil::unpack_doujiantai_top3 (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_doujiantai_top3 ()
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_DOUJIANTAI_TOP3);
	
	SendToServer(m_bytes);
	return 0;
}

/*使用跳点*/
int ProtocolUtil::unpack_use_jump_point (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_use_jump_point (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(MSG_USE_JUMP_POINT);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*出售物品*/
int ProtocolUtil::unpack_bag_item_sell (ByteArray &bytes ,string &item_guid,uint32 &count)
{
	int ret=0;
	//String
	item_guid = bytes.readUTF();
	//uint32
	count = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_bag_item_sell (char const*item_guid,uint32 count)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BAG_ITEM_SELL);
	
	m_bytes.writeUTF(item_guid);
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*整理物品*/
int ProtocolUtil::unpack_bag_item_sort (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_bag_item_sort ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BAG_ITEM_SORT);
	
	SendToServer(m_bytes);
	return 0;
}

/*提交日常任务*/
int ProtocolUtil::unpack_submit_quest_daily2 (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_submit_quest_daily2 ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SUBMIT_QUEST_DAILY2);
	
	SendToServer(m_bytes);
	return 0;
}

/*属性改变*/
int ProtocolUtil::unpack_attribute_changed (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_attribute_changed ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_ATTRIBUTE_CHANGED);
	
	SendToServer(m_bytes);
	return 0;
}

/*背包有更强装备*/
int ProtocolUtil::unpack_bag_find_equip_better (ByteArray &bytes ,uint32 &item_id,uint32 &pos)
{
	int ret=0;
	//uint32
	item_id = bytes.readUnsignedInt();
	//uint32
	pos = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_bag_find_equip_better (uint32 item_id,uint32 pos)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_BAG_FIND_EQUIP_BETTER);
	
	m_bytes.writeBytes((uint8*)&item_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&pos, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*模块解锁*/
int ProtocolUtil::unpack_module_active (ByteArray &bytes ,uint32 &moduleId)
{
	int ret=0;
	//uint32
	moduleId = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_module_active (uint32 moduleId)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_MODULE_ACTIVE);
	
	m_bytes.writeBytes((uint8*)&moduleId, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*领取日常任务奖励*/
int ProtocolUtil::unpack_pick_daily2_quest_reward (ByteArray &bytes ,uint8 &indx)
{
	int ret=0;
	//uint8
	indx = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_pick_daily2_quest_reward (uint8 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PICK_DAILY2_QUEST_REWARD);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*完成当前引导*/
int ProtocolUtil::unpack_finish_now_guide (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_finish_now_guide ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FINISH_NOW_GUIDE);
	
	SendToServer(m_bytes);
	return 0;
}

/*取得修炼场信息*/
int ProtocolUtil::unpack_get_cultivation_info (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_get_cultivation_info ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_CULTIVATION_INFO);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回修炼场信息*/
int ProtocolUtil::unpack_update_cultivation_info (ByteArray &bytes ,uint32 &start_time,uint32 &lost)
{
	int ret=0;
	//uint32
	start_time = bytes.readUnsignedInt();
	//uint32
	lost = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_update_cultivation_info (uint32 start_time,uint32 lost)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_UPDATE_CULTIVATION_INFO);
	
	m_bytes.writeBytes((uint8*)&start_time, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&lost, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*取得当前所有修炼场对手信息*/
int ProtocolUtil::unpack_get_cultivation_rivals_info (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_get_cultivation_rivals_info ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_CULTIVATION_RIVALS_INFO);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回修炼场对手信息*/
int ProtocolUtil::unpack_update_cultivation_rivals_info_list (ByteArray &bytes , vector< cultivation_rivals_info > &list)
{
	int ret=0;
	//cultivation_rivals_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_update_cultivation_rivals_info_list (const vector< cultivation_rivals_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_UPDATE_CULTIVATION_RIVALS_INFO_LIST);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*领取修炼场奖励*/
int ProtocolUtil::unpack_get_cultivation_reward (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_get_cultivation_reward ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_CULTIVATION_REWARD);
	
	SendToServer(m_bytes);
	return 0;
}

/*刷新修炼场对手*/
int ProtocolUtil::unpack_refresh_cultivation_rivals (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_refresh_cultivation_rivals ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_REFRESH_CULTIVATION_RIVALS);
	
	SendToServer(m_bytes);
	return 0;
}

/*掠夺修炼场对手*/
int ProtocolUtil::unpack_plunder_cultivation_rival (ByteArray &bytes ,uint32 &index)
{
	int ret=0;
	//uint32
	index = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_plunder_cultivation_rival (uint32 index)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PLUNDER_CULTIVATION_RIVAL);
	
	m_bytes.writeBytes((uint8*)&index, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*反击复仇修炼场对手*/
int ProtocolUtil::unpack_revenge_cultivation_rival (ByteArray &bytes ,uint32 &index)
{
	int ret=0;
	//uint32
	index = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_revenge_cultivation_rival (uint32 index)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_REVENGE_CULTIVATION_RIVAL);
	
	m_bytes.writeBytes((uint8*)&index, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*增加修炼场剩余挑战次数*/
int ProtocolUtil::unpack_buy_cultivation_left_plunder_count (ByteArray &bytes ,uint32 &count)
{
	int ret=0;
	//uint32
	count = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_buy_cultivation_left_plunder_count (uint32 count)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BUY_CULTIVATION_LEFT_PLUNDER_COUNT);
	
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*返回修炼场战斗结果*/
int ProtocolUtil::unpack_show_cultivation_result_list (ByteArray &bytes ,int32 &result,string &name, vector< item_reward_info > &list)
{
	int ret=0;
	//int32
	result = bytes.readInt();
	//String
	name = bytes.readUTF();
	//item_reward_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_show_cultivation_result_list (int32 result,char const*name,const vector< item_reward_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_CULTIVATION_RESULT_LIST);
	
	m_bytes.writeBytes((uint8*)&result, sizeof(int32));
	m_bytes.writeUTF(name);
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*领取登录大礼奖励*/
int ProtocolUtil::unpack_get_login_activity_reward (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_get_login_activity_reward (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_LOGIN_ACTIVITY_REWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*通知客户端释放蓄力技能*/
int ProtocolUtil::unpack_cast_spell_start (ByteArray &bytes ,uint32 &caster_guid,uint32 &target_guid,uint16 &spellid,string &data)
{
	int ret=0;
	//uint32
	caster_guid = bytes.readUnsignedInt();
	//uint32
	target_guid = bytes.readUnsignedInt();
	//uint16
	spellid = bytes.readUnsignedShort();
	//String
	data = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_cast_spell_start (uint32 caster_guid,uint32 target_guid,uint16 spellid,char const*data)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_CAST_SPELL_START);
	
	m_bytes.writeBytes((uint8*)&caster_guid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&target_guid, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&spellid, sizeof(uint16));
	m_bytes.writeUTF(data);
	SendToServer(m_bytes);
	return 0;
}

/*完成非强制引导的步骤*/
int ProtocolUtil::unpack_finish_optional_guide_step (ByteArray &bytes ,uint32 &guide_id,uint8 &step)
{
	int ret=0;
	//uint32
	guide_id = bytes.readUnsignedInt();
	//uint8
	step = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_finish_optional_guide_step (uint32 guide_id,uint8 step)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_FINISH_OPTIONAL_GUIDE_STEP);
	
	m_bytes.writeBytes((uint8*)&guide_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&step, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*执行接到任务以后的命令*/
int ProtocolUtil::unpack_execute_quest_cmd_after_accepted (ByteArray &bytes ,uint16 &indx)
{
	int ret=0;
	//uint16
	indx = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_execute_quest_cmd_after_accepted (uint16 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_EXECUTE_QUEST_CMD_AFTER_ACCEPTED);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*通知客户端显示属性*/
int ProtocolUtil::unpack_show_unit_attribute (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_show_unit_attribute ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_UNIT_ATTRIBUTE);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回家族*/
int ProtocolUtil::unpack_back_to_famity (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_back_to_famity ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BACK_TO_FAMITY);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回家族boss结果*/
int ProtocolUtil::unpack_faction_boss_send_result (ByteArray &bytes ,uint32 &result,uint32 &boss_id,uint32 &money)
{
	int ret=0;
	//uint32
	result = bytes.readUnsignedInt();
	//uint32
	boss_id = bytes.readUnsignedInt();
	//uint32
	money = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_faction_boss_send_result (uint32 result,uint32 boss_id,uint32 money)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_FACTION_BOSS_SEND_RESULT);
	
	m_bytes.writeBytes((uint8*)&result, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&boss_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&money, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*挑战boss*/
int ProtocolUtil::unpack_challange_boss (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_challange_boss ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CHALLANGE_BOSS);
	
	SendToServer(m_bytes);
	return 0;
}

/*领取离线奖励*/
int ProtocolUtil::unpack_pick_offline_reward (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_pick_offline_reward ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PICK_OFFLINE_REWARD);
	
	SendToServer(m_bytes);
	return 0;
}

/*离线奖励结果*/
int ProtocolUtil::unpack_offline_reward_result (ByteArray &bytes ,uint32 &reserve,uint32 &reserve2,uint32 &reserve3,uint32 &reserve4, vector< item_reward_info > &list)
{
	int ret=0;
	//uint32
	reserve = bytes.readUnsignedInt();
	//uint32
	reserve2 = bytes.readUnsignedInt();
	//uint32
	reserve3 = bytes.readUnsignedInt();
	//uint32
	reserve4 = bytes.readUnsignedInt();
	//item_reward_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_offline_reward_result (uint32 reserve,uint32 reserve2,uint32 reserve3,uint32 reserve4,const vector< item_reward_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_OFFLINE_REWARD_RESULT);
	
	m_bytes.writeBytes((uint8*)&reserve, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&reserve2, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&reserve3, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&reserve4, sizeof(uint32));
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*熔炼装备*/
int ProtocolUtil::unpack_smelting_equip (ByteArray &bytes ,string &pos_str)
{
	int ret=0;
	//String
	pos_str = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_smelting_equip (char const*pos_str)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SMELTING_EQUIP);
	
	m_bytes.writeUTF(pos_str);
	SendToServer(m_bytes);
	return 0;
}

/*上交装备*/
int ProtocolUtil::unpack_storehouse_hand_in (ByteArray &bytes ,string &pos_str)
{
	int ret=0;
	//String
	pos_str = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_storehouse_hand_in (char const*pos_str)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_STOREHOUSE_HAND_IN);
	
	m_bytes.writeUTF(pos_str);
	SendToServer(m_bytes);
	return 0;
}

/*兑换装备*/
int ProtocolUtil::unpack_storehouse_exchange (ByteArray &bytes ,uint32 &pos)
{
	int ret=0;
	//uint32
	pos = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_storehouse_exchange (uint32 pos)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_STOREHOUSE_EXCHANGE);
	
	m_bytes.writeBytes((uint8*)&pos, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*销毁装备*/
int ProtocolUtil::unpack_storehouse_destroy (ByteArray &bytes ,uint32 &pos)
{
	int ret=0;
	//uint32
	pos = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_storehouse_destroy (uint32 pos)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_STOREHOUSE_DESTROY);
	
	m_bytes.writeBytes((uint8*)&pos, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*赠送礼物*/
int ProtocolUtil::unpack_send_faction_gift (ByteArray &bytes , vector< item_reward_info > &list,string &msg,uint32 &msg_type)
{
	int ret=0;
	//item_reward_info
	ASSERT(false);
	//String
	msg = bytes.readUTF();
	//uint32
	msg_type = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_send_faction_gift (const vector< item_reward_info > &list ,char const*msg,uint32 msg_type)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SEND_FACTION_GIFT);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	m_bytes.writeUTF(msg);
	m_bytes.writeBytes((uint8*)&msg_type, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*领取额外奖励*/
int ProtocolUtil::unpack_get_faction_gift_exreward (ByteArray &bytes ,uint32 &count_id)
{
	int ret=0;
	//uint32
	count_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_get_faction_gift_exreward (uint32 count_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_FACTION_GIFT_EXREWARD);
	
	m_bytes.writeBytes((uint8*)&count_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*领取所有额外奖励*/
int ProtocolUtil::unpack_get_all_faction_gift_exreward (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_get_all_faction_gift_exreward ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_ALL_FACTION_GIFT_EXREWARD);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回礼物列表*/
int ProtocolUtil::unpack_show_faction_gift_page (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_show_faction_gift_page ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_GIFT_PAGE);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回礼物信息*/
int ProtocolUtil::unpack_show_faction_gift_info (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_show_faction_gift_info ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_GIFT_INFO);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回女王未感谢礼物*/
int ProtocolUtil::unpack_show_faction_gift_unthank_page (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_show_faction_gift_unthank_page ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_GIFT_UNTHANK_PAGE);
	
	SendToServer(m_bytes);
	return 0;
}

/*返回女王历史记录*/
int ProtocolUtil::unpack_show_faction_gift_history_page (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_show_faction_gift_history_page ()
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_GIFT_HISTORY_PAGE);
	
	SendToServer(m_bytes);
	return 0;
}

/*请求家族魅力排行*/
int ProtocolUtil::unpack_get_faction_gift_rank_page (ByteArray &bytes ,uint32 &page)
{
	int ret=0;
	//uint32
	page = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_get_faction_gift_rank_page (uint32 page)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_FACTION_GIFT_RANK_PAGE);
	
	m_bytes.writeBytes((uint8*)&page, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*返回家族魅力排行*/
int ProtocolUtil::unpack_show_faction_gift_rank_result_list (ByteArray &bytes , vector< faction_gift_rank_info > &list, faction_gift_rank_info **info,uint32 &page)
{
	int ret=0;
	//faction_gift_rank_info
	ASSERT(false);
	//faction_gift_rank_info
	*info = (faction_gift_rank_info*)bytes.cur_data();
	bytes.position(bytes.position() + sizeof(faction_gift_rank_info));
	//uint32
	page = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_show_faction_gift_rank_result_list (const vector< faction_gift_rank_info > &list ,faction_gift_rank_info *info,uint32 page)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_GIFT_RANK_RESULT_LIST);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	m_bytes.writeBytes((uint8*)info, sizeof(faction_gift_rank_info));
	m_bytes.writeBytes((uint8*)&page, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*返回家族魅力排行变化*/
int ProtocolUtil::unpack_show_faction_gift_rank_change (ByteArray &bytes ,uint32 &old_rank,uint32 &new_rank, faction_gift_rank_info **info)
{
	int ret=0;
	//uint32
	old_rank = bytes.readUnsignedInt();
	//uint32
	new_rank = bytes.readUnsignedInt();
	//faction_gift_rank_info
	*info = (faction_gift_rank_info*)bytes.cur_data();
	bytes.position(bytes.position() + sizeof(faction_gift_rank_info));
	return ret;
}

int ProtocolUtil::send_show_faction_gift_rank_change (uint32 old_rank,uint32 new_rank,faction_gift_rank_info *info)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_GIFT_RANK_CHANGE);
	
	m_bytes.writeBytes((uint8*)&old_rank, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&new_rank, sizeof(uint32));
	m_bytes.writeBytes((uint8*)info, sizeof(faction_gift_rank_info));
	SendToServer(m_bytes);
	return 0;
}

/*返回本家族魅力排行*/
int ProtocolUtil::unpack_show_faction_gift_rank_info (ByteArray &bytes , faction_gift_rank_info **info)
{
	int ret=0;
	//faction_gift_rank_info
	*info = (faction_gift_rank_info*)bytes.cur_data();
	bytes.position(bytes.position() + sizeof(faction_gift_rank_info));
	return ret;
}

int ProtocolUtil::send_show_faction_gift_rank_info (faction_gift_rank_info *info)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_GIFT_RANK_INFO);
	
	m_bytes.writeBytes((uint8*)info, sizeof(faction_gift_rank_info));
	SendToServer(m_bytes);
	return 0;
}

/*神兵强化*/
int ProtocolUtil::unpack_divine_forge (ByteArray &bytes ,uint32 &id,uint32 &count)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	//uint32
	count = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_divine_forge (uint32 id,uint32 count)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DIVINE_FORGE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*神兵升阶*/
int ProtocolUtil::unpack_divine_advance (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_divine_advance (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DIVINE_ADVANCE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*神兵铸魂*/
int ProtocolUtil::unpack_divine_spirit (ByteArray &bytes ,uint32 &id,uint32 &protect,uint32 &improve)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	//uint32
	protect = bytes.readUnsignedInt();
	//uint32
	improve = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_divine_spirit (uint32 id,uint32 protect,uint32 improve)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DIVINE_SPIRIT);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&protect, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&improve, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*查询全民boss信息*/
int ProtocolUtil::unpack_query_mass_boss_info (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_query_mass_boss_info (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUERY_MASS_BOSS_INFO);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*全民boss信息结果*/
int ProtocolUtil::unpack_mass_boss_info_ret (ByteArray &bytes ,uint32 &count,uint8 &percent)
{
	int ret=0;
	//uint32
	count = bytes.readUnsignedInt();
	//uint8
	percent = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_mass_boss_info_ret (uint32 count,uint8 percent)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_MASS_BOSS_INFO_RET);
	
	m_bytes.writeBytes((uint8*)&count, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&percent, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*查询全民boss排行榜*/
int ProtocolUtil::unpack_query_mass_boss_rank (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_query_mass_boss_rank (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_QUERY_MASS_BOSS_RANK);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*全民boss排行结果*/
int ProtocolUtil::unpack_mass_boss_rank_result (ByteArray &bytes , vector< mass_boss_rank_info > &info)
{
	int ret=0;
	//mass_boss_rank_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_mass_boss_rank_result (const vector< mass_boss_rank_info > &info )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_MASS_BOSS_RANK_RESULT);
	
	m_bytes.writeShort(info .size());
	for (auto it:info)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*挑战全民boss*/
int ProtocolUtil::unpack_try_mass_boss (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_try_mass_boss (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TRY_MASS_BOSS);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*购买挑战全民boss次数*/
int ProtocolUtil::unpack_buy_mass_boss_times (ByteArray &bytes ,uint8 &cnt)
{
	int ret=0;
	//uint8
	cnt = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_buy_mass_boss_times (uint8 cnt)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BUY_MASS_BOSS_TIMES);
	
	m_bytes.writeBytes((uint8*)&cnt, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*组队副本跨服匹配*/
int ProtocolUtil::unpack_group_instance_match (ByteArray &bytes ,uint8 &indx)
{
	int ret=0;
	//uint8
	indx = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_group_instance_match (uint8 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GROUP_INSTANCE_MATCH);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*组队副本跨服次数购买*/
int ProtocolUtil::unpack_buy_group_instance_times (ByteArray &bytes ,uint8 &count)
{
	int ret=0;
	//uint8
	count = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_buy_group_instance_times (uint8 count)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BUY_GROUP_INSTANCE_TIMES);
	
	m_bytes.writeBytes((uint8*)&count, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*法宝激活*/
int ProtocolUtil::unpack_talisman_active (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_talisman_active (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TALISMAN_ACTIVE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*法宝注灵*/
int ProtocolUtil::unpack_talisman_lvup (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_talisman_lvup (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_TALISMAN_LVUP);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*神羽激活*/
int ProtocolUtil::unpack_wings_active (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_wings_active ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WINGS_ACTIVE);
	
	SendToServer(m_bytes);
	return 0;
}

/*神羽祝福*/
int ProtocolUtil::unpack_wings_bless (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_wings_bless ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WINGS_BLESS);
	
	SendToServer(m_bytes);
	return 0;
}

/*神羽升阶*/
int ProtocolUtil::unpack_wings_rankup (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_wings_rankup ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WINGS_RANKUP);
	
	SendToServer(m_bytes);
	return 0;
}

/*神羽强化*/
int ProtocolUtil::unpack_wings_strength (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_wings_strength ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WINGS_STRENGTH);
	
	SendToServer(m_bytes);
	return 0;
}

/*经脉修炼*/
int ProtocolUtil::unpack_meridian_practise (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_meridian_practise ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_MERIDIAN_PRACTISE);
	
	SendToServer(m_bytes);
	return 0;
}

/*加经脉修炼经验值*/
int ProtocolUtil::unpack_add_meridian_exp (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_add_meridian_exp (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ADD_MERIDIAN_EXP);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*提升坐骑等级*/
int ProtocolUtil::unpack_raise_mount_level_base (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_raise_mount_level_base ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RAISE_MOUNT_LEVEL_BASE);
	
	SendToServer(m_bytes);
	return 0;
}

/*解锁坐骑*/
int ProtocolUtil::unpack_active_mount (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_active_mount ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ACTIVE_MOUNT);
	
	SendToServer(m_bytes);
	return 0;
}

/*家族领主挑战输出排行*/
int ProtocolUtil::unpack_show_faction_bossdefense_damage_list (ByteArray &bytes , vector< mass_boss_rank_info > &list)
{
	int ret=0;
	//mass_boss_rank_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_show_faction_bossdefense_damage_list (const vector< mass_boss_rank_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_BOSSDEFENSE_DAMAGE_LIST);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*家族无尽远征扫荡结果*/
int ProtocolUtil::unpack_show_faction_tower_sweep_list (ByteArray &bytes , vector< item_reward_info > &list)
{
	int ret=0;
	//item_reward_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_show_faction_tower_sweep_list (const vector< item_reward_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_TOWER_SWEEP_LIST);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*副本结果*/
int ProtocolUtil::unpack_send_instance_result (ByteArray &bytes ,uint8 &state,uint8 &cd, vector< item_reward_info > &list,uint8 &type,string &data)
{
	int ret=0;
	//uint8
	state = bytes.readUnsignedByte();
	//uint8
	cd = bytes.readUnsignedByte();
	//item_reward_info
	ASSERT(false);
	//uint8
	type = bytes.readUnsignedByte();
	//String
	data = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_send_instance_result (uint8 state,uint8 cd,const vector< item_reward_info > &list ,uint8 type,char const*data)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SEND_INSTANCE_RESULT);
	
	m_bytes.writeBytes((uint8*)&state, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&cd, sizeof(uint8));
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	m_bytes.writeUTF(data);
	SendToServer(m_bytes);
	return 0;
}

/*匹配单人pvp*/
int ProtocolUtil::unpack_match_single_pvp (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_match_single_pvp ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_MATCH_SINGLE_PVP);
	
	SendToServer(m_bytes);
	return 0;
}

/*购买单人pvp次数*/
int ProtocolUtil::unpack_buy_match_single_pvp_times (ByteArray &bytes ,uint8 &cnt)
{
	int ret=0;
	//uint8
	cnt = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_buy_match_single_pvp_times (uint8 cnt)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BUY_MATCH_SINGLE_PVP_TIMES);
	
	m_bytes.writeBytes((uint8*)&cnt, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*领取单人pvp额外奖励*/
int ProtocolUtil::unpack_pick_match_single_pvp_extra_reward (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_pick_match_single_pvp_extra_reward (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_PICK_MATCH_SINGLE_PVP_EXTRA_REWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*装备养成操作*/
int ProtocolUtil::unpack_equipdevelop_operate (ByteArray &bytes ,uint8 &opt_type,uint16 &reserve_int1,uint16 &reserve_int2,string &reserve_str1,string &reserve_str2)
{
	int ret=0;
	//uint8
	opt_type = bytes.readUnsignedByte();
	//uint16
	reserve_int1 = bytes.readUnsignedShort();
	//uint16
	reserve_int2 = bytes.readUnsignedShort();
	//String
	reserve_str1 = bytes.readUTF();
	//String
	reserve_str2 = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_equipdevelop_operate (uint8 opt_type,uint16 reserve_int1,uint16 reserve_int2,char const*reserve_str1,char const*reserve_str2)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_EQUIPDEVELOP_OPERATE);
	
	m_bytes.writeBytes((uint8*)&opt_type, sizeof(uint8));
	m_bytes.writeBytes((uint8*)&reserve_int1, sizeof(uint16));
	m_bytes.writeBytes((uint8*)&reserve_int2, sizeof(uint16));
	m_bytes.writeUTF(reserve_str1);
	m_bytes.writeUTF(reserve_str2);
	SendToServer(m_bytes);
	return 0;
}

/*激活外观*/
int ProtocolUtil::unpack_active_appearance (ByteArray &bytes ,uint16 &id)
{
	int ret=0;
	//uint16
	id = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_active_appearance (uint16 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ACTIVE_APPEARANCE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*装备外观*/
int ProtocolUtil::unpack_equip_appearance (ByteArray &bytes ,uint16 &id)
{
	int ret=0;
	//uint16
	id = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_equip_appearance (uint16 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_EQUIP_APPEARANCE);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

/*取消装备外观*/
int ProtocolUtil::unpack_cancel_equip_appearance (ByteArray &bytes ,uint8 &type)
{
	int ret=0;
	//uint8
	type = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_cancel_equip_appearance (uint8 type)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_CANCEL_EQUIP_APPEARANCE);
	
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*改名*/
int ProtocolUtil::unpack_rename (ByteArray &bytes ,string &name)
{
	int ret=0;
	//String
	name = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_rename (char const*name)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RENAME);
	
	m_bytes.writeUTF(name);
	SendToServer(m_bytes);
	return 0;
}

/*道具解锁称号*/
int ProtocolUtil::unpack_unlock_title (ByteArray &bytes ,uint8 &indx)
{
	int ret=0;
	//uint8
	indx = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_unlock_title (uint8 indx)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_UNLOCK_TITLE);
	
	m_bytes.writeBytes((uint8*)&indx, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*购买复仇次数*/
int ProtocolUtil::unpack_social_buy_revenge_times (ByteArray &bytes ,uint8 &count)
{
	int ret=0;
	//uint8
	count = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_social_buy_revenge_times (uint8 count)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_BUY_REVENGE_TIMES);
	
	m_bytes.writeBytes((uint8*)&count, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*请求进入世界冒险副本*/
int ProtocolUtil::unpack_enter_risk_instance (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_enter_risk_instance ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ENTER_RISK_INSTANCE);
	
	SendToServer(m_bytes);
	return 0;
}

/*删除仇人*/
int ProtocolUtil::unpack_social_remove_enemy (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_social_remove_enemy (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SOCIAL_REMOVE_ENEMY);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*查看玩家详情*/
int ProtocolUtil::unpack_get_player_overview (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_get_player_overview (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_GET_PLAYER_OVERVIEW);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*返回玩家详情*/
int ProtocolUtil::unpack_show_player_overview (ByteArray &bytes ,string &guid,string &name,uint32 &force,uint32 &vip,uint32 &title,uint32 &gender,uint32 &coat,uint32 &weapon)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//String
	name = bytes.readUTF();
	//uint32
	force = bytes.readUnsignedInt();
	//uint32
	vip = bytes.readUnsignedInt();
	//uint32
	title = bytes.readUnsignedInt();
	//uint32
	gender = bytes.readUnsignedInt();
	//uint32
	coat = bytes.readUnsignedInt();
	//uint32
	weapon = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_show_player_overview (char const*guid,char const*name,uint32 force,uint32 vip,uint32 title,uint32 gender,uint32 coat,uint32 weapon)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_PLAYER_OVERVIEW);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeUTF(name);
	m_bytes.writeBytes((uint8*)&force, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&vip, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&title, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&gender, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&coat, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&weapon, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*邀请加入帮派*/
int ProtocolUtil::unpack_send_faction_invite (ByteArray &bytes ,string &guid)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_send_faction_invite (char const*guid)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SEND_FACTION_INVITE);
	
	m_bytes.writeUTF(guid);
	SendToServer(m_bytes);
	return 0;
}

/*显示邀请*/
int ProtocolUtil::unpack_show_faction_invite (ByteArray &bytes ,string &guid,string &name,string &faction_guid,string &faction_name)
{
	int ret=0;
	//String
	guid = bytes.readUTF();
	//String
	name = bytes.readUTF();
	//String
	faction_guid = bytes.readUTF();
	//String
	faction_name = bytes.readUTF();
	return ret;
}

int ProtocolUtil::send_show_faction_invite (char const*guid,char const*name,char const*faction_guid,char const*faction_name)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SHOW_FACTION_INVITE);
	
	m_bytes.writeUTF(guid);
	m_bytes.writeUTF(name);
	m_bytes.writeUTF(faction_guid);
	m_bytes.writeUTF(faction_name);
	SendToServer(m_bytes);
	return 0;
}

/*购买vip礼包*/
int ProtocolUtil::unpack_buy_vipgift (ByteArray &bytes ,uint32 &id)
{
	int ret=0;
	//uint32
	id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_buy_vipgift (uint32 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_BUY_VIPGIFT);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*购买活动每日礼包*/
int ProtocolUtil::unpack_activity_opt_buy_dailygift (ByteArray &bytes ,uint32 &act_id,uint32 &index)
{
	int ret=0;
	//uint32
	act_id = bytes.readUnsignedInt();
	//uint32
	index = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_activity_opt_buy_dailygift (uint32 act_id,uint32 index)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ACTIVITY_OPT_BUY_DAILYGIFT);
	
	m_bytes.writeBytes((uint8*)&act_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&index, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*抽奖*/
int ProtocolUtil::unpack_draw_lottery (ByteArray &bytes ,uint32 &actId,uint8 &type)
{
	int ret=0;
	//uint32
	actId = bytes.readUnsignedInt();
	//uint8
	type = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_draw_lottery (uint32 actId,uint8 type)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_DRAW_LOTTERY);
	
	m_bytes.writeBytes((uint8*)&actId, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&type, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*获取活动开服排行进度奖励*/
int ProtocolUtil::unpack_activity_opt_get_rank_process_reward (ByteArray &bytes ,uint32 &act_id,uint32 &index)
{
	int ret=0;
	//uint32
	act_id = bytes.readUnsignedInt();
	//uint32
	index = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_activity_opt_get_rank_process_reward (uint32 act_id,uint32 index)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ACTIVITY_OPT_GET_RANK_PROCESS_REWARD);
	
	m_bytes.writeBytes((uint8*)&act_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&index, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*获取活动开服排行榜*/
int ProtocolUtil::unpack_activity_opt_get_rank_list (ByteArray &bytes ,uint32 &act_id)
{
	int ret=0;
	//uint32
	act_id = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_activity_opt_get_rank_list (uint32 act_id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ACTIVITY_OPT_GET_RANK_LIST);
	
	m_bytes.writeBytes((uint8*)&act_id, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*返回活动开服排行榜*/
int ProtocolUtil::unpack_activity_opt_show_rank_list (ByteArray &bytes ,uint32 &act_id, vector< act_rank_info > &list)
{
	int ret=0;
	//uint32
	act_id = bytes.readUnsignedInt();
	//act_rank_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_activity_opt_show_rank_list (uint32 act_id,const vector< act_rank_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_ACTIVITY_OPT_SHOW_RANK_LIST);
	
	m_bytes.writeBytes((uint8*)&act_id, sizeof(uint32));
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*购买活动限定礼包*/
int ProtocolUtil::unpack_activity_opt_buy_limitgift (ByteArray &bytes ,uint32 &act_id,uint32 &index)
{
	int ret=0;
	//uint32
	act_id = bytes.readUnsignedInt();
	//uint32
	index = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_activity_opt_buy_limitgift (uint32 act_id,uint32 index)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_ACTIVITY_OPT_BUY_LIMITGIFT);
	
	m_bytes.writeBytes((uint8*)&act_id, sizeof(uint32));
	m_bytes.writeBytes((uint8*)&index, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*领取累计充值奖励*/
int ProtocolUtil::unpack_welfare_get_recharge_reward (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_welfare_get_recharge_reward (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_GET_RECHARGE_REWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*领取累计消费奖励*/
int ProtocolUtil::unpack_welfare_get_consume_reward (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_welfare_get_consume_reward (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_GET_CONSUME_REWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*领取七日大礼奖励*/
int ProtocolUtil::unpack_welfare_get_sevenday_reward (ByteArray &bytes ,uint8 &id)
{
	int ret=0;
	//uint8
	id = bytes.readUnsignedByte();
	return ret;
}

int ProtocolUtil::send_welfare_get_sevenday_reward (uint8 id)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_WELFARE_GET_SEVENDAY_REWARD);
	
	m_bytes.writeBytes((uint8*)&id, sizeof(uint8));
	SendToServer(m_bytes);
	return 0;
}

/*服务器开服时间*/
int ProtocolUtil::unpack_send_server_open_time (ByteArray &bytes ,uint32 &open_time)
{
	int ret=0;
	//uint32
	open_time = bytes.readUnsignedInt();
	return ret;
}

int ProtocolUtil::send_send_server_open_time (uint32 open_time)
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_SEND_SERVER_OPEN_TIME);
	
	m_bytes.writeBytes((uint8*)&open_time, sizeof(uint32));
	SendToServer(m_bytes);
	return 0;
}

/*请求世界冒险排行榜*/
int ProtocolUtil::unpack_risk_get_rank (ByteArray &bytes )
{
	int ret=0;
	return ret;
}

int ProtocolUtil::send_risk_get_rank ()
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_RISK_GET_RANK);
	
	SendToServer(m_bytes);
	return 0;
}

/*世界冒险排行榜信息 */
int ProtocolUtil::unpack_risk_get_rank_result (ByteArray &bytes , vector< act_rank_info > &list)
{
	int ret=0;
	//act_rank_info
	ASSERT(false);
	return ret;
}

int ProtocolUtil::send_risk_get_rank_result (const vector< act_rank_info > &list )
{
	m_bytes.clear();
	m_bytes.writeShort(SMSG_RISK_GET_RANK_RESULT);
	
	m_bytes.writeShort(list .size());
	for (auto it:list)
	{
		m_bytes.writeT(it);
	}
	SendToServer(m_bytes);
	return 0;
}

/*设置朝向*/
int ProtocolUtil::unpack_set_orient (ByteArray &bytes ,uint16 &angle)
{
	int ret=0;
	//uint16
	angle = bytes.readUnsignedShort();
	return ret;
}

int ProtocolUtil::send_set_orient (uint16 angle)
{
	m_bytes.clear();
	m_bytes.writeShort(CMSG_SET_ORIENT);
	
	m_bytes.writeBytes((uint8*)&angle, sizeof(uint16));
	SendToServer(m_bytes);
	return 0;
}

