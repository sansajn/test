#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


struct foo
{
	int val;
	foo() {}
	foo(int v) : val(v) {}
};

bool ValCompare(foo const & a, foo const & b) {return a.val < b.val;}

int main(int argc, char * argv[])
{	
	vector<foo> d;
	for (int i = 12; i < 100; ++i)
		d.push_back(foo(i));
	random_shuffle(d.begin(), d.end());
	
	auto it = min_element(d.begin(), d.end(), ValCompare);
	cout << "min value is " << it->val << "\n";

	return 0;
}
