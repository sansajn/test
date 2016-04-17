// clone server model one
#include <czmq.h>
#include "kvsimple.h"

int main(int argc, char * argv[])
{
	zctx_t * ctx = zctx_new();
	void * publisher = zsocket_new(ctx, ZMQ_PUB);
	zsocket_bind(publisher, "tcp://*:5556");
	zclock_sleep(200);

	zhash_t * kvmap = zhash_new();
	int64_t sequence = 0;
	srandom((unsigned)time(NULL));

	while (!zctx_interrupted)
	{
		// distribute as key-value message
		kvmsg_t * kvmsg = kvmsg_new(++sequence);
		kvmsg_fmt_key(kvmsg, "%d", randof(10000));
		kvmsg_fmt_body(kvmsg, "%d", randof(1000000));
		kvmsg_send(kvmsg, publisher);
		kvmsg_store(&kvmsg, kvmap);
	}

	printf(" Interrupted\n%d messages out\n", (int)sequence);
	zhash_destroy(&kvmap);
	zctx_destroy(&ctx);

	return 0;
}
