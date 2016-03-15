// vypocet hranic
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

using std::cout;
using boost::geometry::expand;
using boost::geometry::make_inverse;
using boost::geometry::dsv;

using point = boost::geometry::model::d2::point_xy<float>;
using box = boost::geometry::model::box<point>;


int main(int argc, char * argv[])
{
	box b = make_inverse<box>();
	cout << dsv(b) << std::endl;
	
	expand(b, point(481122779, 171118302));
	cout << dsv(b) << std::endl;
	
	expand(b, point(481124962, 171118441));
	cout << dsv(b) << std::endl;
	
	return 0;
}