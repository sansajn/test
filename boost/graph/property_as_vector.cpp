// vytvorenie property-map s std::vector
#include <vector>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>

using std::vector;
using std::pair;
using namespace boost;

// graph vertices
enum {a, b, c, d, e, f, g, N};
char name[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};

struct custom_visitor : public default_dfs_visitor
{
	template<typename Vertex, typename Graph>
	void finish_vertex(Vertex u, Graph const & g) 
	{
		std::cout << name[u] << ", ";
	}
};

int main(int argc, char * argv[])
{
	// prepare graph data
	using E = pair<int, int>;
	E edge_array[] = {E{a,b}, E{a,c}, E{b,d}, E{e,d}, E{e,f}, E{e,g}};

	// create a graph
	using graph_t = adjacency_list<vecS, vecS, directedS>;
	graph_t grp{edge_array, edge_array + sizeof(edge_array) / sizeof(E), N};

	// release dfs
	using colormap_t = vector<default_color_type>;
	colormap_t cmap{N};

	custom_visitor vis;
	depth_first_search(grp, vis,
		iterator_property_map<colormap_t::iterator, identity_property_map>{cmap.begin()});

	std::cout << "\ndone!\n";
	return 0;
}
