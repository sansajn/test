// použitie unique_ptr
#include <memory>
#include <iostream>

using std::unique_ptr;
using std::cout;

unique_ptr<int[]> make_sequence(int n)
{
	unique_ptr<int[]> p{new int[n]};
	for (int i = 0; i < n; ++i)
		p[i] = i;
	return p;
}

int main(int argc, char * argv[])
{
	std::unique_ptr<int> u1{new int(15)};
	std::unique_ptr<int[]> u2{make_sequence(10)};
	std::unique_ptr<int[]> u3 = make_sequence(10);

	cout << "u1:" << *u1 << "\n";
	
	cout << "u2:[";
	for (int i = 0; i < 10; ++i)
		cout << u2[i] << ", ";
	cout << "]\n";

	return 0;
}
