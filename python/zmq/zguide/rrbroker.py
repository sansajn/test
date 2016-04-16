# Simple request-reply broker

import zmq

# prepare our context and sockets
context = zmq.Context()
frontend = context.socket(zmq.ROUTER)
backend = context.socket(zmq.DEALER)
frontend.bind('tcp://*:5559')
backend.bind('tcp://*:5560')

# initialize poll set
poller = zmq.Poller()
poller.register(frontend, zmq.POLLIN)
poller.register(backend, zmq.POLLIN)

# switch messages between sockets
while True:
	socks = dict(poller.poll())
	
	if socks.get(frontend) == zmq.POLLIN:
		message = frontend.recv_multipart()
		backend.send_multipart(message)
	
	if socks.get(backend) == zmq.POLLIN:
		message = backend.recv_multipart()
		frontend.send_multipart(message)
