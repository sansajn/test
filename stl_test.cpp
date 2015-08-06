#include <vector>
#include <list>
#include <map>
#include <string>
#include <set>
#include <unordered_map>
using namespace::std;

int main(int argc, char * argv[])
{
	vector<int> vi{1, 2, 3, 4, 5};
	list<int> li{5, 4, 3, 2, 1};
	
	map<int, int> mi;
	mi[10] = 1;
	mi[20] = 2;
	mi[30] = 3;

	string s{"hello"};
	
	vector<string> vs{"hello", "peter", "how", "are", "you"};

	set<int> si{5, 4, 3, 2, 1};

	multimap<int, int> mmi{
		make_pair(10, 1),
		make_pair(10, 101),
		make_pair(20, 2),
		make_pair(30, 3)};

	unordered_map<int, int> umi;
	umi[10] = 1;
	umi[20] = 2;
	umi[30] = 3;
	
	return 0;
}
