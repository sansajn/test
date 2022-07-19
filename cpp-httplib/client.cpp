// HTTP client sample from readme (section *Simple examples*)
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

int main(int argc, char * argv[]) {
	// HTTP
	httplib::Client cli("http://localhost:8080");
	auto res = cli.Get("/hi");
	
	std::cout << res->body << "\n"
		<< "done!\n";

	return 0;
}
