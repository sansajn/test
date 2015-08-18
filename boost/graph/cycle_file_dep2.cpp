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

template<typename Visitor>
void dfs_v1(file_dep_graph const & g, vertex_t u, default_color_type * color, Visitor vis)
{
	color[u] = gray_color;
	vis.discover_vertex(u, g);
	graph_traits<file_dep_graph>::out_edge_iterator ei, ei_end;
	for (tie(ei, ei_end) = out_edges(u, g); ei != ei_end; ++ei)
	{
		if (color[target(*ei, g)] == white_color)
		{
			vis.tree_edge(*ei, g);
			dfs_v1(g, target(*ei, g), color, vis);
		}
		else if (color[target(*ei, g)] == gray_color)
			vis.back_edge(*ei, g);
		else
			vis.forward_or_cross_edge(*ei, g);
	}

	color[u] = black_color;
	vis.finish_vertex(u, g);
}

template<typename Visitor>
void generic_dfs_v1(file_dep_graph const & g, Visitor vis)
{
	std::vector<default_color_type> color(num_vertices(g), white_color);
	graph_traits<file_dep_graph>::vertex_iterator vi, vi_end;
	for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
		if (color[*vi] == white_color)
			dfs_v1(g, *vi, &color[0], vis);
}

struct default_dfs_visitor 
{
	template<typename V, typename G>
	void discover_vertex(V, G const & g) {}

	template<typename E, typename G>
	void tree_edge(E, G const & g) {}

	template<typename E, typename G>
	void back_edge(E, G const & g) {}

	template<typename E, typename G>
	void forward_or_cross_edge(E, G const & g) {}

	template<typename V, typename G>
	void finish_vertex(V, G const & g) {}
};

struct cycle_detector : public default_dfs_visitor
{
	cycle_detector(bool & cycle) : has_cycle{cycle} {}

	void back_edge(edge_t, file_dep_graph const & g)
	{
		has_cycle = true;
	}

	bool & has_cycle;
};

bool has_cycle(file_dep_graph const & g)
{
	bool cycle = false;
	cycle_detector vis{cycle};
	generic_dfs_v1(g, vis);
	return cycle;
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

	if (has_cycle(g))
		std::cout << "cycle detected" << std::endl;
	else
		std::cout << "cycle free" << std::endl;

	return 0;
}