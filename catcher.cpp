#include <iostream>
using std::cout;

struct goo {};

void special(int) {}

class foo
{
public:
	template <typename T>
	void fce(T const & t) {
		special(t);
		cout << "foo::fce<T>()\n";
	}

	/*! Toto nemože fungovať, lebo kompilátor sa pokúsi vytvoriť aj všeobecnú
	implementáciu fce<T> v ktorej narazí na funkciu special(int), ktorá nemá
	implementáciu pre goo.

		template <typename T>
		void fce(goo const & g) {cout << "foo::fce(goo const &)\n";}
	*/

	/* Táto verzia už fungovať bude. */
	void fce(goo const & g) {cout << "foo::fce(goo const &)\n";}
};

int main(int argc, char * argv[])
{
	goo g;
	foo f;
	f.fce(g);
	return 0;
}
