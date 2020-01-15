//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SERIALIZATION_CLIENT_HPP
#define SERIALIZATION_CLIENT_HPP

#include "../stdafx.h";

#include "connection.hpp" // Must come before boost/serialization headers.

using namespace std::placeholders;

namespace serialization {

/// Downloads stock quote information from a server.
class client
{
public:
  /// Constructor starts the asynchronous connect operation.
  client(asio::io_context& io_context,
      const std::string& host, const std::string& service)
    : connection_(io_context)
  {
    // Resolve the host name into an IP address.
    asio::ip::tcp::resolver resolver(io_context);
    asio::ip::tcp::resolver::query query(host, service);
    asio::ip::tcp::resolver::iterator endpoint_iterator =
      resolver.resolve(query);

    // Start an asynchronous connect operation.
    asio::async_connect(connection_.socket(), endpoint_iterator,
        std::bind(&client::handle_connect, this,  _1));
  }

  /// Handle completion of a connect operation.
  void handle_connect(const asio::error_code& e)
  {
    if (!e)
    {
      // Successfully established connection. Start operation to read the list
      // of stocks. The connection::async_read() function will automatically
      // decode the data that is read from the underlying socket.
      connection_.async_read(camelMessage_,
          std::bind(&client::handle_read, this, _1));
    }
    else
    {
      // An error occurred. Log it and return. Since we are not starting a new
      // operation the io_context will run out of work to do and the client will
      // exit.
      std::cerr << e.message() << std::endl;
    }
  }

  void to_write(CamelMessage camelMessage) {

	  connection_.async_write(camelMessage,
		  std::bind(&client::handle_write,this,_1,_2));
  }

  void handle_write(const asio::error_code& ec ,std::size_t bytes_transferred) {
	  if (!ec) {
		  std::this_thread::sleep_for(std::chrono::seconds(2));
		  std::cout << "send data " << bytes_transferred <<" bytes ;" << std::endl;
	  }
  }

  /// Handle completion of a read operation.
  void handle_read(const asio::error_code& e)
  {
    if (!e)
    {
		// Print out the data that was received.
		if (camelMessage_.onlyone_case() != CamelMessage::kAllowFromAgvs)
		{
			std::cerr << "the message type is wrong, not AllowFromAgvs" << std::endl;
			return;
		}
		if (!camelMessage_.has_allowfromagvs())
		{
			std::cerr << "the agvs data is null" << std::endl;
			return;
		}

		const CamelMessage_AllowFromAgvs allowFromAgvs = camelMessage_.allowfromagvs();

		std::cout << std::endl << "======================= received data info from agvs ==========================" << std::endl;
		if (allowFromAgvs.has_agv())
		{
			std::cout << "  Agv info: name            " << allowFromAgvs.agv().name() << std::endl;
			std::cout << "            current station " << allowFromAgvs.agv().currentstation() << std::endl;
			std::cout << "            target station  " << allowFromAgvs.agv().targetstation() << std::endl;
			std::cout << "            isUseable       " << allowFromAgvs.agv().isuseable() << std::endl;
			std::cout << "            hasTask         " << allowFromAgvs.agv().hastask() << std::endl;
		}
		std::cout << "  errorMessage : " << allowFromAgvs.errormessage() << std::endl;
		std::cout << "  allowCharging: " << allowFromAgvs.allowcharging() << std::endl;

		for (int i = 0; i < allowFromAgvs.otheragv_size(); i++)
		{
			std::cout << "  =====Other AGV " << i << " info==========" << std::endl;
			std::cout << "            name            " << allowFromAgvs.otheragv(i).name() << std::endl;
			std::cout << "            current station " << allowFromAgvs.otheragv(i).currentstation() << std::endl;
			std::cout << "            target station  " << allowFromAgvs.otheragv(i).targetstation() << std::endl;
			std::cout << "            isUseable       " << allowFromAgvs.otheragv(i).isuseable() << std::endl;
			std::cout << "            hasTask         " << allowFromAgvs.otheragv(i).hastask() << std::endl;
		}
		std::cout << "======================= received data info end ==========================" << std::endl;

    }
    else
    {
      // An error occurred.
      std::cerr << e.message() << std::endl;
    }

    // Since we are not starting a new operation the io_context will run out of
    // work to do and the client will exit.
  }


private:
  /// The connection to the server.
  connection connection_;

  /// The data received from the server.
  CamelMessage camelMessage_;
};

} // namespace s11n_example

#endif