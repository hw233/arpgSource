#include "logind_app.h"
#include "logind_context.h"
#include "map_manager.h"
#include "logind_script.h"
#include "logind_object_manager.h"
#include "logind_mongodb.h"
#include "logind_cache.h"
#include "logind_gm_table.h"
#include <object/SharedDef.h>
#include <object/OprateResult.h>
#include <shared/log_handler.h>
#include <shared/util.h>

//////////////////////////////////////////////////////////////////////////
//for login

/*登陆*/
void  LogindContext::Handle_Player_Login(packet &pkt)
{
    char *guid;

	/*uint16_t len = *((uint16_t*)(pkt.content));
	printf("%d = ", len);
	for (int i = 2; i < pkt.head->len-4; ++ i) {
		printf("%d ", *(pkt.content + i));
	}
	puts("");*/

    if(unpack_player_login(&pkt, &guid))
	{
		WriteAttackPacker(m_account,"",pkt.head->optcode,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	tea_pdebug("LogindContext::Handle_Player_Login account %s , guid %s", m_account.c_str(), guid);
	//验证账号是否被封
	if(GmTable.GetLockInfo(m_account) > 0 && GmTable.GetLockInfo(m_account) > (uint32)time(NULL))
	{
		string str = "";
		DoGetCharacters(KICKING_TYPE_GM_LOCK_ACCOUNT,str);
		Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE49,str);
		tea_pinfo("player logind lock account is %s, time_end is %u",m_account.c_str(),GmTable.GetLockInfo(m_account));
		return;
	}
	string lguid = guid;

	account_table *info = g_Cache.GetAccount(m_account);
	ASSERT(info);
	//验证一下是不是该玩家下面的角色
	vector<char_create_info> char_list;
	g_Cache.GetCharList(m_account,char_list);
	bool is_my_char = false;
	for (auto it:char_list)
	{
		if(lguid == it.guid)
		{
			is_my_char = true;
			break;
		}
	}

	if(!is_my_char)
	{
		WriteAttackPacker(m_account,"",pkt.head->optcode,ACCACK_PACKET_TYPE_DATA,"");
		Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE6,"");
		return;
	}

	//直入登录的排除
	this->m_lguid = lguid;
	LogindContext::SessionQues.push_back(fd_);
}

//保存到数据库
void LogindContext::SaveToDB(bool fsync /*= false*/)
{
	vector<GuidObject*> vec;
	ObjMgr.GetDataSetAllObject(m_lguid,vec);
	ASSERT(!vec.empty());
	for (auto it:vec)
	{
		BinLogObject *obj = dynamic_cast<BinLogObject*>(it);
		ASSERT(obj);
		g_DAL.SaveBinlog(obj, fsync);
	}
}

void LogindContext::SetPayLevel()
{
	if(m_player)
	{
		uint8 level = DoPlayerPayLevel(m_player);
		if(m_player->GetByte(PLAYER_FIELD_BYTES_2, 1) != level)
			m_player->SetByte(PLAYER_FIELD_BYTES_2, 1, level);
	}
}

uint32 LogindContext::LoginPlayerCount()
{
	uint32 result = 0;
	for (auto it:LogindApp::g_app->context_map_)
	{
		if(it.second->GetStatus() == STATUS_TRANSFER || it.second->GetStatus() == STATUS_LOGGEDIN)
		{
			result++;
		}
	}
	return result;
}

//玩家小退
void LogindContext::Handle_Player_Logout(packet& pkt)	
{
	WriteAttackPacker(m_account,m_lguid,CMSG_PLAYER_LOGOUT,ACCACK_PACKET_TYPE_DATA,"not logout");
}

