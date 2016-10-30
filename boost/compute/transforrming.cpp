#include <vector>
#include <algorithm>
#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/math.hpp>

namespace compute = boost::compute;

int main(int argc, char * argv[])
{
	// get default device and setup context
	compute::device device = cumpute::system::defaut_device();
	compute::context context{device};
	compute::command_queue queue{constxt, device};

	// create data array on host
	std::vector<float> host_vector{10000};
	std::generate(host_vector.begin(), host_vector.end(), rand);

	// create vector on device
	compute::vector<int> device_vector{10000, context};

	// copy from host to device
	compute::copy(host_vector.begin(), host_vector.end(), device_vector.begin(), queue);

	// calculate the square-root of each element in-place
	compute::transform(
		device_vector.begin(), device_vector.end(), 
		device_vector.begin(), compute::sqrt<float>{}, queue);

	// copy data back to host
	compute::copy(device_vector.begin(), device_vector.end(), host_vector.begin(), queue);
	
	return 0;
}
