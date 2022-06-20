/*! Boost.Accumulators hello sample taken from [Example 58.1. Counting with boost::accumulators::tag::count](https://theboostcpplibraries.com/boost.accumulators#ex.accumulators_01) 
\note Working with boost 1.74 (Ubuntu 22.04 LTS). */
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <iostream>
using std::cout;
using namespace boost::accumulators;

int main(int argc, char * argv[]) {
  accumulator_set<int, features<tag::count>> acc;
  acc(4);
  acc(-6);
  acc(9);
  cout << count(acc) << '\n';  // returns 3 (3 elements in acc)
  return 0;
}
