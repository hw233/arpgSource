#include "robotd_app.h"
#include "robotd_context.h"
#include "robotd_script.h"
#include "robotd_object_manager.h"
#include <shared/storage_strus.h>
#include <shared/storage_loader.h>
#include <shared/map_template.h>
#include <protocol/external.h>
#include <shared/log_handler.h>
#include "http/server/server.hpp"

bool RobotConfig::_Load(config_loader *loader)
{
	if(DbSvrCoreConfig::_Load(loader))
	{
		//平台加密密钥
		char* _value = config_loader_get_char(loader,"login_key","");
		login_key = _value;
		free(_value);

		_value = config_loader_get_char(loader,"hdd_path","");
		hdd_path = _value;
		free(_value);

		if(hdd_path.empty())
		{
			hdd_path = log_folder + "/../data/robotd/";
		}

		_value = config_loader_get_char(loader,"login_queue","Y");
		login_queue = _value[0] == 'Y' || _value[0] == 'y';
		free(_value);
		
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// for RobotdApp
RobotdApp *RobotdApp::g_app;

RobotdApp::RobotdApp(SvrCoreParams &params, RobotConfig &config):m_config(config),web_svr_(0)
{
	m_config.web_port = params.web_port;
	m_config.context_file_name = params.context_file_name.empty() ? "context" + m_config.web_port + ".txt" : params.context_file_name;
}

RobotdApp::~RobotdApp()
{

}

void RobotdApp::OnSignal(int s)
{
	g_app->m_config.ReLoad();
	safe_delete(g_app->logger_);
	g_app->logger_ = new core_socket::Logger;
	bool b = g_app->logger_->Init(g_app->m_config.log_folder.c_str(),g_app->m_config.log_name.c_str(),g_app->m_config.log_level);
	ASSERT(b);

	signal(s, OnSignal);
}

bool RobotdApp::Open()
{
	svrcore_init();

	Init(m_config.log_folder,m_config.log_name,m_config.log_level);
	if(!Application::Open())
		return false;

	//注册一下重新载入配置文件的信号
#define SIGHUP 1	
	signal(SIGHUP, OnSignal);

	//载入脚本
	if(scripts_init(m_config.script_folder.c_str()) != 0)
		return false;
	tea_pinfo("\n\n开始载入地图模板信息...");
	if(!MapTemplate::Load(m_config.map_folder))
		return false;
	
	tea_pinfo("init conn mgr");
	connMgr_ = new core_socket::ConnectionMgr();

	m_storage = new core_obj::Storage(nullptr);
	//////////////////////////////////////////////////////////////////////////
	//web管理界面
	web_svr_ = new http::server::server("0.0.0.0", m_config.web_port.empty()?"8090":m_config.web_port);
	RegHttpHandler();

	//读取硬盘中存储的玩家列表
	map<string, Tokens> contexts;
	m_storage->Goto(g_Config.hdd_path);
	m_storage->ReadFile(m_config.context_file_name, [&contexts](const string &txt){
		if(txt.empty())
			return true;
		Tokens tokens;
		StrSplit(tokens, txt, ' ');
		if(tokens.size() < 5)
		{
			tea_perror("RobotdApp::Open load robot err, %s", txt.c_str());
			return true;
		}

		string pid = tokens[2];
		string sid = tokens[3];
		string uid = tokens[4];
		string account = pid + "_" + sid + "_" + uid;
		if(tokens[1] == "login")
			contexts[account] = tokens;
		else if(tokens[1] == "logout")
			contexts.erase(account);
		else
			ASSERT(false);
		return true;
	});

	for (auto it:contexts)
	{
		ASSERT(it.second.size() >= 9);
		string pid = it.second[2];
		string sid = it.second[3];
		string uid = it.second[4];
		string netgd_host = it.second[5];
		ASSERT(!netgd_host.empty());
		uint32 netgd_port = atol(it.second[6].c_str());
		ASSERT(netgd_port);
		string logind_key = g_Config.login_key;
		string remote_ip = it.second[8];
		string params = "";
		if(it.second.size() > 10)//最后一个是"/r"
			params = it.second[9];
		tea_pinfo("load hdd context : %s %s %u %s %s %s", it.first.c_str(), netgd_host.c_str(), netgd_port, logind_key.c_str(), remote_ip.c_str(), params.c_str());
		RobotdContext *context = new RobotdContext(pid, sid, uid, FromHdd(remote_ip), logind_key, netgd_host, netgd_port, FromHdd(params));
		m_all_context[it.first] = context;
	}

	m_opcode_size_timer.SetInterval(60000);
	m_login_timer.SetInterval(10000);
	for (int i = 0 ; i <= 5000; i++)
	{
		OpSize os;
		os.opcode = i;
		os.size = 0;
		all_op_sizie_map.push_back(os);
	}
	return true;
}

void RobotdApp::Close()
{

	svrcore_free();
	safe_delete(m_storage);

	for(auto it:m_all_context)
	{
		safe_delete(it);
	}
	m_all_context.clear();
	
	safe_delete(web_svr_);
	MapTemplate::UnLoad();
	scripts_free();
	core_socket::Application::Close();
	safe_delete(connMgr_);
}

void RobotdApp::Update(uint32 diff)
{
	//base的心跳
	Application::Update(diff);
	//web管理器开始心跳
	web_svr_->poll_one();

#if PLATFORM != PLATFORM_WINDOWS
	//延迟十秒跳以下的心跳，防止占用其他机器人进程端口
	if(!m_login_timer.Passed())
	{
		m_login_timer.Update(diff);
		return;
	}
#endif

	//登录队列心跳
	if(g_Config.login_queue && !m_login_queue.empty())
	{
		string account = m_login_queue.front();
		RobotdContext *context = GetContext(account);
		if(context)
		{
			context->Update(diff);
			if(context->IsLoginOk())
				m_login_queue.pop();
		}
		else
		{
			m_login_queue.pop();
		}	
	}

	//每个机器人的心跳
	for (auto it:m_all_context)	
	{
		//如果有要求一定要排队登录，那么登录成功的才在这里跳
		if(!g_Config.login_queue || it.second->IsLoginOk())
			it.second->Update(diff);
	}

	DoRobotUpdate(diff);

	//打印下标更新统计
	PrintUpdate(diff);
}

RobotdContext *RobotdApp::GetContext(const string &account)
{
	auto it = m_all_context.find(account);
	if(it == m_all_context.end())
		return nullptr;
	return it->second;
}

RobotdContext *RobotdApp::GetContext(const string &pid,const string &sid,const string &uid)
{
	return GetContext(pid+"_"+sid+"_"+uid);
}

//下标更新统计
void RobotdApp::StatUpdateIndex(core_obj::SyncEventRecorder *blobj, UpdateMask& mask,UpdateMask& str_mask)
{
	static char guid_s[10];
	guid_s[0] = (GuidManager::GetPrefix(blobj->guid()));
	guid_s[1] = 0;
	string guid = guid_s;

	auto iter = update_index_stat.find(guid);
	if(iter == update_index_stat.end())
	{
		vector<uint32> a;
		a.resize(65535 * 2, 0);
		update_index_stat.insert(make_pair(guid, a));
		iter = update_index_stat.find(guid);
	}
	vector<uint32> &vec = iter->second;
	for (int i = 0; i < blobj->uint32_values_size(); i++)
	{
		if(mask.GetBit(i))
			vec[i]++;
	}
	for (int i = 0; i < blobj->str_values_size(); i++)
	{
		if(str_mask.GetBit(i))
			vec[i + 65535]++;
	}
}

void RobotdApp::PrintUpdate(uint32 diff)
{

	//打印流量
	m_opcode_size_timer.Update(diff);
	if(!m_opcode_size_timer.Passed())
		return;

	m_opcode_size_timer.Reset2();
	vector<OpSize> temp_vec;
	uint32 all_val = 0;
	for (auto it = all_op_sizie_map.begin(); it != all_op_sizie_map.end(); ++it)
	{
		temp_vec.push_back(*it);
		all_val += it->size;
	}
	std::sort(temp_vec.begin(),temp_vec.end(),[](const OpSize left,const OpSize right)->bool{
		return left.size > right.size;
	});
	uint32 player_count = 0;

	for (auto it:m_all_context)
	{
		if(it.second->IsLoginOk())
			player_count++;
	}
	tea_pinfo("=================================================================");
	tea_pinfo("online %u,  %uM%uK", player_count, all_val/1048576, (all_val % 1048576)/1024);
	for (uint32 i = 0; i < 10; i++)
	{
		tea_pinfo("%s = %uM %uK", GetOpcodeName(temp_vec[i].opcode), temp_vec[i].size / 1048576, (temp_vec[i].size % 1048576) / 1024);
	}
	for(auto it = all_op_sizie_map.begin(); it != all_op_sizie_map.end(); ++it)
	{
		(*it).size = 0;
	}
	tea_pinfo("=================================================================");
	
	tea_pinfo("-----------------------------------------------------------------");
		
	for (auto it : update_index_stat)
	{				
		vector<OpSize> temp_vec;
		for (size_t i = 0; i < it.second.size(); i++)
		{
			OpSize os;
			os.opcode = i;
			os.size = it.second[i];
			temp_vec.push_back(os);
		}
		std::sort(temp_vec.begin(),temp_vec.end(),[](const OpSize left,const OpSize right)->bool{
			return left.size > right.size;
		});
		for (int i = 0; i < 10 && temp_vec[i].size > 10; i++)
		{
			auto &item = temp_vec[i];

			if(item.opcode >= 65535)
				tea_pinfo("guid %s, str index %u, count %u", it.first.c_str(), temp_vec[i].opcode - 65535, temp_vec[i].size);
			else
				tea_pinfo("guid %s, index %u, count %u", it.first.c_str(), temp_vec[i].opcode, temp_vec[i].size);
			
		}
	}
	tea_pinfo("-----------------------------------------------------------------");
}
