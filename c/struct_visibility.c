#include <time.h>

struct person {
	char name[256];
	struct stardate {  // stardate is globally visible there is not any person scope there
		struct tm date;
		struct timespec precision;
	} bdate;
};

int main() {
	struct stardate sd = {0};
	return 0;
}