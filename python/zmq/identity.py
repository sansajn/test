# demonstrate identities as used by the request-reply pattern

import zmq, zhelpers

context = zmq.Context()

sink = context.socket(zmq.ROUTER)
sink.bind('inproc://example')

# first allow 0MQ to set the identity
anonymous = context.socket(zmq.DEALER)
anonymous.connect('inproc://example')
anonymous.send(b'ROUTER uses a generated 5 byte identity')
zhelpers.dump(sink)

# then set the identity ourselves
identified = context.socket(zmq.DEALER)
identified.setsockopt(zmq.IDENTITY, b'PEER2')
identified.connect('inproc://example')
identified.send(b"ROUTER socket uses REQ's socket identity")
zhelpers.dump(sink)
