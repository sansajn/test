/* Implementacia parsera minimalneho XML dokumentu.
http://www.boost.org/doc/libs/1_59_0/libs/spirit/doc/html/spirit/qi/tutorials/mini_xml___asts_.html#spirit.qi.tutorials.mini_xml___asts_.first_cut */
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <fstream>
#include <algorithm>

#define BOOST_SPIRIT_USE_PHOENIX_V3
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

struct mini_xml;

using mini_xml_node = 
	boost::variant<
		boost::recursive_wrapper<mini_xml>,
		std::string
	>;

struct mini_xml
{
	std::string name;
	std::vector<mini_xml_node> children;
};

BOOST_FUSION_ADAPT_STRUCT(
	mini_xml,
	(std::string, name)
	(std::vector<mini_xml_node>, children)
);


template <typename Iterator>
struct mini_xml_grammar : qi::grammar<Iterator, mini_xml(), ascii::space_type>
{
	mini_xml_grammar() : mini_xml_grammar::base_type(xml)
	{
		using qi::lit;
		using qi::lexeme;
		using ascii::char_;
		using ascii::string;
		using namespace qi::labels;
		using phoenix::at_c;
		using phoenix::push_back;

		text = lexeme[+(char_ - '<')[_val += _1]];
		node = (xml|text)[_val = _1];

		start_tag = 
				'<'
			>> !lit('/')
			>> lexeme[+(char_ - '>')[_val += _1]]
			>> '>';

		end_tag =
				"</"
			>> lit(_r1)  // _r1 - inheretted attribute placeholder
			>> '>';

		xml = 
				start_tag[at_c<0>(_val) = _1]
			>> *node[push_back(at_c<1>(_val), _1)]
			>> end_tag(at_c<0>(_val));
	}

	qi::rule<Iterator, mini_xml(), ascii::space_type> xml;
	qi::rule<Iterator, mini_xml_node(), ascii::space_type> node;
	qi::rule<Iterator, std::string(), ascii::space_type> text;
	qi::rule<Iterator, std::string(), ascii::space_type> start_tag;
	qi::rule<Iterator, void(std::string), ascii::space_type> end_tag;
};

int const tabsize = 3;

void tab(int indent)
{
	for (int i = 0; i < indent; ++i)
		std::cout << ' ';
}

struct mini_xml_printer
{
	mini_xml_printer(int indent = 0) : indent(indent) {}
	void operator()(mini_xml const & xml) const;
	int indent;
};

struct mini_xml_node_printer : boost::static_visitor<>
{
	mini_xml_node_printer(int indent = 0) : indent(indent) {}

	void operator()(mini_xml const & xml) const
	{
		mini_xml_printer{indent+tabsize}(xml);
	}

	void operator()(std::string const & text) const
	{
		tab(indent+tabsize);
		std::cout << "text: \"" << text << "\"\n";
	}

	int indent;
};

void mini_xml_printer::operator()(mini_xml const & xml) const
{
	tab(indent);
	std::cout << "tag: " << xml.name << "\n";
	tab(indent);
	std::cout << "{\n";

	for (mini_xml_node const & n : xml.children)
		boost::apply_visitor(mini_xml_node_printer{indent}, n);

	tab(indent);
	std::cout << "}\n";
}

int main(int argc, char * argv[])
{
	if (argc < 2)
		throw std::logic_error{"no input file provided"};

	std::ifstream in{argv[1]};
	if (!in.is_open())
		throw std::logic_error{std::string{"could not open input file '"} + argv[1] + std::string{"'"}};

	std::string storage;
	in.unsetf(std::ios::skipws);
	std::copy(std::istream_iterator<char>{in}, std::istream_iterator<char>{}, std::back_inserter(storage));

	mini_xml_grammar<std::string::const_iterator> xml;
	mini_xml ast;  // abstract syntax tree

	auto it = storage.cbegin();
	auto end = storage.cend();
	bool r = qi::phrase_parse(it, end, xml, ascii::space, ast);

	if (r && it == end)
	{
		std::cout << "parsing succeeded\n";
		mini_xml_printer printer;
		printer(ast);
	}
	else
	{
		auto some = it+30;
		std::string context{it, std::min(some, end)};
		std::cout << "parsing failed: " << context << "\n";
	}

	return 0;
}
