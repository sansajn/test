/* sparsuje zoznam takychto zaznamou
	"origin"	-1 ( -0.000000 0.001643 -0.000604 ) ( -0.707107 -0.000242 -0.707107 )
*/
#include <string>
#include <stdexcept>
#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using qi::char_;
using qi::int_;
using qi::float_;

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
)

inline std::ostream & operator<<(std::ostream & o, triplet_type const & t)
{
	o << "{" << std::get<0>(t) << ", " << std::get<1>(t) << ", " << std::get<2>(t) << "}";
	return o;
}

int main(int argc, char * argv[])
{
	std::string s = R"("origin"	-1 ( -0.000000 0.001643 -0.000604 ) ( -0.707107 -0.000242 -0.707107 ), "sheath"	0 ( 1.100481 -0.317714 3.170247 ) ( 0.307041 -0.578615 0.354181 ), "sword"	1 ( 0.980960 -0.936157 4.075372 ) ( 0.305556 -0.578156 0.353505 ), "pubis"	0 ( 0.001408 0.206444 2.614459 ) ( -0.466932 -0.531013 -0.466932 ))";

	qi::rule<std::string::iterator, std::string(), ascii::space_type> qstring =
		qi::lexeme[
			'"' >> *(char_ - '"') >> '"'
		];

	qi::rule<std::string::iterator, triplet_type(), ascii::space_type> triplet =
		'(' >> float_ >> float_ >> float_ >> ')';

	qi::rule<std::string::iterator, joint_record(), ascii::space_type> joint_row =
		qstring >> int_ >> triplet >> triplet;

	std::vector<joint_record> joints;

	auto it = s.begin();
	bool match = qi::phrase_parse(it, s.end(),
		(
			joint_row % ','
		)
		, ascii::space, joints);

	if (!match || it != s.end())
		throw std::logic_error{std::string{"unable to parse an '"} + std::string{it, s.end()} + "' expression"};

	for (joint_record const & j : joints)
		std::cout << "joint:{" << j.name << ", " << j.parent << ", " << j.position << ", " << j.orientation << "}\n";

	std::cout << "done!\n";

	return 0;
}