void LogindContext::Handle_Change_Line(packet& pkt) {
	uint32 lineNo;
	if(unpack_change_line(&pkt, &lineNo))
	{
		WriteAttackPacker(m_account,"",pkt.head->optcode,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	uint32 mapid = this->GetPlayer()->GetMapId();
	uint32 lineC = this->GetPlayer()->GetMapLineNo();

	// 不换线就没必要发
	if (lineNo < 0 || lineC == lineNo) {
		return;
	}

	if (DoIsWorldMapID(mapid)) {
		if (MapMgr->isMapLineCreated(mapid, lineNo) || 1 <= lineNo && lineNo <= MAX_DEFAULT_LINE_COUNT) {
			uint32 conn = MapMgr->GetScenedConn(MapMgr->FindInstance(mapid, "", lineC));
			WorldPacket pkt_scened(INTERNAL_OPT_LOGIND_TELEPORT);
			pkt_scened << this->GetPlayer()->GetGuid() << lineNo;
			LogindApp::g_app->SendToScened(pkt_scened, conn);
		}
	}
}

void LogindContext::Handle_Show_Line(packet& pkt) {
	uint32 mapid = this->GetPlayer()->GetMapId();
	std::map<uint16, uint32> ln = MapManager::GetMapLines(mapid);
	
	uint8 limit;
	OnGetMapLineCount(mapid, limit);
	std::set<uint16> ss;

	std::vector<line_info> vec;
	for (auto it : ln) {
		uint16 lineNo = it.first;
		uint32 instance_id = it.second;

		ss.insert(lineNo);

		int index = MapMgr->FindInstance(mapid, "", lineNo);
		uint32 count = MapMgr->PlayerCount(index);

		line_info info;
		memset(&info,0,sizeof(info));
		info.lineNo = lineNo;
		info.rate = (uint8)ceil(count*100/limit);

		vec.push_back(info);
	}

	Call_send_map_line(this->m_delegate_sendpkt, vec);
}

//登录完成，进行最后操作
void LogindContext::LoginOK()
{
	tea_pdebug("LogindContext::LoginOK player %s, fd %u", m_lguid.c_str(), GetFD());
	logind_player *p = ObjMgr.FindPlayer(m_lguid);
	ASSERT(p);
	if(!IsKuafuPlayer())
	{
		//初始化一下数据
		if(!p->InitDatabase())
		{
			tea_perror("LogindContext::Login player guid ==  %s,not InitDatabase ", m_lguid.c_str());
			Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE9,"");
			return ;
		}

		//看看有没有玩家数据需要修复或者升级的
		bool repait_data = false;
		DoRepairPlayerData(m_account, m_temp_vec, repait_data);
		if(!repait_data)
		{
			tea_pinfo("repair player account %s  %s  err, ", m_account.c_str(), m_lguid.c_str());
			Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE10,"");
			return;

		}
	}

	//关联m_player
	if(!SetPlayer(p))
		return;

	m_player->SetSession(this);
	ASSERT(m_player);

	//跨服的玩家
	if(IsKuafuPlayer())
	{	
		m_player->SaveDBKuafuInfo(m_warid, m_kuafutype, (uint16)m_number, m_backsvrname);
	}
	else
	{	
		//情况跨服信息
		m_player->ClearDBKuafuInfo();

		//重设账号信息
		account_table * accountInfo = g_Cache.GetAccount(m_account);
		ASSERT(accountInfo);

		//重新刷新一下防沉迷的信息
		if(m_bHasfcm && !accountInfo->is_FCM)
			m_player->SetFCMLoginTime(-1);
		//从非防沉迷到防沉迷的
		else if(m_bHasfcm && accountInfo->is_FCM && m_player->GetFCMLoginTime() == (uint32)-1)
			m_player->SetFCMLoginTime(0);
		//计算处理防沉迷相关时间
		m_player->CalculFCMLogoutTime();	
		//设置一下RMB充值等级
		m_player->GetSession()->SetPayLevel();	
		if(m_bHasplatdata && m_player->GetPlatData() != accountInfo->platdata)
			m_player->SetPlatData(accountInfo->platdata);
		if(g_Config.gm_open)
			m_player->SetGmNum(3);
		

		//设置下是否托管
		if(m_player->GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_LOGIN) != m_is_hosting)
		{
			if(m_is_hosting)
				m_player->SetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_LOGIN);
			else
				m_player->UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_IS_HOSTING_LOGIN);
		}

		//设置页游手游在线情况
		if (!m_player->GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_YEYOU_ONLINE))
			m_player->SetFlags(PLAYER_APPD_INT_FIELD_FLAGS_YEYOU_ONLINE);

		bool is_phone_online = MobileContext::FindSessionID(m_account) != 0 ? true: false;
		if (is_phone_online)
		{//手游在线
			if (!m_player->GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_PHONE_ONLINE))
				m_player->SetFlags(PLAYER_APPD_INT_FIELD_FLAGS_PHONE_ONLINE);
		}
		else
		{//手游不在线
			if (m_player->GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_PHONE_ONLINE))
				m_player->UnSetFlags(PLAYER_APPD_INT_FIELD_FLAGS_PHONE_ONLINE);
		}
	}

	// 经验差值
	double picked = p->GetDouble(PLAYER_EXPAND_INT_XP_PICKED);
	if (picked > p->GetDouble(PLAYER_EXPAND_INT_XP_ADDED)) {
		picked = p->GetDouble(PLAYER_EXPAND_INT_XP_ADDED);
	}
	p->SubDouble(PLAYER_EXPAND_INT_XP_ADDED, picked);
	p->SetDouble(PLAYER_EXPAND_INT_XP_PICKED, 0);

	//通知其他服务器为该玩家建立会话信息	
	Call_Create_Connection();
	
	//客户端对玩家对象的监听(由于客户端的先后顺序 玩家数据先发)
	ObjMgr.CallAddTagWatch(fd_, m_lguid);

	string factionId = p->GetFactionId();

	//登录完成，准备传送	
	MapMgr->PlayerLogin(m_player);
	// TODO:直接登录, 设置好登录的map信息
	this->onRealLoginOk(factionId);

}

