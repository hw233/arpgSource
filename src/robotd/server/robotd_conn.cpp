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

void RobotdGameConnection::OpcodeHandlerInitor()
{
	for (uint32 i = 0; i < NUM_MSG_TYPES; i++)
	{
		opcode_handler_[i].handler = nullptr;
	}
	opcode_handler_[SMSG_CHARS_LIST].handler = &RobotdContext::Handle_Chars_List;

	opcode_handler_[CMSG_UD_CONTROL].handler = &RobotdContext::Handle_Ud_Object;
	opcode_handler_[SMSG_UD_OBJECT].handler = &RobotdContext::Handle_Ud_Object; 

	opcode_handler_[SMSG_GRID_UNIT_MOVE].handler =  &RobotdContext::Handle_Grid_Unit_Move;	
	opcode_handler_[SMSG_GRID_UNIT_MOVE_2].handler =  &RobotdContext::Handle_Grid_Unit_Move;	
	opcode_handler_[MSG_MOVE_STOP].handler = &RobotdContext::Handle_Unit_Move_Stop;	

	opcode_handler_[SMSG_GRID_UD_OBJECT].handler = &RobotdContext::Handle_Grid_Ud_Object;
	opcode_handler_[SMSG_GRID_UD_OBJECT_2].handler = &RobotdContext::Handle_Grid_Ud_Object;
	opcode_handler_[SMSG_MAP_UPDATE_OBJECT].handler = &RobotdContext::Handle_Map_Info_Update;

	opcode_handler_[SMSG_OPERATION_FAILED].handler = &RobotdContext::Handle_Operation_failed;
	opcode_handler_[SMSG_MERGE_SERVER_MSG].handler = &RobotdContext::Handle_Merge_Server_Msg;

	opcode_handler_[MSG_SYNC_MSTIME].handler =  &RobotdContext::Handle_Sync_Mstime;
	opcode_handler_[SMSG_FIGHTING_INFO_UPDATE_OBJECT].handler =  &RobotdContext::Handle_Fighting_Info_Update_Object;
	opcode_handler_[SMSG_FIGHTING_INFO_UPDATE_OBJECT_2].handler =  &RobotdContext::Handle_Fighting_Info_Update_Object_2;

	opcode_handler_[MSG_WORLD_WAR_SC_PLAYER_INFO].handler =  &RobotdContext::Handle_World_War_SC_Player_info;

	opcode_handler_[SMSG_STRENGTH_SUCCESS].handler =  &RobotdContext::Handle_Strength_Success;

	opcode_handler_[SMSG_KUAFU_3V3_WAIT_INFO].handler =  &RobotdContext::Handle_World3v3_WaitInfo;
}

void RobotdConnection::OnClose()
{
	SetOpen(false);
	//如果这时候有关联context，则证明是被远程断开的
	if(m_owner)
		m_owner->OnClose();
	core_socket::TcpConnection::OnClose();
}

void RobotdGameConnection::SetLoginState(uint16 val)
{
	if(val == m_login_state)
		return;

	if(m_login_state == ROBOT_LOGIN_STATE_LOGIN_SUCCEED)
		RobotdApp::g_app->m_login_queue.push(m_context->GetAccount());

	if(ROBOT_LOGIN_STATE_FAILURE != m_login_state)
		m_login_state = (RobotLoginState)val;
}

void RobotdGameConnection::CreateConn()
{
	m_context->ResetConn(conn_);
}

bool RobotdGameConnection::ConnToNetgd()
{
	conn_ = new RobotdConnection(this);

	if(!RobotdApp::g_app->conn_mgr()->Connect(m_conn_host, m_conn_port, conn_))
	{
		conn_->UnsetOwner();
		safe_delete(conn_);
		return false;//连接不成功
	}

	conn_->SetOpen(true);
	conn_->AddRef();

	conn_->onPacket([](core_socket::TcpConnection *conn, PacketType *&pkt){
		RobotdConnection *robotd_conn = dynamic_cast<RobotdConnection*>(conn);
		if(!robotd_conn ||!robotd_conn->IsOpen())
			return;

		uint16 optcode = pkt->readUnsignedShort();
		tea_pdebug("robot conn onpacket %u %s", optcode, GetOpcodeName(optcode));
		if(optcode < NUM_MSG_TYPES)
		{
			OpcodeHandler& op_handle = robotd_conn->GetOwner()->opcode_handler_[optcode];
			if(op_handle.handler) {
				(robotd_conn->GetOwner()->m_context->*op_handle.handler)(optcode, *pkt);
			} else {
				robotd_conn->GetOwner()->m_context->Handle_Lua(optcode, *pkt);
			}
			RobotdApp::g_app->all_op_sizie_map[optcode].size += pkt->length();
		}
	});
	CreateConn();
	return true;
}

