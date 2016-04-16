# show how to handle ctrl-c

import signal, time, zmq

interrupted = False

def signal_handler(signum, frame):
	print('W: custom interrupt handler called.')

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind('tcp://*:5558')

# SIGINT will normally raise a KeyboardInterrupt, just line any other Python called
try:
	socket.recv()
except KeyboardInterrupt:
	print('W: interrupt received, proceeding...')

# or you can use custom handler
# in which case recv will fail with EINTR
signal.signal(signal.SIGINT, signal_handler)
try:
	message = socket.recv()
except zmq.ZMQError as e:
	print('W: recv failed with: %s' % e)
