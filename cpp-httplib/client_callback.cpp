// HTTP client with callback API sample from the project [cpp-httplib](https://github.com/yhirose/cpp-httplib) page.
#include <string_view>
#include <iostream>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

using std::string_view, std::empty, std::cout;

int main(int argc, char * argv[]) {
	// HTTP
	httplib::Client cli("http://localhost:8080");
	auto res = cli.Get("/hi", [](char const * data, size_t data_length){  // calls Get(path, content_receiver)
		cout << ">> " << string_view{data, data_length} << std::endl;
		return true;
	});

	assert(empty(res->body));  // NOTE: res->body is empty in case of \c content_receiver Get function

	assert(res->status == 200);
	cout << "redult-status:" << res->status << "\n"
		<< "result-body:" << res->body << "\n"  // this is expected to be empty
		<< "done!\n";

	return 0;
}
