#include <iostream>
using std::cout;
using std::ostream;
#include <iomanip>
using std::hex;
using std::dec;
#include <cstdint>


void print_number(uint32_t n);

uint8_t mem[4] = {0x00, 0x00, 0x30, 0x39};  // 12345


void tole4() {}

//! Big to little endian conversion.
template <typename T, typename ... Args>
void tole4(T & value, Args & ... args)
{
	uint8_t * p = (uint8_t *)&value;
	value = (p[0] << 24)|(p[1] << 16)|(p[2] << 8)|p[3];
	tole4(args ...);
}


int main()
{
	uint32_t j = 0;
	uint8_t * p = (uint8_t *)&j;
	p[0] = mem[0];
	p[1] = mem[1];
	p[2] = mem[2];
	p[3] = mem[3];
	cout << hex << "j:" << j << "\n";

	uint32_t k = j;
	tole4(k);
	cout << "k:" << k << "\n";

	

	return 0;
}

void print_number(uint32_t n)
{
	uint8_t * p = (uint8_t *)&n;
	cout << hex << int(p[0]) << " " << int(p[1]) << " " << int(p[2]) 
		<< " " << int(p[3]) << dec;
}