void LogindContext::onSetInvitedTeleportInfo(const string& factionId, uint32 mapid, uint32 x, uint32 y, const string& generalId) {

}

void LogindContext::onRealLoginOk(const string& factionId) {

	//登录完毕，状态置一下
	SetStatus(STATUS_LOGGEDIN);

	//发个登录登录服完毕的包给客户端
	Call_join_or_leave_server(m_delegate_sendpkt, 0, SERVER_TYPE_LOGIND, getpid(), LogindApp::g_app->Get_Connection_ID(), uint32(time(nullptr)));

	//保存一下登录记录
	SavePlayerLoginLog();

	//监听帮派的
	if (!factionId.empty()) {
		ObjMgr.CallAddWatch(fd_, factionId);
	}

	//发送世界变量
	ObjMgr.CallAddWatch(fd_, GLOBAL_OBJ_GUID);
	ObjMgr.CallAddWatch(fd_, GLOBAL_GAME_CONFIG);
	ObjMgr.CallAddWatch(fd_, GLOBAL_RIGHT_FLOAT_GUID);
}

void LogindContext::OnClosed()
{
	tea_pdebug("player %s %u logout. begin", m_lguid.c_str(), fd_);
	if(m_player)
	{
		tea_pdebug("player %s %u logout. remove player", m_lguid.c_str(), fd_);
		ASSERT(GetStatus() == STATUS_LOGGEDIN);
		//通知其他服务器玩家下线
		WorldPacket pkt(INTERNAL_OPT_PLAYER_LOGOUT);
		pkt << fd_ << m_player->GetGuid();
		//通知应用服
		if(!LogindApp::g_app->SendToAppd(pkt))
		{
			tea_pdebug("player %s %u logout. appd collapse", m_lguid.c_str(), fd_);
		}
		//通知日志服
		if(!LogindApp::g_app->SendToPoliced(pkt))
		{
			tea_pdebug("player %s %u logout. policed collapse", m_lguid.c_str(), fd_);
		}

		//从地图管理器退出
		MapMgr->PlayerLogout(m_player);

		//玩家离开记录玩家退出时间
		m_player->SetLastLogoutTime((uint32)time(NULL));
		//// 只有在冒险世界下线才需要设置
		//if (m_player->GetPickRiskRewardFlag() == 1) {
		//	m_player->SetPickRiskRewardFlag(0);
		//	m_player->SetLastLeaveRiskTime((uint32)time(NULL));
		//}

		//玩家下线做点什么
		DoPlayerLogout(m_player);

		//数据对象的session置空
		m_player->SetSession(NULL);

		//进入等待移除列表
		g_Cache.AddLogoutPlayer(m_lguid);
	}
	
	//从账号映射表删除
	if (fd_ && fd_ == LogindContext::FindSessionID(m_account))
	{
		LogindContext::SessionMaps.erase(m_account);
	}

	//保存一下登出LOG
	SavePlayerLogoutLog();
	//从app中移除自己
	LogindApp::g_app->RomoveContext(this);
	tea_pdebug("player %s %u logout. end", m_lguid.c_str(), fd_);
}

//踢人
void LogindContext::Handle_Player_Kicking(packet& pkt)
{
	char* player_id;
	if(unpack_player_kicking(&pkt, &player_id))
	{
		WriteAttackPacker(m_account,"",CMSG_PLAYER_KICKING,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	if(GetPlayer()->GetFalseGM() == 0 && GetPlayer()->GetGMLevel() == 0)
		return;
	logind_player *player = FindPlayer(player_id);
	if(player && player->GetSession())
	{
		tea_pdebug("Handle_Player_Kicking player guid == %s ",player->GetGuid().c_str());
		player->GetSession()->Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE12,"");
	}
}
