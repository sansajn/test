// weakly directory first sort implementation
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>

using std::sort;
using std::vector;
using std::string;
using std::cout;
using boost::filesystem::path;

bool has_dotdot(path const & p)
{
	static path dotdot{".."};
	for (auto e : p)
		if (e == dotdot)
			return true;
	return false;
}

bool parent_directory(path const & a, path const & b)  //!< true if a path is parent of b path
{
	return !has_dotdot(relative(b.parent_path(), a.parent_path()));
}

void weakly_directory_first_sort(vector<path> & files)
{
	sort(files.begin(), files.end(), [](path const & a, path const & b) {
		return !parent_directory(a, b);
	});
}

int main(int argc, char * argv[])
{
	vector<path> files{
		"/music/a.opus",
		"/music/b.opus",
		"/music/d/a.opus",
		"/music/c/a.opus"
	};

	weakly_directory_first_sort(files);

	cout << "files:\n";
	for (path const & f : files)
		cout << "  " << f << "\n";

	return 0;
}
