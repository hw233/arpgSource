//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"

using std::string;

namespace http {
namespace server {

request_handler::request_handler(const std::string& doc_root)
  : doc_root_(doc_root)
{
}

void request_handler::add_hanlde(std::string uri, hanlder hanlder)
{
	all_hanlder_[uri] = hanlder;
	//all_hanlder_.insert(std::make_pair(uri, hanlder));
}

void request_handler::handle_request(const request& req, reply& rep)
{
	//如果可以找到正常处理的
  auto it = all_hanlder_.find(req.uri);
  if(it != all_hanlder_.end())
	return it->second(req,rep);

  ////写入包内容长度
  //rep.headers.resize(1);
  //rep.headers[0].name = "Content-Length";
  //rep.headers[0].value = std::to_string(rep.content.size());

  //查找一下默认handler
  it = all_hanlder_.find("");
  if(it != all_hanlder_.end())
	 return it->second(req,rep);

  //这个时间看看要不要变态文件处理
  if(!doc_root_.empty())
	  return hanlde_static_file(req,rep);

  //还是没找到处理的
  rep = reply::stock_reply(reply::not_found);  
}

//根据?a=1&b=2返回map
bool parse_query(const string& q,std::map<string,string>& qm)
{
	qm.clear();
	if(q.empty())
		return true;

	int size = q.size();
	string key,value;
	bool read_key = true;

	int i = q[0] == '?' ? 1 : 0;
	for(; i < size; ){
		int pos = q.find(read_key?'=':'&',i);
		if(pos == string::npos){
			if(read_key)
				key = q.substr(i,size-i);
			else
				value = q.substr(i,size-i);
			qm.insert(std::make_pair(key,value));
			break;
		} else if(pos < size){
			if(read_key)
				key = q.substr(i,pos-i);
			else
				value = q.substr(i,pos-i); 
			i = pos+1;
			read_key = !read_key;
			//如果不是读值，说明已经读完一个key-value
			if(read_key){
				qm[key] = value;
				key = value = "";
			}
		}
	}
	if(!key.empty())
		qm.insert(std::make_pair(key,""));
	return true;
}

void request_handler::hanlde_static_file(const request& req, reply& rep)
{
	// Decode url to path.
	std::string request_path;
	if (!url_decode(req.uri, request_path))
	{
		rep = reply::stock_reply(reply::bad_request);
		return;
	}

	// Request path must be absolute and not contain "..".
	if (request_path.empty() || request_path[0] != '/'
		|| request_path.find("..") != std::string::npos)
	{
		rep = reply::stock_reply(reply::bad_request);
		return;
	}

	// If path ends in slash (i.e. is a directory) then add "index.html".
	if (request_path[request_path.size() - 1] == '/')
	{
		request_path += "index.html";
	}

	// Determine the file extension.
	std::size_t last_slash_pos = request_path.find_last_of("/");
	std::size_t last_dot_pos = request_path.find_last_of(".");
	std::string extension;
	if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
	{
		extension = request_path.substr(last_dot_pos + 1);
	}

	// Open the file to send back.
	std::string full_path = doc_root_ + request_path;
	std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
	if (!is)
	{
		rep = reply::stock_reply(reply::not_found);
		return;
	}

	// Fill out the reply to be sent to the client.
	rep.status = reply::ok;
	char buf[512];
	while (is.read(buf, sizeof(buf)).gcount() > 0)
		rep.content.append(buf, (int)is.gcount());
	rep.headers.resize(2);
	rep.headers[0].name = "Content-Length";
	rep.headers[0].value = std::to_string(rep.content.size());
	rep.headers[1].name = "Content-Type";
	rep.headers[1].value = mime_types::extension_to_type(extension);
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server
} // namespace http
