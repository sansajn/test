#include <iostream>
#include "vec3.hpp"

using std::cout;

int main(int argc, char * argv[]) {
	constexpr vec3 u{1,1,0},
		v{2,2,0},
		k=u+v;

	cout << "u+v=(" << k.x << "," << k.y << "," << k.z << ")\n";

	return 0;
}
