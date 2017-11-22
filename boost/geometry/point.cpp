#include <iostream>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/arithmetic/arithmetic.hpp>

namespace bg = boost::geometry;

using std::cout;
using bg::get;
using bg::set;
using bg::add_point;
using bg::subtract_point;
using point_xy = bg::model::d2::point_xy<double>;


int main(int argc, char * argv[])
{
	point_xy p{1.0, 2.0};
	set<0>(p, 3.0);
	cout << "p = (" << get<0>(p) << ", " << get<1>(p) << ")\n";
	
	// aritmetics
	add_point(p, point_xy{2.0, 3.0});
	subtract_point(p, point_xy{1.0, 2.0});
	
	return 0;
}
