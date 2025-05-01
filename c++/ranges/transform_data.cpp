// Normalizing a data series with transform.
#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>
#include <iostream>
using std::vector, std::accumulate, std::sqrt, std::cout;

int main(int argc, char * argv[]) {
	vector data{10.0, 12.0, 14.0, 16.0, 18.0};
	
	double mean = accumulate(begin(data), end(data), 0.0) / size(data);
	
	// compute standard deviation
	double variance = 0.0;
	for (double x : data)
		variance += (x - mean) * (x - mean);
	variance /= size(data);
	double stddev = sqrt(variance);

	// normalize data (in-place)
	std::ranges::transform(data, begin(data),
		[mean, stddev](double x){return (x - mean) / stddev;});

	for (double x : data)
		cout << x << ", ";

	cout << "\n"
		<< "done\n";

	return 0;
}