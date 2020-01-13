// SampleAsio.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "test_example.h"

int main()
{
	try {
		//test_iostream_client();

		//test_chat_client();

		//test_echo_server();

		//test_heartbeat();

		//test_timer();

		//test_buffer();

		//test_async_func();

		test_service();

		std::cout << "Hello World!\n";
	}
	catch (std::exception ex) {
		std::cerr << ex.what() << std::endl;
	}


}

