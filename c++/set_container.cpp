// triedenie pomocou set kontajneru
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>

using std::set;
using std::copy;
using std::ostream_iterator;
using std::cout;

int main(int argc, char * argv[])
{
	set<int> s{4, 5, 0, 1, 3, 2};
	copy(s.begin(), s.end(), ostream_iterator<int>{cout, ", "});
	cout << "\ndone!\n";
	return 0;
}
