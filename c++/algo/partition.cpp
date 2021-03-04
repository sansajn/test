/*! partition algorithm sample: move all even elements to the beginning and odd 
elements to the end of array */
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>

using std::partition, std::copy, std::vector, std::ostream_iterator, std::cout; 

int main(int argc, char * argv[])
{
	vector v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
	partition(begin(v), end(v), [](auto e){
		return e % 2 == 0;
	});

	copy(begin(v), end(v), ostream_iterator<int>{cout, ", "});  // expected {0, 2, 8, 4, 6, 5, 7, 3, 9, 1}
	cout << "\n";
	
	return 0;
}
