// exposes some custom defined wariable to outside world sample
// $ snmpwalk -v 2c -c public 127.0.0.1:4700 SNMPv2-SMI::enterprises

#include <stdlib.h>
#include <signal.h>

#include <agent_pp/agent++.h>
#include <agent_pp/snmp_group.h>
#include <agent_pp/system_group.h>
#include <agent_pp/snmp_target_mib.h>
#include <agent_pp/snmp_notification_mib.h>
#include <agent_pp/notification_originator.h>
#include <agent_pp/mib_complex_entry.h>

#include <snmp_pp/oid_def.h>
#include <snmp_pp/log.h>

using namespace Snmp_pp;
using namespace Agentpp;

// globals:

static const char* loggerModuleName = "agent++.static_table";

unsigned short port;
Mib* mib;
RequestList* reqList;
bool run = TRUE;

static void sig(int signo);
static void init_signals();

#define oidCustomGroup "1.3.6.1.4.1.4976.1.1.1.1.1"
#define oidCustomHello "1.3.6.1.4.1.4976.1.1.1.1.2.1"

class customGroup : public MibGroup
{
public:
	customGroup() : MibGroup{oidCustomGroup, "customGroup"}
	{
		_hello = new OctetStr{"Hello Lisbon!"};
		_content_id = 0;
		add(new SnmpDisplayString{oidCustomHello, READWRITE, _hello});
	}

	void update_value()
	{
		if (_content_id == 0)
			*_hello = "Hello Lisbon!";
		else
			*_hello = "Hello Jane!";

		_content_id = (_content_id + 1) % 2;
	}

private:
	OctetStr * _hello;
	unsigned _content_id;
};

customGroup * __custom = nullptr;

void init(Mib& mib)
{
	mib.add(new sysGroup("AGENT++v3.4 Static Table Sample Agent - Use 'MD5' as SNMPv3 user and 'MD5UserAuthPassword' as authentication",
				  "1.3.6.1.4.1.4976", 10));

	if (!__custom)
		__custom = new customGroup{};
	mib.add(__custom);
}

int main(int argc, char * argv[])
{
	if (argc>1)
		port = atoi(argv[1]);
	else
		port = 4700;

#ifndef _NO_LOGGING
	DefaultLog::log()->set_filter(ERROR_LOG, 5);
	DefaultLog::log()->set_filter(WARNING_LOG, 5);
	DefaultLog::log()->set_filter(EVENT_LOG, 5);
	DefaultLog::log()->set_filter(INFO_LOG, 5);
	DefaultLog::log()->set_filter(DEBUG_LOG, 6);
#endif

	int status;
	Snmp::socket_startup();  // Initialize socket subsystem
	Snmpx snmp(status, port);

	if (status == SNMP_CLASS_SUCCESS) {
		LOG_BEGIN(loggerModuleName, EVENT_LOG | 1);
		LOG("main: SNMP listen port");
		LOG(port);
		LOG_END;
	}
	else {
		LOG_BEGIN(loggerModuleName, ERROR_LOG | 0);
		LOG("main: SNMP port init failed");
		LOG(status);
		LOG_END;
		exit(1);
	}
	mib = new Mib();

	reqList = new RequestList();

	// register requestList for outgoing requests
	mib->set_request_list(reqList);

	init_signals();

	// add supported objects
	init(*mib);
	// load persitent objects from disk
	mib->init();

	reqList->set_snmp(&snmp);

	Request* req;
	while (run) {

		req = reqList->receive(2);

		if (req) {
			 mib->process_request(req);
		}
		else {
			 mib->cleanup();
			 __custom->update_value();
			 std::cout << "value updated" << std::endl;
		}
	}
	delete mib;
	Snmp::socket_cleanup();  // Shut down socket subsystem
	return 0;
}

void sig(int signo)
{
	if ((signo == SIGTERM) || (signo == SIGINT) ||
		 (signo == SIGSEGV)) {

		printf ("\n");

		switch (signo) {
		case SIGSEGV: {
			printf ("Segmentation fault, aborting.\n");
			exit(1);
		}
		case SIGTERM:
		case SIGINT: {
				  if (run) {
				run = FALSE;
				printf ("User abort\n");
			}
		}
		}
	}
}

void init_signals()
{
	signal(SIGTERM, sig);
	signal(SIGINT, sig);
	signal(SIGSEGV, sig);
}

