// open nested namespaces sample

namespace foo::goo {
	
struct hoo {};
	
}   // foo::goo

int main(int argc, char * argv[])
{
	foo::goo::hoo h;
	return 0;
}
