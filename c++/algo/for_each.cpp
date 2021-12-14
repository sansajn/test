// for_each and for_each_n sample
#include <algorithm>
#include <vector>
#include <iostream>
using std::for_each, std::for_each_n, std::vector, std::cout;

int main(int argc, char * argv[]) {
	vector v{1,2,3,4,5};

	for_each(begin(v), end(v), 
		[](auto const & v){
			cout << v << " ";});
	cout << "\n";

	for_each_n(begin(v), 3, 
		[](auto const & v){
			cout << v << " ";});
	cout << "\n";

	return 0;
}