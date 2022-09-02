/*! Concept sample, taken from *C++20 The Complete Guide*, chapter 3, sample `lang/add.cpp` */
#include <vector>
#include <set>
#include <ranges>
#include <atomic>
#include <iostream>

using std::vector, std::set;

// concept for container with push_back()
template <typename Coll>
concept SupportsPushBack = requires (Coll coll, Coll::value_type val) {
	coll.push_back(val);
};

// concept to disable narrowing conversions
template <typename From, typename To>
concept ConvertsWithoutNarrowing = 
	std::convertible_to<From, To> &&
	requires (From && x) {
		{ std::type_identity_t<To[]>{std::forward<From>(x)} }
			-> std::same_as<To[1]>;
	};

// add() for single values
template <typename Coll, typename T>
	requires ConvertsWithoutNarrowing<T, typename Coll::value_type>
void add(Coll & coll, T const & val) {
	if constexpr (SupportsPushBack<Coll>)
		coll.push_back(val);
	else
		coll.insert(val);
}

// add() for multiplevalues
template <typename Coll, std::ranges::input_range T>
	requires ConvertsWithoutNarrowing<std::ranges::range_value_t<T>, typename Coll::value_type>
void add(Coll & coll, T const & val) {
	if constexpr (SupportsPushBack<Coll>)
		coll.insert(end(coll), std::ranges::begin(val), std::ranges::end(val));
	else
		coll.insert(std::ranges::begin(val), std::ranges::end(val));
}	

int main([[maybe_unused]] int argc, [[maybe_unused]] char * argv[]) {
	vector<int> ivec;
	add(ivec, 42);  // OK: calls push_back() for T being int

	set<int> iset;
	add(iset, 42);  // OK: calls insert() for T being int

	short s = 42;
	add(ivec, s);  // OK: calls push_back() for T being short

	[[maybe_unused]] long long ll = 42;
	//add(ivec, ll);  // ERROR: narrowing
	//add(ivec, 7.7);  // ERROR: narrowing

	vector<double> dvec;
	add(dvec, .7);  // OK: calls push_back() for loating-point types
	add(dvec, .7f);  // OK: calls push_back() for loating-point types
	//add(dvec, 7);  // ERROR: narrowing

	// insert collections
	add(ivec, iset);  // OK: insert set elements into a vector
	add(iset, ivec);  // OK: insert vector elements into a set

	// can even insert raw array
	int vals[] = {0, 8, 18};
	add(ivec, vals);  // OK
	//add(dvec, vals);  // ERROR: narrowing

	return 0;
}
