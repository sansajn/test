#include <cmath>
#include <cstring>
#include <cassert>
#include <iostream>
using std::cout;

/*! Skonvertuje gps koordinát reprezentovaný reťazcom na celočíselný 
koordinát v tvare XXXYYYYYYY kde XXX je celá žasť koordinátu a YYYYYYY je 
jeho desatiná časť. */
int to_signed_coordinate(char const * float_coordinate);

int main(int argc, char * argv[])
{
	int c1 = to_signed_coordinate("12.3456");
	assert(c1 == 123456000 && "nespravny vysledok");

	int c2 = to_signed_coordinate("110.9087671");
	assert(c2 == 1109087671 && "nespravny vysledok");
	
	int c3 = to_signed_coordinate("0.4087671");
	assert(c3 == 4087671 && "nespravny vysledok");

	int c4 = to_signed_coordinate("0.40");
	assert(c4 == 4000000 && "nespravny vysledok");

	int c5 = to_signed_coordinate("-12.765");
	assert(c5 == -127650000 && "nespravny vysledok");

	int c6 = to_signed_coordinate("-12");
	assert(c6 == -120000000 && "nespravny vysledok");

	return 0;
}

int to_signed_coordinate(char const * float_coordinate)
{
	char const * beg = float_coordinate;
	if (*beg == '-' || *beg == '+')
		beg += 1;

	char const * end = float_coordinate + strlen(float_coordinate);

	char const * dot_it = strchr(float_coordinate, '.');
	if (!dot_it)
		dot_it = end;

	assert((dot_it-beg) < 4
		&& "konvertovane cislo je v nespravnom formate");

	// cela časť
	int a = 0;
	int exp = 7;
	char const * dig = dot_it-1;
	while (dig >= beg)
		a += (*(dig--) - '0') * pow(10, exp++);

	// desatina časť
	int b = 0;
	exp = 6;
	dig = dot_it+1;
	while (dig < end)
		b += (*(dig++) - '0') * pow(10, exp--);

	int val = (float_coordinate[0] == '-' ? -1 : 1) * (a+b);

	return val;	
}

