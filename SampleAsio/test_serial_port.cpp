#include "stdafx.h"


class serial_port{
public:
	serial_port(asio::io_context& io_context, std::string device)
		: sp(io_context,device)
	{
		sp.set_option(asio::serial_port::baud_rate(9600));
		sp.set_option(asio::serial_port::flow_control());
		sp.set_option(asio::serial_port::parity());
		sp.set_option(asio::serial_port::stop_bits());
		sp.set_option(asio::serial_port::character_size(8));

		start_read();
	}

	void wirte(std::string str) {

		sp.async_write_some(asio::buffer(str.c_str(),str.length()),
			[](const asio::error_code& ec, // Result of operation.
				std::size_t bytes_transferred) {

			if (!ec) {
				//do nothing
			}
		});
	
	}

private:
	void start_read() {
		
		sp.async_read_some(asio::buffer(data_,1024),
			[this](const asio::error_code &ec, std::size_t bytes_transferred) {
			
			if (!ec) {
				std::string str(data_, bytes_transferred);
				std::cout << " receive data : " << str << std::endl;

				start_read();
			}
		});
	}

	asio::serial_port sp;
	char data_[1024];

};

void test_serial_port() {

	try {
		asio::io_context io_context;
		serial_port sp(io_context, "COM1");
		sp.wirte("hello asio");
		io_context.run();
	}
	catch (std::exception &ex) {
		std::cerr << "Exception Error: " << ex.what() << std::endl;
	}
}