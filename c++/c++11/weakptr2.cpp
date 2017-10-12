// riesenie cyklickeho memory leaku (weakptr.cpp) pomocou std::weak_ptr
#include <memory>
#include <iostream>
using namespace std;

class B;
class A {public: shared_ptr<B> b; ~A() {cout << "~A()" << endl;}};
class B {public: weak_ptr<A> a; ~B() {cout << "~B()" << endl;}};

void foo()
{
	shared_ptr<A> x(new A);
	x->b = make_shared<B>();
	x->b->a = x;
	cout << x.use_count() << endl;

	shared_ptr<A> y = x->b->a.lock();
}

int main(int argc, char * argv[])
{
	foo();
	return 0;
}

