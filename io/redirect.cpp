// presmerovanie prudu (cout) do suboru
#include <iostream>
#include <fstream>

using namespace std;

void redirect(ostream &);

int main(int argc, char * argv[])
{
	cout << "prvy riadok" << endl;
	redirect(cout);
	cout << "poslledny riadok" << endl;
	return 0;
}

void redirect(ostream & strm)
{
	ofstream file{"presmerovanie.txt"};
	streambuf * strm_buffer = strm.rdbuf();  // ulozenie povodneho buffru
	strm.rdbuf(file.rdbuf());  // presmerovanie
	file << "jeden riadok pre subor" << endl;
	strm << "jeden riadok pre prud" << endl;
	strm.rdbuf(strm_buffer);  // obnovenie povodneho buffru
}
