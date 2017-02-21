// dynamic table sample(based on dynamic_table example from agent++)
#include <stdlib.h>
#include <signal.h>

#include <agent_pp/agent++.h>
#include <agent_pp/snmp_group.h>
#include <agent_pp/system_group.h>
#include <agent_pp/snmp_target_mib.h>
#include <agent_pp/snmp_notification_mib.h>
#include <agent_pp/notification_originator.h>
#include <agent_pp/mib_complex_entry.h>
#include <agent_pp/v3_mib.h>
#include <agent_pp/vacm.h>

#include <snmp_pp/oid_def.h>
#include <snmp_pp/mp_v3.h>
#include <snmp_pp/log.h>

using namespace Snmp_pp;
using namespace Agentpp;

static const char* loggerModuleName = "agent++.dynamic_table";

// globals:

unsigned short port;
Mib* mib;
RequestList* reqList;
bool run = TRUE;

static void sig(int signo);
static void init_signals();

const index_info indDynamicTable[1] = {
  { sNMP_SYNTAX_INT, FALSE, 1, 1 }
};

class DynamicTable: public MibTable
{
public:
  DynamicTable(const Oidx& o, const index_info* ind, unsigned int sz): 
    MibTable(o, ind, sz) {
    currentRequest = 0;
    add_col(new MibLeaf("1", READWRITE, new SnmpInt32(0), VMODE_NONE));
  }

virtual void update(Request* req) {

  LOG_BEGIN(loggerModuleName, INFO_LOG | 1);
  LOG("DynamicTable: updating table");
  LOG_END;

  if (currentRequest == req->get_request_id()) return;
  currentRequest = req->get_request_id();

  start_synch();
  int r = rand();
  if (r >= RAND_MAX/2) {
	MibTableRow* row = add_row(get_next_avail_index());
	row->get_nth(0)->replace_value(new SnmpInt32(r));
  }
  else {
	if (!is_empty())
		remove_row(content.first()->get_index());
  }
  end_synch();
}

private:
  unsigned long currentRequest;

};




void init(Mib& mib)
{
        mib.add(new sysGroup("AGENT++v3.4 Dynamic Table Sample Agent - Use 'MD5' as SNMPv3 user and 'MD5UserAuthPassword' as authentication", 
			     "1.3.6.1.4.1.4976", 10)); 
 	mib.add(new snmpGroup());
	mib.add(new snmp_target_mib());
	mib.add(new snmp_notification_mib());

	DynamicTable* dt = new DynamicTable("1.3.6.1.4.1.4976.6.2.1",
					    indDynamicTable, 1);
	mib.add(dt);
}	


int main (int argc, char* argv[])
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
	// load persistent objects from disk
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
	signal (SIGTERM, sig);
	signal (SIGINT, sig);
	signal (SIGSEGV, sig);
}
