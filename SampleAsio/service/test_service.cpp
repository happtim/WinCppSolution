//
// daytime_client.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "../stdafx.h"

#include "logger.hpp"
#include "commander.hpp"

using asio::ip::tcp;
using namespace std::placeholders;

char read_buffer[1024];

void read_handler(const asio::error_code& e,
	std::size_t bytes_transferred, tcp::socket* s)
{
	if (!e)
	{
		std::string str(read_buffer, bytes_transferred);
		std::cout << "socket receive : " << str << std::endl;
		
		//ִ����������cpuִ����io��ͬ.��Ҫ�첽����.
		services::commander commander(s->get_executor().context());
		commander.execute_command(str);

		s->async_read_some(asio::buffer(read_buffer),
			std::bind(read_handler, _1,_2, s));
	}
	else
	{
		//��д�ļ� ʼ��io��,��Ҫ�첽����.
		services::logger logger(s->get_executor().context(), "read_handler");

		std::string msg = "Read error: ";
		msg += e.message();
		logger.log(msg);
	}
}

void connect_handler(const asio::error_code& e, tcp::socket* s)
{
	services::logger logger(s->get_executor().context(), "connect_handler");

	if (!e)
	{
		logger.log("Connection established");

		s->async_read_some(asio::buffer(read_buffer),
			std::bind(read_handler, _1,_2, s));
	}
	else
	{
		std::string msg = "Unable to establish connection: ";
		msg += e.message();
		logger.log(msg);
	}
}

/*
�ò��� ģ����һ���¼��첽�����ģʽ
socket ������������Ϣ
commander ˳��ִ�� ÿ��ִ��sleep 5s;
*/
void test_service()
{
	try
	{

		asio::io_context io_context;

		// Set the name of the file that all logger instances will use.
		services::logger logger(io_context, "");
		logger.use_file("log.txt");

		// Resolve the address corresponding to the given host.
		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "9601");

		// Start an asynchronous connect.
		tcp::socket socket(io_context);

		asio::async_connect(socket, endpoints,
			std::bind(connect_handler, _1, &socket));

		// Run the io_context until all operations have finished.
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}