
#include "stdafx.h"

#include "test_example.h"


void my_task()
{
	std::cout << "my_task"  << std::endl;
}

namespace asio {
	struct Foo {
		void print_sum(int n1, int n2)
		{
			std::cout << n1 + n2 << '\n';
		}

		void print_sum(int n1, int n2 , int n3)
		{
			std::cout << n1 + n2 + n3 << '\n';
		}
		int data = 10;
	};
}

/*
	提供post网io_context增加执行函数, 当run的时候队列执行.
*/
void test_async_func() {
	using namespace std::placeholders;
	asio::io_context io_context;
	//make_work_guard 当没有事件触发时,也不会io_context.run()退出.
	auto worker = asio::make_work_guard(io_context);

	std::thread thread([&io_context]() { io_context.run(); });

	auto f3 = std::bind( (asio::io_context::count_type(asio::io_context::*)()) &asio::io_context::run, &io_context );

	asio::Foo foo;
	auto f4 = std::bind( (void(asio::Foo::*)(int a,int b,int c)) &asio::Foo::print_sum, &foo);

	// Submit a function to the io_context.
	asio::post(io_context, my_task);

	std::string str = "hello async";
	// Submit a lambda object to the io_context.
	asio::post(io_context,
		[str]()
	{
		std::cout << "my_task " << str << std::endl;
	});

	// Run the io_context until it runs out of work.

	std::thread t([&io_context]() { 
		std::this_thread::sleep_for(std::chrono::seconds(2)); 
		asio::post(io_context, my_task);
	});

	// Submit a function to the io_context.
	asio::post(io_context, my_task);

	io_context.stop();
	thread.join();
	

}