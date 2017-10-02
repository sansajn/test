#include <string>
#include <iostream>
#include <cassert>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

using std::string;
using std::cout;

namespace snmp {

class sync_client  //!< SNMP 2c blocking client for debug purposes
{
public:
	sync_client();
	~sync_client();
	string get_request(string const & obj);  // GET

private:
	snmp_session * _sess;
};

sync_client::sync_client()
{
	snmp_session ss;
	snmp_sess_init(&ss);
	ss.peername = strdup("127.0.0.1:4700");  // TODO: replace hardcoded address
	ss.version = SNMP_VERSION_2c;
	string comunnity = "public";
	ss.community = (u_char *)comunnity.data();
	ss.community_len = comunnity.size();

	_sess = snmp_open(&ss);
	assert(_sess);
}

sync_client::~sync_client()
{
	snmp_close(_sess);
}

string sync_client::get_request(string const & obj)
{
	// vreate request
	snmp_pdu * pdu = snmp_pdu_create(SNMP_MSG_GET);
	oid buf[MAX_OID_LEN];
	size_t len = MAX_OID_LEN;
	oid * returned_oid_rep = snmp_parse_oid(obj.c_str(), buf, &len);
	assert(returned_oid_rep);
	snmp_add_null_var(pdu, buf, len);

	// send request
	snmp_pdu * response = nullptr;
	int status = snmp_synch_response(_sess, pdu, &response);
	assert(status == STAT_SUCCESS);
	assert(response->errstat == SNMP_ERR_NOERROR);

	// obtain result
	string result;
	for (variable_list * vars = response->variables; vars; vars = vars->next_variable)
	{
		if (vars->type == ASN_OCTET_STR)
			result.assign((char *)vars->val.string, vars->val_len);
		else
			assert(0 && "unsupported result type");
	}

	snmp_free_pdu(response);

	return result;
}

}  // snmp

char const * PM_LABEL_NAME = "iso.3.6.1.4.1.30036.1.1.5.1.9.1.3.1.0";


int main(int argc, char * argv[])
{
	snmp::sync_client c;
	string response = c.get_request(PM_LABEL_NAME);
	cout << "response: " << response << std::endl;
	return 0;
}
