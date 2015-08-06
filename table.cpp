#include <string>
using std::string;
#include <utility>
using std::pair;
#include <iostream>
using std::cout;

struct foo {
	template <typename T>
	foo & operator>>(pair<string, T &> value) {
		value.second = 1010;
		return *this;
	}
};

template <typename T>
inline pair<string, T &> table(string key, T & value)
{
	return pair<string, T &>(key, value);
}

int main(int argc, char * argv[])
{
	foo f;
	int a = 0;
	f >> table("a", a);
	cout << "a:" << a << "\n";
	return 0;
}
