/* par a referencia */
#include <iostream>
#include <utility>
#include <functional>
#include <vector>
using std::pair;
using std::cout;
using std::reference_wrapper;
using std::vector;


int main()
{
	int k = 10, v = 15;
//	pair<int const, int &> p(k, v);  // v pohode

//	pair<int const, int &> p2;  // compiler error, default konstruktor


//	pair<int const, reference_wrapper<int>> p3;


	vector<int> nums;

	vector<reference_wrapper<int>> nums_refs(10);
	

	return 0;
}
