// prints unifex library version
#include <iostream>
#include <unifex/config.hpp>

using std::cout;

int main(int argc, char * argv[]) {
	cout << "unifex version is " << UNIFEX_VERSION_MAJOR << "." << UNIFEX_VERSION_MINOR << '\n'
		<< "done!\n";
	return 0;
}
