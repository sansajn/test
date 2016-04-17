// clone client model two (see figure 59 from zguide)
#include <czmq.h>
#include "kvsimple.h"

int main(int argc, char * argv[])
{
	zctx_t * ctx = zctx_new();
	void * snapshot = zsocket_new(ctx, ZMQ_DEALER);
	zsocket_connect(snapshot, "tcp://localhost:5556");
	void * subscriber = zsocket_new(ctx, ZMQ_SUB);
	zsocket_set_subscribe(subscriber, "");
	zsocket_connect(subscriber, "tcp://localhost:5557");

	zhash_t * kvmap = zhash_new();

	// get state snapshot
	int64_t sequence = 0;
	zstr_send(snapshot, "ICANHAZ?");
	while (1)
	{
		kvmsg_t * kvmsg = kvmsg_recv(snapshot);
		if (!kvmsg)
			break;
		if (streq(kvmsg_key(kvmsg), "KTHXBAI"))
		{
			sequence = kvmsg_sequence(kvmsg);
			printf("received snapshot=%d\n", (int)sequence);
			kvmsg_destroy(&kvmsg);  // preco sa destroy roby tu ?
			break;
		}
		kvmsg_store(&kvmsg, kvmap);
	}

	// now apply pending updates, discard out-of-sequence messages
	while (!zctx_interrupted)
	{
		kvmsg_t * kvmsg = kvmsg_recv(subscriber);
		if (!kvmsg)
			break;
		if (kvmsg_sequence(kvmsg) > sequence)
		{
			sequence = kvmsg_sequence(kvmsg);
			kvmsg_store(&kvmsg, kvmap);
		}
		else
			kvmsg_destroy(&kvmsg);  // memory leak ?
	}

	printf(" Interrupted\n%d messages in\n", (int)sequence);
	zhash_destroy(&kvmap);
	zctx_destroy(&ctx);

	return 0;
}
