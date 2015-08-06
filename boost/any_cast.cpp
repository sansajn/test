#include <memory>
#include <iostream>
#include <boost/any.hpp>

using namespace std;

struct foo {};
struct goo : public foo {};

int main(int argc, char * argv[])
{
	shared_ptr<goo> g = make_shared<goo>();
	boost::any a = g;
	cout << a.type().name() << "\n";

	try {
		shared_ptr<foo> f = boost::any_cast<shared_ptr<foo>>(a);
	}
	catch (boost::bad_any_cast &) {
		cout << "cast failed\n";
	}

	return 0;
}

