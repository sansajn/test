#include <tuple>
#include <iostream>

using std::tuple;
using std::cout;

int main(int argc, char * argv[])
{
	tuple<int, int, int> a{1, 2, 3};
	tuple<int, int, int> b{1, 2, 3};
	tuple<int, int, int> c{4, 5, 6};

	if (a == b)
		cout << "a == b\n";
	else
		cout << "a != b\n";

	if (a != c)
		cout << "a != c\n";
	else
		cout << "a == c\n";

	return 0;
}
