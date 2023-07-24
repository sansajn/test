#include <stdio.h>

int main() {
	enum corvid {magpie, raven, jay, corvid_num};
	char const*const bird[corvid_num] = {
		[raven] = "raven",
		[magpie] = "magpie",
		[jay] = "jay"
	};

	for (unsigned i = 0; i < corvid_num; ++i)
		printf("Corvid %u is the %s\n", i, bird[i]);
	
	return 0;
}
