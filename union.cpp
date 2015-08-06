class A 
{
public:
	A() {}	
};

class B {};

union U
{
	A a;
	B b;
};

int main(int argc, char * argv[])
{
	U u;	
	return 0;
}

