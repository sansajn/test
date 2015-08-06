#include <vector>
#include <memory>
using namespace std;

/*Čo ale keď mám uložené zdielatelne inštancie goo, potom zruším vlastníka foo
ale goo inštancie žijú dalej ? 

Definovať goo::owner() vracajúcu foo & nemôžem, lebo po umretí vlastníka je
odkaz naň neplatný (čo je indikované hodnotou nullptr).

goo inštancie žijú dalej, ale owner je teraz neplatný 

(?) ak je životnosť goo ohraničená životnosťou foo, aký ma zmyseľ vo foo
uchovávať zdielateľné ordakzy na goo ? */

class foo;  // fwd

class goo 
{
public:
	goo() : _owner(nullptr) {}
	foo * owner() {return _owner;}

private:
	foo * _owner;

	friend foo;  // nastavuje vlastnictvo
};

class foo 
{
public:
	void append(shared_ptr<goo> g) {
		g->_owner = this;
		_data.push_back(g);
	}

	~foo() {
		for (auto g : _data)
			g->_owner = nullptr;
	}

private:	
	vector<shared_ptr<goo>> _data;
};



int main(int argc, char * argv[])
{
	shared_ptr<goo> g = make_shared<goo>();
	foo f;
	f.append(g);
	return 0;
}
