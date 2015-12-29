#include <streambuf>
#include <iostream>

class log_streambuf : public std::streambuf
{
public:
	log_streambuf(int log_prio);
	~log_streambuf();

protected:
	int_type overflow(int_type c) override;  //!< buffer je plny, vyprazdni ho
	int sync() override;

private:
	int_type flush_buffer();

	static int const BUFSIZE = 1024;
	char _buf[BUFSIZE];
	int _prio;
};


log_streambuf::log_streambuf(int log_prio) : _prio{log_prio}
{
	setp(_buf, _buf + BUFSIZE-2);  // rezervuj si 2 znaky na overflow znak a znak '\0'
}

log_streambuf::~log_streambuf()
{
	sync();
}

log_streambuf::int_type log_streambuf::overflow(int_type c)
{
	if (c != EOF)
	{
		*pptr() = c;
		pbump(1);
	}

	if (flush_buffer() == EOF)
		return EOF;

	return c;
}

log_streambuf::int_type log_streambuf::flush_buffer()
{
	int num = pptr() - pbase();
	if (num > 0)
		std::cout.write(_buf, num);
	pbump(-num);  // setp(_buf, _buf + MAX_BUFSIZE-2);
	return num;
}

int log_streambuf::sync()
{
	return flush_buffer() != EOF ? 0 : -1;
}


int main(int argc, char * argv[])
{
	log_streambuf out{0};
	std::streambuf * cerr_buff = std::cerr.rdbuf();
	std::cerr.rdbuf(&out);

	std::cerr << "hello" << std::endl;
	std::cerr << "lisbon" << std::endl;


	std::cerr.rdbuf(cerr_buff);

	return 0;
}
