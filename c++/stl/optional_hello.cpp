// basic optional usage
#include <optional>
#include <cassert>
using std::optional;

int main(int argc, char * argv[]) {
	optional<int> o;
	
	assert(!o.has_value());
	assert(!o && "doesn't have a value yet");
	
	int val = o.value_or(10);
	assert(val == 10);

	o = 42;
	assert(o.has_value());
	assert(o.value() == 42);
	assert(o.value_or(10) == 42);
	
	return 0;
}
