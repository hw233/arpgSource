
#include "logind_app.h"
#include "logind_context.h"
#include "logind_context_login_mgr.h"
#include <shared/log_handler.h>
#include <object/OprateResult.h>

#include "logind_mongodb.h"
#include "logind_cache.h"
#include "logind_script.h"
#include "map_manager.h"
#include "http_handler.h"

/*获取sessionKey对象*/
bool LogindContext::Get_Session(std::map<string,string>& querys)
{
	 //如果已经登录过了
    if(!m_account.empty() || m_player)
    {
    	tea_perror("account[%s] re get session", m_account.c_str());	
		//Call_operation_failed(m_delegate_sendpkt,OPERTE_TYPE_CLOSE,PLAYER_CLOSE_OPERTE_LOGDIN_ONE40,"");
    	return false;
    }

	//解析URL
	string pid = querys["pid"],sid = querys["sid"],uid = querys["uid"],fcm = querys["indulge"], platdata = querys["platdata"];
	//玩家是否托管的登录
	m_is_hosting = querys["hosting"]=="y";
	bool indulge = fcm=="y";
	m_isFcm = indulge;
	if (!fcm.empty())
		m_bHasfcm = true;
	if (!platdata.empty())
		m_bHasplatdata = true;
	
	m_watcher_guid = querys["watcher"];
	//是观察者
	if (!m_watcher_guid.empty())
	{
		//发内部协议到场景服,给这个连接下发对应的地图数据等
		m_general_id = querys["generalid"];
		if(!SendToScenedAddMapWatcher())
			return false;
	}
	//帐户名称，并且判断一下是否超长
	m_account = pid+"_"+sid+"_"+uid;
	if(m_account.size() >= sizeof((account_table*)NULL)->name)
	{
		tea_perror("m_account_string:%s too long!",m_account.c_str());
		Call_operation_failed(m_delegate_sendpkt,OPERTE_TYPE_CLOSE,PLAYER_CLOSE_OPERTE_LOGDIN_ONE41,"");
		return false;
	}

	//取出跨服信息
	m_backsvrname = querys["backsvrname"];
	m_kuafutype = (uint8)atol(querys["kuafutype"].c_str());
	m_warid = atol(querys["warid"].c_str());
	m_kuafu_svrname = querys["battlesvrname"];
	m_number = atol(querys["kuafunumber"].c_str());
	m_kuafu_reserve = atol(querys["kuafureserve"].c_str());
	m_kuafu_reserve_str = querys["reservestr"];
	string invited = querys["factionguid"];

	//根据规则生成用户名称,如果超过长度就关闭连接
	m_from_server_name = pid+"_"+sid;
	string svr_name = m_from_server_name;
	if (!m_kuafu_svrname.empty())
		svr_name = m_kuafu_svrname;

	//验证session_key是否可以登录这个服务器
	if(!Get_Server_List(svr_name))
	{
		//单网关多中心服的部署方式，这个打印会非常常见，打印等级不需要高
		tea_pdebug("m_from_server_name:%s is't in this server",m_from_server_name.c_str());
		//Call_operation_failed(m_delegate_sendpkt,OPRATE_TYPE_LOGIN,OPRATE_RESULT_PID_OR_SID_ERROR, "");
		//Call_operation_failed(m_delegate_sendpkt,OPERTE_TYPE_CLOSE,PLAYER_CLOSE_OPERTE_LOGDIN_ONE43,"");
		return false;
	}

	tea_pdebug("account %s getsession!", m_account.c_str());

	//状态变成验证通过	
	SetStatus(STATUS_AUTHED);

	uint32 old_fd = LogindContext::FindSessionID(m_account);
	//当前已经在线 或者 在session表里面找到成员也认为是已经有角色在线
	if (old_fd)
	{
		//发条错误信息
		tea_pwarn("LogindContext::Get_Session %s, but char online", m_account.c_str());

		//通知网关服,可以把顶人的协议转发过来，这样才有办法顶掉其他人	
		LogindApp::g_app->RegSessionOpts(GetFD(),CMSG_FORCED_INTO);			
		//m_is_canInto = true;

		//通知角色已经登录
		Call_operation_failed(m_delegate_sendpkt,OPRATE_TYPE_LOGIN,OPRATE_RESULT_LOGINED_IN, m_account.c_str());

		//通知已经登录的客户端
		auto old_context = LogindContext::FindContext(old_fd);
		if(old_context)
			Call_operation_failed(old_context->m_delegate_sendpkt,OPRATE_TYPE_LOGIN,OPRATE_RESULT_OTHER_LOGINED, m_account.c_str());

		return true;
	}


	//如果不是跨服
	if(m_kuafu_svrname.empty())
	{
		//如果该平台帐户初次登录				
		account_table *info = g_DAL.UpsertAccount(m_account,pid,sid,uid,indulge,m_remote_ip,platdata);
		ASSERT(info);

		int fd = GetFD();
		g_DAL.GetCharsList(m_account,[fd,this,invited](bool sucess,vector<char_create_info> chars) {
			auto *session = LogindContext::FindContext(fd);
			if(!session) return;
			string faction_name;
			string queen_name;
			uint8 icon = 0;
			// 如果是被邀请的需要显示当前帮派信息
			if (chars.size() == 0 && !invited.empty()) {
				// 查询帮派信息
				BinLogObject *factionInfo = (BinLogObject*)ObjMgr.Get(invited);
				if (factionInfo) {
					faction_name = factionInfo->GetStr(BINLOG_STRING_FIELD_NAME);
					queen_name = factionInfo->GetStr(FACTION_STRING_FIELD_MANGER_NAME);
					icon = factionInfo->GetByte(FACTION_INT_FIELD_FLAGS_ID, 0);
				}
			}
			Call_chars_list(session->m_delegate_sendpkt, chars, faction_name.c_str(), queen_name.c_str(), icon);
		});
	}
	else
	{
		string faction_name;
		string queen_name;
		//是跨服的话，直接给个空角色列表就可以了
		vector<char_create_info> char_list;
		Call_chars_list(m_delegate_sendpkt, char_list, faction_name.c_str(), queen_name.c_str(), 0);
	}

	//把其他协议注册一下
	LogindApp::g_app->RegSessionOpts(fd_);
	SessionMaps[m_account] = GetFD();					//用于查找在线Session			

	//通过了把这条协议注册给中心服，设置为默认连接	
	LogindApp::g_app->RegSessionOpts(GetFD(),CMSG_GET_SESSION,ServerList.GetCentdFd());

	//为客户端监听其最新的模块信息
	ObjMgr.CallAddWatch(fd_, GLOBAL_CLIENT_GAME_CONFIG);	

	return true;
}



