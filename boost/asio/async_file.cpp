// async file sample implementation
#include <string>
#include <filesystem>
#include <iostream>
#include <boost/filesystem/string_file.hpp>
#include <boost/asio/io_context.hpp>

using std::string;
using std::cout, std::endl;
using std::filesystem::path;
namespace asio = boost::asio;

class afile
{
public:
	afile(asio::io_context & io, path const & p)
		: _p{p}, _io{io}
	{}

	template <typename Handler>
	void async_read(Handler && h) {
		_io.post([this, h]{
			string content;
			boost::filesystem::load_string_file(boost::filesystem::path{_p.c_str()}, content);
			h(content);
		});
	}

private:
	path _p;
	asio::io_context & _io;
};

int main(int argc, char * argv[])
{
	asio::io_context io;

	afile f{io, "async_file.cpp"};
	f.async_read([](string const & content){
		cout << "file is " << size(content) << " bytes long" << endl;
	});

	io.run();

	cout << "done!\n";
	return 0;
}
