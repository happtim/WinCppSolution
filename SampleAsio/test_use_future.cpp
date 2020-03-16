
#include <iostream>
#include <memory>
#include <utility>

#include <asio.hpp>
#include <gtest/gtest.h>
#include <asio/use_future.hpp>

using asio::ip::tcp;

class client_future
{
public:
	client_future(asio::io_context& io_context, const tcp::resolver::results_type& endpoints) :
		io_context_(io_context), socket_(io_context)
	{
		do_connect(endpoints);
	}

	void write(const std::string& msg)
	{
		socket_.async_write_some(asio::buffer(msg),
			[](const asio::error_code& ec, size_t t/*bytes_transferred*/) {

			if (!ec) {
				std::cout << "send " << t << " bytes" << std::endl;
			}
		});

	}

	void close()
	{
		asio::post(io_context_, [this]() { socket_.close(); });
	}

private:

	void do_connect(const tcp::resolver::results_type& endpoints)
	{
		std::future<asio::ip::tcp::endpoint> aa = asio::async_connect(socket_, endpoints, asio::use_future);

		asio::ip::tcp::endpoint ep =  aa.get();

		int a = 1;
		//asio::async_connect(socket_, endpoints,
		//	[this](std::error_code ec, tcp::endpoint)
		//{
		//	if (!ec)
		//	{
		//		do_read();
		//	}
		//});
	}

	void do_read() {

		socket_.async_read_some(asio::buffer(data_, max_length),
			[this](const asio::error_code& ec, size_t length) {

			if (!ec) {
				std::string receive(data_, length);
				std::cout << receive << std::endl;

				do_read();
			}

		});
	}

	asio::io_context& io_context_;
	tcp::socket socket_;
	enum { max_length = 1024 };
	char data_[max_length];
};

/*
发什么回什么的客户端
*/
void test_async_echo_client_use_future() {

	try {

		asio::io_context io_context;
		auto work = asio::make_work_guard(io_context);
		std::thread t([&io_context]() {io_context.run(); });

		tcp::resolver resolver(io_context);
		auto endpoints = resolver.resolve("127.0.0.1", "9602");
		client_future c(io_context, endpoints);

		//io_context.run();


		
		std::string send = "123456789";
		c.write(send);
		send = "abcfef";
		c.write(send);

		c.close();
		t.join();

	}
	catch (std::exception & e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

}

TEST(TestASIO, test_async_echo_client_use_future) {
	test_async_echo_client_use_future();
}