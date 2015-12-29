// najjednoduchsi streambuf
#include <streambuf>
#include <locale>
#include <cstdio>
#include <iostream>

using namespace std;

class outbuf : public streambuf
{
protected:
	// hlavna funkcia vystupu, tlaci znaky v rezimu velkych pismen
	virtual int_type overflow(int_type c)
	{
		if (c != EOF)
		{
//			c = toupper(c, getloc());  // toto nefunguje
			if (putchar(c) == EOF)
				return EOF;
		}
		return c;
	}
};

int main(int argc, char * argv[])
{
	outbuf ob;  // vyrovnavacia pamat prudu
	ostream out{&ob};
	out << "31 sestnackove: " << hex << 31 << endl;
	return 0;
}
