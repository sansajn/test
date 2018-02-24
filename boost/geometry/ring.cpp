#include <vector>
#include <iostream>
#include <boost/geometry.hpp>

using std::vector;
using std::cout;
namespace bg = boost::geometry;

using point = bg::model::point<double, 2, bg::cs::cartesian>;
using ring = bg::model::ring<point>;

int main(int argc, char * argv[])
{
	// vytvorenie ringu po jednom bode
	ring r;
	bg::append(r, point{0.0, 0.0});
	bg::append(r, point{0.0, 5.0});
	bg::append(r, point{5.0, 5.0});
	bg::append(r, point{5.0, 0.0});
	bg::append(r, point{0.0, 0.0});

	cout << "area(r)=" << bg::area(r) << "\n";

	// vytvorenie ringu z vektora
	vector<point> verts{
		{0.0, 0.0}, {0.0, 5.0}, {5.0, 5.0}, {5.0, 0.0}, {0.0, 0.0}};

	ring r2;
	bg::append(r2, verts);

	cout << "area(r2)=" << bg::area(r2) << "\n";

	// iteracia ringom
	cout << "vertices(r2)=";
	for (point const & v : r2)
		cout << bg::dsv(v) << ", ";
	cout << "\n";

	cout << "vertices(r2)=";
	for (auto it = std::begin(r2); it != std::end(r2); ++it)
		cout << bg::dsv(*it) << ", ";
	cout << "\n";

	cout << "vertices(r2)=";
	for (size_t i = 0; i < r2.size(); ++i)
		cout << bg::dsv(r2[i]) << ", ";
	cout << "\n";

	return 0;
}
