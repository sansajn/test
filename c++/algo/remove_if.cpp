// remove_if algorithm and erase-remove idiom sample
#include <algorithm>
#include <vector>
#include <iterator>
#include <iostream>
#include <cassert>
using std::remove_if, std::copy, std::vector, 
	std::ostream_iterator, std::cout;

int main(int argc, char * argv[]) {
	vector v1{1,9,2,3,7,4,5,8,6};

	cout << "v looks: ";
	copy(begin(v1), end(v1), ostream_iterator<int>{cout, ", "});

	remove_if(begin(v1), end(v1), [](auto const & e){
		return e > 5;});
	
	// nothing removed yet, elements not satisfied the condition are 
	// arranged at the beggining of the collection
	assert(size(v1) == 9);
	cout << "\nafter remove_if called v looks: ";
	copy(begin(v1), end(v1), ostream_iterator<int>{cout, ", "});

	// to remove element ne need to tie remove_if with erase, this way
	vector v2{1,9,2,3,7,4,5,8,6};

	v2.erase(
		remove_if(begin(v2), end(v2), [](auto const & e){
			return e > 5;}),
		end(v2)
	);

	assert(size(v2) == 5);

	cout << "\nafter erase-remove_if v looks: ";
	copy(begin(v2), end(v2), ostream_iterator<int>{cout, ", "});

	cout << "\ndone!\n";
	return 0;
}
