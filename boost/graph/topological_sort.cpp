// topologicke usporadanie prvkou grafu
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

using std::vector;
using std::pair;
using std::for_each;
using std::cout;
using namespace boost;

// graph vertices
enum {a, b, c, d, e, f, g, N};
char name[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};

int main(int argc, char * argv[])
{
	using E = pair<int, int>;
	E edge_array[] = {E{a,b}, E{a,c}, E{b,d}, E{e,d}, E{e,f}, E{e,g}};

	using graph = adjacency_list<vecS, vecS, directedS>;
	graph grp{edge_array, edge_array + sizeof(edge_array) / sizeof(E), N};

	using vertex = graph_traits<graph>::vertex_descriptor;
	vector<vertex> order;
	topological_sort(grp, back_inserter(order));

	for_each(order.rbegin(), order.rend(), [](auto & u){cout << name[u] << ",";});

	std::cout << "\ndone!\n";
	return 0;
}
