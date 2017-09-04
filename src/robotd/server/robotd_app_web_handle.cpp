#include "robotd_app.h"
#include "robotd_context.h"
#include "robotd_script.h"
#include "http/server/server.hpp"
#include "http/server/request_handler.hpp"

void RobotdApp::RegHttpHandler()
{
	auto& request_handler = web_svr_->get_request_handler();

	using http::server::request;
	using http::server::reply;

	// /login?pid={0}&sid={1}&uid={2}&remote_ip={3}&netgd_host={4}&netgd_port={5}&login_key={6}
	request_handler.add_hanlde("/login",[&](const request& req, reply& rep){
		map<string, string> m;
		if(http::server::parse_query(req.query_string, m))
		{
	#define HTTP_ROBOT_LOGIN_RESULT_ERROR	"1"	//添加机器人账号失败
	#define HTTP_ROBOT_LOGIN_RESULT_OK		"2"	//添加机器人账号成功
			string pid = m["pid"];
			string sid = m["sid"];
			string uid = m["uid"];
			string remote_ip = m["remote_ip"];
			string netgd_host = m["netgd_host"];
			string netgd_port = m["netgd_port"];
			uint32 port = atol(netgd_port.c_str());
			string params = m["params"];

			tea_pinfo("login web request : pid:%s , sid:%s, uid:%s, remote_ip:%s, netgd_host:%s, netgd_port:%s, params:%s",
				pid.c_str(), sid.c_str(), uid.c_str(), remote_ip.c_str(), netgd_host.c_str(), netgd_port.c_str(), params.c_str());
			if(uid.empty() || sid.empty() || pid.empty() || netgd_host.empty() || port == 0)
			{
				rep.status = reply::ok;
				rep.content = HTTP_ROBOT_LOGIN_RESULT_ERROR;
				return;
			}

			string account = pid + "_" + sid + "_" + uid;
			auto context = GetContext(account);
			if(context)
			{
				if(context->GetConnHost() != netgd_host || context->GetConnPort() != port || context->GetRemoteIP() != remote_ip)
				{
					m_all_context.erase(account);
					delete context;
				}
				else
				{
					rep.status = reply::ok;
					rep.content = HTTP_ROBOT_LOGIN_RESULT_OK;
					return;
				}
			}

			context = new RobotdContext(pid, sid, uid, remote_ip, g_Config.login_key,netgd_host , port, params);
			m_all_context[account] = context;

			//保存到硬盘文件
			stringstream ss;
			ss	<< time(nullptr) << " "
				<< "login" << " "
				<< pid << " "
				<< sid << " "
				<< uid << " "
				<< netgd_host << " "
				<< port << " "
				<< g_Config.login_key << " "
				<< ToHdd(remote_ip) << " "
				<< ToHdd(params) << " ";
			m_storage->Goto(g_Config.hdd_path);
			m_storage->AddContent(m_config.context_file_name, ss.str() + "\r\n");

			rep.status = reply::ok;
			rep.content = HTTP_ROBOT_LOGIN_RESULT_OK;
		}
	});

	// /logout?pid={0}&sid={1}&uid={2}
	request_handler.add_hanlde("/logout",[&](const request& req, reply& rep){
		map<string, string> m;
		if(http::server::parse_query(req.query_string, m))
		{
#define HTTP_ROBOT_LOGOUT_RESULT_PARAM_ERROR	"1"	//参数有误
#define HTTP_ROBOT_LOGOUT_RESULT_OK				"2"	//机器人退出登录成功
#define HTTP_ROBOT_LOGOUT_RESULT_NOT_FOUND		"3"	//账号未找到
			string pid = m["pid"];
			string sid = m["sid"];
			string uid = m["uid"];
			tea_pinfo("logout web request : pid:%s , sid:%s, uid:%s",pid.c_str(), sid.c_str(), uid.c_str());
			if(uid.empty() || sid.empty() || pid.empty())
			{
				rep.status = reply::ok;
				rep.content = HTTP_ROBOT_LOGOUT_RESULT_PARAM_ERROR;
				return;
			}

			//保存到硬盘文件
			stringstream ss;
			ss	<< time(nullptr) << " "
				<< "logout" << " "
				<< pid << " "
				<< sid << " "
				<< uid << " ";
			m_storage->Goto(g_Config.hdd_path);
			m_storage->AddContent(m_config.context_file_name, ss.str() + "\r\n");

			string account = pid + "_" + sid + "_" + uid;
			RobotdContext *context = GetContext(account);

			rep.status = reply::ok;
			if(context)
			{
				m_all_context.erase(account);
				delete context;
				rep.content = HTTP_ROBOT_LOGOUT_RESULT_OK;
			}
			else
			{
				tea_pdebug("RobotdApp::RegHttpHandler %s logout, but not found.", account.c_str());
				rep.content = HTTP_ROBOT_LOGOUT_RESULT_NOT_FOUND;
			}
		}
	});

	// /list
	request_handler.add_hanlde("/list",[&](const request& req, reply& rep){
		stringstream ss1;
		stringstream ss2;
		int online = 0, offline = 0;
		for (auto it:m_all_context)
		{
			auto context = it.second;
			if(context->IsLoginOk())
			{
				online++;
				ss2 << context->GetAccount() << "|"
					<< context->guid() << "|"
					<< context->GetName() << "|"
					<< context->GetLevel() << "|"
					<< uint32(context->GetMoney(MONEY_TYPE_GOLD_INGOT)) << "|"
					<< context->GetOnlineTime() << "|"
					<< context->GetMapId() << "|"
					<< uint32(context->GetMoney(MONEY_TYPE_BIND_GOLD)) << "|"
					<< uint32(context->GetMoney(MONEY_TYPE_SILVER)) << "|"
					<< uint32(context->GetForce()) << "|"
					<< uint32(context->GetExp()) << "|"
					<< "<br>";
			}
			else
			{
				offline++;
				ss2 << context->GetAccount() << "<br>";
			}
		}
		ss1 << "<html> <meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\"/>" << "online:" << online << "    offline:" << offline << "<br>" << ss2.str() << "</html>";
		rep.status = reply::ok;
		rep.content = ss1.str();
	});

	// /info_detail
	request_handler.add_hanlde("/info_detail",[&](const request& req, reply& rep){
		stringstream ss, ss1;
		int online = 0, offline = 0;		
		for (auto it:m_all_context)
		{
			auto context = it.second;
			if(context->IsLoginOk())
			{
				online++;			
			}
			else
			{
				offline++;
				ss << context->GetAccount() << '\n';
			}
		}
		ss1  << "online:" << online << "    offline:" << offline << '\n' << ss.str();
		rep.status = reply::ok;
		rep.content = ss1.str();
	});

	// /info
	request_handler.add_hanlde("/info",[&](const request& req, reply& rep){
		stringstream ss1;
		int online = 0, offline = 0;
		for (auto it:m_all_context)
		{
			auto context = it.second;
			if(context->IsLoginOk())
			{
				online++;			
			}
			else
			{
				offline++;
			}
		}
		ss1  << "online:" << online << "    offline:" << offline;
		rep.status = reply::ok;
		rep.content = ss1.str();
	});

	// /detail?pid={0}&sid={1}&uid={2}
	request_handler.add_hanlde("/detail",[&](const request& req, reply& rep){
#define HTTP_ROBOT_DETAIL_RESULT_OFFLINE "1"
#define HTTP_ROBOT_DETAIL_RESULT_NOT_FOUND "2"
#define HTTP_ROBOT_DETAIL_RESULT_ERROR "3"
#define HTTP_ROBOT_DETAIL_RESULT_ONLINE "4"
		map<string, string> m;
		if(http::server::parse_query(req.query_string, m))
		{
			string pid = m["pid"];
			string sid = m["sid"];
			string uid = m["uid"];
			if(uid.empty() || sid.empty() || pid.empty())
			{
				rep.status = reply::ok;
				rep.content = HTTP_ROBOT_DETAIL_RESULT_ERROR;
				return;
			}

			string account = pid + "_" + sid + "_" + uid;
			auto context = GetContext(account);
			rep.status = reply::ok;
			if(context)
			{
				if(context->IsLoginOk())
				{
					stringstream ss;
					ss	<< HTTP_ROBOT_DETAIL_RESULT_ONLINE << "|"
						<< context->GetLevel() << "|"
						<< context->GetMoney(MONEY_TYPE_GOLD_INGOT) << "|"
						<< context->GetOnlineTime() << "|"
						<< context->GetName();
					rep.content = ss.str();
				}
				else
				{
					rep.content = HTTP_ROBOT_DETAIL_RESULT_OFFLINE;
				}
			}
			else
			{
				rep.content = HTTP_ROBOT_DETAIL_RESULT_NOT_FOUND;
			}
		}
	});

	// /detailStack?pid={0}&sid={1}&uid={2}
	request_handler.add_hanlde("/detailStrack",[&](const request& req, reply& rep){
#define HTTP_ROBOT_DETAILSTACK_RESULT_OFFLINE "1"
#define HTTP_ROBOT_DETAILSTACK_RESULT_NOT_FOUND "2"
#define HTTP_ROBOT_DETAILSTACK_RESULT_ERROR "3"
#define HTTP_ROBOT_DETAILSTACK_RESULT_ONLINE "4"
		map<string, string> m;
		if(http::server::parse_query(req.query_string, m))
		{
			string pid = m["pid"];
			string sid = m["sid"];
			string uid = m["uid"];
			if(uid.empty() || sid.empty() || pid.empty())
			{
				rep.status = reply::ok;
				rep.content = HTTP_ROBOT_DETAILSTACK_RESULT_ERROR;
				return;
			}

			string account = pid + "_" + sid + "_" + uid;
			auto context = GetContext(account);
			rep.status = reply::ok;
			if(context)
			{
				if(context->IsLoginOk())
				{
					string info;
					stringstream ss;

					DoGetStack(account,info);
					ss	<< HTTP_ROBOT_DETAILSTACK_RESULT_ONLINE << "|"
						<< info.c_str();
					rep.content = ss.str();
				}
				else
				{
					rep.content = HTTP_ROBOT_DETAILSTACK_RESULT_OFFLINE;
				}
			}
			else
			{
				rep.content = HTTP_ROBOT_DETAILSTACK_RESULT_NOT_FOUND;
			}
		}
	});
}
