// clone server model three (see figure 60 in zguide)
#include <czmq.h>
#include "kvsimple.h"

static int s_send_single(char const * key, void * data, void * args);

typedef struct {
	void * socket;  // ROUTER socket to send to
	zframe_t * identity;  // identity of peer who requested state
} kvroute_t;


int main(int argc, char * argv[])
{
	zctx_t * ctx = zctx_new();
	void * snapshot = zsocket_new(ctx, ZMQ_ROUTER);
	zsocket_bind(snapshot, "tcp://*:5556");
	void * publisher = zsocket_new(ctx, ZMQ_PUB);
	zsocket_bind(publisher, "tcp://*:5557");
	void * collector = zsocket_new(ctx, ZMQ_PULL);
	zsocket_bind(collector, "tcp://*:5558");

	// the body of the main task collects updates from clients and publishes them
	// back out to clients.

	int64_t sequence = 0;
	zhash_t * kvmap = zhash_new();

	zmq_pollitem_t items[] = {
		{collector, 0, ZMQ_POLLIN, 0}, {snapshot, 0, ZMQ_POLLIN, 0}
	};


	while (!zctx_interrupted)
	{
		int rc = zmq_poll(items, 2, 1000 * ZMQ_POLL_MSEC);

		// apply state update from main thread
		if (items[0].revents & ZMQ_POLLIN)
		{
			kvmsg_t * kvmsg = kvmsg_recv(collector);
			if (!kvmsg)
				break;
			kvmsg_set_sequence(kvmsg, ++sequence);
			kvmsg_send(kvmsg, publisher);
			kvmsg_store(&kvmsg, kvmap);
			printf("I: publishing update %5d\n", (int)sequence);
		}

		// execute state snapshot request
		if (items[1].revents & ZMQ_POLLIN)
		{
			zframe_t * identity = zframe_recv(snapshot);
			if (!identity)
				break;

			char * request = zstr_recv(snapshot);
			if (streq(request, "ICANHAZ?"))
				free(request);  // do nothing ...
			else
			{
				printf("E: bad request, aborting\n");
				break;
			}

			// send state snapshot
			kvroute_t routing = {snapshot, identity};

			// for each entry in kvmap, send kvmsg to client
			zhash_foreach(kvmap, s_send_single, &routing);

			// now send ENDmessage with sequence number
			printf("I: sending snapshot=%d\n", (int)sequence);
			zframe_send(&identity, snapshot, ZFRAME_MORE);
			kvmsg_t * kvmsg = kvmsg_new(sequence);
			kvmsg_set_key(kvmsg, "KTHXBAI");
			kvmsg_set_body(kvmsg, (byte *)"", 0);
			kvmsg_send(kvmsg, snapshot);
			kvmsg_destroy(&kvmsg);
		}
	}

	printf(" Interrupted\n%d messages out\n", (int)sequence);
	zhash_destroy(&kvmap);
	zctx_destroy(&ctx);

	return 0;
}


// send one state snapshot key-value pair to a socket
// hash item data is our kvmsg object, ready to send
int s_send_single(char const * key, void * data, void * args)
{
	kvroute_t * route = (kvroute_t *)args;
	// send identity of recipient first
	zframe_send(&route->identity, route->socket, ZFRAME_MORE + ZFRAME_REUSE);
	kvmsg_t * kvmsg = (kvmsg_t *)data;
	kvmsg_send(kvmsg, route->socket);
	return 0;
}
