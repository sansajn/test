#include <streambuf>
#include <iostream>
#include <cassert>

// vlozenie kazdeho znaku sposoby overflow() (co je pomale oproti priamemu volaniu log funkcie)
class log_overflow : public std::streambuf
{
public:
	log_overflow() : _cur{_buf} {}
	~log_overflow() {flush();}

protected:
	int_type overflow(int_type c) override  //!< buffer je plny, vyprazdni ho
	{
		if (c == '\n')  // znak '\n' odosle buffer do logu
			flush();
		else
		{
			if (_cur == _buf+BUFSIZE)
				flush();

			*_cur = c;
			++_cur;
		}
		return c;
	}

private:
	void flush()
	{
		if (_cur == _buf)
			return;

		std::cout << "LOG:";
		std::cout.write(_buf, _cur - _buf);
		std::cout << std::endl;
		_cur = _buf;
	}

	static int const BUFSIZE = 1024;
	char _buf[BUFSIZE];
	char * _cur;
};


// do logu odosle buffer ak poslednym znakom je znak '\n'
class log_sync : public std::streambuf
{
public:
	log_sync() {setp(_buf, _buf + BUFSIZE-1);}
	~log_sync() {flush();}

protected:
	int sync() override
	{
		if (*(pptr()-1) == '\n')
			flush();
		return 0;
	}

	int_type overflow(int_type c) override
	{
		assert(0 && "buffer overflow");
		flush();
		setp(_buf, _buf + BUFSIZE-1);
		return c;
	}

private:
	void flush()
	{
		if (pptr() == _buf)
			return;

		std::cout << "LOG:";
		std::cout.write(_buf, pptr() - pbase() - 1);  // ignore '\n'
		std::cout << std::endl;
		setp(_buf, _buf + BUFSIZE-1);
	}

	static int const BUFSIZE = 1024;
	char _buf[BUFSIZE];
};


void test_log_overflow()
{
	log_overflow out;
	std::streambuf * cerr_buff = std::cerr.rdbuf();
	std::cerr.rdbuf(&out);

	int ival = 3;
	std::cerr << "hello " << "lisbon, " << ival << "x times" << std::endl;

	std::cerr.rdbuf(cerr_buff);
}

void test_log_sync()
{
	log_sync out;
	std::streambuf * cerr_buff = std::cerr.rdbuf();
	std::cerr.rdbuf(&out);

	int ival = 3;
	std::cerr << "hello " << "lisbon, " << ival << "x times" << std::endl;

	std::cerr.rdbuf(cerr_buff);
}


int main(int argc, char * argv[])
{
//	test_log_overflow();
	test_log_sync();

	return 0;
}
