// HTTP client with callback API sample from the project [cpp-httplib](https://github.com/yhirose/cpp-httplib) page.
#include <string_view>
#include <iostream>
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

using std::string_view, std::empty, std::cout;

int main(int argc, char * argv[]) {
	// HTTP
	httplib::Client cli("http://localhost:8080");

	cout << "<< GET(http://localhost:8080/hi)" << std::endl;

	// NOTE: Get(path, content_receiver) call is still synchronized ...
	cli.Get("/hi", [](char const * data, size_t data_length){
		cout << ">> " << string_view{data, data_length} << std::endl;
		return true;
	});

	cout << "done!\n";

	return 0;
}
