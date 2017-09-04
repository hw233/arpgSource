//
// request_handler.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <string>
#include <boost/function.hpp>
#include <unordered_map>
#include <map>

#include "reply.hpp"
#include "request.hpp"

namespace http {
namespace server {


/// The common handler for all incoming requests.
class request_handler
{
private:
  request_handler(const request_handler&)/* = delete*/;
  request_handler& operator=(const request_handler&) /*= delete*/;

public:
  typedef boost::function<void(const request&,reply&)> hanlder;

  /// Construct with a directory containing files to be served.
  explicit request_handler(const std::string& doc_root);

  /// Handle a request and produce a reply.
  void handle_request(const request& req, reply& rep);

  void add_hanlde(std::string, hanlder);
private:
  /// The directory containing the files to be served.
  std::string doc_root_;

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool url_decode(const std::string& in, std::string& out);

  /// MapHanlder
  void hanlde_static_file(const request& req, reply& rep);

  std::unordered_map<std::string,hanlder> all_hanlder_;
};

//¸ù¾Ý?a=1&b=2·µ»Ømap
extern bool parse_query(const std::string& q,std::map<std::string,std::string>& qm);

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_HPP
