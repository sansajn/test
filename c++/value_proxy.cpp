#include <iostream>

using std::cout;

template <typename T>
class like_value
{
public:
	like_value(T * v) 
		: _v{v}
	{}
	
	~like_value() {delete _v;}
	
	operator T() {return *_v;}
	
private:
	T * _v;
};

like_value<int> foo()
{
	return like_value<int>{new int{3}};
}

int main(int argc, char * argv[])
{
	if (3 == foo())
		cout << "3\n";
	else
		cout << "X\n";
	
	return 0;
}
