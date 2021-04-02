// parallel transform sample
#include <algorithm>
#include <execution>
#include <vector>
#include <iterator>
#include <iostream>

using std::vector, std::transform, std::ostream_iterator, std::cout, std::copy;
namespace execution = std::execution;

int main(int argc, char * argv[]) {
	vector in{1, 2, 3, 4, 5};
	
	int out[5];
	transform(execution::par, begin(in), end(in), out, [](int n){
		return n+1;
	});

	copy(out, out+5, ostream_iterator<int>{cout, ","});

	cout << "\ndone!\n";
	return 0;
}
