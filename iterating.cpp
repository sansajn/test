// test možnosti iterácie kontajnerom
#include <map>
#include <string>
#include <iostream>
#include <boost/range.hpp>
#include <boost/range/adaptor/map.hpp>

using namespace std;

class foo
{
public:
	foo();

	// #1 boost
	typedef boost::iterator_range<map<string, string>::iterator> range_type;
	range_type boost_iter() {return boost::make_iterator_range(_names);}

	typedef boost::iterator_range<map<string, string>::const_iterator> const_range_type;
	const_range_type cboost_iter() const {return boost::make_iterator_range(_names);}

	// #2 std kontajner
	map<string, string> & std_iter() {return _names;}
	map<string, string> const & cstd_iter() const {return _names;}

	// #3 pair iterator
	typedef map<string, string>::iterator iter_type;
	pair<iter_type, iter_type> pair_iter() {return make_pair(_names.begin(), _names.end());}

	typedef map<string, string>::const_iterator const_iter_type;
	pair<const_iter_type, const_iter_type> cpair_iter() const {return make_pair(_names.cbegin(), _names.cend());}
	
private:
	map<string, string> _names;
};

foo::foo()
{
	_names["Jane"] = "Patric";
	_names["Lisbon"] = "Teresa";
	_names["Kimball"] = "Cho";
	_names["Rigsby"] = "Vayne";
	_names["Van Pelt"] = "Grace";
}

int main(int argc, char * argv[])
{
	foo names;
	
	// boost
	cout << "boost:\n";
	for (auto & name : names.boost_iter())
		cout << name.second << " " << name.first << "\n";

	cout << "\nboost const version:\n";
	for (auto & name : names.cboost_iter())
		cout << name.second << " " << name.first << "\n";

	// std for-loop
	cout << "\nstd\n";
	for (auto & name : names.std_iter())
		cout << name.second << " " << name.first << "\n";

	cout << "\nstd const version:\n";
	for (auto & name : names.cstd_iter())
		cout << name.second << " " << name.first << "\n";

	// pair iterator
	cout << "\npair of iterators:\n";
	for (auto r = names.pair_iter(); r.first != r.second; ++r.first)
		cout << r.first->second << " " << r.first->first << "\n";

	cout << "\npair of iterators const version\n";
	for (auto r = names.cpair_iter(); r.first != r.second; ++r.first)
		cout << r.first->second << " " << r.first->first << "\n";


	// by value
	cout << "\nboost by value\n";
	for (string const & v : names.cboost_iter()|boost::adaptors::map_values)
		cout << v << "\n";

	return 0;
}
