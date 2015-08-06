struct foo {};

class goo : public foo {};

int main(int argc, char * argv[])
{
	foo * b = new goo;
	return 0;
}
