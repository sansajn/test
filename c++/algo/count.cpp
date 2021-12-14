// count, count_if algorithm sample
#include <algorithm>
#include <vector>
#include <cassert>
using std::count, std::count_if, std::vector;

int main(int argc, char * argv[]) {
	vector v{1,2,1,4,1};
	auto equal_one = count(begin(v), end(v), 1);
	assert(equal_one == 3);

	auto greater_three = count_if(begin(v), end(v),
		[](auto const & v){
			return v > 3;});
	assert(greater_three == 1);

	return 0;
}
