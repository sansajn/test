// iterating set sample
#include <set>
#include <iostream>
using std::set, std::cout;

int main(int argc, char * argv[]) {
	set s{1,2,3,4,5};
	for (auto const & e : s)
		cout << e << ", ";

	cout << "\ndone!\n";
	return 0;
}
