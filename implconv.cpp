// implicitna konverzia na sablonovy typ
#include <typeinfo>
#include <string>
#include <iostream>

using namespace std;

int ivar = 10;
double dvar = 10.10;
std::string svar = "hello!";

template <typename T>
class fake_ref
{
public:
	fake_ref(T * adr) : _buf(adr) {}

	operator T() 
	{
		cout << "operator " << typeid(T).name() << "()\n";
		return *_buf;
	}

	fake_ref & operator=(T const & rhs) {*_buf = rhs; return *this;}

private:
	T * _buf;
};  // fake_ref

template <typename V>
fake_ref<V> find();

template <> fake_ref<int> find() {return fake_ref<int>(&ivar);}

int main(int argc, char * argv[])
{
	int i = find<int>();
	cout << "i=" << i << "\n";
	
	find<int>() = 101;
	cout << "ivar=" << ivar << "\n";

	cout << "sizeof(fake_ref<int>)=" << sizeof(fake_ref<int>) << "\n";
	return 0;
}
