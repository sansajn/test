/*! \file memfun.cpp
Ukazka volania clenskych funkcii skrz ukazatele na clenske funkcie, prevzate s knihy
moderni programovani v c++. */


// stl
#include <iostream>
using std::cout;


class parrot
{
public:
	void eat()
	{
		cout << "Tsk, knick, tsk ...\n";
	}

	void talk()
	{
		cout << "O capitan, o my capitan.\n";
	}
};


int main(int argc, char * argv[])
{
	typedef void (parrot::*memfce)();  // ukazatel na clensku funkciu parrot::void(*)(void)

	// operator .*
	memfce action = &parrot::talk;  // jedna sa skor o index do triedy ako o adresu
	parrot geronimo;
	(geronimo.*action)();  // calls geronimo.talk

	// operator ->*
	parrot * pgeronimo = &geronimo;
	action = &parrot::eat;
	(pgeronimo->*action)();  // calls geronimo.eat

	return 0;
}