// 完整消息头的长度
int RobotdConnection::get_real_head_len(PacketType *cur_pkt_) {
	int packetLenFlag = parser_websocket_packet_len_flag(cur_pkt_->data());
	int headLen = 0;
	if (packetLenFlag == 126) {
		headLen = PACKET_HEAD_FIRST_PART_SIZE + 2;
	}
	else if (packetLenFlag == 127) {
		headLen = PACKET_HEAD_FIRST_PART_SIZE + 8;
	}
	else {
		headLen = PACKET_HEAD_FIRST_PART_SIZE;
	}

	return headLen;
}

//获得消息的长度
int RobotdConnection::get_packet_payload(PacketType *cur_pkt_) {
	int packetLenFlag = parser_websocket_packet_len_flag(cur_pkt_->data());
	uint32_t payload = packetLenFlag;
	int i = 0;

	if (packetLenFlag == 126) {
		payload = 0;
		for (i = 0; i < 2; ++ i) {
			int val = parser_websocket_byte(cur_pkt_->data(), i + 2);
			payload = (payload << 8) + val;
		}
	}
	else if (packetLenFlag == 127) {
		payload = 0;
		for (i = 0; i < 8; ++ i) {
			int val = parser_websocket_byte(cur_pkt_->data(), i + 2);
			payload = (payload << 8) + val;
		}
	}

	return payload;
}

void RobotdConnection::ReadPacket() {
	//tea_pdebug("ReadPacket\n");
	// 是否是握手协议
	if (isShakeHand) {
		int length = recv_buf_.length();
		m_owner->SetLoginState(ROBOT_LOGIN_STATE_SHAKE_HAND_END);
		recv_buf_.add_read(length);
		isShakeHand = false;
		return;
	}

	//没东西可读了		;
	//可以解包了
	while (recv_buf_.length() > PACKET_HEAD_FIRST_PART_SIZE){
		int total = ComplatePacket((uint8_t*)recv_buf_.data(),recv_buf_.length());
		// 数据不足情况下的
		if (total == -1) {
			break;
		}
		recv_buf_.add_read(total);
	}
}

int RobotdConnection::ComplatePacket(uint8_t *buf,int len) {
	int total = 0;
	//剩下的大于指示包长的就可以读

	if(!cur_pkt_) {
		cur_pkt_ = new PacketType;
	}
	cur_pkt_->clear();

	//先解析包头的第一部分
	cur_pkt_->writeBytes(buf, PACKET_HEAD_FIRST_PART_SIZE);

	// 解析包头的第二部分
	// 得到包头的长度
	int headLen = get_real_head_len(cur_pkt_);
	// 无法组成一个完整包头
	if (len < headLen) {
		return -1;
	}
	cur_pkt_->position(0);
	cur_pkt_->writeBytes(buf, headLen);

	// 解析内容
	// 取内容的长度
	int payload = get_packet_payload(cur_pkt_);
	// 无法组成完成的包
	if (len < payload + headLen) {
		return -1;
	}

	//复制完整包
	cur_pkt_->position(0);
	cur_pkt_->writeBytes(buf, payload + headLen);

	//缓冲区偏移
	total = payload + headLen;

	//调用处理函数，如果有返回数据包就清空后使用,如果没有返回就构造一个新的
	if(onPacket_){
		//跳过包长
		cur_pkt_->position(headLen);
		onPacket_(this,cur_pkt_);
	}

	//已读字节数量
	return total;
}



void RobotdConnection::SendPacket(PacketType& pkt){
	int len = pkt.length();
	// 大数据不发
	if (len > 65535) {
		return;
	}
	// 先写头
	uint8_t headA = -126;
	uint8_t flag = 0;
	send_buf_.write((char*)&headA, sizeof(headA));
	// 判断是否需要写后续长度
	if (len > 125) {
		flag = 126;
		uint8_t a = ((uint16_t)len) >> 8;
		uint8_t b = ((uint16_t)len) & 0xFF;
		send_buf_.write((char*)&flag, sizeof(flag));
		send_buf_.write((char*)&a, sizeof(a));
		send_buf_.write((char*)&b, sizeof(b));
	}
	else {
		flag = (uint8_t)len;
		send_buf_.write((char*)&flag, sizeof(flag));
	}
	// 写内容
	send_buf_.write(pkt.data(),pkt.length());
}


