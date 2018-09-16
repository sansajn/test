//
#include <iostream>
#include <boost/filesystem.hpp>

using std::cout;
using boost::filesystem::path;
using boost::filesystem::relative;

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

void test_patch(path const & a, path const & b)
{
	if (parent_directory(a, b))
		cout << a << " parent " << b << "\n";
	else if (parent_directory(b, a))
		cout << b << " parent " << a << "\n";
	else
		cout << "independent patches\n";
}

int main(int argc, char * argv[])
{
	test_patch("music/a.txt", "music/2018/a.txt");  // a parent b
	test_patch("/music/2017/a.txt", "/music/2018/a.txt");  // independent
	test_patch("music/2018/a.txt", "music/a.txt");  // b parent a
	return 0;
}
