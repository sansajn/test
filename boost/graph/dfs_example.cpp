// ukazka pouzitia dfs algoritmu s referencneho manualu
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/range/irange.hpp>
#include <boost/pending/indirect_cmp.hpp>

using namespace boost;

template <typename TimeMap>
class dfs_time_visitor : public default_dfs_visitor
{
	typedef typename property_traits<TimeMap>::value_type T;

public:
	dfs_time_visitor(TimeMap dmap, TimeMap fmap, T & t)
		: _dtimemap(dmap), _ftimemap(fmap), _time(t)
	{}

	template <typename Vertex, typename Graph>
	void discover_vertex(Vertex u, Graph const & g) const
	{
		put(_dtimemap, u, _time++);
	}

	template <typename Vertex, typename Graph>
	void finish_vertex(Vertex u, Graph const & g) const
	{
		put(_ftimemap, u, _time++);
	}

	TimeMap _dtimemap;
	TimeMap _ftimemap;
	T & _time;
}; 

int main(int argc, char * argv[])
{
	// select the grph type we wish to use
	typedef adjacency_list<vecS, vecS, directedS> graph_t;
	typedef graph_traits<graph_t>::vertices_size_type size_type;

	// set up the vertex names
	enum {u, v, w, x, y, z, N};
	char name[] = {'u', 'v', 'w', 'x', 'y', 'z'};

	// specify the edges in the graph
	typedef std::pair<int, int> E;
	E edge_array[] = {E(u,v), E(u,x), E(x,v), E(y,x), E(v,y), E(w,y), E(w,z), E(z,z)};
	graph_t g(edge_array, edge_array + sizeof(edge_array)/sizeof(E), N);

	// discover time and finish time properties	
	std::vector<size_type> dtime(num_vertices(g));
	std::vector<size_type> ftime(num_vertices(g));
	typedef iterator_property_map<
		std::vector<size_type>::iterator, property_map<graph_t, vertex_index_t>::const_type> time_pm_type;
	time_pm_type dtime_pm(dtime.begin(), get(vertex_index, g));
	time_pm_type ftime_pm(ftime.begin(), get(vertex_index, g));
	size_type t = 0;	

	dfs_time_visitor<time_pm_type> vis(dtime_pm, ftime_pm, t);

	depth_first_search(g, visitor(vis));

	// use std::sort to order the vertices by their discover time
	std::vector<size_type> discover_order(N);
	integer_range<size_type> r(0, N);
	std::copy(r.begin(), r.end(), discover_order.begin());
	std::sort(discover_order.begin(), discover_order.end(), indirect_cmp<time_pm_type, std::less<size_type>>(dtime_pm));
	std::cout << "order of discovery: ";
	for (int i = 0; i < N; ++i)
		std::cout << name[discover_order[i]] << " ";

	std::vector<size_type> finish_order(N);
	std::copy(r.begin(), r.end(), finish_order.begin());
	std::sort(finish_order.begin(), finish_order.end(), indirect_cmp<time_pm_type, std::less<size_type>>(ftime_pm));
	std::cout << std::endl << "order of finish: ";
	for (int i = 0; i < N; ++i)
		std::cout << name[finish_order[i]] << " ";
	std::cout << std::endl;

	return 0;
}
