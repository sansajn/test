// meranie času v c++11
#include <cmath>
#include <chrono>
#include <limits>
#include <string>
#include <vector>
#include <iostream>
#include <boost/lexical_cast.hpp>
using namespace std;

void math_task();
void conversion_task(vector<string> const & data);
void generate_random_vector(vector<string> & rands, int n);


int main(int argc, char * argv[])
{
	typedef chrono::high_resolution_clock clock;

	clock::time_point start_tm = clock::now();
	math_task();
	clock::duration dt = clock::now() - start_tm;

	cout << "math-task takes " 
		<< chrono::duration_cast<chrono::milliseconds>(dt).count() << " ms\n";

	vector<string> positions(1000000);
	generate_random_vector(positions, 1000000);

	start_tm = clock::now();
	conversion_task(positions);
	dt = clock::now() - start_tm;

	cout << "conversion-task (1M values) takes " 
		<< chrono::duration_cast<chrono::milliseconds>(dt).count() << " ms\n";

	return 0;
}


void math_task()
{
	int const max = numeric_limits<unsigned short>::max();
	for (int x = max; x; --x)
		volatile double y = acos(double(x)/max);
}

void conversion_task(vector<string> const & data)
{
	for (auto pos : data)
		volatile float f = atof(pos.c_str());
}

void generate_random_vector(vector<string> & rands, int n)
{
	std::default_random_engine dre;
	std::uniform_real_distribution<float> dr(0, 180);
	for (int i = 0; i < n; ++i)
		rands[i] = boost::lexical_cast<string>(dr(dre));
}

