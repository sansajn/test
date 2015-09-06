#include <string>
#include <tuple>
#include <stdexcept>
#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phx = boost::phoenix;

using qi::int_;
using qi::float_;
using qi::char_;
using qi::lit;  // literal

std::string data = R"(
mesh {
	shader "bob_body"

	numverts 489
	vert 0 ( 0.683594 0.455078 ) 0 3
	vert 1 ( 0.773438 0.439453 ) 3 2
	vert 2 ( 0.738281 0.464844 ) 5 2
	vert 3 ( 0.277344 0.306641 ) 7 2
	vert 4 ( 0.318359 0.181641 ) 9 2
}
)";

using pair_type = std::tuple<float, float>;  // pair, triplet, quadruplet

struct vertex_record
{
	int index;
	pair_type coords;
	int start_weight;
	int weight_count;
};

struct triangle_record
{
	int index;
	int v0, v1, v2;  // vertex indices
};

BOOST_FUSION_ADAPT_STRUCT(
	vertex_record,
	(int, index)
	(pair_type, coords)
	(int, start_weight)
	(int, weight_count)
);

BOOST_FUSION_ADAPT_STRUCT(
	triangle_record,
	(int, index)
	(int, v0)
	(int, v1)
	(int, v2)
);

int main(int argc, char * argv[])
{
	qi::rule<std::string::iterator, pair_type(), ascii::space_type> pair =
		'(' >> float_ >> float_ >> ')';

	qi::rule<std::string::iterator, vertex_record(), ascii::space_type> vertex_row =
		qi::lit("vert") >> int_ >> pair >> int_ >> int_;

	qi::rule<std::string::iterator, std::string(), ascii::space_type> qstring =
		qi::lexeme['"' >> *(char_ - '"') >> '"'];

	std::string shader;
	int verts_count;
	std::vector<vertex_record> verts;

	auto it = data.begin();
	bool match = qi::phrase_parse(it, data.end(),
		(
			lit("mesh") >> '{'
				>>	lit("shader") >> qstring
				>> lit("numverts") >> int_[phx::reserve(phx::ref(verts), qi::_1)]  // rezervuj miesto pre vrcholy
				>> +vertex_row
			>> '}'
		)
		, ascii::space, shader, verts_count, verts);

	if (!match || it != data.end())
		throw std::logic_error{std::string{"unable to parse an '"} + std::string{it, data.end()} + "'"};

	std::cout << "shader:" << shader << "\n";
	for (vertex_record const & v : verts)
		std::cout << "{" << v.index << ", " << "{" << std::get<0>(v.coords) << ", " << std::get<1>(v.coords) << "}, "
			<< v.start_weight << ", " << v.weight_count << "}\n";

	std::cout << "done!\n";

	return 0;
}
