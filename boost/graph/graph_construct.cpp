using file_dep_graph = 
	adjacency_list<
		listS,       // store out-edges of each vertex in a std::list
		vecS,        // store vertex set in a std::vector
		directedS>;  // directed graph