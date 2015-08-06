#include <memory>
#include <iostream>

template <typename T>
using ptr = std::shared_ptr<T>;

template <typename T, typename ... Args>
ptr<T> make_ptr(Args && ... args) {return std::make_shared<T>(args ...);}


int main(int argc, char * argv[])
{
	ptr<int> p(new int(42));
	std::cout << "p=" << *p << std::endl;

	ptr<double> r = std::make_shared<double>(10.10);

	ptr<float> f = make_ptr<float>(11.11f);
	std::cout << "f=" << *f << std::endl;

	return 0;
}
