#include "logind_app.h"
#include "logind_context.h"
#include "logind_script.h"
#include "logind_mongodb.h"
#include "logind_cache.h"
#include "map_manager.h"
#include "logind_context_login_mgr.h"
#include <shared/log_handler.h>
#include <object/OprateResult.h>
#include <comm/b64.h>
#include <comm/crypt.h>

#include "mobile_context.h"

//////////////////////////////////////////////////////////////////////////
//for LogindApp

//存储着所有连接ID的IP和端口
typedef std::pair<string,uint16> host_info;
typedef std::unordered_map<uint32, host_info> HostInfoMap;
static HostInfoMap gHostInfoMap;

//存储着所有的IP关联的sessionkey
static std::unordered_map<string,string> ip2sessionkey;

//对象更新
int LogindApp::on_centd_guid_object_table(tcp_connection *,server_packet *pkt)
{ 
	static ByteArray bytes ;
	bytes.clear();
	bytes.writeBytes(pkt->content,pkt->wpos);
	bytes.position(0);
	ObjMgr.HandlePacket(0,pkt->head->optcode,bytes);
	return 0;
}

int LogindApp::on_netgd_create_conn(tcp_connection *, server_packet *pkt)
{
	int fd = 0;
	string remote_ip;
	uint16 remote_port;
	int ret = packet_read(pkt,(char*)&fd,sizeof(int));
	*pkt >> remote_ip >> remote_port;
	if(g_app->GetStatus() != APP_STATUS_READY_OK)
	{
		tea_pwarn("on_netgd_create_conn server data loading, fd:%u,ip:%s,port:%u",fd,remote_ip.c_str(),remote_port);	
		return 0;
	}
	gHostInfoMap[fd] = std::make_pair(remote_ip, remote_port);
	tea_pdebug("on_netgd_create_conn fd:%u,ip:%s,port:%u",fd,remote_ip.c_str(),remote_port);	
	return 0;
}

int LogindApp::on_netgd_destory_conn(tcp_connection *, server_packet *pkt)
{
	int fd = 0;
	int ret = packet_read(pkt,(char*)&fd,sizeof(int));
	//fd可能是手游或者页游
	CommonContext *context = CommonContext::FindContext(fd);	
	if(context)
	{
		tea_pdebug("on_netgd_destory_conn fd:%u,ip:%s,port:%u",fd,context->m_remote_ip.c_str(),context->m_remote_port);
		if(context->GetAccount().empty())
		{
			context->OnClosed();
			safe_delete(context);
		}
		else
		{
			g_app->m_login_queue->PushAction(context->GetGuid(), context->GetFD(), CONTEXT_LOGIN_ACTION_CLOSE);
		}
	}
	else
	{
		tea_pwarn("on_netgd_destory_conn but not found! fd:%u",fd);
	}
	gHostInfoMap.erase(fd);
	return 0;
}

//解开客户端传送过来了,目前主要的参数及类型
//pid,sid,uid,indulge,hosting,otherdata,time
bool ParseSessionKey(const char *session_key,std::map<string,string>& querys)
{	
	/*必须以等号开头*/
	if(*session_key != '?')
		return false; 

	std::map<string,string> tmpQuerys;
	if(!parseURL(session_key+1,tmpQuerys))
		return false;

	string auth = tmpQuerys["auth"];	//auth数据的校验码，用来校验数据是否恶意修改。
	string sign = tmpQuerys["sign"];	//传送数据，包括用户帐号等信息。数据通过base64编码。

	//auth以base64编码,解码
	static char tmp[1024];
	memset(tmp, 0, sizeof(tmp));

	if(base64_decode(tmp,sizeof(tmp),auth.c_str(),auth.size()+1) != 0)
	{
		tea_perror("base64_decode");
		return false;
	}

	if(!parseURL(tmp,querys))
		return false;

	//验证key的有效性
	//84adc98ee63b26e2063a796ac982ad77
	string auth_temp = auth + g_Config.login_key;
	const char * md5_str = MD5CryptToStr32((int8*)auth_temp.c_str(),auth_temp.size(),tmp);
	if(sign != md5_str && querys["uid"]!= g_Config.test_server_live_uid)
	{
		//用万能密钥再校验下
		auth_temp = auth + WANNENG_LOGIN_KEY;
		md5_str = MD5CryptToStr32((int8*)auth_temp.c_str(),auth_temp.size(),tmp);
		if(sign != md5_str)
		{
			tea_pwarn("ParseSessionKey sign:%s md5_str:%s",sign.c_str(),md5_str);
			return false;
		}
	}

	//非空选项控制
	if( querys["pid"].empty() || querys["sid"].empty() || querys["uid"].empty())
	{
		tea_pwarn("pid:%s sid:%s uid:%s failed",querys["pid"].c_str(),querys["sid"].c_str(),querys["uid"].c_str());	
		return false;
	}

	return true;
}

