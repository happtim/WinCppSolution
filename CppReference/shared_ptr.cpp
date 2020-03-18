#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include "gtest/gtest.h"

struct Base
{
	Base() { std::cout << "  Base::Base()\n"; }
	// Note: non-virtual destructor is OK here
	~Base() { std::cout << "  Base::~Base()\n"; }
};

struct Derived : public Base
{
	Derived() { std::cout << "  Derived::Derived()\n"; }
	Derived(int in) :a(in) { std::cout << " Derived::Derived(int)\n"; }
	~Derived() { std::cout << "  Derived::~Derived()\n"; }
	void print() const { std::cout << " a = " << a << std::endl; }
	int a;
};

void thr(std::shared_ptr<Base> p)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::shared_ptr<Base> lp = p; // thread-safe, even though the
								  // shared use_count is incremented
	{
		static std::mutex io_mutex;
		std::lock_guard<std::mutex> lk(io_mutex);
		std::cout << "local pointer in a thread:\n"
			<< "  lp.get() = " << lp.get()
			<< ", lp.use_count() = " << lp.use_count() << '\n';
	}
}

void test_shared_ptr() {

	std::cout << " Shared_ptr Initialize" << std::endl;
	std::shared_ptr<Derived> d = std::make_shared<Derived>(1);
	d->print();
	(*d).print();
	d.get()->print();

	std::shared_ptr<Base> p = std::make_shared<Derived>();

	std::cout << "Created a shared Derived (as a pointer to Base)\n"
		<< "  p.get() = " << p.get()
		<< ", p.use_count() = " << p.use_count() << '\n';
	std::thread t1(thr, p), t2(thr, p), t3(thr, p);
	p.reset(); // release ownership from main
	std::cout << "Shared ownership between 3 threads and released\n"
		<< "ownership from main:\n"
		<< "  p.get() = " << p.get()
		<< ", p.use_count() = " << p.use_count() << '\n';
	t1.join(); t2.join(); t3.join();
	std::cout << "All threads completed, the last one deleted Derived\n";

}


//TEST(TestMemory,ShardPtr) {
	//test_shared_ptr();
//}