/* generovanie nahodných čísiel */
#include <random>
#include <iostream>
using std::cout;
using std::endl;


int main(int argc, char * argv[])
{
	std::default_random_engine dre;
	std::uniform_real_distribution<double> dr(0, 180);
	for (int i = 0; i < 10; ++i)
		cout << dr(dre) << " ";

	cout << endl;

	return 0;
}

