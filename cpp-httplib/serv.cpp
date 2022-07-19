// HTTP server sample from the project [cpp-httplib](https://github.com/yhirose/cpp-httplib) page.
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

int main(int argc, char * argv[]) {
	// HTTP
	httplib::Server svr;

	svr.Get("/hi", [](const httplib::Request &, httplib::Response &res) {
		res.set_content("Hello World!", "text/plain");
	});

	svr.listen("0.0.0.0", 8080);  // blocking

	return 0;
}
