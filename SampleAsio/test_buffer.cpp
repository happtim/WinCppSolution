
#include "stdafx.h"


void test_const_buffer();
void test_mutable_buffer();
void test_dynamic_buffer();
void test_buffer() {

	test_const_buffer();
	test_mutable_buffer();
	test_dynamic_buffer();
}

void test_const_buffer(){

	const char buf[128] = "hello const buffer";
	asio::const_buffer const_buffer(buf, 128);
	std::cout <<(char*) const_buffer.data() << std::endl;

	const_buffer = asio::buffer(buf, 128);
	std::cout << (char*)const_buffer .data() << std::endl;
}

void test_mutable_buffer() {

	char buf[128] = "hello mutable buffer";
	asio::mutable_buffer mutable_buffer(buf, 128);
	std::cout << (char*)mutable_buffer.data() << std::endl;
	strcpy(buf, "modify mutable buffer");
	std::cout << (char*)mutable_buffer.data() << std::endl;

	mutable_buffer =  asio::buffer(buf, 128);
	strcpy(buf, "modify mutable buffer2");
	std::cout << (char*) mutable_buffer .data() << std::endl;

}

void test_dynamic_buffer() {
	std::string buf = "string buffer";
	auto dynamic_buffer =  asio::dynamic_buffer(buf);
	std::cout << (const char*) dynamic_buffer.data().data() << std::endl;

	buf = "modify string buffer";
	std::cout << (const char*)dynamic_buffer.data().data() << std::endl;
}