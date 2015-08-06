// detekci konštantného typu
#include <iostream>

using std::cout;

template <typename Cond, typename T, typename F>
struct if_const
{
	using type = F;
};

template <typename Cond, typename T, typename F>
struct if_const<Cond const, T, F>
{
	using type = T;
};

struct foo
{
	using reference = int &;
	using const_reference = int const &;

	foo() : _value(101) {}

	reference value() {return _value;}
	const_reference value() const {return _value;}

	int _value;
};

template <typename F>
void selector_test(F & f)
{
	typename if_const<F, typename F::const_reference, typename F::reference>::type v = f.value();
	cout << "value:" << v << std::endl;
}

void const_call(foo const & f)
{
	selector_test(f);
}

int main(int argc, char * argv[])
{
	foo f;
	const_call(f);
	return 0;
}
