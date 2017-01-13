#include <sstream>
#include <string>
#include <iostream>
#include <b64/encode.h>
#include <b64/decode.h>

using std::cout;
using std::string;
using std::ostringstream;
using std::istringstream;

int main(int argc, char * argv[]) 
{
	string test_text = "<html><body>Hello!!!</body></html>";

	base64::encoder E;
	istringstream enc_in{test_text};
	ostringstream enc_out;
	E.encode(enc_in, enc_out);
	string encoded{enc_out.str()};

	base64::decoder D;
	istringstream dec_in{encoded};
	ostringstream dec_out;
	D.decode(dec_in, dec_out);
	string decoded{dec_out.str()};

	if (decoded == test_text)
		cout << "encoding/decoding success" << std::endl;
	else
		cout << "failed" << std::endl;

	return 0;
}