uint16 __CheckName1(string name)
{

	if (Pingbi((char*)name.c_str()))
	{
		tea_pdebug("error: has pingbi !!!");
		return OPRATE_RESULT_NAME_HAS_PINGBI;
	}

	if (name == "")
	{
		tea_pdebug("error: name is null !!!");
		return OPRATE_RESULT_NAME_ILLEGAL;
	}
	auto& vec = g_Config.g_cant_make_name;
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		if(name.find(*it) != string::npos)
		{
			tea_pdebug("error: has illegal char");
			return OPRATE_RESULT_NAME_ILLEGAL;
		}
	}
	return OPRATE_RESULT_SUCCESS;
}

uint16 __CheckName2(string name, int additionSize)
{
	if (name.size() > uint32(20 + additionSize))
	{
		tea_pdebug("error:name is too long!!!");
		return OPRATE_RESULT_NAME_TOO_LONG;
	}

	bool has_err;
	string guid = g_DAL.FindGuidByName(name, has_err);
	if(has_err)
	{
		tea_pinfo("g_DAL.FindGuidByName(name, has_err); %s ", name.c_str());
		return OPRATE_RESULT_DB_RESULT_ERROR;
	}

	if (!guid.empty())
	{
		tea_pdebug("error: name repeat : %s",name.c_str());
		return OPRATE_RESULT_NAME_REPEAT;		
	}

	return OPRATE_RESULT_SUCCESS;
}

void LogindContext::Handle_Foeced_Into(packet &)
{
	//if (!m_is_canInto)
	//{
	//	WriteAttackPacker(m_account,"",CMSG_FORCED_INTO,ACCACK_PACKET_TYPE_DATA,"没权限");
	//	return;
	//}
	//m_is_canInto = false;
	uint32 sessionid = LogindContext::FindSessionID(m_account);

	if(sessionid)
	{
		LogindContext *session = dynamic_cast<LogindContext*>(CommonContext::FindContext(sessionid));
		tea_pdebug("forced_into session,%s",m_account.c_str());
		//通知角色已经在别处登录
		Call_operation_failed(session->m_delegate_sendpkt,OPRATE_TYPE_LOGIN,OPRATE_RESULT_OTHER_LOGINED,"");
		session->Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE52,"",true);
	}
	m_account = "";
}

