// expand algorithm test
#include <iostream>
#include <boost/geometry/geometry.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#if GLM_VERSION >= 990
	#include "boost_geometry_adapt_0990.hpp"
#else
	#include "boost_geometry_adapt_0963.hpp"
#endif

using boost::geometry::make;
using boost::geometry::make_inverse;
using boost::geometry::expand;

void vec2_expand_test()
{
	using box2 = boost::geometry::model::box<glm::vec2>;

	box2 b = make_inverse<box2>();
	expand(b, glm::vec2{0,0});
	expand(b, glm::vec2{1,2});
	expand(b, glm::vec2{5,4});
	expand(b, make<box2>(3,3, 5,5));

	std::cout << boost::geometry::dsv(b) << std::endl;
}

void dvec2_expand_test()
{
	using box2 = boost::geometry::model::box<glm::dvec2>;

	box2 b = make_inverse<box2>();
	expand(b, glm::dvec2{0,0});
	expand(b, glm::dvec2{1,2});
	expand(b, glm::dvec2{5,4});
	expand(b, make<box2>(3,3, 5,5));

	std::cout << boost::geometry::dsv(b) << std::endl;
}

void vec3_expand_test()
{
	using box3 = boost::geometry::model::box<glm::vec3>;

	box3 b = make_inverse<box3>();
	expand(b, glm::vec3{0,0,0});
	expand(b, glm::vec3{1,2,0});
	expand(b, glm::vec3{5,4,0});
//	expand(b, make<box3>(3,3,0, 5,5,0));

	std::cout << boost::geometry::dsv(b) << std::endl;
}


int main(int argc, char * argv[])
{
	vec2_expand_test();
	dvec2_expand_test();
	vec3_expand_test();
	return 0;
}
