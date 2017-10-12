// hratky s priestormi
namespace xxx {

struct mesh {};

struct foo {
	void mesh() {}
	xxx::mesh m;
};

struct goo {
	void mesh() {}
	struct mesh m;
};

}

int main()
{
	xxx::foo f;
	f.mesh();
	return 0;
}
