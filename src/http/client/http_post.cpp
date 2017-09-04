//
//  http_post.cpp
//  unix-example
//
//  Created by ying on 15/7/19.
//  Copyright (c) 2015 linbc. All rights reserved.
//

#include "http_post.h"

#include <set>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using boost::asio::ip::tcp;

namespace http{
namespace client{

#ifndef WIN32
#define strnicmp strncasecmp
#endif

/*去空白*/
static char *_ltrim(char *pStr){
	for(;;){
		if (*pStr == '\0') {
			return pStr;
		}
		if (*pStr != ' ' && *pStr != '\r' && *pStr != '\n' && *pStr != '\t' && *pStr != '\b') {
			return pStr;
		}
		pStr++;
	}
}

/*
*	从URL中得到主机名及端口
*	注意了，url里面一定要包含'/'
*/
static int http_parse_host(char* url,char *out_host,char *out_port,char *page){
	int size = 0;
	char *sp_port = NULL;
	//先置
	out_host[size] = '\0';
	strcpy(out_port , "80");
	//去掉头部空白
	url = _ltrim(url);
	if(strnicmp(url,"http://",7) == 0)
		url = url + 7;
	//pos = strchr(cur_pos,'/');
	for(;;){
		if(url[size] == '/')
			break;
		if(url[size] == '\0'|| url[size] == ' ' || url[size] == '\n' || url[size] == '\r' || url[size] == '\t' || url[size] == '\b')
			return -1;
		out_host[size] = url[size];
		if(out_host[size] == ':')
			sp_port = &out_host[size];
		size++;
	}
	out_host[size] = '\0';
	if(sp_port != NULL){
		int nport = 0;
		sscanf(sp_port,":%d",&nport);
		sprintf(out_port, "%d", nport);
		*sp_port = '\0';
	}
	strcpy(page,url + size);
	return 0;
}

//////////////////////////////////////
struct http_request:public boost::enable_shared_from_this<http_request>
{
	typedef std::set< boost::shared_ptr<http_request> > request_set;

	http_request(boost::asio::io_service& io_service,request_set& set): status_(0),
		resolver_(io_service),
		socket_(io_service),
		all_req_(set)
	{   		
	}

