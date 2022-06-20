/*! image statistics (min, max, mean, median, variance) sample with accumulators library 
see [Descriptive Stats with C++ and Boost](https://www.nu42.com/2016/12/descriptive-stats-with-cpp-boost.html) */

#include <algorithm>
#include <iostream>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/variance.hpp>
using std::for_each, std::begin, std::end, std::cout;
using namespace boost::accumulators;


int main(int argc, char * argv[]) {
	uint8_t pixels[] = {0x00, 0xff, 0xff, 0x00};
	accumulator_set<double, features<
		tag::min, tag::max, tag::mean, tag::median, tag::variance>> acc;
	acc = for_each(begin(pixels), end(pixels), acc);  // error prome (it is easy to forgot `acc =` part)
	// for (uint8_t v : pixels)
	// 	acc(v);

	auto const n = count(acc);
	double const var = static_cast<double>(n)/(n-1) * variance(acc),
		sd = sqrt(var);

	cout << "count=" << n << "\n"
		<< "min=" << min(acc) << "\n"
		<< "max=" << max(acc) << "\n"
		<< "mean=" << mean(acc) << "\n"
		<< "median=" << median(acc) << "\n"
		<< "var=" << var << "\n"
		<< "sd=" << sd << "\n"
		<< "done\n";

	return 0;
}