//传入session验证是否已经过期
bool CheckSessionKeyTimeout(uint32 fd,const char* session_key, uint32 key_time, uint32 gaptime)
{
	//校验时间
	if(!g_Config.check_session_key_time)
		return true;

	//本连接的IP
	string this_ip = gHostInfoMap[fd].first;
	if (this_ip.empty())
	{
		tea_pwarn("CheckSessionKeyTimeout this_ip is empty!");		
		return false;
	}

	//如果上次的IP跟本次没有变化则不过期
	if (ip2sessionkey[this_ip] == session_key)
		return true;
	
	//验证时效性,允许误差
	uint32 now = (uint32)time(NULL);
	//误差操作过了且不是断线重连上来的
	if(key_time < now - gaptime || key_time > now + gaptime)
	{
		tea_pwarn("CheckSessionKeyTimeout key_time is faild");		
		return false;
	}

	return true;
}

//客户端连接来了
int LogindApp::on_create_conn_get_session(tcp_connection *,server_packet *pkt)
{
	uint32 fd;
	char *session_key;
	char *account;
	char *version;
	*pkt >> fd >> session_key >> account >> version;

	//如果登录服未准备好
	if(g_app->m_status != APP_STATUS_READY_OK)
	{
		LogindApp::g_app->CloseSession(fd);
		tea_pwarn("LogindApp::on_create_conn_get_session g_app->m_status != APP_STATUS_READY_OK");
		return 0;
	}

	//在这里就已经验证session的有效性并解析出一个map交给GetSession
	map<string,string> querys;
	if(!ParseSessionKey(session_key,querys))
	{
		//这个还是蛮正常的，因为会出现一台机器多个平台的情况，平台密钥不会相同，所以tea_pdebug就好
		tea_pdebug("LogindApp::on_create_conn_get_session ParseSessionKey failed");
		LogindApp::g_app->CloseSession(fd);
		return 0;
	}

	uint32 gaptime = 900;		//页游默认为15分钟
	if (querys["mobile"] == "y")		
		gaptime = 86400*365;	//手游默认为24小时
	//验证一下session_key的时效性
	if(!CheckSessionKeyTimeout(fd,session_key,atoi(querys["time"].c_str()), gaptime))
	{
		tea_pwarn("LogindApp::on_create_conn_get_session CheckSessionKeyTimeout failed");
		LogindApp::g_app->CloseSession(fd);
		return 0;
	}

	//如果连接不存在,则根据解开的session结果进行创建对应的session实例
	LogindContext* login_context = NULL;
	MobileContext* mobile_context = NULL;
	CommonContext* session = CommonContext::FindContext(fd);
	if (session && session->GetContextType() == CONTEXT_TYPE_YEYOU)
	{
		login_context = dynamic_cast<LogindContext*>(session);
		if(login_context && login_context->GetStatus() == STATUS_NEVER)
		{
			tea_pwarn("LogindApp::on_create_conn_get_session create_sesstion duplicate!");
			login_context->Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE13,"");
			return 0;
		}
	}
	else if (session && session->GetContextType() == CONTEXT_TYPE_PHONE)
	{
		mobile_context = dynamic_cast<MobileContext*>(session);
		if (mobile_context)
		{
			tea_pwarn("LogindApp::on_create_conn_get_session create_mobile_context duplicate!");
			mobile_context->Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE55,"",true);
			//TODO: 这时我加的 这里重复的不需要return么 还是忘了写了
			return 0;
		}
	}

	//创建会话
	if (querys["mobile"] == "y")
	{
		auto *mobile_session = new MobileContext(fd,LogindApp::g_app);
		mobile_session->m_remote_ip = gHostInfoMap[fd].first;
		mobile_session->m_remote_port = gHostInfoMap[fd].second;
		LogindApp::g_app->InsertContext(mobile_session);
		mobile_session->Get_Session(querys);
		return 0;
	}
	
	//为顶号服务的
	if(!login_context)
	{
		login_context = new LogindContext(fd,LogindApp::g_app);//LogindContext::CreateContext(fd);
		if(querys["remote_ip"].empty())
			login_context->m_remote_ip = gHostInfoMap[fd].first;
		else
			login_context->m_remote_ip = querys["remote_ip"];
		login_context->m_remote_port = gHostInfoMap[fd].second;
		LogindApp::g_app->InsertContext(login_context);
	}	

	//要么成功,要么失败
	if(login_context->Get_Session(querys))
	{
		ip2sessionkey[login_context->m_remote_ip] = session_key;
	}
	else
	{
		login_context->Close(0,"");
	}
	return 0;
}