void RobotdGameConnection::Close()
{
	if(conn_) 
	{
		//若不置空，会引发死循环
		auto conn = conn_;
		conn_ = nullptr;
		conn->SetOpen(false);
		conn->UnsetOwner();
		conn->Close();
		conn->SubRef();
	}
	OnClose();
}

void RobotdGameConnection::OnClose()
{
	tea_pinfo("RobotdGameConnection::OnClose() %s", m_context->guid().c_str());
	if(m_close_count++ <= 3)
		SetLoginState(ROBOT_LOGIN_STATE_NONE);
	else
		SetLoginState(ROBOT_LOGIN_STATE_FAILURE);
	m_context->OnClose();
	conn_ = nullptr;
}

void RobotdGameConnection::OnConnToWar()
{
	opcode_handler_[SMSG_GRID_UNIT_MOVE].handler =  nullptr;
	opcode_handler_[SMSG_GRID_UNIT_MOVE_2].handler =  nullptr;
	opcode_handler_[MSG_MOVE_STOP].handler = nullptr;

	opcode_handler_[SMSG_GRID_UD_OBJECT].handler = nullptr;
	opcode_handler_[SMSG_GRID_UD_OBJECT_2].handler = nullptr;
	opcode_handler_[SMSG_MAP_UPDATE_OBJECT].handler = nullptr;

	opcode_handler_[SMSG_FIGHTING_INFO_UPDATE_OBJECT].handler = nullptr;
	opcode_handler_[SMSG_FIGHTING_INFO_UPDATE_OBJECT_2].handler = nullptr;
}

void RobotdGameConnection::OnFreeWar()
{
	OpcodeHandlerInitor();
}

void RobotdGameConnection::SendFirstShakeProtocol(RobotdConnection *conn_) {
	// 第一次握手
	string str = "GET / HTTP/1.1\r\nOrigin: http://localhost:16686\r\nSec-WebSocket-Key: 0bM6nklG3nzQpB/uRVfJGA==\r\nConnection: Upgrade\r\nUpgrade: Websocket\r\n\r\n";
	conn_->SendFirstPacket(str.c_str(), str.length());
}

