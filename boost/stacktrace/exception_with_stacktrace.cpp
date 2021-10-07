// exception with stacktrace dump implementation sample
#include <stdexcept>
#include <string>
#include <sstream>
#include <boost/stacktrace.hpp>

using std::string, std::exception, std::ostringstream;

class custom_error : public exception {
public:
	custom_error(string const & what) {
		ostringstream oss;
		oss << what << "\n"
			<< "\n"
			<< "Stack trace:\n"
			<< boost::stacktrace::stacktrace{} << "\n";
		
		_err = oss.str();
	}
	
	char const * what() const noexcept override {
		return _err.c_str();
	}

private:
	string _err;
};


int main(int argc, char * argv[]) {
	throw custom_error{"the error message"};
	return 0;
}
