// all_of, any_of, none_of algorithms sample
#include <algorithm>
#include <vector>
#include <cassert>
using std::all_of, std::any_of, std::none_of, std::vector;

int main(int argc, char * argv[]) {
	// all_of
	vector const v{2,4,8,2,6};
	bool multiple_of_two = all_of(begin(v), end(v), 
		[](auto const & v){
			return v % 2 == 0;});
	assert(multiple_of_two);  // all elements are multiple of 2

	// any_of
	bool greater_than_six = any_of(begin(v), end(v), 
		[](auto const & v){
			return v > 6;});
	assert(greater_than_six);  // there is 8 there
		
	// none_of
	bool none_less_than_zero = none_of(begin(v), end(v), 
		[](auto const & v){
			return v < 0;});
	assert(none_less_than_zero);
		
	return 0;
}