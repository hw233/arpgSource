#if 0
#include "http_post.h"
#include <iostream>

#include "server.hpp"
#include "request_handler.hpp"

int main()
{
	bool runing = true;
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF  );	

	//////////////////////////////////////////////////////////////////////////
	//服务器
	http::server::server svr("0.0.0.0","0","./");
	auto& handlers = svr.get_request_handler();
	handlers.add_hanlde("/cow_config/server_alias/get",[](const http::server::request& req,http::server::reply& rep){				
		rep.content = "321";
	});

	//////////////////////////////////////////////////////////////////////////
	//客户端
	http::client::client client;
	
	//{//测试同步方式post
	//	int status;
	//	string response;
	//	status = client.post("http://192.168.30.13:8081/cow_config/server_alias/get","server_name=349_1", response);
	//	std::cout << response;
	//}

	//异步方式post
	auto port = svr.get_port();
	string url = "http://127.0.0.1:" + std::to_string(port) + "/cow_config/server_alias/get";	
	client.async_post(url,"server_name=349_1",[&](int status,const string& response){
		std::cout << status << std::endl;
		std::cout << response << std::endl;

		runing = false;
	});

	while (runing)
	{
		svr.poll_one();
		client.poll_one();
	}

	return 0;
}
#endif