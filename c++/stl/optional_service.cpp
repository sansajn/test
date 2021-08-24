// using optional to provide optional service instance sample
#include <optional>
#include <string>
#include <iostream>
#include <cassert>
using std::optional, std::string, std::cout;

struct service {
	service(int n) {}
	void say_hello() const {cout << "hello!\n";}
};

int main(int argc, char * argv[]) {
	optional<service> s{service{42}};
	assert(s);
	if (s)
		s->say_hello();

	optional<service> null;
	assert(!null);
	if (null)
		null->say_hello();  // will not be called

	return 0;
}
