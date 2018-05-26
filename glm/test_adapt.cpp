// expand algorithm test
#include <iostream>
#include <boost/geometry/geometry.hpp>
#include <glm/vec2.hpp>
#include "boost_geometry_adapt_0990.hpp"

using boost::geometry::make;
using boost::geometry::make_inverse;
using boost::geometry::expand;

using box2 = boost::geometry::model::box<glm::vec2>;


int main(int argc, char * argv[])
{
	box2 b = make_inverse<box2>();
	expand(b, glm::vec2{0,0});
	expand(b, glm::vec2{1,2});
	expand(b, glm::vec2{5,4});
	expand(b, make<box2>(3,3, 5,5));

	std::cout << boost::geometry::dsv(b) << std::endl;

	return 0;
}
