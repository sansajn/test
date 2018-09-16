// implementacia funkcie relative_path, vracajucu rozdiel p2 - p1 (v zmysle cesty)
#include <string>
#include <cassert>
#include <iostream>
#include <boost/filesystem/path.hpp>

using std::string;
using std::cout;
namespace fs = boost::filesystem;


fs::path relative_path(fs::path const & p1, fs::path const & p2)
{
	assert(p1.native().size() <= p2.native().size() && "p1 is not a subpath of p2");

	fs::path::iterator it = p2.begin();
	
	for (auto elem : p1)
	{
		if (*it == elem)
			++it;
		else
			break;
	}

	fs::path result;
	for (; it != p2.end(); ++it)
		result /= *it;
	
	return result;
}


int main(int argc, char * argv[])
{
	cout << relative_path(fs::path{"temp/drop"}, fs::path{"temp/drop/files/apps"});
	cout << relative_path(fs::path{"temp/drop/files/apps"}, fs::path{"temp/drop"});
	return 0;
}
