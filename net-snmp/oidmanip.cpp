#include <iostream>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

using std::cout;

int main(int argc, char * argv[])
{
	init_snmp("test");

	// konverzia textovej formy OID na ciselnu
	char oidstr[] = "1.3.6.1.4.1.30036.1.1.5.1";
	oid oidbuf[32];
	size_t oidlen = sizeof(oidbuf) / sizeof(oid);
	read_objid(oidstr, oidbuf, &oidlen);

	cout << "oidlen: " << oidlen << "\n";
	for (int i = 0; i < oidlen; ++i)
		cout << oidbuf[i] << "\n";

	// porovnavanie oid
	oid oid1[] = {1,2,3};
	oid oid2[] = {1,2,3,1};
	oid oid3[] = {1,2,4};

	if (!netsnmp_oid_equals(oid1, 3, oid2, 4))
		cout << "oid1 != oid2\n";
	else
		cout << "wrong\n";

	if (netsnmp_oid_equals(oid1, 3, oid1, 3))
		cout << "oid1 == oid1\n";
	else
		cout << "wrong\n";

	if (snmp_oid_compare(oid1, 3, oid2, 4) == -1)
		cout << "oid1 < oid2\n";
	else
		cout << "wrong\n";

	if (snmp_oid_compare(oid2, 4, oid1, 3) == 1)
		cout << "oid2 > oid1\n";
	else
		cout << "wrong\n";

	if (netsnmp_oid_is_subtree(oid1, 3, oid2, 4) == 0)
		cout << "oid2 is oid1 subtree\n";
	else
		cout << "wrong\n";

	if (netsnmp_oid_is_subtree(oid2, 4, oid1, 3) != 0)
		cout << "oid1 is not oid2 subtree\n";
	else
		cout << "wrong\n";

	if (netsnmp_oid_is_subtree(oid1, 3, oid3, 3) != 0)
		cout << "oid3 is not oid1 subtree\n";
	else
		cout << "wrong\n";


	snmp_shutdown("test");
	return 0;
}
