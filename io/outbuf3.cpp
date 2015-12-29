// suborovy stream-buffer (vystupny)
#include <iostream>
#include <streambuf>
#include <cstdio>
#include <unistd.h>  // write()

using namespace std;

class outbuf : public streambuf
{
protected:
	static const int bufferSize = 10;
	char buffer[bufferSize];

public:
	/* inicializuje vyrovnavaciu pamet
		- o jeden znak menej, aby mohol znak s poradim bufferSize sposobit volanie funkcie overflow() */
	outbuf() {
		setp(buffer, buffer+(bufferSize-1));
	}

	virtual ~outbuf() {
		sync();
	}

protected:
	// vyprazdni znaky vo vyrovnavacej pameti
	int flushBuffer() {
		int num = pptr() - pbase();
		if (write(1, buffer, num) != num)
			return EOF;
		pbump(-num);  // znovu nastav kladaci ukazatel
		return num;
	}

	// vyrovnavacia pamet je plna, vypise c a vsetky predchadzajuce znaky
	virtual int_type overflow(int_type c) {
		if (c != EOF) {
			*pptr() = c;  // vloz znak do vyrovnavacej pameti
			pbump(1);
		}

		if (flushBuffer() == EOF)
			return EOF;  // chyba

		return c;
	}

	// synchronizacia dat zo suborom/cielom
	virtual int sync() {
		if (flushBuffer() == EOF)
			return EOF;
		return 0;
	}
};

class fdostream : public ostream
{
protected:
	fdoutbuf buf;
public:
	fdostream(int fd) : ostream{0}, buf{fd}
	{
		rdbuf(&buf);
	}
};

int main(int argc, char * argv[])
{
	fdostream out{1};
	out << "31 sestnactkove: " << hex << 31 << endl;
	return 0;
}
