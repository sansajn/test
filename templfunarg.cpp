/* 'Meno' šablónovej funkcie (add3) nemôže biť parametrom šablónovej
funkcie invoke. */
#include <iostream>
using std::cout;

template <typename T, class Pred>
void invoke(T & t, Pred fn)
{
	t = fn(t);
}

template <typename T>
inline T add3(T const & t)
{
	return t+3;
}

template <typename T>
struct add3functor
{
	T operator()(T const & t) {return t+3;}
};


int main()
{
	int i = 9;
	//invoke(i, add3);
	invoke(i, add3<int>);
	cout << "i=" << i << "\n";
	
	add3functor<int> add3obj;
	invoke(i, add3obj);
	
	cout << "i=" << i << "\n";
	return 0;
}
