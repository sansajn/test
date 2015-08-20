 // pouzitie vektora ako grafu
#include <vector>
#include <list>
#include <boost/graph/vector_as_graph.hpp>
#include <boost/graph/graph_utility.hpp>

using boost::add_edge;

int main(int argc, char * argv[])
{
	enum {r, s, t, u, v, w, x, y, N};
	char name[] = "rstuvwxy";
	using graph = std::vector<std::list<int>>;
	graph g(N);
	add_edge(r, v, g);  // vyzaduje mutable graph
	add_edge(s, r, g);
	add_edge(s, w, g);
	add_edge(t, x, g);
	add_edge(u, t, g);
	add_edge(w, t, g);
	add_edge(w, x, g);
	add_edge(x, y, g);
	add_edge(y, u, g);
	boost::print_graph(g, name);
	return 0;
}