bool RobotdGameConnection::LoginUpdate(uint32 diff)
{
	if(m_login_state != ROBOT_LOGIN_STATE_FAILURE)
	{
		m_timer_clear_close_count.Update(diff);
		if(m_timer_clear_close_count.Passed())
		{
			m_timer_clear_close_count.Reset2();
			m_close_count = 0;
		}
	}

	switch(m_login_state)
	{
	case  ROBOT_LOGIN_STATE_NONE://无状态
		{
			SetLoginState(ROBOT_LOGIN_STATE_CONN_SERVER_COUNTDOWN);
			break;
		}
		break;
	case ROBOT_LOGIN_STATE_CONN_SERVER_COUNTDOWN://连接服务器倒计时中
		{
			if(!ConnToNetgd())
			{
				tea_pdebug("RobotActionLogin::Update ConnToNetgd fail %s %s %u", m_context->GetAccount().c_str(), GetConnHost().c_str(), GetConnPort());
				SetLoginState(ROBOT_LOGIN_STATE_FAILURE);
				return false;
			}
			tea_pdebug("RobotActionLogin::Update ConnToNetgd ok %s %s %u", m_context->GetAccount().c_str(), GetConnHost().c_str(), GetConnPort());
			m_context->ResetWarConn(nullptr);
			// 第一次握手
			RobotdGameConnection::SendFirstShakeProtocol(conn_);
			SetLoginState(ROBOT_LOGIN_STATE_SHAKING_HAND);
		}
		break;
	case ROBOT_LOGIN_STATE_SHAKING_HAND: // 握手中
		break;
	case ROBOT_LOGIN_STATE_SHAKE_HAND_END: //握手结束
		DoRobotLoginOk(m_context->GetAccount(), GetFD(), m_context);
		//连接服务器
		SetLoginState(ROBOT_LOGIN_STATE_CONN_SERVER_OK);
		break;
	case ROBOT_LOGIN_STATE_CONN_SERVER_OK:
		{
			packet *pkt = NULL;
			string sessionkey = m_context->MakeSessionKey(false);
			m_context->send_get_session(sessionkey.c_str(),"","");
			SetLoginState(ROBOT_LOGIN_STATE_GETSESSION);
			tea_pdebug("RobotActionLogin Update %d %s %d %x send GetSession ", GetFD(), m_context->GetAccount().c_str(), diff, this);
		}
		break;
	case ROBOT_LOGIN_STATE_GETSESSION:	//正在Get Session
		//tea_pdebug("RobotActionLogin Update %d %s 等待 GetSession 返回", GetFD(), guid().c_str());
		m_timer_getSession.Update(diff);
		if(m_timer_getSession.Passed())
		{
			m_timer_getSession.Reset2();
			Close();
			tea_pinfo("RobotActionLogin::Update getsession timeout %s %d", m_context->GetAccount().c_str(), diff);
		}
		break;
	case ROBOT_LOGIN_STATE_NOT_CHARS:	//Get Session已返回，当前未建角色
		{
			
			Tokens token;
			StrSplit(token, this->m_context->GetAccount(), '_');	
			int32 len = token.size();
			string name = token[len-1];

			char_create_info info;
			memset(&info, 0, sizeof(char_create_info));
			info.faction = 0;

			//CHAR_GENDER play_sex[] = {CHAR_GENDER_MALE,CHAR_GENDER_FEMALE};
			//info.gender = play_sex[rand() % 1];
			info.gender = irand(1, 2);
			info.hair_id = 0;
			info.head_id = HEAD_NONE;
			info.level = 1;
			info.race = RACE_NONE;

			strcpy(info.name, name.c_str());
			m_context->send_char_create(&info);
			SetLoginState(ROBOT_LOGIN_STATE_CREATE_CHARS);

			tea_pdebug("RobotActionLogin Update %d %s %d send CreateChar", GetFD(), m_context->GetAccount().c_str(), diff);
			m_timer_GaiMing.SetCurrent(0);
		}
		break;
	case ROBOT_LOGIN_STATE_GAIMIN:
		{
			m_timer_GaiMing.Update(diff);
			if (m_timer_GaiMing.Passed())
			{
				string name = "";
				GetRandName(name);
				char_create_info info;
				memset(&info,0,sizeof(char_create_info));
				CHAR_GENDER play_sex[] = {CHAR_GENDER_MALE,CHAR_GENDER_FEMALE};
				info.gender = play_sex[rand() % 2];
				strcpy(info.name,name.c_str());
				m_context->send_char_update_info(&info);
				SetLoginState(ROBOT_LOGIN_STATE_CREATE_CHARS);
				tea_pdebug("RobotActionLogin Update %d %s %d send CreateGaiMing",GetFD(),m_context->GetAccount().c_str(),diff);
			}
		}
		break;
	case ROBOT_LOGIN_STATE_CREATE_CHARS:	//创建角色中
		//tea_pdebug("RobotActionLogin Update %d %s 等待 CreateChar 返回", GetFD(), guid().c_str());
		m_timer_login.Update(diff);
		if(m_timer_login.Passed())
		{
			m_timer_login.Reset2();
			Close();
			tea_pinfo("RobotActionLogin::Update create chars timeout %s", m_context->GetAccount().c_str());
		}
		//send_instance_exit();
		break;
	case ROBOT_LOGIN_STATE_ERR_NAME:		//创建角色名字异常
		{
			SetLoginState(ROBOT_LOGIN_STATE_NOT_CHARS);		
			tea_pdebug("RobotActionLogin Update %d %s CreateChar fail，retry", GetFD(), m_context->GetAccount().c_str());
		}
		break;
	case ROBOT_LOGIN_STATE_HAS_CHARS:	//Get Session已返回，当前已有角色
		{
			ASSERT(!m_context->guid().empty());
			m_context->send_player_login(m_context->guid().c_str());
			SetLoginState(ROBOT_LOGIN_STATE_LOGIN);
			tea_pdebug("RobotActionLogin Update %s %d %s send login", m_context->GetAccount().c_str(), GetFD(), m_context->guid().c_str());
		}
		break;
	case ROBOT_LOGIN_STATE_LOGIN:	//角色登录中
		//tea_pdebug("RobotActionLogin Update %d %s 等待 login 返回", GetFD(), guid().c_str());
		m_timer_login.Update(diff);
		if(m_timer_login.Passed())
		{
			m_timer_login.Reset2();
			Close();
			tea_pinfo("RobotActionLogin::Update login timeout %s", m_context->GetAccount().c_str());
		}
		break;
	case ROBOT_LOGIN_STATE_LOGIN_DATA_HAS://已收到玩家数据
		m_timer_has_player_data.Update(diff);
		if(m_timer_has_player_data.Passed())
		{
			m_timer_has_player_data.Reset2();
			Close();
			tea_pinfo("RobotActionLogin::Update wait unit data timeout %s", m_context->GetAccount().c_str());
		}
		break;
	case ROBOT_LOGIN_STATE_LOGIN_UNIT_HAS:	//已收到精灵数据
		{
			//要给几秒时间给服务器准备好
			m_timer_wait_unit.Update(diff);
			if(m_timer_wait_unit.Passed())
			{
				SetLoginState(ROBOT_LOGIN_STATE_LOGIN_SUCCEED);			

				uint32 player_count = 0;
				RobotdApp::g_app->ForeachAllContext([&player_count](RobotdContext *context){
					if(context->IsLoginOk())
					{
						//context->PushScenedAction(ROBOT_SCENED_ACTION_TYPE_MAIN);
						player_count++;
					}
				});
				tea_pinfo("%s %d %s login ok,max player %u, login count %u"
					, m_context->GetUserId().c_str(), GetFD(), m_context->guid().c_str(), RobotdApp::g_app->RobotCount(), player_count);
			}
		}
		break;
	case ROBOT_LOGIN_STATE_LOGIN_SUCCEED:
		return true;
		break;
	case ROBOT_LOGIN_STATE_FORCED_INTO:
		{
			m_timer_forced_info.Update(diff);
			if(m_timer_forced_info.Passed())
			{
				m_timer_forced_info.Reset2();
				Close();
			}
			return true;
		}
		break;
	case ROBOT_LOGIN_STATE_WAIT_CLOSE:
		break;
	case ROBOT_LOGIN_STATE_FAILURE:
		break;
	default:
		ASSERT(false);//状态乱了
	}
	return false;
}

