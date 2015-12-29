// zlucovanie rotacie a posunutia
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glmprint.hpp"

using std::cout;
using namespace glm;

int main(int argc, char * argv[])
{
	mat4 M1 = translate(mat4{1}, vec3{1,0,0}) * rotate(mat4{1}, radians(30.0f), vec3{0,0,1});
	cout << "M1=" << M1 << "\n";
	mat4 M2 = rotate(mat4{1}, radians(30.0f), vec3{0,0,1}) * translate(mat4{1}, vec3{1,0,0});
	cout << "M2=" << M2 << "\n";
	return 0;
}

