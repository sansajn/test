// shows how to use envelope algorithm witha a string of points
#include <iostream>
#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/io/dsv/write.hpp>

namespace bg = boost::geometry;

using std::cout;
using bg::return_envelope,
	bg::append,
	bg::dsv;

using point = bg::model::d2::point_xy<double>;
using linestring = bg::model::linestring<point>;
using box = bg::model::box<point>;

int main(int argc, char * argv[])
{
	linestring road = {{0,0}, {1,1}, {2,2}, {3,3}};
	append(road, point{4,4});  // we can also use append to add point to a line-string

	box bb = return_envelope<box>(road);
	cout << "road bounding box is " << dsv(bb) << "\n"
		<< "done!\n";

	return 0;
}
