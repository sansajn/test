// iota nad take views sample
#include <ranges>
#include <numeric>
#include <vector>
#include <iostream>

using std::vector;
using std::views::iota, std::views::iota, std::views::take;
using std::cout;

int main(int argc, char * argv[])
{
	vector<int> v, v2;
	for (int i : iota(0, 10)) v.push_back(i);
	for (int i: iota(0) | take(10)) v2.push_back(i);

	cout << std::boolalpha;
	cout << "v == v2: " << (v == v2) << "\n";

	for (int i: v) cout << i << " ";
	cout << "\n";

	cout << "done!\n";
	return 0;
}
