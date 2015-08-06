#include <map>
#include <memory>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	map<shared_ptr<int>, int> m;
	shared_ptr<int> a(new int(0));
	shared_ptr<int> b(new int(1));
	shared_ptr<int> c(new int(2));
	m[a] = 9;
	m[b] = 8;
	m[c] = 7;
	
	int & i = *b;

	if (b == &(*b))
		cout << "b == b\n";

/*	
	auto it = m.find(&i);
	if (it != m.end())
		cout << it->first << " : " << it->second << "\n";
*/

	return 0;
}

