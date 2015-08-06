#include <iostream>

struct foo {
	void introduce() {std::cout << "i'm foo\n";}
};


class goo
{
public:
	foo foo() const {return f;}

private:
	::foo f;
};


int main(int argc, char * argv[])
{
	goo g;
	foo f = g.foo();
	f.introduce();
	return 0;
}