void RobotdWarConnection::OpcodeHandlerInitor()
{
	for (uint32 i = 0; i < NUM_MSG_TYPES; i++)
	{
		opcode_handler_[i].handler = nullptr;
	}
	opcode_handler_[SMSG_CHARS_LIST].handler = &RobotdContext::Handle_Chars_List;
	
	opcode_handler_[SMSG_GRID_UNIT_MOVE].handler =  &RobotdContext::Handle_Grid_Unit_Move;	
	opcode_handler_[SMSG_GRID_UNIT_MOVE_2].handler =  &RobotdContext::Handle_Grid_Unit_Move;	
	opcode_handler_[MSG_MOVE_STOP].handler = &RobotdContext::Handle_Unit_Move_Stop;	

	opcode_handler_[SMSG_GRID_UD_OBJECT].handler = &RobotdContext::Handle_Grid_Ud_Object;
	opcode_handler_[SMSG_GRID_UD_OBJECT_2].handler = &RobotdContext::Handle_Grid_Ud_Object;
	opcode_handler_[SMSG_MAP_UPDATE_OBJECT].handler = &RobotdContext::Handle_Map_Info_Update;

	opcode_handler_[SMSG_OPERATION_FAILED].handler = &RobotdContext::Handle_Operation_failed;
	opcode_handler_[SMSG_MERGE_SERVER_MSG].handler = &RobotdContext::Handle_Merge_Server_Msg;

	opcode_handler_[MSG_SYNC_MSTIME].handler =  &RobotdContext::Handle_Sync_Mstime;
	opcode_handler_[SMSG_FIGHTING_INFO_UPDATE_OBJECT].handler =  &RobotdContext::Handle_Fighting_Info_Update_Object;
	opcode_handler_[SMSG_FIGHTING_INFO_UPDATE_OBJECT_2].handler =  &RobotdContext::Handle_Fighting_Info_Update_Object_2;

}

void RobotdWarConnection::CreateConn()
{
	m_context->ResetWarConn(conn_);
}

void RobotdWarConnection::OnClose()
{
	tea_pinfo("RobotdWarConnection::OnClose() %s", m_context->guid().c_str());
	m_context->OnWarClose();
}

