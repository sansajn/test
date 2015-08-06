// downcast, upcast konverzie std::shared_ptr
#include <memory>
#include <iostream>
using namespace std;

struct base 
{
	virtual void introduce() const {cout << "base::introduce()" << endl;}
};

struct derived : public base 
{
	void introduce() const override {cout << "derived::introduce()" << endl;}
	void extension() const {cout << "derived::extension()" << endl;}
};


int main(int argc, char * argv[])
{
	shared_ptr<derived> p = make_shared<derived>();
	shared_ptr<base> r = p;  // upcast
	r->introduce();
	shared_ptr<derived> s = std::dynamic_pointer_cast<derived>(r);  // downcast
	if (s)
		s->extension();
	return 0;
}

