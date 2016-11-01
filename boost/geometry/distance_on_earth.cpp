// spocita vzdialenost medzi dvoma bodmi na zemi
#include <iostream>
#include <boost/geometry.hpp>

typedef boost::geometry::model::point<
	double, 
	2, 
	boost::geometry::cs::spherical_equatorial<
		boost::geometry::degree>
> spherical_point;

int main(int argc, char * argv[])
{
	spherical_point paris{2.35, 48.86};
	spherical_point amsterdam{4.9, 52.37};
	double const earth_radius = 6371;  // in km
	std::cout << "distance: " << distance(amsterdam, paris) * earth_radius << " km" << std::endl;

	return 0;
}
