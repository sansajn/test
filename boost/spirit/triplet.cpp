/* dokaze sparsovat takuto (-12.45 11.12 4) trojicu realnych cisel */
#include <string>
#include <tuple>
#include <stdexcept>
#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>

using namespace boost::spirit;
using boost::spirit::qi::_1;
using boost::spirit::qi::_2;
using boost::spirit::qi::_3;
using boost::spirit::qi::double_;
using boost::phoenix::ref;
using boost::phoenix::bind;
namespace fusion = boost::fusion;

void just_parse()  // len parsovanie
{
	std::string s = "(-12.45 11.12 4)";
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		'(' >> double_ >> double_ >> double_ >> ')', ascii::space);
	if (!match || it != s.end())
		throw std::logic_error{std::string{"Unable to parse '"} + std::string{it, s.end()} + "' expression"};
	std::cout << "just_parse(): done!\n";
}

void store_using_actions()  // ulozenie trojice pomocou akcii
{
	std::string s = "(-12.45 11.12 4)";

	double a, b, c;
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		'(' >> double_[ref(a) = _1] >> double_[ref(b) = _1] >> double_[ref(c) = _1] >> ')', ascii::space);

	if (!match || it != s.end())
		throw std::logic_error{std::string{"Unable to parse '"} + std::string{it, s.end()} + "' expression"};

	std::cout << a << ", " << b << ", " << c << "\n"
		<< "store_using_actions(): done!\n";
}

void action_fce()
{
	std::cout << "action_fce()\n";
}


void action_fce_args(double const & a, double const & b, double const & c)
{
	std::cout << a << ", " << b << ". " << c << "\n";
}

void forward_to_action_func()  // parametre parsera posle do funkcie pomocou bind()
{
	std::string s = "(-12.45 11.12 4)";
	std::tuple<double, double, double> result;
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		'(' >> (double_ >> double_ >> double_)[bind(action_fce_args, _1, _2, _3)] >> ')', ascii::space);
	if (!match || it != s.end())
		throw std::logic_error{std::string{"Unable to parse '"} + std::string{it, s.end()} + "' expression"};
	std::cout << "forward_to_action_func(): done!\n";
}

void action_fce_fusion_vector3(fusion::vector3<double, double, double> & v)
{
	std::cout << "{" << fusion::at_c<0>(v) << ", " << fusion::at_c<1>(v) << ", " << fusion::at_c<2>(v) << "}\n";
}

void vector3_action_func()
{
	std::string s = "(-12.45 11.12 4)";
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		'(' >> (double_ >> double_ >> double_)[action_fce_fusion_vector3] >> ')', ascii::space);
	if (!match || it != s.end())
		throw std::logic_error{std::string{"Unable to parse '"} + std::string{it, s.end()} + "' expression"};
	std::cout << "vector3_action_func(): done!\n";
}

void using_rule()
{
	std::string s = "(-12.45 11.12 4)";

	qi::rule<std::string::iterator, std::tuple<double, double, double>(), ascii::space_type> r =
		'(' >> double_ >> double_ >> double_ >> ')';

	std::tuple<double, double, double> result;
	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(), r, ascii::space, result);
	if (!match || it != s.end())
		throw std::logic_error{std::string{"unable to parse an '"} + std::string{it, s.end()} + "' expression"};

	std::cout << "{" << std::get<0>(result) << ", " << std::get<1>(result) << ", " << std::get<2>(result) << "}\n";
	std::cout << "using_rule(): done!\n";
}

int main(int argc, char * argv[])
{
	just_parse();
	store_using_actions();
	forward_to_action_func();
	vector3_action_func();
	using_rule();
	return 0;
}
