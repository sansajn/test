#include <stdio.h>

typedef struct {
	char const name[256];
	unsigned age;
} person;

int main() {
	person p = {
		.name = "Jon",
		.age = 42
	};
	printf("%s is %d years old\n", p.name, p.age);

	return 0;
}