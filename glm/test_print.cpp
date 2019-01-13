#include <string>
#include <iostream>
#include <glm/matrix.hpp>
#include "glmprint.hpp"

using std::string;
using std::cout;
using glm::mat4;
using glm::mat3;

int main(int argc, char * argv[])
{
	mat4 M = mat4{
		11, 21, 31, 41,  // column 1
		12, 22, 32, 42,
		13, 23, 33, 43,
		14, 24, 34, 44};

	cout << with_label{"M", M};

	mat3 M3 = mat3{M};
	cout << with_label{"M3", M3};

	cout << "done!\n";

	return 0;
}
