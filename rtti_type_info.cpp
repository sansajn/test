#include <typeinfo>
#include <iostream>

using namespace std;

struct foo {};
struct goo : public foo {};

int main(int argc, char * argv[])
{
	goo g;
	cout << typeid(g).name() << "\n";

	cout << std::boolalpha;
	cout << (typeid(foo) == typeid(goo)) << "\n";  // false
	cout << (typeid(foo *) == typeid(goo *)) << "\n";  // false
	cout << typeid(foo *).before(typeid(goo *)) << "\n";  // true
	cout << typeid(goo *).before(typeid(foo *)) << "\n";  // false

	return 0;
}

