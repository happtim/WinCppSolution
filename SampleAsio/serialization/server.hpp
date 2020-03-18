//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SERIALIZATION_SERVER_HPP
#define SERIALIZATION_SERVER_HPP


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

    // Start an accept operation for a new connection.
    connection_ptr new_conn(new connection(io_context));
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
		auto pAllowFromAgvs = camelMessage_.mutable_allowfromagvs();
		pAllowFromAgvs->set_errormessage("ÖÐÎçÄØ error");
		pAllowFromAgvs->set_allowcharging(false);

		auto pAgv = pAllowFromAgvs->mutable_agv();
		pAgv->set_name("AGV#01");
		pAgv->set_currentstation("S0001");
		pAgv->set_targetstation("S0002");
		pAgv->set_isuseable(true);
		pAgv->set_hastask(true);

		auto pOtherAgv = pAllowFromAgvs->add_otheragv();
		pOtherAgv->set_name("AGV#02");
		pOtherAgv->set_currentstation("S0002");
		pOtherAgv->set_targetstation("S0003");
		pOtherAgv->set_isuseable(true);
		pOtherAgv->set_hastask(true);

      conn->async_write(camelMessage_,
          std::bind(&server::handle_write, this, _1, conn));

	  conn->async_read(camelMessage_,
		  std::bind(&server::handle_read, this, _1, conn));
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

  /// Handle completion of a read operation.
  void handle_read(const asio::error_code& e, connection_ptr conn)
  {
	  if (!e)
	  {
		  std::this_thread::sleep_for(std::chrono::seconds(1));
		  // Print out the data that was received.
		  if (camelMessage_.onlyone_case() != CamelMessage::kReportToAgvs)
		  {
			  std::cerr << "the message type is wrong, not AllowFromAgvs" << std::endl;
			  return;
		  }
		  if (!camelMessage_.has_reporttoagvs())
		  {
			  std::cerr << "the agvs data is null" << std::endl;
			  return;
		  }

		  const CamelMessage_ReportToAgvs reportToAgvs = camelMessage_.reporttoagvs();

		  std::cout << "======================= received data info begin ==========================" << std::endl;
		  std::cout << "  Current station is " << reportToAgvs.currentstation() << std::endl;
		  std::cout << "  Version is " << reportToAgvs.version() << std::endl;
		  std::cout << "  battery is " << reportToAgvs.battery() << std::endl;
		  std::cout << "  isCharging is " << reportToAgvs.ischarging() << std::endl;

		  for (int i = 0; i < reportToAgvs.devices_size(); i++)
		  {
			  std::cout << "  =====Device " << i << " info======" << std::endl;
			  std::cout << "     id       : " << reportToAgvs.devices(i).id() << std::endl;
			  std::cout << "     name     : " << reportToAgvs.devices(i).name() << std::endl;
			  std::cout << "     value    : " << reportToAgvs.devices(i).value() << std::endl;
			  std::cout << "     isActive : " << reportToAgvs.devices(i).isactivate() << std::endl;
		  }

		  if (reportToAgvs.has_posion())
		  {
			  std::cout << "  ===== Posion info  ======" << std::endl;
			  std::cout << "         x    : " << reportToAgvs.posion().x() << std::endl;
			  std::cout << "         y    : " << reportToAgvs.posion().y() << std::endl;
			  std::cout << "         Theta: " << reportToAgvs.posion().theta() << std::endl;
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
  /// The acceptor object used to accept incoming socket connections.
  asio::ip::tcp::acceptor acceptor_;

  /// The data to be sent to each client.
  CamelMessage camelMessage_;
};

} // namespace s11n_example

#endif // !SERVICES_BASIC_COMMANDER_HPP