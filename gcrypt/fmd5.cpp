// count md5 file hash
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <gcrypt.h>

using std::string;
using std::cout;
using std::cerr;
using std::ostringstream;
using std::ifstream;
namespace fs = boost::filesystem;


string to_hex(unsigned char * s, size_t len)
{
	ostringstream hex_out;
	for (size_t i = 0; i < len; ++i)
		hex_out << std::hex << std::setw(2) << std::setfill('0') << (int)s[i];

	return hex_out.str();
}

string md5sum(fs::path const & in)
{
	ifstream fin{in.string().c_str()};
	if (!fin.is_open())
		return string{};

	gcry_md_hd_t hd;
	gcry_error_t err = gcry_md_open(&hd, GCRY_MD_MD5, 0);
	if (err)
	{
		cerr << "LibGCrypt error " << gcry_strsource(err) << "/" << gcry_strerror(err) << std::endl;
		return string{};
	}

	unsigned char buf[4096];
	while (fin)
	{
		fin.read((char *)buf, sizeof(buf));
		gcry_md_write(hd, buf, fin.gcount());
	}

	fin.close();

	string result = to_hex(gcry_md_read(hd, 0), gcry_md_get_algo_dlen(GCRY_MD_MD5));

	gcry_md_reset(hd);
	gcry_md_close(hd);

	return result;
}


int main(int argc, char * argv[])
{
	fs::path in = argc > 1 ? argv[1] : argv[0];

	cout << md5sum(in) << "\t" << in << "\n";

	cout << "done!\n";

	return 0;
}
