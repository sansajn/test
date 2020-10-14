// find similar files in specified directory sample with std::filesystem
#include <filesystem>
#include <vector>
#include <iostream>

using std::cout;
using std::vector;
using namespace std::filesystem;

/*! Find similar files in directory.
\param[in] directory Directory to search.
\param[in] filename Filename without an extension. */
vector<path> find_similar_files(path const & directory, path const & filename)
{
	vector<path> result;
	for (auto const & e : directory_iterator{directory})
	{
		if (e.path().stem() == filename)
			result.push_back(e.path());
	}
	return result;
}

int main(int argc, char * argv[])
{
	path const filename = "test", 
		directory = "/home/adam";

	vector<path> similar_files = find_similar_files(directory, filename);
	for (path const & p : similar_files)
		cout << p << "\n";

	cout << "done!\n";

	return 0;
}