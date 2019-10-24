// we can call member function as free-standing one using std::function
#include <functional>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char * argv[])
{
	string s = "A small pond";
	function<bool (string)> f;
	f = &string::empty;

	cout << f(s) << "\n";

	return 0;
}
