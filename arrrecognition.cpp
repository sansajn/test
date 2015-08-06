// rozpoznanie pola
#include <iostream>
#include <utility>

using std::cout;
using std::make_pair;

template <typename T>
std::pair<T *, unsigned> uniform_array(T * a, unsigned n)
{
	return std::pair<T *, unsigned>(a, n);
}

class uniform
{
public:

	template <typename T>
	void operator=(T const & v) {
		cout << "uniform::operator=(T const &)\n";
	}

	void operator=(float f[]) {  // to iste ako pre float *
		cout << "uniform::operator=(float [])\n";
	}

	// verzia pre polia pomocou dvojice
	template <typename T>
	void operator=(std::pair<T *, unsigned> const & arr) {
		cout << "uniform::operator=(pair<T *, int> const &)\n";
	}
};

int main(int argc, char * argv[])
{
	float widths[5];
	float * p;
	float & r = widths[0];
	uniform u;
	u = widths;  // operator=(float [])
	u = p;       // operator=(float [])
	u = r;       // operator=(T const &)
	u = uniform_array(widths, 5);
	return 0;
}

