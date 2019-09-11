#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

using std::string;
using std::cout;
using std::cerr;

string find_ip4_address(string const & network_adapter_name);

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cerr << "error: network adapter argument missing\n";
		return 1;
	}
	
	cout << argv[1] << " -> " << find_ip4_address(argv[1]) << "\n";
	cout << "done\n";
	return 0;
}

string find_ip4_address(string const & network_adapter_name)
{
	int fd = socket(AF_INET, SOCK_DGRAM, 0);

	// we want to get an IPv4 IP address for specific (e.g. "eth1") interface
	ifreq ifr;
	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, network_adapter_name.c_str(), IFNAMSIZ-1);

	string ip_addr;
	if (ioctl(fd, SIOCGIFADDR, &ifr) != -1)
		ip_addr.append(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	else
		ip_addr = "0.0.0.0";

	close(fd);

	return ip_addr;
}
