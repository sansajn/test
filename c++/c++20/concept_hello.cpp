// floating_point concept sample
#include <concepts>

std::floating_point auto multiply(std::floating_point auto a, std::floating_point auto b) {
	return a*b;
}

int main(int argc, char * argv[]) {
	auto f = multiply(10.9, 2.2);
	return 0;
}
