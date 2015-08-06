#include <map>
#include <string>
using std::map;
using std::string;

template <typename Value>
using string_map = map<string, Value>;


int main(int argc, char * argv[])
{
	string_map<int> m;
	return 0;
}


