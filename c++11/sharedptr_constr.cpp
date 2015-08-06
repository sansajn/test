// vytvorenie zdielatelneho ukazateľa
#include <memory>
#include <iostream>

using std::cout;

int main(int argc, char * argv[])
{
	std::shared_ptr<int> p1 = std::make_shared<int>(15);
	std::shared_ptr<int> p2{new int(15)};

	std::shared_ptr<int> ** pp;

	auto p = new std::shared_ptr<int> * [5];  // pole 5 ukazatelou na shared-pointer
	for (int i = 0; i < 5; ++i)
		p[i] = new std::shared_ptr<int>(new int(i*i));

	pp = p;
	
	for (int i = 0; i < 5; ++i)
		cout << *(*(pp[i])) << " ";  // $ 0 1 4 9 16

	cout << "\n";
	return 0;
}
