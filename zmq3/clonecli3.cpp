// clone client model three (see figure 60 from zguide)
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
	void * publisher = zsocket_new(ctx, ZMQ_PUSH);
	zsocket_connect(publisher, "tcp://localhost:5558");

	zhash_t * kvmap = zhash_new();
	srandom((unsigned)time(NULL));

	// we first request a state snapshot
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
			printf("I: received snapshot=%d\n", (int)sequence);
			kvmsg_destroy(&kvmsg);  // preco sa destroy roby tu ?
			break;
		}
		kvmsg_store(&kvmsg, kvmap);
	}

	// now we wait for updates from the server and evry so often, we send a random
	// key-value update to the server
	int64_t alarm = zclock_time() + 1000;
	while (!zctx_interrupted)
	{
		zmq_pollitem_t items[] = {{subscriber, 0, ZMQ_POLLIN, 0}};
		int tickless = (int)((alarm - zclock_time()));
		if (tickless < 0)
			tickless = 0;
		int rc = zmq_poll(items, 1, tickless * ZMQ_POLL_MSEC);
		if (rc == -1)
			break;

		if (items[0].revents & ZMQ_POLLIN)
		{
			kvmsg_t * kvmsg = kvmsg_recv(subscriber);
			if (!kvmsg)
				break;
			// discard out0of-sequence kvmsgs, incl. heartbeats
			if (kvmsg_sequence(kvmsg) > sequence)
			{
				sequence = kvmsg_sequence(kvmsg);
				kvmsg_store(&kvmsg, kvmap);
				printf("I: received update=%d\n", (int)sequence);
			}
			else
				kvmsg_destroy(&kvmsg);  // memory leak ?
		}

		// if we timed out, generate a random kvmsg
		if (zclock_time() > alarm)
		{
			kvmsg_t * kvmsg = kvmsg_new(0);
			kvmsg_fmt_key(kvmsg, "%d", randof(10000));
			kvmsg_fmt_body(kvmsg, "%d", randof(1000000));
			kvmsg_send(kvmsg, publisher);
			kvmsg_destroy(&kvmsg);
			alarm = zclock_time() + 1000;
		}
	}

	printf(" Interrupted\n%d messages in\n", (int)sequence);
	zhash_destroy(&kvmap);
	zctx_destroy(&ctx);

	return 0;
}
