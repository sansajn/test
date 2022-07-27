// basic fmt library sample
#include <iostream>
#include <fmt/format.h>
using std::cout;
using fmt::format;

int main() {
	cout << format("Hello from {} library!", "fmt") << '\n';
	return 0;
}
