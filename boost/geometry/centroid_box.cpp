// spocita stred geometrie
#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
 
using std::cout;
using boost::geometry::dsv;
using boost::geometry::centroid;

using point = boost::geometry::model::d2::point_xy<float>;
using box = boost::geometry::model::box<point>;

int main(int argc, char * argv[])
{
	box b{point{0,0}, point{1,1}};
	cout << "b:" << dsv(b) << std::endl;
	point c;
	centroid(b, c);
	cout << "c:" << dsv(c) << std::endl;
	return 0;
}
