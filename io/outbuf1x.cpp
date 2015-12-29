#include <streambuf>
#include <locale>
#include <cstdio>

using namespace std;

template <class charT, class traits = char_traits<charT>>
class basic_outbuf : public basic_streambuf<charT, traits>
{
protected:
	virtual typename traits::int_type overflow(typename traits::int_type c)
	{
		if (traits::eq_int_type(c, traits::eof()))
		{
			c = toupper(c, getloc());
			if (putchar(c) == EOF)
				return traits::eof();
		}
		return traits::not_eof(c);
	}
};

typedef basic_outbuf<char> outbuf;
typedef basic_outbuf<wchar_t> woutbuf;
