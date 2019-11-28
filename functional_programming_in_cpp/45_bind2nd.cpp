// listing 4.3 call operator for partial function application
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <functional>
#include <iostream>

using std::vector;
using std::ostream_iterator;
using std::partition;
using std::copy;
using std::greater;
using std::cout;

template <typename Function, typename SecondArgType>
class partial_application_bind2nd_impl
{
public:
	partial_application_bind2nd_impl(Function funct, SecondArgType second_arg)
		: _funct{funct}
		, _second_arg{second_arg}
	{}

	template <typename FirstArgType>
	auto operator()(FirstArgType && first_arg) const
	{
		return _funct(std::forward<FirstArgType>(first_arg), _second_arg);
	}

private:
	Function _funct;
	SecondArgType _second_arg;
};

template <typename Function, typename SecondArgType>
partial_application_bind2nd_impl<Function, SecondArgType> bind2nd(
	Function && function, SecondArgType && second_arg)
{
	return partial_application_bind2nd_impl<Function, SecondArgType>{
		std::forward<Function>(function), std::forward<SecondArgType>(second_arg)};
}

int main(int argc, char * argv[])
{
	vector xs = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	partition(begin(xs), end(xs), bind2nd(greater<int>{}, 6));
//	partition(begin(xs), end(xs), std::bind(greater<int>{}, std::placeholders::_1, 6));
	copy(begin(xs), end(xs), ostream_iterator<int>(cout, ", "));
	cout << "\n";
	return 0;
}
