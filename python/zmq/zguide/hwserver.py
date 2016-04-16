# hello world server
# binds REP socket to tcp://*:5555
# expects b"Hello" from client, replies with b"World"

import time, zmq

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind('tcp://*:5555')

while True:
	# wait for next request from client
	message = socket.recv()
	print('received request: %s' % message)
	
	# do some work ...
	time.sleep(1)
	
	# send reply back to client
	socket.send(b'World')
