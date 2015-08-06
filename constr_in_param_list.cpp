/* Testuje volanie konštruktora v zozneme parametreu inného
konštruktora. 
Záver: Delegované konštruktory sú dostupné až v C++11. */
template <typename T>
struct foo
{
	foo();
	foo(char const * fname) {}
};

template <typename T>
foo<T>::foo()
	: foo("Hello.txt")  // works with -std=c++0x in GCC 4.7
{
}


int main()
{
	foo<int> f;
	return 0;
}
