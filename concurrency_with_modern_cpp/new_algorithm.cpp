// new algorithms
#include <algorithm>
#include <execution>
#include <numeric>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char * argv[])
{
	vector<int> data1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	for_each_n(execution::par, data1.begin(), 5, [](int & arg){arg *= arg;});

	cout << "for_each_n: ";
	for (auto v : data1)
		cout << v << " ";
	cout << "\n\n";

	// exclusive_scan and inclusive_scan
	vector<int> data2{1, 2, 3, 4, 5, 6, 7, 8, 9};
	exclusive_scan(execution::par, data2.begin(), data2.end(), data2.begin(), 1,
		[](int fir, int sec){return fir * sec;});

	cout << "exclusive_scan: ";
	for (auto v : data2)
		cout << v << " ";
	cout << endl;

	vector<int> data3{1, 2, 3, 4, 5, 6, 7, 8, 9};
	inclusive_scan(execution::par, data3.begin(), data3.end(), data3.begin(),
		[](int fir, int sec){return fir * sec;}, 1);

	cout << "inclusive_scan: ";
	for (auto v : data3)
		cout << v << " ";
	cout << "\n\n";

	// transform_exclusive_scan and transform_inclusive_scan
	vector<int> data4{1, 2, 4, 5, 6, 7, 8, 9};
	vector<int> data5(data4.size());
	transform_exclusive_scan(execution::par, data4.begin(), data4.end(),
		data5.begin(), 0,
		[](int fir, int sec){return fir + sec;},
		[](int arg){return arg *= arg;});

	cout << "transform_exclusive_scan: ";
	for (auto v : data5)
		cout << v << " ";
	cout << endl;

	vector<string> data6{"Only", "for", "testing", "purpose"};
	vector<string> data7(data6.size());
	transform_inclusive_scan(execution::par, data6.begin(), data6.end(),
		data7.begin(),
		[](auto fir, auto sec){return fir + sec;},
		[](auto s){return s.length();}, 0);

	cout << "transform_inclusive_scan: ";
	for (auto v : data7)
		cout << v << " ";
	cout << "\n\n";

	// reduce and transform_reduce
	vector<string> data8{"Only", "for", "testing", "purpose"};
	string res = reduce(execution::par, data8.begin() + 1, data8.end(), data8[0],
		[](auto fir, auto sec){return fir + ":" + sec;});

	cout << "reduce: " << res << endl;

	size_t res2 = transform_reduce(execution::par, data8.begin(), data8.end(),
		0, [](size_t a, size_t b){return a+b;},
		[](string s){return s.length();});

	cout << "transform_reduce: " << res2 << endl;

	return 0;
}
