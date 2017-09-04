#include "policed_app.h"
#include "http_handler.h"
#include <comm/b64.h>
#include <http/server/server.hpp>
#include <http/client/http_post.h>
#include <http/server/request_handler.hpp>
#include "policed_mongodb.h"
#include "game_police.h"
#include <object/OprateResult.h>
#include <object/SharedDef.h>

#if 0
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

static bool getJson(const string& str, pt::ptree& tree)
{
	stringstream ss;
	ss << str;

	try
	{
		pt::read_json(ss,tree);
		return true;
	}
	catch (std::exception& e)
	{
		tea_perror("getJson %s",e.what());
		return false;	
	}
}

static string toJson(std::map<string,string>& m)
{
	if(m.empty())
		return "{}";
	pt::ptree tree;
	for (auto kv: m)
		tree.add(kv.first, kv.second);
	stringstream ss;
	pt::write_json(ss, tree);
	return ss.str();
}
#else

#include <minijson/minijson_writer.hpp>
#include <minijson/minijson_reader.hpp>

static string toJson(std::map<string,string>& m)
{
	if(m.empty())
		return "{}";

	std::stringstream ss;
	minijson::object_writer writer(ss);
	for (auto kv: m)	
		writer.write(kv.first.c_str(),kv.second);
	writer.close();
	return ss.str();
}

typedef std::vector< std::map<string,string> > JsonArray;
static bool fromJsonArray(const string& str_json,JsonArray& ar)
{
	ar.clear();
	std::map<string,string> m;
	try
	{
		stringstream ss;
		ss << str_json;
		minijson::const_buffer_context ctx(str_json.c_str(), str_json.size()); // may throw
		minijson::parse_array(ctx,[&](minijson::value v){
			m.clear();
			minijson::parse_object(ctx,[&](const char* k,minijson::value v){		
				m[k] = v.as_string();
			});
			ar.push_back(m);
		});
	}
	catch (std::exception& e)
	{
		tea_perror("fromJson :%s",e.what());
		return false;
	}	
	return true;
}

static bool fromJson(const string& str_json,std::map<string,string>& m)
{
	m.clear();
	try
	{
		minijson::const_buffer_context ctx(str_json.c_str(), str_json.size()); // may throw
		minijson::parse_object(ctx,[&](const char* k,minijson::value v){		
			m[k] = v.as_string();
		});
	}
	catch (std::exception& e)
	{
		tea_perror("fromJson :%s",e.what());
		return false;
	}	
	return true;
}

#endif

////为了一个base64 至于吗
//#include <boost/archive/iterators/base64_from_binary.hpp>
//#include <boost/archive/iterators/insert_linebreaks.hpp>
//#include <boost/archive/iterators/transform_width.hpp>
//#include <boost/archive/iterators/ostream_iterator.hpp>
//
//static string toBase64(const string& input)
//{
//	using namespace boost::archive::iterators;
//	std::stringstream os;
//	typedef 
//		insert_linebreaks<         // insert line breaks every 72 characters
//		base64_from_binary<    // convert binary values to base64 characters
//		transform_width<   // retrieve 6 bit integers from a sequence of 8 bit bytes
//		const char *,
//		6,
//		8
//		>
//		> 
//		,72
//		> 
//		base64_text; // compose all the above operations in to a new iterator
//
//	std::copy(
//		base64_text(input.c_str()),
//		base64_text(input.c_str() + input.size()),
//		ostream_iterator<char>(os)
//		);
//	return os.str();
//}


namespace hs = http::server;

static string toQueryString(std::map<string,string>& m)
{
	if(m.empty())
		return "";
	stringstream ss;
	for (auto kv:m)	
		ss << kv.first << "=" << kv.second << "&";
	string dst = ss.str();
	dst.erase(dst.end()-1);	
	return dst;
}

static string getUrl(string host_port,string page)
{	
	ASSERT(!host_port.empty());
	ASSERT(!page.empty() && page[0] == '/');
	if(*(host_port.end()-1) == '/')
		host_port.erase(host_port.end()-1);
	return host_port+page;
}

//////////////////////////////////////////////////////////////////////////
//for HttpHandler
HttpHandler::HttpHandler():m_server(0),m_client(0),m_reg_state(0)
{
	m_server = new hs::server("0.0.0.0","");
	m_client = new http::client::client;

	//打印一下监听的端口
	tea_pinfo("$> http listen on port:%d",m_server->get_port());

	Init();
}

HttpHandler::~HttpHandler()
{
	safe_delete(m_server);
	safe_delete(m_client);
}

void HttpHandler::Init()
{
	auto& h = m_server->get_request_handler();

	h.add_hanlde("/",[](const hs::request& req, hs::reply& rep){
		rep.content = "oh no, policed!";
	});
}

void HttpHandler::AddHandler(string uri, Handler handler)
{
	auto& h = m_server->get_request_handler();
	h.add_hanlde(uri, std::bind(handler, this, std::placeholders::_1, std::placeholders::_2));
}

void HttpHandler::Update()
{
	m_server->poll();
	m_client->poll();

	//发送去注册一下
	if(m_reg_state == 0)
		RegisterWeb();	
}
//TODO:注册web端口地址,如果是腾讯还需要单独注册一个地方
void HttpHandler::RegisterWeb(bool force/* = false*/)
{
	//是否强制刷新
	if(force) m_reg_state = 0;

	//如果已经注册过了，则返回
	if(m_reg_state != 0)
		return;

	auto *g = dynamic_cast<GlobalValueManager*>(PolicedApp::g_app);
	auto conf_svr_url = g->GetConfSvrUrl();//g->GetExtWebInterface();

	if(conf_svr_url.empty())
	{
		m_reg_state = -1;
		return;
	}

	//取得所有合服表
	vector<string> server_names;
	g->GetMergeServerName(server_names);
	string all_server_names;
	StrJoin(server_names,all_server_names, ';');

	//构造注册包体
	std::map<string,string> data;
	data["server_names"] = all_server_names;
	data["port"] = std::to_string(m_server->get_port());
	data["type"] = "policed";

	//TODO:这里等正式上线的时候可以把URL改为正式地址 /cow_config/app_addr/reg
	m_client->async_post(getUrl(conf_svr_url, "/cow_config/app_addr/reg"),toQueryString(data),[this](int status_code,const string& content){
		if (status_code == 200)
			m_reg_state = 2;
		else
			m_reg_state = 0;	// 重头再来		
	});
	m_reg_state = 1;	
}


string HttpHandler::MakeResponse(int ret, string msg)
{
	stringstream ss;
	ss << ret;	
	std::map<string,string> response;
	response["ret"] = ss.str();
	response["msg"] = msg;
	return toJson(response);
}


