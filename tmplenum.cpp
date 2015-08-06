#include <iostream>
using std::cout;

struct foo
{
	enum A {A1, A2, A3};
	enum B {B1 = A3+1, B2, B3};
	int state;

	template <typename T>
	bool set_state(T t);

	template <>
	bool set_state<A>(A a) 
	{
		cout << "foo::set_state<A>()\n";
		state = a; 
		return true;
	}

	template <>
	bool set_state<B>(B b) 
	{
		cout << "foo::set_state<B>()\n";
		state = b; 
		return false;
	}
};


int main(int argc, char * argv[])
{
	foo f;
	f.set_state(foo::A1);
	f.set_state(foo::B3);	

	return 0;
}


