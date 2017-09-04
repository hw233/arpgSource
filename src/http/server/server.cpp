//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.hpp"
#include <signal.h>
#include <utility>

//#define SHUT_RDWR 2

#include <boost/asio.hpp>
#include "connection.hpp"
#include "connection_manager.hpp"

#include "request_handler.hpp"

namespace http {
namespace server {

//////////////////////////////////////////////////////////////////////////
//for server_impl
struct server_impl
{
	server_impl(const std::string& address, const std::string& port,const std::string& doc_root): 
		io_service_(),
		signals_(io_service_),
		acceptor_(io_service_),
		connection_manager_(),
		socket_(io_service_),
		request_handler_(doc_root)
	{
		// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
		boost::asio::ip::tcp::resolver resolver(io_service_);
		boost::asio::ip::tcp::resolver::query query(address, port);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();

		do_accept();
		do_await_stop();
	}


	void do_accept()
	{
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
		{
			// Check whether the server was stopped by a signal before this
			// completion handler had a chance to run.
			if (!acceptor_.is_open())
			{
				return;
			}

			if (!ec)
			{
				connection_manager_.start(std::make_shared<connection>(
					std::move(socket_), connection_manager_, request_handler_));
			}

			do_accept();
		});
	}

	void do_await_stop()
	{
		signals_.async_wait(
			[this](boost::system::error_code /*ec*/, int /*signo*/)
		{
			// The server is stopped by cancelling all outstanding asynchronous
			// operations. Once all operations have finished the io_service::run()
			// call will exit.
			acceptor_.close();
			connection_manager_.stop_all();
		});
	}

	/// The io_service used to perform asynchronous operations.
	boost::asio::io_service io_service_;

	/// The signal_set is used to register for process termination notifications.
	boost::asio::signal_set signals_;

	/// Acceptor used to listen for incoming connections.
	boost::asio::ip::tcp::acceptor acceptor_;

	/// The connection manager which owns all live connections.
	connection_manager connection_manager_;

	/// The next socket to be accepted.
	boost::asio::ip::tcp::socket socket_;

	/// The handler for all incoming requests.
	request_handler request_handler_;
};

//////////////////////////////////////////////////////////////////////////
//for server
server::server(const std::string& address, const std::string& port,const std::string& doc_root)
{
	impl_ = new server_impl(address,port,doc_root);

}

server::~server()
{
	if (impl_)
	{
		delete impl_;
		impl_ = nullptr;
	}

}

void server::hook_signle()
{
	// Register to handle the signals that indicate when the server should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.
	impl_->signals_.add(SIGINT);
	impl_->signals_.add(SIGTERM);
#if defined(SIGQUIT)
	impl_->signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
}

void server::run()
{
	// The io_service::run() call will block until all asynchronous operations
	// have finished. While the server is running, there is always at least one
	// asynchronous operation outstanding: the asynchronous accept call waiting
	// for new incoming connections.
	impl_->io_service_.run();
}

void server::poll_one()
{
	impl_->io_service_.poll_one();
}

void server::poll()
{
	impl_->io_service_.poll();
}

request_handler& server::get_request_handler()
{
	return impl_->request_handler_;
};

unsigned short server::get_port()
{
	auto et = impl_->acceptor_.local_endpoint();
	return et.port();
}

} // namespace server
} // namespace http
