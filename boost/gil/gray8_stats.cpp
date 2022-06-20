//! Create gray 8bit view from array of pixels and save it to a png file (working with boost 1.74).
#include <vector>
#include <iostream>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/variance.hpp>

using std::vector, std::cout;
using boost::gil::gray8_view_t, boost::gil::gray8_pixel_t, boost::gil::interleaved_view,
	boost::gil::write_view, boost::gil::png_tag, boost::gil::for_each_pixel;
using boost::accumulators::accumulator_set, boost::accumulators::features,
	boost::accumulators::min, boost::accumulators::max, boost::accumulators::mean,
	boost::accumulators::median, boost::accumulators::variance, boost::accumulators::count;
namespace tag = boost::accumulators::tag;


int main(int argc, char * argv[]) {
	vector<uint8_t> const pixels{0, 0xff, 0xff, 0};
	size_t const w = 2,
		h = 2;

	gray8_view_t pixel_view = interleaved_view(w, h, (gray8_pixel_t *)pixels.data(), w);
	write_view("test.png", pixel_view, png_tag());

	// calculate descriptive statistica
	accumulator_set<double, features<
		tag::min, tag::max, tag::mean, tag::median, tag::variance>> acc;

	acc = for_each_pixel(pixel_view, acc);

	auto const n = count(acc);
	double const var = static_cast<double>(n)/(n-1) * variance(acc),
		sd = sqrt(var);

	cout << "stats:\n"
		<< "  min=" << min(acc) << "\n"
		<< "  max=" << max(acc) << "\n"
		<< "  range=" << max(acc) - min(acc) << "\n"
		<< "  mean=" << mean(acc) << "\n"
		<< "  median=" << median(acc) << "\n"
		<< "  sd=" << sd << "\n";

	return 0;
}
