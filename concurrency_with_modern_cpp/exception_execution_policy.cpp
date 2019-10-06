// exceptions with execution policies (note: execution policies not implementeg in gcc 7.4.0)
#include <algorithm>
//#include <execution>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char * argv[])
{
	vector<int> v{1, 2, 3, 4, 5};

	try {
		for_each(v.begin(), v.end(),
			[](int){throw runtime_error{"without execution policy"};}
		);
	}
	catch (runtime_error const & e) {
		cout << e.what() << endl;
	}

/*
	try {
		for_each(execution::seq, v.begin(), v.end(),
			[](int){throw runtime_error{"with execution policy"};}
		);
	}
	catch(runtime_error const & e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "catch-all exceptions" << endl;
	}
*/

	return 0;
}
