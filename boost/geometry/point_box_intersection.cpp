// Intersection of point and box sample.
#include <iostream>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/io/dsv/write.hpp>  // for value output

using std::cout;
namespace bg = boost::geometry;

int main(int argc, char * argv[]) {
	using point = bg::model::point<double, 2, bg::cs::cartesian>;
	using box = bg::model::box<point>;

	point pt = {2,3};
	point min_corner = {1,1};
	point max_corner = {4,4};

	box area = {min_corner, max_corner};

	if (bg::intersects(pt, area))
		cout << "The point " << bg::dsv(pt) << " intersects with the box " << bg::dsv(area) << "!\n"; 
	else
		cout << "The point " << bg::dsv(pt) << " does not intersects with the box " << bg::dsv(area) << "!\n"; 

	return 0;
}
