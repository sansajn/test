#include <iostream>
using std::cout;

void f() {}  // koncova podmienka pre f()

template <typename T, typename... Tail>
void f(T head, Tail ... tail)
{
	cout << head << " ";
	f(tail...);
}

int main(int argc, char * argv[])
{
	f(10, 12.5f, "Ahoj", 0.1234);
	return 0;
}

