// dot product with four futures

#include <future>
#include <random>
#include <vector>
#include <numeric>
#include <iostream>

using namespace std;

static int const NUM = 100000000;

long long get_dot_product(vector<int> & v, vector<int> & w)
{
	size_t size = v.size();

	auto future1 = async([&]{return inner_product(&v[0], &v[size/4], &w[0], 0LL);});
	auto future2 = async([&]{return inner_product(&v[size/4], &v[size/2], &w[size/4], 0LL);});
	auto future3 = async([&]{return inner_product(&v[size/2], &v[size * 3/4], &w[size/2], 0LL);});
	auto future4 = async([&]{return inner_product(&v[size * 3/4], &v[size], &w[size * 3/4], 0LL);});

	return future1.get() + future2.get() + future3.get() + future4.get();
}

int main(int argc, char * argv[])
{
	random_device seed;
	mt19937 engine(seed());
	uniform_int_distribution<int> dist(0, 100);

	vector<int> v(NUM), w(NUM);
	for (int i = 0; i < NUM; ++i)
	{
		v.push_back(dist(engine));
		w.push_back(dist(engine));
	}

	cout << "get_dot_product(v, w)=" << get_dot_product(v, w) << endl;

	return 0;
}
