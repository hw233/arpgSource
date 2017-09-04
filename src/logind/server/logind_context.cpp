#include "logind_context.h"
#include <comm/log.h>
#include <object/SharedDef.h>
#include <object/OprateResult.h>
#include <time.h>
#include "logind_app.h"
#include <shared/log_handler.h>
#include <shared/chat_analysis.h>
#include "logind_mongodb.h"
#include "logind_cache.h"
#include "map_manager.h"
#include "logind_script.h"
#include "logind_mongodb.h"
#include "logind_object_manager.h"
#include "logind_mongodb.h"
#include <net/http.h>
#include <comm/b64.h>

SessionMap LogindContext::SessionMaps;
SessionQue LogindContext::SessionQues;

//////////////////////////////////////////////////////////////////////////
//for OptcodeHandle
void LogindContext::OpcodeHandlerInitor()
{
	opcode_handler_[CMSG_FORCED_INTO].handler = std::bind(&LogindContext::Handle_Foeced_Into, this, std::placeholders::_1);
	opcode_handler_[CMSG_FORCED_INTO].status = STATUS_AUTHED;

	opcode_handler_[CMSG_GET_CHARS_LIST].handler = std::bind(&LogindContext::Handle_Get_Chars_List, this, std::placeholders::_1);
	opcode_handler_[CMSG_GET_CHARS_LIST].status = STATUS_AUTHED;

	opcode_handler_[CMSG_CHAR_CREATE].handler = std::bind(&LogindContext::Handle_Char_Create, this, std::placeholders::_1);
	opcode_handler_[CMSG_CHAR_CREATE].status = STATUS_AUTHED;

	opcode_handler_[CMSG_CHAR_UPDATE_INFO].handler = std::bind(&LogindContext::Handle_Char_Update_Info, this, std::placeholders::_1);
	opcode_handler_[CMSG_CHAR_UPDATE_INFO].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_DELETE_CHAR].handler = std::bind(&LogindContext::Handle_Delete_Char, this, std::placeholders::_1);
	opcode_handler_[CMSG_DELETE_CHAR].status = STATUS_AUTHED;

	opcode_handler_[CMSG_PLAYER_LOGIN].handler = std::bind(&LogindContext::Handle_Player_Login, this, std::placeholders::_1);
	opcode_handler_[CMSG_PLAYER_LOGIN].status = STATUS_AUTHED;

	opcode_handler_[CMSG_PLAYER_LOGOUT].handler = std::bind(&LogindContext::Handle_Player_Logout, this, std::placeholders::_1);
	opcode_handler_[CMSG_PLAYER_LOGOUT].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_CHANGE_LINE].handler = std::bind(&LogindContext::Handle_Change_Line, this, std::placeholders::_1);
	opcode_handler_[CMSG_CHANGE_LINE].status = STATUS_LOGGEDIN;

	opcode_handler_[CMSG_SHOW_MAP_LINE].handler = std::bind(&LogindContext::Handle_Show_Line, this, std::placeholders::_1);
	opcode_handler_[CMSG_SHOW_MAP_LINE].status = STATUS_LOGGEDIN;
	
	opcode_handler_[CMSG_REGULARISE_ACCOUNT].handler = std::bind(&LogindContext::Handle_Regularise_Account, this, std::placeholders::_1);
	opcode_handler_[CMSG_REGULARISE_ACCOUNT].status = STATUS_LOGGEDIN;	

	opcode_handler_[CMSG_PLAYER_KICKING].handler = std::bind(&LogindContext::Handle_Player_Kicking, this, std::placeholders::_1);
	opcode_handler_[CMSG_PLAYER_KICKING].status = STATUS_LOGGEDIN;
	
	opcode_handler_[CMSG_GOBACK_TO_GAME_SERVER].handler = std::bind(&LogindContext::Handle_GoBackTo_GameServer, this, std::placeholders::_1);
	opcode_handler_[CMSG_GOBACK_TO_GAME_SERVER].status = STATUS_NEVER;

	opcode_handler_[CMSG_WORLD_WAR_CS_PLAYER_INFO].handler = std::bind(&LogindContext::Hanlde_pk_login, this, std::placeholders::_1);
	opcode_handler_[CMSG_WORLD_WAR_CS_PLAYER_INFO].status = STATUS_AUTHED;

	opcode_handler_[CMSG_MODIFY_WATCH].handler = std::bind(&LogindContext::Hanlde_Modify_Watch, this, std::placeholders::_1);
	opcode_handler_[CMSG_MODIFY_WATCH].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_MODIFY_WATCH].cooldown = 200;

	opcode_handler_[CMSG_KUAFU_CHUANSONG].handler = std::bind(&LogindContext::Hanlde_Kuafu_Enter, this, std::placeholders::_1);
	opcode_handler_[CMSG_KUAFU_CHUANSONG].status = STATUS_LOGGEDIN;
	opcode_handler_[CMSG_KUAFU_CHUANSONG].cooldown = 200;
}


//////////////////////////////////////////////////////////////////////////
// for context

LogindContext::LogindContext(int fd,ContextManager *mgr):CommonContext(fd,mgr)
	,m_is_hosting(false),m_bHasReceiveMatchInfo(false)
	,to_mapid(0),to_instanceid(0),to_line_no(0),to_teleport_id("")
{
	//ASSERT(LogindApp::GetConn());
	m_is_save_login_log = false;
	m_is_canInto = false;
	m_bHasplatdata = false;
	m_bHasfcm = false;
	m_isFcm = false;

	OpcodeHandlerInitor();
}

