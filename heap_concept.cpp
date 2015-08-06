template <typename A, typename B>
struct heap {
	void introduce() {cout << "heap\n";}
};

template <typename A, template<class, class> class B, typename C>
class foo {
private:
	B<A, C> _h;
};


int main(int argc, char * argv[])
{
	foo<int, heap, int> f;
	return 0;
}


