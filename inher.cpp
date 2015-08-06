/* dedičnosť a referencie */
#include <vector>
#include <iostream>
using std::vector;
using std::cout;

struct f {
	virtual void fce() {cout << "f::fce()\n";}
};

struct g : public f {
	void fce() {cout << "g::fce()\n";}
};

void foo(f & a);
void goo(vector<f> & v);

int main(int argc, char * argv[])
{
	g a;
	foo(a);

	vector<f> v;
	v.push_back(g());
	v.push_back(f());
	v.push_back(g());

	goo(v);

	return 0;
}

void foo(f & a)
{
	a.fce();
}

void goo(vector<f> & v)
{
	for (auto e : v)
		e.fce();
}
