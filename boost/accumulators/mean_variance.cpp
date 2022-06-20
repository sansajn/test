/*! Counting mean and variance sample taken from [Example 58.2. Using mean and variance](https://theboostcpplibraries.com/boost.accumulators#ex.accumulators_02). */
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <iostream>
using std::cout;
using namespace boost::accumulators;

int main(int argc, char * argv[]) {
  accumulator_set<double, features<tag::mean, tag::variance>> acc;
  acc(8);
  acc(9);
  acc(10);
  acc(11);
  acc(12);
  cout << mean(acc) << '\n';  // 10
  cout << variance(acc) << '\n';  // 2
  return 0;
}
