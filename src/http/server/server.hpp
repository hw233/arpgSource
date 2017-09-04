//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <string>

namespace http {
namespace server {

class request_handler;
struct server_impl;

/// The top-level class of the HTTP server.
class server
{
private:
  server(const server&)/* = delete*/;

  server& operator=(const server&)/* = delete*/;
public:
  /// Construct the server to listen on the specified TCP address and port, and
  /// serve up files from the given directory.
  explicit server(const std::string& address = "0.0.0.0",
	  const std::string& port = "8090",
      const std::string& doc_root = "./");
   ~server();

  /// Run the server's io_service loop.
  void run();

  void poll_one();

  void poll();

  //  register for process termination notifications
  void hook_signle();

  request_handler& get_request_handler();

  unsigned short get_port();
private:
  server_impl *impl_;
};

} // namespace server
} // namespace http

#endif // HTTP_SERVER_HPP
