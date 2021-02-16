// bullet rotate function helper
#include <bullet/LinearMath/btTransform.h>

//! \param[in] angle angle in radians
btTransform rotate(btScalar angle, btVector3 const & axis)
{
	return btTransform{btQuaternion{axis, angle}};
}


int main(int argc, char * argv[])
{
	btTransform R = rotate(30, btVector3{1,1,1});
	return 0;
}
