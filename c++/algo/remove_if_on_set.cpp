// remove_if algorithm can not be used with std::set
#include <algorithm>
#include <set>
#include <iterator>
#include <iostream>
#include <cassert>
using std::remove_if, std::erase, std::copy, std::set, 
	std::ostream_iterator, std::cout;

int main(int argc, char * argv[]) {
	set s{1,9,2,3,7,4,5,8,6};

	// not possible with *error: assignment of read-only location* error
	//remove_if(begin(s), end(s), [](auto const & e){return e > 5;});
	
	return 0;
}