int LogindApp::on_scened_del_map(tcp_connection *, server_packet *pkt)
{
	uint32 fd, instanceid;
	*pkt >> fd >> instanceid;
	MapMgr->TryClose(instanceid);
	return 0;
}

//连接中心服成功,并取得所有在线玩家以后
int LogindApp::on_get_online_player(tcp_connection *, server_packet *pkt)
{
	//先帮中心服把索引加一下
	g_app->CallCentdCreateIndex();
	//检查一些变量是否存在，不存在则创建
	g_app->CheckObject();
	//崩溃重启，或者数据修复
	if(LogindApp::g_app->m_server_conn_list->GetServerReadyCount(SERVER_TYPE_LOGIND))
	{
		g_app->CollapseReset(pkt);
	}
	else
	{
		tea_pinfo("logind init!");
		tea_pinfo("load global value begin");
		vector<GuidObject*> vec;
		g_app->m_cache->LoadGlobalValue();
		g_app->m_db_access_mgr->Load_GlobalValue();
		tea_pinfo("load global value end");

		tea_pinfo("load league value begin");
		g_app->m_cache->LoadFractionValue();
		tea_pinfo("load league value end");

		tea_pinfo("load faction data begin");
		g_app->m_cache->LoadFractionData();
		tea_pinfo("load faction data end");

		//载入脚本
		scripts_init(g_Config.script_folder.c_str());
		LoadTemplate();

		g_app->OnDataReady();
	}
	
	return 0;
}

