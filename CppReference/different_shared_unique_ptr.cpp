#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include "gtest/gtest.h"

namespace test_different_shared_unique_ptr {


struct Base
{
	Base() { std::cout << "  Base::Base()\n"; }
	// Note: non-virtual destructor is OK here
	~Base() { std::cout << "  Base::~Base()\n"; }
};

struct Derived : public Base
{
	Derived(int in) :a(in) { std::cout << " Derived::Derived(int)\n"; }
	Derived() { std::cout << "  Derived::Derived()\n"; }
	Derived(const  Derived & other) { std::cout << "Derived::Derived(const & Derived other) \n"; a = other.a; };
	~Derived() { std::cout << "  Derived::~Derived()\n"; }
	void print() const { std::cout << " a = " << a  <<  std::endl; }
	int a = 1;
};


// a function consuming a unique_ptr can take it by value or by rvalue reference
std::unique_ptr<Derived> pass_through(std::unique_ptr<Derived> p)
{
	p->print();
	return p;
}

std::shared_ptr<Derived> pass_through(std::shared_ptr<Derived> p)
{
	p->print();
	std::cout << "use count: " << p.use_count() << std::endl;
	return p;
}

void test_different_shared_unique_ptr() {

	std::cout << "unique ownership semantics demo\n";
	{
		auto p = std::make_unique<Derived>(1111); // p is a unique_ptr that owns a D
		auto q = pass_through(std::move(p));
		assert(!p); // now p owns nothing and holds a null pointer
		q->print();   // and q owns the D object
	} // ~D called here

	std::cout << "shared ownership semantics demo \n";
	{
		auto p = std::make_shared<Derived>(1111); // p is a unique_ptr that owns a D
		auto q = pass_through(std::move(p));
		assert(!p); // now p owns nothing and holds a null pointer
		q->print();   // and q owns the D object
	} // ~D called here

}


TEST(TestMemory,test_different_shared_unique_ptr) {
	test_different_shared_unique_ptr();
}

}