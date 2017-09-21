// genericka implmentacia add_variable
#include <string>
#include <cassert>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

using std::string;

namespace snmp {

template <typename T>
struct variable_traits
{
	static unsigned char const type;
};

// bool
template <>
struct variable_traits<bool>
{
	static unsigned char const type = ASN_BOOLEAN;
};

// int specialization
template <>
struct variable_traits<int>
{
	static unsigned char const type = ASN_INTEGER;
};

// string
template <>
struct variable_traits<string>
{
	static unsigned char const type = ASN_OCTET_STR;
};



template <typename T>
void add_variable(snmp_pdu & response, string const & obj, T const & value)
{
	oid objoid[32];
	size_t len = 32;
	snmp_parse_oid(obj.c_str(), objoid, &len);
	snmp_pdu_add_variable(&response, objoid, len, variable_traits<T>::type,	&value, sizeof(T));
}

}  // snmp


int main(int argc, char * argv[])
{
	init_snmp("test");

	snmp_pdu response;
	string obj = "1.1.1.1.1.1";

	int n = 101;
	snmp::add_variable(response, obj, n);

	bool b = false;
	snmp::add_variable(response, obj, b);

	string s = "hello";
	snmp::add_variable(response, obj, s);

	return 0;
}
