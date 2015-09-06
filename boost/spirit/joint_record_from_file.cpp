/* sparsuje zoznam takychto zaznamou
	"origin"	-1 ( -0.000000 0.001643 -0.000604 ) ( -0.707107 -0.000242 -0.707107 )
*/
#include <string>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using qi::char_;
using qi::int_;
using qi::float_;

std::string file_path = "joints.txt";

using triplet_type = std::tuple<float, float, float>;

struct joint_record
{
	std::string name;
	int parent;
	triplet_type position;
	triplet_type orientation;
};

BOOST_FUSION_ADAPT_STRUCT(
	joint_record,
	(std::string, name)
	(int, parent)
	(triplet_type, position)
	(triplet_type, orientation)
);

inline std::ostream & operator<<(std::ostream & o, triplet_type const & t)
{
	o << "{" << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << "}";
	return o;
}

std::string read_file(std::string const & fname)
{
	std::ifstream in{fname};
	if (!in.is_open())
		throw std::logic_error{std::string{"unable to open file '"} + fname + "'"};
	std::ostringstream ss;
	ss << in.rdbuf();
	return ss.str();
}

int main(int argc, char * argv[])
{
	std::string s = read_file(file_path);

	qi::rule<std::string::iterator, std::string(), ascii::blank_type> qstring =
		qi::lexeme[
			'"' >> *(char_ - '"') >> '"'
		];

	qi::rule<std::string::iterator, triplet_type(), ascii::blank_type> triplet =
		'(' >> float_ >> float_ >> float_ >> ')';

	qi::rule<std::string::iterator, joint_record(), ascii::blank_type> joint_row =
		qstring >> int_ >> triplet >> triplet;

	std::vector<joint_record> joints;

	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		(
			joint_row % qi::eol >> *qi::eol
		)
		, ascii::blank, joints);

	if (!match || it != s.end())
		throw std::logic_error{std::string{"unable to parse an '"} + std::string{it, s.end()} + "' expression"};

	for (joint_record const & j : joints)
		std::cout << "joint:{" << j.name << ", " << j.parent << ", " << j.position << ", " << j.orientation << "}\n";

	std::cout << "done!\n";

	return 0;
}
