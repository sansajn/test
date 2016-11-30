#include <fstream>

using namespace::std;

int main(int argc, char * argv[])
{
	int dummy = 101;
	
	ofstream fout{"out.txt", ios::binary};
	fout << dummy;  // iba textovy vystup, bez ohladu na ios::binary
	fout.close();
	
	fout.open("out_bin.txt");
	fout.write((char const *)&dummy, sizeof(int));  // binarny vystup
	fout.close();
	
	return 0;
}
