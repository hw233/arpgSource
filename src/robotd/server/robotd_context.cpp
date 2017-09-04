#include "robotd_app.h"
#include "robotd_context.h"
#include "robotd_script.h"
#include "robotd_object_manager.h"
#include "robotd_unit.h"
#include <object/OprateResult.h>
#include <shared/storage_strus.h>
#include <shared/storage_loader.h>
#include <vector>
#include <comm/b64.h>
#include <iosfwd>


//////////////////////////////////////////////////////////////////////////
//for robotd_context
RobotdContext::RobotdContext(const string &pid, const string &sid, const string &uid, const string &remote_ip, const string &login_key, const string &conn_host, uint32 conn_port, const string &params):
	PlayerBase(core_obj::SYNC_NONE)
	,conn_(nullptr),war_conn_(nullptr),unit_(nullptr)
	,m_pid(pid), m_sid(sid), m_uid(uid),m_remote_ip(remote_ip),m_login_key(login_key)
	,m_obj_mgr(nullptr),m_params(params),m_action_switch(0)
{
	ASSERT(!m_pid.empty());
	ASSERT(!m_sid.empty());
	ASSERT(!m_uid.empty());

	if(!m_params.empty())
		m_action_switch = atol(m_params.c_str());
	else
		m_action_switch = uint32(-1);

	this->OnGameServerLogin(conn_host, conn_port);
}

void RobotdContext::OnGameServerLogin(const string& login_host, uint32 login_port) {
	conn_ = new RobotdGameConnection(this, login_host, login_port);
	Reset();
	RobotdApp::g_app->m_login_queue.push(GetAccount());
}

RobotdContext::~RobotdContext()
{
	safe_delete(conn_);
	safe_delete(war_conn_);
	safe_delete(m_obj_mgr);
	//对象管理器管理指针存活
	unit_ = nullptr;
}


RobotdGameConnection *RobotdContext::GetConn()
{
	if(war_conn_)
		return war_conn_;
	return conn_;
}

void RobotdContext::OnClose()
{
	DoRobotLogout(GetAccount(), GetFD());
	safe_delete(war_conn_);
	Reset();
}

void RobotdContext::OnWarClose()
{
	safe_delete(war_conn_);
	if(conn_->conn_)
		DoRobotWarClose(GetAccount(), GetFD());
}

void RobotdContext::Reset()
{
	//if(conn_)
	//{
	//	conn_->Reset();
	//}

	SetMyUnit(nullptr);
	safe_delete(m_obj_mgr);
	if(notify_info_)
	{
		delete notify_info_;
		notify_info_ = nullptr;
	}

	m_obj_mgr = new RobotdObjectManager(nullptr, this);
}

string RobotdContext::MakeSessionKey(bool indulge, bool mobile)
{
	string session_key = "";
	string sIndulge = "n";
	if(indulge)
		sIndulge = "y";
	uint32 now = (uint32)time(NULL);
	//b64加密
	char buf[1000];
	sprintf(buf,"pid=%s&sid=%s&uid=%s&time=%u&indulge=%s&mobile=%s&remote_ip=%s",m_pid.c_str(),m_sid.c_str(),m_uid.c_str(),now,sIndulge.c_str(),mobile?"y":"n",m_remote_ip.c_str());
	string src = buf;
	char dst[1000];
	if(base64_encode(src.c_str(),src.size(),dst,(int)sizeof(dst)) != 0)
	{
		tea_perror("MakeSessionKey() error in base64_encode()");
	}
	else
	{
		string auth = dst;
		auth += m_login_key;
		static char tmp[1000];
		const char * md5_str = MD5CryptToStr32((int8*)auth.c_str(),auth.size(),tmp);
		//校验码md5_str
		sprintf(buf,"?auth=%s&sign=%s",dst,md5_str);
		session_key = buf;
	}
	return session_key;
}

bool RobotdContext::IsLoginOk()
{
	return conn_->GetLoginState() == ROBOT_LOGIN_STATE_LOGIN_SUCCEED;
}

void RobotdContext::Update(uint32 diff)
{
	//对象管理器心跳
	if(m_obj_mgr)
		m_obj_mgr->Update();

	//登录没成功下面就不用跳了
	if(!IsLoginOk() && !conn_->LoginUpdate(diff))
		return;

	if(war_conn_)
		war_conn_->LoginUpdate(diff);

	for (auto it:m_obj_mgr->m_all_player_unit)
	{
		it->UpdateLocate(diff);
	}
	/**
	if(unit_)
		unit_->UpdateLocate(diff);
	*/
}

