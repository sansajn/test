// ako na buffre
#include <array>
#include <string>
#include <iostream>
#include <azmq/socket.hpp>

using std::array;
using std::string;
using std::cout;
namespace asio = boost::asio;
 
int main(int argc, char * argv[])
{
	// pole const buffrou
	array<asio::const_buffer, 2> const_bufs = {asio::buffer("Hello"), asio::buffer("Patrick")};
	cout << "const_bufs[0]: '" << asio::buffer_cast<char const *>(const_bufs[0]) << "'\n"
		<< "const_bufs[1]: '" << asio::buffer_cast<char const *>(const_bufs[1]) << "'\n";
	
	// const buffer zo stringu
	string s = "hello!";
	asio::const_buffer cbuf = asio::buffer(s);
	cout << "cbuf: '" << asio::buffer_cast<char const *>(cbuf) << "'\n";

	// const buffer z retazca
	char const * msg = "TEST";
	asio::const_buffer cbuf2 = asio::const_buffer{msg, 5};

	// const buffer from literal
	asio::const_buffer cbuf3 = asio::buffer("hello");

	// mutable buffer z array
	array<char, 5> ident;
	asio::mutable_buffer mbuf = asio::buffer(ident);

	// modifikovatelny buffer zo pola
//	asio::mutable_buffer mbuf = asio::buffer(s);
//	cout << "mbuf: '" << asio::buffer_cast<char const *>(mbuf) << "\n";

	cout << "done.\n";
	return 0;
}