LogindContext::~LogindContext()
{	
}

uint32 LogindContext::FindSessionID(const string &account)
{
	SessionMap::iterator iter = LogindContext::SessionMaps.find(account);
	if (iter == LogindContext::SessionMaps.end())
	{
		return 0;
	}
	return iter->second;
}

//通过guid查找页游会话
LogindContext* LogindContext::FindContextByGuid(string guid)
{
	for (auto it: LogindContext::SessionMaps)
	{
		LogindContext* login_session = dynamic_cast<LogindContext*>(CommonContext::FindContext(it.second));
		if (login_session && login_session->GetGuid() == guid)
		{
			return login_session;
		}
	}	
	return NULL;
}

void LogindContext::Update(uint32 diff)
{
	if(GetStatus() != STATUS_LOGGEDIN)
		return;
	MapMgr->UpdateTeleport(this);
	if(m_player)
		g_Cache.Update(m_player);
}

//登录服崩溃以后重新初始化
void LogindContext::CollapseReset(logind_player *player)
{
	SetPlayer(player);
	player->SetSession(this);
	//TODO: 有必要把这个重置么?
	if (player->GetTeleportInstanceId() != 0) {
		player->SetTeleportInstanceId(0);
	}
	SessionMaps[m_account] = fd_;
	SetStatus(STATUS_LOGGEDIN);
	//发个登录登录服完毕的包给客户端
	Call_join_or_leave_server(m_delegate_sendpkt, 0, SERVER_TYPE_LOGIND, getpid(), LogindApp::g_app->Get_Connection_ID(), uint32(time(nullptr)));
	LogindApp::g_app->RegSessionOpts(fd_);
	g_DAL.UpdateCharsOnline(m_player->GetGuid(), PLAYER_ONLINE_STATE_ONLINE);
	//向中心服重新订阅本tag,必须要创建包，不排除重启期间有新的物品产生
	ObjMgr.CallAddTagWatch(m_lguid);
	//如果为跨服玩家则初始化下跨服相关的信息
	ResetKuafuInfo();
}

//保存玩家登录记录到登录服
void LogindContext::SavePlayerLoginLog()
{
	if(m_is_save_login_log) return;

	WriteLogin(m_account, m_player ? m_player->GetGuid() : 0, m_player ? m_player->GetLevel() : 0, m_remote_ip.c_str()
		, m_player ? m_player->GetMapId() : 0, "", m_player->GetForce());
	//腾讯日志
	if(m_player && LogindApp::g_app->GetPlatformID() == PLATFORM_QQ)
		WriteTXLoginLog(m_account, m_player->GetGuid(),m_player->GetName(),m_player->GetLevel(),"","",GetPlatInfo(m_player->GetPlatData(),"pf"),m_remote_ip.c_str(),(uint32)time(NULL));
	if(m_player)
	{
		m_player->SetLastLoginTime((uint32)time(NULL));
		m_player->AddNumberOfLogins();
		m_player->SetLastLoginIp(m_remote_ip);
		g_DAL.UpdateCharsOnline(m_player->GetGuid(), PLAYER_ONLINE_STATE_ONLINE);
	}
	m_is_save_login_log = true;
}

//保存玩家登出记录到登录服
void LogindContext::SavePlayerLogoutLog()
{
	if(m_player)
	{
		WriteLogout(m_account, m_player->GetGuid(), m_player->GetLevel(), m_remote_ip.c_str()
			, m_player->GetMapId(), "");
		if (m_player->GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_YEYOU_ONLINE))
			m_player->UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_YEYOU_ONLINE);
		g_DAL.UpdateCharsOnline(m_player->GetGuid(), PLAYER_ONLINE_STATE_OUTLINE);
		//腾讯日志
		if(LogindApp::g_app->GetPlatformID() == PLATFORM_QQ)
			WriteTXOffineLog(m_account,m_player->GetGuid(),m_player->GetName(),m_player->GetLevel(),"","",GetPlatInfo(m_player->GetPlatData(),"pf")
				,m_remote_ip.c_str(),(uint32)time(NULL) - m_player->GetLastLoginTime(),m_player->GetOnlineTime(),(uint32)time(NULL));
	}
}

void LogindContext::Call_Create_Connection()
{
	if (!m_player)
	{
		return;
	}

	WorldPacket pkt(INTERNAL_OPT_PLAYER_LOGIN);
	pkt << fd_ << m_player->GetGuid() << uint8(CONTEXT_TYPE_YEYOU);
	
	//通知中心服建立连接
	LogindApp::g_app->SendToCentd(pkt);
	//通知应用服建立连接
	LogindApp::g_app->SendToAppd(pkt);
	//通知日志服建立连接
	LogindApp::g_app->SendToPoliced(pkt);
}

void LogindContext::Hanlde_Modify_Watch(packet& pkt)
{
	uint8 opt;
	uint32 cid;
	char *key;
	if(unpack_modify_watch(&pkt,&opt,&cid,&key))
	{
		WriteAttackPacker(GetAccount(), GetGuid(),pkt.head->optcode,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	if(cid == 0)
		cid = GetFD();
	auto *o = ObjMgr.Get(key);
	if(!o)
	{
		tea_pwarn("i'm:[%s] calladdwatch fd:%u obj_key:[%s] but unfound!",GetGuid().c_str(),cid,key);
		return;
	}
	if(opt)
		ObjMgr.CallAddWatch(cid,key);
	else
		ObjMgr.CallDelWatch(cid,key);
}

