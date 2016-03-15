// vypocet ohranicenia
#include <iostream>
#include <list>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

using boost::geometry::expand;

using point = boost::geometry::model::d2::point_xy<int>;
using box = boost::geometry::model::box<point>;

int main(int argc, char * argv[])
{
	box b = boost::geometry::make_inverse<box>();  // ((2147483647, 2147483647), (-2147483648, -2147483648))
	std::cout << boost::geometry::dsv(b) << std::endl;

	expand(b, point{0,0});  // ((0,0), (0,0))
	expand(b, point{1,2});
	expand(b, point{5,4});
	expand(b, boost::geometry::make<box>(3,3, 5,5));

	std::cout << boost::geometry::dsv(b) << std::endl;

	return 0;
}