void RobotdContext::SetMyUnit(RobotdUnit *u)
{
	if(unit_)
		tea_pdebug("RobotdContext::SetMyUnit before %s", unit_->guid().c_str());
	else
		tea_pdebug("RobotdContext::SetMyUnit before nullptr");
	unit_ = u;
	if(unit_)
		tea_pdebug("RobotdContext::SetMyUnit after %s", unit_->guid().c_str());
	else
		tea_pdebug("RobotdContext::SetMyUnit after nullptr");
}

void RobotdContext::OnAfterUpdate(SyncEventRecorder *data,int flags,UpdateMask& mask,UpdateMask& str_mask)
{
	RobotdApp::g_app->StatUpdateIndex(data, mask, mask_string_);
	
	if(data == this && conn_->GetLoginState() < ROBOT_LOGIN_STATE_LOGIN_DATA_HAS)
		conn_->SetLoginState(ROBOT_LOGIN_STATE_LOGIN_DATA_HAS);
}

void RobotdContext::Handle_Lua(uint16 optcode, PacketType &pkt)
{
	packet *_pkt = external_protocol_new_packet(optcode);
	if(pkt.bytesAvailable())
		packet_write(_pkt, (const char *)pkt.cur_data() + pkt.position(), pkt.bytesAvailable());
	update_packet_len(_pkt);
	DoPacketHandle(GetAccount(), GetFD(), _pkt);
	external_protocol_free_packet(_pkt);
}

void RobotdContext::Handle_Ud_Object(uint16 optcode, PacketType &pkt)
{
	m_obj_mgr->HandlePacket(GetFD(), optcode, pkt);
}

//地图对象更新
void RobotdContext::Handle_Map_Info_Update(uint16 optcode, PacketType &pkt)
{
	m_obj_mgr->HandlePacket(GetFD(), SMSG_UD_OBJECT, pkt);
}

void RobotdContext::Handle_Strength_Success(uint16 optcode, PacketType &pkt) {
	uint16 value;
	pkt >> value;
	tea_pdebug("strength success, level = %d", value);
}

void RobotdContext::Handle_World3v3_WaitInfo(uint16 optcode, PacketType &pkt) {
	uint16 len;
	char name[50];
	uint8 state;
	bool does = false;

	pkt >> len;
	for (int i = 0; i < len; ++ i) {
		pkt >> name >> state;
		if (string(name) == this->GetName()) {
			if (state) {
				does = true;
			}
		}
	}

	// 接受匹配
	if (!does) {
		this->send_kuafu_3v3_match_oper(1);
	}
	else {
		uint32 oper = irand(0, 1);
		this->send_kuafu_3v3_match_oper(oper);
	}
}

//grid更新精灵
void RobotdContext::Handle_Grid_Ud_Object(uint16 optcode, PacketType &pkt)
{
	//ASSERT(GetLoginState() == ROBOT_LOGIN_STATE_CREATE_CHARS || GetLoginState() >= ROBOT_LOGIN_STATE_LOGIN);
	if(optcode == SMSG_GRID_UD_OBJECT_2)
	{
		uint32 p = pkt.position();
		m_obj_mgr->Compress(pkt);
		pkt.position(p);
	}
	m_obj_mgr->HandlePacket(GetFD(), SMSG_UD_OBJECT, pkt);
}

void RobotdContext::Handle_Unit_Move_Stop(uint16 optcode, PacketType &pkt)
{
	uint32 u_guid;
	uint16 px, py;
	unpack_move_stop(pkt, u_guid, px, py);
	string guid = m_obj_mgr->GetByUGuid(u_guid);
	RobotdUnit *unit = dynamic_cast<RobotdUnit*>(m_obj_mgr->Get(guid));

	if(!unit)
	{
		//tea_perror("RobotdContext::Handle_Unit_Move_Stop %s %u not find", guid.c_str(), u_guid);
		return;
	}

	//if(unit == unit_)
	//	tea_pdebug("RobotdContext::Handle_Unit_Move_Stop player %s pos(%u,%u) stop move!",guid.c_str(), (uint16)px, (uint16)py);

	unit->StopMoving((uint16)px, (uint16)py);
}

