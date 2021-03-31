/* memory sanitizer test (available only for clang)
compile as `clang++ memory.cpp -Wall -Werror -g -fsanitize=memory` */
#include <iostream>
using std::cout;

void set_val(bool & b, int const val) {
	if (val > 1)
		b = false;
}

int main(int argc, char * argv[]) {
	bool b;
	set_val(b, argc);
	if (b)  // <--- bug there, b can be still uninitialized
		cout << "value set\n";
	return 0;
}