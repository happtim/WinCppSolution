
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <nngpp/nngpp.h>
#include <nngpp/protocol/req0.h>
#include <nngpp/protocol/rep0.h>

void test_req_rep() {

	try {
		// create a socket for the rep protocol
		nng::socket rep_sock = nng::rep::open();

		// rep starts listening using the tcp transport
		rep_sock.listen("tcp://localhost:8000");

		// create a socket for the req protocol
		nng::socket req_sock = nng::req::open();

		// req dials and establishes a connection
		req_sock.dial("tcp://localhost:8000");

		// req sends "hello" including the null terminator
		req_sock.send("hello");

		// rep receives a message
		nng::buffer rep_buf = rep_sock.recv();

		std::string rep_string((char*)rep_buf.data(), rep_buf.size());

		std::cout << "rep_sock receive :" << rep_string << std::endl;

		// check the content
		if (rep_buf == "hello") {
			// rep sends "world" in response
			rep_sock.send("world");
		}

		// req receives "world"
		nng::buffer req_buf = req_sock.recv();
		std::string req_string((char*)req_buf.data(), req_buf.size());
		std::cout << "req_sock receive :" << req_string << std::endl;


	}
	catch (const nng::exception& e) {
		// who() is the name of the nng function that produced the error
		// what() is a description of the error code
		printf("%s: %s\n", e.who(), e.what());
	}
}


//TEST(TestUtility, test_req_rep) {
//	test_req_rep();
//}