//gm命令
int LogindApp::on_gm_commands(tcp_connection *,server_packet *pkt)
{
	string lguid;
	string str;
	*pkt >> lguid;
	*pkt >> str;

	tea_pinfo("player %s send gm command : %s", lguid.c_str(), str.c_str());

	logind_player *player = LogindContext::FindPlayer(lguid);
	if(!player)
		return 0;

	Tokens tokens;
	StrSplit(tokens, str, ' ');
	if(tokens.empty() || tokens[0].empty())
		return 0;


	int gm_comand_key = GetGmCommandKey(tokens[0]);

	if(gm_comand_key == GM_COMMAND_PINGBICI)//"@屏蔽词")
	{
		LogindApp::g_app->LoadFuckPingbi(LogindApp::g_app->GetConfig().storage_path);
	}
	else if(gm_comand_key == GM_COMMAND_ZAIXIAN)//"@在线"
	{
		if (player->GetGMLevel()<GM_LEVEL_1)
		{		
			return 0;
		}
		stringstream ss;
		ss << "player：" << ObjMgr.indexer().get(REGEX_PLAYER)->size() << ",session:" << LogindContext::SessionMaps.size();
		//TODO: Call_chat_whisper(player->GetSession()->m_delegate_sendpkt,player->GetGuid().c_str(),player->GetFaction(),player->GetName().c_str(),ss.str().c_str());
	}
	else if(gm_comand_key == GM_COMMAND_SUICIDE)//@自爆
	{
		if (player->GetGMLevel()< GM_LEVEL_1)
		{
			return 0;
		}
		player->SetZiBao();
		//跟客户端说自爆了
		Call_operation_failed(player->GetSession()->m_delegate_sendpkt,OPRATE_TYPE_LOGIN,OPRATE_RESULT_PLAYER_ZIBAO, player->GetAccount().c_str());
		//告诉应用服，删号了
		WorldPacket pkt(INTERNAL_OPT_DEL_CHAR);
		pkt << player->GetGuid();
		LogindApp::g_app->SendToAppd(pkt);
		//删除角色
		g_DAL.DelChars(player->GetGuid().c_str());
		//把这个角色删除
		g_Cache.AddAccountToChar(player->GetAccount(),"");
		g_Cache.SaveAccountCharInfo(player->GetAccount(), "");
		//关闭连接
		player->GetSession()->Close(PLAYER_CLOSE_OPERTE_LOGDIN_ONE57,"");
	}
	else if(gm_comand_key == GM_COMMAND_CHONGZHI)//@充值
	{
		if (player->GetGMLevel()< GM_LEVEL_3)
		{
			return 0;
		}
		uint32 value = 0;
		if(tokens.size() >= 2)
			value = atoi(tokens[1].c_str());
		uint32 type = 1;
		if(tokens.size() >= 3)
			type = atoi(tokens[2].c_str());
		if(value > 0 && type > 0 && type < 5)
			g_DAL.SetGmRechargeNum(player->GetAccount().c_str(),value,type);
	}
	else if(gm_comand_key == GM_COMMAND_KAIFUSHIJIAN)//@开服时间
	{
		if (player->GetGMLevel()< GM_LEVEL_3)
		{
			return 0;
		}
		uint32 open_time = 0;
		if(tokens.size() == 2)
		{
			open_time = atoi(tokens[1].c_str());
			g_app->SetKaiFuShiJian(open_time);
			g_DAL.UpdateKaiFuShiJian(open_time);
		}
		
	}
	else if(gm_comand_key == GM_COMMAND_TEST_SHUJU)//@测试数据
	{
		if (player->GetGMLevel()< GM_LEVEL_3)
		{
			return 0;
		}
	}
	else if(gm_comand_key == GM_COMMAND_UPDATE_SERVER)//@更新服务器
	{
		if (player->GetGMLevel()< GM_LEVEL_3)
		{
			return 0;
		}
		if(tokens.size() == 2)
		{
			uint32 server_type = atoi(tokens[1].c_str());
			g_app->m_db_access_mgr->ServerUpdate(server_type);
		}
	}
	else if(gm_comand_key == GM_COMMAND_RESESSION_OPTS)//@协议注册
	{
		if (player->GetGMLevel()< GM_LEVEL_3)
		{
			return 0;
		}
		g_app->RegSessionOpts(player->GetSession()->GetFD());
	}
	else if(gm_comand_key == GM_COMMAND_DEL_MAP_INFO)//@清空地图信息
	{
		if (player->GetGMLevel()< GM_LEVEL_3)
		{
			return 0;
		}
		vector<GuidObject*> vec;
		ObjMgr.GetDataSetAllObject(MAP_INSTANCE_PLAYER_INFO_OWNER_STRING, vec);
		vector<string> vec_guid;
		for (auto it:vec)
		{
			vec_guid.push_back(it->guid());
		}
		for (auto it:vec_guid)
		{
			ObjMgr.CallRemoveObject(it);
		}
	}
	else if(gm_comand_key == GM_COMMAND_BACKUP_DATA)//@备份数据
	{
		if(tokens.size() == 2)
		{
			string backupfilename = tokens[1].c_str();
			g_app->SaveAllData();
			g_Cache.Backup(backupfilename);
		}
	}
	else if(gm_comand_key == GM_COMMAND_RETURN_DATA)	//@回档数据
	{
		if(tokens.size() == 2)
		{
			string data_time = tokens[1].c_str();
			g_Cache.HuidangPlayerInfos(lguid,data_time);
		}
	}
	else if(gm_comand_key == GM_COMMAND_DB)	//@数据库
	{
		bool save_all = false;
		if(tokens.size() == 2 && atoi(tokens[1].c_str()) == 1)
			save_all = true;
		g_Cache.SaveChangePlayerToDB(save_all);
	}
	else if(gm_comand_key == GM_COMMAND_SUBLINE)//"@分线"
	{
		if (player->GetGMLevel()<GM_LEVEL_1)
		{		
			return 0;
		}

		uint32 map_id = player->GetMapId();
		if(tokens.size() >= 2)
			map_id = atoi(tokens[1].c_str());
		string general_id = "";
		if(tokens.size() >= 3)
			general_id = tokens[2];

		stringstream ss;
		ss << "map " << map_id << std::endl;

		for(uint32 lines = 0; lines < 100; lines++)
		{
			int32 index = MapMgr->FindInstance(map_id, general_id, lines);
			if (index < 0)
			{
				continue;
			}

			ss << "line " << lines << " count " << MapMgr->PlayerCount(index) << std::endl; 
		}

		//TODO: Call_chat_whisper(player->GetSession()->m_delegate_sendpkt,player->guid().c_str(), 0, player->GetName().c_str(), ss.str().c_str());	
		
		return 0;
	}
	else
	{
		//DoGMScripts(player, str);
	}
	return 0;
}

