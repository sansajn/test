/* pretypovanie referencie na uint a spet */
#include <iostream>
using std::cout;

struct foo
{
	void introduce() {cout << "Hello I'm foo class.";}
};

void invoke(unsigned int param)
{
	foo & ref = *((foo *)&param);
	ref.introduce();	
}


int main(int argc, char * argv[])
{
	foo f;
	invoke((unsigned int)(&f));
	return 0;
}


