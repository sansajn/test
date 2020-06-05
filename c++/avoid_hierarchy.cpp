/* sample from [c++ talk](https://www.youtube.com/watch?v=W2tWOdzgXHA) allows 
to create vector of drawable objects without any class hierarchies */
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <ostream>
#include <iostream>

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::move;
using std::string;
using std::ostream;
using std::endl;
using std::cout;

class object_t
{
public:
	template <typename T>  // T models Drawable
	object_t(T x) : _self{make_shared<model<T>>(move(x))} 
	{}

	void draw(ostream & out, size_t position) const
	{
		_self->draw(out, position);
	}

private:
	struct concept_t 
	{
		virtual ~concept_t() = default;
		virtual void draw(ostream &, size_t) const = 0;
	};

	template <typename T>
	struct model : concept_t
	{
		model(T x) : _data{move(x)}
		{}

		void draw(ostream & out, size_t position) const
		{
			_data.draw(out, position);
		}

		T _data;
	};

	shared_ptr<concept_t const> _self;
};


class my_class_t
{
public:
	void draw(ostream & out, size_t position) const
	{
		out << string(' ', position) << "my_class_t" << endl;
	}
};

using document_t = vector<object_t>;

void draw(document_t const & x, ostream & out, size_t position)
{
	out << string(' ', position) << "<document>" << endl;
	for (auto const & e : x)
		e.draw(out, position + 2);
	out << string(' ', position) << "</document>" << endl;
}

int main(int argc, char * argv[])
{
	document_t doc;
	doc.emplace_back(my_class_t{});
	draw(doc, cout, 0);
	return 0;
}
