#include <memory>
#include <iostream>
#include <boost/any.hpp>

using std::cout;
using std::shared_ptr;

class foo
{
public:
	foo() : _id(id_gen++) {cout << "foo::foo():" << _id << "\n";}
	~foo() {cout << "foo::~foo():" << _id << "\n";}
	void introduce() {cout << "foo:" << _id << "\n";}

private:
	int _id;
	static int id_gen;
};

int foo::id_gen = 1;

void shared()
{
	cout << "shared()\n";

	shared_ptr<foo> r;

	{
		boost::any a;
		{
			shared_ptr<foo> p(new foo());
			a = p;
		}
		shared_ptr<foo> * result = boost::any_cast<shared_ptr<foo>>(&a);
		if (result)
			r = *result;
	}

	if (r)
		r->introduce();

	cout << "shared() done.\n";
}

int main(int argc, char * argv[])
{
	boost::any a;
	a = foo();
	foo * f = boost::any_cast<foo>(&a);
	if (f)
		f->introduce();

	boost::any b(*f);

	shared();

	return 0;
}
