/* S knihy 'Boost C++ Application Development Cookbook' od Anthony Polukhin
Kapitola 2, Parsing input */
#include <string>
#include <cassert>
#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>

class datetime
{
public:
	enum zone_offsets_t {
		OFFSET_NOT_SET,
		OFFSET_Z,
		OFFSET_UTC_PLUS,
		OFFSET_UTC_MINUS
	};

	datetime()
		: year_{0}, month_{0}, day_{0}, hours_{0}, minutes_{0}, seconds_{0}, 
			zone_offset_type_{OFFSET_NOT_SET}, zone_offset_in_min_{0}
	{}

	unsigned short year_;
	unsigned short month_;
	unsigned short day_;
	unsigned short hours_;
	unsigned short minutes_;
	unsigned short seconds_;
	zone_offsets_t zone_offset_type_;
	unsigned int zone_offset_in_min_;

	void set_year(unsigned short const & y) {year_ = y;}
	void set_month(unsigned short const & m) {month_ = m;}
	void set_day(unsigned short const & d) {day_ = d;}
	void set_hours(unsigned short const & h) {hours_ = h;}
	void set_minutes(unsigned short const & m) {minutes_ = m;}
	void set_seconds(unsigned short const & s) {seconds_ = s;}
	void set_zone_offset_type(zone_offsets_t const & t) {zone_offset_type_ = t;}

	static void dt_assert(bool v, char const * msg) {
		if (!v)
			throw std::logic_error("Assertion failed: " + std::string(msg));
	}
};

void set_zone_offset(datetime & dt, char sign, unsigned short hours, unsigned short minutes)
{
	dt.zone_offset_type_ = (sign == '+' ? datetime::OFFSET_UTC_PLUS : datetime::OFFSET_UTC_MINUS);
	dt.zone_offset_in_min_= hours * 60 + minutes;
}

datetime parse_datetime(std::string const & s)
{
	using boost::spirit::qi::_1;
	using boost::spirit::qi::_2;
	using boost::spirit::qi::_3;
	using boost::spirit::qi::uint_parser;
	using boost::spirit::qi::char_;
	using boost::phoenix::bind;
	using boost::phoenix::ref;

	datetime result;
	uint_parser<unsigned short, 10, 2, 2> u2_;  // radix 10, from 2 to 2 digits
	uint_parser<unsigned short, 10, 4, 4> u4_;

	boost::spirit::qi::rule<char const *, void()> timezone_parser 
		= -(  // unary minus means optional rule
				char_('Z')[bind(&datetime::set_zone_offset_type, &result, datetime::OFFSET_Z)]
				|  // OR
				((char_('+')|char_('-')) >> u2_ >> ':' >> u2_) [
					bind(&set_zone_offset, ref(result), _1, _2, _3)]  // ? co roby toto
			);

	boost::spirit::qi::rule<char const *, void()> date_parser
		= u4_[bind(&datetime::set_year, &result, _1)] >> char_('-') 
		>> u2_[bind(&datetime::set_month, &result, _1)] >> char_('-')
		>> u2_[bind(&datetime::set_day, &result, _1)];

	boost::spirit::qi::rule<char const *,void()> time_parser
		= u2_[bind(&datetime::set_hours, &result, _1)] >> char_(':')
		>> u2_[bind(&datetime::set_minutes, &result, _1)] >> char_(':')
		>> u2_[bind(&datetime::set_seconds, &result, _1)];

	char const * first = s.data();
	char const * const end = first + s.size();

	bool success = boost::spirit::qi::parse(first, end, 
		((date_parser >> char_('T') >> time_parser) | date_parser | time_parser) 
		>> timezone_parser
	);

	if (!success || first != end)
	{
		std::cout << first << "\n";
		throw std::logic_error("Parsing of '" + s + "' failed");
	}

	return result;
}

int main(int argc, char * argv[])
{
	datetime d = parse_datetime("2012-10-20T10:00:00+09:15");
	
	assert(d.year_ == 2012);
	assert(d.month_ == 10);
	assert(d.day_ == 20);
	assert(d.hours_ == 10);
	assert(d.minutes_ == 0);
	assert(d.seconds_ == 0);
	assert(d.zone_offset_type_ == datetime::OFFSET_UTC_PLUS);
	assert(d.zone_offset_in_min_ == 9*60+15);

	std::cout << d.year_ << ", " << d.month_ << ", " << d.day_ << " -> " << d.hours_ << ":" << d.minutes_ << "\n";
	return 0;
}