// shared_ptr na konstantny objekt a konverzia s nekonstantneho na konstantny
#include <memory>
#include <iostream>

using std::shared_ptr;
using std::cout;

struct foo 
{
	void goo() {}
	~foo() {cout << "bye!" << std::endl;}
};

int main(int argc, char * argv[])
{
	shared_ptr<foo const> ci;
	
	{
		shared_ptr<foo> i{new foo};
		ci = i;
		// ci->goo();  // unable to call non const methods
	}
	
	cout << "done\n";
		
	return 0;
}
