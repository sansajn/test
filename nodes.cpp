template <class Base>
struct list_node 
{
	Base * next;
};

template <class Base>
struct fibonaci_node
{
	Base * left;
	Base * right;
	Base * top;
};

template <typename T>
struct element : public list_node< element<T> >
{
	T value;
	element(T const & val) : value(val) {}
};

template <class Element>
class container
{
public:
	void append(Element * x) {
		if (_first)
		{
			if (x->value > _first->value)
			{
				x->next = _first;
				_first = x;
			}
			else
			{
				x->next = _first->next;
				_first->next = x;
			}
		}
		else
			_first = x;
	}

private:
	Element * _first;
};


int main(int argc, char * argv[])
{
	container c;
	c.append(new element<int>(12));
	return 0;
}

