// HTTPS client sample from the project [cpp-httplib](https://github.com/yhirose/cpp-httplib) page.
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

int main(int argc, char * argv[]) {
	// HTTPS
	httplib::Client cli("https://localhost:8080");
	cli.enable_server_certificate_verification(true);
	cli.set_ca_cert_path("myCA.pem");
	auto res = cli.Get("/hi");
	

	assert(res->status == 200);

	std::cout << res->body << "\n"
		<< "done!\n";

	return 0;
}
