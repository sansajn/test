// ceil() inmplementation for C++11
#include <chrono>
#include <iostream>
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::cout;
using namespace std::chrono_literals;

// C++17 like ceil() implementation
template <typename To, typename Rep, typename Period>
constexpr To ceil(duration<Rep, Period> const & d) {
	auto t = duration_cast<To>(d);
	if (t < d)
		return t + duration_cast<To>(1);
	return t;
}

int main(int argc, char * argv[]) {
	auto a = 1200ms;
	auto b = duration_cast<seconds>(a);
	cout << "b=" << b.count() << "s\n";  // expect 1 so we lost fraction time (200ms)
	cout << "ceil<s>(1.2ms)=" << ceil<seconds>(1200ms).count() << "s\n";  // 2s
	return 0;
}
