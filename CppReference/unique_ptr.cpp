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


// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<Derived> pass_through(std::unique_ptr<Derived> p)
{
	p ->print();
	return p;
}

void test_unique_ptr() {

	std::cout << "unique ownership semantics demo\n";
	{
		auto p = std::make_unique<Derived>(1111); // p is a unique_ptr that owns a D
		auto q = pass_through(std::move(p));
		assert(!p); // now p owns nothing and holds a null pointer
		q->print();   // and q owns the D object
	} // ~D called here

}

//
//TEST(TestMemory,test_unique_ptr) {
//	test_unique_ptr();
//}