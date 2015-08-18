// implementacia topo-sort funkcie (BGL tutorial)
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

using file_dep_graph = 
	adjacency_list<
		listS,       // store out-edges of each vertex in a std::list
		vecS,        // store vertex set in a std::vector
		directedS>;  // directed graph

using vertex_t = graph_traits<file_dep_graph>::vertex_descriptor;
using edge_t = graph_traits<file_dep_graph>::edge_descriptor;

void topo_sort_dfs(file_dep_graph const & g, vertex_t u, vertex_t *& topo_order, int * mark)
{
	mark[u] = 1;  // 1 means visited, 0 means not yet visited
	graph_traits<file_dep_graph>::adjacency_iterator vi, vi_end;
	for (tie(vi, vi_end) = adjacent_vertices(u, g); vi != vi_end; ++vi)
		if (mark[*vi] == 0)
			topo_sort_dfs(g, *vi, topo_order, mark);
	*--topo_order = u;
}

void topo_sort(file_dep_graph const & g, vertex_t * topo_order)
{
	std::vector<int> mark(num_vertices(g), 0);
	graph_traits<file_dep_graph>::vertex_iterator vi, vi_end;
	for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
		if (mark[*vi] == 0)
			topo_sort_dfs(g, *vi, topo_order, &mark[0]);
}

namespace std {

template<typename T>
std::istream & operator>>(std::istream & in, std::pair<T, T> & p)
{
	in >> p.first >> p.second;
	return in;
}

}  // std

int main(int argc, char * argv[])
{
	std::ifstream file_in("makefile_dependencies.dat");
	using size_type = graph_traits<file_dep_graph>::vertices_size_type;
	size_type n_vertices;
	file_in >> n_vertices;  // read in number of vertices
	std::istream_iterator<std::pair<size_type, size_type>> input_begin(file_in), input_end;

	file_dep_graph g{input_begin, input_end, n_vertices};

	// loads vertex names from file
	std::vector<std::string> name(num_vertices(g));
	std::ifstream name_in("makefile_target_names.dat");
	graph_traits<file_dep_graph>::vertex_iterator vi, vi_end;
	for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
		name_in >> name[*vi];

	std::vector<vertex_t> order(num_vertices(g));
	topo_sort(g, &order[0] + num_vertices(g));  // do order zapisuje odzadu
	for (int i = 0; i < num_vertices(g); ++i)
		std::cout << name[order[i]] << std::endl;

	return 0;
}
