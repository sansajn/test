/* ukazka s dokumentacie 'six degrees of kevin bacon'
http://www.boost.org/doc/libs/1_59_0/libs/graph/doc/kevin_bacon.html */

#include <string>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>

using namespace boost;

template <typename DistanceMap>
class bacon_number_recorder : public default_bfs_visitor
{
public:
	bacon_number_recorder(DistanceMap dist) : d{dist} {}

	template <typename Edge, typename Graph>
	void tree_edge(Edge e, Graph const & g) const
	{
		typename graph_traits<Graph>::vertex_descriptor u = source(e, g), v = target(e, g);
		d[v] = d[u] + 1;
	}

private:
	DistanceMap d;
};

template <typename DistanceMap>
bacon_number_recorder<DistanceMap> record_bacon_number(DistanceMap d)
{
	return bacon_number_recorder<DistanceMap>(d);
}


int main(int argc, char * argv[])
{
	std::ifstream datafile("./kevin-bacon.dat");
	if (!datafile)
	{
		std::cerr << "no ./kevin-bacon.dat file" << std::endl;
		return 1;
	}

	using Graph = adjacency_list<vecS, vecS, undirectedS, 
		property<vertex_name_t, std::string>, property<edge_name_t, std::string>>;
	using Vertex = graph_traits<Graph>::vertex_descriptor;
	using actor_name_map_t = property_map<Graph, vertex_name_t>::type;		
	using movie_name_map_t = property_map<Graph, edge_name_t>::type;
	using NameVertexMap = std::map<std::string, Vertex>;
	
	Graph g;
	actor_name_map_t actor_name = get(vertex_name, g);
	movie_name_map_t connecting_movie = get(edge_name, g);
	NameVertexMap actors;

	for (std::string line; std::getline(datafile, line);)
	{
		char_delimiters_separator<char> sep(false, "", ";");
		tokenizer<> line_toks(line, sep);
		tokenizer<>::iterator i = line_toks.begin();
		
		std::string actors_name = *i++;
		Vertex u, v;
		NameVertexMap::iterator pos;
		bool inserted;
		tie(pos, inserted) = actors.insert(make_pair(actors_name, Vertex()));
		if (inserted)
		{
			u = add_vertex(g);
			actor_name[u] = actors_name;
			pos->second = u;
		}
		else
			u = pos->second;

		std::string movie_name = *i++;

		tie(pos, inserted) = actors.insert(make_pair(*i, Vertex()));
		if (inserted)
		{
			v = add_vertex(g);
			actor_name[v] = *i;
			pos->second = v;
		}
		else
			v = pos->second;

		graph_traits<Graph>::edge_descriptor e;
		tie(e, inserted) = add_edge(u, v, g);
		if (inserted)
			connecting_movie[e] = movie_name;
	}

	std::vector<int> bacon_number(num_vertices(g));

	Vertex src = actors["Kevin Bacon"];
	bacon_number[src] = 0;

	breadth_first_search(g, src, visitor(record_bacon_number(&bacon_number[0])));

	graph_traits<Graph>::vertex_iterator i, end;
	for (tie(i, end) = vertices(g); i != end; ++i)
		std::cout << actor_name[*i] << " has a Bacon number of " << bacon_number[*i] << std::endl;

	return 0;
}
