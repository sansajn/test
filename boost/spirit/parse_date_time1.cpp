/* S knihy 'Boost C++ Application Development Cookbook' od Anthony Polukhin
Kapitola 2, Parsing simple input */
#include <string>
#include <cassert>
#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

struct date 
{
	unsigned short year;
	unsigned short month;
	unsigned short day;
};

date parse_date_time1(std::string const & s)
{
	using boost::spirit::qi::_1;
	using boost::spirit::qi::ushort_;
	using boost::spirit::qi::char_;
	using boost::phoenix::ref;

	date res;
	char const * first = s.data();
	char const * const end = first + s.size();
	bool success = boost::spirit::qi::parse(first, end, 
		ushort_[ref(res.year) = _1] >> char('-') 
			>> ushort_[ref(res.month) = _1] >> char('-') 
			>> ushort_[ref(res.day) = _1]);
	if (!success || first != end)
		throw std::logic_error("parsing failed");

	return res;
}

int main(int argc, char * argv[])
{
	date d = parse_date_time1("2012-12-31");
	assert(d.year == 2012);
	assert(d.month == 12);
	assert(d.day == 31);
	std::cout << d.year << ", " << d.month << ", " << d.day << "\n";
	return 0;
}
