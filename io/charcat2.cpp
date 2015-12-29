// test iteratorou vyrovnavacej pameti (zadany vstup zapise na vystup)
#include <iostream>
#include <iterator>

using namespace std;

int main(int rgc, char * argv[])
{
	istreambuf_iterator<char> inpos{cin}, endpos;
	
	ostreambuf_iterator<char> outpos{cout};
	
	while (inpos != endpos)
	{
		*outpos = *inpos;
		++inpos;
		++outpos;
	}
	
	return 0;
}
