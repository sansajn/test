#include <tuple>
#include <utility>
#include <vector>

using std::pair;
using std::tuple;
using std::vector;

int main(int argc, char * argv[])
{
	pair p{10, 15.14};  // types int, double are deduced
	tuple t{10.11, 5, 12.13f};  // types double, int, float are deduced
	vector v{1, 2, 3, 4};  //  vector<int> deduced
	
	return 0;
}
