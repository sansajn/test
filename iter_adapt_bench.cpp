/* Benchmark iterátorvých adaptérov. 
Zostav príkazom: $ g++ --std=c++0x -o <output-bin> <input-src> -lrt -O2 */

// std
#include <iterator>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
// loe
#include "stopwatch.hpp"


template <class Container>
inline void fill(Container & dst, Container const & src)
{
	for (auto & e : src)
		dst.push_back(e);
}

template <class Inserter, class Container>
inline void fill(Inserter dst, Container const & src)
{
	for (auto & e : src)
		dst = e;
}

template <class Container>
inline void filli(Container & dst, Container const & src)
{
	for (auto & e : src)
		std::back_inserter(dst) = e;
}

template <class Container>
inline void fillh(Container & dst, Container const & src)
{
	size_t size = src.size();
	for (size_t i = 0; i < size; ++i)
		dst[i] = src[i];
}


template <class Container>
void create_benchmark_data(Container & data);


const int N_ELEMS = 10000000;


int main()
{
	std::vector<int> data;
	create_benchmark_data(data);
	loe::stop_watch watch;

	std::vector<int> a;
	a.reserve(N_ELEMS);

	watch.start();
	fill(a, data);
	long dt_mem = watch.stop();
	a.clear();


	std::vector<int> b;
	b.reserve(N_ELEMS);

	watch.start();
	fill(std::back_inserter(b), data);
	long dt_insert = watch.stop();
	b.clear();


	std::vector<int> c;
	c.reserve(N_ELEMS);

	watch.start();
	filli(c, data);
	long dt_inserti = watch.stop();
	c.clear();


	std::vector<int> d;
	d.reserve(N_ELEMS);

	watch.start();
	fillh(d, data);
	long dt_hand = watch.stop();
	d.clear();
	
	std::cout << "Member function: " << dt_mem << " ns\n";
	std::cout << "Inserter       : " << dt_insert << " ns\n";
	std::cout << "Inserter inline: " << dt_inserti << " ns\n";
	std::cout << "Hand written   : " << dt_hand << " ns\n";
	
	return 0;
}


template <class Container>
void create_benchmark_data(Container & data)
{
	srand(time(0));
	const int n_elems = N_ELEMS;
	for (int i = 0; i < n_elems; ++i)
		data.push_back(rand());
}

