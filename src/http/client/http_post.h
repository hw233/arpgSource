//
//  http_post.h
//  unix-example
//
//  Created by ying on 15/7/19.
//  Copyright (c) 2015 linbc. All rights reserved.
//

#ifndef __unix_example__http_post__
#define __unix_example__http_post__

#include <string>
#include <map>
#include <functional>

using std::string;

namespace http{
namespace client{

struct client_impl;

class client
{
public:
    typedef std::function<void(int,const std::string&)> callback;
    
    enum DATA_FORMAT
    {
        QUERY_STRING,
        JSON,
        XML
    };
    
public:
    client();
    ~client();
    
    string get_data(const std::map<string,string>& data,DATA_FORMAT fmt);
    
    int post(const string& url, const string& data, string& reponse);
    
    bool async_post(const string& url, const string& data,callback cb);

    void poll();

	void poll_one();
    
private:
    client_impl *impl_;
};


}
}

#endif /* defined(__unix_example__http_post__) */
