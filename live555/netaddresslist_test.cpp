#include <iostream>
#include <NetAddress.hh>

using std::cout;
using std::cerr;


int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cerr << "specify interface-name-or-address argument missing" << std::endl;
		return 1;
	}
	
	NetAddressList addresses{argv[1]};
	if (addresses.numAddresses() == 0)
	{
		cerr << "failed to find network address for \"" << argv[1] << "\"" << std::endl;
		return 2;
	}
	else
		cout << addresses.firstAddress()->data() << std::endl;
	
	cout << "done!\n";
	
	return 0;
}
