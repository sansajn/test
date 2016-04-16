# Hello World client in Python
# Connects REQ socket to tcp://localhost:5555
# Sends "Hello" to server, expects "World" back

import zmq

context = zmq.Context()

print('connecting to hello world server ...')
socket = context.socket(zmq.REQ)
socket.connect('tcp://localhost:5555')

# do 10 requests, waiting each time for a response
for req in range(10):
	print('sending request %s ...' % req)
	socket.send(b'Hello')
	# get replay
	msg = socket.recv()
	print('received reply %s [ %s ]' % (req, msg))

