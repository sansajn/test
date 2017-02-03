// $ snmptrap -v1 -c public 127.0.0.1 1.3.6.1.4.1.20408.4.1.1.2 127.0.0.1 6 432 12345 1.3.6.1.2.1.1.1.0 s 'my system'
#include <cassert>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <netdb.h>

oid objid_enterprise[] = {1, 3, 6, 1, 4, 1, 20408, 4, 1, 1, 2};
oid objid_sysuptime[] = {1, 3, 6, 1, 2, 1, 1, 3, 0};
oid objid_snmptrap[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};

int snmp_input(int operation, netsnmp_session * session, int reqid, 
	netsnmp_pdu * pdu, void * magic)
{
	return 1;
}

int main(int argc, char * argv[])
{
	init_snmp("snmpapp");

	snmp_session session;
	snmp_sess_init(&session);
	session.peername = strdup("127.0.0.1");  // AGENT
	session.version = SNMP_VERSION_1;
	session.community = (unsigned char *)strdup("public");  // -c comunity
	session.community_len = strlen("public");
	session.callback = snmp_input;
	session.callback_magic = nullptr;

	setup_engineID(nullptr, nullptr);

	// if we don't have a contextEngineID set via command line 
	// arguments, use our internal engineID as the context.
	if (session.contextEngineIDLen == 0 || !session.contextEngineID)
		session.contextEngineID = snmpv3_generate_engineID(&session.contextEngineIDLen);

	netsnmp_session * ss = snmp_add(&session,
		netsnmp_transport_open_client("snmptrap", session.peername), nullptr, nullptr);

	if (!ss)
	{
		snmp_sess_perror("snmptrap", &session);
		exit(1);
	}

	// for SNMP v1
	netsnmp_pdu * pdu = snmp_pdu_create(SNMP_MSG_TRAP);
	assert(pdu && "failed to create notiffication PDU");

	// enterpise oid
	pdu->enterprise = (oid *)malloc(sizeof(objid_enterprise));
	memcpy(pdu->enterprise, objid_enterprise, sizeof(objid_enterprise));
	pdu->enterprise_length = sizeof(objid_enterprise) / sizeof(oid);

	// host (address of this computer)
	in_addr_t * pdu_in_addr_t = (in_addr_t *)pdu->agent_addr;
	hostent * h = gethostbyname("127.0.0.1");
	assert(h);
	memcpy(pdu_in_addr_t , h->h_addr, sizeof(in_addr_t));

	// generic trap parameter
	pdu->trap_type = 6;

	// specific trap parameter
	pdu->specific_type = 432;

	// uptime
	pdu->time = 1234;

	// custom trap data
	size_t name_length = MAX_OID_LEN;
	char const * data_oid = "1.3.6.1.2.1.1.1.0"; 
	char const * data_value = "hello!";
	oid name[MAX_OID_LEN];  // key
	oid * res_oid = snmp_parse_oid(data_oid, name, &name_length);
	assert(res_oid && "parse oid failed");
	int res = snmp_add_var(pdu, name, name_length, 's', data_value);
	assert(res == 0 && "unable to add custom-data variable");

	int status = snmp_send(ss, pdu);
	assert(status != 0 && "SNMP send failed");

	snmp_close(ss);
	snmp_shutdown("snmpapp");

	return 0;
}
