#include <iostream>
#include <boost/geometry/geometries/point_xy.hpp>

using std::cout;
using boost::geometry::get;
using boost::geometry::set;
using point_xy = boost::geometry::model::d2::point_xy<double>;

int main(int argc, char * argv[])
{
	point_xy p{1.0, 2.0};
	set<0>(p, 3.0);
	cout << "p = (" << get<0>(p) << ", " << get<1>(p) << ")\n";

	return 0;
}
