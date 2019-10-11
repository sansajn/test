// performance of the various execution policies
#include <algorithm>
#include <cmath>
#include <chrono>
#include <execution>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

constexpr long long SIZE = 100'000'000;
double const pi = std::acos(-1);

template <typename Func>
void execution_time(string const & title, Func func)
{
	auto t0 = steady_clock::now();
	func();
	duration<double> dur = steady_clock::now() - t0;
	cout << title << ": " << dur.count() << " sec. " << endl;
}

int main(int argc, char * argv[])
{
	vector<double> rand_vals;
	rand_vals.reserve(SIZE);

	mt19937 engine;
	uniform_real_distribution uniform_dist{0.0, pi/2};
	for (long long i = 0; i < SIZE; ++i)
		rand_vals.push_back(uniform_dist(engine));

	vector<double> work_vec{rand_vals};

	execution_time("execution::seq", [work_vec]() mutable {
		transform(execution::seq, work_vec.begin(), work_vec.end(), work_vec.begin(),
			[](double arg){return tan(arg);});
	});

	execution_time("execution::par", [work_vec]() mutable {
		transform(execution::par, work_vec.begin(), work_vec.end(), work_vec.begin(),
			[](double arg){return tan(arg);});
	});

	execution_time("execution::par_unseq", [work_vec]() mutable {
		transform(execution::par_unseq, work_vec.begin(), work_vec.end(), work_vec.begin(),
			[](double arg){return tan(arg);});
	});

	return 0;
}
