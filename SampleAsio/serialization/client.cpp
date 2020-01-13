//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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

  /// Handle completion of a read operation.
  void handle_read(const asio::error_code& e)
  {
    if (!e)
    {
      // Print out the data that was received.

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

void test_serialization_client()
{
  try
  {
    
    asio::io_context io_context;
	serialization::client client(io_context, "127.0.0.1", "9601");
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

}
