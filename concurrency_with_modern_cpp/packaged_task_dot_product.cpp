// packaged_task dot product sample
#include <future>
#include <vector>
#include <numeric>
#include <random>
#include <deque>
#include <iostream>

using namespace std;

static int const NUM = 100000000;

using input_iter = vector<int>::iterator;
using inner_product_task = packaged_task<long long (int *, int *, int *, long long)>;

long long get_dot_product(vector<int> & v, vector<int> & w)
{
	// wrap tasks
	inner_product_task prod_task1{inner_product<int *, int *, long long>};
	inner_product_task prod_task2{inner_product<int *, int *, long long>};
	inner_product_task prod_task3{inner_product<int *, int *, long long>};
	inner_product_task prod_task4{inner_product<int *, int *, long long>};

	// create futures
	future<long long> prod_result1 = prod_task1.get_future();
	future<long long> prod_result2 = prod_task2.get_future();
	future<long long> prod_result3 = prod_task3.get_future();
	future<long long> prod_result4 = prod_task4.get_future();

	// perform each calculation in a separate thread
	size_t size = v.size();

	thread t1{move(prod_task1), &v[0], &v[size/4], &w[0], 0LL};
	thread t2{move(prod_task2), &v[size/4], &v[size/2], &w[size/4], 0LL};
	thread t3{move(prod_task3), &v[size/2], &v[size * 3/4], &w[size/2], 0LL};
	thread t4{move(prod_task4), &v[size * 3/4], &v[size], &w[size * 3/4], 0LL};

	t1.detach();
	t2.detach();
	t3.detach();
	t4.detach();

	// pick up results
	long long sum = prod_result1.get() + prod_result2.get() +
		prod_result3.get() + prod_result4.get();

	return sum;
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
