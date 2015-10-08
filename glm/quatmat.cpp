// quaternion ako rotacia
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

using std::ostream;
using std::cout;
using glm::vec3;
using glm::quat;
using glm::angleAxis;
using glm::radians;
using glm::conjugate;
using glm::length;

ostream & operator<<(ostream & o, quat const & q)
{
	o << "[" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << "]";
	return o;
}

ostream & operator<<(ostream & o, vec3 const & v)
{
	o << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return o;
}

int main(int argc, char * argv[])
{
	quat p = angleAxis(radians(30.0f), vec3{0,0,1});
	quat q = angleAxis(radians(60.0f), vec3{0,0,1});
	quat r = q*p;  // spojenie p a q transformacie (90 okolo z)
	quat s = conjugate(p)*r;  // 90 - 30 okolo z
	
	cout << "p = " << p << ", " << length(p) << "\n";
	cout << "q = " << q << ", " << length(q) << "\n";
	cout << "r = " << r << ", " << length(r) << "\n";
	cout << "s = " << s << ", " << length(s) << "\n";

	vec3 v = vec3{1,0,0};
	vec3 u = r*v;  // transformacia (nie, nasobenie)
	cout << u << "\n";

	quat t = quat{1,2,3,4};
	cout << "length(" << t << ") : " << length(t) << "\n";

	return 0;
}
