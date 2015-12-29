// bazove vektory transformacie (rotacie)
#include <iostream>
#include <glm/matrix.hpp>
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glmprint.hpp"

using namespace std;
using namespace glm;

vec3 right(mat3 const & r)
{
	return r[0];
}

vec3 up(mat3 const & r)
{
	return r[1];
}

vec3 forward(mat3 const & r)
{
	return r[2];
}

vec3 right(quat const & r)
{
	return mat3_cast(r)[0];
}

vec3 up(quat const & r)
{
	return mat3_cast(r)[1];
}

vec3 forward(quat const & r)
{
	return normalize(axis(r));
}

int main(int argc, char * argv[])
{
	mat4 R = rotate(mat4{1}, radians(90.0f), vec3{0,0,1});  // rotacia okolo z o 90
	cout << "R=" << R << "\n";
	quat r = angleAxis(radians(90.0f), vec3{0,0,1});
	cout << "r=" << r << "\n";
	cout << "normalize(r(x,y,z))=" << normalize(vec3{r.x, r.y, r.z}) << "\n";

	vec4 p{1,0,0,0};
	cout << "p=" << p << "\n";
	cout << "R*p=" << R*p << "\n";
	cout << "r*p=" << r*p << "\n";

	cout << "right:" << right(mat3{R}) << ", " << right(r) << "\n";
	cout << "up:" << up(mat3{R}) << ", " << up(r) << "\n";
	cout << "forward:" << forward(mat3{R}) << ", " << forward(r) << "\n";

	return 0;
}
