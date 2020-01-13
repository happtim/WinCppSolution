
#include "stdafx.h"

#include "test_example.h"


void my_task()
{
	std::cout << "my_task"  << std::endl;
}


/*
	�ṩpost��io_context����ִ�к���, ��run��ʱ�����ִ��.
*/
void test_async_func() {
	using namespace std::placeholders;
	asio::io_context io_context;
	//make_work_guard ��û���¼�����ʱ,Ҳ����io_context.run()�˳�.
	auto worker = asio::make_work_guard(io_context);

	std::thread thread([&io_context]() { io_context.run(); });

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