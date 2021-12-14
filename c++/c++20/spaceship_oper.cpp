// spaceship operator test
#include <compare>

struct data {
	int i, j;
	auto operator<=>(data const & rhs) const = default;
};

int main() {
	data d1{1, 3}, 
		d2{3, 2};

	d1 == d2;
	d1 < d2;
	d1 <= d2;

	return 0;
}
