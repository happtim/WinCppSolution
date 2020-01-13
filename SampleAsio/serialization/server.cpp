//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "../stdafx.h"

#include "connection.hpp" // Must come before boost/serialization headers.

using namespace std::placeholders;

namespace serialization {

/// Serves stock quote information to any client that connects to it.
class server
{
public:
  /// Constructor opens the acceptor and starts waiting for the first incoming
  /// connection.
  server(asio::io_context& io_context, unsigned short port)
    : acceptor_(io_context,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
  {

	  auto pReportToAgvs =  std::make_shared<CamelMessage_ReportToAgvs>();
	  camelMessage_.set_allocated_reporttoagvs(pReportToAgvs.get());
	  pReportToAgvs->set_battery(0.8);
	  pReportToAgvs->set_currentstation("S200");
	  pReportToAgvs->set_ischarging(false);
	  pReportToAgvs->set_version("0102");

	  auto pPosion = std::make_shared<Posion>();
	  pReportToAgvs->set_allocated_posion(pPosion.get());
	  pPosion->set_x(1.11);
	  pPosion->set_y(-2.22);
	  pPosion->set_theta(1.57);

	  auto pDevice = std::shared_ptr<Device>( pReportToAgvs->add_devices() );
	  pDevice->set_id(1);
	  pDevice->set_name("test01");
	  pDevice->set_value(0.2);
	  pDevice->set_isactivate(true);

	  if ((time(0) % 2))
	  {
		  pDevice = std::shared_ptr<Device>(pReportToAgvs->add_devices());
		  pDevice->set_id(2);
		  pDevice->set_name("test05");
		  pDevice->set_value(0.8);
		  pDevice->set_isactivate(true);
	  }

    // Start an accept operation for a new connection.
    connection_ptr new_conn(new connection(acceptor_.get_io_context()));
    acceptor_.async_accept(new_conn->socket(),
        std::bind(&server::handle_accept, this, _1, new_conn));
  }

  /// Handle completion of a accept operation.
  void handle_accept(const asio::error_code& e, connection_ptr conn)
  {
    if (!e)
    {
      // Successfully accepted a new connection. Send the list of stocks to the
      // client. The connection::async_write() function will automatically
      // serialize the data structure for us.
      conn->async_write(camelMessage_,
          std::bind(&server::handle_write, this, _1, conn));
    }

    // Start an accept operation for a new connection.
    connection_ptr new_conn(new connection(acceptor_.get_io_context()));
    acceptor_.async_accept(new_conn->socket(),
        std::bind(&server::handle_accept, this,  _1 , new_conn));
  }

  /// Handle completion of a write operation.
  void handle_write(const asio::error_code& e, connection_ptr conn)
  {
    // Nothing to do. The socket will be closed automatically when the last
    // reference to the connection object goes away.
  }

private:
  /// The acceptor object used to accept incoming socket connections.
  asio::ip::tcp::acceptor acceptor_;

  /// The data to be sent to each client.
  CamelMessage camelMessage_;
};

} // namespace s11n_example

void test_serialization_server()
{
  try
  {

    asio::io_context io_context;
	serialization::server server(io_context, 9601);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

}
