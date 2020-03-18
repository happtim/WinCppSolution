
#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include <nngpp/nngpp.h>
#include <nngpp/protocol/pub0.h>
#include <nngpp/protocol/sub0.h>

void test_pub_sub() {

	try {
		// create a socket for the rep protocol
		nng::socket pub_sock = nng::pub::open();

		// rep starts listening using the tcp transport
		pub_sock.listen("tcp://localhost:8000");

		// create a socket for the req protocol
		nng::socket sub_sock = nng::sub::open();

		// req dials and establishes a connection
		sub_sock.dial("tcp://localhost:8000");

		// req sends "hello" including the null terminator
		pub_sock.send("hello");

		// rep receives a message
		nng::buffer rep_buf = sub_sock.recv();

		std::string rep_string((char*)rep_buf.data(), rep_buf.size());

		std::cout << "rep_sock receive :" << rep_string << std::endl;

		// check the content
		if (rep_buf == "hello") {
			// rep sends "world" in response
			pub_sock.send("world");
		}

		// req receives "world"
		nng::buffer req_buf = sub_sock.recv();
		std::string req_string((char*)req_buf.data(), req_buf.size());
		std::cout << "req_sock receive :" << req_string << std::endl;


	}
	catch (const nng::exception& e) {
		// who() is the name of the nng function that produced the error
		// what() is a description of the error code
		printf("%s: %s\n", e.who(), e.what());
	}
}


TEST(TestUtility, test_pub_sub) {
	test_pub_sub();
}