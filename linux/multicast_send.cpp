/* mcsend.c, ASM (Any Source Multicast) sending sample
use wireshark to see UDP traffic with filter 'dst 239.255.10.10 and port 10000' */
#include <string>
#include <iostream>
#include <cstring>
#include <cassert>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::string;
using std::getline;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;


unsigned short const MIN_PORT = 1024,
	MAX_PORT = 65535;


int main(int argc, char * argv[])
{
	if (argc != 3)
	{
		cerr << "Usage: " << argv[0] << " <Multicast IP> <<ulticast Port>" << endl;
		exit(1);
	}
	
	char const * mc_addr_str = argv[1];
	unsigned short mc_port = (unsigned short)atoi(argv[2]);
	
	// port ranges
	if ((mc_port < MIN_PORT) || (mc_port > MAX_PORT))
	{
		cerr << "Invalid port number argument " << mc_port << "\n"
			<< "Valid range is between " << MIN_PORT << " and " << MAX_PORT << endl;
		exit(1);
	}
	
	// create socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		cerr << "socket() failed" << endl;
		exit(1);
	}
	
	// ttl
	unsigned char ttl = 255;
	if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&ttl, sizeof(ttl)) < 0)
	{
		cerr << "setsockopt() failed" << endl;
		exit(1);
	}
	
	// set-up address
	sockaddr_in mc_addr;
	memset(&mc_addr, 0, sizeof(mc_addr));
	mc_addr.sin_family = AF_INET;
	mc_addr.sin_addr.s_addr = inet_addr(mc_addr_str);
	mc_addr.sin_port = htons(mc_port);
	
	cout << "Begin typing (return to send, ctrl-c to quit):" << endl;
	
	string line;
	while (getline(cin, line))
	{
		// send line to multicast address
		if (sendto(sock, line.c_str(), line.size(), 0, (sockaddr *)&mc_addr, 
			sizeof(mc_addr)) != line.size())
		{
			cerr << "sendto() sent incorrect number of bytes" << endl;
			exit(1);
		}
	}
	
	close(sock);
	
	return 0;
}
