// http://www.net-snmp.org/wiki/index.php/TUT:Simple_Application
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

char const * our_v3_passphrase = "The Net-SNMP Demo Password";

int main(int argc, char * argv[])
{	
	init_snmp("snmpapp");

	//{ initialize a 'session' that defines who we're going to talk to
	snmp_session session;
	snmp_sess_init(&session);
	session.peername = strdup("test.net-snmp.org");
	session.version = SNMP_VERSION_3;

	// user name
	session.securityName = strdup("MD5User");
	session.securityNameLen = strlen(session.securityName);

	session.securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;

	// set the authentication method to MD5
	session.securityAuthProto = usmHMACMD5AuthProtocol;
	session.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
	session.securityAuthKeyLen = USM_AUTH_KU_LEN;

	// set the authentication key to a MD5 hashed version
	if (
		generate_Ku(
			session.securityAuthProto,
			session.securityAuthProtoLen,
			(u_char *)our_v3_passphrase,
			strlen(our_v3_passphrase),
			session.securityAuthKey,
			&session.securityAuthKeyLen
		) != SNMPERR_SUCCESS)
	{
		snmp_perror(argv[0]);
		snmp_log(LOG_ERR, "Error generating Ku from authentication pass phrase.\n");
		exit(1);
	}	
	//}

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
	snmp_pdu * pdu = snmp_pdu_create(SNMP_MSG_GET);
	oid anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;
	if (!snmp_parse_oid(".1.3.6.1.2.1.1.1.0", anOID, &anOID_len))
	{
		snmp_perror(".1.3.6.1.2.1.1.1.0");
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
