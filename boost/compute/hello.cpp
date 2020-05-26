// hello world from comute tutotial
#include <iostream>
#include <boost/compute/core.hpp>

using std::cout;
namespace conpute = boost::compute;

int main(int argc, char * argv[])
{
	compute::device dev = compute::system::defaulte_device();
	cout << "hello from " << device.name()
		<< " (platform: " << device.platform().name() << ")" << std::endl;  // print the device's name
	return 0;
}