int LogindApp::on_offline_oper(tcp_connection *, server_packet *pkt)
{
	string guid, callback_name;
	uint32 callback_id;
	*pkt >> guid >> callback_id >> callback_name;

	//校验一下是否本服的对象
	string guid_server_name = MongoDB::GetServerName(guid);
	if(!g_app->IsMyServer(guid_server_name))
	{
		tea_pinfo("LogindApp::on_offline_oper not local data, %s %s", guid.c_str(), callback_name.c_str());
		return 0;
	}


	vector<GuidObject*> vec;
	auto fun = [guid, callback_name, callback_id](bool need_free, vector<GuidObject*> &vec){
		//没数据？那就算了吧
		if(vec.empty())
		{
			tea_pwarn("LogindApp::on_offline_oper player info not found, %s %s", guid.c_str(), callback_name.c_str());
			return;
		}
		//打包数据
		static ByteArray bytes;
		bytes.clear();
		for (auto it:vec)
		{
			it->WriteCreateBlock(bytes);
			if(need_free)
				safe_delete(it);
		}
		WorldPacket wpkt(INTERNAL_OPT_OFFLINE_OPER);
		bytes.position(0);
		ObjMgr.Compress(bytes);
		wpkt << guid << callback_id << callback_name << bytes;
		LogindApp::g_app->SendToAppd(wpkt);
	};

	bool need_free = true;
	//两种情况
	//玩家不在线，去硬盘或者数据库取
	auto ds = ObjMgr.FindDataSetByOwnerGuid(guid);
	if(!ds || ds->GetAllData().empty())
	{
		logind_player *player = nullptr;
		if(!g_Cache.LoadPlayer(guid, &player, vec) )
		{
			static map<uint32, vector<GuidObject*>> obj_vec_map;
			//ASSERT(obj_vec_map.find(callback_id) == obj_vec_map.end());
			obj_vec_map[callback_id].clear();
			g_DAL.LoadPlayer("", guid, obj_vec_map[callback_id],[fun, callback_name, guid](bool load_succeed, vector<GuidObject*> &vec){
				if(!load_succeed)
				{
					tea_pwarn("LogindApp::on_offline_oper player info db err, %s %s", guid.c_str(), callback_name.c_str());
					return;
				}
				if(vec.empty())
				{
					tea_pwarn("LogindApp::on_offline_oper player info db not found, %s %s", guid.c_str(), callback_name.c_str());
					return;
				}
				logind_player *player = dynamic_cast<logind_player*>(vec[0]);
				ASSERT(player);
				//先存到本地硬盘来吧
				g_Cache.SavePlayerData(guid, vec, player->GetLevel());
				fun(true, vec);
			});
			return 0;
		}
	}
	else//玩家正好登录上来了
	{
		ObjMgr.GetDataSetAllObject(guid, vec);
		need_free = false;
	}

	fun(need_free, vec);
	return 0;
}

