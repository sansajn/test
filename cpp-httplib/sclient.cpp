// HTTPS client sample from the project [cpp-httplib](https://github.com/yhirose/cpp-httplib) page.
#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

using std::cout, std::string;
namespace po = boost::program_options;

constexpr char const * default_ca_cert_file = "myCA.pem";


int main(int argc, char * argv[]) {
	string arg_cacert = default_ca_cert_file;

	po::options_description desc{"Options"};
	desc.add_options()
		("help", "produce this help message")		
		("cacert", po::value<string>(&arg_cacert)->default_value(default_ca_cert_file), "CA certificate to verify server is secure");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << "Usage: sclient [OPTION]...\n"
			<< desc << "\n";
		return 1;
	}

	cout << "certificate: " << arg_cacert << "\n";

	// HTTPS
	httplib::Client cli("https://localhost:8080");
	cli.enable_server_certificate_verification(true);
	cli.set_ca_cert_path(arg_cacert.c_str());
	if (auto res = cli.Get("/hi")) {
		assert(res->status == 200);
		cout << res->body << "\n";
	}
	else
		cout << "GET request failed, what: " << to_string(res.error()) << "\n";

	cout << "done!\n";

	return 0;
}
