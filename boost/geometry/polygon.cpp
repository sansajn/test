#include <vector>
#include <iostream>
#include <boost/geometry.hpp>

using std::vector;
using std::cout;
namespace bg = boost::geometry;

using point = bg::model::point<double, 2, bg::cs::cartesian>;
using polygon = bg::model::polygon<point>;
using box = bg::model::box<point>;


int main(int argc, char * argv[])
{
	polygon p;
	
	// vkladanie po jednom
	bg::append(p.outer(), point{0.0, 0.0});
	bg::append(p.outer(), point{0.0, 5.0});
	bg::append(p.outer(), point{5.0, 5.0});
	bg::append(p.outer(), point{5.0, 0.0});
	bg::append(p.outer(), point{0.0, 0.0});
	
	p.inners().resize(1);
	bg::append(p.inners()[0], point{1.0, 1.0});
	bg::append(p.inners()[0], point{4.0, 1.0});
	bg::append(p.inners()[0], point{4.0, 4.0});
	bg::append(p.inners()[0], point{1.0, 4.0});
	bg::append(p.inners()[0], point{1.0, 1.0});
	
	cout << "area(p)=" << bg::area(p) << "\n";
	
	// vytvorenie s kontajneru
	vector<point> exterior{
		{0.0, 0.0}, {0.0, 5.0}, {5.0, 5.0}, {5.0, 0.0}, {0.0, 0.0}};
	
	vector<point> interior{
		{1.0, 1.0}, {4.0, 1.0}, {4.0, 4.0}, {1.0, 4.0}, {1.0, 1.0}};
		
	polygon p2;
	bg::append(p2.outer(), exterior);
	p2.inners().resize(1);
	bg::append(p2.inners()[0], interior);
	
	cout << "area(p2)=" << bg::area(p2) << "\n";
	
	box b;
	bg::envelope(p2, b);
	
	cout << "envelope(p2)=" << bg::dsv(b) << "\n";
	
	return 0;
}
