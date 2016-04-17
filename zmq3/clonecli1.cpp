// clone client model one (see figure 58 from zguide)
#include <czmq.h>
#include "kvsimple.h"

int main(int argc, char * argv[])
{
	zctx_t * ctx = zctx_new();
	void * updates = zsocket_new(ctx, ZMQ_SUB);
	zsocket_set_subscribe(updates, "");
	zsocket_connect(updates, "tcp://localhost:5556");

	zhash_t * kvmap = zhash_new();
	int64_t sequence = 0;

	while (!zctx_interrupted)
	{
		kvmsg_t * kvmsg = kvmsg_recv(updates);
		if (!kvmsg)
			break;
		kvmsg_store(&kvmsg, kvmap);
		sequence++;
	}

	printf(" Interrupted\n%d messages in\n", (int)sequence);
	zhash_destroy(&kvmap);
	zctx_destroy(&ctx);

	return 0;
}
