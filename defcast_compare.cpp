#include <string>
#include <iostream>

using std::string;
using std::cout;

template <typename T>
struct foo
{
	operator T() {
		cout << "operator T()\n";
		return T("5");
	}

//	bool operator==(T const & rhs) const {return rhs == "5";}
};

//template <typename T, typename U>
//bool operator==(U const & lhs, foo<T> const & rhs) {return rhs == lhs;}

//template <typename T, typename U>
//bool operator==(foo<T> const & lhs, U const & rhs) {return lhs == rhs;}

template <typename T>
bool operator==(foo<T> const & lhs, T const & rhs) {return lhs == rhs;}

bool operator==(foo<string> const & lhs, string const & rhs) {return lhs == rhs;}

foo<string> getfoo() {return foo<string>();}

int main(int argc, char * argv[])
{
	foo<string> f;
//	cout << (string(f) == "5") << "\n";
//	cout << ("5" == string(f)) << "\n";
//	cout << ("5" == f) << "\n";
//	cout << (f == "5") << "\n";	
	f == string("5");
	f == "5";
	getfoo() == "5";
	return 0;
}

