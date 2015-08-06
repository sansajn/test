struct foo
{
	void g() {p = this;}
	void h() const {p = const_cast<foo *>(this);}
	static foo * p;
};

foo * foo::p = nullptr;


int main(int argc, char * argv[])
{
	foo f;
	f.g();
	return 0;
}
