
#include "stdafx.h"
#include "test_example.h"


void test_timer_block();
void test_timer_async();



void test_timer_block() {

	asio::io_context io_context;
	asio::steady_timer timer(io_context);

	std::time_t result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << result << " seconds since the test_timer_block begin\n";

	timer.expires_after(std::chrono::seconds(5));

	timer.wait();

	result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << result << " seconds since the test_timer_block end\n";
}

void handler(const asio::error_code &ec) {

	if (!ec) {
		std::time_t result = std::time(nullptr);
		std::cout << std::asctime(std::localtime(&result)) << result << " seconds since the test_timer_async end\n";
	}

}

void  test_timer_async() {

	std::time_t result = std::time(nullptr);
	std::cout << std::asctime(std::localtime(&result)) << result << " seconds since the test_timer_async begin\n";

	asio::io_context io_context;
	asio::steady_timer timer(io_context,
		std::chrono::steady_clock::now() + std::chrono::seconds(5));
	
	// Start an asynchronous wait.
	timer.async_wait(handler);
	io_context.run();

	std::this_thread::sleep_for(std::chrono::seconds(15));
}

/*
测试定时器触发
*/

void test_timer() {
	/*
	Timer 有两个状态,一个是expires ,一个是 not expires.
	当调用wait时,如果expires就直接调用wait的回调函数
	*/

	//test_timer_block();
	test_timer_async();

}