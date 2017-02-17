// simplified static_table.cpp sample
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

void init(Mib& mib)
{
	mib.add(new sysGroup("AGENT++v3.4 Static Table Sample Agent - Use 'MD5' as SNMPv3 user and 'MD5UserAuthPassword' as authentication",
				  "1.3.6.1.4.1.4976", 10));
	mib.add(new snmpGroup());
	mib.add(new snmp_target_mib());
	mib.add(new snmp_notification_mib());

	// An example usage of the MibStaticTable for a read-only table
	// implementation:
	MibStaticTable* st = new MibStaticTable("1.3.6.1.4.1.4976.6.1.1");
	st->add(MibStaticEntry("2.224",
					 SnmpInt32(1)));
	// an oldstyle entry with fully specified OID (deprecated)
	st->add(MibStaticEntry("1.3.6.1.4.1.4976.6.1.1.3.224",
					 OctetStr("An oldstyle table text")));
	st->add(MibStaticEntry("2.71",
					 SnmpInt32(2)));
	st->add(MibStaticEntry("3.71",
					 OctetStr("A table text")));

	// An example usage of the MibStaticTable for a read-only scalar
	// group:
	MibStaticTable* ssg = new MibStaticTable("1.3.6.1.4.1.4976.6.1.2");
	ssg->add(MibStaticEntry("1.0",
				SnmpInt32(1)));
	ssg->add(MibStaticEntry("2.0",
				OctetStr("A scalar text object")));
	ssg->add(MibStaticEntry("3.0",
				Counter32(123456)));
	ssg->add(MibStaticEntry("4.1.0",
				OctetStr("A scalar text in a sub group")));
	ssg->add(MibStaticEntry("4.2.0",
				Gauge32(65535)));
	mib.add(ssg);
	mib.add(st);
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