void RobotdContext::Handle_Grid_Unit_Move(uint16 optcode, PacketType &pkt)
{
	static ByteArray unzip;
	static RobotdUnit __unit(nullptr);
	unzip.clear();
	if(optcode == SMSG_GRID_UNIT_MOVE_2) 
		m_obj_mgr->UnCompress(pkt,unzip);
	else
		unzip.writeBytes(pkt.cur_data(), pkt.bytesAvailable());
	unzip.position(0);
	uint16 count = unzip.readShort();
	for (int j = 0; j < count; j++)
	{
		uint32 u_guid = unzip.readUnsignedInt();
		string guid = m_obj_mgr->GetByUGuid(u_guid);
		RobotdUnit *unit = dynamic_cast<RobotdUnit*>(m_obj_mgr->Get(guid));
		uint8 b = unzip.readByte();
		if(!unit)
		{
			//tea_pdebug("RobotdContext::Handle_Grid_Unit_Move %s %u not find", guid.c_str(), u_guid);
			unit = &__unit;
		}
		//if (u_guid != this->unit_->GetUIntGuid()) {
		unit->ReadOffsetPath(unzip);
		//}
	}
}
void RobotdContext::Handle_Operation_failed(uint16 optcode, PacketType &pkt)
{
	uint16 type, reason;
	string data;
	pkt >> type >> reason >> data;

	if(type == OPRATE_TYPE_LOGIN)
	{
		switch (reason)
		{
		case OPRATE_RESULT_NAME_REPEAT:
			{
				//角色创建失败
				conn_->SetLoginState(ROBOT_LOGIN_STATE_ERR_NAME);
			}
			break;
		case OPRATE_RESULT_NAME_ILLEGAL:
			{
				//角色创建失败
				conn_->SetLoginState(ROBOT_LOGIN_STATE_ERR_NAME);
			}
			break;
		case OPRATE_RESULT_CHAR_CAP:
			{
				//角色创建失败
				conn_->SetLoginState(ROBOT_LOGIN_STATE_ERR_NAME);
			}
			break;
		case OPRATE_RESULT_LOGINED_IN:
			{
				tea_pinfo("RobotActionLogin::HandleOptcode sid %s is online！！！。", GetUserId().c_str());
				send_forced_into();
				conn_->SetLoginState(ROBOT_LOGIN_STATE_FORCED_INTO);
			}
			break;
		default:
			break;
		}
	}
	else if (type == OPERTE_TYPE_STRENGTH) {
		switch (reason) {
		case STRENGTH_OPERTE_FAIL: {
			tea_pinfo("强化失败");
			break;
								   }
		default:
			break;
		}
	}
}

void RobotdContext::Handle_Chars_List(uint16 optcode, PacketType &pkt)
{
	vector<char_create_info*> list;
	uint16 len;
	pkt >> len;
	for (uint16 i = 0; i < len; i++)
	{
		list.push_back((char_create_info*)pkt.cur_data());
		pkt.position(pkt.position() + sizeof(char_create_info));
	}
	ASSERT(len <= 1);

	if(len == 0)
	{
		GetConn()->SetLoginState(ROBOT_LOGIN_STATE_NOT_CHARS);
		return;
	}
	ASSERT(!war_conn_);
	SetGuid(string(list[0]->guid));//12j只有一个角色
	conn_->SetLoginState(ROBOT_LOGIN_STATE_HAS_CHARS);
}

//同步服务器时间
void RobotdContext::Handle_Sync_Mstime(uint16 optcode, PacketType &pkt)
{
	//uint32 time;
	//uint32 opentime;
	//uint32 curtime = ms_time();
	//pkt >> m_svr_runtime >> time >> opentime;
}

//合服通知
void RobotdContext::Handle_Merge_Server_Msg(uint16 optcode, PacketType &pkt)
{
	string merge_host,merge_key;
	uint32 merge_port, merge_type, reserve, reserve2;
	pkt>>merge_host>>merge_port>>merge_key>>merge_type>>reserve>>reserve2;

	if(merge_type == MERGE_TYPE_GAME_TO_PK)
	{
		tea_pinfo("RobotdContext::Handle_Merge_Server_Msg GAME_TO_PK %s %s host = %s port = %d", GetAccount().c_str(), guid().c_str(), merge_host.c_str(), merge_port);
		ASSERT(!war_conn_);
		war_conn_ = new RobotdWarConnection(this, merge_host, merge_port, merge_key);
		conn_->OnConnToWar();
	}
	else if (merge_type == MERGE_TYPE_PK_TO_GAME)
	{
		tea_pinfo("RobotdContext::Handle_Merge_Server_Msg PK_TO_GAME  %s %s host = %s port = %d", GetAccount().c_str(), guid().c_str(), merge_host.c_str(), merge_port);
		ASSERT(war_conn_);
		war_conn_->Close();
		conn_->Close();
		this->OnGameServerLogin(merge_host, merge_port);
	}
	m_obj_mgr->ClearUnit();
}

