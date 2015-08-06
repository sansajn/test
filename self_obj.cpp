#include <iostream>
using std::cout;

class foo
{
public:
	static foo & reference() {
		if (!p) {
			p = new foo;
			cout << "object constructd\n";
		}
		return *p;
	}

	~foo() {
		if (p) {
			delete p;
			cout << "object deleted\n";
		}
	}

	void invoke() const {
		cout << "foo::invoke()\n";
	}

private:
	static foo * p;
};

foo * foo::p = NULL;


int main()
{
	foo & f = foo::reference();
	f.invoke();
	return 0;
}
