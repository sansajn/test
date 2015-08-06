#include <iomanip>
#include <iostream>
#include <glm/gtc/quaternion.hpp>

using std::cout;

void print_vector(glm::vec3 const & v);

int main(int argc, char * argv[])
{
	glm::vec3 v = glm::vec3(1,0,0);
	glm::quat q = glm::angleAxis(glm::radians(45.0f), glm::vec3(0,1,0));
	glm::vec3 v_ = q*v;  // transformacia (nie nsobenie)
	print_vector(v_);
	return 0;
}

void print_vector(glm::vec3 const & v)
{
	cout << "\n  ";
	for (int i = 0; i < 3; ++i)
		cout << std::setw(9) << std::setprecision(5) << v[i] << " ";
	cout << "\n";
}