//加入获取离开否逻辑服务器
void RobotdContext::Handle_join_or_leave_server(uint16 optcode, PacketType &bytes)
{
	uint8 join_or_leave,server_type;
	uint32 server_pid,server_fd,time;
	unpack_join_or_leave_server(bytes, join_or_leave, server_type, server_pid,server_fd,time);
}

void RobotdContext::OnRobotRespawn() {
	DoRobotRespawn(this);
}

void setDamageValue(RobotdUnit* unit_, uint32 flag, int damage)
{
	//uint32 oldValue;
	//uint32 maxValue;
	//switch (flag)
	//{
	//case HITINFO_NORMALSWING: //普通
	//case HITINFO_CRITHIT: //暴击
	//case HITINFO_HCRITHIT: //高爆
	//case HITINFO_WHET_STONE: //磨刀伤害
	//case HITINFO_BUFF_HP: //状态 加血扣血 加血发的是负，扣血发的是正
	//	oldValue = unit_->GetUInt32(UNIT_FIELD_HEALTH);
	//	maxValue = unit_->GetUInt32(UNIT_FIELD_MAXHEALTH);
	//	unit_->SetUInt32(UNIT_FIELD_HEALTH, oldValue - damage);
	//	break;
	//case HITINFO_BUFF_MP: //状态 加蓝扣蓝
	//	oldValue = unit_->GetUInt32(UNIT_FIELD_MAGIC);
	//	unit_->SetUInt32(UNIT_FIELD_MAGIC, oldValue - damage);
	//	break;
	//}
}

void readFightingInfoUpdateObject(RobotdUnit* unit_,PacketType &bytes)
{
	if (!unit_)
	{
		return;
	}
	/**
	var caster: number = bytes.readUnsignedInt();
	var count: number = bytes.readUnsignedByte();
	var sdAry: Array<SkillBeAttackData> = new Array;
	for (var i: number = 0; i < count; i++) {

	var sd: SkillBeAttackData = new SkillBeAttackData();
	sd.target = bytes.readUnsignedInt();
	sd.lastHp = bytes.readUnsignedInt();
	sd.utype = bytes.readUnsignedByte();
	sd.isKilled = bytes.readUnsignedByte();
	if (sd.lastHp == 0) {
	sd.isKilled = 1
	}
	sdAry.push(sd);

	}
	var skillid: number = bytes.readUnsignedInt();
	var $tx: number = bytes.readShort();
	var $ty: number = bytes.readShort();
	*/

}

void RobotdContext::Handle_Fighting_Info_Update_Object(uint16 optcode, PacketType &bytes)
{
	readFightingInfoUpdateObject(unit_,bytes);
}

void RobotdContext::Handle_Fighting_Info_Update_Object_2(uint16 optcode, PacketType &bytes)
{
	static ByteArray unzip;
	static RobotdUnit __unit(nullptr);
	unzip.clear();
	m_obj_mgr->UnCompress(bytes,unzip);
	unzip.position(0);
	readFightingInfoUpdateObject(unit_,unzip);
}

//跨服玩家数据
void RobotdContext::Handle_World_War_SC_Player_info(uint16 optcode, PacketType &bytes)
{
	ASSERT(m_war_bytes.length() == 0);
	m_war_bytes.writeBytes(bytes, bytes.position());
}

int RobotdContext::send_world_war_SC_player_info___()
{
	static ByteArray __bytes;
	__bytes.clear();
	__bytes.writeShort(CMSG_WORLD_WAR_CS_PLAYER_INFO);
	__bytes.writeBytes(m_war_bytes);
	tea_pinfo("send CMSG_WORLD_WAR_CS_PLAYER_INFO bytes = %d", m_war_bytes.length());
	SendToServer(__bytes);
	m_war_bytes.clear();
	return 0;
}

int RobotdContext::LuaClose(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	RobotdContext *context = (RobotdContext*)lua_touserdata(scriptL, 1);
	ASSERT(context);
	context->Close();
	return 0;
}

int RobotdContext::LuaIsLoginOk(lua_State *scriptL)
{
	CHECK_LUA_NIL_PARAMETER(scriptL);
	int n = lua_gettop(scriptL);
	ASSERT(n == 1);
	RobotdContext *context = (RobotdContext*)lua_touserdata(scriptL, 1);
	ASSERT(context);
	lua_pushboolean(scriptL, context->IsLoginOk() ? TRUE : FALSE);
	return 1;
}
