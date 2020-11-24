// sample algorithm sample

#include <algorithm>
#include <vector>
#include <random>
#include <iostream>

using std::vector, std::sample, std::cout, std::mt19937, std::random_device;

int main(int argc, char * argv[])
{
	vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
		out;

	sample(begin(v), end(v), back_inserter(out), 3, mt19937{random_device{}()});

	cout << "sampled values: ";
	for (auto const & e : out)
		cout << e << ", ";
	cout << "\n";

	cout << "done!\n";

	return 0;
}
