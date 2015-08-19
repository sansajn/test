// typ deskriptorou pre urcity adjacency_list graf

#include <typeinfo>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

int main(int argc, char * argv[])
{
	using graph_t = adjacency_list<vecS, vecS, directedS>;
	std::cout << "vertex_descriptor:" << typeid(graph_traits<graph_t>::vertex_descriptor).name() << std::endl;
	return 0;
}