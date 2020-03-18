#include <gtest/gtest.h>


class String {
public:

	String(std::string p):str(p) {
		std::cout << str << " " << &str<< " String::String(std::string) \n";
	}

	String(const String& s) {
		std::cout << s.str <<" " << &str << " String::String(const String&) \n";
		this->str = s.str;
	}

	~String() {
		std::cout << str<<" " << &str << " String::~String() \n";
	}

	friend std::ostream& operator<<(std::ostream &out,const String& s) {
		out << s.str;
		return out;
	}

private:
	std::string str;

};

void test_move() {

	String foo( "foo-string");
	String bar( "bar-string");
	std::vector<String> myvector;

	std::cout << " initialize complish " << std::endl;
	myvector.push_back(foo);                    // copies
	myvector.push_back(std::move(bar));         // moves
	std::cout << "foo bar push_back end" << std::endl;

	std::cout << "myvector contains:";
	int i = 0;
	for (String& x : myvector) 
		std::cout << " " << i++ <<  " " << x;
	std::cout << '\n';
}


//TEST(TestUtility, Move) {
	//test_move();
//}