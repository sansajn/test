// clone server model two (see figure 59 from zguide)
#include <czmq.h>
#include "kvsimple.h"

static int s_send_single(char const * key, void * data, void * args);
static void state_manager(void * args, zctx_t * ctx, void * pipe);

int main(int argc, char * argv[])
{
	zctx_t * ctx = zctx_new();
	void * publisher = zsocket_new(ctx, ZMQ_PUB);
	zsocket_bind(publisher, "tcp://*:5557");

	int64_t sequence = 0;
	srandom((unsigned)time(NULL));

	// start state manager and wait for synchronization signal
	void * updates = zthread_fork(ctx, state_manager, NULL);
	free(zstr_recv(updates));

	while (!zctx_interrupted)
	{
		// distribute as key-value message
		kvmsg_t * kvmsg = kvmsg_new(++sequence);
		kvmsg_fmt_key(kvmsg, "%d", randof(10000));
		kvmsg_fmt_body(kvmsg, "%d", randof(1000000));
		kvmsg_send(kvmsg, publisher);
		kvmsg_send(kvmsg, updates);
		kvmsg_destroy(&kvmsg);
	}

	printf(" Interrupted\n%d messages out\n", (int)sequence);
	zctx_destroy(&ctx);

	return 0;
}

typedef struct {
	void * socket;  // ROUTER socket to send to
	zframe_t * identity;  // identity of peer who requested state
} kvroute_t;

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

// the state manager task maintains the state and handles requests from clients for snapshots
void state_manager(void * args, zctx_t * ctx, void * pipe)
{
	zhash_t * kvmap = zhash_new();

	zstr_send(pipe, "READY");
	void * snapshot = zsocket_new(ctx, ZMQ_ROUTER);
	zsocket_bind(snapshot, "tcp://*:5556");

	zmq_pollitem_t items[] = {
		{pipe, 0, ZMQ_POLLIN, 0}, {snapshot, 0, ZMQ_POLLIN, 0}
	};

	int64_t sequence = 0;  // current snapshot version number
	while (!zctx_interrupted)
	{
		int rc = zmq_poll(items, 2, -1);
		if (rc == -1 && errno == ETERM)
			break;  // context has been shut down

		// apply state update from main thread
		if (items[0].revents & ZMQ_POLLIN)
		{
			kvmsg_t * kvmsg = kvmsg_recv(pipe);
			if (!kvmsg)
				break;
			sequence = kvmsg_sequence(kvmsg);
			kvmsg_store(&kvmsg, kvmap);
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
			printf("sending state snapshot=%d\n", (int)sequence);
			zframe_send(&identity, snapshot, ZFRAME_MORE);
			kvmsg_t * kvmsg = kvmsg_new(sequence);
			kvmsg_set_key(kvmsg, "KTHXBAI");
			kvmsg_set_body(kvmsg, (byte *)"", 0);
			kvmsg_send(kvmsg, snapshot);
			kvmsg_destroy(&kvmsg);
		}
	}
	zhash_destroy(&kvmap);
}
