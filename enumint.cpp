/* (?) Je možné pri volaní funkcie rozpoznať enum a int ? */
#include <iostream>
using std::cout;

struct foo
{
	enum prop_e {max_vertex_distance};
	void property(int v) {cout << "property(int)\n";}
	void property(prop_e prop) {cout << "property(prop_e)\n";}
};


int main()
{
	int v = 0;
	foo f;
	f.property(v);  // volá property(int)
	f.property(foo::max_vertex_distance);  // volá property(prop_e)
	f.property(0);  // property(int)
	f.property(10);  // property(int)
	return 0;
}

