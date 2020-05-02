// fiber sample
#include <string>
#include <iostream>
#include <boost/fiber/all.hpp>

using std::string;
using std::cout, 
	std::endl;
using namespace boost::fibers;
namespace this_fiber = boost::this_fiber;

int main(int argc, char * argv[])
{
	fiber fb{[](string const & str, int n){
		for (int i = 0; i < n; ++i)
		{
			cout << i << ": " << str << endl;
			this_fiber::yield();
		}
	}, "hello", 10};

	cout << "fiber: " << fb.get_id() << endl;
	fb.join();
	cout << "done." << endl;

	return 0;
}
