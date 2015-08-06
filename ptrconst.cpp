// clang ptrconst.cpp -fsyntax-only
struct foo {
	void boundary() {}
};

struct goo {
	foo * _f;
	void boundary() const {
		_f->boundary();
	}
};

int main()
{
	goo g;
	g.boundary();
	return 0;
}
