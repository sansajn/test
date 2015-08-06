enum foo {one, two, three};

struct goo {
	enum hoo {four, five, six};
};

void func(goo::hoo type) {}

int main(int argc, char * argv[])
{
	foo type;
	type = two;

	goo::hoo type2;
	type2 = goo::five;

	func(type2);

	return 0;
}

