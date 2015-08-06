#include <iostream>
#include <boost/range.hpp>
#include <boost/range/adaptors.hpp>


template <typename Cond, typename R>
boost::filtered_range<Cond, R> filter_array(R & ids, Cond cond) {return boost::adaptors::filter(ids, cond);}

template <typename Cond, typename R>
boost::filtered_range<Cond, R> filter_array2(R & ids, Cond cond)
{
	return boost::filtered_range<Cond, R>(cond, ids);
}

template <typename Cond>
boost::filtered_range<Cond, int[3]> filter_array3(int (& ids)[3], Cond cond)
{
	return boost::filtered_range<Cond, int[3]>(cond, ids);
}



struct valid_id_pred
{
	bool operator()(int & v) {return v > 0;}
};

struct foo
{
	boost::filtered_range<valid_id_pred, int[3]> ids() {
		valid_id_pred pred;
		return boost::filtered_range<valid_id_pred, int[3]>(pred, _ids);
	}
	
	int _ids[3];
};


int main(int argc, char * argv[])
{
	int ids[3];
	ids[0] = 1;
	ids[1] = 0;
	ids[2] = 3;
	
/*
	auto r = boost::make_iterator_range(ids)|boost::adaptors::filtered([](int & v){return v > 0;});
	for (int id : r)
		std::cout << id << ", ";
	
	std::cout << "\n";
	
	auto fr = boost::adaptors::filter(ids, [](int & v){return v > 0;});
	for (int id : fr) 
		std::cout << id << ", ";
	
	std::cout << "\n";
	
	auto r3 = filter_array(ids, [](int & v){return v > 0;});
	for (int id : r3)
		std::cout << id << ", ";
	
	std::cout << "\n";
	
	auto r2 = filter_array2(ids, [](int & v){return v > 0;});
	for (int id : r2)
		std::cout << id << ", ";
	
	std::cout << "\n";
	
	auto r4 = filter_array3(ids, [](int & v){return v > 0;});
	for (int id : r4)
		std::cout << id << ", ";
	
	std::cout << "\n";
*/

	foo f;
	f._ids[0] = 1;
	f._ids[1] = 0;
	f._ids[2] = 2;
	for (int id : f.ids())
		std::cout << id << ", ";
	
	std::cout << "\n";
	
	return 0;
}