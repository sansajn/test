// Secure HTTPS server sample from the project [cpp-httplib](https://github.com/yhirose/cpp-httplib) page.
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

int main(int argc, char * argv[]) {
	// HTTP
	httplib::SSLServer svr{"server.crt", "server.key"};

	svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
		res.set_content("Hello Secure World!", "text/plain");
	});

	svr.listen("0.0.0.0", 8080);  // blocking

	// TODO: no error, warning nothing in case certificate can't be read

	return 0;
}
