#include <iostream>
using std::cout;

class Pointer
{
public:
	Pointer(int * p) : _p(p) {}
	
	operator bool() const 
	{
		cout << "Pointer::bool()\n"; 
		return _p != 0;
	}

	bool operator==(Pointer const & rhs) const 
	{
		cout << "Pointer::operator==()\n";
		return _p == rhs._p;
	}

private:
	int * _p;
};


int main(int argc, char * argv[])
{
	Pointer p1(new int(10));
	Pointer p2(new int(12));
	if (p1 == p2)  // skonvertuje sa na bool a porovna
		;
	return 0;
}

