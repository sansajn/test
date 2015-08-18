// spocita dlzku kompilacie suborou s 'projektu' (BGL tutorial)
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/property_iter_range.hpp>

namespace std {

template<typename T>
std::istream & operator>>(std::istream & in, std::pair<T, T> & p)
{
	in >> p.first >> p.second;
	return in;
}

}  // std

namespace boost
{
	enum vertex_compile_cost_t {vertex_compile_cost};
	BOOST_INSTALL_PROPERTY(vertex, compile_cost);
}

using namespace boost;

using file_dep_graph2 = 
	adjacency_list<
		listS,       // store out-edges of each vertex in a std::list
		listS,       // store vertex set in a std::vector
		directedS,   // directed graph
		// vertex properties
		property<vertex_name_t, std::string,
		property<vertex_compile_cost_t, float,
		property<vertex_distance_t, float,
		property<vertex_color_t, default_color_type>>>>,
		// an edge property
		property<edge_weight_t, float> >;

using vertex_t = graph_traits<file_dep_graph2>::vertex_descriptor;
using edge_t = graph_traits<file_dep_graph2>::edge_descriptor;


int main(int argc, char * argv[])
{
	std::ifstream file_in("makefile_dependencies.dat");
	using size_type = graph_traits<file_dep_graph2>::vertices_size_type;
	size_type n_vertices;
	file_in >> n_vertices;  // read in number of vertices
	std::istream_iterator<std::pair<size_type, size_type>> input_begin(file_in), input_end;

	file_dep_graph2 g{input_begin, input_end, n_vertices};

	using name_map_t = property_map<file_dep_graph2, vertex_name_t>::type;
	using compile_cost_map_t = property_map<file_dep_graph2, vertex_compile_cost_t>::type;
	using distance_map_t = property_map<file_dep_graph2, vertex_distance_t>::type;
	using color_map_t = property_map<file_dep_graph2, vertex_color_t>::type;

	name_map_t name_map = get(vertex_name, g);
	compile_cost_map_t compile_cost_map = get(vertex_compile_cost, g);
	distance_map_t distance_map = get(vertex_distance, g);
	color_map_t color_map = get(vertex_color, g);

	// loads vertex names from file
	std::ifstream name_in("makefile_target_names.dat");
	std::ifstream compile_cost_in("target_compile_costs.dat");
	graph_traits<file_dep_graph2>::vertex_iterator vi, vi_end;
	for (tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
	{
		name_in >> name_map[*vi];
		compile_cost_in >> compile_cost_map[*vi];
	}

	graph_property_iter_range<file_dep_graph2, vertex_compile_cost_t>::iterator ci, ci_end;
	tie(ci, ci_end) = get_property_iter_range(g, vertex_compile_cost);
	std::cout << "total (sequential) compile time: " 
		<< std::accumulate(ci, ci_end, 0.0) << std::endl;

	return 0;
}