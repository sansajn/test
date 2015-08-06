#include <iostream>

class foo
{
public:
	template <typename T>
	void set(T * data) {_data = (void *)data;}

	template <typename R>
	R * get() const {return (R *)_data;}

private:
	void * _data;
};


int main(int argc, char * argv[])
{
	int i = 101;
	foo f;
	f.set(&i);

	int j = *f.get<int>();

	std::cout << "i=" << i << "\n"
		<< "j=" << j << "\n";

	return 0;
}
