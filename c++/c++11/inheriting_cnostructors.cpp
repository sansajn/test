// inheriting constructort
struct foo
{
	foo(int) {}
};

struct goo : foo
{
	using foo::foo;  // goo dedí foo:foo
};

int main(int argc, char * argv[])
{
/*	
	goo g;      // error, nie je definovaný goo::goo()
*/	
	goo g(12);  // ok
	return 0;
}
