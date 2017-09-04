#ifndef _HTTP_HANLDER_H
#define _HTTP_HANLDER_H

namespace http{
namespace client{class client;}
namespace server{class server; struct reply; struct request;}
}

class HttpHandler
{
public:
	typedef http::server::request Request;
	typedef  http::server::reply Reply;
	typedef void (HttpHandler::*Handler)(const Request& req, Reply& rep);

	HttpHandler();
	~HttpHandler();

	void Init();

	//发送本地监听web接口给远程服务器
	void RegisterWeb(bool force = false);
	void AddHandler(string uri, Handler handler);
	string MakeResponse(int ret, string msg);

	void Update();
private:
	http::server::server *m_server;
	http::client::client *m_client;

	//注册状态, 0:初始化 1:准备注册 2:注册成功 -1:不需要注册
	int m_reg_state;
};


#endif
