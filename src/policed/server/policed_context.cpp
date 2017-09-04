#include "policed_context.h"
#include "log_file_operate.h"
#include "game_police.h"
#include "policed_mongodb.h"
#include <object/OprateResult.h>

//////////////////////////////////////////////////////////////////////////
//for OptcodeHandle
void PolicedContext::OpcodeHandlerInitor()
{
	opcode_handler_[CMSG_WRITE_CLIENT_LOG].handler = std::bind(&PolicedContext::Handle_write_client_log, this, std::placeholders::_1);
	opcode_handler_[CMSG_WRITE_CLIENT_LOG].status = STATUS_NEVER;
	opcode_handler_[CMSG_PLAYER_GAG].handler = std::bind(&PolicedContext::Handle_Player_Gag, this, std::placeholders::_1);
	opcode_handler_[CMSG_PLAYER_GAG].status = STATUS_NEVER;
}


PolicedContext::PolicedContext(ContextManager* mgr):SvrContext(0,mgr),uip(0),m_sid(""),m_pid("")
{
	OpcodeHandlerInitor();
}

void PolicedContext::Handle_write_client_log(packet& pkt)
{
	if(PolicedApp::g_app->m_log_fild_operate)
	{
		PolicedApp::g_app->m_log_fild_operate->WriteClientLog(&pkt);
	}
}

//禁言
void PolicedContext::Handle_Player_Gag(packet& pkt)
{
	char* player_id;
	char* content;
	uint32 hour;
	if(unpack_player_gag(&pkt, &player_id,&hour,&content))
	{
		WriteAttackPacker(GetAccount(),"",CMSG_PLAYER_GAG,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	if(GetFalseGM() == 0 && GetGMLevel() == 0)
		return;
	uint32 end_time = (uint32)time(NULL) + hour*3600;
	PolicedContext *player = ObjMgr.FindPlayer(player_id);
	if(player)
	{
		player->SetGagEndTime(end_time);
	}	
}

void PolicedContext::Login(uint32 fd)
{
	tea_pdebug("player %s login.", guid().c_str());
	if(fd_ != 0)
	{
		PolicedContext *context = new PolicedContext(PolicedApp::g_app);
		context->fd_ = fd;
		context->Close(PLAYER_CLOSE_OPERTE_POLICED_4001, "", true);
		safe_delete(context);
		return;
	}
	m_lguid = guid();
	//当有新玩家进来的时候去网关服注册一下，把日志记录注册到这边
	PolicedApp::g_app->RegSessionOpts(fd);
	PolicedApp::g_app->ChangeSessionID(this,fd);
	SetStatus(STATUS_LOGGEDIN);
	if(!PolicedApp::g_app->IsPKServer())
	{
		g_LOG.SaveHTBasicInfo(this,true);
	}
}

void PolicedContext::Logout()
{
	tea_pdebug("player %s logout.", guid().c_str());
	ASSERT(fd_);
	if(!PolicedApp::g_app->IsPKServer())
	{
		g_LOG.SaveHTBasicInfo(this,false);
	}
	PolicedApp::g_app->ChangeSessionID(this,0);

	ObjMgr.CallDelWatch(guid());
}

void PolicedContext::SetServerId()
{
	string g = guid();
	ASSERT(g.length());
	size_t pos = g.rfind('.');
	ASSERT(pos != string::npos);
	g = g.substr(pos+1,g.length()-pos);

	pos = g.rfind('_');
	ASSERT(pos != string::npos);
	m_sid = g.substr(pos+1, g.length()-pos);
	m_pid = g.substr(0, pos);
}
