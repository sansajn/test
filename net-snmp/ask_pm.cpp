/* http://www.net-snmp.org/wiki/index.php/TUT:Simple_Application
ziska hodnotu premennej s beziaceho snmp agenta (tzn. implementacia jednoducheho klienta) */
#include <string>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

using std::string;

int main(int argc, char * argv[])
{	
	init_snmp("test-snmp-client");

	//{ initialize a 'session' that defines who we're going to talk to
	snmp_session session;
	snmp_sess_init(&session);
	session.peername = strdup("127.0.0.1:4700");

	// SNMP 2c
	string const community = "public";
	session.version = SNMP_VERSION_2c;
	session.community = (u_char *)community.c_str();
	session.community_len = community.size();

	//{ open the session
	SOCK_STARTUP;
	snmp_session * ss = snmp_open(&session);
	if (!ss)
	{
		snmp_sess_perror("ack", &session);
		SOCK_CLEANUP;
		exit(1);
	}
	//}

	//{ request some data
	string name_oid = "iso.3.6.1.4.1.30036.1.1.5.1.9.1.3.1.0";
	snmp_pdu * pdu = snmp_pdu_create(SNMP_MSG_GET);
	oid anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;
	if (!snmp_parse_oid(name_oid.c_str(), anOID, &anOID_len))
	{
		snmp_perror(name_oid.c_str());
		SOCK_CLEANUP;
		exit(1);
	}
	
	snmp_add_null_var(pdu, anOID, anOID_len);

	// send the request out
	snmp_pdu * response;
	int status = snmp_synch_response(ss, pdu, &response);
	//}

	//{ process the request
	if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR)	
	{
		for (variable_list * vars = response->variables; vars; vars = vars->next_variable)
			print_variable(vars->name, vars->name_length, vars);

		// manipulate the information ourselves
		int count = 1;
		for (variable_list * vars = response->variables; vars; vars = vars->next_variable)
		{
			if (vars->type == ASN_OCTET_STR)
			{
				char * sp = (char *)malloc(1 + vars->val_len);
				memcpy(sp, vars->val.string, vars->val_len);
				sp[vars->val_len] = '\0';
				printf("value #%d is a string: %s\n", count++, sp);
				free(sp);
			}
			else
				printf("value #%d in NOT a string! Ack!\n", count++);
		}
	}
	else  // failure
	{
		if (status == STAT_SUCCESS)
			fprintf(stderr, "Error in packet\nReason: %s\n", 
				snmp_errstring(response->errstat));
		else if (status == STAT_TIMEOUT)
			fprintf(stderr, "Timeout: No response from %s.\n", session.peername);
		else
			snmp_sess_perror("snmpdemoapp", ss);
	}
	//}

	//{ cleanu-up
	if (response)
		snmp_free_pdu(response);
	snmp_close(ss);

	SOCK_CLEANUP;
	//}

	return 0;
}