void ApplyBlock(ByteArray &bytes)
{
	while(bytes.bytesAvailable() > 0){
		int flags = bytes.readByte();		//先读出标志

		string guid;
		
		guid = bytes.readUTF();
		auto cur_obj =ObjMgr.Get(guid);	

		core_obj::BinLogStru binlog;
		int len = bytes.readUnsignedShort();				
		for(int i = 0; i < len; i++){
			binlog.ReadFrom(bytes);

			switch (binlog.opt)
			{
			case core_obj::OPT_SET:
				switch (binlog.typ)
				{
				case core_obj::TYPE_DOUBLE:
					cur_obj->SetDouble(binlog.index,binlog.double_val);
					break;
				case core_obj::TYPE_UINT32:
				case core_obj::TYPE_INT32:
					cur_obj->SetInt32(binlog.index,binlog.value);
					break;
				case core_obj::TYPE_FLOAT:
					cur_obj->SetFloat(binlog.index, binlog.float_val);
					break;
				case core_obj::TYPE_INT16:
					cur_obj->SetInt16(binlog.index,binlog.offset,binlog.int16_val);
					break;
				case core_obj::TYPE_UINT16:
					cur_obj->SetUInt16(binlog.index,binlog.offset,binlog.uint16_val);
					break;
				case core_obj::TYPE_UINT8:
					cur_obj->SetByte(binlog.index,binlog.offset,binlog.byte_val);
					break;
				case core_obj::TYPE_BIT:
					cur_obj->SetBit(binlog.index,binlog.value);
					break;
				case core_obj::TYPE_STRING:
					cur_obj->SetStr(binlog.index,binlog.str_val);
					break;
				}
				break;
			case core_obj::OPT_UNSET:
				switch (binlog.typ)
				{
				case core_obj::TYPE_BIT:
					cur_obj->UnSetBit(binlog.index,binlog.value);
					break;
				default:
					ASSERT(0);
				}
				break;
			case core_obj::OPT_ADD:
				switch (binlog.typ)
				{
				case core_obj::TYPE_DOUBLE:
					cur_obj->AddDouble(binlog.index,binlog.double_val);
					break;
				case core_obj::TYPE_UINT32:
					cur_obj->AddUInt32(binlog.index,binlog.value);
					break;
				case core_obj::TYPE_INT32:
					cur_obj->AddInt32(binlog.index,binlog.int32_val);
					break;
				case core_obj::TYPE_FLOAT:
					cur_obj->AddFloat(binlog.index,binlog.float_val);
					break;
				case core_obj::TYPE_INT16:
					cur_obj->AddInt16(binlog.index,binlog.offset,binlog.int16_val);
					break;
				case core_obj::TYPE_UINT16:
					cur_obj->AddUInt16(binlog.index,binlog.offset,binlog.uint16_val);
					break;
				case  core_obj::TYPE_UINT8:
					cur_obj->AddByte(binlog.index,binlog.offset,binlog.byte_val);
					break;
				default:
					ASSERT(0);
				}
				break;
			case core_obj::OPT_SUB:
				switch (binlog.typ)
				{
				case core_obj::TYPE_DOUBLE:
					cur_obj->SubDouble(binlog.index,binlog.double_val);
					break;
				case core_obj::TYPE_UINT32:
					cur_obj->SubUInt32(binlog.index,binlog.value);
					break;
				case core_obj::TYPE_INT32:
					cur_obj->SubInt32(binlog.index,binlog.int32_val);
					break;
				case core_obj::TYPE_FLOAT:
					cur_obj->SubFloat(binlog.index,binlog.float_val);
					break;
				case core_obj::TYPE_INT16:
					cur_obj->SubInt16(binlog.index,binlog.offset,binlog.int16_val);
					break;
				case core_obj::TYPE_UINT16:
					cur_obj->SubUInt16(binlog.index,binlog.offset,binlog.uint16_val);
					break;
				case  core_obj::TYPE_UINT8:
					cur_obj->SubByte(binlog.index,binlog.offset,binlog.byte_val);
					break;
				}
				break;
			default:
				ASSERT(0);
			}
		}

	}    
}

