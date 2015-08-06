// implicitna konverzia na sablonovy typ
#include <typeinfo>
#include <iostream>

using namespace std;

template <typename R>
class foo
{
public:
	operator R() {
		cout << "operator " << typeid(R).name() << "()\n";
		return R();
	}
};


int main(int argc, char * argv[])
{
	foo<int> f;
	int j = f;
	return 0;
}
