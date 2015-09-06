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
#include <boost/spirit/include/support_istream_iterator.hpp>

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

int main(int argc, char * argv[])
{
	std::ifstream in{file_path};
	if (!in.is_open())
		throw std::logic_error{std::string{"unable to open file '"} + file_path + "'"};

	in.unsetf(std::ios::skipws);

	using iterator_type = boost::spirit::istream_iterator;
	iterator_type it{in}, end;

	qi::rule<iterator_type, std::string(), ascii::blank_type> qstring =
		qi::lexeme[
			'"' >> *(char_ - '"') >> '"'
		];

	qi::rule<iterator_type, triplet_type(), ascii::blank_type> triplet =
		'(' >> float_ >> float_ >> float_ >> ')';

	qi::rule<iterator_type, joint_record(), ascii::blank_type> joint_row =
		qstring >> int_ >> triplet >> triplet;

	std::vector<joint_record> joints;

	bool match = qi::phrase_parse(it, end,
		(
			joint_row % qi::eol >> *qi::eol
		)
		, ascii::blank, joints);

	if (!match || it != end)
		throw std::logic_error{std::string{"unable to parse an expression"}};

	for (joint_record const & j : joints)
		std::cout << "joint:{" << j.name << ", " << j.parent << ", " << j.position << ", " << j.orientation << "}\n";

	std::cout << "done!\n";

	return 0;
}
