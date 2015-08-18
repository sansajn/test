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

bool has_cycle_dfs(file_dep_graph const & g, vertex_t u, default_color_type * color)
{
	color[u] = gray_color;
	graph_traits<file_dep_graph>::adjacency_iterator vi, vi_end;
	for (tie(vi, vi_end) = adjacent_vertices(u, g); vi != vi_end; ++vi)
	{
		if (color[*vi] == white_color)
		{
			if (has_cycle_dfs(g, *vi, color))
				return true;  // cycle detected
		}
		else if (color[*vi] == gray_color)
			return true;
	}

	color[u] = black_color;
	return false;
}

bool has_cycle(file_dep_graph const & g)
{
	std::vector<default_color_type> color(num_vertices(g), white_color);
	graph_traits<file_dep_graph>::vertex_iterator vi, vi_end;
	for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
	{
		if (color[*vi] == white_color)
		{
			if (has_cycle_dfs(g, *vi, &color[0]))
				return true;
		}
	}
	return false;
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