	void init( const std::string& host, const std::string& port,
		const std::string& path, const std::string& data,
		client::callback cb)
	{
		cb_ = cb;

		string host_port = host;
		if(!port.empty())
			host_port = host_port + ":" + port;

		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		std::ostream request_stream(&request_);
		request_stream << "POST " << path << " HTTP/1.0\r\n";
		request_stream << "Host: " << host_port << "\r\n";		
		request_stream << "Accept: */*\r\n";
		request_stream << "Content-Length: " << data.length() << "\r\n";
		request_stream << "Content-Type: application/x-www-form-urlencoded\r\n";
		request_stream << "Connection: close\r\n";
		request_stream << "\r\n";

		request_stream << data;

		// Start an asynchronous resolve to translate the server and service names
		// into a list of endpoints.
		tcp::resolver::query query(host, port);
#if 1
		resolver_.async_resolve(query,
			boost::bind(&http_request::handle_resolve, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::iterator));
#else				
		boost::system::error_code err;
		tcp::resolver::iterator endpoint_iterator = resolver_.resolve(query);
		handle_resolve(err, endpoint_iterator);
#endif
	}

private:
	void handle_resolve(const boost::system::error_code& err,
		tcp::resolver::iterator endpoint_iterator)
	{
		if (!err)
		{
			// Attempt a connection to each endpoint in the list until we
			// successfully establish a connection.
			boost::asio::async_connect(socket_, endpoint_iterator,
				boost::bind(&http_request::handle_connect,  shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			status_ = -1;
			reponse_data_ = "Error: " + err.message();
			do_callback();
		}
	}

	void handle_connect(const boost::system::error_code& err)
	{
		if (!err)
		{
			// The connection was successful. Send the request.
			boost::asio::async_write(socket_, request_,
				boost::bind(&http_request::handle_write_request,  shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			status_ = -2;
			reponse_data_ = "Error: " + err.message();
			do_callback();
		}
	}

	void handle_write_request(const boost::system::error_code& err)
	{
		if (!err)
		{
			// Read the response status line. The response_ streambuf will
			// automatically grow to accommodate the entire line. The growth may be
			// limited by passing a maximum size to the streambuf constructor.
			boost::asio::async_read_until(socket_, response_, "\r\n",
				boost::bind(&http_request::handle_read_status_line,  shared_from_this(),

				boost::asio::placeholders::error));
		}
		else
		{
			status_ = -3;
			reponse_data_ = "Error: " + err.message();
			do_callback();
		}
	}

	void handle_read_status_line(const boost::system::error_code& err)
	{
		if (!err)
		{
			// Check that response is OK.
			std::istream response_stream(&response_);
			std::string http_version;
			response_stream >> http_version;
			unsigned int status_code;
			response_stream >> status_code;
			std::string status_message;
			std::getline(response_stream, status_message);
			if (!response_stream || http_version.substr(0, 5) != "HTTP/")
			{					
				status_ = -4;
				reponse_data_ = "Invalid response";
				return;
			}
			if (status_code != 200)
			{
				//如果不是正确的头
				status_ = status_code;
				//reponse_data_ = "Response returned with status code ";					
				response_stream >> reponse_data_;
				return;
			}

			// Read the response headers, which are terminated by a blank line.
			boost::asio::async_read_until(socket_, response_, "\r\n\r\n",
				boost::bind(&http_request::handle_read_headers,  shared_from_this(),
				boost::asio::placeholders::error));

			//这里设置一下http的状态
			status_ = status_code;
		}
		else
		{
			status_ = -5;
			reponse_data_ = "Error: " + err.message();
			do_callback();
		}
	}

	void handle_read_headers(const boost::system::error_code& err)
	{
		if (!err)
		{
			// Process the response headers.
			std::istream response_stream(&response_);
			std::string header;

			//TODO:目前头没什么用,如果以后要加入cookis的功能的时候可能就有用了
			std::vector<string> headers;
			while (std::getline(response_stream, header) && header != "\r")
				headers.push_back(header);

			//// Write whatever content we already have to output.
			//if (response_.size() > 0)
			//	response_stream >> reponse_data_;

			// Start reading remaining data until EOF.
			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				boost::bind(&http_request::handle_read_content,  shared_from_this(),
				boost::asio::placeholders::error));
		}
		else
		{
			status_ = -6;
			reponse_data_ = "Error: " + err.message();
			do_callback();
		}
	}

	void handle_read_content(const boost::system::error_code& err)
	{
		if (!err)
		{
			std::istream response_stream(&response_);			

			// Write all of the data that has been read so far.			
			//std::cout << &response_;

			// Continue reading remaining data until EOF.
			boost::asio::async_read(socket_, response_,
				boost::asio::transfer_at_least(1),
				boost::bind(&http_request::handle_read_content,  shared_from_this(),
				boost::asio::placeholders::error));
			return;
		}

		if (err != boost::asio::error::eof)
		{
			status_ = -7;
			reponse_data_ = "Error: " + err.message();
		}

		if (response_.size())
		{
			std::istream response_stream(&response_);
			std::istreambuf_iterator<char> eos;
			reponse_data_ = string(std::istreambuf_iterator<char>(response_stream), eos);						
		}

		do_callback();
	}

	void do_callback()
	{
		//执行回调后从容器中移除
		if(status_ != 0 && cb_)
			cb_(status_, reponse_data_);
		all_req_.erase(shared_from_this());
	}

public:
	string host_;
	string port_;
	string request_path_;
	client::callback cb_;

	int status_;				//http请求返回的状态
	string reponse_data_;		//post返回的内容

private:
	tcp::resolver resolver_;
	tcp::socket socket_;
	boost::asio::streambuf request_;
	boost::asio::streambuf response_;
	request_set& all_req_;				//所有请求集合
};

//////////////////////////////////////
//for clientImpl
struct client_impl
{
	http_request::request_set all_request_;
	boost::asio::io_service io_service_;
};


//////////////////////////////////////
//for client
client::client()
{
	impl_ = new client_impl;
}

client::~client()
{
	if (impl_) {
		delete impl_;
		impl_ = nullptr;
	}
}

int client::post(const string& url, const string& data, string& reponse_data)
{
	char host[50],port[10],page[500];
	/*解析URL*/
	if(http_parse_host((char*)url.c_str(),host,port,page)){
		return -1;
	}

	string host_port = host;
	if(strlen(port)>0)
		host_port = host_port + ":" + port;

	unsigned int status_code = 0;
	try
	{   

		if (impl_->io_service_.stopped())
			impl_->io_service_.reset();

		auto& io_service = impl_->io_service_;

		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(host, port);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		// Try each endpoint until we successfully establish a connection.
		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);

		// Form the request. We specify the "Connection: close" header so that the
		// server will close the socket after transmitting the response. This will
		// allow us to treat all data up until the EOF as the content.
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
 		request_stream << "POST " << page << " HTTP/1.0\r\n";
		request_stream << "Host: " << host_port << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Content-Length: " << data.length() << "\r\n";
		request_stream << "Content-Type: application/x-www-form-urlencoded\r\n";
		request_stream << "Connection: close\r\n\r\n";
		request_stream << data;

		// Send the request.
		boost::asio::write(socket, request);

		// Read the response status line. The response streambuf will automatically
		// grow to accommodate the entire line. The growth may be limited by passing
		// a maximum size to the streambuf constructor.
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		//unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			reponse_data = "Invalid response";
			return -2;
		}
		if (status_code != 200)
		{
			reponse_data = "Response returned with status code != 200 " ;
			return status_code;
		}

		// Process the response headers.
		std::string header;
		std::vector<string> headers;		
		while (std::getline(response_stream, header) && header != "\r")
			headers.push_back(header);

		// Read the response headers, which are terminated by a blank line.
		//boost::asio::read_until(socket, response, "\r\n\r\n");

		// Read until EOF, writing data to output as we go.
		boost::system::error_code error;
		while (boost::asio::read(socket, response,
			boost::asio::transfer_at_least(1), error))
		{			
			//
			//// Write all of the data that has been read so far.			
			////std::cout << &response_;
			//response_stream >> reponse_data;
		}

		if (response.size())
		{
			std::istream response_stream(&response);
			std::istreambuf_iterator<char> eos;
			reponse_data = string(std::istreambuf_iterator<char>(response_stream), eos);						
		}

		if (error != boost::asio::error::eof)
		{
			reponse_data = error.message();
			return -3;
		}
	}
	catch (std::exception& e)
	{
		reponse_data = e.what();
		return -4;		
	}

	return status_code;
}

bool client::async_post(const string& url, const string& data,callback cb)
{
	char host[50],port[10],page[500];
	memset(host,0,sizeof(host));
	memset(port,0,sizeof(port));
	memset(page,0,sizeof(page));
	/*解析URL*/
	if(http_parse_host((char*)url.c_str(),host,port,page)){
		return false;
	}

	auto req = boost::shared_ptr<http_request>(new http_request(impl_->io_service_,impl_->all_request_));
	req->init(host,port,page,data,cb);
	if (impl_->io_service_.stopped())
		impl_->io_service_.reset();

	return true;
}

void client::poll()
{
	impl_->io_service_.poll();
}

void client::poll_one()
{
	impl_->io_service_.poll_one();
}

}
}