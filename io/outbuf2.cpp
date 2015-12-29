// suborovy stream-buffer (vystupny)
#include <iostream>
#include <streambuf>
#include <cstdio>
#include <unistd.h>  // write()

using namespace std;

class fdoutbuf : public streambuf
{
protected:
	int fd;
public:
	fdoutbuf(int _fd) : fd{_fd} {}
protected:
	// vypisuje jeden znak
	virtual int_type overflow(int_type c)
	{
		if (c != EOF)
		{
			char z = c;
			if (write(fd, &z, 1) != 1)
				return EOF;
		}
		return c;
	}
	
	// zapisuje viac znakou
	virtual streamsize xputn(char const * s, streamsize num)
	{
		return write(fd, s, num);
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