int LogindApp::on_offline_oper_result(tcp_connection *, server_packet *pkt)
{
	string guid;
	*pkt >> guid;
	static ByteArray bytes;
	bytes.clear();
	bytes.writeBytes(pkt->content + pkt->rpos,pkt->wpos - pkt->rpos);
	bytes.position(0);
	//说不定已经上线了
	if(ObjMgr.FindPlayer(guid))
	{
		ApplyBlock(bytes);
		return 0;
	}
	//还没上线的，那就读硬盘写硬盘吧
	vector<GuidObject*> vec;
	logind_player *player = nullptr;
	
	static core_obj::GuidObjectTable table_(core_obj::SYNC_SLAVE);
	if(g_Cache.LoadPlayer(guid, &player, vec))
	{
		//放进容器
		for (auto it:vec)
		{
			table_.AttachObject(it);
		}
		//应用binlog
		table_.ApplyBlock(bytes);
		//保存
		g_Cache.SavePlayerData(guid, vec, player->GetLevel());
		//从容器移除,这里就有释放功能了
		for (auto it:vec)
		{
			table_.ReleaseObject(it->guid());
		}
	}
	else
	{
		//如果实在读不到，那就算了
		tea_pwarn("LogindApp::on_offline_oper_result load player fail %s", guid.c_str());
	}
	return 0;
}

int LogindApp::on_save_all_data(tcp_connection *, server_packet *pkt)
{
	//打印一下
	tea_pinfo("LogindApp::on_save_all_data centd");
	//保存所有信息
	//g_app->SaveAllData();
	return 0;
}

//地图给地图发包
int LogindApp::on_map_router_pkt(tcp_connection* coon, server_packet* pkt)
{
	MapMgr->HandleMapPkt(pkt);
	return 0;
}


//托管登录
int LogindApp::on_logind_hosting(tcp_connection* coon, server_packet* pkt)
{
	string player_guid,hosting_guid;
	*pkt >> player_guid >> hosting_guid;
	logind_player *player = LogindContext::FindPlayer(player_guid);
	string hosting_account = g_DAL.FindAccountByGuid(hosting_guid);
	//账号信息
	account_table *accountInfo = g_Cache.GetAccount(hosting_account);
	//bool load_result = g_DAL.LoadAccount(hosting_account, &accountInfo);
	if (player && player->GetSession() && accountInfo != NULL)
	{
		//bool is_fcm = accountInfo->is_FCM != 1;
		//托管就不防沉迷了
		bool is_fcm = false;
		Call_operation_failed(player->GetSession()->m_delegate_sendpkt,OPERTE_TYPE_HOSTING,HOSTING_OPERTE_LOGIN,
			LogindApp::g_app->MakeSessionKey(hosting_account,is_fcm,"","", true).c_str());
	}	
	return 0;
}