/*获取角色列表*/
void LogindContext::Handle_Get_Chars_List(packet &)
{
	//Get_Chars_List();
}

/*检查名称*/
bool  LogindContext::Check_Name(string& name, string& realName, string& prefix)
{
	////在创建角色的用户名中加入平台ID,服务器ID并且
	string charName = string(name);
	//没加区服之前的校验
	uint16 check_reslut = __CheckName1(charName);
	if(check_reslut != OPRATE_RESULT_SUCCESS) {
		Call_operation_failed(m_delegate_sendpkt, OPRATE_TYPE_LOGIN, check_reslut, "");
		return false;
	}
	////按照规则拼结用户名
	// prefix = accountInfo->pid + "," + accountInfo->sid + ",";
	charName = prefix;
	int additionSize = prefix.size();
	charName += string(name);
	//加了区服以后的校验
	check_reslut = __CheckName2(charName, additionSize);
	if(check_reslut == OPRATE_RESULT_DB_RESULT_ERROR) {
		//数据库异常无法创建
		return false;
	}
	if(check_reslut != OPRATE_RESULT_SUCCESS) {
		Call_operation_failed(m_delegate_sendpkt, OPRATE_TYPE_LOGIN, check_reslut, "");
		return false;
	}
	realName = charName;

	return true;
}

// 更新名字
void LogindContext::Char_Name_Update(string &name) {
	logind_player *player = GetPlayer();
	player->SetName(name);
	g_DAL.InsertGuidByName(player->guid(), name);
}			

