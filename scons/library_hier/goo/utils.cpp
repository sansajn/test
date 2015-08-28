#include "utils.hpp"
using std::string;


int utils::count(string const & s)
{	
	int count = 0;
	int pos = s.find('/');
	while (pos != string::npos)
	{
		count += 1;
		pos = s.find('/', pos+1);
	}

	return count;
}
