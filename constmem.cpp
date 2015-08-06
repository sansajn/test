// clang -fsyntax-only -std=c++0x constmem.cpp
#include <memory>
#include <vector>

struct foo {
	void boundary() {};
};

struct goo {
	goo() {
		_f.push_back(std::shared_ptr<foo>(new foo()));
	}

	void boundary() const {_f[0]->boundary();}
	std::vector<std::shared_ptr<foo>> _f;
};

int main()
{
	goo g;
	g.boundary();
	return 0;
}