/*角色创建*/
void LogindContext::Handle_Char_Create(packet &pkt)
{
	//不是游戏服不能创建角色
	if(!LogindApp::g_app->IsGameServer())
	{
		return;
	}
	
    char_create_info *info;

    if(unpack_char_create(&pkt, &info))
	{
		WriteAttackPacker(m_account,"",CMSG_CHAR_CREATE,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}

	// 玩家种类只能1-6
	if (info->gender < 1 || info->gender > 6) {
		Call_operation_failed(m_delegate_sendpkt, OPRATE_TYPE_LOGIN, OPRATE_RESULT_GENDER_ILLEGAL, "");
		return;
	}

	const uint32 MAX_CHAR_COUNT = 1;

	//出来阵营一定是0了
	info->faction = 0;

	//账号信息
	account_table *accountInfo = g_Cache.GetAccount(m_account);
	ASSERT(accountInfo);
	////在创建角色的用户名中加入平台ID,服务器ID并且
	string charName = info->name;
	string realName;
	string prefix = string(accountInfo->pid) + "," + string(accountInfo->sid) + ",";
	if (!this->Check_Name(charName, realName, prefix)) {
		return;
	}
	//账号情况
	int login_time = 0;			
	if(!accountInfo->is_FCM)
		login_time = -1;
	else
		login_time = 0;

    //生成新的GUID
	uint32 lguid = g_GuidMgr.NewIndex(ObjectTypePlayer);
    string guid = g_GuidMgr.MAKE_NEW_GUID(ObjectTypePlayer,lguid,m_from_server_name.c_str());

    //初始化新角色属性
    logind_player *new_player = new logind_player;	
	new_player->SetGuid(guid);
	new_player->SetOwner(guid);
	new_player->SetAccount(m_account.c_str());
	new_player->InitNewPlayer(realName, info->faction, info->gender, info->head_id, info->hair_id,info->race,login_time == 0);
	new_player->SetCreateLoginIp(m_remote_ip);
	new_player->SetPlatData(accountInfo->platdata);
	new_player->SetFactionId("");
	
	/**
	new_player->SetStr(PLAYER_STRING_FIELD_INVITE_FACTION_GUID, info->inviteGuid);
	string invited = string(info->inviteGuid);
	// 是女性角色且不是邀请的
	if ((info->gender & 1) == 0 && invited.empty()) {
		string faction_name = string(info->faction_name);
		if (!faction_name.empty()) {
			new_player->SetStr(PLAYER_STRING_FIELD_CREATE_FACTION_NAME, faction_name);
			new_player->SetUInt32(PLAYER_INT_FIELD_CREATE_ICON, 1);
		}
	}
	*/
	//记一下日志
	//g_DAL.InsertGuidByName(guid,charName);	
	WriteCreateRole(m_account, new_player->GetGuid(), new_player->GetName().c_str(), m_remote_ip);
	//腾讯日志
	if(LogindApp::g_app->GetPlatformID() == PLATFORM_QQ)
		WriteTXUserLog(m_account,new_player->GetGuid(), new_player->GetName().c_str(),1,GetPlatInfo(accountInfo->platdata,"pf"),(uint32)time(NULL));
	//这样才会存，不然永远不存
	new_player->m_db_hashcode = new_player->GetHashCode();

	//保存新角色	
	string data,data_str;
	new_player->ToString(data,data_str);
	g_DAL.AddChars(new_player->GetGuid(), m_account, realName, m_remote_ip, data, data_str);
	g_Cache.AddAccountToChar(m_account, new_player->GetGuid());
	g_Cache.SaveAccountCharInfo(m_account, new_player->GetGuid());
	g_DAL.InsertGuidByName(guid, realName);
	//g_LOG.AddHtBaiscInfo(new_player->GetGuid(), m_account, info->name, m_remote_ip);

	//put到中心服，并回调
	m_temp_vec.push_back(new_player);

	m_lguid = guid;
	SetStatus(STATUS_TRANSFER2);
	LogindContext::SessionQues.push_back(fd_);
}

void LogindContext::Handle_Char_Update_Info(packet &pkt)
{
	char_create_info *info;

	if(unpack_char_create(&pkt, &info))
	{
		WriteAttackPacker(m_account,"",CMSG_CHAR_UPDATE_INFO,ACCACK_PACKET_TYPE_UNPACK,"");
		return;
	}
	logind_player *player = GetPlayer();
	//是否已有姓名
	if(player->GetName() != "")
		return;

	//如果是壮男或者俊男，那只能是男性
	if ((info->race == RACE_STRONG || info->race == RACE_HANDSOME)&&info->gender != CHAR_GENDER_MALE)
	{
		Call_operation_failed(m_delegate_sendpkt, OPRATE_TYPE_LOGIN, OPRATE_RESULT_NAME_ILLEGAL, "");
		return;
	}
	//如果是萝莉或者美女，那只能是女性
	if ((info->race == RACE_LAURIE || info->race == RACE_BEAUTY)&&info->gender != CHAR_GENDER_FEMALE)
	{
		Call_operation_failed(m_delegate_sendpkt, OPRATE_TYPE_LOGIN, OPRATE_RESULT_NAME_ILLEGAL, "");
		return;
	}
	//账号信息
	account_table *accountInfo = g_Cache.GetAccount(m_account);
	if(!accountInfo)
	{
		Close(PLAYER_CLOSE_OPERTE_LOGDIN_2047, "");
		return;
	}
	//在创建角色的用户名中加入平台ID,服务器ID并且
	string charName = info->name;
	string realName;
	string prefix = string(accountInfo->pid) + "," + string(accountInfo->sid) + ",";
	if (!this->Check_Name(charName, realName, prefix)) {
		return;
	}
	
	player->SetGender(info->gender);
	player->SetRace(info->race);
	player->SetHair(info->hair_id);
	player->SetHead(info->head_id);

	this->Char_Name_Update(realName);	
	Call_operation_failed(m_delegate_sendpkt,OPRATE_TYPE_LOGIN,OPRATE_RESULT_SUCCESS,"");
}

/*角色删除*/
void  LogindContext::Handle_Delete_Char(packet &pkt)
{
	//12J中没有这个功能
}

/*特殊帐户改名*/
void LogindContext::Handle_Regularise_Account(packet &pkt)
{
	//12J中没有这个功能
}

//发内部协议到场景服,给这个连接下发对应的地图数据等
bool LogindContext::SendToScenedAddMapWatcher()
{
	if (m_general_id.empty())
		return false;

	int32 index = MapMgr->FindInstance(m_general_id);
	if (index < 0)
		return false;		//没有找到
	uint32 scened_conn = MapMgr->GetScenedConn(index);
	if (!scened_conn)
		return false;	

	WorldPacket spkt(INTERNAL_OPT_ADD_MAP_WATHER);
	spkt << GetFD() << m_watcher_guid << m_general_id;
	LogindApp::g_app->SendToScened(spkt, scened_conn);
	return true;
}

