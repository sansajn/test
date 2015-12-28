// test vystupneho iteratora vyrovnavacej pamati (ostreambuf_iterator)
#include <iterator>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	ostreambuf_iterator<char> it{cout};
	string s{"nazdar, streambuf\n"};
	copy(s.begin(), s.end(), it);
	return 0;
}
