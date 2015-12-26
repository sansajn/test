#include <list>
#include <iostream>
#include <cassert>

using namespace std;

int main(int argc, char * argv[])
{
	list<int> lst;
	for (int i = 0; i < 10; ++i)
		lst.push_back(i);
	
	for (auto it = lst.begin(); it != lst.end(); ++it)
		if (*it % 2)  // zmaz neparne
			it = lst.erase(it);
		
	assert(lst.size() == 5);
	
	for (auto & e : lst)
		cout << e << ",";
	cout << "\n";
	
	return 0;
}
