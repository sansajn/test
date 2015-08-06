#include <glm/glm.hpp>
#include "print_matrix.hpp"

int main(int argc, char * argv[])
{
	/*[1,2,3; 4,5,6; 7,8,9]*/
	glm::mat3 a(
		1, 4, 7,
		2, 5, 8,
		3, 6, 9);

	/*[9,8,7; 6,5,4; 3,2,1]*/
	glm::mat3 b(
		9, 6, 3,
		8, 5, 2,
		7, 4, 1);

	glm::mat3 ab = a*b;

/* expected	
    30    24    18
    84    69    54
   138   114    90
*/	

	print_matrix("a*b", ab);

	glm::mat4 c(
		 1,  2,  3,  4,   // column 1
		 5,  6,  7,  8, 
		 9, 10, 11, 12,
		13, 14, 15, 16);  // column 4

	glm::vec4 v = c[3];
	std::cout << "v=(" << v[0] << " " << v[1] << " " << v[2] << " " << v[3] 
		<< ")\n";

	return 0;
}
