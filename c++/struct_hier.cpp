struct foo 
{
	int i;
};

struct goo : public foo
{
	goo() {
		i = 15;
	}
};

int main(int argc, char * argv[]) 
{
	goo g;
	g.i = 10;
	return 0;
}