int LogindApp::on_kuafu_enter(tcp_connection* , server_packet* pkt) {
	uint32 kuafu_type, pos;
	string player_guid, war_id, battle_server, watcher_guid;
	
	*pkt >> player_guid >> war_id >> pos >> battle_server >> watcher_guid >> kuafu_type;

	logind_player *m_player = LogindContext::FindPlayer(player_guid);
	if (!m_player)
	{
		tea_pdebug("LogindContext::on_kuafu_enter m_player is null!");
		return 1;
	}

	if (!m_player->GetSession())
	{
		tea_pdebug("func on_kuafu_enter player session is null !!! guid = %s", player_guid.c_str());
		return 1;
	}

	if (kuafu_type == KUAFU_TYPE_FENGLIUZHEN || kuafu_type == KUAFU_TYPE_XIANFU || kuafu_type == KUAFU_TYPE_GROUP_INSTANCE)
	{
		//风流镇

		// war_id = "1486557614#23#p1200.2_1003#p1197.2_1003"
		// 直接
		//battle_server = "2_65534;192.168.88.5;443"
		//切割字符串
		

		Tokens token1;
		StrSplit(token1, war_id, '#');
		uint32 room_id = atol(token1[0].c_str());			//房间id

		Tokens token;
		StrSplit(token, battle_server, ';');
		if (token.size() != 3)
		{
			tea_pdebug("func on_kuafu_enter data is %s, token.size() != 3", battle_server.c_str());
			return 1;
		}

		string battle_server_name = token[0];
		string host = token[1];
		string port = token[2];

		//到这该校验的都已校验完了
		if(!m_player->GetFlags(PLAYER_APPD_INT_FIELD_FLAGS_FROM_KUAFU))
			m_player->SetFlags(PLAYER_APPD_INT_FIELD_FLAGS_FROM_KUAFU);			//把跨服标志置上

		//根据匹配信息生成登录串
		bool is_fcm = m_player->GetFCMLoginTime() != (-1);
		char other_temp[1024] = {0};
		//游戏服server_name|跨服类型|场次id|分配的战斗服server_name|int保留字|str保留字
		sprintf(other_temp, "&backsvrname=%s&kuafutype=%d&warid=%u&battlesvrname=%s&kuafureserve=%d&reservestr=%s", 
			g_Config.server_name.c_str(), kuafu_type, room_id, battle_server_name.c_str(), pos, war_id.c_str());	
		string session_str = LogindApp::g_app->MakeSessionKey(m_player->GetAccount(),is_fcm, other_temp, WANNENG_LOGIN_KEY);

		//通知客户端断开游戏服连到pk服
		m_player->GetSession()->Call_World_War_CS_Player_Info(host, atoi(port.c_str()), session_str);
	}

	return 0;
}

int LogindApp::on_rename_check(tcp_connection* coon, server_packet* pkt) {
	string player_guid;
	string name;

	*pkt >> player_guid >> name;

	bool ret = false;
	string realName;
	logind_player *m_player = LogindContext::FindPlayer(player_guid);
	if (m_player && m_player->GetSession()) {
		account_table *accountInfo = g_Cache.GetAccount(m_player->GetAccount());
		string prefix = string(accountInfo->pid) + "," + string(accountInfo->sid) + ",";
		ret = m_player->GetSession()->Check_Name(name, realName, prefix);
	}
	uint32 available = (ret ? 1 : 0);
	// 发送给应用服
	WorldPacket wpkt(INTERNAL_OPT_RENAME_CHECK_RESULT);
	wpkt << player_guid << available << realName;
	LogindApp::g_app->SendToAppd(wpkt);

	return 0;
}

int LogindApp::on_update_charname(tcp_connection* coon, server_packet* pkt) {
	string player_guid;
	string name;

	*pkt >> player_guid >> name;
	logind_player *m_player = LogindContext::FindPlayer(player_guid);
	if (m_player) {
		m_player->GetSession()->Char_Name_Update(name);
	}
	return 0;
}

//主动让跨服回游戏服
int LogindApp::on_kuafu_back_to_game(tcp_connection* coon, server_packet* pkt) {
	// 传个空包就行
	packet pack;

	string player_guid;
	*pkt >> player_guid;
	logind_player *m_player = LogindContext::FindPlayer(player_guid);
	if (m_player) {
		m_player->GetSession()->Handle_GoBackTo_GameServer(pack);
	}
	return 0;
}

//帮派邀请结果
int LogindApp::on_faction_invite_result(tcp_connection* coon, server_packet* pkt) {

	string player_guid;
	string faction_id;
	uint32 mapid;
	uint32 x;
	uint32 y;
	string general_id;

	*pkt >> player_guid >> faction_id >> mapid >> x >> y >> general_id;
	logind_player *m_player = LogindContext::FindPlayer(player_guid);
	if (m_player && m_player->GetSession()) {
		m_player->GetSession()->onSetInvitedTeleportInfo(faction_id, mapid, x, y, general_id);
	}
	return 0;
}