bool RobotdWarConnection::LoginUpdate(uint32 diff)
{
	switch (m_login_state)
	{
	case ROBOT_LOGIN_STATE_NONE:
		SetLoginState(ROBOT_LOGIN_STATE_CONN_SERVER_COUNTDOWN);
		break;
	case ROBOT_LOGIN_STATE_CONN_SERVER_COUNTDOWN:
		{
			if(!ConnToNetgd())
			{
				tea_pinfo("RobotdWarConnection::LoginUpdate ConnToNetgd fail %s %s %u", m_context->GetAccount().c_str(), GetConnHost().c_str(), GetConnPort());
				return false;
			}
			tea_pdebug("RobotdWarConnection::LoginUpdate  ConnToNetgd ok %s %s %u", m_context->GetAccount().c_str(), GetConnHost().c_str(), GetConnPort());	
			RobotdGameConnection::SendFirstShakeProtocol(conn_);

			DoRobotWarConn(m_context->GetAccount(), GetFD());
			//连接服务器
			SetLoginState(ROBOT_LOGIN_STATE_CONN_SERVER_OK);
		}
		break;
	case ROBOT_LOGIN_STATE_CONN_SERVER_OK:
		{
			m_context->send_get_session(m_session_str.c_str(),"","");
			SetLoginState(ROBOT_LOGIN_STATE_GETSESSION);
			tea_pdebug("RobotdWarConnection::LoginUpdate %d %s %d %x send GetSession ", GetFD(), m_context->GetAccount().c_str(), diff, this);
		}
		break;
	case ROBOT_LOGIN_STATE_GETSESSION:
		//tea_pdebug("RobotWarActionLogin Update %d %s wait GetSession return", GetFD(), m_context->guid().c_str());
		m_timer_getSession.Update(diff);
		if(m_timer_getSession.Passed())
		{
			m_timer_getSession.Reset2();
			tea_pinfo("RobotdWarConnection::LoginUpdate getsession timeout %s %d", m_context->GetAccount().c_str(), diff);
			Close();
		}
		break;
	case ROBOT_LOGIN_STATE_NOT_CHARS:
		m_context->send_world_war_SC_player_info___();
		//发送玩家信息包上去
		SetLoginState(ROBOT_LOGIN_STATE_LOGIN);
		break;
	case ROBOT_LOGIN_STATE_CREATE_CHARS:
		break;
	case ROBOT_LOGIN_STATE_ERR_NAME:
		break;
	case ROBOT_LOGIN_STATE_HAS_CHARS:
		break;
	case ROBOT_LOGIN_STATE_LOGIN:
		//tea_pinfo("RobotActionLogin Update %d %s wait login return", GetFD(), m_context->guid().c_str());
		m_timer_login.Update(diff);
		if(m_timer_login.Passed())
		{
			m_timer_login.Reset2();
			Close();
			tea_pinfo("RobotActionLogin::Update login timeout %s", m_context->GetAccount().c_str());
		}
		break;
	case ROBOT_LOGIN_STATE_LOGIN_DATA_HAS:
		m_timer_has_player_data.Update(diff);
		if(m_timer_has_player_data.Passed())
		{
			m_timer_has_player_data.Reset2();
			Close();
			tea_pinfo("RobotActionLogin::Update wait unit data timeout %s", m_context->GetAccount().c_str());
		}
		break;
	case ROBOT_LOGIN_STATE_LOGIN_UNIT_HAS:	//已收到精灵数据
		{
			//要给几秒时间给服务器准备好
			m_timer_wait_unit.Update(diff);
			if(m_timer_wait_unit.Passed())
			{
				SetLoginState(ROBOT_LOGIN_STATE_LOGIN_SUCCEED);
			}
		}
	case ROBOT_LOGIN_STATE_LOGIN_SUCCEED:
		break;
	case ROBOT_LOGIN_STATE_FORCED_INTO:
		{
			m_timer_forced_info.Update(diff);
			if(m_timer_forced_info.Passed())
			{
				m_timer_forced_info.Reset2();
				Close();
			}
			return true;
		}
		break;
	case ROBOT_LOGIN_STATE_WAIT_CLOSE:
		break;
	case ROBOT_LOGIN_STATE_FAILURE:
		break;
	case ROBOT_LOGIN_STATE_GAIMIN:
		break;
	case MAX_ROBOT_LOGIN_STATE:
		break;
	default:
		break;
	}
	return true;
}


