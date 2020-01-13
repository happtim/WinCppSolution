
#include <iostream>

#include "test_example.h"

using asio::ip::tcp;

std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}
/*
以流的方式进行io
*/
void test_iostream_client() {

	try
	{
		tcp::iostream s("127.0.0.1", "9601");

		if (!s)
		{
			std::cout << "Unable to connect: " << s.error().message() << std::endl;
			return;
		}

		for (;;) {
			s << "hello world" << std::endl << make_daytime_string();
			Sleep(1000);
		}

		/*
		hello world
		Sun Dec 29 17:54:37 2019
		hello world
		Sun Dec 29 17:54:38 2019
		hello world
		Sun Dec 29 17:54:39 2019
		hello world
		Sun Dec 29 17:54:40 2019
		hello world
		Sun Dec 29 17:54:41 2019
		*/

		s.close();
	}
	catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}


}