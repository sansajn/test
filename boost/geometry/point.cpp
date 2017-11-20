#include <boost/geometry/geometries/point_xy.hpp>

namespace bg = boost::geometry;

int main(int argc, char * argv[])
{
	using point_xy = bg::model::d2::point_xy<double>;

	point_xy p{1.0, 2.0};

	return 0;
}
